#include <bits/allocator.h>

#include <cstdint>

#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#include <immintrin.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <compare>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Rational;

using u32 = uint32_t;
using u64 = uint64_t;

double timer() {
    static double old = 0;
    double new_t = 1.0 * clock() / CLOCKS_PER_SEC, diff = new_t - old;
    old = new_t;
    return diff;
}

namespace simd {
using i128 = __m128i;
using i256 = __m256i;
using u32x8 = u32 __attribute__((vector_size(32)));
using u64x4 = u64 __attribute__((vector_size(32)));

u32x8 load_u32x8(u32 *ptr) {
    return (u32x8)(_mm256_load_si256((i256 *)ptr));
}
u32x8 loadu_u32x8(u32 *ptr) {
    return (u32x8)(_mm256_loadu_si256((i256 *)ptr));
}
void store_u32x8(u32 *ptr, u32x8 val) {
    _mm256_store_si256((i256 *)ptr, (i256)(val));
}
void storeu_u32x8(u32 *ptr, u32x8 val) {
    _mm256_storeu_si256((i256 *)ptr, (i256)(val));
}

u64x4 load_u64x4(u64 *ptr) {
    return (u64x4)(_mm256_load_si256((i256 *)ptr));
}
u64x4 loadu_u64x4(u64 *ptr) {
    return (u64x4)(_mm256_loadu_si256((i256 *)ptr));
}
void store_u64x4(u64 *ptr, u64x4 val) {
    _mm256_store_si256((i256 *)ptr, (i256)(val));
}
void storeu_u64x4(u64 *ptr, u64x4 val) {
    _mm256_storeu_si256((i256 *)ptr, (i256)(val));
}

u32x8 set1_u32x8(u32 val) {
    return (u32x8)(_mm256_set1_epi32(val));
}
u64x4 set1_u64x4(u64 val) {
    return (u64x4)(_mm256_set1_epi64x(val));
}

u32x8 setr_u32x8(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7) {
    return (u32x8)(_mm256_setr_epi32(a0, a1, a2, a3, a4, a5, a6, a7));
}
u64x4 setr_u64x4(u64 a0, u64 a1, u64 a2, u64 a3) {
    return (u64x4)(_mm256_setr_epi64x(a0, a1, a2, a3));
}

template <int imm8>
u32x8 shuffle_u32x8(u32x8 val) {
    return (u32x8)(_mm256_shuffle_epi32((i256)(val), imm8));
}
u32x8 permute_u32x8(u32x8 val, u32x8 p) {
    return (u32x8)(_mm256_permutevar8x32_epi32((i256)(val), (i256)(p)));
}

template <int imm8>
u32x8 permute_u32x8_epi128(u32x8 a, u32x8 b) {
    return (u32x8)(_mm256_permute2x128_si256((i256)(a), (i256)(b), imm8));
}

template <int imm8>
u32x8 blend_u32x8(u32x8 a, u32x8 b) {
    return (u32x8)(_mm256_blend_epi32((i256)(a), (i256)(b), imm8));
}

template <int imm8>
u32x8 shift_left_u32x8_epi128(u32x8 val) {
    return (u32x8)(_mm256_bslli_epi128((i256)(val), imm8));
}
template <int imm8>
u32x8 shift_right_u32x8_epi128(u32x8 val) {
    return (u32x8)(_mm256_bsrli_epi128((i256)(val), imm8));
}

u32x8 shift_left_u32x8_epi64(u32x8 val, int imm8) {
    return (u32x8)(_mm256_slli_epi64((i256)(val), imm8));
}
u32x8 shift_right_u32x8_epi64(u32x8 val, int imm8) {
    return (u32x8)(_mm256_srli_epi64((i256)(val), imm8));
}

u32x8 min_u32x8(u32x8 a, u32x8 b) {
    return (u32x8)(_mm256_min_epu32((i256)(a), (i256)(b)));
}
u32x8 mul64_u32x8(u32x8 a, u32x8 b) {
    return (u32x8)(_mm256_mul_epu32((i256)(a), (i256)(b)));
}

u32x8 add_u32x8(u32x8 a, u32x8 b) {
    return (u32x8)(_mm256_add_epi32((i256)(a), (i256)(b)));
}
u64x4 add_u64x4(u64x4 a, u64x4 b) {
    return (u64x4)(_mm256_add_epi32((i256)(a), (i256)(b)));
}

u32x8 sub_u32x8(u32x8 a, u32x8 b) {
    return (u32x8)(_mm256_sub_epi32((i256)(a), (i256)(b)));
}
};  // namespace simd
using namespace simd;

struct Montgomery {
    uint32_t mod, mod2, n_inv, r, r2;

    Montgomery() = default;
    Montgomery(uint32_t mod) : mod(mod) {
        assert(mod % 2);
        assert(mod < (1 << 30));
        n_inv = -mod & 3;
        for (int i = 0; i < 4; i++) {
            n_inv *= 2u + n_inv * mod;
        }
        assert(n_inv * mod == -1u);

        mod2 = 2 * mod;
        r = static_cast<uint32_t>((1ULL << 32) % mod);
        r2 = static_cast<uint32_t>(r * uint64_t(r) % mod);
    }

    uint32_t shrink(uint32_t val) const { return std::min(val, val - mod); }
    uint32_t shrink2(uint32_t val) const { return std::min(val, val - mod2); }
    uint32_t shrink_n(uint32_t val) const { return std::min(val, val + mod); }
    uint32_t shrink2_n(uint32_t val) const { return std::min(val, val + mod2); }

    template <bool strict = false>
    uint32_t reduce(uint64_t val) const {
        uint32_t res = (val + uint32_t(val) * n_inv * uint64_t(mod)) >> 32;
        if constexpr (strict)
            res = shrink(res);
        return res;
    }

    template <bool strict = false>
    uint32_t mul(uint32_t a, uint32_t b) const {
        uint64_t val = uint64_t(a) * b;
        uint32_t res = (val + uint32_t(val) * n_inv * uint64_t(mod)) >> 32;
        if constexpr (strict)
            res = shrink(res);
        return res;
    }

    template <bool input_in_space = false, bool in_space_res = true>
    uint32_t power(uint32_t b, uint32_t e) const {
        if constexpr (!input_in_space)
            b = mul<true>(b, r2);
        uint32_t res = (in_space_res ? r : 1);
        for (; e > 0; e >>= 1) {
            if (e & 1)
                res = mul(res, b);
            b = mul(b, b);
        }

        res = shrink(res);
        return res;
    }

    template <bool input_in_space = false, bool in_space_res = true>
    uint32_t inv(uint32_t a) const {
        return power<input_in_space, in_space_res>(a, mod - 2);
    }
};

struct Montgomery_simd {
    alignas(32) u32x8 mod;
    alignas(32) u32x8 mod2;
    alignas(32) u32x8 n_inv;
    alignas(32) u32x8 r;
    alignas(32) u32x8 r2;

    Montgomery_simd() = default;
    Montgomery_simd(u32 md) {
        Montgomery mt(md);
        mod = set1_u32x8(mt.mod);
        mod2 = set1_u32x8(mt.mod2);
        n_inv = set1_u32x8(mt.n_inv);
        r = set1_u32x8(mt.r);
        r2 = set1_u32x8(mt.r2);
    }

    u32x8 shrink(u32x8 val) const {
        return min_u32x8(val, val - mod);
    }
    u32x8 shrink2(u32x8 val) const {
        return min_u32x8(val, val - mod2);
    }
    u32x8 shrink_n(u32x8 val) const {
        return min_u32x8(val, val + mod);
    }
    u32x8 shrink2_n(u32x8 val) const {
        return min_u32x8(val, val + mod2);
    }

    template <bool strict = false>
    u64x4 reduce(u64x4 val) const {
        val = (u64x4)shift_right_u32x8_epi64(u32x8(val + (u64x4)mul64_u32x8(mul64_u32x8((u32x8)val, n_inv), mod)), 32);
        if constexpr (strict) {
            val = (u64x4)shrink((u32x8)val);
        }
        return val;
    }

    template <bool strict = false>
    u32x8 reduce(u64x4 x0246, u64x4 x1357) const {
        u32x8 x0246_ninv = mul64_u32x8((u32x8)x0246, n_inv);
        u32x8 x1357_ninv = mul64_u32x8((u32x8)x1357, n_inv);
        u32x8 res = blend_u32x8<0b10'10'10'10>(shift_right_u32x8_epi128<4>(u32x8((u64x4)x0246 + (u64x4)mul64_u32x8(x0246_ninv, mod))),
                                               u32x8((u64x4)x1357 + (u64x4)mul64_u32x8(x1357_ninv, mod)));
        if constexpr (strict)
            res = shrink(res);
        return res;
    }

    template <bool strict = false, bool eq_b = false>
    u32x8 mul(u32x8 a, u32x8 b) const {
        u32x8 x0246 = mul64_u32x8(a, b);
        u32x8 b_sh = b;
        if constexpr (!eq_b) {
            b_sh = shift_right_u32x8_epi128<4>(b);
        }
        u32x8 x1357 = mul64_u32x8(shift_right_u32x8_epi128<4>(a), b_sh);

        return reduce<strict>((u64x4)x0246, (u64x4)x1357);
    }

    template <bool strict = false>
    u64x4 mul_to_hi(u64x4 a, u64x4 b) const {
        u32x8 val = mul64_u32x8((u32x8)a, (u32x8)b);
        u32x8 val_ninv = mul64_u32x8(val, n_inv);
        u32x8 res = u32x8(u64x4(val) + u64x4(mul64_u32x8(val_ninv, mod)));
        if constexpr (strict)
            res = shrink(res);
        return (u64x4)res;
    }

    template <bool strict = false>
    u64x4 mul(u64x4 a, u64x4 b) const {
        u32x8 val = mul64_u32x8((u32x8)a, (u32x8)b);
        return reduce<strict>((u64x4)val);
    }
};

