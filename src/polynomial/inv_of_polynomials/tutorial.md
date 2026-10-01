# 任意多项式模逆

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 196.253 (cmk666), sum 1273.705 (cmk666)。

- `main.cxx` max: 183.728 ms (-6.38%), sum: 1191.813 ms (-6.43%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
逆存在当且仅当 f、g 互素；扩展 Euclid 求 u*f+v*g=d，d 为非零常数时
答案为 u/d。g 为常数时按题意输出零多项式。

[polynomial_gcd](../../../include/toy/polynomial_gcd.md) 用半 GCD 降低次数，
将主要工作转成共享 NTT 的多项式矩阵乘法。

## 尝试过程与取舍

先以半 GCD 的扩展 Euclid 实现任意模多项式求逆。后续将短除法更新向量化，并比较不同递归/短除法阈值（32、128）与向量版本，避免仅凭理论复杂度选择阈值。最终使用全量对照胜出的组合。
