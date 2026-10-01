# 双端优先队列

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 71.355 (nandhagk), sum 497.517 (nandhagk)。

- `main.cxx` max: 45.101 ms (-36.79%), sum: 351.893 ms (-29.27%)
- `naive.cxx` max: 108.985 ms (+52.74%), sum: 764.681 ms (+53.70%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解使用 [PartitionQueue](../../../include/toy/partition_queue.md)，把集合保存在
按值域有序的桶中。取最小/最大值时才划分相应端点的大桶，桶内不必全部排序。
AVX2 负责分桶和小桶极值，重复值用隐式次数保存，避免反复展开或逐个划分。

通用比较器的最小最大堆保留在 naive.cxx，对应 [minmax_heap](../../../include/toy/minmax_heap.md)；
旧 .cpp 实现继续作为参考。两种实现均在同一题目框架验证正确性。

主解已通过 GCC/Clang 官方、朴素对照及 sanitizer。

## 尝试过程与取舍

先以通用 MinMaxHeap 建立正确基线，并比较坐标压缩、读取位数和 SIMD 读取等路径。最终改为端点查询时才划分大桶的 PartitionQueue，重复值用次数表示，避免对所有元素持续维持完整堆序。通用最小最大堆作为经典对照保留在 naive.cxx。
