# buckets

`Buckets<T>(count, items, key)` 消费 Buffer，按 [0,count) 内整数键稳定分组。
`buckets[i]` 返回该组只读 span。所有组共享连续 items 存储与一个 offset 数组。
稳定性保留组内输入顺序，适合把离线事件按对象分组而不破坏时间顺序。
构造与空间均为 O(count+items.size())，下标与元素数应适合 u32。
