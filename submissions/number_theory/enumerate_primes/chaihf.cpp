// BEGIN bundled: problems/enumerate_primes/main.cpp
// BEGIN bundled: include/toy/io.hpp

// Linux x86-64 / GCC / C++23 only. Reader APIs encode token shape so fixed,
// bounded, full-range, and 128-bit inputs use independently benchmarked paths.
// Writer uses base-10^4 groups; signed 128-bit output is split at 10^19 so no
// compiler __divti3/__modti3 helper remains in the hot loop.
#include <bits/extc++.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef NDEBUG
#define toy_assert(expr) [[assume(expr)]]
#else
#define toy_assert(expr) assert(expr)
#endif

namespace toy {

using namespace std;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;
using isize = intptr_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;
using usize = uintptr_t;
using f32 = float;
using f64 = double;
using f80 = long double;

namespace detail {

inline void write_all(const char* p, usize n) {
    while (n) {
        isize z = ::write(1, p, n);
        if (z > 0) p += z, n -= z;
        else if (z < 0 && errno == EINTR) continue;
        else break;
    }
}

// High half of a 128x128 product, assembled from four 64x64 products.
inline u128 mulhi(u128 a, u128 b) {
    u64 al = (u64)a, ah = (u64)(a >> 64);
    u64 bl = (u64)b, bh = (u64)(b >> 64);
    u128 ll = (u128)al * bl, lh = (u128)al * bh;
    u128 hl = (u128)ah * bl, hh = (u128)ah * bh;
    u128 mid = (ll >> 64) + (u64)lh + (u64)hl;
    return hh + (lh >> 64) + (hl >> 64) + (mid >> 64);
}

// q=floor(x/10^19) via ceil(2^192/10^19); r=x-q*10^19.
inline pair<u64, u64> divmod_1e19(u128 x) {
    constexpr u64 B = 10'000'000'000'000'000'000ULL;
    constexpr u128 M = ((u128)0xd83c94fb6d2ac34aULL << 64) |
                       0x5663d3c7a0d865cbULL;
    if (x < B) return {0, (u64)x};
    u128 h = mulhi(x, M), s = x + h;
    u64 q = (u64)((s >> 64) + (s < x));
    return {q, (u64)(x - (u128)q * B)};
}

} // namespace detail

class Reader {
    const char* p;

