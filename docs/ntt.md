# ntt

`NTT<P> plan(capacity)` 预计算正、逆变换的根表；capacity 是二次幂。
P 必须是小于 2^30 的奇素数，容量不超过 P-1 的最大二次幂因子。

`plan.forward(span)` 将普通余数转为 bit-reversed 顺序的频点；
`plan.inverse(span, scale=1)` 还原并归一化，可额外乘普通余数 scale。
实际长度须为不超过 capacity 的二次幂。较短变换所用的单位根恰好对应较长变换的前缀，
因此将多项式按较短周期折叠，等价于取相应频点前缀。
每次变换 O(N log N)，根表空间 O(capacity)。

大蝶形用 AVX2 同时处理八个系数，最小三个维度用寄存器内 shuffle 完成。
普通卷积继续使用 `convolution.h` 的部分变换和多项式叶子；本接口用于需要
独立访问标量频点、重用根表的算法。

已通过 GCC/Clang 下四个模数的朴素 DFT、不同长度往返及 ASan/UBSan 检查。

可用第二个构造参数指定生成元 g；0（默认值）自动寻找原根。
自选 g 应使其二次幂部分具有所需阶数。[radix_ntt](radix_ntt.md) 用此接口
与部分变换选择相同的单位根。
