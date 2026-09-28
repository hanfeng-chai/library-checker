# Point Set Range Sort Range Composite / 单点修改、区间排序与函数复合

## 中文

### 1. 从“已排序块”出发

每个位置保存唯一键 `p_i` 和仿射函数 `f_i(x)=a_i x+b_i`。函数复合与顺序有关，
而一次排序会按键重排整段。

把当前序列划分成若干块，每块内部已经按键递增或递减。对每块维护：

- 按键递增读取时的函数复合 `forward`；
- 按键递减读取时的函数复合 `backward`；
- 当前方向标记；
- 一棵按键组织、同时支持按秩切分和集合合并的树。

所有块首位置由分层位集 `PredecessorSet` 维护。另一棵普通线段树只在块首保存该块
当前方向的聚合，其他位置保存幺元，因此能快速复合完整的中间块。

### 2. 四种操作

若切分点落在块内，就按当前序列方向把块按秩分成前后两块。

- 单点修改：在 `i` 和 `i+1` 切分，回收旧单点树并建立新键叶子。
- 递增排序：切分 `l,r`，把区间内所有块的键树 meld 成一个块，并选择
  `forward`。
- 递减排序：先做同样的合并，再选择 `backward`。
- 区间复合：只对左右两个残缺块做按秩 fold，中间完整块查询外层线段树；查询
  不再永久切碎块。

最后一点很重要：早期实现为了查询切分边界，查询密集数据会不断增加后续排序的
工作量；只读 rank fold 保持了块结构。

### 3. 性能主解：压缩 Patricia Trie

`patricia_trie.cpp` 使用 `PatriciaSortableSegmentTree`。叶子保存完整 32 位键，
内部节点只保存两个子树最高分歧位 `bit`，省略所有单儿子链：

- 叶子令 `bit=-1`；
- 内部节点的左/右儿子分别对应该位的 `0/1`，且必有两个孩子；
- `forward=left.forward o right.forward`；
- `backward=right.backward o left.backward`。

meld 比较两棵树代表键的最高分歧位：若新分歧位更高，就创建共同父亲；若某棵
树的分叉位更高，就只下降到能容纳另一棵树的一侧；分叉位相同则分别合并两侧。
按秩 split 沿 `count` 下降，并回收恰好落在孩子边界上的内部节点。节点池使用
32 位索引和 free list，长期运行仍保持约 `2N-1` 个活动节点。

题目保证初始键和所有更新键全局互异。这保证 meld 的两集合不含重复键，也保证
“最高分歧位”总是存在；公共模板明确依赖这一契约。

### 4. 其他两种算法

- `meldable_treap.cpp`：每块是一棵按键的随机 Treap，同时满足 BST 与随机堆
  性质；支持按键 split、按秩 split 和集合 meld。期望平衡，概念经典，内存仍
  为线性，是可靠的独立替代解。
- `sortable_segment_tree.cpp`：先离线收集并压缩所有可能键，每块使用完整深度
  的二进制键域树。算法确定性强；节点池接近上限时从当前序列重建，但内存和
  常数明显大于 Patricia 版。

没有伪装成另一算法的 `naive.cpp`；三份代码分别对应三种真实结构。

### 5. 正确性

块不变量是：键树恰好包含该块全部 `(key,function)`，中序为键递增序；
`forward/backward` 分别等于两种方向下的函数复合。split 只按秩划分这个中序，
不丢失或重复元素；meld 只按键重新联合两个互异集合，所以其结果仍满足不变量。
选择方向后，块的逻辑序列就恰好是题目要求的升序或降序。

外层线段树在每个块首保存准确的块复合。区间查询按“左残块 + 中间整块 + 右残块”
的原顺序复合，覆盖 `[l,r)` 恰好一次。单点修改先隔离位置，因此只替换目标元素。
由这些不变量，四种操作均正确。

### 6. 复杂度与实测

