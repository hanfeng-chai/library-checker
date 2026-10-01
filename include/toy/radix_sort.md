# radix_sort

`radix_sort<Bits,Digit>(values,key)` 稳定排序平凡可复制记录，默认 Bits=32、Digit=8。
key 返回小于 2^Bits 的无符号整数，1≤Bits≤64、1≤Digit≤16，记录数不超过 u32 上限。

LSD 按低位到高位分配到桶，在两块缓冲间交换；所有键在某一位相同时跳过该轮。
小于 64 项时使用稳定插入排序。时间 O(ceil(Bits/Digit)*(n+2^Digit))，额外空间 O(n+2^Digit)。
GCC/Clang 的全 u64、重复、常数、逆序、空数组对照及 ASan/UBSan 通过。
