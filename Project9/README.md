# 实现方式
## AES
AES（Advanced Encryption Standard）是一种对称加密算法，它使用相同的密钥对数据进行加密和解密。AES128 使用 128 位密钥对数据进行加密和解密，它的加密过程可以用以下公式表示：
### 分组
将明文数据按照128位一组进行分组，可以将每个分组表示为一个 $4 \times 4$ 的矩阵 $S$，其中 $S_{i,j}$ 表示矩阵中第 $i$ 行第 $j$ 列的元素。
### 密钥扩展
将 128 位密钥扩展为 176 个字节的轮密钥，每个轮密钥的长度为 16 字节。扩展密钥的过程包括以下步骤：  
初始轮密钥：将 128 位密钥按照字节拆分为 16 个字节，作为初始轮密钥。  
轮密钥扩展：根据初始轮密钥生成其他的轮密钥，直到得到 10 个轮密钥，每个轮密钥占用 16 个字节。  
### 轮函数  
轮函数是 AES 加密算法中的核心部分，它包含四个步骤：SubBytes、ShiftRows、MixColumns 和 AddRoundKey。  
SubBytes：使用一个 S 盒将每个字节替换为另一个字节。  
ShiftRows：对每个分组进行行移位，第 $i$ 行移位 $i-1$ 个字节。  
MixColumns：对每个分组进行列混淆，使用固定的矩阵乘法计算。  
AddRoundKey：每个分组与对应的轮密钥进行按位异或运算。  
### 加密
加密过程是将明文数据按照分组进行加密，并依次执行 10 个轮函数，最后得到密文数据。  
### 解密
解密过程与加密过程类似，只是轮密钥的顺序与加密过程相反。解密过程中需要执行 10 个轮函数，每个轮函数使用对应的轮密钥进行解密操作。  

# 实现效果

<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/AES.png">
</div>

<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SM4.png">
</div>
