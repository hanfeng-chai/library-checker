# 队列整体函数复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 18.852 (nandhagk), sum 198.047 (nandhagk)。

- `main.cxx` max: 16.802 ms (-10.87%), sum: 178.390 ms (-9.93%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
仿射函数的复合满足结合律，但不交换。主解使用 [FoldQueue](../../../include/toy/fold.md)：
队首半段保存后缀复合，队尾半段保存原函数和一个总复合；队首半段耗尽时，
一次反向扫描把队尾原位变成后缀复合。每个元素至多被转换一次。

更新均摊 O(1)，整体复合 O(1)，缓冲只存每个位置的一份函数。
系数与复合方向见 [affine](../../../include/toy/affine.md)。
GCC/Clang 官方、非交换朴素对照及 sanitizer 通过。

## 尝试过程与取舍

经典双栈复合队列采用原位转换：队首半段耗尽时，把队尾原函数反向改写成后缀复合。每个元素只转换一次，不重复保存两份聚合；空队列使用单位元。首批完整比较双项领先。
