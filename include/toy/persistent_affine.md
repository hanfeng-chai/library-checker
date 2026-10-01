# persistent_affine

`PersistentAffineArray<P>(values,reserve)` 创建初始 root，reserve 为预计节点数。
`apply(root,l,r,{a,b})` 返回区间仿射更新后的新根；
`copy(destination,source,l,r)` 把 source 的同一区间复制到 destination，返回新根；
`sum(root,l,r)` 读取该版本的区间和。所有区间半开，旧根始终有效，查询不分配节点。

节点存普通余数的和与 Montgomery 编码的懒标记。修改仅复制两条边界路径及
必要的标记节点；完整子树尽量共享。递归复制时分别携带两个版本的祖先变换，
避免把某个版本的标记错误地作用到另一个版本。查询从孩子返回局部和，再作用
当前节点的标记，用实际交集长度计算平移项。

P 为小于 2^30 的奇数，n<2^31，输入值与系数为规范余数。混合表示的乘加合并
为一次 Montgomery 规约。每次操作 O(log n)，q 次修改空间 O(n+q log n)。
GCC/Clang 官方、旧版本/区间复制/零斜率/小模数/空区间/扩容对照及 sanitizer 通过。
