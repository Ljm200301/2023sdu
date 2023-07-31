# 实现方式
用Python实现一个简单的版本。代码中主要做了如下事情：
- 用户输入欲转账的比特币数量
- 使用MD5算法对种子(seed)进行哈希加密，生成证明(proof)和加密余额(encrypted_balance)
- 对余额(BALANCE)进行哈希加密，生成验证余额(verified_balance)
- 利用证明(proof)和加密余额(encrypted_balance)计算得到验证余额(verified_balance)，并与加密余额(encrypted_balance)进行比较，以确定交易是否有效。

# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/Project6.png">
</div>
