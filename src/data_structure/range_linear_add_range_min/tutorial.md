# Range Linear Add Range Min / 区间线性加与区间最小值

## 中文

更新给 `[l,r)` 中的第 `i` 个数加 `b*i+c`。把每个数组元素看成关于参数 `t`
的一条直线

```text
f_i(t) = current_value_i + t*i。
```

给整段加 `b*i+c`，等价于把参数整体前进 `b`，再给所有直线加同一个常数
`c`。问题变成维护一组直线在参数移动时的最小值。

### 解法一：双向熔点 Kinetic Segment Tree

`kinetic_segment_tree.cpp` 是性能主解。每个结点保存：

- 当前最小值 `minimum`；
- 取得最小值的下标/斜率 `slope`；
- 参数向正方向还能移动多少而不更换最优直线 `forward_melt`；
- 参数向负方向的对应余量 `backward_melt`；
- 尚未下传的 `(lazy_slope,lazy_constant)`。

合并两个孩子时，先选当前值更小的一侧为赢家。孩子内部各自的下一次变化仍是
候选熔点；此外，若赢家斜率更大，随着参数正向移动，另一条赢家直线可能追上：

```text
cross = (other_min - winner_min) /
        (winner_slope - other_slope)。
```

负方向完全对称。取这些候选的最小值，就得到父结点的双向熔点。

给完整结点加 `(b,c)` 时：

- 若 `b>=0` 且 `b<=forward_melt`，或 `b<0` 且 `-b<=backward_melt`，
  当前最优下标不会变，可直接做
  `minimum += b*slope+c` 并移动两个熔点；
- 否则这次移动跨过了至少一个最优切换时刻，先下传到孩子，递归更新后重新
  合并父结点。

等号是安全的：交点处旧赢家与新赢家相等，继续保留旧赢家仍给出正确最小值；
下一次再移动时会触发重建。

#### 正确性

结点记录的是其所有叶直线的下包络当前最低线。熔点是当前最低线在对应移动
方向上第一次可能失效的时刻；在熔点以内，所有直线相对顺序保证当前赢家仍
最低，所以懒更新公式正确。跨过熔点时递归到孩子会显式处理发生变化的下包络，
重新合并后恢复同一不变量。区间更新由线段树规范分解组成，区间查询取这些
结点最小值，故答案正确。

### 解法二：Bridge 下凸包树

`bridge_hull_tree.cpp` 保存左右孩子下凸包公切线的两个端点。给完整结点加同一
直线不会改变孩子内部凸包结构；边界路径上的结点沿旧 bridge 向下寻找新公切
线。查询完整子树时比较 bridge 两端在累计懒标记后的高度，向更低一侧下降。
叉积使用 128 位整数。

它提供了几何上更直接、最坏界更明确的 `O(log^2N)` 模板，也可作为 kinetic
不变量的独立验证，但常数显著更大。

### 复杂度与性能

Bridge 版本每次操作 `O(log^2N)`、空间 `O(N)`。Kinetic tree 的一次安全更新
为线段树层数级；跨熔点的递归由下包络切换摊还，在本题数据和通用实践中非常
快，理论最坏可访问更多结点。

最大随机用例中，Kinetic 版本约 `268 ms`，优于公开第一 `#382268` 的
`318 ms`；Bridge 版本约 `1270 ms`。

## English

Treat each element as a line in a kinetic parameter:

```text
f_i(t) = current_value_i + t*i.
```

Adding `b*i+c` to a whole node advances the parameter by `b` and adds a common
constant `c`.

### Solution 1: bidirectional-melt kinetic segment tree

Each node stores its current minimum, the winning line slope/index, the
remaining safe movement in both parameter directions, and a lazy linear shift.
When merging children, their own melt times remain candidates. If the current
winner has the larger slope, the other child catches it after

```text
(other_min - winner_min) / (winner_slope - other_slope)
```

units of forward movement; the backward case is symmetric.

If an update stays within the relevant melt distance, the winner cannot change,
so update the minimum by `b*slope+c`, shift both melt distances, and keep a lazy
tag. If it crosses a melt point, push and recurse before rebuilding the node.
Equality is safe because both candidate lines tie at the crossing.

The stored winner is exactly the current lowest line of the subtree envelope.
Movement before the first crossing preserves that winner; movement past a
crossing is explicitly resolved in descendants. Induction over merges and the
segment-tree range decomposition proves both updates and minimum queries.

### Solution 2: bridge lower-hull tree

`bridge_hull_tree.cpp` stores the common tangent between child lower hulls.
A full-node linear shift preserves internal hull structure, while boundary-path
bridges are rebuilt. Queries descend through the lower bridge endpoint.
128-bit cross products avoid overflow. It offers a clear `O(log^2N)` geometric
alternative with larger constants.

On the maximum random case, the kinetic version takes about `268 ms`, beating
public leader `#382268` at about `318 ms`; the bridge variant is about
`1270 ms`.
