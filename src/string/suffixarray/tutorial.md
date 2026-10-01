# Suffix Array

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 39.476 (nandhagk), sum 610.955 (nandhagk)。

- `main.cxx` max: 28.014 ms (-29.03%), sum: 477.580 ms (-21.83%)
- `naive.cxx` max: 56.461 ms (+43.03%), sum: 935.566 ms (+53.13%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
SA-IS 对 LMS 子串命名、递归排序，再诱导全部后缀。LMS 位置不相邻，编号表
只需按位置除二存储；位图用于从已排序后缀中提取 LMS。诱导项的符号记录前驱
属于 L 还是 S，避免随机读取另一份类型数组。只有两个不同符号时直接用
正反向队列诱导，不再扫描空槽或维护符号位。

`main.cxx` 在此基础上尝试短周期、有界前缀、交替最小分隔符、短 LMS 直接
命名等精确证书，分别减少排序规模或省去诱导步骤。完整验证成功才采用；
重复前缀未分出顺序、桶过大等情况均回退 SA-IS。`naive.cxx` 保留
不使用这些证书的标准 SA-IS，仍共用相同诱导排序核心和批量输出模板。

接口与证书规则见 [suffix_array.md](../../../include/toy/suffix_array.md)、
[suffix_order.md](../../../include/toy/suffix_order.md)。
