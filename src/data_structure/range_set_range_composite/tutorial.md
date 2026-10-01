# 区间赋值、区间函数复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 346.140 (nandhagk), sum 2995.477 (nandhagk)。

- `main.cxx` max: 330.421 ms (-4.54%), sum: 2894.038 ms (-3.39%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 range_set_composite.h。赋值时计算同一函数的二进制幂，
完整覆盖的节点直接取对应长度的幂，标记记录这次赋值的幂表位置。
查询遇到懒标记就按实际交集长度计算重复复合，不必把标记推到更深层。
其他部分用迭代前缀、后缀遍历按下标顺序合并，保证非交换复合的次序。

内部 Montgomery 表示减少模乘规约开销，输出时才还原普通余数。
大数组建议透明大页；不依赖建议是否生效。修改 O(log N)，查询 O(log N)，
树空间 O(N)，累计幂表 O(Q log N)。
GCC/Clang 官方及含零斜率、非二次幂、空区间的朴素对照和 sanitizer 通过。

## 尝试过程与取舍

先用区间赋值和二进制幂表处理相同函数的重复复合，之后比较 Montgomery 表示、快速路径及迭代遍历。查询遇到赋值标记时直接按交集长度取幂，减少下推。最终仍支持零乘数，按顺序合并前缀和后缀。
