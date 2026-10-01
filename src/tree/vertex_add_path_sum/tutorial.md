# 单点加、树路径和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 274.619 (Anonymous), sum 2570.802 (Anonymous)。

- `main.cxx` max: 239.221 ms (-12.89%), sum: 2311.230 ms (-10.10%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 用 heavy_light.h 建立连续子树区间，再将父亲重编号为 DFS 位置，
交给 ordered_lca.h。读入操作时预先映射位置并求 LCA；这些几何信息不依赖权值，
实际加法与查询仍按原顺序执行。

维护每点到根的权值和。给 v 加 delta 等价于给 v 的所有后代的根路径和加 delta，
在 DFS 差分中只改两个端点。WideFenwick.add_difference 把相反更新的共同高层
直接抵消。路径和为 S(u)+S(v)−2S(lca)+a[lca]；祖先情形只需一次区间差。
静态几何表处理完后释放，在线循环只访问操作记录、当前权值与 Fenwick。

19 个官方测例通过 GCC/Clang。任意标签/根、子树区间和逐点路径和的朴素对照
及 ASan/UBSan 通过。

## 尝试过程与取舍

先将根路径和表示为 DFS 区间上的差分，再用 OrderedLCA 还原任意路径和。随后加入两个相反更新在共同祖先处抵消，并预先解析所有操作的 DFS 位置与 LCA；更新仍按原次序执行。执行前释放几何数组，减少热路径中的间接读取；共享 Fenwick 的旧题也重新回归。
