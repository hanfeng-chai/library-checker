# Counting Eulerian Circuits

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 26.135 (toomer), sum 219.636 (toomer)。

- `main.cxx` max: 25.830 ms (-1.17%), sum: 155.984 ms (-28.98%)
- `naive.cxx` max: 129.038 ms (+393.74%), sum: 586.443 ms (+167.01%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
去掉孤立点后检查入出度平衡与有效边集连通性。BEST 定理将答案写成有向生成树数乘各点 `(outdegree-1)!`；边带标签，起始边固定。

库：[spanning_count.h](../../../include/toy/spanning_count.h), [determinant.h](../../../include/toy/determinant.h)。

## 实现与取舍

`naive.cxx` 使用标准消元；主解复用成对主元、成对目标行的 u64 延迟归约。单个有效顶点和自环自然落入空余子式。

## 正确性

正式数据及矩阵内核的独立对照，包含失衡、非连通、孤立点和只有自环的情形。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
