# Chordal Graph Recognition

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 76.503 (Nachia), sum 943.346 (Nachia)。

- `main.cxx` max: 71.795 ms (-6.15%), sum: 787.818 ms (-16.49%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先剥掉度为零或一的单纯点；剩下全为环时直接判断。一般 2-core 用最大势搜索生成完美消除序候选，按最早后继邻居分组验证后继形成团。失败时恢复诱导环。

库：[chordal.h](../../../include/toy/chordal.h)。

## 实现与取舍

完整图 MCS 在长环和附有大量树枝的图上做了多余工作。剥叶及环组件路径避免了这些状态访问，核心算法与识别条件保持精确。

## 正确性

小图枚举诱导环判断弦图，并验证返回消除序或无弦环见证。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
