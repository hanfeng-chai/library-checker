# Lowest Common Ancestor / 最近公共祖先

## 中文

### 1. 问题与本题的额外结构

`LCA(u, v)` 是从根到 `u`、`v` 两条路径上深度最大的公共顶点。
本题不仅是一棵静态有根树，还保证：

```text
parent[v] < v
```

也就是说，编号本身就是一种拓扑序。通用的 Euler Tour、Tarjan 离线
算法当然仍然适用，但性能主解可以利用这个额外条件，省掉邻接表、深度和
`O(N log N)` 祖先表。

本目录保留四种完整解法：

| 文件 | 算法 | 预处理 | 单次查询 | 额外空间 | 查询方式 |
|---|---|---:|---:|---:|---|
| `schieber_vishkin.cpp` | Schieber--Vishkin | `O(N)` | `O(1)` | `O(N)` | 在线 |
| `block_rmq.cpp` | 特殊 DFS 序 + 分块 RMQ | `O(N + N/B log(N/B))` | 最多扫描 `B` 项 | `O(N + N/B log(N/B))` | 在线 |
| `euler_sparse_table.cpp` | Euler Tour + Sparse Table | `O(N log N)` | `O(1)` | `O(N log N)` | 在线 |
| `tarjan_offline.cpp` | Tarjan + 并查集 | `O((N+Q) alpha(N))` | 批量求解 | `O(N+Q)` | 离线 |

### 2. 主解：Schieber--Vishkin

这是一种真正线性预处理、常数时间查询的经典 LCA 算法。核心思想是把根到
顶点的路径压缩成若干条带编号的竖直链，再用一个整数位掩码同时表示路径上
经过了哪些链。

#### 2.1 用叶子区间产生链标签

先自底向上计算每棵子树中的叶子数。随后为叶子分配从 `1` 开始的连续编号。
每棵子树的叶子编号必然形成连续区间。

对每个顶点 `v`，在它的叶子区间中选取 `lowbit` 最大的编号作为
`inlabel[v]`：

```text
lowbit(x) = x & -x
```

实现不需要真的枚举区间。先让叶子保留自己的编号、内部点标签清零，再按
逆拓扑序把 `lowbit` 更大的孩子标签向父亲传播即可。

选择最大 `lowbit` 有一个关键性质：具有相同 `inlabel` 的顶点构成一条连续
的祖先链，不会分叉。`head_parent[label]` 记录这条链最高点的父亲，因此可在
一次数组访问中跳到链的上方。

#### 2.2 用位掩码编码根路径

令：

```text
ascendant[v]
    = 根到 v 路径上所有链标签的 lowbit 的按位或
```

不同的 `lowbit` 是不同二进制位，所以一个 32 位整数就足以表示本题最多
50 万个叶子所需的所有层级。

查询 `u, v` 时：

1. `inlabel[u] ^ inlabel[v]` 的最高位给出两条链第一次可能分开的尺度；
2. `ascendant[u] & ascendant[v]` 保留两条根路径共同经过的链；
3. 清掉低于分叉尺度的位，得到最深的公共链信息；
4. 对每个端点取最高的不同位，通过 `head_parent` 一次跳到该公共链附近；
5. 两个候选已位于同一祖先链上。由于 `parent[v] < v`，较小编号就是较浅的
   候选，也就是 LCA。

整个查询只有整数位运算和少量随机数组读取，没有循环。

#### 2.3 正确性要点

**引理 1：子树叶子区间连续。** 根的区间被依次切成各个孩子的区间，而每个
孩子再以同样方式切分，所以由归纳法可知任意子树都占据一个连续区间。

**引理 2：相同 `inlabel` 的顶点形成祖先链。** 一个顶点选择其叶子区间中
`lowbit` 最大的编号。若父子选择同一编号，它们属于同一链；一旦某个孩子
选择不同编号，两个不相交的孩子区间不可能在更深处重新得到相同编号。因此
同标签集合连续且不分叉。

**引理 3：`ascendant[v]` 精确记录根路径经过的链层级。** 根到 `v` 逐边
递推，每到一个顶点就加入其 `inlabel` 的 `lowbit`，故结论直接成立。

**定理。** 查询中的异或最高位确定两端标签分离的最高尺度；共同
`ascendant` 掩码确定两条路径仍共享的链层级。`head_parent` 调整后，两个
候选落到包含最近公共祖先的同一条链上。该链上祖先编号严格小于后代编号，
所以返回较小候选恰为 `LCA(u,v)`。

### 3. 替代解一：特殊 DFS 序 + 分块 RMQ

`block_rmq.cpp` 与 OJ 第一名 `#278239` 属于同一算法族。

利用子树大小，可以在线性时间构造一个“反向孩子顺序”的 DFS 编号
`order[v]`。在位置 `order[v]` 放置 `parent[v]`。对两个不同顶点，设
`l < r` 为它们的 DFS 位置，则：

