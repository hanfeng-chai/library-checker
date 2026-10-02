# adjugate_matrix

对 [A|I] 做域上的完整消元。满秩时用 det(A)·A⁻¹；秩至多 n−2 时伴随矩阵为零；秩为 n−1 时用左右零空间向量的外积，并由行变换的主元乘积和缺列位置确定比例。三种情况都来自同一次消元。

见 [linear_system.md](../../../include/toy/linear_system.md)。
