# Three-Edge-Connected Components

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 65.978 (Rohan_Kapri), sum 413.108 (Kiffaz11)。

- `main.cxx` max: 61.739 ms (-6.43%), sum: 368.726 ms (-10.74%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
无向 DFS 保存尚未吸收的路径和回边计数。遇到能够绕开两条割边的连接时，沿活动路径把顶点并入同一个三边连通分量；分组最终由并查集给出。自环不影响分组，重边按原边号处理。

库：[three_edge_components.h](../../../include/toy/three_edge_components.h)。

## 实现与取舍

图遍历改为显式 DFS 帧，避免长链的调用栈压力；保留 pre/post 区间来判断一个点是否属于尚未吸收的子树。分组采用连续数组输出，不为每个组分配容器。

## 正确性

小图枚举删除零条、一条、两条边后的连通关系，验证任意一对点是否始终连通。官方数据和随机多重图检查通过，相关对照经过 ASan/UBSan。
