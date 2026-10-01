# 十六进制大整数乘法

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 36.779 (Anonymous), sum 558.602 (Rohan_Kapri)。

- `main.cxx` max: 31.331 ms (-14.81%), sum: 492.300 ms (-11.87%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先乘绝对值，结果符号取异或，零统一为非负。原数以 2^64 为一肢，
短乘数用朴素乘法；大数拆成 14 位系数，以打包实数 FFT 求卷积，
用 u128 累加系数并恢复机器字。输出大写十六进制。
同一 `IntegerFFT` 复用根表与工作区，平方只做一次正变换。

普通乘积超过已验证的 FFT 长度范围时退回双模精确卷积。
实现与浮点路径的精度范围见 [big_integer](../../../include/toy/big_integer.md) 和
[fft_integer](../../../include/toy/fft_integer.md)，性能见 [评测审计](../../../tools/measurement_audit.md)。

## 尝试过程与取舍

从精确整数卷积出发，比较原 FFT 与 14 位拆块、提取方式，再测试大页、直接进位、局部分配和 FFT 缓存块。14 位系数只有在经过验证的长度与精度范围内才走浮点快速路径，范围外仍保留精确回退。最终批次及其后的内存诊断都列出，后者不是重新挑选更快的验收结果。
