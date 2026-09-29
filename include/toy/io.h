#pragma once
#include <toy/common.h>

namespace toy {
namespace io_detail {
inline constexpr auto shift = [] {
    array<array<u8, 16>, 17> a{};
    for (int n = 0; n <= 16; ++n)
        for (int i = 0; i < 16; ++i) a[n][i] = i < 16 - n ? 128 : i - 16 + n;
    return a;
}();
inline constexpr auto power = [] {
    array<u64, 20> a{1};
    for (int i = 1; i <= 19; ++i) a[i] = a[i - 1] * 10;
    return a;
}();

[[gnu::always_inline]] inline u64 decimal8(u64 x) {
    x &= 0x0f0f0f0f0f0f0f0fULL;
    x = (x * 10 + (x >> 8)) & 0x00ff00ff00ff00ffULL;
    x = (x * 100 + (x >> 16)) & 0x0000ffff0000ffffULL;
    return (x * 10000 + (x >> 32)) & 0xffffffff;
}
inline constexpr auto digits = [] {
    array<array<char, 4>, 10000> a{};
    for (int i = 0; i < 10000; ++i)
        for (int j = 0, x = i; j < 4; ++j, x /= 10) a[i][3 - j] = '0' + x % 10;
    return a;
}();
inline constexpr auto trim = [] {
    array<array<u8, 16>, 17> a{};
    for (int n = 0; n <= 16; ++n)
        for (int i = 0; i < 16; ++i) a[n][i] = i < n ? 16 - n + i : 128;
    return a;
}();

inline constexpr auto first = [] {
    array<u32, 10000> a{};
    for (int i = 0; i < 10000; ++i) {
        int skip = (i < 1000) + (i < 100) + (i < 10);
        u32 word = bit_cast<u32>(digits[i]) >> (skip * 8);
        a[i] = word | (u32(3 - skip) << 30);
    }
    return a;
}();

template<int Digits>
[[gnu::always_inline]] inline u64 quotient(u64 x) {
    // Exact for every u64: magic*10^Digits - 2^(64+shift) < 2^shift.
    constexpr u64 magic[] = {0, 0x346dc5d63886594bULL, 0xabcc77118461cefdULL,
                            0x232f33025bd42233ULL, 0x39a5652fb1137857ULL};
    constexpr int shift[] = {0, 11, 26, 37, 51};
    if constexpr (Digits == 0) return x;
    else if constexpr (Digits % 4 == 0)
        return (u128(x) * magic[Digits / 4]) >> (64 + shift[Digits / 4]);
    else return x / power[Digits];
}

// 16 decimal bytes -> eight pairs -> four groups of 4 -> two groups of 8.
[[gnu::always_inline]] inline u64 decimal16(__m128i x) {
    x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010a));
    x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));
    x = _mm_packus_epi32(x, x); // 4-digit groups fit in 16 bits.
    x = _mm_madd_epi16(x, _mm_set1_epi32(0x00012710));
    u64 y = _mm_cvtsi128_si64(x);
    return u64(u32(y)) * 100000000 + (y >> 32);
}

// floor(x / 10^19). Reciprocal is ceil(2^192 / 10^19) - 2^128.
// Four 64x64 products give the high half of x * reciprocal; retain the carry
// from adding x so the formula also covers the full unsigned 128-bit range.
[[gnu::always_inline]] inline u128 div19(u128 x) {
    constexpr u64 ml = 0x5663d3c7a0d865cbULL, mh = 0xd83c94fb6d2ac34aULL;
    u64 lo = x, hi = x >> 64;
    u128 a = u128(lo) * ml, b = u128(lo) * mh, c = u128(hi) * ml;
    u128 h = u128(hi) * mh + (b >> 64) + (c >> 64)
           + (((a >> 64) + u64(b) + u64(c)) >> 64);
    u128 s = x + h;
    return (s >> 64) + (u128(s < x) << 64);
}
}

