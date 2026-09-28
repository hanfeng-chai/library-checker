# Static Range Sum / 静态区间和

## 中文

### 前缀和（`prefix_sum.cpp`）

定义 `prefix[i]=a[0]+...+a[i-1]`，则半开区间 `[l,r)` 的和为
`prefix[r]-prefix[l]`。预处理 `O(N)`，每次查询 `O(1)`，空间 `O(N)`。
减去两个前缀时，`[0,l)` 被精确抵消，只留下 `[l,r)`，因此正确。

### 线段树（`segment_tree.cpp`）

叶子保存原数组，父节点保存两子区间之和。查询把 `[l,r)` 分解成
`O(log N)` 个互不重叠的树节点。它比前缀和慢，但同一通用结构还能
支持点修改，是理解下一题的自然过渡。预处理 `O(N)`，查询
`O(log N)`，空间 `O(N)`。

两份源码都使用 direct mapping、六位索引和针对十位混合长度数组值的解析
路径，并共享整题实测更快的普通 Writer。

## English

`prefix_sum.cpp` answers `[l,r)` as `prefix[r]-prefix[l]` after linear preprocessing,
so each query is `O(1)`. `segment_tree.cpp` uses a general segment tree and decomposes
the interval into `O(log N)` nodes. Both use `O(N)` memory.

Both use direct mapping, six-digit indices, the measured mixed-width
ten-digit value path, and the same non-compact Writer.
