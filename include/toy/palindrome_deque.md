# palindrome_deque

`PalindromeDeque(q)` 支持最多 q 次小写字符插入及两端删除。
`push<false>(c)` / `push<true>(c)` 在前端 / 后端插入编号 `[0,26)`；
`pop<false>()` / `pop<true>()` 删除对应端，要求非空。
`distinct`、`prefix()`、`suffix()` 给出当前不同回文数及最长回文前后缀长度。

采用双端回文树的 surface 表示：每个字符位置只保留尚未被更长回文覆盖的
前后缀端点。插入、删除更新边界 surface；quick link 跳过相同扩展字符的
后缀链接，避免逐个试探长链。

已经创建的节点不会删除，之后可复用；节点的 surface 计数和活跃 suffix 子节点
计数共同决定它是否仍出现在串中。空间 O(q)，没有逐次分配。算法参考题库官方
解答中的 surface / quick-link 双端回文树。
