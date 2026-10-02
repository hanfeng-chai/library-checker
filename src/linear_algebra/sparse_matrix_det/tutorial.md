# sparse_matrix_det

选择非零项较少的主元行、列做精确消元，以位图维护填充变化；零行列和带权置换矩阵可直接确认答案。使用稠密数值工作区换取简单、连续的存储，遍历仍由稀疏位图控制。

见 [sparse_determinant.md](../../../include/toy/sparse_determinant.md)。
