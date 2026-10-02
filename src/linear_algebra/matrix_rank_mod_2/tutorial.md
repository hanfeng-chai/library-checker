# matrix_rank_mod_2

通常用 Four Russians 分组消元；列数不大且行数更多时改用流式线性基，达到满列秩后可直接结束，因为后续行不可能再增加秩。位串按实际列宽打包，避免窄矩阵的填充开销。

见 [bit_matrix.md](../../../include/toy/bit_matrix.md)。
