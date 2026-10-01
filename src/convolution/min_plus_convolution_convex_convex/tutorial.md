# 两个凸序列的 min-plus 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 32.371 (Rohan_Kapri), sum 521.805 (urectanc)。

- `main.cxx` max: 28.657 ms (-11.47%), sum: 452.230 ms (-13.33%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
凸序列的相邻差分有序。每次比较两个下一步的和，选择较小的增量，相当于合并两个差分序列。时间、空间均 O(N+M)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`min_plus_convolution.md`](../../../include/toy/min_plus_convolution.md)。

## 尝试过程与取舍

把两个凸序列的差分看成有序序列，直接合并下一步增量。这里线性算法比引入通用卷积或分治结构更合适；在朴素对照通过后，首批完整测量即达到目标。
