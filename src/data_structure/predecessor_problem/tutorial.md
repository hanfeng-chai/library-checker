# Predecessor Problem / 前驱后继集合

## 中文

### Fenwick 计数（`fenwick_order_statistics.cpp`）

每个位置存 0/1，Fenwick 前缀和给出某段内元素数量。第 `k` 个元素可
用 Fenwick 的二进制提升 `lower_bound` 找到，因此前驱/后继都可转化
为秩查询。每次操作 `O(log N)`，空间 `O(N)`，思路直观并能 AC。

### 层级 bitset（`hierarchical_bitset.cpp`）

第 0 层每一位表示一个键是否存在；第 1 层每一位表示第 0 层对应的
64-bit word 是否非空；继续向上，直到顶层只有一个 word。

找后继时先在当前 word 中屏蔽查询位置之前的位并用 `countr_zero`
取最低置位；若当前 word 为空，就上升到父层找下一个非空子块，命中
后逐层选择最低置位下降。前驱完全对称，使用 `countl_zero`。
插入/删除只在 word 从空变非空或反向变化时继续向上传播。

层数为 `O(log_64 N)`，本题最多四层；空间是
`N/8 * (1+1/64+...)` 字节。注意每层只能检查当前 word，不能线性扫
后续 words，否则全空数据会退化成 `O(N/64)`。

两份源码都使用 direct mapping、定长 `N` 位初始串、一位操作码、七位键和
相同普通 Writer。定长 token 避免对最多一千万字符的 bitstring 再扫描一次。

## English

`fenwick_order_statistics.cpp` stores 0/1 counts in a Fenwick tree and converts predecessor and
successor queries to rank selection in `O(log N)`.

`hierarchical_bitset.cpp` builds a hierarchy of 64-bit occupancy words. A failed local search
ascends one level; a hit descends by selecting the lowest/highest set bit.
Operations take `O(log_64 N)` and compact bitset memory.

Both use direct mapping, a fixed-length initial token, one-digit operations,
seven-digit keys, and the same Writer.
