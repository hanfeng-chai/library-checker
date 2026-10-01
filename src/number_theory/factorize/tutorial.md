# Factorize / 64 位整数质因数分解

## 中文

### 从“判素”到“找因子”

若 `n=1`，分解为空；若 Miller--Rabin 判断 `n` 为素数，就直接加入
答案。否则需要找到一个严格位于 `1` 和 `n` 之间的因子，再递归分解
两半。

Pollard--Rho 在模 `n` 的序列 `x <- x^2+c` 上寻找环。若两个位置在模
某个未知素因子 `p` 下相遇，则它们的差被 `p` 整除，因而
`gcd(|x-y|,n)` 很可能给出非平凡因子。

### Brent 批处理（`pollard_rho_brent.cpp`）

库内使用 Brent 的分段推进方式，并把最多 128 个差值先相乘，最后只
做一次 GCD。模乘使用 Montgomery 表示；若批量乘积与 `n` 的 GCD
恰好为 `n`，再逐项回退定位。SplitMix64 根据 `n` 产生可复现的重启
参数，避免固定多项式被构造数据卡住。

高性能入口还选择适合结构化整数列表的输出策略。最终因子排序后输出，
因此满足题目要求。

这里只保留这一份正式解法。先前两份入口调用完全相同的 `factorize`，区别
仅在空白输出格式，不能算两种算法。朴素 Floyd Rho 在官方固定随机数攻击
数据上会严重退化，也不应为了“多解”而保留。

输入使用 direct mapping；`Q<=100` 走三位 scalar 路径，整数
`n<=10^18` 走十九位 SIMD 路径。因子个数使用六位以下专用输出，质因子使用
通用 64 位 Writer。I/O 与数学核心均集中在公共库中。

#

## 正确性与复杂度

递归只在确定素数时终止；每次非平凡因子 `d` 都满足 `d | n`，所以
两棵子树的乘积始终等于原数。期望寻找大小约为 `p` 的因子需要
`O(sqrt(p))` 次迭代，递归深度不超过质因子个数。

## English

Pollard--Rho searches the sequence `x^2+c mod n`. A collision modulo an
unknown prime factor makes `gcd(|x-y|,n)` reveal a divisor. The implementation
uses Brent batching, Montgomery multiplication, deterministic SplitMix64
restarts, and Miller--Rabin leaves.

`pollard_rho_brent.cpp` is the single production variant. Two former adapters
called the identical core and differed only in whitespace, so retaining both
would not constitute multiple algorithms. A basic Floyd Rho is also omitted
because fixed-RNG adversarial cases make it unreliable.

The direct-mapped Reader uses the three-digit path for `Q` and the
nineteen-digit SIMD path for values up to `10^18`. Expected work is roughly
`O(sqrt(p))` for the smallest non-trivial factor `p`.
