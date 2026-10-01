# Z Algorithm

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 10.657 (Anonymous), sum 168.374 (sortA0329)。

- `main.cxx` max: 6.656 ms (-37.55%), sum: 113.571 ms (-32.55%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
用线性 Z 算法维护与前缀相同的最右区间，区间外的扩展调用有界 AVX2 LCP。
每个字符至多使最右边界前进一次；无需额外哨兵或复制字符串。
输出是大量不超过六位的整数，使用 `write_bulk6` 批量格式化。

算法与接口见 [string_basic.md](../../../include/toy/string_basic.md)。
