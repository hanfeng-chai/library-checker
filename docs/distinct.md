# distinct

`range_distinct(values,queries)` 返回各个半开区间的不同值数量。
查询用 RangeQuery{left,right} 表示，允许空数组和空区间；值为 u32，不能为全一哨兵。

先记录每个位置的前次出现位置加一，按右端点分桶处理查询。
扫到 right 时，把每个 previous+1 的计数加入 32 叉 SIMD 前缀树；前缀计数减去 left
恰好得到 [left,right) 中首次出现的值数量。期望 O(n+(n+q)log n)，空间 O(n+q)。
