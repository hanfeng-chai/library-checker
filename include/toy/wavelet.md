# wavelet

`WaveletMatrix(values)` 以稳定基数排序压缩 u32 值域，每层记录位图与机器字前缀计数。
kth(l,r,k) 在线查询 [l,r) 的第 k 小（从零开始），也可消费三个端点/排名 Buffer 调用 kth_batch。
要求 l<r、k<r-l。查询按层稳定映射到零组或一组，时间 O(log σ)。

构造 O(n log σ)，位图约 O(n log σ/64) 个机器字；原值表和构造工作区 O(n)。
允许单一值，零层变换直接返回该值。GCC/Clang、朴素排序对照和 sanitizer 通过。
只有一批查询时可使用 [offline_kth](offline_kth.md)，减少存储和访存。
