# range_add_min

`RangeAddMin(values)` 支持 `add(l,r,delta)`、`minimum(l,r)`，使用 i64 算术。
空区间最小值返回 infinity=2^60；要求实际值、累积偏移与差值远离该哨兵且不溢出。

每个节点含八个相对父最小值的孩子最小值，归一化后最小差为零。
局部更新用 AVX2 修改完整覆盖的孩子，重新归一化后向父层传递差值。
全区间更新只增加 root。查询沿边界累积相对值，结合完整孩子最小值。
时间 O(log n)，空间 O(n)。prefetch(l,r) 可提前读取将访问的底层节点。
GCC/Clang、官方与独立朴素对照及 sanitizer 通过。
