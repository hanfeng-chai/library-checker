# 非交换群上的带势并查集

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 30.198 (chaihf), sum 340.886 (chaihf)。

- `main.cxx` max: 26.184 ms (-13.29%), sum: 287.578 ms (-15.64%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
矩阵权值满足 a[x]=a[parent[x]]*weight[x]；压缩路径时按从根到点的顺序相乘。
合并根时按约束换算边权，换合并方向时取逆。题目矩阵行列式为一，逆不需要幂运算。

实现见 [potential_dsu](../../../include/toy/potential_dsu.md) 与 [group](../../../include/toy/group.md)。
按大小合并保证树深，直接父节点为根时省去与单位元的运算。
GCC/Clang 官方、独立矩阵关系对照和 sanitizer 通过。

## 尝试过程与取舍

与加法势能 DSU 共用结构，但矩阵乘法不交换，压缩和反向合并必须保持顺序。利用题目行列式为一的条件直接求逆，避免幂运算；小规模独立矩阵关系对照后再进行完整评测。
