# 双端队列整体函数复合

主解使用 [FoldDeque](../../../docs/fold.md)：左右两半分别维护朝外端的复合，
某一半耗尽时将剩余序列均分并重建。插入、删除均摊 O(1)，整体复合 O(1)。
空队列对应恒等函数，复合方向见 [affine](../../../docs/affine.md)。

系数实际不超过九位；主解使用较宽的 16 位读取提示来选择已有 SIMD 解码路径，
在本题上比八位块加标量尾部更快，默认 Writer 保持原样。
GCC/Clang 官方测例和非交换朴素对照、sanitizer 通过。
Lenovo 单次 max/sum 为 20.246/200.383 ms，对照最小值为 20.395/207.220 ms。
证据：`bench/ds-candidates-20260930/round2/selected-deque/`。旧 .cpp 保留作参考。
