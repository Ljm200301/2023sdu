#include <iostream>
#include <string>
#include <vector>
#include "openssl/x509.h"
#include <cassert>
#include <iomanip> 
#include<windows.h>
//利用openssl 1.1实现
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")

//生成ECDSA密钥对
void GenerateKeys(EC_KEY*& eckey)
{
    bool success(true);
    eckey = EC_KEY_new();
    if (!eckey)
    {
        std::cout << "Failed to create new EC Key." << std::endl;
        success = false;
    }
    EC_GROUP* ecgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
    if (!ecgroup)
    {
        std::cout << "Failed to create new EC Group." << std::endl;
        success = false;
    }
    if (EC_KEY_set_group(eckey, ecgroup) == 0)
    {
        std::cout << "Failed to set group for EC Key." << std::endl;
        success = false;
    }
    if (EC_KEY_generate_key(eckey) == 0)
    {
        std::cout << "Failed to generate EC Key." << std::endl;
        success = false;
    }
    EC_GROUP_free(ecgroup);
    if (!success)
    {
        EC_KEY_free(eckey);
        std::cout << "Failed to create EC key pair." << std::endl;
        return;
    }
}

// 使用 ECDSA 私钥来签名
bool ecdsaSign(const std::string& message, EC_KEY* private_key, std::string& signature)
{
    if (!private_key)
    {
        std::cout << "Private key is null" << std::endl;
        return false;
    }

    // 对消息进行哈希
    const unsigned char* hash = reinterpret_cast<const unsigned char*>(message.c_str());
    unsigned int hash_len = static_cast<unsigned int>(message.size());
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(hash, hash_len, digest);

    // 使用 ECDSA 签名
    ECDSA_SIG* sig = ECDSA_do_sign(digest, SHA256_DIGEST_LENGTH, private_key);
    if (!sig)
    {
        std::cout << "Failed to generate ECDSA signature" << std::endl;
        return false;
    }

    // 将签名编码为 DER 字节序列
    int der_len = i2d_ECDSA_SIG(sig, nullptr);
    if (der_len <= 0)
    {
        std::cout << "Failed to encode ECDSA signature" << std::endl;
        ECDSA_SIG_free(sig);
        return false;
    }
    std::vector<unsigned char> der(der_len);
    unsigned char* der_pos = &der[0];
    if (i2d_ECDSA_SIG(sig, &der_pos) != der_len)
    {
        std::cout << "Failed to encode ECDSA signature" << std::endl;
        ECDSA_SIG_free(sig);
        return false;
    }

    // 将 DER 字节序列转换为字符串
    signature.assign(reinterpret_cast<const char*>(der.data()), der.size());

    ECDSA_SIG_free(sig);
    return true;
}

bool ecdsaVerify(const std::string& message, const std::string& signature, EC_KEY* public_key)
{
    if (!public_key)
    {
        std::cout << "Public key is null" << std::endl;
        return false;
    }

    // 对消息进行哈希，注意这里使用了指针和长度参数来避免重复计算哈希值
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(message.c_str()), message.size(), digest);

    // 将签名字符串解码为 DER 字节序列，注意这里使用了指针和长度参数来避免拷贝签名字符串的内容
    const unsigned char* der_data = reinterpret_cast<const unsigned char*>(signature.c_str());
    ECDSA_SIG* sig = d2i_ECDSA_SIG(nullptr, &der_data, signature.size());
    if (!sig)
    {
        std::cout << "Failed to decode ECDSA signature" << std::endl;
        return false;
    }

    // 验证签名
    int result = ECDSA_do_verify(digest, SHA256_DIGEST_LENGTH, sig, public_key);
    ECDSA_SIG_free(sig);
    if (result == 1)
    {
        return true;
    }
    else if (result == 0)
    {
        std::cout << "Failed to verify ECDSA signature: signature is not valid" << std::endl;
    }
    else
    {
        std::cout << "Failed to verify ECDSA signature: an error occurred" << std::endl;
    }
    return false;
}
int main()
{
    LARGE_INTEGER t1, t2, t3, t4, tc;
    EC_KEY* eckey = nullptr;

    GenerateKeys(eckey);
    if (eckey == nullptr)
    {
        std::cout << "generate key fail" << std::endl;
        return -1;
    }
    std::string message = "sdu";
    std::string signature;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    if (ecdsaSign(message, eckey, signature) == false)
    {
        std::cout << "sign fail" << std::endl;
        return -1;
    }

    std::vector<unsigned char> vchSig;
    for (int i = 0; i < signature.size(); ++i)
    {
        vchSig.push_back(signature[i]);
    }

    if (vchSig.empty())
    {
        std::cout << "invalid signature" << std::endl;
        return -1;
    }

    unsigned char* norm_der = NULL;
    ECDSA_SIG* norm_sig = ECDSA_SIG_new();
    const unsigned char* sigptr = &vchSig[0];
    assert(norm_sig);
    if (d2i_ECDSA_SIG(&norm_sig, &sigptr, vchSig.size()) == NULL)
    {
        ECDSA_SIG_free(norm_sig);
        std::cout << "invalid signature format" << std::endl;
        return -1;
    }

    const BIGNUM* r_bn = nullptr;
    const BIGNUM* s_bn = nullptr;

    ECDSA_SIG_get0(norm_sig, &r_bn, &s_bn);
    QueryPerformanceCounter(&t2);

    std::cout << "Signature: " << std::endl;
    std::cout << "r = " << BN_bn2hex(r_bn) << std::endl;
    std::cout << "s = " << BN_bn2hex(s_bn) << std::endl;
    std::cout << "Sign time：" << ((long double)(t2.QuadPart - t1.QuadPart) / (long double)tc.QuadPart) * 1000 << "ms\n";

    ECDSA_SIG_free(norm_sig);

    if (ecdsaVerify(message, signature, eckey) == false)
    {
        std::cout << "verify fail" << std::endl;
        return -1;
    }
    std::cout << "Verify success!" << std::endl;

    return 0;
}

