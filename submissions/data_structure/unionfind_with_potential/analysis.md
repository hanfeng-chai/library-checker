# Unionfind with Potential：把已知差值沿父链相加

## 题意：约束可以被拒绝，查询只使用已接受的信息

有 N 个未知数 A[0…N-1]，所有运算模 `M=998244353`。
操作 `0 u v x` 声明 `A[u]-A[v]≡x`，若与此前接受的约束相容，就接受并
输出 1；若矛盾，输出 0 且不加入这条约束。`1 u v` 查询这个差值，
不能唯一确定就输出 -1。`N,Q≤200000`，见
[`task.md`](../../../upstream/data_structure/unionfind_with_potential/task.md)、
[`info.toml`](../../../upstream/data_structure/unionfind_with_potential/info.toml)。

```text
输入                    输出
4 9                     -1
1 0 1                   1
0 0 1 5                 1
0 1 2 7                 12
1 0 2                   998244341
1 2 0                   1
0 0 2 12                0
0 0 2 13                12
1 0 2                   0
1 3 3
```

接受 `A0-A1=5` 和 `A1-A2=7` 后，必有 `A0-A2=12`，反向差为
`-12 mod M=M-12=998244341`。再次声明差值 12 是一致的，仍输出 1；
声明 13 则拒绝，之后查询依旧为 12。点 3 虽未与其他点关联，自己减自己
永远是 0。这里只能确定相对差：给同一连通组的所有 A 同时加任意常数，
所有差值不变，所以没有必要恢复每个 A 的绝对数值。

## 朴素方法：把差值看成带方向的边

约束 `A[u]-A[v]=x` 可视为从 v 到 u 走时加 x，从 u 到 v 走时加 -x。
查两点时在已接受的图中寻找路径，沿路径累加；找不到说明二者之间尚无信息。
新约束若两端已有路径，就把路径差值与 x 比较；若没有，总可以用新边把
两组接起来。拒绝矛盾边后，已接受图里的任意闭路总差值都为 0。

保留一片生成森林就足以代表这些信息，但每次 BFS/DFS 仍可能访问 O(N)
个点，总计最坏 `O(NQ)`。普通并查集已经能近乎常数时间判断“有没有路径”，
现在要为父指针多保存一项，让它同时回答“沿这条路径总共差多少”。
分组、负大小数组与路径压缩的基础可参阅
[`Unionfind 教程`](../unionfind/analysis.md)。

## 父边势差的不变量

选择如下方向，后文公式都以它为准：

```text
edge[v] = A[v]-A[parent[v]]
weight[v] = A[v]-A[root(v)]
```

沿父链把 edge 相加，中间 A 项两两抵消，就得到 weight。
例如 `3→2→1`，边差为 4、-7，则 `A3-A1=4-7=-3`。
若 u、v 同根，根的绝对势抵消，
`A[u]-A[v]=weight[u]-weight[v]`；若不同根，两个根之间的偏移未知，
答案还不能确定。

路径压缩时，不能只改 parent。若旧父亲为 p，先求出 p 到根的差，
再将 `edge[v]←edge[p]+edge[v]`，最后改 `parent[v]←root`。
上例压缩 3 直连 1 后，边差应为 -3；若仍留 4，就改变了原来的关系。

```text
find(v):
    if v 是根: return v
    p=parent[v]
    r=find(p)
    edge[v]=edge[p]+edge[v] mod M
    parent[v]=r
    return r
```

递归返回时，edge[p] 已经表示 p 到根，所以加法顺序与下标时机都明确。
同一操作还可以一起返回 `(root,weight)`，避免外层再次查找根。

## 把一个根接到另一个根，差值应该是多少

新约束是 `A[u]=A[v]+x`。查找后得到 ru、rv 及 wu、wv：

```text
A[u]=A[ru]+wu
A[v]=A[rv]+wv
A[ru]+wu=A[rv]+wv+x
所以 A[ru]-A[rv]=wv+x-wu
```

若准备把 ru 接到 rv，下挂根的 edge 就填 `wv+x-wu`。
例如 wu=2、wv=7、x=5，新根边为 10；u 相对 rv 的差成为 10+2=12，
再减 v 的 7，恰为要求的 5。若按大小合并决定反向挂接，则把这个整体差
取负，即 `wu-wv-x`，不能仍使用原方向的值。

两根相同时不必接边，只比较 `wu-wv` 与 x。一致时接受，矛盾时拒绝；
关系集合不变，但查找过程中的路径压缩仍可改变内部父指针表示。
这与“矛盾时所有内存完全不许改写”不同，重要的是既有约束语义不变。

