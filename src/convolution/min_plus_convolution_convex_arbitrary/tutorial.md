# 凸序列与任意序列的 min-plus 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 80.900 (MeIoN), sum 1514.293 (MeIoN)。

- `main.cxx` max: 45.949 ms (-43.20%), sum: 869.052 ms (-42.61%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
对输出下标 k，最小化 A[k-j]+B[j]。A 凸时，选择最左的最优 j 随 k 单调不减；分治只扫描相邻最优决策限定的区间。时间 O((N+M) log(N+M))，空间 O(N+M)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`min_plus_convolution.md`](../../../include/toy/min_plus_convolution.md)。

## 尝试过程与取舍

利用最左最优决策单调不减做分治，只扫描相邻最优位置限定的范围。统一接口处理空输入与负数，用小规模朴素卷积验证单调性方向和边界。首批完整比较双项领先。
