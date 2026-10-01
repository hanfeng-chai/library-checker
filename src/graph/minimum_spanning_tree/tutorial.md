# Minimum Spanning Tree

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 49.671 (sigma), sum 671.105 (sigma)。

- `main.cxx` max: 48.659 ms (-2.04%), sum: 657.377 ms (-2.05%)
- `boruvka.cxx` max: 82.575 ms (+66.24%), sum: 965.961 ms (+43.94%)
- `prim.cxx` max: 267.103 ms (+437.74%), sum: 2662.653 ms (+296.76%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解按权重做四趟字节基数排序，再用并查集依次选边。调用方提供两块静态工作区；排序结束后，临时数组的存储重新用于并查集和答案，不再为这些阶段分别分配内存。树输入直接保留全部边。

库：[kruskal_workspace.h](../../../include/toy/kruskal_workspace.h)、[spanning_tree.h](../../../include/toy/spanning_tree.h)。`prim.cxx` 保留邻接表加二叉堆 Prim，`boruvka.cxx` 保留 Boruvka，并在稠密图使用矩阵 Prim。

## 实现与取舍

尝试过完整 Kruskal、剥叶后排序 2-core、压缩度二路径、交错剥叶及 Boruvka。缩减图的边数确实变少，但额外的节点随机访存抵消了收益，未作为最终主解保留。

最终回到更短的标准 Kruskal：合并统计字节直方图、固定排序趟数、复用工作区，配合有界批量读入。常数权重无需排序。采样进一步定位并查集端点读取，用预取覆盖首次访问延迟；主解的 GCC 编译启用循环展开。所有选择来自图或数值性质，不读取测例名称。

## 正确性

所有版本通过官方 checker。独立随机测试覆盖连通图、非连通森林、自环、平行边和零权，比较 Kruskal、Prim、Boruvka 的费用，检查所选边无环及费用一致；工作区生命周期切换通过 ASan/UBSan。