    static const char* map_stdin() {
        struct stat st{};
        fstat(0, &st);
        usize page = (usize)sysconf(_SC_PAGESIZE);
        usize size = (usize)st.st_size;
        usize mapped = (size + page - 1) & -page;
        void* base = mmap(nullptr, mapped + page, PROT_READ,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        toy_assert(base != MAP_FAILED);
        if (size) {
            void* file = mmap(base, size, PROT_READ,
                              MAP_PRIVATE | MAP_FIXED, 0, 0);
            toy_assert(file == base);
        }
        return (const char*)base;
    }

    static const array<array<char, 16>, 17>& masks() {
        static constexpr auto a = [] {
            array<array<char, 16>, 17> result{};
            for (int n = 0; n <= 16; ++n)
                for (int i = 0; i < 16; ++i)
                    result[n][i] = i < 16 - n ? (char)0x80 : (char)(i - 16 + n);
            return result;
        }();
        return a;
    }

    static u64 parse16(__m128i x) {
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010a));
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));
        __m128i a = _mm_mul_epu32(x, _mm_set_epi32(1, 10000, 1, 10000));
        x = _mm_add_epi64(a, _mm_srli_epi64(x, 32));
        return (u64)_mm_cvtsi128_si64(x) * 100000000ULL +
               (u64)_mm_extract_epi64(x, 1);
    }

    static bool all_digit(u64 x) {
        return !(((x ^ 0x3030303030303030ULL) & 0xf0f0f0f0f0f0f0f0ULL));
    }

    static u64 parse8(u64 x) {
        x ^= 0x3030303030303030ULL;
        x = (x * 10 + (x >> 8)) & 0x00ff00ff00ff00ffULL;
        x = (x * 100 + (x >> 16)) & 0x0000ffff0000ffffULL;
        return (x * 10000 + (x >> 32)) & 0x00000000ffffffffULL;
    }

    static u128 parse32(__m256i x) {
        x = _mm256_maddubs_epi16(x, _mm256_set1_epi16(0x010a));
        x = _mm256_madd_epi16(x, _mm256_set1_epi32(0x00010064));
        __m256i a = _mm256_mul_epu32(
            x, _mm256_set_epi32(1, 10000, 1, 10000, 1, 10000, 1, 10000));
        x = _mm256_add_epi64(a, _mm256_srli_epi64(x, 32));
        __m128i a0 = _mm256_castsi256_si128(x);
        __m128i a1 = _mm256_extracti128_si256(x, 1);
        u64 hi = (u64)_mm_cvtsi128_si64(a0) * 100000000ULL +
                 (u64)_mm_extract_epi64(a0, 1);
        u64 lo = (u64)_mm_cvtsi128_si64(a1) * 100000000ULL +
                 (u64)_mm_extract_epi64(a1, 1);
        return (u128)hi * 10000000000000000ULL + lo;
    }

    static const array<u64, 17>& powers10() {
        static constexpr auto a = [] {
            array<u64, 17> result{1};
            for (int i = 1; i <= 16; ++i) result[i] = result[i - 1] * 10;
            return result;
        }();
        return a;
    }

    [[gnu::always_inline]] static u64 parse_short16(__m128i x, u32 boundary,
                                                     int& digits) {
        digits = __builtin_ctz(boundary);
        x = _mm_shuffle_epi8(
            x, _mm_loadu_si128((const __m128i*)masks()[digits].data()));
        return parse16(x);
    }

    [[gnu::always_inline]] u64 read_swar_u64() {
        const char* q = p;
        u64 a, b;
        memcpy(&a, q, 8); memcpy(&b, q + 8, 8);
        if (all_digit(a)) {
            u64 v = parse8(a); q += 8;
            if (all_digit(b)) v = v * 100000000ULL + parse8(b), q += 8;
            while (*q >= '0') v = v * 10 + *q++ - '0';
            p = q + 1;
            return v;
        }
        u64 v = 0;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        p = q + 1;
        return v;
    }

    [[gnu::always_inline]] u64 read_scalar_u64() {
        u64 v = 0;
        while (*p >= '0') v = v * 10 + *p++ - '0';
        ++p;
        return v;
    }

    [[gnu::always_inline]] u64 read_simd_u64() {
        const char* q = p;
        __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                                  _mm_set1_epi8('0'));
        u32 boundary = (u32)_mm_movemask_epi8(x);
        if (__builtin_expect(boundary != 0, 1)) {
            int digits = __builtin_ctz(boundary);
            x = _mm_shuffle_epi8(
                x, _mm_loadu_si128((const __m128i*)masks()[digits].data()));
            p = q + digits + 1;
            return parse16(x);
        }
        u64 v = parse16(x); q += 16;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        p = q + 1;
        return v;
    }

    [[gnu::always_inline]] static u128 read_avx_u128(const char*& cursor) {
        const char* q = cursor;
        u128 v;
        __m256i x = _mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)q),
                                     _mm256_set1_epi8('0'));
        __m256i hi = _mm256_and_si256(x, _mm256_set1_epi8((char)0xf0));
        u32 bad = ~((u32)_mm256_movemask_epi8(
            _mm256_cmpeq_epi8(hi, _mm256_setzero_si256())));
        if (!bad) {
            v = parse32(x); q += 32;
            u64 tail = 0, scale = 1;
            while (*q >= '0') {
                tail = tail * 10 + *q++ - '0';
                scale *= 10;
            }
            v = v * scale + tail;
            cursor = q + 1;
            return v;
        }
        int n = __builtin_ctz(bad);
        if (n) {
            if (n <= 16) {
                __m128i lo = _mm256_castsi256_si128(x);
                lo = _mm_shuffle_epi8(
                    lo, _mm_loadu_si128((const __m128i*)masks()[n].data()));
                v = parse16(lo);
            } else {
                alignas(32) u8 b[64]{};
                _mm256_storeu_si256((__m256i*)(b + 32 - n), x);
                v = parse32(_mm256_load_si256((const __m256i*)b));
            }
            cursor = q + n + 1;
            return v;
        }
        v = 0;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        cursor = q + 1;
        return v;
    }

    [[gnu::always_inline]] static u128 read_staged_u128(const char*& cursor) {
        const char* q = cursor;
        __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                                  _mm_set1_epi8('0'));
        u32 boundary = (u32)_mm_movemask_epi8(x);
        if (boundary) {
            int digits;
            u128 value = parse_short16(x, boundary, digits);
            cursor = q + digits + 1;
            return value;
        }

        u128 value = parse16(x);
        q += 16;
        x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                         _mm_set1_epi8('0'));
        boundary = (u32)_mm_movemask_epi8(x);
        if (boundary) {
            int digits;
            u64 tail = parse_short16(x, boundary, digits);
            cursor = q + digits + 1;
            return value * powers10()[digits] + tail;
        }

        value = value * 10000000000000000ULL + parse16(x);
        q += 16;
        x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                         _mm_set1_epi8('0'));
        boundary = (u32)_mm_movemask_epi8(x);
        int digits;
        u64 tail = parse_short16(x, boundary, digits);
        cursor = q + digits + 1;
        return value * powers10()[digits] + tail;
    }

    template<int Digits, class U>
    [[gnu::always_inline]] U read_fixed_unsigned() {
        static_assert(1 <= Digits && Digits <= 37);
        U v;
        if constexpr (Digits <= 7) {
            v = 0;
            for (int i = 0; i < Digits; ++i) v = v * 10 + p[i] - '0';
        } else if constexpr (Digits == 8) {
            u64 x;
            memcpy(&x, p, 8);
            v = (U)parse8(x);
        } else if constexpr (Digits <= 16) {
            __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p),
                                      _mm_set1_epi8('0'));
            x = _mm_shuffle_epi8(
                x, _mm_loadu_si128((const __m128i*)masks()[Digits].data()));
            v = (U)parse16(x);
        } else if constexpr (Digits < 32) {
            if constexpr (Digits >= 24) {
                v = 0;
                int offset = 0;
                for (; offset + 8 <= Digits; offset += 8) {
                    u64 block;
                    memcpy(&block, p + offset, 8);
                    v = v * 100000000 + parse8(block);
                }
                for (; offset < Digits; ++offset) v = v * 10 + p[offset] - '0';
            } else {
                __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p),
                                          _mm_set1_epi8('0'));
                v = (U)parse16(x);
                for (int i = 16; i < Digits; ++i) v = v * 10 + p[i] - '0';
            }
        } else {
            __m256i x = _mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)p),
                                         _mm256_set1_epi8('0'));
            v = (U)parse32(x);
            for (int i = 32; i < Digits; ++i) v = v * 10 + p[i] - '0';
        }
        p += Digits + 1;
        return v;
    }

    template<class T>
    [[gnu::always_inline]] T read_signed(auto read_unsigned) {
        bool neg = *p == '-'; p += neg;
        auto v = (this->*read_unsigned)();
        using U = make_unsigned_t<T>;
        U magnitude = (U)v;
        return neg ? (T)(U(0) - magnitude) : (T)magnitude;
    }

