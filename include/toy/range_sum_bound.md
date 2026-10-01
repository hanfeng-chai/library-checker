# range_sum_bound

`range_sum_bound(values,queries)` 返回各 [left,right) 中不大于 bound 的元素个数与总和。
查询为 BoundedSumQuery{left,right,bound,index}，index 是输出位置，覆盖 0..q-1。

按值排序数组项、按上界排序查询，用 [CountSumTree](count_sum.md) 顺序插入可见项，
每个原位置只插入一次，并预取之后第八个插入位置的底层节点。支持 u32 值，结果和须能放进 u64。
时间 O((n+q)log n)，工作区 O(n+q)。GCC/Clang 官方与朴素对照、sanitizer 通过。
