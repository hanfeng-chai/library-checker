# Static Range Frequency / 静态区间频数

## English

### Fast solution: `offline_prefix_sweep.cpp`

For every query `(l,r,x)`, create two endpoint events:

- at `l`, remember the number of occurrences of `x` in `a[0..l)`;
- at `r`, remember the number of occurrences of `x` in `a[0..r)`.

Events are placed into boundary buckets with a compact CSR layout. Sweep the
array from left to right while a flat hash table stores the prefix frequency
of every seen value. At a boundary, read the requested hash count and add it
to the answer with sign `-1` for `l` and `+1` for `r`.

### Correctness

Let `F(p,x)` be the count of `x` in `a[0..p)`. The required answer is

`F(r,x) - F(l,x)`.

Immediately before processing `a[p]`, the sweep hash table stores exactly
`F(p,x)` for every value `x`, by induction on `p`. Therefore each endpoint
event reads the correct prefix count, and combining its two signed events gives
the exact range frequency.

Expected time is `O(N+Q)` with the flat hash table; memory is `O(N+Q)` for the
array, endpoint CSR, answers, and distinct hash entries.

### Other implementations and performance

- `sorted_positions.cpp`: sort `(value,index)` pairs. Two `lower_bound` calls
  answer each query in `O(log N)` after `O(N log N)` preprocessing.
- `wavelet_matrix.cpp`: follow all 30 bits of `x`; the final mapped interval
  length is its frequency. Build is `O(30N)`, query is `O(30)`.

The offline sweep is locally indistinguishable from public `#362490` across
the complete official corpus. The two alternatives provide a simple static
index and a reusable order-statistics structure.

## 中文

### 高性能解：`offline_prefix_sweep.cpp`

对每个查询 `(l,r,x)` 建立两个端点事件：

- 在 `l` 记录 `x` 在 `a[0..l)` 中的出现次数；
- 在 `r` 记录 `x` 在 `a[0..r)` 中的出现次数。

事件用紧凑 CSR 按边界分桶。从左到右扫描数组，flat hash table 维护所有已见值
的前缀频数；到达边界时读取目标值计数，左端点以负号、右端点以正号加入答案。

### 正确性

令 `F(p,x)` 表示 `x` 在 `a[0..p)` 中的出现次数，则答案为
`F(r,x)-F(l,x)`。

处理 `a[p]` 之前，扫描哈希表对每个值都准确保存 `F(p,x)`；该性质可直接对
`p` 归纳。因此两个端点事件都读取正确前缀计数，带符号相加后正好是区间频数。

使用 flat hash table 时期望时间 `O(N+Q)`，数组、CSR、答案和不同值哈希项共占
`O(N+Q)` 空间。

### 其他实现与性能

- `sorted_positions.cpp`：排序 `(值,下标)`，每次两次 `lower_bound`，预处理
  `O(N log N)`、查询 `O(log N)`；
- `wavelet_matrix.cpp`：沿 `x` 的 30 位映射区间，最终桶长度就是频数，构建
  `O(30N)`、查询 `O(30)`。

离线扫描在完整官方语料的本地基准中与公开 `#362490` 无可分辨差距；另两版
分别提供最简单的静态索引与可复用的顺序统计结构。
