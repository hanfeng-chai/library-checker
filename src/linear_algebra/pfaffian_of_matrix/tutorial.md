# pfaffian_of_matrix

每次选一个非零 2×2 主元块，用反对称 Schur 补去掉两个顶点，累乘主元并记录交换符号。只更新上三角，配对主元共享 AVX2 目标加载，运算全部精确。

见 [pfaffian.md](../../../include/toy/pfaffian.md)。
