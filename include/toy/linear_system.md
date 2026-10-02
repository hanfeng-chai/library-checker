# linear_system

`FieldEchelon<P>(n,width,columns,input,reduced=false)` 消去素数域上的行优先矩阵。
前 columns 列是系数，其余列作为右端或变换矩阵一起更新。P 为小于 2^30 的
奇素数，输入是 `[0,P)` 的规范剩余类。

`pivot` 保存递增的主元列，长度就是秩。reduced=true 同时消去主元上方的元素，
可直接读出特解、零空间和逆矩阵。`get(i,j)` 返回规范剩余类；内部 row 值仍可能
是未约减的 u64，不能直接当作答案。scale 是带行交换符号的主元乘积，只在
方阵满秩时等于行列式。

主元行规范化，目标行采用四路 AVX2 u64 累加。每八次更新减去 8P²，使累加值
始终小于 16P²，避免逐乘积取模。复杂度 O(n·columns·width)，空间 O(n·width)。
