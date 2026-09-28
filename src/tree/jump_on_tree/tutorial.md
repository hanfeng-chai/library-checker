# Jump on Tree / 树上路径跳跃

## 中文

### 1. 路径公式

查询给出 `s, t, k`，要求从 `s` 沿唯一简单路径走向 `t` 后的第 `k` 个点，
起点对应 `k=0`。令 `c = LCA(s,t)`：

```text
up       = depth[s] - depth[c]
distance = depth[s] + depth[t] - 2 * depth[c]
```

- `k > distance`：目标不存在，输出 `-1`；
- `k <= up`：答案是 `s` 的第 `k` 级祖先；
- 否则从终点反向走，答案是 `t` 的第 `distance-k` 级祖先。

难点因此归结为 LCA 和任意级祖先。本目录保留一套更快的离线算法和一套通用
在线 HLD：

| 文件 | 查询模式 | 预处理/批处理 | 单次或均摊查询 | 空间 |
|---|---|---:|---:|---:|
| `offline_dfs_bucket.cpp` | 离线 | `O(N log N + Q)` | `O(1)` RMQ 后批量扫描 | `O(N log N + Q)` |
| `heavy_light_decomposition.cpp` | 在线 | `O(N)` | `O(log N)` | `O(N)` |

### 2. 性能解：离线 DFS 分桶

#### 2.1 不存双向邻接表地定根

对每个顶点只保存：

```text
degree[v]
neighbor_xor[v] = 所有邻居编号的异或
```

叶子的度数为一，所以 `neighbor_xor[v]` 就是它唯一的邻居。删除叶子时，将
它从邻居的度数和异或中消去。反复叶剥离即可得到每个非根点的父亲、子树大小
和父亲优先的 DFS 顺序，省掉 `2(N-1)` 条边记录。

#### 2.2 在 DFS 序上求 LCA 深度

设 `position[v]` 是特制 DFS 序中的位置，数组元素为该位置顶点的深度。对
不同位置 `l < r`，区间 `(l,r]` 的最小深度减一就是两点 LCA 的深度。

直观上，从左端子树移动到右端时，区间一定经过 LCA 下方的第一层，而区间中
不可能出现更浅的顶点。只需深度而非 LCA 编号，因为路径公式随后只要求把
某个端点提升确定的距离。

深度 RMQ 使用 16 元素块：

- 每个位置保存块内前缀、后缀最小值；
- 完整块的最小值建立 Sparse Table；
- 跨块查询由左右边缘和中间完整块共同回答；
- 同块最多扫描 16 项。

#### 2.3 把路径查询转成祖先查询

由 LCA 深度计算总路径长度后，每条合法查询都可改写为：

```text
在 DFS 位置 pos 对应的顶点上，求第 d 级祖先
```

无效查询使用哨兵位置 `N`。按 `pos` 计数排序所有查询，只需 `O(N+Q)`，无需
比较排序。

最后按 DFS 顺序扫描顶点，同时维护当前根路径栈。处理位置 `pos` 时，栈顶是
该顶点，`stack_size-d` 处正好是它的第 `d` 级祖先。挂在同一位置的全部查询
随即以一次数组访问回答。

这就是离线优势：在线倍增/HLD 的每次向上跳跃，被一次全局 DFS 栈扫描取代。

#### 2.4 正确性

**叶剥离不变量。** 当前叶子的异或值始终等于唯一未删除邻居；删除叶子后对
邻居再异或一次，恰好移除该边。因此记录的邻居就是定根后的父亲。

**RMQ 引理。** 两个 DFS 位置之间必跨过从 LCA 进入目标分支的边，故区间出现
深度 `depth[LCA]+1`；区间内其余点仍在 LCA 子树中，深度不会更小。因此区间
最小值减一等于 LCA 深度。

**祖先栈引理。** 父亲优先 DFS 扫描到顶点 `v` 时，先弹出上一分支，直到栈顶
等于 `parent[v]`，再压入 `v`。所以栈从底到顶始终恰为根到当前顶点的路径。

由路径公式，每个合法查询被转换为某端点的正确祖先距离；祖先栈引理保证
批量读取的顶点就是原路径上的第 `k` 个点。无效查询被独立标记为 `-1`。

### 3. 在线解：重链剖分

`heavy_light_decomposition.cpp` 调用公共
`FixedHeavyLightTree<Capacity>` 模板：

1. 父亲优先遍历求 `parent/depth/order`；
2. 逆序累加子树大小，每个点选择最大孩子作为重孩子；
3. 每条重链在分解序中连续，记录链头、位置和位置对应顶点。

一条根路径每跨过轻边，剩余子树大小至少减半，所以最多经过 `O(log N)` 条
重链。LCA 反复把更深链头跳到其父亲；第 `k` 级祖先在同一重链内可用位置
直接定位，跨链时同样只会跳 `O(log N)` 次。

它可边读查询边回答，空间仅 `O(N)`，是可复用在线模板。小于 32 层的短跳跃
使用直接父亲步进，避免 HLD 固定开销；这只是常数优化，不改变复杂度。