按大小或秩连接可防止树退化，配合路径压缩通常得到
`O((N+Q)α(N))` 总时间、`O(N)` 空间。α 是极慢增长的反阿克曼函数，
这里计每次模加减为固定字宽常数操作；这是摊还界，不是每问固定访问两个点。

## 上游 `correct.cpp` 的参数顺序

上游 [`correct.cpp`](../../../upstream/data_structure/unionfind_with_potential/sol/correct.cpp)
的泛型 DSU 保存 bos、sz、_pot 三条数组，根指向自身。
`query(v)` 递归压缩，执行 `_pot[v]=op(_pot[bos[v]],_pot[v])`。
本题实例化 `op=模加法、id=0、inv=取负`，所以 _pot 就是上面的 edge。

但其 `merge(v1,v2,d)` 接口声明的是 `A[v2]=A[v1]+d`，和题目参数顺序
相反，因此 main 调 **merge(v,u,x)**。源码把 b1 接到 b2 时填
`_pot[v2]-d-_pot[v1]`，正是把这套参数代入根边公式。
查询也调 `query(v,u)`，才得到题目要求的 `A[u]-A[v]`。
不能把 main 的交换去掉，只照抄模板注释。

主程序在查询时先分别查根判断是否连通，同根后又调用双点 query 再各查一次。
第二遍往往很短，但确实有重复调用。模板按大小合并，初始化 O(N)，
后续按标准势并查集摊还界运行；`scanf/printf` 负责 I/O。

模运算也有明确范围：两个规范余数相加小于 2M，至多减一次 M；
根边 `wv+x-wu` 在 `(-(M-1),2M-2)` 内，补一次负值再减一次大于等于 M
的值即可规范化。本题这个区间还在有符号 32 位范围内。
这些界允许用条件加减代替通用余数表达式，但实际编译器是否已经对 `%M`
做同样优化，需要看生成代码，不能仅凭源码字符推断除法指令数量。

## 当前五份源码与范围

下表来自 [`global.json`](global.json) 的 **2026-09-26 05:12:10 UTC**
不同用户前五，均为当时最新测试版本 AC。源码、语言和哈希见
[`config.json`](../../selected/unionfind_with_potential/config.json)。
本题没有归档 `candidate_pool.json`，本文只审阅这五份，不声称完整覆盖
所有算法族或前五之外的最快实现。最新状态不等于本文主动重测每份代码。

