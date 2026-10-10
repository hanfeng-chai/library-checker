#define FPS_NATIVE_MINT
#ifndef FPS_998244353_HPP
#define FPS_998244353_HPP
#include <bits/stdc++.h>
#if defined(__ARM_NEON)
#include <arm_neon.h>
#elif defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#endif

#if __has_include(<atcoder/modint>) && !defined(FPS_NATIVE_MINT)
#include <atcoder/modint>
#endif

#if defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma GCC push_options
#pragma GCC optimize("O3")
#endif

namespace fps_detail {
struct Mint {
    unsigned v = 0;
    static constexpr unsigned mod() { return 998244353; }
    static unsigned reduce(unsigned long long x) {
        unsigned r = (x + (unsigned long long)(unsigned(x) * 998244351u) * mod()) >> 32;
        return r;
    }
    Mint() = default;
    template <class I, std::enable_if_t<std::is_integral_v<I>, int> = 0>
    Mint(I x) {
        long long r = x % (long long)mod();
        if (r < 0) r += mod();
        v = reduce((unsigned long long)r * 932051910);
    }
    static Mint raw(unsigned x) { Mint a; a.v = reduce((unsigned long long)x * 932051910); return a; }
    static Mint word(unsigned x) { Mint a; a.v = x; return a; }
    unsigned val() const { unsigned r = reduce(v); return r < mod() ? r : r - mod(); }
    Mint& operator+=(Mint b) { v += b.v; if (v >= 2 * mod()) v -= 2 * mod(); return *this; }
    Mint& operator-=(Mint b) { v += 2 * mod() - b.v; if (v >= 2 * mod()) v -= 2 * mod(); return *this; }
    Mint& operator*=(Mint b) { v = reduce((unsigned long long)v * b.v); return *this; }
    Mint& operator/=(Mint b) { return *this *= b.inv(); }
    Mint operator+() const { return *this; }
    Mint operator-() const { return word(v ? 2 * mod() - v : 0); }
    Mint operator+(Mint b) const { return Mint(*this) += b; }
    Mint operator-(Mint b) const { return Mint(*this) -= b; }
    Mint operator*(Mint b) const { return Mint(*this) *= b; }
    Mint operator/(Mint b) const { return Mint(*this) /= b; }
    bool operator==(Mint b) const { return (v < mod() ? v : v - mod()) == (b.v < mod() ? b.v : b.v - mod()); }
    bool operator!=(Mint b) const { return !(*this == b); }
    Mint pow(long long n) const { assert(n >= 0); Mint r(1), a=*this; for (; n; n >>= 1, a *= a) if (n & 1) r *= a; return r; }
    Mint inv() const { assert(*this != Mint(0)); return pow(mod() - 2); }
};
struct NTT {
    using T = Mint;
    typedef unsigned U __attribute__((may_alias));
    using V = std::vector<unsigned>;
    static constexpr U mod = 998244353, twice = 2 * mod, one = 301989884;
    V roots, inverse_roots, twists, inverse_twists;
    V root_quotients, inverse_root_quotients;
    static U reduce(unsigned long long x) {
        return (x + (unsigned long long)(U(x) * 998244351u) * mod) >> 32;
    }
    static U mul(U a, U b) { return reduce((unsigned long long)a * b); }
    static U add(U a, U b) {
        U c = a + b;
        return c < twice ? c : c - twice;
    }
    static U sub(U a, U b) {
        U c = a - b;
        return c < twice ? c : c + twice;
    }
    static U in(U a) { return mul(a, 932051910); }
    static U out(U a) {
        a = reduce(a);
        return a < mod ? a : a - mod;
    }
    static U quotient(U r) { return ((unsigned long long)r << 32) / mod; }
    static U fixed_mul(U a, U r, U q) {
        return a * r - U(((unsigned long long)a * q) >> 32) * mod;
    }
    NTT(int size, bool composition = true)
        : roots(size, one),
          inverse_roots(size, one),
          twists(composition ? size : 0, one),
          inverse_twists(composition ? size : 0, one),
          root_quotients(size),
          inverse_root_quotients(size) {
        for (int h = 1; h < size; h <<= 1) {
            U r = in(T(3).pow((mod - 1) / (4 * h)).val());
            U ir = in(T(3).pow(mod - 1 - (mod - 1) / (4 * h)).val());
            U t = in(T(3).pow((mod - 1) / (2 * h)).val());
            U it = in(T(3).pow(mod - 1 - (mod - 1) / (2 * h)).val());
            U a = in(mod - (mod - 1) / h), b = a;
            for (int i = 0; i < h; ++i) {
                roots[h + i] = mul(roots[i], r);
                inverse_roots[h + i] = mul(inverse_roots[i], ir);
                if (composition) twists[h + i] = a, inverse_twists[h + i] = b;
                a = mul(a, t);
                b = mul(b, it);
            }
        }
        for (int i = 0; i < size; ++i) {
            root_quotients[i] = (roots[i] < mod ? roots[i] : roots[i] - mod) * 998244351u;
            inverse_root_quotients[i] = (inverse_roots[i] < mod ? inverse_roots[i] : inverse_roots[i] - mod) * 998244351u;
            roots[i] = out(roots[i]);
            inverse_roots[i] = out(inverse_roots[i]);
            if (composition) twists[i] = out(twists[i]), inverse_twists[i] = out(inverse_twists[i]);
        }
    }

#if defined(__ARM_NEON)
    using I = uint32x4_t;
    static I mul4(I a, I b) {
        uint64x2_t x = vmull_u32(vget_low_u32(a), vget_low_u32(b));
        uint64x2_t y = vmull_u32(vget_high_u32(a), vget_high_u32(b));
        auto reduce2 = [](uint64x2_t z) {
            uint32x2_t q = vmul_u32(vmovn_u64(z), vdup_n_u32(998244351));
            return vshrn_n_u64(vaddq_u64(z, vmull_u32(q, vdup_n_u32(mod))), 32);
        };
        return vcombine_u32(reduce2(x), reduce2(y));
    }
    static I add4(I a, I b) {
        I c = vaddq_u32(a, b);
        return vminq_u32(c, vsubq_u32(c, vdupq_n_u32(twice)));
    }
    static I sub4(I a, I b) {
        I c = vsubq_u32(a, b);
        return vminq_u32(c, vaddq_u32(c, vdupq_n_u32(twice)));
    }
    static I fixed_mul4(I a, I r, I q) {
        uint64x2_t x = vmull_u32(vget_low_u32(a), vget_low_u32(q));
        uint64x2_t y = vmull_u32(vget_high_u32(a), vget_high_u32(q));
        I t = vcombine_u32(vshrn_n_u64(x, 32), vshrn_n_u64(y, 32));
        return vmlsq_u32(vmulq_u32(a, r), t, vdupq_n_u32(mod));
    }
    template <bool inverse>
    static void fixed_butterfly4(I& x, I& y, I r, I q, bool unit = false) {
        if constexpr (!inverse)
            if (!unit) y = fixed_mul4(y, r, q);
        I z = sub4(x, y);
        x = add4(x, y);
        y = inverse && !unit ? fixed_mul4(z, r, q) : z;
    }
#endif
#if defined(__x86_64__) || defined(__i386__)
    static bool vectorized() {
#if defined(__AVX2__)
        return true;
#else
        static const bool available = __builtin_cpu_supports("avx2");
        return available;
#endif
    }
    using I8 = __m256i;
    __attribute__((target("avx2"))) static I8 load8(const U* a) {
        return _mm256_loadu_si256(reinterpret_cast<const I8*>(a));
    }
    __attribute__((target("avx2"))) static void store8(U* a, I8 x) {
        _mm256_storeu_si256(reinterpret_cast<I8*>(a), x);
    }
    __attribute__((target("avx2"))) static I8 mul8(I8 a, I8 b) {
        I8 x = _mm256_mul_epu32(a, b);
        I8 y = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
        I8 r = _mm256_set1_epi32(998244351), p = _mm256_set1_epi32(mod);
        x = _mm256_add_epi64(x, _mm256_mul_epu32(_mm256_mul_epu32(x, r), p));
        y = _mm256_add_epi64(y, _mm256_mul_epu32(_mm256_mul_epu32(y, r), p));
        return _mm256_blend_epi32(_mm256_srli_epi64(x, 32), y, 0xaa);
    }
    __attribute__((target("avx2"))) static I8 fixed_mul8(I8 a, I8 r, I8 q) {
        I8 x = _mm256_srli_epi64(_mm256_mul_epu32(a, q), 32);
        I8 y = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), q);
        I8 t = _mm256_blend_epi32(x, y, 0xaa);
        return _mm256_sub_epi32(_mm256_mullo_epi32(a, r),
                                _mm256_mullo_epi32(t, _mm256_set1_epi32(mod)));
    }
    __attribute__((target("avx2"))) static I8 fixed_even8(I8 a, I8 r, I8 q) {
        I8 t = _mm256_srli_epi64(_mm256_mul_epu32(a, q), 32);
        return _mm256_sub_epi64(_mm256_mul_epu32(a, r),
                                _mm256_mul_epu32(t, _mm256_set1_epi32(mod)));
    }
    __attribute__((target("avx2"))) static I8 add8(I8 a, I8 b) {
        I8 x = _mm256_add_epi32(a, b);
        return _mm256_min_epu32(x, _mm256_sub_epi32(x, _mm256_set1_epi32(twice)));
    }
    __attribute__((target("avx2"))) static I8 sub8(I8 a, I8 b) {
        I8 x = _mm256_sub_epi32(a, b);
        return _mm256_min_epu32(x, _mm256_add_epi32(x, _mm256_set1_epi32(twice)));
    }

    template <int stride>
    __attribute__((target("avx2"))) static I8 rates8(const V& values, int k) {
        if constexpr (stride == 1)
            return _mm256_cvtepu32_epi64(
                _mm_loadu_si128(reinterpret_cast<const __m128i*>(values.data() + 4 * k)));
        if constexpr (stride == 2)
            return _mm256_permute4x64_epi64(
                _mm256_cvtepu32_epi64(
                    _mm_loadl_epi64(reinterpret_cast<const __m128i*>(values.data() + 2 * k))),
                0x50);
        if constexpr (stride == 4) return _mm256_set1_epi32(values[k]);
    }
    template <int stride, bool inverse>
    __attribute__((target("avx2"))) I8 small8(I8 z, int k) const {
        if constexpr (stride == 4) {
            if (k == 0) {
                I8 x = _mm256_permute2x128_si256(z, z, 0x00),
                   y = _mm256_permute2x128_si256(z, z, 0x11);
                return _mm256_blend_epi32(add8(x, y), sub8(x, y), 0xf0);
            }
        }
        const V& w = inverse ? inverse_roots : roots;
        const V& quot = inverse ? inverse_root_quotients : root_quotients;
        I8 r = rates8<stride>(w, k), q = rates8<stride>(quot, k);
        if constexpr (inverse) {
            if constexpr (stride == 2) z = _mm256_shuffle_epi32(z, 0xd8);
            if constexpr (stride == 4)
                z = _mm256_permutevar8x32_epi32(z, _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7));
            I8 y = _mm256_srli_epi64(z, 32), x = add8(z, y);
            I8 d = fixed_even8(sub8(z, y), r, q);
            z = _mm256_blend_epi32(x, _mm256_slli_epi64(d, 32), 0xaa);
        } else {
            if constexpr (stride == 1) z = _mm256_shuffle_epi32(z, 0xb1);
            if constexpr (stride == 2) z = _mm256_shuffle_epi32(z, 0x72);
            if constexpr (stride == 4)
                z = _mm256_permutevar8x32_epi32(z, _mm256_setr_epi32(4, 0, 5, 1, 6, 2, 7, 3));
            I8 y = fixed_even8(z, r, q);
            I8 x = _mm256_blend_epi32(_mm256_srli_epi64(z, 32), z, 0xaa);
            y = _mm256_blend_epi32(
                y, _mm256_sub_epi32(_mm256_set1_epi32(twice), _mm256_slli_epi64(y, 32)), 0xaa);
            z = add8(x, y);
        }
        if constexpr (stride == 2) z = _mm256_shuffle_epi32(z, 0xd8);
        if constexpr (stride == 4)
            z = _mm256_permutevar8x32_epi32(z, _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7));
        return z;
    }
    template <bool inverse>
    __attribute__((target("avx2"))) void fft8(U* a, int length, int limit = 0) const {
        if (!limit) limit = length;
        if constexpr (!inverse) {
            int h = limit / 2;
            for (; h >= 16; h >>= 2) radix4_8<false>(a, h / 2, length);
            if (h == 8) {
                if (limit >= 32) for (int i = 0; i < length; i += 8) {
                    I8 x = load8(a+i), p = _mm256_set1_epi32(twice);
                    store8(a+i,_mm256_min_epu32(x,_mm256_sub_epi32(x,p)));
                }
                stage8<false>(a, h, length);
            }
        }
        for (int j = 0; j < length; j += 8) {
            I8 z = load8(a + j);
            if constexpr (!inverse) if ((limit >= 32) && (__builtin_ctz((unsigned)limit) & 1)) {
                I8 p = _mm256_set1_epi32(twice);
                z = _mm256_min_epu32(z,_mm256_sub_epi32(z,p));
            }
            if constexpr (inverse)
                z = small8<4, true>(small8<2, true>(small8<1, true>(z, j / 8), j / 8), j / 8);
            else
                z = small8<1, false>(small8<2, false>(small8<4, false>(z, j / 8), j / 8),
                                     j / 8);
            store8(a + j, z);
        }
        if constexpr (inverse) {
            int h = 8;
            for (; 4 * h <= limit; h <<= 2) radix4_8<true>(a, h, length);
            if (h < limit) stage8<true>(a, h, length);
        }
    }
    template <bool inverse>
    __attribute__((target("avx2"))) void radix4_8(U* a, int stride, int length) const {
        const V& w = inverse ? inverse_roots : roots;
        const V& q = inverse ? inverse_root_quotients : root_quotients;
        I8 p = _mm256_set1_epi32(twice);
        I8 im = _mm256_set1_epi32(w[1]), iq = _mm256_set1_epi32(q[1]);
        for (int j = 0, k = 0; j < length; j += 4 * stride, ++k) {
            U cube = (unsigned long long)w[k] * w[2 * k] % mod;
            I8 r = _mm256_set1_epi32(w[k]), rq = _mm256_set1_epi32(q[k]);
            I8 r0 = _mm256_set1_epi32(w[2*k]), q0 = _mm256_set1_epi32(q[2*k]);
            I8 r3 = _mm256_set1_epi32(cube), q3 = _mm256_set1_epi32(quotient(cube));
            for (int i = 0; i < stride; i += 8) {
                I8 x0 = load8(a + j + i), x1 = load8(a + j + stride + i);
                I8 x2 = load8(a + j + 2*stride + i), x3 = load8(a + j + 3*stride + i);
                if constexpr (inverse) {
                    I8 u = add8(x0,x1), v = sub8(x0,x1);
                    I8 s = add8(x2,x3);
                    I8 t = fixed_mul8(_mm256_sub_epi32(_mm256_add_epi32(x2,p),x3),im,iq);
                    x0 = add8(u,s);
                    x1 = _mm256_add_epi32(v,t);
                    x2 = _mm256_sub_epi32(_mm256_add_epi32(u,p),s);
                    x3 = _mm256_sub_epi32(_mm256_add_epi32(v,p),t);
                    if (k) {
                        x1 = fixed_mul8(x1,r0,q0); x2 = fixed_mul8(x2,r,rq); x3 = fixed_mul8(x3,r3,q3);
                    } else {
                        x1 = _mm256_min_epu32(x1,_mm256_sub_epi32(x1,p));
                        x2 = _mm256_min_epu32(x2,_mm256_sub_epi32(x2,p));
                        x3 = _mm256_min_epu32(x3,_mm256_sub_epi32(x3,p));
                    }
                } else {
                    // Shoup multiplication accepts every 32-bit input, including <4p.
                    x0 = _mm256_min_epu32(x0,_mm256_sub_epi32(x0,p));
                    if (!k) {
                        x1 = _mm256_min_epu32(x1,_mm256_sub_epi32(x1,p));
                        x2 = _mm256_min_epu32(x2,_mm256_sub_epi32(x2,p));
                        x3 = _mm256_min_epu32(x3,_mm256_sub_epi32(x3,p));
                    }
                    if (k) {
                        x1 = fixed_mul8(x1,r0,q0); x2 = fixed_mul8(x2,r,rq); x3 = fixed_mul8(x3,r3,q3);
                    }
                    I8 u = add8(x0,x2), v = sub8(x0,x2), s = add8(x1,x3);
                    I8 t = fixed_mul8(_mm256_sub_epi32(_mm256_add_epi32(x1,p),x3),im,iq);
                    x0 = _mm256_add_epi32(u,s); x1 = _mm256_sub_epi32(_mm256_add_epi32(u,p),s);
                    x2 = _mm256_add_epi32(v,t); x3 = _mm256_sub_epi32(_mm256_add_epi32(v,p),t);
                }
                store8(a+j+i,x0);store8(a+j+stride+i,x1);
                store8(a+j+2*stride+i,x2);store8(a+j+3*stride+i,x3);
            }
        }
    }
    template <bool inverse>
    __attribute__((target("avx2"))) void stage8(U* a, int stride, int length, int first = 0) const {
        const V& w = inverse ? inverse_roots : roots;
        const V& quot = inverse ? inverse_root_quotients : root_quotients;
        if (stride < 8) {
            for (int j = 0; j < length; j += 8) {
                I8 z = load8(a + j);
                if (stride == 1)
                    z = small8<1, inverse>(z, j / 8);
                else if (stride == 2)
                    z = small8<2, inverse>(z, j / 8);
                else
                    z = small8<4, inverse>(z, j / 8);
                store8(a + j, z);
            }
        } else {
            for (int j = 0, k = first; j < length; j += 2 * stride, ++k) {
                I8 r = _mm256_set1_epi32(w[k]), q = _mm256_set1_epi32(quot[k]);
                for (int i = 0; i < stride; i += 8) {
                    I8 x = load8(a + j + i), y = load8(a + j + stride + i);
                    if constexpr (!inverse)
                        if (k) y = fixed_mul8(y, r, q);
                    I8 z = sub8(x, y);
                    x = add8(x, y);
                    if constexpr (inverse)
                        if (k) z = fixed_mul8(z, r, q);
                    store8(a + j + i, x);
                    store8(a + j + stride + i, z);
                }
            }
        }
    }

    template <bool inverse, bool lazy = false>
    __attribute__((target("avx2"), always_inline)) static void fixed_butterfly8(I8& x, I8& y, I8 r, I8 q, bool unit = false) {
        if constexpr (lazy && !inverse) {
            I8 p = _mm256_set1_epi32(twice);
            x = _mm256_min_epu32(x, _mm256_sub_epi32(x, p));
            y = unit ? _mm256_min_epu32(y, _mm256_sub_epi32(y, p)) : fixed_mul8(y, r, q);
            I8 z = _mm256_sub_epi32(_mm256_add_epi32(x, p), y);
            x = _mm256_add_epi32(x, y); y = z;
        } else {
            if constexpr (!inverse) if (!unit) y = fixed_mul8(y, r, q);
            I8 z = sub8(x, y); x = add8(x, y);
            y = inverse && !unit ? fixed_mul8(z, r, q) : z;
        }
    }
    template <bool inverse, bool lazy = false>
    __attribute__((target("avx2"))) void stage_pair8(U* a, int stride, int length, int first = 0) const {
        const V& w = inverse ? inverse_roots : roots;
        const V& q = inverse ? inverse_root_quotients : root_quotients;
        for (int j = 0, k = first; j < length; j += 4 * stride, ++k) {
            I8 r = _mm256_set1_epi32(w[k]), rq = _mm256_set1_epi32(q[k]);
            I8 r0 = _mm256_set1_epi32(w[2 * k]), q0 = _mm256_set1_epi32(q[2 * k]);
            I8 r1 = _mm256_set1_epi32(w[2 * k + 1]), q1 = _mm256_set1_epi32(q[2 * k + 1]);
            for (int i = 0; i < stride; i += 8) {
                I8 x0 = load8(a + j + i), x1 = load8(a + j + stride + i);
                I8 x2 = load8(a + j + 2 * stride + i), x3 = load8(a + j + 3 * stride + i);
                if constexpr (inverse) {
                    fixed_butterfly8<inverse, lazy>(x0, x1, r0, q0, k == 0); fixed_butterfly8<inverse, lazy>(x2, x3, r1, q1, false);
                    fixed_butterfly8<inverse, lazy>(x0, x2, r, rq, k == 0); fixed_butterfly8<inverse, lazy>(x1, x3, r, rq, k == 0);
                } else {
                    fixed_butterfly8<inverse, lazy>(x0, x2, r, rq, k == 0); fixed_butterfly8<inverse, lazy>(x1, x3, r, rq, k == 0);
                    fixed_butterfly8<inverse, lazy>(x0, x1, r0, q0, k == 0); fixed_butterfly8<inverse, lazy>(x2, x3, r1, q1, false);
                }
                store8(a + j + i, x0); store8(a + j + stride + i, x1);
                store8(a + j + 2 * stride + i, x2); store8(a + j + 3 * stride + i, x3);
            }
        }
    }
    __attribute__((target("avx2"))) static void fold8(U* a, int columns) {
        for (int i = 0; i < columns; i += 8) {
            I8 x = add8(load8(a + i), load8(a + columns + i));
            store8(a + i, x);
            store8(a + columns + i, x);
        }
    }
    __attribute__((target("avx2"))) void project8_8(U* a,int rows) const {
        for(int j=0;j<rows;++j) {
            I8 x0=load8(a+16*j+0);
            x0=small8<4,true>(small8<2,true>(small8<1,true>(x0,0),0),0);
            I8 x1=load8(a+16*j+8);
            x1=small8<4,true>(small8<2,true>(small8<1,true>(x1,1),1),1);
            x0=add8(x0,x1);x1=x0;
            x0=small8<1,false>(small8<2,false>(small8<4,false>(x0,0),0),0);
            store8(a+16*j+0,x0);
            x1=small8<1,false>(small8<2,false>(small8<4,false>(x1,1),1),1);
            store8(a+16*j+8,x1);
        }
    }
    __attribute__((target("avx2"))) void project16_8(U* a,int rows) const {
        for(int j=0;j<rows;++j) {
            I8 x0=load8(a+32*j+0);
            x0=small8<4,true>(small8<2,true>(small8<1,true>(x0,0),0),0);
            I8 x1=load8(a+32*j+8);
            x1=small8<4,true>(small8<2,true>(small8<1,true>(x1,1),1),1);
            I8 x2=load8(a+32*j+16);
            x2=small8<4,true>(small8<2,true>(small8<1,true>(x2,2),2),2);
            I8 x3=load8(a+32*j+24);
            x3=small8<4,true>(small8<2,true>(small8<1,true>(x3,3),3),3);
            fixed_butterfly8<true>(x0,x1,_mm256_set1_epi32(inverse_roots[0]),_mm256_set1_epi32(inverse_root_quotients[0]),true);
            fixed_butterfly8<true>(x2,x3,_mm256_set1_epi32(inverse_roots[1]),_mm256_set1_epi32(inverse_root_quotients[1]),false);
            x0=add8(x0,x2);x2=x0;
            x1=add8(x1,x3);x3=x1;
            fixed_butterfly8<false>(x0,x1,_mm256_set1_epi32(roots[0]),_mm256_set1_epi32(root_quotients[0]),true);
            fixed_butterfly8<false>(x2,x3,_mm256_set1_epi32(roots[1]),_mm256_set1_epi32(root_quotients[1]),false);
            x0=small8<1,false>(small8<2,false>(small8<4,false>(x0,0),0),0);
            store8(a+32*j+0,x0);
            x1=small8<1,false>(small8<2,false>(small8<4,false>(x1,1),1),1);
            store8(a+32*j+8,x1);
            x2=small8<1,false>(small8<2,false>(small8<4,false>(x2,2),2),2);
            store8(a+32*j+16,x2);
            x3=small8<1,false>(small8<2,false>(small8<4,false>(x3,3),3),3);
            store8(a+32*j+24,x3);
        }
    }
    __attribute__((target("avx2"))) static void scale_rows8(U* a,int rows,int columns,const U* factors) {
        if(columns==2) {
            int j=0;
            for(;j+1<rows;j+=2) {
                U w0=factors[j],w1=factors[j+1],q0=quotient(w0),q1=quotient(w1);
                I8 w=_mm256_setr_epi32(w0,w0,w0,w0,w1,w1,w1,w1);
                I8 q=_mm256_setr_epi32(q0,q0,q0,q0,q1,q1,q1,q1);
                store8(a+4*j,fixed_mul8(load8(a+4*j),w,q));
            }
            if(j<rows) for(int i=0;i<4;++i) a[4*j+i]=fixed_mul(a[4*j+i],factors[j],quotient(factors[j]));
        } else {
            for(int j=0;j<rows;++j) {
                U w=factors[j];I8 v=_mm256_set1_epi32(w),q=_mm256_set1_epi32(quotient(w));
                for(int i=0;i<2*columns;i+=8) store8(a+2*columns*j+i,fixed_mul8(load8(a+2*columns*j+i),v,q));
            }
        }
    }
    __attribute__((target("avx2"))) void project4_8(U* a, int rows) const {
        for (int j = 0; j < rows; ++j) {
            I8 x = small8<2, true>(small8<1, true>(load8(a + 8 * j), 0), 0);
            x = add8(x, _mm256_permute2x128_si256(x, x, 1));
            x = small8<1, false>(small8<2, false>(x, 0), 0);
            store8(a + 8 * j, x);
        }
    }
    __attribute__((target("avx2"))) static void load_pairs(const U* a, I8& x, I8& y) {
        I8 index = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
        I8 u = _mm256_permutevar8x32_epi32(load8(a), index),
           v = _mm256_permutevar8x32_epi32(load8(a + 8), index);
        x = _mm256_permute2x128_si256(u, v, 0x20);
        y = _mm256_permute2x128_si256(u, v, 0x31);
    }
    __attribute__((target("avx2"))) static void store_pairs(U* a, I8 x, I8 y) {
        I8 index = _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7);
        store8(a, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(x, y, 0x20), index));
        store8(a + 8,
               _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(x, y, 0x31), index));
    }
    __attribute__((target("avx2"))) static void scale8(U* a, int length, U r) {
        I8 v = _mm256_set1_epi32(r), q = _mm256_set1_epi32(quotient(r));
        int i = 0;
        for (; i + 8 <= length; i += 8) store8(a + i, fixed_mul8(load8(a + i), v, q));
        for (; i < length; ++i) a[i] = fixed_mul(a[i], r, quotient(r));
    }
    __attribute__((target("avx2"))) static void pair_product8(U* a,const U* b,int length) {
        I8 ni=_mm256_set1_epi32(998244351),modulus=_mm256_set1_epi32(mod),p=_mm256_set1_epi32(twice);
        I8 index=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
        for(int i=0;i<length;i+=8) {
            I8 x=load8(b+2*i),y=load8(b+2*i+8);
            x=_mm256_min_epu32(x,_mm256_sub_epi32(x,p));y=_mm256_min_epu32(y,_mm256_sub_epi32(y,p));
            x=_mm256_mul_epu32(x,_mm256_srli_epi64(x,32));
            y=_mm256_mul_epu32(y,_mm256_srli_epi64(y,32));
            x=_mm256_add_epi64(x,_mm256_mul_epu32(_mm256_mul_epu32(x,ni),modulus));
            y=_mm256_add_epi64(y,_mm256_mul_epu32(_mm256_mul_epu32(y,ni),modulus));
            x=_mm256_blend_epi32(_mm256_srli_epi64(x,32),y,0xaa);
            store8(a+i,_mm256_permutevar8x32_epi32(x,index));
        }
    }
    __attribute__((target("avx2"))) static void pair_multiply8(U* a,const U* b,int length) {
        I8 index0=_mm256_setr_epi32(0,0,1,1,2,2,3,3),index1=_mm256_setr_epi32(4,4,5,5,6,6,7,7);
        I8 p=_mm256_set1_epi32(twice);
        for(int i=0;i<length;i+=8) {
            I8 r=load8(b+i),x=load8(a+2*i),y=load8(a+2*i+8);
            x=_mm256_min_epu32(x,_mm256_sub_epi32(x,p));y=_mm256_min_epu32(y,_mm256_sub_epi32(y,p));
            x=_mm256_shuffle_epi32(x,0xb1);y=_mm256_shuffle_epi32(y,0xb1);
            store8(a+2*i,mul8(x,_mm256_permutevar8x32_epi32(r,index0)));
            store8(a+2*i+8,mul8(y,_mm256_permutevar8x32_epi32(r,index1)));
        }
    }
    __attribute__((target("avx2"))) static void difference8(U* a, const U* b, const U* c,
                                                            int length) {
        for (int i = 0; i < length; i += 8) store8(a + i, sub8(load8(b + i), load8(c + i)));
    }
    __attribute__((target("avx2"))) static void project1_8(U* a, int rows) {
        for (int j = 0; j < rows; j += 8) {
            I8 x, y;
            load_pairs(a + 2 * j, x, y);
            x = add8(x, y);
            store_pairs(a + 2 * j, x, x);
        }
    }
    __attribute__((target("avx2"))) void project2_8(U* a, int rows) const {
        const I8 index = _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7);
        for (int j = 0; j < rows; j += 8) {
            U* p = a + 4 * j;
            I8 u = load8(p), v = load8(p + 8), w = load8(p + 16), z = load8(p + 24);
            I8 uv0 = _mm256_unpacklo_epi32(u, v), uv1 = _mm256_unpackhi_epi32(u, v);
            I8 wz0 = _mm256_unpacklo_epi32(w, z), wz1 = _mm256_unpackhi_epi32(w, z);
            I8 a0 = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(uv0, wz0), index);
            I8 a1 = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(uv0, wz0), index);
            I8 a2 = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(uv1, wz1), index);
            I8 a3 = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(uv1, wz1), index);
            I8 x = add8(add8(a0, a1), add8(a2, a3));
            I8 y =
                add8(sub8(a0, a1), fixed_mul8(sub8(a2, a3), _mm256_set1_epi32(inverse_roots[1]),
                                              _mm256_set1_epi32(inverse_root_quotients[1])));
            a0 = add8(x, y);
            a1 = sub8(x, y);
            y = fixed_mul8(y, _mm256_set1_epi32(roots[1]),
                           _mm256_set1_epi32(root_quotients[1]));
            a2 = add8(x, y);
            a3 = sub8(x, y);
            uv0 = _mm256_unpacklo_epi32(a0, a1);
            uv1 = _mm256_unpackhi_epi32(a0, a1);
            wz0 = _mm256_unpacklo_epi32(a2, a3);
            wz1 = _mm256_unpackhi_epi32(a2, a3);
            u = _mm256_unpacklo_epi64(uv0, wz0);
            v = _mm256_unpackhi_epi64(uv0, wz0);
            w = _mm256_unpacklo_epi64(uv1, wz1);
            z = _mm256_unpackhi_epi64(uv1, wz1);
            store8(p, _mm256_permute2x128_si256(u, v, 0x20));
            store8(p + 8, _mm256_permute2x128_si256(w, z, 0x20));
            store8(p + 16, _mm256_permute2x128_si256(u, v, 0x31));
            store8(p + 24, _mm256_permute2x128_si256(w, z, 0x31));
        }
    }
