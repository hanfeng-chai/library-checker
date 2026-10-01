# 关联数组

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 98.937 (IceKylin), sum 1066.258 (Rohan_Kapri)。

- `main.cxx` max: 74.903 ms (-24.29%), sum: 1026.445 ms (-3.73%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
使用 [HashMap](../../../include/toy/hash.md) 的开放寻址与线性探测；题目键值范围允许
将 u64 全一保留为空槽标记，预留的表长使负载率低于 1/2。
每批读取 128 个操作并预取初始桶，按原顺序执行、批量输出。
缺失键本来就映射到 0，因此零赋值只更新已有槽，避免创建无效条目。

GCC/Clang 官方与哈希对照、sanitizer 通过。

## 尝试过程与取舍

首版开放寻址表已避免标准容器的逐节点分配，后续把操作分成 128 项批次，预取初始桶并批量输出。缺失键的默认值本来为零，因此零赋值不必创建新槽；改动保持操作执行顺序，第二轮完整比较用于确定最终版本。
