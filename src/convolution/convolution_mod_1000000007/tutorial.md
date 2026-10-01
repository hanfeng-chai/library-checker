# 模 10^9+7 卷积

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 72.193 (Rohan_Kapri), sum 1619.985 (Rohan_Kapri)。

- `main.cxx` max: 70.209 ms (-2.75%), sum: 1576.633 ms (-2.68%)
- `naive.cxx` max: 105.318 ms (+45.88%), sum: 2392.830 ms (+47.71%)
- `crt.cxx` max: 91.469 ms (+26.70%), sum: 2056.355 ms (+26.94%)
- `crt_cpp.cxx` max: 105.314 ms (+45.88%), sum: 2380.303 ms (+46.93%)
- `fft_naive.cxx` max: 257.124 ms (+256.16%), sum: 5877.682 ms (+262.82%)
- `naive.cxx [Clang 23]` max: 98.098 ms (+35.88%), sum: 2246.673 ms (+38.68%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
本题模数是 10^9+7，需要精确整数路线时使用多模数 NTT＋CRT；
浮点路线保留优化版及经典分块基线。所有版本使用相同的最新 `read_bulk<u32,10>`
和 Writer 数组输出。输出可能达到十位，因此不用仅支持九位的 `write_bulk9`。

| 文件 | 实现 |
|---|---|
| main.cxx | 原有的小坐标二次整数环嵌入＋FFT，换用最新批量读取 |
| fft_naive.cxx | 有符号分块＋完整长度复数 FFT 的经典基线 |
| crt.cxx | 三模数融合 NTT＋CRT，含 AVX2 汇编 |
| crt_cpp.cxx | 同一新版 CRT，关闭手写汇编 |
| naive.cxx | 原有三模数 NTT＋CRT |

旧 CRT 保留在 `convolution_crt.h`；新版与它使用相同的素数和重建公式，
只替换逐素数的卷积内核。`fft_naive.cxx` 是本次补充的基线，不冒充历史版本。
较短输入直接相乘；本题全部大数据位于现有主解的浮点路径范围内。

接口和精度条件见 [fft_convolution.md](../../../include/toy/fft_convolution.md)、
[convolution_fft_split.md](../../../include/toy/convolution_fft_split.md) 和
[convolution_crt_fast.md](../../../include/toy/convolution_crt_fast.md)。
五份用户提交复用原记录，新 I/O 下的各自有程序单独测量。
