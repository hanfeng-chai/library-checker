# Point Add Rectangle Sum / 动态加点、矩形求和

## 中文

操作按时间发生：加入一个带权点，或询问当前所有点在半开矩形
`[l,r) x [d,u)` 中的权值和。虽然必须保持在线时间语义，但可以先读完输入，
因为未来可能出现的点坐标不会因答案而改变。

### 解法一：Wavelet Matrix + 每层 Fenwick

`wavelet_fenwick.cpp` 是性能主解。

#### 建树

1. 收集初始点和未来所有加点坐标，按 `x` 排序。于是矩形的横坐标范围可由
   两次 `lower_bound` 变成数组区间 `[xl,xr)`。
2. 压缩 `y`，把按 `x` 排列的 y-rank 建成 Wavelet Matrix。
3. 每一层保存位向量及按 64 位块计算的 `rank1` 前缀。
4. 当前层稳定地把 0-bit 放前、1-bit 放后；在这个“下一层顺序”上建立一棵
   Fenwick 树，存当前有效权值。

所有层的位向量、rank 前缀和 Fenwick 数组都使用扁平连续内存。

#### 加点

每个预注册点记录它在最初 x 顺序中的位置。从最高位向下，用位向量的 rank
算出它稳定分划后的新位置，并在该层 Fenwick 对这个位置加权。重复所有层后，
更新完成。未来点建树时权值为零，到其真实操作时刻才执行这条路径，因此不会
提前影响答案。

#### 矩形查询

横轴先得到 `[xl,xr)`。Wavelet Matrix 按 y 的二进制位向下走，把
`y<Y` 分解为若干个完整的 0-bit 区间；每个完整区间直接向对应层 Fenwick
询问 `[xl,xr)` 映射后的权值和。分别计算 `y<u` 和 `y<d`，相减即为
`d<=y<u`。

#### 正确性

Wavelet Matrix 的稳定分划不会改变任意值集合，只改变它们的位置；rank 公式
精确给出原数组区间在下一层的对应区间。查询每遇到阈值位为 1，就把该层所有
0-bit 元素完整加入答案，继续检查 1-bit 分支，因此最终恰好枚举所有
`y<Y` 的点且不重不漏。每层 Fenwick 保存的正是当前时刻这些点的权值，所以
上下阈值之差就是目标矩形和。

### 解法二：稀疏二维 Fenwick

`sparse_fenwick.cpp` 更接近常见模板。外层是压缩 x 的 Fenwick；每个外层结点
只收集将来可能更新到它的 y，并在内部建立一棵 Fenwick。点更新沿外层更新
路径走，在每个结点更新对应 y；二维前缀沿外层查询路径累加内部 y 前缀，矩形
再做四项容斥。

它更容易从一维 Fenwick 推导，也适合需要一般二维前缀接口的场景，但嵌套
二分、许多小 vector 和较差的局部性使常数明显更大。

### 复杂度与性能

设 y 值域层数为 `L=O(logN)`。两种实现的更新和查询最坏均为
`O(L logN)=O(log^2N)`，空间均为 `O(NL)`；Wavelet 版本的优势主要来自扁平
内存、位 rank 和更规则的访问。

代表性最大随机用例中，Wavelet 版本约 `420 ms`，公开第一 `#206958` 约
`418 ms`，已处于同一性能水平；稀疏二维 Fenwick 约 `1070 ms`，作为清晰的
在线替代模板保留。

## English

Operations insert weighted points and query the current sum in
`[l,r) x [d,u)`. Time order must be respected, but all future coordinates can
be registered before execution because they do not depend on answers.

### Solution 1: Wavelet Matrix with per-level Fenwick trees

`wavelet_fenwick.cpp` is the performance variant.

Sort every possible point by `x`, compress `y`, and build a Wavelet Matrix over
the y-ranks. Each level stores a 64-bit rank bitvector. After its stable
zero/one partition, a Fenwick tree is built in the next-level order. All levels
use flat contiguous arrays.

A registered point knows its original x-order position. An insertion follows
the rank mapping through every level and adds the weight to the corresponding
Fenwick position. Future points start with zero weight and are updated only at
their actual operation time.

For a query, binary search produces the x-index interval. A Wavelet traversal
decomposes `y<Y` into complete zero-bit ranges; their current sums come from
the relevant Fenwick trees. Computing this for `u` and `d` and subtracting gives
exactly `d<=y<u`.

Stable partition preserves the represented elements, and rank maps any source
interval exactly to its child interval. Whenever a threshold bit is one, the
entire zero branch is below the threshold and is added once; the remaining
branch is examined recursively. Thus the traversal selects precisely `y<Y`,
while the Fenwick trees provide their current weights, proving correctness.

### Solution 2: sparse two-dimensional Fenwick

`sparse_fenwick.cpp` uses an outer Fenwick over x. Every outer node compresses
only the y-coordinates that can update it and owns an inner Fenwick tree. It is
the more direct reusable 2D-prefix template, but nested searches, many small
allocations, and weaker locality make it slower.

### Complexity and performance

With `L=O(logN)` Wavelet levels, updates and queries are
`O(L logN)=O(log^2N)` and memory is `O(NL)` for both approaches. On the
representative maximum random case, the Wavelet version is about `420 ms`,
essentially tied with public leader `#206958` at `418 ms`; the clearer sparse
Fenwick alternative is about `1070 ms`.
