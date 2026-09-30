# 静态区间最小值

主解使用 [RMQ](../../../docs/rmq.md) 的 u32 特化：按 16 项分块，SIMD 构造
块内前缀/后缀最小值，整块建立 sparse table。块内查询做 SIMD 最小值归约；
跨块查询合并两个边缘与中间完整块，额外的单位元行统一处理相邻块。
输入查询分批读取并预取，答案批量输出。

GCC/Clang 官方、朴素对照与 sanitizer 通过。Lenovo 单次 max/sum 为
30.356/408.298 ms，对照最小值为 35.085/459.494 ms。
证据：`bench/ds-basic-20260930/round2/selected-rmq/`。旧 .cpp 保留作参考。
