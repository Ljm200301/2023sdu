#include <iostream>
#include <ctime>
#include <string>
#include <cmath>
#include "sm3.h"

using namespace std;


const int Collisionlen = 16;

#define F(x,c) (x*x)

int Pollard_Rho(uint32_t image, unsigned char* H, uint32_t c, uint32_t* preiamge) //H = SM3(image) 
{
    uint32_t m1 = rand();
    uint32_t m2 = m1;
    while (true)
    {
        m1 = F(m1, c);
        m2 = F(F(m2, c), c);
        if (m2 == m1)
            return 1;
        uint32_t tmp = m2 - m1;
        string input = to_string(tmp).c_str();
        unsigned char output[SM3_OUTLEN];
        SM3(input, output);

        // 直接在while循环中比较哈希值的前Collisionlen位
        uint32_t a = *(int*)H;
        uint32_t b = *(int*)output;
        uint32_t mask = (int)pow(2, Collisionlen) - 1;
        if ((a & mask) == (b & mask) && tmp != image)
        {
            *preiamge = tmp;
            cout << "找到一组碰撞：" << endl;
            cout << "SM3(" << image << ") = ";
            print_Hashvalue(H, SM3_OUTLEN);
            cout << "SM3(" << tmp << ") = ";
            print_Hashvalue(output, SM3_OUTLEN);
            return 0;
        }
    }
}

void PreimageAttack(uint32_t image)
{
    uint32_t preimage;
    string image_input = to_string(image);
    unsigned char image_output[SM3_OUTLEN];
    SM3(image_input, image_output);
    cout << "要寻找的原像：" << image << endl;
    cout << "SM3(" << image << ") = ";
    print_Hashvalue(image_output, SM3_OUTLEN);

    uint32_t c = rand();

    clock_t start_time = clock();
    while (Pollard_Rho(image, image_output, c, &preimage))
    {
        c = rand();
    }
    clock_t end_time = clock();

    cout << endl;
    cout << "找到碰撞所用的随机数 c = " << c << endl;
    cout << "找到的原像：" << preimage << endl;
    cout << "寻找碰撞花费时间：time = " << double(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
}

int main()
{
    srand(time(NULL));
    unsigned int image = rand();
    PreimageAttack(image);
    return 0;
}
