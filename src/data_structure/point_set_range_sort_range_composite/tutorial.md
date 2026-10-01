# 单点修改、区间排序与函数复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 608.425 (chaihf), sum 6515.350 (Rohan_Kapri)。

- `main.cxx` max: 579.709 ms (-4.72%), sum: 6266.149 ms (-3.82%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 sortable.h 的 SortableSequence。把序列分成递增或递减的块，
每块用 Patricia 压缩字典树保存互异键及正反两个方向的函数复合。
排序只需按秩切开边界、合并区间中的字典树，再指定方向；单点覆盖先隔离位置。
查询只读两端残块，完整中间块交给外层 SegmentTree，不额外切碎块。

字典树计数与分叉位打包，Affine 节点为 32 字节；空闲节点循环复用。
Bitmap 维护块边界。当前键须互异，不要求新键与已删除的旧键互异。
查询为 O(32+log N)，排序代价取决于合并时实际访问的分叉数；空间 O(N)。

23 个官方测例通过 GCC/Clang，另有随机数组对照及 ASan/UBSan 检查。

## 尝试过程与取舍

用按值组织的分块结构维护区间排序，块中保存双向函数复合，按下标定位边界后再拆分和合并。排序方向与非交换复合分别维护；完整对照前检查重复键、零斜率及相反方向操作，保留最终确定性算法。
