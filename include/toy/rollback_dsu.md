# rollback_dsu

`RollbackDSU(n)` 按大小合并，不做路径压缩。leader/same 为 O(log n)。
`merge(a,b)` 返回 Change，`undo(change)` 必须按逆序撤销，耗时 O(1)。
未发生合并时也返回可安全撤销的记录。父数组 O(n)，变更历史由调用方保存。
可配合 [version_tree](version_tree.md) 离线遍历各版本。
