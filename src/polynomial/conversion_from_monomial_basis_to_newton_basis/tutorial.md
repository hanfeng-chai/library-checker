# 单项式基转牛顿基

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 335.123 (Rohan_Kapri), sum 2506.259 (Rohan_Kapri)。

- `main.cxx` max: 152.898 ms (-54.38%), sum: 1160.028 ms (-53.71%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
建立点列对应的子积树。用左半点列的根多项式将 f 分成商与余数：
余数递归得到低段牛顿系数，商递归得到高段系数；叶子使用综合除法。
牛顿基只依赖点的顺序，不要求点互异。

实现复用 `Multipoint::to_newton`，接口与验证见 [multipoint](../../../include/toy/multipoint.md)。

## 尝试过程与取舍

先用子积树的商余分解得到牛顿基系数。后续与多点求值、插值共同比较叶子向量化、根节点求逆路径和混合策略，再随半 GCD 与短除法优化回归。叶子的综合除法保持直接形式；批次数据用于判断共享修改是否真的帮助本题。
