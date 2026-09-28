# Queue Operate All Composite / 队列整体函数复合

## Idea / 思路

**English.** Affine functions form an ordered monoid: composing
`(a,b)` and then `(c,d)` gives `(ca, cb+d)`, modulo `998244353`. We need the
fold of a queue under a non-commutative operation.

The fast solution uses two aggregate stacks. The front stack stores elements
in pop order and its aggregate from top to bottom; the back stack stores
elements in insertion order and its aggregate from bottom to top. When the
front is empty, all back elements are transferred once in reverse order.
The whole queue is `fold(front)` followed by `fold(back)`.

**中文。** 仿射函数构成有序幺半群：先执行 `(a,b)` 再执行 `(c,d)`，结果为
`(ca, cb+d)`（模 `998244353`）。问题就是维护一个不可交换运算下的队列折叠值。

高性能解法使用两个“带聚合值的栈”。前栈按出队顺序保存元素，聚合方向为
栈顶到栈底；后栈按入队顺序保存元素，聚合方向为栈底到栈顶。当前栈为空时，
把后栈元素反序搬运过去；每个元素至多入栈、搬运、出栈各一次。整个队列的
复合值等于前栈聚合值后接后栈聚合值。

## Correctness / 正确性

**English.** Each stack node stores exactly the ordered fold from that node to
the relevant end of its stack. Transfer reverses storage order but also
reverses the aggregation direction, preserving queue order. Therefore the
front aggregate represents every front element, the back aggregate represents
every remaining back element, and composing them represents the entire queue.
Applying that affine function to `x` yields the requested answer.

**中文。** 每个栈节点都准确保存从该节点到对应栈端点的有序复合。搬运虽然
反转了存储顺序，却同时调整了聚合方向，因此队列顺序保持不变。于是前栈聚合
代表所有队首元素，后栈聚合代表其余队尾元素，二者顺序复合就代表完整队列；
将所得函数作用于 `x` 即为答案。

## Complexity / 复杂度

- Push, pop / 入队、出队: amortized `O(1)` / 摊还 `O(1)`
- Query / 查询: `O(1)`
- Memory / 空间: `O(Q)`

## Alternative / 另一种解法

`segment_tree_time_axis.cpp` assigns every insertion a monotonically increasing
position in a segment tree. Popping replaces the oldest live position with the
identity; the root always stores the queue composition. Every operation is
`O(log Q)`.

`segment_tree_time_axis.cpp` 为每次入队分配递增的时间轴位置。出队时把最早
的有效位置改成单位函数，线段树根始终保存整个队列的复合。每次操作为
`O(log Q)`，思路直观且能通过全部官方数据。

## Engineering and performance / 工程与性能

`two_stack_aggregate.cpp` uses fixed-capacity raw storage, so it neither
reallocates vectors nor default-constructs a large array of affine identities.
Every variant uses direct mapping, unsigned shape-aware reads, and padded
modular output. / 性能版使用固定容量原始存储，既不触发 vector 扩容，也不会
在启动时默认构造整片仿射单位元；所有变体都使用相同的最快 I/O。

On all official cases, its summed median is `0.998x` that of accepted public
C++ baseline `#234953`; `large_max_00` is `1.000x`. The result is effectively
equal within noise while retaining a generic monoid API. / 相对公开 AC
`#234953`，全套总中位数为 `0.998x`，最大用例为 `1.000x`；在保留通用幺半群
接口的同时达到同机持平。
