# Sum of Floor of Linear / 线性函数下取整之和

## 中文

计算

`sum(i=0..n-1) floor((a*i+b)/m)`。

先拆出 `a/m` 和 `b/m` 的整数部分：

- `a/m` 贡献 `(0+...+n-1)*(a/m)`；
- `b/m` 对每项都贡献一次，共 `n*(b/m)`。

于是可以令 `0<=a,b<m`。此时若 `a*n+b<m`，所有剩余项都是零。否则
把直线下方的格点图形转置：新的项数为 `(a*n+b)/m`，并交换斜率中的
`a` 与 `m`。这正是欧几里得算法，所以每轮参数都会缩小。

### 两种解法

- `recursive_euclidean.cpp`：递归写法，直接对应“拆整数部分—转置”的数学推导。
- `iterative_euclidean.cpp`：等价迭代写法，避免函数调用并保持所有状态在寄存器中。

二者共享 `toy/integer.hpp`，时间 `O(log m)`；递归版空间 `O(log m)`，
迭代版额外空间 `O(1)`。

两份源码均使用 direct mapping，查询数为六位路径，四个参数按
`[0,10^9]` 的十位 value-uniform 路径读取，并共享整题实测更快的普通
Writer。递归版只简化数学核心。

#

## 正确性直观

拆出的整数部分显然逐项相等。剩余部分统计直线
`y=(a*x+b)/m` 下的整格点数；交换横纵坐标不会改变格点总数，只改变
参数表示，因此递归保持答案。

## English

Remove whole multiples from `a` and `b`, then transpose the lattice region
below `y=(a*x+b)/m`. This swaps the Euclidean parameters and yields
`O(log m)` complexity.

`recursive_euclidean.cpp` follows the recursive derivation.
`iterative_euclidean.cpp` implements the same invariant iteratively with
`O(1)` auxiliary memory.

Both use direct mapping, six- and ten-digit input policies, and the same
whole-problem-validated Writer.
