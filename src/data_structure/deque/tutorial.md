# 双端队列

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 13.967 (learningstud), sum 297.389 (learningstud)。

- `main.cxx` max: 13.543 ms (-3.04%), sum: 267.205 ms (-10.15%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
已知操作总数 Q，分配居中的连续缓冲，两端指针分别表示队首、队尾。
插入、删除、随机访问都只需移动指针或访问数组，单次 O(1)，空间 O(Q)。
实现见 [deque](../../../include/toy/deque.md)。旧 `.cpp` 的循环缓冲和标准容器方案保留作参考。

GCC/Clang 官方测例通过。

## 尝试过程与取舍

本题已知操作上限，使用居中的连续缓冲即可完成两端插入、删除和随机访问。省去了循环位置归约和动态扩容；首批完整对照达到目标，主解本身就是简洁的经典数组双端队列。
