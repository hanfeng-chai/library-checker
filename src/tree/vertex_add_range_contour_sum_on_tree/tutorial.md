# 点加与距离区间和

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 293.341 (nandhagk), sum 2938.517 (nandhagk)。

- `main.cxx` max: 259.703 ms (-11.47%), sum: 2468.585 ms (-15.99%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 centroid_sum.h。一般树做点分解，少分支重心避免保存重复的
全体直方图，底部小树用距离矩阵批量筛选。纯链化为线性区间，星形树直接按
距离 0、1、2 求和。

36 个正式测例通过 GCC/Clang，独立 BFS 对照通过 ASan/UBSan。

## 尝试过程与取舍

首版为每层重心同时保存全体与本支距离 Fenwick，查询做相减。先省去一、二分支重心的全体重复表，再将至多 32 点的小树交给字节距离矩阵与 SIMD。仍被纯链拖高 max，于是链用一维区间、星形按距离直接计算。最后短距离表直接存数组，初始/修改值按十位读取，并用连续记录加一次散写替代链式路径追寻，减少预处理随机访存。
