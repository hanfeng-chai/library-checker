# Link-cut tree 增量最小生成森林

`IncrementalMST(n,m)` 的 `add(u,v,w,id)` 返回删除的边号，或 -1 表示直接合并。要求正且互异的 u32 权重。并查集维护连通性，LinkCutSum 用 `PathMaximum` 聚合路径最大边；每条活跃森林边复用一个树节点，替换时切断旧端点再连到新端点。经典 O(log n) 摊还更新，保留作 AM-tree 的对照。
