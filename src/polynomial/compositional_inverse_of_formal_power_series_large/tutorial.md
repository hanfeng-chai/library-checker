# 幂级数复合逆

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 323.908 (Rohan_Kapri), sum 2950.969 (cmk666)。

- `main.cxx` max: 285.177 ms (-11.96%), sum: 2333.825 ms (-20.91%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
固定 M=n-1，先求全部 [x^M]f(x)^i。Lagrange 公式将这些系数转换成
(x/g(x))^M 的前 M 项，取 -1/M 次幂即可恢复 g(x)/x。

两种规模共用 [fps_composition](../../../include/toy/fps_composition.md) 的幂投影，
再复用 fps 的分块求幂，省去反复调用复合的 Newton 迭代。

## 尝试过程与取舍

采用与普通规模相同的幂投影、Lagrange 公式和幂级数幂。half 候选专门减少投影所需工作，并在本题全量大数据上复核；最终选择同时改善 max、sum 的实现，而不是只按小规模表现推断。