namespace pizda {
u32 buf[1 << 23];
struct NTT {
    const u32 mod;
    const Montgomery mt;
    const Montgomery_simd mts;

    u32 pr_root;
    uint32_t kth_root[24];
    uint32_t szth_fix[24];

    alignas(32) uint32_t w_precalc[24][8];

    NTT() = default;
    NTT(uint32_t in) : mod(in), mt(in), mts(in) {
        std::vector<int> candidates_for_checking;

        int n = static_cast<int>(mod) - 1;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                candidates_for_checking.push_back(i);
                while (n % i == 0) {
                    n /= i;
                }
            }
        }

        if (n > 1) {
            candidates_for_checking.push_back(n);
        }

        for (uint32_t i = 2; i < mod; i++) {
            bool is_pr_root = false;
            for (int j : candidates_for_checking) {
                if (mt.power<false, false>(i, static_cast<uint32_t>((static_cast<int>(mod) - 1) / j)) == 1) {
                    is_pr_root = true;
                }
            }

            if (!is_pr_root) {
                pr_root = i;
                break;
            }
        }

        szth_fix[0] = mt.mul(mt.r2, mt.inv<true, true>(mt.r));
        for (int k = 1; k < 24; k++) {
            szth_fix[k] = mt.mul(mt.r2, mt.inv<true, true>(mt.power<false, true>(2u, static_cast<uint32_t>(k))));
            kth_root[k] = mt.power<false, true>(pr_root, (mod - 1) >> (k));
        }
    }

    inline void butterfly(u32x8 &a, u32x8 &b, u32x8 &w) const {
        const auto mts = this->mts;
        u32x8 c = mts.mul<1>(b, w);
        b = mts.shrink_n(sub_u32x8(a, c));
        a = mts.shrink(add_u32x8(a, c));
    }

    [[gnu::noinline]] void ntt(int lg, uint32_t *a) const {
        const auto mt = this->mt;
        const auto mts = this->mts;
        std::array<uint32_t *, 2> swapping_data{a, buf};
        bool where_buf = 1;

        u32 sz = 1 << lg, half_sz = 1 << lg - 1;

        auto ntt_iteration = [&](int k) {
            uint32_t w = mt.r, st = kth_root[lg - k];
            if (k == 0) {
                for (int i = 0; i < half_sz; i++) {
                    u32 a = swapping_data[!where_buf][i], b = swapping_data[!where_buf][i + half_sz];
                    u32 c = mt.mul<1>(b, w);
                    swapping_data[where_buf][i] = mt.shrink(a + c);
                    swapping_data[where_buf][i + half_sz] = mt.shrink_n(a - c);
                    w = mt.mul(w, st);
                }

                return;
            }

            for (int i = 0; i < (1 << (lg - 1)); i += (1 << k)) {
                for (int j = 0; j < (1 << k - 1); j++) {
                    uint32_t a = swapping_data[!where_buf][i + j], b = swapping_data[!where_buf][i + (1 << (lg - 1)) + j];
                    uint32_t c = mt.mul<true>(w, b);
                    swapping_data[where_buf][i / 2 + j] = mt.shrink(a + c);
                    swapping_data[where_buf][i / 2 + (1 << (lg - 2)) + j] = mt.shrink_n(a - c);
                }

                for (int j = 0; j < (1 << (k - 1)); j++) {
                    uint32_t a = swapping_data[!where_buf][i + j + (1 << (k - 1))], b = swapping_data[!where_buf][i + (1 << (lg - 1)) + j + (1 << (k - 1))];
                    uint32_t c = mt.mul<true>(w, b);
                    swapping_data[where_buf][i / 2 + (1 << (lg - 1)) + j] = mt.shrink(a + c);
                    swapping_data[where_buf][i / 2 + (1 << (lg - 1)) + (1 << (lg - 2)) + j] = mt.shrink_n(a - c);
                }

                w = mt.mul(w, st);
            }
        };

        auto ntt_iteration_simd = [&](int k) {
            u32x8 w = mts.r, st = set1_u32x8(kth_root[lg - k]);
            for (int i = 0; i < (1 << (lg - 1)); i += (1 << k)) {
                for (int j = 0; j < (1 << k - 1); j += 8) {
                    u32x8 a = loadu_u32x8(swapping_data[!where_buf] + i + j), b = loadu_u32x8(swapping_data[!where_buf] + i + (1 << (lg - 1)) + j);
                    butterfly(a, b, w);
                    storeu_u32x8(swapping_data[where_buf] + i / 2 + j, a);
                    storeu_u32x8(swapping_data[where_buf] + i / 2 + (1 << (lg - 2)) + j, b);
                }

                for (int j = 0; j < (1 << (k - 1)); j += 8) {
                    u32x8 a = loadu_u32x8(swapping_data[!where_buf] + i + j + (1 << (k - 1))), b = loadu_u32x8(swapping_data[!where_buf] + i + (1 << (lg - 1)) + j + (1 << (k - 1)));
                    butterfly(a, b, w);
                    storeu_u32x8(swapping_data[where_buf] + i / 2 + half_sz + j, a);
                    storeu_u32x8(swapping_data[where_buf] + i / 2 + half_sz + (1 << (lg - 2)) + j, b);
                }

                w = mts.mul(w, st);
            }
        };

        for (int k = lg - 1; k >= 4; k--) {
            ntt_iteration_simd(k);
            where_buf ^= 1;
            // cerr << "iteration_fast : " << timer() << "\n";
        }

        // timer();
        for (int k = std::min(lg - 1, 3); k >= 0; k--) {
            ntt_iteration(k);
            where_buf ^= 1;
            // cerr << "iteration_slow : " << timer() << "\n";
        }

        if (!where_buf) {
            memcpy(a, buf, 4 << lg);
        }
    }

    void inverse_ntt(int lg, uint32_t *data) const {
        std::reverse(data + 1, data + (1 << lg));
        ntt(lg, data);
    }

    template <bool are_a_b_extended = false, bool is_square = false>
    void inplace_convolve(std::vector<uint32_t> &lhs_poly, std::vector<uint32_t> &rhs_poly) const {
        int sz = 0;
        for (; (1 << sz) < (are_a_b_extended ? lhs_poly.size() : lhs_poly.size() + rhs_poly.size() - 1); sz++);

        ntt(sz, lhs_poly.data());
        if (!is_square)
            ntt(sz, rhs_poly.data());
        else
            rhs_poly = lhs_poly;
        uint32_t fix = szth_fix[sz];
        for (int i = 0; i < (1 << sz); i++) {
            lhs_poly[i] = mt.mul<1>(lhs_poly[i], mt.mul(rhs_poly[i], fix));
        }

        inverse_ntt(sz, lhs_poly.data());
    }

    template <bool are_a_b_extended = false, bool is_square = false>
    void inplace_convolve(int sz, u32 *lhs_poly, u32 *rhs_poly) const {
        ntt(sz, lhs_poly);
        if (!is_square)
            ntt(sz, rhs_poly);
        else
            rhs_poly = lhs_poly;
        uint32_t fix = szth_fix[sz];
        for (int i = 0; i < (1 << sz); i++) {
            lhs_poly[i] = mt.mul<1>(lhs_poly[i], mt.mul(rhs_poly[i], fix));
        }

        inverse_ntt(sz, lhs_poly);
    }
};
}  // namespace pizda

namespace super_fast_NTT {
#pragma GCC target("avx2,bmi")

using u32 = uint32_t;
using u64 = uint64_t;

struct Montgomery {
    u32 mod;    // mod
    u32 mod2;   // 2 * mod
    u32 n_inv;  // n_inv * mod == -1 (mod 2^32)
    u32 r;      // 2^32 % mod
    u32 r2;     // (2^32)^2 % mod

    Montgomery() = default;
    Montgomery(u32 mod) : mod(mod) {
        assert(mod % 2 == 1);
        assert(mod < (1 << 30));
        mod2 = 2 * mod;
        n_inv = 1;
        for (int i = 0; i < 5; i++) {
            n_inv *= 2 + n_inv * mod;
        }
        r = (u64(1) << 32) % mod;
        r2 = u64(r) * r % mod;
    }

    u32 shrink(u32 val) const {
        return std::min(val, val - mod);
    }
    u32 shrink2(u32 val) const {
        return std::min(val, val - mod2);
    }

    template <bool strict = true>
    u32 reduce(u64 val) const {
        u32 res = val + u32(val) * n_inv * u64(mod) >> 32;
        if constexpr (strict)
            res = shrink(res);
        return res;
    }

    template <bool strict = true>
    u32 mul(u32 a, u32 b) const {
        return reduce<strict>(u64(a) * b);
    }

    template <bool input_in_space = false, bool output_in_space = false>
    u32 power(u32 b, u32 e) const {
        if (!input_in_space)
            b = mul<false>(b, r2);
        u32 r = output_in_space ? this->r : 1;
        for (; e > 0; e >>= 1) {
            if (e & 1)
                r = mul<false>(r, b);
            b = mul<false>(b, b);
        }
        return shrink(r);
    }

