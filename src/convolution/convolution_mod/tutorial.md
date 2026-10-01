# Convolution

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 35.559 (Aiyiyi), sum 780.377 (Aiyiyi)。

- `main.cxx` max: 29.405 ms (-17.31%), sum: 692.496 ms (-11.26%)
- `naive.cxx` max: 31.348 ms (-11.84%), sum: 736.817 ms (-5.58%)
- `naive.cxx [Clang 23]` max: 32.415 ms (-8.84%), sum: 762.187 ms (-2.33%)
- `ntt.cxx` max: 33.968 ms (-4.48%), sum: 797.817 ms (+2.23%)
- `ntt.cxx [Clang 23]` max: 33.609 ms (-5.49%), sum: 791.777 ms (+1.46%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
本题模数是 998244353，直接使用单模数 NTT，不经过 CRT，也不保留 FFT 对照。
三份程序的数组读取、输出均使用相同的最新 `io_batch.h`。

| 文件 | 实现 |
|---|---|
| main.cxx | 缓存融合的部分 NTT，保留关键 AVX2 汇编 |
| ntt.cxx | 同一新版算法，关闭手写汇编，由编译器生成指令 |
| naive.cxx | 原有 `convolution.h` 的 C++ intrinsic 内核 |

算法量级都为 O(N log N)。旧版保留八项叶子；新版改变缓存调度、根表布局，
把两次正变换、叶子乘积和逆变换放在同一子树中完成。额外复杂性主要来自约
1000 行指令调度，而不是新的数学算法。说明见
[convolution_fast.md](../../../include/toy/convolution_fast.md)。

GCC 与 Clang 23 的 `ntt.cxx` 使用相同目标指令集和同一套 libc/libm。
Clang 对照在报告中标为 `ntt-clang23`，与默认 GCC 产物分开记录。
旧用户提交与未变化的 main 均复用已有两轮记录。
