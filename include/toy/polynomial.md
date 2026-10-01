# polynomial

`polynomial_divmod<P>(std::move(f),g)` 返回商和余数两个 Buffer。
输入最高项非零，除数非空，系数为普通模余数。
倒序后用分块幂级数除法求商，再倒回原顺序；余数只减去乘积的低段。
短除数或短商直接向量化长除。裁去余数末尾的零，零多项式为空数组。
时间 O(N log N)，工作区 O(N)。

`polynomial_taylor_shift(std::move(f),c)` 返回 f(x+c)，用阶乘缩放后的一次卷积。
`polynomial_eval_geometric(f,m,a,r)` 返回 f(ar^i)，支持 a/r 为零及重复求值点。
用三角数指数构造一次循环卷积，补齐到 bit_ceil(n+m-1)。

`polynomial_interpolate_geometric(std::move(values),a,r)` 要求求值点互异。
用几何点的显式导数权重、幂和及 Gaussian 二项式恢复系数；
阶数 0/1、r=0、r^n=1 单独处理。

`polynomial_product(span<Buffer<u32>>)` 折叠常数后，用最小堆优先合并较短多项式，
会移动输入条目。序列乘积题的主解使用 [product_pool](product_pool.md)，
按总次数平衡分治并复用连续工作区。

`polynomial_shift_samples(values,m,c)` 将次数小于 n 的多项式在 0..n-1 的值
转换为在 c..c+m-1 的值。阶乘重心权重配合一次循环卷积，分母批量求逆；
已知采样点直接取值，滑动积处理经过零点和模数回绕。

`polynomial_prefix_sum(f)` 返回 g，满足 g(0)=0、g(x+1)-g(x)=f(x)。
用 `1/((exp(x)-1)/x)` 的系数生成 Bernoulli 数，再做一次阶乘缩放卷积。
涉及阶乘、积分或整数逆元的接口要求相关整数阶数小于模数，变换长度符合根容量。

各接口已通过 GCC/Clang 官方测例、朴素对照及 ASan/UBSan。
当前性能及原始证据统一见 [评测审计](../../tools/measurement_audit.md)。
