# ordered_lca

`OrderedLCA(parent)` 要求 parent[0]=0，且每个非根满足 parent[v]<v；lca(a,b)
返回最近公共祖先。适合编号已经按祖先在前排列的树，不能直接用于任意父编号。

Schieber–Vishkin 标号先按叶子顺序分配编号，再选出子树里 lowbit 最大的标号。
相同标号的点形成祖先链，ascendant 位集记录根路径上的链。查询找到两端共享链，
用 head_parent 跳过各自最后一条分支；同链上编号较小的点就是祖先。
预处理和空间 O(n)，查询 O(1)。链、星形、二叉树和随机父数组均通过父链朴素
对照、GCC/Clang 与 sanitizer。
