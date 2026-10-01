# Chromatic Polynomial

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 395.590 (sansen), sum 2340.765 (barty)。

- `main.cxx` max: 145.887 ms (-63.12%), sum: 1240.342 ms (-47.01%)
- `naive.cxx` max: 144.655 ms (-63.43%), sum: 1552.769 ms (-33.66%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
独立集的集合幂给出顶点分拆数。固定一个点所属的颜色类，可把投影规模降到 n-1 个变量；再将下降阶乘基转换成普通幂系数。

库：[chromatic.h](../../../include/toy/chromatic.h), [set_series.h](../../../include/toy/set_series.h)。

## 实现与取舍

`naive.cxx` 保留完整集合幂级数路径。主解先拆连通分量，并尝试单纯点消除；可完全消除的图直接乘线性因子。一般图仍走已有的集合幂级数库，复用了上一阶段的优化。

## 正确性

对小图逐色数枚举着色，核对多项式在 0..n 处的值；另检查主解与集合幂级数基线一致。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
