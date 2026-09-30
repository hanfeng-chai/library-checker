# hash

`HashMap<Key,Value,Empty>(capacity)` 是不带删除的开放寻址表；capacity 是
最多插入的不同键数，表长为至少 2*capacity+1 的二次幂。
Key 为不超过 64 位的无符号整数，Empty 默认全一，不能作为实际键使用。
Value 为平凡可复制类型，表项通过 Buffer 分配。

`get(key)` 在缺失时返回 Value{}；`find(key)` 返回值指针或 nullptr；
`operator[]` 在缺失时插入零初始化值，并返回引用。
乘法混合后线性探测，期望 O(1)，最坏 O(capacity)，空间 O(capacity)。
