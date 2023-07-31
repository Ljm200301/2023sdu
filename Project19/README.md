# 实现方式

伪造算法的目的是伪造一个和原始签名可以通过验证的伪造签名。

具体来说，伪造算法的伪造过程如下：

- 随机生成两个正整数 u 和 v。
- 计算 $R_{forge} = u G + v P$，其中 $G$ 是椭圆曲线上的基点，$P$ 是验证公钥，$u G$ 和 $v P$ 分别表示将基点 $G$ 和验证公钥 $P$ 分别乘以 $u$ 和 $v$ 后得到的结果，$+$ 表示椭圆曲线上的点加运算。
- 计算 $e_{forge} = r_{forge} \cdot u \cdot v^{-1} \bmod n$，其中 $n$ 是椭圆曲线上的模数，$r_{forge}$ 是 $R_{forge}$ 的 $x$ 坐标。
- 计算 $s_{forge} = r_{forge} \cdot v^{-1} \bmod n$。
- 如果通过验证，则返回伪造签名 $(r_{forge}, s_{forge})$。
- 这个算法的核心思想是，通过随机生成 $u$ 和 $v$，构造出一个新的点 $R_{forge}$，并根据 $R_{forge}$ 的 $x$ 坐标计算出伪造签名的 $r_{forge}$。然后，根据 $r_{forge}$ 和随机数 $v$ 计算出伪造签名的 $s_{forge}$，并根据 $r_{forge}$、 $s_{forge}$ 和随机数 $u$ 计算出伪造签名的 $e_{forge}$ 。最后，通过验证函数验证伪造签名的有效性。如果伪造签名通过验证，则说明伪造成功。
  
# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/Forge_Satoshi .png">
</div>
