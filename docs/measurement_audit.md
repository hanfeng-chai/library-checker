# 评测审计

2026-09-30，四类共 55 题全部完成。对当前生产二进制重新计算 SHA-256，核对完整测例、原始首轮记录及各题五份下载提交。每题 main 的 max、sum 均严格小于五份对照中的最小值。

评测使用独立空闲的 Lenovo，不绑核，每例一次；同一静默任务串行运行，期间没有其他评测机操作。数据与程序暂存于 `/dev/shm`，环境检查全部通过。max、sum 包含样例，指标为 task-clock，单位 ms。对照的最佳 max、sum 可能来自不同提交。

| 分类 | 完成题数 |
| --- | ---: |
| convolution | 16 |
| polynomial | 28 |
| set_power_series | 5 |
| big_integer | 6 |

I/O 的五份生产二进制也与已验收产物逐字节一致。其诊断重复统计、checksum 逐范围取舍及 static_range_sum/naive 的 chaihf 对照见 [io.md](io.md)。

全部源程序已在 GCC、Clang 下通过官方测例；各库的独立对照、边界与 sanitizer 验证见对应文档。完整审计清单在 `build/polish-audit.json`，逐测例表格在 `bench/<分类>/<题目>/`。下列证据保留输入、二进制哈希及原始 perf 记录。

## 当前结果

