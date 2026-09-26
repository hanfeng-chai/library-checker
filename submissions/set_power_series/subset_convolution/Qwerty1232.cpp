#include <bits/allocator.h>
#pragma GCC optimize("O3")
#pragma GCC target("avx2,bmi2")

#include <immintrin.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

using u32 = uint32_t;
using u64 = uint64_t;

struct Montgomery {
    u32 mod;    //
    u32 mod2;   // mod * 2
    u32 n_inv;  // n_inv * mod == -1 (mod 2^32)
    u32 r;      // 2^32 % mod
    u32 r2;     // r^2 % mod;

    Montgomery() = default;
    Montgomery(u32 mod) {
        assert(mod % 2);
        assert(mod < (1 << 30));
        this->mod = mod;
        mod2 = 2 * mod;
        n_inv = 1;
        for (int i = 0; i < 5; i++) {
            n_inv *= 2 + n_inv * mod;
        }
        assert(n_inv * mod == u32(-1));
        r = (u64(1) << 32) % mod;
        r2 = u64(r) * r % mod;
    }

    u32 shrink(u32 val) const {
        return std::min(val, val - mod);
    }
    u32 shrink_n(u32 val) const {
        return std::min(val, val + mod);
    }

    // result * 2^32 == val
    template <bool strict = true>
    u32 reduce(u64 val) const {
        u32 res = val + u32(val) * n_inv * u64(mod) >> 32;
        if constexpr (strict) {
            res = shrink(res);
        }
        return res;
    }

    // result * 2^32 == a * b
    template <bool strict = true>
    u32 mul(u32 a, u32 b) const {
        return reduce<strict>(u64(a) * b);
    }
};

using u32x8 = __attribute__((vector_size(32))) u32;
using u64x4 = __attribute__((vector_size(32))) u64;
using i256 = __m256i;

u32x8 load_u32x8(const u32 *data) {
    return u32x8(_mm256_load_si256((i256 *)data));
}
u32x8 load_unaligned_u32x8(const u32 *data) {
    return u32x8(_mm256_loadu_si256((i256 *)data));
}
void store_u32x8(u32 *data, u32x8 vec) {
    _mm256_store_si256((i256 *)data, (i256)vec);
}
void store_unaligned_u32x8(u32 *data, u32x8 vec) {
    _mm256_storeu_si256((i256 *)data, (i256)vec);
}

struct Montgomery_simd {
    u32x8 mod;
    u32x8 mod2;
    u32x8 n_inv;
    u32x8 r, r2;

    Montgomery_simd(u32 mod) {
        Montgomery mt(mod);
        this->mod = (u32x8)_mm256_set1_epi32(mt.mod);
        this->mod2 = (u32x8)_mm256_set1_epi32(mt.mod2);
        this->n_inv = (u32x8)_mm256_set1_epi32(mt.n_inv);
        this->r = (u32x8)_mm256_set1_epi32(mt.r);
        this->r2 = (u32x8)_mm256_set1_epi32(mt.r2);
    }

    u32x8 shrink(u32x8 val) const {
        return (u32x8)_mm256_min_epu32((i256)val, _mm256_sub_epi32((i256)val, (i256)mod));
    }
    u32x8 shrink_n(u32x8 val) const {
        return (u32x8)_mm256_min_epu32((i256)val, _mm256_add_epi32((i256)val, (i256)mod));
    }
    u32x8 shrink2(u32x8 val) const {
        return (u32x8)_mm256_min_epu32((i256)val, _mm256_sub_epi32((i256)val, (i256)mod2));
    }

    template <bool strict = true>
    u32x8 reduce(u64x4 x0246, u64x4 x1357) const {
        u64x4 x0246_ninv = (u64x4)_mm256_mul_epu32((i256)x0246, (i256)n_inv);
        u64x4 x1357_ninv = (u64x4)_mm256_mul_epu32((i256)x1357, (i256)n_inv);
        u64x4 x0246_res =
            (u64x4)_mm256_add_epi64((i256)x0246, _mm256_mul_epu32((i256)x0246_ninv, (i256)mod));
        u64x4 x1357_res =
            (u64x4)_mm256_add_epi64((i256)x1357, _mm256_mul_epu32((i256)x1357_ninv, (i256)mod));
        u32x8 result =
            (u32x8)_mm256_or_si256(_mm256_bsrli_epi128((i256)x0246_res, 4), (i256)x1357_res);
        if constexpr (strict) {
            result = shrink(result);
        }
        return result;
    }

