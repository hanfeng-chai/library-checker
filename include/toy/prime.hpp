#pragma once

#include <bits/extc++.h>
#include <immintrin.h>
#include <toy/mod64.hpp>

namespace toy {

[[nodiscard]] inline bool is_prime(u64 n) {
    if (n < 2) return false;
    constexpr u32 small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (u32 p : small) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    u64 d = n - 1;
    int s = std::countr_zero(d);
    d >>= s;
    Montgomery64 mont(n);
    constexpr u64 witnesses[] = {2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022};
    for (u64 a : witnesses) {
        if (a % n == 0) continue;
        u64 x = mont.pow(a, d);
        if (x == mont.one()) continue;
        u64 minus_one = mont.init(n - 1);
        bool probable = x == minus_one;
        for (int r = 1; r < s && !probable; ++r) {
            x = mont.mul(x, x);
            probable = x == minus_one;
        }
        if (!probable) return false;
    }
    return true;
}

[[nodiscard]] inline bool is_prime_parallel(u64 n) {
    constexpr u32 small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (u32 p : small) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    constexpr std::array<u64, 7> witnesses = {2,       325,       9'375,        28'178,
                                              450'775, 9'780'504, 1'795'265'022};
    u64 d = n - 1;
    int s = std::countr_zero(d);
    d >>= s;
    Montgomery64 mont(n);
    std::array<u64, witnesses.size()> powers, values;
    for (u32 i = 0; i < witnesses.size(); ++i) {
        u64 base = witnesses[i] % n;
        powers[i] = mont.init(base ? base : 1);
        values[i] = mont.one();
    }
    for (u64 exponent = d; exponent; exponent >>= 1) {
        if (exponent & 1)
            for (u32 i = 0; i < witnesses.size(); ++i) values[i] = mont.mul(values[i], powers[i]);
        for (u32 i = 0; i < witnesses.size(); ++i) powers[i] = mont.mul(powers[i], powers[i]);
    }

    u64 one = mont.one(), minus_one = mont.init(n - 1);
    for (u64 x : values) {
        if (x == one || x == minus_one) continue;
        bool probable = false;
        for (int r = 1; r < s; ++r) {
            x = mont.mul(x, x);
            if (x == minus_one) {
                probable = true;
                break;
            }
        }
        if (!probable) return false;
    }
    return true;
}

class OddSegmentedSieve {
    static constexpr u32 segment_odds = 1U << 20;

  public:
    template <class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback &&callback) {
        if (limit < 2) return 0;
        u32 root = std::sqrt((long double)limit);
        while ((u64)(root + 1) * (root + 1) <= limit) ++root;
        while ((u64)root * root > limit) --root;

        std::vector<bool> base_composite(root / 2 + 1);
        std::vector<u32> base;
        for (u32 p = 3; p <= root; p += 2) {
            if (base_composite[p / 2]) continue;
            base.push_back(p);
            if ((u64)p * p <= root)
                for (u32 x = p * p; x <= root; x += 2 * p) base_composite[x / 2] = true;
        }

        u64 index = 0;
        callback(index++, 2);
        std::vector<u64> composite((segment_odds + 63) / 64);
        for (u64 low = 3; low <= limit; low += 2ULL * segment_odds) {
            u64 high = std::min<u64>(limit + 1ULL, low + 2ULL * segment_odds);
            u32 count = (high - low + 1) / 2;
            std::fill(composite.begin(), composite.begin() + (count + 63) / 64, 0);
            for (u32 p : base) {
                u64 start = std::max<u64>((u64)p * p, ((low + p - 1) / p) * p);
                if (!(start & 1)) start += p;
                if (start >= high) continue;
                for (u64 x = start; x < high; x += 2ULL * p) {
                    u32 bit = (x - low) / 2;
                    composite[bit / 64] |= 1ULL << (bit % 64);
                }
            }
            for (u32 bit = 0; bit < count; ++bit) {
                if (!(composite[bit / 64] >> (bit % 64) & 1)) callback(index++, low + 2ULL * bit);
            }
        }
        return index;
    }
};

