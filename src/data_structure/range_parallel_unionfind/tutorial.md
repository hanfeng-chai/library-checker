# Range Parallel Union-Find / 区间平行并查集

## 中文

一次操作要求对所有 `0<=i<k` 合并 `a+i` 与 `b+i`。逐对处理会在大量重复区间
上退化；关键是让已经处理过的整块关系可以直接跳过。

### 1. 多层并查集

第 `h` 层的元素 `p` 表示长度 `2^h` 的块 `[p,p+2^h)`。调用
`unite(h,a,b)` 表示这两个等长块的对应位置都应连通。

- 若它们在第 h 层已经同组，说明这对整块关系以前处理过，立即返回；
- 若首次合并且 `h>0`，递归合并左右两半：

  ```text
  unite(h-1, a, b)
  unite(h-1, a+2^(h-1), b+2^(h-1))；
  ```

- 到 `h=0` 时才真正合并原数组位置。

高层并查集不是最终答案中的连通关系，而是“这对块的对应边已经展开过”的
记忆化结构。

### 2. 在线维护答案

底层每个连通块维护原始权值和 `component_sum`。若两个不同底层块的权值和为
`A,B`，合并后新出现的同块无序点对恰好是一边取旧第一块、一边取旧第二块，
贡献

```text
A * B。
```

把它加到答案并令新块权值和为 `A+B` 即可，全程模 `998244353`。已经同组时
不产生新点对。

### 3. 两种区间分解

`overlapping_power_blocks.cpp` 取 `h=floor(log2 k)` 和 `len=2^h`，只调用

```text
unite(h, a, b)
unite(h, a+k-len, b+k-len)。
```

前后两个最大二次幂块可能重叠，但它们并集覆盖整个长度 k；并查集合并具有
幂等性，重叠只会触发“已同组”快速返回，不影响正确性。这使每次输入层面只有
两次高层调用。

`binary_decomposition.cpp` 按 k 的二进制位拆成互不相交块，逐块调用相应层，
逻辑更直接，也验证了另一种通用分解方式。

### 4. 正确性与复杂度

归纳 h：首次合并两个 `2^h` 块时，递归调用正确建立左右半块所有对应位置的
关系；若高层已同组，这些递归以前已经全部执行，因此跳过安全。两种区间分解
都覆盖且只要求原操作中的对应位置；重叠重复合并不改变等价关系。

每层每个块关系只会成功合并有限次，成功后永久被并查集记住，所以全部操作的
展开总量为 `O(NlogN)` 次并查集合并，再乘反阿克曼因子。空间
`O(NlogN)`。代表性 `large_k` 用例中，重叠块版约 `268 ms`，与公开第一
`#251479` 持平；不交二进制分解约 `318 ms`。

## English

An operation unions `a+i` with `b+i` for every `0<=i<k`. Processing every pair
again would be too slow on repeated ranges.

At level `h`, DSU element `p` represents block `[p,p+2^h)`. The first successful
union of two level-h blocks recursively unions their corresponding left and
right halves. If the high-level blocks are already connected, those underlying
relations were expanded earlier and can be skipped. Level zero performs the
actual element union.

Each base component stores its original weight sum. Merging different
components of sums `A` and `B` creates exactly `A*B` new weighted unordered
pairs; add this value to the answer and replace the component sum by `A+B`.

`overlapping_power_blocks.cpp` covers a length-k range by its first and last
blocks of length `2^floor(log2 k)`. They may overlap, but union is idempotent,
so two high-level calls suffice. `binary_decomposition.cpp` instead uses
disjoint blocks for every set bit of k.

Induction on the level proves that a successful block union creates every
required base relation, while a failed union may safely skip already-expanded
work. Across all operations there are only `O(NlogN)` successful memoized block
unions, each with inverse-Ackermann DSU cost, and `O(NlogN)` memory. The fast
variant is about `268 ms` on `large_k`, tied with public leader `#251479`.
