# Lyndon Factorization

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 5.764 (nandhagk), sum 38.366 (nandhagk)。

- `main.cxx` max: 4.245 ms (-26.35%), sum: 33.749 ms (-12.04%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
Duval 分解。相等字符段用 AVX2 LCP 整段跳过，分解边界直接写入连续数组。
长周期输入因此减少了逐字符循环；大量边界使用 `write_bulk6` 输出。
时间 O(n)，输出空间 O(n)，保留标准 Duval 的分解顺序。

接口见 [string_basic.md](../../../include/toy/string_basic.md)。
