# Cartesian Tree / 笛卡尔树

## 中文

### 定义

笛卡尔树同时满足：

1. 中序遍历的顶点顺序是原数组下标 `0,1,...,N-1`；
2. 每个父亲的值小于孩子的值，即最小值位于根。

数值互异，因此树唯一。

### 单调栈构造

从左到右维护一个按数组值严格递增的下标栈。处理新下标 `i`：

1. 不断弹出值大于 `a[i]` 的栈顶，记最后弹出的根为 `left_root`；
2. 若栈仍非空，新栈顶是左侧第一个更小值，它成为 `i` 的父亲；
3. 若弹出过顶点，`left_root` 是被整体截下的连续后缀子树，它的父亲改为
   `i`；
4. 将 `i` 入栈。

为什么只连接最后弹出的点？更早弹出的点已经位于 `left_root` 的子树内，
保留原父子关系即可。剩余栈顶比 `a[i]` 小，且在下标上离 `i` 最近，正好
保持堆序与中序顺序。

每个下标只入栈、出栈一次，时间 `O(N)`、空间 `O(N)`。最终栈底是全局最小
值，对题目要求令其父亲为自身。

### 两个实现

`fixed_monotonic_stack.cpp` 调用固定容量公共模板，避免动态分配，作为性能
实现。`vector_monotonic_stack.cpp` 使用 `std::vector` 展开同一个不变量，
代码更便于单题阅读，但仍是最优的 `O(N)` 算法，并非故意缓慢的朴素解。

这里不存在既明显更简单、又能在百万点最坏数据上可靠 AC 的不同算法；递归
寻找区间最小值会在单调数组退化为 `O(N^2)`，因此不作为正式变体。

所有变体都使用 direct-mapped Reader。`N` 位于七位范围、数组值位于十位
范围且近似数值均匀，分别调用 `read_uniform<7>` 和
`read_uniform<10>`。父亲编号小于一百万，统一调用
`write_token_u32_6`，没有让较简洁变体退回通用 I/O。

在完整 25 个官方用例、CPU 3、统一
`-Ofast -flto -fno-exceptions -fno-rtti -march=native` 的交错测试中，固定
容量版本相对当时最快公开实现的总耗时比约为 `0.939x`，即快约 6.1%。

## English

### Definition and invariant

A Cartesian tree has the original indices as its inorder traversal and obeys
the min-heap property. Distinct values make it unique.

Scan left to right while maintaining indices with strictly increasing values.
For a new index `i`, pop every larger stack top. The remaining top, if any,
is the nearest smaller value on the left and becomes the parent of `i`. The
last popped root is the whole suffix subtree displaced by `i`, so its parent
becomes `i`.

Earlier popped vertices already belong below that last root and retain their
relationships. Consequently both inorder order and heap order remain valid.
Each index is pushed and popped once, giving `O(N)` time and `O(N)` memory.
The stack bottom is the global minimum and is reported as its own parent.

`fixed_monotonic_stack.cpp` uses the reusable fixed-capacity template and
avoids dynamic allocation. `vector_monotonic_stack.cpp` spells out the same
invariant with `std::vector`; it is easier to read but remains optimal
`O(N)`, rather than being intentionally slow.

A recursive range-minimum construction is superficially simpler but degrades
to `O(N^2)` on monotone arrays, so it is not a production variant.

Both sources use the direct-mapped Reader, value-uniform policies for the
seven- and ten-digit bounds, and `write_token_u32_6` for parent IDs. The
simpler algorithm presentation does not fall back to slower generic I/O.

Across all 25 official cases on CPU 3, with both programs compiled using
`-Ofast -flto -fno-exceptions -fno-rtti -march=native` and run in interleaved
order, the fixed-capacity implementation measured about `0.939x` the total
time of the fastest public baseline available during this audit.
