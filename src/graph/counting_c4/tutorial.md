# Counting $C _ 4$'s

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 1136.762 (Rohan_Kapri), sum 7060.145 (MeIoN)。

- `main.cxx` max: 323.404 ms (-71.55%), sum: 2499.884 ms (-64.59%)
- `naive.cxx` max: 1608.883 ms (+41.53%), sum: 8423.045 ms (+19.30%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
本题输出每条原边参与的四环数，且允许平行边。先把平行边压成权重，再按度数选唯一的最大顶点，汇总两步路径的权重乘积，并回填四条边的贡献。

库：[count_c4.h](../../../include/toy/count_c4.h)。

## 实现与取舍

`naive.cxx` 保留按优先级过滤邻接的经典实现。主解用可缩短的邻接区间消除内层比较；稠密简单图通过位交集计算 A²，再求逐边三步游走数并减去重复端点项。初稿误写成简单图总数，正式检查发现后已改正，错误版本没有参与计时。

## 正确性

随机多重图枚举四个互异顶点及边重数，逐边验证，而非只检查总数。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
