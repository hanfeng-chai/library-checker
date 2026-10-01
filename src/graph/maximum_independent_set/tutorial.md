# Maximum Independent Set

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 1.167 (Rohan_Kapri), sum 17.400 (isaunoya)。

- `main.cxx` max: 0.864 ms (-25.95%), sum: 13.337 ms (-23.35%)
- `naive.cxx` max: 7.695 ms (+559.26%), sum: 67.255 ms (+286.53%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
最大独立集等于补图的最大团。搜索当前候选集，用贪心染色的颜色数限制还能加入多少点，再从较高颜色开始分支。

库：[clique.h](../../../include/toy/clique.h)。

## 实现与取舍

`naive.cxx` 保留折半子集 DP。主解只用一个 u64 表示顶点集，避免递归中动态分配候选容器。

## 正确性

小图枚举全部子集，对照最大值和返回集合的独立性。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
