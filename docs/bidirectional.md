# bidirectional

`BidirectionalTree<T,Operation>(values,identity,operation)` 缓存正向与反向聚合，
支持 set(i,value)、fold(l,r) 和 fold<true>(l,r)。区间半开，空聚合返回 identity。
T 须可平凡复制，operation 满足结合律，不要求交换律。

每个节点分别合并左到右和右到左的结果，查询只计算需要的方向。
时间 O(log n)，空间 O(n)。非交换的仿射映射、零斜率、非二次幂和空范围通过
独立函数链对照、GCC/Clang 与 sanitizer。
