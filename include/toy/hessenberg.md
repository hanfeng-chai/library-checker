# hessenberg

`characteristic_hessenberg<P>(n,a)` 返回 `det(xI-A)` 的升幂系数。
用行消元和对应的逆列变换得到上 Hessenberg 矩阵，再递推各阶主子式的
特征多项式。行更新共用 Shoup 向量核，列更新使用同样的固定乘数约减。

时间 O(n³)，空间 O(n²)，不使用随机化；作为 Frobenius/Krylov 方法的经典对照。
