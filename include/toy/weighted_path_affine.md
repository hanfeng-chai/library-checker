# weighted_path_affine

`WeightedPathAffine<P>(hld, values)` 在已构造的 HeavyLight 上维护点的仿射函数。
`set(v,f)` 修改函数，`apply(u,v,x)` 按树上 u 到 v 的路径依次应用。
允许零乘数、任意初始根；P 为小于 2^30 的奇数，输入为规范余数。
对象复制所需布局，不持有 hld 或 values 的引用。

每条重链构造按“顶点加轻子树大小”加权的 Cartesian 二叉树，使跨轻边和链内
操作的深度相互抵消，总更新、查询时间 O(log n)。节点缓存整段和左半段加本点
的正反向复合，斜率积共用；查询直接作用于 x，不求逆。
元数据按 DFS 序编号，路径循环按链头的 DFS 编号决定上跳方向，减少原编号与
DFS 编号间的反复访问。初始化、空间 O(n)。

它避免普通 HLD 在不利分解下的 O(log² n)，但常数较大，不一定在长链或浅树上
更快。具体取舍及阶段数据见 vertex_set_path_composite 的 tutorial.md。
