# multivariate_convolution

`multivariate_convolution(dimensions, std::move(a), std::move(b))`
计算模每个 `x_i^dimensions[i]` 的截断卷积。系数默认模 998244353，
最低维下标变化最快；数组长度等于维度乘积 N，至多 32 维，每维至少 2。
零维视为两个常数相乘。

把下标 i 染成 `sum floor(i / 各前缀维度乘积) mod K`。
两个下标相加产生的低位进位数在 0 到 K-1 之间，所以取回对应颜色的结果，
恰好排除越过各维上界的项。颜色按混合进制计数器递增，避免逐元素除法。

全部维度为 2 时，直接采用 `subset_convolution`；它在该情形更快。
其余情况每种颜色做 NTT，频点处按颜色循环卷积。八个频点一起处理，颜色数据留在 L1；
至多八个乘积先累加到 u64，再归约一次。乘法与归约的溢出边界见 `mod.h`。
一维直接复用普通卷积，小数据朴素计算。

时间 O(KN log N + K²N)，空间 O(KN)。较大的多维输入要求
`bit_ceil(2*N-1)` 不超过系数模数的完整 NTT 根容量。
17 个官方测例通过 GCC/Clang，66 项朴素对照与 ASan/UBSan 通过。

全二元分支复用最新的缩半 XOR 子集内核后，已重新通过 GCC/Clang 和静默评测。
当前 max/sum 及五份对照统一见 [评测审计](../../tools/measurement_audit.md)。
