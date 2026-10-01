# 集合幂级数对数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 310.223 (adamant), sum 1035.222 (adamant)。

- `main.cxx` max: 230.783 ms (-25.61%), sum: 754.822 ms (-27.09%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
逐个加入满足 x²=0 的变量，将输入写成 A+xB，则
log(A+xB)=log(A)+x B/A。低半段沿用已有结果，高半段直接调用子集除法；
频点上按秩递推求商，复用子集卷积内核。要求输入空集系数为 1。

接口、复杂度和验证见 [set_series](../../../include/toy/set_series.md)。

## 尝试过程与取舍

对数与指数复用集合幂级数的分块及子集卷积结构，避免为对数再维护一套卷积。每轮底层布局、缩半 XOR、融合和工作区改动均在本题实际输入上比较；用指数/对数关系及小规模朴素对照检查符号和截断边界。
