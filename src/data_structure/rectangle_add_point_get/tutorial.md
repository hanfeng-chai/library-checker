# Rectangle Add Point Get / 矩形加、单点查询

## 中文

矩形更新按时间加入，点查询只统计此前的更新。仓库同时提供一个高速离线算法
和一个更直接的在线数据结构。

### 解法一：时间前缀的二进制块分解

`offline_binary_blocks.cpp` 使用公共 `OfflineRectangleAddPointGet`。

#### 1. 把时间前缀拆成块

一次查询发生在已经加入 `t` 个矩形之后，它需要矩形编号区间 `[0,t)`。
像二进制/Fenwick 前缀一样，把它拆成至多 `O(log t)` 个互不相交的对齐块：

```text
[0, highest_pow2(t)),
接着处理剩余部分的最高位，直到覆盖 [0,t)。
```

每个块都形如 `[r-lowbit(r), r)`。实现先把查询放入第一个块；处理完块 `r`
后，如还有剩余时间，再把它转发到唯一的下一个块。这样不必一次性复制完整的
分解表，且转发后的 x/y 顺序仍可复用。

#### 2. 一个时间块怎样回答点查询

块内的矩形已经固定。每个矩形只产生两条 x 事件：

- `x=l`：在 y 上加入 `[d,u)` 的差分；
- `x=r`：撤销这份差分。

把块内 x 事件和分配给该块的查询点按 x 扫描。当事件横坐标
`event_x<=point_x` 时更新一维 y-Fenwick：

```text
d 加 sign*w，u 加 -sign*w。
```

查询 `y` 前缀即得到该点被这个时间块中多少权值覆盖。等号的处理保证左、下
边界包含，右、上边界排除。

#### 3. 为什么预处理仍然快

所有矩形的 x 端点和 y 端点最初按时间排列。遍历块右端 `r` 时，用
`inplace_merge` 按二进制层级合并相邻已排序段，于是切片
`[r-lowbit(r),r)` 已经按 x/y 排好，不必为每个块重新排序。块内 y 坐标和
查询 y 再线性归并完成压缩。

#### 正确性

一个查询的二进制块两两不交，并且并集恰好是它发生前的全部矩形。单个块的
x 扫描在点 `x` 处恰好保留满足 `l<=x<r` 的矩形；y 差分前缀再恰好保留
`d<=y<u`。所以每个块贡献的是该时间片中覆盖查询点的权值和。把所有块贡献
相加，既不遗漏也不重复，正好得到查询时刻的答案。

### 解法二：稀疏二维 Fenwick

`sparse_fenwick.cpp` 对每个矩形做标准二维差分四角：

```text
(l,d)+w, (r,d)-w, (l,u)-w, (r,u)+w。
```

点 `(x,y)` 的答案是差分角的二维前缀。公共 `SparseFenwick2D` 用“外层 x
Fenwick + 每个结点内层 y Fenwick”维护它。操作顺序可直接在线执行；本题
实现仅提前收集坐标以压缩内层数组。

### 复杂度与性能

两种算法最坏时间都是 `O((R+P)log^2R)` 量级。离线块算法使用连续排序切片和
一维 Fenwick，实际常数远小于通用二维 Fenwick；查询转发列表最坏占
`O(PlogR)` 空间，端点和工作树为线性空间。

代表性最大随机用例中，离线版约 `318 ms`，公开第一 `#186151` 约 `368 ms`；
稀疏二维 Fenwick 约 `3025 ms`。后者仍作为清楚、可靠的时间流在线模板保留。

## English

Rectangle updates arrive over time, and a point query sees only earlier
updates. The repository provides both a fast offline algorithm and a direct
online-style structure.

### Solution 1: binary blocks of the time prefix

A query after `t` rectangles needs the update prefix `[0,t)`. Decompose that
prefix into at most `O(log t)` disjoint aligned blocks of the form
`[r-lowbit(r),r)`. A query is first assigned to its largest block; after that
block is processed, it is forwarded to the unique next block for its remaining
time prefix.

Inside one block, every rectangle creates two x events. At `l`, insert the y
difference `+w` at `d` and `-w` at `u`; at `r`, apply the opposite signs.
Sweep events and assigned query points by x. Processing events with
`event_x<=point_x` and taking a y-Fenwick prefix enforces exactly
`l<=x<r` and `d<=y<u`.

Endpoint arrays begin in time order. Hierarchical `inplace_merge` operations
maintain each binary block's x- and y-sorted slice, avoiding a fresh sort per
block. Query y-coordinates are merged linearly with the block endpoints.

The binary blocks are disjoint and cover precisely all prior updates. The sweep
computes exactly the contribution of one block at the point, so summing all
block contributions is correct.

### Solution 2: sparse 2D Fenwick

`sparse_fenwick.cpp` applies the standard four-corner difference

```text
(l,d)+w, (r,d)-w, (l,u)-w, (r,u)+w
```

and answers a point by a two-dimensional prefix sum. It executes operations in
their original online order; only the coordinate universe is collected in
advance for compact storage.

### Complexity and performance

Both approaches have an `O((R+P)log^2R)` worst-case scale. The block method has
much better locality and constants. On the representative maximum random case
it takes about `318 ms`, beating public leader `#186151` at about `368 ms`;
the simpler sparse 2D Fenwick takes about `3025 ms`.
