# Dynamic Sequence Range Affine Range Sum / 动态序列仿射与区间和

## 中文

### 1. 序列需要什么

题目同时要求任意位置插入/删除、区间翻转、区间仿射变换和区间和。数组无法快速
移动元素，普通线段树又无法改变下标，因此使用“中序遍历就是序列”的隐式平衡树。
节点不保存显式下标，而保存子树大小；第 `k` 个元素由左子树大小定位。

### 2. 性能主解：隐式 Splay

`implicit_splay.cpp` 的公共 `AffineSplaySequence` 在真实序列两端各放一个哨兵。
要暴露 `[l,r)`：

1. 把左哨兵（秩为 `l`）伸展到根；
2. 在根的右子树中，把右哨兵伸展到该子树根；
3. 右哨兵的左儿子恰好包含 `[l,r)`。

之后翻转或仿射修改只需给这棵子树打标记，查询直接读取其和。插入把新节点接到
两个相邻哨兵之间，删除则暴露单点后断开。

Splay 的 zig-zig / zig-zag 旋转在沿秩下降前先下推标记。旋转时复用旧根已经
正确的整棵子树大小与和，只重新计算变成孩子的节点；区间修改后也用“旧子树和
替换为新子树和”的增量更新两个祖先，减少重复 pull。

### 3. 懒标记

节点维护 `size`、单点值、模意义下的 `sum`、仿射标记和翻转位。

- `x -> a*x+b` 令 `sum -> a*sum+b*size`；
- 新仿射标记按实际时间顺序复合；
- 翻转交换左右儿子并异或翻转位，区间和不变；
- 结构下降前先把翻转和仿射标记传给孩子。

不变量是：忽略尚未下推的标记时，节点记录的 `value` 和 `sum` 已经反映全部
更新；按需 push 后，中序顺序及孩子信息与之完全一致。旋转保持中序序列，pull
重新组合左右和，因此所有序列操作正确。

### 4. 随机 Treap 替代解

`implicit_treap.cpp` 用随机优先级保持平衡。按子树大小 split 两次即可隔离区间，
merge 恢复整棵树；它使用相同的和、仿射与翻转不变量。Treap 代码路径规则、易于
迁移，期望复杂度稳定，是独立且可过最大数据的算法，不是降速版。

### 5. 复杂度与性能

- Splay：每个操作摊还 `O(log N)`。
- Treap：每个操作期望 `O(log N)`。
- 两者空间均为 `O(N+I)`，`I` 是插入次数；本实现按 `N+Q` 预留节点。

两种实现均通过 33/33 官方用例及与 `std::vector` 的随机对拍。固定 CPU 的最大
用例中，Splay 的 cycles 是公开第一 `#278236` 的 `0.9994x`，指令数为
`0.969x`，属于榜首级且略少指令；Treap 保留为更容易复用的随机平衡方案。

## English

### 1. Implicit balanced trees

The sequence needs insertion, deletion, reversal, affine range updates, and
range sums. In an implicit tree, inorder traversal is the sequence and subtree
sizes replace explicit keys, allowing rank-based navigation.

### 2. Preferred implicit splay

`implicit_splay.cpp` uses the reusable `AffineSplaySequence` with one sentinel
at each end. To expose `[l,r)`, it splays the left boundary to the root, then
splays the right boundary inside the root's right subtree. The left child of
that second boundary is exactly the requested interval.

A range operation then touches one subtree. Insertion links a new node between
adjacent boundaries; deletion exposes a singleton and detaches it. Lazy tags
are pushed before rank descent and rotations. Rotations reuse the old whole
subtree aggregate, and range updates replace ancestor sums by delta instead of
recomputing unchanged paths.

### 3. Lazy invariants

Each node stores size, value, sum, affine tag, and a reversal bit.

- `x -> a*x+b` changes `sum` to `a*sum+b*size`;
- affine tags compose in chronological order;
- reversal swaps children and toggles the bit without changing the sum;
- both tags are propagated before structural descent.

Recorded values and sums already include every tag at that node. Pushing makes
children consistent, rotations preserve inorder order, and pulling recombines
correct child aggregates. These invariants prove every operation.

### 4. Randomized treap alternative

`implicit_treap.cpp` balances by random priorities. Two rank splits isolate a
range and merges restore the tree, using the same lazy invariants. It is an
independent maximum-constraint solution with expected `O(log N)` operations,
not a deliberately slow reference.

### 5. Complexity and performance

Splay operations are amortized `O(log N)`; treap operations are expected
`O(log N)`. Both use `O(N+I)` nodes for `I` insertions.

Both pass all 33 official cases and randomized differential tests against a
vector. On the largest pinned case, the splay uses `0.9994x` the cycles and
`0.969x` the instructions of public leader `#278236`, placing it in the same
top performance tier.
