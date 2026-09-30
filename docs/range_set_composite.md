# range_set_composite

`RangeSetComposite<P>(values, assignments)` 支持 `set(l,r,f)`、`fold(l,r)` 和
`evaluate(l,r,x)`。函数按下标递增顺序应用，P 是小于 2^30 的奇数。
输入/返回 Affine 和 x 均为普通余数，内部使用 Montgomery 冗余表示。

每次赋值缓存 f 的二进制幂，各懒标记保存幂表下标。边界更新下推旧赋值，
完整节点直接采用对应长度的幂。只读查询遇到赋值标记时直接计算所需子区间长度
的幂；其余分成迭代前缀与后缀遍历，避免递归和为查询下推标记。

assignments 是预计赋值次数，用于预留幂表，可自动增长；幂表不回收旧赋值。
树空间 O(n)，幂表 O(assignments·log n)，修改 O(log n)，查询 O(log n)。
较大树/标记/幂表建议使用透明大页，正确性不依赖内核是否采纳。
GCC/Clang 官方与含零斜率、空区间、非二次幂长度的随机对照及 sanitizer 通过。
