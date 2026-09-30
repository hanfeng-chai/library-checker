# 幂级数复合

用首一分母 y-g(x) 隐式表示全部幂，每轮配对 x 和 -x，将 x 精度减半。
逆序应用系数提取的转置，得到外层系数加权的 f(g(x))。

两种数据规模共用 [fps_composition](../../../docs/fps_composition.md)，
单项式内层直接处理，其余用 radix-4 NTT 和转置 Bostan–Mori。
