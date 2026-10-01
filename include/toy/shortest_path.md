# 非负权最短路

`shortest_path<Radix,Bounded>(g,s,t)` 返回距离和顶点路径；不可达距离为 `~0ull`。默认使用完整 u64 radix heap，`Bounded=true` 要求边权小于 2^31；`Radix=false` 使用二叉堆。已发现的目标距离作为上界，不向队列加入目标和无关汇点。

`single_predecessor_path` 检查入度至多一的情况，此时逆向路径唯一，可直接回溯；nullopt 表示应使用一般算法。出度至多一的图也有直接游走路径。完整 radix heap 的最坏界可写成 O((n+m) log U)，U 为距离范围；二叉堆为 O((n+m) log m)。
