# Wildcard Pattern Matching

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 14.791 (Rohan_Kapri), sum 224.479 (adamant)。

- `main.cxx` max: 11.422 ms (-22.78%), sum: 103.978 ms (-53.68%)
- `naive.cxx` max: 68.768 ms (+364.94%), sum: 773.955 ms (+244.78%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 先用精确过滤。非通配的不等字符对很少时直接枚举；否则为每个
字符建立文本兼容位置的位图，每个模式字符平移位图并与候选取交。候选清空
的块立即移除。模式位置按互质步长遍历，使前几个约束分布在整个模式串上，
避免连续通配区域造成大量重复的无效过滤。工作预算耗尽则回退 NTT。

`naive.cxx` 保留纯 NTT。每个不匹配字符对贡献 `(a-b)^2`，通配贡献零；
展开为三个卷积，六次正变换后将乘积相加，只做一次逆变换。最大总值小于
998244353，结果为零当且仅当匹配，没有模数碰撞和浮点舍入误判。

参考提交 Rohan_Kapri 使用随机浮点投影：本次检查曾在同一 `random_ab_04`
输入上出现一次错误，随后五次运行及重新检查通过。原提交保持原样，时序结果
照常列出；它的偶发误判不作为自有实现可接受的行为。

接口见 [wildcard_match.md](../../../include/toy/wildcard_match.md)。
