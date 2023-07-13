#include <iostream>
#include <ctime>
#include <string>
#include <cmath>
#include "sm3.h"

using namespace std;

#define uint unsigned int

const int Collisionlen = 16;

#define F(x,c) (x*x)

int Pollard_Rho(uint image, unsigned char* H, uint c, uint* preiamge) //H = SM3(image) 
{
    uint m1 = rand();
    uint m2 = m1;
    while (true)
    {
        m1 = F(m1, c);
        m2 = F(F(m2, c), c);
        if (m2 == m1)
            return 1;
        uint tmp = m2 - m1;
        string input = to_string(tmp).c_str();
        unsigned char output[SM3_OUTLEN];
        SM3(input, output);

        // ֱ����whileѭ���бȽϹ�ϣֵ��ǰCollisionlenλ
        uint a = *(int*)H;
        uint b = *(int*)output;
        uint mask = (int)pow(2, Collisionlen) - 1;
        if ((a & mask) == (b & mask) && tmp != image)
        {
            *preiamge = tmp;
            cout << "�ҵ�һ����ײ��" << endl;
            cout << "SM3(" << image << ") = ";
            print_Hashvalue(H, SM3_OUTLEN);
            cout << "SM3(" << tmp << ") = ";
            print_Hashvalue(output, SM3_OUTLEN);
            return 0;
        }
    }
}

void PreimageAttack(uint image)
{
    uint preimage;
    string image_input = to_string(image);
    unsigned char image_output[SM3_OUTLEN];
    SM3(image_input, image_output);
    cout << "ҪѰ�ҵ�ԭ��" << image << endl;
    cout << "SM3(" << image << ") = ";
    print_Hashvalue(image_output, SM3_OUTLEN);

    uint c = rand();

    clock_t start_time = clock();
    while (Pollard_Rho(image, image_output, c, &preimage))
    {
        c = rand();
    }
    clock_t end_time = clock();

    cout << endl;
    cout << "�ҵ���ײ���õ������ c = " << c << endl;
    cout << "�ҵ���ԭ��" << preimage << endl;
    cout << "Ѱ����ײ����ʱ�䣺time = " << double(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
}

int main()
{
    srand(time(NULL));
    unsigned int image = rand();
    PreimageAttack(image);
    return 0;
}