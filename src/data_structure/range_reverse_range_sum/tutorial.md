# Range Reverse Range Sum / 区间翻转、区间求和

## 中文

数组元素本身没有修改，只需要改变顺序并查询和。两种实现都使用“子树中序
序列就是当前序列”的隐式平衡树，并在结点维护子树大小与权值和。

### 解法一：双哨兵隐式 Splay

`implicit_splay.cpp` 在真实序列两端各放一个权值为零的哨兵，并初始构造成
完全平衡的二叉树。要隔离半开区间 `[l,r)`：

1. 把中序排名 l 的左哨兵/边界结点 splay 到根；
2. 在根的右子树中，把相对排名 `r-l` 的右边界 splay 到该右子树根；
3. 此时目标区间恰好是“根的右孩子的左子树”。

求和直接读取这棵中段子树的 `sum`。翻转只需给中段切换 `reversed` 标记。
标记下传时交换左右孩子，并把翻转标记异或给两个孩子。

这里使用按排名的自顶向下递归 splay：下降前先下传标记，根据左子树大小决定
方向，再做 zig-zig 或 zig-zag 旋转。一次操作连续访问相近的两个边界，Splay
会把它们留在树顶附近。

#### 正确性

两次 splay 只做旋转，不改变中序序列，因此隔离出的左子树中序恰好是原序列
排名 `[l,r)`。翻转一棵树的中序序列等价于交换左右孩子并递归翻转二者；懒
标记正是延迟执行这个等式。任何按排名下降或旋转前都会 push，所以结构操作
看到的左右顺序真实有效。翻转不改变元素集合，子树和保持不变，故两种操作
都正确。

### 解法二：隐式随机 Treap

`implicit_treap.cpp` 用随机优先级保持期望平衡。两次 split 隔离 `[l,r)`，
读取和或切换翻转标记，再用 merge 恢复。它是插入、删除、剪切粘贴等序列题
中更易组合的通用模板；本题只需要固定序列区间操作，Splay 的双边界隔离更快。

### 复杂度与性能

- Splay：`O(N)` 平衡建树，每次操作均摊 `O(logN)`；
- Treap：期望 `O(NlogN)` 建树，每次操作期望 `O(logN)`；
- 两者空间均为 `O(N)`。

硬件计数器下，Splay 主解在最大随机用例与公开第一 `#278235` 的 cycles 差约
`1.5%`；Treap 墙钟约 `819 ms`，作为更通用的替代解保留。

## English

Both variants use an implicit tree whose in-order traversal is the current
sequence. Every node stores subtree size and sum.

### Solution 1: implicit Splay with two sentinels

Place zero-valued sentinels around the real sequence and initially build a
balanced tree. To isolate `[l,r)`, splay the node of in-order rank `l` to the
root, then splay relative rank `r-l` inside its right subtree. The target range
is now exactly the left child of that right-subtree root.

A sum reads this middle subtree aggregate. Reversal toggles its lazy flag.
Pushing a reversal swaps both children and toggles the flag on each child.
Rank-based recursive splaying pushes before descent and uses zig-zig or
zig-zag rotations based on subtree sizes.

Rotations preserve in-order order, so the two boundary splays isolate exactly
the requested ranks. Recursively reversing an in-order sequence is equivalent
to swapping its children and reversing both; the lazy tag records precisely
this operation. Sums are unchanged by reversal, proving correctness.

### Solution 2: implicit randomized Treap

Two splits isolate the range, its aggregate is read or its reversal flag is
toggled, and merges restore the sequence. The Treap is easier to extend with
insertions, deletions, and cut/paste operations, while the sentinel Splay is
faster for this fixed sequence.

The Splay builds in `O(N)` and has amortized `O(logN)` operations. The Treap
builds in expected `O(NlogN)` and has expected `O(logN)` operations. Both use
`O(N)` memory. Hardware cycles for the Splay are within about `1.5%` of public
leader `#278235`.
