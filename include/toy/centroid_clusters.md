# centroid_clusters

`centroid_clusters(graph, visit, leaf, limit)` 与 centroid_decompose 一样迭代寻找
重心，但剩余连通块大小不超过 limit 时直接调用 leaf(vertices)。较大连通块的
重心、分支距离和边界传给 visit；回调拿到的 span 仅在回调期间有效。

用于把分解底部的小树交给紧凑距离矩阵处理，避免大量很小的独立数据结构。
时间 O(n log n)，临时空间 O(n)，不依赖长链递归。
