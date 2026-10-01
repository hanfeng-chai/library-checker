# prime

`primes(limit)` 返回不超过 limit 的所有素数，升序存于 `Buffer<u32>`。
只筛奇数，位图每位表示一个奇数，适用于 GCD/LCM 卷积等百万规模上界。

时间 O(limit log log limit)，空间 O(limit)。旧 `prime.hpp` 的大范围分段筛
仍保留，尚未纳入这轮打磨；本接口没有替代其全部能力。
