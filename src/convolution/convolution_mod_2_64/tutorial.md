# 模 2^64 卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 190.680 (toomer), sum 3158.567 (toomer)。

- `main.cxx` max: 199.151 ms (+4.44%), sum: 3498.189 ms (+10.75%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
用五个素数下的 NTT 和 CRT 还原整数系数，再利用无符号整数溢出取模 2^64。

接口、约束及验证见 [`convolution_crt.md`](../../../include/toy/convolution_crt.md)。

## 尝试过程与取舍

使用五个素数下的精确 NTT/CRT，再取低 64 位。后续卷积内核调整时，把旧产物作为 old 对照重新测量，检查共享修改是否回退。本题没有使用浮点近似代替模 2^64 的精确结果；最终取舍以同批完整测例的两项汇总为准。
