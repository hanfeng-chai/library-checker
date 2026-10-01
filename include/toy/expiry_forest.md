# 按到期时间维护动态连通性

`expiry_component_sum(initial,operations)` 与 dynamic_component_sum 使用相同接口。预处理每次插边的删除时刻，将其作为最大瓶颈森林的边权；删除发生时，它是当前最小的到期标签，所以无需寻找替代边。

`ExpiryForest` 在 AM-tree 的旋转和捷径中同时维护子树大小、分量和、删除时刻到节点的索引。保持经典回滚实现作对照；此带删除的扩展不直接套用纯增量 AM-tree 的摊还复杂度证明。
