# multiplicative_convolution

两个接口都接收等长普通余数数组的 span，返回 Buffer，默认系数模数 Q=998244353。

`multiplicative_convolution_prime<Q>(a,b)` 要求数组长度 p 是素数。
非零下标写成原根的幂，下标乘法变成模 p-1 的加法，用普通卷积后折叠；
下标 0 的贡献单独求和。时间 O(p log p)，空间 O(p)。

`multiplicative_convolution_2n<Q>(a,b)` 要求长度是 2^N。
按下标的二进制末尾零数分层，奇数部分写成 `(-1)^s * 5^j`。
在每层的两个循环群上变换；不同层相乘时只取频点前缀，即可折叠到目标层。
最后逆变换并恢复下标。Q 的根须支持最大层长度 `max(1, 2^(N-2))`。
时间 O(N*2^N)，空间 O(2^N)。

两个算法及依赖的原根、Barrett、NTT 已通过 GCC/Clang 朴素对照和 sanitizer；
官方测例分别为 40、47 个，均已通过。

2026-09-30，Lenovo 独立空闲、环境检查通过、不绑核；非样例各三次取中位数，
单位 ms，max/sum 包含全部测例：

| 题目 | main max | main sum | 最快对照 max | 最快对照 sum |
| --- | ---: | ---: | ---: | ---: |
| 模素数下标 | 51.738 | 333.113 | 127.522 | 780.583 |
| 模 2^N 下标 | 97.603 | 678.917 | 189.186 | 1313.335 |

第一题对照 max 来自 adamant、sum 来自 Rohan_Kapri，第二题均来自 nor。
两题两项汇总均胜过五份提交，原始记录在 `bench/large-mul-20260930/round1/`。
