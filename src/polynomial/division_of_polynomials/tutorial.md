# 多项式除法

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 69.970 (Anonymous), sum 882.324 (Rohan_Kapri)。

- `main.cxx` max: 59.722 ms (-14.65%), sum: 718.255 ms (-18.60%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
倒序后用分块幂级数除法得到商，再用低段乘积求余数；短除数或短商直接长除。

接口、约束及验证见 [`polynomial.md`](../../../include/toy/polynomial.md)。

## 尝试过程与取舍

首版倒序求逆后相乘，再用低段乘积恢复余数。随后采用分块级数除法以复用频谱，短除数、短商保持直接长除；最后共享短除法的向量化改动再次验证。保留商乘除数加余数等于原式及余数次数界的独立检查。
