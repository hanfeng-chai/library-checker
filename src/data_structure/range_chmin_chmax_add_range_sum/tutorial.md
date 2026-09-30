# 区间取 min/max、加法与求和

`main.cxx` 调用 beats.h。节点维护最值、严格第二最值、最值出现次数和区间和。
例如 chmin(x) 在 second_maximum < x < maximum 时只改变最大值组，和增加
(x−maximum)·maximum_count；其余情况跳过或递归。chmax 对称。
加法同时平移极值、和及懒标记，下传时先加再按父节点范围截断。

更新沿不对齐的两条边界路径下传，共同祖先只走一次，并提前预取底部六层。
等值节点取 min/max 无需重写未变的计数与第二极值。只读查询累积祖先加法和
上下界，不把标记写到孩子；空叶子通过实际元素数排除。

33 个官方测例通过 GCC/Clang；随机混合操作、非二次幂、空区间及反复大幅
平移后截断通过朴素对照与 ASan/UBSan。
Lenovo 静默、不绑核、每例一次：main max/sum 为 581.261/7311.890 ms，
五份参考的最小值分别为 592.062/7925.260 ms。
证据：bench/ds-beats-uniform-20261001/round1/selected/。旧 .cpp 保留作参考。
