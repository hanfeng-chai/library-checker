# Enumerate Quotients / 枚举整除商

## 中文

需要按升序输出所有不同的 `floor(N/i)`。直接枚举 `i=1..N` 不可行。

设 `s=floor(sqrt(N))`。所有不同商分成两部分：

1. 不超过约 `s` 的商，它们是连续整数；
2. 大于 `s` 的商，它们依次来自 `N/s, N/(s-1), ..., N/1`。

商大于 `sqrt(N)` 时，对应的除数一定小于 `sqrt(N)`，所以第二部分不会
漏；商较小时，相同商虽然对应许多除数，但只需输出一次。边界
`s*s+s<=N` 决定 `s` 是否同时属于两部分，避免重复。

### 解法一：物化向量（`vector_enumeration.cpp`）

`enumerate_quotients` 先生成小值和大值，再把后半段翻转。代码简洁，
便于后续需要随机访问商值的算法复用，时间、空间都是 `O(sqrt(N))`。

### 解法二：流式输出（`direct_enumeration.cpp`）

先精确修正浮点平方根，然后直接输出连续前缀和倒序除数对应的商。
无需保存最多约两百万个 `u64`，辅助空间 `O(1)`，也省去分配和反转。
该版本在 OJ 为 12 ms，并成为提交时的全球第一。

两份解法都使用 direct mapping 和 `N<=10^12` 的十三位解析路径，也都使用
相同的 bounded CompactWriter。向量版只承担更易复用的存储差异，不牺牲
I/O。

## English

Distinct values of `floor(N/i)` are a consecutive small prefix plus
`N/s, N/(s-1), ..., N/1`, where `s=floor(sqrt(N))`.

`vector_enumeration.cpp` materializes the reusable vector in `O(sqrt(N))`
memory. `direct_enumeration.cpp` streams both halves directly in sorted order,
using `O(1)`
auxiliary memory and reaching 12 ms on the judge.

Both use direct mapping, the thirteen-digit policy for `N<=10^12`, and the
same bounded CompactWriter.
