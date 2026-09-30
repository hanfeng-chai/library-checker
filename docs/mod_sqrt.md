# mod_sqrt

`mod_sqrt<P>(x)` 返回奇素数模 P 下较小的平方根，非二次剩余返回 nullopt。
输入为普通余数，P<2^30。P≡3 (mod 4) 时直接用幂，其他情况用 Tonelli–Shanks。

通过小素数的穷举检查、稀疏平方根的朴素平方对照和官方测例，支持 GCC/Clang。
