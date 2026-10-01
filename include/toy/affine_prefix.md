# affine_prefix

`AffinePrefixTree<P=998244353>(values)` 维护仿射函数序列的逆前缀。
P 须为小于 2^30 的奇素数，每个斜率非零；初值系数、更新参数及返回分数均为
Montgomery 编码。下标为 u32，n<2^31。与支持零斜率的 affine_tree.h 分开使用。

change(i,scale,translation) 应用局部纠正 E=new_function⁻¹∘old_function，
第四个可选重载参数为预先计算的 1−scale。query(l,r,x) 的 x 是普通规范余数，
返回 numerator/denominator，表示按顺序复合 [l,r) 后的值；空区间返回 x。
调用者可批量求逆后一次性还原答案，避免每次查询做快速幂。

每个 16 叉节点存孩子之前的逆前缀。单点替换只需把 E 按此前的前缀共轭，
对其后的前缀同时做仿射变换，再把纠正传到上一层。AVX2 每次处理八个系数，
固定乘数的商预先算好，用 Shoup 规约。查询的左右逆前缀与分母独立累积，
两端相同的高位事先消去；各层基址也预先保存，减少地址计算。

空间 O(n)，操作 O(log₁₆ n)，每个更新层最多处理 16 个槽。存储按 2 MiB 对齐
并建议使用大页，但正确性不依赖大页是否实际启用。GCC/Clang 官方与朴素函数链
对照、空区间和 radix 边界、ASan/UBSan 通过。
