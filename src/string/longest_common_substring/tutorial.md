# Longest Common Substring

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 31.667 (nandhagk), sum 310.464 (nandhagk)。

- `main.cxx` max: 29.800 ms (-5.90%), sum: 269.033 ms (-13.35%)
- `suffix_array.cxx` max: 180.172 ms (+468.96%), sum: 1868.573 ms (+501.86%)
- `suffix_automaton.cxx` max: 265.669 ms (+738.95%), sum: 2148.807 ms (+592.13%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 先尝试精确的短串键筛选：把 q 个字符无损压入整数，字典保存所有
出现位置，匹配后双向延伸。答案变长时扩大采样间隔，但保证任何更长的公共
子串都含有一个采样键。哈希只选桶，比较完整键；工作量超过线性预算就回退
后缀自动机。完整包含、字母集合不交和单字母串另有直接可验证的路径。

保留两种经典对照：`suffix_array.cxx` 将两串用分隔符连接，求 SA/LCP 后扫描
来源不同的相邻后缀；`suffix_automaton.cxx` 为较短串建立 SAM，再扫描另一串。
两者都使用最新输入输出模板。筛选加速来自省去无需建立的完整索引，不改变
最长公共子串的精确定义。接口见
[common_substring.md](../../../include/toy/common_substring.md)。