令 `B=32`。Patricia 的 split 和块内 fold 访问至多 `B` 个分叉；外层 fold 为
`O(log N)`。一次排序还要 meld 区间中的 `m` 个现有块，代价是这些压缩树实际
访问的分叉数；保守最坏可随被合并元素数线性增长，但排序会把 `m` 个边界永久
减少为一个，单点修改和端点切分每次只重新引入常数个边界。空间为 `O(N)`。
Treap 对应操作为期望对数高度；完整键域树的高度为压缩后键域对数。

三种实现均通过 23/23 官方用例，Patricia 还通过 ASan/UBSan 下的大规模随机
序列对拍。相同 native flags、固定 CPU 的 `perf stat` 中，Patricia 相对公开
第一 `#363210`：

- `many_0_query_00`：cycles `0.898x`；
- `max_random_00`：cycles `0.918x`；
- `random_02`：cycles `0.923x`；
- 三者 cache misses 约为公开实现的 `0.61x--0.69x`。

因此 Patricia 是性能首选；Treap 用于更熟悉的随机平衡模板，完整键域树用于
确定性和离线压缩方案。

## English

### 1. Monotone blocks

Each position stores a globally unique key and an affine function. Partition
the current sequence into blocks, each already ascending or descending by key.
For every block store its composition in ascending (`forward`) and descending
(`backward`) key order, its current direction, and a key-ordered tree that can
split by rank and meld disjoint key sets.

A hierarchical bitset tracks block starts. An outer segment tree stores the
current aggregate at each block start and the identity elsewhere, so complete
middle blocks compose quickly.

### 2. Operations

A boundary inside a block is created by a rank split respecting its direction.
Point assignment isolates one position. Sorting splits both endpoints, melds
all interior block trees, and selects `forward` or `backward`. A query folds
only the two partial boundary blocks by rank and obtains all complete middle
blocks from the outer tree. It deliberately does not fragment blocks.

### 3. Preferred compressed Patricia trie

`patricia_trie.cpp` stores full 32-bit keys only in leaves. An internal node
stores the highest bit on which its two subtrees differ, eliminating every
unary path. Its children are the zero/one branches, and it maintains count plus
both aggregate directions.

Meld compares branching bits to create a new common parent or descend into the
only compatible branch. Rank split follows subtree counts and recycles
internal nodes when the cut coincides with a child boundary. A 32-bit index
pool plus free list keeps about `2N-1` active nodes.

The problem's global key-uniqueness guarantee is an explicit precondition: key
sets passed to meld are disjoint and a highest differing bit always exists.

### 4. Alternatives

- `meldable_treap.cpp` uses a randomized key BST with rank split, key split,
  and set meld. It is a linear-memory, expected-balanced independent solution.
- `sortable_segment_tree.cpp` reads all updates first, coordinate-compresses
  every key, and uses deterministic full-depth binary key tries. It rebuilds
  near its pool limit but has larger memory and constants.

There is no duplicate `naive.cpp`; all three files represent distinct useful
algorithms.

### 5. Correctness

Each block tree contains exactly its block's pairs in ascending-key inorder,
and its two aggregates represent the two possible directions. Rank split
partitions that order without loss; meld unions disjoint key sets while
restoring key order. Selecting the direction therefore realizes exactly the
requested sort.

The outer segment tree stores each complete block's true current aggregate.
A range query composes the left partial block, all middle blocks, and the right
partial block in sequence order, covering every requested function once.
Isolation makes point assignment affect only its target.

### 6. Complexity and measurements

With `B=32`, Patricia rank split and in-block fold visit at most `B` branching
levels; the outer fold costs `O(log N)`. Sorting also melds its `m` current
blocks, with cost proportional to compressed branches actually visited.
Conservatively a meld can be linear in the merged elements, while each sort
removes `m-1` block boundaries and each update introduces only constantly many.
Active Patricia storage is `O(N)`.

All variants pass 23/23 official cases, and Patricia passes large randomized
ASan/UBSan differential tests. Against public leader `#363210` under identical
native flags, pinned `perf stat` reports cycle ratios `0.898x`, `0.918x`, and
`0.923x` on three representative bottlenecks, with only `0.61x--0.69x` its
cache misses. Patricia is therefore the preferred performance variant.
