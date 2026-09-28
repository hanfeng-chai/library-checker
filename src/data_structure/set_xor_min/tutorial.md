# Set XOR-Min / 集合异或最小值

## English

### Fast solution: `chunked_mask_trie.cpp`

A pointer-based binary trie performs 30 dependent node accesses per query.
The fast implementation groups the 30 bits as `3 + 6 + 6 + 6 + 9`.

- The first four levels store one nonempty-child bit mask per prefix.
- A bottom 9-bit group initially stores its only value directly inside the
  group index.
- Only when a second value enters the same group do we allocate a leaf of
  eight 64-bit words.
- Empty/nonempty transitions propagate one bit through the upper masks.

For a query `x`, each mask chooses the child whose six-bit index minimizes XOR
with the corresponding chunk of `x`. This is done bit by bit inside a register,
without following pointers. The bottom leaf first chooses one of its eight
words and then one bit in that word.

### Correctness

Integer comparison is decided by the highest differing result bit. At every
chunk, all values in the child producing the smallest XOR chunk are better
than every value in a child producing a larger chunk, regardless of lower
bits. Choosing the minimum available chunk is therefore the usual binary-trie
greedy choice, six bits at a time.

The upper masks contain a child exactly when its subtree is nonempty. A direct
bottom entry or a leaf bit contains a value exactly when that value is in the
set. Thus the greedy walk never selects an erased value and reconstructs the
globally minimum XOR.

The hierarchy has a fixed five-stage depth, so all operations are constant
time for the 30-bit problem universe. Memory is the fixed mask/index tables
plus leaves allocated only for colliding 9-bit groups.

### Other implementations and performance

- `patricia_trie.cpp`: a path-compressed binary trie. It uses memory
  proportional to the number of set values and is a useful general sparse
  template.
- `binary_trie.cpp`: the conventional counted 30-level trie and the clearest
  proof-oriented implementation.

The chunked trie reduced the official-test wall time from roughly `1.37 s`
for Patricia to `0.62 s`. Its aggregate large-case local time is about
`1.07x` public `#332979`, with most individual cases at the same scheduler
quantum.

## 中文

### 高性能解：`chunked_mask_trie.cpp`

普通指针二进制 Trie 的一次查询需要 30 次相互依赖的节点访问。高性能版本把
30 位拆成 `3 + 6 + 6 + 6 + 9`：

- 前四层为每个前缀保存“哪些孩子非空”的位掩码；
- 底部 9 位分组只有一个值时，直接把该值内联在分组索引中；
- 同组出现第二个值时，才分配由八个 64 位机器字组成的叶子；
- 子树从空变非空或反向变化时，才向上修改对应掩码位。

查询 `x` 时，每层从掩码中选择与 `x` 当前 6 位异或后最小的非空孩子。选择
过程完全在寄存器位运算中完成，不追逐指针。叶子先选择八个机器字之一，再选
机器字内的一位。

### 正确性

整数大小由最高个不同结果位决定。在任一分块上，产生最小异或块的孩子中的
所有值，都优于产生更大异或块的孩子，与更低位无关。因此逐块贪心与逐位
二进制 Trie 贪心完全等价。

上层掩码中的一位当且仅当对应子树非空；底层内联值或叶子位当且仅当该值仍在
集合中。所以查询不会走入空子树，也不会选中已删除值，最终得到全局最小异或。

层数固定为五，故在本题 30 位值域上各操作都是常数时间。空间由固定掩码、
分组索引以及发生 9 位后缀冲突时才分配的叶子组成。

### 其他实现与性能

- `patricia_trie.cpp`：路径压缩 Trie，空间与集合元素数成正比，是通用稀疏模板；
- `binary_trie.cpp`：传统 30 层计数 Trie，逻辑与证明最直观。

分块掩码 Trie 把官方测试总墙钟从 Patricia 的约 `1.37 s` 降至 `0.62 s`；
大型本地用例总时间约为公开 `#332979` 的 `1.07x`，多数单用例处于同一调度
时间片。
