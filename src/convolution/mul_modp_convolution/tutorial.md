# 素数模下标的乘法卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 127.773 (Rohan_Kapri), sum 783.932 (adamant)。

- `main.cxx` max: 51.674 ms (-59.56%), sum: 333.523 ms (-57.46%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
用原根指数将非零下标的乘法转换成循环卷积，零下标单独统计。

接口、约束及验证见 [`multiplicative_convolution.md`](../../../include/toy/multiplicative_convolution.md)。

## 尝试过程与取舍

经典做法是用原根指数把非零下标的乘法变为循环卷积，再单独统计零下标。复用已验证的原根、Barrett 与普通卷积实现，减少专用代码。首批完整比较已达标，后续保留作为共享卷积的使用案例。
