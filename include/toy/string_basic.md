# string_basic

三个线性算法共用 `common_prefix(a,b,limit)`：先排除首字符不等，再用 64 位异或找出前八字节
的首个差异，再用 AVX2 每次比较 32 字节，尾部按八字节和单字节处理。所有加载都在 `limit` 内，不要求输入尾部填充。

- `z_function(s)`：返回 Z 数组，`z[0]=s.size()`。
- `lyndon_factorization(s)`：Duval 分解，返回包含 0 和 n 的所有分界点。
  相等区间一次跳过，减少长周期字符串上的逐字符比较。
- `palindrome_lengths(s)`：Manacher，依次返回字符中心、间隙中心的最长回文长度，
  共 `2*n-1` 个；空串返回空数组。镜像完全位于当前回文内时直接复制答案。

时间、输出空间均 O(n)。返回值拥有存储；字符比较按 ASCII 顺序。
