# polynomial_mod

`PolynomialMod<P>(f)` 固定一个正次数模多项式，内部正规化为首一。
`reduce(std::move(a))` 处理次数不超过 `2*deg(f)-2` 的值；
`square(std::move(a))` 要求 a 已经约减；`linear_power(c,e)` 返回 `(x+c)^e mod f`。
系数为普通模余数，接口返回的零多项式为空数组。

缓存模多项式和反向级数逆的频谱，约减时用高段求商，再减去商乘模多项式。
计算一次线性多项式幂时只有平方需要卷积，乘 x+c 用线性扫描完成。
小次数直接长除。变换长度须符合模数的根容量。

GCC/Clang 通过朴素模幂和小模数对照，ASan/UBSan 通过。
