# wavelet_sum

`WaveletSum<T=u64>(xs,ys,weights)` 预登记所有坐标与初值，add(id,delta) 增加指定点的
权重，rectangle(l,d,r,u) 返回半开矩形的权和。可登记重复坐标与初始零权的未来点；
更新按登记编号寻址。n<2^31，累计值不超出 T 的范围。

按 x 排序、压缩 y 后建立 Wavelet Matrix。每层只为零分支保存 Fenwick，
无需对一分支或顶层总和重复更新。查询先下降共同高位，再处理上下边界，
到边界的尾部零位即可停止。Fenwick 两条前缀路径的公共部分也直接消去。

坐标查找使用条件移动，两个端点交替推进，以减少随机分支并重叠访存。
位图每 64 点记录一份此前的 1 数量，rank 由这份计数加 popcount 得到。
空间 O(n log n)，更新和查询 O(log² n)。

`WaveletSum<T,true>` 提供对偶接口 add_rectangle(l,d,r,u,delta)、get(id)：
Fenwick 改存差分，只更新零分支，顶层另保存覆盖整个 y 范围的差分及初值。
这一接口保留作通用在线方案；rectangle_add_point_get 当前主解采用更快的离线时间分块。

普通/对偶版本均通过独立朴素对照、GCC/Clang 与 sanitizer，覆盖重复点、空输入、
位图边界、完整 u32 坐标，以及在更新后才查询未来点的情况。
