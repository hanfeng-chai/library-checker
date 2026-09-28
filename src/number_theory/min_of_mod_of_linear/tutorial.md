# Min of Mod of Linear / 线性函数取模最小值

## 中文

求 `0<=i<n` 时 `(a*i+b) mod m` 的最小值。直接枚举是 `O(n)`。

### 解法一：答案二分 + floor-sum（`floor_sum_binary_search.cpp`）

二分一个阈值 `x`，判断是否存在余数小于 `x`。对每个 `i`，

`floor((a*i+b+m-x)/m)-floor((a*i+b)/m)`

在余数至少为 `x` 时等于 `1`，否则为 `0`。两次 `floor_sum` 的差就
统计了余数至少为 `x` 的个数；若少于 `n`，说明存在余数 `<x`。

判定单调，故总复杂度 `O(log^2 m)`。它复用了上一题的通用库，容易
理解和验证，官方数据可 AC。

### 解法二：连分数/欧几里得跳跃（`continued_fraction.cpp`）

高性能版本直接追踪序列前缀最小值出现的位置。这些位置由斜率 `a/m`
的连分数收敛分母组成若干等差段。状态保存当前是否反射，以及相邻两
段的长度；若查询范围落在当前段就直接算出答案，否则做一次
`(m,a)->(a,m mod a)` 的欧几里得递归。

每层都缩小模数，故时间 `O(log m)`、栈空间 `O(log m)`。本地比二分
版本快约一个数量级。

两份源码均使用 direct mapping、六位查询计数和十位参数路径，并共享
CompactWriter。曾测试按九位上界特化的 bounded 输出，但本题答案分布偏小，
整题反而明显回退，因此没有仅凭最大位数强行接入。

## English

`floor_sum_binary_search.cpp` binary-searches the answer. Two floor sums count residues above a
threshold, giving `O(log^2 m)` complexity.

`continued_fraction.cpp` follows continued-fraction prefix-minimum segments directly. Each
recursive step is one Euclidean reduction, so it runs in `O(log m)`.

Both use direct mapping, six-/ten-digit input policies, and the same
CompactWriter. A bounded-output specialization was rejected after it regressed
the real answer distribution.
