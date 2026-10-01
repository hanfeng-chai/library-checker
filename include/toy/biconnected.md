# 点双连通分量

`BiconnectedComponents(indexed_graph)` 返回 VertexGroups。一个割点可以出现在多个组中；孤立点单独成组。显式 DFS 维护 low-link 和未归组顶点栈，子树回边无法越过父节点时弹出一个块。支持无自环的无向重图。O(n+m) 时间、空间。
