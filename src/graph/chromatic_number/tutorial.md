# Chromatic Number

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 6.830 (littlepants), sum 33.015 (littlepants)。

- `main.cxx` max: 3.723 ms (-45.49%), sum: 24.400 ms (-26.10%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
DSATUR 每次选择已见颜色最多的未染色点，尝试可用颜色；贪心得到上界，最大团给出下界，两者相遇即可停止。

库：[chromatic.h](../../../include/toy/chromatic.h)。

## 实现与取舍

颜色集合和邻接集用机器字保存，递归只恢复改变过的邻居饱和度。无需遍历全部 2^n 子集。

## 正确性

小图枚举合法着色，验证最少颜色数。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
