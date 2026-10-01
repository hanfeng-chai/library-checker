# 区间取 min/max、加法与求和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 591.585 (oldyan), sum 7916.042 (oldyan)。

- `main.cxx` max: 580.864 ms (-1.81%), sum: 7315.510 ms (-7.59%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 调用 beats.h。节点维护最值、严格第二最值、最值出现次数和区间和。
例如 chmin(x) 在 second_maximum < x < maximum 时只改变最大值组，和增加
(x−maximum)·maximum_count；其余情况跳过或递归。chmax 对称。
加法同时平移极值、和及懒标记，下传时先加再按父节点范围截断。

更新沿不对齐的两条边界路径下传，共同祖先只走一次，并提前预取底部六层。
等值节点取 min/max 无需重写未变的计数与第二极值。只读查询累积祖先加法和
上下界，不把标记写到孩子；空叶子通过实际元素数排除。

33 个官方测例通过 GCC/Clang；随机混合操作、非二次幂、空区间及反复大幅
平移后截断通过朴素对照与 ASan/UBSan。

## 尝试过程与取舍

从经典 Segment Tree Beats 出发，先比较只读查询与会下推的查询，再比较迭代边界路径、推送顺序和选择方式。后续尝试稠密布局、不同预取深度、前瞻与等值节点专门处理。最终跳过未变化的计数和第二极值写入，合并共同边界祖先；每步都检查混合更新与大幅平移后的截断。
