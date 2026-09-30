# 凹序列与任意序列的 min-plus 卷积

把 B 分成长度不超过 N 的块，每块分为左右两个三角区域。平移凹序列之间的差单调，用带失效位置的栈维护最小值包络；二分确定新候选的有效前缀。时间 O((N+M) log N)，空间 O(N+M)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`min_plus_convolution.md`](../../../docs/min_plus_convolution.md)。
