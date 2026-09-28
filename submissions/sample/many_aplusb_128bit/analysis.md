# 128 位 Many A+B：分块解析与精确的定点倒数除法

## 1. 数有多大，是否需要大整数库

本题有至多 500000 组有符号整数 A、B，范围为 [−10³⁷,10³⁷]，
输出 A+B，见 [官方参数](../../../upstream/sample/many_aplusb_128bit/info.toml)。
最大和的绝对值是 2×10³⁷，小于 2¹²⁷−1≈1.70×10³⁸，
所以固定宽度的 __int128 或 Rust i128 已经够用。
两端等号意味着数 10³⁷ 本身有 **38 个十进制数字**，解析尾部不能只按 37 位考虑。

~~~text
输入：                     输出：
3                         -7
-12 5                     100000000000000000000
99999999999999999999 1     0
-100000000000000000000 100000000000000000000
~~~

二进制 128 位加法是固定宽度操作，通常由少量机器字加法和进位完成。
困难在于把最多 38 位的十进制文本转成两个机器字，再把结果写回来。
和 [64 位 Many A+B](../many_aplusb/analysis.md) 一样，总体处理量线性于文本字节数；
这里多出来的是符号、宽整数乘法和宽整数格式化的成本。

## 2. 上游的直观流程，以及它实际多做了什么

[correct.cpp](../../../upstream/sample/many_aplusb_128bit/sol/correct.cpp)
先 scanf 读两个字符数组，再调用
[base.hpp](../../../upstream/sample/many_aplusb_128bit/base.hpp) 的
str_to_i128，逐位做 res=res×10+digit，最后处理符号。
做完 a+b 后，i128_to_str 反复取余 10、除以 10，向字符串追加最低位，
末尾加负号并翻转，再由 printf 输出。

比如格式化 1203，依次取得 3、0、2、1，临时字符串为 "3021"，
翻转后才是 "1203"。负数先求幅值，所以 −1203 会额外带负号。
题目范围没有触及最小 i128，因而上游直接取负在本题内合法；
通用整数库若要支持 −2¹²⁷，则需要无符号幅值等方法。

上游对每组构造输入/输出字符串，并逐位依赖地处理最多几十位。
128 位除法也没有一条覆盖通用 128÷128 的普通 x86 指令；
编译器可能展开，或使用运行库帮助函数，常量除数还可能被强度削减。
因此具体出现多少除法指令或库调用要看生成代码，不能从每个 %10 符号直接计数。

## 3. 解析：16+16+尾部，而不是一路乘十

### 3.1 每块仍按十进制位权精确合并

先看一个短例子 123456789012345678：

~~~text
前16位 = 1234567890123456
末2位  = 78
完整值 = 前16位×100 + 78
~~~

更长的 token 分成两块十六位和一个不足十六位的尾部：

~~~text
value = (block0 × 10¹⁶ + block1) × 10^尾长 + tail
~~~

每个十六位块能用 64 位保存；最终乘权重和合并在 u128 内完成。
块内 SIMD 先将两位合成四位、八位、十六位，具体手算见
[64 位教程的分层归约](../many_aplusb/analysis.md)。
这是对同一个十进制位权公式重新分组，所有中间结果都应保持整数精确。

### 3.2 分阶段读取为何对变长数有意义

先加载十六字节，若已找到分隔符，就不必再解析第二块；
只有 token 确实更长，才加载下一段。最坏处理 16+16+6 位。
如果一开始总做三十二字节归约，短数也要支付较宽加载、排列和归约的成本。
反过来，全是长数时三十二字节方案可能减少分支，因此两种策略应按长度分布测量。

符号单独处理：跳过 '-'，把后面的数字解析为非负幅值，再赋予符号。
求负数幅值的通用办法是先转无符号，再做无符号的 0−bits，
避免在有符号域直接对最小负数取负。它支持更宽的接口边界；
本题只需的 [−2×10³⁷,2×10³⁷] 已在安全范围之内。

## 4. 输出：一次宽分割后，把剩下的事交给 64 位

取 B=10¹⁹，将非负幅值 x 写成 x=qB+r，0≤r<B。
本题 q≤2×10¹⁸，r<10¹⁹，都能放进 uint64；
r 未必能放进 int64，因此这里要用无符号类型。

~~~text
x = 123456789012345678901
q = 12
r = 3456789012345678901
输出 "12" 后跟固定19位的 r
~~~

更容易出错的是尾部很小的情况：

~~~text
x = 12×10¹⁹ + 45
输出 q 的 "12"，然后输出19位的 "0000000000000000045"
~~~

低组若不补足 19 位，就相当于改变 B。q=0 时只输出 r 的普通表示，
否则会多出一个前导零。负号仅放在整个数前面。

