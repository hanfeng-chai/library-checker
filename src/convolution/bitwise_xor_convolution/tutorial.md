# 按位 XOR 卷积

Walsh–Hadamard 变换后点乘，再逆变换。归一化系数提前合入 B，避免逆变换逐层除二。时间 O(N log N)，空间 O(N)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`bitwise_convolution.md`](../../../docs/bitwise_convolution.md)。
