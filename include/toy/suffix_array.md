# suffix_array

`suffix_array(string_view)` 返回所有非空后缀的起点，按字典序排列；字符串支持
ASCII `[0,127]`。`suffix_array<T>(span,upper)` 接受 `[0,upper]` 的整数序列。
`lcp_array(s,sa)` 返回相邻后缀的 LCP，长度为 `max(n-1,0)`。

主体是 SA-IS：分类 L/S，排序 LMS 子串，递归其名字，再诱导全部后缀。
LMS 名字互异时省去递归，小序列直接排序。LMS 不相邻，名字索引按位置除二
存储，另用位图识别 LMS；无须保留整份 L/S 类型表。字符串入口先尝试短周期和有界前缀
证书；失败仍执行完整 SA-IS，证书规则见 [suffix_order.md](suffix_order.md)。
另外，若一个奇偶位置上恰好都是严格最小字符，可验证后删除这一层分隔符，
递归另一半再恢复顺序；短 LMS 子串的种类很少时直接命名。
这些路径都保持精确排序，不依赖随机哈希。

`distinct_substrings(s)` 使用后缀后继数组直接累加 LCP，不另存 rank 和 LCP 数组。
一般字母表诱导时将前驱的 L/S 类型放入 SA 项的符号位，减少随机访存。
只有两个不同符号时，两只桶可直接作为正反向队列，省去空槽扫描、清空和符号编码。

SA-IS 时间 O(n+upper)，空间 O(n+upper)。LCP 使用 Kasai，延伸部分共用有界
AVX2 字节比较，时间 O(n)、空间 O(n)。返回数组拥有存储。

基础诱导排序参考 ac-library 的 CC0 SA-IS 实现。
