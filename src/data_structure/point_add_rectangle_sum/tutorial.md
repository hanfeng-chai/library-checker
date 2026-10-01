# 单点加、矩形求和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 277.784 (toomer), sum 2194.929 (toomer)。

- `main.cxx` max: 266.631 ms (-4.02%), sum: 2141.937 ms (-2.41%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 预读全部操作，登记初始点和未来加入点的坐标，再用 wavelet_sum.h
建立带 Fenwick 的 Wavelet Matrix。未来点初值为零，按原顺序重放时才增加权重，
因此预读不会让未来的权重提前参与查询。

按 x 排序后，矩形在 x 轴变成连续区间；y 轴压缩后逐位划分。每层仅零分支
保存 Fenwick，矩形查询在上下 y 边界首次分叉处拆开计算。共同前缀和尾部零位
无需重复访问。四次坐标定位改为两组无随机分支的双端二分，重叠独立访存。
空间 O(N log N)，每次操作 O(log² N)。

18 个官方测例通过 GCC/Clang，重复坐标、空范围和未来零权点通过朴素对照及
ASan/UBSan。

## 尝试过程与取舍

先将坐标与操作离线整理，逐步比较零初始化、裁剪无效层、查询搜索路径和两端成对处理。重点是减少波列层上的重复访问，同时保持坐标重复、空区间及操作时序语义；最后以整题完整对照确定组合。
