#pragma once

#include <bits/extc++.h>
#include <toy/prime.hpp>

namespace toy {

namespace factor_detail {

inline u64 splitmix64(u64& state) {
    u64 z = (state += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

inline u64 pollard_rho(u64 n) {
    if (!(n & 1)) return 2;
    if (n % 3 == 0) return 3;
    Montgomery64 mont(n);
    u64 state = n ^ 0x243f6a8885a308d3ULL;
    for (;;) {
        u64 y = mont.init(splitmix64(state) % (n - 1) + 1);
        u64 c = mont.init(splitmix64(state) % (n - 1) + 1);
        auto f = [&](u64 x) {
            u64 next = mont.mul(x, x) + c;
            return next < n ? next : next - n;
        };
        u64 x = 0, saved = 0, g = 1, r = 1;
        while (g == 1) {
            x = y;
            for (u64 i = 0; i < r; ++i) y = f(y);
            for (u64 k = 0; k < r && g == 1; k += 128) {
                saved = y;
                u64 product = mont.one();
                for (u64 i = 0; i < std::min<u64>(128, r - k); ++i) {
                    y = f(y);
                    product = mont.mul(product, x > y ? x - y : y - x);
                }
                g = std::gcd(product, n);
            }
            r <<= 1;
        }
        if (g == n) {
            do {
                saved = f(saved);
                g = std::gcd(x > saved ? x - saved : saved - x, n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

inline void factor_rec(u64 n, std::vector<u64>& result) {
    if (n == 1) return;
    if (is_prime(n)) {
        result.push_back(n);
        return;
    }
    u64 divisor = pollard_rho(n);
    factor_rec(divisor, result);
    factor_rec(n / divisor, result);
}

} // namespace factor_detail

[[nodiscard]] inline std::vector<u64> factorize(u64 n) {
    std::vector<u64> result;
    factor_detail::factor_rec(n, result);
    std::sort(result.begin(), result.end());
    return result;
}

[[nodiscard]] inline u64 primitive_root_prime(u64 p) {
    if (p == 2) return 1;
    std::vector<u64> factors = factorize(p - 1);
    factors.erase(std::unique(factors.begin(), factors.end()), factors.end());
    Montgomery64 mont(p);
    for (u64 root = 2;; ++root) {
        bool valid = true;
        for (u64 q : factors) {
            if (mont.value(mont.pow(root, (p - 1) / q)) == 1) {
                valid = false;
                break;
            }
        }
        if (valid) return root;
    }
}

} // namespace toy
