# GCD 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 78.623 (oldyan), sum 738.934 (toomer)。

- `main.cxx` max: 63.477 ms (-19.26%), sum: 593.692 ms (-19.66%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
令 A[d] 为所有 d 的倍数上的系数和；对 A、B 点乘后在倍数偏序上反演。按素数筛遍历，时间 O(N log log N)，空间 O(N)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`divisor_convolution.md`](../../../include/toy/divisor_convolution.md)。

## 尝试过程与取舍

采用经典倍数偏序上的 zeta 变换与反演，按素数筛安排遍历，避免逐个枚举所有约数关系。整理为与 LCM 共用的接口，复用现有快速 I/O 和 Buffer；用朴素卷积对照确认变换方向，再做完整评测。
