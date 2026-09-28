# Static Range Mode Query / 静态区间众数

## English

### Fast solution: `block_table.cpp`

Compress values and store all occurrence positions of each value in one
contiguous group. For every array position, also store its index inside that
occurrence group.

Choose block width near `sqrt(N)`. Precompute the mode and its frequency for
every interval consisting of complete blocks. To answer `[l,r)`:

1. take the precomputed mode of the complete middle blocks;
2. inspect every value occurring in the at-most-two boundary fragments;
3. use that value's occurrence-position group to test whether the occurrence
   `current_frequency` steps away still lies inside `[l,r)`;
4. extend while it does, updating the candidate.

If the current frequency reaches at least half the interval length, no other
value can exceed it, so the scan may stop early.

The block width is shape-aware. For at most 64 distinct values, use
`bit_ceil(sqrt(N))` to reduce preprocessing blocks; otherwise use
`bit_floor(sqrt(N))` to reduce query boundary work.

### Correctness

Let `m` be the precomputed mode of the complete middle. If the true whole-range
mode is `m`, extending its occurrence count through the boundaries finds its
exact frequency.

Otherwise, a value beating `m` must gain at least one occurrence from a
boundary fragment, so that value is explicitly inspected there. The grouped
position tests count all its occurrences in `[l,r)` without scanning the
middle. Thus every possible winner is considered and the returned
value-frequency pair is a valid mode.

For block width `B`, preprocessing is `O(N^2/B)` and each query is `O(B)`;
with `B ~= sqrt(N)`, both become the standard square-root bounds. Memory is
`O(N + (N/B)^2)`.

### Alternative and performance

`mo_algorithm.cpp` moves a current range and maintains value frequencies plus
frequency buckets. Add/remove and mode retrieval are `O(1)`, with about
`O((N+Q)sqrt(N))` movements.

After adaptive block sizing and majority early exit, `perf` reports about
`1.02x` public `#211820` on the small-alphabet random case and `1.14x` on the
most expensive binary case; wall times are usually identical or one scheduler
quantum apart.

## 中文

### 高性能解：`block_table.cpp`

先压缩值域，把每个值的全部出现位置连续存放；同时记录每个数组位置在所属出现
位置组中的下标。

取接近 `sqrt(N)` 的块宽，并预处理任意完整块区间的众数及频数。回答 `[l,r)`：

1. 先取中间完整块区间的预处理众数；
2. 枚举左右至多两个边界碎片中出现的值；
3. 在该值的出现位置组中，检查距离当前频数的位置是否仍落在 `[l,r)`；
4. 若仍在区间，就继续扩展并更新候选。

当前频数达到区间长度一半后，其他值不可能再超过它，可以立即停止。

块宽会适应值域形状：不同值不超过 64 时取 `bit_ceil(sqrt(N))`，减少预处理块数；
否则取 `bit_floor(sqrt(N))`，减少查询边界工作。

### 正确性

设中间完整块的预处理众数为 `m`。若整个区间的众数也是 `m`，边界扩展会求出
它在 `[l,r)` 中的精确频数。

若真正众数不是 `m`，它要超过中间众数，就必须至少在某个边界碎片出现一次，
因此必会被显式枚举。出现位置组能在不扫描中间的情况下验证该值全部区间频数。
所以所有可能胜出的值都被检查，返回的值与频数一定是合法众数。

块宽为 `B` 时，预处理 `O(N^2/B)`、每次查询 `O(B)`；取 `B ~= sqrt(N)` 得到
经典根号界。空间 `O(N+(N/B)^2)`。

### 替代实现与性能

`mo_algorithm.cpp` 移动当前区间，并同时维护值频数与“按频数分桶”；加入、删除、
读取众数均为 `O(1)`，总移动约 `O((N+Q)sqrt(N))`。

加入自适应块宽和过半早停后，`perf` 相对公开 `#211820`：小值域随机用例约
`1.02x`，最重二值用例约 `1.14x`；墙钟通常相同或只差一个调度时间片。
