# 大规模卷积

沿用普通模卷积内核。部分 NTT 在八项多项式处停止，因此长度 N 只需要 N/8 阶单位根，可覆盖本题超出完整 NTT 根容量的数据。

接口、约束及验证见 [`convolution.md`](../../../docs/convolution.md)。
