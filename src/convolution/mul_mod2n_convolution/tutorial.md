# 模 2^N 下标的乘法卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 189.476 (Rohan_Kapri), sum 1314.736 (Rohan_Kapri)。

- `main.cxx` max: 97.921 ms (-48.32%), sum: 679.821 ms (-48.29%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
按照二进制末尾零数分层，以 -1 和 5 表示奇数部分，用 NTT 频点前缀合并不同层。

接口、约束及验证见 [`multiplicative_convolution.md`](../../../include/toy/multiplicative_convolution.md)。

## 尝试过程与取舍

先按末尾零数分层，奇数部分分解为 -1 与 5 的幂，跨层乘法只取相应频点前缀。这样复用已有 NTT 而无需逐层重新卷积。共享数学库整理后再次做完整比较，确认最终主解保持两项领先。
