# 大规模卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 1081.079 (Aiyiyi), sum 22929.496 (Aiyiyi)。

- `main.cxx` max: 1452.134 ms (+34.32%), sum: 29868.939 ms (+30.26%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
沿用普通模卷积内核。部分 NTT 在八项多项式处停止，因此长度 N 只需要 N/8 阶单位根，可覆盖本题超出完整 NTT 根容量的数据。

接口、约束及验证见 [`convolution.md`](../../../include/toy/convolution.md)。

## 尝试过程与取舍

本题超过普通完整 NTT 的根容量，采用在八项多项式处停止的部分变换，只需要 N/8 阶单位根。复用普通模卷积的缓存和八点内核，避免另写一套仅服务大规模题目的算法。完整对照中两项汇总均领先。
