# A + B / 两数之和

## 中文

读入 `0<=A,B<=10^9`，输出 `A+B`。算法只有一次精确整数加法：

```text
answer = A + B
```

输入操作数可放入 32 位无符号整数，和最大为 `2*10^9`，这里提升到 `u64`
后相加。时间和额外空间均为 `O(1)`。

`integer_addition.cpp` 也承担仓库最小冒烟测试的角色：它包含公共
`toy/io.hpp`、bundle 为独立源码、编译并经过官方 checker。两个输入均是
十位上界内的非负数，使用 `read_uniform<10,u32>`；官方/OJ 输入是普通文件，
因此使用 direct mapping。

本题计算量过小，进程启动、动态链接、页映射和调度噪声远大于一次加法。
所以它属于用户允许的“小数据例外”，不以完整进程墙钟时间争夺算法榜首。
Library Checker 另有 function 提交模式，它更接近纯加法内核；本仓库的普通
可执行版本主要验证工具链和公共 I/O 调用是否正确。

## English

Read `0<=A,B<=10^9` and print their exact sum:

```text
answer = A + B
```

Promoting both operands to `u64` keeps the maximum `2*10^9` safely in range.
Time and auxiliary memory are `O(1)`.

`integer_addition.cpp` is also the repository's smallest pipeline smoke test:
it includes `toy/io.hpp`, bundles into a standalone source, compiles, and runs
through the official checker. Both operands use
`read_uniform<10,u32>` on the regular-file judge path with direct mapping.

Process startup, mapping, and scheduler noise dominate one addition, so this
is the explicit tiny-workload exception to leaderboard tuning. Library
Checker's function mode is better for measuring the pure arithmetic kernel;
the executable variant primarily validates build, bundle, test, I/O, and
submission plumbing.
