# 带附加信息的 CSR 图

`Graph<Arc>(n, edges, directed, make_arc)` 从以 `(from,to)` 开头的记录建图，`g[v]` 返回邻接区间。`indexed_graph` 保存原边号，`weighted_graph` 保存非负 u32 边权。无向边存两份；自环、重边保留。构造 O(n+m)，空间 O(n+m)。
