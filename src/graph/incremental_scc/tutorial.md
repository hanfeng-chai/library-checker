# Strongly Connected Components (Incremental)

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 440.260 (Rohan_Kapri), sum 3116.795 (Rohan_Kapri)。

- `main.cxx` max: 414.548 ms (-5.84%), sum: 2859.813 ms (-8.25%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
离线按插入时间分治。中点已在一个 SCC 内的边去左半；其他边在中点 SCC 上缩点后去右半。得到合并时刻后，并查集维护各分量的权重和，合并贡献为两组权重和之积。

库：[incremental_scc.h](../../../include/toy/incremental_scc.h), [packed_digraph.h](../../../include/toy/packed_digraph.h)。

## 实现与取舍

完整图先排除永不参与合并的跨 SCC 边。只需编号的调用不构造拓扑分组；分治进入子问题前释放中点图工作区，避免递归保留大块临时存储。

## 正确性

每个小图插入前缀用传递闭包重新计算 SCC 和答案，包含自环、重边与无变化插入。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。

进一步将端点就地改成紧凑编号，并复用触及列表存储 SCC 编号，减少建图和划分时的两级间接访问。
