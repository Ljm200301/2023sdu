# 实现方式
代码主要部分包括:

- EC 类表示椭圆曲线
- Ehash() 函数根据消息和椭圆曲线点生成确定性哈希值
- ESchnorr_sign() 生成Schnorr签名
- ESchnorr_verify() 验证Schnorr签名
- Emsg_signing() 演示签名消息
- ESchnorr_batch_verify() 批量验证多个签名
- Egenerate_coprime_int() 生成与给定素数互素的整数
- Etest_Schnorr_batch_verify() 测试批量验证
批量验证的优点:
- 更快速:验证n个签名分别需要n次哈希和n次曲线点加法。批量验证只需要2次哈希和2次点加法
- 带宽更低:不需要发送n个签名,只需发送一个组合签名

# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SchnorrBacth.png">
</div>
