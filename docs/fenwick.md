# fenwick

`Fenwick<T>(n)` 初始化零数组，也可用只读 span 线性建树。
`add(i,delta)` 单点加；`prefix(r)` 求 [0,r) 的和，`sum(l,r)` 求 [l,r) 的和。
下标从零开始，查询允许空区间，单次 O(log n)，空间 O(n)。

`WideFenwick(std::move(values))` 面向 u64，接口相同。每个十六叉节点保存
各孩子之前的前缀和，查询每层只读一个值；更新用 AVX2 给后续前缀加 delta。
补一个零哨兵，使 prefix(n) 与其他查询使用相同路径。
算术按 u64 模 2^64；需要普通整数结果时由调用方保证范围。

WideFenwick 的 sum(l,r) 在两端到达共同高位前缀时停止，省掉相互抵消的读取。
add_difference(l,r,delta) 要求 0≤l≤r≤n，表示给位置 l 加 delta、位置 r 减 delta；合流时用
两个后缀掩码的 xor 一次更新，随后停止。可用于差分数组上的区间加；它并非
给原数组 [l,r) 中的每项加值。位置 r 可为构造时原数组长度的哨兵。
完整 u64 环运算、空范围与多层边界通过独立数组对照及 sanitizer。
