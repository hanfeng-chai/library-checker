# Two-Edge-Connected Components

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 51.387 (oldyan), sum 323.005 (oldyan)。

- `main.cxx` max: 43.784 ms (-14.80%), sum: 279.637 ms (-13.43%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
无向 DFS 按边号跳过父边，维护 low-link；桥的两侧分入不同组，组内任意单边删除不破坏连通性。显式 DFS 帧与连续分组使深图不依赖调用栈。

库：[scc.h](../../../include/toy/scc.h)。

## 实现与取舍

复用 low-link 遍历骨架，不创建桥树再遍历一次。

## 正确性

逐条删除小图中的边，检查任意两点的连通关系是否始终不变。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
