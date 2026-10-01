# 二分图最大匹配

`BipartitePush(l,r,edges)` 使用带周期性全局重标号的 push-relabel 匹配。`BipartiteMatching(l,r,Buffer<edge>)` 是 Hopcroft–Karp，消费输入，先做贪心匹配再分层增广；较小侧放左边，增广使用显式栈。两者提供 `left`、`right`、`size`，未匹配为 `~0u`。Hopcroft–Karp 最坏 O(m sqrt(n))；保留它作为经典基线。
