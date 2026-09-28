# 双端队列整体复合：为什么要分半重平衡，而不能总把全部函数搬过去

## 1. 题目与运算顺序

维护一列仿射函数f(x)=ax+b，可以向两端插入、从两端删除，
并询问按队首到队尾顺序依次作用于x的结果，计算模998244353。
操作码0/1为前/后插入，2/3为前/后删除，4为求值，
空序列返回x，见[题面](../../../upstream/data_structure/deque_operate_all_composite/task.md)。

用f(x)=2x+1、g(x)=3x+4、h(x)=5x+6：

| 操作 | 函数队列 | 对x=5的结果 |
|---|---|---:|
| 尾插g | [g] | 19 |
| 头插f | [f,g] | g(f(5))=37 |
| 尾插h | [f,g,h] | h(g(f(5)))=191 |
| 删尾 | [f,g] | 37 |
| 删头 | [g] | 19 |

先l后r的合并公式为
(al,bl)⊙(ar,br)=(ar·al, ar·bl+br)。
它满足结合律，但不交换；恒等函数为(1,0)。
例如f⊙g=(6,7)，g⊙f=(6,9)，交换方向就会改变答案。
更完整的函数推导见[队列聚合教程](../queue_operate_all_composite/analysis.md)。

直接在每次查询遍历全部函数，最坏O(Q²)；
若使用一个居中位置数组和线段树维护区间复合，可将每次操作做到O(log Q)。
本题还可以利用只有两端增删的限制，做到摊还O(1)。

## 2. 两边各存一条带聚合的栈

把逻辑序列分成左半与右半，左右两边各存原函数和相应方向的聚合。
左栈的顶部就是队首，因此新增函数x的聚合为x⊙旧左聚合；
右栈的顶部是队尾，新增函数x的聚合为旧右聚合⊙x。
整个序列的聚合为左聚合⊙右聚合。

两边都有元素时，插入/删除只改自己一侧栈顶，查询读取两个聚合即可。
困难在于请求从一个空侧删除，而元素全在另一侧。
单端队列可以把另一栈全搬过来；
双端队列如果照搬这个策略，下一次从相反端删除又可能全部搬回。

例如6个元素都在右侧，依次删头、删尾、删头、删尾：
如果每次都全搬，搬运数量接近6+5+4+3+…，出现二次工作。

## 3. 正确的重平衡：让两侧各保留约一半

当左侧为空、右侧逻辑顺序为[f1,f2,f3,f4,f5,f6]时，
将前半作为左侧，后半留在右侧：

~~~text
逻辑左侧：[f1,f2,f3]    左栈底到顶：[f3,f2,f1]
逻辑右侧：[f4,f5,f6]    右栈底到顶：[f4,f5,f6]
~~~

随后删除队首f1，左边还有f2、f3；再删除队尾f6，右边还有f4、f5。
两端交替操作无需立即重建。
重建仅改变内部的分界和存储方向，不改变逻辑函数序列，
并按各自方向重新计算聚合，所以正确性不变量保持成立。

### 3.1 摊还界怎样证明

用两侧大小差的绝对值作为势能的一部分，Φ=c·|L−R|。
普通一次插入或删除只使大小差变化至多1，势能最多增加常数c。
一侧为空、另一侧有k项时，重建成本O(k)，
而均分后大小差降到至多1（再删一个也只差常数），
势能下降约ck，足够支付重建。

因此整个序列总成本O(Q)，单次更新摊还O(1)、查询最坏O(1)、空间O(Q)。
与单端标准双栈不同，一个元素可能参与多次重平衡，
所以这里应使用均衡势能证明，不能声称每个元素只搬一次。

## 4. 上游的真实实现：四个vector与重建

[correct.cpp](../../../upstream/data_structure/deque_operate_all_composite/sol/correct.cpp)
使用A[0]/A[1]存两边原值，Prod[0]/Prod[1]存各自聚合。
Prod开头都放单位元，使空侧也可直接读取back()。
recalcProd从头重建两条聚合：

~~~text
左侧：Prod0[i+1] = A0[i] ⊙ Prod0[i]
右侧：Prod1[i+1] = Prod1[i] ⊙ A1[i]
~~~

删除侧为空时，上游构造两个新vector，
把另一侧近似均分、反转需要成为左栈的一段，再调用recalcProd。
查询返回(Prod0.back()⊙Prod1.back())(x)。

所以基线已经有正确的摊还线性算法。
公开快解主要省去重平衡时的原值复制/临时分配，
改用连续固定空间，或减少查询时的模乘。

## 5. 居中数组：原函数不搬，只移动中点并重算聚合

若总操作数至多Q，可以分配约2Q个条目，将空序列放在中间。
每条目存value和aggregate；有效区间[begin,end)，中点middle把左右分开。
向前插入减begin，向后插入增end，删除反向移动端点。
总共最多Q次端点移动，因此无需回绕或整体搬家。
这依赖**总操作预算**，不是仅依赖同时存活的元素个数。

左半每个位置保存从该位置到middle−1的有序后缀，
右半每个位置保存从middle到该位置的有序前缀。
查询取data[begin].aggregate和data[end−1].aggregate。
重平衡只重新选middle，再扫描两边填写aggregate，
value仍留在原槽，省去上游两个新vector的分配与原值复制。

