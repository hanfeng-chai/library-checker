#pragma once

#include <bits/extc++.h>

namespace toy {

using u64 = uint64_t;
using u128 = __uint128_t;

[[nodiscard]] inline bool power_leq(u64 base, unsigned exponent, u64 limit) {
    u64 result = 1;
    while (exponent) {
        if ((exponent & 1) && __builtin_mul_overflow(result, base, &result)) return false;
        exponent >>= 1;
        if (!exponent) break;
        if (__builtin_mul_overflow(base, base, &base)) return false;
    }
    return result <= limit;
}

[[nodiscard]] inline u64 kth_root_floor(u64 n, unsigned k) {
    if (k == 1 || n <= 1) return n;
    if (k == 2) {
        u64 estimate = std::sqrt((long double)n);
        while (power_leq(estimate + 1, 2, n)) ++estimate;
        while (!power_leq(estimate, 2, n)) --estimate;
        return estimate;
    }
    if (k >= 64) return 1;
    if (k >= 32) {
        u64 result = 1 + (n >= (1ULL << k));
        if (k > 40) return result;
        u64 power_of_three = 1;
        for (unsigned i = 0; i < k; ++i) power_of_three *= 3;
        return result + (n >= power_of_three);
    }
    double reciprocal = std::nextafter(1.0 / k, 0.0);
    u64 estimate = std::pow((double)n, reciprocal);
    while (estimate < UINT64_MAX && power_leq(estimate + 1, k, n)) ++estimate;
    while (!power_leq(estimate, k, n)) --estimate;
    return estimate;
}

[[nodiscard]] inline u64 kth_root_floor_binary(u64 n, unsigned k) {
    if (k == 1 || n <= 1) return n;
    u64 low = 0, high = n;
    while (low < high) {
        u64 middle = low + (high - low) / 2 + 1;
        if (power_leq(middle, k, n))
            low = middle;
        else
            high = middle - 1;
    }
    return low;
}

template <class Int>
[[nodiscard]] std::vector<Int> enumerate_quotients(Int n) {
    std::vector<Int> result;
    Int boundary = 1;
    for (; boundary * boundary < n; ++boundary) result.push_back(boundary);
    std::size_t split = result.size();
    for (Int k = 1; k * boundary <= n; ++k) {
        result.push_back(k & 1 ? n / k : result[split + k / 2 - 1] / 2);
    }
    std::reverse(result.begin() + split, result.end());
    return result;
}

template <class Int, class Callback>
[[nodiscard]] u64 enumerate_quotients(Int n, Callback &&callback) {
    Int root = std::sqrt((long double)n);
    while ((root + 1) <= n / (root + 1)) ++root;
    while (root > n / root) --root;
    Int small = (root * root + root <= n) ? root : root - 1;
    for (Int value = 1; value <= small; ++value) callback(value);
    for (Int divisor = root; divisor; --divisor) callback(n / divisor);
    return root + small;
}

[[nodiscard]] inline u64 floor_sum(u64 n, u64 m, u64 a, u64 b) {
    u64 result = 0;
    for (;;) {
        if (a >= m) {
            result += n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            result += n * (b / m);
            b %= m;
        }
        u64 y = a * n + b;
        if (y < m) return result;
        n = y / m;
        b = y % m;
        std::swap(a, m);
    }
}

[[nodiscard]] inline u64 floor_sum_recursive(u64 n, u64 m, u64 a, u64 b) {
    u64 result = n * (n - 1) / 2 * (a / m) + n * (b / m);
    a %= m;
    b %= m;
    u64 y = a * n + b;
    if (y < m) return result;
    return result + floor_sum_recursive(y / m, a, m, y % m);
}

namespace detail {

inline u64 min_mod_recursive(u64 n, u64 m, u64 a, u64 b, bool odd = true, u64 p = 1, u64 q = 1) {
    if (!a) return b;
    if ((odd && b >= a) || (!odd && b < m - a)) {
        u64 offset = odd ? 1 : 0;
        u64 times = (m - b + a * offset - 1) / a;
        u64 consumed = (times - offset) * p + q * offset;
        if (n <= consumed) return odd ? b : a * ((n - 1) / p) + b;
        n -= consumed;
        b += a * times - m * offset;
    }
    b = (odd ? a : m) - 1 - b;
    u64 quotient = m / a;
    u64 result = min_mod_recursive(n, a, m % a, b, !odd, (quotient - 1) * p + q, quotient * p + q);
    return odd ? a - 1 - result : m - 1 - result;
}

} // namespace detail

[[nodiscard]] inline u64 min_mod_linear(u64 n, u64 m, u64 a, u64 b) {
    return detail::min_mod_recursive(n, m, a % m, b % m);
}

[[nodiscard]] inline u64 min_mod_linear_binary(u64 n, u64 m, u64 a, u64 b) {
    a %= m;
    b %= m;
    u64 base = floor_sum(n, m, a, b);
    u64 low = 0, high = m;
    while (low + 1 < high) {
        u64 middle = (low + high) / 2;
        u64 at_least = floor_sum(n, m, a, b + m - middle) - base;
        if (at_least < n)
            high = middle;
        else
            low = middle;
    }
    return high - 1;
}

} // namespace toy
