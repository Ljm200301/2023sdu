#include <iostream>
#include <stdio.h>
#include <time.h>
#include"sm3.h"
#include <windows.h>
#include "openssl/evp.h"
#include "optimized_SM3.h"

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
using namespace std;

void printf_hash(unsigned char*dgst1) {
	for (int i = 0; i < 32; i++)
		printf("%02x", dgst1[i]);
	printf("\n");
}
int main() {
	LARGE_INTEGER t1, t2, t3, t4, tc;
	unsigned char msg[] = "Once upon a time, there was a boy named Jack who loved to play video games. One day, he found a mysterious game cartridge in his attic. He inserted it into his console and was immediately transported to a magical world. In this world, he met a talking dragon who needed his help to defeat an evil sorcerer. Jack accepted the quest and embarked on an adventure filled with danger and excitement. Along the way, he made new friends and discovered his own inner strength. With courage and determination, Jack and his companions finally defeated the sorcerer and saved the dragon kingdom. From that day forward, Jack knew that anything was possible if he believed in himself.";
	size_t msg_len = strlen((const char*)msg);
	unsigned char dgst1[32];
	unsigned char dgst2[32];
	uint32_t out2_size = 32;

	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	for (int i = 0; i < 100000; i++)
		sm3_hash(dgst1, msg, msg_len);

	QueryPerformanceCounter(&t2);

	std::cout << "Time taken by SM3 to hash the message 100000 times: " << ((long double)(t2.QuadPart - t1.QuadPart) / (long double)tc.QuadPart) * 1000 << " ms\n";


	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	for (int i = 0; i < 100000; i++)
		sm3_hash_simd(dgst2, msg, msg_len);

	QueryPerformanceCounter(&t2);
	std::cout << "Time taken by optimized SM3 to hash the message 100000 times: " << ((long double)(t2.QuadPart - t1.QuadPart) / (long double)tc.QuadPart) * 1000 << " ms\n";

	printf("\n");


	std::cout << "Hash value generated by SM3: ";
	printf_hash(dgst1);
	std::cout << "Hash value generated by optimized SM3: ";
	printf_hash(dgst2);

}