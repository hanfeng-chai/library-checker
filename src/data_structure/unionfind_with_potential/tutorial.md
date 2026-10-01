# 带势并查集

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 18.398 (sortA0329), sum 187.561 (Anonymous)。

- `main.cxx` max: 17.184 ms (-6.60%), sum: 172.428 ms (-8.07%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
加法群中的权值记录相对父节点的差。压缩路径时沿根到点累加权值，
连接两个根时换算需要的父边权值；若已经连通，则检查新约束与已有差值是否一致。

实现见 [potential_dsu](../../../include/toy/potential_dsu.md) 与 [group](../../../include/toy/group.md)。
按大小合并保证树深，直接父节点为根时省去与单位元的运算。
GCC/Clang 官方、独立矩阵关系对照和 sanitizer 通过。

## 尝试过程与取舍

记录相对父亲的加法权值，压缩时按根到点的顺序累积，合并时换算父边关系。直接父亲为根时省去单位元运算，按大小合并控制树深；首批完整比较达标。
