#include <iostream>
#include <cstring>
#include <chrono>
#include <stdint.h>
#include <arm_neon.h>
// AES128��Կ��չ����������16�ֽ���Կ��ָ��44��32λ����Կ��ָ��
void AES128_key_expansion_ARMv8(const uint8_t key[16], uint32_t round_key[44]) {
    uint8x16_t key_128 = vld1q_u8(key);

    // ʹ��vreinterpretq_u32_u8��uint8x16_tת��Ϊuint32x4_t����
    uint32x4_t key_0 = vreinterpretq_u32_u8(key_128);

    // �ֳ���
    const uint8_t RC[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

    // ��Կ��չ
    uint32x4_t temp;
    for (int i = 0; i < 10; i++) {
        temp = key_0;
        temp = vextq_u32(temp, temp, 3);
        temp = vreinterpretq_u32_u8(vqtbl1q_u8(vreinterpretq_u8_u32(temp), vreinterpretq_u8_u32(vld1q_u32((const uint32_t*)&aes_sbox[0]))));
        temp = veorq_u32(temp, vdupq_n_u32((uint32_t)RC[i] << 24));
        temp = veorq_u32(temp, key_0);
        round_key[i] = vgetq_lane_u32(temp, 0);
        round_key[i + 1] = vgetq_lane_u32(temp, 1);
        round_key[i + 2] = vgetq_lane_u32(temp, 2);
        round_key[i + 3] = vgetq_lane_u32(temp, 3);

        key_0 = temp;
    }
}

// AES128���ܺ���������16�ֽ��������ݿ顢ָ��44��32λ����Կ��ָ���������ݿ�ָ��
void AES_encrypt(const uint8_t input[16], uint8_t output[16], const uint32_t round_key[44]) {

    uint8x16_t data_block = vld1q_u8(input);
    uint8_t* p8 = (uint8_t*)round_key;
    data_block = veorq_u8(data_block, vld1q_u8(p8 + 16 * 0));

    // Round 1-9
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 1)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 2)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 3)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 4)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 5)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 6)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 7)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 8)));
    data_block = vaesmcq_u8(vaeseq_u8(data_block, vld1q_u8(p8 + 16 * 9)));

    // ���һ��
    data_block = veorq_u8(data_block, vld1q_u8(p8 + 16 * 10));
    vst1q_u8(output, data_block);
}


int main() {
    unsigned char key[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                             0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x// ����AES��Կ�ʹ���������
    unsigned char aes_key[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                                  0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
    unsigned char input_data[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                                     0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };

    // ִ����Կ��չ
    uint32_t round_key[44];
    AES128_key_expansion_ARMv8(aes_key, round_key);

    // ִ�м��ܲ���
    unsigned char output_data[16];
    AES_encrypt(input_data, output_data, round_key);

    return 0;
    }