// Valid decimal tokens separated by exactly one space/newline; stdin is a file.
// One Reader per process. A zero page after the file makes SIMD tail loads safe.
struct Reader {
    const char* p;
    Reader() {
        struct stat st;
        fstat(0, &st);
        usize n = (st.st_size + 4095) & -usize(4096);
        void* base = mmap(nullptr, n + 4096, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (n) mmap(base, st.st_size, PROT_READ, MAP_PRIVATE | MAP_FIXED, 0, 0);
        p = (const char*)base;
    }

    // Digits is an optional bound on the magnitude's decimal length.
    template<class T = u32, int Digits = (sizeof(T) * 8 * 30103 / 100000 + 1)>
    [[gnu::always_inline]] T read() {
        using U = conditional_t<(sizeof(T) > 8), u128, u64>;
        bool neg = false;
        if constexpr (T(-1) < T(0)) neg = *p == '-', p += neg;
        U v;
        if constexpr (Digits > 13) {
            if (p[1] < '0') {
                v = *p - '0'; p += 2;
                return T(neg ? U(0) - v : v);
            }
        }
        if constexpr (Digits == 1) {
            v = *p - '0'; p += 2;
        } else if constexpr (Digits == 2) {
            bool two = p[1] >= '0';
            v = *p - '0';
            v = two ? v * 10 + p[1] - '0' : v;
            p += 2 + two;
        } else if constexpr (Digits <= 3) {
            v = 0;
            while (*p >= '0') v = v * 10 + *p++ - '0';
            ++p;
        } else if constexpr (Digits <= 8) {
            u64 x;
            memcpy(&x, p, 8);
            // Digits have bit 4 set; space/newline do not. Right-align the
            // digit nibbles, then combine adjacent groups of 1, 2 and 4 bytes.
            int n = countr_zero(~x & 0x1010101010101010ULL) / 8;
            v = io_detail::decimal8(x << ((8 - n) * 8));
            p += n + 1;
        } else if constexpr (Digits <= 13) {
            u64 x;
            memcpy(&x, p, 8);
            v = 0;
            if ((x & 0x1010101010101010ULL) == 0x1010101010101010ULL)
                v = io_detail::decimal8(x), p += 8;
            while (*p >= '0') v = v * 10 + *p++ - '0';
            ++p;
        } else {
            auto load = [&] {
                return _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p), _mm_set1_epi8('0'));
            };
            auto short16 = [&] (__m128i x, int n) {
                x = _mm_shuffle_epi8(x, _mm_loadu_si128((const __m128i*)io_detail::shift[n].data()));
                return io_detail::decimal16(x);
            };
            __m128i x = load();
            // Delimiters acquire a sign bit after subtracting '0'. Full
            // blocks bypass the length calculation and alignment shuffle.
            unsigned mask = _mm_movemask_epi8(x);
            if constexpr (Digits <= 16) {
                int n = __builtin_ctz(mask | 0x10000);
                v = short16(x, n); p += n + 1;
            } else {
                if (mask) {
                    int n = __builtin_ctz(mask);
                    v = short16(x, n); p += n + 1;
                } else {
                    v = io_detail::decimal16(x); p += 16;
                    if constexpr (sizeof(T) <= 8) {
                        while (*p >= '0') v = v * 10 + *p++ - '0';
                        ++p;
                    } else {
                        x = load(); mask = _mm_movemask_epi8(x);
                        if (!mask) {
                            v = v * io_detail::power[16] + io_detail::decimal16(x);
                            p += 16; x = load(); mask = _mm_movemask_epi8(x);
                        }
                        int n = __builtin_ctz(mask);
                        v = v * io_detail::power[n] + short16(x, n); p += n + 1;
                    }
                }
            }
        }
        return T(neg ? U(0) - v : v);
    }

    string_view token() {
        const char* begin = p;
        while (*p > ' ') ++p;
        return {begin, usize(p++ - begin)};
    }
};

