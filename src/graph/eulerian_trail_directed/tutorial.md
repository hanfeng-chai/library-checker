# Eulerian Trail (Directed)

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 37.586 (Rohan_Kapri), sum 468.580 (Rohan_Kapri)。

- `main.cxx` max: 31.271 ms (-16.80%), sum: 345.684 ms (-26.23%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先检查入出度差，再按度数选择起点。Hierholzer 消费出边并倒序记录回退路径，最后要求消费边数恰好为 m，以排除不连通的有效边集。

库：[euler_trail.h](../../../include/toy/euler_trail.h)。

## 实现与取舍

使用 CSR、游标和连续栈；空边集和单边无需完整工作区。

## 正确性

验证度数及有效边集连通性，检查输出每条原边恰好出现一次。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
