# rectangle_add_sum

`rectangle_add_sum<P>(rectangles,queries)` 返回静态矩形加权后，各查询矩形内的总和。
WeightedRectangle 为 {left,down,right,up,weight}；Query 类型提供四个同名坐标字段。
坐标为 u32，矩形半开，允许退化矩形，weight 为规范余数。

扫描横坐标。每个矩形的左右边界在纵坐标两端加入相反的角点贡献，表示为
w·(x−x0)·(y−y0) 的四个双线性系数。Fenwick 按 y 维护四路模和；查询取左右
横前缀与上下纵前缀的差。全部端点一起排序直接得到下标，执行阶段不做坐标二分，
并预取后续第八个更新的记录与两个底层树节点。

时间 O((n+q)log(n+q))，空间 O(n+q)。GCC/Clang 官方及重叠/相接/退化/空输入/
完整 u32 坐标的小模数与 u128 面积对照、sanitizer 通过。
