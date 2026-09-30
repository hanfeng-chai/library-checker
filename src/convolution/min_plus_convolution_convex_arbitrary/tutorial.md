# 凸序列与任意序列的 min-plus 卷积

对输出下标 k，最小化 A[k-j]+B[j]。A 凸时，选择最左的最优 j 随 k 单调不减；分治只扫描相邻最优决策限定的区间。时间 O((N+M) log(N+M))，空间 O(N+M)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`min_plus_convolution.md`](../../../docs/min_plus_convolution.md)。
