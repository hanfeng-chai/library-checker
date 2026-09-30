# Static Range Sum

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

## Lenovo 对照（2026-09-30）

通过环境检查，不绑核；准备完成后只运行单个串行评测任务，期间没有其他 SSH 或操作。
每个非样例各六次，轮换顺序，取 task-clock 中位数后求 max/sum（包含样例）。单位 ms。

| 解答 | max | sum |
| --- | ---: | ---: |
| main | 21.089 | 167.228 |
| naive | 28.333 | 218.133 |
| Anonymous | 26.063 | 205.600 |
| chaihf | 28.649 | 222.224 |
| cheat_when_I_was_young | 29.010 | 222.427 |
| sortA0329 | 35.209 | 270.666 |
| IceKylin | 35.381 | 274.387 |

主解的 max、sum 均胜过下载的五份提交；朴素版的两项也胜过 chaihf。
朴素版的 `random_02` 仍略慢，因此不能把汇总优势理解为逐测例都更快。
成对读入后，主解在该测例为 11.540 ms，Anonymous 为 12.001 ms。

新库下比较了数组值的 u32/u64 读取、朴素/分块主流程；最终主解和朴素版都选用 u64。
GCC、Clang 均通过全部官方数据，展开后的程序也通过编译。
完整记录和选中二进制哈希在 `bench/io-final-20260930/`，库的接口与其他题目结果见
[io.md](../../../docs/io.md)。
