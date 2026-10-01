# Palindromes in Deque

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 119.616 (adamant), sum 1988.843 (adamant)。

- `main.cxx` max: 48.695 ms (-59.29%), sum: 621.905 ms (-68.73%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
双端回文树采用 surface 表示：字符串两端的最长回文，以及没有被覆盖的内部
回文端点，足以支持前后插入和删除。quick link 跳过不能扩展的后缀段；节点
保留供之后复用，surface 计数和活跃 suffix 子节点数决定它是否仍出现在串中。
每次操作直接得到不同回文数量、最长回文前缀及后缀，不重新扫描当前串。

转移仍共用稀疏 `AlphabetTrie`。库见
[palindrome_deque.md](../../../include/toy/palindrome_deque.md)。
