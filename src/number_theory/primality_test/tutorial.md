# Primality Test / 64 位整数素性测试

## 中文

### 题意

对每个不超过 `10^18` 的正整数，判断它是不是素数。逐个试除到
`sqrt(n)` 最坏要做十亿次除法，显然不可行。

### 解法一：串行 Miller--Rabin（`serial_miller_rabin.cpp`）

先用 `2,3,...,37` 这些小素数试除。对剩下的奇数，把
`n-1` 唯一写成 `d * 2^s`，其中 `d` 是奇数。对一个底数 `a` 计算
`x=a^d mod n`：若 `x` 既不是 `1` 也不是 `-1`，就反复平方至多
`s-1` 次；始终没有得到 `-1` 就能证明 `n` 是合数。

一般的 Miller--Rabin 是概率算法，但对 64 位无符号整数，固定七个
底数 `2, 325, 9375, 28178, 450775, 9780504, 1795265022` 已被证明足以
确定性判断。因此这里没有误判概率。

### 解法二：并行 witness（`parallel_miller_rabin.cpp`）

七个底数的模幂互不依赖。高性能版本在扫描指数二进制位时同时更新
七组 Montgomery 数，让乱序执行器交错处理多条乘法依赖链。素数输入
必须跑完全部底数，因此这种写法尤其适合本题的 prime-heavy 数据。

`toy/mod64.hpp` 用 Montgomery reduction 把热循环中的 `% n` 替换为
乘法、移位和条件减法。串行版本仍保留给 Pollard--Rho：合数通常第一
个底数就能退出，强行并行反而浪费计算。

#

## 正确性与复杂度

小素数试除不会错过任何对应因子。七底数定理保证：通过全部测试的
64 位整数一定是素数。每个底数需要 `O(log n)` 次模乘，额外空间
`O(1)`。

两份源码都使用 direct-mapped Reader。查询数按六位上界读取，待测数按
`10^18` 的十九位 value-uniform 路径读取；输出均为固定字符串。串行版本只
简化 witness 调度，没有使用较慢 I/O。当前项目保留无大哈希表的通用七底数
方案；这是有意的通用性取舍，而不是遗漏榜首技巧。

## English

For odd `n`, write `n-1=d*2^s` with odd `d`. A Miller--Rabin witness proves
compositeness unless `a^d` is `1`, `-1`, or reaches `-1` during the squaring
chain. The seven listed bases are deterministic over the complete `uint64_t`
domain.

`serial_miller_rabin.cpp` tests witnesses serially and can reject composites early.
`parallel_miller_rabin.cpp` evaluates seven Montgomery exponentiation chains together, exposing
instruction-level parallelism on prime-heavy input. Both take `O(log n)` work
per witness and `O(1)` memory.

Both use direct mapping, a six-digit query-count policy, and the measured
nineteen-digit path for values up to `10^18`; only witness scheduling differs.
The repository deliberately keeps the general seven-base method instead of a
large problem-specific witness hash table.
