# 幂级数复合逆

固定 M=n-1，先求全部 [x^M]f(x)^i。Lagrange 公式将这些系数转换成
(x/g(x))^M 的前 M 项，取 -1/M 次幂即可恢复 g(x)/x。

两种规模共用 [fps_composition](../../../docs/fps_composition.md) 的幂投影，
再复用 fps 的分块求幂，省去反复调用复合的 Newton 迭代。
