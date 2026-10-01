# 按位 XOR 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 56.997 (Rohan_Kapri), sum 201.953 (Rohan_Kapri)。

- `main.cxx` max: 44.404 ms (-22.09%), sum: 155.533 ms (-22.99%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
Walsh–Hadamard 变换后点乘，再逆变换。归一化系数提前合入 B，避免逆变换逐层除二。时间 O(N log N)，空间 O(N)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`bitwise_convolution.md`](../../../include/toy/bitwise_convolution.md)。

## 尝试过程与取舍

从经典 Walsh–Hadamard 变换出发，将正变换、点乘和逆变换递归融合，底部三个维度做 SIMD。归一化 1/N 预先吸收到 B，避免逆变换逐层除二。首批完整对照已双项达标，因此保留这一路径；合数模数的独立对照也检查了归一化的适用条件。
