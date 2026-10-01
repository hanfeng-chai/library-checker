# heap

`BinaryHeap<T,Minimum=false>` 维护可平凡复制、可比较的值，默认最大堆。
push/pop/top/empty/clear 是普通堆操作；replace_top(x) 用 x 替换堆顶并返回旧值，
push_pop(x) 等价于先插入再弹出，但合并为一次调整。后三种取值操作要求非空。

values 是 Buffer，可批量填入后调用 heapify() 线性建堆。push 按需扩容，
其他操作不分配。LIS 的两种块堆覆盖了普通与复合调整，并通过独立递推与 sanitizer。
