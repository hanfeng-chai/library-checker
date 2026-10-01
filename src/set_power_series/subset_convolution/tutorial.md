# 子集卷积

<!-- benchmark-summary -->
2026-10-01 Lenovo，两轮全量逐测例取较小 task-clock；单位 ms，包含样例。
五份参考最佳：max 305.966 (adamant), sum 1515.430 (Rohan_Kapri)。

- `main.cxx` max: 266.892 ms (-12.77%), sum: 1324.617 ms (-12.59%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
按非空集合的大小分层，分别做 XOR 变换，在频点上相乘秩多项式，逆变换后
只取与目标集合大小相同的秩。因为 |A|+|B|=|A xor B| 当且仅当 A、B 不交，
保留的正是子集卷积所需项。

秩的奇偶性决定掩码最低位，因此只变换 mask>>1，规模减半；空集贡献单独补回。
时间 O(n² 2^n)，工作区 O(n 2^n)。实现和验证见
[subset_convolution](../../../include/toy/subset_convolution.md)。

## 尝试过程与取舍

先实现按集合大小分层的经典 zeta/逆 zeta 子集卷积，再比较补齐布局与大页提示。随后改为缩半 XOR 内核，并依次验证融合变换、频点操作和工作区复用。最后整理直接路径与通用接口；本题与多维卷积的全二元分支共用实现，因此每次共享修改都一并回归。
