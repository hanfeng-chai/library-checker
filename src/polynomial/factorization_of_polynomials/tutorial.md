# 有限域多项式分解

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 376.744 (anon123), sum 2936.727 (anon123)。

- `main.cxx` max: 28.769 ms (-92.36%), sum: 261.373 ms (-91.10%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
无平方分解确定重数；gcd(f,x^(p^d)-x) 找出次数为 d 的不可约因子的乘积。
同次数部分通过随机范数（奇特征）或迹（特征 2）反复二分。
零导数时提取 p 次根，递归后将重数乘 p。

[polynomial_factorization](../../../include/toy/polynomial_factorization.md) 缓存 Frobenius
映射，避免在每次分组和递归中重新计算长指数幂。67 个官方测例通过 GCC/Clang。

## 尝试过程与取舍

先实现无平方、不同次数与同次数分解，并缓存 Frobenius 映射，避免重复计算大指数模幂。次数下降的主要算术复用半 GCD 和固定除数多项式模幂。用乘回原式及不可约性检查验证分解，首批完整比较双项领先。
