# centroid

`centroid_decompose(graph,visit)` 要求输入为无向树，返回每点在点分树中的父亲，
点分根的父亲为 UINT32_MAX。空树返回空数组。

visit(center,points,starts) 在选定重心后调用：points[0] 是重心，距离为零；
随后每个 starts 区间对应去掉重心后的一支，元素为 {vertex,distance}。
这些 span 借用工作区，只能在本次回调内使用。每支至多包含当前分量的一半顶点。

收集分量、求子树大小、按支收集距离都用迭代扫描，工作区复用，无线性深度递归。
时间 O(n log n)，辅助空间 O(n)。BFS 对照检查距离、支的划分、平衡条件以及
每个点恰好成为一次重心，GCC/Clang 与 sanitizer 通过。
