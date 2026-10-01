# 几何序列多点求值

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 80.042 (Rohan_Kapri), sum 1114.627 (QedDust413)。

- `main.cxx` max: 53.448 ms (-33.23%), sum: 597.853 ms (-46.36%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
利用 ij=C(i+j,2)-C(i,2)-C(j,2) 构造循环卷积，统一处理任意公比。

接口和验证见 [`polynomial.md`](../../../include/toy/polynomial.md)。

## 尝试过程与取舍

用二次指数恒等式将几何点求值转为循环卷积，复用精确多项式算术。公比为零及已知位置等边界单独处理；此题与同库的平移、除法一同验证，再进行完整对照。
