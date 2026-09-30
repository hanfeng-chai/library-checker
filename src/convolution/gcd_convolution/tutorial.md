# GCD 卷积

令 A[d] 为所有 d 的倍数上的系数和；对 A、B 点乘后在倍数偏序上反演。按素数筛遍历，时间 O(N log log N)，空间 O(N)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`divisor_convolution.md`](../../../docs/divisor_convolution.md)。
