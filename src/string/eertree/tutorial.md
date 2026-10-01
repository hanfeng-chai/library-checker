# Eertree

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 114.553 (adamant), sum 1690.619 (adamant)。

- `main.cxx` max: 68.226 ms (-40.44%), sum: 943.540 ms (-44.19%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
逐字符扩展回文树，每次至多新增一个回文节点，保存去掉首尾后的父亲及最长
真回文后缀。转移共用稀疏 `AlphabetTrie`，只为实际出现的字符分配空间。
内部 -1、0 长度根编号为 0、1，输出时统一减一。

与 AC 自动机一样，百万级节点编号使用通用 Writer。
库见 [eertree.md](../../../include/toy/eertree.md)。
