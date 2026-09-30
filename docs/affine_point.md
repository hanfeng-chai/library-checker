# affine_point

`AffinePointTree<P>(values, queries)` 支持区间仿射变换 `apply(l,r,{a,b})`、
单点 `get(i)`。P 为小于 2^30 的奇数模数，输入为规范余数。

八叉树按层保存乘法/加法懒标记，边界路径先下推，再 SIMD 更新完整孩子组。
底层直接保存元素值，省去无用乘法标记。内部 Montgomery 冗余余数位于 [0,2P)，
减少每次乘法后的规约；AVX2 分别处理偶数、奇数 lane 的 64 位乘积。

若答案不参与后续操作，可用 `collect(i)` 保存查询时各层标记，最后 `resolve()`
并行计算八个答案。构造参数 queries 是两次 resolve 间的查询容量；返回视图直到
下一次 collect/resolve 才失效。普通 get 无额外存储，时间 O(log n)；
批量暂存 O(queries log n)，树空间 O(n)。GCC/Clang 官方、随机对照及 sanitizer 通过。
