# Persistent Range Affine Range Sum / 持久化区间仿射、复制与求和

## 中文

### 1. 节点信息与仿射复合

`path_copying_lazy_segment_tree.cpp` 使用公共 `PersistentAffineArray`。每个根
代表一个不可变数组版本；节点保存左右孩子、区间和 `sum`，以及尚未传给孩子的
仿射标记 `f(x)=a*x+b`。

若区间长度为 `len`，对整段应用 `f` 后

`sum' = a*sum + b*len (mod 998244353)`。

已有标记为 `g(x)=c*x+d`，之后再应用 `f`，组合标记为
`f(g(x))=(a*c)x+(a*d+b)`。顺序不能颠倒。

### 2. 不主动 push 的路径复制

普通持久化懒线段树若先把标记 push 给两个孩子，会为一次边界下降复制许多无关
节点。本实现把祖先累积变换 `(after_a,after_b)` 作为递归参数：

- 完整覆盖时，只复制当前节点并一次性更新和与标记；
- 部分覆盖时，把当前节点标记与祖先参数复合后传给两个递归分支；
- 完全不相交的分支只在确有祖先变换时物化，否则直接共享旧节点。

树按真实长度 `N` 递归建立，不补到二次幂，避免无效叶子。

### 3. 区间复制

复制操作要令目标版本的 `[l,r)` 等于来源版本相同位置的 `[l,r)`。递归同时携带
目标节点、来源节点以及两边各自的祖先变换：

- 区间外保留目标子树；
- 完整覆盖时直接返回来源子树（若有未物化祖先变换则只包一层新节点）；
- 只有两个边界经过的节点需要继续下降并重新连接。

因此复制的是结构引用，而不是逐元素复制。来源版本和目标旧版本都保持不变。

### 4. 查询与正确性

查询同样不修改树。完整覆盖节点时，把祖先累积变换直接作用于节点已经维护的
`sum`；部分覆盖才继续下降。

不变量是：节点的 `sum` 已包含节点自身标记对整段的效果，而孩子尚未包含该
标记；从根到节点的递归参数恰好表示所有还未物化的祖先变换。更新使用正确的
复合式维持不变量，复制在完整覆盖时选用来源表示、区间外选用目标表示，查询则
把恰好缺少的祖先变换补到和上。对递归区间归纳即可证明三种操作都正确。

### 5. 复杂度与工程结果

连续区间只有 `O(log N)` 个边界节点，修改、复制和查询均为 `O(log N)` 时间；
初始树 `O(N)`，每次修改/复制增加 `O(log N)` 节点，总空间
`O(N+Q log N)`。节点用连续索引池并按最坏操作数预留容量，避免指针和反复扩容。

实现通过全部官方用例和独立随机版本对拍。相同 native flags 下，最大瓶颈用例
约 `368 ms`，公开 `#307048` 约 `418 ms`；优化前的“同时 push 来源和目标”
版本明显更慢。输入中的版本号允许 `-1`，因此保留有符号读取；其他非负字段使用
对应范围的 shape-aware 读取。

## English

### 1. Node state and affine composition

`path_copying_lazy_segment_tree.cpp` uses the reusable
`PersistentAffineArray`. Every root denotes an immutable array version. A node
stores children, its segment sum, and a pending affine map `f(x)=a*x+b`.

For segment length `len`,

`sum' = a*sum + b*len (mod 998244353)`.

If the old tag is `g(x)=c*x+d` and `f` happens later, the combined tag is
`f(g(x))=(a*c)x+(a*d+b)`. Composition order is essential.

### 2. Path copying without eager push

Eagerly pushing a lazy tag into both children copies unrelated persistent
nodes. Instead, recursion carries the accumulated ancestor transform
`(after_a,after_b)`:

- a fully covered node is copied and transformed once;
- a partial overlap composes its tag into the parameters passed downward;
- a disjoint subtree is shared unless an accumulated transform really needs
  materialization.

The tree is built over the actual length `N`, not a padded power of two.

### 3. Range copy

Copying makes destination `[l,r)` equal source `[l,r)` at the same indices.
The recursion carries both roots and both accumulated transforms. Outside the
range it keeps the destination; on full coverage it directly shares the
source subtree (materializing only a pending ancestor transform); only boundary
nodes descend and reconnect. Neither old version is mutated.

### 4. Correctness

A node sum already includes its own tag, while its children do not; recursive
parameters represent exactly the unmaterialized tags of all ancestors. Update
preserves this invariant using affine composition. Copy chooses the source
representation exactly inside the interval and the destination exactly
outside. Query applies precisely the missing ancestor transform to every
fully covered sum. Structural induction proves all three operations.

### 5. Complexity and engineering

Each contiguous range has only `O(log N)` boundary nodes, so update, copy, and
query take `O(log N)`. Memory is `O(N+Q log N)`. Nodes use a reserved contiguous
index pool.

The implementation passes the full official corpus and independent randomized
version differential tests. Under identical native flags, its bottleneck case
is about `368 ms` versus `418 ms` for public `#307048`. Signed parsing is kept
for version `-1`; all nonnegative fields use their measured shape-aware policy.
