# 静态区间不同值数量

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 105.240 (Rohan_Kapri), sum 554.010 (IceKylin)。

- `main.cxx` max: 91.484 ms (-13.07%), sum: 492.088 ms (-11.18%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
记录每个位置的前次出现位置加一，查询按右端点分桶。
扫到 right 时，在前次出现位置上加一；统计不大于 left 的前次出现位置，
减去 [0,left) 中固定的 left 个位置，得到 [left,right) 的不同值数。

实现见 distinct.h，前缀统计复用 prefix_tree.h 的 32 叉 SIMD 计数树，
每层读取一个前缀，更新并行修改后续孩子的前缀。旧 .cpp 保留作参考。
GCC/Clang 官方、朴素区间对照及 sanitizer 通过。

## 尝试过程与取舍

用前次出现位置把不同值数变成离线前缀计数，查询按右端点扫描。之后复用 SIMD 宽前缀树以减少层数；保持重复值、空区间与边界位置的精确语义，并与朴素集合计数对照。
