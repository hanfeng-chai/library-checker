# affine_mont

`MontAffine<P>::encode(a,b)` 接收规范余数，得到仿射变换。斜率使用 Montgomery
编码，截距保持普通余数；P 为小于 2^30 的奇数，允许零斜率及合数模数。
默认构造为恒等映射，operator()(x) 接收、返回普通规范余数。

ComposeMontAffine(a,b) 表示先 a 再 b，两个混合 Montgomery 乘法完成复合。
内部系数允许冗余余数，不应直接按字段比较数学上的相等性。模数 15、17 和
998244353 下的正反序函数链及树路径朴素对照通过 GCC/Clang 与 sanitizer。
