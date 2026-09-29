# io

已整理的接口在 `include/toy/io.h`，标准头文件和类型别名集中在 `common.h`。
面向 x86-64-v3、Linux、GCC/Clang，使用当前 `cxx_flags.txt`，无需链接 libstdc++。
旧 `io.hpp` 暂供尚未迁移的 `.cpp` 使用；已整理的 `.h` 统一由 `common.h` 引入系统头文件。

```cpp
#include <toy/io.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    u64 a = in.read<u64>();
    i128 b = in.read<i128>();
    u32 index = in.read<u32, 6>();  // 可选：绝对值最多 6 位，符号不计
    out.write(a);                 // 默认附加换行；第二个参数可改成 ' '
    out.write(b);
    u64 values[] = {a, index};
    out.write(span(values));      // 批量整数输出
}
```

输入约定：stdin 是普通文件，十进制整数合法且可放进目标类型，没有前导零或 `+`，
token 之间恰好一个空格/换行，最后也有分隔符。每进程一个 Reader，按题目给出的数量读取。
`token()` 返回映射中的字符串视图。输出使用局部缓冲区，Writer 析构时刷新。
竞赛输入不做错误恢复；文件尾额外映射一页，保证 SIMD 尾部读取安全。

位数提示只限定范围，不承诺分布。单个数字、短整数、完整 8/16 位块分别走简单路径；
128 位输入分成 16 位块。输出按 4 位分组，5–16 位结果用 SIMD 去掉前导零，
128 位结果用常数倒数乘法拆成 19 位组。SIMD、倒数常量和游标处理的注释就在实现旁边。

常见输入范围与新题覆盖见下文。

```sh
make gen-integer_checksum gen-many_aplusb gen-many_aplusb_128bit
make check-integer_checksum check-many_aplusb-main check-many_aplusb_128bit-main
```

GCC、Clang 均通过三题的 85 项检查，生产二进制使用 `-nostdlib++` 链接。
额外以 Python 整数核对完整 i64/u64/i128/u128 范围的 400496 个读写值，
覆盖极值和十进制进位；小输出缓冲区的批量路径另用 ASan/UBSan 检查。

## Lenovo 实测（2026-09-29）

先完成所有准备并通过 `bench_env.py`，之后单个 SSH 任务串行测量，
期间没有额外 SSH、编译、传输或查询。不绑核、不预热；本次优化实验每个非 example
测例交错运行三次，先取各测例 task-clock 中位数，再求 max、sum。
生产 `bench.py` 仍然只跑一次。方法和使用约定见 [benchmark.md](../tools/benchmark.md)。

单位 ms；“下载最佳”只指当前仓库下载的五份提交，不代表实时榜单。

| 题目 | 新版 max | 下载最佳 max | 新版 sum | 下载最佳 sum |
| --- | ---: | ---: | ---: | ---: |
| many_aplusb | 41.544 | 41.290（nandhagk） | 203.757 | 215.258（nandhagk） |
| many_aplusb_128bit | 54.323 | 59.701（Rohan_Kapri） | 376.784 | 410.745（learningstud） |

64 位总时间减少约 5.3%，最慢测例仍慢约 0.6%，尚不能称为全面超过。
128 位 max 减少约 9.0%，sum 减少约 8.3%；个别均匀分布测例仍是 learningstud 更快。
旧库对应的 max/sum 为 49.502/238.562 和 59.226/410.803 ms。

完整对照批次中，89 组超过 50 ms 的测量，相对标准差中位数约 0.03%，最大约 0.12%。
每组仅三次，这是本轮观测，不是精度保证；小于 1% 的差异仍应谨慎解读。
原始 perf 输出、二进制 SHA-256、配置和源码快照在本地
`bench/fastio-20260929/final/`，此目录按仓库约定不入 git。