toy 的 fixed19 真实实现是“最高 3 位 + 四组各 4 位”，总计 19 位；
其中所有位置都补零。商及小值使用四位查表与裁去前导零的路径。
这把许多次宽整数取位，改成一次宽分割加常量数量的 64 位分组输出。

## 5. 定点倒数：如何把除法换成精确乘法

### 5.1 不是用浮点近似 1/B

设 M=ceil(2ᴷ/B)。希望通过 q=floor(xM/2ᴷ) 计算 floor(x/B)，
乘法后右移 K 位即可。M/2ᴷ 比 1/B 稍大，
所以必须证明这个误差不会把商推进到下一个整数。

一个小例子说明不能随便选 K：B=10，K=8，则 M=26。
x=69 时 floor(69×26/256)=7，但 69/10 的整数商是 6，算错了。
增大 K 或限制 x 的范围，才可能保证精确。

因为 0≤M/2ᴷ−1/B<1/2ᴷ，乘 x 后的误差小于 x/2ᴷ。
而一个非整数 x/B 距下一个整数至少 1/B。
因此 **xB<2ᴷ** 是一个简单的充分条件：误差达不到跨越下一整数所需距离。
这是范围证明，不是“用浮点算得差不多再四舍五入”。

### 5.2 本题源码怎样实现 192 位缩放

#393714 / #400962 使用 K=192，B=10¹⁹，
并写成 M=2¹²⁸+M'，其中 M' 是固定的 128 位常量。
对本题 x≤2×10³⁷，有 xB≤2×10⁵⁶<2¹⁹²≈6.28×10⁵⁷，
所以这个定点倒数能给出精确商。

