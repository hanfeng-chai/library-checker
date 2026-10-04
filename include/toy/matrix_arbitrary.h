#pragma once
#include <toy/buffer.h>
#include <toy/mod.h>
namespace toy {
inline u32 inverse_coprime(u32 a, u32 m) {
    i64 x = 1, y = 0;
    u32 b = m;
    while (b) {
        u32 q = a / b;
        std::swap(a -= q * b, b);
        std::swap(x -= i64(q) * y, y);
    }
    return (x % i64(m) + m) % m;
}
// With a fixed multiplier f, floor(f*2^32/p) gives a Shoup quotient. Each
// SIMD lane's residual is in [0,2p), so one subtraction makes it canonical.
inline void modular_row_sub(u32 *__restrict dst, const u32 *__restrict src, u32 first, u32 n,
                            u32 factor, u32 p) {
    u32 ratio = (u64(factor) << 32) / p;
    auto f = _mm256_set1_epi32(factor), q = _mm256_set1_epi32(ratio), mod = _mm256_set1_epi32(p);
    u32 j = first;
    for (; j + 8 <= n; j += 8) {
        auto x = _mm256_loadu_si256((const __m256i *)(src + j));
        auto even = _mm256_srli_epi64(_mm256_mul_epu32(x, q), 32),
             odd = _mm256_mul_epu32(_mm256_srli_epi64(x, 32), q);
        auto quotient = _mm256_blend_epi32(even, odd, 0xaa);
        auto product =
            _mm256_sub_epi32(_mm256_mullo_epi32(x, f), _mm256_mullo_epi32(quotient, mod));
        product = _mm256_min_epu32(product, _mm256_sub_epi32(product, mod));
        auto value = _mm256_sub_epi32(_mm256_loadu_si256((const __m256i *)(dst + j)), product);
        _mm256_storeu_si256((__m256i *)(dst + j),
                            _mm256_min_epu32(value, _mm256_add_epi32(value, mod)));
    }
    for (; j < n; ++j) {
        u32 x = src[j], v = u64(x) * factor - (u64(x) * ratio >> 32) * p;
        v = std::min(v, v - p);
        dst[j] = std::min(dst[j] - v, dst[j] - v + p);
    }
}
inline u32 determinant_prime_power(u32 n, std::span<const u32> input, u32 prime, u32 modulus) {
    Buffer<u32> a(input.size());
    for (u32 i = 0; i < input.size(); ++i) a[i] = input[i] % modulus;
    Buffer<u32 *> row(n);
    for (u32 i = 0; i < n; ++i) row[i] = a.p + usize(i) * n;
    Barrett mod(modulus);
    u32 answer = 1;
    for (u32 k = 0; k < n; ++k) {
        u32 best = n, power = modulus;
        for (u32 i = k; i < n; ++i)
            if (u32 x = row[i][k]) {
                u32 v = 1;
                while (x % prime == 0) {
                    v *= prime;
                    x /= prime;
                }
                if (v < power) {
                    best = i;
                    power = v;
                    if (v == 1) break;
                }
            }
        if (best == n) return 0;
        if (best != k) {
            std::swap(row[best], row[k]);
            answer = modulus - answer;
        }
        answer = mod.mul(answer, row[k][k]);
        if (!answer) return 0;
        u32 inverse = inverse_coprime(row[k][k] / power, modulus);
        for (u32 i = k + 1; i < n; ++i)
            if (u32 value = row[i][k]) {
                u32 factor = mod.mul(value / power, inverse);
                modular_row_sub(row[i], row[k], k + 1, n, factor, modulus);
                row[i][k] = 0;
            }
    }
    return answer;
}
inline u32 determinant_arbitrary(u32 n, std::span<const u32> a, u32 modulus) {
    if (modulus == 1) return 0;
    std::array<std::pair<u32, u32>, 16> factors;
    u32 count = 0, rest = modulus;
    for (u32 p = 2; u64(p) * p <= rest; p += p == 2 ? 1 : 2)
        if (rest % p == 0) {
            u32 power = 1;
            do {
                rest /= p;
                power *= p;
            } while (rest % p == 0);
            factors[count++] = {p, power};
        }
    if (rest > 1) factors[count++] = {rest, rest};
    u64 answer = 0, product = 1;
    for (u32 i = 0; i < count; ++i) {
        auto [p, q] = factors[i];
        u32 value = determinant_prime_power(n, a, p, q), delta = (value + q - answer % q) % q;
        answer += product * (u64(delta) * inverse_coprime(product % q, q) % q);
        product *= q;
    }
    return answer;
}
inline u32 determinant_euclid(u32 n, std::span<const u32> input, u32 p) {
    if (p == 1) return 0;
    Buffer<u32> a(input.size());
    memcpy(a.p, input.data(), input.size_bytes());
    Buffer<u32 *> row(n);
    for (u32 i = 0; i < n; ++i) row[i] = a.p + usize(i) * n;
    Barrett mod(p);
    u32 answer = 1;
    for (u32 k = 0; k < n; ++k) {
        for (u32 i = k + 1; i < n; ++i) {
            while (row[i][k]) {
                if (row[k][k]) {
                    u32 factor = row[i][k] / row[k][k];
                    if (factor) {
                        modular_row_sub(row[i], row[k], k, n, factor, p);
                    }
                }
                std::swap(row[i], row[k]);
                answer = answer ? p - answer : 0;
            }
        }
        answer = mod.mul(answer, row[k][k]);
        if (!answer) return 0;
    }
    return answer;
}
} // namespace toy
