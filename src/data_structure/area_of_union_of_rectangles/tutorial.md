# Area of Union of Rectangles / 矩形并面积

## 中文

### 1. 从二维面积变成一维覆盖长度

每个矩形都是半开区域 `[l,r) x [d,u)`。把左边界看成“加入纵区间
`[d,u)`”，右边界看成“删除同一个纵区间”，再按横坐标从小到大扫描。

假设相邻两条事件边的横坐标是 `previous_x` 和 `x`。在这段横向条带内，
活动矩形集合没有变化，因此新增面积正好是

```text
(x - previous_x) * 当前所有活动纵区间的并长度。
```

于是问题只剩下：动态加入或删除纵区间，并随时得到所有区间的并长度。

### 2. 覆盖长度树维护什么

先压缩全部 `d,u`。相邻坐标 `ys[i],ys[i+1]` 才是真正的叶区间；坐标点本身
没有长度。公共结构 `CoveredLengthTree` 的每个结点保存：

- `span`：该结点代表的完整几何长度；
- `cover`：有多少个更新完整覆盖了该结点；
- `child_sum`：两个孩子当前覆盖长度之和。

结点的答案为：

```text
covered(node) = cover > 0 ? span : child_sum。
```

如果 `cover>0`，无论孩子内部怎样变化，整段都至少被一个矩形覆盖；否则只能
从孩子合并。这正是区间并长度线段树的核心不变量。

### 3. 实现中的细节

`iterative_cover_tree.cpp` 使用连续数组和迭代标准区间分解，不创建指针结点，
也不递归。所有坐标和事件用稳定 32 位基数排序。读入一个矩形时，左事件把
端点保存为 `down < up`，右事件反向保存为 `up > down`；扫描时比较两个下标
即可判断本事件是加入还是删除，不必额外保存符号字段。

同一横坐标可能有很多事件。实现逐个处理也完全正确，因为它们之间
`x-previous_x=0`，不会产生虚假的面积。

### 4. 正确性

扫描到任意事件横坐标之前，覆盖长度树中恰好包含所有满足
`l <= current_x < r` 的矩形纵区间：左边界加入，右边界删除，正好对应半开
边界。两条相邻事件边之间活动集合不变，覆盖树给出的正是该条带中每个横截面
的纵向并长度，所以“宽度乘高度”得到该条带的矩形并面积。所有条带互不重叠，
累加后就是完整答案。

### 5. 复杂度与性能

- 排序：`O(N log N)`，基数排序在本题固定宽度整数上实际为线性趟数；
- 每条边更新：`O(log N)`；
- 空间：`O(N)`。

本仓库只保留这一份不增加核心复杂度的高性能实现。固定 CPU 的代表性
`max_random` 测试中，它约为公开第一 `#241472` 的 `0.85x` 墙钟时间。

## English

### 1. Reduce area to covered length

Each rectangle is half-open: `[l,r) x [d,u)`. Sweep vertical edges by `x`.
The left edge inserts `[d,u)` and the right edge removes it. Between consecutive
event coordinates `previous_x` and `x`, the active set is unchanged, so the
new area is

```text
(x - previous_x) * union length of all active y-intervals.
```

The two-dimensional problem is therefore reduced to dynamic one-dimensional
interval-union length.

### 2. Covered-length tree invariant

Compress every `d` and `u`; leaves represent the elementary intervals between
adjacent coordinates. Each `CoveredLengthTree` node stores its full geometric
`span`, a full-cover counter, and the sum reported by its children:

```text
covered(node) = cover > 0 ? span : child_sum.
```

A positive counter proves that the entire node interval is covered, regardless
of its descendants. Otherwise its answer is exactly the union represented by
the children.

### 3. Implementation details

`iterative_cover_tree.cpp` uses a contiguous iterative tree, avoiding pointer
nodes and recursion. Coordinates and events are sorted with stable 32-bit radix
sorts. A left event stores its endpoints in increasing order and the matching
right event in decreasing order, so the endpoint order itself encodes insertion
versus deletion. Events at the same `x` may be processed one by one: their
intermediate strip width is zero.

### 4. Correctness

Immediately before every strip, the tree contains exactly the rectangles with
`l <= current_x < r`, matching the half-open convention. Its root reports the
union length of the strip's vertical cross-section. Multiplying that length by
the strip width gives precisely the covered area in that strip. The strips are
disjoint and cover every possible contribution, so their sum is the union area.

### 5. Complexity and performance

Sorting costs `O(N log N)`, every edge update costs `O(log N)`, and memory is
`O(N)`. Only the fast, still compact implementation is retained. On the pinned
representative `max_random` benchmark it runs at about `0.85x` the wall time of
public leader `#241472`.