class AtkinSieve {
    static constexpr u32 max_limit = 500'000'010;
    static constexpr u32 words = ((max_limit + 1) / 2 + 63) / 64;
    alignas(64) inline static std::array<u64, words> bits;

    static void flip(u32 index) { bits[index / 64] ^= 1ULL << (index % 64); }
    static void reset(u32 index) { bits[index / 64] &= ~(1ULL << (index % 64)); }
    static bool test(u32 index) { return bits[index / 64] >> (index % 64) & 1; }

  public:
    template <class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback &&callback) {
        assert(limit <= max_limit);
        u32 odd_count = (limit + 1) / 2;
        std::fill(bits.begin(), bits.begin() + (odd_count + 63) / 64, 0);

        for (int y = 1, m; (m = (y * y + 36) / 2) < (int)odd_count; y += 2) {
            if (y % 3)
                for (int k = 0; m < (int)odd_count; m += (k += 36) + 18) flip(m);
        }
        for (int x = 1, m; (m = (4 * x * x + 1) / 2) < (int)odd_count; ++x) {
            if (x % 3)
                for (int k = 0; m < (int)odd_count; m += (k += 4)) flip(m);
        }
        for (int y = 2, m; (m = (y * y + 3) / 2) < (int)odd_count; y += 2) {
            if (y % 3)
                for (int k = 0; m < (int)odd_count; m += (k += 12)) flip(m);
        }
        for (int y = 1, m; (m = ((2 * y + 6) * y + 3) / 2) < (int)odd_count; ++y) {
            if (y % 3)
                for (int k = 6 * y; m < (int)odd_count; m += (k += 12)) flip(m);
        }
        for (int p = 5, square; (square = p * p) / 2 < (int)odd_count; p += 2) {
            if (test(p / 2))
                for (int m = square / 2; m < (int)odd_count; m += square) reset(m);
        }
        if (odd_count > 1) bits[0] |= 1ULL << 1;

        u64 index = 0;
        if (limit >= 2) callback(index++, 2);
        for (u32 n = 3; n <= limit; n += 2)
            if (test(n / 2)) callback(index++, n);
        return index;
    }
};

class DenseOddSieve {
    static constexpr u32 max_limit = 500'000'010;
    static constexpr u32 words = ((max_limit + 1) / 2 + 63) / 64;
    alignas(64) inline static std::array<u64, words> composite;

  public:
    template <class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback &&callback) {
        assert(limit <= max_limit);
        u32 odd_count = (limit + 1) / 2;
        u32 used_words = (odd_count + 63) / 64;
        std::fill(composite.begin(), composite.begin() + used_words, 0);
        u32 root = std::sqrt((long double)limit);
        for (u32 p = 3; p <= root; p += 2) {
            u32 index = p / 2;
            if (composite[index / 64] >> (index % 64) & 1) continue;
            for (u64 bit = (u64)p * p / 2; bit < odd_count; bit += p)
                composite[bit / 64] |= 1ULL << (bit % 64);
        }

        u64 index = 0;
        if (limit >= 2) callback(index++, 2);
        if (odd_count) composite[0] |= 1;
        for (u32 word = 0; word < used_words; ++word) {
            u64 primes = ~composite[word];
            if (word + 1 == used_words && odd_count % 64) primes &= (1ULL << (odd_count % 64)) - 1;
            while (primes) {
                u32 bit = std::countr_zero(primes);
                callback(index++, 2ULL * (word * 64ULL + bit) + 1);
                primes &= primes - 1;
            }
        }
        return index;
    }
};

