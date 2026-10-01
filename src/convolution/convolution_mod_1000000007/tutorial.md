# 模 10^9+7 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 72.193 (Rohan_Kapri), sum 1619.985 (Rohan_Kapri)。

- `main.cxx` max: 70.850 ms (-1.86%), sum: 1585.665 ms (-2.12%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
把余数嵌入小坐标的虚二次整数环，做复数 FFT 后投影回模 10^9+7。
大页缓冲区和缓存内子树减少访存开销；短边或超出浮点快速路径的长度使用精确 CRT。

接口、精度范围及验证见 [`fft_convolution.md`](../../../include/toy/fft_convolution.md)。

## 尝试过程与取舍

先以精确 CRT 卷积建立正确性基线，再尝试复数 FFT。后续依次比较频谱工作区和根表缓存、更新的 FFT 内核、除法相关路径与大页缓冲区。最终采用虚二次整数环的小坐标表示及缓存内子树，短边和浮点快速路径范围外仍走精确 CRT。各候选的速度与精度条件分开检查，不能只因某次 FFT 更快就放宽正确性范围。
