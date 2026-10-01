# 树上路径函数复合和

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 80.857 (Rohan_Kapri), sum 755.096 (nandhagk)。

- `main.cxx` max: 39.078 ms (-51.67%), sum: 349.829 ms (-53.67%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 tree_affine_sum.h。剥叶时把子树的值通过父边传上去：贡献为
b·subtree_sum+c·subtree_size，缓存这份贡献。逆删除顺序换根时，从父亲总和中
减去该子树贡献，再通过同一条边传回，得到本点看到的外部贡献。

每个方向只做一次变换，时间、空间 O(N)，不需要邻接表或模逆。相关字段放在
一个 32 字节节点里；和为普通余数，边系数为 Montgomery 编码，混合乘加一次规约。

16 个官方测例通过 GCC/Clang；全部根的逐路径函数复合、零斜率、小模数和任意
剥叶根通过独立对照及 ASan/UBSan。

## 尝试过程与取舍

将所有根的路径函数和归结为两遍换根 DP。用度数和邻居异或剥叶并缓存孩子贡献，再逆向恢复另一侧贡献；节点压紧，和保持普通余数、斜率用 Montgomery 表示。允许零斜率，不以除法撤销贡献，首批完整比较达标。
