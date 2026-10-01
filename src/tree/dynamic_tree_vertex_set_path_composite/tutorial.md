# 动态树点赋值与路径复合

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 557.593 (Rohan_Kapri), sum 2535.094 (Rohan_Kapri)。

- `main.cxx` max: 520.233 ms (-6.70%), sum: 2366.506 ms (-6.65%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 link_cut_affine.h，Link-cut Tree 维护双向仿射函数聚合。
一个斜率积配两个方向的截距，支持零乘数，无需求逆。节点缓存孩子方向，
access 直接旋转接入；单点修改只做 splay，避免额外路径切换。

23 个正式测例通过 GCC/Clang，动态边替换与朴素路径复合的独立对拍通过
ASan/UBSan。

## 尝试过程与取舍

先实现保存双向函数复合的 Link-cut Tree，支持零斜率和奇合数模数。去掉祖先标记栈后仍未达标；随后缓存孩子方向，access 直接旋转接入，点修改只做 splay，减少昂贵的重复模乘聚合。该版本在完整比较中同时领先，后续命名空间限定保持产物一致。
