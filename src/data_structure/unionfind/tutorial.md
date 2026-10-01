# 并查集

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 12.395 (sortA0329), sum 118.740 (highlighter_math)。

- `main.cxx` max: 11.315 ms (-8.71%), sum: 113.736 ms (-4.21%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解复用 [DSU](../../../include/toy/dsu.md)：按大小合并、路径折半。
每批读取 128 个操作并预取两个端点的父数组，仍按原顺序执行，再批量输出判断结果。
GCC/Clang 官方与独立朴素对照、sanitizer 通过。

## 尝试过程与取舍

经典按大小合并和路径折半保持不变，后续把输入分成 128 项一批并预取两个端点的父数组。操作仍按原顺序执行，查询答案批量输出，收益来自访存与 I/O 安排。
