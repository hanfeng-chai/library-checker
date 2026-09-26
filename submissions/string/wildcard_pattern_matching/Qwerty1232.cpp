#include <immintrin.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#pragma GCC optimize("O3")
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
};

using i256 = __m256i;
using u32x8 = u32 __attribute__((vector_size(32)));
using u64x4 = u64 __attribute__((vector_size(32)));

u32x8 load_u32x8(const u32* ptr) {
    return (u32x8)_mm256_load_si256((const i256*)ptr);
}
void store_u32x8(u32* ptr, u32x8 vec) {
    _mm256_store_si256((i256*)ptr, (i256)vec);
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

   private:
    static constexpr int LG = 32;  // more than enough for u32

    Montgomery mt;
    Montgomery_simd mts;

    u32 w[4], wr[4];
    u32 wd[LG], wrd[LG];

    u64x4 wt_init, wrt_init;
    u64x4 wd_x4[LG], wrd_x4[LG];

    u64x4 wl_init;
    u64x4 wld_x4[LG];

    static u32 find_pr_root(u32 mod, const Montgomery& mt) {
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
    static void butterfly_x2(u32* ptr_a, u32* ptr_b, u32x8 w, const Montgomery_simd& mts) {
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
    static void butterfly_x4(u32* ptr_a, u32* ptr_b, u32* ptr_c, u32* ptr_d, u32x8 w1, u32x8 w2, u32x8 w3, const Montgomery_simd& mts) {
        u32x8 a = load_u32x8(ptr_a), b = load_u32x8(ptr_b), c = load_u32x8(ptr_c), d = load_u32x8(ptr_d);
        if (!transposed) {
            butterfly_x2<false, trivial>((u32*)&a, (u32*)&c, w1, mts);
            butterfly_x2<false, trivial>((u32*)&b, (u32*)&d, w1, mts);
            butterfly_x2<false, trivial>((u32*)&a, (u32*)&b, w2, mts);
            butterfly_x2<false, false>((u32*)&c, (u32*)&d, w3, mts);
        } else {
            butterfly_x2<true, trivial>((u32*)&a, (u32*)&b, w2, mts);
            butterfly_x2<true, false>((u32*)&c, (u32*)&d, w3, mts);
            butterfly_x2<true, trivial>((u32*)&a, (u32*)&c, w1, mts);
            butterfly_x2<true, trivial>((u32*)&b, (u32*)&d, w1, mts);
        }
        store_u32x8(ptr_a, a), store_u32x8(ptr_b, b), store_u32x8(ptr_c, c), store_u32x8(ptr_d, d);
    }

    template <bool inverse, bool trivial = false>
    void transform_aux(int k, int i, u32* data, u64x4& wi, const Montgomery_simd& mts) const {
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
    void transform_forward(int lg, u32* data) const {
        auto beg = clock();

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

        std::cerr << "transform_forward: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
    }

    // input in [0, 2 * mod)
    // output in [0, mod)
    // data must be 32-byte aligned
    template <bool mul_by_sc = false>
    void transform_inverse(int lg, u32* data, /* as normal number */ u32 sc = u32()) const {
        auto beg = clock();
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
        std::cerr << "transform_inverse: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
    }

   private:
    // input in [0, 4 * mod)
    // output in [0, 2 * mod)
    // multiplies mod (x^2^L - w)
    template <int L, int K, bool remove_montgomery_reduction_factor = true>
    /* !!! O3 is crucial here !!! */
    __attribute__((optimize("O3"))) static void aux_mul_mod_x2L(const u32* a, const u32* b, u32* c, const std::array<u32x8, K>& ar_w, const Montgomery_simd& mts) {
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

                store_u32x8((u32*)(aux_b[k] + i + 0), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_w, _mm256_setr_epi64x(0, 1, 2, 3)));
                store_u32x8((u32*)(aux_b[k] + i + 4), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_w, _mm256_setr_epi64x(4, 5, 6, 7)));
                store_u32x8((u32*)(aux_b[k] + n + i + 0), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_0, _mm256_setr_epi64x(0, 1, 2, 3)));
                store_u32x8((u32*)(aux_b[k] + n + i + 4), (u32x8)_mm256_permutevar8x32_epi32((i256)bi_0, _mm256_setr_epi64x(4, 5, 6, 7)));
            }
        }

        u64x4 aux_ans[K][n / 4];
        memset(aux_ans, 0, sizeof(aux_ans));

        for (int i = 0; i < n; i++) {
            for (int k = 0; k < K; k++) {
                u64x4 ai = (u64x4)_mm256_set1_epi32(aux_a[k][i]);
                for (int j = 0; j < n; j += 4) {
                    u64x4 bi = (u64x4)_mm256_loadu_si256((i256*)(aux_b[k] + n - i + j));
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
    void aux_mul_mod_full(int lg, const u32* a, const u32* b, u32* c) const {
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
    // input in [0, 4 * mod)
    // output in [0, 2 * mod)
    template <bool remove_montgomery_reduction_factor = true>
    void aux_dot_mod(int lg, const u32* a, const u32* b, u32* c) const {
        auto beg = clock();

        int L = get_low_lg(lg);
        if (L == L0) {
            aux_mul_mod_full<L0, remove_montgomery_reduction_factor>(lg, a, b, c);
        } else {
            aux_mul_mod_full<L0 + 1, remove_montgomery_reduction_factor>(lg, a, b, c);
        }

        std::cerr << "aux_dot_mod:       " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
    }

    // lg must be greater than or equal to 3
    // a, b must be 32-byte aligned
    void convolve_cyclic(int lg, u32* a, u32* b) const {
        auto beg = clock();

        transform_forward(lg, a);
        transform_forward(lg, b);
        aux_dot_mod<false>(lg, a, b, a);
        transform_inverse<true>(lg, a, mt.r);

        std::cerr << "convolve_cyclic: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
    }
};

#include <sys/mman.h>

#include <random>
#include <string>

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#define CP_ALGO_USE_MMAP 1
#include <sys/mman.h>
#else
#define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T>
    T* new_big(std::size_t len) {
#if CP_ALGO_USE_MMAP
        void* raw = mmap(nullptr, len * sizeof(T),
                         PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS,
                         -1, 0);
        // Advise the kernel for huge pages and pre-populate writes
        madvise(raw, len * sizeof(T), MADV_HUGEPAGE);
        madvise(raw, len * sizeof(T), MADV_POPULATE_WRITE);
        return static_cast<T*>(raw);
#else
        // Fallback allocation for non-POSIX platforms
        return new (std::align_val_t(64)) T[len];
#endif
    }
    template <typename T>
    void delete_big(T* ptr, [[maybe_unused]] std::size_t len) {
#if CP_ALGO_USE_MMAP
        munmap(ptr, len * sizeof(T));
#else
        // Match allocation with delete[] on non-POSIX platforms
        delete[] ptr;
#endif
    }
}  // namespace cp_algo

std::string meow(const std::string& p, const std::string& s) {
    clock_t beg = clock();

    std::mt19937 rnd(12345);

    const int mod = 998'244'353;
    Montgomery mt(mod);
    Montgomery_simd mts(mod);
    NTT ntt(mod);

    std::vector<u32> meow(256), meow_inv(256);
    for (int i = 0; i < meow.size(); i++) {
        meow[i] = rnd() % mod;
        meow_inv[i] = mt.power<false, false>(meow[i], mod - 2);
    }
    meow['*'] = 0, meow_inv['*'] = 0;

    std::cerr << "init: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl, beg = clock();

    int lg = std::max(3, std::bit_width<size_t>(s.size() - 1));
    assert(s.size() <= (1 << lg));

    // u32* ptr1 = (u32*)std::aligned_alloc(32, 4 << lg);
    // u32* ptr2 = (u32*)std::aligned_alloc(32, 4 << lg);
    // u32* ptr3 = (u32*)std::aligned_alloc(32, 4 << lg);

    u32* ptr1 = cp_algo::new_big<u32>(1 << lg);
    u32* ptr2 = cp_algo::new_big<u32>(1 << lg);
    u32* ptr3 = cp_algo::new_big<u32>(1 << lg);

    // memset(ptr1, 0, 4 << lg);
    // memset(ptr2, 0, 4 << lg);
    // memset(ptr3, 0, 4 << lg);

    std::cerr << "allocate: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl, beg = clock();
    for (int i = 0; i < s.size(); i++) {
        ptr1[i] = meow[s[i]];
    }
    std::fill(ptr1 + s.size(), ptr1 + (1 << lg), 0);
    for (int i = 0; i < p.size(); i++) {
        ptr2[i] = meow_inv[p.rbegin()[i]];
    }
    std::fill(ptr2 + p.size(), ptr2 + (1 << lg), 0);

    std::cerr << "fill_1: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl, beg = clock();

    ntt.transform_forward(lg, ptr1);
    ntt.transform_forward(lg, ptr2);

    ntt.aux_dot_mod<false>(lg, ptr1, ptr2, ptr1);

    beg = clock();

    for (int i = 0; i < s.size(); i++) {
        ptr2[i] = (s[i] != '*');
    }
    std::fill(ptr2 + s.size(), ptr2 + (1 << lg), 0);
    for (int i = 0; i < p.size(); i++) {
        ptr3[i] = (p.rbegin()[i] != '*');
    }
    std::fill(ptr3 + p.size(), ptr3 + (1 << lg), 0);

    std::cerr << "fill_2: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl, beg = clock();

    ntt.transform_forward(lg, ptr2);
    ntt.transform_forward(lg, ptr3);

    ntt.aux_dot_mod<false>(lg, ptr2, ptr3, ptr2);

    beg = clock();
    for (int i = 0; i < (1 << lg); i += 8) {
        store_u32x8(ptr1 + i, mts.shrink2_n(load_u32x8(ptr1 + i) - load_u32x8(ptr2 + i)));
    }
    std::cerr << "add: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl, beg = clock();

    ntt.transform_inverse(lg, ptr1);

    beg = clock();
    std::string res(s.size() - p.size() + 1, '-');
    for (int i = 0; i < res.size(); i++) {
        res[i] = "01"[ptr1[i + (p.size() - 1)] == 0];
    }
    std::cerr << "fill_ans: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl, beg = clock();

    // _mm_free(ptr1), _mm_free(ptr2), _mm_free(ptr3);

    beg = clock();
    cp_algo::delete_big(ptr1, 1 << lg);
    cp_algo::delete_big(ptr2, 1 << lg);
    cp_algo::delete_big(ptr3, 1 << lg);
    std::cerr << "deallocate: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl, beg = clock();

    return res;
}

// NOLINTBEGIN
// clang-format off
// DO NOT REMOVE THIS MESSAGE. The mess that follows is a minified build of
// https://github.com/purplesyringa/blazingio. Refer to the repository for
// a human-readable version and documentation.
// Options: cbfoiedrhWLMXaIaAn
#define M$(x,...)_mm256_##x##_epi8(__VA_ARGS__)
#define $u(...)__VA_ARGS__
#if __APPLE__
#define $m(A,B)A
#else
#define $m(A,B)B
#endif
#if _WIN32
#define $w(A,B)A
#else
#define $w(A,B)B
#endif
#if __i386__|_M_IX86
#define $H(A,B)A
#else
#define $H(A,B)B
#endif
#if __aarch64__
#define $a(A,B)A
#else
#define $a(A,B)B
#endif
#define $P(x)void F(x K){
#define $T template<$c T
#define $c class
#define $C constexpr
#define $R return
#define $O operator
#define u$ uint64_t
#define $r $R*this;
#include<array>
#include<bitset>
#include<complex>
#include<cstring>
#include $a(<arm_neon.h>,<immintrin.h>)
#include<stdint.h>
#include $w(<windows.h>,<sys/mman.h>)
#include<sys/stat.h>
#include $w(<io.h>,<unistd.h>)
#include $w(<ios>,<sys/resource.h>)
#if _MSC_VER
#define __builtin_add_overflow(a,b,c)_addcarry_u64(0,a,b,c)
#define $s
#else
$H(,u$ _umul128(u$ a,u$ b,u$*D){auto x=(__uint128_t)a*b;*D=u$(x>>64);$R(u$)x;})
#define $s $a(,__attribute__((target("avx2"))))
#endif
#define $z $a(16,32)
#define $t $a(uint8x16_t,__m256i)
#define $I $w(__forceinline,__attribute__((always_inline)))
#define $F M(),
#define E$(x)if(!(x))abort();
$w(LONG WINAPI $x(_EXCEPTION_POINTERS*);,)namespace $f{using namespace std;struct B{enum $c A:char{}c;B&$O=(char x){c=A{x};$r}$O char(){$R(char)c;}};$C u$ C=~0ULL/255;struct D{string&K;};static B E[65568];template<int F>struct G{B*H,*S;void K(off_t C){$w(char*D=(char*)VirtualAlloc(0,(C+8191)&-4096,8192,1);E$(D)E$(VirtualFree(D,0,32768))DWORD A=C&-65536;E$(!A||MapViewOfFileEx(CreateFileMapping(GetStdHandle(-10),0,2,0,A,0),4,0,0,0,D)==D)E$(VirtualAlloc(D+A,65536,12288,4)==D+A)E$(~_lseek(0,A,0))DWORD E=0;ReadFile(GetStdHandle(-10),D+A,65536,&E,0);,int A=getpagesize();char*D=(char*)mmap(0,C+A,3,2,0,0);E$(D!=(void*)-1)E$(mmap(D+((C+A-1)&-A),A,3,$m(4114,50),-1,0)!=(void*)-1))H=(B*)D+C;*H=10;H[1]=48;H[2]=0;S=(B*)D;}void L(){H=S=E;}$I void M(){if(F&&S==H){$w(DWORD A=0;ReadFile(GetStdHandle(-10),S=E,65536,&A,0);,$a($u(register long A asm("x0")=0,D asm("x1")=(long)E,G asm("x2")=65536,C asm($m("x16","x8"))=$m(3,63);asm volatile("svc 0" $m("x80",):"+r"(A),"+r"(D):"r"(C),"r"(G));S=launder(E);),off_t A=$H(3,$m(33554435,0));B*D=E;asm volatile($H("int $128","syscall"):"+a"(A),$H("+c"(D):"b","+S"(D):"D")(0),"d"(65536)$H(,$u(:"rcx","r11")));S=D;))H=S+A;*H=10;if(!A)E[1]=48,E[2]=0;}}$T>$I void N(T&x){while($F(*S&240)==48)x=T(x*10+(*S++-48));}$T>$I decltype((void)~T{1})O(T&x){M();int A=is_signed_v<T>&&*S==45;S+=A;N(x=0);x=A?1+~x:x;}$T>$I decltype((void)T{1.})O(T&x){M();int A=*S==45;S+=A;$F S+=*S==43;u$ n=0;int i=0;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48;int B=20;int C=*S==46;S+=C;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48,B-=C;x=(T)n;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;if(*S==46)S++,C=1;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;int D;if((*S|32)==101)S++,$F S+=*S==43,O(D),B+=D;static $C auto E=[](){array<T,41>E{};T x=1;for(int i=21;i--;)E[40-i]=x,E[i]=1/x,x*=10;$R E;}();while(B>40)x*=(T)1e10,B-=10;while(B<0)x*=(T)1e-10,B+=10;x*=E[B];x=A?-x:x;}$I void O(bool&x){$F x=*S++==49;}$I void O(char&x){$F x=*S++;}$I void O(uint8_t&x){$F x=*S++;}$I void O(int8_t&x){$F x=*S++;}$T>$s void P(string&K,T C){M();B*G=S;C();K.assign((char*)G,S-G);while(F&&S==H&&($F H!=E)){C();K.append(E,S);}}$s void O(string&K){P(K,[&]()$s{B*p=S;$w(ULONG R;,)$t x;$a(uint64x2_t A;while(memcpy(&x,p,16),A=uint64x2_t(x<33),!(A[0]|A[1]))p+=16;S=p+(A[0]?0:8)+$w((_BitScanForward64(&R,A[0]?A[0]:A[1]),R),__builtin_ctzll(A[0]?A[0]:A[1]))/8;,int J;$t C=M$(set1,32);while(memcpy(&x,p,32),!(J=M$(movemask,M$(cmpeq,C,_mm256_max_epu8(C,x)))))p+=32;S=p+$w((_BitScanForward(&R,J),R),__builtin_ctz(J));)});}$s void O(D&A){P(A.K,[&](){S=(B*)memchr(S,10,H-S+1);});if(A.K.size()&&A.K.back()==13)A.K.pop_back();if(A.K.empty()||S<H)S+=*S==10;}$T>$I void O(complex<T>&K){T A,B{};if($F*S==40){S++;O(A);if($F*S++==44)Q(B),S++;}else O(A);K={A,B};}template<size_t N>$s void O(bitset<N>&K){if(N>4095&&!*this)$R;ptrdiff_t i=N;while(i)if($F i%$z||H-S<$z)K[--i]=*S++==49;else{B*p=S;for(int64_t j=0;j<min(i,H-S)/$z;j++){i-=$z;$t x;memcpy(&x,p,$z);$a(auto B=(uint8x16_t)vdupq_n_u64(~2ULL/254)&(48-x);auto C=vzip_u8(vget_high_u8(B),vget_low_u8(B));auto y=vaddvq_u16((uint16x8_t)vcombine_u8(C.val[0],C.val[1]));,u$ a=~0ULL/65025;auto y=$w(_byteswap_ulong,__builtin_bswap32)(M$(movemask,M$(shuffle,_mm256_slli_epi32(x,7),_mm256_set_epi64x(a+C*24,a+C*16,a+C*8,a))));)p+=$z;memcpy((char*)&K+i/8,&y,$z/8);}S=p;}}$T>$I void Q(T&K){if(!is_same_v<T,D>)while($F(uint8_t)*S<33)S++;O(K);}$O bool(){$R!!*this;}bool $O!(){$R S>H;}};struct U{G<0>A;G<1>B;U(){struct stat D;E$(~fstat(0,&D))(D.st_mode>>12)==8?A.K(D.st_size):B.L();}U*tie(nullptr_t){$R this;}void sync_with_stdio(bool){}$T>$I U&$O>>(T&K){A.S?A.Q(K):B.Q(K);$r}$O bool(){$R!!*this;}bool $O!(){$R A.S?!A:!B;}};short A[100];char L[64]{1};struct
V{char*D;B*S;int J;V(){$w(E$(D=(char*)VirtualAlloc(0,536870912,8192,4))E$(VirtualAlloc(D,4096,4096,260))AddVectoredExceptionHandler(1,$x);,size_t C=536870912;$m(,rlimit E;getrlimit(RLIMIT_AS,&E);if(~E.rlim_cur)C=25165824;)D=(char*)mmap(0,C,3,$m(4162,16418),-1,0);E$(D!=(void*)-1))S=(B*)D;for(int i=0;i<100;i++)A[i]=short((48+i/10)|((48+i%10)<<8));for(int i=1;i<64;i++)L[i]=L[i-1]+(0x8922489224892249>>i&1);}~V(){flush($w(!J,));}void flush($w(int F=0,)){$w(J=1;auto E=GetStdHandle(-11);auto C=F?ReOpenFile(E,1073741824,7,2684354560):(void*)-1;DWORD A;E$(C==(void*)-1?WriteFile(E,D,DWORD((char*)S-D),&A,0):(WriteFile(C,D,DWORD(((char*)S-D+4095)&-4096),&A,0)&&~_chsize(1,int((char*)S-D)))),auto G=D;ssize_t A;while((A=write(1,G,(char*)S-G))>0)G+=A;E$(~A))S=(B*)D;}$P(char)*S++=K;}$P(uint8_t)*S++=K;}$P(int8_t)*S++=K;}$P(bool)*S++=48+K;}$T>decltype((void)~T{1})F(T K){using D=make_unsigned_t<T>;D C=K;if(K<0)F('-'),C=1+~C;static $C auto N=[](){array<D,5*sizeof(T)/2>N{};D n=1;for(size_t i=1;i<N.size();i++)n*=10,N[i]=n;$R N;}();$w(ULONG M;,)int G=L[$w(($H(_BitScanReverse(&M,ULONG((int64_t)C>>32))?M+=32:_BitScanReverse(&M,(ULONG)C|1),_BitScanReverse64(&M,C|1)),M),63^__builtin_clzll(C|1))];G-=C<N[G-1];short H[20];if $C(sizeof(T)==2){auto n=33555U*C-C/2;u$ H=A[n>>25];n=(n&33554431)*25;H|=A[n>>23]<<16;H|=u$(48+((n&8388607)*5>>22))<<32;H>>=40-G*8;memcpy(S,&H,8);}else if $C(sizeof(T)==4){auto n=1441151881ULL*C;$H(n>>=25;n++;for(int i=0;i<5;i++){H[i]=A[n>>32];n=(n&~0U)*100;},int K=57;auto J=~0ULL>>7;for(int i=0;i<5;i++){H[i]=A[n>>K];n=(n&J)*25;K-=2;J/=4;})memcpy(S,(B*)H+10-G,16);}else{$H($u(if(C<(1ULL<<32)){$R F((uint32_t)C);}auto J=(u$)1e10;auto x=C/J,y=C%J;int K=100000,b[]{int(x/K),int(x%K),int(y/K),int(y%K)};B H[40];for(int i=0;i<4;i++){int n=int((429497ULL*b[i]>>7)+1);B*p=H+i*5;*p=48+char(n>>25);n=(n&~0U>>7)*25;memcpy(p+1,A+(n>>23),2);memcpy(p+3,A+((n&~0U>>9)*25>>21),2);}),$u(u$ D,E=_umul128(18,C,&D),F;_umul128(0x725dd1d243aba0e8,C,&F);D+=__builtin_add_overflow(E,F+1,&E);for(int i=0;i<10;i++)H[i]=A[D],E=_umul128(100,E,&D);))memcpy(S,(B*)H+20-G,20);}S+=G;}$T>decltype((void)T{1.})F(T K){if(K<0)F('-'),K=-K;auto G=[&](){auto x=u$(K*1e12);$H($u(x-=x>999999999999;uint32_t n[]{uint32_t(x/1000000*429497>>7)+1,uint32_t(x%1000000*429497>>7)+1};int K=25,J=~0U>>7;for(int i=0;i<3;i++){for(int j=0;j<2;j++)memcpy(S+i*2+j*6,A+(n[j]>>K),2),n[j]=(n[j]&J)*25;K-=2;J/=4;}S+=12;),$u(u$ D,E=_umul128(472236648287,x,&D)>>8;E|=D<<56;D>>=8;E++;for(int i=0;i<6;i++)memcpy(S,A+D,2),S+=2,E=_umul128(100,E,&D);))};if(K==0)$R F('0');if(K>=1e16){K*=(T)1e-16;int B=16;while(K>=1)K*=(T).1,B++;F("0.");G();F('e');F(B);}else if(K>=1){auto B=(u$)K;F(B);if((K-=(T)B)>0)F('.'),G();}else F("0."),G();}$P(const char*)$w(size_t A=strlen(K);memcpy((char*)S,K,A);S+=A;,S=(B*)stpcpy((char*)S,K);)}$P(const uint8_t*)F((char*)K);}$P(const int8_t*)F((char*)K);}$P(string_view)memcpy(S,K.data(),K.size());S+=K.size();}$T>$P(complex<T>)*this<<'('<<K.real()<<','<<K.imag()<<')';}template<size_t N>$s $P(const bitset<N>&)auto i=N;while(i%$z)*S++=48+K[--i];B*p=S;while(i){i-=$z;$a(short,int)x;memcpy(&x,(char*)&K+i/8,$z/8);$a(auto A=(uint8x8_t)vdup_n_u16(x);vst1q_u8((uint8_t*)p,48-vtstq_u8(vcombine_u8(vuzp2_u8(A,A),vuzp1_u8(A,A)),(uint8x16_t)vdupq_n_u64(~2ULL/254)));,auto b=_mm256_set1_epi64x(~2ULL/254);_mm256_storeu_si256(($t*)p,M$(sub,M$(set1,48),M$(cmpeq,_mm256_and_si256(M$(shuffle,_mm256_set1_epi32(x),_mm256_set_epi64x(0,C,C*2,C*3)),b),b)));)p+=$z;}S=p;}$T>V&$O<<(const T&K){F(K);$r}V&$O<<(V&(*A)(V&)){$R A(*this);}};struct W{$T>W&$O<<(const T&K){$r}W&$O<<(W&(*A)(W&)){$R A(*this);}};}namespace std{$f::U i$;$f::V o$;$f::W e$;$f::U&getline($f::U&B,string&K){$f::D A{K};$R B>>A;}$f::V&flush($f::V&B){if(!i$.A.S)B.flush();$R B;}$f::V&endl($f::V&B){$R B<<'\n'<<flush;}$f::W&endl($f::W&B){$R B;}$f::W&flush($f::W&B){$R B;}}$w(LONG WINAPI $x(_EXCEPTION_POINTERS*A){auto C=A->ExceptionRecord;auto B=C->ExceptionInformation[1];if(C->ExceptionCode==2147483649&&B-(ULONG_PTR)std::o$.D<0x40000000){E$(VirtualAlloc((char*)B,16777216,4096,4)&&VirtualAlloc((char*)(B+16777216),4096,4096,260))$R-1;}$R 0;},)
#define freopen(...)if(freopen(__VA_ARGS__)==stdin)std::i$=$f::U{}
#define cin i$
#define cout o$
#ifdef ONLINE_JUDGE
#define cerr e$
#define clog e$
#endif
// End of blazingio
// NOLINTEND
// clang-format once __io;

int32_t main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string a, b;
    std::cin >> a >> b;

    std::string c;
    {
        auto beg = clock();
        c = meow(b, a);
        std::cerr << "\ntotal: " << (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
    }

    std::cout << c << "\n";

    // while (clock() * 1.0 / CLOCKS_PER_SEC < 0.1) {
    //     ;
    // }

    return 0;
}

/*

abacabadabacabaeabacabadabacabafabacabadabacabadddddddddddddddd
abacaba

*/
