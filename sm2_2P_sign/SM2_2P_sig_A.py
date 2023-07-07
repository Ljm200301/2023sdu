import socket
from SM2 import *
import struct

# 生成待签名数据Z和M
Z = b'Z'
M = b'M'

# 创建一个 SM2 对象
sm2_crypt = CryptSM2(private_key=None, public_key="")

# 建立连接
print('Waiting for connection...')
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('localhost', 8888))
server_socket.listen(1)
print("connected!")


# 等待连接
client_socket, address = server_socket.accept()

# 接收签名者发送的 P1，并根据其生成 P
length_bytes = client_socket.recv(4)
length = int.from_bytes(length_bytes, byteorder='big')
data_bytes = b''
while len(data_bytes) < length:
    packet = client_socket.recv(length - len(data_bytes))
    if not packet:
        # 连接已经关闭
        break
    data_bytes += packet

# 将字节串转换成原始数据
P1 = data_bytes.decode()
P = sm2_crypt.genrateP(P1)

# 发送 P 给签名者
P_bytes = P.encode()
client_socket.send(P_bytes)

# 生成公钥和私钥，并发送给签名者
# public_key, private_key = sm2_crypt.genrateKey(P)
# client_socket.send(public_key)
# client_socket.send(private_key)

# 接收签名者发送的 Q1 和 e，并根据其计算 r、s2 和 s3
Q1_bytes = client_socket.recv(1024)
Q1=Q1_bytes.decode()
e_len = struct.unpack('<I', client_socket.recv(4))[0]
e_bytes = client_socket.recv(e_len)
e = int.from_bytes(e_bytes, byteorder='big')
r, s2, s3 = sm2_crypt.get_r_s2_s3(Q1, e)
print('r:')
print(r)
print('s2:')
print(s2)
print('s3:')
print(s3)

# 将 r、s2 和 s3 发送给签名者
r_bytes = r.to_bytes((r.bit_length() + 7) // 8, byteorder='big')
r_length = len(r_bytes)
client_socket.sendall(struct.pack('<I', r_length) + r_bytes)

s2_bytes = s2.to_bytes((s2.bit_length() + 7) // 8, byteorder='big')
s2_length = len(s2_bytes)
client_socket.sendall(struct.pack('<I', s2_length) + s2_bytes)

s3_bytes = s3.to_bytes((s3.bit_length() + 7) // 8, byteorder='big')
s3_length = len(s3_bytes)
client_socket.sendall(struct.pack('<I', s3_length) + s3_bytes)

# 接收签名者发送的签名值 sig，并进行验证
sig_bytes = client_socket.recv(1024)
sig = sig_bytes.decode()
d1_len = struct.unpack('<I', client_socket.recv(4))[0]
d1_bytes = client_socket.recv(d1_len)
d1 = int.from_bytes(d1_bytes, byteorder='big')

a=(d1*sm2_crypt.d2)%int(sm2_crypt.ecc_table['n'], base=16)
private_key=str(hex(sm2_crypt.inverse_mod_prime(a,int(sm2_crypt.ecc_table['n'], base=16))-1))
sm2_crypt_C = CryptSM2(public_key=P, private_key=private_key)
M_=Z+M
e = sm3.sm3_hash(func.bytes_to_list(M_)).encode()
verify = sm2_crypt_C.verify(sig,e)
print('验证结果:')
print(verify)

if verify:
    print('Signature verification succeeded!')
else:
    print('Signature verification failed!')

# 关闭连接
client_socket.close()
server_socket.close()