#include "sm3.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

void print_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
}

void length_extension_attack(uint8_t* hash, int len, const string& message) {
    const int block_size = 64;
    // Ϊ�������Ϣ����ռ�
    uint8_t* padded_message = new uint8_t[block_size + message.length()];
    memset(padded_message, 0, block_size + message.length());
    // ��ԭʼ��Ϣ���Ƶ��������Ϣ��
    memcpy(padded_message, message.c_str(), message.length());
    // �������ֽ�
    padded_message[message.length()] = 0x80;
    // ������Ϣ�� bit ���ȣ��������
    uint64_t message_bit_length = len * 8;
    padded_message[block_size - 8] = (message_bit_length >> 56) & 0xff;
    padded_message[block_size - 7] = (message_bit_length >> 48) & 0xff;
    padded_message[block_size - 6] = (message_bit_length >> 40) & 0xff;
    padded_message[block_size - 5] = (message_bit_length >> 32) & 0xff;
    padded_message[block_size - 4] = (message_bit_length >> 24) & 0xff;
    padded_message[block_size - 3] = (message_bit_length >> 16) & 0xff;
    padded_message[block_size - 2] = (message_bit_length >> 8) & 0xff;
    padded_message[block_size - 1] = message_bit_length & 0xff;
    // ������䳤��
    int pad_len = block_size * ceil((len + 9) / (double)block_size);
    // ������չ��Ϣ�ĳ���
    int extended_len = pad_len + message.length();
    // ����ռ�
    uint8_t* extended_message = new uint8_t[extended_len];
    memset(extended_message, 0, extended_len);
    // �������ԭʼ��Ϣ����չ��Ϣƴ��
    memcpy(extended_message, padded_message, pad_len);
    memcpy(extended_message + pad_len, message.c_str(), message.length());
    // ���� SM3 �����ģ���ԭʼ��ϣֵ��Ϊ��ʼ IV
    SM3_CTX ctx;
    ctx.msgLen = ctx.curlen = 0;
    uint8_t* data = hash;
    (void)HOST_c2l(data, ctx.state[0]);
    (void)HOST_c2l(data, ctx.state[1]);
    (void)HOST_c2l(data, ctx.state[2]);
    (void)HOST_c2l(data, ctx.state[3]);
    (void)HOST_c2l(data, ctx.state[4]);
    (void)HOST_c2l(data, ctx.state[5]);
    (void)HOST_c2l(data, ctx.state[6]);
    (void)HOST_c2l(data, ctx.state[7]);
    // ������չ��Ϣ�Ĺ�ϣֵ
    ctx.msgLen = extended_len << 3;
    SM3_process(&ctx, extended_message, message.length());
    SM3_paddingpart(&ctx, hash);
    cout << "����ԭʼ��ϣֵ�ͳ��ȼ��� SM3(M3)������ IV Ϊԭʼ��ϣֵ��" << endl;
    cout << "  - ԭʼ��ϣֵ��" << endl;
    cout << "    ";
    print_hex(hash, SM3_OUTLEN);
    cout << endl;
    cout << "  - ԭʼ��Ϣ���ȣ��ֽڣ���" << len << endl;
    cout << "  - ��չ��Ϣ��" << message << endl;
    cout << "  - �����ԭʼ��Ϣ����չ��Ϣ��" << endl;
    cout << "    ";
    print_hex(extended_message, extended_len);
    cout << endl;
    cout << "  - ��䳤�ȣ��ֽڣ���" << pad_len - len << endl;
    cout << " - ��չ��Ϣ���ȣ��ֽڣ���" << message.length() << endl;
    cout << "  - ƴ�Ӻ����Ϣ���ȣ��ֽڣ���" << extended_len << endl;
    cout << "  - ����õ��� SM3(M3) ֵ��" << endl;
    cout << "    ";
    print_hex(hash, SM3_OUTLEN);
    cout << endl;
    // �ͷ��ڴ�
    delete[] padded_message;
    delete[] extended_message;
}

int main() {
    const string message1 = "SDUliujiaming";  // ԭʼ��Ϣ
    const string message3 = "and";          // ��չ��Ϣ
    // ����ԭʼ��Ϣ�Ĺ�ϣֵ
    uint8_t hash[SM3_OUTLEN];
    SM3(message1, hash);
    cout << "����ԭʼ��Ϣ�Ĺ�ϣֵ SM3(M1)��" << endl;
    cout << "  - ԭʼ��Ϣ��" << message1 << endl;
    cout << "  - ����õ��Ĺ�ϣֵ��" << endl;
    cout << "    ";
    print_hex(hash, SM3_OUTLEN);
    cout << endl;
    // ������չ��Ϣ�������ϣֵ
    length_extension_attack(hash, message1.length(), message3);
    return 0;
}