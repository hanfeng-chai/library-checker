# frequency

`StaticFrequency(values)` 统计静态 u32 数组。`count(l,r,value)` 返回
半开区间 [l,r) 中 value 的次数，允许空数组、空区间。

哈希分配值的组号，再用连续存储收集各组的出现位置；组内下标自然递增，
查询做两次二分，小于等于八个位置时直接扫描。
值不能等于 u32 的全一哨兵；预处理期望 O(n)，空间 O(n)。

已知全部查询时，`static_frequencies(values,queries)` 将左右端点按位置分桶，
从左向右维护各值出现次数，做前缀差。每个查询是 `FrequencyQuery{left,right,value}`，
答案按原顺序返回。期望 O(n+q) 时间和空间，避免长位置表的反复二分。
