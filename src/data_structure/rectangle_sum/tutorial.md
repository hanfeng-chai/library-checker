# 矩形权值和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 189.165 (chaihf), sum 1557.202 (chaihf)。

- `main.cxx` max: 102.265 ms (-45.94%), sum: 854.445 ms (-45.13%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
把每个矩形拆成左右两个 x 前缀事件。将点的 y 与所有查询上下端点一起压缩，
按 x 扫描，在每个事件前插入横坐标严格较小的点，再用宽前缀树统计 y 区间和。
左右事件的差就是两个方向均为半开区间的矩形和。

实现见 rectangle_sum.h，支持重复坐标、空点集和退化矩形。
GCC/Clang 官方、朴素二维对照与 sanitizer 通过。
共享 Fenwick 加入共同高位抵消后，已重新核对正确性与完整测例的性能。

## 尝试过程与取舍

经典二维扫描线以 x 前缀差恢复矩形和，纵向复用宽 Fenwick。树类加入共同高位抵消后，依赖检查发现本题也间接使用该头文件，因此重新检查正确性并做完整五参考回归；这次共享修改的结果也保留在表中。
