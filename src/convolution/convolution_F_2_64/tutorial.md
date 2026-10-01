# GF(2^64) 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 969.192 (Anonymous), sum 18424.546 (Rohan_Kapri)。

- `main.cxx` max: 597.895 ms (-38.31%), sum: 12579.787 ms (-31.72%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
多项式系数在 GF(2^64) 中，用 XOR 加法和无进位乘法。通过固定 Cantor 基上的加法 FFT 求卷积。

实现、约束及验证见 [`convolution_gf64.md`](../../../include/toy/convolution_gf64.md)。

## 尝试过程与取舍

参考已有加法 FFT 提交，使用固定 Cantor 基，去掉随机找基、运行期求逆和额外归一化。短边直接相乘，刚好多一项会使变换翻倍时单独补最高项。固定基与普通域乘法保持接口统一；朴素域乘法、卷积对照先验证，再进行完整比较。
