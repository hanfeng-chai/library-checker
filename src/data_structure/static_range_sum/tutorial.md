# Static Range Sum

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 26.518 (Anonymous), sum 208.707 (Anonymous)。

- `main.cxx` max: 21.565 ms (-18.68%), sum: 170.502 ms (-18.31%)
- `naive.cxx` max: 28.940 ms (+9.13%), sum: 222.977 ms (+6.84%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
令 `prefix[k]` 表示前 k 个元素之和，`prefix[0]=0`。
读入时计算 `prefix[i+1]=prefix[i]+a[i]`，查询 `[l,r)` 返回
`prefix[r]-prefix[l]`。时间 `O(N+Q)`，空间 `O(N)`。

`N,Q <= 500000`，总和最多 `5e14`，前缀和用 u64，端点用 u32。
数组值以 `read<u64,10>()` 直接进入前缀和；端点最多六位。

`main.cxx` 每 128 个查询分三步处理：

1. 用 `read_pair<6>()` 一次读取两个端点，并预取对应的前缀和。
2. 计算这一批答案，让随机访存有机会提前完成。
3. 调用 `Writer::write(span)` 批量输出。

`naive.cxx` 对齐 chaihf 的主流程：读入时求前缀和，随后逐个读查询、计算、输出。
它没有预取或查询分块，用于比较简单调用方式下的 I/O 性能。

```sh
make gen-static_range_sum
make check-static_range_sum-main check-static_range_sum-naive
make bundle-data_structure-static_range_sum-main
```

## 尝试过程与取舍

先以朴素前缀和逐项读写建立基线，再比较内联、读算分离、不同预取批量和分阶段处理。最终 128 项一批预取、计算、输出；随后 I/O 内核继续迭代，并将 naive 对齐 chaihf 的逐项流程单独检查。主解需要赢五份参考，naive 只要求 max/sum 不劣于 chaihf。
