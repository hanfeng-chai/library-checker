# 多项式插值

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 129.883 (cmk666), sum 669.887 (Anonymous)。

- `main.cxx` max: 123.278 ms (-5.09%), sum: 655.905 ms (-2.09%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先求根多项式导数在各点的值，形成插值权重，再利用子积树合并。

接口与验证见 [`multipoint.md`](../../../include/toy/multipoint.md)。

## 尝试过程与取舍

先用导数求值和批量逆元形成插值权重，再沿子积树合并。与多点求值一起比较 32 点变换、较小叶子、根部求逆和叶子向量化；因为插值还包含一次求值，局部改进必须在整条流程上验证。最终保留混合根部策略及并行叶子处理。
