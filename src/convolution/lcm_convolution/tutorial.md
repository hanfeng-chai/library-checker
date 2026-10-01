# LCM 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 77.149 (toomer), sum 738.673 (toomer)。

- `main.cxx` max: 67.480 ms (-12.53%), sum: 635.877 ms (-13.92%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
令 A[d] 为所有 d 的约数上的系数和；对 A、B 点乘后在约数偏序上反演。按素数筛遍历，时间 O(N log log N)，空间 O(N)。

实现使用 toy 的统一 I/O 和 Buffer。SIMD、接口约定与 Lenovo 对照见
[`divisor_convolution.md`](../../../include/toy/divisor_convolution.md)。

## 尝试过程与取舍

与 GCD 卷积共用按素数筛遍历的实现，改为约数偏序的变换方向。保留统一的 Montgomery/SIMD 算术和输入输出接口，先做朴素对照，再与五份提交完整比较；本题首批即达到两项门槛。
