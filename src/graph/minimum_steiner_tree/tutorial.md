# Minimum Steiner Tree

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 52.912 (Anonymous), sum 409.415 (Anonymous)。

- `main.cxx` max: 22.314 ms (-57.83%), sum: 178.123 ms (-56.49%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
固定最后一个终端为根，对其余 k-1 个终端做子集 DP。先合并两个非空子集，再通过全点对最短距离移动根。最优见证沿严格下降的费用或最优子集拆分恢复。

库：[steiner_tree.h](../../../include/toy/steiner_tree.h)。

## 实现与取舍

去掉根终端的一维把状态数减半。n≤100 时连续矩阵传播比为每个状态构造堆更直接，合并过程保持顺序访存。

## 正确性

小图枚举所有边子集，验证最优费用及返回树连接全部终端。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
