# Static Range Sum with Upper Bound / 带上界的静态区间计数与和

## English

### Fast solution: `offline_value_sweep_fenwick.cpp`

Sort array items `(value,position)` and queries `(bound,l,r)` by value/bound
with fixed-pass radix sort. Sweep queries in increasing bound. Before answering
bound `x`, insert every item with `value <= x` into a Fenwick tree at its array
position.

Each Fenwick node stores an aggregate `(count,sum)`. Its range difference on
`[l,r)` therefore returns both:

- how many eligible elements are in the index range;
- the sum of those elements.

### Correctness

When query bound `x` is processed, the value sweep has inserted an item exactly
when its value is at most `x`. Thus a position contributes `(1,a[i])` to the
Fenwick tree exactly when it satisfies the value condition.

Fenwick range subtraction selects exactly positions `l <= i < r`. Summing the
stored aggregates over that range therefore yields precisely the requested
count and sum.

Radix sorting is linear for the fixed 30-bit keys. The sweep performs one
Fenwick insertion per item and one range query per request, for
`O((N+Q) log N)` time and `O(N+Q)` memory.

### Other implementations and performance

- `persistent_value_tree.cpp`: one persistent value-segment-tree root per array
  prefix. Root `r` minus root `l` represents the index range; query the value
  prefix through `x`. Time `O(log N)`, memory `O(N log N)`.
- `merge_sort_tree.cpp`: sorted values and local prefix sums at every segment
  node. It answers in `O(log^2 N)` and is the most direct static alternative.

The offline sweep is substantially faster than public `#340340` here:
maximum random cases take roughly `0.69x` its wall time, while retaining a
generic `(count,sum)` Fenwick aggregate.

## 中文

### 高性能解：`offline_value_sweep_fenwick.cpp`

使用固定趟 radix sort，分别按值排序数组项 `(值,位置)`，按上界排序查询
`(上界,l,r)`。按上界递增扫描；回答上界 `x` 前，把所有 `value<=x` 的数组项
在其原位置加入 Fenwick 树。

Fenwick 节点保存聚合量 `(个数,总和)`。对 `[l,r)` 做区间差即可同时得到：

- 下标区间内满足上界的元素个数；
- 这些元素的总和。

### 正确性

处理上界 `x` 时，一个数组项当且仅当值不大于 `x` 才已被加入。因此位置 `i`
当且仅当满足值条件时向 Fenwick 贡献 `(1,a[i])`。

Fenwick 区间差又恰好选择 `l<=i<r` 的位置，所以聚合后的两个分量分别就是题目
要求的数量与和。

固定 30 位键的 radix sort 为线性时间。每个数组项更新一次、每个查询做一次
区间和，总时间 `O((N+Q)logN)`，空间 `O(N+Q)`。

### 其他实现与性能

- `persistent_value_tree.cpp`：每个数组前缀一棵持久化值域线段树；版本 `r-l`
  表示下标区间，再查到 `x` 的值域前缀。时间 `O(logN)`、空间 `O(NlogN)`；
- `merge_sort_tree.cpp`：每个线段树节点存有序值与局部前缀和，查询
  `O(log^2N)`，是最直观的静态替代。

本地离线扫描明显快于公开 `#340340`：最大随机用例墙钟约为其 `0.69x`，同时
保留了通用 `(count,sum)` Fenwick 聚合封装。
