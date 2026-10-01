# polynomial_gcd

`polynomial_inv_mod<P>(f,g)` 求满足 `f*h=1 mod g` 的唯一低次多项式。
输入系数为普通余数，g 非零，非空输入的最高项非零；无逆返回 `nullopt`。
常数模多项式返回空数组表示零多项式。

半 GCD 先在高次部分计算欧几里得变换矩阵，再作用回完整多项式，
精确消元后处理另一半。矩阵乘法共享输入 NTT，两个乘积相加后统一逆变换。
最后将常数 GCD 正规化，取 Bézout 系数。复杂度 O(N log² N)。

18 个官方测例通过 GCC/Clang，独立长除 GCD、逆的乘积余数对照通过。
短商长除使用向量模乘，矩阵的短乘积直接减入目标，减少临时分配。
ASan/UBSan 通过。Lenovo 每例一次，max/sum 为 **183.813/1193.355 ms**，
五份对照最小值为 196.592/1276.298 ms，均已达标。
证据在 `bench/poly-gcd-final-20260930/round1/` 的 poly-gcd-vector 候选。

`polynomial_gcd(std::move(a),std::move(b))` 返回首一 GCD（两者皆零时为空），
复用同一半 GCD 递归，不维护不需要的 Bézout 系数。