public:
    Reader(): p(map_stdin()) {}
    explicit Reader(const char* input): p(input) {}

    [[gnu::always_inline]] u32 read_digit() {
        u32 value = (u32)(*p - '0');
        p += 2;
        return value;
    }

    [[gnu::always_inline]] u32 read_u32() { return (u32)read_swar_u64(); }
    [[gnu::always_inline]] i32 read_i32() { return read_signed<i32>(&Reader::read_simd_u64); }
    [[gnu::always_inline]] u64 read_u64() { return read_simd_u64(); }
    [[gnu::always_inline]] i64 read_i64() { return read_signed<i64>(&Reader::read_simd_u64); }
    [[gnu::always_inline]] u128 read_u128() { return read_avx_u128(p); }
    [[gnu::always_inline]] i128 read_i128() {
        bool neg = *p == '-'; p += neg;
        u128 value = read_avx_u128(p);
        return neg ? (i128)(u128(0) - value) : (i128)value;
    }
    [[gnu::always_inline]] i128 read_staged_i128() {
        bool neg = *p == '-'; p += neg;
        u128 value = read_staged_u128(p);
        return neg ? (i128)(u128(0) - value) : (i128)value;
    }

    template<int Digits, class T = u64>
    [[gnu::always_inline]] T read_fixed() {
        static_assert(is_integral_v<T> || same_as<T, i128> || same_as<T, u128>);
        if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value = read_fixed_unsigned<Digits, u128>();
            return neg ? (i128)(u128(0) - value) : (i128)value;
        } else if constexpr (same_as<T, u128>) {
            return read_fixed_unsigned<Digits, u128>();
        } else if constexpr (is_signed_v<T>) {
            bool neg = *p == '-'; p += neg;
            using U = make_unsigned_t<T>;
            U value = read_fixed_unsigned<Digits, U>();
            return neg ? (T)(U(0) - value) : (T)value;
        } else {
            return read_fixed_unsigned<Digits, T>();
        }
    }

    template<int MaxDigits, class T = u64>
    [[gnu::always_inline]] T read_var() {
        static_assert(1 <= MaxDigits && MaxDigits <= 37);
        if constexpr (same_as<T, u128>) {
            if constexpr (MaxDigits <= 19) return (u128)read_simd_u64();
            else return read_u128();
        } else if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value;
            if constexpr (MaxDigits <= 18) value = read_simd_u64();
            else value = read_avx_u128(p);
            return neg ? (i128)(u128(0) - value) : (i128)value;
        }
        else if constexpr (is_signed_v<T>) {
            if constexpr (MaxDigits <= 2)
                return read_signed<T>(&Reader::read_scalar_u64);
            else return read_signed<T>(&Reader::read_simd_u64);
        } else {
            if constexpr (MaxDigits <= 2) return (T)read_scalar_u64();
            else return (T)read_simd_u64();
        }
    }

    template<int MaxDigits, class T = u64>
    [[gnu::always_inline]] T read_uniform() {
        static_assert(1 <= MaxDigits && MaxDigits <= 37);
        if constexpr (same_as<T, u128>) {
            if constexpr (MaxDigits <= 19) return (u128)read_simd_u64();
            else if constexpr (32 <= MaxDigits && MaxDigits <= 34)
                return read_avx_u128(p);
            else return read_staged_u128(p);
        } else if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value;
            if constexpr (MaxDigits <= 18) value = read_simd_u64();
            else if constexpr (32 <= MaxDigits && MaxDigits <= 36)
                value = read_avx_u128(p);
            else value = read_staged_u128(p);
            return neg ? (i128)(u128(0) - value) : (i128)value;
        } else {
            bool neg = false;
            if constexpr (is_signed_v<T>) neg = *p == '-', p += neg;
            u64 value;
            if constexpr (is_signed_v<T>) {
                if constexpr (MaxDigits <= 7) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 13) value = read_swar_u64();
                else value = read_simd_u64();
            } else {
                if constexpr (MaxDigits <= 7) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 12) value = read_swar_u64();
                else value = read_simd_u64();
            }
            using U = make_unsigned_t<T>;
            U magnitude = (U)value;
            if constexpr (is_signed_v<T>)
                return neg ? (T)(U(0) - magnitude) : (T)magnitude;
            else return (T)magnitude;
        }
    }

    template<u32 Mod>
    [[gnu::always_inline]] u32 read_mod() {
        u32 value = read_u32();
        toy_assert(value < Mod);
        return value;
    }

    template<class T = u32>
    [[gnu::always_inline]] T read_index(T size) {
        T value = read<T>();
        toy_assert(value < size);
        return value;
    }

    string_view read_token() {
        const char* begin = p;
        while (*p > ' ') ++p;
        string_view token(begin, p);
        ++p;
        return token;
    }

    template<class T> [[gnu::always_inline]] T read() {
        if constexpr (same_as<T, i128>) return read_i128();
        else if constexpr (same_as<T, u128>) return read_u128();
        else if constexpr (is_signed_v<T> && sizeof(T) <= 4) return (T)read_i32();
        else if constexpr (is_signed_v<T>) return (T)read_i64();
        else if constexpr (sizeof(T) <= 4) return (T)read_u32();
        else return (T)read_u64();
    }
};

