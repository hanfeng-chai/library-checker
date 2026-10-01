# Counting Spanning Trees (Directed)

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 33.309 (toomer), sum 157.500 (toomer)。

- `main.cxx` max: 25.925 ms (-22.17%), sum: 114.108 ms (-27.55%)
- `naive.cxx` max: 130.229 ms (+290.97%), sum: 444.738 ms (+182.37%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
对入度 Laplacian 删除根对应行列，行列式就是从根向外的有向生成树数。自环抵消，重边累加。

库：[spanning_count.h](../../../include/toy/spanning_count.h), [determinant.h](../../../include/toy/determinant.h)。

## 实现与取舍

主解使用延迟 u64 归约的 AVX2 高斯消元，成对处理主元和目标行；`naive.cxx` 保留每次乘减后取模的标准消元。两个版本构造同一个余子式并使用相同读入。

## 正确性

正式数据；行列式另与排列展开及独立逐项取模消元对照，覆盖奇异矩阵和换行主元。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
