# 二元幂级数求逆

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 369.345 (sansen), sum 6256.204 (sansen)。

- `main.cxx` max: 159.192 ms (-56.90%), sum: 2251.066 ms (-64.02%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
要求 f*g=1 mod (x^n,y^m)。先从首行的一元逆出发，沿较短维度倍增精度，
用 Newton 公式只补出逆的新高半段。

行内补零后用一元 NTT 计算二维乘积，每轮裁去超出另一维精度的项，
避免串入相邻行；单行、单列退化为一元求逆。
实现与约束见 [fps_2d](../../../include/toy/fps_2d.md)。

## 尝试过程与取舍

把二维截断求逆化为沿较短维度的 Newton 倍增，再通过行内补零的一元 NTT 做乘积。每轮裁剪另一维，避免相邻行串扰；单行、单列直接退化为已有一元接口。验证先检查二维朴素卷积，再进行完整对照。
