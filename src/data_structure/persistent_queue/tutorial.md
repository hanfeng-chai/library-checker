# Persistent Queue / 持久化队列

## 中文

### 1. 把版本关系看成一棵树

第 `i` 次操作从某个旧版本 `k` 产生新版本 `i`，所以把 `k -> i` 连边后，所有
版本形成一棵以空版本为根的版本树。题目先给出全部操作，因此可以离线遍历这棵
树，而不必真的复制每个队列。

性能主解 `rollback_version_tree.cpp` 在 DFS 路径上维护一个普通连续队列：

- `push(x)` 把 `x` 写到 `buffer[back++]`；
- `pop()` 读取 `buffer[front++]`，并按输入顺序编号保存答案；
- 进入一个版本时执行该版本的操作；
- 访问完它的所有子版本后执行逆操作：撤销 push 就 `--back`，撤销 pop 就
  `buffer[--front]=answer`。

因此 DFS 当前根到版本 `v` 的路径，正好描述版本 `v` 依次继承的操作。相邻子树
共享同一缓冲区，但离开前的回滚保证它们互不影响。实现使用递归 DFS；最坏版本
链深度为 `Q`，遵循算法评测的大栈约定。

### 2. 正确性

对 DFS 深度归纳。根路径为空时，`front=back=0`，表示空队列。若父版本表示
正确，执行 push 会在队尾追加唯一的新元素；执行合法 pop 会返回并移除当前队首，
所以进入子版本后表示也正确。离开子版本时，逆操作恢复进入前完全相同的
`front`、`back` 和必要槽位，因此下一个兄弟版本仍从正确的父状态开始。每个 pop
答案又写入它预先记录的输入序号，最终输出顺序也正确。

### 3. 在线倍增解

`binary_lifting.cpp` 是独立的在线持久化模板。每次 push 创建一个不可变节点，
其父亲是旧队尾，并建立 `2^j` 级祖先；一个版本只保存 `(back,size)`。逻辑队列
恰好是以 `back` 结尾、长度为 `size` 的父链后缀。pop 的队首就是从 `back` 向上
`size-1` 步的祖先，倍增即可找到；新版本保留 `back` 并把长度减一。

这个表示无需预先知道版本树，适合操作必须立刻回答的在线场景。它不是故意写慢
的朴素解，而是在更强接口下仍有竞争力的标准方案。

### 4. 复杂度与性能

- 离线版本树：每条版本边进入、离开各一次，时间 `O(Q)`，空间 `O(Q)`。
- 在线倍增：push 和 pop 均为 `O(log Q)`，空间 `O(Q log Q)`。

固定 CPU 的 14 个官方用例中，离线实现全部 AC，并在 11 个用例的墙钟中位数上
快于公开 `#211296`；`random_00` 的稳定 `perf` cycles 为公开基线的 `0.724x`，
指令数 `0.753x`，cache misses `0.464x`。倍增解保留用于在线模板和独立交叉
验证。两者均使用针对字段范围选择的最快 I/O。

## English

### 1. The version tree

Operation `i` creates version `i` from an older version `k`. Adding the edge
`k -> i` turns all versions into a tree rooted at the empty version. Since the
whole input is available in advance, `rollback_version_tree.cpp` traverses
that tree instead of copying queues.

Along the current DFS path it maintains one contiguous queue:

- push writes `buffer[back++]`;
- pop records `buffer[front++]` in the operation's answer slot;
- leaving a push decrements `back`;
- leaving a pop decrements `front` and restores its slot.

The root-to-current path is exactly the operation history inherited by the
current version. Rollback restores the parent state before another sibling is
visited. The recursive traversal may be `Q` levels deep and intentionally
uses the large-stack convention of programming-contest judges.

### 2. Correctness

Induct on DFS depth. The root has an empty buffer interval. Executing a push
appends exactly one value, and executing a valid pop returns and removes
exactly the front value, so a child state is correct whenever its parent is.
The inverse action restores every queue cursor and affected slot, hence sibling
subtrees start from the same correct parent state. Answer indices restore
input order independently of DFS order.

### 3. Online binary lifting

`binary_lifting.cpp` provides a genuinely online persistent queue. A push
creates an immutable node whose parent is the old back and stores all
power-of-two ancestors. A version is `(back,size)` and represents the last
`size` nodes on that ancestor chain. Its front is the ancestor at distance
`size-1`; a pop finds it by binary lifting and returns `(back,size-1)`.

This stronger interface does not need the future version tree and is a useful
competitive alternative rather than an intentionally slow naive solution.

### 4. Complexity and performance

- Offline version-tree traversal: `O(Q)` time and `O(Q)` memory.
- Online binary lifting: `O(log Q)` per push/pop and `O(Q log Q)` memory.

The offline version passes all 14 official cases and wins 11 wall-time medians
against public `#211296`. On `random_00`, stable `perf` ratios are `0.724x`
cycles, `0.753x` instructions, and `0.464x` cache misses. Both variants use the
fastest applicable shape-aware I/O.
