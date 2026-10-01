# tree_path_affine

本头文件提供两种经典路径函数复合，并组合 weighted_path_affine.h 的接口。
构造都接收已完成的 HeavyLight 和点函数数组，提供 set(v,f)、apply(u,v,x)，
对象拥有后续所需的值与布局；构造后可释放输入。

- `ParentPathAffine<P>`：沿父亲逐点求值，更新 O(1)，查询 O(路径长度)。复用临时
  路径栈保存终点一侧，适合浅树。
- `HeavyPathAffine<P>`：普通 HLD 加 PathAffineTree，更新 O(log n)，路径查询
  O(log² n)，紧凑线段树的常数较小。
- `WeightedPathAffine<P>`：加权重链树，更新、查询 O(log n)，适合重链分解不利的树。

`affine_path_method(hld)` 返回 0/1/2：高度不超过 64 时选父链；否则估计根路径上
各重链长度的 bit_width 之和，不超过 `2*bit_width(n)` 时选普通 HLD，其他情况
选加权重链。这个选择只依赖树形，是常数优化的经验规则，也可直接指定具体结构。
主解在进入查询循环前选择具体类型，循环内不重复判断算法。

允许零乘数和奇合数模数，具体余数范围与 path_affine 相同。朴素路径对照覆盖
任意编号、初始根、正反方向和模 15/17/998244353，GCC/Clang 与 ASan/UBSan 通过。
