# matrix_arbitrary

`determinant_arbitrary(n,a,m)` 求任意模数下的行列式，要求 1 <= m < 2^30。
先将 m 分解为互素的素数幂，再分别消元并 CRT 合并。

模 p^e 的每列选 p-adic 赋值最小的主元，列中其他值都能整除相同的 p 次幂；
只对剩下的单位部分求逆。全过程都是可逆行加法和行交换，不要求模数为素数。
主元乘积已为零时可提前结束。

`modular_row_sub` 用固定乘数的 Shoup 商近似，AVX2 同时更新八个规范剩余类，
误差范围确保一次减模就够。`inverse_coprime` 要求两参数互素且模数大于一。

`determinant_euclid` 保留不分解模数的欧几里得消元对照；同样使用 Shoup 行更新。
