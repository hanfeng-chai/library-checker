# min_plus_convolution

三个接口都接收两个 `span<const T>`，返回 `Buffer<T>`：

- `min_plus_convex_convex`：两边凸，合并差分，O(N+M)。
- `min_plus_convex_arbitrary`：第一边凸，决策单调性分治，O((N+M) log(N+M))。
- `min_plus_concave_arbitrary`：第一边凹，分块维护平移函数的下包络，O((N+M) log(N+1))。

凸/凹分别指相邻差分不减/不增。允许负数，要求输入有限、任意两项的和能由 T 表示。
空输入返回空数组，额外空间均为 O(N+M)。分治和包络版本的总长度须能用 int 表示。

凹序列方案把任意序列按 N 分块，每块分成两个三角区域，右侧反向访问即可，
无需复制。新候选与旧候选之差随位置单调增加：若插入时已经不优，直接丢弃；
否则二分其有效前缀，用栈存候选及失效位置。这个包络思路也出现在仓库的
Anonymous、rqi 提交中。

三个版本均通过全部官方测例、GCC/Clang 的 6,375 项小规模朴素对照，
初版通过 ASan/UBSan；凹序列增加提前丢弃判断后重新通过官方和朴素对照。

三个主解均已达到五份对照的 max/sum 标准。
当前首轮结果和原始证据统一见 [评测审计](measurement_audit.md)。
