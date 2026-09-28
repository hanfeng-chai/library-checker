# Potential Union-Find over a Non-Commutative Group
# 非交换群带势能并查集

## 中文

### 1. 为什么顺序不能交换

令每个顶点有群元素 `A_v`，定义

`weight(v)=A_root^{-1} A_v`，

父边保存 `A_parent^{-1} A_v`。若 `p` 是 `v` 的父亲，则

`weight(v)=weight(p) * edge(p,v)`。

矩阵乘法不满足交换律，所以递归压缩路径时必须严格按这个顺序相乘。查询两个
连通点 `u,v` 时返回

`weight(v)^{-1} * weight(u) = A_v^{-1} A_u`。

### 2. 合并公式

输入约束为 `A_u=A_v X`，等价于 `A_v^{-1}A_u=X`。若把 `root_u` 挂到
`root_v`，新父边为

`A_root_v^{-1} A_root_u`
`= weight(v) * X * weight(u)^{-1}`。

这三个因子的顺序是由等式唯一确定的。按大小合并若反向挂接，就对整个乘积取逆，
不能分别随意调换因子。若两点已经同根，则用查询公式与 `X` 比较，只报告约束
是否一致。

### 3. 矩阵群

本题群元素为模 `998244353` 的 `2x2` 行列式一矩阵。若
`A=[[a,b],[c,d]]`，则

`A^{-1}=[[d,-b],[-c,a]]`。

四个矩阵乘法分量都用 64 位中间值再取模。`Matrix2Group` 与加法群实现同一套
`identity/multiply/inverse` 接口，因此 `PotentialUnionFind` 不含任何针对矩阵
的特判。

### 4. 两种实现

- `path_compression.cpp`：一次 `root_and_weight` 同时求根与有序路径积，并做
  路径压缩，摊还近常数。
- `union_by_size.cpp`：只按大小合并，不压缩，父链最坏 `O(log N)`；乘法顺序
  更容易逐边检查，也能迁移到需要回滚的场景。

正确性来自父边不变量：路径有序乘积始终等于根势能的逆乘顶点势能；上面的根边
公式使新约束成立，取逆处理反向挂接，而路径压缩只把等价的多条边替换为一条。

两种实现空间均为 `O(N)`，分别为摊还 `O(alpha(N))` 和最坏 `O(log N)` 每次
操作，均通过 18/18 官方用例。固定 CPU 的 `max_random_00` 上，路径压缩版
cycles 为公开 `#236404` 的 `0.866x`，指令数为 `0.809x`；其他随机形状多为
持平或更快，只按大小版保留为更清晰的独立模板。

## English

### 1. Ordered orientation

For group-valued vertex labels define
`weight(v)=A_root^{-1} A_v`, and store `A_parent^{-1} A_v` on a parent edge.
For parent `p`,

`weight(v)=weight(p) * edge(p,v)`.

Matrix multiplication is non-commutative, so path compression must preserve
this exact order. A connected query returns
`weight(v)^{-1} * weight(u)=A_v^{-1} A_u`.

### 2. Union formula

The constraint `A_u=A_v X` means `A_v^{-1}A_u=X`. Attaching `root_u` below
`root_v` therefore requires

`A_root_v^{-1} A_root_u = weight(v) * X * weight(u)^{-1}`.

If union by size chooses the opposite direction, invert the whole product.
For an already connected pair, compare the existing ordered difference with
`X` without modifying the structure.

### 3. Matrix group

Inputs are determinant-one `2x2` matrices modulo `998244353`. Thus
`[[a,b],[c,d]]^{-1}=[[d,-b],[-c,a]]`; multiplication uses 64-bit
intermediates. `Matrix2Group` implements the same identity/multiply/inverse
interface as the additive group, so the DSU itself has no matrix-specific
branch.

### 4. Variants, proof, and performance

`path_compression.cpp` obtains root and ordered path product in one traversal
and compresses the path. `union_by_size.cpp` keeps worst-case logarithmic
height without compression, making multiplication order especially easy to
audit and suitable for rollback adaptations.

The edge invariant makes every ordered path product equal the root-relative
weight. The derived root edge satisfies the new constraint, inversion handles
the reverse attachment, and compression replaces a path by one equivalent
edge. Therefore both implementations are correct.

They use `O(N)` memory and respectively amortized `O(alpha(N))` and worst-case
`O(log N)` per operation. Both pass 18/18 official cases. On pinned
`max_random_00`, the compressed version uses `0.866x` the cycles and `0.809x`
the instructions of public `#236404`; most other random shapes are tied or
faster.
