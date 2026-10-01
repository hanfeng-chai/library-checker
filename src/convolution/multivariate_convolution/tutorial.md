# 多维截断卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 465.935 (Rohan_Kapri), sum 2299.055 (Rohan_Kapri)。

- `main.cxx` max: 351.193 ms (-24.63%), sum: 1254.886 ms (-45.42%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
用下标颜色标记进位，在 NTT 频点上按颜色循环卷积，取回对应颜色以排除跨维进位。

实现、约束及验证见 [`multivariate_convolution.md`](../../../include/toy/multivariate_convolution.md)。

## 尝试过程与取舍

先按进位颜色做 NTT，频点上再按颜色循环卷积。发现全二元维度可直接转为子集卷积后加入分派；子集内核随后经历补齐布局、缩半 XOR、融合、工作区复用与清理等阶段，本题随之回归。保留一般颜色算法与全二元分支，各自处理适合的输入形状，而不是把整题固定为一种表示。
