# 带上界的区间个数与和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 130.751 (nandhagk), sum 936.723 (nandhagk)。

- `main.cxx` max: 121.041 ms (-7.43%), sum: 892.866 ms (-4.68%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 range_sum_bound.h。把数组项按值、查询按上界排序，扫描查询时
插入所有 value≤bound 的数组项，然后在原下标 [l,r) 上查询个数与和。
查询时恰好插入满足上界的项，因此筛选值域与下标区间两个条件都成立。

count_sum.h 的四层十六叉局部前缀树将次数与总和打包进 u64，按 65536 个位置
分块，整块部分另用 Fenwick 汇总。AVX2 更新局部后缀，预取后续插入位置，
独立分配的大页友好存储减少随机访存的地址转换成本。

复杂度 O((N+Q)log N)，空间 O(N+Q)。支持完整 u32 值，包含最大值和块界的
朴素对照、GCC/Clang 官方及 sanitizer 均通过。

## 尝试过程与取舍

先按值和查询上界排序，以位置区间的个数、和回答。之后比较基数排序、SIMD、循环展开、预取与大页友好存储，将局部计数和总和打包到宽前缀树，整块另作汇总。检查最大 u32 值与块界后再选择最终组合。
