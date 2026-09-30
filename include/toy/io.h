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

// For x > 0 with b significant bits, floor(log10(x)) differs from
// floor(b*log10(2)) by at most one. Adding x to this bias carries into
// the high word exactly when x reaches the next power of ten.
inline constexpr auto length = [] {
    array<u128, 64> a{};
    for (int z = 0; z < 64; ++z) {
        int d = ((64 - z) * 1233) >> 12;
        a[z] = (u128(d + 1) << 64) - power[d];
    }
    return a;
}();

template<bool Parallel = false>
[[gnu::always_inline]] inline u64 decimal8(u64 x) {
    x &= 0x0f0f0f0f0f0f0f0fULL;
    x = (x * 10 + (x >> 8)) & 0x00ff00ff00ff00ffULL;
    if constexpr (Parallel) {
        // c0,c1,c2,c3 are 2-digit groups. Two independent products compute
        // c0*10^6+c2*100 and c1*10000+c3 in their high words.
        u64 a = x & 0x000000ff000000ffULL;
        u64 b = (x >> 16) & 0x000000ff000000ffULL;
        return (a * ((1000000ULL << 32) + 100) + b * ((10000ULL << 32) + 1)) >> 32;
    } else {
        x = (x * 100 + (x >> 16)) & 0x0000ffff0000ffffULL;
        return (x * 10000 + (x >> 32)) & 0xffffffff;
    }
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

// ASCII tokens separated by exactly one space/newline; read() parses decimal.
// Construct one file mapping per process; copied cursors share its zero padding.
struct Reader {
    const char* p;
    [[gnu::always_inline]] Reader() {
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
        if constexpr (sizeof(T) > 8 && T(-1) < T(0)) {
            bool neg = *p == '-'; p += neg;
            U v = read<U, Digits>();
            // Form both halves from one word; a wide subtraction costs more.
            u64 mask = -u64(neg);
            return T((v ^ ((u128(mask) << 64) | mask)) + neg);
        }
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
            // digit nibbles, then reduce pairs of decimal digits.
            int n = countr_zero(~x & 0x1010101010101010ULL) / 8;
            if constexpr (Digits == 4) {
                u32 y = u32(x) << ((4 - n) * 8);
                y &= 0x0f0f0f0f;
                y = (y * 10 + (y >> 8)) & 0x00ff00ff;
                v = (y * 100 + (y >> 16)) & 0xffff;
            } else v = io_detail::decimal8(x << ((8 - n) * 8));
            p += n + 1;
        } else if constexpr (Digits <= 13) {
            u64 x;
            memcpy(&x, p, 8);
            v = 0;
            if ((x & 0x1010101010101010ULL) == 0x1010101010101010ULL)
                v = io_detail::decimal8<true>(x), p += 8;
            while (*p >= '0') v = v * 10 + *p++ - '0';
            ++p;
        } else {
            auto load = [&] {
                return _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p), _mm_set1_epi8('0'));
            };
            auto short16 = [&] (__m128i x, int n) {
                // n + [-16,...,-1] selects digits; negative indices insert zeros.
                auto indices = _mm_setr_epi8(-16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1);
                x = _mm_shuffle_epi8(x, _mm_add_epi8(indices, _mm_set1_epi8(n)));
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
                        if constexpr (Digits >= 18) {
                            u32 tail;
                            memcpy(&tail, p, 4);
                            u32 flags = tail & 0x00101010;
                            if (flags == 0x00001010) {
                                u32 y = tail & 0x0f0f;
                                v = v * 100 + ((y * 10 + (y >> 8)) & 255);
                                p += 3;
                                return T(neg ? U(0) - v : v);
                            }
                            if (flags == 0x00101010) {
                                // Only full-range u64 inputs can need four more
                                // digits. Handle their 3/4-digit tails without a loop.
                                u32 four = 0;
                                if constexpr (Digits >= 20 && T(-1) > T(0)) four = (tail >> 28) & 1;
                                u32 y = (tail & 0x0f0f0f0f) << ((1 - four) * 8);
                                y = (y * 10 + (y >> 8)) & 0x00ff00ff;
                                v = v * (four ? 10000 : 1000) + ((y * 100 + (y >> 16)) & 0xffff);
                                p += 4 + four;
                                return T(neg ? U(0) - v : v);
                            }
                        }
                        while (*p >= '0') v = v * 10 + *p++ - '0';
                        ++p;
                    } else {
                        x = load(); mask = _mm_movemask_epi8(x);
                        if (!mask) {
                            if constexpr (Digits <= 38) {
                                // The token has at least 32 digits here. With a
                                // 38-digit bound, two <=19-digit blocks fit u64,
                                // so their combination needs only one 64x64 multiply.
                                u64 high = u64(v) * 1000 + (p[0] - '0') * 100 + (p[1] - '0') * 10 + p[2] - '0';
                                p += 3; x = load(); mask = _mm_movemask_epi8(x);
                                u64 low;
                                int n;
                                if (mask) {
                                    n = countr_zero(mask); low = short16(x, n);
                                } else {
                                    low = io_detail::decimal16(x); n = 16;
                                    while (n < Digits - 19 && p[n] >= '0') low = low * 10 + p[n++] - '0';
                                }
                                v = u128(high) * io_detail::power[n] + low;
                                p += n + 1;
                                return T(neg ? U(0) - v : v);
                            }
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

    // Two unsigned integers of at most 7 digits fit in one 16-byte load.
    // Shuffle each field into an independent eight-digit lane before reducing.
    template<int Digits = 7>
    [[gnu::always_inline]] array<u32, 2> read_pair() {
        static_assert(1 <= Digits && Digits <= 7);
        __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p), _mm_set1_epi8('0'));
        unsigned mask = _mm_movemask_epi8(x);
        int a = countr_zero(mask), b = countr_zero(mask & (mask - 1));
        auto first = _mm_loadu_si128((const __m128i*)io_detail::shift[a].data());
        auto second = _mm_loadu_si128((const __m128i*)io_detail::shift[b - a - 1].data());
        auto indices = _mm_add_epi8(_mm_unpackhi_epi64(first, second), _mm_set_epi64x(0x0101010101010101ULL * (a + 1), 0));
        x = _mm_shuffle_epi8(x, indices);
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010a));
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));
        x = _mm_packus_epi32(x, x);
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00012710));
        p += b + 1;
        return bit_cast<array<u32, 2>>(u64(_mm_cvtsi128_si64(x)));
    }

    [[gnu::always_inline]] string_view token() {
        const char* begin = p;
        for (;;) {
            auto x = _mm256_loadu_si256((const __m256i*)p);
            u32 stop = _mm256_movemask_epi8(_mm256_cmpgt_epi8(_mm256_set1_epi8(' ' + 1), x));
            if (stop) { p += countr_zero(stop); break; }
            p += 32;
        }
        return {begin, usize(p++ - begin)};
    }
};

