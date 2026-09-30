# Convolution

把两列数视为多项式系数，计算乘积并模 998244353。
主解预留 `bit_ceil(N+M-1)` 容量，读入后把存储直接交给 `toy::convolution`，
省去一次分配和输入复制。系数最多九位，N、M 最多六位。

长数组使用缓存分块的 radix-4 变换和融合 8 点叶子；短边不超过 16 时直接计算。
实现与测量见 [convolution.md](../../../docs/convolution.md)。

```sh
make gen-convolution_mod
make check-convolution_mod-main
make bundle-convolution-convolution_mod-main
```
