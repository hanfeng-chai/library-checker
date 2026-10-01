# 经典最小生成树

`kruskal(n,edges)` 用基数排序和并查集，`boruvka(n,edges)` 每轮选各分量最便宜的边并压缩剩余边表；两者支持无向非负 u32 权图，返回费用和原边号，非连通时返回最小生成森林。

`prim(n,edges)` 是邻接表加二叉堆的经典 Prim；`dense_prim` 使用连续邻接矩阵。这两个接口要求图连通。矩阵版本为 O(n²+m) 时间、O(n²) 空间。

题目的主解使用 [kruskal_workspace.h](kruskal_workspace.md)，允许调用方提供和复用工作区。上述接口返回拥有内存的 Buffer，适合作为经典算法对照。
