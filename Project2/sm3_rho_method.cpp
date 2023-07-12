#include <iostream>
#include "openssl/evp.h"
#include "openssl/rand.h"
#include <random>
#include<windows.h>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
const int COLLISION_LEN = 24;
const int COLLISION_BYTE = COLLISION_LEN >> 3;

std::vector<uint8_t> data(32);
void rho_method() {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();

    std::vector<uint8_t> out1(32);
    uint32_t out1_size = 0;

    std::vector<uint8_t> out2(32);
    std::vector<uint8_t> out3(32);
    std::vector<uint8_t> out4(32);
    uint32_t out2_size = 0;

    RAND_bytes(data.data(), 32);
    std::cout << "初始值为：";
    for (uint8_t c : data) {
        printf("%02X", c);
    }
    printf("\n");

    EVP_Digest(data.data(), 32, out1.data(), &out1_size, EVP_sm3(), NULL);
    EVP_Digest(out1.data(), 32, out2.data(), &out2_size, EVP_sm3(), NULL);

    while (1) {
        EVP_Digest(out1.data(), 32, out3.data(), &out1_size, EVP_sm3(), NULL);
        EVP_Digest(out2.data(), 32, out2.data(), &out2_size, EVP_sm3(), NULL);
        EVP_Digest(out2.data(), 32, out4.data(), &out2_size, EVP_sm3(), NULL);

        uint32_t key1 = 0;
        std::memcpy(&key1, out3.data(), COLLISION_BYTE);

        uint32_t key2 = 0;
        std::memcpy(&key2, out4.data(), COLLISION_BYTE);

        if (key1 == key2) {
            std::cout << "已经找到碰撞！\n";
            std::cout << "第一段消息为：";
            for (uint8_t c : out1) {
                printf("%02X", c);
            }
            printf("\n");

            std::cout << "第一段消息的 SM3 哈希值为：";
            for (uint8_t c : out3) {
                printf("%02X", c);
            }
            printf("\n");

            std::cout << "第二段消息为：";
            for (uint8_t c : out2) {
                printf("%02X", c);
            }
            printf("\n");

            std::cout << "第二段消息的 SM3 哈希值为：";
            for (uint8_t c : out4) {
                printf("%02X", c);
            }
            printf("\n");

            break;
        }

        out1 = out3;
        out2 = out4;
    }

    EVP_MD_CTX_free(ctx);
}

int main() {
    LARGE_INTEGER t1, t2, t3, t4, tc;

    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    rho_method();
    QueryPerformanceCounter(&t2);

    std::cout << "查找碰撞的时间为：" << (long double)(t2.QuadPart - t1.QuadPart) / (long double)tc.QuadPart << "秒\n";

    return 0;
}
