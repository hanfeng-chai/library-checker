# 带势并查集

加法群中的权值记录相对父节点的差。压缩路径时沿根到点累加权值，
连接两个根时换算需要的父边权值；若已经连通，则检查新约束与已有差值是否一致。

实现见 [potential_dsu](../../../docs/potential_dsu.md) 与 [group](../../../docs/group.md)。
按大小合并保证树深，直接父节点为根时省去与单位元的运算。
GCC/Clang 官方、独立矩阵关系对照和 sanitizer 通过。
Lenovo 单次 max/sum 为 17.122/170.850 ms，对照最小值为 18.344/185.396 ms。
证据：bench/ds-order-20260930/round1/。旧 .cpp 保留作参考。
