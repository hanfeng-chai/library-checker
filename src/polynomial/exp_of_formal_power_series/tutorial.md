# 幂级数指数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 94.366 (Rohan_Kapri), sum 1113.005 (Anonymous)。

- `main.cxx` max: 80.507 ms (-14.69%), sum: 965.004 ms (-13.30%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
同时维护 g=exp(f) 和倒数前缀；只计算残差的新高段并复用频谱。

接口、复杂度及验证见 [`fps.md`](../../../include/toy/fps.md)。

## 尝试过程与取舍

采用同时维护 exp 与倒数前缀的 Newton 倍增，只计算新高半段残差，并复用已有频谱。正确性用朴素系数递推和 exp/log 关系验证；完整对照首批达到两项标准，因此保留这一较集中的实现。