#endif

#if defined(__ARM_NEON)
    template <int stride, bool inverse>
    void small4(I& a, I& b, int k) const {
        const V& w = inverse ? inverse_roots : roots;
        const V& quot = inverse ? inverse_root_quotients : root_quotients;
        if constexpr (stride == 4) {
            if (k == 0) {
                I x = sub4(a, b);
                a = add4(a, b);
                b = x;
            } else
                fixed_butterfly4<inverse>(a, b, vdupq_n_u32(w[k]), vdupq_n_u32(quot[k]));
        }
        if constexpr (stride == 2) {
            I x = vcombine_u32(vget_low_u32(a), vget_low_u32(b));
            I y = vcombine_u32(vget_high_u32(a), vget_high_u32(b));
            fixed_butterfly4<inverse>(
                x, y, vcombine_u32(vdup_n_u32(w[2 * k]), vdup_n_u32(w[2 * k + 1])),
                vcombine_u32(vdup_n_u32(quot[2 * k]), vdup_n_u32(quot[2 * k + 1])));
            a = vcombine_u32(vget_low_u32(x), vget_low_u32(y));
            b = vcombine_u32(vget_high_u32(x), vget_high_u32(y));
        }
        if constexpr (stride == 1) {
            uint32x4x2_t z = vuzpq_u32(a, b);
            fixed_butterfly4<inverse>(z.val[0], z.val[1], vld1q_u32(w.data() + 4 * k),
                                      vld1q_u32(quot.data() + 4 * k));
            z = vzipq_u32(z.val[0], z.val[1]);
            a = z.val[0];
            b = z.val[1];
        }
    }
    template <bool inverse>
    void fft4(U* a, int length, int limit = 0) const {
        if (!limit) limit = length;
        if constexpr (!inverse) {
            int h = limit / 2;
            for (; h >= 16; h >>= 2) stage_pair<false>(a, h / 2, length);
            if (h == 8) stage<false>(a, h, length);
        }
        for (int j = 0; j < length; j += 8) {
            I x = vld1q_u32(a + j), y = vld1q_u32(a + j + 4);
            if constexpr (inverse) {
                small4<1, true>(x, y, j / 8);
                small4<2, true>(x, y, j / 8);
                small4<4, true>(x, y, j / 8);
            } else {
                small4<4, false>(x, y, j / 8);
                small4<2, false>(x, y, j / 8);
                small4<1, false>(x, y, j / 8);
            }
            vst1q_u32(a + j, x);
            vst1q_u32(a + j + 4, y);
        }
        if constexpr (inverse) {
            int h = 8;
            for (; 4 * h <= limit; h <<= 2) stage_pair<true>(a, h, length);
            if (h < limit) stage<true>(a, h, length);
        }
    }
