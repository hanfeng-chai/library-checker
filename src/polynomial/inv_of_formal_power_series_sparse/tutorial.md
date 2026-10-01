# 稀疏幂级数求逆

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 44.787 (sakikuroe), sum 379.500 (sakikuroe)。

- `main.cxx` max: 35.025 ms (-21.80%), sum: 192.880 ms (-49.18%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
将常数项归一化后向前代入，只从非零系数传播贡献。

接口、复杂度及验证见 [`fps_sparse.md`](../../../include/toy/fps_sparse.md)。

## 尝试过程与取舍

将常数项归一化后做前代入，只从非零系数传播贡献。与稠密求逆共享 I/O 和余数表示，保留稀疏递推的结构。正式测例和小规模朴素系数对照通过后，首批完整测量达到目标。
