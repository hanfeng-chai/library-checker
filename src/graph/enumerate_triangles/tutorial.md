# Enumerate Triangles

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 22.828 (jeoffrey0522), sum 137.857 (soryuusi0219)。

- `main.cxx` max: 18.285 ms (-19.90%), sum: 94.722 ms (-31.29%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
按 `(degree,vertex)` 把边由小到大定向，保证前向度数 O(sqrt m)。标记 u 的前向邻居权重，对每条 u→v 枚举 v 的前向邻居，聚合共同邻居的权重。最后乘以 u、v 权重。

库：[small_cycles.h](../../../include/toy/small_cycles.h)。

## 实现与取舍

先累计一批共同邻居再取模，使用四个独立累加器，降低串行加法依赖；不为每个三角形单独调用输出回调。

## 正确性

小图枚举所有三元组，直接计算权重乘积之和。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
