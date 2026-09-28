# Many A+B：从十进制文本到机器整数，再回到文本

## 1. 题目是什么，为什么这次需要研究 I/O

输入 T 组非负整数 A、B，逐组输出 A+B。
[官方参数](../../../upstream/sample/many_aplusb/info.toml)为 T≤1000000、
A,B≤10¹⁸，所以和≤2×10¹⁸，可以放入有符号或无符号 64 位整数。
没有排序、取模、大整数进位数组，也没有跨组依赖。

~~~text
输入：                          输出：
3                              42
12 30                          0
0 0                            1000000000000000001
1000000000000000000 1
~~~

[上游 correct.cpp](../../../upstream/sample/many_aplusb/sol/correct.cpp)
用 scanf 读 T，然后逐组读两个 long long、做一次加法、printf 输出。
它在数学上已经是最直接的线性算法。快解主要研究的工作量来自文本：
每个最多 19 位的数需要从十几个字符转成一个机器整数，答案再转回字符；
百万组意味着数千万字节的数字转换，而每组只有一次机器字加法。
若 Din、Dout 分别为输入、输出字节数，所有这些方案的时间都是
O(Din+Dout)；定长机器整数约束下也可写为 O(T)。

## 2. 基线解析器做了什么，怎样减少依赖链

逐位扫描 12345678 时，维护 x=10x+d，产生
0→1→12→123→…→12345678 的串行依赖。scanf 还要解释格式串，
处理通用的状态和语法。标准 I/O 自身通常已经缓冲，
所以不能说每次 scanf 都一定触发系统调用；改进的是整个格式处理路径。

同一个数字可以按位权分组：

~~~text
数字：       1  2  3  4  5  6  7  8
两位组：      12    34    56    78       10×左 + 右
四位组：        1234        5678         100×左 + 右
八位组：             12345678            10000×左 + 右
~~~

每层中的各组互不依赖，可以并行算。16 位数先归约成两个八位组，
再用 high×10⁸+low 合并。SIMD 指令把字符减去 '0'，依次用
(10,1)、(100,1) 等权重合并邻近槽位；maddubs、madd 对应的正是这些步骤。
SWAR 则将几个字节放进普通 64 位字，用掩码、乘法和移位完成类似归约。
两者都精确保持十进制位权，没有浮点近似。

## 3. 变长 token、映射输入与读取边界

加载 16 字节后，在本题“数字与空白”的语法下，减去 '0' 所得字节的符号位
可以区别数字与空格/换行。movemask 把符号位集合成整数；
最低置位的位置告诉我们首个分隔符在哪里。比如 "123 45…" 的当前数长 3，
shuffle 将这三位放到归约所需的位置，其他槽填零，复用相同内核。
首 16 字节全是数字时，则归约它们，再处理余下至多三位。

这是针对有效题目输入的扫描器，不能当作任意字符串合法性的完整检查。
“逻辑上只消费三个字符”也不代表机器只读取三字节；
SIMD 实际可能加载十六字节，末尾必须预留可读填充或提供尾部回退。

mmap 把普通输入文件映射成可连续访问的内存，省去部分自建缓冲的拷贝，
仍有页映射和页面调入成本；管道不一定能走同一路径。
当前各实现对文件、管道、额外可读页的处理不同，
复制解析表达式时必须同时理解其存储契约。

## 4. 输出的逆过程：四位查表与成对格式化

逐位取 x%10、x/=10 会形成另一条依赖链，还需要翻转逆序数字。
以 10000 为基数分组可一次输出四位。例如：

~~~text
123000045 = 1×10000² + 2300×10000 + 45
输出："1" + "2300" + "0045"
~~~

低组 45 必须补成 0045，否则丢失位权。预计算 0000…9999 的四位表，
每组拷贝四字节即可；一张表约 40 KB，减少格式化操作也增加表访问。
最高组不能补前导零，可以按长度裁掉，或把多余位改为空格兼作分隔符。
零需要单独保留字符 '0'，不能被最高组的去零过程变成空 token。