class Wheel30Sieve {
    static constexpr u32 max_limit = 500'000'010;
    static constexpr u32 blocks = (max_limit + 29) / 30;
    alignas(64) inline static std::array<u8, blocks> composite;
    inline static constexpr std::array<u8, 8> residues = {1, 7, 11, 13, 17, 19, 23, 29};
    inline static constexpr std::array<u8, 8> gaps = {6, 4, 2, 4, 2, 4, 6, 2};
    inline static constexpr auto residue_index = [] {
        std::array<i8, 30> result{};
        result.fill(-1);
        for (u32 i = 0; i < residues.size(); ++i) result[residues[i]] = i;
        return result;
    }();

    static bool marked(u64 n) {
        i8 bit = residue_index[n % 30];
        return composite[n / 30] >> bit & 1;
    }

  public:
    template <class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback &&callback) {
        assert(limit <= max_limit);
        u32 used_blocks = (limit + 30) / 30;
        std::fill(composite.begin(), composite.begin() + used_blocks, 0);
        composite[0] |= 1;
        u32 root = std::sqrt((long double)limit);
        for (u32 p = 7; p <= root; p += 2) {
            if (p % 3 == 0 || p % 5 == 0 || marked(p)) continue;
            u64 q = p;
            u32 step = residue_index[q % 30];
            for (u64 product = (u64)p * q; product <= limit; product = (u64)p * q) {
                composite[product / 30] |= 1U << residue_index[product % 30];
                q += gaps[step];
                step = (step + 1) & 7;
            }
        }

        u64 index = 0;
        if (limit >= 2) callback(index++, 2);
        if (limit >= 3) callback(index++, 3);
        if (limit >= 5) callback(index++, 5);
        for (u32 block = 0; block < used_blocks; ++block) {
            u8 primes = ~composite[block];
            while (primes) {
                u32 bit = std::countr_zero((u32)primes);
                u64 prime = 30ULL * block + residues[bit];
                if (prime > limit) break;
                callback(index++, prime);
                primes &= primes - 1;
            }
        }
        return index;
    }
};

class HybridWheel30Sieve {
    static constexpr u32 max_limit = 500'000'010;
    static constexpr u32 blocks = (max_limit + 29) / 30;
    static constexpr u32 dense_prime_limit = 300;
    static constexpr u32 max_mask_blocks = 1U << 20;
    alignas(64) inline static std::array<u8, blocks> prime_bits;
    inline static constexpr std::array<u8, 8> residues = {1, 7, 11, 13, 17, 19, 23, 29};
    inline static constexpr std::array<u8, 8> gaps = {6, 4, 2, 4, 2, 4, 6, 2};
    inline static constexpr auto residue_index = [] {
        std::array<i8, 30> result{};
        result.fill(-1);
        for (u32 i = 0; i < residues.size(); ++i) result[residues[i]] = i;
        return result;
    }();

    static u32 ordinal(u64 value) { return value / 30 * 8 + residue_index[value % 30]; }

    static std::vector<u32> base_primes(u32 root) {
        std::vector<bool> composite(root + 1);
        std::vector<u32> result;
        for (u32 p = 2; p <= root; ++p) {
            if (composite[p]) continue;
            result.push_back(p);
            if ((u64)p * p <= root)
                for (u32 multiple = p * p; multiple <= root; multiple += p)
                    composite[multiple] = true;
        }
        return result;
    }

    static void apply_dense_group(std::span<const u32> group, u32 product, u32 used_blocks) {
        std::vector<u8> mask(product, 0xff);
        u64 range = 30ULL * product;
        for (u32 p : group) {
            for (u64 multiple = p; multiple < range; multiple += p) {
                i8 bit = residue_index[multiple % 30];
                if (bit >= 0) mask[multiple / 30] &= ~(1U << bit);
            }
        }
        for (u32 offset = 0; offset < used_blocks; offset += product) {
            u32 count = std::min(product, used_blocks - offset);
            u8 *destination = prime_bits.data() + offset;
            const u8 *source = mask.data();
            u32 i = 0;
            for (; i + 32 <= count; i += 32) {
                __m256i values = _mm256_loadu_si256((const __m256i *)(destination + i));
                __m256i filter = _mm256_loadu_si256((const __m256i *)(source + i));
                _mm256_storeu_si256((__m256i *)(destination + i), _mm256_and_si256(values, filter));
            }
            for (; i < count; ++i) destination[i] &= source[i];
        }
    }

