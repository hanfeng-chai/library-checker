# 单点修改、区间频率

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 69.245 (chaihf), sum 1074.039 (Rohan_Kapri)。

- `main.cxx` max: 62.920 ms (-9.13%), sum: 961.245 ms (-10.50%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 将值映射为稠密编号，修改分解为旧值移除与新值加入，查询记录
目标值、[l,r) 和答案编号。若目标值至今未出现，答案直接保持为零。

offline_frequency.h 按值稳定分组，组内从初始位置开始依时间顺序处理所有事件。
位图保存当前值出现的位置，PrefixTree32 保存每个字的置位数，两个前缀之差
就是区间频率。结束后清除该值最终占用的位置，下一组从空状态开始。

时间 O((N+Q)log N)，空间 O(N+Q)。GCC/Clang 官方、空数组/重复赋值/位图边界
的朴素对照及 sanitizer 通过。

## 尝试过程与取舍

先将查询和修改按值分组，用同一份位图与前缀计数存储处理各组，保留组内时间顺序。与多数查询共用精确频率模块，避免为每个值建立独立动态树；正式测例和逐操作朴素对照通过后完成完整比较。
