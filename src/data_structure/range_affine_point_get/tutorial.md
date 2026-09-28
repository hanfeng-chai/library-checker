# Range Affine Point Get / 区间仿射、单点查询

## Algebra / 代数基础

An update is `f(x)=ax+b (mod 998244353)`. Applying `f` and then `g` gives
`g(f(x))=(g.a*f.a)x+(g.a*f.b+g.b)`, so pending updates can be represented by
one affine function, but their chronological order must never be reversed.

每次修改是 `f(x)=ax+b (mod 998244353)`。先做 `f` 再做 `g` 得
`g(f(x))=(g.a*f.a)x+(g.a*f.b+g.b)`，因此多个待处理修改可合成一个仿射
函数，但复合不可交换，时间顺序不能颠倒。

## Online dual segment tree / 在线对偶线段树

`dual_segment_tree.cpp` stores tags rather than interval sums. Before a range
update, tags on the two boundary paths are pushed downward; the new function
is then attached to the usual `O(log N)` canonical nodes. This push is what
guarantees that an older ancestor tag is placed below a newer descendant tag.
A point query pushes one root-to-leaf path and applies the leaf tag to the
initial value.

`dual_segment_tree.cpp` 只保存标记，不维护无用的区间和。区间修改前先下推
左右边界路径，再给 `O(log N)` 个规范节点打新标记。边界下推保证“旧祖先标记”
先落到子节点，再接上“新后代标记”。单点查询只下推一条根到叶路径，并把叶标记
作用于初值。

## Wide SIMD tree / 宽 SIMD 树

`wide_simd_tree.cpp` is the performance version. It uses branching factor
eight. Every boundary block contains exactly eight affine tags, so AVX2 updates
all lanes together; fully covered blocks become one tag on the next level.
The tree height drops from about 19 to 7.

Values use 32-bit Montgomery form. A multiplication replaces division by the
constant modulus with low-word multiplication and a shift. Queries collect the
seven tags on their point path. After all operations, eight collected queries
are evaluated together with AVX2 and converted back from Montgomery form.
This batching is legal because collection snapshots every tag needed at that
exact time; later updates cannot alter the stored snapshot.

`wide_simd_tree.cpp` 是性能版，分支数为 8。边界块恰有八个仿射标记，可用
AVX2 同时更新；完整覆盖的块提升为上一层的一个标记，树高由约 19 降到 7。
数值采用 32 位 Montgomery 表示，以低位乘法和移位代替常数模除。查询发生时
保存该点七层路径上的全部标记，所有操作结束后每八个查询并行求值并转回普通
模数表示。保存的是查询当时的完整快照，所以离线批量计算不会改变语义。

## Treap alternative / Treap 替代解

`implicit_treap.cpp` splits at `l` and `r`, attaches one lazy affine tag to the
isolated middle tree, then merges the pieces. It is expected `O(log N)`, supports
future insertion/erasure naturally, and is an independent cross-check.

`implicit_treap.cpp` 在 `l,r` 处分裂，给中段根打一个懒仿射标记后再合并；
期望 `O(log N)`，天然便于扩展插入删除，也提供了独立交叉验证。

## Correctness / 正确性

Each stored tag is the chronological composition of exactly the updates not
yet propagated below that node. Pushing composes it before any newer child tag,
preserving this invariant. Hence the tags from leaf level upward describe all
updates affecting one point in time order. The wide tree applies the same
invariant to groups of eight; SIMD changes only how eight independent lanes are
computed. / 每个标记恰好表示尚未下传的修改，并按时间顺序复合；下推把它放在
更晚的子标记之前，故不变量保持。自叶向根取得的标记正好覆盖目标点的全部修改。
宽树只把八个独立标量运算并行执行，逻辑完全相同。

## Complexity and engineering / 复杂度与工程

- Binary dual tree: update and query `O(log N)`, memory `O(N)`.
- Wide tree: update `O(log_8 N)` vector blocks, collection `O(log_8 N)`,
  memory `O(N+Q log_8 N)`.
- Implicit Treap: expected `O(log N)` per operation, memory `O(N)`.

All variants use direct unsigned six-/nine-digit parsing and padded `u32`
output. On focused seven-repeat measurements, the wide tree used `167.053 ms`
versus `167.409 ms` for accepted public leader `#269952` on `max_random_00`;
the small case was `10.576 ms` versus `10.832 ms`. The binary tree remains the
recommended concise online template. / 所有变体使用相同的最快 I/O。七轮聚焦
测量中，宽树在最大随机用例为 `167.053 ms`，公开第一 `#269952` 为
`167.409 ms`；小用例为 `10.576 ms` 对 `10.832 ms`。二叉版本则是更推荐的
简洁在线模板。
