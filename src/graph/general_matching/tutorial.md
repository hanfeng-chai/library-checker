# Matching on General Graph

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 3.267 (nor), sum 14.183 (nor)。

- `main.cxx` max: 3.011 ms (-7.85%), sum: 13.654 ms (-3.73%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先贪心匹配，再寻找交替增广路。两个外点之间的边形成奇环时，把交替树路径收缩成花；增广时通过前驱链接恢复原匹配。

库：[general_matching.h](../../../include/toy/general_matching.h)。

## 实现与取舍

使用连续 CSR、队列和花编号数组，避免每次搜索动态构造容器。

## 正确性

子集 DP 独立求小图最大匹配，并检查所有输出边存在且端点不重复。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
