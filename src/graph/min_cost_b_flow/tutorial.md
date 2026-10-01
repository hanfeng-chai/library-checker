# Minimum Cost b-flow

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 2.804 (m1une), sum 75.933 (m1une)。

- `main.cxx` max: 2.521 ms (-10.12%), sum: 69.663 ms (-8.26%)
- `naive.cxx` max: 24.650 ms (+778.96%), sum: 360.364 ms (+374.58%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解用网络单纯形。人工根和惩罚边构造初始可行基树，负检验数边引入基本环，推流后用饱和边换基。人工流全部归零才表示原问题可行。

库：[network_simplex.h](../../../include/toy/network_simplex.h), [min_cost_flow.h](../../../include/toy/min_cost_flow.h)。

## 实现与取舍

`naive.cxx` 保留 Dinic 可行流加 epsilon 费用缩放。首个始终用 Bland 选边的版本在 anti_ssp 上出现极慢的微小改进，未完成的远端批次已作废。主解改用分块选最负检验数，连续退化换基时才启用 Bland。

## 正确性

小网络枚举上下界内所有整数流，核对可行性、最优费用和对偶势；正式数据包含反 SSP 构造。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。

采样显示递归求势占较大比例。n≤128 时用 u128 子树集合维护换基切下的分量，一次平移其全部对偶势；更大规模保留路径缓存。
