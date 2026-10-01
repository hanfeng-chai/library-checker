# Enumerate Palindromes

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（6 份）：max 19.598 (kostylevGO), sum 228.285 (kostylevGO)。

- `main.cxx` max: 12.019 ms (-38.67%), sum: 165.158 ms (-27.65%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
Manacher 直接在字符中心、间隙中心之间交替扫描，半径直接表示回文长度，
无需构造带分隔符的新串。镜像回文完全落在已知范围内时直接复制；碰到边界
才继续比较。共 `2*n-1` 个答案使用 `write_bulk6` 输出。

时间与空间 O(n)，接口见 [string_basic.md](../../../include/toy/string_basic.md)。
