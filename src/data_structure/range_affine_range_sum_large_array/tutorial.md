# Range Affine Range Sum (Large Array) / 稀疏大数组区间仿射求和

## Compression / 压缩

All `N <= 10^9` values start at zero. A range update can change values only
across one of its two endpoints; queries introduce useful boundaries for
aggregation but do not change this fact. Collecting every operation's `l,r`
partitions the original array into `O(Q)` elementary runs on which all values
are always equal.

所有初值为零。区间修改只可能在两个端点处改变分段；查询端点虽不改变值，也适合
作为聚合边界。收集全部操作的 `l,r` 后，原数组被划成 `O(Q)` 个基本连续段，
每段内部的值始终相同。

## Length-aware lazy tree / 带真实长度的懒线段树

`compressed_lazy_segment_tree.cpp` stores each compressed leaf's physical
length, not merely one compressed
index. A node keeps total physical length `L`, modular sum `S`, and a lazy
affine function. Applying `x -> ax+b` changes it to `aS+bL`. Lazy functions
compose in chronological order.

压缩叶子保存原数组中的真实长度，而不是简单记作 1。节点维护真实总长 `L`、
模和 `S` 与懒仿射标记；执行 `x -> ax+b` 后变为 `aS+bL`，标记按时间顺序复合。

## Correctness / 正确性

Compression loses no boundary at which values may differ. For every node,
`sum` equals the current sum over its physical interval and its tag is exactly
the transformation not yet sent to children. The affine sum formula preserves
this invariant on full cover; push and pull preserve it on partial cover.
Query nodes partition the requested original interval, so their sums add to
the answer.

压缩没有遗漏任何可能出现值变化的边界。节点 `sum` 等于对应原区间当前和，标记
恰好是尚未传给孩子的变换。整段公式、下推和上拉都保持不变量；查询节点又恰好
划分目标原区间，因此求和正确。

## Complexity, edge cases, and performance / 复杂度、边界与性能

Sorting costs `O(Q log Q)`, construction `O(Q)`, each operation `O(log Q)`,
and memory `O(Q)`, independent of `N`. Zero-length gaps disappear after
deduplication; endpoints `0` and `N` are inserted explicitly. The implementation
uses unsigned direct shape-aware input and padded modular output.

排序 `O(Q log Q)`、建树 `O(Q)`、每次操作 `O(log Q)`、空间 `O(Q)`，与
`N` 无关。去重会消除零长度段，并显式加入 `0,N`。实现使用无符号 direct
shape-aware 输入与 padded 模数输出。

Against fastest usable public C++ baseline `#302877`, the summed official-case
median is `0.936x`; representative large random data is `1.000x`, with the
aggregate gain coming from other distribution shapes. The global OJ leader is
Rust, so cross-language milliseconds are reported separately rather than
treated as a same-toolchain baseline. / 相对最快可用公开 C++ `#302877`，全套
总中位数为 `0.936x`，典型大随机用例为 `1.000x`；OJ 总榜第一为 Rust，
不把跨语言毫秒数冒充同工具链基线。
