# 费用缩放的有界最小费用流

`MinCostFlow(n,m)` 中 balance 是顶点供给量（流出减流入），`add(u,v,lo,hi,cost)` 支持有符号上下界和费用。`solve()` 返回可行性；成功后 cost 为 i128 总费用，flow 为逐边流量，potential 为可验证的对偶势。

先用 Dinic 找可行流，再进行 epsilon 费用缩放。费用乘 n+1 后，epsilon=1 已足以排除负费用残量环。最后在原费用上恢复精确对偶势。容量、势使用 i64，目标值使用 i128。
