# 多项式求根

先提取零根，再以 gcd(f,x^(P-1)-1) 得到非零根对应的首一无重因子。
随机平移后，用 Euler 判别将根集分成两组，递归直至线性或二次因子。

实现复用 [polynomial_roots](../../../docs/polynomial_roots.md)、固定除数模幂和半 GCD。
