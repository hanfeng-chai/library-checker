# 非交换群上的带势并查集

矩阵权值满足 a[x]=a[parent[x]]*weight[x]；压缩路径时按从根到点的顺序相乘。
合并根时按约束换算边权，换合并方向时取逆。题目矩阵行列式为一，逆不需要幂运算。

实现见 [potential_dsu](../../../docs/potential_dsu.md) 与 [group](../../../docs/group.md)。
按大小合并保证树深，直接父节点为根时省去与单位元的运算。
GCC/Clang 官方、独立矩阵关系对照和 sanitizer 通过。
Lenovo 单次 max/sum 为 25.987/284.729 ms，对照最小值为 29.744/337.033 ms。
证据：bench/ds-order-20260930/round1/。旧 .cpp 保留作参考。