    template <bool input_in_space = false, bool in_space_res = true>
    uint32_t inv(uint32_t a) const {
        return power<input_in_space, in_space_res>(a, mod - 2);
    }
};

using i256 = __m256i;
using u32x8 = u32 __attribute__((vector_size(32)));
using u64x4 = u64 __attribute__((vector_size(32)));

u32x8 load_u32x8(const u32 *ptr) {
    return (u32x8)_mm256_load_si256((const i256 *)ptr);
}
void store_u32x8(u32 *ptr, u32x8 vec) {
    _mm256_store_si256((i256 *)ptr, (i256)vec);
}

struct Montgomery_simd {
    u32x8 mod;    // mod
    u32x8 mod2;   // 2 * mod
    u32x8 n_inv;  // n_inv * mod == -1 (mod 2^32)
    u32x8 r;      // 2^32 % mod
    u32x8 r2;     // (2^32)^2 % mod

    Montgomery_simd() = default;
    Montgomery_simd(u32 mod) {
        Montgomery mt(mod);
        this->mod = (u32x8)_mm256_set1_epi32(mt.mod);
        this->mod2 = (u32x8)_mm256_set1_epi32(mt.mod2);
        this->n_inv = (u32x8)_mm256_set1_epi32(mt.n_inv);
        this->r = (u32x8)_mm256_set1_epi32(mt.r);
        this->r2 = (u32x8)_mm256_set1_epi32(mt.r2);
    }

    u32x8 shrink(u32x8 vec) const {
        return (u32x8)_mm256_min_epu32((i256)vec, _mm256_sub_epi32((i256)vec, (i256)mod));
    }
    u32x8 shrink2(u32x8 vec) const {
        return (u32x8)_mm256_min_epu32((i256)vec, _mm256_sub_epi32((i256)vec, (i256)mod2));
    }
    u32x8 shrink_n(u32x8 vec) const {
        return (u32x8)_mm256_min_epu32((i256)vec, _mm256_add_epi32((i256)vec, (i256)mod));
    }
    u32x8 shrink2_n(u32x8 vec) const {
        return (u32x8)_mm256_min_epu32((i256)vec, _mm256_add_epi32((i256)vec, (i256)mod2));
    }

    template <bool strict = true>
    u32x8 reduce(u64x4 x0246, u64x4 x1357) const {
        u64x4 x0246_ninv = (u64x4)_mm256_mul_epu32((i256)x0246, (i256)n_inv);
        u64x4 x1357_ninv = (u64x4)_mm256_mul_epu32((i256)x1357, (i256)n_inv);
        u64x4 x0246_res = (u64x4)_mm256_add_epi64((i256)x0246, _mm256_mul_epu32((i256)x0246_ninv, (i256)mod));
        u64x4 x1357_res = (u64x4)_mm256_add_epi64((i256)x1357, _mm256_mul_epu32((i256)x1357_ninv, (i256)mod));
        u32x8 res = (u32x8)_mm256_or_si256(_mm256_bsrli_epi128((i256)x0246_res, 4), (i256)x1357_res);
        if (strict)
            res = shrink(res);
        return res;
    }

    template <bool strict = true, bool b_use_only_even = false>
    u32x8 mul_u32x8(u32x8 a, u32x8 b) const {
        u32x8 a_sh = (u32x8)_mm256_bsrli_epi128((i256)a, 4);
        u32x8 b_sh = b_use_only_even ? b : (u32x8)_mm256_bsrli_epi128((i256)b, 4);
        u64x4 x0246 = (u64x4)_mm256_mul_epu32((i256)a, (i256)b);
        u64x4 x1357 = (u64x4)_mm256_mul_epu32((i256)a_sh, (i256)b_sh);
        return reduce<strict>(x0246, x1357);
    }

    template <bool strict = true>
    u64x4 mul_u64x4(u64x4 a, u64x4 b) const {
        u64x4 pr = (u64x4)_mm256_mul_epu32((i256)a, (i256)b);
        u64x4 pr2 = (u64x4)_mm256_mul_epu32(_mm256_mul_epu32((i256)pr, (i256)n_inv), (i256)mod);
        u64x4 res = (u64x4)_mm256_bsrli_epi128(_mm256_add_epi64((i256)pr, (i256)pr2), 4);
        if (strict)
            res = (u64x4)shrink((u32x8)res);
        return res;
    }
};

class NTT {
   public:
    u32 mod, pr_root;

    Montgomery mt;
    Montgomery_simd mts;

   private:
    static constexpr int LG = 32;  // more than enough for u32

    u32 w[4], wr[4];
    u32 wd[LG], wrd[LG];

    u64x4 wt_init, wrt_init;
    u64x4 wd_x4[LG], wrd_x4[LG];

    u64x4 wl_init;
    u64x4 wld_x4[LG];

    static u32 find_pr_root(u32 mod, const Montgomery &mt) {
        std::vector<u32> factors;
        u32 n = mod - 1;
        for (u32 i = 2; u64(i) * i <= n; i++) {
            if (n % i == 0) {
                factors.push_back(i);
                do {
                    n /= i;
                } while (n % i == 0);
            }
        }
        if (n > 1) {
            factors.push_back(n);
        }
        for (u32 i = 2; i < mod; i++) {
            if (std::all_of(factors.begin(), factors.end(), [&](u32 f) { return mt.power<false, false>(i, (mod - 1) / f) != 1; })) {
                return i;
            }
        }
        assert(false && "primitive root not found");
    }

   public:
    NTT() = default;
    NTT(u32 mod) : mod(mod), mt(mod), mts(mod) {
        const Montgomery mt = this->mt;
        const Montgomery_simd mts = this->mts;

        pr_root = find_pr_root(mod, mt);

        int lg = __builtin_ctz(mod - 1);
        assert(lg <= LG);

        memset(w, 0, sizeof(w));
        memset(wr, 0, sizeof(wr));
        memset(wd_x4, 0, sizeof(wd_x4));
        memset(wrd_x4, 0, sizeof(wrd_x4));
        memset(wld_x4, 0, sizeof(wld_x4));

        std::vector<u32> vec(lg + 1), vecr(lg + 1);
        vec[lg] = mt.power<false, true>(pr_root, mod - 1 >> lg);
        vecr[lg] = mt.power<true, true>(vec[lg], mod - 2);
        for (int i = lg - 1; i >= 0; i--) {
            vec[i] = mt.mul<true>(vec[i + 1], vec[i + 1]);
            vecr[i] = mt.mul<true>(vecr[i + 1], vecr[i + 1]);
        }

        w[0] = wr[0] = mt.r;
        if (lg >= 2) {
            w[1] = vec[2], wr[1] = vecr[2];
            if (lg >= 3) {
                w[2] = vec[3], wr[2] = vecr[3];
                w[3] = mt.mul<true>(w[1], w[2]);
                wr[3] = mt.mul<true>(wr[1], wr[2]);
            }
        }
        wt_init = (u64x4)_mm256_setr_epi64x(w[0], w[0], w[0], w[1]);
        wrt_init = (u64x4)_mm256_setr_epi64x(wr[0], wr[0], wr[0], wr[1]);

        wl_init = (u64x4)_mm256_setr_epi64x(w[0], w[1], w[2], w[3]);

        u32 prf = mt.r, prf_r = mt.r;
        for (int i = 0; i < lg - 2; i++) {
            u32 f = mt.mul<true>(prf, vec[i + 3]), fr = mt.mul<true>(prf_r, vecr[i + 3]);
            prf = mt.mul<true>(prf, vecr[i + 3]), prf_r = mt.mul<true>(prf_r, vec[i + 3]);
            u32 f2 = mt.mul<true>(f, f), f2r = mt.mul<true>(fr, fr);

            wd_x4[i] = (u64x4)_mm256_setr_epi64x(f2, f, f2, f);
            wrd_x4[i] = (u64x4)_mm256_setr_epi64x(f2r, fr, f2r, fr);
        }

        prf = mt.r;
        for (int i = 0; i < lg - 3; i++) {
            u32 f = mt.mul<true>(prf, vec[i + 4]);
            prf = mt.mul<true>(prf, vecr[i + 4]);
            wld_x4[i] = (u64x4)_mm256_set1_epi64x(f);
        }
    }

   private:
    static constexpr int L0 = 3;
    int get_low_lg(int lg) const {
        return lg % 2 == L0 % 2 ? L0 : L0 + 1;
    }

    //    public:
    //     bool lg_available(int lg) {
    //         return L0 <= lg && lg <= __builtin_ctz(mod - 1) + get_low_lg(lg);
    //     }

   private:
    template <bool transposed, bool trivial = false>
    static void butterfly_x2(u32 *ptr_a, u32 *ptr_b, u32x8 w, const Montgomery_simd &mts) {
        u32x8 a = load_u32x8(ptr_a), b = load_u32x8(ptr_b);
        u32x8 a2, b2;
        if (!transposed) {
            a = mts.shrink2(a), b = trivial ? mts.shrink2(b) : mts.mul_u32x8<false, true>(b, w);
            a2 = a + b, b2 = a + mts.mod2 - b;
        } else {
            a2 = mts.shrink2(a + b), b2 = trivial ? mts.shrink2_n(a - b) : mts.mul_u32x8<false, true>(a + mts.mod2 - b, w);
        }
        store_u32x8(ptr_a, a2), store_u32x8(ptr_b, b2);
    }