    template <bool strict = true, bool b_use_only_even = false>
    u32x8 mul(u32x8 a, u32x8 b) const {
        u32x8 a_sh = (u32x8)_mm256_bsrli_epi128((i256)a, 4);
        u32x8 b_sh = b_use_only_even ? b : (u32x8)_mm256_bsrli_epi128((i256)b, 4);
        u64x4 x0246 = (u64x4)_mm256_mul_epu32((i256)a, (i256)b);
        u64x4 x1357 = (u64x4)_mm256_mul_epu32((i256)a_sh, (i256)b_sh);
        return reduce<strict>(x0246, x1357);
    }
};

template <typename Functor, size_t... S>
__attribute__((always_inline)) constexpr void static_foreach_seq(Functor function,
                                                                 std::index_sequence<S...>) {
    ((function(std::integral_constant<size_t, S>())), ...);
}

template <size_t Size, typename Functor>
__attribute__((always_inline)) constexpr void static_for(Functor functor) {
    return static_foreach_seq(functor, std::make_index_sequence<Size>());
}

#include <iostream>
#include <map>
#include <sstream>

struct TimerPrint {

    void add(const std::string &key, double dlt) {
        if (!map.contains(key)) {
            map[key] = {(int)map.size(), 0};
        }
        map[key].second += dlt;
    }

    ~TimerPrint() {
        std::vector<std::tuple<int, std::string, double>> vec;
        size_t max_len = 0;
        for (auto &[x, y] : map) {
            max_len = std::max(max_len, x.size());
            vec.push_back({y.first, x, y.second});
        }
        std::sort(vec.begin(), vec.end());
        for (auto [a, b, c] : vec) {
            // std::cerr << std::fixed;
            // std::cerr.precision(3);
            b += ':';
            b.resize(max_len + 1, ' ');

            std::stringstream ss;
            ss << std::fixed;
            ss.precision(3);
            ss << c;
            std::string t = ss.str();
            while (t.size() < 10) {
                t = ' ' + t;
            }

            std::cerr << b << "  " << t << " ms" << std::endl;
        }
    }

    std::map<std::string, std::pair<int, double>> map;
};

TimerPrint printer;

struct Timer {
    Timer(std::string msg) : msg(std::move(msg)) {
        beg = clock();
    }

    double elapsed_ms() {
        double tm = (clock() - beg) * 1.0 / CLOCKS_PER_SEC * 1000;
        return tm;
    }

    // void print() {
    //     std::cerr << msg << ": " << elapsed_ms() << "ms" << std::endl;
    //     print_called = true;
    // }

    ~Timer() {
        printer.add(msg, elapsed_ms());
    }

private:
    clock_t beg;
    std::string msg;
    // bool print_called = false;
};
struct WTF {
    u32 mod;
    Montgomery mt;
    Montgomery_simd mts;

    WTF() = default;
    WTF(u32 mod) : mod(mod), mt(mod), mts(mod) {
        ;
    }

