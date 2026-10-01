# ordered_set

`OrderedSet(universe,initial)` 保存 [0,universe) 的整数集合，支持 insert/erase、
contains、size、rank(end)（小于 end 的元素数）和从零开始的 kth(rank)。
rank 允许 end=universe；kth 要求 rank<size，universe<2^31。

每 64 个键放在一个机器字，字的元素数用 32 叉前缀树维护。
更新以 AVX2 修改每层中后续孩子的前缀；选秩时并行比较 32 个前缀，
最后用 BMI2 PDEP 选出字内的第 k 个置位。初始集合批量建树。
操作 O(log_32(universe/64)+1)，存储 O(universe/64)。

next(x)/previous(x) 直接查询成员字和非空字的分层位图；不存在时返回 -1。
前驱允许超出右端点，后继要求非负；这样无需经过秩与选秩的两次遍历。
