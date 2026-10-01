# Cycle Detection (Undirected)

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 147.078 (Kiffaz11), sum 948.163 (Kiffaz11)。

- `main.cxx` max: 108.119 ms (-26.49%), sum: 471.752 ms (-50.25%)
- `naive.cxx` max: 114.795 ms (-21.95%), sum: 778.885 ms (-17.85%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
顺序加入边并维护真实生成森林；并查集首次发现两端已经连通时，该边和森林路径构成环。小分量换根可摊为 O(n log n)，找到环后停止读入余下边。

库：[forest_cycle.h](../../../include/toy/forest_cycle.h), [graph_cycle.h](../../../include/toy/graph_cycle.h)。

## 实现与取舍

`naive.cxx` 是完整建图后的显式 DFS。早停对很早出现环的大图有明显收益；主解仍保存真实父边，输出原边号，不能用压缩后的并查集父链代替路径。

## 正确性

独立并查集判断首次闭环位置，检查顶点、边号见证；自环和双重边单独覆盖。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
