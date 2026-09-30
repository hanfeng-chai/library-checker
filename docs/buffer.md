# buffer

`Buffer<T>` 是仅用于平凡可复制类型的 64 字节对齐动态数组，使用 libc 分配，
无需 libstdc++。`p/n/capacity` 分别是数据、长度和容量；支持下标及转为 span。

构造 `Buffer<T>(n, capacity)` 不初始化元素。`resize(n)` 扩大时补零；
`reserve(capacity)` 只扩容，保留原数据。禁止复制，可移动，析构自动释放。
算法按值接收 Buffer 时，用 `std::move` 交出所有权，便于复用输入存储。
