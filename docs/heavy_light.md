# heavy_light

`HeavyLight(n)` 依次 add_edge(u,v)，然后 build(root=0)。只构建一次，输入为一棵
树；支持任意编号和根，空树可构建。结果字段为 parent、depth、size、heavy、head、
position、vertex，根的父亲为自己。子树占连续的 position 区间。

剥叶得到父指针、子树大小、重孩子和逆删除顺序。按祖先在前的顺序分配位置：
重孩子紧接父亲，其余孩子从子树区间末尾向前分配。无需邻接表，也不需要递归。
预处理和空间 O(n)，lca、ancestor、jump 为 O(log n)；越界跳转返回 UINT32_MAX。

独立父链对照检查任意标签/根、重链位置、完整子树区间和路径跳转；根路径差分
加 WideFenwick 的动态路径和也与逐点求和对照。长链不依赖调用栈深度。
