# eertree

`Eertree(capacity)` 维护小写字母串的不同回文。按位置递增调用 `extend(s,at)`，
返回以 at 结尾的最长回文节点；s 的已用前缀在调用时必须可读。

内部根 0 长度 -1，根 1 长度 0。普通节点保存回文长度、去掉首尾后的 parent、
最长真回文后缀 suffix 和 series link。转移使用 `AlphabetTrie`，每个字符最多
新建一个节点，capacity 至少为 n（构造函数另留两个根）。节点编号与题目输出编号相差一。
