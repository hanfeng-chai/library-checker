# Associative Array / 关联数组

## 中文

### 标准哈希表（`unordered_map.cpp`）

使用 `unordered_map`，但把默认整数哈希替换为 SplitMix64，并提前
`reserve(Q)`。SplitMix64 会充分混合相近、等差或二次幂键，避免题目
专门构造的数据让桶分布退化。查询不存在的键时按题意输出 0。

### 开放寻址（`open_addressing.cpp`）

表大小取不小于 `2Q` 的二次幂，负载率不超过 1/2。初始槽位为空；
从 `hash(key)&mask` 开始线性探测，遇到同键或空槽结束。题目没有删除，
所以不需要墓碑，查找逻辑尤其简单。

连续槽位提高缓存局部性，也避免每个节点单独分配；SplitMix64 防止
公开 killer 数据形成长簇。期望插入/查询 `O(1)`，最坏 `O(Q)`，空间
`O(Q)`。

两份源码都使用 direct mapping、一位操作码、十九位键值路径和相同普通
Writer。开放寻址版在既有同机测试中约比历史个人第一快 12%。

## English

`unordered_map.cpp` uses a reserved `unordered_map` with SplitMix64 to resist crafted
integer keys. `open_addressing.cpp` uses a power-of-two, at-most-half-full linear-probing
table. Since the problem has no deletion, an empty slot terminates lookup and
no tombstones are needed. Expected operation time is `O(1)`.

Both use direct mapping, one-digit operation codes, nineteen-digit keys and
values, and the same Writer. The open-addressing variant measured about 12%
faster than the previous personal leader on the same host.
