# Range Affine Range Sum / 区间仿射、区间求和

## Derivation / 推导

For a segment of length `L` and sum `S`, applying `x -> ax+b` to every element
changes the sum to `aS+bL`. If an old lazy function is `f` and a new update is
`g`, the replacement tag is `g∘f`, not `f∘g`.

长度为 `L`、区间和为 `S` 时，对每个元素执行 `x -> ax+b` 后，新和为
`aS+bL`。若旧标记为 `f`、新修改为 `g`，合并结果必须是 `g∘f`。

## Iterative lazy tree / 迭代懒线段树

`iterative_lazy_segment_tree.cpp` stores leaves at `[N,2N)` without rounding
`N` up to a power of two. Every node keeps its real number of leaves, sum, and
pending affine tag. Before an update, both boundary paths are pushed. Bit masks
enumerate the canonical covered nodes, and only the affected ancestor paths are
rebuilt.

A query does not push whole subtrees. It gathers covered node sums from both
sides while climbing. At each level it applies that ancestor's lazy function
to the accumulated `(sum,length)` pair. Thus tags are accounted for exactly
once and in chronological order.

`iterative_lazy_segment_tree.cpp` 直接把叶子放在 `[N,2N)`，不把 `N` 补成
二次幂。节点维护真实长度、区间和与懒仿射标记。修改前下推两条边界路径，
用位掩码枚举规范覆盖节点，并只重建受影响的祖先。查询无需下推整棵子树：
从两端收集 `(sum,length)`，每上升一层就应用该祖先标记，因此每个标记恰好
按时间顺序计入一次。

## Implicit Treap / 隐式 Treap

`implicit_treap.cpp` isolates `[l,r)` with two splits. Its nodes maintain size,
sum, priority, and one affine lazy tag; merge restores aggregates. This is an
independent expected-`O(log N)` solution and supports sequence insertion or
erasure. / `implicit_treap.cpp` 通过两次分裂隔离 `[l,r)`；节点维护大小、和、
优先级和懒标记，合并时恢复聚合。它是独立的期望 `O(log N)` 解，也便于扩展
动态序列。

## Correctness / 正确性

The node invariant is: `sum` already includes its own lazy tag, while the tag
is precisely the transformation still owed to its children. Applying
`aS+bL` preserves the invariant on full cover; pushing gives both children the
same chronological transformation; pulling adds two exact child sums.
Canonical query nodes partition `[l,r)`, so their adjusted sums equal the
requested answer. / 节点的 `sum` 已包含自身标记，标记则恰好表示尚欠子节点的
变换。整段修改公式、下推和上拉都保持这一不变量；查询规范节点不交且并成
`[l,r)`，故答案正确。

## Complexity, I/O, and performance / 复杂度、I/O 与性能

Both implementations use `O(log N)` expected/worst-case work per operation and
`O(N)` memory. All nonnegative fields use unsigned shape-aware direct parsing;
answers use padded nine-digit modular output. Across all official cases, the
iterative tree's summed median was `0.963x` accepted public baseline `#268857`;
on `random_00` it was `367.84 ms` versus `418.13 ms` (wall-clock scheduling
quantization makes cycle data preferable for finer claims).

两种实现每次操作分别为最坏/期望 `O(log N)`，空间 `O(N)`。所有非负字段走
无符号 shape-aware direct 输入，答案走九位模数 padded 输出。全套总中位数
相对公开 AC `#268857` 为 `0.963x`；`random_00` 为 `367.84 ms` 对
`418.13 ms`，更细的差异应结合 cycles 判断。
