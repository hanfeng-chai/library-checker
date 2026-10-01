# 大规模卷积

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 1081.079 (Aiyiyi), sum 22929.496 (Aiyiyi)。

- `main.cxx` max: 1046.740 ms (-3.18%), sum: 22352.351 ms (-2.52%)
- `naive.cxx` max: 1123.328 ms (+3.91%), sum: 24060.535 ms (+4.93%)
- `naive.cxx [Clang 23]` max: 1161.559 ms (+7.44%), sum: 24889.773 ms (+8.55%)
- `ntt.cxx` max: 1216.117 ms (+12.49%), sum: 26070.294 ms (+13.70%)
- `ntt.cxx [Clang 23]` max: 1202.583 ms (+11.24%), sum: 25794.685 ms (+12.50%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
本题同样模 998244353，只比较单模数 NTT；没有 FFT 或多模数 CRT 对照。
部分 NTT 在八项多项式处停止，补齐长度 N 只需 N/8 阶单位根，覆盖本题数据。

| 文件 | 实现 |
|---|---|
| main.cxx | 新版缓存融合 NTT，关键循环使用 AVX2 汇编 |
| ntt.cxx | 同算法的 C++ intrinsic 版本，无手写汇编 |
| naive.cxx | 原有单模数 NTT 内核 |

三个版本使用相同的最新批量 I/O。新版顶层利用零区，省去会被覆盖的补零，
并将归一化并入末级逆变换。旧内核完整保留，方便阅读和对照。
细节见 [convolution_fast.md](../../../include/toy/convolution_fast.md)。

Clang 23 的同源对照在 bench.txt 中标为 `ntt-clang23`，采用宿主机 sysroot，
没有混入容器的新 libc/libm。此前已测过的 main 和用户提交不重复计时。
