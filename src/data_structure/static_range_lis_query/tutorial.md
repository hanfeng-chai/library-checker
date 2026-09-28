# Static Range LIS Query / 静态区间 LIS 查询

## English

### Solution: `bumping_blocks.cpp`

The input is a permutation. Sweep the right endpoint from left to right and
maintain the semi-local patience-bumping state indexed by permutation value.
For a new pair `(value x, position r)`:

1. write `r` at slot `x`;
2. carry zero through slots with value greater than `x`;
3. whenever a slot contains a larger position than the carry, swap them;
4. the final carry is the one position evicted from the active set.

The active positions are maintained in a Fenwick tree: insert `r` and erase the
evicted position. A standard semi-local LIS invariant says that after processing
right endpoint `r`, the number of active positions in `[l,r]` equals the LIS
length of that subarray. Queries are bucketed by right endpoint and answered
with one Fenwick range sum.

A literal suffix bump is linear. Values are therefore split into square-root
blocks:

- the block containing `x` is materialized and scanned;
- every later block only replaces its current maximum if that maximum exceeds
  the carry;
- a max-heap stores each block maximum;
- a pending min-heap records lazy replacements until the block is materialized.

The heaps are specialized integer heaps with retained capacity. `replace_top`
and `push_pop` fuse the operations used by the bumping transition.

### Correctness

The unblocked transition is the patience-bumping recurrence for a permutation:
inserting the new endpoint creates one new critical position, and the carried
maximum is the unique critical position that ceases to be active. The
semi-local invariant therefore remains true by induction on the right endpoint.

The blocked implementation performs the same transition. Inside the first
block it executes swaps explicitly. In every complete later block, sequential
bumping changes only the largest stored position greater than the incoming
carry; replacing that maximum and forwarding it is equivalent. Lazy pending
heaps preserve the same multiset and materialization restores the exact slot
order. Thus both representations evict the same position.

With block width `B`, preprocessing costs
`O(N(B + N log B / B))`; `B ~= sqrt(N)` gives the intended square-root
bound. Query bucketing is linear and each answer costs `O(log N)` for Fenwick.
Memory is `O(N+Q)`.

### Performance note

This is the only retained maximum-constraint solution; a per-query patience
sort would be intentionally non-AC on worst cases. Locally it is faster than
public `#331206` on sorted and almost-sorted permutations, and about `1.23x`
on maximum random permutations after custom-heap optimization. The mixed
corpus total is close, with the adversarial issue case essentially equal.

## 中文

### 解法：`bumping_blocks.cpp`

输入是一个排列。按右端点从左到右扫描，并维护按排列值编号的半局部 patience
bumping 状态。加入新二元组 `(值 x, 位置 r)` 时：

1. 在槽 `x` 写入 `r`；
2. 令携带值为零，向所有更大的值槽传播；
3. 若当前槽位置大于携带值，就交换二者；
4. 最终携带出的那个位置是唯一被逐出活动集合的位置。

Fenwick 树维护活动位置：加入 `r`，删除被逐出位置。半局部 LIS 的经典不变量为：
处理完右端点 `r` 后，活动位置在 `[l,r]` 中的个数正好等于该子数组的 LIS 长度。
因此把查询按右端点分桶后，只需一次 Fenwick 区间和。

直接扫描整个值后缀是线性的，所以按根号大小分块：

- 显式展开并扫描包含 `x` 的块；
- 后续完整块只需在块最大位置大于携带值时替换该最大值；
- 每块用最大堆维护最大位置；
- 尚未展开的替换放入最小堆，直到该块被物化。

堆使用保留容量的专用整数实现，并用 `replace_top`、`push_pop` 合并 bumping 中的
常见复合操作。

### 正确性

未分块过程就是排列上的 patience-bumping 递推：新右端点产生一个新临界位置，
最终携带出的最大位置是唯一失效的临界位置。因此可按右端点归纳保持半局部不变量。

分块版本执行完全相同的变换。首块显式交换；对之后完整块，顺序 bumping 只会
替换块内大于携带值的最大位置，再把旧最大值继续传递。最大堆替换与之等价；
延迟最小堆保存同一多重集合，物化时恢复精确槽顺序。因此两种表示逐出的下标相同。

块宽为 `B` 时，预处理为 `O(N(B + N log B / B))`；取
`B ~= sqrt(N)` 得到目标根号复杂度。查询分桶线性，每个答案做 `O(log N)` 的
Fenwick 查询，空间 `O(N+Q)`。

### 性能说明

这里只保留能通过最大约束的解；逐查询 patience sorting 在最坏数据上必然超时。
本地相对公开 `#331206`：有序和近有序排列更快，最大随机排列约为 `1.23x`；
专用堆优化后混合语料已接近，对抗 issue 用例基本持平。
