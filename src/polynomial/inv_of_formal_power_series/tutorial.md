# 形式幂级数求逆

已知 `fG=1 mod x^m`，令 E 为 fG-1 的高 m 项，新的高半段就是 `-GE`。
长度 2m 的循环卷积只会污染低半段，所以保留所需高段即可，G 的频谱可复用。

实现与验证见 [`fps.md`](../../../docs/fps.md)。
原来迁移的 [`analysis.md`](../../../submissions/polynomial/inv_of_formal_power_series/analysis.md)
还说明了最快提交的三次乘积恢复等方案；目前较简单的实现已达到性能要求。
