# 实现方式

在ECDSA算法中，数字签名的安全性基于椭圆曲线上的离散对数难题。一个好的椭圆曲线参数和随机数$k$的选择可以保证离散对数难题的安全性。但是，如果选择的参数和随机数存在漏洞，就会导致签名被伪造。

在代码中，伪造算法 $\text{Pretend}$ 模拟了一个攻击者伪造数字签名的过程。攻击者利用随机数$u$和$v$计算出一个伪造的签名值 $(r_{\text{forge}} , s_{\text{forge}})$ 。具体实现过程如下：

生成两个随机数 $u$ 和 $v$ ；
$$u, v \leftarrow \text{random}(1, n-1)$$

计算出伪造签名的 $r$ 值，即 $\text{Add}(\text{Multi}(u, G), \text{Multi}(v, P))[0]$ ；

$$ R_{\text{forge}} \leftarrow \text{Add}(\text{Multi}(u, G), \text{Multi}(v, P)) $$

$$ r_{\text{forge}} \leftarrow R_{\text{forge}}[0] $$

计算出与 $r$ 值对应的 $e$ 值，即 $(r_{\text{forge}} \cdot u \cdot \text{multi_inverse}(v, n)) \bmod n$；
$$ e_{\text{forge}} \leftarrow (r_{\text{forge}} \cdot u \cdot \text{multi_inverse}(v, n)) \bmod n $$

计算出与 $e$ 值对应的 $s$ 值，即 $(u \cdot \text{multi_inverse}(v, n)) \bmod n$；
$$s_{\text{forge}} \leftarrow (u \cdot \text{multi_inverse}(v, n)) \bmod n$$

调用 $\text{Verify}$ 函数验证伪造的签名值 $(r_{\text{forge}}, s_{\text{forge}})$ 是否合法。
$$\text{Verify}(r_{\text{forge}}, s_{\text{forge}}, e_{\text{forge}}, G, P)$$

- 如果伪造的签名值能够通过$\text{Verify}$函数的验证，则表明攻击者成功地伪造了数字签名。需要注意的是，伪造签名的成功率取决于椭圆曲线参数和随机数的选择。如果参数和随机数选择得当，则伪造签名的成功率非常低。因此，在实际应用中，需要选择安全性较高的椭圆曲线参数和随机数，并对签名值进行更严格的验证，以保证签名的安全性。
  
# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/Forge_Satoshi .png">
</div>