设 h 为 x×M' 的乘积高 128 位，则商可以由 (x+h)>>64 得到。
为什么？xM/2¹⁹² 等于 (x+xM'/2¹²⁸)/2⁶⁴，
舍去 xM'/2¹²⁸ 的不足 1 部分，不会跨过下一条 2⁶⁴ 的整数边界。
源码的 mulhi 将每个 128 位数拆成高、低两个 64 位，
用四个 64×64 乘积和中间进位组合出 h，不需要保存完整 256 位积。
本题幅值上界下 x+h 不会溢出 128 位；不能脱离输入上界，
把返回 64 位商的接口泛化为任意 u128 除法。

最后 r=x−qB。只有 q 的范围与精确性得到证明，这个相减得到的 r 才保证
落在 [0,B)，才能安全转换为 uint64 并作为固定十九位尾段输出。

## 6. 当前五份提交的实际路径

下面采用 2026-09-26 05:10 UTC 刷新的
[config](../../selected/many_aplusb_128bit/config.json)。
五条均为当时最新数据的 AC、不同用户前五；这不等价于这五条本轮均被主动重测。

| 排名 | 提交/用户 | 时间 | 实际方案 |
|---:|---|---:|---|
| 1 | [400962.cpp](../../selected/many_aplusb_128bit/400962.cpp)，Rohan_Kapri | 29 ms | 分阶段16位输入、10¹⁹定点倒数输出、CompactWriter |
| 2 | [393714.cpp](../../selected/many_aplusb_128bit/393714.cpp)，chaihf | 30 ms | 与上一份执行代码相同，差别仅换行、注释和空行 |
| 3 | [392686.cpp](../../selected/many_aplusb_128bit/392686.cpp)，QgQ | 31 ms | 分阶段16位输入、4096组分块、10¹⁶输出分组 |
| 4 | [384987.rs](../../selected/many_aplusb_128bit/384987.rs)，learningstud | 31 ms | AVX2 32位输入与尾部回退、10¹⁹定点倒数输出 |
| 5 | [405450.rs](../../selected/many_aplusb_128bit/405450.rs)，toomer | 32 ms | 4组分块、编译期SIMD/SWAR路径、10¹⁹输出分组 |

### #400962 与 #393714：两个用户，一份执行逻辑

统一 CRLF/LF 后的逐行比较显示，差别仅为 bundle 标记、说明注释与空行。
两份 main 都建立 toy::Reader 和 CompactWriter，逐组调用
read_staged_i128、a+b、writeln。没有 sums 大块中间数组；
输入按十六字节阶段处理，输出走上一节的 divmod_1e19。
29 与 30 ms 的差距不能归因于执行算法的变化。

CompactWriter 把多种最高组格式化政策收敛到一张四位数字表，减少模板实例化
和编译期资源。历史编译内存记录属于下面的既有实验，不能用运行内存或 OJ
1 ms 差距替代编译资源测量。当前本仓库
[staged_i128.cpp](../../../problems/sample/many_aplusb_128bit/staged_i128.cpp)
后来显式选择了 direct_mapping 等接口，不应假定它与历史 bundle 字节相同。

### #392686：按 10¹⁶ 分组，保留批处理

read_i128 先取符号，再由 read_u128 分阶段解析十六位。
文件里 FASTIO_UNSAFE_BLOCK_LOG 有多处受 ifndef 保护的默认值，
最前面生效的是 12，所以批次是 4096 个 i128，占 64 KiB，
不是看到后文默认值 14 就判成 16384。

emit_u128_unchecked 对能放入 u64 的值直接格式化，否则按 10¹⁶ 拆低组；
剩下的高组若仍太大，再拆一次，至多输出三个块。
它没有调用 toy 的手写 10¹⁹ 定点倒数，源码使用常量 / 和 %，
实际指令应由编译器生成结果判定。分块集中解析与输出，并以空白分隔 token。

### #384987：32 字节归约和明确的尾部检查

main 直接进入带 avx2,bmi1,bmi2 属性的 run，
读入路径映射普通输入文件。read_i128 在剩余不足 32 字节时逐字符处理，
否则先加载 32 字节，按第一个分隔符决定短块归约，或解析完整 32 位后
继续处理尾部。这与十六位分阶段方案的工作分配不同。

输出先将大幅值按 10¹⁹ 拆分，Rust carrying_mul 的高半乘积和进位组合
实现与 toy 同类的定点倒数；低组固定十九位，再追加换行。
程序没有在 main 为该 CPU 路径做运行时特性选择，目标机器条件属于其接口。

### #405450：四组小批次与编译期分派

solve 暂存四个 i128 和，集中调用快速输出宏。
FastInput 的 SSSE3 编译路径按十六字节逐阶段解析，
另有普通字操作的分组后备路线；没有完整编译参数时不能声称观察到了某一分支。
输出对大于 u64 的幅值反复以 10¹⁹ 拆组，再使用四位表输出固定低段。
这里使用源语言的宽整数 /、%，没有调用前两份的显式 mulhi 常量内核；
这不等于断言编译器无法优化它。

## 7. 应该怎样做控制变量实验

| 变化项 | 保持不变 | 重点输入 |
|---|---|---|
| 16字节分阶段 vs 32字节首读 | 同一格式化器 | 短/中/长token混合、38位端点 |
| 10¹⁶ vs 10¹⁹输出拆分 | 同一解析器 | 64位边界、低段接近0、连续进位 |
| 手写定点倒数 vs 常量除法 | 同一缓冲与输出分组 | 实际汇编和相同数值分布 |
| 单组 vs 4/4096组批处理 | 同一输入输出内核 | 大T与缓存计数 |
| CompactWriter表策略 | 同一源码功能 | 编译峰值内存、运行时间分别记录 |

正确性至少包括零、异号相消、最大绝对值、10¹⁹边界前后、低段补零、
数字尾部跨页。小输入可直接与 Python 整数结果对照。
本页没有新增性能测量；榜单的 29–32 ms 是完整程序时间，
需要区分解析、输出、编译政策、CPU 和测量波动的作用。

## 8. 2026-08-15 阶段的历史实验记录

下文旧记录中的“30 ms 超过 31 ms 榜首”指当时比较的提交；
现在前两名的执行代码相同，新的快照第一名是 29 ms，不能继续把历史排名当当前结论。

Fetched sets:

- [global.json](global.json): five fastest deduplicated public submissions;
- [chaihf.json](chaihf.json): five fastest personal submissions.

The final 2026-08-15 submission reports `30 ms`, improving on the previous
public `31 ms` leader.

## Recurring Techniques

- regular-file `mmap` and pointer parsing;
- AVX2 reduction of 32 decimal digits into four 8-digit groups, then two 16-digit halves;
- scalar handling only for the remaining 0–6 digits and signs;
- a portable buffered fallback in reusable implementations;
- careful unsigned-magnitude conversion for negative signed-128 values;
- splitting output at $10^{19}$ so the quotient and remainder fit 64 bits;
- replacing compiler runtime `__udivti3` with reciprocal-multiply high-half division by $10^{19}$;
- four-digit lookup tables for the remaining formatting work;
- large output buffers and few `write` syscalls.

These optimizations target genuinely different bottlenecks. SIMD parsing alone is insufficient if every output still performs dozens of software 128-bit divisions.

## Adopted Design

- staged 16-byte parsing, which beats the 32-byte AVX path on the
  mixed-length official distribution;
- reciprocal division by $10^{19}$ and grouped output formatting;
- `CompactWriter`, which instantiates one 10,000-entry table instead of three.

The compact policy reduced a bundled cold compile from roughly 1.13 GB peak
RSS (OJ compiler killed) to about 557 MB. It costs a few percent in isolation,
but the staged parser recovers that cost and improves the final OJ result.
All ten official cases and the independent Writer/guard-page tests pass.
