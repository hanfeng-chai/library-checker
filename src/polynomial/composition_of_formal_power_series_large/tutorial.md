# 幂级数复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 349.357 (Rohan_Kapri), sum 4944.779 (Rohan_Kapri)。

- `main.cxx` max: 282.524 ms (-19.13%), sum: 4012.163 ms (-18.86%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
用首一分母 y-g(x) 隐式表示全部幂，每轮配对 x 和 -x，将 x 精度减半。
逆序应用系数提取的转置，得到外层系数加权的 f(g(x))。

两种数据规模共用 [fps_composition](../../../include/toy/fps_composition.md)，
单项式内层直接处理，其余用 radix-4 NTT 和转置 Bostan–Mori。

## 尝试过程与取舍

与普通规模共用二元分母和转置 Bostan–Mori，不为 large 版另写专用算法。这里主要检查大规模的空间、变换长度和缓存行为；小规模朴素复合对照先确认转置方向，完整比较再验证实际收益。
