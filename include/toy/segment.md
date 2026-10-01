# segment

`SegmentTree<T,Operation>(values,identity,operation)` 维护可平凡复制的幺半群值。
set(i,value) 单点覆盖，fold(l,r) 按下标递增顺序合并半开区间；空区间返回 identity。
Operation 须满足结合律，identity 为单位元，不要求交换律。
构造 O(n)、修改/查询 O(log n)、空间 O(n)，由可排序区间的非交换函数复合对照覆盖。
