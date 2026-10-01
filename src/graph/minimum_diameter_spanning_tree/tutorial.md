# Minimum Diameter Spanning Tree

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 3.974 (nandhagk), sum 57.822 (nandhagk)。

- `main.cxx` max: 3.204 ms (-19.38%), sum: 49.028 ms (-15.21%)
- `naive.cxx` max: 378.983 ms (+9436.29%), sum: 1826.056 ms (+3058.05%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
最优生成树可由图的绝对中心构造，中心允许在边内部。最远点约束逐步给出每条边上半径的下界；对候选中心做精确最短路，达到下界即得到最优性证书。

库：[minimum_diameter_tree.h](../../../include/toy/minimum_diameter_tree.h)。

## 实现与取舍

`naive.cxx` 保留全点对中心搜索。主解最多加入 16 个最远点约束，仍不能证明最优时转入剩余边的完整搜索；缩小包络工作区也降低了小图的初始化开销。

## 正确性

小图枚举全部生成树及其直径，验证最优值、边集和零权边情形。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
