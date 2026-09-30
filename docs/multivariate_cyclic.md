# multivariate_cyclic

`multivariate_cyclic(dimensions, std::move(a), std::move(b), p)`
计算各维循环卷积。p 是小于 2^30 的素数，各维长度至少 2，且整除 p-1；
数组按最低维优先展开，零维为常数相乘。

依次沿每一维做 DFT，点乘后逆变换。长度 2 直接做蝶形，长度不超过 16
使用小矩阵；其他长度用 chirp-z，预计算固定卷积核的三个素数频谱。
三角数指数 `i*(i-1)/2` 避免为偶数长度额外要求二倍阶单位根。
`CyclicCRT` 精确还原运行期模数下的循环卷积，复用频谱，不用浮点数。

较大维度的 `bit_ceil(2*dimension-1)` 须不超过 2^24。
24 个官方测例通过 GCC/Clang，72 项朴素对照、完整 u32 乘数的 Barrett 检查
及 ASan/UBSan 通过。

2026-09-30，Lenovo 独立空闲、环境检查通过、不绑核，每个测例测一次。
main 的 max/sum 为 **225.810/1350.609 ms**，均胜过下载的五份提交。
最快对照 Yuezheng_Ling_fans 为 **290.111/2600.385 ms**。
完整记录在 `bench/gf-multi-20260930/round1/`。
