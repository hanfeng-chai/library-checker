# 动态树单点加与子树和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 267.496 (Rohan_Kapri), sum 1892.120 (nandhagk)。

- `main.cxx` max: 246.707 ms (-7.77%), sum: 1561.314 ms (-17.48%)
- `naive.cxx` max: 380.051 ms (+42.08%), sum: 2696.491 ms (+42.51%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 offline_forest_sum.h。查询时临时断开给定父边，读取连通块和后重新连接；按删除时刻赋权，离线维护连通块。单点加版本不保存范围加标记。

18 个正式测例通过 GCC/Clang，独立朴素对照通过 ASan/UBSan。

## 尝试过程与取舍

先实现在线 Link-cut Tree 的虚子树和。局部下传、孩子方向缓存及已有父边的一次 access 查询虽减少开销，仍落后于最快离线算法。主解改为预知删边时刻的 anti-monopoly 连通块树：查询临时断开父边，再恢复。在线经典实现保留在 naive.cxx，并复用紧凑 DFS 编号与新 LinkCutSum 接口。
