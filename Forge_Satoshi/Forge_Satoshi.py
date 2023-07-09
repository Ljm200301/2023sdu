import hashlib
import random
def Gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a
#扩展欧几里得
def extended_euclidean(a, b):
    x0, x1, y0, y1 = 1, 0, 0, 1
    while b != 0:
        q, a, b = a // b, b, a % b
        x0, x1 = x1, x0 - q * x1
        y0, y1 = y1, y0 - q * y1
    return x0, y0

# 求乘法逆元
def multi_inverse(a, b):
    m = abs(b)
    x0, x1, y0, y1 = 1, 0, 0, 1
    while b != 0:
        q, a, b = a // b, b, a % b
        x0, x1 = x1, x0 - q * x1
        y0, y1 = y1, y0 - q * y1
    if a != 1:
        return None
    return x0 % m
#点加算法
def Add(P, Q):
    if P[0] == Q[0]:
        if P[1] == -Q[1]:
            return 0
        else:
            lam = ((3 * pow(P[0], 2) + a) * multi_inverse(2 * P[1], mod_n)) % mod_n
    else:
        lam = ((Q[1] - P[1]) * multi_inverse(Q[0] - P[0], mod_n)) % mod_n

    Rx = (pow(lam, 2) - P[0] - Q[0]) % mod_n
    Ry = (lam * (P[0] - Rx) - P[1]) % mod_n
    return (Rx, Ry)
#点乘算法
def Multi(n, point):
    if n == 0:
        return 0
    elif n == 1:
        return point
    elif n % 2 == 0:
        half = Multi(n // 2, point)
        return Add(half, half)
    else:
        half = Multi((n - 1) // 2, point)
        return Add(point, Add(half, half))
#ECDSA签名
def Sign(m, G, d,k):
    e = hash(m)
    R = Multi(k, G)
    r = R[0] % mod_n
    s = (multi_inverse(k, mod_n) * (e + d * r)) % mod_n
    return r, s
#ECDSA验证
def Verify(r,s,e,G,P):
    w = multi_inverse(s, mod_n)
    ele1 = (e * w) % mod_n
    ele2 = (r * w) % mod_n
    w = Add(Multi(ele1, G), Multi(ele2, P))
    if (w == 0):
        return 0
    else:
        if (w[0] % mod_n == r):
            return 1
        else:
            return 0
#伪造算法
def Pretend(r, s,G, P):
    u = random.randrange(1, mod_n - 1)
    v = random.randrange(1, mod_n - 1)
    r_forge = Add(Multi(u, G), Multi(v, P))[0]
    e_forge = (r_forge * u * multi_inverse(v, mod_n)) % mod_n
    s_forge = (r_forge * multi_inverse(v, mod_n)) % mod_n
    if(Verify( r_forge, s_forge,e_forge, G, P)):
        print("伪造成功！")
        return (r_forge,s_forge)

mod_n = 23
a = 4
b = 5
G=[5, 1]
k = 3
message = "ljm"
d = random.randint(1, mod_n-2)
while Gcd(d, mod_n-1) != 1:
    d = random.randint(1, mod_n-2)

r,s=Sign(message,G,d,k)
P = Multi(d, G)
print("待签名消息为",message)
print("验证公钥为",P)
print("原始签名为",(r,s))
print("伪造的签名为",Pretend(r,s,G,P))