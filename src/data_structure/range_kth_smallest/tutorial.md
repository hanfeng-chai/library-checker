# Range K-th Smallest / 区间第 k 小

## English

### Fast solution: `batched_wavelet_matrix.cpp`

Values are first radix-compressed. If there are `S` distinct values, only
`ceil(log2 S)` Wavelet-Matrix levels are required instead of all 30 input bits.

At every level:

1. one bit per current value is packed into 64-bit words;
2. each word stores the number of one-bits before it;
3. zero-bit values and one-bit values are stably partitioned;
4. bitmap construction and partition collection happen in the same scan.

All queries are stored in structure-of-arrays form. The outer loop is over
levels and the inner loop is over queries, so each bitmap row remains hot.
For `[l,r)`, two rank operations give the number of zeros. An all-zero or
all-one arithmetic mask updates `l`, `r`, `k`, and the answer bit without an
unpredictable branch.

### Correctness

At a level, every value with a zero current bit precedes every value with a
one current bit. If the zero count in the current interval exceeds `k`, the
requested rank lies in the zero partition. Otherwise it lies in the one
partition and its new rank is `k - zero_count`.

Stable partitioning preserves the relative order needed to map both endpoints
to the next level. Induction over all compressed-value bits proves that the
accumulated rank is the compressed k-th value; the inverse compression returns
the original value.

Construction is `O(N log S)`, all queries cost `O(Q log S)`, and memory is
`O(N log S / 64 + N + Q)` machine words.

### Alternative and performance

`merge_sort_tree.cpp` stores a sorted array at every segment-tree node and
binary-searches the value domain. It is independent and easy to generalize,
with `O(log^2 N)` query time and `O(N log N)` memory.

After fusing construction passes and batching queries branchlessly, the fast
version matches public `#376681` within benchmark noise on both dense and
random maximum cases. Its official-test wall time fell from about `1.04 s` to
`0.72 s`.

## 中文

### 高性能解：`batched_wavelet_matrix.cpp`

先用 radix sort 压缩值域。若不同值只有 `S` 个，只需
`ceil(log2 S)` 层 Wavelet Matrix，而不必固定处理输入的 30 位。

每一层执行：

1. 把当前位打包为 64 位机器字；
2. 每个机器字保存它之前的一位数量；
3. 对零位值和一位值做稳定划分；
4. 在同一次扫描中同时构造位图并收集两侧元素。

全部查询以多个连续数组保存。外层遍历层、内层遍历查询，使当前位图行保持在
缓存中。对 `[l,r)` 做两次 rank 即得零元素数，再用全零或全一掩码无分支地更新
`l,r,k` 和答案位。

### 正确性

在任一层，当前位为零的值全部排在当前位为一的值之前。若区间零元素数大于
`k`，目标必在零分组；否则目标在一分组，组内排名变为 `k-zero_count`。

稳定划分保留了映射到下一层所需的相对次序。逐层归纳可知最终累积的压缩排名
就是区间第 k 小，逆压缩后得到原值。

构建 `O(N log S)`，全部查询 `O(Q log S)`；空间为
`O(N log S / 64 + N + Q)` 个机器字。

### 替代实现与性能

`merge_sort_tree.cpp` 在每个线段树节点保存有序数组，并在值域上二分答案。
它是独立、易推广的 `O(log^2 N)` 查询方案，空间 `O(N log N)`。

合并构建扫描并改为无分支批处理后，主解在稠密值域和最大随机用例上均与公开
`#376681` 处于测量噪声内；官方测试墙钟从约 `1.04 s` 降至 `0.72 s`。
