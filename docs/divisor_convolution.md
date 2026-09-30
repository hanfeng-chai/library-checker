# divisor_convolution

`divisor_convolution<Divisor::Gcd/Lcm, P>(std::move(a), std::move(b))`
消费两个等长 Buffer。下标 0 不参与，返回下标 1 到 N 的 GCD/LCM 卷积；
LCM 大于 N 的项舍弃。输入和输出均为 `[0, P)` 内的普通余数，
P 是小于 2^30 的奇数，不要求素数。

按素数逐层做约数/倍数 zeta 变换，点乘后反向遍历完成 Möbius 逆变换。
A/B 正变换合并访问；B 单独转入 Montgomery 表示，点乘直接得到普通余数。
转换和点乘使用 AVX2，筛素数使用 `prime.h` 的奇数位图。
时间复杂度 O(N log log N)，额外空间 O(N)。

2026-09-30，Lenovo 独立空闲、不绑核、环境检查通过。
非样例各三次取中位数，max/sum 包含全部测例，单位 ms：

| 题目 | main max | main sum | 最快对照 max | 最快对照 sum |
| --- | ---: | ---: | ---: | ---: |
| GCD | 63.649 | 595.721 | 79.373 | 763.520 |
| LCM | 67.474 | 634.923 | 81.283 | 792.180 |

最快对照均为 oldyan，两个汇总指标均胜过五份提交（LCM 包括 Rust 的 urectanc）。
每题 29 个官方测例通过，另外通过小规模朴素卷积、素数表和 ASan/UBSan 检查。
完整记录在 `bench/divisor-20260930/round1/`。
