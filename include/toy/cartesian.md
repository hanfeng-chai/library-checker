# cartesian

`cartesian_tree(values,compare=less<T>{})` 返回父下标 Buffer，根的父亲为自己，
空数组返回空数组。默认构造最小堆笛卡尔树；相同值优先较早位置，compare 可反转
优先级。比较关系须为严格弱序。

单调栈保存已处理前缀的最右路径，新节点取代比自己大的栈尾，最后弹出的子树
成为其左孩子。每个元素至多入栈、出栈一次，时间、空间均为 O(n)。
独立递归选最小值的对照包含重复值和空输入，GCC/Clang 与 sanitizer 通过。

栈项同时缓存原值与下标，比较栈顶时不用再间接读取 values[stack_index]。
