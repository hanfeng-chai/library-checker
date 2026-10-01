# 集合幂级数幂投影

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 473.618 (Rohan_Kapri), sum 2747.653 (Rohan_Kapri)。

- `main.cxx` max: 365.372 ms (-22.86%), sum: 2093.952 ms (-23.79%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
要求各次幂与给定权重的内积，采用集合幂级数复合过程的转置。
逆序消去变量，将高半段权重通过转置子集乘法传给下一阶导数；
转置乘法由补集反转、子集卷积、再反转实现。

最后把导数权重转换为幂的权重：空集系数为 0 时乘阶乘，非零时另做一次普通卷积。
接口、复杂度和验证见 [set_series](../../../include/toy/set_series.md)。

## 尝试过程与取舍

本题使用与集合幂级数复合对应的转置运算，底层仍复用子集卷积。布局和工作区变化不仅要改善吞吐，也必须保持转置关系；因此与复合、exp/log 及 subset_convolution 一起做正确性和性能回归。完整阶段数据如下。
