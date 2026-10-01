# 复用工作区的 Kruskal

`MSTEdge{u,v,weight,id}` 保存原边号。`kruskal_workspace(n,input,scratch)` 原地稳定基数排序 input，返回 `MSTView{cost,edge}`；edge 是借用 scratch 的 span。

scratch 容量至少为 `max(m,ceil((2n-1)/4))` 个 MSTEdge。排序完成后，用 placement new 将其存储重新用于并查集和答案数组，明确切换对象生命周期。调用方可以使用静态数组或自己管理的缓冲，不需要额外分配。

四个字节直方图合并统计，全部权重相同时跳过排序；否则固定四趟分配，便于编译器展开并确定工作区地址。分配时预取后续写入位置，并查集阶段预取后续边的端点。并查集按大小合并并压缩路径。输入顶点合法、权重为 u32；断开的图返回最小生成森林，单次求解会消费工作区。
