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

## Lenovo 对照（2026-10-01）

环境检查通过、不绑核，准备后只运行单个串行评测任务，期间没有其他 SSH 或操作。
每例单次，全部用例 task-clock 的 max/sum（包含样例），单位 ms：

| 解答 | max | sum |
| --- | ---: | ---: |
| main | 21.358 | 168.644 |
| naive | 28.956 | 221.089 |
| Anonymous | 26.770 | 207.793 |
| chaihf | 29.856 | 229.532 |
| cheat_when_I_was_young | 30.229 | 226.069 |
| sortA0329 | 36.130 | 275.370 |
| IceKylin | 36.867 | 281.770 |

主解两项均胜过五份提交；朴素版两项也胜过 chaihf，不代表逐测例全部更快。
证据：bench/ds-sequence-20261001/round1/。生产代码沿用此前定稿版本，未因本次复核修改。
GCC/Clang 官方检查通过。
