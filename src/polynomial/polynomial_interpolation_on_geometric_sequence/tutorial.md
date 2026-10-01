# 几何序列插值

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 196.342 (QedDust413), sum 2013.851 (QedDust413)。

- `main.cxx` max: 169.168 ms (-13.84%), sum: 1682.614 ms (-16.45%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
显式计算各几何点的插值权重，用幂和与根多项式恢复系数。

接口和验证见 [`polynomial.md`](../../../include/toy/polynomial.md)。

## 尝试过程与取舍

显式计算几何点的插值权重，利用幂和与根多项式恢复系数，复用现有卷积。重点验证点互异等接口前提及与直接插值的等价性，完整批次双项达到目标。
