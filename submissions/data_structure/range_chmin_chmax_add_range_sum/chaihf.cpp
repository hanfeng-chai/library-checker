// BEGIN bundled: problems/data_structure/range_chmin_chmax_add_range_sum/iterative_segment_tree_beats.cpp
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

struct DirectMappingTag {};
inline constexpr DirectMappingTag direct_mapping;
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

    static const char* map_stdin_direct() {
        struct stat st{};
        fstat(0, &st);
        void* base = mmap(nullptr, (usize)st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
        toy_assert(base != MAP_FAILED);
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

    static bool all_digit4(u32 x) {
        return !(((x ^ 0x30303030U) & 0xf0f0f0f0U));
    }

    static u32 parse4(u32 x) {
        x ^= 0x30303030U;
        x = (x * 10 + (x >> 8)) & 0x00ff00ffU;
        return (x * 100 + (x >> 16)) & 0x0000ffffU;
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

    [[gnu::always_inline]] u64 read_swar4_u64() {
        const char* q = p;
        u32 block;
        memcpy(&block, q, 4);
        u64 value = 0;
        if (all_digit4(block)) {
            value = parse4(block);
            q += 4;
        }
        while (*q >= '0') value = value * 10 + *q++ - '0';
        p = q + 1;
        return value;
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
    explicit Reader(DirectMappingTag): p(map_stdin_direct()) {}
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
                if constexpr (MaxDigits <= 3) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 7) value = read_swar4_u64();
                else if constexpr (MaxDigits <= 13) value = read_swar_u64();
                else value = read_simd_u64();
            } else {
                if constexpr (MaxDigits <= 3) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 7) value = read_swar4_u64();
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

    [[gnu::always_inline]] string_view read_token(usize length) {
        string_view token(p, length);
        p += length;
        toy_assert(*p <= ' ');
        ++p;
        return token;
    }

    [[gnu::always_inline]] void skip_spaces() {
        while (*p <= ' ') ++p;
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
            group(cursor, x ? first_lut()[x]
                            : (u32)' ' | (u32)' ' << 8 |
                              (u32)' ' << 16 | (u32)'0' << 24);
        }
    }

    [[gnu::always_inline]] static void u64_raw(char*& cursor, u64 x) {
        const auto& table = lut();
        if constexpr (!Compact) {
            if (x >= 10'000'000'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                u64 middle = high % 10000;
                u64 top = high / 10000;
                first_group(cursor, top / 10000);
                group(cursor, table[top % 10000]);
                group(cursor, table[middle]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 1'000'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                first_group(cursor, high / 10000);
                group(cursor, table[high % 10000]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 100'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                first_group(cursor, x / 100'000'000ULL);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 10000ULL) {
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

    template<u64 Max>
    [[gnu::always_inline]] static void compact_u64_bounded(char*& cursor, u64 x) {
        static_assert(Max <= numeric_limits<u64>::max());
        const auto& table = lut();
        if constexpr (Max <= 9'999ULL) {
            first_group(cursor, x);
        } else if constexpr (Max <= 99'999'999ULL) {
            u64 q1 = div_1e4(x);
            if (q1) first_group(cursor, q1), group(cursor, table[x - q1 * 10000]);
            else first_group(cursor, x);
        } else if constexpr (Max <= 999'999'999'999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            if (q2) {
                first_group(cursor, q2);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q1) {
                first_group(cursor, q1);
                group(cursor, table[x - q1 * 10000]);
            } else {
                first_group(cursor, x);
            }
        } else if constexpr (Max <= 9'999'999'999'999'999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            if (q3) {
                first_group(cursor, q3);
                group(cursor, table[q2 - q3 * 10000]);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q2) {
                first_group(cursor, q2);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q1) {
                first_group(cursor, q1);
                group(cursor, table[x - q1 * 10000]);
            } else {
                first_group(cursor, x);
            }
        } else {
            u64_raw(cursor, x);
        }
    }

    template<unsigned Digits>
    [[gnu::always_inline]] static void u64_fixed(char*& cursor, u64 x) {
        static_assert(1 <= Digits && Digits <= 20);
        constexpr unsigned groups = (Digits + 3) / 4;
        constexpr unsigned leading = Digits - 4 * (groups - 1);
        auto first_fixed = [&](u64 value) {
            u32 first = lut()[value] >> ((4 - leading) * 8);
            memcpy(cursor, &first, 4);
            cursor += leading;
        };
        const auto& table = lut();
        if constexpr (groups == 1) {
            first_fixed(x);
        } else if constexpr (groups == 2) {
            u64 q1 = div_1e4(x);
            first_fixed(q1);
            group(cursor, table[x - q1 * 10000]);
        } else if constexpr (groups == 3) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            first_fixed(q2);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if constexpr (groups == 4) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            first_fixed(q3);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            u64 q3 = div_1e12(x), q4 = div_1e16(x);
            first_fixed(q4);
            group(cursor, table[q3 - q4 * 10000]);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        }
    }

    template<unsigned Digits>
    [[gnu::always_inline]] static void i128_fixed(char*& cursor, i128 x) {
        static_assert(1 <= Digits && Digits <= 39);
        bool negative = x < 0;
        u128 magnitude = negative ? u128(0) - (u128)x : (u128)x;
        if (negative) *cursor++ = '-';
        if constexpr (Digits <= 20) {
            u64_fixed<Digits>(cursor, (u64)magnitude);
        } else {
            auto [high, low] = detail::divmod_1e19(magnitude);
            u64_fixed<Digits - 19>(cursor, high);
            fixed19(cursor, low);
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

    [[gnu::always_inline]] void write_padded_u32(u32 x) {
        ensure(16);
        char* cursor = p;
        *cursor++ = ' ';
        if (x > 99'999'999U) {
            u32 high = x / 100'000'000U;
            u32 middle = x / 10'000U % 10'000U;
            group(cursor, first_lut()[high]);
            group(cursor, lut()[middle]);
            group(cursor, lut()[x % 10'000U]);
        } else if (x > 9'999U) {
            u32 high = x / 10'000U;
            group(cursor, first_lut()[high]);
            group(cursor, lut()[x % 10'000U]);
        } else if (x) {
            group(cursor, first_lut()[x]);
        } else {
            group(cursor, (u32)' ' | (u32)' ' << 8 |
                          (u32)' ' << 16 | (u32)'0' << 24);
        }
        p = cursor;
    }

    [[gnu::always_inline]] void write_token_u32_6(u32 x) {
        toy_assert(x < 1'000'000U);
        ensure(8);
        char* cursor = p;
        *cursor++ = ' ';
        if (x >= 10'000U) {
            u32 high = x / 10'000U;
            u32 digits = lut()[high];
            if (high >= 10) {
                digits >>= 16;
                memcpy(cursor, &digits, 2);
                cursor += 2;
            } else {
                *cursor++ = (char)(digits >> 24);
            }
            group(cursor, lut()[x % 10'000U]);
        } else if (x) {
            group(cursor, first_lut()[x]);
        } else {
            group(cursor, (u32)' ' | (u32)' ' << 8 |
                          (u32)' ' << 16 | (u32)'0' << 24);
        }
        p = cursor;
    }

    [[gnu::always_inline]] void writeln(u64 x) {
        ensure(24);
        char* cursor = p;
        u64_raw(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    [[gnu::always_inline]] void writeln_i64(i64 x) {
        ensure(24);
        char* cursor = p;
        if (x < 0)
            i64_raw(cursor, u64(0) - (u64)x);
        else
            u64_raw(cursor, (u64)x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void writeln_fixed(u64 x) {
        ensure(24);
        char* cursor = p;
        u64_fixed<Digits>(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void write_token_fixed(u64 x) {
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        u64_fixed<Digits>(cursor, x);
        p = cursor;
    }
    template<u64 Max>
    [[gnu::always_inline]] void writeln_bounded(u64 x) {
        toy_assert(x <= Max);
        ensure(24);
        char* cursor = p;
        if constexpr (Compact) compact_u64_bounded<Max>(cursor, x);
        else u64_raw(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    [[gnu::always_inline]] void write_token(u64 x) {
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        u64_raw(cursor, x);
        p = cursor;
    }
    template<u64 Max>
    [[gnu::always_inline]] void write_token_bounded(u64 x) {
        toy_assert(x <= Max);
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        if constexpr (Compact) compact_u64_bounded<Max>(cursor, x);
        else u64_raw(cursor, x);
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
    template<unsigned Digits>
    [[gnu::always_inline]] void writeln_fixed(i128 x) {
        ensure(48);
        char* cursor = p;
        i128_fixed<Digits>(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void write_token_fixed(i128 x) {
        ensure(48);
        char* cursor = p;
        *cursor++ = ' ';
        i128_fixed<Digits>(cursor, x);
        p = cursor;
    }
};

template<usize N = 1 << 19>
using CompactWriter = Writer<N, true>;

} // namespace toy
// END bundled: include/toy/io.hpp
// BEGIN bundled: include/toy/range.hpp

#include <bits/extc++.h>

namespace toy {

template<class T>
class PrefixSum {
    std::vector<T> prefix;

public:
    template<class Range>
    explicit PrefixSum(const Range& values) : prefix(values.size() + 1) {
        std::partial_sum(values.begin(), values.end(), prefix.begin() + 1);
    }

    T sum(int left, int right) const { return prefix[right] - prefix[left]; }
};

template<class T, class Operation>
class SegmentTree {
    int size;
    T identity;
    Operation operation;
    std::vector<T> data;

public:
    template<class Range>
    SegmentTree(const Range& values, T identity_value, Operation combine = {})
        : size(std::bit_ceil((unsigned)values.size())), identity(identity_value),
          operation(combine), data(2 * size, identity_value) {
        std::copy(values.begin(), values.end(), data.begin() + size);
        for (int i = size - 1; i; --i) data[i] = operation(data[2 * i], data[2 * i + 1]);
    }

    void set(int index, T value) {
        data[index += size] = value;
        while (index >>= 1) data[index] = operation(data[2 * index], data[2 * index + 1]);
    }

    T get(int index) const { return data[size + index]; }

    T fold(int left, int right) const {
        T lhs = identity, rhs = identity;
        for (left += size, right += size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) lhs = operation(lhs, data[left++]);
            if (right & 1) rhs = operation(data[--right], rhs);
        }
        return operation(lhs, rhs);
    }
};

template<class T, class Operation>
class SparseTable {
    Operation operation;
    std::vector<std::vector<T>> table;

public:
    template<class Range>
    explicit SparseTable(const Range& values, Operation combine = {})
        : operation(combine), table(std::bit_width(values.size())) {
        table[0].assign(values.begin(), values.end());
        for (int level = 1; level < (int)table.size(); ++level) {
            int half = 1 << (level - 1);
            int count = values.size() - (1 << level) + 1;
            table[level].resize(std::max(count, 0));
            for (int i = 0; i < count; ++i)
                table[level][i] = operation(table[level - 1][i],
                                            table[level - 1][i + half]);
        }
    }

    T fold_idempotent(int left, int right) const {
        int level = std::bit_width((unsigned)(right - left)) - 1;
        return operation(table[level][left], table[level][right - (1 << level)]);
    }
};

class RangeAddMinTree {
    static constexpr int64_t infinity =
        4'000'000'000'000'000'000LL;

    struct PrefixMinimum {
        int64_t sum = 0;
        int64_t minimum = infinity;
    };

    int length;
    int size;
    std::vector<int64_t> difference;
    std::vector<PrefixMinimum> data;

    static PrefixMinimum combine(
        const PrefixMinimum& left, const PrefixMinimum& right) {
        return {
            left.sum + right.sum,
            std::min(left.minimum, left.sum + right.minimum)};
    }

    void set(int index) {
        int node = size + index;
        data[node] = {difference[index], difference[index]};
        while (node >>= 1)
            data[node] = combine(data[node * 2], data[node * 2 + 1]);
    }

    PrefixMinimum fold(int left, int right) const {
        uint32_t lower = left + size - 1;
        uint32_t upper = right + size;
        int width = std::bit_width(lower ^ upper) - 1;
        uint32_t mask = (uint32_t(1) << width) - 1;
        PrefixMinimum result;

        uint32_t bits = ~lower & mask;
        while (bits != 0) {
            int level = std::countr_zero(bits);
            bits ^= uint32_t(1) << level;
            result = combine(
                result, data[(lower >> level) ^ 1]);
        }
        bits = upper & mask;
        while (bits != 0) {
            int level = std::bit_width(bits) - 1;
            bits ^= uint32_t(1) << level;
            result = combine(
                result, data[(upper >> level) ^ 1]);
        }
        return result;
    }

    int64_t prefix_sum(int end) const {
        int64_t result = 0;
        for (uint32_t node = size + end; node > 1; node >>= 1)
            if (node & 1) result += data[node - 1].sum;
        return result;
    }

public:
    explicit RangeAddMinTree(const std::vector<int64_t>& values)
        : length(values.size()),
          size(std::bit_ceil((unsigned)values.size())),
          difference(values.size()), data(2 * size) {
        int64_t previous = 0;
        for (int i = 0; i < length; ++i) {
            difference[i] = values[i] - previous;
            previous = values[i];
            data[size + i] = {difference[i], difference[i]};
        }
        for (int node = size - 1; node; --node)
            data[node] = combine(data[node * 2], data[node * 2 + 1]);
    }

    void add(int left, int right, int64_t value) {
        difference[left] += value;
        set(left);
        if (right < length) {
            difference[right] -= value;
            set(right);
        }
    }

    int64_t minimum_of(int left, int right) const {
        return prefix_sum(left) + fold(left, right).minimum;
    }
};

class RecursiveRangeClampAddSumTree {
    static constexpr int64_t infinity = std::numeric_limits<int64_t>::max();
    struct Node {
        int64_t maximum = -infinity, second_maximum = -infinity;
        int64_t minimum = infinity, second_minimum = infinity;
        int64_t sum = 0, lazy_add = 0;
        int maximum_count = 0, minimum_count = 0;
    };

    int size;
    std::vector<Node> data;

    void pull(int node) {
        const Node& left = data[node * 2];
        const Node& right = data[node * 2 + 1];
        Node& current = data[node];
        current.sum = left.sum + right.sum;
        current.maximum = std::max(left.maximum, right.maximum);
        current.maximum_count =
            (left.maximum == current.maximum ? left.maximum_count : 0) +
            (right.maximum == current.maximum ? right.maximum_count : 0);
        current.second_maximum =
            std::max(left.maximum == current.maximum ? left.second_maximum : left.maximum,
                     right.maximum == current.maximum ? right.second_maximum : right.maximum);
        current.minimum = std::min(left.minimum, right.minimum);
        current.minimum_count =
            (left.minimum == current.minimum ? left.minimum_count : 0) +
            (right.minimum == current.minimum ? right.minimum_count : 0);
        current.second_minimum =
            std::min(left.minimum == current.minimum ? left.second_minimum : left.minimum,
                     right.minimum == current.minimum ? right.second_minimum : right.minimum);
        current.lazy_add = 0;
    }
    void apply_add(int node, int length, int64_t value) {
        Node& current = data[node];
        current.sum += value * length;
        current.maximum += value;
        current.minimum += value;
        if (current.second_maximum != -infinity) current.second_maximum += value;
        if (current.second_minimum != infinity) current.second_minimum += value;
        current.lazy_add += value;
    }
    void apply_chmin(int node, int64_t value) {
        Node& current = data[node];
        current.sum += (value - current.maximum) * current.maximum_count;
        if (current.minimum == current.maximum) current.minimum = value;
        else if (current.second_minimum == current.maximum) current.second_minimum = value;
        current.maximum = value;
    }
    void apply_chmax(int node, int64_t value) {
        Node& current = data[node];
        current.sum += (value - current.minimum) * current.minimum_count;
        if (current.maximum == current.minimum) current.maximum = value;
        else if (current.second_maximum == current.minimum) current.second_maximum = value;
        current.minimum = value;
    }
    void push(int node, int left_length, int right_length) {
        Node& current = data[node];
        if (current.lazy_add) {
            apply_add(node * 2, left_length, current.lazy_add);
            apply_add(node * 2 + 1, right_length, current.lazy_add);
            current.lazy_add = 0;
        }
        if (data[node * 2].maximum > current.maximum)
            apply_chmin(node * 2, current.maximum);
        if (data[node * 2 + 1].maximum > current.maximum)
            apply_chmin(node * 2 + 1, current.maximum);
        if (data[node * 2].minimum < current.minimum)
            apply_chmax(node * 2, current.minimum);
        if (data[node * 2 + 1].minimum < current.minimum)
            apply_chmax(node * 2 + 1, current.minimum);
    }
    void build(int node, int left, int right, const std::vector<int64_t>& values) {
        if (right - left == 1) {
            int64_t value = values[left];
            data[node] = {value, -infinity, value, infinity, value, 0, 1, 1};
            return;
        }
        int middle = (left + right) / 2;
        build(node * 2, left, middle, values);
        build(node * 2 + 1, middle, right, values);
        pull(node);
    }
    void chmin(int node, int left, int right, int query_left, int query_right,
               int64_t value) {
        if (query_right <= left || right <= query_left || data[node].maximum <= value)
            return;
        if (query_left <= left && right <= query_right &&
            data[node].second_maximum < value) {
            apply_chmin(node, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        chmin(node * 2, left, middle, query_left, query_right, value);
        chmin(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    void chmax(int node, int left, int right, int query_left, int query_right,
               int64_t value) {
        if (query_right <= left || right <= query_left || value <= data[node].minimum)
            return;
        if (query_left <= left && right <= query_right &&
            value < data[node].second_minimum) {
            apply_chmax(node, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        chmax(node * 2, left, middle, query_left, query_right, value);
        chmax(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    void add(int node, int left, int right, int query_left, int query_right,
             int64_t value) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            apply_add(node, right - left, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        add(node * 2, left, middle, query_left, query_right, value);
        add(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    int64_t sum(int node, int left, int right, int query_left, int query_right) {
        if (query_right <= left || right <= query_left) return 0;
        if (query_left <= left && right <= query_right) return data[node].sum;
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        return sum(node * 2, left, middle, query_left, query_right) +
               sum(node * 2 + 1, middle, right, query_left, query_right);
    }

public:
    explicit RecursiveRangeClampAddSumTree(
        const std::vector<int64_t>& values)
        : size(values.size()), data(values.size() * 4) {
        build(1, 0, size, values);
    }
    void chmin(int left, int right, int64_t value) {
        chmin(1, 0, size, left, right, value);
    }
    void chmax(int left, int right, int64_t value) {
        chmax(1, 0, size, left, right, value);
    }
    void add(int left, int right, int64_t value) {
        add(1, 0, size, left, right, value);
    }
    int64_t sum(int left, int right) {
        return sum(1, 0, size, left, right);
    }
};

class RangeClampAddSumTree {
    static constexpr int64_t infinity =
        std::numeric_limits<int64_t>::max();

    struct Node {
        int64_t maximum = -infinity;
        int64_t second_maximum = -infinity;
        int64_t minimum = infinity;
        int64_t second_minimum = infinity;
        int64_t sum = 0;
        int64_t lazy_add = 0;
        uint32_t maximum_count = 0;
        uint32_t minimum_count = 0;
    };

    int length;
    uint32_t capacity;
    uint32_t depth;
    std::vector<Node> data;

    void pull(uint32_t node) {
        const Node& left = data[node * 2];
        const Node& right = data[node * 2 + 1];
        Node& current = data[node];
        current.sum = left.sum + right.sum;
        if (left.maximum == right.maximum) {
            current.maximum = left.maximum;
            current.second_maximum =
                std::max(left.second_maximum, right.second_maximum);
            current.maximum_count =
                left.maximum_count + right.maximum_count;
        } else if (left.maximum > right.maximum) {
            current.maximum = left.maximum;
            current.second_maximum =
                std::max(left.second_maximum, right.maximum);
            current.maximum_count = left.maximum_count;
        } else {
            current.maximum = right.maximum;
            current.second_maximum =
                std::max(left.maximum, right.second_maximum);
            current.maximum_count = right.maximum_count;
        }
        if (left.minimum == right.minimum) {
            current.minimum = left.minimum;
            current.second_minimum =
                std::min(left.second_minimum, right.second_minimum);
            current.minimum_count =
                left.minimum_count + right.minimum_count;
        } else if (left.minimum < right.minimum) {
            current.minimum = left.minimum;
            current.second_minimum =
                std::min(left.second_minimum, right.minimum);
            current.minimum_count = left.minimum_count;
        } else {
            current.minimum = right.minimum;
            current.second_minimum =
                std::min(left.minimum, right.second_minimum);
            current.minimum_count = right.minimum_count;
        }
        current.lazy_add = 0;
    }

    void apply_add(uint32_t node, uint32_t node_length, int64_t value) {
        Node& current = data[node];
        current.sum += value * node_length;
        current.maximum += value;
        current.minimum += value;
        if (current.second_maximum != -infinity)
            current.second_maximum += value;
        if (current.second_minimum != infinity)
            current.second_minimum += value;
        current.lazy_add += value;
    }

    void apply_chmin(uint32_t node, int64_t value) {
        Node& current = data[node];
        current.sum +=
            (value - current.maximum) * current.maximum_count;
        if (current.minimum == current.maximum)
            current.minimum = value;
        else if (current.second_minimum == current.maximum)
            current.second_minimum = value;
        current.maximum = value;
    }

    void apply_chmax(uint32_t node, int64_t value) {
        Node& current = data[node];
        current.sum +=
            (value - current.minimum) * current.minimum_count;
        if (current.maximum == current.minimum)
            current.maximum = value;
        else if (current.second_maximum == current.minimum)
            current.second_maximum = value;
        current.minimum = value;
    }

    void push(uint32_t node, uint32_t node_length) {
        Node& current = data[node];
        uint32_t child_length = node_length >> 1;
        if (current.lazy_add != 0) {
            apply_add(node * 2, child_length, current.lazy_add);
            apply_add(node * 2 + 1, child_length, current.lazy_add);
            current.lazy_add = 0;
        }
        if (data[node * 2].maximum > current.maximum)
            apply_chmin(node * 2, current.maximum);
        if (data[node * 2].minimum < current.minimum)
            apply_chmax(node * 2, current.minimum);
        if (data[node * 2 + 1].maximum > current.maximum)
            apply_chmin(node * 2 + 1, current.maximum);
        if (data[node * 2 + 1].minimum < current.minimum)
            apply_chmax(node * 2 + 1, current.minimum);
    }

    void apply_chmin_subtree(
        uint32_t node, uint32_t node_length, int64_t value) {
        if (data[node].maximum <= value) return;
        if (data[node].second_maximum < value) {
            apply_chmin(node, value);
            return;
        }
        push(node, node_length);
        apply_chmin_subtree(node * 2, node_length >> 1, value);
        apply_chmin_subtree(node * 2 + 1, node_length >> 1, value);
        pull(node);
    }

    void apply_chmax_subtree(
        uint32_t node, uint32_t node_length, int64_t value) {
        if (data[node].minimum >= value) return;
        if (value < data[node].second_minimum) {
            apply_chmax(node, value);
            return;
        }
        push(node, node_length);
        apply_chmax_subtree(node * 2, node_length >> 1, value);
        apply_chmax_subtree(node * 2 + 1, node_length >> 1, value);
        pull(node);
    }

    template<class Apply>
    void apply_range(int left, int right, Apply apply) {
        uint32_t lower = left + capacity;
        uint32_t upper = right - 1 + capacity;
        if (lower == upper) {
            uint32_t node_length = capacity;
            for (uint32_t level = depth; level != 0; --level) {
                push(lower >> level, node_length);
                node_length >>= 1;
            }
            apply(lower, 1);
            while (lower >>= 1) pull(lower);
            return;
        }

        uint32_t split_level =
            std::bit_width(lower ^ upper) - 1;
        uint32_t node_length = capacity;
        for (uint32_t level = depth; level > split_level; --level) {
            push(lower >> level, node_length);
            node_length >>= 1;
        }
        for (uint32_t level = split_level; level != 0; --level) {
            push(lower >> level, node_length);
            push(upper >> level, node_length);
            node_length >>= 1;
        }

        apply(lower, 1);
        apply(upper, 1);
        node_length = 1;
        while ((lower >> 1) < (upper >> 1)) {
            if ((lower & 1) == 0)
                apply(lower + 1, node_length);
            pull(lower >>= 1);
            if (upper & 1)
                apply(upper - 1, node_length);
            pull(upper >>= 1);
            node_length <<= 1;
        }
        while (lower >>= 1) pull(lower);
    }

    void push_boundary_paths(uint32_t lower, uint32_t upper) {
        if (lower == upper) {
            uint32_t node_length = capacity;
            for (uint32_t level = depth; level != 0; --level) {
                push(lower >> level, node_length);
                node_length >>= 1;
            }
            return;
        }
        uint32_t split_level =
            std::bit_width(lower ^ upper) - 1;
        uint32_t node_length = capacity;
        for (uint32_t level = depth; level > split_level; --level) {
            push(lower >> level, node_length);
            node_length >>= 1;
        }
        for (uint32_t level = split_level; level != 0; --level) {
            push(lower >> level, node_length);
            push(upper >> level, node_length);
            node_length >>= 1;
        }
    }

public:
    explicit RangeClampAddSumTree(
        const std::vector<int64_t>& values)
        : length(values.size()),
          capacity(std::bit_ceil((uint32_t)values.size())),
          depth(std::bit_width(capacity) - 1),
          data(2 * capacity) {
        for (uint32_t i = 0; i < values.size(); ++i) {
            int64_t value = values[i];
            data[capacity + i] = {
                value, -infinity, value, infinity,
                value, 0, 1, 1};
        }
        for (uint32_t node = capacity - 1; node != 0; --node)
            pull(node);
    }

    void chmin(int left, int right, int64_t value) {
        apply_range(
            left, right,
            [&](uint32_t node, uint32_t node_length) {
                apply_chmin_subtree(node, node_length, value);
            });
    }

    void chmax(int left, int right, int64_t value) {
        apply_range(
            left, right,
            [&](uint32_t node, uint32_t node_length) {
                apply_chmax_subtree(node, node_length, value);
            });
    }

    void add(int left, int right, int64_t value) {
        apply_range(
            left, right,
            [&](uint32_t node, uint32_t node_length) {
                apply_add(node, node_length, value);
            });
    }

    int64_t sum(int left, int right) {
        uint32_t lower = left + capacity;
        uint32_t upper = right - 1 + capacity;
        push_boundary_paths(lower, upper);
        int64_t left_sum = data[lower].sum;
        if (lower == upper) return left_sum;
        int64_t right_sum = data[upper].sum;
        while ((lower >> 1) != (upper >> 1)) {
            if ((lower & 1) == 0)
                left_sum += data[lower + 1].sum;
            if (upper & 1)
                right_sum += data[upper - 1].sum;
            lower >>= 1;
            upper >>= 1;
        }
        return left_sum + right_sum;
    }
};

} // namespace toy
// END bundled: include/toy/range.hpp

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, int>();
    int q = input.read_uniform<6, int>();
    std::vector<int64_t> values(n);
    for (auto& value : values) value = input.read_uniform<13, int64_t>();
    toy::RangeClampAddSumTree tree(values);
    while (q--) {
        int type = input.read_fixed<1, int>();
        int left = input.read_uniform<6, int>();
        int right = input.read_uniform<6, int>();
        if (type == 0) tree.chmin(left, right, input.read_uniform<13, int64_t>());
        else if (type == 1) tree.chmax(left, right, input.read_uniform<13, int64_t>());
        else if (type == 2) tree.add(left, right, input.read_uniform<13, int64_t>());
        else output.writeln_i64(tree.sum(left, right));
    }
}
// END bundled: problems/data_structure/range_chmin_chmax_add_range_sum/iterative_segment_tree_beats.cpp
