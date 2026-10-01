# convolution_fast

`convolution_fast<P>(Buffer<u32> a, Buffer<u32> b)` 消费两个普通余数数组，
返回模 P 的卷积。P 是小于 2^30 且 `8 | P-1` 的素数；补齐长度 N 所需的
N/8 阶单位根必须存在。短边不超过 16 时复用 `convolution` 的直接乘法。

```cpp
usize size = std::bit_ceil(n + m - 1);
auto a = ntt_storage<u32>(n, size + 16), b = ntt_storage<u32>(m, size + 16);
// 填入 [0,P) 的系数。
auto c = convolution_fast(std::move(a), std::move(b));
```

`ntt_storage<T>(size, capacity)` 返回未初始化的 Buffer，大块存储按 2 MiB 对齐并
请求透明大页。提前预留容量可避免输入复制；内核自行补零和预留 SIMD 读取的余量。

算法仍为 O(N log N)、O(N) 空间的 radix-4 部分 NTT，在八项多项式处停止。
相对 `convolution.h`，改变的是整次乘法的组织方式：

- 按缓存子树完成两次正变换、小卷积和逆变换，避免三遍独立扫过大数组。
- 根表按八个根、八个 Shoup 商排列，SIMD 批量生成；叶子乘积用 Montgomery 归约。
- 当前叶子相乘时准备下一组叶子的窗口；窗口中的回绕项已经乘上模 `x^8-w` 的 w。
- 顶层利用已知为零的上半区，省去无用蝶形与会被覆盖的补零；归一化并入最后一级。

缓存调度和汇编内核改编自本仓库 Aiyiyi 的两份模卷积提交。只保留实际使用的
AVX2 蝶形及融合叶子循环；首尾处理用 C++，汇编从约 1600 行减到约 1000 行。
布局、范围约束见源码；汇编集中在 [`ntt_asm.h`](ntt_asm.h)。这仍明显比纯 C++
实现复杂，是有实测收益的性能取舍，不应把行数减少解释成算法复杂度下降。

纯 C++ 的缓存融合、批量根表和 SIMD I/O 候选已验证，但未同时满足两题的 max/sum
要求。当前接口适合一次完整乘法，可用于其他素数卷积、CRT、多项式和大整数乘法；
需要保存或复用频谱时，继续使用 `convolution_detail::info<P>`，不能直接换成融合接口。

## 编译器生成的版本

在包含任何相关头文件前写 `#define TOY_NTT_ASM 0`，可关闭手写汇编和空汇编
屏障，改由编译器生成全部指令；C++ intrinsics、缓存调度和根表布局仍然保留。
同一程序的各翻译单元应保持此设置一致。默认值为 1，默认主解的产物没有变化。

当前实际使用这套汇编的是 `convolution_mod`、`convolution_mod_large`、
`convolution_mod_2_64` 的 main，以及新增的模 10^9+7 的 `crt.cxx` 对照。
998 两题只保留单模数 NTT 的新版、无汇编版和旧版，不保留 FFT 或 CRT 对照。

GCC 16、Clang 23 的无手写汇编版本均通过正式数据；Clang 23 略快，但本次
三个题目的两项汇总没有全部达到参考门槛。汇编不是正确性条件，保留它是当前
性能取舍。Clang 23 使用宿主机 sysroot，保持 libc/libm 与 GCC 一致，命令见
[benchmark.md](../../tools/benchmark.md)。

最终数值见两题的 [普通卷积](../../src/convolution/convolution_mod/bench.txt) 和
[大卷积](../../src/convolution/convolution_mod_large/bench.txt)。逐程序两轮、逐测例取
较小 task-clock；未变化的参考复用已有记录，完整候选与源码快照在
`bench/conv-revisit-20261001/`。

统一新 I/O 后，普通 998 卷积的旧版 NTT 已能双项胜过参考；u64 的
“旧 NTT＋新 CRT”GCC 组合也在本轮达标。大规模 998 卷积目前仍由汇编主解
满足门槛。是否保留汇编应按题目判断，而不是认为 C++ 在原理上不能生成快代码。
