# Range Chmin Chmax Add Range Sum / 区间取界、加法与区间和

## 中文

### 1. 为什么普通懒标记不够

`chmin(x)` 只降低大于 `x` 的元素。同一个结点里可能有很多不同值，不能像
区间加那样用一个统一标记更新区间和。Segment Tree Beats 额外记录“第二极值”，
判断一次取界是否只会影响当前极值。

每个结点保存：

- `maximum`、严格次大值 `second_maximum`、最大值个数；
- `minimum`、严格次小值 `second_minimum`、最小值个数；
- `sum` 和区间加懒标记。

### 2. 三种更新

对完整结点执行 `chmin(x)`：

1. 若 `maximum<=x`，什么都不做；
2. 若 `second_maximum<x<maximum`，只有所有最大值变成 `x`，于是

   ```text
   sum += (x-maximum) * maximum_count
   maximum = x；
   ```

3. 否则至少有两档值会被改变，必须下传并递归。

`chmax` 对最小值完全对称。区间加 `v` 会把四个有限极值都平移 `v`，并做
`sum += v*length`。

下传顺序必须是：先把加法传给孩子，再用父结点当前的 `maximum/minimum`
约束孩子。父结点可能曾在不下传的情况下只压低最大值或抬高最小值，这两个界
正是相应的隐式 Beats 标记。

### 3. 迭代版与递归版

`iterative_segment_tree_beats.cpp` 是性能主解。它先沿查询左右边界把懒标记
推到叶，再用标准迭代线段树分解直接找到完整覆盖的规范结点。只有某个规范
结点无法在第二极值条件下整块处理时，才在该子树内部递归 Beats；最后沿两条
边界路径重新合并。这样避免普通递归区间遍历对大量不相交结点的判断。

`recursive_segment_tree_beats.cpp` 使用更常见的“结点区间与查询区间比较”写法。
不变量完全相同，代码流程更直观，适合作为模板教学和独立验证。

### 4. 正确性

第二极值条件成立时，除最大值组外所有元素都不大于 `second_maximum<x`，
所以 `chmin` 恰好只改变已知数量的最大值，聚合公式精确；条件不成立时递归
不会漏掉任何受影响值。`chmax` 同理。加法保持各值大小关系，因此平移极值
即可。下传后孩子受到与父亲相同的加法和上下界，重新 pull 恢复全部极值、
计数和区间和不变量。归纳每次操作后，查询和即为真实数组区间和。

### 5. 复杂度与性能

Segment Tree Beats 的势能分析保证整批操作的经典均摊界，通常表述为每次
`O(logN)` 均摊、总计 `O((N+Q)logN)` 量级；空间 `O(N)`。

最大随机用例中，迭代版、递归版和公开第一 `#204482` 都约 `618 ms`；
在完整官方用例集上迭代版总墙钟约 `8.3 s`，递归版约 `10.1 s`，因此前者
作为性能主解。

## English

### 1. Beats metadata

A uniform lazy tag cannot represent `chmin(x)`, because only values above `x`
change. Each node therefore stores the maximum, strict second maximum and
maximum count, the symmetric minimum data, the sum, and a lazy addition.

For a fully covered node:

- if `maximum<=x`, `chmin` is a no-op;
- if `second_maximum<x`, exactly the maximum group changes, so update the sum
  by `(x-maximum)*maximum_count`;
- otherwise descend.

`chmax` is symmetric. Addition shifts every finite extremum and adds
`value*length` to the sum. Propagation sends addition first, then clamps each
child to the parent's current maximum and minimum.

### 2. Iterative and recursive variants

`iterative_segment_tree_beats.cpp` pushes only the two boundary paths and
enumerates canonical covered nodes iteratively. Recursion occurs only inside a
canonical node when the second-extremum condition fails. Boundary ancestors are
then rebuilt. `recursive_segment_tree_beats.cpp` uses the more familiar
interval-overlap recursion and preserves the same invariants.

When the second-maximum condition holds, every non-maximum element is already
below `x`, so the aggregate update changes exactly the known maximum group.
The symmetric argument handles `chmax`; addition preserves ordering.
Propagation and pulling restore all metadata, proving correctness inductively.

The standard Beats potential argument gives an amortized
`O((N+Q)logN)` scale and `O(N)` memory. All three implementations are about
`618 ms` on the maximum random case, while the iterative variant is faster over
the complete official suite (`8.3 s` versus `10.1 s` for the recursive form).