本题 [checker](../../../upstream/sample/many_aplusb/checker.cpp)
按空白分隔后比较字符串，因此空格/换行可互换，数字文本必须一致；
“读成整数相同”还不足以允许随意加前导零。
第一名还把两个结果放入 AVX2 的两个半区，并行生成十进制尾段，
再按各自长度写出；小数和长数的组合有各自的分流。

## 5. 为什么先读算一批，再输出一批

~~~text
while 还有数据:
    count = min(批次大小, 剩余组数)
    连续解析 count 对数，把 A+B 放进 sums
    连续格式化 sums，写入输出缓冲
~~~

这个安排把两套较长的解析/格式化路径分别集中，便于展开、调度和缓存复用。
它没有少读字符，也没有减少加法次数，还多了 sums 数组的写入与读回。
批次太大可能挤占缓存，所以前五出现 4、8、2048、16384 等不同选择，
属于需要实际测量的取舍。

## 6. 当前前五逐份怎样组合这些技巧

以下来自 2026-09-26 05:10 UTC 的
[config](../../selected/many_aplusb/config.json) 和 [global.json](global.json)。
五条均为当时最新数据版本的 AC，按不同用户去重。
本轮 62 题原始前 20 已完成重测收敛，不表示这五个 ID 都由本轮主动重测，
也不覆盖更深排名的所有 I/O 方案。

| 排名 | 提交、用户 | OJ 时间 | 主要路径 |
|---:|---|---:|---|
| 1 | [402646.cpp](../../selected/many_aplusb/402646.cpp)，nandhagk | 23 ms | 缓存分隔符位置、成对 AVX2 解码/格式化、8 组分块 |
| 2 | [402692.rs](../../selected/many_aplusb/402692.rs)，toomer | 24 ms | 4 组分块，按编译特性选择 SIMD/SWAR，四位组输出 |
| 3 | [392126.cpp](../../selected/many_aplusb/392126.cpp)，QgQ | 24 ms | 16 字节解析、2048 组分块、前导空格输出组 |
| 4 | [393713.cpp](../../selected/many_aplusb/393713.cpp)，chaihf | 25 ms | toy Reader/Writer、2048 组分块、有界 token 输出 |
| 5 | [356719.cpp](../../selected/many_aplusb/356719.cpp)，Anonymous | 25 ms | 16 字节解析、16384 组分块、两张四位表 |

### #402646：一对数共享归约和格式化指令

main 使用 padded_file、delimited_reader、u64_shuffle_avx2_codec。
reader 缓存 32 字节窗口的分隔符位图；窗口里还有两个分隔符时，
read_pair 一次得到两个 token 的起点/长度，跨窗口则回退 next_field。
两个数字分别放进 256 位向量的两个半区，一起做分层归约。
两个一位数直接返回；两数都很长时走已有长字段路径。

每批计算 8 个和，再由 u64_paired_avx2_formatter 两两格式化。
两个数都小于 10000 时用紧凑小数路径；两个都至少 10¹⁶ 时走大数路径；
其余组合并行生成各自的十六位尾段，配合最高组和实际长度写出。
它的优势方向是分隔符扫描复用、成对转换和按 token 形状分流，
相应需要输入填充和输出额外可写空间。

### #402692：四组小批次，编译期选择读取内核

solve 使用 prepare!(fast) 建 FastInput/FastOutput，以 step_by(4) 分批。
在 x86-64 且编译时启用 SSSE3 时，u64 入口走 16 字节 SIMD；
否则使用八字节分组的 SWAR。源码包含两条路径，
config 没有完整编译参数，不能只凭快照断言已独立观测到哪条 CPU 分支。

FastInput 为合适文件建立额外可读页，不适合时整份读入并补空白；
FastOutput 使用 256 KiB 缓冲与四位数字表。这里只暂存四个和，
因此分块与额外访存的权衡和 2048/16384 组版本不同。

### #392126：精简的裸游标路径与 16 KiB 中间块

read_u64 先加载十六字节，短 token 用非数字掩码与 shuffle，
长 token 的尾部逐位处理。FASTIO_UNSAFE_BLOCK_LOG 默认为 11，
所以 sums 有 2048 个 uint64，占 16 KiB。
输出使用最高组前导空格、后续组补零的四字节表，
刷新缓冲处理短写和 EINTR。trusted scanner 的语法与范围来自有效输入，
没有承担通用解析器的全部检查。

