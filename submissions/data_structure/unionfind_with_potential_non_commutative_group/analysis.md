# 非交换群势并查集：矩阵沿父链必须按顺序相乘

## 题意：未知的是矩阵，已知的是相对变换

每点 v 对应未知可逆 2×2 矩阵 A[v]，所有元素模 `M=998244353`。
`0 u v X` 声明 `A[u]=A[v]X`；与先前接受的信息一致就输出 1 并接受，
否则输出 0、忽略新约束。`1 u v` 查询 `A[v]^{-1}A[u]`，
不能确定则输出 -1。`N,Q≤200000`，输入 X 保证行列式为 1，见
[`task.md`](../../../upstream/data_structure/unionfind_with_potential_non_commutative_group/task.md)
和 [`info.toml`](../../../upstream/data_structure/unionfind_with_potential_non_commutative_group/info.toml)。

一个矩阵按行输入/输出四个数 `[a,b,c,d]`，表示 `[[a,b],[c,d]]`。
先用两个矩阵看清顺序：

```text
P = [1 1]     Q = [1 0]
    [0 1]         [1 1]

PQ= [2 1]     QP= [1 1]
    [1 1]         [1 2]
```

PQ 与 QP 不相等，尽管 P、Q 各自行列式都为 1。
下面完整输入先声明 `A1=A0P`，再声明 `A2=A1Q`，所以只能推出
`A2=A0(PQ)`，不能把它写成 A0(QP)：

```text
输入                         输出
4 8                          -1
1 0 1                        1
0 1 0 1 1 0 1                1
0 2 1 1 0 1 1                2 1 1 1
1 2 0                        1 998244352 998244352 2
1 0 2                        1
0 2 0 2 1 1 1                0
0 2 0 1 1 1 2                1 0 0 1
1 3 3
```

反向查询得到 `(PQ)^{-1}=[[1,-1],[-1,2]]`，负一规范为 M-1。
再次给 PQ 一致，给 QP 矛盾；点 3 与自身的相对变换总是单位矩阵 I。
未知 A 的绝对值没有必要恢复，同一组的全部 A 左乘同一个可逆矩阵，
所有 `A[v]^{-1}A[u]` 都保持不变。

## 最少的矩阵知识：乘法、单位元、逆元

矩阵乘法第 i 行第 j 列等于左矩阵第 i 行与右矩阵第 j 列的点积。
例如 PQ 的左上角为 `1*1+1*1=2`，右下角为 `0*0+1*1=1`。
四个输出各需两个乘积再相加，结果最后模 M。
单位矩阵 `I=[[1,0],[0,1]]` 满足 IX=XI=X。

矩阵乘法满足结合律 `(XY)Z=X(YZ)`，允许改变括号，但一般不允许交换
因子顺序。可逆矩阵的逆元满足 `X^{-1}X=XX^{-1}=I`，且
`(XY)^{-1}=Y^{-1}X^{-1}`：取逆时顺序反转。
“有结合律、单位元、每个元素可逆”的这种结构叫群；不要求交换律，所以
这里称非交换群。

对 `X=[[a,b],[c,d]]`，一般逆矩阵还需除行列式 `ad-bc`。本题输入
行列式恰为 1，而乘积、逆的行列式仍为 1，因此内部相对变换的逆简化为
`[[d,-b],[-c,a]]`。只需交换两项、取负两项，不需高斯消元或求模逆幂。
绝对未知 A[v] 未必行列式为 1，但沿输入约束得到的相对矩阵具有这个性质。

## 朴素图搜索，和势并查集保留了什么

把 `A[u]=A[v]X` 作为从 v 到 u 的右乘 X 变换，反向边右乘 X^{-1}。
每次找路径并按行走顺序相乘，即可求相对变换；已有路径时检查新约束是否
一致，跨组时接受并连边。可只保留生成森林，但每问仍最坏 O(N)，
总计 O(NQ) 次矩阵组合，二十万规模不可行。

