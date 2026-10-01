# 十进制大整数加法

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 15.561 (Anonymous), sum 162.339 (Anonymous)。

- `main.cxx` max: 15.438 ms (-0.80%), sum: 109.115 ms (-32.79%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
符号与绝对值分开保存；同号相加，异号比较绝对值后相减，零统一输出 0。
两数都不超过 18 位（不计符号）时直接解码为 i64 相加。
长数以 10^8 为一肢，使用 SIMD 解析及八肢进位/借位前缀，复用输入和结果缓冲。

实现、边界约定与验证见 [big_integer](../../../include/toy/big_integer.md)。

## 尝试过程与取舍

先统一带符号解析、零和输出格式，再把十进制按 10^8 分肢。进位/借位由 SIMD 前缀处理，输入一次转换 32 个字符，短整数保留机器字路径。后续六题整体验证、快速版本和最终复核均保留在表中，避免只引用最早一批的较小时间。
