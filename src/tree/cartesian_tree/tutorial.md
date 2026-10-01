# 笛卡尔树

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 39.620 (oldyan), sum 309.867 (oldyan)。

- `main.cxx` max: 37.072 ms (-6.43%), sum: 278.251 ms (-10.20%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 cartesian.h 的单调栈。栈保存已处理前缀的最右路径；新值弹出
比它大的节点，最后弹出的子树接为左孩子，未弹出的栈顶成为父亲。根的父亲为自己。
每个栈项缓存值与下标，避免先读下标再随机访问原数组的依赖链。时间、空间 O(N)。

25 个官方测例通过 GCC/Clang，包含重复值的独立递归最小值对照与 sanitizer 通过。

## 尝试过程与取舍

先使用经典单调栈构造 Cartesian tree。首版栈只存下标，比较时还要重新读原值；随后让栈项同时缓存值和编号，减少间接读取，完整比较后采用。算法和相等值的约定保持不变，优化集中在必要数据的布局。