```text
LCA(u, v) = min(parent_at_order[l + 1 ... r])
```

原因是这个区间必然跨过一条从 LCA 直接进入目标子树的边，其值正好是 LCA；
区间内其他边的父端都是 LCA 的后代。由于父编号小于子编号，它们都不可能
比 LCA 更小。

RMQ 将数组分成大小为 `B` 的块：

- 每个位置保存块内前缀最小值和后缀最小值；
- 完整块的最小值建立 Sparse Table；
- 查询的两个边缘块由前后缀回答，中间完整块由两个 Sparse Table 区间回答；
- 两个端点落在同一块时直接扫描，最多检查 `B` 个元素。

该版本接近公开榜首，结构也很适合作为静态幂等 RMQ 模板。

### 4. 替代解二：Euler Tour + Sparse Table

显式栈 DFS 在进入顶点和从孩子返回时记录顶点。设 `first[v]` 为 `v` 在
Euler 序中的第一次出现位置，则区间
`[first[u], first[v]]` 中深度最小的顶点就是 LCA。

对 Euler 序按“深度较小者”建立 Sparse Table。最小值运算具有幂等性，
长度为 `len` 的查询可用两个长度 `2^floor(log2(len))` 的重叠区间回答。

这套方法不依赖 `parent[v] < v`，适用于任意静态有根树；代价是
`O(N log N)` 时间和空间。

### 5. 替代解三：Tarjan 离线 LCA

当所有查询可预先读入时，可以把每条查询挂到两个端点。DFS 完成一个孩子
后，将孩子的并查集连到父亲；当查询的另一端已经完成时，它当前的并查集
代表元就是 LCA。

每条树边和查询边只处理常数次，复杂度为
`O((N+Q) alpha(N))`，空间为 `O(N+Q)`。它不能边读查询边输出，但展示了
经典的离线解法。实现使用显式 DFS 栈，极深链也不依赖进程栈上限。

### 6. I/O 与性能选择

本题顶点和查询端点位于 `[0, 500000)`，数值近似均匀而位数可变。因此主解
使用：

```cpp
input.read_uniform<6, uint32_t>()
```

它会选择 4-byte SWAR 路径，与公开快解的 `rd::uh()` 是同一种底层解析。
官方测试和 OJ 使用普通文件输入，所以性能入口使用已验证的
`direct_mapping`。答案严格小于一百万，输出使用
`write_token_u32_6`；checker 接受任意空白分隔，不要求每个答案独占一行。

整题消融比较过 scalar、通用 SWAR、SIMD、padded 与 compact token 路径，
没有仅凭最大位数选择 API。统一高性能编译参数下，当前主解在最大随机数据上
快于 OJ 第一 `#278239`，并与本机最快的 `#354023` 处于接近区间；后者在
OJ 榜单仅列第五，说明本机和 OJ 的排序会受机器与运行噪声影响。

### 7. 边界情况

- `u == v` 时答案就是自身；
- 一个点是另一个点祖先时，链跳转仍会落到该祖先；
- `N = 1` 时没有父亲输入，所有查询答案都是根；
- 所有主数组为固定容量且位于静态存储区，不会把数十 MiB 放到线程栈；
- 四种实现都应在 50 万点长链上工作，不把递归栈大小当作正确性前提。

---

## English

### 1. Problem and additional structure

`LCA(u, v)` is the deepest vertex shared by the two root paths. This instance
also guarantees `parent[v] < v`, so vertex IDs form a topological order.
Generic Euler-tour and Tarjan algorithms still work, but the performance
solution can exploit this order and avoid an `O(N log N)` ancestor table.

The directory contains four complete solutions:

| File | Algorithm | Preprocessing | Query | Extra memory | Mode |
|---|---|---:|---:|---:|---|
| `schieber_vishkin.cpp` | Schieber--Vishkin | `O(N)` | `O(1)` | `O(N)` | online |
| `block_rmq.cpp` | special DFS order + blocked RMQ | `O(N + N/B log(N/B))` | scans at most `B` entries | `O(N + N/B log(N/B))` | online |
| `euler_sparse_table.cpp` | Euler tour + sparse table | `O(N log N)` | `O(1)` | `O(N log N)` | online |
| `tarjan_offline.cpp` | Tarjan + disjoint sets | `O((N+Q) alpha(N))` | batched | `O(N+Q)` | offline |

### 2. Main solution: Schieber--Vishkin

Schieber--Vishkin is a classic linear-preprocessing, constant-query LCA
algorithm. It partitions every root path into vertical labelled chains and
encodes all chains on a path in one integer bit mask.

#### 2.1 Labels from leaf intervals

