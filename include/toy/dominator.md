# 支配树

`dominator_tree(n,edges,root)` 返回每点的直接支配者，根的父亲为自身，不可达点为 -1。采用 Lengauer–Tarjan 半支配树算法，DFS 和带标签的并查集路径压缩均使用显式栈。反图遍历前驱，bucket 延迟确定直接支配者；O(n+m) 空间。