此外，若查询只需要最终数值，
可先执行左聚合L(x)，再执行右聚合R(L(x))。
与先构造R∘L再求值相比，两侧都非空时可少一次模乘；
这是第一名apply接口的实际用途。

## 6. 当前前五逐份核对

本表依据2026-09-26 05:09:30 UTC的
[config](../../selected/deque_operate_all_composite/config.json)与
[global.json](global.json)，均为当时最新测试版本AC。
没有据此声称各算法族的全站最快都包含在五位用户中。

| 排名 | 提交/用户 | OJ时间 | 主路径 |
|---:|---|---:|---|
| 1 | [403007.cpp](../../selected/deque_operate_all_composite/403007.cpp)，nandhagk | 11ms | 居中数组、删除后重平衡、两段直接apply |
| 2 | [393898.cpp](../../selected/deque_operate_all_composite/393898.cpp)，chaihf | 13ms | 固定居中数组、删除前重建、fold后求值 |
| 3 | [211474.cpp](../../selected/deque_operate_all_composite/211474.cpp)，oldyan | 14ms | 一个全局连续池，两侧视图改游标 |
| 4 | [363683.rs](../../selected/deque_operate_all_composite/363683.rs)，urectanc | 15ms | 四个Vec、分半转移/反转并重建 |
| 5 | [401530.cpp](../../selected/deque_operate_all_composite/401530.cpp)，Rohan_Kapri | 17ms | 与第二名执行代码相同，注释/换行有别 |

### #403007：合并两端接口，也利用仿射公式的共同部分

centered_aggregate_deque用top_[2]与cut_[2]表示两端和分界，
runtime的sequence_end选择方向。
end_fold的仿射特化发现：两种插入方向的乘法系数都是a_new·a_old，
只有平移项的三个操作数不同；源码用掩码选择这三个操作数，
共用一次公式，而不是分成两套完整的复合计算。
它是否更省分支成本需要实际CPU测量，掩码操作也有成本。

当删除来自当前空的聚合侧时，它先从原值区删除目标，
再对剩下的序列均分重建，避免为即将删除的函数计算聚合。
main查询调用apply，直接执行左右两段函数，不构造整个复合。
预留槽含端点哨兵，构造参数是操作/漂移预算；并非可无限入删的通用循环容器。

### #393898 / #401530：FixedFoldableDeque，而非线段树

main实例化FixedFoldableDeque<Affine,ComposeAffine,500000>。
文件里虽然打包了线段树和多种其他仿射结构，
这些不在本题的执行路径；不能按模板关键词把它误标为线段树解法。
两个版本的执行代码一致，差别在bundle标记、注释/空行与换行符。

当begin==middle时，pop_front先rebuild_left，再增加begin；
pop_back对称。重建只移动middle并扫描聚合，不复制原函数。
fold按左右是否为空返回单位元、单侧或两侧组合，
main再将所得函数作用于x。专用I/O与第一名同样是端到端时间的一部分，
13/17ms不能由源码相同的两条记录推出算法变化。

### #211474：底层adapter决定是否真的搬元素

main实例化GlobalContinuousInfoDeque<…,1000000>。
这两个StaticContinuousAdapter共享一个居中全局池，
_trans_left/_trans_right因adapter的is_special为真，
实际只修改区间指针并重算m_sum。
模板里的drop_head搬运分支服务于其他adapter，本题没有走。

main在空队列时直接输出x，否则query_all().calc(x)。
cin/cout是OY LinuxIO宏替换。
与上游相比省掉临时vector，和第二名一样属于居中固定存储的重平衡路线。

### #363683：通用Rust双端聚合容器

FoldableDeque保存front、back及两条acc向量。
某侧为空时，swap/split_off/append/reverse将约一半元素移到该侧，
rebuild重算两边聚合；这些是真实执行的移动和分配管理，
不像上面三种居中池只改分界。
算法仍为摊还O(1)，但更接近可增长的通用容器接口，
无需预先承诺全部端点操作的漂移预算。
模数运算与快输入输出来自urectanc库，查询走fold再求值。

## 7. 怎样选择实现并验证

若只需完成本题，固定连续池可以把已知Q上界换成较简单的热路径；
若需要长期运行、容量动态增长的库接口，vector重平衡更自然；
若需要每次操作的最坏延迟有明确界，线段树的O(log Q)也是可用替代。
当前前五都使用均衡两侧聚合，不等于其他路线不存在快提交。

[本地教程](../../../problems/data_structure/deque_operate_all_composite/tutorial.md)
记录了两种路线及既有同机实验，其中
[centered_segment_tree.cpp](../../../problems/data_structure/deque_operate_all_composite/centered_segment_tree.cpp)
是另一份本地实现，与当前个人#393898的主路径应区别开。

正确性对照可直接保存函数vector，每次查询逐个作用，
重点测试交替两端删除、全部元素在一边、单元素反复切换、
空队列查询和不可交换函数。
本文核对了小例、方向、实际adapter/容器入口与五份哈希，
没有新增性能benchmark。
