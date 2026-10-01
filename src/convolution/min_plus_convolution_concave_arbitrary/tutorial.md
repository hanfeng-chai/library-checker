# 凹序列与任意序列的 min-plus 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 296.521 (soryuusi0219), sum 5017.299 (Rohan_Kapri)。

- `main.cxx` max: 259.782 ms (-12.39%), sum: 1082.841 ms (-78.42%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
把 B 分成长度不超过 N 的块，每块分为左右两个三角区域。平移凹序列之间的差单调，用带失效位置的栈维护最小值包络；二分确定新候选的有效前缀。时间 O((N+M) log N)，空间 O(N+M)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`min_plus_convolution.md`](../../../include/toy/min_plus_convolution.md)。

## 尝试过程与取舍

凹序列不能直接套凸序列的单调决策分治。初版改为分块维护平移函数的下包络，每块拆成两个三角区域，右半反向访问而不复制。后续加入插入时已不优就提前丢弃的判断，并比较 branchless 候选；保留通用的包络算法。
