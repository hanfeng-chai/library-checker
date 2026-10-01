# suffix_automaton

`SuffixAutomaton(s)` 为小写字母串建立后缀自动机，转移使用 `AlphabetTrie`。
节点保存最大长度、suffix link 和一个出现位置；转移克隆只复制实际存在的边。

- `distinct`：不同非空子串数，构造时累加 `len[v]-len[link[v]]`。
- `longest_common_substring(t)`：返回 `{a,b,c,d}`，使 `s[a:b] == t[c:d]`
  且长度最大。并列答案任取一个，空答案返回全零。

最多 `2*n+1` 个预留节点；固定字母表下构建、扫描均为线性时间，空间 O(n)。