| 名次 | 提交 / 用户 | 语言 | 时间 | 主路径 |
|---:|---|---|---:|---|
| 1 | [#224108](https://judge.yosupo.jp/submission/224108) / sortA0329，[源码](../../selected/unionfind_with_potential/224108.cpp) | cpp | 8 ms | 泛型负大小 DSU，完整压缩，模加法势差 |
| 2 | [#404924](https://judge.yosupo.jp/submission/404924) / toomer，[源码](../../selected/unionfind_with_potential/404924.rs) | rust | 9 ms | 按大小、迭代路径分裂，内部势差方向相反 |
| 3 | [#368008](https://judge.yosupo.jp/submission/368008) / Anonymous，[源码](../../selected/unionfind_with_potential/368008.cpp) | cpp | 9 ms | 紧凑 Node、按秩、条件模约减与预取 |
| 4 | [#400981](https://judge.yosupo.jp/submission/400981) / Rohan_Kapri，[源码](../../selected/unionfind_with_potential/400981.cpp) | cpp | 10 ms | 相近 Node 结构，但秩递增分支不可达 |
| 5 | [#393948](https://judge.yosupo.jp/submission/393948) / chaihf，[源码](../../selected/unionfind_with_potential/393948.cpp) | cpp | 10 ms | root_and_weight 一次返回根与势差 |

### #224108：泛型接口并未替换主算法

main 实例化 `PotentializedUnionFind<u32,模加,模负>`，调用 merge_valid、
same 和 potential。parent 根槽存负大小，diff 存父边差，root 返回时
合成根相对势差。合并时根据负大小选择方向，用 func/inv 写出同一个
`wv+x-wu` 或它的相反数；同根时比较 `diff[u]==diff[v]+x`，
省去单独先求差再比较，但数学条件相同。

查询仍先 same、后 potential，两次各找双方根。源码里其他普通、可撤销
并查集以及 extract/groups 接口未调用，不能算成本题持久化或分组枚举。
I/O 是 MmapReader 与 BasicWriter，操作码和布尔结果直接按字符处理。
表中第一名并没有新的渐进算法，不能从 8 ms 推出泛型封装本身更快。

### #404924：相反方向的内部势，不等于输出反了

Rust `main` 使用 `PotentializedUnionFind<AdditiveOperation<M>>`。
其 `difference(u,v)` 返回 `pot[v]-pot[u]`，可以把内部 pot 理解为
`A[root]-A[vertex]`，是本文 weight 的负值；于是差的顺序反过来后，
结果仍为 `A[u]-A[v]`。首次合并 0、1、差值 5 时，若根 1 接到 0，
它存 pot[1]=5，表示 `A0-A1=5`，而本文常规方向会在该边存 -5。

find 每步先累计旧父边，再把当前点改接祖父，但循环仍前往**旧父亲**，
这是路径分裂，不是一次跳两级的路径折半，也不是递归回程完整压缩。
合成祖父边时同步更新势值，避免压缩后方向信息丢失。
类型别名指定按大小与允许压缩，不使用同文件的 UndoableUnionFind。

新增约束先调用 difference；同根比较即可，不同根再调用 unite_with，
因此成功连接时会重复求根。Rust trait 接口与数值封装不改变这条实际控制流。
缓冲读写通过 FastInput/FastOutput 的宏入口，不能因文件很长把其他
集合或模数模板计入每问开销。

### #368008：把父亲和差值放到同一小节点

Node 为 8 字节，含 pot 与 parent，秩另存静态数组。200001 个节点的
初始自指父亲在 constinit 中生成，查询时每次取父亲往往也能取到差值。
find 递归压缩；unite 按秩，相等时确实增加新根秩，满足标准平衡条件。
模加、模差在已知范围内用一次条件减完成。

query 和 unite 开头预取两个端点 Node，尝试提前把首个缓存行取来；
深层父亲尚未知，预取并不能使所有父链访问都连续。两种操作只各查双方
根一次，省去上游查询的第二轮根调用。
输入按操作码直接移动字符指针，其他字段用定界整数读取，输出为固定容量
缓冲与查表数字格式化。这些工程因素与 Node 布局都进入 9 ms 整程序成绩。

### #400981：不能把无效的秩代码当成按秩合并

Node 两字段顺序与上一份相反，reduce 使用掩码式条件减。关键差异是：
分支 `rnk[ru]>=rnk[rv]` 直接把 rv 接到 ru，却没有在相等时增秩；
增秩条件反而放在 else 中。进入 else 已有 `rnk[ru]<rnk[rv]`，
里面再判断相等永远不成立。秩初值全 0，因此实际上一直是“第二根接到
第一根”，没有平衡合并保证。

势差公式仍保持连通与差值关系，但不能给这份源码套“按秩+压缩”的 α 界。
例如依次加入 `(u=i,v=i-1,x=0)`，可以把旧根一层层接到新点，最后再查
0，会走很长的递归链。仅路径压缩可引用更保守的摊还界，单次查找深度
仍可能 O(N)，也带来栈风险。当前 AC 记录不是这段不可达增秩逻辑正确的证据；
复用应把相等时增秩放回实际选中新根的分支，再重新验证。

### #393948：一次查找同时返回根与势差

main 只实例化 `PotentialUnionFind<AdditiveModGroup<M>,Compress=true>`。
root_and_weight 在递归返回时合并父势差，返回 pair；difference 和
unite 各对两端调用一次，随后直接使用所得根和 weight，避免先判连通、
再求差的重复入口。负大小数组同时承担父亲/集合大小的存储，按大小决定
是否对整条新根边取逆。

群接口使用条件减的模加和条件模负。文件中的 Matrix2Group 与
Compress=false 分支并未进入本题主路径；后者只是同一公共模板的备用参数。
I/O 使用 direct_mapping、字段位数特化和带填充的整数输出。
摊还复杂度仍与上游同阶，收益来自少一次遍历接口与较直接的算术路径，
不能把 8/9/10 ms 当作单独测量这项收益的实验。

## 验证与本仓库对应实现

本地 [`path_compression.cpp`](../../../problems/data_structure/unionfind_with_potential/path_compression.cpp)
和 [`union_by_size.cpp`](../../../problems/data_structure/unionfind_with_potential/union_by_size.cpp)
分别保留压缩与不压缩版，说明见
[`tutorial.md`](../../../problems/data_structure/unionfind_with_potential/tutorial.md)。
不压缩但按大小时每次最坏 `O(log N)`，适合迁移到要回滚的结构；
本题不需要保留旧版本，通常可用压缩。

小例可在带差值图中 BFS 作参照，覆盖 u=v、x=0、跨组首次连接、同组
重复一致、矛盾被拒绝后再查询、反向差及 M-1 附近的模回绕。
还应构造倾向长链的连接顺序，区分真正的按大小/秩与只在源码中声明了秩。
本篇复核本地源码与算例，没有 OJ 请求或新性能测试；本地 tutorial 的
历史 cycles 比值不能当作这次前五快照的控制变量实验。
