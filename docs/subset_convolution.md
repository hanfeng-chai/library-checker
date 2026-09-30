# subset_convolution

`subset_convolution(std::move(a),std::move(b))` 计算
`c[S]=sum(a[T]*b[S\T],T⊆S)`，消费两个等长 Buffer；也可传入两个只读 span。
长度须为 2^K，K≤31。默认模数 998244353，乘法支持小于 2^30 的任意奇数。
`subset_division(a,b)` 返回满足 b*c=a 的集合幂级数，要求素数模数且 b[0] 非零。

对非空集合按大小分层，用 XOR 变换做秩多项式运算。
当 |A|+|B|=|A xor B| 时交集必为空，取与目标集合大小相同的秩即为子集卷积。
秩的奇偶性已经确定掩码最低位，因此只变换 mask>>1，规模减半；空集贡献单独补回。
乘法按降序秩原位写回，除法按升序秩递推，共用一份 SIMD 点积循环。
八个乘积合并约减一次，保证累加与 Montgomery 修正都不溢出 u64。

两个秩数组错开 64 字节，减少缓存冲突。大于等于 1 MiB 的这两个工作区
采用 2 MiB 对齐并提示 MADV_HUGEPAGE；默认 Buffer 分配策略保持原样。
时间 O(K² 2^K)，空间 O(K 2^K)。多元卷积的全二元维度也复用本内核。

GCC/Clang 官方测例、朴素子集乘法、一般非单位常数项的除法对照和 ASan/UBSan 通过。
Lenovo 每例一次，子集卷积 max/sum 为 **267.673/1324.843 ms**，
五份对照最小值为 304.963/1514.558 ms。
证据在 `bench/set-series-20260930/round4/` 的 set-clean 候选。
