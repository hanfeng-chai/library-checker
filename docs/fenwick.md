# fenwick

`Fenwick<T>(n)` 初始化零数组，也可用只读 span 线性建树。
`add(i,delta)` 单点加；`prefix(r)` 求 [0,r) 的和，`sum(l,r)` 求 [l,r) 的和。
下标从零开始，查询允许空区间，单次 O(log n)，空间 O(n)。

`WideFenwick(std::move(values))` 面向 u64，接口相同。每个十六叉节点保存
各孩子之前的前缀和，查询每层只读一个值；更新用 AVX2 给后续前缀加 delta。
补一个零哨兵，使 prefix(n) 与其他查询使用相同路径。
算术按 u64 模 2^64；需要普通整数结果时由调用方保证范围。
