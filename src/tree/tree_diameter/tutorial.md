# 树的直径

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 78.509 (IceKylin), sum 486.693 (IceKylin)。

- `main.cxx` max: 72.205 ms (-8.03%), sum: 450.703 ms (-7.39%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
`main.cxx` 使用 diameter.h 的剥叶算法。每个点存剩余度数、邻居编号和边权的 xor；
成为叶子时，这两个 xor 就是唯一剩余边。将最长向下路径传给父亲，并检查与
父亲已有最长路径相接后的候选直径，最后沿保存的父指针恢复两个端点的路径。
非负边权下时间、空间均为 O(N)，无需邻接表与递归；单点树返回长度零的单点路径。

16 个官方测例通过 GCC/Clang，Floyd–Warshall 距离及返回路径的独立对照通过
ASan/UBSan，覆盖任意根与零权边。

## 尝试过程与取舍

采用经典树形 DP，但通过度数和邻居异或剥叶，避免保存完整邻接表。每点合并两条最长下行路并保留端点，最后恢复路径；零权边和单点树在独立全点对对照中验证。首批完整测量双项领先。
