# 静态区间不同值数量

记录每个位置的前次出现位置加一，查询按右端点分桶。
扫到 right 时，在前次出现位置上加一；统计不大于 left 的前次出现位置，
减去 [0,left) 中固定的 left 个位置，得到 [left,right) 的不同值数。

实现见 distinct.h，前缀统计复用 prefix_tree.h 的 32 叉 SIMD 计数树，
每层读取一个前缀，更新并行修改后续孩子的前缀。旧 .cpp 保留作参考。
GCC/Clang 官方、朴素区间对照及 sanitizer 通过。
Lenovo 单次 max/sum 为 91.549/489.673 ms，对照最小值为 105.429/549.292 ms。
证据：bench/ds-range-20260930/round1/。
