# 幂级数平方根

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 58.901 (Rohan_Kapri), sum 573.713 (Rohan_Kapri)。

- `main.cxx` max: 57.010 ms (-3.21%), sum: 480.915 ms (-16.17%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
检查前导零与首项的可开方性，用 Newton 更新根的新高段，同时维护其倒数。

接口、约束及验证见 [`fps.md`](../../../include/toy/fps.md)。

## 尝试过程与取舍

检查前导零和首项平方根后，使用 Newton 更新新高半段并维护倒数。与朴素系数递推及平方后截断的结果对照，统一处理不存在平方根的情形，再进行完整比较。
