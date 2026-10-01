# Edge Coloring of Bipartite Graph

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 155.318 (Nachia), sum 1715.489 (Rohan_Kapri)。

- `main.cxx` max: 109.701 ms (-29.37%), sum: 1079.201 ms (-37.09%)
- `naive.cxx` max: 351.662 ms (+126.41%), sum: 1341.292 ms (-21.81%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
将同侧低度顶点合并并补成正则二分图。偶数度沿欧拉回路交替拆分；奇数度先删去一个完美匹配，再递归处理剩余边，最终恰好使用最大度种颜色。

库：[bipartite_coloring.h](../../../include/toy/bipartite_coloring.h)。

## 实现与取舍

主解使用 Hopcroft–Karp 找完美匹配，`naive.cxx` 保留固定种子的随机交替游走版本。实际对照中，确定性版本 max 约 110 ms，随机游走版本在 many_smalls 上约 352 ms，因此采用前者；它们的已测记录通过 binary SHA-256 直接换名复用。

## 正确性

随机二分重图检查每点每种颜色最多出现一次，颜色数恰好等于最大度。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
