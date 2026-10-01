# convolution_crt

复用 `convolution.h` 的整数内核，在几个素数下分别卷积，再用 CRT 精确还原。
接口接收普通余数的 span，不修改输入，返回 Buffer。

- `convolution_crt<P>(a,b)`：P 小于 2^30，可为合数。使用
  998244353、1004535809、469762049 三个素数。要求
  `min(N,M)*(P-1)^2` 小于三者的乘积。
- 模 2^64 的 `convolution_u64(a,b)` 已独立到
  [`convolution_u64.h`](convolution_u64.md)，使用自适应模数个数和融合 NTT。
- `convolution_u64_basic(a,b)` 保留历史固定五模数版本，供模 2^64 的
  `naive.cxx` 对照使用；同样要求 `min(N,M)<=2^20`、补齐长度不超过 2^24。

本接口的变换长度须不超过 2^24。短边不超过 16 时直接计算。
复杂度 O((N+M) log(N+M))，空间 O(N+M)，没有浮点舍入。

旧版 NTT 和 CRT 都保留，并换用与主解相同的最新 I/O。模 998244353 的
`naive.cxx` 调用单模数 `convolution`，只有非 998 两题才使用这里的 CRT 接口。

以下是 2026-09-30 的旧五模数版本记录；当前模 2^64 的数值见
[本题 bench.txt](../../src/convolution/convolution_mod_2_64/bench.txt)。
当时 Lenovo 独立空闲、不绑核，非样例各三次取中位数，单位 ms：

| 模 2^64 解答 | max | sum |
| --- | ---: | ---: |
| main | 198.154 | 3488.296 |
| Qwerty1232 | 203.653 | 3619.971 |
| Rohan_Kapri | 207.314 | 3650.453 |
| adamant | 207.716 | 3646.630 |
| tayu0110 | 395.293 | 6938.068 |
| Yuezheng_Ling_fans | 544.394 | 10058.809 |

模 2^64 的两项汇总均胜出。模 10^9+7 的 CRT 初版未达到性能目标：
104.243/2376.132 ms，随后主解改用通过验收的 [`fft_convolution.h`](fft_convolution.md)。
CRT 保留作为精确接口、测试对照及回退路径。

两题全部官方测例通过 GCC/Clang；另外通过 936 项普通模数、312 项 u64 的
整数卷积对照以及 ASan/UBSan。原始结果在 `bench/crt-20260930/round1/`。

加入共用接口后的重新编译也已复核：模 2^64 单次 max/sum 为
197.968/3494.852 ms，仍胜过五份对照，记录在 `bench/conv-final-20260930/round1/`。

`CyclicCRT(b,p)` 缓存固定右操作数的三个频谱，随后用 `plan(a)` 计算循环卷积。
两边长度相同，是 `[64,2^24]` 内的二次幂，`1 < p < 2^30`。
归约按普通整数系数精确还原，允许运行期模数，供 chirp-z 等重复乘法复用。
