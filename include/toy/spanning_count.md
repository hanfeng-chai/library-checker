# 生成树和欧拉回路计数

`SpanningCount<P>(n,root)` 的 `add(u,v)` 加一条有向边，`count<Wide>()` 对入度 Laplacian 余子式求行列式，计数从根向外的生成树。无向边按两个方向加入。原边总数须小于 P，自环自动抵消。

`euler_circuit_count` 用 BEST 定理：忽略孤立点，检查入出度平衡和有效边集连通性，乘以每个有效顶点的 `(outdegree-1)!`。原边有标签，计数从一条固定边起始的欧拉回路。两个接口复用 determinant；Wide=false 可选经典消元。
