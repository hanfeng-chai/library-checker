# Range Set Range Composite / 区间赋函数、区间复合

## Idea / 思路

Every array element is an affine function. A node stores their left-to-right
composition. Assigning one function `f` to a node covering `2^k` positions
makes its aggregate `f^(2^k)`.

数组元素均为仿射函数，节点保存从左到右的有序复合。若把覆盖 `2^k` 个位置的
节点整体赋为 `f`，聚合值就是 `f^(2^k)`。

A direct implementation exponentiates `f` separately for every canonical node
and again during every push, becoming `O(log^2 N)`. The performance tree builds
one power chain per update:

`power[0]=f, power[k+1]=power[k]∘power[k]`.

All covered nodes store an offset into this immutable chain. A level-`k` node
reads `power[k]`; when pushed, both children reuse `power[k-1]`. Thus one update
computes each square only once.

直接实现会对每个覆盖节点、每次下推重复快速幂，退化为 `O(log^2 N)`。性能版
为每次赋值只建立一次不可变幂链
`power[0]=f, power[k+1]=power[k]∘power[k]`。节点保存链偏移；第 `k` 层读取
`power[k]`，下推时两个孩子复用 `power[k-1]`，每个平方只计算一次。

## Iterative propagation / 迭代传播

`power_chain_lazy_segment_tree.cpp` uses a power-of-two iterative tree. It first
pushes only the two boundary paths, assigns the canonical nodes while tracking
their levels, then rebuilds only non-fully-covered boundary ancestors. Queries
push their boundary paths and combine a left and a right accumulator so
noncommutative order is preserved.

实现采用二次幂容量的迭代树：先下推两条边界路径，带层号地赋值规范节点，再只
重建没有被整段覆盖的边界祖先。查询同样下推边界，并使用左右两个累积量，确保
不可交换的函数复合顺序不被颠倒。

## Correctness / 正确性

Leaves equal the current assigned functions. An internal aggregate is the left
child followed by the right child. A lazy chain offset means every leaf below
the node has the same base function, and `power[level]` is exactly its repeated
composition over that node length. Assignment, push, pull, and ordered query
combination all preserve these statements, proving the returned function is
precisely the requested interval composition.

叶子等于当前位置函数，内部节点等于左孩子后接右孩子。懒偏移表示整棵子树被赋
为同一基础函数，而 `power[level]` 正好是对应长度的重复复合。赋值、下推、
上拉和有序查询均保持这些不变量，因此查询函数准确无误。

## Complexity and performance / 复杂度与性能

Each update and query is `O(log N)`; memory is `O(N+U log N)` for `U` range
assignments. Immutable chains intentionally trade memory for eliminating
repeated modular exponentiation. Direct unsigned parsing and padded modular
output are used.

每次修改、查询 `O(log N)`；若赋值次数为 `U`，空间为 `O(N+U log N)`。这里
有意用内存换掉重复模幂，并使用无符号 direct 输入和 padded 模数输出。

Against accepted public baseline `#227239`, seven-run `perf stat` on
`random_01` measured 697.1M versus 687.6M cycles (`1.014x`, about 0.2--0.5%
run variation). This is a reproducibly close result after replacing the former
`O(log^2 N)` implementation, which was about 3.5x slower. / 相对公开 AC
`#227239`，`random_01` 七轮 cycles 为 697.1M 对 687.6M（`1.014x`，波动约
0.2--0.5%）。旧版约慢 3.5 倍；当前结果属于稳定接近。
