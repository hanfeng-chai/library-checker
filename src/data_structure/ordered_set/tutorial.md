# 有序集合

初始集合已经递增，只对查询键做稳定基数排序，再线性归并完成坐标压缩。
用 64 位机器字保存成员关系，32 叉前缀树维护秩及第 k 小。
前驱、后继直接查当前字与非空字位图，避免先求秩再选第 k 小的两次遍历。

实现见 ordered_set.h、prefix_tree.h 与 radix_sort.h。旧 .cpp 保留作参考。
GCC/Clang 官方、朴素秩/选秩/前驱后继对照与 sanitizer 通过。
Lenovo 单次 max/sum 为 67.806/1681.882 ms，对照最小值为 89.570/1687.903 ms。
证据：bench/ds-spatial-20261001/round1/。
