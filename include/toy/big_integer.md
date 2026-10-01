# big_integer

`BigInteger<false>` 以 10^8 为基数保存十进制整数，`BigInteger<true>` 以 2^64
为基数保存十六进制整数。`assign(string_view)` 解析带可选负号的有效整数，
`assign(i128)` 直接赋值；零统一为非负，数组低位在前。

`add(a,b,out)`、`multiply(a,b,out,workspace)` 支持 out 与输入重合，独立输出时复用原有容量。
`write(writer,end)` 批量写入 toy::Writer。十六进制输出大写字母。

十进制一次解析 32 字符；加减用八肢 SIMD 进位/借位前缀，完整长链同样正确。
十六进制每 16 字符直接转换为一个机器字，加减使用机器进位指令。
小乘数用朴素乘法，大数拆成十进制 4 位或二进制 14 位系数，使用
[fft_integer](fft_integer.md) 的打包实数卷积。连续乘法传入复用的 `IntegerFFT*`，
可省去反复生成根表与申请工作区；平方复用同一份频谱。普通乘积超过已验证的
浮点长度范围时，退回 [convolution_integer](convolution_integer.md) 的双模精确卷积。

`small_hex` 只供带 Reader 零填充的 token 使用，最多 31 个有效数字；
两个这样的带符号值之和能放进 i128。`small_decimal` 根据已知长度解码最多 18 位十进制数。

已通过 GCC/Clang 官方加法、乘法测例；Python 对照覆盖两种进制、正负零、
进位链、朴素/FFT 乘法阈值、原位结果和文件页边界。ASan/UBSan 通过。

静默单次测量（ms，`bench/big-final-20260930/round1/`）：

| 题目 | main max / sum | 五份提交的最佳 max / sum |
| --- | ---: | ---: |
| 十进制加法 | 15.527 / 110.341 | 15.629 / 163.581 |
| 十六进制加法 | 11.428 / 91.045 | 14.713 / 151.670 |
| 十进制乘法 | 49.799 / 564.622 | 52.822 / 818.016 |
| 十六进制乘法 | 35.934 / 503.340 | 36.771 / 558.559 |
| 十进制除法 | 65.647 / 963.885 | 75.433 / 1293.854 |
| 十六进制除法 | 59.850 / 699.991 | 60.816 / 728.304 |

除法接口见 [big_division](big_division.md)，全库证据见 [评测审计](../../tools/measurement_audit.md)。
