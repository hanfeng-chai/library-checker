# 多点求值

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 91.626 (Rohan_Kapri), sum 497.131 (Rohan_Kapri)。

- `main.cxx` max: 90.127 ms (-1.64%), sum: 493.156 ms (-0.80%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
建立反向子积树，在根求一次级数商，再用相关乘积向下传递权重。

接口与验证见 [`multipoint.md`](../../../include/toy/multipoint.md)。

## 尝试过程与取舍

先构造反向子积树并转置向下传播，避免每节点重复求商余。依次比较完整 32 点变换、根部求逆后卷积、八点叶子、叶子批量处理与向量化。单项调整仍未达标后，将根部除法/求逆按实际长度分派，并行处理叶子的 Horner 链，形成最终混合方案。早期尝试把部分变换直接降到 32 点没有通过恒等式检查，修正后才进入性能表。
