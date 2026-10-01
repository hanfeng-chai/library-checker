# 稀疏幂级数对数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 71.958 (adamant), sum 848.031 (urectanc)。

- `main.cxx` max: 45.577 ms (-36.66%), sum: 311.345 ms (-63.29%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先向前代入求 x*f'/f，再按次数积分；跳过零贡献。

接口、复杂度及验证见 [`fps_sparse.md`](../../../include/toy/fps_sparse.md)。

## 尝试过程与取舍

直接前代入计算 x*f'/f，最后按次数积分，跳过零项贡献。它保留了经典递推便于检查，算术及 I/O 复用统一库；与稠密接口互相对照后进行完整评测。
