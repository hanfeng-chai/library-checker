# 十六进制大整数商余数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 60.976 (Anonymous), sum 720.877 (Anonymous)。

- `main.cxx` max: 59.781 ms (-1.96%), sum: 697.823 ms (-3.20%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
题目输入非负。两数都不超过 16 位时用 u64 除法，不超过 31 位时用 u128；
被除数位数较少时直接输出商 0 和原被除数。其余输入以 2^64 为一肢，
复用 `BigDivision<true>`，同时计算商和余数。

小除数或很短的商用 Knuth D，128/64 位试商直接使用 x86 DIV。
大数用 Newton 倒数估计商，循环卷积缩短误差项的计算；长商分块求解，
按 FFT 长度边界选择块宽，复用频谱，最后用原除数校正商余数。

当前实现无需 GMP。接口与浮点快速路径的精度范围见
[big_division](../../../include/toy/big_division.md) 和 [fft_integer](../../../include/toy/fft_integer.md)。
性能结果见 [评测审计](../../../tools/measurement_audit.md)。

## 尝试过程与取舍

GMP 对照未达标后，先实现原生 Knuth D 与 Newton 倒数。初版原生算法虽正确，但 max 远高于参考；此后逐步加入循环卷积残差、频谱复用、直接进位、局部工作区和较小 FFT 缓存块。倒数优化先改善了 sum，max 仍受长商块影响；缩小块宽并按 FFT 边界估计整体工作量，才同时降低两项。128/64 试商直接用 x86 DIV，并给短输入加 u64/u128 路径。最后保留原除数校正商余数的步骤，速度优化没有放宽精确性要求。
