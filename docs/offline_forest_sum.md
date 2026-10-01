# offline_forest_sum

`OfflineForestSum<T=i64, RangeAdd=false>(n,q)` 先记录操作，再用 `solve(values)`
返回按记录顺序排列的查询答案。森林初始无边；link(u,v) 要求连接不同连通块，
cut(u,v) 要求边存在。`add_vertex(v,x)` 做单点加，`sum(v)` 查询连通块和；
RangeAdd=true 另支持 `add_component(v,x)`。允许负权，约定 T 足以容纳中间值。

q 用于预分配：最多 n+3q 条事件、n+q 种不同边键。树的子树操作可记录为
“临时 cut(v,p)、操作 v 所在连通块、link(v,p)”，本库不读取题目或生成器。

边权设为负的删除时刻，最后仍存在的边取最小值。这样每次删除的都是当前
最大权边，能用 anti-monopoly tree 维护按阈值划分的连通块。子树超过父亲
大小的 2/3 时提升；点权和沿父链维护，整块加使用相对父亲的差分偏移。
内部树的边不等于原森林的边，因此内部 ExpiryForest 不能用于任意在线删边。
算法参考仓库中 nandhagk/Rohan_Kapri 和 anon123 的提交。

事件预处理期望 O(n+q)，每个事件摊还 O(log n)，总空间 O(n+q)。不需要 splay
旋转中的路径聚合，也不需要为查询保存函数对象。单点版本省去范围加标记。
两道动态子树和各 18 个正式测例通过 GCC/Clang；带正负权、重复断接边、双向
子树操作及十万点长链的独立 BFS 对照通过 ASan/UBSan。
