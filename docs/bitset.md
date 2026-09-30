# bitset

`BitSet(n)` 建立 [0,n) 的空整数集合，也可用 0/1 字符串初始化。
insert/erase/contains 要求键在范围内；next(x) 返回不小于 x 的最小元素，
previous(x) 返回不大于 x 的最大元素，无解返回 -1。next 要求 x≥0。
支持 n≤2^31-1，允许空集合。

底层 Bitmap 每层用一位标记下一层的非空 64 位字，更新只在整字状态改变时传播。
修改返回是否改变该位，以算术方式维护元素数量，避免重复检查带来的随机分支。
不超过 16 个元素时，第一次查询缓存有序键；后续用两组 AVX2 比较和 popcount 求秩。
每次修改使缓存失效。较大集合先无分支选择当前字内的前驱/后继，失败再查高层。

`bound(key,reverse)` 要求 key<n，reverse 为 true 表示前驱；主解用它合并两种查询方向。
初始化字符串使用 AVX2 比较与 movemask。基本位图操作 O(log_64 n)，存储 O(n/64)。
小集合缓存至多重新枚举 16 个键；const 查询可能刷新缓存。

GCC/Clang 官方及边界、随机修改对照和 sanitizer 已通过。性能见对应 tutorial。
