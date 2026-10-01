# 双端队列整体函数复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 20.914 (nandhagk), sum 211.437 (nandhagk)。

- `main.cxx` max: 20.624 ms (-1.39%), sum: 203.567 ms (-3.72%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解使用 [FoldDeque](../../../include/toy/fold.md)：左右两半分别维护朝外端的复合，
某一半耗尽时将剩余序列均分并重建。插入、删除均摊 O(1)，整体复合 O(1)。
空队列对应恒等函数，复合方向见 [affine](../../../include/toy/affine.md)。

系数实际不超过九位；主解使用较宽的 16 位读取提示来选择已有 SIMD 解码路径，
在本题上比八位块加标量尾部更快，默认 Writer 保持原样。
GCC/Clang 官方测例和非交换朴素对照、sanitizer 通过。

## 尝试过程与取舍

先做左右两半的均摊复合队列，确认非交换方向与重建过程。随后比较逐项、批量操作与 SIMD 读取的不同组合。题目系数虽最多九位，较宽的 16 位提示会选中另一条 SIMD 解码路径，在本题上反而更快，因此不能仅凭位数上界推断性能。
