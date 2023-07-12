# 实现方式

<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SM21.png">
</div>

**对A(服务端)**  
生成待签名数据 Z 和 M。  
创建 CryptSM2 类的一个实例。  
等待来自客户端的连接请求。  
接收客户端发送的 P1，并使用 CryptSM2 实例生成 P。  
将 P 发送回客户端。  
接收客户端发送的 Q1 和 e，并使用 CryptSM2 实例计算 r、s2 和 s3。  
将 r、s2 和 s3 发送回客户端。  
接收客户端发送的签名值 sig 和 d1。  
使用 d1 计算私钥，并使用计算出的私钥和接收到的签名值进行验证。  
关闭连接。

**对B(客户端)**
导入 socket 和 CryptSM2 模块，以及 gmssl 库中的 sm3 和 func 函数。  
创建一个 CryptSM2 类的对象 sm2_crypt，用于进行 SM2 加密和签名操作。  
建立与服务器的连接，使用 socket 模块的 socket 函数创建一个套接字对象 client_socket，并使用 connect 方法连接服务器。  
使用 CryptSM2 实例的 genrateP1 方法生成随机数 P1，并将其发送给服务器。  
接收服务器发送的 P，并使用 CryptSM2 实例生成公私钥对。  
生成待签名数据 Z 和 M。  
使用 CryptSM2 实例的 get_e 和 get_Q1 方法计算 e 和 Q1，并将它们发送给服务器。  
接收服务器发送的 r、s2 和 s3，并根据其计算出签名值 sig。  
将签名值发送给服务器，并发送私钥 d1。  
关闭连接。


# 实现效果

