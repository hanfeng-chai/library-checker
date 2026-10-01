# Dominator Tree

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 56.651 (Rohan_Kapri), sum 338.579 (Rohan_Kapri)。

- `main.cxx` max: 51.703 ms (-8.73%), sum: 302.786 ms (-10.57%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
DFS 编号后，用 Lengauer–Tarjan 半支配树算法处理反向前驱。并查集路径压缩同时保留半支配者最小的标签，bucket 延迟确定直接支配者，最后修正间接引用。

库：[dominator.h](../../../include/toy/dominator.h)。

## 实现与取舍

DFS 和 eval 都使用显式栈，避免长链上的递归开销；结果保持原始顶点编号，不可达点输出 -1。

## 正确性

逐个删除顶点后重新检查根可达性，得到小图的支配关系，再验证直接支配者。正式数据均使用官方 checker 检查；相关随机对照同时经过 ASan/UBSan。
