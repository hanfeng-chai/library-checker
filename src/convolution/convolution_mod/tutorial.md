# Convolution

<!-- benchmark-summary -->
2026-10-01 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 35.559 (Aiyiyi), sum 780.377 (Aiyiyi)。

- `main.cxx` max: 41.091 ms (+15.56%), sum: 927.157 ms (+18.81%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
把两列数视为多项式系数，计算乘积并模 998244353。
主解预留 `bit_ceil(N+M-1)` 容量，读入后把存储直接交给 `toy::convolution`，
省去一次分配和输入复制。系数最多九位，N、M 最多六位。

长数组使用缓存分块的 radix-4 变换和融合 8 点叶子；短边不超过 16 时直接计算。
实现与测量见 [convolution.md](../../../include/toy/convolution.md)。

```sh
make gen-convolution_mod
make check-convolution_mod-main
make bundle-convolution-convolution_mod-main
```

## 尝试过程与取舍

先复用参考中的 radix-4 与融合八点叶子结构，整理为消费 Buffer 的通用卷积接口。主解按最终变换长度预留容量，读入后直接交给内核，避免重新分配、复制输入。短边不超过 16 时直接相乘。首轮即双项领先；后续其他数学题复用内核时保留本题作为回归对象。