校验和题的最终 `bounded` 版本另与旧库复测：22 个测例 sum 为 2048.687 / 2640.901 ms，
减少约 22.4%；max 为 259.399 / 328.447 ms。
其中 `[0,99]` 为 163.685 / 233.775 ms，`[0,5*10^5]` 为 191.607 / 249.967 ms，
`[0,10^9]` 为 93.783 / 107.929 ms。
完整 u64/i64 范围仍比旧库慢约 3.8%/4.5%，这部分保留为后续优化点。
该批次位于 `bench/fastio-20260929/checksum-final/`；两道 Many A+B 的二进制哈希
与完整对照批次一致。各批次的 `summary.txt` 可直接查看逐测例表格。

## 输入范围与校验和题

对 253 道原有题目做过一次源码统计，按题去重、负数按绝对值合并。
常见输入上界集中在模数 998244353、数组/下标范围 5e5，以及数值范围 1e9。

| 数值 | 参数中出现的题数 | 重复读取引用该参数的题数 |
| ---: | ---: | ---: |
| 998244353 | 87 | 77 |
| 500000 | 67 | 34 |
| 1000000000 | 66 | 64 |
| 200000 | 41 | 23 |
| 100000 | 37 | 12 |
| 1000000 | 25 | 7 |
| 1000000000000000000 | 18 | 11 |
| 524288 | 14 | 1 |
| 1000000000000 | 4 | 3 |

“重复读取”通过 verifier 的循环、批量读取方法和简单变量引用推断，
不是完整 C++ 语义分析。间接调用、运行时推导的界和字符串编码的 128 位整数可能漏计；
循环次数、数组长度也可能被计入。它用来定位常见范围，不代表实际输入 token 的占比或分布。
操作类型等写在代码中的小常数需要另外考虑。

例如 `static_range_sum` 包含最多 500000 个 `[0, 10^9]` 的值，
以及最多 500000 对端点 `0 <= l < r <= N`：应同时覆盖 `10^9` 和 `5*10^5`。
模 998244353 的系数实际取值通常是 `[0, 998244352]`。
即使生成器随机，端点经过排序、取值经过筛选后也未必仍然均匀。

`many_aplusb` 的 `digit_random` 先均匀选择位数，再选该位数范围内的值，
一对 A、B 共用位数；128 位版本分别选择两者的位数和符号。它们不是高斯分布。

读入 N、L、R 和 N 个整数，输出和模 2^64。L、R 仅承诺范围，不承诺分布。
输出只有一个数字，主要测读取；两道 `many_aplusb` 继续测完整读写。
16 组均匀数据每组最多约 128 MiB，使大多数读取测试超过 50 ms：

| uniform 编号 | 闭区间 |
| --- | --- |
| 00–03 | [0,1]、[0,9]、[0,99]、[0,1000] |
| 04–07 | [0,10^5]、[0,2*10^5]、[0,5*10^5]、[0,10^6] |
| 08–11 | [0,998244352]、[0,10^9]、[0,10^12]、[0,10^18] |
| 12–13 | [-10^9,10^9]、[-10^18,10^18] |
| 14–15 | 完整 u64、完整 i64 |

边界数据另含十进制位数切换、正负极值、零，以及末尾恰好落在 4096 字节页边界的输入。
独立标程使用标量 fread 解析，校验和自然溢出；不使用 toy 库。

```sh
make gen-integer_checksum
make check-integer_checksum
make bench-integer_checksum
```

## 展开提交

`tools/bundle.py` 递归展开 toy、ACL 和项目内的相对 include，保留注释和系统头文件。
它做文本展开，不解释宏 include 或条件编译；当前库采用直接包含、单次定义的头文件。

```sh
make bundle-sample-many_aplusb-main  # 单个解答
make bundle-sample-many_aplusb       # 整题
make bundle-sample                   # 整个分类
make bundle                          # 全部已整理的 .cxx
# 也支持短名：make bundle-many_aplusb-main
```

产物为 `bundle/sample/many_aplusb/main.cxx`，可以作为单文件提交。
Makefile 记录递归头文件依赖，源码、相关头文件或展开脚本变化后才重新生成。
也可直接运行 `python3 tools/bundle.py src/sample/many_aplusb/main.cxx -o bundle/main.cxx`。
历史 benchmark 快照中的 `fastio` 是当前 `main` 解答更名前的名称。
