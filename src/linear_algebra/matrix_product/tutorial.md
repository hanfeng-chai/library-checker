# matrix_product

`main.cxx` 使用 Strassen–Winograd，`naive.cxx` 保留经典立方复杂度的分块乘法。二者共用同一个 4×8 AVX2 微内核、Montgomery 编码和批量读写，比较只改变乘法算法。

见 [matrix_product.md](../../../include/toy/matrix_product.md)。
