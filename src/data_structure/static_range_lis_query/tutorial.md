# 静态区间 LIS

`main.cxx` 调用 lis.h，查询按右端点分组。维护半局部 LIS 的临界位置：插入当前
位置后，按排列值向右进行 bumping，移除最终被推出的位置。活动集合在 [l,r)
中的计数等于该区间 LIS 长度，因此用 PrefixTree32 即可回答。

bumping 按值分块，首块显式计算，后续整块用最大堆与延迟最小堆维护等价变换。
显式部分的八路前缀最大值并行处理槽位，跳过全零块；堆重建使用线性 heapify。
新 BinaryHeap 的 replace_top、push_pop 合并了常见的两步操作。

GCC/Clang 全部官方测例通过；逐步对照未分块递推，逐查询对照独立 LIS，并通过
sanitizer。Lenovo 单次 max/sum 为 345.498/1645.625 ms，五份参考最小值为
454.100/1857.430 ms。证据：bench/ds-lis-20261001/round1/。旧 .cpp 保留作参考。
