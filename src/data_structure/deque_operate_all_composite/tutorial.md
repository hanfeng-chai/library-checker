# Deque Operate All Composite / 双端队列整体函数复合

## Algorithm / 算法

**English.** Affine functions are an ordered monoid under composition. Maintain
two aggregate stacks. The left stack is stored from the middle toward the
front, so its aggregate runs from stack top to bottom. The right stack is
stored from the middle toward the back, so its aggregate runs from bottom to
top. Their ordered combination is exactly the whole deque.

If a pop is requested from an empty side, split the other stack approximately
in half and rebuild both directional aggregates. Keeping half on each side is
essential: moving everything could repeatedly cost linear time when pops
alternate between the two ends.

**中文。** 仿射函数在有序复合下构成幺半群。维护两个带聚合值的栈：左栈从
中间向队首存储，聚合方向为栈顶到栈底；右栈从中间向队尾存储，聚合方向为
栈底到栈顶。按顺序合并二者，恰好得到整个双端队列的函数复合。

若要从空的一侧弹出，就把另一栈近似均分并重建两种方向的聚合值。必须保留
一半元素在原侧；若每次全部搬走，交替从两端弹出会退化为平方复杂度。

## Correctness / 正确性

Each stack aggregate follows the logical deque order. Pushes extend the
corresponding aggregate in the correct direction. Rebuilding partitions the
same sequence without reordering it, then recomputes exact aggregates.
Therefore the combined aggregate always represents every deque function,
from front to back. / 两个栈的聚合方向都与逻辑队列顺序一致；插入按正确方向
扩展聚合，重建只重新分段而不改变元素次序。因此两侧聚合的组合始终准确代表
从队首到队尾的全部函数。

## Complexity / 复杂度

Each operation is amortized `O(1)` and memory is `O(Q)`. A rebuild costs
linear time but leaves half the elements on each side, so enough operations
must occur before the same side needs rebuilding again. / 每次操作摊还 `O(1)`，
空间 `O(Q)`；一次重建虽为线性，但均分后必须经过足够多次操作才会再次重建。

`centered_segment_tree.cpp` uses a segment tree over a centered time axis. It
is a simpler reliable alternative with `O(log Q)` per operation. /
`centered_segment_tree.cpp` 在线段树的中心化时间轴上维护双端点，是每次
`O(log Q)` 的直观可靠替代方案。

## Storage, I/O, and performance / 存储、I/O 与性能

`two_stack_aggregate.cpp` stores both sides in one fixed continuous buffer.
The logical deque is always one contiguous interval separated by a middle
pointer. Rebalancing only moves that pointer and recomputes aggregates; it does
not copy values or allocate temporary vectors. All fields use unsigned
shape-aware direct input and padded modular output. / 性能版把两侧放在同一块
连续固定缓冲区中；逻辑双端队列始终是由中点分隔的一段连续区间。重平衡只移动
中点并重算聚合，不复制元素、不分配临时 vector。I/O 使用无符号 shape-aware
direct 路径和模数 padded 输出。

The summed official-case median is `0.999x` accepted public baseline `#211474`;
representative maximum random data is `0.998x`. / 全部官方用例总中位数相对公开
AC `#211474` 为 `0.999x`，典型最大随机数据为 `0.998x`。
