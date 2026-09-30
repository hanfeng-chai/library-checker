# 两个凸序列的 min-plus 卷积

凸序列的相邻差分有序。每次比较两个下一步的和，选择较小的增量，相当于合并两个差分序列。时间、空间均 O(N+M)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`min_plus_convolution.md`](../../../docs/min_plus_convolution.md)。
