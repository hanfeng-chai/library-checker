# Biconnected Components

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 186.110 (Rohan_Kapri), sum 1695.931 (Kiffaz11)。

- `main.cxx` max: 157.137 ms (-15.57%), sum: 1336.937 ms (-21.17%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
DFS 维护 low-link 和活动顶点栈。若子树最低回边不能越过父点，则从栈中弹出一个点双连通块，并把父点加入其中。割点可同时属于多个块，孤立点也要输出。

库：[biconnected.h](../../../include/toy/biconnected.h)。

## 实现与取舍

用显式栈处理深图，按边号跳过父边以支持平行边。所有组集中存储，避免每个块单独分配。

## 正确性

小图枚举顶点子集，检查删去任意一个点后仍连通的极大块，再与分解结果对照。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
