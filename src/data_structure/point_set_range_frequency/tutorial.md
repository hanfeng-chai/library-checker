# Point Set Range Frequency / 单点赋值、区间频数

## English

### Fast solution: `offline_value_timelines.cpp`

Values receive compact IDs online while input is read through a 32-bit flat
hash map, so this problem does not pay for unused 64-bit key/value storage.
For each update `a[p] = y`, append two time-ordered events:

- remove position `p` from the old value;
- add position `p` to value `y`.

For each frequency query, append a range event to the requested value. Values
that have not appeared yet are answered with zero immediately. Stable CSR
grouping puts every value's events together without changing their time order.

One reusable `FenwickBitset` then processes values one by one. It stores the
active positions in 64-bit words, answers the two boundary fragments with
`popcount`, and keeps a Fenwick tree only over whole-word counts:

1. activate positions initially holding this value;
2. replay its changes and range queries in chronological order;
3. remove positions holding it in the final array, returning the tree to zero.

Timelines with no query are skipped.

### Correctness

At the start of one value's replay, the Fenwick tree marks exactly its initial
positions. Each chronological remove/add event mirrors the corresponding array
assignment, so by induction it marks exactly the positions currently equal to
that value at every query time.

The Fenwick sum on `[l,r)` is therefore precisely the requested frequency.
After removing final positions the tree is empty, so processing the next value
cannot inherit stale state.

There are `O(N+Q)` events. Stable grouping is linear. A relevant position
change or query costs `O(log ceil(N/64))` plus constant-time word operations,
which is `O(log N)`. Total memory is `O(N+Q)`.

### Other implementations and performance

- `online_per_value_fenwick.cpp`: precollect every position a value may occupy
  and build one sparse Fenwick tree per value. Query processing itself is
  online, with `O(log N)` operations and `O(N+Q)` total sparse storage.
- `policy_based_tree.cpp`: maintain all `(value,position)` pairs in one PBDS
  order-statistics tree. It is fully online and independently verifies answers.

Against public `#312395`, fixed-CPU `perf` measurements on three representative
maximum shapes use `0.84x–0.94x` cycles and `0.66x–0.79x` instructions. The
shared 64-position Fenwick blocks and compact integer hash map therefore retain
the reusable offline interface while also beating the public baseline locally.

## 中文

### 高性能解：`offline_value_timelines.cpp`

读入时用 32 位 flat hash map 在线给已出现值分配紧凑 ID，避免为本题的 32 位
键和值浪费 64 位存储。对修改 `a[p]=y`，按时间加入两个事件：

- 从旧值中删除位置 `p`；
- 向新值 `y` 加入位置 `p`。

频数查询则加入目标值的区间事件。尚未出现过的查询值直接回答零。稳定 CSR 按值
聚合事件，同时保持每个值内部的原时间顺序。

之后只复用一个 `FenwickBitset`，逐个值处理。它用 64 位字保存活跃位置，两端
残块由 `popcount` 直接统计，Fenwick 只维护完整字的计数：

1. 激活初始时等于该值的位置；
2. 按时间回放修改与区间查询；
3. 删除最终数组中等于该值的位置，使 Fenwick 恢复全零。

完全没有查询的值时间线会被跳过。

### 正确性

回放某个值前，Fenwick 恰好标记其初始位置。每个删除/加入事件与原数组赋值同步，
故按时间归纳可知，在任一查询时刻，树中置一的位置恰好是当前等于该值的位置。

所以 `[l,r)` 的 Fenwick 和就是所求频数。最后清除终态位置后树重新为空，处理
下一个值时不会继承旧状态。

事件总数 `O(N+Q)`，稳定分组线性；每个相关位置变化或查询为
`O(log ceil(N/64))` 加常数次字操作，也就是 `O(logN)`；空间 `O(N+Q)`。

### 其他实现与性能

- `online_per_value_fenwick.cpp`：预收集每个值可能占据的位置，为每个值建立稀疏
  Fenwick；真正处理查询时在线，每次 `O(logN)`，总稀疏空间 `O(N+Q)`；
- `policy_based_tree.cpp`：一棵 PBDS 顺序统计树维护全部 `(值,位置)`，完全在线，
  也是独立正确性参考。

与公开 `#312395` 同机比较，三个代表性最大形状的固定 CPU `perf` cycles 为
`0.84x–0.94x`，指令数为 `0.66x–0.79x`。因此公共的 64 位置 Fenwick 分块与紧凑
整数哈希既保留了离线时间线接口的通用性，也在本地严格快于公开基线。
