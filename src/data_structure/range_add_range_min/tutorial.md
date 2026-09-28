# Range Add Range Min / 区间加、区间最小值

## 中文

### 解法一：差分数组的前缀最小值

`difference_prefix_min.cpp` 利用了本题操作形状，而不是套通用懒线段树。

令

```text
d[0] = a[0]
d[i] = a[i] - a[i-1]  (i>0)。
```

则 `a[i]=d[0]+...+d[i]`。给 `[l,r)` 全部加 `x` 时，中间相邻元素的差不变，
只需

```text
d[l] += x
d[r] -= x  (若 r<n)。
```

一次区间更新因此退化成至多两个单点更新。

查询 `[l,r)` 的最小值时，先求 `a[l-1]=sum(d[0..l))`。对差分子段
`d[l..r)`，设它的所有非空前缀和最小值为 `prefix_min`，那么答案是

```text
sum(d[0..l)) + prefix_min(d[l..r))。
```

线段树结点保存二元组 `(sum, prefix_min)`。相邻两段 `L,R` 的合并为

```text
sum = L.sum + R.sum
prefix_min = min(L.prefix_min, L.sum + R.prefix_min)。
```

这个运算有顺序但满足结合律，所以可以用迭代线段树。实现用位运算直接枚举
查询区间的规范结点，并用一条从叶到根的路径计算左侧前缀和，减少通用 fold
的常数。

#### 正确性

区间加只改变差分的两个边界，这是差分定义的直接推论。合并公式把一个前缀
分为“完全位于左段”或“包含整个左段再进入右段”两种且仅有的情况，所以结点
确实保存其区间的最小非空前缀和。把 `l` 之前的总和加回，正好恢复每个
`a[l],...,a[r-1]` 的真实值，因此查询正确。

### 解法二：隐式 Treap

`implicit_treap.cpp` 把数组下标作为隐式中序键。每个结点保存子树大小、最小
值和区间加懒标记。两次 split 隔离 `[l,r)`，整棵中段加懒标记或读取最小值，
再 merge 回去。

它的期望复杂度同样是 `O(logN)`，还可自然扩展插入、删除、翻转等序列操作；
但本题数组长度固定，差分线段树的常数和缓存局部性明显更好。

### 复杂度与性能

- 差分线段树：建树 `O(N)`，每次更新/查询 `O(logN)`，空间 `O(N)`；
- 隐式 Treap：建树 `O(NlogN)`，每次操作期望 `O(logN)`，空间 `O(N)`。

固定 CPU 的最大随机用例中，差分版约 `267.6 ms`，与公开第一 `#361524`
约 `267.5 ms` 持平；Treap 约 `1872 ms`，作为功能更一般的替代模板保留。

## English

### Solution 1: prefix minima of the difference array

Define `d[0]=a[0]` and `d[i]=a[i]-a[i-1]`. A range addition on `[l,r)` changes
only `d[l]+=x` and, when `r<n`, `d[r]-=x`.

For a query, `sum(d[0..l))` is the value immediately before the requested
range. If `prefix_min(d[l..r))` denotes the minimum non-empty prefix sum of that
difference subarray, then

```text
answer = sum(d[0..l)) + prefix_min(d[l..r)).
```

A segment-tree node stores `(sum,prefix_min)`. Ordered concatenation is

```text
(L.sum + R.sum, min(L.prefix_min, L.sum + R.prefix_min)).
```

Every prefix of the concatenation lies either entirely in `L`, or contains all
of `L` followed by a prefix of `R`; hence this associative merge is exact.
The implementation enumerates canonical iterative-tree nodes directly and
computes the left prefix along one leaf-to-root path.

### Solution 2: implicit Treap

`implicit_treap.cpp` stores subtree size, minimum, and a lazy addition tag.
Two splits isolate `[l,r)`, an aggregate operation is applied, and merges restore
the sequence. It is more general and supports future sequence edits naturally,
but has much larger constants for this fixed-length problem.

### Complexity and performance

The difference tree builds in `O(N)` and handles each operation in `O(logN)`;
the Treap builds in expected `O(NlogN)` and has expected `O(logN)` operations.
Both use `O(N)` memory. The fast variant is about `267.6 ms` on the maximum
random case, tied with public leader `#361524` at about `267.5 ms`.
