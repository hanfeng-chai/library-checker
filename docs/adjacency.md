# adjacency

`Adjacency(n,edges,directed=false)` 从端点对建立连续 CSR 邻接表，graph[v] 返回邻居
span。默认无向，每条边保存两个方向；有向模式只保存给定方向。构造时间与空间
为 O(n+m)，邻居顺序保持输入顺序，结果拥有自己的存储。
