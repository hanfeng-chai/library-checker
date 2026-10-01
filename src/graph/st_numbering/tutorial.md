# st-Numbering

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 80.137 (Anonymous), sum 693.595 (nandhagk)。

- `main.cxx` max: 62.105 ms (-22.50%), sum: 543.334 ms (-21.66%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
从 s、t 开始 DFS，low-link 的符号决定把一个点插到父点前面还是后面。双向链表维护当前顺序；最终验证 s 最前、t 最后，以及每个中间点都有前后邻居。

库：[st_numbering.h](../../../include/toy/st_numbering.h)。

## 实现与取舍

低链接遍历用显式栈。连通性、s=t 和不满足编号条件的情况均由实际图检查。

## 正确性

小图枚举所有端点固定的排列，比较存在性，再检查输出排名。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
