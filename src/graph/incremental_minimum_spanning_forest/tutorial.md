# Incremental Minimum Spanning Forest

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 341.737 (nandhagk), sum 2349.660 (nandhagk)。

- `main.cxx` max: 290.091 ms (-15.11%), sum: 2128.841 ms (-9.40%)
- `naive.cxx` max: 2665.153 ms (+679.89%), sum: 17080.105 ms (+626.92%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
新增边若连接两个分量就保留；否则比较现有路径最大边，较轻时替换它。主解使用 AM-tree 的变换森林维护瓶颈标签。

库：[am_tree.h](../../../include/toy/am_tree.h), [incremental_mst.h](../../../include/toy/incremental_mst.h)。

## 实现与取舍

`naive.cxx` 保留经典 link-cut tree，活跃边节点反复复用。AM-tree 用大小校准、短路径探查和 Stitch／Perch 减少旋转；连通后的森林最大边权只降不升，可用周期性上界直接拒绝更重的边。

## 正确性

每个随机插入前缀重跑 Kruskal，逐步核对丢弃边号，另与 link-cut tree 对照。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