    template <bool transposed, bool trivial = false>
    static void butterfly_x4(u32 *ptr_a, u32 *ptr_b, u32 *ptr_c, u32 *ptr_d, u32x8 w1, u32x8 w2, u32x8 w3, const Montgomery_simd &mts) {
        u32x8 a = load_u32x8(ptr_a), b = load_u32x8(ptr_b), c = load_u32x8(ptr_c), d = load_u32x8(ptr_d);
        if (!transposed) {
            butterfly_x2<false, trivial>((u32 *)&a, (u32 *)&c, w1, mts);
            butterfly_x2<false, trivial>((u32 *)&b, (u32 *)&d, w1, mts);
            butterfly_x2<false, trivial>((u32 *)&a, (u32 *)&b, w2, mts);
            butterfly_x2<false, false>((u32 *)&c, (u32 *)&d, w3, mts);
        } else {
            butterfly_x2<true, trivial>((u32 *)&a, (u32 *)&b, w2, mts);
            butterfly_x2<true, false>((u32 *)&c, (u32 *)&d, w3, mts);
            butterfly_x2<true, trivial>((u32 *)&a, (u32 *)&c, w1, mts);
            butterfly_x2<true, trivial>((u32 *)&b, (u32 *)&d, w1, mts);
        }
        store_u32x8(ptr_a, a), store_u32x8(ptr_b, b), store_u32x8(ptr_c, c), store_u32x8(ptr_d, d);
    }

    template <bool inverse, bool trivial = false>
    void transform_aux(int k, int i, u32 *data, u64x4 &wi, const Montgomery_simd &mts) const {
        u32x8 w1 = (u32x8)_mm256_shuffle_epi32((i256)wi, 0b00'00'00'00);
        u32x8 w2 = (u32x8)_mm256_permute4x64_epi64((i256)wi, 0b01'01'01'01);  // only even indices will be used
        u32x8 w3 = (u32x8)_mm256_permute4x64_epi64((i256)wi, 0b11'11'11'11);  // only even indices will be used
        for (int j = 0; j < (1 << k); j += 8) {
            butterfly_x4<inverse, trivial>(data + i + (1 << k) * 0 + j, data + i + (1 << k) * 1 + j,
                                           data + i + (1 << k) * 2 + j, data + i + (1 << k) * 3 + j,
                                           w1, w2, w3, mts);
        }
        wi = mts.mul_u64x4<true>(wi, (inverse ? wrd_x4 : wd_x4)[__builtin_ctz(~i >> k + 2)]);
    }

   public:
    // input in [0, 4 * mod)
    // output in [0, 4 * mod)
    // data must be 32-byte aligned
    void transform_forward(int lg, u32 *data) const {
        const Montgomery_simd mts = this->mts;
        const int L = get_low_lg(lg);

        // for (int k = lg - 2; k >= L; k -= 2) {
        //     u64x4 wi = wt_init;
        //     transform_aux<false, true>(k, 0, data, wi, mts);
        //     for (int i = (1 << k + 2); i < (1 << lg); i += (1 << k + 2)) {
        //         transform_aux<false>(k, i, data, wi, mts);
        //     }
        // }

        if (L < lg) {
            const int lc = (lg - L) / 2;
            u64x4 wi_data[LG / 2];
            std::fill(wi_data, wi_data + lc, wt_init);

            for (int k = lg - 2; k >= L; k -= 2) {
                transform_aux<false, true>(k, 0, data, wi_data[k - L >> 1], mts);
            }
            for (int i = 1; i < (1 << lc * 2 - 2); i++) {
                int s = __builtin_ctz(i) >> 1;
                for (int k = s; k >= 0; k--) {
                    transform_aux<false>(2 * k + L, i * (1 << L + 2), data, wi_data[k], mts);
                }
            }
        }
    }

    // input in [0, 2 * mod)
    // output in [0, mod)
    // data must be 32-byte aligned
    template <bool mul_by_sc = false>
    void transform_inverse(int lg, u32 *data, /* as normal number */ u32 sc = u32()) const {
        const Montgomery_simd mts = this->mts;
        const int L = get_low_lg(lg);

        // for (int k = L; k + 2 <= lg; k += 2) {
        //     u64x4 wi = wrt_init;
        //     transform_aux<true, true>(k, 0, data, wi, mts);
        //     for (int i = (1 << k + 2); i < (1 << lg); i += (1 << k + 2)) {
        //         transform_aux<true>(k, i, data, wi, mts);
        //     }
        // }

        if (L < lg) {
            const int lc = (lg - L) / 2;
            u64x4 wi_data[LG / 2];
            std::fill(wi_data, wi_data + lc, wrt_init);

            for (int i = 0; i < (1 << lc * 2 - 2); i++) {
                int s = __builtin_ctz(~i) >> 1;
                if (i + 1 == (1 << 2 * s)) {
                    s--;
                }
                for (int k = 0; k <= s; k++) {
                    transform_aux<true>(2 * k + L, (i + 1 - (1 << 2 * k)) * (1 << L + 2), data, wi_data[k], mts);
                }
                if (i + 1 == (1 << 2 * (s + 1))) {
                    s++;
                    transform_aux<true, true>(2 * s + L, (i + 1 - (1 << 2 * s)) * (1 << L + 2), data, wi_data[s], mts);
                }
            }
        }

        const Montgomery mt = this->mt;
        u32 f = mt.power<false, true>(mod + 1 >> 1, lg - L);
        if constexpr (mul_by_sc)
            f = mt.mul<true>(f, mt.mul<false>(mt.r2, sc));
        u32x8 f_x8 = (u32x8)_mm256_set1_epi32(f);
        for (int i = 0; i < (1 << lg); i += 8) {
            store_u32x8(data + i, mts.mul_u32x8<true, true>(load_u32x8(data + i), f_x8));
        }
    }

   private:
    // input in [0, 4 * mod)
    // output in [0, 2 * mod)
    // multiplies mod (x^2^L - w)
    template <int L, int K, bool remove_montgomery_reduction_factor = true>
    /* !!! O3 is crucial here !!! */ __attribute__((optimize("O3"))) static void aux_mul_mod_x2L(const u32 *a, const u32 *b, u32 *c, const std::array<u32x8, K> &ar_w, const Montgomery_simd &mts) {
        static_assert(L >= 3);
        // static_assert(L == L0 || L == L0 + 1);

        constexpr int n = 1 << L;
        alignas(64) u32 aux_a[K][n];
        alignas(64) u64 aux_b[K][n * 2];
        for (int k = 0; k < K; k++) {
            for (int i = 0; i < n; i += 8) {
                u32x8 ai = load_u32x8(a + n * k + i);
                if constexpr (remove_montgomery_reduction_factor) {
                    ai = mts.mul_u32x8<true, true>(ai, mts.r2);
                } else {
                    ai = mts.shrink(mts.shrink2(ai));
                }
                store_u32x8(aux_a[k] + i, ai);

                u32x8 bi = load_u32x8(b + n * k + i);
                u32x8 bi_0 = mts.shrink(mts.shrink2(bi));
                u32x8 bi_w = mts.mul_u32x8<true, true>(bi, ar_w[k]);

                store_u32x8((u32 *)(aux_b[k] + i + 0), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_w, _mm256_setr_epi64x(0, 1, 2, 3)));
                store_u32x8((u32 *)(aux_b[k] + i + 4), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_w, _mm256_setr_epi64x(4, 5, 6, 7)));
                store_u32x8((u32 *)(aux_b[k] + n + i + 0), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_0, _mm256_setr_epi64x(0, 1, 2, 3)));
                store_u32x8((u32 *)(aux_b[k] + n + i + 4), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_0, _mm256_setr_epi64x(4, 5, 6, 7)));
            }
        }

