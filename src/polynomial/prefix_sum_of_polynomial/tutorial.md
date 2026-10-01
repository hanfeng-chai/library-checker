# 多项式前缀和

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 335.945 (adamant), sum 1449.538 (kk2a)。

- `main.cxx` max: 84.083 ms (-74.97%), sum: 359.760 ms (-75.18%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
求 g，使 g(0)=0 且 g(x+1)-g(x)=f(x)，于是 g(k) 为 f(0)..f(k-1) 的和。
先求 x/(exp(x)-1) 的系数，得到 Bernoulli 数的阶乘缩放形式，
再用一次阶乘缩放卷积恢复 g 的各项。

实现与次数约束见 [polynomial](../../../include/toy/polynomial.md)。

## 尝试过程与取舍

使用 Bernoulli 数的阶乘缩放形式，将离散前缀和转换成一次卷积。复用 exp、求逆和卷积接口，重点验证 g(0)=0 与 g(x+1)-g(x)=f(x)，首批完整比较即达标。
