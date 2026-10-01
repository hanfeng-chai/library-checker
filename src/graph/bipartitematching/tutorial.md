# Matching on Bipartite Graph

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 141.078 (Rohan_Kapri), sum 2039.739 (Rohan_Kapri)。

- `main.cxx` max: 138.314 ms (-1.96%), sum: 1870.765 ms (-8.28%)
- `naive.cxx` max: 2029.078 ms (+1338.27%), sum: 18097.391 ms (+787.24%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解使用 push-relabel 匹配：自由右点抢占距离最小的左邻居，被替换的右点重新入队；周期性从自由左点重建全局距离。

库：[bipartite_matching.h](../../../include/toy/bipartite_matching.h)。

## 实现与取舍

`naive.cxx` 是显式增广栈的 Hopcroft–Karp。它在部分构造数据上需要很多分层增广，首次主解 max 达到约 2 秒；push-relabel 将该批最大值降到约 138 ms。保留经典版本用于算法对照。

## 正确性

小二分图以右侧掩码 DP 求最大匹配，检查双向 mate 一致及边存在。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