Count the leaves in every subtree and assign consecutive IDs starting at one
to the leaves. The leaves of each subtree occupy a contiguous interval.

For a vertex `v`, choose the number in its interval with the largest

```text
lowbit(x) = x & -x
```

as `inlabel[v]`. The implementation need not scan an interval: leaves retain
their IDs, internal labels start at zero, and a reverse topological pass
propagates the child label with the largest low bit.

Vertices with one `inlabel` form one contiguous ancestor chain.
`head_parent[label]` stores the parent immediately above the chain head, which
allows a chain jump with one lookup.

#### 2.2 Encoding a root path

Define `ascendant[v]` as the bitwise OR of
`lowbit(inlabel[x])` over the root-to-`v` path. Distinct low bits occupy
distinct bit positions, and 32 bits cover at most 500,000 leaves.

For a query:

1. the highest bit of `inlabel[u] ^ inlabel[v]` gives the scale at which the
   two labels diverge;
2. `ascendant[u] & ascendant[v]` keeps chain levels shared by both root paths;
3. levels below the divergence are masked out;
4. the highest remaining difference for each endpoint selects one
   `head_parent` jump;
5. both candidates are now on the chain containing the LCA. Because parents
   have smaller IDs than descendants, the smaller candidate is the LCA.

There is no loop in a query, only bit operations and a few array accesses.

#### 2.3 Correctness outline

**Leaf-interval lemma.** The root interval is partitioned into consecutive
child intervals, recursively, so every subtree owns one contiguous interval.

**Chain lemma.** A vertex selects the maximum-lowbit number of its interval.
Equal selected numbers can continue through parent-child edges, but disjoint
sibling intervals can never merge below their parent. Thus equal labels form
one non-branching ancestor chain.

**Path-mask lemma.** The recurrence for `ascendant` inserts exactly the
low-bit signature of each chain crossed by a root path.

**Theorem.** The highest differing label bit and the intersection of the two
path masks identify their deepest common chain level. The `head_parent`
adjustments move both endpoints to that chain. Its two candidates are
ancestor-related; topological vertex numbering makes the smaller one exactly
`LCA(u,v)`.

### 3. Alternative: special DFS order and blocked RMQ

`block_rmq.cpp` uses the same algorithm family as OJ leader `#278239`.
Subtree sizes construct a reverse-child DFS order in linear time. Store
`parent[v]` at `order[v]`. For distinct endpoints at positions `l < r`,

```text
LCA(u, v) = min(parent_at_order[l + 1 ... r]).
```

The interval crosses an edge directly below the LCA, whose stored value is the
LCA. Every other stored parent is its descendant and therefore has a larger
ID.

Blocked RMQ stores per-position prefix/suffix minima and a sparse table over
whole-block minima. Only a same-block query scans directly, inspecting at most
`B` entries.

### 4. Alternative: Euler tour and sparse table

An explicit-stack DFS records vertices on entry and after returning from a
child. The minimum-depth vertex between the first occurrences of `u` and `v`
is their LCA. A sparse table answers this idempotent RMQ with two overlapping
power-of-two intervals.

This method works for arbitrary static rooted trees without ordered parent
IDs, at the cost of `O(N log N)` preprocessing and memory.

### 5. Alternative: offline Tarjan

Attach every query to both endpoints. After a DFS child is completed, union
its set into the parent. When the other query endpoint has already completed,
its current disjoint-set representative is the LCA.

The total complexity is `O((N+Q) alpha(N))` with `O(N+Q)` memory. It is
offline, unlike the other variants. The implementation uses an explicit DFS
stack, so a 500,000-vertex path does not rely on a large process stack.

### 6. I/O and performance engineering

Vertex IDs are variable-width values approximately uniform in
`[0, 500000)`. The main solution therefore calls
`read_uniform<6, uint32_t>()`, which selects the same four-byte SWAR parsing
shape as the public fast solution's `rd::uh()`. `direct_mapping` is used only
on the validated regular-file judge path.

Every answer is below one million, so `write_token_u32_6` uses the bounded
compact output path. Arbitrary whitespace is valid for the checker. Scalar,
generic SWAR, SIMD, padded, and compact-token alternatives were compared
end-to-end on the official corpus rather than selected from the maximum width
alone.

With identical high-performance compiler flags, the selected solution beats
OJ leader `#278239` on the maximum random local case and is close to locally
fastest `#354023`. The latter ranks only fifth on the OJ, illustrating why
same-host measurements and OJ timings need separate interpretation.

### 7. Edge cases

- `u == v` returns the vertex itself.
- Ancestor-descendant queries remain on the ancestor's chain.
- For `N = 1`, there is no parent input and every answer is the root.
- Large fixed-capacity arrays use static storage, not the thread stack.
- All four variants must handle a 500,000-vertex path without requiring a
  larger recursion stack.
