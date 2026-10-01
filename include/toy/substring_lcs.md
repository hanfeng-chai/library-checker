# substring_lcs

维护 seaweed 标签，回答 `LCS(s[0:a],t[b:c])`。`lcs_row` 是一个字符对应的
DP 行更新：相等字符，或逆序的两根标签，相互交换。负标签来自左边界。

`prefix_substring_lcs(s,t,queries)` 是离线接口。输入记录为
`(a<<39)|(b<<29)|(c<<19)|id`，要求两个串长度均小于 1024，询问数小于 `2^19`。
按 a、b 做 radix 排序，每行用 16 个 64 位位图维护标签小于 b 的位置，
询问只需一次前缀计数和 popcount；结果按 id 排列。标签跨越阈值时，用两组
AVX2 比较更新 16 个位图块的前缀计数。固定这组范围后时间 O(|s||t|+q)，空间
O(|t|+q)。

`PrefixSubstringLCS(s,t)` 保存所有行，`query(a,b,c)` 可在线查询，采用
16 位标签的 AVX2 比较。构造 O(|s||t|)，查询 O((c-b)/16+1)，空间 O(|s||t|)。
