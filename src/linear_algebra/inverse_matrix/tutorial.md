# inverse_matrix

对 [A|I] 做 Gauss–Jordan 消元，秩不足返回 -1，满秩时右半即逆矩阵。u64 懒累加减少约减次数，向量更新始终保留已经消去的列。

见 [linear_system.md](../../../include/toy/linear_system.md)。
