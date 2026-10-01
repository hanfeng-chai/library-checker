# 欧拉迹

`euler_trail(n, edges, directed)` 返回 `exists`、顶点序列和原边号序列，存在时长度分别为 m+1、m。度数条件筛除不可能的图，Hierholzer 显式栈消费边；最终边数校验排除非连通的有效边集。支持自环、重边、空边集；n 至少为 1。O(n+m)。