#endif
    template <bool inverse>
    void stage(U* a, int stride, int length, int first = 0) const {
        const V& w = inverse ? inverse_roots : roots;
        const V& quot = inverse ? inverse_root_quotients : root_quotients;
#if defined(__x86_64__) || defined(__i386__)
        if (length >= 8 && vectorized()) {
            stage8<inverse>(a, stride, length, first);
            return;
        }
#endif
        for (int j = 0, k = first; j < length; j += 2 * stride, ++k) {
            U r = w[k], q = quot[k];
            int i = 0;
#if defined(__ARM_NEON)
            for (; i + 4 <= stride; i += 4) {
                I x = vld1q_u32(a + j + i), y = vld1q_u32(a + j + stride + i);
                if constexpr (!inverse)
                    if (k) y = fixed_mul4(y, vdupq_n_u32(r), vdupq_n_u32(q));
                I z = sub4(x, y);
                x = add4(x, y);
                if constexpr (inverse)
                    if (k) z = fixed_mul4(z, vdupq_n_u32(r), vdupq_n_u32(q));
                y = z;
                vst1q_u32(a + j + i, x);
                vst1q_u32(a + j + stride + i, y);
            }
#endif
            for (; i < stride; ++i) {
                U x = a[j + i], y = a[j + stride + i];
                if constexpr (!inverse)
                    if (k) y = fixed_mul(y, r, q);
                a[j + i] = add(x, y);
                a[j + stride + i] = inverse && k ? fixed_mul(sub(x, y), r, q) : sub(x, y);
            }
        }
    }

    template <bool inverse>
    void stage_pair(U* a, int stride, int length, int first = 0) const {
#if defined(__x86_64__) || defined(__i386__)
        if (stride >= 8 && vectorized()) {
            stage_pair8<inverse>(a, stride, length, first);
            return;
        }
#endif
#if defined(__ARM_NEON)
        if (stride >= 4) {
            const V& w = inverse ? inverse_roots : roots;
            const V& q = inverse ? inverse_root_quotients : root_quotients;
            for (int j = 0, k = first; j < length; j += 4 * stride, ++k) {
                I r = vdupq_n_u32(w[k]), rq = vdupq_n_u32(q[k]);
                I r0 = vdupq_n_u32(w[2 * k]), q0 = vdupq_n_u32(q[2 * k]);
                I r1 = vdupq_n_u32(w[2 * k + 1]), q1 = vdupq_n_u32(q[2 * k + 1]);
                for (int i = 0; i < stride; i += 4) {
                    I x0 = vld1q_u32(a + j + i), x1 = vld1q_u32(a + j + stride + i);
                    I x2 = vld1q_u32(a + j + 2 * stride + i),
                      x3 = vld1q_u32(a + j + 3 * stride + i);
                    if constexpr (inverse) {
                        fixed_butterfly4<true>(x0, x1, r0, q0, k == 0);
                        fixed_butterfly4<true>(x2, x3, r1, q1);
                        fixed_butterfly4<true>(x0, x2, r, rq, k == 0);
                        fixed_butterfly4<true>(x1, x3, r, rq, k == 0);
                    } else {
                        fixed_butterfly4<false>(x0, x2, r, rq, k == 0);
                        fixed_butterfly4<false>(x1, x3, r, rq, k == 0);
                        fixed_butterfly4<false>(x0, x1, r0, q0, k == 0);
                        fixed_butterfly4<false>(x2, x3, r1, q1);
                    }
                    vst1q_u32(a + j + i, x0);
                    vst1q_u32(a + j + stride + i, x1);
                    vst1q_u32(a + j + 2 * stride + i, x2);
                    vst1q_u32(a + j + 3 * stride + i, x3);
                }
            }
            return;
        }
#endif
        if constexpr (inverse)
            stage<true>(a, stride, length, 2 * first), stage<true>(a, 2 * stride, length, first);
        else
            stage<false>(a, 2 * stride, length, first), stage<false>(a, stride, length, 2 * first);
    }
    template <bool inverse>
    void fft(U* a, int length, int limit = 0) const {
        if (!limit) limit = length;
#if defined(__ARM_NEON)
        if (limit >= 8) {
            fft4<inverse>(a, length, limit);
            return;
        }
#elif defined(__x86_64__) || defined(__i386__)
        if (limit >= 8 && vectorized()) {
            fft8<inverse>(a, length, limit);
            return;
        }
#endif
        if constexpr (inverse) {
            for (int h = 1; h < limit; h <<= 1) stage<true>(a, h, length);
        } else {
            for (int h = limit / 2; h; h >>= 1) stage<false>(a, h, length);
        }
    }
#if defined(__x86_64__) || defined(__i386__)
    __attribute__((target("avx2"))) static void product8(U* dst, const U* a, const U* b, int n) {
        int i = 0;
        for (; i + 8 <= n; i += 8) {
            I8 x = mul8(load8(a + i), load8(b + i));
            store8(dst + i, x);
        }
        for (; i < n; ++i) dst[i] = mul(a[i], b[i]);
    }
    __attribute__((target("avx2"))) static void weighted8(U* dst, const U* src, int n, int first) {
        alignas(32) U factors[8];
        for (int j = 0; j < 8; ++j) factors[j] = in(first + j);
        I8 r = load8(factors), step = _mm256_set1_epi32(in(8));
        int i = 0;
        for (; i + 8 <= n; i += 8) {
            store8(dst + i, mul8(load8(src + i), r));
            r = add8(r, step);
        }
        for (; i < n; ++i) dst[i] = mul(src[i], in(first + i));
    }
    // Eight independent prefix products, one SIMD inversion, then a reverse sweep.
    __attribute__((target("avx2"))) static void inverse_sequence8(U* dst, int first, int n) {

        alignas(32) U initial[8];
        for (int j = 0; j < 8; ++j) initial[j] = in(first + j);
        I8 factor = load8(initial), step = _mm256_set1_epi32(in(8));
        I8 p = factor;
        store8(dst, p);
        for (int i = 8; i < n; i += 8) {
            factor = add8(factor, step);
            p = mul8(p, factor);
            store8(dst + i, p);
        }
        I8 inverse = _mm256_set1_epi32(one);
        for (U e = mod - 2; e; e >>= 1, p = mul8(p, p))
            if (e & 1) inverse = mul8(inverse, p);
        for (int i = n - 8; i >= 8; i -= 8) {
            store8(dst + i, mul8(inverse, load8(dst + i - 8)));
            inverse = mul8(inverse, factor);
            factor = sub8(factor, step);
        }
        store8(dst, inverse);
    }
    __attribute__((target("avx2"))) static void normalize8(U* a, int n) {
        I8 p = _mm256_set1_epi32(mod);
        int i = 0;
        for (; i + 8 <= n; i += 8) { I8 x = load8(a + i); store8(a + i, _mm256_min_epu32(x, _mm256_sub_epi32(x, p))); }
        for (; i < n; ++i) if (a[i] >= mod) a[i] -= mod;
    }
#endif
    static void weighted(U* dst, const U* src, int n, int first) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { weighted8(dst, src, n, first); return; }
#endif
        for (int i = 0; i < n; ++i) dst[i] = mul(src[i], in(first + i));
    }
    static void normalize(U* a, int n) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { normalize8(a, n); return; }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= n; i += 4) { I x = vld1q_u32(a + i); vst1q_u32(a + i, vminq_u32(x, vsubq_u32(x, vdupq_n_u32(mod)))); }
#endif
        for (; i < n; ++i) if (a[i] >= mod) a[i] -= mod;
    }
    static void product(U* dst, const U* a, const U* b, int n) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { product8(dst, a, b, n); return; }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= n; i += 4) {
            I x = mul4(vld1q_u32(a + i), vld1q_u32(b + i));
            vst1q_u32(dst + i, x);
        }
#endif
        for (; i < n; ++i) dst[i] = mul(a[i], b[i]);
    }
#if defined(__x86_64__) || defined(__i386__)
    template <bool inverse>
    __attribute__((target("avx2"), always_inline)) void convolution_stage8(U* a, int stride, int k) const {
        const V& w = inverse ? inverse_roots : roots;
        const V& q = inverse ? inverse_root_quotients : root_quotients;
        I8 r = _mm256_set1_epi32(w[k]), rq = _mm256_set1_epi32(q[k]);
        for (int i = 0; i < stride; i += 8) {
            I8 x = load8(a + i), y = load8(a + stride + i);
            fixed_butterfly8<inverse, true>(x, y, r, rq, k == 0);
            store8(a + i, x); store8(a + stride + i, y);
        }
    }
    __attribute__((target("avx2"), always_inline)) void leaf8(U* a, const U* b, int k) const {
        U w = roots[k / 2], q = root_quotients[k / 2];
        if (k & 1) w = mod - w, q = ~q;
        I8 p = _mm256_set1_epi32(mod), pp = _mm256_set1_epi32(twice);
        I8 x = load8(a), y = load8(b);
        x = _mm256_min_epu32(x, _mm256_sub_epi32(x, pp));
        y = _mm256_min_epu32(y, _mm256_sub_epi32(y, pp));
        x = _mm256_min_epu32(x, _mm256_sub_epi32(x, p));
        y = _mm256_min_epu32(y, _mm256_sub_epi32(y, p));
        alignas(32) U window[16], coefficients[8];
        I8 low = fixed_mul8(x, _mm256_set1_epi32(w), _mm256_set1_epi32(q));
        store8(window, _mm256_min_epu32(low, _mm256_sub_epi32(low, p)));
        store8(window + 8, x); store8(coefficients, y);
        I8 e = _mm256_setzero_si256(), o = e;
        for (int i = 0; i < 8; ++i) {
            I8 z = load8(window + 8 - i), r = _mm256_set1_epi32(coefficients[i]);
            e = _mm256_add_epi64(e, _mm256_mul_epu32(z, r));
            o = _mm256_add_epi64(o, _mm256_mul_epu32(_mm256_srli_epi64(z, 32), r));
        }
        I8 ni = _mm256_set1_epi32(998244351);
        e = _mm256_add_epi64(e, _mm256_mul_epu32(_mm256_mul_epu32(e, ni), p));
        o = _mm256_add_epi64(o, _mm256_mul_epu32(_mm256_mul_epu32(o, ni), p));
        x = _mm256_blend_epi32(_mm256_srli_epi64(e, 32), o, 0xaa);
        x = _mm256_min_epu32(x, _mm256_sub_epi32(x, pp));
        store8(a, _mm256_min_epu32(x, _mm256_sub_epi32(x, p)));
    }
    // Canonical inputs give 8*(p-1)^2 + p*(2^32-1) < 2^64.
    // Reduce once after the eight products; the result is below 4p.
    template<int count>
    __attribute__((target("avx2"), always_inline)) void inverse_leaf_products8(U* a,U* b,const U* ws,const U* qs) const {
        I8 p=_mm256_set1_epi32(mod),pp=_mm256_set1_epi32(twice);
        alignas(32) U window[count][16], coefficients[count][8];
        I8 e[count],o[count];
        for(int k=0;k<count;++k) {
            I8 x=load8(a+8*k),y=load8(b+8*k);
            x=_mm256_min_epu32(x,_mm256_sub_epi32(x,pp));
            y=_mm256_min_epu32(y,_mm256_sub_epi32(y,pp));
            x=_mm256_min_epu32(x,_mm256_sub_epi32(x,p));
            y=_mm256_min_epu32(y,_mm256_sub_epi32(y,p));
            I8 low=fixed_mul8(x,_mm256_set1_epi32(ws[k]),_mm256_set1_epi32(qs[k]));
            store8(window[k],_mm256_min_epu32(low,_mm256_sub_epi32(low,p)));
            store8(window[k]+8,x);store8(coefficients[k],y);store8(b+8*k,y);
            e[k]=o[k]=_mm256_setzero_si256();
        }
        for(int i=0;i<8;++i) for(int k=0;k<count;++k) {
            I8 z=load8(window[k]+8-i),r=_mm256_set1_epi32(coefficients[k][i]);
            e[k]=_mm256_add_epi64(e[k],_mm256_mul_epu32(z,r));
            o[k]=_mm256_add_epi64(o[k],_mm256_mul_epu32(_mm256_srli_epi64(z,32),r));
        }
        I8 ni=_mm256_set1_epi32(998244351);
        for(int k=0;k<count;++k) {
            I8 x=e[k],y=o[k];
            x=_mm256_add_epi64(x,_mm256_mul_epu32(_mm256_mul_epu32(x,ni),p));
            y=_mm256_add_epi64(y,_mm256_mul_epu32(_mm256_mul_epu32(y,ni),p));
            x=_mm256_blend_epi32(_mm256_srli_epi64(x,32),y,0xaa);
            store8(a+8*k,_mm256_min_epu32(x,_mm256_sub_epi32(x,pp)));
        }
    }
    __attribute__((target("avx2"))) static void inverse_fold8(U* a,const U* f,int n) {
        int i=0;for(;i+8<=n;i+=8) store8(a+i,add8(load8(a+i),load8(f+i)));
        for(;i<n;++i) {a[i]+=f[i];if(a[i]>=twice) a[i]-=twice;}
    }
    __attribute__((target("avx2"))) static void inverse_recover8(U* g,const U* a,int m) {
        int n=2*m;U ratio=T(3).pow((mod-1)/(2*(n/8))).inv().val();
        U first=mod-(mod-1)/(2*(n/8));
        U phase=T(3).pow((mod-1)/4).inv().val();
        U powers[8];powers[0]=first;
        for(int j=1;j<8;++j) powers[j]=(unsigned long long)powers[j-1]*ratio%mod;
        U step=T(ratio).pow(8).val();
        I8 factors=load8(powers),s=_mm256_set1_epi32(step),sq=_mm256_set1_epi32(quotient(step));
        I8 vphase=_mm256_set1_epi32(phase),qphase=_mm256_set1_epi32(quotient(phase)),p=_mm256_set1_epi32(mod),z=_mm256_setzero_si256();
        for(int i=0;i<m;i+=64) {
            store8(powers,factors);
            for(int j=0;j<8;++j) {
                int at=i+8*j;U w=powers[j];I8 v=_mm256_set1_epi32(w),q=_mm256_set1_epi32(quotient(w));
                I8 r=_mm256_add_epi32(load8(a+at),fixed_mul8(load8(a+m+at),vphase,qphase));
                r=fixed_mul8(r,v,q);
                store8(g+m+at,sub8(z,add8(r,load8(g+m+at))));
            }
            factors=fixed_mul8(factors,s,sq);factors=_mm256_min_epu32(factors,_mm256_sub_epi32(factors,p));
        }
    }
    template<bool inverse>
    __attribute__((target("avx2"))) void inverse_partial8(U* a,int n,int limit=0) const {
        if(!limit) limit=n;
        if constexpr(!inverse) {
            int stride=limit/2;
            for(;stride>=16;stride>>=2) radix4_8<false>(a,stride/2,n);
            if(stride==8) for(int i=0;i<n;i+=16) convolution_stage8<false>(a+i,8,i/16);
        } else {
            int stride=8;
            for(;4*stride<=n;stride<<=2) radix4_8<true>(a,stride,n);
            if(stride<n) stage8<true>(a,stride,n);
        }
    }
    __attribute__((target("avx2"))) static void inverse_twist8(U* a,U* b,int n,U ratio,U first=one) {
        U powers[8];powers[0]=out(first);U r=out(ratio);
        for(int j=1;j<8;++j) powers[j]=(unsigned long long)powers[j-1]*r%mod;
        U step=T::word(ratio).pow(8).val();
        I8 factors=load8(powers),s=_mm256_set1_epi32(step),sq=_mm256_set1_epi32(quotient(step)),p=_mm256_set1_epi32(mod);
        for(int i=0;i<n;i+=64) {
            store8(powers,factors);
            for(int j=0;j<8 && i+8*j<n;++j) {
                U w=powers[j];I8 v=_mm256_set1_epi32(w),q=_mm256_set1_epi32(quotient(w));
                store8(a+i+8*j,fixed_mul8(load8(a+i+8*j),v,q));
                if(b && i+8*j<n/2) store8(b+i+8*j,fixed_mul8(load8(b+i+8*j),v,q));
            }
            factors=fixed_mul8(factors,s,sq);factors=_mm256_min_epu32(factors,_mm256_sub_epi32(factors,p));
        }
    }
    // P=f*g*g-g. The cyclic branch returns P/2 modulo x^n-1.
    // The negative branch leaves block twists and inverse scale for recovery;
    // its g input must have a zero upper half. Keep eight coefficients per leaf.
    template<bool negative>
    __attribute__((target("avx2"))) void inverse_cubic8(U* a,U* b,int n) const {
        U shift=one;
        if constexpr(negative) {
            T psi=T(3).pow((mod-1)/(2*(n/8)));
            shift=psi.v;
            if(shift>=mod) shift-=mod;
            inverse_twist8(a,b,n,shift);
        }
        inverse_partial8<false>(a,n);
        if constexpr(negative) {std::copy_n(b,n/2,b+n/2);inverse_partial8<false>(b,n,n/2);}
        else inverse_partial8<false>(b,n);
        I8 p=_mm256_set1_epi32(twice);
        for(int i=0;i<n;i+=8*8) {
            U ws[8],qs[8];
            for(int j=0;j<8;++j) {
                int k=i/8+j;
                U w=roots[k/2];if(k&1) w=mod-w;
                if constexpr(negative) {w=mul(w,shift);if(w>=mod) w-=mod;qs[j]=quotient(w);}
                else qs[j]=(k&1)?~root_quotients[k/2]:root_quotients[k/2];
                ws[j]=w;
            }
            inverse_leaf_products8<8>(a+i,b+i,ws,qs);inverse_leaf_products8<8>(a+i,b+i,ws,qs);
            for(int j=0;j<8;++j) store8(a+i+8*j,sub8(load8(a+i+8*j),load8(b+i+8*j)));
        }
        inverse_partial8<true>(a,n);
        U normal=mod-(mod-1)/(2*(n/8));
        if constexpr(!negative) scale8(a,n,normal);
    }
    template <int n>
    __attribute__((target("avx2"), always_inline)) void convolution_tile8(U* a, U* b, int k) const {
        if constexpr (n == 8) leaf8(a, b, k);
        else if constexpr (n == 16) {
            convolution_stage8<false>(a, n / 2, k); convolution_stage8<false>(b, n / 2, k);
            convolution_tile8<n / 2>(a, b, 2 * k);
            convolution_tile8<n / 2>(a + n / 2, b + n / 2, 2 * k + 1);
            convolution_stage8<true>(a, n / 2, k);
        } else {
            stage_pair8<false, true>(a, n / 4, n, k); stage_pair8<false, true>(b, n / 4, n, k);
            for (int i = 0; i < 4; ++i) convolution_tile8<n / 4>(a + i * (n / 4), b + i * (n / 4), 4 * k + i);
            stage_pair8<true>(a, n / 4, n, k);
        }
    }
    __attribute__((target("avx2"))) void convolution8(U* a, U* b, int n, int k = 0) const {
        if (n == 256) { convolution_tile8<256>(a, b, k); return; }
        if (n == 128) { convolution_tile8<128>(a, b, k); return; }
        if (n == 64) { convolution_tile8<64>(a, b, k); return; }
        stage_pair8<false, true>(a, n / 4, n, k); stage_pair8<false, true>(b, n / 4, n, k);
        for (int i = 0; i < 4; ++i) convolution8(a + i * (n / 4), b + i * (n / 4), n / 4, 4 * k + i);
        stage_pair8<true>(a, n / 4, n, k);
    }
