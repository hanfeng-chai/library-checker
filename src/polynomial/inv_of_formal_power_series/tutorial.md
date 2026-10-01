# 形式幂级数求逆

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 55.725 (QgQ), sum 668.629 (QgQ)。

- `main.cxx` max: 42.971 ms (-22.89%), sum: 516.150 ms (-22.80%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
已知 `fG=1 mod x^m`，令 E 为 fG-1 的高 m 项，新的高半段就是 `-GE`。
长度 2m 的循环卷积只会污染低半段，所以保留所需高段即可，G 的频谱可复用。

实现与验证见 [`fps.md`](../../../include/toy/fps.md)。
原来迁移的 [`analysis.md`](../../../submissions/polynomial/inv_of_formal_power_series/analysis.md)
还说明了最快提交的三次乘积恢复等方案；目前较简单的实现已达到性能要求。

## 尝试过程与取舍

以经典 Newton 倍增建立基线，利用循环卷积只污染低半段的性质，只恢复所需高半段，复用旧逆的频谱。第二轮比较 fused 候选，融合相关步骤；旧分析中还有更复杂的乘积恢复方案，本轮先保留已经达标的实现。
