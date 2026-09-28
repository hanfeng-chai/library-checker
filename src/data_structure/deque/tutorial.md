# Deque / 双端队列

## 中文

### 1. 问题与核心表示

需要支持队首/队尾插入、队首/队尾删除，以及按当前队列下标随机访问。操作总数
不超过 `Q`，因此任意时刻的元素个数也不超过 `Q`。

性能主解 `centered_array.cpp` 使用公共模板 `FixedCenteredDeque<T,Q>`。它分配
`2Q+1` 个连续槽位，并令空队列的左右游标都从正中央 `Q` 开始：

- 当前元素恰好位于半开区间 `[left,right)`；
- 队首插入写入 `data[--left]`；
- 队尾插入写入 `data[right++]`；
- 两端删除只移动一个游标；
- 第 `i` 个元素就是 `data[left+i]`。

因为最多只有 `Q` 次操作，所以游标无论一直向左还是一直向右都不会越界。这个
表示没有取模、扩容和分段寻址，热路径只包含一次加减和一次连续内存访问。

### 2. 正确性

不变量是：`data[left],...,data[right-1]` 按顺序等于抽象双端队列。空队列时
区间为空，不变量成立。队首插入在旧区间前写入新值，队尾插入在旧区间后写入
新值；两种删除分别移除区间的第一个或最后一个槽位。因此每次修改后不变量仍
成立，而 `data[left+i]` 自然就是队列下标 `i` 的元素。

### 3. 复杂度与其他实现

所有操作最坏 `O(1)`，预分配空间 `O(Q)`。

- `centered_array.cpp`：无取模的定长连续数组，本仓库的性能首选。
- `circular_buffer.cpp`：只分配 `Q+1` 个槽位，用 `(front,size)` 和取模实现，
  空间常数更小，是经典循环队列模板。
- `standard_deque.cpp`：直接使用 `std::deque`，代码最短；标准库采用分块存储，
  仍保证本题所需操作为 `O(1)`。

三种实现都使用同一套 shape-aware 快速 I/O。固定 CPU、相同 native flags 的
36 个官方用例全部通过；在 `max_random_00` 上，连续居中数组的 `perf` cycles
为公开 `#362377` 的 `0.965x`，指令数为 `0.848x`。循环数组会多做整数取模，
标准库版本则多一层分块寻址。短用例受进程启动时间主导，不据此宣称微小胜负。

## English

### 1. Representation

The task needs pushes and pops at both ends plus indexed access. Since there
are at most `Q` operations, the deque can never contain more than `Q` values.

The preferred `centered_array.cpp` uses the reusable
`FixedCenteredDeque<T,Q>`. It allocates `2Q+1` contiguous slots and starts an
empty half-open interval `[left,right)` at the center:

- push-front writes `data[--left]`;
- push-back writes `data[right++]`;
- either pop moves one cursor;
- index `i` is stored at `data[left+i]`.

Even a sequence consisting entirely of pushes to one end fits, because at most
`Q` cursor moves occur. The hot path needs neither modulo arithmetic,
reallocation, nor segmented addressing.

### 2. Correctness

Maintain the invariant that `data[left..right)` equals the abstract deque in
order. It is true for the empty interval. Each push extends the corresponding
end with exactly the new value, and each pop removes exactly the corresponding
endpoint. Thus the invariant is preserved, and indexed access returns the
correct element.

### 3. Complexity and variants

Every operation is worst-case `O(1)` and storage is `O(Q)`.

- `centered_array.cpp`: fixed contiguous storage without modulo; preferred for
  performance.
- `circular_buffer.cpp`: the classical `(front,size)` circular representation
  in `Q+1` slots; it saves capacity but performs modulo arithmetic.
- `standard_deque.cpp`: the shortest implementation, using the standard
  segmented deque while retaining `O(1)` required operations.

All variants use the same shape-aware fast I/O and pass all 36 official cases.
On `max_random_00`, the centered array uses `0.965x` the `perf` cycles and
`0.848x` the instructions of public `#362377` under identical native flags.
Startup dominates tiny cases, so sub-millisecond differences there are not
treated as evidence.
