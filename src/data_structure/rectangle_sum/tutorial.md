# Rectangle Sum / 静态矩形权值和

## 中文

给定带权点，查询半开矩形 `[l,r) x [d,u)` 内的权值和。本题保留两种真正
不同且都为 `O(log N)` 查询核心的解法。

### 解法一：离线 Fenwick 扫描线

定义扫描到横坐标 `X` 时

```text
G(X,d,u) = 所有满足 px < X 且 d <= py < u 的点权和。
```

一个矩形答案就是 `G(r,d,u)-G(l,d,u)`。因此每个查询只需两个横坐标事件，
而不是把二维前缀机械地拆成四个事件。

将点按 `x` 排序，将查询的左右事件也按 `x` 排序。扫描事件 `X` 前，把所有
`px<X` 的点加入压缩 `y` 上的 Fenwick 树；随后用两个 Fenwick 前缀之差取得
`[d,u)`。右事件加、左事件减，即得答案。

严格使用 `<X` 很重要：题目矩形是半开的，位于 `x=r` 的点不能进入查询。

#### 正确性

处理事件 `X` 时，Fenwick 树恰好包含且只包含 `px<X` 的点，因此区间前缀差
就是 `G(X,d,u)`。集合 `{l<=px<r}` 等于 `{px<r}` 减去 `{px<l}`，所以两个
事件的差恰好保留查询矩形中的点；纵轴上的 Fenwick 区间差同理保留
`d<=py<u`。

### 解法二：持久化 y 值域树

`persistent_y_tree.cpp` 先按 `x` 排序点。依次加入每个点，并为每个横坐标前缀
保存一个持久化 `y` 线段树根。加入一个点时只复制根到对应叶路径上的
`O(log N)` 个结点，其余结点与旧版本共享。

查询时：

1. 二分得到 `x<l` 和 `x<r` 的两个版本；
2. 两棵根的结点和作差，表示 `l<=x<r` 的点；
3. 再查询压缩纵坐标区间 `[d,u)`。

它无需预先知道查询，构建完成后可在线回答任意矩形；代价是约
`N log N` 个持久化结点，常数大于离线 Fenwick。

### 复杂度与选择

| 实现 | 构建/整体时间 | 单次查询 | 空间 | 适用场景 |
|---|---:|---:|---:|---|
| `offline_fenwick_sweep.cpp` | `O((N+Q)logN)` | 离线 | `O(N+Q)` | 批量查询、最快 |
| `persistent_y_tree.cpp` | `O(NlogN+QlogN)` | `O(logN)` | `O(NlogN)` | 构建后在线查询、保留历史 |

代表性大用例中，离线版约 `217 ms`，公开第一 `#220264` 约 `268 ms`；持久化
版本是功能更强但不以榜首常数为目标的替代解。

## English

For weighted static points, answer sums in the half-open rectangle
`[l,r) x [d,u)`. Two genuinely different solutions are provided.

### Solution 1: offline Fenwick sweep

Let

```text
G(X,d,u) = sum of points with px < X and d <= py < u.
```

Then the answer is `G(r,d,u)-G(l,d,u)`, so each rectangle needs only two
`x` events. Sort points and events by `x`. Before an event at `X`, insert every
point with `px<X` into a Fenwick tree over compressed `y`; one Fenwick range sum
gives `G`. The strict comparison implements the half-open right boundary.

At each event the tree contains exactly the required `x` prefix. Subtracting
the prefix at `l` from the prefix at `r` leaves precisely `l<=px<r`, while the
Fenwick prefix difference leaves `d<=py<u`; this proves correctness.

### Solution 2: persistent y-value tree

Sort points by `x` and build one persistent y-segment-tree root per point
prefix. Inserting a point copies only the root-to-leaf path and shares every
other node. For a query, binary-search the versions for `x<l` and `x<r`, subtract
their node sums, and query the y interval `[d,u)`.

After construction this version answers arbitrary queries online and exposes
useful historical-prefix functionality, at the cost of `O(N log N)` nodes.

### Complexity and choice

| Variant | Total/build time | Query | Memory | Best use |
|---|---:|---:|---:|---|
| `offline_fenwick_sweep.cpp` | `O((N+Q)logN)` | offline | `O(N+Q)` | fastest batch processing |
| `persistent_y_tree.cpp` | `O(NlogN+QlogN)` | `O(logN)` | `O(NlogN)` | online queries after build |

On the representative large case, the offline version takes about `217 ms`
versus roughly `268 ms` for public leader `#220264`.
