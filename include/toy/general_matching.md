# 一般图最大匹配

`GeneralMatching(n,edges)` 的 `mate` 使用 1-based 内部顶点，0 表示未匹配，`size` 是边数。先贪心匹配，再用 Edmonds 交替森林收缩奇环；前驱链接在翻转增广路时隐式展开花。O(n³) 的经典算法，邻接表空间 O(n+m)。
