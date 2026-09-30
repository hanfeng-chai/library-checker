# 多项式前缀和

求 g，使 g(0)=0 且 g(x+1)-g(x)=f(x)，于是 g(k) 为 f(0)..f(k-1) 的和。
先求 x/(exp(x)-1) 的系数，得到 Bernoulli 数的阶乘缩放形式，
再用一次阶乘缩放卷积恢复 g 的各项。

实现与次数约束见 [polynomial](../../../docs/polynomial.md)。
