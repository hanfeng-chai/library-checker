# 最近公共祖先

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 56.598 (chaihf), sum 875.490 (2qbingxuan)。

- `main.cxx` max: 49.932 ms (-11.78%), sum: 857.334 ms (-2.07%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
题目保证 parent[v]<v。`main.cxx` 使用 ordered_lca.h 的 Schieber–Vishkin 标号，
按叶序选择 lowbit 最大的链标号，以位集保存祖先链。查询跳到共同链后，较小
编号即最近公共祖先；预处理、空间 O(N)，单次查询 O(1)。

主解先读入查询，提前八次查询预取两个端点的标号，将答案写回第一端点数组，
最后批量输出。该版本比八路 SIMD 查询版本的总时间更好，且保留较简单的标号逻辑。
25 个官方测例通过 GCC/Clang；链、星形、二叉树及随机父数组与父链朴素 LCA
对照，ASan/UBSan 通过。

## 尝试过程与取舍

利用本题父亲编号小于孩子的条件，采用 Schieber–Vishkin 标签。随后比较 SIMD 批量查询和提前预取；SIMD 候选虽然 max 更低，sum 却未达标，因此保留标量标签查询，主解预读端点并提前八项预取，最后批量输出。
