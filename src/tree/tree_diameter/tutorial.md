# Tree Diameter / 树的直径

## 中文

树的直径是权值和最大的简单路径。本题边权为正。

### 性能解：XOR 叶剥离

`xor_leaf_peeling.cpp` 采用叶剥离 DP：每个点只保存度数、
相邻点编号异或和、相邻边权异或和。叶子的异或值就是唯一邻居及边权，删除它
后用异或从邻居状态中消去该边，并把最长下降路径贡献合并到邻居。这样无需
`2(N-1)` 条显式邻接边，连续数组和单遍扫描显著改善缓存与初始化成本。

正确性来自树路径唯一且边权为正：从任意点沿最长方向走到的最远点必可作为
某条直径端点；再从该端点取最远点便得到最大路径。

每删除一片叶子，就把“从该叶子方向到达的最长路径”合并进邻居。邻居已经
保存的最长下降链与新贡献拼接，形成一条经过邻居的候选直径。每条边恰好被
删除一次，所以时间、空间均为 `O(N)`。

### 替代解：树形 DP

`bottom_up_dp.cpp` 任选根并生成父亲优先的遍历序，再逆序处理。对每个顶点维护向子树延伸的最长
下降路径。经过该顶点的最佳路径由两个最大的“孩子下降长度 + 边权”拼成。
在所有顶点中取最大值即为直径。

同时记录下降路径端点，就能得到直径两个端点；利用父亲和深度让两端向 LCA
靠拢，可在线性时间恢复完整路径。

该方法同样是 `O(N)` 时间和空间，只遍历树一次（加一次逆序 DP），算法机制
与叶剥离不同，但代码仍保持适中。

### I/O 与同机性能

两种解法都使用 direct-mapped Reader；顶点编号用六位 value-uniform 路径，
边权用十位路径。路径中的顶点数和编号均小于一百万，统一使用
`write_token_u32_6`，只有可能达到 `5e14` 的直径长度走通用 64 位输出。
简洁 DP 版本没有换用慢 I/O。

完整 16 个官方用例、CPU 3、统一高性能 flags 的交错测量中，XOR 叶剥离版本
相对当时最快公开实现的总耗时比约为 `0.94x` 到 `0.96x`，稳定处于更快一侧。

### 边界

- `N=1` 时直径长度为 0，路径仅含顶点 0；
- 输出的是顶点数而非边数；
- 总权值最大约为 `5e14`，必须使用 64 位整数。

## English

The diameter is a maximum-weight simple path; all edge weights are positive.

### Performance solution: XOR leaf peeling

`xor_leaf_peeling.cpp` uses leaf-peeling DP. Each vertex stores only its
degree, XOR of neighbor IDs, and XOR of incident weights. At a leaf these XOR
values identify its sole edge; removing it XOR-erases that edge from the
neighbor and merges the leaf's best downward contribution.

This avoids storing `2(N-1)` explicit adjacency records. Every removed leaf
contributes its best downward path to its neighbor; combining it with the
neighbor's previous best produces a diameter candidate. Each edge is removed
once, giving `O(N)` time and memory. Parent links recorded during peeling
reconstruct the selected endpoints.

### Alternative: bottom-up tree DP

`bottom_up_dp.cpp` roots the tree and processes vertices in reverse parent-first order. For every
vertex, retain its longest downward path. The best path passing through that
vertex combines the two largest child contributions. Taking the maximum over
all vertices yields the diameter.

Recording contribution endpoints gives both diameter endpoints. Parent and
depth arrays then reconstruct their path by moving both endpoints toward
their LCA. This is also `O(N)` time and memory but is algorithmically distinct
from leaf peeling.

### I/O and same-host performance

Both variants use the direct-mapped Reader, six-digit value-uniform parsing
for vertex IDs, and the ten-digit path for weights. Path sizes and IDs use
`write_token_u32_6`; only the diameter length, which may approach `5e14`, uses
general 64-bit formatting. The clearer DP variant does not use slower I/O.

Across all 16 official cases on CPU 3 under identical high-performance flags,
interleaved measurements placed XOR leaf peeling at roughly `0.94x` to
`0.96x` the total time of the fastest public baseline available during the
audit.

For `N=1`, the answer is length zero with path `[0]`. The reported path size
counts vertices, and 64-bit arithmetic is required for the total weight.
