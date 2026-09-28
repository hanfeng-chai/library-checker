# Static Range Inversions Query / 静态区间逆序对

## English

### Fast solution: `delta_sweep.cpp`

Queries are placed in alternating Mo order, but the current interval is not
maintained with a Fenwick operation for every movement. Instead, every movement
is converted into a deferred range contribution.

Define

`g(t,x) = #{ j <= t : rank(a[j]) >= rank(x) }`.

When a right or left boundary moves across a consecutive position range, the
change in inversion count is a signed sum of terms of the form
`g(t,a[p])`, sometimes plus the simple interval length `t-p+1`. The algorithm
records one event `(sweep t, moved range, sign, add_length)` instead of
evaluating those terms immediately.

After all query transitions are recorded, sweep `t` once from left to right.
A square-root-decomposed counter supports:

- increment every rank not greater than the newly seen rank;
- read `g(t,a[p])` at one rank.

Each event is now evaluated in `O(1)` per moved position. Prefix inversion
counts provide the base value, and accumulated deltas reconstruct answers in
the same Mo order.

Equal values receive ranks ordered by original position. An earlier equal
value then has a smaller rank, so it is not incorrectly counted as a strict
inversion.

### Correctness

Appending `a[p]` to the right adds the number of earlier interval values
greater than it; prepending it adds the number of later values smaller than it.
The four possible boundary movements are algebraic rearrangements of these
two facts. Their recorded `g` expressions therefore equal the exact change in
the current inversion count.

During the second sweep, the prefix counter contains precisely the values with
index at most `t`, so every deferred `g(t,a[p])` is evaluated exactly. Starting
from the prefix-inversion base and applying deltas in query order maintains the
invariant that the running value is the inversion count of the current query.

With the standard Mo movement bound and block counter, time is
`O((N+Q)sqrt(N))` up to constants, and memory is `O(N+Q)`.

### Alternative and performance

`fenwick_mo.cpp` is the conventional solution: maintain current value
frequencies in a Fenwick tree and update the inversion count on each movement.
It is simpler, costs an extra `log N` per movement, and remains a useful
general template.

The deferred delta sweep is faster than public `#181308` on every maximum and
small-`N` benchmark in this environment, while using the same algorithmic
family and the shared fast I/O.

## 中文

### 高性能解：`delta_sweep.cpp`

查询仍按左右块交替的 Mo 顺序排列，但不再为区间每移动一步执行一次 Fenwick
操作，而是把整段移动转换成延迟计算的贡献。

定义

`g(t,x) = #{ j <= t : rank(a[j]) >= rank(x) }`。

左右端点跨过一段连续位置时，逆序数变化可写成若干
`g(t,a[p])` 的带符号和，有时再加简单长度 `t-p+1`。算法只记录
`(扫描时刻 t, 移动区间, 符号, 是否加长度)`，暂不逐项求值。

全部查询转移记录完后，再从左到右扫描一次 `t`。根号分块计数器支持：

- 对不大于新值排名的整个前缀加一；
- 在一个排名处读取 `g(t,a[p])`。

这样每个被移动位置只需常数时间求贡献。前缀逆序数提供基值，按 Mo 顺序累加
各查询差分即可恢复答案。

相等值按原下标分配连续排名；较早的相等值排名更小，因此不会被误计为严格逆序。

### 正确性

从右侧加入 `a[p]`，新增逆序对数是区间中更大的旧元素数；从左侧加入时，则是
右侧比它小的元素数。四种端点移动公式只是这两个事实的代数变形，所以记录的
`g` 表达式恰好等于真实逆序数变化。

第二次扫描到 `t` 时，计数器恰好包含所有下标不超过 `t` 的值，故每个延迟
`g(t,a[p])` 都被准确求出。从前缀逆序基值出发，按查询顺序加入差分，始终保持
“当前值等于当前查询区间逆序数”的不变量。

结合标准 Mo 移动上界和根号计数器，时间为
`O((N+Q)sqrt(N))`（忽略常数），空间 `O(N+Q)`。

### 替代实现与性能

`fenwick_mo.cpp` 是经典方案：Fenwick 维护当前值频数，每次移动即时更新逆序数。
它更直观，但每个移动多一个 `log N`，适合作为通用模板。

本机所有最大用例和小 `N` 用例中，延迟差分扫描均快于公开 `#181308`，同时使用
同一算法族与共享 fast I/O。
