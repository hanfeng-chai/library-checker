# K-th Root (Integer) / 整数 k 次根

## 中文

目标是求最大的整数 `r`，满足 `r^k <= n`。浮点 `pow` 只能给估计，
不能直接作为答案：在完全幂附近，哪怕一个 ulp 的误差也会 WA。

### 解法一：二分答案（`binary_search.cpp`）

在 `[0,n]` 上二分。判断 `x^k <= n` 时用快速幂，并用
`__builtin_mul_overflow` 在乘法溢出时立即判为过大。判定具有单调性，
所以二分结束时 `low` 正是最大合法值。

此解法短、直观、完全整数化，复杂度 `O(log n log k)`，足以通过。

### 解法二：按指数分区（`specialized_integer_root.cpp`）

- `k=1` 直接返回 `n`；
- `k=2` 用 `sqrtl` 估计，再用精确判定上下修正；
- `3<=k<32` 用向下偏置的 `pow` 估计，通常只需一次修正；
- `32<=k<=40` 时答案至多为 `3`，直接比较 `2^k`、`3^k`；
- 更大的 `k` 下答案至多为 `2`，只比较 `2^k`；
- `k>=64` 且 `n` 为 64 位数时，除 `0/1` 外答案必为 `1`。

浮点数在这里永远只是“定位器”，最终结果仍由无溢出的整数幂比较证明。
复杂度降为每个询问常数次或 `O(log k)` 次乘法。

两份源码都使用 direct mapping：`T` 为六位上界，`A` 覆盖完整 64 位无符号
范围而走二十位 SIMD 路径，`K<=64` 走两位 scalar 路径。输出统一使用此前
整题验证过的 CompactWriter；二分版本没有在 I/O 上放水。

## English

`binary_search.cpp` binary-searches the monotone predicate `x^k<=n`, with
overflow-checked exponentiation. It is easy to prove and runs in
`O(log n log k)`.

`specialized_integer_root.cpp` specializes exponent ranges: square root, a floating estimate with
exact correction, and tiny-answer threshold tests for large `k`. Floating
point never decides correctness; integer checks prove the final answer.

Both use direct mapping with six-, twenty-, and two-digit policies for
`T`, `A`, and `K`, plus the same measured CompactWriter.
