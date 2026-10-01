# Eulerian Trail (Undirected)

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 54.138 (Rohan_Kapri), sum 621.515 (Rohan_Kapri)。

- `main.cxx` max: 45.653 ms (-15.67%), sum: 477.563 ms (-23.16%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
奇度点必须为零个或两个。Hierholzer 中两份邻接记录共享原边号，用标记保证一条无向边只消费一次；回退时同时恢复顶点与边序列。

库：[euler_trail.h](../../../include/toy/euler_trail.h)。

## 实现与取舍

与有向版共享主体；无向模式只增加按边号的消费标记，支持自环和重边。

## 正确性

随机重图检查存在性及完整轨迹，覆盖空图、单边、自环、非连通有效边集。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
