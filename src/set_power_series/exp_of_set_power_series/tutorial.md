# 集合幂级数指数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 260.228 (adamant), sum 1119.237 (adamant)。

- `main.cxx` max: 217.021 ms (-16.60%), sum: 920.954 ms (-17.72%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
逐个加入满足 x²=0 的变量，将输入写成 A+xB，则
exp(A+xB)=exp(A)+x B exp(A)。已知低半段 exp(A)，一次子集卷积得到高半段。
从常数 1 出发倍增数组长度，要求输入空集系数为 0。

接口、复杂度和验证见 [set_series](../../../include/toy/set_series.md)。

## 尝试过程与取舍

指数通过分块递推复用子集卷积。主要实验集中在底层子集内核：补齐存储、缩半 XOR、融合步骤、大页提示、工作区复用和最后的清理版本。最终按完整批次选择，而不是单看某个频点内核的局部速度。
