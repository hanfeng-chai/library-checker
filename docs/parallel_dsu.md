# parallel_dsu

`ParallelDSU<Bits=2>(n)` 维护 [0,n) 的并查集，分支数 B=2^Bits，1≤Bits≤4。
`unite(a,b,length,merge)` 合并全部 (a+i,b+i)，0≤i<length；每次真正合并底层
连通块时调用 merge(保留根,移除根)。leader(i) 返回底层根。要求 n<2^31，区间合法。

第 k 层表示长度 B^k 的区间逐位置等价。首次合并时递归合并 B 个子区间；
已经等价则跳过。任意长度由至多 B 个最长 B 的幂次区间覆盖，重叠不影响结果。
底层回调总计至多 n−1 次，可用于维护连通块权值。

构造/空间 O(n log_B n)，q 次合并总计 O(B·(q+n log_B n)·α(n))。
本题八叉比二叉、四叉更快，主解选择 Bits=3。GCC/Clang 官方、重叠区间/
空长度/非幂次边界的逐元素合并对照及 sanitizer 通过。
