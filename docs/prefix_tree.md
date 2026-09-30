# prefix_tree

`PrefixTree32(n)` 建立零权值数组，也可消费 Buffer<u32> 线性建树。
add(i,delta) 修改权值；prefix(end) 返回 [0,end) 的和；select(k) 返回
第 k 个单位权重所在的下标及其在该权值内的偏移。要求权值非负、总和小于 2^31，
select 的 k 小于 total，查询端点不超过数组长度。

32 叉节点存排他的孩子前缀；更新用四个 AVX2 向量，选秩用并行比较与位掩码。
零哨兵统一处理 prefix(n) 的边界。操作 O(log_32 n)，空间 O(n)。
有序集合的字计数与静态区间去重共同使用该内核。
