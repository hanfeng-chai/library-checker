# 静态矩形加、矩形求和

`main.cxx` 调用 rectangle_add_sum.h。矩形四个角点各自贡献带符号的
w·(x−x0)·(y−y0)，扫描 x 后，用 y 前缀 Fenwick 保存四个双线性系数。
查询矩形是左右、上下四个前缀的容斥。相接边界贡献为零，符合半开矩形定义。

bilinear.h 把四个系数保存为 u32 规范余数，SSE 同时模加减，减少树的存储和
后续求值的规约。排序同时压缩更新与查询端点，避免逐查询二分。

GCC/Clang 官方、几何交集面积 u128 朴素对照及 sanitizer 通过。
扫描时预取后续第八个更新的矩形记录与两个底层 Fenwick 位置，隐藏部分随机访存。
最终 Lenovo 单次 max/sum 为 302.084/1409.839 ms，参考最小值为 343.788/1559.259 ms。
证据：bench/ds-rectangle-area-20261001/round2/selected/。旧 .cpp 保留作参考。
