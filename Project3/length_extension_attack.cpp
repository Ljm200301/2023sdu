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
    // 为填充后的消息分配空间
    uint8_t* padded_message = new uint8_t[block_size + message.length()];
    memset(padded_message, 0, block_size + message.length());
    // 将原始消息复制到填充后的消息中
    memcpy(padded_message, message.c_str(), message.length());
    // 添加填充字节
    padded_message[message.length()] = 0x80;
    // 计算消息的 bit 长度，用于填充
    uint64_t message_bit_length = len * 8;
    padded_message[block_size - 8] = (message_bit_length >> 56) & 0xff;
    padded_message[block_size - 7] = (message_bit_length >> 48) & 0xff;
    padded_message[block_size - 6] = (message_bit_length >> 40) & 0xff;
    padded_message[block_size - 5] = (message_bit_length >> 32) & 0xff;
    padded_message[block_size - 4] = (message_bit_length >> 24) & 0xff;
    padded_message[block_size - 3] = (message_bit_length >> 16) & 0xff;
    padded_message[block_size - 2] = (message_bit_length >> 8) & 0xff;
    padded_message[block_size - 1] = message_bit_length & 0xff;
    // 计算填充长度
    int pad_len = block_size * ceil((len + 9) / (double)block_size);
    // 计算拓展消息的长度
    int extended_len = pad_len + message.length();
    // 分配空间
    uint8_t* extended_message = new uint8_t[extended_len];
    memset(extended_message, 0, extended_len);
    // 将填充后的原始消息和拓展消息拼接
    memcpy(extended_message, padded_message, pad_len);
    memcpy(extended_message + pad_len, message.c_str(), message.length());
    // 构造 SM3 上下文，将原始哈希值作为初始 IV
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
    // 计算拓展消息的哈希值
    ctx.msgLen = extended_len << 3;
    SM3_process(&ctx, extended_message, message.length());
    SM3_paddingpart(&ctx, hash);
    cout << "利用原始哈希值和长度计算 SM3(M3)，其中 IV 为原始哈希值：" << endl;
    cout << "  - 原始哈希值：" << endl;
    cout << "    ";
    print_hex(hash, SM3_OUTLEN);
    cout << endl;
    cout << "  - 原始消息长度（字节）：" << len << endl;
    cout << "  - 拓展消息：" << message << endl;
    cout << "  - 填充后的原始消息和拓展消息：" << endl;
    cout << "    ";
    print_hex(extended_message, extended_len);
    cout << endl;
    cout << "  - 填充长度（字节）：" << pad_len - len << endl;
    cout << " - 拓展消息长度（字节）：" << message.length() << endl;
    cout << "  - 拼接后的消息长度（字节）：" << extended_len << endl;
    cout << "  - 计算得到的 SM3(M3) 值：" << endl;
    cout << "    ";
    print_hex(hash, SM3_OUTLEN);
    cout << endl;
    // 释放内存
    delete[] padded_message;
    delete[] extended_message;
}

int main() {
    const string message1 = "SDUliujiaming";  // 原始消息
    const string message3 = "and";          // 拓展消息
    // 计算原始消息的哈希值
    uint8_t hash[SM3_OUTLEN];
    SM3(message1, hash);
    cout << "计算原始消息的哈希值 SM3(M1)：" << endl;
    cout << "  - 原始消息：" << message1 << endl;
    cout << "  - 计算得到的哈希值：" << endl;
    cout << "    ";
    print_hex(hash, SM3_OUTLEN);
    cout << endl;
    // 构造拓展消息并计算哈希值
    length_extension_attack(hash, message1.length(), message3);
    return 0;
}