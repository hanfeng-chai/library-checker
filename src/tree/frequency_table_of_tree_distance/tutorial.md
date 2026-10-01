# 树距离频数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 355.707 (andyli), sum 3048.551 (ssmkybb)。

- `main.cxx` max: 333.238 ms (-6.32%), sum: 2146.841 ms (-29.58%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 tree_distance_histogram.h。对每个点分重心，从只含重心的
距离直方图开始，按各支大小递增加入：新支与此前部分做一次卷积，即得到
经过重心、位于不同部分的无序点对。随后合并直方图，不计算再抵消同支点对。

小直方图直接相乘，较大者复用 IntegerFFT。小支在前，避免长直方图反复乘许多
小支；对于两支情形，也省掉分别平方三次的工作。时间 O(N log² N)，空间 O(N)。
20 个官方测例通过 GCC/Clang，BFS 全点对与大规模长链、扫帚树闭式频数的
独立对照通过 ASan/UBSan。

## 尝试过程与取舍

先在每个重心计算全体距离直方图的平方，再减去各支平方，最后除二。首版 sum 已优于参考但 max 未达标。改为从重心单点开始，按支大小递增加入，每次只卷积新支与已有部分，直接累计无序点对，省去同支点对的计算与抵消。小直方图直接相乘，大者复用 IntegerFFT。
