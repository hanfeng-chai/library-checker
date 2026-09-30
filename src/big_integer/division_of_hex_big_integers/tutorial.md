# 十六进制大整数商余数

题目输入非负。两数都不超过 16 位时用 u64 除法，不超过 31 位时用 u128；
被除数位数较少时直接输出商 0 和原被除数。其余输入以 2^64 为一肢，
复用 `BigDivision<true>`，同时计算商和余数。

小除数或很短的商用 Knuth D，128/64 位试商直接使用 x86 DIV。
大数用 Newton 倒数估计商，循环卷积缩短误差项的计算；长商分块求解，
按 FFT 长度边界选择块宽，复用频谱，最后用原除数校正商余数。

当前实现无需 GMP。接口与浮点快速路径的精度范围见
[big_division](../../../docs/big_division.md) 和 [fft_integer](../../../docs/fft_integer.md)。
性能结果见 [评测审计](../../../docs/measurement_audit.md)。
