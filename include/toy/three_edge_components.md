# 三边连通分量

`three_edge_components(indexed_graph)` 返回 VertexGroups。无向重图的 DFS 维护尚未吸收的路径和回边计数；出现第三条独立连接时将路径上的顶点并入同一组。使用显式 DFS 栈和并查集，自环不影响分组；只按原边号跳过一条父边。
