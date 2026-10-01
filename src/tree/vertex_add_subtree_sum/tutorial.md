# 单点加、子树和

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 71.255 (Rohan_Kapri), sum 562.973 (Rohan_Kapri)。

- `main.cxx` max: 65.888 ms (-7.53%), sum: 515.255 ms (-8.48%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
题目保证 parent[v]<v，preorder.h 因而能用两遍顺序扫描求出连续子树区间，
无需邻接表。`main.cxx` 先将所有操作的顶点编号映射成位置或区间端点，再释放
几何表，按原顺序操作 WideFenwick；查询答案暂存后批量输出。

操作记录只占八字节，在线循环不再随机访问顶点到区间的映射。Fenwick 区间和
到共同高位后停止，减少相互抵消的读取。构造 O(N)，单次操作 O(log N)。
16 个官方测例通过 GCC/Clang，子树区间及完整 u64 环运算的独立对照与 sanitizer 通过。

## 尝试过程与取舍

利用父亲编号在前的输入直接构造连续子树区间，转为点加、区间和。共享 Fenwick 的抵消优化本身未让本题达标，之后预先解析操作位置、释放建树数组并批量输出，才同时降低 max 和 sum。
