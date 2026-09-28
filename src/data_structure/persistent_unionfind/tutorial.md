# Persistent Union-Find / 持久化并查集

## 中文

### 1. 离线版本树 + 回滚并查集

和持久化队列一样，每次操作都从旧版本产生新版本，所有版本构成一棵树。
`rollback_version_tree.cpp` 深度优先遍历版本树，并只维护一份当前并查集。

并查集使用 `parent_or_size`：根保存负大小，非根保存父亲。为了能够撤销，它只做
按大小合并而不做路径压缩；这样父链高度最多 `O(log N)`。一次 merge 只会修改
两个根，历史栈记录“父根、子根、子树旧大小”。即使两个点本来已经连通，也压入
一个空记录，使每个 merge 都能恰好对应一次 `undo()`。

进入合并版本时执行 merge，离开该版本子树时 undo；查询版本只读取 `same(u,v)`，
不会改变状态。答案带有输入顺序编号，所以 DFS 顺序不影响输出顺序。

### 2. 正确性

DFS 不变量是：进入版本 `v` 的操作前，并查集恰好包含根到 `parent(v)` 路径上的
所有合并。执行 `v` 的 merge 后，就得到版本 `v` 指定的连通关系；查询则直接在
这一关系上判断。按大小合并和普通并查集完全相同，所以两点根相同当且仅当连通。
访问完子树后，`undo()` 恢复被修改的两个数组项（空合并也正确跳过），从而重新
得到父版本状态。由版本树归纳，所有答案正确。

### 3. 在线路径复制线段树

`persistent_segment_tree.cpp` 提供真正在线的持久化方案。把
`parent_or_size[0..N)` 放进持久化线段树；一个线段树根就是一个不可变 DSU
版本。读取一个位置和写时复制一个位置均为 `O(log N)`，一次成功合并只改两个
位置，其他节点与旧版本共享。

不使用路径压缩，但按大小合并保证父链至多 `O(log N)`；寻找代表元会做
`O(log N)` 次点查询，因此同组查询和合并都是 `O(log^2 N)`。该方案无需先读完
操作，旧版本也永不被修改，适合作为在线持久并查集模板。

### 4. 复杂度、栈与性能

- 回滚版本树：时间 `O(Q log N)`，空间 `O(N+Q)`；递归深度最坏 `Q`，使用
  评测环境的大栈约定。
- 持久化线段树：每次操作 `O(log^2 N)`，每次成功合并新增 `O(log N)` 节点，
  总空间 `O(N+Q log N)`（空初始树按需创建节点）。

两种实现都通过 14/14 官方用例。回滚版在全套固定 CPU 墙钟 benchmark 中
12/14 用例更快；量化影响最小的 `perf` 计数显示，`max_random_00` cycles 为
公开 `#218108` 的 `0.613x`，指令数为 `0.763x`。在线线段树的全套用例比值
中位数约慢 `5.5x`，但提供离线版不具备的接口，因此保留而不称为 naive。

## English

### 1. Offline version tree with rollback

Every operation creates a child of an older version, so versions form a tree.
`rollback_version_tree.cpp` performs a DFS while maintaining one current DSU.

The DSU stores negative sizes at roots and parent indices elsewhere. It uses
union by size without path compression, keeping height `O(log N)` while making
updates reversible. A merge changes at most two roots and records their old
state. Even a redundant merge pushes an empty history record, so every merge
has exactly one matching `undo()`.

Entering a merge-version applies that merge; leaving its subtree undoes it.
A query-version only reads connectivity. Answers carry input-order indices, so
DFS order is irrelevant.

### 2. Correctness

Before processing version `v`, the DSU contains exactly the merges on the path
to `parent(v)`. Applying `v`'s merge therefore creates precisely version `v`;
a query observes that exact state. Standard union-by-size invariants make
equal roots equivalent to connectivity. After the subtree, `undo()` restores
both changed entries (or skips a redundant merge), re-establishing the parent
state. Induction over the version tree proves all answers.

### 3. Online persistent segment tree

`persistent_segment_tree.cpp` stores `parent_or_size[0..N)` in a persistent
segment tree. Each segment-tree root names one immutable DSU version. Point
reads and copy-on-write point assignments cost `O(log N)`, and a successful
union changes only two positions.

Union by size bounds each parent chain by `O(log N)`. A leader search performs
`O(log N)` persistent point reads, giving `O(log^2 N)` queries and unions. This
variant works online and preserves every old version without knowing the
future version tree.

### 4. Complexity and performance

- Rollback version tree: `O(Q log N)` time and `O(N+Q)` memory; recursion can
  reach depth `Q` under the judge's large-stack convention.
- Persistent segment tree: `O(log^2 N)` per operation and `O(Q log N)` added
  nodes.

Both pass all 14 official cases, and rollback wins 12/14 pinned wall-time
medians. Less-quantized `perf` counters on `max_random_00` show `0.613x` the
cycles and `0.763x` the instructions of public `#218108`. The online tree is
roughly `5.5x` slower across the corpus but exposes a strictly stronger
interface, so it remains a meaningful named alternative.