    template <bool inverse = false>
    void SOS(int lg, u32 *data) const {
        // return;
        // const auto mt = this->mt;
        // auto add = [&](u32 a, u32 b) {
        //     return !inverse ? mt.shrink(a + b) : mt.shrink_n(a - b);
        // };
        // for (int k = lg - 1; k >= 0; k--) {
        //     for (int i = 0; i < (1 << lg); i += (1 << k + 1)) {
        //         for (int j = 0; j < (1 << k); j++) {
        //             data[i + (1 << k) + j] = add(data[i + (1 << k) + j], data[i + j]);
        //         }
        //     }
        // }

        const auto mts = this->mts;
        auto add = [&](u32x8 a, u32x8 b) {
            return !inverse ? mts.shrink(a + b) : mts.shrink_n(a - b);
        };
        int k = lg;

#define FUCK(r)                                                                              \
    while (k - r >= 3) {                                                                     \
        k -= r;                                                                              \
        alignas(64) u32x8 dt[1 << r];                                                        \
        for (size_t i = 0; i < (1ULL << lg); i += (1ULL << k + r)) {                         \
            for (size_t j = 0; j < (1ULL << k); j += 8) {                                    \
                static_for<1 << r>(                                                          \
                    [&](auto it) { dt[it] = load_u32x8(data + i + j + (1ULL << k) * it); }); \
                static_for<r>([&](auto k) {                                                  \
                    static_for<1 << r - k - 1>([&](auto i) {                                 \
                        static_for<1 << k>([&](auto j) {                                     \
                            u32x8 a = dt[(i << k + 1) + j],                                  \
                                  b = dt[(i << k + 1) + (1ULL << k) + j];                    \
                            dt[(i << k + 1) + (1ULL << k) + j] = add(b, a);                  \
                        });                                                                  \
                    });                                                                      \
                });                                                                          \
                static_for<1 << r>(                                                          \
                    [&](auto it) { store_u32x8(data + i + j + (1ULL << k) * it, dt[it]); }); \
            }                                                                                \
        }                                                                                    \
        if (lg >= 15) {                                                                      \
            for (int i = 0; i < (1ULL << lg); i += (1ULL << k)) {                            \
                SOS<inverse>(k, data + i);                                                   \
            }                                                                                \
            return;                                                                          \
        }                                                                                    \
    }

        FUCK(3);
        FUCK(2);
        FUCK(1);

#undef FUCK

        for (int i = 0; i < (1 << lg); i += 8) {
            u32x8 val = load_u32x8(data + i);
            val = mts.shrink((u32x8)_mm256_blend_epi32(
                (i256)val, (i256)add(val, (u32x8)_mm256_shuffle_epi32((i256)val, 0b10'11'00'01)),
                0b10'10'10'10));
            val = mts.shrink((u32x8)_mm256_blend_epi32(
                (i256)val, (i256)add(val, (u32x8)_mm256_shuffle_epi32((i256)val, 0b01'00'11'10)),
                0b11'00'11'00));
            val = mts.shrink((u32x8)_mm256_blend_epi32(
                (i256)val,
                (i256)add(val, (u32x8)_mm256_permute2x128_si256((i256)val, (i256)val, 0x01)),
                0b11'11'00'00));

            store_u32x8(data + i, val);
        }
    }

    void conv_h_aux(int lg, int N, const std::vector<u32 *> &vec1,
                    const std::vector<u32 *> &vec2) const {
        assert(N == vec1.size());
        constexpr int G = 1;
        u64x4 *help = (u64x4 *)_mm_malloc((2 * G) * 32 * N, 64);
        for (int i = 0; i < (1 << lg); i += 8 * G) {
            memset(help, 0, (2 * G) * 32 * N);
            for (int x = 1; x <= N; x++) {
                if (x >= 16 && x % 8 == 0) {
                    for (int j = 0; j < (2 * G) * N; j++) {
                        help[j] = (u64x4)mts.shrink2((u32x8)help[j]);
                    }
                }
                for (int y = 1; x + y <= N; y++) {
                    int ind = x + y - 1;
                    u32x8 a = load_u32x8(vec1[x - 1] + i), b = load_u32x8(vec2[y - 1] + i);
                    u32x8 a_sh = (u32x8)_mm256_bsrli_epi128((i256)a, 4),
                          b_sh = (u32x8)_mm256_bsrli_epi128((i256)b, 4);
                    u64x4 c = (u64x4)_mm256_mul_epu32((i256)a, (i256)b),
                          c_sh = (u64x4)_mm256_mul_epu32((i256)a_sh, (i256)b_sh);
                    help[2 * ind] += c;
                    help[2 * ind + 1] += c_sh;
                }
            }
            for (int j = 0; j < (2 * G) * N; j += 2) {
                u64x4 a = (u64x4)mts.shrink2((u32x8)help[j]);
                u64x4 b = (u64x4)mts.shrink2((u32x8)help[j + 1]);
                u32x8 c = mts.shrink(mts.reduce<true>(a, b));
                store_u32x8(vec1[j >> 1] + i, c);
            }
        }
        _mm_free(help);
    }

    // ! ptr1, ptr2 must be 32-byte aligned
    // ! alters ptr1 and ptr2
    void convolve_subset(int lg, u32 *ptr1, u32 *ptr2, u32 *ptr_out) const {
        const int K = std::min(6, lg - 3);
        assert(lg >= 3 && lg >= 3 + K);
        const auto mt = this->mt;
        const auto mts = this->mts;
        u32 *p_out = (u32 *)_mm_malloc(4 << lg, 64);
        std::vector<u32 *> vec1(lg - 1), vec2(lg - 1);
        for (int i = 0; i < lg - 1; i++) {
            vec1[i] = (u32 *)_mm_malloc(4 << lg - K, 64);
            vec2[i] = (u32 *)_mm_malloc(4 << lg - K, 64);
        }

        for (int i = 0; i < (1 << lg); i += 8) {
            store_u32x8(ptr1 + i, mts.mul<true, true>(load_u32x8(ptr1 + i), mts.r2));
            store_u32x8(ptr2 + i, mts.mul<true, true>(load_u32x8(ptr2 + i), mts.r2));
        }
        {
            u32x8 val1 = (u32x8)_mm256_set1_epi32(ptr1[0]);
            u32x8 val2 = (u32x8)_mm256_set1_epi32(ptr2[0]);
            u64x4 sum = (u64x4)_mm256_setzero_si256();
            for (int i = 0; i < (1 << lg); i += 8) {
                int p_cnt = __builtin_popcount(i);
                u32x8 vc1 = load_u32x8(ptr1 + i);
                u32x8 vc2 = load_u32x8(ptr2 + i);
                u32x8 vc3 = load_u32x8(ptr2 + ((1 << lg) - i - 8));
                vc3 = (u32x8)_mm256_permutevar8x32_epi32((i256)vc3,
                                                         _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0));

                u64x4 dlt = (u64x4)_mm256_mul_epu32((i256)vc1, (i256)vc3) +
                            (u64x4)_mm256_mul_epu32(_mm256_bsrli_epi128((i256)vc1, 4),
                                                    _mm256_bsrli_epi128((i256)vc3, 4));
                sum = (u64x4)mts.shrink((u32x8)(sum + dlt));
                store_u32x8(p_out + i, mts.shrink(mts.shrink2(mts.mul<false, true>(vc1, val2) +
                                                              mts.mul<false, true>(vc2, val1))));
            }
            p_out[0] = mt.shrink_n(p_out[0] - mt.mul<true>(val1[0], val2[0]));
            u32x8 sum2 = mts.reduce<true>(sum, sum);
            sum2 = mts.shrink(sum2 + (u32x8)_mm256_permute2x128_si256((i256)sum2, (i256)sum2, 1));
            sum2 = mts.shrink(sum2 + (u32x8)_mm256_bsrli_epi128((i256)sum2, 8));
            u32 sm = sum2[0];
            p_out[(1 << lg) - 1] = sm;
        }
        for (int t = 0; t < (1 << K); t++) {
            {
                Timer _("meow1");

                for (int i = 0; i < lg - 1; i++) {
                    memset(vec1[i], 0, 4 << lg - K);
                    memset(vec2[i], 0, 4 << lg - K);
                }

                for (int t2 = 0; t2 < (1 << K); t2++) {
                    if ((t & t2) != t2) {
                        continue;
                    }
                    for (int i2 = 0; i2 < (1 << lg - K); i2 += 8) {
                        int i = (t2 << lg - K) + i2;
                        int p_cnt = __builtin_popcount(i);
                        u32x8 vc1 = load_u32x8(ptr1 + i);
                        u32x8 vc2 = load_u32x8(ptr2 + i);

                        static_for<4>([&](auto j) {
                            int pc = p_cnt + j;
                            i256 mask = _mm256_setr_epi32(-int(j == 0), -int(j == 1), -int(j == 1),
                                                          -int(j == 2), -int(j == 1), -int(j == 2),
                                                          -int(j == 2), -int(j == 3));
                            if (pc == 0 || pc == lg) {
                                return;
                            }
                            auto my_mask_store = [](u32 *ptr, i256 mask, i256 vec) {
                                i256 vec0 = (i256)load_u32x8(ptr);
                                i256 vec1 = _mm256_blendv_epi8(vec0, vec, mask);
                                store_u32x8(ptr, (u32x8)vec1);
                            };
                            // _mm256_maskstore_epi32
                            my_mask_store((vec1[pc - 1] + i2), mask,
                                          (i256)mts.shrink(load_u32x8(vec1[pc - 1] + i2) + vc1));
                            // _mm256_maskstore_epi32
                            my_mask_store((vec2[pc - 1] + i2), mask,
                                          (i256)mts.shrink(load_u32x8(vec2[pc - 1] + i2) + vc2));
                        });
                    }
                }
            }
            {
                Timer t("transorm forward");
                for (int i = 0; i < lg - 2; i++) {
                    SOS<false>(lg - K, vec1[i]);
                    SOS<false>(lg - K, vec2[i]);
                }
            }
            {
                Timer t("convolve");
                conv_h_aux(lg - K, lg - 1, vec1, vec2);
            }
            {
                Timer t("transorm inverse");

                for (int i = 1; i < lg - 1; i++) {
                    SOS<true>(lg - K, vec1[i]);
                }
            }
            {

                Timer hfdkjhsdfkhsdfakjasdf("meow2");

                for (int t2 = 0; t2 < (1 << K); t2++) {
                    if ((t & t2) != t) {
                        continue;
                    }
                    int sgn = __builtin_popcount(t ^ t2) & 1;
                    for (int i2 = 0; i2 < (1 << lg - K); i2 += 8) {
                        int i = (t2 << lg - K) + i2;
                        int p_cnt = __builtin_popcount(i);
                        u32x8 val = load_u32x8(p_out + i);
                        static_for<4>([&](auto j) {
                            int pc = p_cnt + j;
                            i256 mask = _mm256_setr_epi32(-int(j == 0), -int(j == 1), -int(j == 1),
                                                          -int(j == 2), -int(j == 1), -int(j == 2),
                                                          -int(j == 2), -int(j == 3));
                            if (pc == 0 || pc == lg) {
                                return;
                            }
                            u32x8 dlt = load_u32x8(vec1[pc - 1] + i2) & (u32x8)mask;
                            val = sgn ? mts.shrink_n(val - dlt) : mts.shrink(val + dlt);
                        });
                        store_u32x8(p_out + i, val);
                    }
                }
            }
        }
        for (int i = 0; i < lg - 1; i++) {
            _mm_free(vec1[i]);
            _mm_free(vec2[i]);
        }
        for (int i = 0; i < (1 << lg); i += 8) {
            store_unaligned_u32x8(ptr_out + i, mts.mul<true, true>(load_u32x8(p_out + i),
                                                                   (u32x8)_mm256_set1_epi32(1)));
        }
        _mm_free(p_out);
    }
};

#include <sys/mman.h>
#include <sys/stat.h>

#include <algorithm>
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
    ~Qinf() {
        munmap(bg, Fl.st_size + 1);
    }
    void skip_space() {
        while (*p <= ' ') {
            ++p;
        }
    }
    char get() {
        return *p++;
    }
    char seek() {
        return *p;
    }
    bool eof() {
        return p == ed;
    }
    Qinf &read(char *s, size_t count) {
        return memcpy(s, p, count), p += count, *this;
    }
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
    Qoutf(FILE *fo, size_t sz = O_buffer_default_size)
        : f(fo),
          bg(new char[sz]),
          ed(bg + sz),
          p(bg),
          ed_thre(ed - O_buffer_default_flush_threshold),
          fp(6),
          _fpi(1000000ull) {
    }
    void flush() {
        fwrite_unlocked(bg, 1, p - bg, f), p = bg;
    }
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
    Qoutf &operator<<(char ch) {
        return *p++ = ch, *this;
    }
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

// #include "subset.hpp"

// #define qin std::cin
// #define qout std::cout

int32_t main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int mod = 998'244'353;
    WTF wtf(mod);

    int n;
    qin >> n;
    int lg = std::max(3, n);

    u32 *a = (u32 *)_mm_malloc(4 << lg, 64);
    u32 *b = (u32 *)_mm_malloc(4 << lg, 64);

    for (int i = 0; i < (1 << n); i++) {
        qin >> a[i];
    }
    for (int i = 0; i < (1 << n); i++) {
        qin >> b[i];
    }
    memset(a + (1 << n), 0, (4 << lg) - (4 << n));
    memset(b + (1 << n), 0, (4 << lg) - (4 << n));
    {
        Timer t("work total");
        wtf.convolve_subset(lg, a, b, a);
    }

    for (int i = 0; i < (1 << n); i++) {
        qout << a[i] << " \n"[i == (1 << n) - 1];
    }

    _mm_free(a);
    _mm_free(b);
}
