# Point Set Range Composite (Large Array) / 稀疏大数组函数复合

## Observation / 观察

The conceptual array has up to `10^9` elements, all initially identity.
Only positions appearing in point updates can ever become non-identity.
Therefore a query `[l,r)` only needs the updated coordinates satisfying
`l <= p < r`; query boundaries themselves do not need segment-tree leaves.

概念数组长达 `10^9`，初始全为单位函数。只有在单点修改中出现的位置才可能
变成非单位元。因此查询 `[l,r)` 只需考虑满足 `l <= p < r` 的修改坐标，
查询端点本身不必建叶子。

## Offline radix compression / 离线基数压缩

`radix_compression_segment_tree.cpp` records one tagged event for an update
coordinate and two for a query. Stable two-pass radix sort orders all 30-bit
coordinates with 15 bits per pass. One scan assigns:

- each distinct update coordinate its leaf index;
- each query boundary the count of update coordinates strictly below it.

Thus a point update becomes one leaf assignment and `[l,r)` becomes a compressed
half-open range. Equal-coordinate event order is harmless: before the update
event the current count is the number strictly below `p`; after it, subtracting
the equality flag gives the same count.

实现为修改坐标记录一个带标签事件，为查询记录两个端点事件。30 位坐标用每轮
15 位的稳定两趟基数排序；一次扫描给不同修改坐标编号，并把查询端点映射为
“严格小于该端点的修改坐标数”。同坐标事件无论先后都得到相同边界：修改事件
之前计数尚未增加，之后则减去相等标记。

## Segment tree and correctness / 线段树与正确性

The affine tree stores only update coordinates in increasing order. Its
direct-evaluation query applies canonical node functions to `x` from left to
right, exactly matching original coordinate order. Identity gaps are omitted
without changing composition. If a range contains no update coordinate, `x`
is returned unchanged. / 仿射树只按升序保存修改坐标；查询从左到右把规范节点
直接作用于 `x`，与原坐标顺序一致。省略的间隙全是单位函数，不影响复合；若
区间内没有修改坐标，答案就是原 `x`。

## Complexity and performance / 复杂度与性能

Radix compression is `O(Q+2^15)`, each online operation is `O(log Q)`, and
memory is `O(Q)`. Input uses 10-digit unsigned positions, six-digit query
counts, and nine-digit coefficients through direct mapping; output is padded
`u32`.

基数压缩 `O(Q+2^15)`，每次操作 `O(log Q)`，空间 `O(Q)`。I/O 分别声明
十位坐标、六位操作数和九位系数，并使用 padded `u32` 输出。

The summed official-case median is `0.928x` accepted public baseline `#361521`;
the query-heavy maximum case is `1.001x`, while compression-heavy cases provide
the aggregate win. / 全套总中位数相对公开 AC `#361521` 为 `0.928x`；
查询密集最大用例为 `1.001x`，总体优势主要来自压缩阶段。
