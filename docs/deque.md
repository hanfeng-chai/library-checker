# deque

`Deque<T>(capacity)` 分配居中的连续缓冲，生命周期内向任一端的 push 总数
不能超过 capacity。支持 push_front/back、pop_front/back、size 和下标访问。
操作 O(1)，空间 O(capacity)；调用方保证 pop 和下标有效，T 是平凡可复制类型。
