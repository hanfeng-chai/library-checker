# 幂级数复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 35.528 (cmk666), sum 559.350 (cmk666)。

- `main.cxx` max: 13.037 ms (-63.31%), sum: 209.381 ms (-62.57%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
用首一分母 y-g(x) 隐式表示全部幂，每轮配对 x 和 -x，将 x 精度减半。
逆序应用系数提取的转置，得到外层系数加权的 f(g(x))。

两种数据规模共用 [fps_composition](../../../include/toy/fps_composition.md)，
单项式内层直接处理，其余用 radix-4 NTT 和转置 Bostan–Mori。

## 尝试过程与取舍

将全部内层幂隐式放进首一二元分母，使用转置 Bostan–Mori，复用 radix-4 NTT。小规模和单项式内层保留直接路径，普通版与 large 版使用同一库。现存完整对照首批即双项领先，之后复合逆的缩半投影改动没有改变本题产物。
