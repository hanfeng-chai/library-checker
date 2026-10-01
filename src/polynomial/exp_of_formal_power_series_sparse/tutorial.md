# 稀疏幂级数指数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 76.597 (adamant), sum 819.851 (urectanc)。

- `main.cxx` max: 64.981 ms (-15.16%), sum: 379.018 ms (-53.77%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
由 g'=f'g 得到递推，处理第 i 项时才除以 i，再向后传播。

接口、复杂度及验证见 [`fps_sparse.md`](../../../include/toy/fps_sparse.md)。

## 尝试过程与取舍

从 g'=f'g 的经典系数递推出发，只沿非零项传播贡献，并延后次数除法。这样利用稀疏性而不分配完整卷积工作区；在朴素递推及正式测例通过后完成全量比较。