#endif
#if defined(__x86_64__) || defined(__i386__)
    template <bool odd>
    __attribute__((target("avx2"))) static void reduce_pairs8(U* a, U* b, const U* twist, int n) {
        for (int i = 0; i < n; i += 8) {
            I8 x, y, u, v;
            load_pairs(a + 2 * i, x, y); load_pairs(b + 2 * i, u, v);
            x = mul8(x, v); y = mul8(y, u);
            if constexpr (odd) x = mul8(sub8(x, y), load8(twist + i)); else x = add8(x, y);
            store8(a + i, x); store8(b + i, mul8(u, v));
        }
    }
    __attribute__((target("avx2"))) static void combine8(U* a, const U* b, const U* x, const U* y, int n) {
        for (int i = 0; i < n; i += 8) store8(a + i, add8(mul8(load8(a + i), load8(y + i)), mul8(load8(b + i), load8(x + i))));
    }
#endif
    template <bool odd> static void reduce_pairs(U* a, U* b, const U* twist, int n) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { reduce_pairs8<odd>(a, b, twist, n); return; }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= n; i += 4) {
            auto p = vld2q_u32(a + 2 * i), q = vld2q_u32(b + 2 * i);
            I x = mul4(p.val[0], q.val[1]), y = mul4(p.val[1], q.val[0]);
            if constexpr (odd) x = mul4(sub4(x, y), vld1q_u32(twist + i)); else x = add4(x, y);
            vst1q_u32(a + i, x); vst1q_u32(b + i, mul4(q.val[0], q.val[1]));
        }
#endif
        for (; i < n; ++i) {
            U u = mul(a[2 * i], b[2 * i + 1]), v = mul(a[2 * i + 1], b[2 * i]);
            a[i] = odd ? mul(sub(u, v), twist[i]) : add(u, v);
            b[i] = mul(b[2 * i], b[2 * i + 1]);
        }
    }
    static void combine(U* a, const U* b, const U* x, const U* y, int n) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { combine8(a, b, x, y, n); return; }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= n; i += 4) vst1q_u32(a + i, add4(mul4(vld1q_u32(a + i), vld1q_u32(y + i)), mul4(vld1q_u32(b + i), vld1q_u32(x + i))));
#endif
        for (; i < n; ++i) a[i] = add(mul(a[i], y[i]), mul(b[i], x[i]));
    }
#if defined(__x86_64__) || defined(__i386__)
    __attribute__((target("avx2"))) static void geometric8(U* a, int n, U first, U step) {
        alignas(32) U factors[8]; U power = one;
        for (int i = 0; i < 8; ++i) { factors[i] = mul(first, power); power = mul(power, step); }
        I8 r = load8(factors), ratio = _mm256_set1_epi32(power);
        int i = 0;
        for (; i + 8 <= n; i += 8) { store8(a + i, mul8(load8(a + i), r)); r = mul8(r, ratio); }
        store8(factors, r);
        for (int j = 0; i < n; ++i, ++j) a[i] = mul(a[i], factors[j]);
    }
#endif
    static void geometric(U* a, int n, U first, U step) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { geometric8(a, n, first, step); return; }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        U factors[4], power = one;
        for (int j = 0; j < 4; ++j) { factors[j] = mul(first, power); power = mul(power, step); }
        I r = vld1q_u32(factors), ratio = vdupq_n_u32(power);
        for (; i + 4 <= n; i += 4) { vst1q_u32(a + i, mul4(vld1q_u32(a + i), r)); r = mul4(r, ratio); }
        vst1q_u32(factors, r);
        first = factors[0];
#endif
        for (; i < n; ++i) { a[i] = mul(a[i], first); first = mul(first, step); }
    }
#if defined(__x86_64__) || defined(__i386__)
    __attribute__((target("avx2"))) static void add_scaled8(U* a, const U* b, int n, U r) {
        U q = quotient(r); I8 v = _mm256_set1_epi32(r), vq = _mm256_set1_epi32(q);
        int i = 0;
        for (; i + 8 <= n; i += 8) store8(a + i, add8(load8(a + i), fixed_mul8(load8(b + i), v, vq)));
        for (; i < n; ++i) a[i] = add(a[i], fixed_mul(b[i], r, q));
    }
#endif
    static void add_scaled(U* a, const U* b, int n, U r) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { add_scaled8(a, b, n, r); return; }
#endif
        int i = 0; U q = quotient(r);
#if defined(__ARM_NEON)
        for (; i + 4 <= n; i += 4) vst1q_u32(a + i, add4(vld1q_u32(a + i), fixed_mul4(vld1q_u32(b + i), vdupq_n_u32(r), vdupq_n_u32(q))));
#endif
        for (; i < n; ++i) a[i] = add(a[i], fixed_mul(b[i], r, q));
    }
    void leaf(U* a, const U* b, int k) const {
        U w = roots[k / 2], q = root_quotients[k / 2];
        if (k & 1) w = mod - w, q = ~q;
        U window[16], coefficients[8];
        for (int i = 0; i < 8; ++i) {
            U x = a[i] < mod ? a[i] : a[i] - mod;
            U z = fixed_mul(x, w, q);
            window[i] = z < mod ? z : z - mod; window[8 + i] = x;
            coefficients[i] = b[i] < mod ? b[i] : b[i] - mod;
        }
#if defined(__ARM_NEON)
        for (int j = 0; j < 8; j += 4) {
            uint64x2_t lo = vdupq_n_u64(0), hi = lo;
            for (int i = 0; i < 8; ++i) {
                I z = vld1q_u32(window + 8 - i + j);
                lo = vmlal_n_u32(lo, vget_low_u32(z), coefficients[i]);
                hi = vmlal_n_u32(hi, vget_high_u32(z), coefficients[i]);
            }
            uint32x2_t x = vmul_n_u32(vmovn_u64(lo), 998244351u), y = vmul_n_u32(vmovn_u64(hi), 998244351u);
            I z = vcombine_u32(vshrn_n_u64(vmlal_n_u32(lo, x, mod), 32), vshrn_n_u64(vmlal_n_u32(hi, y, mod), 32));
            z = vminq_u32(z, vsubq_u32(z, vdupq_n_u32(twice)));
            vst1q_u32(a + j, vminq_u32(z, vsubq_u32(z, vdupq_n_u32(mod))));
        }
#else
        for (int j = 0; j < 8; ++j) {
            unsigned long long sum = 0;
            for (int i = 0; i < 8; ++i) sum += (unsigned long long)window[8 - i + j] * coefficients[i];
            U x = reduce(sum); if (x >= twice) x -= twice;
            a[j] = x < mod ? x : x - mod;
        }
#endif
    }
    template <int n> void convolution_tile(U* a, U* b, int k) const {
        if constexpr (n == 8) leaf(a, b, k);
        else if constexpr (n == 16) {
            stage<false>(a, n / 2, n, k); stage<false>(b, n / 2, n, k);
            convolution_tile<n / 2>(a, b, 2 * k);
            convolution_tile<n / 2>(a + n / 2, b + n / 2, 2 * k + 1);
            stage<true>(a, n / 2, n, k);
        } else {
            stage_pair<false>(a, n / 4, n, k); stage_pair<false>(b, n / 4, n, k);
            for (int i = 0; i < 4; ++i) convolution_tile<n / 4>(a + i * (n / 4), b + i * (n / 4), 4 * k + i);
            stage_pair<true>(a, n / 4, n, k);
        }
    }
    void convolution(U* a, U* b, int n, int k = 0) const {
#if defined(__x86_64__) || defined(__i386__)
        if (vectorized()) { convolution8(a, b, n, k); return; }
#endif
        if (n == 256) { convolution_tile<256>(a, b, k); return; }
        if (n == 128) { convolution_tile<128>(a, b, k); return; }
        if (n == 64) { convolution_tile<64>(a, b, k); return; }
        stage_pair<false>(a, n / 4, n, k); stage_pair<false>(b, n / 4, n, k);
        for (int i = 0; i < 4; ++i) convolution(a + i * (n / 4), b + i * (n / 4), n / 4, 4 * k + i);
        stage_pair<true>(a, n / 4, n, k);
    }
#if defined(__x86_64__) || defined(__i386__)
    __attribute__((target("avx2"))) static void accumulate8(U* dst, const U* a, const U* b, const U* c, int n, bool minus) {
        for (int i = 0; i < n; i += 8) {
            I8 x = load8(b + i), y = load8(c + i);
            // a is normalized below mod; b +/- c may stay below 4*mod.
            x = minus ? _mm256_sub_epi32(_mm256_add_epi32(x, _mm256_set1_epi32(twice)), y)
                      : _mm256_add_epi32(x, y);
            store8(dst + i, add8(load8(dst + i), mul8(load8(a + i), x)));
        }
    }
#endif
#if defined(__x86_64__) || defined(__i386__)
    template<int count>
    __attribute__((target("avx2"))) static void accumulate_batch8(U* dst, const U* const* a, const U* const* b,
                                                                const U* const* c, int n, bool minus) {
        static_assert(count>=1 && count<=3);
        I8 p=_mm256_set1_epi32(mod), pp=_mm256_set1_epi32(twice), ni=_mm256_set1_epi32(998244351);
        int offset=minus?n:0;
        for(int i=0;i<n;i+=8) {
            I8 even=_mm256_setzero_si256(), odd=even;
            #pragma GCC unroll 3
            for(int k=0;k<count;++k) {
                I8 x=load8(b[k]+offset+i), y=load8(c[k]+offset+i);
                x=minus?_mm256_sub_epi32(_mm256_add_epi32(x,pp),y):_mm256_add_epi32(x,y);
                I8 z=load8(a[k]+offset+i);
                even=_mm256_add_epi64(even,_mm256_mul_epu32(z,x));
                odd=_mm256_add_epi64(odd,_mm256_mul_epu32(_mm256_srli_epi64(z,32),_mm256_srli_epi64(x,32)));
            }
            // 3 * p * (4p) + p * (2^32-1) < 2^64, and the reduced result is <4p.
            even=_mm256_add_epi64(even,_mm256_mul_epu32(_mm256_mul_epu32(even,ni),p));
            odd=_mm256_add_epi64(odd,_mm256_mul_epu32(_mm256_mul_epu32(odd,ni),p));
            I8 value=_mm256_blend_epi32(_mm256_srli_epi64(even,32),odd,0xaa);
            value=_mm256_min_epu32(value,_mm256_sub_epi32(value,pp));
            store8(dst+i,add8(load8(dst+i),value));
        }
    }
#endif
    template<int count>
    static void accumulate_batch(U* dst, const U* const* a, const U* const* b, const U* const* c, int n, bool minus) {
#if defined(__x86_64__) || defined(__i386__)
        if(n>=8 && vectorized()) {accumulate_batch8<count>(dst,a,b,c,n,minus);return;}
#endif
        int offset=minus?n:0;
        for(int k=0;k<count;++k) accumulate(dst,a[k]+offset,b[k]+offset,c[k]+offset,n,minus);
    }
    static void accumulate(U* dst, const U* a, const U* b, const U* c, int n, bool minus) {
#if defined(__x86_64__) || defined(__i386__)
        if (n >= 8 && vectorized()) { accumulate8(dst, a, b, c, n, minus); return; }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= n; i += 4) {
            I x = vld1q_u32(b + i), y = vld1q_u32(c + i);
            x = minus ? sub4(x, y) : add4(x, y);
            vst1q_u32(dst + i, add4(vld1q_u32(dst + i), mul4(vld1q_u32(a + i), x)));
        }
#endif
        for (; i < n; ++i) dst[i] = add(dst[i], mul(a[i], minus ? sub(b[i], c[i]) : add(b[i], c[i])));
    }
    void scale(U* a, int length, U r) const {
#if defined(__x86_64__) || defined(__i386__)
        if (length >= 8 && vectorized()) {
            scale8(a, length, r);
            return;
        }
#endif
        int i = 0;
        U q = quotient(r);
#if defined(__ARM_NEON)
        for (; i + 4 <= length; i += 4)
            vst1q_u32(a + i, fixed_mul4(vld1q_u32(a + i), vdupq_n_u32(r), vdupq_n_u32(q)));
#endif
        for (; i < length; ++i) a[i] = fixed_mul(a[i], r, q);
    }

    static void pair_product(U* a, const U* b, int length) {
#if defined(__x86_64__) || defined(__i386__)
        if (length >= 8 && vectorized()) {
            pair_product8(a, b, length);
            return;
        }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= length; i += 4) {
            uint32x4x2_t v = vld2q_u32(b + 2 * i);
            vst1q_u32(a + i, mul4(v.val[0], v.val[1]));
        }
