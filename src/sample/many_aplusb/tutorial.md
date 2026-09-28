# Many A + B

## Problem / 题目

For up to $10^6$ test cases, read unsigned 64-bit-range integers $A,B$ and output $A+B$ for every case.

最多有 $10^6$ 组数据，每组读入位于无符号64位范围内的整数 $A,B$，输出 $A+B$。

The arithmetic remains trivial, but tens of millions of decimal bytes make parsing and formatting the real algorithm-engineering problem.

计算仍然简单，但数千万字节十进制文本使解析和格式化成为真正的算法工程问题。

## Baseline Algorithm / 基础算法

Repeat $T$ times:

1. parse $A$ and $B$;
2. compute their 64-bit sum;
3. format the sum followed by whitespace.

重复 $T$ 次：解析两个数、做64位加法、输出和及分隔空白。

## Why iostream/stdio Are Slow Here / 为什么标准I/O较慢

Formatted standard I/O offers locale handling, stream state, generic format parsing, locking, and frequent abstraction boundaries. Those features are valuable in applications, but this input grammar is only:

```text
ASCII digits + spaces/newlines
```

A specialized parser can reduce the work to pointer traversal and integer arithmetic. A specialized printer can buffer all output and avoid generic format parsing.

标准格式化I/O提供locale、流状态、通用格式解析、锁等能力，适合普通应用；但本题语法只有ASCII数字与空白。专项解析器可退化为指针遍历和整数运算，专项输出器可集中缓冲并绕开通用格式解析。

## Header-only Backend / Header-only后端

`toy/io.hpp` separates token-shape policy from input/output mechanics. It is
intentionally tuned for Linux x86-64 and GCC.

### Contiguous input

- The official and OJ paths provide stdin as a regular file.
- `direct_mapping` uses `fstat` + one read-only `mmap`.
- The default Reader additionally reserves a zero guard page for general
  local use; the measured submission path avoids that setup.

This avoids per-character virtual calls. Benchmarks and fuzzing feed ordinary
files, matching the direct-mapping contract.

官方与 OJ 的 stdin 是普通文件；`direct_mapping` 只做一次只读 mmap。默认
Reader 另行保留零值 guard page，适合一般本地使用，提交性能路径则省去这层
初始化。benchmark 与 fuzz 都使用普通文件以满足契约。

### Buffered output

- Accumulate output in a large heap buffer.
- Format two digits per division using a 100-entry table.
- Flush with a robust `write` loop that handles partial writes and `EINTR`.

输出写入大块堆缓冲；用两位数字表减少除法次数；最终通过处理短写和 `EINTR` 的 `write` 循环刷新。

This portable fast variant is roughly 5.5 times faster than iostream on the initial all-max benchmark.

初始all-max benchmark中，可移植快速版约为iostream的5.5倍。

## SIMD Decimal Backend / SIMD十进制后端

### Parsing 16 digits

Load 16 bytes and subtract ASCII `'0'`. For digits, byte values become $0\ldots9$; separators become negative bytes. A movemask locates the first separator.

For a full 16-digit block, reduce in stages:

1. `_mm_maddubs_epi16` combines adjacent digits with weights $(10,1)$ into 8 two-digit values;
2. `_mm_madd_epi16` combines them with $(100,1)$ into 4 four-digit values;
3. multiply/add combines pairs with $10^4$ into 2 eight-digit values;
4. scalar combine with $10^8$ yields the 16-digit integer.

For shorter numbers, a `pshufb` mask right-aligns digits with zero fill before the same reduction. For 17–19 digit values, parse the first 16 digits then handle the tail scalarly.

加载16字节并减去 `'0'`。数字变成0到9，分隔符变成负字节，movemask可定位首个分隔符。随后通过 $(10,1)$、$(100,1)$、$10^4$、$10^8$ 四层归约，把16位十进制数转成整数。短数字用 `pshufb` 补零对齐，17–19位数字只需标量处理尾部。

### Four-digit output groups

Precompute all strings `0000` to `9999`. Split each result into base-$10^4$ groups, print the highest group without leading zeros, then memcpy fixed four-byte groups. A megabyte buffer amortizes syscalls.

预计算 `0000` 到 `9999`，把结果拆成 $10^4$ 进制组。最高组去前导零，其余组直接拷贝4字节LUT；大缓冲摊薄系统调用。

## Correctness / 正确性

Every parser path implements the same positional identity
$$d_0d_1\dots d_{r-1}=\sum_{i=0}^{r-1}d_i10^{r-1-i}.$$
SIMD only reassociates this exact integer computation into groups; no floating point is used. The formatter performs the inverse base-$10^4$ decomposition. Every variant passes all nine official generated cases, including zeros, all maxima, random lengths, and one million cases.

所有解析路径都严格实现十进制位权公式。SIMD只是对精确整数运算重新分组，不使用浮点；格式化执行逆向的 $10^4$ 分解。所有变体均通过9组官方数据，包括零、最大值、随机位数及百万组输入。

## Distribution-aware policy / 分布感知策略

The shared Reader distinguishes three contracts:

- `read_fixed<N,T>()`: every magnitude has exactly $N$ digits;
- `read_var<N,T>()`: digit counts are mixed;
- `read_uniform<N,T>()`: values are approximately uniform in a bounded range.

An exhaustive signed/unsigned 1–37 digit matrix selects scalar, SWAR, SSE,
staged-16, or AVX2-32 parsing at measured crossover points. This matters for
future tasks: a uniformly distributed 10-digit bound is not equivalent to
uniformly random digit counts up to 10.

共享Reader区分固定宽度、位数混合和有界数值均匀三种契约，并通过有符号/无符号1至37位完整矩阵选择解析后端。

## Performance Findings / 性能结论

Initial host, `all_max_00.in`:

- iostream: about 367 ms median;
- stdio: about 417 ms;
- portable fast: about 67 ms;
- AVX2: about 66 ms.

The OJ bottleneck is `digit_random`, not the largest `all_max` file.
`blocked_shape_io.cpp` parses and computes blocks before formatting, uses packed
base-$10^4$ groups, and emits whitespace-separated tokens without one newline
per answer. It reports 25 ms online versus the 24 ms public leader, while
beating that leader source on every official case under the same prepared
local build.

OJ瓶颈是 `digit_random` 而不是最大的 `all_max`。最终版本在本地同编译参数下所有官方用例均不弱于榜首源码，线上为25 ms，接近24 ms榜首。

## Complexity / 复杂度

The solution processes each input/output byte a constant number of times:
$O(\text{input bytes}+\text{output bytes})$ time and bounded I/O buffers.

解法对每个输入输出字节只做常数次工作，时间为
$O(输入字节+输出字节)$；额外空间为固定分块数组和有界输出缓冲。
