# Point Add Range Sum / 单点加、区间和

## 中文

### Fenwick 树（`fenwick_tree.cpp`）

Fenwick 数组的下标 `i` 保存长度为 `lowbit(i)`、以 `i` 结尾的一段
前缀。单点增加时不断执行 `i += lowbit(i)` 更新所有包含该点的块；
求前缀和时不断执行 `i -= lowbit(i)`，这些块恰好无重叠地拼成前缀。
区间和等于两个前缀和之差。

初始数组用线性建树：先把值放进对应位置，再把每个节点一次性加到
父节点。更新和查询均为 `O(log N)`，空间 `O(N)`，常数很小。

### 线段树（`segment_tree.cpp`）

维护完整二叉树，点修改后沿祖先重算，区间查询分解为左右累积器。
同样是 `O(log N)`，但内存访问更多；优点是容易推广到非可逆结合运算。

两份源码均使用 direct mapping、一位固定操作码、六位索引、十位增量和同一
CompactWriter。线段树也按操作类型选择第三字段的真实形状。

## English

The Fenwick tree stores a `lowbit(i)`-sized suffix of each prefix. Updates walk
upward and prefix queries walk downward, both in `O(log N)`. A range sum is the
difference of two prefixes. `segment_tree.cpp` uses a general segment tree with the
same asymptotic complexity but larger constants.

Both use direct mapping, fixed one-digit operation codes, six-digit indices,
ten-digit increments, and the same CompactWriter.
