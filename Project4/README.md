# 实现方式
利用SIMD优化SM3，按照ppt中所讲进行优化，测试100000次哈希所用时间

<center>
| 未优化SM3 | SIMD优化SM3 | 优化百分比 |
|:--|:--|:--|
| 1990.45ms | 1350.44ms | 32% |
</center>


主要优化点集中在:

- __SIMD指令__:利用SIMD指令的并行计算能力
- __循环优化__:对循环进行unroll和其他优化
- __状态更新__:利用SIMD指令一次性推送/存储多个状态
- __填充优化__:缓存填充块,减少不必要的填充计算
- __多核并行__:利用多核 CPU 加速
- __其他细节__:减少分支、提高内存效率等
## 代码分析
1. 数据类型
定义sm3_ctx_simd为SIMD上下文的数据类型,包含:
- digest: 存储中间状态
- block:存储缓冲区,用于填充
- nblocks:当前处理的块数
- num:当前处理的字节数
2. 主要函数
- sm3_init_simd:初始化SIMD上下文
- sm3_update_simd:更新SIMD上下文,处理输入数据
- sm3_final_simd:最后计算哈希值,输出digest
- sm3_compress_simd:压缩函数,处理一个块的数据
3. SIMD优化
使用SIMD指令优化sm3_compress_simd函数:
- 使用_mm256_loadu_epi32加载4个W[]的值到128位寄存器
- 使用_mm256_xor_si256异或两个128位寄存器,计算W1[]
- 使用_mm256_storeu_epi32存回W1[]
4. 其他函数
- sm3_hash_simd:直接计算整个消息的哈希值
- sm3_hash_verify_simd:验证哈希值







# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/optimize_SM3.png">
</div>

