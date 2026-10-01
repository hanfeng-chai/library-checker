# Prefix Substring LCS

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 101.251 (Today03), sum 527.991 (Today03)。

- `main.cxx` max: 51.482 ms (-49.15%), sum: 295.195 ms (-44.09%)
- `naive.cxx` max: 67.184 ms (-33.65%), sum: 389.358 ms (-26.26%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 用 seaweed 标签表示半局部 LCS，按 s 的前缀长度、t 的左边界离线
排序询问。每行维护标签小于左边界的位置位图，答案是右端点的位图前缀计数
减去左边界。最多 16 个 64 位块，两组 AVX2 指令更新全部块的前缀计数。
固定本题字符串长度上限后，时间 O(|s||t|+q)，空间 O(|t|+q)。

`naive.cxx` 保留经典在线版本：预先存下各 DP 行的 seaweed 标签，每个询问
使用 AVX2 统计区间内合适的标签。它无需排序询问，但查询仍与区间长度有关，
空间 O(|s||t|)。两者共用相同输入输出模板。

接口见 [substring_lcs.md](../../../include/toy/substring_lcs.md)。
