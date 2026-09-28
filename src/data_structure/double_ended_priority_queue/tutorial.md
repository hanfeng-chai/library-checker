# Double-Ended Priority Queue / 双端优先队列

## English

### Fast solution: `min_max_heap.cpp`

A min-max heap is one array whose tree levels alternate between minimum and
maximum levels.

- The root is the global minimum.
- The larger of indices `1` and `2` is the global maximum and is cached.
- A minimum-level node is no larger than all descendants on minimum levels;
  a maximum-level node is no smaller than all descendants on maximum levels.

Insertion first compares the new value with its parent. It either crosses to
the opposite level or stays on its own level, then moves through grandparents
only. Removing an endpoint moves the last array value into the hole and
pushes it down through the best of four grandchildren. Near the bottom, the
remaining children and grandchildren are handled explicitly.

The implementation uses move-hole propagation rather than repeated `swap`.
Initial values are heapified bottom-up, and the maximum root is refreshed in
constant time.

### Correctness

The parent comparison after insertion restores the relation between adjacent
min/max levels. Moving only through grandparents then restores the invariant
among nodes of the same kind; all untouched relations remain valid.

For deletion, the best grandchild is the only same-level descendant that can
replace the hole without skipping a more extreme value. Moving it upward and,
when necessary, repairing its relation with its parent preserves both
alternating invariants. Induction down the path restores the whole heap.
Therefore the root is always the minimum and the cached larger child is always
the maximum.

Bottom-up construction is `O(N)`. Push, pop-min, and pop-max are `O(log N)`;
memory is `O(N)`.

### Alternative and performance

`multiset.cpp` uses `std::multiset`. It is a fully online, very readable
`O(log N)` solution with direct access to both endpoints, but each element is
a separately allocated tree node.

The optimized single-array heap is locally close to public `#343905`: most
large cases share the same wall-time quantum, while pop-heavy cases are about
one quantum slower. It is substantially faster and smaller than the earlier
four-heap lazy-deletion design.

## 中文

### 高性能解：`min_max_heap.cpp`

Min-max heap 只使用一个数组，树层在“最小层”和“最大层”之间交替：

- 根节点是全局最小值；
- 下标 `1`、`2` 中较大的值是全局最大值，并被缓存；
- 最小层节点不大于其所有最小层后代，最大层节点不小于其所有最大层后代。

插入时先与父亲比较，决定新值是否跨到另一类层，之后只沿祖父链上浮。删除一端
时，把数组末尾值放入空穴，每层从四个孙子中选择最合适者上移；到树底时再显式
处理剩余孩子和孙子。

实现使用 move-hole 传递而不是反复 `swap`。初始数组自底向上堆化，最大根只需
常数时间刷新。

### 正确性

插入后的父子比较恢复相邻最小层/最大层关系；沿祖父链移动只会修改同类层路径，
并恢复该类不变量，其他关系不受影响。

删除时，四个孙子中最极端者是同类层上唯一可能填补空穴的候选。把它上移，并在
必要时修复它与父亲的关系，即可同时保持两类不变量。沿路径归纳后整棵堆重新
合法，因此根始终为最小值，缓存的较大孩子始终为最大值。

批量建堆 `O(N)`；插入、删除最小值、删除最大值均为 `O(log N)`，空间 `O(N)`。

### 替代实现与性能

`multiset.cpp` 使用 `std::multiset`，是完全在线且非常直观的 `O(log N)` 解，
但每个元素都需要独立红黑树节点。

优化后的单数组堆在本地已接近公开 `#343905`：多数大型用例落在同一墙钟时间片，
大量弹出用例约慢一个时间片；相比旧的四堆延迟删除设计，速度和内存均明显更好。