        u64x4 aux_ans[K][n / 4];
        memset(aux_ans, 0, sizeof(aux_ans));
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < K; k++) {
                u64x4 ai = (u64x4)_mm256_set1_epi32(aux_a[k][i]);
                for (int j = 0; j < n; j += 4) {
                    u64x4 bi = (u64x4)_mm256_loadu_si256((i256 *)(aux_b[k] + n - i + j));
                    aux_ans[k][j / 4] += /* 64-bit addition */ (u64x4)_mm256_mul_epu32((i256)ai, (i256)bi);
                }
            }
            if (i >= 8 && (i & 7) == 7) {
                for (int k = 0; k < K; k++) {
                    for (int j = 0; j < n; j += 4) {
                        aux_ans[k][j / 4] = (u64x4)mts.shrink2((u32x8)aux_ans[k][j / 4]);
                    }
                }
            }
        }

        for (int k = 0; k < K; k++) {
            for (int i = 0; i < n; i += 8) {
                u64x4 c0 = aux_ans[k][i / 4], c1 = aux_ans[k][i / 4 + 1];
                u32x8 res = (u32x8)_mm256_permutevar8x32_epi32((i256)mts.reduce<false>(c0, c1), _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7));
                store_u32x8(c + k * n + i, mts.shrink2(res));
            }
        }
    }

    template <int L, bool remove_montgomery_reduction_factor = true>
    void aux_mul_mod_full(int lg, const u32 *a, const u32 *b, u32 *c) const {
        constexpr int sz = 1 << L;
        const Montgomery_simd mts = this->mts;
        int cnt = 1 << lg - L;
        if (cnt == 1) {
            aux_mul_mod_x2L<L, 1, remove_montgomery_reduction_factor>(a, b, c, {mts.r}, mts);
            return;
        }
        if (cnt <= 8) {
            for (int i = 0; i < cnt; i += 2) {
                u32x8 wi = (u32x8)_mm256_set1_epi32(w[i / 2]);
                aux_mul_mod_x2L<L, 2, remove_montgomery_reduction_factor>(a + i * sz, b + i * sz, c + i * sz, {wi, (mts.mod - wi)}, mts);
            }
            return;
        }
        u64x4 wi = wl_init;
        for (int i = 0; i < cnt; i += 8) {
            u32x8 w_ar[4] = {
                (u32x8)_mm256_permute4x64_epi64((i256)wi, 0b00'00'00'00),
                (u32x8)_mm256_permute4x64_epi64((i256)wi, 0b01'01'01'01),
                (u32x8)_mm256_permute4x64_epi64((i256)wi, 0b10'10'10'10),
                (u32x8)_mm256_permute4x64_epi64((i256)wi, 0b11'11'11'11),
            };
            if constexpr (L == L0) {
                for (int j = 0; j < 8; j += 4) {
                    aux_mul_mod_x2L<L, 4, remove_montgomery_reduction_factor>(a + (i + j) * sz, b + (i + j) * sz, c + (i + j) * sz,
                                                                              {w_ar[j / 2], mts.mod - w_ar[j / 2], w_ar[j / 2 + 1], mts.mod - w_ar[j / 2 + 1]}, mts);
                }
            } else {
                for (int j = 0; j < 8; j += 2) {
                    aux_mul_mod_x2L<L, 2, remove_montgomery_reduction_factor>(a + (i + j) * sz, b + (i + j) * sz, c + (i + j) * sz,
                                                                              {w_ar[j / 2], mts.mod - w_ar[j / 2]}, mts);
                }
            }
            wi = mts.mul_u64x4<true>(wi, wld_x4[__builtin_ctz(~i >> 3)]);
        }
    }

   public:
    template <bool remove_montgomery_reduction_factor = true>
    void aux_dot_mod(int lg, const u32 *a, const u32 *b, u32 *c) const {
        int L = get_low_lg(lg);
        if (L == L0) {
            aux_mul_mod_full<L0, remove_montgomery_reduction_factor>(lg, a, b, c);
        } else {
            aux_mul_mod_full<L0 + 1, remove_montgomery_reduction_factor>(lg, a, b, c);
        }
    }

    // lg must be greater than or equal to 3
    // a, b must be 32-byte aligned
    void convolve_cyclic(int lg, u32 *a, u32 *b) const {
        transform_forward(lg, a);
        transform_forward(lg, b);
        aux_dot_mod(lg, a, b, a);
        transform_inverse(lg, a);
    }

    alignas(32) inline static u32 buf1[1 << 20], buf2[1 << 20];
    template <bool are_a_b_extended = false, bool is_square = false>
    void inplace_convolve(std::vector<uint32_t> &lhs_poly, std::vector<uint32_t> &rhs_poly) const {
        int sz = 0;
        for (; (1 << sz) < (are_a_b_extended ? lhs_poly.size() : lhs_poly.size() + rhs_poly.size() - 1); sz++);
        assert(sz >= 3);

        memcpy(buf1, lhs_poly.data(), 4 << sz);
        memcpy(buf2, rhs_poly.data(), 4 << sz);
        convolve_cyclic(sz, buf1, buf2);
        memcpy(lhs_poly.data(), buf1, 4 << sz);
    }
};
}  // namespace super_fast_NTT

using super_fast_NTT::NTT;

class BigInteger {
    using Shifted = std::pair<BigInteger, uint32_t>;
    inline static uint32_t kBaseSize = 9;
    inline static uint32_t kBase = 1'000'000'000;

    friend class Rational;

   public:
    /* crazy math begins */

    void Swap(BigInteger &other) {
        std::swap(data_, other.data_);
        std::swap(is_negative_, other.is_negative_);
    }

   private:
    friend Rational;
    friend BigInteger gcd(BigInteger lhs, BigInteger rhs);
    inline static const NTT ntt_mod1 = NTT(998244353);
    inline static const NTT ntt_mod2 = NTT(897581057);
    inline static const NTT ntt_mod3 = NTT(880803841);
    static const BigInteger kOne;
    static const BigInteger kZero;

    explicit BigInteger(std::vector<uint32_t> data) : data_(data) {
        if (data.empty()) {
            data = {0};
        }

        normalize_helper();
    }

    void normalize_helper() {
        if (data_.size() == 0) {
            data_ = {0};
            is_negative_ = 0;
            return;
        }

        while (data_.size() >= 2 && data_.back() == 0) {
            data_.pop_back();
        }

        if (data_ == std::vector<uint32_t>{0}) is_negative_ = false;
    }

    void normalize() {
        normalize_helper();
    }

    void normalize() const {
        return;  // nothing
    }

    void DivByTwo() {
        operator*=(static_cast<long long>(kBase) / 2);
        data_.erase(data_.begin());
        if (data_.empty()) {
            data_ = {0};
        }
    }

   public:
    std::vector<uint32_t> data_;
    bool is_negative_ = 0;
    BigInteger() : data_({0}), is_negative_(0) {}
    BigInteger(long long x) {
        is_negative_ = (x < 0);
        x = std::abs(x);
        while (x != 0) {
            data_.push_back(static_cast<uint32_t>(x % kBase));
            x /= kBase;
        }

        data_.push_back(0);
        normalize_helper();
    }

    explicit BigInteger(__int128_t x) {
        is_negative_ = (x < 0);
        x = (x < 0 ? -x : x);
        while (x != 0) {
            data_.push_back(static_cast<uint32_t>(x % kBase));
            x /= kBase;
        }

        data_.push_back(0);
        normalize_helper();
    }

    explicit BigInteger(int x) : BigInteger(static_cast<long long>(x)) {}

    explicit BigInteger(unsigned long long x) {
        while (x != 0) {
            data_.push_back(static_cast<uint32_t>(x % kBase));
            x /= kBase;
        }

        data_.push_back(0);
        normalize_helper();
    }

    explicit BigInteger(unsigned long x) : BigInteger(static_cast<unsigned long long>(x)) {}

    explicit BigInteger(std::string_view input) {
        if (input[0] == '-') is_negative_ = true, input.remove_prefix(1);

        if (input.size() == 0) {
            throw std::runtime_error("strange input");
        }

        data_.assign((input.size() + kBaseSize - 1) / kBaseSize, 0);
        for (uint32_t i = 0; i * kBaseSize < input.size(); i++) {
            uint32_t cur = 1;
            for (uint32_t j = i * kBaseSize; j < input.size() && j < i * kBaseSize + kBaseSize; j++) {
                char current_char = input[input.size() - j - 1];
                data_[i] += cur * static_cast<uint32_t>(current_char - '0');
                cur *= 10;
            }
        }

        normalize_helper();
    }

    explicit BigInteger(char const *str, size_t len) : BigInteger(std::string_view(str, len)) {}

    std::string toString() const {
        std::string result;
        if (is_negative_) {
            result = "-";
        }

        result += std::to_string(data_.back());

        std::string current_number_to_string;
        for (uint32_t i = static_cast<uint32_t>(data_.size() - 2); i != -1u; i--) {
            current_number_to_string = std::to_string(data_[i]);
            current_number_to_string = std::string(kBaseSize - current_number_to_string.size(), '0') + current_number_to_string;
            result += current_number_to_string;
        }

        return result;
    }

    friend std::ostream &operator<<(std::ostream &out, const BigInteger &number) {
        if (number.is_negative_) {
            out << '-';
        }

        out << number.data_.back();
        if (number.data_.size() == 1) {
            return out;
        }

        for (uint32_t i = static_cast<uint32_t>(number.data_.size() - 2); i != -1u; i--) {
            out << std::setfill('0') << std::setw(static_cast<int>(kBaseSize)) << number.data_[i];
        }

        return out;
    }

    friend std::istream &operator>>(std::istream &in, BigInteger &number) {
        number.data_.clear();
        number.is_negative_ = 0;
        in >> std::ws;

        std::string s;
        char ch;
        in >> ch;

        if (ch == '-' || isdigit(ch)) {
            s += ch;
        } else {
            throw std::runtime_error("strange input");
        }

        while (isdigit(in.peek())) {
            in >> ch;
            s += ch;
        }

        number = BigInteger(s);
        return in;
    }

   private:
    std::strong_ordering spaceship_helper(const BigInteger &other) const {
        if (data_.size() != other.data_.size()) {
            return (data_.size() < other.data_.size() ? std::strong_ordering::less : std::strong_ordering::greater);
        }

        for (uint32_t i = static_cast<uint32_t>(data_.size() - 1u); i != -1u; i--) {
            if (data_[i] != other.data_[i]) {
                return (data_[i] < other.data_[i] ? std::strong_ordering::less : std::strong_ordering::greater);
            }
        }

        return std::strong_ordering::equal;
    }

   public:
    friend std::strong_ordering operator<=>(const BigInteger &lhs, const BigInteger &rhs) {
        if (lhs.is_negative_ != rhs.is_negative_) return (lhs.is_negative_ ? std::strong_ordering::less : std::strong_ordering::greater);
        std::strong_ordering abs_compare_result = lhs.spaceship_helper(rhs);
        if (abs_compare_result == std::strong_ordering::equal || !lhs.is_negative_) {
            return abs_compare_result;
        }

        if (abs_compare_result == std::strong_ordering::less) {
            return std::strong_ordering::greater;
        } else {
            return std::strong_ordering::less;
        }
    }

    friend bool operator==(const BigInteger &lhs, const BigInteger &rhs) = default;

    BigInteger &operator+=(BigInteger other) {
        if (is_negative_ == other.is_negative_) {
            uint32_t min_size = static_cast<uint32_t>(std::min(data_.size(), other.data_.size()));
            bool from = 0;
            for (uint32_t i = 0; i < min_size; i++) {
                data_[i] += other.data_[i] + from;
                from = 0;
                if (data_[i] >= kBase) {
                    from = 1;
                    data_[i] -= kBase;
                }
            }

            data_.insert(data_.end(), other.data_.begin() + min_size, other.data_.end());
            if (from) {
                uint32_t ind = min_size;
                while (ind < data_.size() && data_[ind] + 1 == kBase) data_[ind] = 0, ind++;
                if (ind == data_.size()) {
                    data_.push_back(1);
                } else {
                    data_[ind] += 1;
                }
            }

            normalize();
            return *this;
        } else {
            is_negative_ ^= 1;
            bool this_less_than_other = ((*this) < other) ^ is_negative_;
            uint32_t min_size = static_cast<uint32_t>(std::min(data_.size(), other.data_.size()));
            bool from = 0;
            for (uint32_t i = 0; i < min_size; i++) {
                std::vector<uint32_t> &b = other.data_;
                if (this_less_than_other) {
                    if (b[i] < data_[i] + from) {
                        data_[i] = b[i] + kBase - data_[i] - from;
                        from = 1;
                    } else {
                        data_[i] = b[i] - data_[i] - from;
                        from = 0;
                    }
                } else {
                    if (data_[i] < b[i] + from) {
                        data_[i] = data_[i] + kBase - b[i] - from;
                        from = 1;
                    } else {
                        data_[i] = data_[i] - b[i] - from;
                        from = 0;
                    }
                }
            }

            if (this_less_than_other) {
                data_.insert(data_.end(), other.data_.begin() + static_cast<int64_t>(data_.size()), other.data_.end());
            }

            if (from) {
                uint32_t ind = min_size;
                while (data_[ind] == 0) {
                    data_[ind] = kBase - 1u;
                    ind++;
                }

                data_[ind] -= 1;
            }

            is_negative_ ^= this_less_than_other ^ 1;
            normalize();
            return *this;
        }
    }

    BigInteger &operator-=(const BigInteger &other) {
        if (this == &other) {
            *this = kZero;
            return *this;
        }

        is_negative_ ^= 1;
        operator+=(other);
        is_negative_ ^= 1;
        normalize();
        return *this;
    }

    friend BigInteger operator+(BigInteger lhs, BigInteger rhs) {
        BigInteger tmp = lhs;
        tmp += rhs;
        tmp.normalize();
        return tmp;
    }

    friend BigInteger operator-(BigInteger lhs, BigInteger rhs) {
        BigInteger tmp = lhs;
        tmp -= rhs;
        tmp.normalize();
        return tmp;
    }

    BigInteger &operator++() {
        operator+=(kOne);
        normalize();
        return *this;
    }

    BigInteger operator++(int) {
        BigInteger result(*this);
        operator++();
        return result;
    }

    BigInteger &operator--() {
        operator-=(kOne);
        normalize();
        return *this;
    }

    BigInteger operator--(int) {
        BigInteger result(*this);
        operator--();
        return result;
    }

   private:
    void stupid_multiplication(const BigInteger &other) {
        uint32_t result_size = static_cast<uint32_t>(data_.size() + other.data_.size() - 1);
        uint64_t from_less_digits = 0;
        std::vector<uint32_t> vc1;
        vc1.resize(result_size);
        for (uint32_t sum_ij = 0; sum_ij < result_size; sum_ij++) {
            uint64_t lower = from_less_digits, upper = 0;
            uint64_t lbound = 0;
            if (sum_ij > other.data_.size()) {
                lbound = sum_ij - other.data_.size() + 1;
            }
            for (uint32_t i = lbound; i < data_.size(); i++) {
                if (i > sum_ij) break;
                uint32_t j = sum_ij - i;
                if (j >= other.data_.size()) continue;
                uint64_t current_mul = static_cast<uint64_t>(data_[i]) * other.data_[j];
                lower += current_mul % kBase;
                upper += current_mul / kBase;
            }

            upper += lower / kBase;
            lower = lower % kBase;
            vc1[sum_ij] = static_cast<uint32_t>(lower);
            from_less_digits = upper;
        }

        while (from_less_digits) {
            vc1.push_back(static_cast<uint32_t>(from_less_digits % kBase));
            from_less_digits /= kBase;
        }

        data_ = vc1;
        is_negative_ ^= other.is_negative_;
        normalize();
    }

    template <bool is_square = false>
    void smart_multiplication(const BigInteger &other) {
        uint64_t from_less_digits = 0;
        uint32_t result_size = static_cast<uint32_t>(data_.size() + other.data_.size() - 1);
        uint32_t sz = 3;
        for (; (1u << sz) < result_size; sz++) {
        }

        const auto mt1 = ntt_mod1.mt, mt2 = ntt_mod2.mt, mt3 = ntt_mod3.mt;
        std::vector<uint32_t> convolved_mod1 = data_, mod1_temp_data = other.data_;
        convolved_mod1.resize(1 << sz), mod1_temp_data.resize(1 << sz);
        std::vector<uint32_t> convolved_mod2 = convolved_mod1, mod2_temp_data = mod1_temp_data, convolved_mod3 = convolved_mod1, mod3_temp_data = mod1_temp_data;
        ntt_mod1.inplace_convolve<1, is_square>(convolved_mod1, mod1_temp_data);
        ntt_mod2.inplace_convolve<1, is_square>(convolved_mod2, mod2_temp_data);
        ntt_mod3.inplace_convolve<1, is_square>(convolved_mod3, mod3_temp_data);

        static uint32_t garner_magic_const_1 = mt2.inv<false, true>(mt1.mod);
        static uint32_t garner_magic_const_2 = mt3.inv<false, true>(static_cast<uint32_t>(static_cast<uint64_t>(mt1.mod) * mt2.mod % mt3.mod));
        static uint32_t garner_magic_const_3 = mt3.inv<false, true>(mt2.mod);

        // __uint128_t wtf = 0;
        // for (int i = 0; i < (1 << sz); i++) {
        //     u32 r1 = mt1.shrink(convolved_mod1[i]);
        //     u32 r2 = mt2.mul<1>(2 * mt2.mod + convolved_mod2[i] - r1, garner_magic_const_1);
        //     u32 r3 = mt3.shrink(mt3.mul<true>(2 * mt3.mod + convolved_mod3[i] - r1, garner_magic_const_2) + mt3.mul<true>(2 * mt3.mod - r2, garner_magic_const_3));
        //     __uint128_t total = r1 + (__uint128_t)r2 * mt1.mod + (__uint128_t)r3 * mt1.mod * mt2.mod;

        //     __uint128_t cur = wtf + total;
        //     convolved_mod1[i] = cur % kBase;
        //     wtf = cur / kBase;
        // }

        // while (wtf) {
        //     convolved_mod1.push_back(wtf % kBase);
        //     wtf /= kBase;
        // }

        convolved_mod1.resize((1 << sz));
        for (uint32_t i = 0; i < (1u << sz); i++) {
            uint32_t mod1_result_residue = mt1.shrink(convolved_mod1[i]);
            uint32_t mod2_result_residue = mt2.mul<true>(2 * mt2.mod + convolved_mod2[i] - mod1_result_residue, garner_magic_const_1);
            uint32_t mod3_result_residue = mt3.shrink(mt3.mul<true>(2 * mt3.mod + convolved_mod3[i] - mod1_result_residue, garner_magic_const_2) + mt3.mul<true>(2 * mt3.mod - mod2_result_residue, garner_magic_const_3));

            uint64_t lower = static_cast<uint64_t>(mt1.mod) * mt2.mod % kBase, upper = static_cast<uint64_t>(mt1.mod) * mt2.mod / kBase;
            upper = upper * mod3_result_residue + lower * mod3_result_residue / kBase;
            lower = lower * mod3_result_residue % kBase;
            lower += static_cast<uint64_t>(mod2_result_residue) * mt1.mod;
            lower += mod1_result_residue + from_less_digits;
            upper += lower / kBase;
            lower %= kBase;

            convolved_mod1[i] = static_cast<uint32_t>(lower);
            from_less_digits = upper;
        }

        while (from_less_digits) {
            convolved_mod1.push_back(static_cast<uint32_t>(from_less_digits % kBase));
            from_less_digits /= kBase;
        }

        data_ = convolved_mod1;
        is_negative_ ^= other.is_negative_;
        normalize();
    }

   public:
    BigInteger &square() {
        if (data_.size() < 8) {
            operator*=(*this);
        } else {
            BigInteger other = (*this);
            smart_multiplication<1>(other);
        }

        return *this;
    }

    BigInteger &operator*=(BigInteger other) {
        if (data_.size() < other.data_.size()) {
            Swap(other);
        }

        if (true && (other.data_.size() * data_.size() < static_cast<uint64_t>(1e3) || other.data_.size() < 8)) {
            stupid_multiplication(other);
        } else {
            // std::cout << "smart" << std::endl;
            smart_multiplication(other);
        }

        return *this;
    }

    Shifted do_inv(uint32_t precision) const {
        /* crazy math begins */
        BigInteger D = (*this);
        D.is_negative_ = 0;

        __int128_t ap = 0;
        __int128_t tmp = 1;
        for (uint32_t i = (data_.size() <= 3 ? 0u : static_cast<uint32_t>(data_.size() - 3u)); i < data_.size(); i++) {
            ap += data_[i] * tmp;
            tmp *= kBase;
        }

        uint32_t ptr = 1;
        __uint128_t suffix = 1;
        tmp = 0;
        while (suffix / ap < kBase) {
            suffix *= kBase;
            tmp++;
        }

        ap = static_cast<uint64_t>(suffix / ap);
        // std::cout << ap * D << std::endl;
        Shifted x = {BigInteger(static_cast<uint64_t>(ap)), tmp + (data_.size() <= 3 ? 0u : static_cast<uint32_t>(data_.size()) - 3)};
        BigInteger two, tmpD;
        while (ptr < precision) {
            // if (ptr < 200) {
            //     std::cout << ptr << ' ' << x.first * D << std::endl;
            // }
            uint32_t shift = static_cast<uint32_t>(D.data_.size()) - std::min(2 * ptr + 1, static_cast<uint32_t>(D.data_.size()));
            tmpD.data_ = std::vector<uint32_t>(D.data_.begin() + static_cast<std::vector<uint32_t>::iterator::difference_type>(shift), D.data_.end());
            two.data_.assign(x.second + 1 - shift, 0u);
            two.data_.back() = 2;
            // std::cerr << "beg: " << 1.0 * clock() / CLOCKS_PER_SEC << std::endl;
            two -= x.first * tmpD;
            x.first *= two;
            // std::cerr << "end " << 1.0 * clock() / CLOCKS_PER_SEC << std::endl;
            x.second += x.second - shift;
            ptr *= 2;
            if (ptr == 1024) ptr = 1023;
            uint32_t needdel = std::max(ptr, static_cast<uint32_t>(x.first.data_.size())) - ptr - 1;
            x.second -= needdel;
            x.first.data_.erase(x.first.data_.begin(), x.first.data_.begin() + needdel);
            // std::cerr << 1.0 * clock() / CLOCKS_PER_SEC << std::endl;
        }

        x.first.normalize();
        return x;
        /* crazy math ends */
    }

    // 2 * a.size() - b.size()

    static std::pair<BigInteger, BigInteger> divmod(BigInteger lhs, BigInteger rhs) {
        if (lhs.data_.size() <= 4 && rhs.data_.size() <= 4) {
            __int128_t ap1 = 0;
            __int128_t tmp = (lhs.is_negative_ ? -1 : 1);
            for (uint32_t i = 0; i < lhs.data_.size(); i++) {
                ap1 += lhs.data_[i] * tmp;
                tmp *= kBase;
            }

            tmp = (rhs.is_negative_ ? -1 : 1);
            __int128_t ap2 = 0;
            for (uint32_t i = 0; i < rhs.data_.size(); i++) {
                ap2 += rhs.data_[i] * tmp;
                tmp *= kBase;
            }

            return {BigInteger(ap1 / ap2), BigInteger(ap1 % ap2)};
        }

        bool asign = lhs.is_negative_;
        bool bsign = rhs.is_negative_;
        lhs.is_negative_ = 0;
        rhs.is_negative_ = 0;
        // std::cout << "!!!!!!!!!!!!!  " << lhs << ' ' << rhs << std::endl;
        if (lhs < rhs) {
            return {0, (asign ? (-lhs) : lhs)};
        }

        Shifted b_inversed = rhs.do_inv(static_cast<uint32_t>(lhs.data_.size() - rhs.data_.size() + 2));
        // std::cout << b_inversed.first << std::endl;
        BigInteger quotient = lhs * b_inversed.first;

        if (quotient.data_.size() < b_inversed.second) {
            quotient.data_.clear();
        } else {
            // std::cout << "WTF???\n";
            quotient.data_.erase(quotient.data_.begin(), quotient.data_.begin() + b_inversed.second);
        }

        if (quotient.data_.size() == 0) {
            quotient = {BigInteger(0ll)};
        }

        BigInteger residue = lhs - rhs * quotient;
        if (residue >= rhs) {
            quotient += 1;
            residue -= rhs;
        }

        if (residue < 0) {
            quotient -= 1;
            residue += rhs;
        }

        if (asign) {
            residue = -residue;
        }

        if (asign != bsign) {
            quotient = -quotient;
        }

        return {quotient, residue};
    }

   public:
    friend BigInteger operator%(const BigInteger &lhs, const BigInteger &rhs) {
        auto result = divmod(lhs, rhs);
        result.second.normalize();
        return result.second;
    }

    friend BigInteger operator/(const BigInteger &lhs, const BigInteger &rhs) {
        auto result = divmod(lhs, rhs);
        result.first.normalize();
        return result.first;
    }

    const BigInteger &operator%=(const BigInteger &other) {
        auto result = divmod((*this), other);
        (*this) = result.second;
        normalize();
        return (*this);
    }

    const BigInteger &operator/=(const BigInteger &other) {
        auto result = divmod((*this), other);
        (*this) = result.first;
        normalize();
        return (*this);
    }

    friend BigInteger operator*(const BigInteger &lhs, const BigInteger &rhs) {
        BigInteger tmp = lhs;
        tmp *= rhs;
        tmp.normalize();
        return tmp;
    }

    explicit operator bool() const {
        return ((*this) != 0);
    }

    BigInteger operator-() const {
        BigInteger copy = (*this);
        copy.is_negative_ ^= 1;
        copy.normalize();
        return copy;
    }

    static BigInteger pow(const BigInteger &x, int deg) {
        BigInteger current_deg2 = x, result = 1;
        while (deg != 0) {
            if (deg % 2) {
                result *= current_deg2;
            }

            deg /= 2;
            if (deg) {
                current_deg2 *= current_deg2;
            }
        }

        return result;
    }
};

BigInteger gcd(BigInteger lhs, BigInteger rhs) {
    int cnt2 = 0;
    lhs.is_negative_ = 0;
    rhs.is_negative_ = 0;
    while (lhs != 0 && rhs != 0) {
        if (lhs.data_[0] % 2 == 0 && rhs.data_[0] % 2 == 0) {
            lhs.DivByTwo();
            rhs.DivByTwo();
            cnt2++;
        } else if ((lhs.data_[0] % 2) ^ (rhs.data_[0] % 2)) {
            (lhs.data_[0] % 2 ? rhs : lhs).DivByTwo();
        } else {
            if (lhs < rhs) {
                rhs -= lhs;
            } else {
                lhs -= rhs;
            }
        }
    }

    return BigInteger::pow(2, cnt2) * (lhs == 0 ? rhs : lhs);
}

class Rational {
    using Shifted = std::pair<BigInteger, uint32_t>;

   public:
    BigInteger numerator, denominator;
    Rational() : numerator(0), denominator(1) {}
    Rational(BigInteger input) {
        numerator = input;
        denominator = 1;
    }

    Rational(int x) {
        numerator = x;
        denominator = 1;
    }

    void normalize() {
        if ((numerator.data_ == std::vector<uint32_t>{0})) {
            numerator = 0;
            numerator.is_negative_ = 0;
            denominator = 1;
            return;
        }

        BigInteger g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;

        if (denominator.is_negative_) {
            numerator.is_negative_ ^= 1;
            denominator.is_negative_ ^= 1;
        }
    }

    void normalize() const {
        return;
    }

    Rational &operator+=(const Rational &other) {
        if (this == &other) {
            numerator *= 2;
            normalize();
            return (*this);
        }

        numerator = numerator * other.denominator + denominator * other.numerator;
        denominator = denominator * other.denominator;
        normalize();
        return (*this);
    }

    friend Rational operator+(const Rational &lhs, const Rational &rhs) {
        Rational copy = lhs;
        copy += rhs;
        return copy;
    }

    Rational &operator-=(const Rational &other) {
        if (this == &other) {
            numerator = BigInteger::kZero;
            denominator = BigInteger::kOne;
            normalize();
            return *this;
        }

        numerator = numerator * other.denominator - denominator * other.numerator;
        denominator = denominator * other.denominator;
        normalize();
        return (*this);
    }

    friend Rational operator-(const Rational &lhs, const Rational &rhs) {
        Rational copy = lhs;
        copy -= rhs;
        return copy;
    }

    Rational &operator*=(const Rational &other) {
        if (this == &other) {
            numerator *= numerator;
            denominator *= denominator;
            normalize();
            return *this;
        }

        numerator *= other.numerator;
        denominator *= other.denominator;
        normalize();
        return (*this);
    }

    friend Rational operator*(const Rational &lhs, const Rational &rhs) {
        Rational copy = lhs;
        copy *= rhs;
        return copy;
    }

    Rational &operator/=(Rational other) {
        if (this == &other) {
            numerator = 1;
            denominator = 1;
            return *this;
        }

        numerator *= other.denominator;
        denominator *= other.numerator;
        normalize();
        return (*this);
    }

    friend Rational operator/(const Rational &lhs, const Rational &rhs) {
        Rational copy = lhs;
        copy /= rhs;
        return copy;
    }

    Rational operator-() {
        Rational copy = (*this);
        copy.numerator.is_negative_ ^= 1;
        copy.normalize();
        return copy;
    }

    friend std::strong_ordering operator<=>(const Rational &lhs, const Rational &rhs) {
        return lhs.numerator * rhs.denominator <=> rhs.numerator * lhs.denominator;
    }

    friend bool operator==(const Rational &a, const Rational &b) = default;

    std::string toString() const {
        normalize();
        std::string res = numerator.toString();
        if (denominator != 1) {
            res += "/" + denominator.toString();
        }

        return res;
    }

    std::string asDecimal(uint32_t precision = 0) const {
        BigInteger a = numerator, b = denominator;
        std::string result = (a.is_negative_ ? "-" : "");
        a.is_negative_ = 0;
        Shifted invb = b.do_inv(static_cast<uint32_t>(2 * a.data_.size() + 2 * precision / 8 + 1));
        BigInteger q = a * invb.first;
        result += BigInteger(std::vector<uint32_t>(q.data_.begin() + std::min(static_cast<uint32_t>(q.data_.size()), invb.second), q.data_.end())).toString();
        result += ".";
        if (precision && invb.second) {
            q.data_.resize(invb.second);
            for (uint32_t i = invb.second - 1; i != -1u; i--) {
                std::string tmp = std::to_string(q.data_[i]);
                tmp = std::string(BigInteger::kBaseSize - tmp.size(), '0') + tmp;
                result += tmp.substr(0, std::min(BigInteger::kBaseSize, precision));
                precision -= std::min(BigInteger::kBaseSize, precision);
                if (precision == 0) break;
            }

            assert(precision == 0);
        } else {
            result += "0";
        }

        return result;
    }

    explicit operator double() const {
        double numerator_approximation = 0, tmp = 1;
        for (uint32_t i = 0; i < std::min<uint64_t>(numerator.data_.size(), 4ull); i++) {
            numerator_approximation += tmp * numerator.data_[numerator.data_.size() - 1 - i];
            tmp /= BigInteger::kBase;
        }

        double denominator_approximation = 0;
        tmp = 1;
        for (uint32_t i = 0; i < std::min<uint64_t>(denominator.data_.size(), 4ull); i++) {
            denominator_approximation += tmp * denominator.data_[denominator.data_.size() - 1 - i];
            tmp /= BigInteger::kBase;
        }

        double result_abs_approximation = numerator_approximation / denominator_approximation * pow(BigInteger::kBase, static_cast<double>(numerator.data_.size()) - static_cast<double>(denominator.data_.size()));
        return result_abs_approximation * (numerator.is_negative_ ^ denominator.is_negative_ ? -1 : 1);
    }
};

std::ostream &operator<<(std::ostream &out, const Rational &number) {
    out << number.toString();
    return out;
}

std::istream &operator>>(std::istream &in, Rational &number) {
    in >> number.numerator;
    number.denominator = 1;
    return in;
}

BigInteger operator""_bi(char const *s, size_t sz) {
    return BigInteger(s, sz);
}

BigInteger operator""_bi(unsigned long long x) {
    return BigInteger(x);
}

const BigInteger BigInteger::kOne = BigInteger(1);
const BigInteger BigInteger::kZero = BigInteger(0);

/*

⠀⠀⠀⠀⠀⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣸⣿⣿⣿⣶⣄⠀⠀⠀⠀⠀⠀⠀⢻⣷⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣠⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⡀⠴⣾⣿⣿⣿⣤⣿⣿⣿⣿⣷⣦⣄⠀⠀⠀⠀⠀⠀⣀⣤⣾⣿⣿⣿⣿⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⡙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣀⠀⠀⣤⣾⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣾⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢹⣿⢸⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⢻⣿⣿⡿⠿⠟⠻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣌⣃⣼⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢀⣠⣤⣴⣿⣿⣍⣠⣶⣶⣶⣦⡈⢻⣿⣿⣿⣿⣿⣿⡿⠟⠋⠉⠋⠉⠛⢿⣿⣿⣿⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠈⠛⠛⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⠾⠿⣿⣿⣿⣿⣿⣤⣴⣶⣿⣿⣷⣶⣀⢹⣿⣿⣤⣶⣶⡶⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣰⣯⣛⣉⢩⡟⠟⢿⣿⣿⣦⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢰⠿⠿⠟⠳⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣍⣀⡤⠀⠝⢉⣹⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣦⣉⣡⣬⣙⣁⣼⣿⣿⣿⣿⣿⣿⣷⠾⠟⠻⢿⡿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢉⣹⣿⣿⣿⣿⣿⣿⣿⣉⣉⣭⣍⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⠿⣷⣾⣿⣿⣿⣿⣿⣿⡿⠟⣓⣈⣅⣙⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⡟⢋⣤⣴⣿⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠾⠿⢿⣿⣿⣿⠏⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣦⠹⡇⣾⣿⣧⢹⣿⡿⠛⢻⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡆⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣶⣤⣀⣉⣁⠈⠠⣤⣶⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣹⣆⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡄⠀⢰⣿⣿⣧⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣧⢹⣿⣿⣿⣆⢻⣿⣿⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣧⠀⣾⣿⣿⣿⣧⡀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⣿⣿⣿⣿⣿⡈⢿⣿⣿⣿⣦⣙⠛⠛⢋⡁⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣰⣿⣿⣿⣿⣿⣷⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⢀⣿⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣁⡀⠀⠀⠀⠀⣀⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⢰⣿⣿⣿⣿⣿⣿⣿⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⢡⣠⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇
⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡄⢽⣿⣿⣿⣿⣿⣿⢌⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠆⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇
*/

using namespace std;

// #define qin cin

#ifdef LOCAL
#define qin cin
#define qout cout
#else
#include <sys/mman.h>
#include <sys/stat.h>

#include <cstring>
#include <iostream>

namespace __io {
using u32 = uint32_t;
using u64 = uint64_t;

namespace QIO_base {
constexpr int O_buffer_default_size = 1 << 18;
constexpr int O_buffer_default_flush_threshold = 40;
struct _int_to_char_tab {
    char tab[40000];
    constexpr _int_to_char_tab() : tab() {
        for (int i = 0; i != 10000; ++i) {
            for (int j = 3, n = i; ~j; --j) {
                tab[i * 4 + j] = n % 10 + 48, n /= 10;
            }
        }
    }
} constexpr _otab;
}  // namespace QIO_base
namespace QIO_I {
using namespace QIO_base;
struct Qinf {
    FILE *f;
    char *bg, *ed, *p;
    struct stat Fl;
    Qinf(FILE *fi) : f(fi) {
        int fd = fileno(f);
        fstat(fd, &Fl);
        bg = (char *)mmap(0, Fl.st_size + 1, PROT_READ, MAP_PRIVATE, fd, 0);
        p = bg, ed = bg + Fl.st_size;
        madvise(p, Fl.st_size + 1, MADV_SEQUENTIAL);
    }
    ~Qinf() { munmap(bg, Fl.st_size + 1); }
    void skip_space() {
        while (*p <= ' ') {
            ++p;
        }
    }
    char get() { return *p++; }
    char seek() { return *p; }
    bool eof() { return p == ed; }
    Qinf &read(char *s, size_t count) { return memcpy(s, p, count), p += count, *this; }
    Qinf &operator>>(u32 &x) {
        skip_space(), x = 0;
        for (; *p > ' '; ++p) {
            x = x * 10 + (*p & 0xf);
        }
        return *this;
    }
    Qinf &operator>>(int &x) {
        skip_space();
        if (*p == '-') {
            for (++p, x = 48 - *p++; *p > ' '; ++p) {
                x = x * 10 - (*p ^ 48);
            }
        } else {
            for (x = *p++ ^ 48; *p > ' '; ++p) {
                x = x * 10 + (*p ^ 48);
            }
        }
        return *this;
    }
    Qinf &operator>>(std::string &s) {
        s.clear();
        skip_space();
        for (; *p > ' '; ++p) {
            s.push_back(*p);
        }
        return *this;
    }
} qin(stdin);
}  // namespace QIO_I
namespace QIO {
using QIO_I::qin;
using QIO_I::Qinf;
}  // namespace QIO
using namespace QIO;
};  // namespace __io
using namespace __io;
#define qout cout
#endif

void fast_bigint_read(BigInteger &number) {
    std::string s;
    qin >> s;
    number = BigInteger(s);
}

void fast_bigint_out(const BigInteger &number) {
    if (number.is_negative_) {
        qout << '-';
    }

    qout << number.data_.back();
    if (number.data_.size() == 1) return;

    constexpr static u32 rofl[9] = {100'000'000, 10'000'000, 1'000'000, 100'000, 10'000, 1'000, 100, 10, 1};
    for (uint32_t i = static_cast<uint32_t>(number.data_.size() - 2); i != -1u; i--) {
        u32 cur = 0;
        for (int j = 0; j < 9; j++) {
            if (number.data_[i] < rofl[j])
                qout << '0';
            else {
                qout << number.data_[i];
                break;
            }
        }

        // out << std::setfill('0') << std::setw(static_cast<int>(kBaseSize)) << number.data_[i];
    }

    // qout << '\n';
}

int32_t main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    BigInteger a = 1, b = 1;
    int n = 1;
    qin >> n;
    while (n--) {
        // cin >> a >> b;
        fast_bigint_read(a);
        fast_bigint_read(b);
        // a.data_.assign(2'000'000 / 9, 0);
        // a.data_.back() = 1;
        // b = 1007;
        auto [p, q] = BigInteger::divmod(a, b);
        // cout << p << ' ' << q << endl;
        fast_bigint_out(p);
        qout << ' ';
        fast_bigint_out(q);
        qout << '\n';
    }

    cerr << 1.0 * clock() / CLOCKS_PER_SEC << endl;

    // cout << "XUI\n";
}