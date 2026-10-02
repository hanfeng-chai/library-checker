# matrix_product

`matrix_product<P,Strassen>(n,m,k,a,b)` 返回 n×m 与 m×k 的乘积，均为行优先、
`[0,P)` 的规范剩余类。P 为小于 2^30 的奇素数，默认 998244353。

大矩阵用 Strassen–Winograd 的七乘十五加，并按递归四分块连续存储；各子问题
复用工作区。叶子是 4×8 AVX2 微内核，八个独立累加器共用右矩阵加载。每八项
收缩一次高字，最后 Montgomery 约减；只预编码左矩阵，输出直接是规范剩余类。
补齐的元素都是实际分配的零，不依赖越界加载。

`Strassen=false` 保留相同微内核的经典 O(nmk) 分块乘法。方阵主版本的乘法
复杂度 O(n^log₂7)，工作区 O(nm+mk+nk)；小维度直接走叶子内核。
