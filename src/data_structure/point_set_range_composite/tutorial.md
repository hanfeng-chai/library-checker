# Point Set Range Composite / 单点修改、区间函数复合

## Idea / 思路

**English.** Represent `f(x)=ax+b` by `(a,b)`. If `first` is followed by
`second`, then

`second(first(x)) = (second.a * first.a)x + second.a * first.b + second.b`.

Thus affine functions form a monoid under ordered composition; `(1,0)` is the
identity. A segment-tree node stores the composition of its interval from left
to right. A point update rebuilds its ancestors, and a range query combines
the usual left and right accumulators without reversing their order.

**中文。** 用二元组 `(a,b)` 表示 `f(x)=ax+b`。若先执行 `first`，再执行
`second`，则

`second(first(x)) = (second.a * first.a)x + second.a * first.b + second.b`。

所以仿射函数在“按顺序复合”下构成幺半群，单位元为 `(1,0)`。线段树每个
节点保存对应区间从左到右的函数复合。单点修改后重算祖先；区间查询使用
左右两个累积量，并严格保持顺序（函数复合不满足交换律）。

## Correctness / 正确性

**English.** Every leaf stores exactly its current function. By induction,
every internal node stores the left interval followed by the right interval,
so it represents precisely all functions in its interval. The segment-tree
fold partitions `[l,r)` into ordered disjoint nodes and composes them in that
order. Its result is therefore `f[r-1](...(f[l](x)))`.

**中文。** 每个叶子保存当前位置的当前函数。归纳可知，每个内部节点先复合
左区间、再复合右区间，故恰好代表整个节点区间。查询把 `[l,r)` 分解成若干
有序且不交的节点，并按原顺序复合，因此最终结果正是
`f[r-1](...(f[l](x)))`。

## Complexity / 复杂度

- Build / 建树: `O(N)`
- Update / 修改: `O(log N)`
- Query / 查询: `O(log N)`
- Memory / 空间: `O(N)`

## Implementations / 实现

- `direct_evaluation_segment_tree.cpp` is the performance version. Its leaves
  are reversed in a compact `2N` tree. A query applies each canonical node
  directly to `x`, avoiding the second modular multiplication needed to first
  materialize one combined affine function. / 性能版使用反向叶序的紧凑 `2N`
  线段树；查询直接把每个规范节点作用到 `x`，避免先合成总函数时额外的一次模乘。
- `recursive_segment_tree.cpp` is the readable recursive implementation. It
  keeps exactly the same `O(log N)` guarantee and maximum-data reliability.
  / 递归版显式展示“不交、全含、部分相交”三种分支，复杂度不变，也能通过最大数据。

## Engineering and performance / 工程与性能

All fields are nonnegative. The solutions therefore use direct regular-file
mapping, unsigned value-uniform six-digit index parsing, nine-digit coefficient
parsing, and padded `u32` output. Signed parsing and generic `u64` formatting
made the same tree about 30% slower on the largest cases. / 所有字段均非负，
因此采用 direct mmap、六位无符号均匀下标、九位系数解析和 padded `u32` 输出；
误用有符号解析与通用 `u64` 输出曾使同一棵树在最大数据上慢约 30%。

Against accepted submission `#278231`, compiled with the same native flags,
the performance version's summed official-case median is `1.000x`; representative
maximum random cases are also within about `0.1%`. This is measurement parity,
not a claim based on cross-machine OJ milliseconds. / 与公开 AC `#278231`
同机同参数交错测量，总官方用例中位数比为 `1.000x`，典型最大随机用例差约
`0.1%`，可视为持平。
