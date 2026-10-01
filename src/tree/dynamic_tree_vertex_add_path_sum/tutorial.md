# 动态树单点加与路径和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 399.498 (Rohan_Kapri), sum 1853.888 (nandhagk)。

- `main.cxx` max: 386.793 ms (-3.18%), sum: 1778.295 ms (-4.08%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 link_cut_sum.h 的在线路径和模式。初始树先由 packed_preorder.h
线性剥叶并按 DFS 序重编号，直接导入父亲在前的数组；之后所有顶点操作使用
固定映射，边仍可动态断开、连接。初始化 O(N)，操作摊还 O(log N)，空间 O(N)。

15 个正式测例通过 GCC/Clang，独立 BFS 对照覆盖带符号权值、动态断接边及
任意初始根的有序父数组导入，ASan/UBSan 通过。

## 尝试过程与取舍

首版 Link-cut Tree 显式下传祖先标记，之后改为旋转前局部下传、缓存孩子方向，并在 access 中直接旋转接入。再比较延迟根聚合、强制内联、打包标记和物理重编号。重编号改善 max，却因完整 HLD 与重复建图拖累 sum；最终用紧凑剥叶 DFS 编号直接导入有序父数组，保留局部性同时降低初始化成本，双项达标。