template<usize N = 1 << 19>
struct Writer {
    static_assert(N >= 48);
    char buf[N], *p = buf;
    [[gnu::always_inline]] ~Writer() { flush(); }
    [[gnu::always_inline]] void flush() { ::write(1, buf, p - buf); p = buf; }
    [[gnu::always_inline]] void put(char c) { if (p == buf + N) flush(); *p++ = c; }
    void append(string_view text) {
        while (!text.empty()) {
            usize count = min(text.size(), usize(buf + N - p));
            if (!count) { flush(); continue; }
            memcpy(p, text.data(), count); p += count; text.remove_prefix(count);
        }
    }

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
        u32 n = u32((u128(x) + io_detail::length[__builtin_clzll(x)]) >> 64);
        digits = _mm_shuffle_epi8(digits, _mm_loadu_si128((const __m128i*)io_detail::trim[n].data()));
        _mm_storeu_si128((__m128i*)p, digits);
        return p + n;
    }

    template<class T>
    [[gnu::always_inline]] static char* format(char* cursor, T x, char end) {
        using U = conditional_t<(sizeof(T) > 8), u128, u64>;
        U v = x;
        if constexpr (sizeof(T) > 8 && T(-1) < T(0)) {
            if (!x) { cursor[0] = '0'; cursor[1] = end; return cursor + 2; }
            U mask = x >> (sizeof(T) * 8 - 1);
            *cursor = '-'; cursor += x < 0;
            v = (v ^ mask) - mask;
        } else if constexpr (T(-1) < T(0))
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
    [[gnu::always_inline]] void write(span<T, Extent> values, char end = '\n') {
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
