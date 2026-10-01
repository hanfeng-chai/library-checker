# 点加、区间求和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 55.027 (IceKylin), sum 422.810 (IceKylin)。

- `main.cxx` max: 44.235 ms (-19.61%), sum: 340.597 ms (-19.44%)
- `fenwick_tree.cxx` max: 51.900 ms (-5.68%), sum: 406.901 ms (-3.76%)
- `segment_tree.cxx` max: 136.413 ms (+147.90%), sum: 1042.771 ms (+146.63%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
主解使用 [WideFenwick](../../../include/toy/fenwick.md)：十六叉节点保存孩子前缀和，
修改用 AVX2 同时更新后续前缀，查询每层读取两端的前缀。构造 O(N)，
操作 O(log N)，空间 O(N)。输入位数按题目范围特化，复用 toy/io.h。

保留两份经典对照解，均可用 make build/check/bench 的同名目标调用：

- `fenwick_tree.cxx`：普通二叉 Fenwick，单点加、两个前缀相减。
- `segment_tree.cxx`：普通二叉线段树，逐层更新祖先，查询合并左右区间。

## 尝试与取舍

首版 WideFenwick 已双项超过五份参考。树类题目复用它时，又观察到区间两端
进入相同高位前缀后，后续读取会相互抵消。因此让查询在两端合流时停止；
同批对照显示 max、sum 都进一步下降。配套的 add_difference 用于差分数组上
两个相反的点更新，在共同祖先处同样可以提前结束。

普通 Fenwick 结构最小、更新简单，也通过了本轮五份参考的门槛；宽节点主解
查询层数更少，仍更快。二叉线段树保留作为经典通用结构对照，其 max、sum
明显较高，不用它替换主解。
