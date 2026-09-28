# Static Range Count Distinct / 静态区间不同值计数

## English

### Fast solution: `previous_occurrence_sweep.cpp`

Let `prev[i]` be the previous position of `a[i]`, or `-1`. A flat hash table
computes these links in one pass. Store `prev[i]+1`, so the missing predecessor
is index zero.

For a query `[l,r)`, position `i` is the first occurrence of its value inside
the range exactly when `l <= i < r` and `prev[i] < l`. Now observe that every
position `i < l` also satisfies `prev[i] < l`. Hence

`distinct(l,r) = #{ i < r : prev[i] < l } - l`.

Bucket queries by `r`. While sweeping positions below `r`, add one at Fenwick
index `prev[i]+1`. The count in the formula is one Fenwick prefix sum through
index `l`.

### Correctness

Every distinct value in `[l,r)` has exactly one first occurrence there, and
that position has `prev[i] < l`. Every later occurrence has
`prev[i] >= l`, so it is excluded.

The sweep prefix also includes all `l` positions before the query range. Each
of those necessarily has `prev[i] < i < l`, so subtracting `l` removes exactly
the unwanted positions. The remaining count is therefore the number of
distinct range values.

The hash pass is expected `O(N)`. The Fenwick sweep costs
`O((N+Q) log N)` and memory is `O(N+Q)`.

### Alternative and performance

`previous_occurrence_merge_sort_tree.cpp` builds a merge-sort tree over
`prev`. It directly counts positions in `[l,r)` whose previous occurrence is
below `l`, in `O(log^2 N)` per query.

The one-update sweep matches public `#363615` on every large benchmark shape.
It is faster than the more common "remove old latest, add new latest" Fenwick
form because each array position performs only one update and each query only
one prefix sum.

## 中文

### 高性能解：`previous_occurrence_sweep.cpp`

令 `prev[i]` 为 `a[i]` 的前一次出现位置，没有则为 `-1`。用 flat hash table
一次扫描即可得到。实际存储 `prev[i]+1`，使“没有前驱”对应下标零。

位置 `i` 是值在 `[l,r)` 中的第一次出现，当且仅当
`l <= i < r` 且 `prev[i] < l`。同时，所有 `i<l` 都必然满足
`prev[i] < i < l`，因此

`distinct(l,r) = #{ i < r : prev[i] < l } - l`。

把查询按右端点 `r` 分桶。扫描所有 `i<r` 时，在 Fenwick 下标 `prev[i]+1`
加一；公式中的计数只需查询到下标 `l` 的一次前缀和。

### 正确性

区间中每个不同值恰有一个第一次出现，该位置满足 `prev[i]<l`；同值之后的出现
都有 `prev[i]>=l`，不会被计入。

扫描前缀还会包含区间左侧的 `l` 个位置，而它们全部满足 `prev[i]<l`。减去
`l` 恰好删除这些额外位置，剩余计数就是区间不同值数。

哈希预处理期望 `O(N)`；Fenwick 扫描总计 `O((N+Q)logN)`，空间 `O(N+Q)`。

### 替代实现与性能

`previous_occurrence_merge_sort_tree.cpp` 在 `prev` 上建立归并排序树，直接统计
`[l,r)` 中 `prev[i]<l` 的位置，每次 `O(log^2 N)`。

单更新扫描在所有大型本地形状上都与公开 `#363615` 持平。相比常见的“删除旧
最后位置、加入新最后位置”，它让每个数组位置只更新一次、每个查询只做一次
前缀和。
