# report on the application of this deduce technique in Ethereum with ECDSA
## ECDSA简介
ECDSA（Elliptic Curve Digital Signature Algorithm，椭圆曲线数字签名算法）是一种基于椭圆曲线密码学的数字签名算法。$ECDSA$ 的安全性基于椭圆曲线离散对数问题，与普通的离散对数问题（DLP）和大数分解问题（IFP）不同，椭圆曲线离散对数问题没有亚指数时间的解决方法。因此椭圆曲线密码的单位比特强度要高于其他公钥体制。   
假设私钥为 $d$，公钥为 $Q$，消息的哈希值为 $H$，随机数为 $k$， $k$ 的逆元为 $k^{-1}$，椭圆曲线参数为 $a$、 $b$ 和 $p$。  
签名过程：  
（1）计算 $kG$ 的 $x$ 坐标，即 $r = x(kG) \bmod p$，其中 $G$ 是椭圆曲线上的基点。  
（2）计算 $s = k^{-1}(H + dr) \bmod p$。  
（3）得到签名 $(r, s)$。  
验证过程：  
（1）计算 $w = s^{-1} \bmod p$。  
（2）计算 $u_1 = Hw \bmod p$ 和 $u_2 = rw \bmod p$。  
（3）计算点 $P = u_1G + u_2Q$。  
（4）如果 $P$ 的 $x$ 坐标等于 $r$，则认为签名是有效的，否则认为签名是无效的。  
