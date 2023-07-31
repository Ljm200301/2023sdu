# 实现方式
Rho方法是一种基于哈希链的碰撞查找算法，通常用于对哈希函数进行攻击。该算法通过维护两个不同的哈希链来查找碰撞。  
Rho方法的基本思路是选择一个随机数作为起点，然后使用哈希函数对其进行一系列的迭代计算，每次计算时都将前一次的结果作为输入，得到一个新的输出。这样，就得到了一个哈希链。为了找到碰撞，需要维护两个不同的哈希链，并不断地对它们进行迭代计算，直到找到两个相同的输出。  
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/rho.png">
</div>

在具体实现的过程中，使用了**OpenSSL库**提供的EVP_MD_CTX结构体和EVP_Digest函数来进行SM3哈希计算。**主要寻找碰撞的过程如下：**  
- 定义了四个长度为32的unsigned char数组out1、out2、out3和out4，用于存储哈希函数的输出结果。  
- 使用EVP_Digest函数对data数组进行SM3哈希计算，将结果存储在out1数组中。  
- 使用EVP_Digest函数对out1数组进行SM3哈希计算，将结果存储在out2数组中。
- 之后进入寻找碰撞过程：
  - 使用EVP_Digest函数对out1数组进行SM3哈希计算，将结果存储在out3数组中。  
  -  使用EVP_Digest函数对out2数组进行SM3哈希计算，将结果存储在out4数组中。  
  - 从out3和out4数组中取出COLLISION_BYTE个字节作为哈希函数输出结果的前缀，如果这两个前缀相同，则表示找到了碰撞，输出碰撞的结果并跳出循环。  
  - 将out3数组的内容复制到out1数组中，将out4数组的内容复制到out2数组中。  
# 实现效果
**找到24bit的碰撞，如下图所示，最多可找到40bit的碰撞。**
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/rho_method.png">
</div>
寻找到24bit碰撞的时间经多次测试为2s左右，寻找40bit碰撞的时间约3分钟.
