# Ordered Set / 有序集合

## English

### Fast solution: `wide_bit_block_tree.cpp`

All values occurring in the initial set or a query are read first and
coordinate-compressed. The radix sort is stable and numeric order is preserved,
so ranks in the compressed universe are also ranks in the original universe.

The compressed universe is split into 64-value words.

1. A bit in a word tells whether that value is present.
2. The popcount of every word is stored in a 32-ary prefix tree.
3. Every tree node stores the exclusive prefix sums of its 32 children.
4. Insertion or deletion changes all lanes after one child. Four AVX2
   load/add/store operations update those 32 lanes together.

The operations then reduce to:

- `rank(x)`: sum complete words through the wide tree, then popcount the part
  of the last word below `x`;
- k-th: descend the 32-ary tree by comparing all 32 prefix sums in SIMD, then
  use BMI2 `PDEP` to select the requested set bit inside the final word;
- predecessor/successor: convert the boundary into a rank and reuse k-th;
- insertion/deletion: change one bit and one root-to-leaf wide-tree path.

The initial set is built in bulk. This matters: inserting all initial elements
one by one would perform hundreds of thousands of unnecessary tree updates.

### Correctness

Compression is order-preserving. A set bit therefore represents exactly one
present original value, and every wide-tree count equals the popcount sum of
its covered words.

Consequently `rank(x)` is exactly the number of present values below `x`.
During k-th descent, the stored child prefixes identify the unique child whose
rank interval contains `k`; subtracting the preceding prefix gives the rank
inside that child. Induction over the tree levels reaches the correct word.
`PDEP` maps the remaining rank to the corresponding set bit, so k-th,
predecessor, and successor all return the required value.

With `M <= N+Q` compressed values, radix compression is linear in `M` for the
fixed 30-bit keys. Each operation costs `O(log_32 M)` wide-tree levels plus
constant word work, and memory is `O(M)`.

### Other implementations and performance

- `coordinate_fenwick.cpp`: the conventional offline solution. It is compact
  and easy to adapt, with `O(log M)` operations.
- `policy_based_tree.cpp`: GNU PBDS, fully online and concise, also
  `O(log N)`, but with pointer-heavy red-black-tree constants.

On the repository's pinned native benchmark, the wide solution's aggregate
large-case wall time is about `1.015x` public submission `#254992`; individual
runs are often inside the machine's scheduler quantum. The Fenwick and PBDS
versions are retained for clarity and online use, not as performance claims.

## 中文

### 高性能解：`wide_bit_block_tree.cpp`

先读入初始集合与全部查询，把所有出现过的值做坐标压缩。稳定 radix sort 保持
数值顺序，因此压缩坐标的排名就是原数值的排名。

把压缩值域按每 64 个值分成一个机器字：

1. 机器字中的一位表示该值是否存在；
2. 每个机器字的 popcount 放入 32 叉前缀树；
3. 每个树节点保存 32 个孩子的排他前缀和；
4. 插入或删除只会让某个孩子之后的前缀和统一加一或减一，使用四组 AVX2
   向量加载、加法和写回即可同时更新 32 个槽。

各操作可统一为：

- `rank(x)`：宽树统计完整机器字，再对最后一个机器字的低位做 popcount；
- 第 k 小：SIMD 同时比较节点内 32 个前缀和，逐层下降；最后用 BMI2 `PDEP`
  选出机器字内第 k 个置位；
- 前驱、后继：先把边界换成排名，再复用第 k 小；
- 插入、删除：修改一位以及一条宽树路径。

初始集合采用批量建树，而不是逐个插入；后者会产生大量没有必要的树更新。

### 正确性

坐标压缩保持顺序，每个置位恰好对应一个当前存在的原值；宽树中的计数也始终
等于覆盖机器字的 popcount 之和。因此 `rank(x)` 正是小于 `x` 的元素个数。

求第 k 小时，节点内的前缀和唯一确定 k 所属的孩子；减去之前孩子的总数后，
得到孩子内部的排名。逐层归纳可知最终到达正确机器字，`PDEP` 再把剩余排名
映射到正确置位。前驱和后继只是选取特定排名，故同样正确。

设压缩后值数为 `M <= N+Q`。30 位键的 radix 压缩为线性时间；每次操作经过
`O(log_32 M)` 层并做常数次字操作，空间 `O(M)`。

### 其他实现与性能

- `coordinate_fenwick.cpp`：经典离线 Fenwick 解，每次 `O(log M)`，最容易理解；
- `policy_based_tree.cpp`：GNU PBDS 在线红黑树，每次 `O(log N)`，代码短但指针
  与节点分配常数较大。

固定 CPU 的 native 基准中，宽树在大型用例上的总墙钟约为公开提交 `#254992`
的 `1.015x`，很多单用例差异小于本机调度时间片。另两版用于教学和在线模板，
不要求达到相同性能。
