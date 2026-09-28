# Primitive Root / 原根

## 中文

### 什么是原根

模素数 `p` 的非零剩余构成大小为 `p-1` 的循环群。若 `g` 的幂可以
遍历全部非零剩余，就称 `g` 是原根；等价地，`g` 的阶恰好为 `p-1`。

### 判定一个候选

先分解 `p-1`，只保留不同的质因子 `q`。候选 `g` 是原根，当且仅当

`g^((p-1)/q) != 1 (mod p)`

对每个 `q` 都成立。原因是：若 `g` 的阶是 `p-1` 的真因子，它一定
整除某个 `(p-1)/q`；反过来，排除全部这些最大真因子后，阶只能是
`p-1`。

### 实现与工程选择

`factor_order_search.cpp` 共享 `toy/factor.hpp` 的确定性 Miller--Rabin、
Brent Pollard--Rho 和 Montgomery 模幂。没有另写题目专用分解器，避免维护
两套容易漂移的实现。`p=2` 时非零群只有元素 `1`，单独返回 `1`。

先前两个入口调用完全相同的公共函数，仅 Writer 不同，现合并为一份；这比把
空白格式差异包装成“naive 解法”更诚实。Reader 使用 direct mapping，
`Q<=100` 走三位路径，`p<=10^18` 走十九位路径。

### 复杂度

主要成本是分解 `p-1`。得到不同质因子后，每个候选做至多
`omega(p-1)` 次 `O(log p)` 模幂；实践中最小原根通常很小。

## English

The multiplicative group modulo prime `p` has order `p-1`. After factoring
`p-1`, a candidate `g` is primitive iff
`g^((p-1)/q) != 1 mod p` for every distinct prime divisor `q`.

`factor_order_search.cpp` shares the robust
Miller--Rabin/Pollard--Rho/Montgomery library. Former duplicate adapters that
differed only in Writer whitespace were merged. Direct mapping uses the
three-digit path for `Q` and nineteen-digit path for `p`. Factoring `p-1`
dominates the running time.
