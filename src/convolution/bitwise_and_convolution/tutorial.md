# 按位 AND 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 56.193 (adamant), sum 199.217 (adamant)。

- `main.cxx` max: 41.194 ms (-26.69%), sum: 145.387 ms (-27.02%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
对超集做 zeta 变换，点乘后 Möbius 逆变换。时间 O(N log N)，空间 O(N)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`bitwise_convolution.md`](../../../include/toy/bitwise_convolution.md)。

## 尝试过程与取舍

本题保留超集 zeta/Möbius 这一经典算法，但把正变换、点乘、逆变换按子问题融合，减少大数组往返。底部三个维度用一个 AVX2 向量处理，只转换 B 的 Montgomery 表示。现存完整记录中，首批实现已双项超过五份参考，保留这一实现。
