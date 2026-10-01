# 多项式序列乘积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 199.943 (Rohan_Kapri), sum 2736.343 (Rohan_Kapri)。

- `main.cxx` max: 173.821 ms (-13.06%), sum: 2361.756 ms (-13.69%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先折叠常数因子，其余多项式按总次数平衡分治。中间结果放在连续工作区中，
整棵树复用两块 NTT 缓冲，减少反复分配。

短因子直接计算；乘积次数恰为二次幂时使用循环卷积，再单独恢复最高项，
避免仅因多一个系数就将变换长度翻倍。最终乘回所有常数因子的乘积。
实现与验证见 [product_pool](../../../include/toy/product_pool.md)。

## 尝试过程与取舍

首版按总次数平衡分治并折叠常数因子。随后采用 pool 连续工作区、复用两块 NTT 缓冲，减少递归过程的反复分配；乘积次数刚好落在二次幂边界时，循环卷积后单独补最高项，避免变换翻倍。
