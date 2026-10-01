# 显式 DFS 栈找环

`graph_cycle(indexed_graph, directed)` 返回 `GraphCycle{vertex,edge}`。两列长度相同，`edge[i]` 连接 `vertex[i]` 与下一个顶点，末尾回到起点；空结果表示无环。无向模式按边号跳过父边，因而支持自环、平行边。时间、空间 O(n+m)。
