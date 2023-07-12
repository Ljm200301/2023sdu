#pragma once
#include <stdint.h>
#include <immintrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include<string.h>
#define SM3_DIGEST_BYTES 32
#define SM3_BLOCK_BYTES 64
#define SM3_HMAC_BYTES SM3_DIGEST_BYTES

typedef struct sm3_ctx_t {
	uint32_t digest[SM3_DIGEST_BYTES / sizeof(uint32_t)];
	int nblocks;
	uint8_t block[SM3_BLOCK_BYTES * 4];
	int num;
}sm3_ctx;

void sm3_init(sm3_ctx* ctx);
void sm3_update(sm3_ctx* ctx, const uint8_t* data, size_t data_len);
void sm3_final(sm3_ctx* ctx, uint8_t* digest);

int sm3_hash(uint8_t* dgst, const uint8_t* msg, size_t len);
int sm3_hash_verify(const uint8_t* data, size_t dlen, const uint8_t* digest);

void sm3_init(sm3_ctx* ctx) {
	ctx->digest[0] = 0x7380166F;
	ctx->digest[1] = 0x49148289;
	ctx->digest[2] = 0x172442D7;
	ctx->digest[3] = 0xDA8A0600;
	ctx->digest[4] = 0xA96F30BC;
	ctx->digest[5] = 0x163138AA;
	ctx->digest[6] = 0xE38DEE4D;
	ctx->digest[7] = 0xB0FB0E4E;

	ctx->nblocks = 0;
	ctx->num = 0;
}

#define ROTL(X,n)  (((X)<<(n)) | ((X)>>(32-(n))))

#define P00(x) (_mm_xor_si128(_mm_xor_si128(_mm_xor_si128(x, _mm_slli_epi32(x, 9)), _mm_slli_epi32(x, 17)), _mm_srli_epi32(x, 25)))
#define P01(x) (_mm_xor_si128(_mm_xor_si128(_mm_xor_si128(x, _mm_slli_epi32(x, 15)), _mm_slli_epi32(x, 23)), _mm_srli_epi32(x, 7)))

#define P0(x) ((x) ^  ROTL((x),9)  ^ ROTL((x),17))
#define P1(x) ((x) ^  ROTL((x),15) ^ ROTL((x),23))

#define FF0(x,y,z) ( (x) ^ (y) ^ (z))
#define FF1(x,y,z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

#define GG0(x,y,z) ( (x) ^ (y) ^ (z))
#define GG1(x,y,z) (((x) & (y)) | ( (~(x)) & (z)) )

static void sm3_compress(uint32_t digest[SM3_DIGEST_BYTES / sizeof(uint32_t)], const uint8_t block[SM3_BLOCK_BYTES]) {
	int j;
	uint32_t W[68], W1[64];
	const uint32_t* pblock = (const uint32_t*)(block);

	uint32_t A = digest[0], B = digest[1], C = digest[2], D = digest[3];
	uint32_t E = digest[4], F = digest[5], G = digest[6], H = digest[7];

	uint32_t SS1, SS2, TT1, TT2, T[64];

	for (j = 0; j < 16; j++) W[j] = _byteswap_ulong(pblock[j]);

	for (j = 16; j < 68; j++) {
		W[j] = P1(W[j - 16] ^ W[j - 9] ^ ROTL(W[j - 3], 15)) ^ ROTL(W[j - 13], 7) ^ W[j - 6];
	}

	for (j = 0; j < 64; j++)W1[j] = W[j] ^ W[j + 4];

	for (j = 0; j < 16; j++) {
		T[j] = 0x79CC4519;
		SS1 = ROTL((ROTL(A, 12) + E + ROTL(T[j], j)), 7);
		SS2 = SS1 ^ ROTL(A, 12);
		TT1 = FF0(A, B, C) + D + SS2 + W1[j];
		TT2 = GG0(E, F, G) + H + SS1 + W[j - 16];
		D = C;
		C = ROTL(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = ROTL(F, 19);
		F = E;
		E = P0(TT2);
	}

	for (j = 16; j < 64; j++) {
		T[j] = 0x7A879D8A;
		SS1 = ROTL((ROTL(A, 12) + E + ROTL(T[j], j)), 7);
		SS2 = SS1 ^ ROTL(A, 12);
		TT1 = FF1(A, B, C) + D + SS2 + W1[j];
		TT2 = GG1(E, F, G) + H + SS1 + W[j - 16];
		D = C;
		C = ROTL(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = ROTL(F, 19);
		F = E;
		E = P0(TT2);
	}

	digest[0] ^= A;
	digest[1] ^= B;
	digest[2] ^= C;
	digest[3] ^= D;
	digest[4] ^= E;
	digest[5] ^= F;
	digest[6] ^= G;
	digest[7] ^= H;
}

void sm3_update(sm3_ctx* ctx, const uint8_t* data, size_t data_len) {
	int i, n;
	uint32_t* p;
	const uint8_t* q;
	if (ctx->num) {
		n = SM3_BLOCK_BYTES - ctx->num;
		if (n > data_len) n = data_len;
		memcpy(ctx->block + ctx->num, data, n);
		ctx->num += n;
		data += n;
		data_len -= n;
		if (ctx->num == SM3_BLOCK_BYTES) {
			sm3_compress(ctx->digest, ctx->block);
			ctx->nblocks++;
			ctx->num = 0;
		}
	}
	p = (uint32_t*)data;
	q = data;
	n = data_len / SM3_BLOCK_BYTES;
	for (i = 0; i < n; i++) {
		sm3_compress(ctx->digest, q);
		ctx->nblocks++;
		p += SM3_BLOCK_BYTES / sizeof(uint32_t);
		q += SM3_BLOCK_BYTES;
	}
	n = data_len % SM3_BLOCK_BYTES;
	memcpy(ctx->block, q, n);
	ctx->num = n;
}

void sm3_final(sm3_ctx* ctx, uint8_t* digest) {
	uint32_t* pdigest = (uint32_t*)digest;
	uint32_t* p;
	int i;
	ctx->block[ctx->num] = 0x80;
	for (i = ctx->num + 1; i < SM3_BLOCK_BYTES; i++) ctx->block[i] = 0;
	if (ctx->num + 8 > SM3_BLOCK_BYTES) {
		sm3_compress(ctx->digest, ctx->block);
		ctx->nblocks++;
		memset(ctx->block, 0, SM3_BLOCK_BYTES);
	}
	p = (uint32_t*)(ctx->block + SM3_BLOCK_BYTES - 8);
	*p++ = 0;
	*p++ = (ctx->nblocks << 9) + (ctx->num << 3);
	sm3_compress(ctx->digest, ctx->block);
	for (i = 0; i < SM3_DIGEST_BYTES / sizeof(uint32_t); i++) {
		*pdigest++ = _byteswap_ulong(ctx->digest[i]);
	}
}

int sm3_hash(uint8_t* dgst, const uint8_t* msg, size_t len) {
	sm3_ctx ctx;
	sm3_init(&ctx);
	sm3_update(&ctx, msg, len);
	sm3_final(&ctx, dgst);
	return 1;
}

int sm3_hash_verify(const uint8_t* data, size_t dlen, const uint8_t* digest) {
	uint8_t tmp_digest[SM3_DIGEST_BYTES];
	sm3_hash(tmp_digest, data, dlen);
	return memcmp(tmp_digest, digest, SM3_DIGEST_BYTES) == 0;
}