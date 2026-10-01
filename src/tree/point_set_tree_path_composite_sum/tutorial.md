# 任意根动态仿射和

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 684.357 (Rohan_Kapri), sum 10527.322 (Rohan_Kapri)。

- `main.cxx` max: 608.081 ms (-11.15%), sum: 8306.426 ms (-21.10%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 tree_affine_updates.h 的 Reroot 模式。重链段缓存正反向仿射聚合，查询根变化时汇集目标路径两侧的子树贡献，无需改变树形。

26 个正式测例通过 GCC/Clang，独立朴素对照通过 ASan/UBSan。

## 尝试过程与取舍

首版扩展点/边节点的 Link-cut Tree，额外维护反向矩阵以支持任意根，重复旋转和聚合较多。随后改为固定拓扑的加权重链树，共享正反向斜率积和点数；查询收集目标路径两侧贡献，跨轻边时排除已覆盖的部分。与固定根版本共用更新结构，并重新核对实际生产产物。
