# Shortest Path

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 101.153 (nandhagk), sum 976.966 (nandhagk)。

- `main.cxx` max: 96.435 ms (-4.66%), sum: 971.809 ms (-0.53%)
- `naive.cxx` max: 160.916 ms (+59.08%), sum: 1290.479 ms (+32.09%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主算法为 Dijkstra，使用连续桶的 radix heap。边权小于 2^31，使活动优先级始终处于宽度小于 2^31 的窗口；桶中用 u32 循环键，完整距离仍为 u64。目标的已发现距离作为上界。

库：[shortest_path.h](../../../include/toy/shortest_path.h), [radix_heap.h](../../../include/toy/radix_heap.h)。

## 实现与取舍

`naive.cxx` 保留二叉堆 Dijkstra。链式桶最初有较大的随机访存开销；连续桶及短队列直接扫描改善了这一点。入度或出度至多一时路径唯一，可直接回溯或游走；先用短前缀排除明显不满足入度条件的图，避免无用的大数组初始化。

## 正确性

与 Floyd 对照可达性及距离，另验证跨越 2^32 的循环桶键和随机单调队列序列。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
