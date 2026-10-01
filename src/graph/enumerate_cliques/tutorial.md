# Enumerate Cliques

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 1.217 (gandalfr), sum 24.466 (mnbvcxz123)。

- `main.cxx` max: 0.985 ms (-19.09%), sum: 19.263 ms (-21.27%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
按顶点次序枚举团：加入 v 后，将候选集与 N(v) 取交，并只保留尚未处理的顶点，因此每个非空团恰好访问一次。递归返回权重乘积和。

库：[clique.h](../../../include/toy/clique.h)。

## 实现与取舍

n≤100，用 u128 保存邻接与候选集；小候选集只需低位提取和按位与。

## 正确性

枚举小图的所有子集，验证团条件和模意义下的加权和。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
