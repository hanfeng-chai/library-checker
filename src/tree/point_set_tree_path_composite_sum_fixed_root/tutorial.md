# 固定根动态仿射和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 373.642 (urectanc), sum 5993.891 (urectanc)。

- `main.cxx` max: 290.625 ms (-22.22%), sum: 4185.037 ms (-30.18%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 tree_affine_updates.h，以轻子树大小加权的重链二叉树维护动态 DP。点或边修改只沿静态结构向上传播，轻子树贡献用新旧差更新。

26 个正式测例通过 GCC/Clang，独立朴素对照通过 ASan/UBSan。

## 尝试过程与取舍

首版把边拆成额外节点，用 Link-cut Tree 维护作用于和、点数的矩阵，虽然正确，但固定拓扑并不需要动态旋转。改为按轻子树大小加权的静态重链树，顶点吸收父边，跨轻边只更新贡献的新旧差。完整数据中两项均明显下降；本机运行库更新后，又以实际生产产物完整复核。
