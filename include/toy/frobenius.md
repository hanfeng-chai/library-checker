# frobenius

`characteristic_polynomial<P>(n,a)` 返回 `det(xI-A)` 的升幂系数；三角矩阵完整验证
后直接相乘对角线的一次因子，否则构造 Krylov 链，求各循环商空间的多项式再相乘。

`matrix_power<P>(n,a,k)` 支持 u64 指数。一般矩阵构造块对角 Frobenius 表示，
在每个块中计算 `x^k mod f`，最后变回原坐标。每行至多一个非零项的矩阵先完整
验证，再用带权函数图倍增处理。指数 0、1、2 直接处理。

`Frobenius<Full,P>` 保存块多项式；Full=true 另保存原始 Krylov 行和逆变换。
消元系数记录每条链的依赖关系，若块仍耦合到早先的链，则修正生成元并重新
验证关系。生成元使用固定种子的伪随机数；无法确认块独立时重建，不接受未经
验证的相似分解。随机化影响运行时间，成功返回的代数关系是精确的。

矩阵向量乘使用八项累加和 Montgomery 约减，向量消元共用 Shoup 核。
`polynomial_x_power` 复用 `PolynomialMod` 的平方和约减，支持 64 位指数。
输入均为行优先规范剩余类；主要工作是 O(n³) 的域运算，空间 O(n²)。
