# Directed MST

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 39.400 (Rohan_Kapri), sum 341.110 (wery0)。

- `main.cxx` max: 33.620 ms (-14.67%), sum: 280.060 ms (-17.90%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
每个非根分量选择最便宜的入边，形成有向环时收缩。懒标记斜堆统一扣除已选费用；回滚并查集保留缩环历史，逆序展开恢复每个原顶点的父边。

库：[directed_mst.h](../../../include/toy/directed_mst.h)。

## 实现与取舍

将堆节点、所选边和收缩记录连续存储；自环直接忽略。费用用 i64，输出原顶点父亲。

## 正确性

小图枚举每个非根点的入边组合，检查根可达性和最优费用。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
