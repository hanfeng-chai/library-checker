#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#include <bits/stdc++.h>
#include <immintrin.h>
using namespace std;

constexpr int mod = 998244353;
constexpr int pr = 3;
using u32 = uint32_t;
using u64 = uint64_t;
using poly = vector<u32>;

mt19937 rnd(227);

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
    u32 mod;
    u32 mod2;
    u32 n_inv;
    u32 r;
    u32 r2;

    Montgomery() = default;
    Montgomery(u32 mod) : mod(mod) {
        assert(mod % 2);
        assert(mod < (1 << 30));
        n_inv = -mod & 3;
        for (int i = 0; i < 4; i++) {
            n_inv *= 2u + n_inv * mod;
        }
        assert(n_inv * mod == -1u);

        mod2 = 2 * mod;
        r = (1ULL << 32) % mod;
        r2 = r * u64(r) % mod;
    }

    u32 shrink(u32 val) const {
        return std::min(val, val - mod);
    }

    u32 shrink2(u32 val) const {
        return std::min(val, val - mod2);
    }

    u32 shrink_n(u32 val) const {
        return std::min(val, val + mod);
    }

    u32 shrink2_n(u32 val) const {
        return std::min(val, val + mod2);
    }

    template <bool strict = false>
    u32 reduce(u64 val) const {
        u32 res = (val + u32(val) * n_inv * u64(mod)) >> 32;
        if constexpr (strict)
            res = shrink(res);
        return res;
    }

    template <bool strict = false>
    u32 mul(u32 a, u32 b) const {
        u64 val = u64(a) * b;
        u32 res = (val + u32(val) * n_inv * u64(mod)) >> 32;
        if constexpr (strict)
            res = shrink(res);
        return res;
    }

    template <bool input_in_space = false, bool in_space_res = true>
    u32 power(u32 b, u32 e) const {
        if constexpr (!input_in_space)
            b = mul(b, r2);
        u32 res = (in_space_res ? r : 1);
        for (; e > 0; e >>= 1) {
            if (e & 1)
                res = mul(res, b);
            b = mul(b, b);
        }

        res = shrink(res);
        return res;
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

const Montgomery_simd mts(mod);
const Montgomery mt(mod);
alignas(32) u32 buf[1 << 22];
array<u32 *, 2> dat;

const int K = 17;
struct precalc {
    u32 pr_huinia[21];
    u32 mul_const[21];
    alignas(32) u32 wtf[2][3][21][8];

    precalc() {
        for (int k = 1; k < 21; k++) {
            pr_huinia[k] = mt.power<false, true>(pr, (mod - 1) >> (k));
            mul_const[k] = mt.power<true, true>(mt.power<false, true>(2, k), mod - 2);
        }

        for (int k = 3; k < 20; k++) {
            {
                u32 tmp_trash = pr_huinia[k - 2];
                u64x4 w = setr_u64x4(mt.r, mt.r, tmp_trash, tmp_trash);
                store_u32x8(wtf[0][0][k], (u32x8)w);
            }

            {
                u32 tmp_trash = pr_huinia[k - 1];
                u64x4 w = setr_u64x4(mt.r, tmp_trash, pr_huinia[k - 2], mt.mul<false>(pr_huinia[k - 1], pr_huinia[k - 2]));
                store_u32x8(wtf[0][1][k], (u32x8)w);
            }

            {
                u32 tmp_trash = pr_huinia[k];
                wtf[0][2][k][0] = mt.r;
                for (int i = 1; i < 8; i++) {
                    wtf[0][2][k][i] = mt.mul<false>(wtf[0][2][k][i - 1], tmp_trash);
                }
            }

            for (int a = 0; a < 3; a++) {
                u32 p = pr_huinia[k - 2 + a + 1];
                for (int j = 0; j < 8; j++) {
                    wtf[1][a][k][j] = mt.mul<false>(wtf[0][a][k][j], p);
                }
            }
        }
    }
};

precalc pablo;

struct ntt {
    int f = 0;
    ntt() {
    }

    template <bool red = false>
    [[gnu::noinline]] void ntt_f_slow(int lg, u32 *a, u32 *buf = ::buf) {
        dat = array<u32 *, 2>{a, buf};
        bool bl = 1;

        f += (1 << lg);

        auto pr_menis_strange = [&](int k) {
            u32 w = (red ? pablo.pr_huinia[lg - k + 1] : mt.r), st = pablo.pr_huinia[lg - k];
            for (int i = 0; i < (1 << lg); i += (2 << k)) {
                for (int j = 0; j < (1 << k); j++) {
                    u32 a = dat[!bl][i + j], b = dat[!bl][i + (1 << k) + j];
                    u32 c = mt.mul<true>(w, b);

                    dat[bl][i / 2 + j] = mt.shrink(a + c);
                    dat[bl][i / 2 + j + (1 << lg - 1)] = mt.shrink_n(a - c);
                }

                w = mt.mul(w, st);
            }
        };

        for (int k = lg - 1; k >= 0; k--) {
            pr_menis_strange(k);
            bl ^= 1;
        }

        if (!bl) {
            memcpy(a, buf, 4 << lg);
        }
    }

    template <bool red = false>
    [[gnu::noinline]] void ntt_f_small_multi(int lg, u32 *a, u32 *buf = ::buf) {
        const auto mt = ::mt;
        const auto mts = ::mts;

        f += 8;
        if (lg == 0) return;
        if (lg == 1) {
            u32x8 dat = load_u32x8(a);
            u32x8 a_ = (u32x8)(_mm256_shuffle_epi32((__m256i)dat, 0b10'10'00'00));
            u32x8 b_ = (u32x8)(_mm256_shuffle_epi32((__m256i)dat, 0b11'11'01'01));
            b_ = blend_u32x8<0b10'10'10'10>(b_, sub_u32x8(set1_u32x8(mod), b_));
            if (red) b_ = mts.mul<true, true>(b_, set1_u32x8(pablo.pr_huinia[2]));

            store_u32x8(a, mts.shrink(add_u32x8(a_, b_)));
        } else if (lg == 2) {
            u32x8 dat = load_u32x8(a);

            u32x8 a_ = (u32x8)(_mm256_shuffle_epi32((__m256i)dat, 0b01'00'01'00));
            u32x8 b_ = (u32x8)(_mm256_shuffle_epi32((__m256i)dat, 0b11'10'11'10));
            b_ = blend_u32x8<0b11'00'11'00>(b_, sub_u32x8(set1_u32x8(mod), b_));
            if (red) b_ = mts.mul<true, true>(b_, set1_u32x8(pablo.pr_huinia[2]));

            a_ = mts.shrink(add_u32x8(a_, b_));
            dat = a_;

            a_ = (u32x8)(_mm256_shuffle_epi32((__m256i)dat, 0b10'00'10'00));
            b_ = (u32x8)(_mm256_shuffle_epi32((__m256i)dat, 0b11'01'11'01));
            if (red)
                b_ = mts.mul<true, false>(b_, setr_u32x8(888603487, 690661211, mod - 888603487, mod - 690661211, 888603487, 690661211, mod - 888603487, mod - 690661211));
            else
                b_ = mts.mul<true, false>(b_, setr_u32x8(301989884, 691295370, mod - 301989884, mod - 691295370, 301989884, 691295370, mod - 301989884, mod - 691295370));

            store_u32x8(a, mts.shrink(add_u32x8(a_, b_)));
        } else if (lg == 3) {
            u32x8 dat = load_u32x8(a);

            u32x8 a_ = (u32x8)_mm256_permute4x64_epi64((__m256i)dat, 0b01'00'01'00);
            u32x8 b_ = (u32x8)_mm256_permute4x64_epi64((__m256i)dat, 0b11'10'11'10);
            b_ = blend_u32x8<0b11'11'00'00>(b_, sub_u32x8(set1_u32x8(mod), b_));
            if (red) b_ = mts.mul<true, true>(b_, set1_u32x8(pablo.pr_huinia[2]));
            a_ = mts.shrink(add_u32x8(a_, b_));

            b_ = (u32x8)(_mm256_shuffle_epi32((__m256i)a_, 0b11'10'11'10));
            a_ = (u32x8)(_mm256_shuffle_epi32((__m256i)a_, 0b01'00'01'00));
            if (red)
                b_ = mts.mul<true, false>(b_, setr_u32x8(888603487, 888603487, 1107885219, 1107885219, 690661211, 690661211, 307583142, 307583142));
            else
                b_ = mts.mul<true, false>(b_, setr_u32x8(301989884, 301989884, 696254469, 696254469, 691295370, 691295370, 306948983, 306948983));

            a_ = mts.shrink(add_u32x8(a_, b_));
            a_ = (u32x8)_mm256_permute4x64_epi64((__m256i)a_, 0b11'01'10'00);

            b_ = (u32x8)(_mm256_shuffle_epi32((__m256i)a_, 0b11'11'01'01));
            a_ = (u32x8)(_mm256_shuffle_epi32((__m256i)a_, 0b10'10'00'00));
            if (red)
                b_ = mts.mul<true, false>(b_, setr_u32x8(54518517, 943725836, 431422394, 566821959, 138723248, 859521105, 49870511, 948373842));
            else
                b_ = mts.mul<true, false>(b_, setr_u32x8(301989884, 696254469, 888603487, 109640866, 691295370, 306948983, 690661211, 307583142));

            a_ = mts.shrink(add_u32x8(a_, b_));
            a_ = (u32x8)_mm256_permutevar8x32_epi32((__m256i)a_, (__m256i)setr_u32x8(0, 2, 4, 6, 1, 3, 5, 7));
            store_u32x8(a, a_);
        } else {
            for (int i = 0; i < 8; i += (1 << lg)) {
                ntt_f_slow<red>(lg, a + i, buf);
            }
        }
    }

    // 301989884 888603487 691295370 690661211 696254469 109640866 306948983 307583142
    // 54518517 431422394 138723248 49870511 943725836 566821959 859521105 948373842

    template <bool red = false>
    [[gnu::noinline]] void ntt_f(int lg, u32 *a, u32 *buf = ::buf) {
        const auto mt = ::mt;
        const auto mts = ::mts;

        if (lg == 0) return;
        dat = array<u32 *, 2>{a, buf};
        bool bl = 1;

        if (lg <= 3) {
            if (lg <= 2)
                ntt_f_slow<red>(lg, a, buf);
            else
                ntt_f_small_multi<red>(lg, a, buf);
            return;
        }

        f += (1 << lg);

        auto mr_penis = [&](int k) {
            u32 sx1 = pablo.pr_huinia[lg - k], sx2 = pablo.pr_huinia[lg - k + 1];

            u64x4 w = setr_u64x4((red ? pablo.pr_huinia[lg - k + 1] : mt.r), (red ? pablo.pr_huinia[lg - k + 2] : mt.r), (red ? pablo.pr_huinia[lg - k + 1] : mt.r), mt.mul((red ? pablo.pr_huinia[lg - k + 2] : mt.r), pablo.pr_huinia[2]));
            u64x4 st = setr_u64x4(sx1, sx2, sx1, sx2);

            for (int i = 0; i < (1 << lg); i += (2 << k)) {
                for (int j = 0; j < (1 << k - 1); j += 8) {
                    u32x8 a = load_u32x8(dat[!bl] + i + j);
                    u32x8 b = load_u32x8(dat[!bl] + i + j + (1 << k - 1));
                    u32x8 c = load_u32x8(dat[!bl] + i + j + (2 << k - 1));
                    u32x8 d = load_u32x8(dat[!bl] + i + j + (3 << k - 1));

                    u32x8 w1 = (u32x8)_mm256_shuffle_epi32((__m256i)w, 0b00'00'00'00);
                    u32x8 w2 = (u32x8)_mm256_permute4x64_epi64((__m256i)w, 0b01'01'01'01);
                    u32x8 w3 = (u32x8)_mm256_permute4x64_epi64((__m256i)w, 0b11'11'11'11);

                    u32x8 ac = mts.mul<true, true>(c, w1);
                    u32x8 bd = mts.mul<true, true>(d, w1);

                    c = mts.shrink_n(sub_u32x8(a, ac));
                    a = mts.shrink(add_u32x8(a, ac));

                    d = mts.shrink_n(sub_u32x8(b, bd));
                    b = mts.shrink(add_u32x8(b, bd));

                    u32x8 ab = mts.mul<true, true>(b, w2);
                    u32x8 cd = mts.mul<true, true>(d, w3);

                    b = mts.shrink_n(sub_u32x8(a, ab));
                    a = mts.shrink(add_u32x8(a, ab));

                    d = mts.shrink_n(sub_u32x8(c, cd));
                    c = mts.shrink(add_u32x8(c, cd));

                    u32 penis = i / 4;
                    store_u32x8(dat[bl] + penis + j, a);
                    store_u32x8(dat[bl] + penis + j + (1 << lg - 2), c);
                    store_u32x8(dat[bl] + penis + j + (2 << lg - 2), b);
                    store_u32x8(dat[bl] + penis + j + (3 << lg - 2), d);
                }

                w = mts.mul(w, st);
            }
        };

        auto pr_menis_strange_simd = [&](int k) {
            u32 w = (red ? pablo.pr_huinia[lg - k + 1] : mt.r), st = pablo.pr_huinia[lg - k];
            for (int i = 0; i < (1 << lg); i += (2 << k)) {
                u32x8 FUCKYOU = set1_u32x8(w);
                for (int j = 0; j < (1 << k); j += 8) {
                    u32x8 a = load_u32x8(dat[!bl] + i + j), b = load_u32x8(dat[!bl] + i + j + (1 << k));
                    u32x8 c = mts.mul<true, true>(b, FUCKYOU);

                    b = mts.shrink_n(sub_u32x8(a, c));
                    a = mts.shrink(add_u32x8(a, c));

                    store_u32x8(dat[bl] + i / 2 + j, a);
                    store_u32x8(dat[bl] + i / 2 + j + (1 << lg - 1), b);
                }

                w = mt.mul<false>(w, st);
            }
        };

        pr_menis_strange_simd(lg - 1);
        bl ^= 1;

        int k = lg - 2;
        for (; k >= 4; k -= 2) {
            mr_penis(k);
            bl ^= 1;
        }

        if (k == 3) {
            pr_menis_strange_simd(k--);
            bl ^= 1;
        }

        // let the chaos begin

        // -3
        {
            u64x4 st = set1_u64x4(pablo.pr_huinia[lg - 3]);
            u64x4 w = (u64x4)load_u32x8(pablo.wtf[red][0][lg]);
            for (int i = 0; i < (1 << lg); i += 16) {
                u32x8 a = load_u32x8(dat[!bl] + i), b = load_u32x8(dat[!bl] + i + 8);
                u32x8 c = (u32x8)_mm256_permute2x128_si256((__m256i)a, (__m256i)b, 0b00'10'00'00);
                u32x8 d = (u32x8)_mm256_permute2x128_si256((__m256i)a, (__m256i)b, 0b00'11'00'01);

                d = mts.mul<true, true>(d, (u32x8)w);

                store_u32x8(dat[bl] + i / 2, mts.shrink(add_u32x8(c, d)));
                store_u32x8(dat[bl] + i / 2 + (1 << lg - 1), mts.shrink_n(sub_u32x8(c, d)));

                w = mts.mul<false>(w, st);
            }

            bl ^= 1;
        }

        // -2
        {
            u64x4 st = set1_u64x4(pablo.pr_huinia[lg - 3]);
            u64x4 w = (u64x4)load_u32x8(pablo.wtf[red][1][lg]);
            for (int i = 0; i < (1 << lg); i += 16) {
                u32x8 a = load_u32x8(dat[!bl] + i), b = load_u32x8(dat[!bl] + i + 8);
                a = (u32x8)_mm256_permutevar8x32_epi32((__m256i)a, (__m256i)setr_u32x8(0, 1, 4, 5, 2, 3, 6, 7));
                b = (u32x8)_mm256_permutevar8x32_epi32((__m256i)b, (__m256i)setr_u32x8(0, 1, 4, 5, 2, 3, 6, 7));
                u32x8 c = (u32x8)_mm256_permute2x128_si256((__m256i)a, (__m256i)b, 0b00'10'00'00);
                u32x8 d = (u32x8)_mm256_permute2x128_si256((__m256i)a, (__m256i)b, 0b00'11'00'01);

                d = mts.mul<true, true>(d, (u32x8)w);

                store_u32x8(dat[bl] + i / 2, mts.shrink(add_u32x8(c, d)));
                store_u32x8(dat[bl] + i / 2 + (1 << lg - 1), mts.shrink_n(sub_u32x8(c, d)));

                w = mts.mul<false>(w, st);
            }

            bl ^= 1;
        }

        // -1
        {
            u32x8 st = set1_u32x8(pablo.pr_huinia[lg - 3]);
            u32x8 w = load_u32x8(pablo.wtf[red][2][lg]);
            for (int i = 0; i < (1 << lg); i += 16) {
                u32x8 a = load_u32x8(dat[!bl] + i), b = load_u32x8(dat[!bl] + i + 8);
                a = (u32x8)_mm256_permutevar8x32_epi32((__m256i)a, (__m256i)setr_u32x8(0, 2, 4, 6, 1, 3, 5, 7));
                b = (u32x8)_mm256_permutevar8x32_epi32((__m256i)b, (__m256i)setr_u32x8(0, 2, 4, 6, 1, 3, 5, 7));
                u32x8 c = (u32x8)_mm256_permute2x128_si256((__m256i)a, (__m256i)b, 0b00'10'00'00);
                u32x8 d = (u32x8)_mm256_permute2x128_si256((__m256i)a, (__m256i)b, 0b00'11'00'01);

                d = mts.mul<true, false>(d, w);

                store_u32x8(dat[bl] + i / 2, mts.shrink(add_u32x8(c, d)));
                store_u32x8(dat[bl] + i / 2 + (1 << lg - 1), mts.shrink_n(sub_u32x8(c, d)));

                w = mts.mul<false>(w, st);
            }

            bl ^= 1;
        }

        if (!bl) {
            memcpy(a, buf, 4 << lg);
        }
    }

    [[gnu::noinline]] void intt_f(int lg, u32 *data, u32 *buf = ::buf) {
        reverse(data + 1, data + (1 << lg));
        ntt_f(lg, data, buf);
    }

    [[gnu::noinline]] void intt_f_small_multi(int lg, u32 *data, u32 *buf = ::buf) {
        if (lg == 2) {
            u32x8 dat = load_u32x8(data);
            store_u32x8(data, (u32x8)_mm256_shuffle_epi32((__m256i)dat, 0b01'10'11'00));
        } else if (lg == 3) {
            u32x8 dat = load_u32x8(data);
            store_u32x8(data, (u32x8)_mm256_permutevar8x32_epi32((__m256i)dat, (__m256i)setr_u32x8(0, 7, 6, 5, 4, 3, 2, 1)));
        } else {
            for (int i = 0; i < 8; i += (1 << lg)) {
                reverse(data + i + 1, data + i + (1 << lg));
            }
        }

        ntt_f_small_multi(lg, data, buf);
    }

    template <bool del = true>
    [[gnu::noinline]] void norm_and_mul_slow(u32 K, u32 *a, u32 *b, u32 *c = nullptr) {
        const auto mt = ::mt;

        u32 tmp = (del ? mt.r : pablo.mul_const[K]);
        for (int i = 0; i < (1 << K); i++) {
            if (!c)
                a[i] = mt.mul<1>(a[i], mt.mul(b[i], tmp));
            else
                c[i] = mt.mul<1>(a[i], mt.mul(b[i], tmp));
        }
    }

    template <bool bl = false, int need = 16>
    [[gnu::noinline]] void norm_and_mul(u32 K, u32 *a, u32 *b, u32 *c = nullptr) {
        const auto mts = ::mts;
        const auto mt = ::mt;

        u32 tmp = (bl ? mt.r2 : pablo.mul_const[K]);
        if (need > 8 || K >= 3) {
            u32x8 md = set1_u32x8(tmp);
            for (int i = 0; i < max(need, (1 << K)); i += 8) {
                u32x8 a_ = load_u32x8(a + i);
                u32x8 b_ = load_u32x8(b + i);

                if (bl) {
                    a_ = mts.mul<true, false>(a_, b_);
                } else {
                    a_ = mts.mul<true, false>(a_, mts.mul<false, true>(b_, md));
                }

                if (!c)
                    store_u32x8(a + i, a_);
                else
                    store_u32x8(c + i, a_);
            }
        } else {
            // FUCK YOURSELF
            norm_and_mul_slow<bl>(K, a, b, c);
        }
    }

    template <bool already_intt = false>
    void transmul(u32 K, u32 *a, u32 *b, u32 *buf = ::buf) {
        if (!already_intt) intt_f(K, a, buf);

        if (K <= 3)
            norm_and_mul_slow<!already_intt>(K, a, b);
        else
            norm_and_mul<already_intt>(K, a, b);

        ntt_f(K, a, buf);

        memset(a + (1 << K - 1), 0, (4 << K - 1));
    }

    template <bool already_intt = false>
    void transmul_multi(u32 K, u32 *a, u32 *b, u32 *buf = ::buf) {
        if (!already_intt) intt_f_small_multi(K, a, buf);

        norm_and_mul<already_intt, 8>(K, a, b);
        ntt_f_small_multi(K, a, buf);

        u32x8 a_ = load_u32x8(a);
        if (K == 1) {
            a_ = blend_u32x8<0b10'10'10'10>(a_, set1_u32x8(0));
        } else if (K == 2) {
            a_ = blend_u32x8<0b11'00'11'00>(a_, set1_u32x8(0));
        } else if (K == 3) {
            a_ = blend_u32x8<0b11'11'00'00>(a_, set1_u32x8(0));
        }

        store_u32x8(a, a_);
    }

    template <bool b_values = false>
    void mul(u32 K, u32 *a, u32 *b, u32 *c = nullptr) {
        ntt_f(K, a);
        ntt_f(K, b);

        if (c) memcpy(c, a, 4 << K);
        norm_and_mul(K, a, b, c);

        intt_f(K, (c ? c : a));
    }

    template <bool wtf = false>
    __attribute__((optimize("O3"))) [[gnu::noinline]] void extend_from_vals(u32 K, u32 *a, u32 *buf) {
        const auto mt = ::mt;
        const auto mts = ::mts;

        u32 sz = max(3u, K);
        memcpy(buf, a, 4 << sz);
        u32 tmp = pablo.mul_const[K];
        auto tmp2 = set1_u32x8(tmp);
        for (int i = 0; i < (1 << sz); i += 8) {
            store_u32x8(a + i, mts.mul<true>(load_u32x8(a + i), tmp2));
        }

        if (K <= 3) {
            intt_f_small_multi(K, a, buf + (1 << sz));
            u32x8 a_ = mts.shrink_n(sub_u32x8(load_u32x8(a), set1_u32x8(mt.r)));
            a_ = mts.shrink(add_u32x8(a_, a_));

            if (K == 1) {
                a_ = shuffle_u32x8<0b10'10'00'00>(a_);
            } else if (K == 2) {
                a_ = shuffle_u32x8<0b00'00'00'00>(a_);
            } else {
                a_ = permute_u32x8(a_, set1_u32x8(0));
            }

            ntt_f_small_multi<1>(K, a, buf + (1 << sz));

            memcpy(buf + (1 << sz), a, 4 << sz);
            u32x8 b_ = load_u32x8(buf + (1 << sz));
            store_u32x8(buf + (1 << sz), mts.shrink_n(sub_u32x8(b_, a_)));
        } else {
            intt_f(K, a, buf + (1 << sz));
            u32 md = mt.shrink_n(a[0] - mt.r);
            md = mt.shrink(md + md);
            ntt_f<1>(K, a, buf + (1 << sz));

            memcpy(buf + (1 << sz), a, 4 << sz);
            if (wtf) {
                for (int i = 0; i < (1 << sz); i += 8) {
                    u32x8 a_ = load_u32x8(buf + i + (1 << sz));
                    store_u32x8(buf + i + (1 << sz), mts.shrink_n(sub_u32x8(a_, set1_u32x8(md))));
                }
            }
        }

        for (int i = 0; i < (1 << sz); i++) {
            a[2 * i] = buf[i];
            a[2 * i + 1] = buf[i + (1 << sz)];
        }
    }

    template <bool res_vals = false>
    void inv(u32 K, u32 *a, u32 *buf) {
        const auto mt = ::mt;
        const auto mts = ::mts;

        memset(buf, 0, 8 << K);
        buf[0] = mt.power<true, true>(a[0], mod - 2);

        for (int i = 0; i < K; i++) {
            memcpy(buf + (2 << K), a, 4 << (i + 1));
            ntt_f(i + 2, buf + (2 << K), buf + (4 << K));
            ntt_f(i + 2, buf, buf + (4 << K));

            if (i == 0)
                for (int j = 0; j < (1 << i + 2); j++) {
                    buf[j] = mt.mul<true>(mt.mul<true>(mt.shrink_n(2 * mt.r - mt.mul<true>(buf[j], buf[(2 << K) + j])), pablo.mul_const[i + 2]), buf[j]);
                }
            else {
                for (int j = 0; j < (1 << i + 2); j += 8) {
                    u32x8 a_ = load_u32x8(buf + j);
                    u32x8 b_ = load_u32x8(buf + j + (2 << K));

                    if (!res_vals || i != K - 1)
                        store_u32x8(buf + j, mts.mul<true, false>(mts.shrink_n(sub_u32x8(set1_u32x8(2 * mt.r), mts.mul<true, false>(a_, b_))), mts.mul<true, true>(a_, set1_u32x8(pablo.mul_const[i + 2]))));
                    else {
                        store_u32x8(buf + j, mts.mul<true, false>(mts.shrink_n(sub_u32x8(set1_u32x8(2 * mt.r), mts.mul<true, false>(a_, b_))), a_));
                    }
                }
            }

            if (!res_vals || i != K - 1) intt_f(i + 2, buf, buf + (4 << K));
            memset(buf + (1 << i + 1), 0, (4 << i + 1));
        }

        memcpy(a, buf, 4 << K);
    }

    template <bool res_vals = false>
    void inv_fast_fromvals(u32 K, u32 *a, u32 *buf, u32 *all_vals = nullptr) {
        const auto mt = ::mt;
        const auto mts = ::mts;

        u32 *X = buf;
        u32 *A = buf + (2 << K);
        u32 *WTF = buf + (4 << K);
        u32 *nw = buf + (6 << K);

        memset(buf, 0, 8 << K);
        buf[0] = mt.power<true, true>(a[0], mod - 2);

        for (int i = 0; i < K; i++) {
            memcpy(A, a, 4 << (i + 1));
            memcpy(WTF, X, 4 << (i));

            if (i != K - 1 || !all_vals)
                ntt_f(i + 1, A, nw);
            else
                memcpy(A, all_vals, 4 << K);

            ntt_f(i + 1, X, nw);

            norm_and_mul<0, 0>(i + 1, A, X);

            intt_f(i + 1, A, nw);

            for (int j = 0; j < (1 << i); j++) {
                A[j] = mod - A[j + (1 << i)];
                A[j + (1 << i)] = 0;
            }

            ntt_f(i + 1, A, nw);
            norm_and_mul<0, 0>(i + 1, X, A);
            intt_f(i + 1, X, nw);
            memcpy(X + (1 << i), X, 4 << i);
            memcpy(X, WTF, 4 << i);
        }

        memcpy(a, X, 4 << K);
    }
};

ntt nt;
alignas(32) u32 coef[1 << 19];

int fnd(u32 *dt, int h, int lg) {
    return find(dt, dt + (1 << lg), h) - dt;
}

#ifdef LOCAL
#define qin cin
#define qout cout
#else
#include <sys/mman.h>
#include <sys/stat.h>

#include <cstring>
#include <iostream>

// io from https://judge.yosupo.jp/submission/142782

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
} qin(stdin);
}  // namespace QIO_I
namespace QIO_O {
using namespace QIO_base;
struct Qoutf {
    FILE *f;
    char *bg, *ed, *p;
    char *ed_thre;
    int fp;
    u64 _fpi;
    Qoutf(FILE *fo, size_t sz = O_buffer_default_size) : f(fo), bg(new char[sz]), ed(bg + sz), p(bg), ed_thre(ed - O_buffer_default_flush_threshold), fp(6), _fpi(1000000ull) {}
    void flush() { fwrite_unlocked(bg, 1, p - bg, f), p = bg; }
    void chk() {
        if (__builtin_expect(p > ed_thre, 0)) {
            flush();
        }
    }
    ~Qoutf() {
        flush();
        delete[] bg;
    }
    void put4(u32 x) {
        if (x > 99u) {
            if (x > 999u) {
                memcpy(p, _otab.tab + (x << 2) + 0, 4), p += 4;
            } else {
                memcpy(p, _otab.tab + (x << 2) + 1, 3), p += 3;
            }
        } else {
            if (x > 9u) {
                memcpy(p, _otab.tab + (x << 2) + 2, 2), p += 2;
            } else {
                *p++ = x ^ 48;
            }
        }
    }
    void put2(u32 x) {
        if (x > 9u) {
            memcpy(p, _otab.tab + (x << 2) + 2, 2), p += 2;
        } else {
            *p++ = x ^ 48;
        }
    }
    Qoutf &write(const char *s, size_t count) {
        if (count > 1024 || p + count > ed_thre) {
            flush(), fwrite_unlocked(s, 1, count, f);
        } else {
            memcpy(p, s, count), p += count, chk();
        }
        return *this;
    }
    Qoutf &operator<<(char ch) { return *p++ = ch, *this; }
    Qoutf &operator<<(u32 x) {
        if (x > 99999999u) {
            put2(x / 100000000u), x %= 100000000u;
            memcpy(p, _otab.tab + ((x / 10000u) << 2), 4), p += 4;
            memcpy(p, _otab.tab + ((x % 10000u) << 2), 4), p += 4;
        } else if (x > 9999u) {
            put4(x / 10000u);
            memcpy(p, _otab.tab + ((x % 10000u) << 2), 4), p += 4;
        } else {
            put4(x);
        }
        return chk(), *this;
    }
    Qoutf &operator<<(int x) {
        if (x < 0) {
            *p++ = '-', x = -x;
        }
        return *this << static_cast<u32>(x);
    }
} qout(stdout);
}  // namespace QIO_O
namespace QIO {
using QIO_I::qin;
using QIO_I::Qinf;
using QIO_O::qout;
using QIO_O::Qoutf;
}  // namespace QIO
using namespace QIO;
};  // namespace __io
using namespace __io;
#endif

// if K <= 3 b is useless
[[gnu::noinline]] void fucking_bullshit2(int K, u32 *a, u32 *b, u32 *c) {
    u32 sz = max(3, K);

    if (K <= 3) {
        for (int i = 0; i < 16; i += (2 << K)) {
            for (int j = 0; j < (1 << K); j++) {
                c[i / 2 + j] = mt.mul<true>(a[i + j], a[i + j + (1 << K)]);
            }
        }

        nt.extend_from_vals<1>(K, c, buf);
    } else {
        nt.norm_and_mul<1>(K, a, b, c);
        nt.extend_from_vals<1>(K, c, buf);
    }
}

void inv_bullshit_fast(int K, u32 *a) {
    auto mts = ::mts;
    // reorder
    {
        for (int i = 0; i < (1 << K); i += 2) {
            buf[i / 2] = a[i];
        }

        for (int i = 1; i < (1 << K); i += 2) {
            buf[i / 2 + (1 << K - 1)] = a[i];
        }

        memcpy(a, buf, 4 << K);
    }

    // cum
    {
        nt.ntt_f(K - 1, a + (1 << K - 1), buf);

        u32x8 st = mts.mul<false, true>(load_u32x8(pablo.wtf[0][2][K]), set1_u32x8(pablo.mul_const[K - 1]));
        u32x8 w = set1_u32x8(pablo.pr_huinia[K - 3]);
        for (int i = 0; i < (1 << K - 1); i += 8) {
            u32x8 a_ = mts.mul<true, false>(load_u32x8(a + i + (1 << K - 1)), st);
            store_u32x8(a + i + (1 << K - 1), a_);
            st = mts.mul<false, true>(st, w);
        }
    }

    // intt
    {
        nt.intt_f(K - 1, a + (1 << K - 1), buf);
    }

    // copy
    {
        for (int i = 0; i < (1 << K - 1); i++) {
            a[i] = mt.shrink(a[i] + a[i + (1 << K - 1)]);
        }
    }

    memcpy(a + (1 << K - 1), a, 4 << (K - 1));
}

int32_t main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    auto const mts = ::mts;
    auto const mt = ::mt;

    int lg = 17;

    u32 n;
    qin >> n;

    for (int i = 0; i < n; i++) {
        qin >> coef[i];
        coef[i] = mt.mul<true>(coef[i], mt.r2);
    }

    nt.inv_fast_fromvals(19, coef, buf);
    for (int i = 0; i < n; i++) {
        qout << mt.mul<true>(1, coef[i]) << ' ';
    }

    return 0;
}

/*
1 2 3 4 0 0 0 0 0 0 0 0 0 0 0 0


mod x^i

p * inv = 1 mod x^i
p * inv - 1 = 0 mod x^i
p^2 * inv^2 - 2 * p * inv + 1 = 0 mod x^2i
2 * p * inv - p^2 * inv^2 = 1 mod x^2i
p * (2 * inv - p * inv^2) = 1 mod x^2i
newinv = 2 * inv - p * inv^2
newinv = inv * (2 - p * inv)

0 86583717 2 911660634 998244352 173167435 3 825076916 998244351 259751153 4 738493198 998244350 346334871 5 651909480
0 998244351 998244352 998244350 998244351 998244349 998244350 998244348 1 998244352 1 998244352 1 998244352 1 998244352

460327768 743761004 914277111 87115334 348278494 599171512 248828830 91827276 14398211 432628176 474022609


*/