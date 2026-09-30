# offline_frequency

`point_value_frequencies(value_count,initial,final,events,answer_count)` 返回按编号排列
的区间频率。initial/final 为同一数组在全部事件前后的状态，值已压缩到
[0,value_count)。事件按时间排列：修改由旧值移除与新值加入两个 change 组成；
query(value,l,r,id) 统计半开区间中该值的次数。

按值稳定分组事件，组内仍按时间执行。每组在同一份位图上装入初始位置、执行
修改与查询，最后移除最终位置，恢复空位图供下一组使用。没有查询的组跳过。
每 64 个位置的置位数量由 PrefixTree32 汇总，查询通过两个前缀计数之差完成。
空间 O(n+events+value_count)，时间 O(n+events+value_count) 加位图索引操作的
O((n+events)log n) 上界。支持空数组、重复赋值和空查询区间。
