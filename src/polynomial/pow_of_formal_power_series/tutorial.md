# 幂级数的整数幂

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 131.660 (rogeryoungh), sum 1369.752 (L1ngYu)。

- `main.cxx` max: 120.586 ms (-8.41%), sum: 1091.221 ms (-20.33%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
先写成 f=x^s*c*h，h(0)=1。原整数指数决定偏移 s*k，常数因子为 c^k；
只在正规化级数的微分方程中将 k 模 P，不能提前把次数偏移中的指数取模。

[fps](../../../include/toy/fps.md) 用分块方程 h*Dg=k*(Dh)*g 直接求 g=h^k，
缓存各块频谱，逐块求残差、乘首块逆、除以对应次数，再乘回 g 的首块。
首块递归，避免分别计算整个 log 和 exp。

37 个官方测例通过 GCC/Clang。当前实现还将块间残差、导数和整数除法向量化，
无需次数位移时直接返回结果缓冲。性能见 [评测审计](../../../tools/measurement_audit.md)。

## 尝试过程与取舍

从 log/exp 路径建立基线，依次尝试分块、relaxed 路线、大页和紧凑存储、不同缓存参数。单纯改变内存布局仍有 max 差距，随后改为直接分块求解微分方程，缓存各块频谱。最后将残差、导数和次数除法向量化；无需次数位移时直接返回结果缓冲，避免额外清零和复制。次数偏移始终使用原整数指数，不能提前按模数约减。
