from ec import EC, EC_point
from random import randint
import time
import random
import math
def hash(msg, point):
    return sum([ord(x) for x in msg]) % 13 + \
           ((point.x + point.y) if not EC_point.is_inf(point) else 1)

def Schnorr_sign(msg,d,ec):
    k = randint(1, ec.size)
    R = ec.n_times_g(k)
    e = hash(msg, R)
    s = (k + e * d) % ec.size
    return (R, s)

def Schnorr_verify(msg, sign, P, ec):
    sG=ec.n_times_g(sign[1])
    e=hash(msg,sign[0])
    eP=ec.n_times_point(P, e)
    R_plus_eP=EC_point.add(eP,sign[0],ec)
    print('Schnorr verfication successfull == ', sG == R_plus_eP)

def msg_signing(msg, ec):
    d = randint(1, ec.p)
    P = ec.n_times_g(d)
    sign = Schnorr_sign(msg,d,ec)
    Schnorr_verify(msg,sign,P,ec)


def Schnorr_batch_verify(msgs, sigs, P_list, ec):

    n = len(msgs)
    e_vals = [hash(msgs[i], sigs[i][0]) for i in range(n)]
    s_vals = [sig[1] for sig in sigs]

    sG_sum = ec.n_times_g(s_vals[0])
    R_plus_eP = EC_point.add(sigs[0][0], ec.n_times_point(P_list[0], e_vals[0]), ec)

    for i in range(1, n):
        sG_sum = EC_point.add(sG_sum, ec.n_times_g(s_vals[i]), ec)
        R_plus_eP = EC_point.add(R_plus_eP, EC_point.add(sigs[i][0], ec.n_times_point(P_list[i], e_vals[i]), ec), ec)

    print('Schnorr Batch verification successful == ', sG_sum == R_plus_eP)


def generate_coprime_int(p):
    while True:
        x = randint(1, p)
        if math.gcd(x, p) == 1:
            return x

def test_Schnorr_batch_verify():
    # 初始化椭圆曲线
    ec = EC(5, 3, 137, EC_point(1, 3))

    # 生成私钥和公钥
    d_list = [generate_coprime_int(ec.p) for _ in range(5)]
    P_list = [ec.n_times_g(d) for d in d_list]


    msgs = ['Hello' + str(i) for i in range(5)]
    sigs = [Schnorr_sign(msg, d, ec) for msg, d in zip(msgs, d_list)]

    start_time = time.perf_counter()
    Schnorr_batch_verify(msgs, sigs, P_list, ec)
    end_time = time.perf_counter()
    print("Schnorr Batch verification time:", end_time - start_time, "seconds")


if __name__ == '__main__':
    #ec = EC(2, 2, 17, EC_point(5, 1))
    ec = EC(5,3,137,EC_point(1,3))

    msg_signing('ljm', ec)

    test_Schnorr_batch_verify()
