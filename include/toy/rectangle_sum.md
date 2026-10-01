# rectangle_sum

`rectangle_sum(points,queries)` 消费 WeightedPoint{x,y,weight} 的 Buffer，
查询为 Rectangle{left,down,right,up}，两个方向均为半开区间。坐标是 u32，权值/结果是 u64。

将点的 y 与所有查询端点一起压缩，避免逐查询二分。按 x 排序点与左右端点事件，
在事件前插入 x 严格较小的点，用 WideFenwick 求 y 区间和，并按左右符号累加答案。
支持空点集、空查询和退化矩形。时间 O((n+q)log(n+q))，空间 O(n+q)。
GCC/Clang 官方、重复坐标与半开边界的独立对照、sanitizer 通过。