### #393713：本仓库历史 bundle 的实际调用路径

main 的 read<u64>() 在该版本进入 read_simd_u64，虽打包了大量其他接口，
128 位和固定长度解析并未参与本题。每批 2048 个和，然后调用
write_token_bounded<2000000000000000000>，其上界与题目和的范围一致，
可省去处理更宽 uint64 数值的分支。

当前 [blocked_shape_io.cpp](../../../problems/sample/many_aplusb/blocked_shape_io.cpp)
已显式使用 direct_mapping、read_var<19,u64> 等形状接口；
它与保存的历史 bundle 不是逐字节相同的版本，比较时要保留哈希和编译参数。

### #356719：直接归约链与更大的分块

parse_16_digits 明确按两位、四位、八位、十六位归约，
SHUFFLE_MASKS 将短 token 补零对齐。每批 16384 个和，占 128 KiB；
LUT1 的最高组用空格替代前导零，LUT2 输出后续补零组，
值零另写包含 '0' 的组。它与另几份保持相同位权恒等式，
区别集中在批次规模、指针路径和缓冲组织。

## 7. 怎样验证每种优化，而不是只比较榜单

先固定输出器比较标量/SWAR/SIMD 解析，再固定输入器比较单个/成对格式化，
最后只改批次大小。覆盖零、1/16/19 位数、一长一短、跨窗口与页边界、
最大 T，并用相同编译器、CPU 亲和性进行多次交错测量。

均匀抽一个 [0,10¹⁸] 数，通常会得到长 token；先随机选位数再生成数字，
短 token 会多得多。二者对分支和归约路径的压力不同，
因此最大的输入文件未必是程序最慢的测试组。
本页核对了当前源码调用链与教学例子，没有新增性能 benchmark，
23–25 ms 不能直接拆成每项优化独立贡献。

## 8. 2026-08-15 阶段的历史实现记录

以下保留旧阶段的英文记录；其中 24 ms “leader”指当时比较的基线，
不是本次快照的 23 ms #402646。详细历史实验见
[本地教程](../../../problems/sample/many_aplusb/tutorial.md)。

Fetched sets:

- [global.json](global.json): fastest submission from each public user;
- [chaihf.json](chaihf.json): five fastest personal submissions.

After the 2026-08-15 iteration, the global C++ leader is `24 ms` and the
selected `chaihf` submission is `25 ms`. On the prepared local host, the
selected implementation is faster than that leader source on every official
case under the same `-O3 -flto -fno-exceptions -fno-rtti -march=native` build.

## Recurring Techniques

Across representative fast sources:

- map regular stdin with `mmap`, often after `fstat`, and walk it with raw pointers;
- keep a buffered-read fallback for non-regular stdin in more reusable libraries;
- parse fixed-width digit chunks instead of multiplying by ten for every character;
- use SWAR or AVX2 `maddubs`/`madd` reduction from 16 ASCII digits to integer groups;
- buffer output and emit four decimal digits at a time from a 10,000-entry lookup table;
- use whitespace (including leading spaces in a four-byte group) as the token separator;
- batch computation/output to keep the hot parser and formatter loops compact;
- separate portable and CPU-specific paths rather than hiding AVX2 requirements.

The fastest sources also show why source size is not a quality metric: submissions range from a few kilobytes of focused I/O to large general-purpose personal libraries with similar online times.

## Adopted Design

- 16-byte SIMD variable-length parsing;
- block parse/add followed by block formatting;
- base-$10^4$ packed output groups;
- a bounded token-output API that proves the result range at compile time and
  removes one hot branch per answer;
- whitespace-leading groups instead of a newline per answer.

The decisive official cases are `digit_random`, not the largest `all_max`
file. OJ timings have millisecond-sized single-run noise, so local interleaved
perf measurements are the acceptance criterion once the result is close to
the public leader.

Raw sources are cached only for research and are not vendored into the library.
