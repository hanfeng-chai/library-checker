# Number of Substrings

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 41.275 (Rohan_Kapri), sum 440.415 (Rohan_Kapri)。

- `main.cxx` max: 38.090 ms (-7.72%), sum: 423.319 ms (-3.88%)
- `naive.cxx` max: 114.984 ms (+178.58%), sum: 1110.985 ms (+152.26%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 `n*(n+1)/2 - sum(LCP)`。先求后缀数组，再将排序转换为后继
下标，直接用 Kasai 累加 LCP，不分配完整 rank/LCP 数组。全相同字符串的答案
直接是 n；其余输入仍经过精确后缀排序。

`naive.cxx` 保留后缀自动机：每次扩展新增
`len[current]-len[link[current]]` 个子串。稀疏转移节省空间，但克隆、转移修改
和非连续访问使其在本题大随机数据上慢于后缀数组。

库见 [suffix_array.md](../../../include/toy/suffix_array.md) 和
[suffix_automaton.md](../../../include/toy/suffix_automaton.md)。