#endif
        for (; i < length; ++i) a[i] = mul(b[2 * i], b[2 * i + 1]);
    }
    static void pair_multiply(U* a, const U* b, int length) {
#if defined(__x86_64__) || defined(__i386__)
        if (length >= 8 && vectorized()) {
            pair_multiply8(a, b, length);
            return;
        }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= length; i += 4) {
            uint32x4x2_t v = vld2q_u32(a + 2 * i);
            I x = mul4(v.val[1], vld1q_u32(b + i));
            v.val[1] = mul4(v.val[0], vld1q_u32(b + i));
            v.val[0] = x;
            vst2q_u32(a + 2 * i, v);
        }
#endif
        for (; i < length; ++i) {
            U x = a[2 * i];
            a[2 * i] = mul(b[i], a[2 * i + 1]);
            a[2 * i + 1] = mul(b[i], x);
        }
    }
    static void difference(U* a, const U* b, const U* c, int length) {
#if defined(__x86_64__) || defined(__i386__)
        if (length >= 8 && vectorized()) {
            difference8(a, b, c, length);
            return;
        }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= length; i += 4)
            vst1q_u32(a + i, sub4(vld1q_u32(b + i), vld1q_u32(c + i)));
#endif
        for (; i < length; ++i) a[i] = sub(b[i], c[i]);
    }
    static void fold(U* a, int columns) {
#if defined(__x86_64__) || defined(__i386__)
        if (columns >= 8 && vectorized()) {
            fold8(a, columns);
            return;
        }
#endif
        int i = 0;
#if defined(__ARM_NEON)
        for (; i + 4 <= columns; i += 4) {
            I x = add4(vld1q_u32(a + i), vld1q_u32(a + columns + i));
            vst1q_u32(a + i, x);
            vst1q_u32(a + columns + i, x);
        }
#endif
        for (; i < columns; ++i) a[i] = a[columns + i] = add(a[i], a[columns + i]);
    }
    void project(U* a, int rows, int columns) const {
        if (columns == 1) {
#if defined(__x86_64__) || defined(__i386__)
            if (rows >= 8 && vectorized()) {
                project1_8(a, rows);
                return;
            }
#endif
            int j = 0;
#if defined(__ARM_NEON)
            for (; j + 4 <= rows; j += 4) {
                uint32x4x2_t v = vld2q_u32(a + 2 * j);
                v.val[0] = v.val[1] = add4(v.val[0], v.val[1]);
                vst2q_u32(a + 2 * j, v);
            }
#endif
            for (; j < rows; ++j) a[2 * j] = a[2 * j + 1] = add(a[2 * j], a[2 * j + 1]);
            return;
        }
        if (columns == 2) {
#if defined(__x86_64__) || defined(__i386__)
            if (rows >= 8 && vectorized()) {
                project2_8(a, rows);
                return;
            }
#endif
            int j = 0;
#if defined(__ARM_NEON)
            for (; j + 4 <= rows; j += 4) {
                uint32x4x4_t v = vld4q_u32(a + 4 * j);
                I x = add4(add4(v.val[0], v.val[1]), add4(v.val[2], v.val[3]));
                I y = add4(sub4(v.val[0], v.val[1]),
                           fixed_mul4(sub4(v.val[2], v.val[3]), vdupq_n_u32(inverse_roots[1]),
                                      vdupq_n_u32(inverse_root_quotients[1])));
                v.val[0] = add4(x, y);
                v.val[1] = sub4(x, y);
                y = fixed_mul4(y, vdupq_n_u32(roots[1]), vdupq_n_u32(root_quotients[1]));
                v.val[2] = add4(x, y);
                v.val[3] = sub4(x, y);
                vst4q_u32(a + 4 * j, v);
            }
#endif
            for (; j < rows; ++j) {
                U* r = a + 4 * j;
                U x = add(add(r[0], r[1]), add(r[2], r[3]));
                U y = add(sub(r[0], r[1]), fixed_mul(sub(r[2], r[3]), inverse_roots[1],
                                                     inverse_root_quotients[1]));
                r[0] = add(x, y);
                r[1] = sub(x, y);
                y = fixed_mul(y, roots[1], root_quotients[1]);
                r[2] = add(x, y);
                r[3] = sub(x, y);
            }
            return;
        }
        if (columns == 4) {
#if defined(__x86_64__) || defined(__i386__)
            if (vectorized()) {
                project4_8(a, rows);
                return;
            }
#endif
#if defined(__ARM_NEON)
            for (int j = 0; j < rows; ++j) {
                I x = vld1q_u32(a + 8 * j), y = vld1q_u32(a + 8 * j + 4);
                small4<1, true>(x, y, 0);
                small4<2, true>(x, y, 0);
                x = y = add4(x, y);
                small4<2, false>(x, y, 0);
                small4<1, false>(x, y, 0);
                vst1q_u32(a + 8 * j, x);
                vst1q_u32(a + 8 * j + 4, y);
            }
            return;
#endif
        }
#if defined(__x86_64__) || defined(__i386__)
        if(columns==8 && vectorized()) { project8_8(a,rows);return; }
#endif
#if defined(__x86_64__) || defined(__i386__)
        if(columns==16 && vectorized()) { project16_8(a,rows);return; }
#endif
        for (int j = 0; j < rows; ++j) {
            U* row = a + 2 * columns * j;
            fft<true>(row, 2 * columns, columns);
            fold(row, columns);
            fft<false>(row, 2 * columns, columns);
        }
    }
    void composition_difference(U* a,const U* b,const U* c,int n) const {
#if defined(__x86_64__) || defined(__i386__)
        if(vectorized()) {composition_difference8(a,b,c,n);return;}
#endif
        difference(a,b,c,n);
    }
#if defined(__x86_64__) || defined(__i386__)
    __attribute__((target("avx2"))) static void composition_difference8(U* a,const U* b,const U* c,int n) {
        I8 p=_mm256_set1_epi32(twice);
        for(int i=0;i<n;i+=8) {I8 x=load8(c+i);x=_mm256_min_epu32(x,_mm256_sub_epi32(x,p));store8(a+i,sub8(load8(b+i),x));}
    }
#endif
    // Fuse the omitted last stage of both inverse FFTs and the final
    // length-4n butterfly with output scaling. Only the first n terms are needed.
    void composition_finish(U* p,const U* v,int columns,U factor) const {
#if defined(__x86_64__) || defined(__i386__)
        if(columns>=8 && vectorized()) {composition_finish8(p,v,columns,out(factor));return;}
#endif
        for(int i=0;i<columns;++i) p[i]=mul(sub(add(v[i],v[columns+i]),add(v[2*columns+i],v[3*columns+i])),factor);
    }
#if defined(__x86_64__) || defined(__i386__)
    __attribute__((target("avx2"))) static void composition_finish8(U* dst,const U* v,int n,U factor) {
        I8 r=_mm256_set1_epi32(factor),q=_mm256_set1_epi32(quotient(factor));
        for(int i=0;i<n;i+=8) {
            I8 x=add8(load8(v+i),load8(v+n+i)),y=add8(load8(v+2*n+i),load8(v+3*n+i));
            store8(dst+i,fixed_mul8(sub8(x,y),r,q));
        }
    }
#endif
    // One aligned, uninitialized slab for all recursion levels. Each level is
    // fully written before use; only the top polynomial needs explicit zeros.
    struct CompositionDelete {
        void operator()(unsigned* p) const {::operator delete[](p,std::align_val_t(64));}
    };
    using CompositionWords=std::unique_ptr<unsigned[],CompositionDelete>;
    // Forward radix-4 may leave values below 4p. Fold its final correction
    // into the next pair product, pair multiplication, or difference.
    void composition_forward_y(U* a,int size,int columns) const {
#if defined(__x86_64__) || defined(__i386__)
        if(vectorized()) {
            int h=size;bool lazy=false;
            for(;h/2>columns && h/2>=8;h>>=2) {radix4_8<false>(a,h/2,2*size);lazy=true;}
            if(lazy && h>columns) composition_shrink8(a,2*size);
            for(;h/2>columns;h>>=2) stage_pair<false>(a,h/2,2*size);
            if(h>columns) stage<false>(a,h,2*size);
            return;
        }
#endif
        int h=size;
        for(;h/2>columns;h>>=2) stage_pair<false>(a,h/2,2*size);
        if(h>columns) stage<false>(a,h,2*size);
    }
#if defined(__x86_64__) || defined(__i386__)
    __attribute__((target("avx2"))) static void composition_shrink8(U* a,int n) {
        I8 p=_mm256_set1_epi32(twice);
        for(int i=0;i<n;i+=8) {I8 x=load8(a+i);store8(a+i,_mm256_min_epu32(x,_mm256_sub_epi32(x,p)));}
    }
#endif
    void double_y(U* a, int rows, int columns, bool inverse) const {
        const int size = rows * columns;
        int h = 2 * columns;
        for (; h <= size / 2; h <<= 2) {
#if defined(__x86_64__) || defined(__i386__)
            if(h>=8 && vectorized()) radix4_8<true>(a,h,2*size);
            else
#endif
            stage_pair<true>(a, h, 2 * size);
        }
        if (h <= size) stage<true>(a, h, 2 * size);
#if defined(__x86_64__) || defined(__i386__)
        if(columns>=2 && vectorized()) {
            scale_rows8(a,rows,columns,(inverse?inverse_twists:twists).data()+rows);return;
        }
#endif
        for (int j = 0; j < rows; ++j)
            scale(a + 2 * columns * j, 2 * columns,
                  (inverse ? inverse_twists : twists)[rows + j]);
    }
    void compose(U* p, const U* q, int rows, int columns, U& p_one, U& q_one, U* workspace=nullptr,int level=0) const {
        const int size = rows * columns;
        if (columns == 1) {
            fft<false>(p, rows);
            for (int i = rows - 1; i >= 0; --i) p[2 * i] = p[2 * i + 1] = p[i];
            return;
        }
        CompositionWords storage;
        if(!workspace) {
            int levels=__builtin_ctz((unsigned)columns);
            storage.reset(static_cast<unsigned*>(::operator new[]((4*size+16)*levels*sizeof(unsigned),std::align_val_t(64))));
            workspace=storage.get();
        }
        // The extra 16 words keep 64-byte alignment and separate cache indices.
        U* v=workspace+(4*size+16)*level;
        if (rows == 1) {
            std::fill_n(v+columns,columns,U(0));
            for (int i = 0; i < columns; ++i) v[i] = sub(0, q[i]);
            fft<false>(v, 2 * columns);
            for (int i = 0; i < 2 * columns; ++i) {
                U x = v[i];
                v[i] = add(x, one);
                v[i + 2 * columns] = sub(x, one);
            }
        } else {
            pair_product(v, q, 2 * size);
            q_one = mul(q_one, q_one);
            project(v, rows, columns);
            std::copy_n(v, 2 * size, v + 2 * size);
            U* upper = v + 2 * size;
            double_y(upper, rows, columns, false);
            q_one = mul(q_one, in(2 * columns));
            U correction = add(q_one, q_one);
            for (int i = 0; i < 2 * columns; ++i) upper[i] = sub(upper[i], correction);
            composition_forward_y(upper,size,columns);
        }
        U saved = q_one;
        compose(p, v, 2 * rows, columns / 2, p_one, q_one,workspace,level+1);
        pair_multiply(v, p, 2 * size);
        p_one = mul(p_one, saved);
        if (rows == 1) {
            fft<true>(v, 2 * columns, columns);
            fft<true>(v + 2 * columns, 2 * columns, columns);
            U factor = in(T(out(mul(p_one, in(4 * size)))).inv().val());
            composition_finish(p,v,columns,factor);
        } else {
            U* upper = v + 2 * size;
            double_y(upper, rows, columns, true);
            composition_forward_y(upper,size,columns);
            p_one = add(p_one, p_one);
            composition_difference(p, v, upper, 2 * size);
            project(p, rows, columns);
            p_one = mul(p_one, in(2 * columns));
        }
    }
};
}

#if __has_include(<atcoder/modint>) && !defined(FPS_NATIVE_MINT)
using fps_mint = atcoder::modint998244353;
#else
using fps_mint = fps_detail::Mint;
#endif

template <class T = fps_mint>
struct FormalPowerSeries : std::vector<T> {
    static_assert(T::mod() == 998244353, "FormalPowerSeries supports only mod 998244353");
    using W = FormalPowerSeries<fps_detail::Mint>;
    using base = std::vector<T>;
    using F = FormalPowerSeries;
    using base::base;

    FormalPowerSeries() = default;
    FormalPowerSeries(base v) : base(std::move(v)) {}
    F& operator=(base v) {
        base::operator=(std::move(v));
        return *this;
    }
    F& operator=(std::initializer_list<T> v) {
        base::operator=(v);
        return *this;
    }

