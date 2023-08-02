# 实现方式
该代码包含三个函数：`trusted_issuer()`、`alice()` 和 `bob()`。

在 `trusted_issuer()` 函数中，首先生成一个128位的随机种子 `seed`。接着计算出一个哈希值 `s`，作为 `seed` 的散列值。然后使用 RSA 算法生成一个2048位的私钥 `private_key` 和相应的公钥 `public_key`。接下来，使用哈希函数将 `s` 迭代哈希 `k` 次，得到哈希值 `c`。最后，使用私钥对 `c` 进行数字签名，生成签名值 `sig_c`。最终，该函数返回 `s`、`sig_c` 和 `public_key`。

在 `alice()` 函数中，首先计算出 `d_0=2000-1978`，然后使用哈希函数将 `s` 迭代哈希 `d_0` 次，得到哈希值 `p`。最终，该函数返回 `p`。

在 `bob()` 函数中，首先计算出 `d_1=2100-2000`，然后使用哈希函数将 `p` 迭代哈希 `d_1` 次，得到哈希值 `c_prime`。接下来，使用公钥对 `sig_c` 和 `c_prime` 进行验证。如果验证成功，即 Alice 的证明是有效的，则输出 "Alice's proof is valid."；否则输出 "Alice's proof is invalid."。

验证 Alice 的证明

要验证 Alice 的证明，首先需要获取 `trusted_issuer()` 函数返回的 `s`、`sig_c` 和 `public_key`。然后，需要调用 `alice()` 函数，并将 `s` 作为输入参数，计算出 `p`。最后，需要调用 `bob()` 函数，并将 `p`、`sig_c` 和 `public_key` 作为输入参数，进行验证。

验证过程中，`bob()` 函数首先使用哈希函数将 `p` 迭代哈希 `d_1` 次，得到哈希值 `c_prime`。然后，使用公钥对 `sig_c` 和 `c_prime` 进行验证。如果验证成功，即 Alice 的证明是有效的，则输出 "Alice's proof is valid."；否则输出 "Alice's proof is invalid."。

Alice (born 1978) wants to prove to Bob that her age $>$ 21. Rely on trusted issuer, proof system need to be useable until 2100. Suppose this year is 2021.

Trusted Issuer:

\begin{itemize}
\item Pick 128-bit random seed, compute $s=H_0(\text{seed})$
\item $k=2100-1978$, $c=H_1^k(s)$, sign over $c$ as $sig_c$
\end{itemize}

Give Alice $s$ and $sig_c$

Alice prove her age $\geq 21$ to Bob:
\begin{itemize}
\item E.g., she was born before 2000
\item Compute $d_0=2000-1978=22$, compute proof $p=H_1^{d0}(s)$
\item Give Bob $(p,sig_c)$
\end{itemize}

Bob verify Alice’s proof:
\begin{itemize}
\item Compute $d_1=2100-2000=100$
\item Compute $c' = H_1^{d1}(p)$, check $sig_c$ is for $c'$
\end{itemize}

# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/Project6.png">
</div>