# 静态区间频次

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 125.030 (IceKylin), sum 630.521 (Rohan_Kapri)。

- `main.cxx` max: 113.583 ms (-9.16%), sum: 578.229 ms (-8.29%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解把每次查询拆成右端点前缀频次减左端点前缀频次，按端点位置分桶。
顺序扫描数组，用哈希表维护已出现的次数，在端点处累加对应贡献。
因此总期望时间 O(n+q)、空间 O(n+q)，避免对高频值的长位置表反复二分。

接口见 [frequency](../../../include/toy/frequency.md)，仍提供按出现位置二分的在线接口。
GCC/Clang 官方、独立对照及 sanitizer 通过。

## 尝试过程与取舍

先用每值出现位置表的经典在线二分建立基线。第二轮把查询拆成两个端点事件，顺序扫描并用哈希表更新频次，避免对高频值长位置表反复二分。库中仍保留在线接口，主解选择已知全部查询时更快的离线处理。
