# 有限域多项式分解

无平方分解确定重数；gcd(f,x^(p^d)-x) 找出次数为 d 的不可约因子的乘积。
同次数部分通过随机范数（奇特征）或迹（特征 2）反复二分。
零导数时提取 p 次根，递归后将重数乘 p。

[polynomial_factorization](../../../docs/polynomial_factorization.md) 缓存 Frobenius
映射，避免在每次分组和递归中重新计算长指数幂。67 个官方测例通过 GCC/Clang。
