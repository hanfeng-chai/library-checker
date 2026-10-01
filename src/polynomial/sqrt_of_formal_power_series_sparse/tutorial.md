# 稀疏幂级数平方根

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 135.484 (JustinRochester), sum 734.551 (JustinRochester)。

- `main.cxx` max: 75.330 ms (-44.40%), sum: 387.706 ms (-47.22%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先检查前导次数和系数的可开方性，再将幂递推中的指数设为 1/2。

接口、复杂度及验证见 [`fps_sparse.md`](../../../include/toy/fps_sparse.md)。

## 尝试过程与取舍

把可开方性检查与前导次数处理保留在外层，内部复用指数为 1/2 的稀疏幂递推。之后比较 dot 候选的内积实现，与稀疏整数幂一起确认共享改动的收益与边界正确性。
