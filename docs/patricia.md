# patricia

`PatriciaSet(capacity)` 是完整 u32 值域的无重复压缩二进制字典树。
支持 insert/erase/contains/size；min_xor(x) 要求非空，返回最小异或值。
内部节点只保留产生分叉的位，按从高到低的分叉位搜索；删除叶时旁路其父节点，
回收叶与父节点。空间 O(当前规模峰值)，操作至多经过 32 个分叉。

set_xor_min/patricia.cxx 提供同框架替代解。其空间随实际集合增长，随机访问
性能落后于已知值域的 xor_set.h，因此主解选择后者。GCC/Clang、朴素对照和
sanitizer 通过。
