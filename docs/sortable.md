# sortable

`SortableSequence<T,Operation>(keys,values,identity,operation)` 支持 set(i,key,value)、
sort(l,r,descending=false)、fold(l,r)。键为 u32，当前所有键须互异，n<2^26；
值可平凡复制，合并满足结合律及单位元条件。排序区间半开，查询保持当前顺序。

每个连续有序块由压缩二进制字典树表示，节点缓存正向、反向聚合。
排序时按排名切出两端，再合并块的字典树；降序只改变块方向。
位图维护块边界，外层 SegmentTree 维护整块聚合，边界的零散部分在字典树内查询。
节点通过内嵌空闲链复用，计数与分叉位打包保存。

GCC/Clang 官方、随机升降序/覆盖/空区间/完整 u32 键及独立数组对照、sanitizer 通过。
相同位置可用同一键覆盖值，但不能产生重复的当前键。
