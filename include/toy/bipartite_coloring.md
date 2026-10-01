# 二分图边染色

`BipartiteColoring<Random>(l,r,edges)` 返回最大度 `degree` 和逐边颜色 `color`，支持重边。合并同侧低度点并补边为正则图；偶数度沿欧拉回路交替分半，奇数度先取一个完美匹配。

`Random=false` 用 Hopcroft–Karp，当前题解主解采用这个版本；`Random=true` 使用固定种子的随机交替游走，仍给出确定的正确染色，但不同图结构的运行时间差异更大。补边、合并不改变原边颜色合法性。
