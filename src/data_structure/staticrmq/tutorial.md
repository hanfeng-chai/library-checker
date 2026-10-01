# 静态区间最小值

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 35.922 (nandhagk), sum 466.914 (nandhagk)。

- `main.cxx` max: 30.845 ms (-14.13%), sum: 414.879 ms (-11.14%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解使用 [RMQ](../../../include/toy/rmq.md) 的 u32 特化：按 16 项分块，SIMD 构造
块内前缀/后缀最小值，整块建立 sparse table。块内查询做 SIMD 最小值归约；
跨块查询合并两个边缘与中间完整块，额外的单位元行统一处理相邻块。
输入查询分批读取并预取，答案批量输出。

GCC/Clang 官方、朴素对照与 sanitizer 通过。

## 尝试过程与取舍

先做分块 RMQ 和中间块 sparse table，再比较 u32 的 SIMD 专门路径。最终 16 项一块，向量化前缀、后缀和块内归约，增加单位元行统一处理相邻块；查询分批读取预取后输出。
