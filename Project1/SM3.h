#pragma once
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;
typedef unsigned int uint;
typedef unsigned char uchar;

# define SM3_WORD uint  
# define SM3_OUTLEN    32  
# define SM3_BLOCK  64      

#define SM3_A 0x7380166f
#define SM3_B 0x4914b2b9
#define SM3_C 0x172442d7
#define SM3_D 0xda8a0600
#define SM3_E 0xa96f30bc
#define SM3_F 0x163138aa
#define SM3_G 0xe38dee4d
#define SM3_H 0xb0fb0e4e

#define SM3_T1 0x79CC4519
#define SM3_T2 0x7A879D8A

typedef struct {
    SM3_WORD state[8];
    uint msgLen;  
    uint curlen; 
    uchar buf[64];   
}SM3_CTX;


#  define HOST_c2l(c,l) (l =(((unsigned long)(*(c++)))<<24),          \
                         l|=(((unsigned long)(*(c++)))<<16),          \
                         l|=(((unsigned long)(*(c++)))<< 8),          \
                         l|=(((unsigned long)(*(c++)))    )           )


#  define HOST_l2c(l,c) (*(c++)=(uchar)(((l)>>24)&0xff),      \
                         *(c++)=(uchar)(((l)>>16)&0xff),      \
                         *(c++)=(uchar)(((l)>> 8)&0xff),      \
                         *(c++)=(uchar)(((l)    )&0xff),      \
                         l)

# define ROTATE(a,n)     (((a)<<(n))|((a&0xffffffff)>>(32-n)))

#define FF0(x,y,z) ( x^ y ^ z)
#define FF1(x,y,z) ((x& y) | ( x & z) | ( y & z))

#define GG0(x,y,z) ( x ^ y ^ z)
#define GG1(x,y,z) ((x & y) | ( (~x) & z) )

#define SHL(x,n) ((x & 0xFFFFFFFF) << n)
#define ROTL(x,n) (SHL(x,n) | (x >> (32 - n))) 

#define P0(x) (x ^  ROTL(x,9) ^ ROTL(x,17))
#define P1(x) (x ^  ROTL(x,15) ^ ROTL(x,23))

void CF(unsigned int W[68], unsigned int W1[64], unsigned int V[]) {
    unsigned int A, B, C, D, E, F, G, H, T, SS1, SS2, TT1, TT2, FF, GG;
    int j;
    A = V[0];
    B = V[1];
    C = V[2];
    D = V[3];
    E = V[4];
    F = V[5];
    G = V[6];
    H = V[7];

    for (j = 0; j < 64; j++) {
        if (j == 0) {
            T = SM3_T1;
        }
        else if (j == 16) {
            T = ROTL(SM3_T2, 16);
        }
        else {
            T = ROTL(T, 1);
        }

        if (j < 16) {
            FF = FF0(A, B, C);
            GG = GG0(E, F, G);
        }
        else {
            FF = FF1(A, B, C);
            GG = GG1(E, F, G);
        }

        SS1 = ROTL((ROTL(A, 12) + E + T), 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = FF + D + SS2 + W1[j];
        TT2 = GG + H + SS1 + W[j];

        D = C;
        C = ROTL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTL(F, 19);
        F = E;
        E = P0(TT2);
    }

    V[0] ^= A;
    V[1] ^= B;
    V[2] ^= C;
    V[3] ^= D;
    V[4] ^= E;
    V[5] ^= F;
    V[6] ^= G;
    V[7] ^= H;
}
void SM3Init(SM3_CTX* ctx) {
    ctx->msgLen = ctx->curlen = 0;
    ctx->state[0] = SM3_A;
    ctx->state[1] = SM3_B;
    ctx->state[2] = SM3_C;
    ctx->state[3] = SM3_D;
    ctx->state[4] = SM3_E;
    ctx->state[5] = SM3_F;
    ctx->state[6] = SM3_G;
    ctx->state[7] = SM3_H;
}

void SM3_W_expend(unsigned int W[68], unsigned int W1[64], const uchar* buf) {
    int i;
    for (i = 0; i < 16; i++)
        (void)HOST_c2l(buf, W[i]);

    unsigned int tmp;
    for (i = 16; i <= 67; i++) {
        tmp = W[i - 16] ^ W[i - 9] ^ ROTL(W[i - 3], 15);
        W[i] = P1(tmp) ^ (ROTL(W[i - 13], 7)) ^ W[i - 6];
    }
    for (i = 0; i <= 63; i++)
        W1[i] = W[i] ^ W[i + 4];
}

void SM3_compress(SM3_CTX* ctx) {
    unsigned int W[68];
    unsigned int W1[64];
    SM3_W_expend(W, W1, (const uchar*)ctx->buf);
    CF(W, W1, ctx->state);
}

void SM3_process(SM3_CTX* ctx, uchar* input, int msg_bytelen) {
    while (msg_bytelen >= 64) {
        memcpy(ctx->buf, input + (int)(ctx->curlen >> 9), 64);
        msg_bytelen -= 64;
        SM3_compress(ctx);
        ctx->msgLen += 512;
        ctx->curlen = 64;
    }
    memset((void*)ctx->buf, 0, 64);
    int tmp = msg_bytelen & 63;
    if (tmp) {
        memcpy(ctx->buf, input + (ctx->msgLen >> 3), tmp);
        ctx->msgLen += tmp << 3;
    }
    ctx->curlen = tmp;
}
void SM3_paddingpart(SM3_CTX* ctx, uchar* output)
{
    ctx->buf[ctx->curlen] = 0x80;
    ctx->curlen++;

    if (ctx->curlen > 56)
    {
        SM3_compress(ctx);
        memset((void*)ctx->buf, 0, 64);
        ctx->curlen = 0;
    }
    ctx->buf[63] = ctx->msgLen & 0xff;
    ctx->buf[62] = (ctx->msgLen >> 8) & 0xff;
    ctx->buf[61] = (ctx->msgLen >> 16) & 0xff;
    ctx->buf[60] = (ctx->msgLen >> 24) & 0xff;

    SM3_compress(ctx);
    memcpy(output, ctx->state, SM3_OUTLEN);
    for (int i = 0; i < 8; i++)
        (void)HOST_l2c(ctx->state[i], output);
}

void SM3(uchar* input, int msg_bytelen, uchar output[SM3_OUTLEN])
{
    SM3_CTX ctx;
    ctx.msgLen = ctx.curlen = 0;
    ctx.state[0] = SM3_A;
    ctx.state[1] = SM3_B;
    ctx.state[2] = SM3_C;
    ctx.state[3] = SM3_D;
    ctx.state[4] = SM3_E;
    ctx.state[5] = SM3_F;
    ctx.state[6] = SM3_G;
    ctx.state[7] = SM3_H;
    SM3_process(&ctx, input, msg_bytelen);
    SM3_paddingpart(&ctx, output);
    memset(&ctx, 0, sizeof(SM3_CTX));
}

void print_Hashvalue(uchar buf[], int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%02x", *(buf + i));
    }
    printf("\n");
}

void SM3(string input_str)
{
    uchar output[SM3_OUTLEN];
    SM3((uchar*)input_str.c_str(), input_str.length(), output);
    cout << "Hash:";
    print_Hashvalue(output, 32);
}

void SM3(string input_str, uchar output[SM3_OUTLEN])
{
    SM3((uchar*)input_str.c_str(), input_str.length(), output);
}