# 前驱与后继

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 34.657 (nandhagk), sum 332.835 (nandhagk)。

- `main.cxx` max: 32.600 ms (-5.93%), sum: 316.678 ms (-4.85%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解使用 [BitSet](../../../include/toy/bitset.md) 的分层位图：高层标记低层非空字，
当前字内无答案时向上寻找邻近的非空字，再向下定位。
集合不超过 16 个元素时缓存有序键，用 SIMD 求秩；修改后再刷新。
主解合并前驱/后继的查询方向，较大集合的字内查询也采用无分支选择。

GCC/Clang 官方、随机边界对照及 sanitizer 通过。

## 尝试过程与取舍

从分层位图开始，逐步比较有界读数、批处理、固定布局和自适应路径。小集合内的查询可复用一次整理，修改后再刷新；大集合继续按非空字索引搜索。最后合并前驱/后继方向及无分支字内选择，按实际集合规模选择策略。
