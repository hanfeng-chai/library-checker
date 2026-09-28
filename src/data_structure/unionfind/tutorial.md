# Union Find / 并查集

## 中文

并查集维护若干互不相交的集合，支持合并两个集合和判断两点是否同属
一个集合。

`rank_path_compression.cpp` 使用经典递归路径压缩与按秩合并：查找时把沿途节点直接
连到根；合并时让矮树挂到高树。`size_path_halving.cpp` 的 `DisjointSetUnion`
把根节点的负集合大小和非根的父亲编号放在同一个 `int` 数组中，并
用迭代路径压缩避免递归调用。按大小合并保证小树挂到大树。

两种写法的均摊复杂度都是 `O(alpha(N))`，其中反 Ackermann 函数在
实际范围内小于 5；空间 `O(N)`。合并不改变连通分量内部的连通性，
且每条新增边恰好合并其两个端点所在分量，因此查询正确。

两份源码都使用 direct mapping、六位顶点/查询路径、一位固定操作码和一位
固定输出；递归版只简化 DSU 核心，没有退回通用 I/O。

## English

Both variants use path compression and union by rank/size, giving amortized
`O(alpha(N))` operations and `O(N)` memory. `rank_path_compression.cpp` is the textbook
recursive implementation. `size_path_halving.cpp` packs parent and negative component size
into one array and compresses paths iteratively.

Both use direct mapping, six-digit bounded fields, fixed one-digit operation
codes, and fixed one-digit answers.
