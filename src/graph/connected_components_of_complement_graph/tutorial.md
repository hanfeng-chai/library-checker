# Connected Components of Complement Graph

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 55.547 (wery0), sum 738.471 (wery0)。

- `main.cxx` max: 40.177 ms (-27.67%), sum: 476.465 ms (-35.48%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
在补图上 BFS，但不显式构造补边。对每个出队点标记它在原图中的邻居，扫描未发现点数组，把未被标记者移入当前分量。每次保留检查对应一条原边，移除检查每点只有一次。

库：[complement_components.h](../../../include/toy/complement_components.h)。

## 实现与取舍

连续数组压缩取代树集合删除，既保留 O(n+m) 的摊还界，也减少指针追踪和分配。

## 正确性

小图显式构造补图并做传递闭包，对比分组。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
