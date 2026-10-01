# 有序集合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 89.057 (nandhagk), sum 1694.382 (nandhagk)。

- `main.cxx` max: 67.711 ms (-23.97%), sum: 1687.679 ms (-0.40%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
初始集合已经递增，只对查询键做稳定基数排序，再线性归并完成坐标压缩。
用 64 位机器字保存成员关系，32 叉前缀树维护秩及第 k 小。
前驱、后继直接查当前字与非空字位图，避免先求秩再选第 k 小的两次遍历。

实现见 ordered_set.h、prefix_tree.h 与 radix_sort.h。旧 .cpp 保留作参考。
GCC/Clang 官方、朴素秩/选秩/前驱后继对照与 sanitizer 通过。

## 尝试过程与取舍

先做坐标压缩与位图秩/选秩，随后利用初始集合有序的条件，只排序查询键再线性归并。前驱后继直接查字内位图和非空字索引，避免先求秩再选第 k 小的重复遍历；各轮数据保留为共享排序与前缀树优化的回归记录。
