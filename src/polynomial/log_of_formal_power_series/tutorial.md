# 幂级数对数

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 76.721 (Anonymous), sum 828.973 (Rohan_Kapri)。

- `main.cxx` max: 74.444 ms (-2.97%), sum: 788.521 ms (-4.88%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
利用 log(f) 的导数等于 f'/f，求逆、卷积后积分。

接口、复杂度及验证见 [`fps.md`](../../../include/toy/fps.md)。

## 尝试过程与取舍

从 f'/f 后积分的经典公式开始，先比较 Montgomery 算术路径，再比较分块级数除法。优化集中在复用求逆和乘积频谱、减少重复变换；选择依据是本题实际完整测例的 max、sum。
