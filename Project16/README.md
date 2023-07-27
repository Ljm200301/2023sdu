# 实现方式
## sm2_2P_decrypt_A
在sm2_2P_decrypt_A中，实现了一种基于SM2算法的加密通信过程，其主要流程如下：

创建一个SM2对象，用于后续的加密和解密操作；
客户端向服务端发起连接请求；
客户端生成公钥P和私钥d2，并将公钥P发送给服务端；
客户端利用SM2算法对待加密的明文进行加密，并将加密结果C和C1发送给服务端；
客户端生成随机数T1，并将其发送给服务端；
服务端利用自己的私钥d1和接收到的随机数T1生成随机数T2，并将其发送给客户端；
客户端利用自己的私钥d2和接收到的随机数T2对加密结果C进行解密，得到明文M。



# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SM2_dec_2P_A.png">
</div>

<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SM2_dec_2P_B.png">
</div>
