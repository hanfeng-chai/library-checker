# convolution_crt_fast

`convolution_crt_fast<P>(a,b)` 与 `convolution_crt<P>` 使用相同的三个素数和精确
CRT 重建，逐素数的卷积改用 `convolution_fast`。输入不变，返回普通余数 Buffer。
要求 `1<P<2^30`、输入小于 P、补齐长度不超过 2^24，且整数系数上界小于
998244353×1004535809×469762049。P 不必是素数。

本接口用于目标模数不适合直接 NTT 的情况，例如 10^9+7。模 998244353
直接调用单模数 `convolution` 或 `convolution_fast`，不经过 CRT。

在包含头文件前定义 `TOY_NTT_ASM 0`，就由编译器根据 C++ intrinsics 生成内核；
默认使用保留的 AVX2 汇编。重建方式、读写模板和算法量级不变。
短边不超过 16 时直接相乘。接口与 [旧版](convolution_crt.md) 并存，便于比较。

`TOY_CRT_FUSED_NTT=0` 可进一步选择旧单模数 NTT；默认仍使用融合内核。
各翻译单元保持宏设置一致。
