# 任意多项式模逆

逆存在当且仅当 f、g 互素；扩展 Euclid 求 u*f+v*g=d，d 为非零常数时
答案为 u/d。g 为常数时按题意输出零多项式。

[polynomial_gcd](../../../docs/polynomial_gcd.md) 用半 GCD 降低次数，
将主要工作转成共享 NTT 的多项式矩阵乘法。
