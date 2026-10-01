# potential_dsu

`PotentialDSU<Group>(n)` 维护群上的相对势。merge(a,b,delta) 约束
value[a]=value[b]*delta，若矛盾返回 false 并保持已有关系；difference(a,b)
返回 value[b]^-1*value[a]，不可确定时返回 nullopt。

父边保存 value[x]=value[parent[x]]*weight[x]。路径压缩按从根到点的顺序复合，
保持非交换群的乘法方向；按大小合并，压缩路径均摊 O(α(n)) 次群运算。
根的权值隐含为单位元，无需初始化整份权值数组。
