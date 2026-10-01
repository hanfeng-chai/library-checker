# Run Enumerate

<!-- benchmark-summary -->
2026-10-02 Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。
参考最佳（5 份）：max 23.821 (Rohan_Kapri), sum 203.472 (Rohan_Kapri)。

- `main.cxx` max: 22.011 ms (-7.60%), sum: 195.633 ms (-3.85%)

明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。
<!-- /benchmark-summary -->
两种相反的字母序分别扫描 Lyndon 单调栈。候选根向左右做 LCE 扩展，形成
极大周期区间；如果左边已有一个完整周期，交给更早的根输出。栈比较得到的
公共前缀直接复用，首字符不等时不进入 LCE 查询。

LCE 先直接比较，每个方向设置线性工作预算；不足时建立 SA/LCP/RMQ，再做
精确查询。全相同串直接验证并输出唯一 run。结果按周期、左右端点做 radix
排序去重，输出使用批量六位整数格式化。整个算法不使用概率哈希。

库见 [string_runs.md](../../../include/toy/string_runs.md)。
