# version_tree

`VersionTree(capacity)` 保存版本树，版本 0 为初始状态。
`add(base)` 创建下一个编号的子版本；base 必须已存在。
`visit(enter,leave)` 进入非零版本时调用 enter，子树结束后调用 leave。

显式栈保存进入与退出标记，每步只压入首个孩子及待访问的兄弟，避免预先追完整条兄弟链。
适用于带可撤销操作的离线可持久化；遍历 O(q) 时间和空间，无深递归栈要求。
兄弟访问次序不作保证，答案按原编号存放。

随机访问的 head/next 数组达到 1 MiB 时采用 2 MiB 对齐并提示 MADV_HUGEPAGE。
`version_storage<T>(n)` 为操作记录提供相同分配方式；连续栈和短队列缓冲仍用普通 Buffer。
大页对内核开销的影响见 [评测审计](measurement_audit.md)，默认 Buffer 分配策略不变。
