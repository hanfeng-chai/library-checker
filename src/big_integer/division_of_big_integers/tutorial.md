# 十进制大整数商余数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 75.419 (Anonymous), sum 1280.460 (Anonymous)。

- `main.cxx` max: 65.829 ms (-12.72%), sum: 960.202 ms (-25.01%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
题目输入非负。两数都不超过 18 位时用 u64 除法；被除数位数较少时，
直接输出商 0 和原被除数。其余输入以 10^8 为一肢，复用 `BigDivision<false>`。

小除数或很短的商用 Knuth D。大数用 Newton 倒数估计商，循环卷积缩短
误差项的计算；长商分块求解并复用倒数、除数的频谱，最后用原除数校正商余数。
分块宽度按 FFT 长度边界选择。

当前实现无需 GMP。接口与浮点快速路径的精度范围见
[big_division](../../../include/toy/big_division.md) 和 [fft_integer](../../../include/toy/fft_integer.md)。
性能结果见 [评测审计](../../../tools/measurement_audit.md)。

## 尝试过程与取舍

最初用 GMP 路径建立对照，但未达到本题要求。原生版本从 Knuth D 和 Newton 倒数开始，随后把长商分块，缓存倒数和除数频谱。迭代包括循环卷积误差项、减少进位后再拆块的往返、局部工作区、较小 FFT 缓存块、缩小商块、机器字除法，以及按 FFT 长度边界平衡块宽。后几轮只与最强参考做诊断，最终重新用全部五份参考完整复核；当前生产实现不依赖 GMP。