template<usize N = 1 << 19>
struct Writer {
    static_assert(N >= 48);
    char buf[N], *p = buf;
    ~Writer() { flush(); }
    void flush() { ::write(1, buf, p - buf); p = buf; }

    template<int Digits, int Offset = 0>
    [[gnu::always_inline]] static char* fixed(char* p, u64 x) {
        // Every quotient uses the original x, rather than waiting for the
        // previous group. Inlining shares quotients between adjacent groups.
        constexpr int n = min(Digits, 4);
        u64 q = io_detail::quotient<Offset>(x)
              - io_detail::quotient<Offset + n>(x) * io_detail::power[n];
        if constexpr (Digits > 4) {
            p = fixed<Digits - 4, Offset + 4>(p, x);
        }
        memcpy(p, io_detail::digits[q].data() + 4 - n, n); return p + n;
    }
    [[gnu::always_inline]] static char* leading(char* p, u64 x) {
        // ASCII needs only six bits per byte; the top two bits store
        // length-1, avoiding three comparisons for every leading group.
        u32 entry = io_detail::first[x], word = entry & 0x3fffffff;
        memcpy(p, &word, 4); return p + (entry >> 30) + 1;
    }
    [[gnu::always_inline]] static char* number(char* p, u64 x) {
        if (x < 10000) return leading(p, x);
        if (x >= 10000000000000000ULL)
            return fixed<16>(leading(p, io_detail::quotient<16>(x)), x);
        // Four 4-digit table entries form one vector. Drop leading zeroes
        // with one byte shuffle, avoiding branches for 5..16 digit outputs.
        u64 a = io_detail::quotient<4>(x), b = io_detail::quotient<8>(x), c = io_detail::quotient<12>(x);
        auto word = [](u64 v) { return bit_cast<u32>(io_detail::digits[v]); };
        auto digits = _mm_set_epi32(word(x - a * 10000), word(a - b * 10000), word(b - c * 10000), word(c));
        // 1233/4096 approximates log10(2); one comparison corrects the estimate.
        int n = (bit_width(x) * 1233) >> 12;
        n += x >= io_detail::power[n];
        digits = _mm_shuffle_epi8(digits, _mm_loadu_si128((const __m128i*)io_detail::trim[n].data()));
        _mm_storeu_si128((__m128i*)p, digits);
        return p + n;
    }

    template<class T>
    [[gnu::always_inline]] static char* format(char* cursor, T x, char end) {
        using U = conditional_t<(sizeof(T) > 8), u128, u64>;
        U v = x;
        if constexpr (T(-1) < T(0))
            if (x < 0) *cursor++ = '-', v = U(0) - v;
        if constexpr (sizeof(T) > 8) {
            constexpr u64 b = 10000000000000000000ULL;
            if (v >= b) {
                u128 q = io_detail::div19(v);
                if (q >= b) cursor = fixed<19>(number(cursor, u64(q / b)), u64(q % b));
                else cursor = number(cursor, u64(q));
                cursor = fixed<19>(cursor, u64(v - q * b));
            } else cursor = number(cursor, u64(v));
        } else cursor = number(cursor, v);
        *cursor++ = end;
        return cursor;
    }
    template<class T>
    [[gnu::always_inline]] void write(T x, char end = '\n') {
        if (p - buf > N - 48) flush();
        p = format(p, x, end);
    }
    template<class T, usize Extent>
    void write(span<T, Extent> values, char end = '\n') {
        for (usize i = 0; i < values.size();) {
            usize n = min(values.size() - i, usize(buf + N - p) / 48);
            if (!n) { flush(); continue; }
            // One capacity check and member update per batch. Character
            // stores cannot force the local cursor back into memory.
            char* cursor = p;
            for (usize stop = i + n; i < stop; ++i) cursor = format(cursor, values[i], end);
            p = cursor;
        }
    }
};
}
