# Aho–Corasick

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 257.052 (adamant), sum 7851.700 (adamant)。

- `main.cxx` max: 197.975 ms (-22.98%), sum: 3784.081 ms (-51.81%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
按输入顺序插入 Trie，再 BFS 构造后缀链接。节点的父亲和编号在插入时确定；
重复字符串共享终点。转移采用 mask + 紧凑孩子数组，避免为百万节点分别初始化
26 个转移。BFS 只访问真实边，缺失边沿后缀链接寻找。

节点编号可能达到 1000000，使用通用 Writer，不采用六位数输出特化。

库见 [aho_corasick.md](../../../include/toy/aho_corasick.md) 和
[alphabet_trie.md](../../../include/toy/alphabet_trie.md)。
