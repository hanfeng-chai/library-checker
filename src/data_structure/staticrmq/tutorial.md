# Static RMQ / 静态区间最小值

## 中文

### Sparse Table（`sparse_table.cpp`）

`table[k][i]` 保存从 `i` 开始、长度 `2^k` 的区间最小值。查询
`[l,r)` 时令 `k=floor(log2(r-l))`，取左端长度 `2^k` 与右端长度
`2^k` 两块的最小值。两块可能重叠，但 `min(x,x)=x`，重复覆盖不影响
答案。

预处理 `O(N log N)`，查询 `O(1)`，空间 `O(N log N)`，适合静态且
查询很多的场景。

### 线段树（`segment_tree.cpp`）

把查询区间拆成 `O(log N)` 个树节点并取最小值。预处理、空间
`O(N)`，查询 `O(log N)`；若未来需要修改，线段树更合适。

两份源码均使用 direct mapping、六位索引、十位非负数组值和同一
CompactWriter；差异只来自 RMQ 核心。

## English

The sparse table stores minima for every power-of-two interval. Any query is
covered by two possibly overlapping blocks; overlap is safe because minimum
is idempotent. This gives `O(N log N)` preprocessing and `O(1)` queries.
`segment_tree.cpp` uses an `O(N)`-space segment tree with `O(log N)` queries.

Both use direct mapping, six-digit indices, ten-digit unsigned values, and the
same CompactWriter.
