# pow_of_matrix

`main.cxx` 在 Frobenius 块内计算 x^k，再变回原坐标；块之间的耦合经修正并验证。带权函数图矩阵先完整识别，再用倍增。`naive.cxx` 保留矩阵二进制快速幂，仍使用相同的高性能矩阵乘法和输入输出。

见 [frobenius.md](../../../include/toy/frobenius.md)。
