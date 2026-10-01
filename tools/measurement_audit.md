# 全量评测审计

本轮按 [benchmark.md](benchmark.md) 的命令执行：master.local 用 -j40 强制
重编译、重新检查，Lenovo 单任务串行完成两轮评测，再逐 binary、逐测例选
较小 task-clock；其他计数器来自同一轮，不混合不同观测的指标。

| 分类 | 题数 |
|---|---:|
| sample | 3 |
| data_structure | 47 |
| tree | 20 |
| convolution | 16 |
| polynomial | 28 |
| set_power_series | 5 |
| big_integer | 6 |

初始全量共 125 题、138 份自有程序、620 份下载提交；checksum 没有 OJ 对照。
758 个 binary 的 18,452 项正式测例检查已全部通过。评测机上的两轮共
36,904 次运行，覆盖与源码/参数/输入/binary 哈希均已核对通过。

各题的可提交明细是 src/<分类>/<题目>/bench.txt，教程顶部给出自有解答的
max、sum 和相对全部已测参考最佳值的百分比。参考 max、sum 可来自不同提交。
每个表格保留编译机、编译器、参数、评测机环境和 binary SHA-256；原始两轮、
完整源码快照及全部输入哈希保存在 bench/full-20261001/stage/。

以这次完整重建的报告为准，早期阶段表和运行库重链接等效声明不再用作当前数值。
结果反映当前机器与这些正式输入，不代表实时 OJ 排名。细小差距应结合两轮波动
判断；task-clock 包含用户态和内核态，缺页、分配及透明大页等成本也会影响观测。
具体精度与独占约定见 benchmark.md，I/O 布局复查见
[io.md](../include/toy/io.md#writer-布局复查2026-10-01)。

## 本轮结论

2026-10-01，北京时间 17:37:44–19:25:01，完整计时约 107 分钟；前后环境检查
均为 OK。测量期间仅运行这个串行任务，没有另开 SSH、编译、检查、同步或监控。

针对初始五份参考，124 道 OJ 题的 main 均已同时优于参考的最佳 max、sum。
初始全量中 range_kth_smallest 的 max 未达标；独立复核确认后，压缩其前缀
工作集，最终 max 35.726、sum 296.491 ms，参考最佳为 43.328/358.628 ms。
仅这个题的 main 依赖 offline_kth.h；其他正式 binary 和测量记录保持原批次。

range_kth_smallest 的正式 binary 与候选 SHA-256 相同，复用其两轮完整记录；
五份参考和 naive 复用初始全量的两轮记录，不再次计时。跟进数据保存在
bench/kth-20261001/final/，该题 bench.txt 分别标明来源，未把不同源码的测量混取。

static_range_sum/naive 为 max 28.940、sum 222.977 ms，chaihf 为
29.776/229.428 ms，两项均更快。自建 checksum 只比较自有实现，不计入 OJ 达标题数。

Writer 布局与 checksum 成对读取扩展未显示值得采用的整体收益，io.h 保持原样。

## 新提交复核

随后新增 19 份提交，全部编译成功，585 项正式测例检查通过。其中 15 份位于
已完成题目，单独在 Lenovo 静默执行两轮，共 952 次；旧 binary 复用原记录。
另外四题（all_furthest_neighbors、matrix_product、sum_of_totient_function、
enumerate_palindromes）尚无自有 .cxx 主解，本轮仅生成正式数据并检查新增提交。

新增提交中，下列三份在 max、sum 两项均胜过当前 main，单位 ms：

| 题目 | 新提交 | main max / sum | 新提交 max / sum |
|---|---|---:|---:|
| convolution_mod | Aiyiyi | 41.091 / 927.157 | 35.559 / 780.377 |
| convolution_mod_2_64 | toomer | 199.151 / 3498.189 | 190.680 / 3158.567 |
| convolution_mod_large | Aiyiyi | 1452.134 / 29868.939 | 1081.079 / 22929.496 |

因此当前 121/124 道 OJ 题的 main 在全部已测参考中两项达标；其余十二份新增
提交两项都未超过 main。15 题的 bench.txt 与教程已更新为六份参考，逐程序列明
复用来源。原始新增记录与合并报告分别位于 bench/new-submissions-20261001/
的 stage/、combined/，没有从额外轮次中重新挑选旧提交的最快值。
