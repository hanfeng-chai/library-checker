# Counting Spanning Trees (Undirected)

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 35.610 (toomer), sum 168.093 (toomer)。

- `main.cxx` max: 28.390 ms (-20.27%), sum: 126.191 ms (-24.93%)
- `naive.cxx` max: 132.553 ms (+272.24%), sum: 455.452 ms (+170.95%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
无向边向 Laplacian 加入两个方向，删除任意一个点的行列后求行列式。该余子式与选择哪个根无关。

库：[spanning_count.h](../../../include/toy/spanning_count.h), [determinant.h](../../../include/toy/determinant.h)。

## 实现与取舍

`naive.cxx` 保留标准高斯消元。主解复用有向计数的成对延迟归约内核，避免为两个题目维护两套矩阵代码。

## 正确性

正式数据及独立模行列式对照。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