    static F multiply(const F& a, const F& b) {
        if (a.empty() || b.empty()) return {};
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) {
            W x = import_work(a);
            return export_work(&a == &b ? W::multiply(x, x) : W::multiply(x, import_work(b)));
        }
        if (std::min(a.size(), b.size()) <= 32) {
            F r(a.size() + b.size() - 1);
            const F& small = a.size() < b.size() ? a : b;
            const F& large = a.size() < b.size() ? b : a;
            if constexpr (std::is_same_v<T, fps_detail::Mint>) {
                for (int j = 0; j < (int)small.size(); ++j)
                    fps_detail::NTT::add_scaled(words(r) + j, words(large), large.size(), small[j].val());
            } else for (int j = 0; j < (int)small.size(); ++j)
                for (int i = 0; i < (int)large.size(); ++i) r[i + j] += large[i] * small[j];
            return r;
        }
        int n = 1, size = a.size() + b.size() - 1;
        while (n < size) n <<= 1;
        assert(n <= (1 << 23));
        F x = a, y;
        x.resize(n);
        if (&a != &b) { y = b; y.resize(n); }
        if (n >= 64 && &a != &b) {
            auto& t = transform(n / 16);
            t.convolution(words(x), words(y), n);
            x.resize(size);
            t.scale(words(x), size, T::mod() - (T::mod() - 1) / (n / 8));
            return x;
        }
        ntt(x);
        if (&a == &b) pointwise(x, x, x, n);
        else { ntt(y); pointwise(x, x, y, n); }
        intt(x);
        x.resize(size);
        return x;
    }

    F operator-() const {
        F r = *this;
        for (T& x : r) x = -x;
        return r;
    }
    F& operator+=(const F& g) {
        if (this->size() < g.size()) this->resize(g.size());
        for (size_t i = 0; i < g.size(); ++i) (*this)[i] += g[i];
        return *this;
    }
    F& operator-=(const F& g) {
        if (this->size() < g.size()) this->resize(g.size());
        for (size_t i = 0; i < g.size(); ++i) (*this)[i] -= g[i];
        return *this;
    }
    F& operator+=(T x) {
        if (this->empty()) this->resize(1);
        (*this)[0] += x;
        return *this;
    }
    F& operator-=(T x) { return *this += -x; }
    F& operator*=(const F& g) { return *this = multiply(*this, g); }
    F& operator*=(T x) {
        if constexpr (std::is_same_v<T, fps_detail::Mint>) { scale_words(*this, x); return *this; }
        for (T& a : *this) a *= x;
        return *this;
    }
    F& operator/=(const F& g) { return *this = divmod(g).first; }
    F& operator%=(const F& g) { return *this = divmod(g).second; }
    F& operator/=(T x) { return *this *= x.inv(); }

    F& operator<<=(int d) {
        assert(d >= 0);
        const int n = (int)this->size();
        if (d >= n) return *this = F(n);
        this->insert(this->begin(), d, T(0));
        this->resize(n);
        return *this;
    }
    F& operator>>=(int d) {
        assert(d >= 0);
        const int n = (int)this->size();
        if (d >= n) return *this = F(n);
        this->erase(this->begin(), this->begin() + d);
        this->resize(n);
        return *this;
    }

    F operator+(const F& g) const { return F(*this) += g; }
    F operator-(const F& g) const { return F(*this) -= g; }
    F operator*(const F& g) const { return multiply(*this, g); }
    F operator/(const F& g) const { return divmod(g).first; }
    F operator%(const F& g) const { return divmod(g).second; }
    F operator+(T x) const { return F(*this) += x; }
    F operator-(T x) const { return F(*this) -= x; }
    F operator*(T x) const { return F(*this) *= x; }
    F operator/(T x) const { return F(*this) /= x; }
    F operator<<(int d) const { return F(*this) <<= d; }
    F operator>>(int d) const { return F(*this) >>= d; }

    F pre(int n) const {
        assert(n >= 0);
        return F(this->begin(), this->begin() + std::min<int>(n, this->size()));
    }

    F div_series(const F& g, int deg = -1) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(import_work(*this).div_series(import_work(g), deg));
        if (deg < 0) deg = this->size();
        assert(deg >= 0);
        if (!deg) return {};
        assert(!g.empty() && g[0] != T(0));
        if (this->empty()) return F(deg);
        if (deg <= 128 || g.size() < 64) {
            F r = multiply(*this, g.inv(deg)).pre(deg);
            r.resize(deg);
            return r;
        }
        int size = 1;
        while (size < deg) size <<= 1;
        const int block = size / 32, length = 2 * block, count = (deg + block - 1) / block;
        F inverse = g.inv(block), r = multiply(pre(block), inverse).pre(block);
        r.resize(deg);
        inverse.resize(length);
        ntt_half(inverse);
        std::vector<base> denominator(count), quotient(count - 1);
        denominator[0] = g.pre(block);
        denominator[0].resize(length);
        ntt_half(denominator[0]);
        for (int i = 1; i < count; ++i) {
            const int offset = i * block, n = std::min(block, deg - offset);
            auto& h = denominator[i];
            h.resize(length);
            if (offset < (int)g.size()) std::copy_n(g.begin() + offset, std::min<int>(block, g.size() - offset), h.begin());
            ntt_half(h);
            auto& q = quotient[i - 1];
            q.resize(length);
            std::copy_n(r.begin() + offset - block, block, q.begin());
            ntt_half(q);
            fps_detail::NTT::normalize(words(q), length);
            base error(length);
            for(int j=0;j<i;j+=3) {
                const Word* a[3], *b[3], *c[3];
                int terms=std::min(3,i-j);
                for(int k=0;k<terms;++k) {
                    a[k]=words(quotient[j+k]);b[k]=words(denominator[i-j-k]);c[k]=words(denominator[i-j-k-1]);
                }
                auto update = [&](auto number) {
                    constexpr int count=decltype(number)::value;
                    fps_detail::NTT::template accumulate_batch<count>(words(error),a,b,c,block,false);
                    fps_detail::NTT::template accumulate_batch<count>(words(error)+block,a,b,c,block,true);
                };
                if(terms==3) update(std::integral_constant<int,3>{});
                else if(terms==2) update(std::integral_constant<int,2>{});
                else update(std::integral_constant<int,1>{});
            }
            intt(error);
            for (int j = 0; j < block; ++j) error[j] = (offset + j < (int)this->size() ? (*this)[offset + j] : T(0)) - error[j];
            std::fill(error.begin() + block, error.end(), T(0));
            ntt_half(error);
            pointwise(error, error, inverse, length);
            intt(error);
            std::copy_n(error.begin(), n, r.begin() + offset);
        }
        return r;
    }

    F inv(int deg = -1) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(import_work(*this).inv(deg));
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        if (deg == 0) return {};
        assert(!this->empty() && (*this)[0] != T(0));
        F g{(*this)[0].inv()};
        auto terms = sparse_terms(deg, 32);
        if (terms.size() <= 32) {
            g.resize(deg);
            for (int i = 1; i < deg; ++i) {
                for (auto [j, x] : terms) {
                    if (j > i) break;
                    g[i] -= x * g[i - j];
                }
                g[i] *= g[0];
            }
            return g;
        }

        if (deg>256 && this->size()>=64) {
#if defined(__x86_64__) || defined(__i386__)
            if (deg<1024 || !fps_detail::NTT::vectorized()) return F{1}.div_series(*this,deg);
#else
            return F{1}.div_series(*this,deg);
#endif
        }
        g.resize(std::min(deg, 32));
        for (int i = 1; i < (int)g.size(); ++i)
            g[i] =
                -g[0] * dot(this->data() + 1, g.data() + i - 1, std::min<int>(i, this->size() - 1));
        base a,b;
        while ((int)g.size() < deg) {
            const int m = (int)g.size(), n = 2 * m;
            if (deg - m <= 32 && m >= 32) {
                g.resize(deg);
                for (int i = m; i < deg; ++i) {
                    const int count = std::min<int>(i, this->size() - 1);
                    g[i] = -g[0] * dot(this->data() + 1, g.data() + i - 1, count);
                }
                break;
            }
#if defined(__x86_64__) || defined(__i386__)
            if (m>=64 && n <= (1<<23) && fps_detail::NTT::vectorized()) {
                // P has no terms below m. Its cyclic length-m and negacyclic
                // length-2m remainders recover exactly the middle m terms.
                a.assign(this->begin(),this->begin()+std::min<int>(m,this->size()));a.resize(m);
                b.assign(g.begin(),g.end());
                int hi=std::max(0,std::min<int>(m,this->size()-m));
                if(hi) fps_detail::NTT::inverse_fold8(words(a),words(*this)+m,hi);
                transform(std::max(1,m/16)).template inverse_cubic8<false>(words(a),words(b),m);
                g.resize(n);std::copy_n(a.begin(),m,g.begin()+m);
                a.assign(this->begin(),this->begin()+std::min<int>(n,this->size()));a.resize(n);
                b.assign(g.begin(),g.begin()+m);b.resize(n);
                transform(std::max(1,n/16)).template inverse_cubic8<true>(words(a),words(b),n);
                fps_detail::NTT::inverse_recover8(words(g),words(a),m);

            } else
#endif
            {
            base f = this->pre(n), r = g;
            f.resize(n);
            r.resize(n);
            ntt(f);
            ntt_half(r);
            pointwise(f, f, r, n);
            ntt_inverse(f);
            std::fill(f.begin(), f.begin() + m, T(0));
            ntt(f);
            pointwise(f, f, r, n);
            ntt_inverse(f);
            const T inv = T::raw(T::mod() - (T::mod() - 1) / n);
            const T scale = -inv * inv;
            g.resize(n);
            transform(1).scale(words(f) + m, m, scale.val());
            std::copy_n(f.begin() + m, m, g.begin() + m);
            }
        }
        return g.pre(deg);
    }

    F sqrt(int deg = -1) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(import_work(*this).sqrt(deg));
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        F f = pre(deg);
        f.trim();
        int first = 0;
        while (first < (int)f.size() && f[first] == T(0)) ++first;
        if (first == (int)f.size()) return F(deg);
        if (first & 1) return {};
        const T constant = mod_sqrt(f[first]);
        if (constant == T(0)) return {};
        if (first + 1 == (int)f.size()) {
            F result(deg);
            result[first / 2] = constant;
            return result;
        }
        const int shift = first / 2, target = deg - shift;
        f.erase(f.begin(), f.begin() + first);
        f.resize(target);
        const T half = T(2).inv(), scale = half / constant;
        F g(std::min(target, 32));
        g[0] = constant;
        for (int i = 1; i < (int)g.size(); ++i) {
            T sum = 0;
            for (int j = 1; j < i; ++j) sum += g[j] * g[i - j];
            g[i] = (f[i] - sum) * scale;
        }
        if (target > 256) {
            int size = 1;
            while (size < target) size <<= 1;
            const int block = size / 32, length = 2 * block, count = (target + block - 1) / block;
            g = f.pre(block).sqrt(block);
            base inverse = g.inv(block);
            inverse.resize(length); ntt_half(inverse);
            std::vector<base> cache(count - 1);
            base zero(length);
            g.resize(target);
            for (int i = 1; i < count; ++i) {
                int offset = i * block, n = std::min(block, target - offset);
                base& q = cache[i - 1]; q.resize(length);
                std::copy_n(g.begin() + offset - block, block, q.begin()); ntt_half(q); fps_detail::NTT::normalize(words(q), length);
                base error(length);
                for(int j=0;j<i;j+=3) {
                    const Word* a[3], *b[3], *c[3];
                    int terms=std::min(3,i-j);
                    for(int k=0;k<terms;++k) {
                        a[k]=words(cache[j+k]);b[k]=words((j+k ? cache[i-j-k] : zero));c[k]=words(cache[i-j-k-1]);
                    }
                    auto update = [&](auto number) {
                        constexpr int count=decltype(number)::value;
                        fps_detail::NTT::template accumulate_batch<count>(words(error),a,b,c,block,false);
                        fps_detail::NTT::template accumulate_batch<count>(words(error)+block,a,b,c,block,true);
                    };
                    if(terms==3) update(std::integral_constant<int,3>{});
                    else if(terms==2) update(std::integral_constant<int,2>{});
                    else update(std::integral_constant<int,1>{});
                }
                intt(error);
                for (int j = 0; j < block; ++j) error[j] = (offset + j < target ? f[offset + j] : T(0)) - error[j];
                std::fill(error.begin() + block, error.end(), T(0)); ntt_half(error);
                pointwise(error, error, inverse, length); scale_words(error, half); intt(error);
                std::copy_n(error.begin(), n, g.begin() + offset);
            }
            g.insert(g.begin(), shift, T(0));
            return g;
        }
        F h;
        base cache;
        if ((int)g.size() < target) {
            h = g.pre(16).inv(16);
            cache = h;
            cache.resize(32);
            ntt(cache);
        }
        while ((int)g.size() < target) {
            const int m = (int)g.size(), n = 2 * m;
            if (target - m <= 32) {
                g.resize(target);
                for (int i = m; i < target; ++i)
                    g[i] = (f[i] - dot(g.data() + 1, g.data() + i - 1, i - 1)) * scale;
                break;
            }
            assert(n <= (1 << 23));
            const T in = T::raw(T::mod() - (T::mod() - 1) / n);
            base a = g, error(n);
            a.resize(n);
            ntt(a);
            inverse_extend(h, a, cache);
            cache = h;
            cache.resize(n);
            ntt(cache);
            pointwise(error, a, a, n);
            ntt_inverse(error);
            std::fill(error.begin(), error.begin() + m, T(0));
            for (int i = m; i < n; ++i) error[i] = (i < target ? f[i] : T(0)) - error[i] * in;
            ntt(error);
            pointwise(error, error, cache, n);
            F::scale_words(error, half);
            ntt_inverse(error);
            g.resize(std::min(n, target));
            for (int i = m; i < (int)g.size(); ++i) g[i] = error[i] * in;
        }
        g.resize(target);
        g.insert(g.begin(), shift, T(0));
        return g;
    }

    static T bostan_mori(F p, F q, long long k) {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return T::raw(W::bostan_mori(import_work(p), import_work(q), k).val());
        assert(k >= 0 && !q.empty() && q[0] != T(0));
        p.trim();
        q.trim();
        if (p.empty()) return 0;
        auto coefficient = [](const base& p, const base& q, int k) {
            F inverse(q.begin(), q.begin() + std::min<int>(k + 1, q.size()));
            inverse = inverse.inv(k + 1);
            return dot(p.data(), inverse.data() + k, std::min<int>(k + 1, p.size()));
        };
        if (k < 32) return coefficient(p, q, k);
        T answer = 0;
        if (p.size() >= q.size()) {
            auto [quotient, remainder] = p.divmod(q);
            if (k < (long long)quotient.size()) answer = quotient[k];
            p = std::move(remainder);
        }
        if (p.empty()) return answer;
        if (q.size() == 2) return answer + p[0] / q[0] * (-q[1] / q[0]).pow(k);
        const int d = (int)q.size() - 1;
        if (k < d) return answer + coefficient(p, q, k);
        int n = 1;
        while (n < d) n <<= 1;
        assert(n <= (1 << 22));
        const T half = T(2).inv(), inv = T(n).inv();
        T correction = 1;
        const T step = T(3).pow((T::mod() - 1) / (2 * n));
        const auto& roots = inverse_ntt_roots(2 * n);
        base powers(n), twists(n), a = p, b = q;
        for (int i = 0; i < n; ++i) twists[i] = roots[2 * i];
        T lead = q.back();
        std::fill(powers.begin(), powers.end(), T(1));
        geometric_scale(powers, inv, step);
        a.resize(2 * n);
        b.resize(2 * n);
        ntt(a);
        ntt(b);
        auto doubling = [&](base& v, T leading) {
            base odd = v;
            ntt_inverse(odd);
            odd[0] -= T(2 * n) * leading;
            pointwise(odd, odd, powers, n);
            ntt(odd);
            v.insert(v.end(), odd.begin(), odd.end());
        };
        while (k) {
            if constexpr (std::is_same_v<T, fps_detail::Mint>) {
                if (k & 1) fps_detail::NTT::reduce_pairs<true>(words(a), words(b), words(twists), n);
                else fps_detail::NTT::reduce_pairs<false>(words(a), words(b), words(twists), n);
            } else {
                for (int i = 0; i < n; ++i) {
                    T u = a[2 * i] * b[2 * i + 1], v = a[2 * i + 1] * b[2 * i];
                    a[i] = k & 1 ? (u - v) * twists[i] : u + v;
                    b[i] = b[2 * i] * b[2 * i + 1];
                }
            }
            correction *= half;
            a.resize(n);
            b.resize(n);
            lead = (d & 1 ? -lead : lead) * lead;
            k >>= 1;
            if (k < d) break;
            doubling(a, T(0));
            doubling(b, d == n ? lead : T(0));
        }
        if (k) {
            intt(a);
            intt(b);
            if (d == n) b[0] -= lead;
            return answer + correction * coefficient(a, b, k);
        }
        T numerator = 0, denominator = 0;
        for (int i = 0; i < n; ++i) numerator += a[i], denominator += b[i];
        if (d == n) denominator -= T(n) * lead;
        return answer + correction * numerator / denominator;
    }

    static T linear_recurrence(const base& initial, const base& coefficients, long long k) {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return T::raw(W::linear_recurrence(import_work(initial), import_work(coefficients), k).val());
        assert(k >= 0 && initial.size() == coefficients.size());
        const int d = (int)initial.size();
        if (k < d) return initial[k];
        if (d == 0) return 0;
        F q(d + 1);
        q[0] = 1;
        for (int i = 0; i < d; ++i) q[i + 1] = -coefficients[i];
        q.trim();
        if (q.size() == 1) return 0;
        F p = multiply(F(initial), q).pre(d);
        return bostan_mori(std::move(p), std::move(q), k);
    }

    F compose(const F& inner, int deg = -1) const {
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0 && (inner.empty() || inner[0] == T(0)));
        if (deg == 0) return {};
        F f = pre(deg), g = inner.pre(deg);
        f.trim();
        g.trim();
        if (f.size() <= 1 || g.size() <= 1) {
            F result(deg);
            if (!f.empty()) result[0] = f[0];
            return result;
        }
        int first = 1;
        while (g[first] == T(0)) ++first;
        if (first + 1 == (int)g.size()) {
            F result(deg);
            T power = 1;
            for (int i = 0; i < (int)f.size() && (long long)i * first < deg; ++i)
                result[i * first] = f[i] * power, power *= g[first];
            return result;
        }
        if (deg <= 32 || f.size() <= 8) {
            F result;
            for (int i = (int)f.size() - 1; i >= 0; --i) {
                result = multiply(result, g).pre(deg);
                result += f[i];
            }
            result.resize(deg);
            return result;
        }
        int size = 1;
        while (size < deg) size <<= 1;
        assert(size <= (1 << 22));
        CompositionNTT transform(size);
        std::vector<unsigned> p(2 * size), q(size);
        for (int i = 0; i < (int)f.size(); ++i) {
            if constexpr (std::is_same_v<T, fps_detail::Mint>) p[i] = f[i].v;
            else p[i] = transform.in(f[i].val());
        }
        for (int i = 0; i < (int)g.size(); ++i) {
            if constexpr (std::is_same_v<T, fps_detail::Mint>) q[i] = g[i].v;
            else q[i] = transform.in(g[i].val());
        }
        unsigned a = transform.one, b = transform.one;
        transform.compose(p.data(), q.data(), 1, size, a, b);
        F result(deg);
        for (int i = 0; i < deg; ++i) {
            if constexpr (std::is_same_v<T, fps_detail::Mint>) result[i] = T::word(p[i]);
            else result[i] = T::raw(transform.out(p[i]));
        }
        return result;
    }

    T eval(T x) const {
        T ans = 0;
        for (auto it = this->rbegin(); it != this->rend(); ++it) ans = ans * x + *it;
        return ans;
    }

    std::vector<T> eval(const std::vector<T>& xs) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(W(import_work(*this).eval(import_work(xs))));
        if (xs.empty()) return {};
        if (this->size() <= 32 || xs.size() <= 32) {
            base r;
            r.reserve(xs.size());
            for (T x : xs) r.push_back(eval(x));
            return r;
        }
        constexpr int block = 8;
        int length = block;
        while (length < (int)std::max(this->size(), xs.size())) length <<= 1;
        assert(length <= (1 << 23));
        int size = length / block;
        std::vector<base> frequency;
        auto tree = product_tree(xs, size, block, true, &frequency);
        F root = *this;
        root.resize(length);
        std::reverse(root.begin(), root.end());
        root = root.div_series(tree[1], length);
        return evaluate_tree(xs, tree, frequency, size, block, std::move(root), true);
    }
    std::vector<T> multipoint_eval(const std::vector<T>& xs) const { return eval(xs); }

    static F interpolate(const std::vector<T>& xs, const std::vector<T>& ys) {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(W::interpolate(import_work(xs), import_work(ys)));
        assert(xs.size() == ys.size());
        const int n = (int)xs.size();
        if (n == 0) return {};
        constexpr int block = 8;
        int size = 1;
        std::vector<base> frequency;
        auto tree = product_tree(xs, size, block, true, &frequency);
        const int length = size * block;
        F root(length);
        for (int i = 0; i < n; ++i) root[length - n + i] = T(n - i) * tree[1][i];
        auto flip = [&] {
            for (int i = 1; i < 2 * size; ++i) {
                std::reverse(tree[i].begin(), tree[i].end());
                base& a = frequency[i];
                if (a.empty()) continue;
                const int length = (int)a.size(), d = (int)tree[i].size() - 1;
                for (int j = 2; j < length; j <<= 1) std::reverse(a.begin() + j, a.begin() + 2 * j);
                if (d == length / 2) {
                    for (int j = length / 2; j < length; ++j) a[j] = -a[j];
                } else {
                    const auto& roots = ntt_roots(length);
                    base phase(length, T(1));
                    for (int half = 1; half < length; half <<= 1) {
                        const T step = roots[half].pow(d);
                        for (int j = 0; j < half; ++j) phase[half + j] = phase[j] * step;
                    }
                    for (int j = 0; j < length; ++j) a[j] *= phase[j];
                }
            }
        };
        root = root.div_series(tree[1], length);
        base den = evaluate_tree(xs, tree, frequency, size, block, std::move(root));
        flip();
        base prefix(n);
        T product = 1;
        for (int i = 0; i < n; ++i) prefix[i] = product, product *= den[i];
        assert(product != T(0));
        product = product.inv();
        std::vector<F> value(2 * size);
        base weights(n);
        for (int i = n - 1; i >= 0; --i) {
            weights[i] = ys[i] * prefix[i] * product;
            product *= den[i];
        }
        for (int i = 0; i * block < n; ++i) {
            const int d = std::min(block, n - i * block);
            F& f = value[size + i];
            f.resize(d);
            const F& q = tree[size + i];
            for (int j = i * block; j < i * block + d; ++j) {
                T coefficient = 1;
                f[d - 1] += weights[j];
                for (int k = d - 2; k >= 0; --k) {
                    coefficient = q[k + 1] + xs[j] * coefficient;
                    f[k] += weights[j] * coefficient;
                }
            }
        }
        for (int i = size - 1; i > 0; --i) {
            if (value[2 * i + 1].empty()) {
                value[i] = std::move(value[2 * i]);
                continue;
            }
            const int length = (int)frequency[2 * i].size();
            if (length && (int)frequency[2 * i + 1].size() == length) {
                base a = std::move(value[2 * i]), b = std::move(value[2 * i + 1]);
                a.resize(length);
                b.resize(length);
                ntt(a);
                ntt(b);
                if constexpr (std::is_same_v<T, fps_detail::Mint>) fps_detail::NTT::combine(words(a), words(b), words(frequency[2 * i]), words(frequency[2 * i + 1]), length);
                else for (int j = 0; j < length; ++j) a[j] = a[j] * frequency[2 * i + 1][j] + b[j] * frequency[2 * i][j];
                intt(a);
                a.resize(tree[i].size() - 1);
                value[i] = std::move(a);
            } else {
                value[i] = value[2 * i] * tree[2 * i + 1] + value[2 * i + 1] * tree[2 * i];
            }
        }
        return std::move(value[1]);
    }

    void onemul(int d, T c) {
        assert(d >= 0);
        if (d == 0) {
            *this *= T(1) + c;
            return;
        }
        for (int i = (int)this->size() - 1; i >= d; --i) (*this)[i] += (*this)[i - d] * c;
    }
    void onediv(int d, T c) {
        assert(d >= 0);
        if (d == 0) {
            *this /= T(1) + c;
            return;
        }
        for (int i = d; i < (int)this->size(); ++i) (*this)[i] -= (*this)[i - d] * c;
    }

    F diff() const {
        const int n = (int)this->size();
        F r(n);
        if constexpr (std::is_same_v<T, fps_detail::Mint>) {
            if (n > 1) fps_detail::NTT::weighted(words(r), words(*this) + 1, n - 1, 1);
        } else for (int i = 1; i < n; ++i) r[i - 1] = (*this)[i] * i;
        return r;
    }
    F integral() const {
        const int n = (int)this->size();
        assert(n < T::mod());
        F r(n);
        const auto& inv = inverses(n);
        if constexpr (std::is_same_v<T, fps_detail::Mint>) {
            if (n > 1) fps_detail::NTT::product(words(r) + 1, words(*this), words(inv) + 1, n - 1);
        } else for (int i = 1; i < n; ++i) r[i] = (*this)[i - 1] * inv[i];
        return r;
    }

    F log(int deg = -1) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(import_work(*this).log(deg));
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        if (deg == 0) return {};
        assert(!this->empty() && (*this)[0] == T(1));
        // Compute x*f'/f and divide coefficient i by i in place.
        F derivative(std::min<int>(deg, this->size()));
        if (derivative.size()>1) fps_detail::NTT::weighted(words(derivative)+1,words(*this)+1,derivative.size()-1,1);
        F r = derivative.div_series(*this,deg);
        const auto& inv = inverses(deg-1);
        fps_detail::NTT::product(words(r),words(r),words(inv),deg);
        return r;
    }

    F exp(int deg = -1) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(import_work(*this).exp(deg));
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        if (deg == 0) return {};
        assert(this->empty() || (*this)[0] == T(0));
        const auto& iv = inverses(deg);
        auto terms = sparse_terms(deg, 64);
        if (terms.size() <= 64) {
            for (auto& [j, x] : terms) x *= T(j);
            F g(deg);
            g[0] = 1;
            for (int i = 1; i < deg; ++i) {
                for (auto [j, x] : terms) {
                    if (j > i) break;
                    g[i] += x * g[i - j];
                }
                g[i] *= iv[i];
            }
            return g;
        }
        const int initial = std::min(deg, 32);
        F g(initial);
        g[0] = 1;
        for (int i = 1; i < initial; ++i) {
            T sum = 0;
            for (int j = 1; j <= i && j < (int)this->size(); ++j)
                sum += T(j) * (*this)[j] * g[i - j];
            g[i] = sum * iv[i];
        }
        if (initial == deg) return g;
        if (deg > 256) {
            int size = 1;
            while (size < deg) size <<= 1;
            const int block = size / 32, length = 2 * block, count = (deg + block - 1) / block;
            g = pre(block).exp(block);
            base inverse = g.inv(block); inverse.resize(length); ntt_half(inverse);
            std::vector<base> derivative(count), cache(count - 1);
            derivative[0].resize(length);
            const int initial_count = std::min<int>(block, this->size());
            if (initial_count > 1) fps_detail::NTT::weighted(words(derivative[0]) + 1, words(*this) + 1, initial_count - 1, 1);
            ntt_half(derivative[0]);
            g.resize(deg);
            for (int i = 1; i < count; ++i) {
                int offset = i * block, n = std::min(block, deg - offset);
                base& d = derivative[i]; d.resize(length);
                const int derivative_count = std::max(0, std::min<int>(n, (int)this->size() - offset));
                if (derivative_count) fps_detail::NTT::weighted(words(d), words(*this) + offset, derivative_count, offset);
                ntt_half(d);
                base& q = cache[i - 1]; q.resize(length);
                std::copy_n(g.begin() + offset - block, block, q.begin()); ntt_half(q); fps_detail::NTT::normalize(words(q), length);
                base error(length);
                for(int j=0;j<i;j+=3) {
                    const Word* a[3], *b[3], *c[3];
                    int terms=std::min(3,i-j);
                    for(int k=0;k<terms;++k) {
                        a[k]=words(cache[j+k]);b[k]=words(derivative[i-j-k]);c[k]=words(derivative[i-j-k-1]);
                    }
                    auto update = [&](auto number) {
                        constexpr int count=decltype(number)::value;
                        fps_detail::NTT::template accumulate_batch<count>(words(error),a,b,c,block,false);
                        fps_detail::NTT::template accumulate_batch<count>(words(error)+block,a,b,c,block,true);
                    };
                    if(terms==3) update(std::integral_constant<int,3>{});
                    else if(terms==2) update(std::integral_constant<int,2>{});
                    else update(std::integral_constant<int,1>{});
                }
                intt(error); std::fill(error.begin() + block, error.end(), T(0)); ntt_half(error);
                pointwise(error, error, inverse, length); intt(error);
                fps_detail::NTT::product(words(error), words(error), words(iv) + offset, n);
                std::fill(error.begin() + n, error.end(), T(0)); ntt_half(error);
                pointwise(error, error, cache[0], length); intt(error);
                std::copy_n(error.begin(), n, g.begin() + offset);
            }
            return g;
        }
        F h = g.pre(16).inv(16);
        base cache = h;
        cache.resize(32);
        ntt(cache);
        while ((int)g.size() < deg) {
            const int m = (int)g.size(), n = 2 * m;
            if (deg - m <= 32) {
                base derivative(std::min<int>(deg, this->size()));
                for (int i = 1; i < (int)derivative.size(); ++i) derivative[i] = T(i) * (*this)[i];
                derivative.resize(std::max<int>(1, derivative.size()));
                g.resize(deg);
                for (int i = m; i < deg; ++i) {
                    const int count = std::min<int>(i, derivative.size() - 1);
                    g[i] = dot(derivative.data() + 1, g.data() + i - 1, count) * iv[i];
                }
                break;
            }
            assert(n <= (1 << 23));
            const T im = T::raw(T::mod() - (T::mod() - 1) / m);
            const T in = T::raw(T::mod() - (T::mod() - 1) / n);
            base a = g, b(m), error(n);
            a.resize(n);
            ntt(a);
            inverse_extend(h, a, cache);
            cache = h;
            cache.resize(n);
            ntt(cache);
            std::fill(b.begin(), b.end(), T(0));
            for (int i = 1; i < m && i < (int)this->size(); ++i) b[i - 1] = T(i) * (*this)[i];
            ntt(b);
            pointwise(b, b, a, m);
            ntt_inverse(b);
            error[m - 1] = b[m - 1] * im;
            for (int i = 0; i < m - 1; ++i) error[m + i] = b[i] * im - T(i + 1) * g[i + 1];
            ntt(error);
            pointwise(error, error, cache, n);
            ntt_inverse(error);
            for (int i = std::min(n, deg) - 1; i >= m; --i)
                error[i] = (i < (int)this->size() ? (*this)[i] : T(0)) + error[i - 1] * in * iv[i];
            std::fill(error.begin(), error.begin() + m, T(0));
            std::fill(error.begin() + std::min(n, deg), error.end(), T(0));
            ntt(error);
            pointwise(error, error, a, n);
            ntt_inverse(error);
            g.resize(std::min(n, deg));
            for (int i = m; i < (int)g.size(); ++i) g[i] = error[i] * in;
        }
        return g;
    }

    F pow(long long k) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) return export_work(import_work(*this).pow(k));
        assert(k >= 0);
        const int n = (int)this->size();
        if (n == 0) return {};
        if (k == 0) {
            F r(n);
            r[0] = 1;
            return r;
        }
        if (k == 1) return *this;
        if (k <= 4) {
            F square = multiply(*this, *this).pre(n);
            return k == 2 ? square : multiply(square, k == 3 ? *this : square).pre(n);
        }
        int first = 0;
        while (first < n && (*this)[first] == T(0)) ++first;
        if (first == n || first > (n - 1) / k) return F(n);
        const int shift = first * k;
        const int len = n - shift;
        const T lead = (*this)[first];
        F f(this->begin() + first, this->end());
        f.resize(len);
        f /= lead;
        f = f.log(len);
        f *= T(k);
        f = f.exp(len);
        f *= lead.pow(k);
        F r(n);
        for (int i = 0; i < len; ++i) r[shift + i] = f[i];
        return r;
    }

    F shift(T c) {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) { W f = import_work(*this); f.shift(fps_detail::Mint::raw(c.val())); return *this = export_work(f); }
        const int n = (int)this->size();
        if (n == 0) return *this;
        assert(n < T::mod());
        const auto& inv = inverses(n);
        F a(n), b(n);
        T fact = 1, invfact = 1, cpow = 1;
        for (int i = 0; i < n; ++i) {
            a[n - 1 - i] = (*this)[i] * fact;
            b[i] = cpow * invfact;
            fact *= i + 1;
            invfact *= inv[i + 1];
            cpow *= c;
        }
        F prod = multiply(a, b);
        invfact = 1;
        for (int i = 0; i < n; ++i) {
            (*this)[i] = prod[n - 1 - i] * invfact;
            invfact *= inv[i + 1];
        }
        return *this;
    }

    std::pair<F, F> divmod(const F& divisor) const {
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) { auto [q, r] = import_work(*this).divmod(import_work(divisor)); return {export_work(q), export_work(r)}; }
        F a = *this;
        a.trim();
        int m = (int)divisor.size();
        while (m && divisor[m - 1] == T(0)) --m;
        assert(m);
        if ((int)a.size() < m) return {{}, std::move(a)};
        const int qn = (int)a.size() - m + 1;
        if (m <= 2 || (size_t)m * qn <= 4096) {
            F q(qn);
            const T inv = divisor[m - 1].inv();
            for (int i = qn - 1; i >= 0; --i) {
                q[i] = a[i + m - 1] * inv;
                for (int j = 0; j < m; ++j) a[i + j] -= q[i] * divisor[j];
            }
            a.resize(m - 1);
            a.trim();
            q.trim();
            return {std::move(q), std::move(a)};
        }
        F ra = a, rb(divisor.begin(), divisor.begin() + m);
        std::reverse(ra.begin(), ra.end());
        std::reverse(rb.begin(), rb.end());
        F q = ra.pre(qn).div_series(rb, qn);
        std::reverse(q.begin(), q.end());
        std::reverse(rb.begin(), rb.end());
        F product = multiply(rb.pre(m - 1), q.pre(m - 1));
        a.resize(m - 1);
        for (int i = 0; i < std::min<int>(a.size(), product.size()); ++i) a[i] -= product[i];
        a.trim();
        q.trim();
        return {std::move(q), std::move(a)};
    }

   private:
    static W import_work(const base& a) {
        W r(a.size());
        for (int i = 0; i < (int)a.size(); ++i) r[i] = fps_detail::Mint::raw(a[i].val());
        return r;
    }
    static F export_work(const W& a) {
        F r(a.size());
        for (int i = 0; i < (int)a.size(); ++i) r[i] = T::raw(a[i].val());
        return r;
    }
    static fps_detail::NTT& transform(int n) {
        static fps_detail::NTT t(1, false);
        if ((int)t.roots.size() < n) t = fps_detail::NTT(n, false);
        return t;
    }
    using Word = fps_detail::NTT::U;
    static Word* words(base& a) {
        static_assert(sizeof(T) == sizeof(Word) && std::is_trivially_copyable_v<T>);
        return reinterpret_cast<Word*>(a.data());
    }
    static const Word* words(const base& a) { return reinterpret_cast<const Word*>(a.data()); }
    template <bool inverse> static void transform_inplace(base& a) {
        int n = a.size();
        assert(n && !(n & (n - 1)) && n <= (1 << 23));
        auto& t = transform(std::max(1, n / 2));
        t.template fft<inverse>(words(a), n);
        if constexpr (!std::is_same_v<T, fps_detail::Mint>) t.normalize(words(a), n);
    }
    static void ntt(base& a) { transform_inplace<false>(a); }
    // The upper half is zero; the first forward butterfly is just a copy.
    static void ntt_half(base& a) {
        int n = a.size();
        std::copy_n(a.begin(),n/2,a.begin()+n/2);
        transform(std::max(1,n/2)).template fft<false>(words(a),n,n/2);
    }
    static void ntt_inverse(base& a) { transform_inplace<true>(a); }
    static void scale_words(base& a, T factor) {
        if (a.empty()) return;
        if constexpr (std::is_same_v<T, fps_detail::Mint>) {
            auto& t = transform(1);
            t.scale(words(a), a.size(), factor.val());
        } else for (T& x : a) x *= factor;
    }
    static void geometric_scale(base& a, T first, T step) {
        if constexpr (std::is_same_v<T, fps_detail::Mint>) fps_detail::NTT::geometric(words(a), a.size(), first.v, step.v);
        else for (T& x : a) { x *= first; first *= step; }
    }
    static void pointwise(base& r, const base& a, const base& b, int n) {
        if constexpr (std::is_same_v<T, fps_detail::Mint>) fps_detail::NTT::product(words(r), words(a), words(b), n);
        else for (int i = 0; i < n; ++i) r[i] = a[i] * b[i];
    }
    std::vector<std::pair<int, T>> sparse_terms(int deg, int limit) const {
        std::vector<std::pair<int, T>> terms;
        for (int i = 1; i < std::min<int>(deg, this->size()) && (int)terms.size() <= limit; ++i)
            if ((*this)[i] != T(0)) terms.emplace_back(i, (*this)[i]);
        return terms;
    }
    static base evaluate_tree(const base& points, const std::vector<F>& tree,
                              std::vector<base>& frequency, int size, int block, F root, bool consume = false) {
        base result(points.size());
        auto dfs = [&](auto&& self, int node, int l, int r, F cur) -> void {
            if (l * block >= (int)points.size()) return;
            const int length = (r - l) * block, half = length / 2;
            if (r - l == 1 || length <= 8) {
                base polynomial(length);
                for (int i = 0; i < length; ++i)
                    for (int j = 0; j + i < length && j < (int)tree[node].size(); ++j)
                        polynomial[length - 1 - i - j] += cur[i] * tree[node][j];
                for (int i = l * block; i < std::min<int>(r * block, points.size()); ++i) {
                    T value = 0;
                    for (int j = length - 1; j >= 0; --j) value = value * points[i] + polynomial[j];
                    result[i] = value;
                }
                return;
            }
            const int mid = (l + r) / 2;
            if (mid * block >= (int)points.size()) {
                self(self, 2 * node, l, mid, F(cur.begin() + half, cur.end()));
                return;
            }
            F left(half), right(half);
            if (!frequency[2 * node].empty()) {
                ntt(static_cast<base&>(cur));
                base a = consume ? std::move(frequency[2 * node]) : frequency[2 * node];
                base b = consume ? std::move(frequency[2 * node + 1]) : frequency[2 * node + 1];
                pointwise(a, a, cur, length);
                pointwise(b, b, cur, length);
                intt(a);
                intt(b);
                std::copy_n(b.begin() + half, half, left.begin());
                std::copy_n(a.begin() + half, half, right.begin());
            } else {
                F a = multiply(cur, tree[2 * node + 1]), b = multiply(cur, tree[2 * node]);
                std::copy_n(a.begin() + half, half, left.begin());
                std::copy_n(b.begin() + half, half, right.begin());
            }
            self(self, 2 * node, l, mid, std::move(left));
            self(self, 2 * node + 1, mid, r, std::move(right));
        };
        dfs(dfs, 1, 0, size, std::move(root));
        return result;
    }
    static void inverse_extend(F& h, const base& a, const base& cache) {
        const int m = (int)cache.size();
        const T im = T::raw(T::mod() - (T::mod() - 1) / m);
        base b(m);
        pointwise(b, a, cache, m);
        ntt_inverse(b);
        std::fill(b.begin(), b.begin() + m / 2, T(0));
        ntt(b);
        pointwise(b, b, cache, m);
        ntt_inverse(b);
        h.resize(m);
        for (int i = m / 2; i < m; ++i) h[i] = -b[i] * im * im;
    }
    static T dot(const T* a, const T* b, int n) {
        unsigned result = 0;
        for (int i = 0; i < n; i += 16) {
            unsigned long long sum = 0;
            for (int j = i; j < std::min(n, i + 16); ++j)
                sum += (unsigned long long)a[j].val() * b[-j].val();
            result += sum % T::mod();
            if (result >= T::mod()) result -= T::mod();
        }
        return T::raw(result);
    }

    using CompositionNTT = fps_detail::NTT;
    static void intt(base& a) {
        ntt_inverse(a);
        const T inv = T::raw(T::mod() - (T::mod() - 1) / (int)a.size());
        scale_words(a, inv);
    }
    static T mod_sqrt(T a) {
        if (a == T(0) || a.pow((T::mod() - 1) / 2) != T(1)) return 0;
        T r = a.pow(60), t = a.pow(119), c = T(3).pow(119);
        int m = 23;
        while (t != T(1)) {
            int i = 0;
            for (T u = t; u != T(1); u *= u) ++i;
            T b = c.pow(1 << (m - i - 1));
            r *= b;
            c = b * b;
            t *= c;
            m = i;
        }
        return r;
    }
    static const base& inverse_ntt_roots(int n) {
        static base roots{1};
        while ((int)roots.size() < n) {
            const int m = (int)roots.size();
            const T step = T(3).pow((T::mod() - 1) / (2 * m)).inv();
            roots.resize(2 * m);
            for (int i = 0; i < m; ++i) roots[m + i] = roots[i] * step;
        }
        return roots;
    }
    static const base& ntt_roots(int n) {
        static base roots{1};
        while ((int)roots.size() < n) {
            const int m = (int)roots.size();
            const T step = T(3).pow((T::mod() - 1) / (2 * m));
            roots.resize(2 * m);
            for (int i = 0; i < m; ++i) roots[m + i] = roots[i] * step;
        }
        return roots;
    }
    static std::vector<F> product_tree(const std::vector<T>& xs, int& size, int block,
                                       bool reciprocal,
                                       std::vector<std::vector<T>>* frequency = nullptr) {
        const int groups = ((int)xs.size() + block - 1) / block;
        while (size < groups) size <<= 1;
        std::vector<F> tree(2 * size);
        if (frequency) frequency->resize(2 * size);
        for (int i = 0; i < size; ++i) {
            if (i >= groups) {
                tree[size + i] = {1};
                continue;
            }
            F p{1};
            for (int j = i * block; j < std::min<int>((i + 1) * block, xs.size()); ++j) {
                const int d = (int)p.size() - 1;
                if (reciprocal) {
                    p.push_back(0);
                    for (int k = d + 1; k > 0; --k) p[k] -= xs[j] * p[k - 1];
                } else {
                    p.push_back(p.back());
                    for (int k = d; k > 0; --k) p[k] = p[k - 1] - xs[j] * p[k];
                    p[0] *= -xs[j];
                }
            }
            tree[size + i] = std::move(p);
        }
        for (int i = size - 1; i > 0; --i) {
            if (tree[2 * i + 1].size() == 1 && tree[2 * i + 1][0] == T(1)) {
                tree[i] = tree[2 * i];
                continue;
            }
            int depth = 31 - __builtin_clz((unsigned)i);
            int length = (size >> depth) * block;
            if (frequency && length >= 32) {
                auto& a = (*frequency)[2 * i];
                auto& b = (*frequency)[2 * i + 1];
                auto transform = [&](base& a, const F& p) {
                    if (a.empty()) {
                        a.resize(length);
                        std::copy(p.begin(), p.end(), a.begin());
                        ntt(a);
                    } else {
                        const int half = length / 2;
                        base odd(half);
                        std::copy_n(p.begin(), std::min<int>(half, p.size()), odd.begin());
                        if ((int)p.size() > half) odd[0] -= p[half];
                        geometric_scale(odd, T(1), ntt_roots(length)[length / 2]);
                        ntt(odd);
                        a.insert(a.end(), odd.begin(), odd.end());
                    }
                };
                transform(a, tree[2 * i]);
                transform(b, tree[2 * i + 1]);
                std::vector<T> c(length);
                pointwise(c, a, b, length);
                if (i > 1) (*frequency)[i] = c;
                ntt_inverse(c);
                const int n = (int)(tree[2 * i].size() + tree[2 * i + 1].size() - 1);
                tree[i].resize(n);
                const T inv = T::raw(T::mod() - (T::mod() - 1) / length);
                for (int j = 0; j < std::min(n, length); ++j) tree[i][j] = c[j] * inv;
                if (n == length + 1) {
                    const T lead = tree[2 * i].back() * tree[2 * i + 1].back();
                    tree[i][0] -= lead;
                    tree[i][length] = lead;
                }
            } else {
                tree[i] = multiply(tree[2 * i], tree[2 * i + 1]);
            }
        }
        return tree;
    }
    void trim() {
        while (!this->empty() && this->back() == T(0)) this->pop_back();
    }
    static const std::vector<T>& inverses(int n) {
        assert(n < T::mod());
        static std::vector<T> inv{0, 1};
#if defined(__x86_64__) || defined(__i386__)
        if constexpr (std::is_same_v<T, fps_detail::Mint>) {
            int first = inv.size(), count = (n + 1 - first) / 8 * 8;
            if (count >= 256 && fps_detail::NTT::vectorized()) {
                inv.reserve(n + 1);
                inv.resize(first + count);
                fps_detail::NTT::inverse_sequence8(reinterpret_cast<Word*>(inv.data()) + first, first, count);
            }
        }
#endif
        while ((int)inv.size() <= n) {
            const int i = (int)inv.size();
            inv.push_back(-inv[T::mod() % i] * (T::mod() / i));
        }
        return inv;
    }
};

