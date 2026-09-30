# 双端优先队列

主解使用 [PartitionQueue](../../../docs/partition_queue.md)，把集合保存在
按值域有序的桶中。取最小/最大值时才划分相应端点的大桶，桶内不必全部排序。
AVX2 负责分桶和小桶极值，重复值用隐式次数保存，避免反复展开或逐个划分。

通用比较器的最小最大堆保留在 heap.cxx，对应 [minmax_heap](../../../docs/minmax_heap.md)；
旧 .cpp 实现继续作为参考。两种实现均在同一题目框架验证正确性。

主解已通过 GCC/Clang 官方、朴素对照及 sanitizer。
Lenovo 单次 max/sum 为 44.972/349.696 ms，对照最小值为 70.779/493.181 ms。
证据：`bench/ds-final-20260930/round1/` 的 partition 候选。