### 4. I/O 与性能

所有字段都在 `[0,500000]` 附近、位数可变且数值近似均匀，因此两种解法均
使用 direct mapping 和 `read_uniform<6, uint32_t>()`。合法答案小于一百万，
调用 `write_token_u32_6`；无效答案直接写入 `" -1"`。在线解并未因代码更
清晰而换用较慢 Reader/Writer。

完整 21 个官方用例、CPU 3、统一
`-Ofast -flto -fno-exceptions -fno-rtti -march=native`、交错运行的结果：

- 离线分桶版本总耗时约 `2638 ms`；
- 当时最快公开实现 `#270103` 约 `3541 ms`；
- 比率约 `0.745x`，本地快约 25.5%；
- 在线 HLD 约比该公开基线慢 8%，但提供了不同查询能力，不承担最快门槛。

### 5. 边界

- `k=0` 返回 `s`；
- `k=distance` 返回 `t`；
- `k>distance` 返回 `-1`；
- `s=t` 时仅 `k=0` 合法；
- 所有核心数组位于静态存储区，50 万点链不会消耗线程栈。

---

## English

### 1. Path reduction

For query `(s,t,k)`, let `c=LCA(s,t)`:

```text
up       = depth[s] - depth[c]
distance = depth[s] + depth[t] - 2 * depth[c]
```

If `k>distance`, no answer exists. If `k<=up`, take the `k`-th ancestor of
`s`; otherwise take the `(distance-k)`-th ancestor of `t`.

The directory retains both query models:

| File | Mode | Preprocessing/batch | Query | Memory |
|---|---|---:|---:|---:|
| `offline_dfs_bucket.cpp` | offline | `O(N log N + Q)` | constant RMQ plus one batch scan | `O(N log N + Q)` |
| `heavy_light_decomposition.cpp` | online | `O(N)` | `O(log N)` | `O(N)` |

### 2. Performance solution: offline DFS buckets

#### 2.1 Rooting by leaf peeling

Store only each degree and the XOR of its neighbor IDs. A current leaf has one
remaining neighbor, exactly equal to that XOR value. Removing the leaf
decrements the neighbor degree and XOR-erases the edge. Repeated peeling
derives parents, subtree sizes, and a parent-first DFS order without storing
`2(N-1)` adjacency records.

#### 2.2 LCA depth from the DFS order

For special DFS positions `l<r`, the minimum vertex depth in `(l,r]`, minus
one, equals the LCA depth. Moving between the positions must cross the first
level below their LCA, while no vertex in the interval can be shallower.

A 16-element blocked RMQ stores in-block prefix/suffix minima and a sparse
table over whole-block minima. Same-block queries inspect at most 16 entries.

#### 2.3 Turning path queries into ancestor queries

The LCA depth converts each valid path query into:

```text
take the d-th ancestor of the vertex at DFS position pos
```

Invalid queries use sentinel position `N`. Counting sort groups all queries by
`pos` in `O(N+Q)`.

One scan of the DFS order maintains the current root path as a stack. At a
position, index `stack_size-d` is exactly the requested ancestor, so every
bucketed query is answered by one lookup. Offline batching replaces the
per-query chain or binary-lifting walk.

#### 2.4 Correctness

Leaf peeling keeps the invariant that a leaf's XOR is its only remaining
neighbor, hence its recorded neighbor is its parent. The DFS interval crosses
depth `depth[LCA]+1` and never a shallower depth, proving the RMQ lemma. During
the final scan, popping to `parent[v]` and then pushing `v` keeps the stack
equal to the current root path. Combined with the path reduction, the selected
stack entry is precisely the requested path vertex.

### 3. Online alternative: heavy-light decomposition

`heavy_light_decomposition.cpp` uses the reusable
`FixedHeavyLightTree<Capacity>`. Subtree sizes select a largest child as the
heavy child, and every heavy chain occupies consecutive decomposition
positions.

Crossing a light edge at least halves the remaining subtree, so a root path
crosses only `O(log N)` chains. LCA jumps between chain heads, while a
within-chain ancestor is a direct position lookup. The template answers each
query online with `O(N)` memory. Very short jumps use direct parent steps to
avoid fixed HLD overhead.

### 4. I/O and measured performance

Both variants use direct mapping and `read_uniform<6,uint32_t>()` for the
approximately value-uniform bounded fields. Valid answers use
`write_token_u32_6`; invalid ones write `" -1"`. The clearer online variant
does not fall back to slower generic I/O.

Across all 21 official cases on CPU 3, interleaved under identical
`-Ofast -flto -fno-exceptions -fno-rtti -march=native` flags, the offline
variant took about `2638 ms` versus `3541 ms` for public leader `#270103`, a
`0.745x` ratio. The online HLD was about 8% slower than that baseline, but
provides a genuinely different online capability.

### 5. Edge cases

`k=0` returns `s`, `k=distance` returns `t`, and larger `k` returns `-1`.
For `s=t`, only zero is valid. Large fixed arrays use static storage, so a
500,000-vertex path does not consume the thread stack.
