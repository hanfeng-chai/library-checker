# General Weighted Matching

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 571.464 (Anonymous), sum 4008.714 (Anonymous)。

- `main.cxx` max: 415.377 ms (-27.31%), sum: 2731.437 ms (-31.86%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
带花树的原始-对偶算法维护紧边和交替森林。没有紧边可扩展时按最小 slack 调整对偶；花的对偶降到零时展开，找到增广路后翻转内部匹配。

库：[weighted_matching.h](../../../include/toy/weighted_matching.h)。

## 实现与取舍

紧凑边记录保存原始端点，避免展开时重新搜索原图。对偶更新仅扫描已进入森林的顶点、根及有 slack 的根，减少每轮对所有槽位的扫描。

## 正确性

小图用子集 DP 求最大权匹配；检查匹配对称、端点唯一及总权重，覆盖不完美匹配和重边。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
