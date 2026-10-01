# 多项式采样点平移

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 91.566 (QedDust413), sum 1204.464 (Rohan_Kapri)。

- `main.cxx` max: 78.813 ms (-13.93%), sum: 1042.807 ms (-13.42%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
已知次数小于 n 的多项式在 0..n-1 的值，要求在 c..c+m-1 的值。
等距点的重心插值权重为 (-1)^(n-1-i)/(i!(n-1-i)!)，与倒数序列卷积后，
乘上各求值点到原点列的差积即可。

已知采样点直接取值，滑动差积处理经过零点及模数回绕的情况。
实现与约束见 [polynomial](../../../include/toy/polynomial.md)。

## 尝试过程与取舍

采用等距点的重心插值权重和卷积。滑动差积必须正确处理已知采样点、经过零点和模数回绕，不能只优化普通区间；这些边界与朴素求值对照通过后完成完整测量。
