# Dynamic Point Set Rectangle Affine Rectangle Sum / 动态点集矩形仿射与矩形和

## 中文

本题同时包含：

- 插入一个此前不存在的点；
- 按编号把单点权值赋成新值；
- 查询矩形权值和；
- 对矩形内每个有效点做 `w <- a*w+b`。

坐标是二维的，更新又需要整块懒标记，因此普通一维线段树无法直接解决。

### 1. 离线坐标、在线语义

先读完操作，只是为了知道未来所有插入点的坐标；执行仍严格按照原时间顺序。
把初始点和未来点一起建立静态平衡 KD-tree。未来点初始 `active=0`、权值为零，
到插入操作发生时才激活，所以预建坐标不会提前贡献答案。

### 2. 平衡 KD-tree 的形状

建树时在 y、x 两个维度间交替，用 `nth_element` 选择当前区间的中位点。
每个树结点自身就保存一个点，而不是只把点放在叶子；左右子树继续处理两侧
点集。结点同时保存整个子树的包围盒：

```text
[min_x,max_x] x [min_y,max_y]。
```

中位数分割保证树高 `O(logN)`。每个点编号记录其结点位置，每个结点记录父亲，
因此单点操作可沿一条父链完成。

### 3. 聚合值和仿射懒标记

每个结点维护：

- 自身点的 `value` 与 `active`；
- 子树有效点数 `active_size`；
- 子树权值和 `sum`；
- 懒标记 `(mul,add)`，表示 `w <- mul*w+add`。

若更新矩形完整覆盖结点包围盒，不必访问孩子：

```text
sum <- sum*a + active_size*b。
```

只有有效点才应增加常数项，所以必须乘 `active_size`，不能乘静态子树大小。
若旧标记是 `(m,c)`，随后再应用 `(a,b)`，复合后为

```text
(m*a, c*a+b)。
```

若包围盒与查询不交，立即返回；完全包含时直接读聚合或打标记；只有部分相交
才下传并递归。

### 4. 插入为什么不会继承过去的更新

一个未来点在过去是无效的，所以过去的矩形仿射不能作用到它。激活前，沿父链
从根到该结点依次下传懒标记。标记下传到无效点时不会修改其 `value`；随后才
写入插入权值并置 `active=1`，最后从该点向根重新合并。这样它只参与插入时刻
之后的更新。

普通单点赋值也先下传整条路径，确保覆盖它的旧懒标记已落实，再覆盖权值。

### 5. 正确性

结点包围盒包含且只包含其子树的静态坐标。三种几何关系中：

- 不交时该子树无贡献；
- 完全覆盖时所有有效点都接受同一仿射，聚合公式精确；
- 部分相交时，只有递归到孩子和结点自身逐一判断才能区分，合并后恢复正确和。

懒标记的复合顺序与操作时间顺序一致；`active_size` 排除了尚未插入的点；父链
下传保证单点覆盖前没有悬空标记。归纳每次操作后，所有结点的 `sum` 都等于其
有效点真实权值和，因此矩形查询正确。

### 6. 复杂度与性能

建树期望 `O(NlogN)`，空间 `O(N)`，单点激活/赋值为 `O(logN)`。二维平衡
KD-tree 的正交矩形查询与懒更新在常见均匀数据上期望约 `O(sqrt(N))`，理论
最坏仍可能退化到 `O(N)`。

`lazy_kd_tree.cpp` 使用公共 `LazyAffineKDTree2D`，仓库只保留这一高性能
实现；逐点扫描不是有意义的可
通过替代解。最大随机用例上，它与公开第一 `#251434` 的本地硬件周期差约
`2%`，处于同一性能档，同时不再直接包含上游 `correct.cpp`。

## English

The problem supports point insertion, point assignment, rectangle sum, and an
affine update `w <- a*w+b` on every active point in a rectangle.

### 1. Offline coordinates, online semantics

Read all operations first only to learn future insertion coordinates. Execution
still follows the original order. Build one static balanced KD-tree containing
initial and future points; future points start inactive with zero contribution
and are activated only at their insertion time.

### 2. Balanced KD-tree

Alternate y and x median splits using `nth_element`. Every tree node stores one
point itself, plus the bounding box of its complete subtree. Median splitting
keeps height `O(logN)`. A point-id-to-node map and parent links make point
updates follow one root path.

### 3. Aggregate and affine lazy tag

Each node stores its own value/active flag, subtree active count, subtree sum,
and a lazy affine tag `(mul,add)`. If an update fully covers the bounding box,

```text
sum <- sum*a + active_size*b.
```

The constant term uses the active count, not the static number of coordinates.
Composing an existing `(m,c)` with a later `(a,b)` gives
`(m*a, c*a+b)`. Disjoint boxes are skipped, covered boxes use the aggregate,
and only partial intersections push and recurse.

### 4. Why future points ignore past updates

Before activation, push every tag on the root-to-point path. Applying a tag to
an inactive point does not change its own value. Only then write the insertion
weight and mark it active, followed by pulls back to the root. Consequently the
point receives exactly the operations occurring after insertion. Point
assignment uses the same path push before overwriting its value.

### 5. Correctness

A node bounding box contains exactly its subtree coordinates. Disjoint,
fully-covered, and partial cases therefore partition every rectangle operation
correctly. The aggregate affine formula is exact for all active points, lazy
composition preserves chronological order, and inactive points are excluded by
`active_size`. Inductively, every stored sum equals the true active-point sum,
so rectangle queries are correct.

### 6. Complexity and performance

Expected build time is `O(NlogN)`, memory is `O(N)`, and point changes are
`O(logN)`. Balanced two-dimensional orthogonal rectangle operations are about
`O(sqrt(N))` on typical uniform data, with an `O(N)` theoretical worst case.

Only the meaningful high-performance `lazy_kd_tree.cpp` variant, backed by
`LazyAffineKDTree2D`, is retained.
On the maximum random case it is within about `2%` hardware cycles of public
leader `#251434`, while no longer including the upstream `correct.cpp`.