  public:
    template <class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback &&callback, u64 stride = 1, u64 offset = 0) {
        assert(limit <= max_limit);
        u32 used_blocks = limit / 30 + 1;
        std::fill(prime_bits.begin(), prime_bits.begin() + used_blocks, 0xff);
        u32 root = std::sqrt((long double)limit);
        auto base = base_primes(root);

        std::vector<u32> group;
        u32 product = 1;
        for (u32 p : base) {
            if (p < 7) continue;
            if (p >= dense_prime_limit) break;
            if ((u64)product * p > max_mask_blocks) {
                apply_dense_group(group, product, used_blocks);
                group.clear();
                product = 1;
            }
            group.push_back(p);
            product *= p;
        }
        if (!group.empty()) apply_dense_group(group, product, used_blocks);

        struct SparsePrime {
            u32 p;
            u32 position;
            u8 state;
            std::array<u32, 8> steps;
        };
        std::vector<SparsePrime> sparse;
        for (u32 p : base) {
            if (p < dense_prime_limit) continue;
            SparsePrime item{p, ordinal((u64)p * p), (u8)residue_index[p % 30], {}};
            for (u32 state = 0; state < 8; ++state) {
                u64 current = (u64)p * residues[state];
                u64 next = (u64)p * (residues[state] + gaps[state]);
                item.steps[state] = ordinal(next) - ordinal(current);
            }
            sparse.push_back(item);
        }
        constexpr u32 sparse_block = (1U << 17) * 8;
        u32 total_ordinals = used_blocks * 8;
        for (u32 block_end = sparse_block;; block_end += sparse_block) {
            u32 end = std::min(block_end, total_ordinals);
            for (auto &item : sparse) {
                u32 position = item.position;
                u32 state = item.state;
                while (position + item.p * 8 <= end) {
#pragma GCC unroll 8
                    for (u32 j = 0; j < 8; ++j) {
                        prime_bits[position / 8] &= ~(1U << (position % 8));
                        position += item.steps[(state + j) & 7];
                    }
                }
                while (position < end) {
                    prime_bits[position / 8] &= ~(1U << (position % 8));
                    position += item.steps[state];
                    state = (state + 1) & 7;
                }
                item.position = position;
                item.state = state;
            }
            if (end == total_ordinals) break;
        }

        prime_bits[0] &= ~1U;
        for (u32 p : base) {
            if (p >= 7) prime_bits[p / 30] |= 1U << residue_index[p % 30];
        }

        u64 index = 0;
        u64 target = offset;
        auto visit_small = [&](u64 prime) {
            if (prime <= limit) {
                if (index == target) callback(index, prime), target += stride;
                ++index;
            }
        };
        visit_small(2);
        visit_small(3);
        visit_small(5);
        u32 used_words = (used_blocks + 7) / 8;
        for (u32 word = 0; word < used_words; ++word) {
            u64 primes;
            memcpy(&primes, prime_bits.data() + word * 8, 8);
            if (word + 1 == used_words) {
                for (u32 bit = 0; bit < 64; ++bit) {
                    u64 ordinal = word * 64ULL + bit;
                    u64 prime = 30 * (ordinal / 8) + residues[ordinal % 8];
                    if (prime > limit) primes &= ~(1ULL << bit);
                }
            }
            u32 count = std::popcount(primes);
            while (target < index + count) {
                u64 selected = _pdep_u64(1ULL << (target - index), primes);
                u32 bit = std::countr_zero(selected);
                u64 ordinal = word * 64ULL + bit;
                callback(target, 30 * (ordinal / 8) + residues[ordinal % 8]);
                target += stride;
            }
            index += count;
        }
        return index;
    }
};

} // namespace toy
