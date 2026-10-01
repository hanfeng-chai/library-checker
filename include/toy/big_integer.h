#pragma once
#include <toy/buffer.h>
#include <toy/convolution_integer.h>
#include <toy/fft_integer.h>
#include <toy/io.h>

namespace toy {
// Decimal limbs use 10^8; hexadecimal limbs use 2^64. Both store magnitude
// least-significant first and keep zero nonnegative. Buffers are reused.
template<bool Hex = false>
struct BigInteger {
    using Limb = std::conditional_t<Hex, u64, u32>;
    static constexpr u32 base = 100000000;
    Buffer<Limb> digits;
    bool negative = false;

    void trim() { while (digits.n && !digits[digits.n - 1]) --digits.n; if (!digits.n) negative = false; }

    [[gnu::always_inline]] static u32 decimal8(const char* p) {
        auto x = _mm_and_si128(_mm_loadl_epi64((const __m128i*)p), _mm_set1_epi8(15));
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010a));
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));
        x = _mm_packus_epi32(x, x);
        return _mm_cvtsi128_si32(_mm_madd_epi16(x, _mm_set1_epi32(0x00012710)));
    }
    [[gnu::always_inline]] static u64 decimal16(__m128i x) {
        x = _mm_and_si128(x, _mm_set1_epi8(15));
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010a));
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));
        x = _mm_packus_epi32(x, x);
        u64 halves = _mm_cvtsi128_si64(_mm_madd_epi16(x, _mm_set1_epi32(0x00012710)));
        return u64(u32(halves)) * base + (halves >> 32);
    }
    // Padded Reader token, at most 18 magnitude digits. Decode its known length
    // directly, avoiding a second delimiter scan in the machine-integer path.
    [[gnu::always_inline]] static i64 small_decimal(std::string_view text) {
        bool sign = text.front() == '-'; text.remove_prefix(sign); u64 value;
        if (text.size() <= 16) {
            auto index = _mm_add_epi8(_mm_setr_epi8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15), _mm_set1_epi8(int(text.size()) - 16));
            value = decimal16(_mm_shuffle_epi8(_mm_loadu_si128((const __m128i*)text.data()), index));
        } else {
            u64 high = text[0] - '0';
            if (text.size() == 18) high = high * 10 + text[1] - '0';
            value = high * 10000000000000000ull + decimal16(_mm_loadu_si128((const __m128i*)(text.data() + text.size() - 16)));
        }
        u64 mask = -u64(sign); return i64((value ^ mask) - mask);
    }
    [[gnu::always_inline]] static u64 hexadecimal(__m128i x) {
        // ASCII A..F (and a..f) need 9 added to their low nibble.
        x = _mm_add_epi8(_mm_and_si128(x, _mm_set1_epi8(15)),
            _mm_and_si128(_mm_cmpgt_epi8(x, _mm_set1_epi8('9')), _mm_set1_epi8(9)));
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x0110));
        x = _mm_packus_epi16(x, _mm_setzero_si128());
        return std::byteswap(u64(_mm_cvtsi128_si64(x)));
    }
    [[gnu::always_inline]] static u64 hexadecimal16(const char* p) {
        return hexadecimal(_mm_loadu_si128((const __m128i*)p));
    }
    // The Reader's padding permits a full load after a short token.
    [[gnu::always_inline]] static u64 hexadecimal_padded(const char* p, usize n) {
        // Negative shuffle indices zero-fill before the token's first n bytes.
        auto index = _mm_add_epi8(_mm_setr_epi8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15), _mm_set1_epi8(int(n) - 16));
        return hexadecimal(_mm_shuffle_epi8(_mm_loadu_si128((const __m128i*)p), index));
    }
    // A padded Reader token with at most 31 magnitude digits; sum of two fits i128.
    [[gnu::always_inline]] static i128 small_hex(std::string_view text) {
        bool sign = text.front() == '-'; text.remove_prefix(sign);
        u128 value = text.size() <= 16 ? hexadecimal_padded(text.data(), text.size())
            : (u128(hexadecimal_padded(text.data(), text.size() - 16)) << 64) | hexadecimal16(text.data() + text.size() - 16);
        i128 mask = -i128(sign); return (i128(value) ^ mask) - mask;
    }

    void assign(i128 value) {
        negative = value < 0; u128 mask = value >> 127, magnitude = (u128(value) ^ mask) - mask;
        digits.reserve(Hex ? 2 : 5); digits.n = 0;
        if constexpr (Hex) {
            digits[0] = magnitude; digits[1] = magnitude >> 64;
            digits.n = digits[1] ? 2 : digits[0] ? 1 : 0;
        } else while (magnitude) { digits[digits.n++] = magnitude % base; magnitude /= base; }
        if (!digits.n) negative = false;
    }

    void assign(std::string_view text) {
        negative = text.front() == '-'; text.remove_prefix(negative);
        constexpr usize width = Hex ? 16 : 8;
        usize length = text.size(), used = 0;
        digits.reserve((length + width - 1) / width);
        if constexpr (!Hex) {
            // Four independent 8-digit groups per vector. Each madd combines
            // decimal pairs; packing exposes the final 10^4 pair to madd16.
            while (length >= 32) {
                length -= 32;
                auto x = _mm256_and_si256(_mm256_loadu_si256((const __m256i*)(text.data() + length)), _mm256_set1_epi8(15));
                x = _mm256_maddubs_epi16(x, _mm256_set1_epi16(0x010a));
                x = _mm256_madd_epi16(x, _mm256_set1_epi32(0x00010064));
                x = _mm256_packus_epi32(x, x);
                x = _mm256_madd_epi16(x, _mm256_set1_epi32(0x00012710));
                auto y = _mm_unpacklo_epi64(_mm256_castsi256_si128(x), _mm256_extracti128_si256(x, 1));
                _mm_storeu_si128((__m128i*)(digits.p + used), _mm_shuffle_epi32(y, _MM_SHUFFLE(0, 1, 2, 3)));
                used += 4;
            }
        }
        while (length >= width) {
            length -= width;
            if constexpr (Hex) digits[used++] = hexadecimal16(text.data() + length);
            else digits[used++] = decimal8(text.data() + length);
        }
        if (length) {
            Limb value = 0;
            for (usize i = 0; i < length; ++i) {
                if constexpr (Hex) value = (value << 4) | ((text[i] & 15) + (text[i] > '9') * 9);
                else value = value * 10 + (text[i] - '0');
            }
            digits[used++] = value;
        }
        digits.n = used; trim();
    }

    static int compare_magnitude(const BigInteger& a, const BigInteger& b) {
        if (a.digits.n != b.digits.n) return a.digits.n < b.digits.n ? -1 : 1;
        for (usize i = a.digits.n; i--;) if (a.digits[i] != b.digits[i]) return a.digits[i] < b.digits[i] ? -1 : 1;
        return 0;
    }

    template<bool Subtract>
    static void magnitude(const BigInteger& a, const BigInteger& b, BigInteger& out) {
        usize n = a.digits.n, m = b.digits.n, i = 0; u32 carry = 0;
        out.digits.reserve(n + 1);
        const Limb* x = a.digits.p; const Limb* y = b.digits.p; Limb* dst = out.digits.p;
        if constexpr (!Hex) {
            auto bound = _mm256_set1_epi32(base), last = _mm256_set1_epi32(base - 1);
            auto bits = _mm256_setr_epi32(1, 2, 4, 8, 16, 32, 64, 128), zero = _mm256_setzero_si256();
            for (; i + 8 <= m; i += 8) {
                auto a = _mm256_loadu_si256((const __m256i*)(x + i)), b = _mm256_loadu_si256((const __m256i*)(y + i));
                auto value = Subtract ? _mm256_sub_epi32(a, b) : _mm256_add_epi32(a, b);
                auto generate = Subtract ? _mm256_cmpgt_epi32(b, a) : _mm256_cmpgt_epi32(value, last);
                auto propagate = Subtract ? _mm256_cmpeq_epi32(a, b) : _mm256_cmpeq_epi32(value, last);
                u32 g = _mm256_movemask_ps(_mm256_castsi256_ps(generate));
                u32 p = _mm256_movemask_ps(_mm256_castsi256_ps(propagate)), c = g | (p & carry);
                // Prefix carries/borrows, including a chain across all eight limbs.
                if (p) { c |= p & (c << 1); p &= p << 1; c |= p & (c << 2); p &= p << 2; c |= p & (c << 4); }
                auto incoming = _mm256_cmpgt_epi32(_mm256_and_si256(_mm256_set1_epi32((c << 1) | carry), bits), zero);
                auto outgoing = _mm256_and_si256(_mm256_cmpgt_epi32(_mm256_and_si256(_mm256_set1_epi32(c), bits), zero), bound);
                if constexpr (Subtract) value = _mm256_add_epi32(_mm256_add_epi32(value, incoming), outgoing);
                else value = _mm256_sub_epi32(_mm256_sub_epi32(value, incoming), outgoing);
                _mm256_storeu_si256((__m256i*)(dst + i), value); carry = c >> 7;
            }
        }
        for (; i < m; ++i) {
            if constexpr (Hex) {
                unsigned long long value;
                if constexpr (Subtract) carry = _subborrow_u64(carry, x[i], y[i], &value);
                else carry = _addcarry_u64(carry, x[i], y[i], &value);
                dst[i] = value;
            } else if constexpr (Subtract) {
                u32 value = y[i] + carry; carry = x[i] < value; dst[i] = x[i] - value + carry * base;
            } else {
                u32 value = x[i] + y[i] + carry; carry = value >= base; dst[i] = value - carry * base;
            }
        }
        for (; carry && i < n; ++i) {
            if constexpr (Hex) {
                unsigned long long value;
                if constexpr (Subtract) carry = _subborrow_u64(carry, x[i], 0, &value);
                else carry = _addcarry_u64(carry, x[i], 0, &value);
                dst[i] = value;
            } else if constexpr (Subtract) { carry = x[i] == 0; dst[i] = x[i] - 1 + carry * base; }
            else { u32 value = x[i] + 1; carry = value == base; dst[i] = value - carry * base; }
        }
        if (i < n && dst != x) memcpy(dst + i, x + i, (n - i) * sizeof(Limb));
        if constexpr (!Subtract) dst[n++] = carry;
        out.digits.n = n; out.trim();
    }

    // out may alias either input.
    static void add(const BigInteger& a, const BigInteger& b, BigInteger& out) {
        bool sign;
        if (a.negative == b.negative) {
            sign = a.negative;
            if (a.digits.n >= b.digits.n) magnitude<false>(a, b, out);
            else magnitude<false>(b, a, out);
        } else {
            int order = compare_magnitude(a, b);
            if (!order) { out.digits.n = 0; out.negative = false; return; }
            if (order > 0) { sign = a.negative; magnitude<true>(a, b, out); }
            else { sign = b.negative; magnitude<true>(b, a, out); }
        }
        out.negative = out.digits.n && sign;
    }

    Buffer<u32> chunks() const {
        auto result = fft_detail::storage<u32>(Hex ? (digits.n * 64 + 13) / 14 : 2 * digits.n);
        if (!digits.n) return result;
        if constexpr (Hex) {
            usize total = result.n, i = 0;
            usize vector_end = ((digits.n * 64 - 32) / 14 + 1) & -usize(8);
            auto bit = _mm256_setr_epi32(0,14,28,42,56,70,84,98), step = _mm256_set1_epi32(112);
            // A 4-byte window covers 14 bits plus the at-most-7-bit byte offset.
            // The scalar tail avoids reading beyond the final native limb.
            for (; i < vector_end; i += 8) {
                auto offset = _mm256_srli_epi32(bit, 3), shift = _mm256_and_si256(bit, _mm256_set1_epi32(7));
                auto value = _mm256_i32gather_epi32((const int*)digits.p, offset, 1);
                value = _mm256_and_si256(_mm256_srlv_epi32(value, shift), _mm256_set1_epi32(16383));
                _mm256_storeu_si256((__m256i*)(result.p + i), value); bit = _mm256_add_epi32(bit, step);
            }
            for (; i < total; ++i) {
                usize position = 14 * i, word = position / 64; unsigned shift = position % 64;
                u64 value = digits[word] >> shift;
                if (shift && word + 1 < digits.n) value |= digits[word + 1] << (64 - shift);
                result[i] = value & 16383;
            }

        } else for (usize i = 0; i < digits.n; ++i) result[2 * i] = digits[i] % 10000, result[2 * i + 1] = digits[i] / 10000;
        while (result.n && !result[result.n - 1]) --result.n;
        return result;
    }

    // Caller reserves enough native limbs; coefficients are nonnegative.
    static void carry_product(std::span<const u64> product, BigInteger& out) {
            usize used = 0; u64 carry = 0;
            if constexpr (Hex) {
                u128 word = 0; int bits = 0; usize i = 0;
                // Accumulate the uncarried coefficients directly into native words;
                // their <2^52 bound leaves ample room for the at-most-63-bit shift.
                while (i < product.size() || word) {
                    while (bits < 64 && i < product.size()) { word += u128(product[i++]) << bits; bits += 14; }
                    out.digits[used++] = word; word >>= 64; bits = bits >= 64 ? bits - 64 : 0;
                }
            } else {
                usize i = 0;
                // Combine two base-10^4 coefficients before carrying in base 10^8.
                // The bounded digit convolution keeps this sum below 2^64.
                for (; i + 1 < product.size(); i += 2) {
                    u64 value = product[i] + product[i + 1] * 10000 + carry;
                    out.digits[used++] = value % base; carry = value / base;
                }
                if (i < product.size()) {
                    u64 value = product[i] + carry; out.digits[used++] = value % base; carry = value / base;
                }
                while (carry) { out.digits[used++] = carry % base; carry /= base; }
            }
            out.digits.n = used;
        out.negative = false; out.trim();
    }

    static void multiply(const BigInteger& a, const BigInteger& b, BigInteger& out, IntegerFFT* workspace = nullptr) {
        if (!a.digits.n || !b.digits.n) { out.digits.n = 0; out.negative = false; return; }
        if (&out == &a || &out == &b) { BigInteger temp; multiply(a, b, temp, workspace); out = std::move(temp); return; }
        const BigInteger* x = &a; const BigInteger* y = &b;
        if (x->digits.n < y->digits.n) std::swap(x, y);
        usize n = x->digits.n, m = y->digits.n;
        if (out.digits.capacity < n + m + 1) out.digits = fft_detail::storage<Limb>(n + m + 1);
        if (m <= 64) {
            if constexpr (Hex) {
                std::fill(out.digits.p, out.digits.p + n + m, 0ull);
                for (usize j = 0; j < m; ++j) {
                    u64 carry = 0;
                    for (usize i = 0; i < n; ++i) {
                        u128 value = u128(x->digits[i]) * y->digits[j] + out.digits[i + j] + carry;
                        out.digits[i + j] = value; carry = value >> 64;
                    }
                    out.digits[n + j] = carry;
                }
            } else {
                u64 carry = 0;
                for (usize k = 0; k < n + m - 1; ++k) {
                    u64 value = carry;
                    for (usize j = k < n ? 0 : k - n + 1; j < std::min(m, k + 1); ++j) value += u64(x->digits[k - j]) * y->digits[j];
                    out.digits[k] = value % base; carry = value / base;
                }
                out.digits[n + m - 1] = carry;
            }
            out.digits.n = n + m;
        } else {
            // Small coefficients fit the validated floating-point FFT range.
            // Larger transforms fall back to exact two-prime reconstruction.
            bool square = a.digits.n == b.digits.n && memcmp(a.digits.p, b.digits.p, a.digits.n * sizeof(Limb)) == 0;
            auto left = a.chunks(); Buffer<u32> right;
            if (!square) right = b.chunks();
            std::span<const u32> rhs = square ? std::span<const u32>(left) : std::span<const u32>(right);
            auto product = workspace ? (*workspace)(left, rhs) : convolution_fft_integer(left, rhs);
            carry_product(std::span<const u64>(product), out);
        }
        out.negative = a.negative != b.negative; out.trim();
    }

    [[gnu::always_inline]] static __m128i hex_digits(u64 value) {
        auto x = _mm_cvtsi64_si128(std::byteswap(value)), mask = _mm_set1_epi8(15);
        auto nibbles = _mm_unpacklo_epi8(_mm_and_si128(_mm_srli_epi16(x, 4), mask), _mm_and_si128(x, mask));
        return _mm_shuffle_epi8(_mm_setr_epi8('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'), nibbles);
    }
    template<usize N>
    void write(Writer<N>& out, char end = '\n') const {
        if (!digits.n) { out.put('0'); out.put(end); return; }
        if (usize(out.p - out.buf) > N - 32) out.flush();
        if (negative) *out.p++ = '-';
        if constexpr (Hex) {
            int size = (64 - std::countl_zero(digits[digits.n - 1]) + 3) / 4;
            auto x = _mm_shuffle_epi8(hex_digits(digits[digits.n - 1]), _mm_loadu_si128((const __m128i*)io_detail::trim[size].data()));
            _mm_storeu_si128((__m128i*)out.p, x); out.p += size;
        } else out.p = Writer<N>::number(out.p, digits[digits.n - 1]);
        constexpr usize width = Hex ? 16 : 8;
        for (usize remaining = digits.n - 1; remaining;) {
            usize count = std::min(remaining, usize(out.buf + N - out.p) / width);
            if (!count) { out.flush(); continue; }
            char* cursor = out.p;
            for (usize stop = remaining - count; remaining != stop;) {
                auto value = digits[--remaining];
                if constexpr (Hex) { _mm_storeu_si128((__m128i*)cursor, hex_digits(value)); cursor += 16; }
                else cursor = Writer<N>::template fixed<8>(cursor, value);
            }
            out.p = cursor;
        }
        out.put(end);
    }
};
}
