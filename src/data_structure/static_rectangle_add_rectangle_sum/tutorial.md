# 静态矩形加、矩形求和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 342.202 (toomer), sum 1560.558 (toomer)。

- `main.cxx` max: 295.422 ms (-13.67%), sum: 1402.966 ms (-10.10%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 调用 rectangle_add_sum.h。矩形四个角点各自贡献带符号的
w·(x−x0)·(y−y0)，扫描 x 后，用 y 前缀 Fenwick 保存四个双线性系数。
查询矩形是左右、上下四个前缀的容斥。相接边界贡献为零，符合半开矩形定义。

bilinear.h 把四个系数保存为 u32 规范余数，SSE 同时模加减，减少树的存储和
后续求值的规约。排序同时压缩更新与查询端点，避免逐查询二分。

GCC/Clang 官方、几何交集面积 u128 朴素对照及 sanitizer 通过。
扫描时预取后续第八个更新的矩形记录与两个底层 Fenwick 位置，隐藏部分随机访存。

## 尝试过程与取舍

先把矩形四角转成带符号的双线性贡献，按 x 扫描、在 y 前缀保存四个系数。四系数改为规范 u32 并用 SIMD 模加减，压缩排序直接写回下标；第二轮预取后续更新记录和底层位置，降低随机访存等待。