| 题目 | main max / sum | 对照最佳 max / sum | 原始证据 |
| --- | ---: | ---: | --- |
| big_integer/addition_of_big_integers | 15.527 / 110.341 | 15.629 / 163.581 | `bench/big-final-20260930/round1/result.json` |
| big_integer/addition_of_hex_big_integers | 11.428 / 91.045 | 14.713 / 151.670 | `bench/big-final-20260930/round1/result.json` |
| big_integer/division_of_big_integers | 65.647 / 963.885 | 75.433 / 1293.854 | `bench/big-final-20260930/round1/result.json` |
| big_integer/division_of_hex_big_integers | 59.850 / 699.991 | 60.816 / 728.304 | `bench/big-final-20260930/round1/result.json` |
| big_integer/multiplication_of_big_integers | 49.799 / 564.622 | 52.822 / 818.016 | `bench/big-final-20260930/round1/result.json` |
| big_integer/multiplication_of_hex_big_integers | 35.934 / 503.340 | 36.771 / 558.559 | `bench/big-final-20260930/round1/result.json` |
| convolution/bitwise_and_convolution | 40.605 / 142.713 | 55.712 / 197.568 | `bench/bitwise-20260930/round1/result.json` |
| convolution/bitwise_xor_convolution | 44.096 / 154.348 | 56.632 / 200.251 | `bench/bitwise-20260930/round1/result.json` |
| convolution/convolution_F_2_64 | 614.846 / 12720.009 | 971.691 / 18425.468 | `bench/gf-multi-20260930/round1/result.json` |
| convolution/convolution_mod | 40.799 / 920.588 | 41.740 / 935.622 | `bench/convolution-20260930/round1/result.json` |
| convolution/convolution_mod_1000000007 | 70.385 / 1571.440 | 72.180 / 1618.730 | `bench/conv-final-20260930/round1/selected/result.json` |
| convolution/convolution_mod_2_64 | 197.968 / 3494.852 | 204.396 / 3620.339 | `bench/conv-final-20260930/round1/selected/result.json` |
| convolution/convolution_mod_large | 1439.845 / 29791.925 | 1533.376 / 31178.788 | `bench/large-mul-20260930/round1/result.json` |
| convolution/gcd_convolution | 63.698 / 596.847 | 79.130 / 760.591 | `bench/divisor-20260930/round1/result.json` |
| convolution/lcm_convolution | 67.528 / 636.335 | 80.987 / 792.085 | `bench/divisor-20260930/round1/result.json` |
| convolution/min_plus_convolution_concave_arbitrary | 261.897 / 1088.015 | 296.506 / 5018.958 | `bench/large-mul-20260930/round1/result.json` |
| convolution/min_plus_convolution_convex_arbitrary | 45.648 / 861.976 | 80.241 / 1505.199 | `bench/min-plus-20260930/round1/result.json` |
| convolution/min_plus_convolution_convex_convex | 28.287 / 444.931 | 32.006 / 515.532 | `bench/min-plus-20260930/round1/result.json` |
| convolution/mul_mod2n_convolution | 98.937 / 679.973 | 189.179 / 1311.640 | `bench/poly-gcd-final-20260930/round1/selected/result.json` |
| convolution/mul_modp_convolution | 51.738 / 335.494 | 127.348 / 780.455 | `bench/large-mul-20260930/round1/result.json` |
| convolution/multivariate_convolution | 353.937 / 1258.917 | 466.072 / 2303.134 | `bench/set-series-20260930/round4/selected/result.json` |
| convolution/multivariate_convolution_cyclic | 225.904 / 1349.753 | 290.107 / 2596.793 | `bench/conv-final-20260930/round1/selected/result.json` |
| polynomial/composition_of_formal_power_series | 12.984 / 208.755 | 35.563 / 559.069 | `bench/composition-20260930/round1/selected/result.json` |
| polynomial/composition_of_formal_power_series_large | 279.278 / 3971.476 | 350.106 / 4961.500 | `bench/composition-20260930/round1/selected/result.json` |
| polynomial/compositional_inverse_of_formal_power_series | 13.276 / 133.738 | 38.684 / 413.510 | `bench/composition-20260930/round1/selected/result.json` |
| polynomial/compositional_inverse_of_formal_power_series_large | 286.056 / 2337.053 | 325.601 / 2952.620 | `bench/composition-20260930/round1/selected/result.json` |
| polynomial/conversion_from_monomial_basis_to_newton_basis | 152.750 / 1158.874 | 334.682 / 2503.404 | `bench/poly-gcd-final-20260930/round1/selected/result.json` |
| polynomial/division_of_polynomials | 59.734 / 712.396 | 69.249 / 875.076 | `bench/poly-gcd-final-20260930/round1/selected/result.json` |
| polynomial/exp_of_formal_power_series | 80.471 / 963.053 | 93.971 / 1107.799 | `bench/fps-power-20260930/round1/result.json` |
| polynomial/exp_of_formal_power_series_sparse | 64.838 / 377.917 | 77.458 / 817.878 | `bench/fps-sparse-20260930/round1/selected/result.json` |
| polynomial/factorization_of_polynomials | 28.800 / 261.444 | 564.193 / 3513.869 | `bench/poly-factor-power-20260930/round1/selected/result.json` |
| polynomial/inv_of_formal_power_series | 42.800 / 513.753 | 55.608 / 667.693 | `bench/fps-inv-20260930/round2/selected/result.json` |
| polynomial/inv_of_formal_power_series_2d | 160.481 / 2250.536 | 358.499 / 6235.658 | `bench/poly-newton-20260930/round1/result.json` |
| polynomial/inv_of_formal_power_series_sparse | 34.967 / 191.881 | 44.657 / 378.279 | `bench/fps-log-20260930/round1/result.json` |
| polynomial/inv_of_polynomials | 183.813 / 1193.355 | 196.592 / 1276.298 | `bench/poly-gcd-final-20260930/round1/selected/result.json` |
| polynomial/log_of_formal_power_series | 74.222 / 785.437 | 76.431 / 827.422 | `bench/fps-block-20260930/round1/result.json` |
| polynomial/log_of_formal_power_series_sparse | 45.769 / 311.452 | 72.504 / 847.091 | `bench/fps-sparse-20260930/round1/selected/result.json` |
| polynomial/multipoint_evaluation | 89.828 / 491.770 | 91.409 / 496.008 | `bench/poly-hybrid-roots-20260930/round1/selected/result.json` |
| polynomial/multipoint_evaluation_on_geometric_sequence | 53.293 / 595.198 | 79.720 / 1111.155 | `bench/fps-block-20260930/round1/selected/result.json` |
| polynomial/polynomial_interpolation | 122.847 / 654.045 | 129.978 / 669.763 | `bench/poly-hybrid-roots-20260930/round1/selected/result.json` |
| polynomial/polynomial_interpolation_on_geometric_sequence | 168.856 / 1682.666 | 195.982 / 2013.451 | `bench/poly-product-20260930/round1/result.json` |
| polynomial/polynomial_root_finding | 129.177 / 1663.732 | 516.346 / 6337.979 | `bench/poly-factor-power-20260930/round1/selected/result.json` |
| polynomial/polynomial_taylor_shift | 52.248 / 740.451 | 75.540 / 1067.940 | `bench/fps-block-20260930/round1/selected/result.json` |
| polynomial/pow_of_formal_power_series | 124.118 / 1093.052 | 131.552 / 1370.415 | `bench/poly-factor-power-20260930/round1/selected/result.json` |
| polynomial/pow_of_formal_power_series_sparse | 70.248 / 516.151 | 94.659 / 1208.955 | `bench/fps-dense-20260930/round1/selected/result.json` |
| polynomial/prefix_sum_of_polynomial | 84.208 / 360.226 | 335.208 / 1467.671 | `bench/poly-tune-20260930/round1/selected/result.json` |
| polynomial/product_of_polynomial_sequence | 173.756 / 2354.952 | 199.601 / 2730.556 | `bench/poly-tree-20260930/round1/selected/result.json` |
| polynomial/shift_of_sampling_points_of_polynomial | 78.586 / 1041.063 | 91.044 / 1201.222 | `bench/poly-tune-20260930/round1/selected/result.json` |
| polynomial/sqrt_of_formal_power_series | 56.905 / 479.018 | 59.399 / 574.835 | `bench/fps-dense-20260930/round1/selected/result.json` |
| polynomial/sqrt_of_formal_power_series_sparse | 75.196 / 386.527 | 135.404 / 733.648 | `bench/fps-dense-20260930/round1/selected/result.json` |
| set_power_series/exp_of_set_power_series | 217.605 / 920.949 | 259.876 / 1117.472 | `bench/set-series-20260930/round4/selected/result.json` |
| set_power_series/log_of_set_power_series | 234.008 / 759.809 | 312.831 / 1044.522 | `bench/set-series-20260930/round4/selected/result.json` |
| set_power_series/polynomial_composite_set_power_series | 370.060 / 1562.866 | 472.848 / 2054.365 | `bench/set-series-20260930/round4/selected/result.json` |
| set_power_series/power_projection_of_set_power_series | 360.779 / 2080.237 | 471.724 / 2741.017 | `bench/set-series-20260930/round4/selected/result.json` |
| set_power_series/subset_convolution | 267.673 / 1324.843 | 304.963 / 1514.558 | `bench/set-series-20260930/round4/selected/result.json` |

