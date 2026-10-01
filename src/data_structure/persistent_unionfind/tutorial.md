# 可持久化并查集

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 30.464 (Anonymous), sum 274.546 (Anonymous)。

- `main.cxx` max: 23.675 ms (-22.29%), sum: 216.364 ms (-21.19%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
已知所有操作，先按引用的基础版本建立版本树。进入合并节点时执行按大小合并，
离开子树时撤销；查询节点只读取当前连通性，并把答案放回原编号位置。
这样每条边只进入、离开一次，操作 O(log n)，不必复制整棵父数组或路径树。

主解复用 [VersionTree](../../../include/toy/version_tree.md) 的显式栈遍历和
[RollbackDSU](../../../include/toy/rollback_dsu.md)，避免深递归与路径压缩的撤销成本。
GCC/Clang 官方、独立对照和 sanitizer 通过。

## 尝试过程与取舍

初版比较在线持久化和离线版本树加回滚并查集。之后与可持久化队列一起测试压紧版本记录、预取、惰性布局、正反遍历与大页。最终每个版本只进出一次，回滚而不复制父数组；保留不同存储尝试的结果来说明主要收益来自布局与遍历。
