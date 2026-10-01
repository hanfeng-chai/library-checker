# link_cut_sum

`LinkCutSum<T=i64, Subtree=false>(values)` 构造点权森林。link(u,v) 连接不同树，
cut(u,v) 删除已有边，add(v,x) 做单点加。默认模式提供 path_sum(u,v)。
Subtree=true 改为维护包含虚子树的和，提供 subtree_sum(v,root) 和已有边
(v,p) 的 side_sum(v,p)，此模式不提供 path_sum。

可在新构造的对象上用 build_tree(graph,root) 线性初始化一棵树。已有拓扑编号
时，build_ordered_tree(parent) 更直接：要求 parent[0]=0，其他 parent[v]<v。

节点缓存自己是左孩子、右孩子还是辅助树根。反转只标记方向，旋转前局部下传，
access 直接旋转接入新父亲。默认路径和模式允许辅助树根的聚合暂不刷新，
查询直接读取暴露路径的左段和加根值；内部 nodes 的缓存不应代替查询接口。
Subtree 模式在首选边变化时增减虚子树贡献，已有父边的查询省去额外换根。

操作摊还 O(log n)，存储 O(n)，没有递归 splay 或路径下传栈。允许负数，要求 T
足以容纳所有中间和。GCC/Clang 正式测例和独立 BFS 对照验证了动态断接边、
换根、正负更新、子树两个方向，以及任意初始根的 DFS 重编号导入；ASan/UBSan 通过。