using fps = FormalPowerSeries<fps_mint>;

#if defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma GCC pop_options
#endif

#endif
#ifndef FPS_YOSUPO_IO_HPP
#define FPS_YOSUPO_IO_HPP
#include <charconv>
#if defined(__linux__)
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

struct FastIO {
    static constexpr int capacity = 1 << 16;
    char input[capacity + 32]{}, output[capacity];
    int pos = 0, length = 0, used = 0;
#if defined(__linux__)
    const char* mapped = nullptr;
    std::size_t mapped_size = 0, mapped_pos = 0;
#endif
    FastIO() {
#if defined(__linux__)
        struct stat s;
        int fd = fileno(stdin);
        if (fstat(fd,&s) == 0 && S_ISREG(s.st_mode) && s.st_size > 0) {
            auto offset = ftello(stdin);
            if (offset < 0 || offset >= s.st_size) return;
            void* p = mmap(nullptr,s.st_size,PROT_READ,MAP_PRIVATE,fd,0);
            if (p != MAP_FAILED) { mapped = static_cast<const char*>(p); mapped_size = s.st_size; mapped_pos = offset; }
        }
#endif
    }
    FastIO(const FastIO&) = delete;
    FastIO& operator=(const FastIO&) = delete;
    ~FastIO() {
        flush();
#if defined(__linux__)
        if (mapped) munmap(const_cast<char*>(mapped),mapped_size);
#endif
    }
    void refill() {
        length -= pos;
        std::memmove(input, input + pos, length);
        pos = 0;
        length += std::fread(input + length, 1, capacity - length, stdin);
        std::memset(input + length, 0, 32);
    }
    long long read() {
#if defined(__linux__)
        if (mapped) {
            const char* p = mapped + mapped_pos, *end = mapped + mapped_size;
            while (p < end && *p <= ' ') ++p;
            if (p == end) { mapped_pos = mapped_size; return 0; }
            bool negative = *p == '-'; p += negative;
            unsigned long long x = 0;
            if (end-p >= 24) {
                        static constexpr auto pairs = [] {
            std::array<unsigned char, 65536> values{};
            for (auto& v : values) v = 255;
            for (int a = 0; a < 10; ++a) for (int b = 0; b < 10; ++b)
                values[('0'+a) | (('0'+b)<<8)] = 10*a+b;
            return values;
        }();
        for (;;) {
            unsigned short w; std::memcpy(&w,p,2);
            unsigned y = pairs[w];
            if (y > 99) break;
            x = 100*x+y; p += 2;
        }

                while ('0' <= *p && *p <= '9') x = 10*x+*p++-'0';
            } else {
                while (p < end && '0' <= *p && *p <= '9') x = 10*x+*p++-'0';
            }
            mapped_pos = p-mapped;
            return static_cast<long long>(negative ? 0ULL-x : x);
        }
#endif
        if (length - pos < 32) refill();
        while (input[pos] <= ' ') {
            if (pos == length) { refill(); if (!length) return 0; }
            else ++pos;
        }
        if (length - pos < 24) refill();
        bool negative = input[pos] == '-';
        pos += negative;
        unsigned long long x = 0;
        const char* p = input + pos;
        static constexpr auto pairs = [] {
            std::array<unsigned char, 65536> values{};
            for (auto& v : values) v = 255;
            for (int a = 0; a < 10; ++a) for (int b = 0; b < 10; ++b)
                values[('0'+a) | (('0'+b)<<8)] = 10*a+b;
            return values;
        }();
        for (;;) {
            unsigned short w; std::memcpy(&w,p,2);
            unsigned y = pairs[w];
            if (y > 99) break;
            x = 100*x+y; p += 2;
        }
        while ('0' <= *p && *p <= '9') x = 10 * x + *p++ - '0';
        pos = p - input;
        return static_cast<long long>(negative ? 0ULL - x : x);
    }
    fps poly(int n) {
        fps f(n);
        int i=0;
#if defined(__linux__)
        if (mapped) {
            const char* p=mapped+mapped_pos, *end=mapped+mapped_size;
            static constexpr auto pairs = [] {
                std::array<unsigned char,65536> values{};
                for(auto& v:values) v=255;
                for(int a=0;a<10;++a) for(int b=0;b<10;++b) values[('0'+a) | (('0'+b)<<8)] = 10*a+b;
                return values;
            }();
            for (;i<n && end-p>=24;++i) {
                while(p<end && *p<=' ') ++p;
                if(end-p<24) break;
                unsigned x=*p++-'0';
                unsigned long long w;
                std::memcpy(&w,p,8);
                if (!(((w-0x3030303030303030ULL) | (w+0x4646464646464646ULL)) & 0x8080808080808080ULL)) {
                    w &= 0x0f0f0f0f0f0f0f0fULL;
                    w = (w*10+(w>>8)) & 0x00ff00ff00ff00ffULL;
                    w = (w*100+(w>>16)) & 0x0000ffff0000ffffULL;
                    x = x*100000000+((w*10000+(w>>32))&0xffffffffULL);p+=8;
                } else {
                for (;;) {
                    unsigned short w;std::memcpy(&w,p,2);
                    unsigned y=pairs[w];if(y>99) break;
                    x=100*x+y;p+=2;
                }
                if ('0'<=*p && *p<='9') x=10*x+*p++-'0';
                }
                f[i]=fps_mint::raw(x);
            }
            mapped_pos=p-mapped;
        }
#endif
        for (;i<n;++i) f[i] = fps_mint::raw(read());
        return f;
    }
    fps sparse(int n, int k) {
        fps f(n);
        for (int j = 0; j < k; ++j) {
            int i = read();
            f[i] = fps_mint::raw(read());
        }
        return f;
    }
    void flush() {
        if (used) std::fwrite(output, 1, used, stdout);
        used = 0;
    }
    void write(long long x, char end = '\n') {
        if (used + 32 > capacity) flush();
        if (0 <= x && x < 1000000000) {
            static constexpr auto digits = [] {
                std::array<unsigned,10000> table{};
                for (unsigned i=0;i<10000;++i) table[i] = ('0'+i/1000) | (('0'+i/100%10)<<8)
                    | (('0'+i/10%10)<<16) | (('0'+i%10)<<24);
                return table;
            }();
            unsigned v=x, q4=v/10000, q8=v/100000000;
            if (v>=100000000) {
                output[used++]='0'+q8;
                unsigned a=digits[q4-q8*10000], b=digits[v-q4*10000];
                std::memcpy(output+used,&a,4);std::memcpy(output+used+4,&b,4);used+=8;
            } else {
                unsigned prefix = v>=10000?q4:v;
                int count=1+(prefix>=10)+(prefix>=100)+(prefix>=1000);
                unsigned packed=digits[prefix] >> ((4-count)*8);
                std::memcpy(output+used,&packed,4);used+=count;
                if(v>=10000) {packed=digits[v-q4*10000];std::memcpy(output+used,&packed,4);used+=4;}
            }
            output[used++]=end;
            return;
        }
        char* finish = 0 <= x && x <= UINT_MAX
            ? std::to_chars(output + used, output + capacity, static_cast<unsigned>(x)).ptr
            : std::to_chars(output + used, output + capacity, x).ptr;
        used = finish - output;
        output[used++] = end;
    }
    template <class V>
    void print(const V& a) {
        if (a.empty()) {
            if (used == capacity) flush();
            output[used++] = '\n';
        }
        for (int i = 0; i < (int)a.size(); ++i)
            write(a[i].val(), i + 1 == (int)a.size() ? '\n' : ' ');
    }
};
#endif

int main() {
    FastIO io;
    int n = io.read();
    auto f = io.poly(n);
    auto g = io.poly(n);
    io.print(f.compose(g, n));
}