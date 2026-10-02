# bit_gaussian

`bit_gaussian(a,columns,reduced=false)` 是经典逐主元的 GF(2) 高斯消元，直接修改
`BitMatrix`，返回秩并写入 pivot。主元按列递增，行更新使用 AVX2 XOR。

它省去了 Four Russians 查表的准备，但每个主元都要扫描目标行。
适合小矩阵，也作为 [bit_matrix](bit_matrix.md) 的经典对照。
