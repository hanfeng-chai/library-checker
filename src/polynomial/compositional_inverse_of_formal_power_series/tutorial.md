# 幂级数复合逆

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 38.946 (cmk666), sum 416.877 (cmk666)。

- `main.cxx` max: 13.224 ms (-66.04%), sum: 133.772 ms (-67.91%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
固定 M=n-1，先求全部 [x^M]f(x)^i。Lagrange 公式将这些系数转换成
(x/g(x))^M 的前 M 项，取 -1/M 次幂即可恢复 g(x)/x。

两种规模共用 [fps_composition](../../../include/toy/fps_composition.md) 的幂投影，
再复用 fps 的分块求幂，省去反复调用复合的 Newton 迭代。

## 尝试过程与取舍

先用幂投影和 Lagrange 公式把复合逆变为一次级数幂，避免反复进行整个复合的 Newton 迭代。随后比较 half 投影候选，缩短所需投影变换；采用候选后重新检查两个复合逆规模，并确认普通复合的二进制没有改变。
