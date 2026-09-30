# 多维循环卷积

逐轴做 DFT，点乘后逐轴逆变换。大维度用 chirp-z 和可重用的精确 CRT 卷积核。

实现、约束及验证见 [`multivariate_cyclic.md`](../../../docs/multivariate_cyclic.md)。
