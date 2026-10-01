# 区间仿射变换、单点查询

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 126.381 (chaihf), sum 714.472 (chaihf)。

- `main.cxx` max: 119.488 ms (-5.45%), sum: 628.207 ms (-12.07%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 `affine_point.h` 的八叉 SIMD 懒标记树。区间更新先下推两条
边界路径上的旧标记，再把完整覆盖的孩子组统一复合新变换，保持操作时间顺序。
单点的值等于底层值依次应用各祖先标记。

题目允许延后输出，因此查询只保存当时的标记快照，最后并行计算八个答案。
快照在查询时取得，之后的更新不会改变已有答案。底层直接保存值，其他层才有
乘法标记；内部使用 Montgomery 冗余表示。旧库 wide_affine.hpp 提供了结构参考。

每次操作 O(log n)，树空间 O(n)，答案快照额外 O(Q log n)。
GCC/Clang 官方、普通 get 与分批 resolve 的朴素对照及 sanitizer 通过。

## 尝试过程与取舍

区间更新必须按时间顺序复合，首版用八叉 SIMD 懒标记树统一处理完整孩子组，边界先下推旧变换。单点查询再沿路径合成；验证非交换顺序和零斜率后，首轮完整比较达标。
