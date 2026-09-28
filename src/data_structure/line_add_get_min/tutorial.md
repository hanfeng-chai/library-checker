# Line Add Get Min / 插入直线、单点最小值

## 中文

### 1. Li Chao Tree 的关键性质

维护若干直线 `f(x)=ax+b`，查询某个 `x` 上的最小值。任意两条不同斜率的直线
最多相交一次，因此若直线 `f` 在区间左端比 `g` 好、在右端却不如 `g`，它们的
优劣只会在中间交换一次。

Li Chao 节点保存一条候选直线。插入新直线时比较区间左端、中点、右端：

- 若新线在两端都更优，直接替换旧线并结束；
- 若两端都不更优，新线不可能在区间内部成为最优，直接结束；
- 否则两线在区间中相交。让中点更优的线留在当前节点，另一条线只需进入仍可能
  反超的一半。

所以每次插入只下降一条根到叶的路径。查询把目标叶到根路径上的所有候选直线
代入 `x` 取最小值；插入时被留在其他分支的直线不可能在这个 `x` 更优。

### 2. 性能主解：离线索引式 Li Chao

`indexed_li_chao.cpp` 先读完操作，只收集真正出现查询的横坐标。它把
“有符号 32 位坐标异或最高位”变成保持数值顺序的无符号键，再用公共
`radix_sort_u32` 两趟稳定基数排序。排序记录同时携带操作编号，因此每个查询直接
得到叶子下标，执行阶段不再 `lower_bound`。

公共 `IndexedLiChaoTree` 补到二次幂并使用连续数组：

- 插入完全迭代，缓存新旧直线在区间端点的值；
- 查询直接从已知叶下标向上，不做坐标二分；
- 斜率和横坐标都是 32 位，题目保证 `a*x+b` 适合 64 位，因此
  `CompactLine` 用一次 64 位乘加，不引入不必要的 128 位运算；
- 无查询的用例不会建立或更新空树。

重复查询坐标可以保留为重复叶子；它们相邻且函数值相同，不影响 Li Chao 的
单交点性质，同时免去额外去重映射。

### 3. 在线完整整数域版本

`dynamic_li_chao.cpp` 使用 `DynamicLiChaoTree`，覆盖题目完整整数域
`[-10^9,10^9]`。孩子只在实际下降时创建，不需要预先知道未来查询坐标。它是
真正在线的接口：每次插入或查询可以立刻处理，也能用于无法离线压缩坐标的题目。

该版本保留通用 64 位直线和 128 位乘法中间值，接口更宽、常数也更大。它仍通过
最大数据，并非故意放慢的朴素枚举。

### 4. 正确性

对任一节点区间，保存不变量：当前节点直线在中点不劣于被继续下放的那条直线；
被下放直线只可能在所选子区间内反超。因为两线差仍是一条一次函数，符号最多
改变一次，端点和中点比较足以确定唯一可能侧。归纳每次插入路径可知，对任意
查询点，所有可能成为最优的直线必在其叶到根路径上。逐一求值取最小值即为全局
答案。

### 5. 复杂度、I/O 与性能

设不同/重复查询记录总数为 `M<=Q`：

- 索引式版本：预处理 `O(Q+2^16)`、每次插入/查询 `O(log M)`、空间 `O(Q)`；
- 动态版本：每次操作 `O(log C)`，`C=2*10^9+1`，按访问路径开点。

两者使用 direct-mapped shape-aware 输入和专用有符号 64 位输出。15/15 官方
用例和 ASan/UBSan 随机对拍全部通过。固定 CPU、相同 native flags 的 `perf`
结果中，索引版在 `max_random_00` 为公开第一 `#362191` 的约 `0.96x` cycles，
在其有利的 `half_00` 约 `1.05x`；即最大随机瓶颈更快，最不利代表形状也保持
在约 5% 内。

## English

### 1. Li Chao invariant

Two distinct lines intersect at most once. During insertion, compare the new
and stored lines at the interval endpoints and midpoint. A line better at both
ends dominates the whole interval; one worse at both ends can be discarded.
Otherwise keep the midpoint winner and descend with the other line into the
only half where it can still overtake.

Insertion therefore follows one root-to-leaf path. A point query evaluates all
lines on its leaf-to-root path; every line absent from that path was proven
unable to win at this coordinate.

### 2. Preferred offline indexed tree

`indexed_li_chao.cpp` reads all operations and collects only actual query
coordinates. Flipping the sign bit turns signed 32-bit order into unsigned
order, allowing the reusable two-pass `radix_sort_u32` to sort records carrying
their operation indices. Every query receives its leaf index in advance, so
execution performs no binary search.

The reusable `IndexedLiChaoTree` is an iterative, power-of-two contiguous tree.
It caches endpoint evaluations during insertion and walks upward from a known
leaf during query. `CompactLine` uses a safe 64-bit multiply-add under this
problem's bounds. Duplicate query coordinates may remain as adjacent leaves,
avoiding another mapping pass without changing any function value.

### 3. Online dynamic alternative

`dynamic_li_chao.cpp` covers the complete integer domain and creates children
only when visited. It needs no future coordinates and answers each operation
immediately. Its general line type uses a 128-bit multiplication intermediate,
so constants are larger, but it remains a maximum-constraint online solution
rather than a naive scan.

### 4. Correctness

At each node the midpoint winner is retained, while the displaced line is sent
to the only half where the sign of their linear difference can favor it.
Induction over insertion shows that every line capable of winning at a query
coordinate occurs on that coordinate's root path. Taking the minimum of those
evaluations is therefore exactly the global minimum.

### 5. Complexity and measurements

For `M` query records, preprocessing is `O(Q+2^16)`, indexed insertion/query
are `O(log M)`, and memory is `O(Q)`. The dynamic version uses `O(log C)` time
per operation over `C=2*10^9+1`.

Both use direct-mapped shape-aware input and dedicated signed-64 output. They
pass all 15 official cases, and the indexed core passes randomized ASan/UBSan
differential tests. Against public leader `#362191`, pinned cycle ratios are
about `0.96x` on `max_random_00` and `1.05x` on the leader-favorable
`half_00`: faster on the random bottleneck and within roughly 5% on the
adversarial representative.
