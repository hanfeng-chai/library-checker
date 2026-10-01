# line_hull

`LineHull(lines)` 消费直线 Buffer，构建静态下包络；minimum(x) 返回单点最小值。
`evaluate(sorted_x,output)` 对非递减坐标批量求值，要求输出长度与坐标数相同。
空直线集返回 Line::infinity，数值边界同 line.h。

按斜率递减排序，同斜率只留最小截距。相邻交点必须递增；若加入新直线使交点
倒置，就删除中间直线。比较交点使用 i128 交叉相乘，避免浮点误差与除法。
包络线按坐标增大依次成为最优，因此单点二分、批量双指针均可。
固定宽度基数排序后构造 O(n)，单点 O(log n)，批量 O(n+q)，空间 O(n)。
GCC/Clang、重复斜率/完整 i32 坐标对照与 sanitizer 通过。
