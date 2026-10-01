# 稀疏幂级数幂

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 94.301 (sakikuroe), sum 1208.760 (sakikuroe)。

- `main.cxx` max: 70.392 ms (-25.35%), sum: 516.497 ms (-57.27%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
剥离前导 x 次幂，用 f*g'=k*f'*g 递推；前导次数乘指数用 u128 防溢出。

接口、复杂度及验证见 [`fps_sparse.md`](../../../include/toy/fps_sparse.md)。

## 尝试过程与取舍

从 f*g'=k*f'*g 的稀疏递推开始，保留前导次数与常数因子的处理，再比较 dot 候选的批量内积路径。前导次数乘原指数用 u128 检查范围；指数的模约减只用于系数递推，不用于决定输出次数。
