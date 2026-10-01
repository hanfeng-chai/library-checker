# 十进制大整数乘法

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 52.966 (Anonymous), sum 813.341 (Anonymous)。

- `main.cxx` max: 32.323 ms (-38.97%), sum: 503.921 ms (-38.04%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先乘绝对值，结果符号取异或，零统一为非负。原数以 10^8 为一肢，
短乘数用朴素乘法；大数拆成 10^4 系数，以打包实数 FFT 求卷积，再合并进位。
同一 `IntegerFFT` 复用根表与工作区，绝对值相同的平方只做一次正变换。

普通乘积超过已验证的 FFT 长度范围时退回双模精确卷积。
实现与浮点路径的精度范围见 [big_integer](../../../include/toy/big_integer.md) 和
[fft_integer](../../../include/toy/fft_integer.md)，性能见 [评测审计](../../../tools/measurement_audit.md)。

## 尝试过程与取舍

初版精确卷积作为正确性基线，随后加入拆成四位十进制系数的打包 FFT。复用根表、缓冲区以及平方时的频谱后，再比较分块整合、大页提示、直接进位、局部分配与较小缓存块。最后选择实际胜出的组合。最终完整批次出现一次较慢观测，仍原样保留；后续内存诊断用于解释用户态/内核态差别，没有拿诊断中的较快次数替换验收值。
