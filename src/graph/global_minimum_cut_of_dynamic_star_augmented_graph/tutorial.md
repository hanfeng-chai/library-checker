# Global Minimum Cut of Dynamic Star Augmented Graph

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 2993.282 (anon123), sum 31048.246 (t98slider)。

- `main.cxx` max: 2794.000 ms (-6.66%), sum: 26295.567 ms (-15.31%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
原图割函数的极小集合构成层状族。最小邻接序反复收缩最后两点，得到包含所需集合的二叉合并树。原边给出基础割权，额外星形边的修改变成叶到根路径加。

库：[apex_min_cut.h](../../../include/toy/apex_min_cut.h), [heavy_light.h](../../../include/toy/heavy_light.h), [range_add_min.h](../../../include/toy/range_add_min.h)。

## 实现与取舍

重链剖分复用已有的 RangeAddMin。构造时使用带位置索引的堆更新剩余邻接权，避免不断产生过期堆项。

## 正确性

小图枚举所有非空点集及其割权，对照每一次星形边更新。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
