# Cycle Detection (Directed)

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 23.754 (learningstud), sum 410.021 (learningstud)。

- `main.cxx` max: 23.512 ms (-1.02%), sum: 374.576 ms (-8.64%)
- `naive.cxx` max: 87.239 ms (+267.27%), sum: 695.021 ms (+69.51%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
DFS 遇到当前路径上的顶点即可恢复一个有向环。主解的顶点状态直接保存栈位置，边号从相应栈区间顺序取出；无需重新沿图找环，也无需反转结果。输入记录和遍历记录均为紧凑 u64。

库：[packed_digraph.h](../../../include/toy/packed_digraph.h), [graph_cycle.h](../../../include/toy/graph_cycle.h)。

## 实现与取舍

`naive.cxx` 保留普通 CSR DFS。曾尝试对出度至多一的图再次沿环恢复，但额外的随机游走使长环变慢，因此主解统一由顺序栈恢复。输出协议要求每个边号单独一行。

## 正确性

以传递闭包判断是否有环，并检查返回边顺序、闭合性和无重复顶点；覆盖自环、平行边。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
