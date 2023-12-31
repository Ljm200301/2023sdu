# 实现方式
## 总体概述
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/sm2_2P_decrypt.png">
</div>

## sm2_2P_decrypt_A
在sm2_2P_decrypt_A中，实现了一种基于SM2算法的加密通信过程，其主要流程如下：

- 创建一个SM2对象，用于后续的加密和解密操作；
- 客户端向服务端发起连接请求；
- 客户端生成公钥P和私钥d2，并将公钥P发送给服务端；
- 客户端利用SM2算法对待加密的明文进行加密，并将加密结果C和C1发送给服务端；
- 客户端生成随机数T1，并将其发送给服务端；
- 服务端利用自己的私钥d1和接收到的随机数T1生成随机数T2，并将其发送给客户端；
- 客户端利用自己的私钥d2和接收到的随机数T2对加密结果C进行解密，得到明文M。
  
## sm2_2P_decrypt_B

- 服务端等待客户端的连接请求；
- 客户端向服务端发起连接请求，并发送随机数T1给服务端；
- 服务端接收到T1后，从客户端接收私钥d2并解析，利用自己的私钥d1和接收到的随机数T1生成随机数T2，并将其发送给客户端；
- 客户端利用自己的私钥d2和接收到的随机数T2对待签名的消息进行签名；
- 客户端将签名结果T2发送给服务端；
- 服务端接收到T2后，根据接收到的T1、T2和公钥P进行验签操作，判断签名是否正确。

# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SM2_dec_2P_A.png">
</div>

<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SM2_dec_2P_B.png">
</div>
