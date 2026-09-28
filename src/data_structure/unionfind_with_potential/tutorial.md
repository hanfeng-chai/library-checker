# Union-Find with Potential / 带势能并查集

## 中文

### 1. 势能方向

题目加入约束 `A_u-A_v=x (mod M)`，并查询已连通两点的差。公共模板把加法看成
群运算；对每个点定义

`weight(v)=A_v-A_root`，

父边保存 `A_v-A_parent`。沿父链把这些值相加就得到 `weight(v)`，于是

`A_u-A_v = weight(u)-weight(v)`。

代码中的通用群方向写成 `A_v^{-1} A_u`，在加法群中正好就是上式。

### 2. 合并公式

先分别一次遍历得到 `(root_u,weight_u)` 和 `(root_v,weight_v)`。若根相同，
已有差值必须等于 `x`；相等返回成功，不相等返回矛盾，并且都不修改结构。

若根不同，并准备把 `root_u` 挂到 `root_v`，新父边应满足

`A_root_v^{-1} A_root_u = weight_v + x - weight_u`

（加法记号）。按大小合并若选择了相反方向，就对整条根边势能取逆，即模意义
取负。这样新树中所有旧约束和新约束同时成立。

### 3. 路径压缩与不压缩版本

- `path_compression.cpp`：递归找到根时，把父亲到根的势能与当前父边势能按顺序
  合并，并把点直接挂到根。一次 `root_and_weight` 同时返回根和势能，避免旧版
  “先找根、再走一遍求势能”的重复遍历。
- `union_by_size.cpp`：不压缩路径，只按大小合并。树高最坏 `O(log N)`，代码
  更容易检查，也适合需要回滚/部分持久化时复用。

两者都使用同一个 `PotentialUnionFind<Group,Compress>` 公共模板和相同的最快
I/O；替代解没有在 I/O 或其他工程层面故意放慢。

### 4. 正确性与复杂度

不变量是每条父边准确保存“父势能的逆乘当前点势能”。路径相乘给出根到点的
势能；查询用第二点势能的逆乘第一点势能，得到要求的差。合并公式由约束直接
移项得到，反向挂接时取逆，因此维持不变量。按大小合并不改变集合划分，路径
压缩也只把多条等价父边替换成一条到根的边。

路径压缩版摊还 `O(alpha(N))`，不压缩版最坏 `O(log N)`，空间均为 `O(N)`。
两版均通过 18/18 官方用例。固定 CPU 的 `path_00` 上，路径压缩版 cycles 为
公开 `#224108` 的 `0.714x`，指令数为 `0.651x`；其余大多数随机用例持平或略
快。一次遍历的优化在长父链上收益最明显。

## English

### 1. Orientation

Constraints are `A_u-A_v=x (mod M)`. Define
`weight(v)=A_v-A_root`, and store `A_v-A_parent` on each parent edge. Summing a
path gives the root-relative weight, so
`A_u-A_v=weight(u)-weight(v)`. The generic group API writes the same direction
as `A_v^{-1} A_u`.

### 2. Union formula

One traversal obtains each `(root,weight)` pair. If roots agree, compare the
known difference with `x` and report consistency without mutation. Otherwise,
when attaching `root_u` below `root_v`, the new edge is
`weight_v + x - weight_u` in additive notation. If union by size reverses the
attachment, invert that entire edge value.

### 3. Variants

- `path_compression.cpp` composes parent weight and edge weight while unwinding
  recursion, then points directly to the root. `root_and_weight` returns both
  results in one pass rather than traversing the path twice.
- `union_by_size.cpp` deliberately omits compression while retaining logarithmic
  worst-case height. It is easier to audit and reusable in rollback settings.

Both instantiate the same `PotentialUnionFind<Group,Compress>` and use
identical optimized I/O.

### 4. Correctness and complexity

Every edge stores parent-potential inverse times child-potential. Ordered path
products yield root-relative weights, and inverse(second) times first is the
requested difference. The derived union edge preserves both old components
and the new constraint; reversing it handles the opposite attachment.
Compression replaces a path by an equivalent direct edge.

The compressed variant is amortized `O(alpha(N))`; union-by-size alone is
worst-case `O(log N)`. Both use `O(N)` memory and pass all 18 official cases.
On pinned `path_00`, the one-pass version uses `0.714x` the cycles and `0.651x`
the instructions of public `#224108`; most random cases are tied or faster.
