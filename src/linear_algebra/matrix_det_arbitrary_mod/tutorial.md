# matrix_det_arbitrary_mod

`main.cxx` 将模数分解为素数幂，选择赋值最小的主元，只逆单位部分，分别消元后 CRT 合并。`naive.cxx` 是不分解模数的欧几里得消元；两者共用 Shoup 行更新和批量读入。

见 [matrix_arbitrary.md](../../../include/toy/matrix_arbitrary.md)。
