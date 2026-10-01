# Strongly Connected Components

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 53.082 (learningstud), sum 422.669 (learningstud)。

- `main.cxx` max: 51.374 ms (-3.22%), sum: 412.247 ms (-2.47%)
- `naive.cxx` max: 83.375 ms (+57.07%), sum: 676.285 ms (+60.00%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
Tarjan 在 DFS 中维护低链接；完成一个根时弹出其整个强连通分量。主解把首条出边缓存到顶点中，并将消费过的边槽复用为返回栈、活动链。反转弹出顺序即可按拓扑序输出。

库：[packed_digraph.h](../../../include/toy/packed_digraph.h), [scc.h](../../../include/toy/scc.h)。

## 实现与取舍

`naive.cxx` 保留 CSR 和显式 DFS 帧的经典 Tarjan。最初这个版本慢于 learningstud，主要多在邻接读取和状态存储上。紧凑记录、批量成对读入和建图预取减少了这些开销；没有按生成器或测例名选路径。

## 正确性

随机有向图与 Floyd 互达关系对照，另检查编号拓扑序。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
