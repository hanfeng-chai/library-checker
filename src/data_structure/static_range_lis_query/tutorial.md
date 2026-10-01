# 静态区间 LIS

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 439.949 (chaihf), sum 1839.167 (chaihf)。

- `main.cxx` max: 332.102 ms (-24.51%), sum: 1633.958 ms (-11.16%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 调用 lis.h，查询按右端点分组。维护半局部 LIS 的临界位置：插入当前
位置后，按排列值向右进行 bumping，移除最终被推出的位置。活动集合在 [l,r)
中的计数等于该区间 LIS 长度，因此用 PrefixTree32 即可回答。

bumping 按值分块，首块显式计算，后续整块用最大堆与延迟最小堆维护等价变换。
显式部分的八路前缀最大值并行处理槽位，跳过全零块；堆重建使用线性 heapify。
新 BinaryHeap 的 replace_top、push_pop 合并了常见的两步操作。

GCC/Clang 全部官方测例通过；逐步对照未分块递推，逐查询对照独立 LIS，并通过
sanitizer。

## 尝试过程与取舍

采用半局部 LIS 的临界位置与 bumping 递推，先与未分块递推逐步对照，再加入按值分块、双堆延迟处理和 SIMD 前缀最大值。堆重建改用线性 heapify，replace_top/push_pop 合并常见的两步操作；逐查询仍与独立 LIS 算法比较。
