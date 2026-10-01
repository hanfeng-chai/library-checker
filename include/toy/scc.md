# 迭代 low-link 分解

`StrongComponents(Adjacency)` 求有向图 SCC，`TwoEdgeComponents(Graph<IndexedArc>)` 求无向图边双连通分量。`id`、`offset`、`vertex` 给出编号与连续分组。SCC 编号为拓扑顺序。显式 DFS 栈避免递归深度限制；无向图按原边号跳过父边，支持重边。均为 O(n+m)。
