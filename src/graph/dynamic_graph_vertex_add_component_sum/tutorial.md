# Dynamic Graph Vertex Add Component Sum

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 127.235 (nandhagk), sum 711.666 (nandhagk)。

- `main.cxx` max: 103.979 ms (-18.28%), sum: 589.059 ms (-17.23%)
- `naive.cxx` max: 277.785 ms (+118.32%), sum: 2173.599 ms (+205.42%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
预处理每次插边的删除时刻，维护按到期时间取最大瓶颈的生成森林。删除时当前标签全局最早，不可能有仍活跃的非树替代边，因此只需切开该树边并维护分量和。

库：[expiry_forest.h](../../../include/toy/expiry_forest.h), [dynamic_component_sum.h](../../../include/toy/dynamic_component_sum.h)。

## 实现与取舍

`naive.cxx` 保留时间线段树加回滚并查集。主解扩展 AM-tree，在变换中维护子树和与到期标签位置；没有删除时直接用普通并查集。此带删除扩展不直接宣称纯增量 AM-tree 的摊还界。

## 正确性

随机合法加删边和加值序列，用每次查询重新 BFS／并查集构造分量作对照。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
