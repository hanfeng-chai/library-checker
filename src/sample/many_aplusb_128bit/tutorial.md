# Many A + B (128 bit)

## Problem / 题目

For up to $5\cdot10^5$ cases, read signed decimal integers in $[-10^{37},10^{37}]$ and output their sums.

最多有 $5\cdot10^5$ 组数据，每组读入区间 $[-10^{37},10^{37}]$ 内的有符号十进制整数并输出其和。

The values and sums fit signed `__int128`, but standard C++ streams and `printf` have no direct `__int128` format. Parsing and formatting are therefore part of the implementation.

数值与结果可放入有符号 `__int128`，但标准C++流和 `printf` 没有直接的 `__int128` 格式，因此解析和格式化本身就是解答的一部分。

## Arithmetic / 算术

Parse each operand exactly into `__int128_t`, add it, and format the exact result. The maximum magnitude is $2\cdot10^{37}$, below
$$2^{127}-1\approx1.70\cdot10^{38}.$$

把每个操作数精确解析为 `__int128_t`，相加后精确格式化。最大绝对值 $2\cdot10^{37}$ 小于有符号128位上界。

## Safe Signed Magnitudes / 安全处理符号

Negating the most negative signed integer directly can overflow. Convert through the unsigned representation:

```cpp
u128 magnitude = negative ? u128(0) - u128(value) : u128(value);
```

The same representation is used when parsing a negative token: parse the unsigned digits, then compute unsigned zero minus magnitude before converting to signed.

直接对最小负数取负会溢出。应通过无符号表示计算绝对值。解析负数时同样先得到无符号幅值，再用无符号0减去幅值，最后转换成有符号类型。

## Portable Variant / 可移植变体

The generic scanner accumulates digits into `__uint128_t`; the generic printer emits two digits at a time. This avoids streams and works without architecture-specific intrinsics, but output still executes repeated software 128-bit division.

通用Scanner把十进制位累积到 `__uint128_t`，通用Printer每次输出两位。它避开iostream且无需特定指令集，但格式化仍会反复执行软件128位除法。

## Staged 16-digit Parsing / 分阶段16位解析

`staged_i128.cpp` processes up to three 16-byte stages:

1. load 16 bytes and locate the first separator;
2. reduce a full block into one 16-digit value;
3. repeat only when the token is longer;
4. combine the final short block with a precomputed power of ten.

This avoids paying for a 32-byte load/reduction on the many short and
medium-length tokens in mixed official cases. The generic policy matrix still
selects the AVX2-32 backend for widths where it wins.

最终官方题策略按16字节分阶段读取，只在数字确实更长时继续下一块；通用矩阵仍会在适合的宽度选择32字节AVX2后端。

## Avoiding Repeated 128-bit Division / 消除重复128位除法

Naively printing 38 digits with `%10` invokes expensive compiler runtime helpers many times. Split magnitude at
$$B=10^{19}:\qquad x=qB+r.$$
Both $q$ and $r$ fit `uint64_t` for signed-128 magnitudes. Then:

- print $q$ normally;
- print $r$ as exactly 19 zero-padded digits.

Even one generic 128-bit division per output is costly at 500,000 cases. The tuned backend computes the quotient using reciprocal multiplication. Let
$$M=\left\lceil\frac{2^{192}}{10^{19}}\right\rceil=2^{128}+M'.$$
A 128-by-128 high-half multiplication with $M'$ plus one carry correction yields
$$q=\left\lfloor x/10^{19}\right\rfloor,$$
and then
$$r=x-q\cdot10^{19}.$$
This replaces a software division call with fixed 64-bit partial products and additions.

朴素逐位 `%10` 会反复调用昂贵的软件128位除法。先按 $10^{19}$ 拆分，使商和余数都落入64位。专项后端进一步用 $2^{192}/10^{19}$ 的定点倒数乘法求商：计算128×128乘法高半部并修正进位，再用 $r=x-q\cdot10^{19}$ 求余，从而消除软件除法调用。

## Grouped Output / 分组输出

The quotient is formatted in base $10^4$. The 19-digit remainder is emitted as:

- one zero-padded three-digit group;
- four zero-padded four-digit lookup groups.

This uses only constant-divisor 64-bit arithmetic and fixed-size copies into a large buffer.

商按 $10^4$ 分组；19位余数输出为1组三位数字和4组四位LUT，均补足位宽。这里只需常数除数的64位运算与定长内存拷贝。

## Correctness / 正确性

The SIMD parser is an exact reassociation of decimal positional arithmetic. Reciprocal division is used only with the proven fixed divisor $10^{19}$; the remainder is reconstructed from the quotient. All ten official cases pass, covering positive/negative extrema, carry propagation, zeros, random lengths, and 500,000 cases.

SIMD解析只是十进制位权运算的精确重结合。倒数乘法只用于固定且已证明的除数 $10^{19}$，余数由商反算。10组官方数据全部通过，覆盖正负极值、连续进位、零、随机长度和50万组输入。

## Performance / 性能

The final submission:

- passes all 10 official generated cases;
- compiles with about 557 MB peak RSS instead of the 1.13 GB version that was
  killed by the OJ compiler;
- reports 30 ms online, improving on the previous 31 ms public leader.

最终提交通过10组官方数据；编译峰值从约1.13 GB降至557 MB；线上30 ms，快于此前31 ms榜首。

## Complexity / 复杂度

The solution is linear in textual input/output size. Extra memory is bounded
by the fixed output buffer; the read-only input mapping is supplied by the
regular-file judge path.

解法时间与文本输入输出总长度线性相关。除只读输入映射与固定输出缓冲外只用
常数空间；direct mapping 的普通文件契约与官方测试和 OJ 一致。
