# preorder

`OrderedPreorder(parent)` 要求 parent[0]=0 且非根 parent[v]<v，返回 position 和
size；v 的子树区间为 [position[v],position[v]+size[v])。支持空树。

先逆序累加子树大小，再正序把孩子分配到父区间的右端；无需邻接表或递归，
同级孩子按编号倒序访问。时间、空间 O(n)，通过逐点祖先关系的独立区间对照。
