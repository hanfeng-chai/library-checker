# convolution_u64

`convolution_u64(span<const u64> a, span<const u64> b)` 不修改输入，返回模 2^64
的整数卷积。头文件独立为 `convolution_u64.h`。要求 `min(N,M) <= 2^20`、
补齐长度不超过 2^24；短边不超过 16 时直接计算。时间 O((N+M) log(N+M))，
空间 O(N+M)。

使用 754974721、880803841、897581057、998244353、1004535809 五个素数，
根据 `min(N,M)*max(a)*max(b)` 的系数上界选三个、四个或五个模数。
通过先除模数乘积再比较来避免上界计算溢出。CRT 还原后取低 64 位，全程整数计算。

各模数调用 [`convolution_fast.h`](convolution_fast.md) 的融合 NTT。
输入拆成高低 32 位，用 AVX2 并行归约；三种 CRT 重建循环单独编译为热函数。
保留 `noinline,hot` 有实际原因：初版条件分支内联进大函数后，GCC 把一些常数取模
改成硬件除法，实测明显变慢；分离热循环后恢复常数乘法归约。

最快参考 toomer 使用有符号 13 位分块的 AVX2/FMA 浮点 FFT，仅计算会影响低
64 位的块组合。当前实现保留精确 NTT/CRT 路线，没有引入浮点舍入条件。
此接口也适用于哈希卷积、整数多项式等需要低 64 位乘积的场景。

本题 `crt.cxx` 同时定义 `TOY_NTT_ASM 0`、`TOY_CRT_FUSED_NTT 0`，使用新版
自适应 CRT、向量归约和旧 NTT 内核；`naive.cxx` 调用旧固定五模数版本。`fft.cxx`、`fft_naive.cxx` 是打包与
普通分块 FFT，见 [convolution_fft_split.md](convolution_fft_split.md)。各版本
数组读取和输出模板相同，不以旧 I/O 的耗时作为算法差距。

结果见 [本题 bench.txt](../../src/convolution/convolution_mod_2_64/bench.txt)，
方法见 [benchmark.md](../../tools/benchmark.md)。旧五模数版本和所有候选的原始记录
保存在 `bench/conv-revisit-20261001/` 与此前全量批次；旧参考不重复计时。

`TOY_CRT_FUSED_NTT` 默认值为 1；设为 0 可复用旧内核，且不改变 CRT 的
数学算法。此选项也适用于复用逐素数转换函数的 `convolution_crt_fast`。
GCC 的旧内核组合本轮为 184.757/3147.361 ms，双项低于参考，sum 优势约 0.35%。
