# 十六进制大整数加法

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 14.793 (Rohan_Kapri), sum 152.458 (Rohan_Kapri)。

- `main.cxx` max: 11.435 ms (-22.70%), sum: 91.092 ms (-40.25%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
符号与绝对值分开保存；同号相加，异号比较绝对值后相减，零统一输出 0。
两数都不超过 31 位（不计符号）时使用 i128，相加仍不会溢出。
长数以 2^64 为一肢，每 16 个字符解析为一个机器字，用进位/借位指令加减；
复用缓冲，输出大写十六进制。

实现、边界约定与验证见 [big_integer](../../../include/toy/big_integer.md)。

## 尝试过程与取舍

十六进制天然对应二进制机器字，采用每 16 个字符一个 u64 肢，使用机器进位/借位指令。很短的带符号输入直接放入 i128。首批已达标，随后随共享解析、输出和大整数接口做回归，未为加法引入 FFT 或 GMP 依赖。