普通并查集用父指针代表分组，势并查集给每条父边附加矩阵。
它不保存原图所有边，而保留足以表达同一组全部相对关系的父指针森林。
加法版的对应推导见 [`Unionfind with Potential`](../unionfind_with_potential/analysis.md)，
本篇重新推导乘法顺序，不能把加减公式机械替换符号。

## 父边方向和路径压缩的正确乘法次序

定义边矩阵与根相对矩阵为：

```text
E[v] = A[parent[v]]^{-1} A[v]
W[v] = A[root(v)]^{-1} A[v]
```

若旧父亲为 p，`W[p]E[v]` 中间的 `A[p]A[p]^{-1}` 抵消，
所以 `W[v]=W[p]E[v]`。这是**父亲到根的结果放左边，原父边放右边**。
示例 0 是根，1→0 的边为 P，2→1 的边为 Q；压缩 2 直连 0 后，
新边必须是 PQ。写成 QP 会直接在前述完整输入的查询中给出错误答案。

```text
find(v):
    if v 是根: return (v,I)
    p=parent[v]
    (r,Wp)=find(p)
    E[v]=Wp * E[v]
    parent[v]=r
    return (r,E[v])
```

同根时查询为 `W[v]^{-1}W[u]`，因为
`(A[r]^{-1}A[v])^{-1}(A[r]^{-1}A[u])=A[v]^{-1}A[u]`。
不是 `W[u]W[v]^{-1}`，后者的中间根矩阵不会按正确顺序抵消。

## 接根公式必须逐步移项

新约束为 `A[u]=A[v]X`，分别把两点表示成自己的根与 W：

```text
A[ru] Wu = A[rv] Wv X
左乘 A[rv]^{-1}，右乘 Wu^{-1}：
A[rv]^{-1} A[ru] = Wv X Wu^{-1}
```

所以 ru 接到 rv 时，新父边为 `Wv*X*inverse(Wu)`。
若按大小合并选择反向连接，则取整个乘积的逆：
`Wu*inverse(X)*inverse(Wv)`。不能只把三项分别取逆却保留原顺序。

用小矩阵手算一个接根条件：设 Wu=P、Wv=I、X=Q，则根边应为
`QP^{-1}=[[1,-1],[1,0]]`。新 u 相对 rv 的矩阵变为
`(QP^{-1})P=Q`，v 的相对矩阵是 I，因此所需相对关系正好为 Q。
即使根矩阵看起来出现负项，模 M 规范化后仍是合法行列式一矩阵。

若 ru=rv，则不连接父边，只比较 `Wu==Wv*X`；这与
`inverse(Wv)*Wu==X` 等价。接受重复一致约束、拒绝矛盾约束，都不改变
关系集合；压缩仍可能修改内部父指针。
按大小加完整压缩的总时间为 `O((N+Q)α(N))` 次固定规模群操作，空间
O(N)。矩阵乘法是常数规模，但远贵于加法，所以少做一次遍历往往也意味着
少做若干矩阵乘法，而不只是少一次整数比较。

## 上游实际执行的模板与矩阵内核

[`correct.cpp`](../../../upstream/data_structure/unionfind_with_potential_non_commutative_group/sol/correct.cpp)
与加法题使用同一 bos/sz/_pot 泛型 DSU。query(v) 的压缩顺序是
`op(_pot[旧父],_pot[v])`；merge 的接口仍是 `A[v2]=A[v1]*d`，
所以 main 传入 `merge(v,u,X)`，查询传入 `query(v,u)`。
模板接 b1 到 b2 时填 `_pot[v2]*inverse(d)*inverse(_pot[v1])`，
代入交换后的端点，正是上述有序根边公式。

矩阵由四个 ACL modint 组成，op 用 i、k、j 三重小循环做八次标量模乘和
累计，inv 用行列式为 1 的伴随矩阵公式。主查询先各查一次根判断同组，
再调双点 query，各查一次根，存在重复访问。
初始化 O(N)，每个群元素四个模数分量；与普通 DSU 相比主要新增父链
矩阵积，不是要存 N×N 的点对关系表。

