# 单点修改、区间排序与函数复合

`main.cxx` 使用 sortable.h 的 SortableSequence。把序列分成递增或递减的块，
每块用 Patricia 压缩字典树保存互异键及正反两个方向的函数复合。
排序只需按秩切开边界、合并区间中的字典树，再指定方向；单点覆盖先隔离位置。
查询只读两端残块，完整中间块交给外层 SegmentTree，不额外切碎块。

字典树计数与分叉位打包，Affine 节点为 32 字节；空闲节点循环复用。
Bitmap 维护块边界。当前键须互异，不要求新键与已删除的旧键互异。
查询为 O(32+log N)，排序代价取决于合并时实际访问的分叉数；空间 O(N)。

23 个官方测例通过 GCC/Clang，另有随机数组对照及 ASan/UBSan 检查。
Lenovo 静默、不绑核、每例一次：main max/sum 为 577.579/6252.847 ms，
五份参考的最小值分别为 606.887/6511.077 ms。
证据：bench/ds-structures-20261001/round1/selected/。
旧 .cpp 为历史替代实现，当前性能结论只对应 main.cxx。
