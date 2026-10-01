# 多维循环卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 290.260 (Rohan_Kapri), sum 2600.098 (Rohan_Kapri)。

- `main.cxx` max: 225.675 ms (-22.25%), sum: 1350.631 ms (-48.05%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
逐轴做 DFT，点乘后逐轴逆变换。大维度用 chirp-z 和可重用的精确 CRT 卷积核。

实现、约束及验证见 [`multivariate_cyclic.md`](../../../include/toy/multivariate_cyclic.md)。

## 尝试过程与取舍

先逐轴做变换，大维度使用 chirp-z 并缓存精确 CRT 卷积核；小轴则比较直接 DFT 路径。direct 是小维度直接计算的候选。保留逐轴通用结构，根据轴规模选择内部实现，避免为固定维数写专用解。
