# minmax_heap

`MinMaxHeap(std::move(values),capacity)` 线性建堆，可选预留容量。
push、pop_min、pop_max 为 O(log n)；min、max、size 为 O(1)。查询、删除要求非空。
T 为平凡可复制类型，可指定严格弱序比较器，允许重复值。

二叉堆的层交替维护最小、最大关系；最小值在根，最大值在根的两个孩子中。
上浮每次跨两层，下沉优先选择四个孙子中的极值，并校正中间父节点的相反序关系。