快解可以把矩阵乘法展开为四个点积，每个点积把两个乘积先相加、最后
只取一次模。因为 `2(M-1)²<2^64`，64 位中间值足够。
这让语义上的模乘归约次数从八个标量乘法降到四个点积归约；循环展开、
常量模优化和 I/O 的实际收益仍需同机测量。

## 当前五份源码与范围

以下来自 [`global.json`](global.json) 的 **2026-09-26 05:12:14 UTC**
不同用户前五，均为当时最新版本 AC；源码与哈希见
[`config.json`](../../selected/unionfind_with_potential_non_commutative_group/config.json)。
没有归档的 `candidate_pool.json`，本篇限定在五份已读主路径，不宣称
已经搜完其他算法族。最新状态不等同于本文执行了重测。

| 名次 | 提交 / 用户 | 语言 | 时间 | 实际特点 |
|---:|---|---|---:|---|
| 1 | [#393949](https://judge.yosupo.jp/submission/393949) / chaihf，[源码](../../selected/unionfind_with_potential_non_commutative_group/393949.cpp) | cpp | 15 ms | 一次返回根与有序势，展开四点积 |
| 2 | [#236404](https://judge.yosupo.jp/submission/236404) / sortA0329，[源码](../../selected/unionfind_with_potential_non_commutative_group/236404.cpp) | cpp | 15 ms | 泛型势 DSU，展开矩阵乘法，查询重复查根 |
| 3 | [#235165](https://judge.yosupo.jp/submission/235165) / oldyan，[源码](../../selected/unionfind_with_potential_non_commutative_group/235165.cpp) | cpp | 17 ms | OY 势 DSU，存逆方向矩阵，压缩次序相反 |
| 4 | [#363681](https://judge.yosupo.jp/submission/363681) / urectanc，[源码](../../selected/unionfind_with_potential_non_commutative_group/363681.rs) | rust | 22 ms | 返回 leader 与势矩阵，模整数包装 |
| 5 | [#332456](https://judge.yosupo.jp/submission/332456) / nandhagk，[源码](../../selected/unionfind_with_potential_non_commutative_group/332456.cpp) | cpp20 | 27 ms | 32 位 Montgomery 模数分量，标准流 I/O |

### #393949：共用 DSU，主路径只实例化矩阵群

main 用 `PotentialUnionFind<Matrix2Group<M>,Compress=true>`，
root_and_weight 一次返回根与 W。unite、difference 各只遍历两个端点
一次，按负大小选方向；根边取逆针对整个有序积，和前面的推导直接对应。
Matrix2Group 四个 uint32 字段，multiply 用四条显式 64 位点积再 `%M`，
inverse 对 0 保持 0，使比较使用规范余数。

文件中的加法群、Compress=false 和其他 I/O 模板未进入该实例。
每次成功查询输出四个整数；该 Writer 允许它们分别带空白输出，checker
按 token 读取，所以源码不一定恰好把四个数写在同一行。
direct_mapping 与按位数字数特化也是整程序时间的一部分。

### #236404：同一有序势不变量，不同接口调用成本

`Main` 实例化 `PotentializedUnionFind<StaticArr<u32,4>,func,inv>`，
实际使用 merge_valid、same、potential。root 按“父势在左”压缩，
parent 的负数根槽保存大小。矩阵乘法也把两乘积合为一次模归约，
与第一份同样只有四个点积表达式。

查询先 same，再 potential，因此会重复调用双方 root；合并同根时
比较 `diff[u]==func(diff[v],X)`。inv 对零的非对角分量返回 M，
这与 0 同余；随后的 func 会再归约成规范矩阵。若抽出 inv 独立使用或
直接逐分量比较，不能假设其返回值已全部在 `[0,M)`。
MmapReader/BasicWriter 负责缓冲 I/O；两份都是 15 ms，不足以量化
一次遍历与重复入口的净收益。

### #235165：存的是 `A[v]^{-1}A[root]`

真正入口是 `main_non_commutative`；文件虽还有 `main_commutative`，
没有被调用。`OY::PDSU::Table<Group,true>` 用 parent、group_size、
dis 三数组。其边方向与前文相反：

```text
edge'[v]=A[v]^{-1}A[parent[v]]
W'[v]=A[v]^{-1}A[root]
W'[v]=edge'[v] * W'[parent[v]]
```

因此 find 压缩写 `m_dis[i]*父结果`，子边在左，不能按前文正方向把它
“修正”为父结果在左。`calc(a,b)=W'[a]*inverse(W'[b])=A[a]^{-1}A[b]`，
所以 main 用 calc(v,u)；unite_by_size 也交换题目端点。
根边表达式为 `inverse(W'[a])*X*W'[b]`，是同一约束在反向表示下的结果。

新增约束先尝试 unite_by_size；已经同组则该函数返回 false，再调用
calc 检查一致性，所以冗余或矛盾操作会再次找根。矩阵内核仍是显式点积，
cin/cout 宏映射到 OY::LinuxIO，不是另一种并查集算法。

### #363681：Rust 泛型 leader 同时返回矩阵

leader(v) 返回 `(根,W[v])`，递归中严格执行父矩阵乘子矩阵；merge 使用
`W[reference]*X*inverse(W[v])`，按负大小反接时再取逆。
Mat 的四个字段是模整数，op 写八次包装后的乘法再相加，未采用前两份
“两个乘积先累加、只做一次模归约”的裸整数表达式。

主路径还有两项可见常数开销：merge 已得到根后，size(root) 再调用
leader，虽根查询很短；设置新根边时又与新根的单位势相乘。
diff 使用 `(同根).then_some(矩阵表达式)`，Rust 调用参数先求值，
所以即使两根不同，这个表达式也会计算后再包装成 None。
这都不改变正确性或 α 摊还阶，但不同于“发现不同根就立即跳过矩阵算术”。
I/O 是定制的 Unix Input 与查表 Output，群模板中 Reverse 等未实例化。

### #332456：Montgomery 改的是标量模乘表示

实际用 `mld::potentialized_union_find<matrix_mul>`。矩阵四个分量
都是 `montgomerymodint998244353`，以 `xR mod M、R=2^32` 编码，
相乘时通过低 32 位抵消和取高半部完成模约减。内部允许 `[0,2M)`
的剩余，比较/输出时再规范化；M 为奇数且小于 `2^30`，满足其范围条件。

这把每个标量 `%M` 换成乘法、移位和加减，但输入要编码、输出要解码，
矩阵一次 op 仍有八次标量乘法。模板里有通用模逆/幂，矩阵逆却仍直接
交换对角和取负非对角，没有调用模幂求逆。
主程序先 potential(u,v) 判断是否已有关系，不同根才 merge，再查一次根；
I/O 是关闭同步的标准 cin/cout。27 ms 不能直接证明 Montgomery 内核
本身比裸点积差，接口调用、归约次数和读写均不同。

## 如何验证乘法次序与优化效果

本地 [`path_compression.cpp`](../../../problems/data_structure/unionfind_with_potential_non_commutative_group/path_compression.cpp)
与 [`union_by_size.cpp`](../../../problems/data_structure/unionfind_with_potential_non_commutative_group/union_by_size.cpp)
对应有/无压缩，见 [`tutorial.md`](../../../problems/data_structure/unionfind_with_potential_non_commutative_group/tutorial.md)。
无压缩但按大小的最坏树高 `O(log N)`，可以逐边检查积次序；压缩版
则用较少的后续矩阵乘法换取父指针写入。

小例应该刻意用 PQ≠QP 的剪切矩阵，若只用单位矩阵或对角矩阵，许多
乘法顺序错误会被掩盖。还要测试反向查询、触发反向按大小合并、同点非单位
约束的拒绝、合法闭路、矛盾后原关系不变，以及 0/M-1 分量。
性能实验应固定输入与 I/O，分别替换根查找调度、四点积与逐模整数、
普通约减与 Montgomery，并统计实际矩阵乘法次数。
本篇仅核对本地缓存和例子，没有新 OJ 请求或性能 benchmark；历史 tutorial
中的实测比值不当成这次前五之间的纯内核比较。
