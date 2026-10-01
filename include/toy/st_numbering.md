# st 编号

`st_numbering(Adjacency,s,t)` 返回顶点排名：s 为 0、t 为 n-1，其他点均有排名更小和更大的邻居。无解返回空 Buffer。DFS low-link 决定双向链表中的插入位置，最后验证每点的前后邻居条件。单点图返回 {0}；多点时 s=t 无解。O(n+m)。
