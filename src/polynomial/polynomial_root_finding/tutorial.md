# 多项式求根

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 498.952 (Rohan_Kapri), sum 6300.626 (cmk666)。

- `main.cxx` max: 128.999 ms (-74.15%), sum: 1662.977 ms (-73.61%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先提取零根，再以 gcd(f,x^(P-1)-1) 得到非零根对应的首一无重因子。
随机平移后，用 Euler 判别将根集分成两组，递归直至线性或二次因子。

实现复用 [polynomial_roots](../../../include/toy/polynomial_roots.md)、固定除数模幂和半 GCD。

## 尝试过程与取舍

先去除零根，计算与 x^(P-1)-1 的 gcd，之后用随机平移和二分分裂根集。固定除数的模幂与半 GCD 共用已验证库；共享短除法进一步优化后再次完整回归，保留两轮结果。
