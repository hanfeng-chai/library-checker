# 模 2^64 卷积

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 190.680 (toomer), sum 3158.567 (toomer)。

- `main.cxx` max: 174.326 ms (-8.58%), sum: 2953.514 ms (-6.49%)
- `naive.cxx` max: 196.509 ms (+3.06%), sum: 3481.097 ms (+10.21%)
- `crt.cxx` max: 184.757 ms (-3.11%), sum: 3147.361 ms (-0.35%)
- `crt.cxx [Clang 23]` max: 189.082 ms (-0.84%), sum: 3265.563 ms (+3.39%)
- `fft.cxx` max: 243.681 ms (+27.80%), sum: 4443.212 ms (+40.67%)
- `fft_naive.cxx` max: 362.395 ms (+90.05%), sum: 6829.863 ms (+116.23%)
- `naive.cxx [Clang 23]` max: 181.183 ms (-4.98%), sum: 3261.727 ms (+3.27%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
本题目标模数是 2^64。NTT 路线在几个合适的素数下计算，再用 CRT 重建；
另保留真正的浮点 FFT 路线。所有程序使用同一套最新批量读取与输出模板。

| 文件 | 实现 |
|---|---|
| main.cxx | 新版自适应三个至五个模数，融合 NTT＋CRT，含 AVX2 汇编 |
| crt.cxx | 新 CRT 的模数选择和向量归约＋旧 NTT 内核，无手写汇编 |
| naive.cxx | 历史固定五模数 NTT＋CRT |
| fft.cxx | 有符号分块，偶数/奇数系数打包，FFT 长度减半 |
| fft_naive.cxx | 相同分块思路，使用完整长度的复数 FFT |

`naive.cxx` 保留旧算法，更新的是 I/O。`fft_naive.cxx` 是新补的经典基线，
并非仓库曾存在的旧 FFT。FFT 两版都先计算各小块，再只合并影响低 64 位的项；
NTT/CRT 是整数精确还原，FFT 则需要控制舍入误差。

接口见 [convolution_u64.md](../../../include/toy/convolution_u64.md) 与
[convolution_fft_split.md](../../../include/toy/convolution_fft_split.md)。
直接将融合 NTT 换成 C++ intrinsics 较慢，因此 `crt.cxx` 选择旧 NTT 内核，
保留新版 CRT 优化。GCC 本轮两项刚好达标，sum 优势约 0.35%，差距很小；
主解仍保留优势更大的汇编版。Clang 23 对照使用与 GCC 相同的 libc/libm。
原 main 和六份用户参考的两轮记录继续复用。
