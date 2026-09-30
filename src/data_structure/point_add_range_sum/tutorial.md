# 点加、区间求和

主解使用 [WideFenwick](../../../docs/fenwick.md)：十六叉节点保存孩子的前缀和，
查询每层读一个值，修改用 AVX2 同时更新该节点中后续的前缀。
一次线性建树后，操作均为 O(log n)，空间 O(n)。u64 足以容纳题目中的和。

当前主解与旧 `.cpp` 的普通 Fenwick、线段树方案共享相同问题定义；旧文件保留作参考。
GCC/Clang 官方测例、朴素对照及 sanitizer 通过。Lenovo 单次 max/sum 为
43.647/336.718 ms，五份对照中的最小值为 52.183/409.020 ms。
证据：`bench/ds-basic-20260930/round1/`。
