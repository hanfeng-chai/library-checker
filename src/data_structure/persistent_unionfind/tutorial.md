# 可持久化并查集

已知所有操作，先按引用的基础版本建立版本树。进入合并节点时执行按大小合并，
离开子树时撤销；查询节点只读取当前连通性，并把答案放回原编号位置。
这样每条边只进入、离开一次，操作 O(log n)，不必复制整棵父数组或路径树。

主解复用 [VersionTree](../../../docs/version_tree.md) 的显式栈遍历和
[RollbackDSU](../../../docs/rollback_dsu.md)，避免深递归与路径压缩的撤销成本。
GCC/Clang 官方、独立对照和 sanitizer 通过。Lenovo 单次 max/sum 为
23.578/215.501 ms，对照最小值为 30.354/273.535 ms。
证据：`bench/ds-last-basic-20260930/round1/selected/`。旧 .cpp 实现保留作参考。
