# K-Shortest Walk

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 111.901 (Rohan_Kapri), sum 648.555 (Rohan_Kapri)。

- `main.cxx` max: 93.956 ms (-16.04%), sum: 640.697 ms (-1.21%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
Eppstein 将一次偏离最短路树的费用增量放进持久化左偏堆。每次展开可以替换最后一次偏离，或在其终点追加下一次偏离，从而按长度枚举游走。

库：[k_shortest_walk.h](../../../include/toy/k_shortest_walk.h), [radix_heap.h](../../../include/toy/radix_heap.h)。

## 实现与取舍

先按源点收集连续记录，再线性建局部堆，替代逐边插堆。增量窗口允许时使用 32 位循环桶键；出度至多一时直接分析轨道和周期，避免建立没有必要的持久化堆。

## 正确性

与允许每点弹出前 k 次的优先队列搜索对照，覆盖零权环、平行边、不可达和 s=t。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
