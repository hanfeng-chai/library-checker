# 多项式与集合幂级数复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 472.711 (adamant), sum 2051.937 (adamant)。

- `main.cxx` max: 370.974 ms (-21.52%), sum: 1557.431 ms (-24.10%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
逐个加入满足 x²=0 的变量。维护外层多项式各阶导数在当前低半段 A 处的值，
由 f^(r)(A+xB)=f^(r)(A)+x B f^(r+1)(A) 得到新高半段。
初值为各阶导数在输入空集系数处的值，每轮减少需要维护的导数个数；
最高阶导数为常数时直接缩放，其余乘法用子集卷积。

接口、复杂度和验证见 [set_series](../../../include/toy/set_series.md)。

## 尝试过程与取舍

先把多项式复合归结到集合幂级数运算，共享 ranked 子集卷积。之后随底层内核测试补齐、缩半 XOR、融合和工作区复用。最终保留统一实现，使复合与 exp/log 的优化来自同一套已验证的算术与存储布局。
