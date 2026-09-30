# primitive_root

`primitive_root(p)` 返回 32 位素数 p 的一个原根。先试除分解 p-1，
再检查候选 g 对每个不同素因子 q 是否满足 `g^((p-1)/q) != 1 (mod p)`。
p=2 返回 1。输入须为素数；不额外做素性检查。

本接口用于素数下标的乘法卷积。旧 `factor.hpp` 已有 `primitive_root_prime` 和分解能力，均仍保留；
百万规模 p 不需要引入其 Pollard–Rho 实现。
