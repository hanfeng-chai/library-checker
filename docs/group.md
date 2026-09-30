# group

AdditiveGroup<P> 是模 P 加法群，Matrix2Group<P> 是行列式为一的 2×2 矩阵群。
两者提供 Value、identity、multiply、inverse，系数使用普通余数；默认 P=998244353。
矩阵值为 Value{a,b,c,d}，乘法按行列顺序；行列式为一时逆矩阵只需交换对角元并取负。
模数约束同 [mod](mod.md)。
