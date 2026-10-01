# Enumerate Primes / 素数枚举

## 中文

题目不仅要求 `pi(N)`，还只输出下标为 `B, A+B, 2A+B, ...` 的素数。
`N` 可达五亿，但输出数量最多一百万。

### 解法一：奇数分段 Eratosthenes（`odd_segmented_sieve.cpp`）

先筛出 `sqrt(N)` 内的小素数，再按固定大小处理奇数区间。对每个小
素数，从当前区间内第一个奇倍数开始标记。段筛只需约 1 MiB 工作
内存，结构接近课本 Eratosthenes，复杂度 `O(N log log N)`。

该版本简单、低内存并能 AC，但会访问每个素数并计算 `index % A`，
在最大数据上较慢。

### 解法二：混合 mod-30 wheel（`hybrid_wheel30_sieve.cpp`）

只保存模 30 余数为 `1,7,11,13,17,19,23,29` 的数，空间压缩到
`8/30`。

1. 小素数的效果合成为不超过 1 MiB 的周期 mask，用 AVX2 成块 AND；
2. 中素数在 128 KiB 压缩块内标记，保持缓存局部性；
3. 每个素数的八种轮状态预计算 ordinal 步长，热循环没有除法；
4. 最后用 64 位 `popcount` 得到 `pi(N)`；
5. 用 BMI2 `pdep` 按秩直接选第 `B, A+B, ...` 个置位，不遍历全部
   约 2635 万个素数。

库中还保留 dense odd、direct wheel、Atkin 和 segmented 四个算法族，
因为不同上限、内存和输出模式下最优策略不同。

两份源码均使用 direct mapping 和九位 value-uniform 输入路径，输出统一
使用 CompactWriter。曾对最大九位输出接 bounded 格式器，完整题目 benchmark
从约 248 ms 回退到 267 ms，因此保留通用 compact 路径。

#

## 正确性与复杂度

30-wheel 只预先排除 `2,3,5` 的倍数；其余合数一定有不超过平方根的
素因子，并会被 dense 或 sparse 阶段清除。未清除候选因此恰为素数。
时间 `O(N log log N)`，主位图约 `N*8/30` 位。

## English

`odd_segmented_sieve.cpp` is a textbook odd segmented sieve with low memory and
`O(N log log N)` work.

`hybrid_wheel30_sieve.cpp` compresses candidates with a mod-30 wheel, applies periodic
small-prime masks using AVX2, marks larger primes in cache-sized blocks, counts
with popcount, and selects requested prime ranks with BMI2 `pdep`. Several
other sieve families remain available because no single mechanism dominates
all limits and output shapes.

Both use direct mapping, the nine-digit value-uniform input policy, and the
same CompactWriter. A bounded output path was rejected after an end-to-end
regression from roughly 248 ms to 267 ms.
