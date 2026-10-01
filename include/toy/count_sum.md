# count_sum

`CountSumTree<MaxValue>(n)` 支持 add(position,value)、prefix(end)、sum(l,r)，
返回 BoundedSum{sum,count}。每个位置最多插入一次，value≤MaxValue；默认支持完整 u32。

按 2^16 个位置分块，块内用十六叉前缀树把次数和总和编码在同一个 u64。
排他的局部前缀严格少于一个完整块，所以默认 48 位总和加 16 位计数也不会溢出。
完整块通过较小的普通 Fenwick 汇总，端点恰落在块界时只取粗层结果。

局部树固定四层，允许编译器展开循环；独立存储按 2 MiB 对齐，并向 Linux 建议
使用透明大页，以减少随机更新的地址转换开销。正确性不依赖建议是否生效。
`prefetch(position)` 可提前读取将要插入的位置。

构造 O(n)，修改/查询 O(log n)。已验证 65535/65536/65537 等块界、
完整 u32 最大值、随机插入次序及 GCC/Clang、ASan/UBSan。
