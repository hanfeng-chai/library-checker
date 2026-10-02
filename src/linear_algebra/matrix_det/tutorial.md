# matrix_det

`main.cxx` 复用素数模行列式内核，两列主元、两条目标行共用加载，每八次乘积才收缩累加范围。`naive.cxx` 保留每步约减的经典高斯消元，使用相同读写模板。

见 [determinant.md](../../../include/toy/determinant.md)。
