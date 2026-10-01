# Assignment Problem

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 10.766 (toomer), sum 76.365 (toomer)。

- `main.cxx` max: 10.432 ms (-3.11%), sum: 72.506 ms (-5.05%)
- `naive.cxx` max: 24.581 ms (+128.32%), sum: 134.920 ms (+76.68%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
维护列势和匹配，使每次增广沿约化费用的最短路径进行。列约简和余量转移构造初始匹配，两轮行约简减少自由行；剩余搜索把等距离列集中处理。

库：[assignment.h](../../../include/toy/assignment.h)。

## 实现与取舍

`naive.cxx` 是逐列搜索的最短增广路。主解还验证 Monge／反 Monge 性质，成立时直接使用对角线匹配；验证来自实际矩阵，而非样例类型。

## 正确性

小矩阵用子集 DP 求最优费用，并检查列排列及输出费用。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
