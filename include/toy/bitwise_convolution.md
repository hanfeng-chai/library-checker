# bitwise_convolution

`bitwise_convolution<Bitwise::And/Or/Xor, P>(std::move(a), std::move(b))`
计算按位卷积，消费两个等长 Buffer，返回普通余数。长度必须是二次幂，
输入属于 `[0, P)`，P 是小于 2^30 的奇数；不要求素数。

上层蝶形递归融合正变换、点乘和逆变换，使子问题留在缓存中。
底层三个维度在一个 AVX2 向量中完成；只把 B 转成 Montgomery 表示，
XOR 的 `1/N` 也在此时乘入，逆变换不再逐层除二。

2026-09-30，Lenovo 独立空闲、不绑核、环境检查通过。非样例各三次取中位数，
max/sum 包含全部测例，单位 ms。两题均胜过下载的五份提交：

| 题目 | main max | main sum | 最快对照 max | 最快对照 sum |
| --- | ---: | ---: | ---: | ---: |
| AND | 40.537 | 142.844 | 55.792 | 197.307 |
| XOR | 43.936 | 154.072 | 56.583 | 200.353 |

此处最快对照均为 Rohan_Kapri。所有官方测例通过 GCC/Clang；
AND/OR/XOR 的 144 项朴素对照（含合数模数）及 ASan/UBSan 通过。
完整五份对照、逐测例 perf、哈希和源码快照保存在
`bench/bitwise-20260930/round1/`。