## 乘法波动的补充诊断

最终批次十进制乘法有一次约 49.8 ms 的观测，较早候选约 32 ms；本表保留这次
较慢观测，仍满足 max/sum 目标。随后四个大测例各三次的静默诊断中，当前程序与
原候选用户态指令数相同，恢复到约 31–32 ms；禁用大页提示则慢到约 47–50 ms，
新增指令主要来自内核态。因此保留当前实现，诊断值没有替换正式验收值。
原异常没有复现，不能据此断言其具体原因。记录在 `bench/big-memory-20260930/`。

内存分配、缺页和透明大页状态也影响 task-clock。THP 的 madvise 整理模式可能在
分配时直接回收、整理内存；配置检查通过并不意味着这些内核开销固定。
参见 [Linux THP 文档](https://docs.kernel.org/admin-guide/mm/transhuge.html)。

## 早期记录的口径校正

部分早期研究脚本对非样例重复三次，旧 summary 曾使用逐测例中位数，若称这些值为“单次”则不准确。生产 `tools/bench.py` 一直保持每例一次。研究脚本现已修正；本表统一取原始记录中的 repeat=0，不从多次观测中挑选最快值。历史原始数据及旧 summary 保留。

发现问题时，已对当时达标的 35 题重新审计首轮，35 题仍全部达标；记录在 `build/first-observation-audit.json`。本表进一步核对了最终 55 题的当前二进制与完整测例。

这些结论只针对当前机器、配置、输入和下载的提交，不代表实时 OJ 排名或每个测例都更快。浮点快速路径的验证范围见 [fft_convolution.md](fft_convolution.md) 和 [fft_integer.md](fft_integer.md)。
## 2026-10-01：命名空间整理与运行库更新

common.h 不再包含 `using namespace std;`，相关标准库名字改为显式 std::。
先使用更新后的同一工具链构建 135 份现有 `.cxx`，再进行命名空间修改并重建：
**135 份二进制的 SHA-256 全部不变**。因此此次限定名字没有改变这些程序的
机器码或运行开销。另用 GCC/Clang 验证 `using namespace toy;` 不引入 span，
也不会让全局自定义的 array 与 std::array 冲突。

此前本机重启启用了更新后的 glibc/启动运行库，同源码重链接时有 126 份产物
发生变化。按本轮约定，将这项环境变化视为影响较小，保留原批次性能数据并
单独记录旧、新产物映射；不把重链接当成算法优化。算法变化仍须重新正确性
检查和静默比较，不能套用这个映射。GCC/Clang 都继续使用不链接 libstdc++ 的配置。

各题的重要尝试、失败原因、阶段 max/sum 和经典对照解会记录在该题的
`tutorial.md`，原始 perf 文件、输入和完整哈希留在未提交的实验目录。

## 2026-10-01：tree 阶段

20 题的 main 均已通过同批五份参考的 max、sum 门槛，生产产物逐项核对，运行库重链接按上节的明确映射处理。
共享修改也重新核对了此前 47 道数据结构题，static_range_sum 的 naive 仍满足
不劣于 chaihf 的约定。各题的完整尝试过程、失败方案和经典对照在相应 tutorial.md。

以下为各题采用批次的真实第 0 次观测，单位 ms；不把多轮的最快测例拼接。

| 题目 | main max / sum | 五份参考最小 max / sum |
|---|---:|---:|
| tree_diameter | 71.876 / 447.509 | 77.549 / 480.211 |
| cartesian_tree | 37.098 / 278.236 | 39.611 / 309.692 |
| lca | 49.421 / 852.148 | 56.527 / 867.988 |
| vertex_add_path_sum | 239.861 / 2297.382 | 266.665 / 2570.970 |
| tree_path_composite_sum | 39.059 / 348.625 | 80.924 / 753.619 |
| vertex_add_subtree_sum | 65.676 / 512.364 | 70.859 / 559.154 |
| rooted_tree_isomorphism_classification | 45.004 / 372.822 | 48.329 / 394.084 |
| rooted_tree_topological_order_with_minimum_inversions | 119.216 / 2301.200 | 240.148 / 4261.176 |
| frequency_table_of_tree_distance | 331.507 / 2147.472 | 356.689 / 3051.815 |
| common_interval_decomposition_tree | 109.447 / 1468.668 | 144.181 / 1986.508 |
| dynamic_tree_vertex_set_path_composite | 522.395 / 2365.933 | 556.798 / 2533.462 |
| point_set_tree_path_composite_sum_fixed_root | 291.211 / 4188.141 | 375.050 / 5995.118 |
| point_set_tree_path_composite_sum | 607.517 / 8336.027 | 684.546 / 10558.316 |
| dynamic_tree_vertex_add_subtree_sum | 246.963 / 1557.950 | 267.040 / 1889.581 |
| dynamic_tree_subtree_add_subtree_sum | 359.113 / 2224.468 | 487.377 / 3117.685 |
| vertex_add_range_contour_sum_on_tree | 259.552 / 2470.535 | 292.961 / 2940.626 |
| jump_on_tree | 152.452 / 1766.028 | 160.352 / 1896.726 |
| vertex_set_path_composite | 299.214 / 1825.169 | 310.213 / 1992.671 |
| dynamic_tree_vertex_add_path_sum | 390.386 / 1783.947 | 401.438 / 1848.033 |
| vertex_get_range_contour_add_on_tree | 263.627 / 2586.965 | 276.008 / 3037.616 |

此前已完成的数学类 55 题与三道 I/O 示例也保留完整阶段记录，本次总计整理
125 份 tutorial。到 tree 为止暂停，后续 graph 等分类待阶段验收后继续。
