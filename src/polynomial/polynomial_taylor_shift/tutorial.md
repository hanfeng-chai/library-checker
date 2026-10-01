# 泰勒平移

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 75.963 (Rohan_Kapri), sum 1071.597 (Rohan_Kapri)。

- `main.cxx` max: 52.387 ms (-31.04%), sum: 741.896 ms (-30.77%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
将系数乘阶乘并倒序，与 c^i/i! 卷积，再除阶乘。

接口和验证见 [`polynomial.md`](../../../include/toy/polynomial.md)。

## 尝试过程与取舍

经典阶乘缩放把泰勒平移化为一次卷积。复用现成高性能卷积与缓冲区接口，避免另写变换；先用朴素二项式展开对照，再完成全量五份参考比较。