template<usize N = 1 << 19, bool Compact = false>
class Writer {
    alignas(64) array<char, N> storage;
    char* begin = storage.data();
    char* p = begin;
    char* end = begin + N;

    static constexpr u32 packed_digits(int x) {
        return (u32)('0' + x / 1000) |
               (u32)('0' + x / 100 % 10) << 8 |
               (u32)('0' + x / 10 % 10) << 16 |
               (u32)('0' + x % 10) << 24;
    }

    static const array<u32, 10000>& lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) result[x] = packed_digits(x);
            return result;
        }();
        return a;
    }

    static const array<u32, 10000>& first_lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) {
                u32 value = packed_digits(x);
                if (x < 1000) value = (value & 0xffffff00U) | ' ';
                if (x < 100) value = (value & 0xffff00ffU) | (u32)' ' << 8;
                if (x < 10) value = (value & 0xff00ffffU) | (u32)' ' << 16;
                if (x == 0) value = (value & 0x00ffffffU) | (u32)' ' << 24;
                result[x] = value;
            }
            return result;
        }();
        return a;
    }

    static const array<u32, 10000>& negative_lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) {
                u32 value = packed_digits(x);
                if (x < 1000) value = (value & 0xffffff00U) | ' ';
                if (x < 100) value = (value & 0xffff00ffU) | (u32)' ' << 8;
                if (x < 10) value = (value & 0xff00ffffU) | (u32)' ' << 16;
                unsigned offset = x >= 100 ? 0 : x >= 10 ? 8 : x ? 16 : 24;
                result[x] = (value & ~(0xffU << offset)) | (u32)'-' << offset;
            }
            return result;
        }();
        return a;
    }

    template<u64 Magic, int Shift>
    [[gnu::always_inline]] static u64 reciprocal_div(u64 x) {
        return (u64)(((u128)x * Magic) >> 64) >> Shift;
    }

    [[gnu::always_inline]] static u64 div_1e4(u64 x) {
        return reciprocal_div<0x346dc5d63886594bULL, 11>(x);
    }

    [[gnu::always_inline]] static u64 div_1e8(u64 x) {
        return reciprocal_div<0xabcc77118461cefdULL, 26>(x);
    }

    [[gnu::always_inline]] static u64 div_1e12(u64 x) {
        return reciprocal_div<0x232f33025bd42233ULL, 37>(x);
    }

    [[gnu::always_inline]] static u64 div_1e16(u64 x) {
        return reciprocal_div<0x39a5652fb1137857ULL, 51>(x);
    }

    [[gnu::always_inline]] void ensure(usize n) {
        if ((usize)(end - p) < n) flush();
    }

    [[gnu::always_inline]] static void group(char*& cursor, u32 s) {
        memcpy(cursor, &s, 4); cursor += 4;
    }

    [[gnu::always_inline]] static void first_group(char*& cursor, u64 x) {
        if constexpr (Compact) {
            unsigned skip = 3 - (x >= 10) - (x >= 100) - (x >= 1000);
            u32 s = lut()[x] >> (skip * 8);
            memcpy(cursor, &s, 4);
            cursor += 4 - skip;
        } else {
            group(cursor, first_lut()[x]);
        }
    }

    [[gnu::always_inline]] static void u64_raw(char*& cursor, u64 x) {
        const auto& table = lut();
        if constexpr (!Compact) {
            if (x >= 1'000'000'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                u64 middle = high % 10000;
                u64 top = high / 10000;
                first_group(cursor, top / 10000);
                group(cursor, table[top % 10000]);
                group(cursor, table[middle]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 100'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                first_group(cursor, high / 10000);
                group(cursor, table[high % 10000]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 10'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                first_group(cursor, x / 100'000'000ULL);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 1000ULL) {
                first_group(cursor, x / 10000);
                group(cursor, table[x % 10000]);
            } else if (x) {
                first_group(cursor, x);
            } else {
                group(cursor, (u32)' ' | (u32)' ' << 8 |
                              (u32)' ' << 16 | (u32)'0' << 24);
            }
        } else if (x > 9999'9999'9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            u64 q3 = div_1e12(x), q4 = div_1e16(x);
            first_group(cursor, q4);
            group(cursor, table[q3 - q4 * 10000]);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999'9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            first_group(cursor, q3);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            first_group(cursor, q2);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999ULL) {
            u64 q1 = div_1e4(x);
            first_group(cursor, q1);
            group(cursor, table[x - q1 * 10000]);
        } else {
            first_group(cursor, x);
        }
    }

    [[gnu::always_inline]] static void i64_raw(char*& cursor, u64 x) {
        if constexpr (Compact) {
            *cursor++ = '-';
            u64_raw(cursor, x);
        } else {
            const auto& next = lut();
            const auto& negative = negative_lut();
            if (x >= 10'000'000'000'000'000'000ULL) {
                *cursor++ = '-'; u64_raw(cursor, x);
            } else if (x > 999'9999'9999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x);
                u64 q3 = div_1e12(x), q4 = div_1e16(x);
                group(cursor, negative[q4]);
                group(cursor, next[q3 - q4 * 10000]);
                group(cursor, next[q2 - q3 * 10000]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999'9999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
                group(cursor, negative[q3]);
                group(cursor, next[q2 - q3 * 10000]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x);
                group(cursor, negative[q2]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999ULL) {
                u64 q1 = div_1e4(x);
                group(cursor, negative[q1]);
                group(cursor, next[x - q1 * 10000]);
            } else {
                group(cursor, negative[x]);
            }
        }
    }

    [[gnu::always_inline]] static void fixed19(char*& cursor, u64 x) {
        const auto& next = lut();
        u64 q1 = div_1e4(x), q2 = div_1e8(x);
        u64 q3 = div_1e12(x), q4 = div_1e16(x);
        u32 first = next[q4] >> 8;
        memcpy(cursor, &first, 4); cursor += 3;
        group(cursor, next[q3 - q4 * 10000]);
        group(cursor, next[q2 - q3 * 10000]);
        group(cursor, next[q1 - q2 * 10000]);
        group(cursor, next[x - q1 * 10000]);
    }

public:
    ~Writer() { flush(); }
    void flush() { detail::write_all(begin, p - begin); p = begin; }
    void put(char c) {
        ensure(1);
        *p++ = c;
    }
    void write(string_view s) {
        while (!s.empty()) {
            usize n = min<usize>(s.size(), end - p);
            if (!n) {
                flush();
                continue;
            }
            memcpy(p, s.data(), n);
            p += n;
            s.remove_prefix(n);
        }
    }

    [[gnu::always_inline]] void writeln(u64 x) {
        ensure(24);
        char* cursor = p;
        u64_raw(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    [[gnu::always_inline]] void write_token(u64 x) {
        ensure(24);
        char* cursor = p;
        if constexpr (Compact) {
            *cursor++ = ' ';
        } else if (x >= 10'000'000'000'000'000'000ULL) {
            *cursor++ = ' ';
        }
        u64_raw(cursor, x);
        p = cursor;
    }
    template<u64 Max>
    [[gnu::always_inline]] void write_token_bounded(u64 x) {
        static_assert(Max < 10'000'000'000'000'000'000ULL);
        toy_assert(x <= Max);
        ensure(24);
        char* cursor = p;
        if constexpr (Compact) *cursor++ = ' ';
        u64_raw(cursor, x);
        p = cursor;
    }
    [[gnu::always_inline]] void writeln(i128 x) {
        ensure(48); bool neg = x < 0; u128 v = neg ? u128(0) - (u128)x : (u128)x;
        auto [q, r] = detail::divmod_1e19(v);
        char* cursor = p;
        if (neg) {
            if (q) i64_raw(cursor, q), fixed19(cursor, r); else i64_raw(cursor, r);
        } else {
            if (q) u64_raw(cursor, q), fixed19(cursor, r); else u64_raw(cursor, r);
        }
        *cursor++ = '\n';
        p = cursor;
    }
};

template<usize N = 1 << 19>
using CompactWriter = Writer<N, true>;

} // namespace toy
// END bundled: include/toy/io.hpp
// BEGIN bundled: include/toy/prime.hpp

#include <bits/extc++.h>
#include <immintrin.h>
// BEGIN bundled: include/toy/mod64.hpp

#include <bits/extc++.h>

namespace toy {

using i8 = int8_t;
using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

class Montgomery64 {
    u64 mod_;
    u64 neg_inv_;
    u64 r2_;

public:
    explicit Montgomery64(u64 mod) : mod_(mod), neg_inv_(mod) {
        for (int i = 0; i < 6; ++i) neg_inv_ *= 2 - mod * neg_inv_;
        neg_inv_ = -neg_inv_;
        u64 r = (u128(1) << 64) % mod_;
        r2_ = (u128)r * r % mod_;
    }

    [[nodiscard]] u64 mod() const { return mod_; }

    [[nodiscard, gnu::always_inline]] u64 reduce(u128 x) const {
        u64 q = (u64)x * neg_inv_;
        u64 result = (x + (u128)q * mod_) >> 64;
        return result >= mod_ ? result - mod_ : result;
    }

    [[nodiscard, gnu::always_inline]] u64 init(u64 x) const {
        return reduce((u128)(x % mod_) * r2_);
    }

    [[nodiscard, gnu::always_inline]] u64 mul(u64 a, u64 b) const {
        return reduce((u128)a * b);
    }

    [[nodiscard]] u64 one() const { return init(1); }
    [[nodiscard]] u64 value(u64 x) const { return reduce(x); }

    [[nodiscard]] u64 pow(u64 base, u64 exponent) const {
        u64 x = init(base), result = one();
        while (exponent) {
            if (exponent & 1) result = mul(result, x);
            x = mul(x, x);
            exponent >>= 1;
        }
        return result;
    }
};

} // namespace toy
// END bundled: include/toy/mod64.hpp

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
    constexpr u64 witnesses[] = {
        2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022
    };
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

    constexpr std::array<u64, 7> witnesses = {
        2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022
    };
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
            for (u32 i = 0; i < witnesses.size(); ++i)
                values[i] = mont.mul(values[i], powers[i]);
        for (u32 i = 0; i < witnesses.size(); ++i)
            powers[i] = mont.mul(powers[i], powers[i]);
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
    template<class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback&& callback) {
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
                for (u32 x = p * p; x <= root; x += 2 * p)
                    base_composite[x / 2] = true;
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
                if (!(composite[bit / 64] >> (bit % 64) & 1))
                    callback(index++, low + 2ULL * bit);
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
    template<class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback&& callback) {
        assert(limit <= max_limit);
        u32 odd_count = (limit + 1) / 2;
        std::fill(bits.begin(), bits.begin() + (odd_count + 63) / 64, 0);

        for (int y = 1, m; (m = (y * y + 36) / 2) < (int)odd_count; y += 2) {
            if (y % 3)
                for (int k = 0; m < (int)odd_count; m += (k += 36) + 18)
                    flip(m);
        }
        for (int x = 1, m; (m = (4 * x * x + 1) / 2) < (int)odd_count; ++x) {
            if (x % 3)
                for (int k = 0; m < (int)odd_count; m += (k += 4))
                    flip(m);
        }
        for (int y = 2, m; (m = (y * y + 3) / 2) < (int)odd_count; y += 2) {
            if (y % 3)
                for (int k = 0; m < (int)odd_count; m += (k += 12))
                    flip(m);
        }
        for (int y = 1, m; (m = ((2 * y + 6) * y + 3) / 2) < (int)odd_count; ++y) {
            if (y % 3)
                for (int k = 6 * y; m < (int)odd_count; m += (k += 12))
                    flip(m);
        }
        for (int p = 5, square; (square = p * p) / 2 < (int)odd_count; p += 2) {
            if (test(p / 2))
                for (int m = square / 2; m < (int)odd_count; m += square)
                    reset(m);
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
    template<class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback&& callback) {
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
            if (word + 1 == used_words && odd_count % 64)
                primes &= (1ULL << (odd_count % 64)) - 1;
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
    template<class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback&& callback) {
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

    static u32 ordinal(u64 value) {
        return value / 30 * 8 + residue_index[value % 30];
    }

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

    static void apply_dense_group(std::span<const u32> group, u32 product,
                                  u32 used_blocks) {
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
            u8* destination = prime_bits.data() + offset;
            const u8* source = mask.data();
            u32 i = 0;
            for (; i + 32 <= count; i += 32) {
                __m256i values = _mm256_loadu_si256((const __m256i*)(destination + i));
                __m256i filter = _mm256_loadu_si256((const __m256i*)(source + i));
                _mm256_storeu_si256((__m256i*)(destination + i),
                                    _mm256_and_si256(values, filter));
            }
            for (; i < count; ++i) destination[i] &= source[i];
        }
    }

public:
    template<class PrimeCallback>
    static u64 enumerate(u32 limit, PrimeCallback&& callback,
                         u64 stride = 1, u64 offset = 0) {
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
        for (u32 block_end = sparse_block; ; block_end += sparse_block) {
            u32 end = std::min(block_end, total_ordinals);
            for (auto& item : sparse) {
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
// END bundled: include/toy/prime.hpp

int main() {
    toy::Reader in;
    toy::CompactWriter<> out;
    toy::u32 n = in.read<toy::u32>();
    toy::u32 a = in.read<toy::u32>();
    toy::u32 b = in.read<toy::u32>();
    std::vector<toy::u32> selected;
    selected.reserve(1'000'000);
    toy::u64 count = toy::HybridWheel30Sieve::enumerate(
        n, [&](toy::u64, toy::u64 prime) {
            selected.push_back(prime);
        }, a, b);
    out.write_token(count);
    out.write_token(selected.size());
    out.put('\n');
    for (toy::u32 prime : selected) out.write_token(prime);
    out.put('\n');
}
// END bundled: problems/enumerate_primes/main.cpp
