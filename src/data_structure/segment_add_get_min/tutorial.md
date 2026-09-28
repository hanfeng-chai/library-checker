# Segment Add Get Min / 插入线段、单点最小值

## 中文

### 1. 从整条直线到受限线段

现在函数 `f(x)=ax+b` 只在半开区间 `[l,r)` 有效。先只保留真正被查询的横坐标，
并建立这些点上的线段树。一个有效区间可分解为 `O(log M)` 个互不相交的标准
节点；在每个标准节点内，该函数覆盖整个节点区间，所以可以执行普通 Li Chao
直线插入。

查询点只经过一条根到叶路径。覆盖该点的每个线段一定被加入路径上的某个标准
节点，不覆盖它的线段则不会出现在路径上；对路径中所有 Li Chao 候选取最小值
即可。若始终保留无穷哨兵，说明没有任何有效线段，输出 `INFINITY`。

### 2. 性能主解：索引式 Segment Li Chao

`indexed_segment_li_chao.cpp` 使用公共 `IndexedLiChaoTree`，但
`add_segment(left_index,right_index,line)` 通过叶下标的二进制分解直接枚举标准
节点，不递归遍历外层线段树。每个标准节点再执行缓存端点值的迭代 Li Chao 插入。

离线坐标映射也避免逐操作二分。把三类事件一起按有符号横坐标基数排序：

- 查询点事件；
- 线段左端点；
- 线段右端点。

稳定扫描时，当前已经出现的不同查询坐标个数正好是端点的 `lower_bound`；若同一
坐标的查询事件已出现，则减一得到该点本身的下标。于是所有 `[l,r)` 端点和查询
都在线性扫描中直接映射。没有查询时，所有区间映射为空，树不会做无用更新。

### 3. 在线动态版本

`dynamic_segment_li_chao.cpp` 在完整整数域上动态开点。区间插入递归到
`O(log C)` 个标准节点，并在每个节点执行一次 `O(log C)` Li Chao 插入，完全
不需要预读后续查询。实现只创建真正与 `[l,r)` 相交的孩子。

它的 `O(log^2 C)` 常数明显大于离线版本，但仍在默认时限通过全部最大用例，
提供离线方案无法替代的在线模板，因此使用算法名而非 `naive.cpp`。

### 4. 正确性

外层标准区间分解恰好覆盖 `[l,r)`，且互不重叠。因此对某个查询点 `x`：

- 若线段覆盖 `x`，其某个标准节点必是 `x` 的祖先；
- 若线段不覆盖 `x`，它的所有标准节点都不在 `x` 的祖先链上。

每个祖先节点内部的 Li Chao 不变量保证返回所有加入该节点直线在 `x` 的最小值。
再对整条祖先链取最小，就恰好得到所有且仅有覆盖 `x` 的线段答案。无穷哨兵在
集合为空时自然保留下来。

### 5. 复杂度与性能

令 `M` 为不同查询横坐标数：

- 索引式版本：事件预处理 `O((N+Q)+2^16)`；插入
  `O(log^2 M)`，查询 `O(log M)`，空间 `O(M)`；
- 动态版本：插入 `O(log^2 C)`、查询 `O(log C)`，按访问节点用空间，
  `C=2*10^9+1`。

两版均使用 direct-mapped shape-aware I/O，并通过 13/13 官方用例；索引核心还
与暴力随机对拍。固定 CPU 对公开第一 `#357121` 的 cycles 比值为：

- `max_random_00`：`0.986x`；
- `random_00`：`0.983x`；
- 最不利的 `all_twice_00`：`1.044x`。

因此索引版在主要随机瓶颈上本地更快，特殊重复形状也保持在约 4.5% 内。

## English

### 1. Segment-restricted lines

A line is now valid only on `[l,r)`. Build a segment tree over actual query
coordinates and decompose each validity interval into `O(log M)` canonical
nodes. Inside each such node the line is valid everywhere, so ordinary Li Chao
insertion applies.

A point query visits one root path. Every segment covering the point was added
to exactly one ancestor canonical node, while a non-covering segment appears
on none. The minimum over all ancestor Li Chao candidates is therefore the
answer; an untouched infinity sentinel means `INFINITY`.

### 2. Preferred indexed segment Li Chao

`indexed_segment_li_chao.cpp` calls
`IndexedLiChaoTree::add_segment`. Bit decomposition of the two leaf boundaries
enumerates canonical nodes directly, avoiding an outer recursive traversal.
Each node then performs iterative Li Chao insertion with cached endpoint
values.

Coordinate mapping is also offline and binary-search-free. Query points and
both kinds of segment endpoints are radix-sorted together. During a stable
scan, the number of distinct query coordinates already encountered is exactly
an endpoint's lower-bound index; ties are adjusted to the current point.
Queries and both interval boundaries receive indices in one linear pass.

### 3. Online dynamic alternative

`dynamic_segment_li_chao.cpp` creates nodes over the full integer domain only
where `[l,r)` intersects. It performs `O(log C)` canonical insertions, each
costing `O(log C)`, but needs no future operations. It passes maximum tests and
is retained as a stronger online interface, not as an intentionally slow
naive variant.

### 4. Correctness

Canonical nodes form a disjoint exact cover of each validity interval. A point
belongs to that interval iff exactly one of those nodes is its ancestor.
Within every ancestor, the standard Li Chao invariant returns the minimum of
all lines stored there. Minimizing over the path therefore considers exactly
all segments covering the point. Infinity remains precisely when that set is
empty.

### 5. Complexity and performance

For `M` distinct query coordinates, preprocessing is
`O(N+Q+2^16)`, insertion is `O(log^2 M)`, query is `O(log M)`, and indexed
storage is `O(M)`. The online domain version uses `O(log^2 C)` insertion and
`O(log C)` query for `C=2*10^9+1`.

Both pass all 13 official cases and use direct-mapped shape-aware I/O; the
indexed core also passes brute-force randomized differential tests. Pinned
cycle ratios against public leader `#357121` are `0.986x` on
`max_random_00`, `0.983x` on `random_00`, and `1.044x` on the least favorable
`all_twice_00`.
