# Majority Voting / 动态区间严格众数

## English

### Fast solution: `offline_candidate_verification.cpp`

Each segment-tree node stores a Boyer-Moore summary `(candidate,balance)`.
Merging equal candidates adds balances; merging different candidates cancels
the smaller balance from the larger one. Point assignment updates one leaf.

For every range query, the segment tree supplies one possible majority
candidate. A candidate is not sufficient by itself, so its true frequency is
verified offline:

1. updates become remove/add events for old and new value IDs;
2. each range query becomes a frequency event for its candidate;
3. stable per-value timelines are replayed with the shared Fenwick helper;
4. output the original value only when `2 * frequency > range_length`.

### Correctness

Pairwise cancellation removes equal numbers of two different values. If a
value occurs in more than half the range, no cancellation sequence can remove
all its excess occurrences, so it must be the final Boyer-Moore candidate.

The converse is not guaranteed, which is why verification is necessary. The
offline timeline invariant states that at each query time the Fenwick tree
marks exactly the current positions of the candidate. Its range sum is the
exact frequency, and the strict inequality is exactly the problem definition.
Thus a majority is always returned and a false candidate is always rejected.

Candidate updates and folds cost `O(log N)`. Timeline grouping is linear and
verification costs `O((N+Q) log N)` in total, with `O(N+Q)` memory.

### Other implementations and performance

- `online_candidate_tree.cpp`: the same candidate segment tree plus sparse
  per-value Fenwick trees, giving an online verification template after
  preprocessing future positions.
- `policy_based_candidate_tree.cpp`: verify candidates in one PBDS tree of
  `(value,position)` pairs; fully online and independent.

The offline solution beats public `#390875` locally: maximum random cases are
about `0.89x` its wall time and the `top2_killer` case about `0.73x`.

## 中文

### 高性能解：`offline_candidate_verification.cpp`

线段树每个节点保存 Boyer-Moore 摘要 `(候选,净票数)`。候选相同则票数相加；
候选不同则用较小票数抵消较大票数。单点赋值只需修改一个叶子。

每个区间查询先由线段树给出唯一可能的严格众数候选，但候选本身并不充分，仍需
离线验证真实频数：

1. 修改转换为旧值删除、新值加入事件；
2. 区间查询转换为候选值的频数事件；
3. 用共享 Fenwick helper 稳定回放每个值的时间线；
4. 仅当 `2*frequency > 区间长度` 时输出原值。

### 正确性

两两抵消每次删除两个不同值各一个。若某值出现次数严格超过区间一半，无论如何
抵消都无法消除它的全部剩余票，所以它必然是最终 Boyer-Moore 候选。

反向并不成立，因此必须验证。离线时间线不变量保证查询时 Fenwick 恰好标记候选
当前所在位置，区间和就是精确频数；严格不等式又与题意完全一致。所以真正众数
一定输出，伪候选一定被拒绝。

候选更新与区间合并均为 `O(logN)`；时间线分组线性，验证总计
`O((N+Q)logN)`，空间 `O(N+Q)`。

### 其他实现与性能

- `online_candidate_tree.cpp`：同一候选线段树加按值稀疏 Fenwick，在预收集未来
  位置后提供在线验证模板；
- `policy_based_candidate_tree.cpp`：用一棵 `(值,位置)` PBDS 验证候选，完全
  在线且算法独立。

离线版本本地快于公开 `#390875`：最大随机用例墙钟约为其 `0.89x`，
`top2_killer` 用例约为 `0.73x`。
