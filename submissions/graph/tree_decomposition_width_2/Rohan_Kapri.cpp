
#if defined(__GNUC__) && !defined(__clang__)
#include <bits/stdc++.h>
#include <immintrin.h>
#pragma GCC target("avx2")
#ifndef __AVX2__
#define __AVX2__ 1
#endif
#endif
#ifndef CANARD_CLIENT_AVX2
#define CANARD_CLIENT_AVX2 1
#endif


#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

namespace canard::io {
    
template <std::unsigned_integral T, T Maximum>
    requires(!std::same_as<T, bool> && !std::same_as<T, char> && sizeof(T) <= 8)
struct bounded {
    using value_type = T;
    static constexpr T maximum = Maximum;
};

template <typename... Fields> struct record {};

namespace detail {

constexpr unsigned decimal_digits(std::uint64_t value) noexcept {
    unsigned result = 1;
    while (value >= 10) {
        value /= 10;
        ++result;
    }
    return result;
}

template <typename T>
concept integer =
    std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, char> && sizeof(T) <= 8;

template <typename Schema> struct field_traits {
    static constexpr bool small_unsigned = false;
};

template <integer T> struct field_traits<T> {
    using value_type = T;
    static constexpr bool bounded_value = false;
    static constexpr unsigned digits = decimal_digits(std::numeric_limits<T>::max());
    static constexpr bool small_unsigned = std::is_unsigned_v<T> && sizeof(T) <= 4;
};

template <std::unsigned_integral T, T Maximum> struct field_traits<bounded<T, Maximum>> {
    using value_type = T;
    static constexpr bool bounded_value = true;
    static constexpr T maximum = Maximum;
    static constexpr unsigned digits = decimal_digits(Maximum);
    static constexpr bool small_unsigned = Maximum <= std::numeric_limits<std::uint32_t>::max();
};

template <typename Schema>
concept field_schema = requires { typename field_traits<Schema>::value_type; };

template <typename Schema> using field_value_t = typename field_traits<Schema>::value_type;

template <typename Schema> struct schema_traits;

template <field_schema Schema> struct schema_traits<Schema> {
    using fields = std::tuple<Schema>;
    using value_type = field_value_t<Schema>;
    static constexpr std::size_t arity = 1;
};

template <field_schema... Fields>
    requires(sizeof...(Fields) > 0)
struct schema_traits<record<Fields...>> {
    using fields = std::tuple<Fields...>;
    using value_type = std::tuple<field_value_t<Fields>...>;
    static constexpr std::size_t arity = sizeof...(Fields);
};

template <field_schema... Fields>
    requires(sizeof...(Fields) > 0)
struct schema_traits<std::tuple<Fields...>> : schema_traits<record<Fields...>> {};

template <field_schema First, field_schema Second>
struct schema_traits<std::pair<First, Second>> : schema_traits<record<First, Second>> {
    using value_type = std::pair<field_value_t<First>, field_value_t<Second>>;
};

template <typename... Fields> struct requested_schema {
    using type = record<Fields...>;
};

template <typename Field> struct requested_schema<Field> {
    using type = Field;
};

template <typename... Fields> using requested_schema_t = typename requested_schema<Fields...>::type;

template <typename Schema> using schema_value_t = typename schema_traits<Schema>::value_type;

template <typename Schema, std::size_t Index>
using schema_field_t = std::tuple_element_t<Index, typename schema_traits<Schema>::fields>;

template <typename Schema>
concept readable_schema = requires { typename schema_traits<Schema>::value_type; };

template <typename T> struct is_tuple : std::false_type {};

template <typename... Elements> struct is_tuple<std::tuple<Elements...>> : std::true_type {};

template <typename First, typename Second>
struct is_tuple<std::pair<First, Second>> : std::true_type {};

template <typename T> inline constexpr bool is_tuple_v = is_tuple<std::remove_cvref_t<T>>::value;

template <typename T>
struct numeric_record : std::bool_constant<integer<std::remove_cvref_t<T>>> {};

template <typename... Elements>
struct numeric_record<std::tuple<Elements...>>
    : std::bool_constant<(integer<std::remove_cvref_t<Elements>> && ...)> {};

template <typename First, typename Second>
struct numeric_record<std::pair<First, Second>>
    : std::bool_constant<integer<std::remove_cvref_t<First>> &&
                         integer<std::remove_cvref_t<Second>>> {};

template <typename T>
concept integer_record = numeric_record<std::remove_cvref_t<T>>::value;

} // namespace detail
} // namespace canard::io
// ===== include/canard/io/text.hpp =====
// Small textual protocol fields, sharing the numeric reader's logical cursor.
#include <cstring>
#include <stdexcept>
#include <string_view>
namespace canard::io {
struct expect { std::string_view token; };
namespace detail {
constexpr bool ascii_space(unsigned char c) noexcept {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f';
}
inline void validate_literal(std::string_view token) {
    if (token.empty()) throw std::invalid_argument("canard: empty expected token");
    for (unsigned char c : token)
        if (ascii_space(c)) throw std::invalid_argument("canard: expected token contains whitespace");
}
inline void expect_view_token(const char*& cursor, const char* end, std::string_view token) {
    validate_literal(token);
    while (cursor != end && ascii_space(static_cast<unsigned char>(*cursor))) ++cursor;
    if (static_cast<std::size_t>(end - cursor) < token.size() ||
        std::memcmp(cursor, token.data(), token.size()) != 0)
        throw std::runtime_error("canard: unexpected literal token");
    cursor += token.size();
    if (cursor != end && !ascii_space(static_cast<unsigned char>(*cursor)))
        throw std::runtime_error("canard: literal is not a complete token");
}
template <typename Source>
void expect_stream_token(Source& source, std::string_view token) {
    validate_literal(token);
    for (;;) {
        source.ensure(1);
        const auto w = source.window();
        std::size_t n = 0;
        while (n != w.size && ascii_space(static_cast<unsigned char>(w.data[n]))) ++n;
        source.consume(n);
        if (n != w.size || w.size == 0) break;
    }
    for (char c : token) {
        source.ensure(1);
        const auto w = source.window();
        if (w.size == 0 || w.data[0] != c)
            throw std::runtime_error("canard: unexpected literal token");
        source.consume(1);
    }
    source.ensure(1);
    const auto w = source.window();
    if (w.size != 0 && !ascii_space(static_cast<unsigned char>(w.data[0])))
        throw std::runtime_error("canard: literal is not a complete token");
}
} // namespace detail
} // namespace canard::io
// ===== include/canard/io/detail/record_decoder.hpp =====

// ===== include/canard/io/detail/target.hpp =====
// ===== include/canard/execution.hpp =====
namespace canard::execution {
// Source-level portable operations. Compiler target flags still govern auto-vectorization.
struct scalar {};
// Explicit opt-in backend; no runtime dispatch or implicit fallback.
struct avx2 {};
}
// ===== include/canard/io/decimal/ascii_scalar.hpp =====
// ===== include/canard/io/decimal/field.hpp =====
namespace canard::io::decimal {
struct field {
    const char* data;
    unsigned digits;
};
}

namespace canard::io::decimal {
struct u64_field {
    const char* source;
    unsigned digits;
};
}
#include <array>
#include <cstdint>
namespace canard::io::decimal {
// Same trusted record grammar and field-width contracts as ascii_avx2_codec.
// Only execution changes; no leading-space or delimiter fallback is hidden here.
struct ascii_scalar_codec {
    [[nodiscard]] static std::uint32_t delimiter_mask(const char* p) noexcept {
        std::uint32_t mask = 0;
        for (unsigned i = 0; i < 32; ++i)
            mask |= std::uint32_t(static_cast<unsigned char>(p[i]) < '0') << i;
        return mask;
    }
    [[nodiscard]] static std::uint32_t decode(field f) noexcept {
        std::uint32_t value = 0;
        for (unsigned i = 0; i < f.digits; ++i)
            value = value * 10 + (f.data[i] - '0');
        return value;
    }
    template <unsigned A = 9, unsigned B = 9>
    [[nodiscard]] static std::array<std::uint32_t, 2> decode_pair(field a, field b) noexcept {
        static_assert(1 <= A && A <= 10 && 1 <= B && B <= 10);
        return {decode(a), decode(b)};
    }
    template <unsigned A = 9, unsigned B = 9, unsigned C = 9>
    [[nodiscard]] static std::array<std::uint32_t, 3> decode_triplet(field a, field b, field c) noexcept {
        static_assert(1 <= A && A <= 10 && 1 <= B && B <= 10 && 1 <= C && C <= 10);
        return {decode(a), decode(b), decode(c)};
    }
    template <unsigned A = 9>
    [[nodiscard]] static std::array<std::uint32_t, 3> decode_three(field a, field b, field c) noexcept {
        return decode_triplet<A>(a, b, c);
    }
    [[nodiscard]] static std::array<std::uint32_t, 4>
    decode_four_short(field a, field b, field c, field d) noexcept {
        return {decode(a), decode(b), decode(c), decode(d)};
    }
};
}
// ===== include/canard/io/detail/basic_trusted_ascii_reader.hpp =====
// ===== include/canard/config/compiler.hpp =====
#if defined(__GNUC__) || defined(__clang__)
# define CANARD_ALWAYS_INLINE [[gnu::always_inline]]
# define CANARD_NOINLINE [[gnu::noinline]]
#elif defined(_MSC_VER)
# define CANARD_ALWAYS_INLINE __forceinline
# define CANARD_NOINLINE __declspec(noinline)
#else
# define CANARD_ALWAYS_INLINE
# define CANARD_NOINLINE
#endif
namespace canard::detail {
template <int ReadWrite = 0, int Locality = 3>
inline void prefetch(const void* address) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_prefetch(address, ReadWrite, Locality);
#else
    (void)address;
#endif
}
}
// ===== include/canard/io/padded_bytes_view.hpp =====
#include <cstddef>
namespace canard::io {
// Borrowed trusted storage. data[-8..size+31] must be readable. Padding is not
// automatically promised by string_view/span. The owner outlives every reader.
struct padded_bytes_view {
    const char* data;
    std::size_t size;
    static constexpr std::size_t prefix_padding = 8;
    static constexpr std::size_t suffix_padding = 32;
};
}
#include <array>
#include <bit>
#include <cstring>
#include <cstdint>
namespace canard::io {
// Borrowing reader over padded bytes; lifetime is controlled by the caller.
// Packed records require exactly one delimiter between fields, <=32 bytes
// including the terminating delimiter, and the stated maximum digit counts.
// read_tagged_triplet(): one-digit tag, then three fields. read_pair(): two
// 1..9-digit fields. read_u32() also accepts repeated leading whitespace.
template <typename Codec>
class basic_trusted_ascii_reader {
    const char* cursor_;
    void skip_whitespace() {
        while (static_cast<unsigned char>(*cursor_) <= ' ') {
            ++cursor_;
        }
    }

    template <typename Word> [[nodiscard]] static constexpr bool all_digits(Word word) noexcept {
        constexpr Word zeros =
            sizeof(Word) == 8 ? static_cast<Word>(0x3030'3030'3030'3030ull) : Word{0x3030'3030u};
        constexpr Word high =
            sizeof(Word) == 8 ? static_cast<Word>(0xf0f0'f0f0'f0f0'f0f0ull) : Word{0xf0f0'f0f0u};
        constexpr Word sixes =
            sizeof(Word) == 8 ? static_cast<Word>(0x0606'0606'0606'0606ull) : Word{0x0606'0606u};
        return (word & high) == zeros && ((word + sixes) & high) == zeros;
    }

    template <typename Word> [[nodiscard]] std::uint32_t read_unsigned() {
        if constexpr (std::endian::native != std::endian::little) {
            skip_whitespace();
            std::uint32_t value = 0;
            while (*cursor_ >= '0' && *cursor_ <= '9')
                value = value * 10 + (*cursor_++ - '0');
            return value;
        }
        skip_whitespace();
        Word word;
        std::memcpy(&word, cursor_, sizeof(word));
        std::uint32_t result = 0;
        if (all_digits(word)) {
            if constexpr (sizeof(Word) == 8) {
                word ^= 0x3030'3030'3030'3030ull;
                word = (word * 10 + (word >> 8)) & 0x00ff'00ff'00ff'00ffull;
                word = (word * 100 + (word >> 16)) & 0x0000'ffff'0000'ffffull;
                word = (word * 10'000 + (word >> 32)) & 0x0000'0000'ffff'ffffull;
            } else {
                word ^= 0x3030'3030u;
                word = (word * 10 + (word >> 8)) & 0x00ff'00ffu;
                word = (word * 100 + (word >> 16)) & 0x0000'ffffu;
            }
            result = static_cast<std::uint32_t>(word);
            cursor_ += sizeof(word);
        }
        // Valid judge input contains only values fitting in uint32_t.
        for (unsigned digit; (digit = static_cast<unsigned>(*cursor_ - '0')) < 10; ++cursor_) {
            result = result * 10 + digit;
        }
        return result;
    }

  public:
    // Two pairs of 1..MaxDigits unsigned fields, each followed by one delimiter.
    // The complete four-field record fits in 32 readable bytes; MaxDigits <= 7.
    template <unsigned MaxDigits = 6>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 4>
    read_two_index_pairs() noexcept {
        static_assert(MaxDigits >= 1 && MaxDigits <= 7);
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned p0 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p1 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p2 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p3 = std::countr_zero(mask);
        const auto result = Codec::decode_four_short({cursor_, p0},
                                                       {cursor_ + p0 + 1, p1 - p0 - 1},
                                                       {cursor_ + p1 + 1, p2 - p1 - 1},
                                                       {cursor_ + p2 + 1, p3 - p2 - 1});
        cursor_ += p3 + 1;
        return result;
    }

    explicit basic_trusted_ascii_reader(padded_bytes_view input) noexcept : cursor_(input.data) {}
    // One codec delimiter mask per record; decimal reductions use packed
    // byte-to-pair, pair-to-quad, and quad-to-eight-digit multiply-adds.
    // A ninth leading digit is added separately. Field lengths are trusted.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 2> read_pair() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = Codec::template decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second;
        ++cursor_;
        return {a, b};
    }
    template <unsigned FirstMaxDigits = 9,
              unsigned SecondMaxDigits = 9,
              unsigned ThirdMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3> read_triplet() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        const auto result =
            Codec::template decode_triplet<FirstMaxDigits, SecondMaxDigits, ThirdMaxDigits>(
                {cursor_, first},
                {cursor_ + first + 1, second - first - 1},
                {cursor_ + second + 1, third - second - 1});
        cursor_ += third + 1;
        return result;
    }

    // Exactly one one-digit tag and two bounded decimal fields.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
    read_tagged_pair() noexcept {
        skip_whitespace();
        const unsigned type = static_cast<unsigned>(cursor_[0] - '0');
        cursor_ += 2;
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = Codec::template decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second + 1;
        return {type, a, b};
    }

    template <unsigned FirstMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 4> read_tagged_triplet() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned fourth = std::countr_zero(mask);
        const auto type = static_cast<unsigned>(cursor_[0] - '0');
        const auto [position, a, b] =
            Codec::template decode_three<FirstMaxDigits>({cursor_ + 2, second - 2},
                                                  {cursor_ + second + 1, third - second - 1},
                                                  {cursor_ + third + 1, fourth - third - 1});
        cursor_ += fourth;
        ++cursor_;
        return {type, position, a, b};
    }

    // Scalar/SWAR parser for general uint32 values, including ten digits.
    // MaxDigits selects a 4- or 8-byte fast prefix, not a run-time validator.
    template <unsigned MaxDigits = 10> [[nodiscard]] std::uint32_t read_u32() {
        static_assert(1 <= MaxDigits && MaxDigits <= 10);
        if constexpr (MaxDigits <= 6)
            return read_unsigned<std::uint32_t>();
        else
            return read_unsigned<std::uint64_t>();
    }
    // Signed input, including ten-digit magnitudes. Successful int32 parsing
    // is a precondition; unsigned subtraction also handles INT32_MIN exactly.
    [[nodiscard]] std::int32_t read_i32() {
        skip_whitespace();
        const bool negative = *cursor_ == '-';
        cursor_ += negative;
        const auto magnitude = read_unsigned<std::uint64_t>();
        return std::bit_cast<std::int32_t>(negative ? 0u - magnitude : magnitude);
    }
    [[nodiscard]] const char* position() const noexcept {
        return cursor_;
    }
};
} // namespace canard::io
// ===== include/canard/io/decimal/decode_u64.hpp =====
// ===== include/canard/io/decimal/integer_types.hpp =====
#include <cstdint>
namespace canard::io::decimal::detail {
using u64 = std::uint64_t;
using u32 = std::uint32_t;
}

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>

namespace canard::io::decimal {

// A trusted unsigned decimal field. It has 1..20 digits, its numeric value fits
// uint64_t, and source[-8..digits+15] is readable. Extra bytes may be arbitrary.


namespace detail {



// Masks align the final sixteen digits into two groups of eight. For a field
// shorter than eight digits the first group is all zero; its source load is
// clamped to source - 8 rather than reading before the borrowing contract.
alignas(64) inline constexpr auto u64_tail_masks = [] {
    std::array<std::array<u64, 2>, 21> masks{};
    for (unsigned n = 1; n <= 20; ++n) {
        const unsigned low = std::min(n, 8u);
        const unsigned high = std::min(n, 16u) - low;
        masks[n][0] = high ? 0x0f0f'0f0f'0f0f'0f0full << ((8 - high) * 8) : 0;
        masks[n][1] = 0x0f0f'0f0f'0f0f'0f0full << ((8 - low) * 8);
    }
    return masks;
}();

inline constexpr auto u64_leading_weights = [] {
    std::array<u64, 21> weights{};
    for (unsigned n = 17; n <= 20; ++n)
        weights[n] = 10'000'000'000'000'000ull;
    return weights;
}();

[[nodiscard]] inline std::array<u64, 2> u64_tail(u64_field field) noexcept {
    u64 high, low;
    std::memcpy(&low, field.source + field.digits - 8, 8);
    std::memcpy(&high, field.source + std::max(field.digits, 8u) - 16, 8);
    return {high & u64_tail_masks[field.digits][0], low & u64_tail_masks[field.digits][1]};
}

[[nodiscard]] inline u64 u64_leading(u64_field field) noexcept {
    u32 word;
    std::memcpy(&word, field.source, 4);
    // Only fields with 17..20 digits have a nonzero weight. Masking the shift
    // count keeps every shorter-field path defined without a data-dependent
    // branch. Garbage outside its actual prefix is shifted out or multiplied
    // by zero, never used as a table index.
    word = (word & 0x0f0f'0f0fu) << (((20 - field.digits) & 3u) * 8);
    word = (word * 10 + (word >> 8)) & 0x00ff'00ffu;
    return u64{(word * 100 + (word >> 16)) & 0xffffu} * u64_leading_weights[field.digits];
}

[[nodiscard]] inline u64 reduce_eight_digits(u64 word) noexcept {
    word = (word * 10 + (word >> 8)) & 0x00ff'00ff'00ff'00ffull;
    word = (word * 100 + (word >> 16)) & 0x0000'ffff'0000'ffffull;
    return (word * 10'000 + (word >> 32)) & 0xffff'ffffull;
}

} // namespace detail

// Portable conversion: SWAR on little-endian targets, byte arithmetic otherwise. The delimiter vocabulary is exactly
// space, LF, CR or tab, with one delimiter between fields (CRLF is two). Zero
// bytes may act as the final EOF delimiter; they are not interior input.
struct u64_scalar_codec {
    using value_type = std::uint64_t;


    [[nodiscard]] static std::uint32_t delimiters32(const char* source) noexcept {
        if constexpr (std::endian::native != std::endian::little) {
            std::uint32_t mask = 0;
            for (unsigned i = 0; i < 32; ++i)
                mask |= std::uint32_t(static_cast<unsigned char>(source[i]) < '0') << i;
            return mask;
        }
        std::uint32_t result = 0;
        for (unsigned group = 0; group < 4; ++group) {
            std::uint64_t word;
            std::memcpy(&word, source + 8 * group, 8);
            const auto flags = (~word >> 4) & 0x0101'0101'0101'0101ull;
            const auto bits = (flags * 0x0102'0408'1020'4080ull) >> 56;
            result |= static_cast<std::uint32_t>(bits) << (8 * group);
        }
        return result;
    }

    [[nodiscard]] static value_type decode(u64_field field) noexcept {
        if constexpr (std::endian::native != std::endian::little) {
            value_type value = 0;
            for (unsigned i = 0; i < field.digits; ++i)
                value = value * 10 + (field.source[i] - '0');
            return value;
        }
        const auto [high, low] = detail::u64_tail(field);
        return detail::reduce_eight_digits(high) * 100'000'000ull +
               detail::reduce_eight_digits(low) + detail::u64_leading(field);
    }

    [[nodiscard]] static std::array<value_type, 2> decode_pair(u64_field first,
                                                            u64_field second) noexcept {
        return {decode(first), decode(second)};
    }
};

} // namespace canard::io::decimal
// ===== include/canard/io/decimal/encode_u64.hpp =====
// ===== include/canard/io/decimal_format.hpp =====
#include <array>
#include <bit>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <limits>
namespace canard::io::decimal {
inline constexpr auto four_digits = [] consteval {
    std::array<std::array<char, 4>, 10'000> result{};
    for (unsigned value = 0; value < result.size(); ++value) {
        result[value] = {static_cast<char>('0' + value / 1000),
                         static_cast<char>('0' + value / 100 % 10),
                         static_cast<char>('0' + value / 10 % 10),
                         static_cast<char>('0' + value % 10)};
    }
    return result;
}();

// Writes canonical decimal plus newline; at least 16 writable bytes supplied.
// Precondition: value <= MaxValue. Default handles all uint32 values;
// a nine-digit bound preserves the original solver's one-byte leading group.
template <std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max()>
[[nodiscard]] inline char* format_u32_line(char* cursor, char* end, std::uint32_t value) noexcept {
    // Most answers have nine digits. Emit their eight low digits with
    // two fixed-size copies; use to_chars only for a short leading group.
    auto append_four = [&cursor](unsigned group) noexcept {
        std::memcpy(cursor, four_digits[group].data(), 4);
        cursor += 4;
    };
    if (value >= 100'000'000) {
        unsigned high = value / 100'000'000;
        value %= 100'000'000;
        if constexpr (MaxValue < 1'000'000'000) {
            *cursor++ = static_cast<char>('0' + high);
        } else {
            cursor = std::to_chars(cursor, end, high).ptr;
        }
        append_four(value / 10'000);
        append_four(value % 10'000);
    } else if (value >= 10'000) {
        cursor = std::to_chars(cursor, end, value / 10'000).ptr;
        append_four(value % 10'000);
    } else {
        cursor = std::to_chars(cursor, end, value).ptr;
    }
    *cursor++ = '\n';
    return cursor;
}
} // namespace canard::io::decimal

namespace canard::io::decimal {
// At least 24 writable bytes. Canonical uint64_t decimal, no delimiter.
// Reuses the shared base-10^4 table; first-group trimming uses one word copy.
[[nodiscard]] inline char* format_u64_token(char* cursor, std::uint64_t value) noexcept {
    if constexpr (std::endian::native != std::endian::little)
        return std::to_chars(cursor, cursor + 24, value).ptr;
    auto group = [&cursor](unsigned x) {
        std::memcpy(cursor, four_digits[x].data(), 4);
        cursor += 4;
    };
    auto first = [&cursor](unsigned x) {
        const unsigned skip = 3 - (x >= 10) - (x >= 100) - (x >= 1000);
        std::uint32_t word;
        std::memcpy(&word, four_digits[x].data(), 4);
        word >>= 8 * skip;
        std::memcpy(cursor, &word, 4);
        cursor += 4 - skip;
    };
    if (value >= 10'000'000'000'000'000ULL) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000,
                            c = value / 1'000'000'000'000ULL, d = value / 10'000'000'000'000'000ULL;
        first(d);
        group(c - d * 10'000);
        group(b - c * 10'000);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 1'000'000'000'000ULL) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000,
                            c = value / 1'000'000'000'000ULL;
        first(c);
        group(b - c * 10'000);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 100'000'000) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000;
        first(b);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 10'000) {
        const std::uint64_t a = value / 10'000;
        first(a);
        group(value - a * 10'000);
    } else
        first(value);
    return cursor;
}

// Canonical decimal token, no delimiter. At least 16 writable bytes are required.
// The first group suppresses leading zeros; all following groups have four digits.
// Like the existing formatter, writes may touch unused bytes after the token.
[[nodiscard]] inline char* format_u32_compact(char* cursor, std::uint32_t value) noexcept {
    auto first = [&](std::uint32_t x) {
        if (x < 10) {
            *cursor++ = static_cast<char>('0' + x);
            return;
        }
        const unsigned skip = 3 - unsigned(x >= 10) - unsigned(x >= 100) - unsigned(x >= 1000);
        if constexpr (std::endian::native == std::endian::little) {
            std::uint32_t word;
            std::memcpy(&word, four_digits[x].data(), 4);
            word >>= 8 * skip;
            std::memcpy(cursor, &word, 4);
        } else {
            std::memcpy(cursor, four_digits[x].data() + skip, 4 - skip);
        }
        cursor += 4 - skip;
    };
    auto next = [&](std::uint32_t x) {
        std::memcpy(cursor, four_digits[x].data(), 4);
        cursor += 4;
    };
    if (value >= 100000000u) {
        first(value / 100000000u);
        next(value / 10000u % 10000u);
        next(value % 10000u);
    } else if (value >= 10000u) {
        first(value / 10000u);
        next(value % 10000u);
    } else
        first(value);
    return cursor;
}
} // namespace canard::io::decimal
namespace canard::io::decimal {
// Canonical decimal, no delimiter. The writer supplies at least token_capacity
// writable bytes; a conversion may write beyond its returned logical end.
struct u64_table_formatter {
    using value_type = std::uint64_t;
    static constexpr unsigned token_capacity = 24;

    [[nodiscard]] char* operator()(char* destination, value_type value) const noexcept {
        return format_u64_token(destination, value);
    }
};


namespace detail {

// A uint64_t has a leading base-10^16 group in [0,1844]. One 64-bit table
// entry contains already-trimmed ASCII in its low word and length in its high
// word. This does not require another full 10,000-entry decimal table.
inline constexpr auto u64_leading_groups = [] {
    constexpr auto count = std::numeric_limits<std::uint64_t>::max() /
                           10'000'000'000'000'000ull + 1;
    std::array<std::uint64_t, count> table{};
    for (unsigned i = 0; i < count; ++i) {
        const unsigned skip = 3 - (i >= 10) - (i >= 100) - (i >= 1000);
        const auto word = std::bit_cast<std::uint32_t>(four_digits[i]) >> (8 * skip);
        table[i] = word | (std::uint64_t{4 - skip} << 32);
    }
    return table;
}();

// Precondition: value >= 10^16, and 24 destination bytes are writable. Split
// twice at base 10^8; the remaining /10000 quotients have 32-bit operands.
[[nodiscard]] inline char* format_u64_large(char* destination, std::uint64_t value) noexcept {
    const auto high = value / 100'000'000;
    const auto top = value / 10'000'000'000'000'000ull;
    const auto middle = static_cast<std::uint32_t>(high - top * 100'000'000);
    const auto low = static_cast<std::uint32_t>(value - high * 100'000'000);
    const auto leading = u64_leading_groups[top];
    std::memcpy(destination, &leading, 4);
    destination += leading >> 32;
    std::memcpy(destination, four_digits[middle / 10'000].data(), 4);
    std::memcpy(destination + 4, four_digits[middle % 10'000].data(), 4);
    std::memcpy(destination + 8, four_digits[low / 10'000].data(), 4);
    std::memcpy(destination + 12, four_digits[low % 10'000].data(), 4);
    return destination + 16;
}

} // namespace detail
} // namespace canard::io::decimal
#if defined(__AVX2__)
// ===== include/canard/io/avx2/trusted_ascii_reader.hpp =====
// ===== include/canard/io/decimal_avx2.hpp =====
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#ifndef __AVX2__
#error "The packed decimal backend requires AVX2."
#endif
namespace canard::io::decimal {
static_assert(std::endian::native == std::endian::little);

// Borrowing field in padded ASCII storage. digits is 1..10, and the eight
// bytes preceding data+digits must be readable, even for a short field.


namespace detail {
inline constexpr int pair_weights = 0x010a;      // [10, 1]
inline constexpr int quad_weights = 0x00010064;  // [100, 1]
inline constexpr int octet_weights = 0x00012710; // [10000, 1]
inline constexpr auto nibble_mask = [] consteval {
    std::array<std::uint64_t, 11> masks{};
    for (unsigned digits = 1; digits <= 10; ++digits)
        masks[digits] = 0x0f0f'0f0f'0f0f'0f0full << (digits < 8 ? (8 - digits) * 8 : 0);
    return masks;
}();
inline constexpr std::array<unsigned, 10> ninth_digit_weight{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 100'000'000};
[[nodiscard]] inline std::uint64_t eight_byte_tail(field input) noexcept {
    std::uint64_t bytes;
    std::memcpy(&bytes, input.data + input.digits - 8, 8);
    return bytes;
}
[[nodiscard]] inline std::uint32_t ninth_digit(field input) noexcept {
    return static_cast<unsigned>(input.data[0] - '0') * ninth_digit_weight[input.digits];
}
template <unsigned MaxDigits>
[[nodiscard]] inline std::uint32_t leading_digits(field input) noexcept {
    if constexpr (MaxDigits <= 8)
        return 0;
    else if constexpr (MaxDigits == 9)
        return ninth_digit(input);
    else {
        if (input.digits <= 8)
            return 0;
        unsigned leading = static_cast<unsigned>(input.data[0] - '0');
        if (input.digits == 10)
            leading = leading * 10 + static_cast<unsigned>(input.data[1] - '0');
        return leading * 100'000'000;
    }
}
} // namespace detail

[[nodiscard]] inline std::uint32_t decode_one(field input) noexcept {
    auto digits = detail::eight_byte_tail(input) & detail::nibble_mask[input.digits];
    digits = digits * 10 + (digits >> 8);
    const auto eight = static_cast<std::uint32_t>(
        ((digits & 0x0000'00ff'0000'00ffull) * 0x000f'4240'0000'0064ull +
         ((digits >> 16) & 0x0000'00ff'0000'00ffull) * 0x0000'2710'0000'0001ull) >>
        32);
    return eight + detail::leading_digits<10>(input);
}

// ASCII -> digit pairs -> four-digit groups -> eight-digit groups.
// Four-digit groups are <=9999, so packing does not saturate and the final
// signed 16-bit multiply-add is exact. A ninth digit is added separately.
template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 2>
decode_pair(field first, field second) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 10);
    static_assert(1 <= SecondMaxDigits && SecondMaxDigits <= 10);
    using namespace detail;
    auto digits = _mm_set_epi64x(std::bit_cast<long long>(eight_byte_tail(second)),
                                 std::bit_cast<long long>(eight_byte_tail(first)));
    digits = _mm_and_si128(digits,
                           _mm_set_epi64x(nibble_mask[second.digits], nibble_mask[first.digits]));
    const auto pairs = _mm_maddubs_epi16(digits, _mm_set1_epi16(pair_weights));
    const auto quads = _mm_madd_epi16(pairs, _mm_set1_epi32(quad_weights));
    const auto compact = _mm_packus_epi32(quads, quads);
    const auto eights = _mm_madd_epi16(compact, _mm_set1_epi32(octet_weights));
    const auto leading_first = leading_digits<FirstMaxDigits>(first);
    const auto leading_second = leading_digits<SecondMaxDigits>(second);
    return {static_cast<std::uint32_t>(_mm_cvtsi128_si32(eights)) + leading_first,
            static_cast<std::uint32_t>(_mm_extract_epi32(eights, 1)) + leading_second};
}

template <unsigned FirstMaxDigits, unsigned SecondMaxDigits, unsigned ThirdMaxDigits>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_triplet(field first, field second, field third) noexcept;

// FirstMaxDigits is a precondition, not a run-time validator. A known-small
// first field (e.g. an index) skips its ninth-digit calculation at compile time.
template <unsigned FirstMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_three(field first, field second, field third) noexcept {
    return decode_triplet<FirstMaxDigits, 9, 9>(first, second, third);
}

// The next 32 bytes must be readable and contain all requested delimiters.
// Only ASCII decimal input is supported, so signed-byte comparison suffices.
[[nodiscard]] inline std::uint32_t delimiter_mask(const char* data) noexcept {
    return static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpgt_epi8(
        _mm256_set1_epi8('0'), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data)))));
}
} // namespace canard::io::decimal

namespace canard::io::decimal {
// Three bounded unsigned fields; total ASCII record (with delimiters) <=32
// bytes. Unlike decode_three's historical interface, every bound is explicit.
template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9, unsigned ThirdMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_triplet(field first, field second, field third) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 10);
    static_assert(1 <= SecondMaxDigits && SecondMaxDigits <= 10);
    static_assert(1 <= ThirdMaxDigits && ThirdMaxDigits <= 10);
    using namespace detail;
    auto digits = _mm256_setr_epi64x(std::bit_cast<long long>(eight_byte_tail(first)),
                                     std::bit_cast<long long>(eight_byte_tail(second)),
                                     std::bit_cast<long long>(eight_byte_tail(third)),
                                     0);
    digits = _mm256_and_si256(
        digits,
        _mm256_setr_epi64x(
            nibble_mask[first.digits], nibble_mask[second.digits], nibble_mask[third.digits], 0));
    const auto pairs = _mm256_maddubs_epi16(digits, _mm256_set1_epi16(pair_weights));
    const auto quads = _mm256_madd_epi16(pairs, _mm256_set1_epi32(quad_weights));
    const auto compact = _mm256_packus_epi32(quads, quads);
    const auto eights = _mm256_madd_epi16(compact, _mm256_set1_epi32(octet_weights));
    return {static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 0)) +
                leading_digits<FirstMaxDigits>(first),
            static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 1)) +
                leading_digits<SecondMaxDigits>(second),
            static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 4)) +
                leading_digits<ThirdMaxDigits>(third)};
}

// Four fields of 1..7 decimal digits. Every eight-byte tail must be readable.
// The length/decimal alphabet are trusted preconditions, not runtime checks.
[[nodiscard]] inline std::array<std::uint32_t, 4>
decode_four_short(field a, field b, field c, field d) noexcept {
    using namespace detail;
    auto x = _mm256_setr_epi64x(std::bit_cast<long long>(eight_byte_tail(a)),
                                std::bit_cast<long long>(eight_byte_tail(b)),
                                std::bit_cast<long long>(eight_byte_tail(c)),
                                std::bit_cast<long long>(eight_byte_tail(d)));
    x = _mm256_and_si256(x,
                         _mm256_setr_epi64x(nibble_mask[a.digits],
                                            nibble_mask[b.digits],
                                            nibble_mask[c.digits],
                                            nibble_mask[d.digits]));
    x = _mm256_maddubs_epi16(x, _mm256_set1_epi16(pair_weights));
    x = _mm256_madd_epi16(x, _mm256_set1_epi32(quad_weights));
    x = _mm256_packus_epi32(x, x);
    x = _mm256_madd_epi16(x, _mm256_set1_epi32(octet_weights));
    // AVX2 packing is local to each 128-bit half: fields occupy lanes 0,1,4,5.
    return {static_cast<std::uint32_t>(_mm256_extract_epi32(x, 0)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 1)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 4)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 5))};
}
} // namespace canard::io::decimal
namespace canard::io::decimal {
struct ascii_avx2_codec {
    CANARD_ALWAYS_INLINE static std::uint32_t delimiter_mask(const char* p) noexcept {
        return decimal::delimiter_mask(p);
    }
    template <unsigned A = 9, unsigned B = 9>
    CANARD_ALWAYS_INLINE static auto decode_pair(field a, field b) noexcept {
        return decimal::decode_pair<A, B>(a, b);
    }
    template <unsigned A = 9, unsigned B = 9, unsigned C = 9>
    CANARD_ALWAYS_INLINE static auto decode_triplet(field a, field b, field c) noexcept {
        return decimal::decode_triplet<A, B, C>(a, b, c);
    }
    template <unsigned A = 9>
    CANARD_ALWAYS_INLINE static auto decode_three(field a, field b, field c) noexcept {
        return decimal::decode_three<A>(a, b, c);
    }
    CANARD_ALWAYS_INLINE static auto decode_four_short(field a, field b, field c, field d) noexcept {
        return decimal::decode_four_short(a, b, c, d);
    }
};
}
namespace canard::io {
using avx2_ascii_reader = basic_trusted_ascii_reader<decimal::ascii_avx2_codec>;
}
// ===== include/canard/io/decimal/decode_u64_avx2.hpp =====
#include <immintrin.h>
#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#ifndef __AVX2__
#error "This explicit decimal backend requires AVX2."
#endif
namespace canard::io::decimal {


// Explicit AVX2 execution of the same field/borrowing contract. Each 128-bit
// half holds one number's sixteen low digits; up to four leading digits are
// added separately. Numeric uint64 values, not decimal chunks, reach the caller.
struct u64_avx2_codec : u64_scalar_codec {
    [[nodiscard]] static std::uint32_t delimiters32(const char* source) noexcept {
        return static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpgt_epi8(
            _mm256_set1_epi8('0'), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(source)))));
    }

    [[nodiscard]] CANARD_ALWAYS_INLINE static inline std::array<value_type, 2> decode_pair(u64_field first,
                                                            u64_field second) noexcept {
        // Scalar single digits avoid expanding a two-byte payload into wide
        // arithmetic; the branch depends on token shape, never on the value.
        if ((first.digits | second.digits) == 1)
            return {static_cast<value_type>(first.source[0] - '0'),
                    static_cast<value_type>(second.source[0] - '0')};
        __m256i digits;
        // Full tails need no alignment masks and read only actual field bytes.
        if (first.digits >= 16 && second.digits >= 16) {
            const auto a = _mm_loadu_si128(reinterpret_cast<const __m128i*>(first.source + first.digits - 16));
            const auto b = _mm_loadu_si128(reinterpret_cast<const __m128i*>(second.source + second.digits - 16));
            digits = _mm256_and_si256(_mm256_set_m128i(b, a), _mm256_set1_epi8(15));
        } else {
            const auto [a_high, a_low] = detail::u64_tail(first);
            const auto [b_high, b_low] = detail::u64_tail(second);
            digits = _mm256_setr_epi64x(std::bit_cast<long long>(a_high),
                                        std::bit_cast<long long>(a_low),
                                        std::bit_cast<long long>(b_high),
                                        std::bit_cast<long long>(b_low));
        }
        digits = _mm256_maddubs_epi16(digits, _mm256_set1_epi16(0x010a));
        digits = _mm256_madd_epi16(digits, _mm256_set1_epi32(0x0001'0064));
        digits = _mm256_packus_epi32(digits, digits);
        digits = _mm256_madd_epi16(digits, _mm256_set1_epi32(0x0001'2710));
        const auto values = _mm256_add_epi64(
            _mm256_mul_epu32(digits, _mm256_set1_epi32(100'000'000)),
            _mm256_srli_epi64(digits, 32));
        return {static_cast<value_type>(_mm256_extract_epi64(values, 0)) + detail::u64_leading(first),
                static_cast<value_type>(_mm256_extract_epi64(values, 2)) + detail::u64_leading(second)};
    }
};

namespace detail {

// Right-align up to sixteen source digits in a 128-bit lane. For wider fields
// the source pointer already selects the final sixteen digits. Each row is
// aligned for a 16-byte load; zero-filled lanes become decimal leading zeroes.
alignas(64) inline constexpr auto align_u64_digits = [] {
    std::array<std::array<unsigned char, 16>, 21> masks{};
    for (unsigned digits = 1; digits <= 20; ++digits) {
        const auto padding = 16 - std::min(digits, 16u);
        for (unsigned lane = 0; lane < 16; ++lane)
            masks[digits][lane] = lane < padding ? 0x80 : lane - padding;
    }
    return masks;
}();

} // namespace detail

// Opt-in paired conversion. Long fields retain the existing unmasked tail
// path. Short fields use two vector loads and two small shuffle rows instead
// of four scalar tail loads and their scalar masks. All other reader semantics,
// including the cached delimiter window and single-field decoder, are shared.
struct u64_shuffle_avx2_codec : u64_avx2_codec {
    [[nodiscard]] CANARD_ALWAYS_INLINE static inline std::array<value_type, 2>
    decode_pair(u64_field first, u64_field second) noexcept {
        if ((first.digits | second.digits) == 1)
            return {static_cast<value_type>(first.source[0] - '0'),
                    static_cast<value_type>(second.source[0] - '0')};
        if (first.digits >= 16 && second.digits >= 16)
            return u64_avx2_codec::decode_pair(first, second);

        const auto a = _mm_loadu_si128(reinterpret_cast<const __m128i*>(
            first.source + std::max(first.digits, 16u) - 16));
        const auto b = _mm_loadu_si128(reinterpret_cast<const __m128i*>(
            second.source + std::max(second.digits, 16u) - 16));
        const auto masks = _mm256_set_m128i(
            _mm_load_si128(reinterpret_cast<const __m128i*>(
                detail::align_u64_digits[second.digits].data())),
            _mm_load_si128(reinterpret_cast<const __m128i*>(
                detail::align_u64_digits[first.digits].data())));
        auto digits = _mm256_and_si256(
            _mm256_shuffle_epi8(_mm256_set_m128i(b, a), masks), _mm256_set1_epi8(15));
        digits = _mm256_maddubs_epi16(digits, _mm256_set1_epi16(0x010a));
        digits = _mm256_madd_epi16(digits, _mm256_set1_epi32(0x0001'0064));
        digits = _mm256_packus_epi32(digits, digits);
        digits = _mm256_madd_epi16(digits, _mm256_set1_epi32(0x0001'2710));
        const auto values = _mm256_add_epi64(
            _mm256_mul_epu32(digits, _mm256_set1_epi32(100'000'000)),
            _mm256_srli_epi64(digits, 32));
        std::array<value_type, 2> result{
            static_cast<value_type>(_mm256_extract_epi64(values, 0)),
            static_cast<value_type>(_mm256_extract_epi64(values, 2))};
        if (first.digits > 16)
            result[0] += detail::u64_leading(first);
        if (second.digits > 16)
            result[1] += detail::u64_leading(second);
        return result;
    }
};

}
// ===== include/canard/io/decimal/encode_u64_avx2.hpp =====
#include <immintrin.h>
#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#ifndef __AVX2__
#error "This explicit decimal backend requires AVX2."
#endif
namespace canard::io::decimal {
namespace detail {

inline constexpr auto decimal_powers64 = [] {
    std::array<std::uint64_t, 20> powers{1};
    for (unsigned i = 1; i < powers.size(); ++i)
        powers[i] = 10 * powers[i - 1];
    return powers;
}();

[[nodiscard]] inline unsigned decimal_length64(std::uint64_t value) noexcept {
    const auto nonzero = value | 1;
    const unsigned estimate = (std::bit_width(nonzero) * 1233) >> 12;
    return estimate + 1 - (nonzero < decimal_powers64[estimate]);
}

alignas(64) inline constexpr auto trim_decimal16 = [] {
    std::array<std::array<unsigned char, 16>, 17> masks{};
    for (unsigned length = 0; length <= 16; ++length)
        for (unsigned lane = 0; lane < 16; ++lane)
            masks[length][lane] = lane < length ? 16 - length + lane : 0x80;
    return masks;
}();

// Exactly sixteen digits from two ordinary integers less than 10^8. The
// reciprocal products are integer arithmetic, with bounded exact quotients:
// x/10000=(x*109951163)>>40 for x<10^8. Each resulting group is below 10000;
// /1000, /100 and /10 then use (8389,23), (5243,19) and (52429,19).
// No float conversion, division instruction or digit lookup loads are used.
[[nodiscard]] inline __m128i encode_sixteen_digits(std::uint32_t high,
                                                 std::uint32_t low) noexcept {
    const auto halves = _mm_setr_epi32(high, 0, low, 0);
    const auto quotient = _mm_srli_epi64(
        _mm_mul_epu32(halves, _mm_set1_epi32(109951163)), 40);
    const auto remainder = _mm_sub_epi32(
        halves, _mm_mullo_epi32(quotient, _mm_set1_epi32(10000)));
    const auto groups32 = _mm_or_si128(quotient, _mm_slli_epi64(remainder, 32));
    const auto groups = _mm_packus_epi32(groups32, groups32);
    const auto thousands = _mm_srli_epi16(_mm_mulhi_epu16(groups, _mm_set1_epi16(8389)), 7);
    const auto hundreds = _mm_srli_epi16(_mm_mulhi_epu16(groups, _mm_set1_epi16(5243)), 3);
    const auto tens = _mm_srli_epi16(_mm_mulhi_epu16(groups, _mm_set1_epi16(-13107)), 3);
    const auto hundred_digits = _mm_sub_epi16(
        hundreds, _mm_mullo_epi16(thousands, _mm_set1_epi16(10)));
    const auto ten_digits = _mm_sub_epi16(tens, _mm_mullo_epi16(hundreds, _mm_set1_epi16(10)));
    const auto one_digits = _mm_sub_epi16(groups, _mm_mullo_epi16(tens, _mm_set1_epi16(10)));
    const auto high_pairs = _mm_unpacklo_epi16(thousands, hundred_digits);
    const auto low_pairs = _mm_unpacklo_epi16(ten_digits, one_digits);
    return _mm_add_epi8(_mm_unpacklo_epi16(_mm_packus_epi16(high_pairs, high_pairs),
                                          _mm_packus_epi16(low_pairs, low_pairs)),
                        _mm_set1_epi8('0'));
}

} // namespace detail

// A hybrid, explicit SIMD conversion policy. Tiny numbers reuse the existing
// scalar digit table; 5..16 digits use integer SIMD; 17..20 digits use the
// new bounded leading-group table path. This is value-size selection within an
// AVX2 policy, not runtime ISA detection or a silent backend fallback.
struct u64_avx2_formatter {
    using value_type = std::uint64_t;
    static constexpr unsigned token_capacity = 24;

    [[nodiscard]] char* operator()(char* destination, value_type value) const noexcept {
        if (value < 10'000)
            return format_u32_compact(destination, static_cast<std::uint32_t>(value));
        if (value >= 10'000'000'000'000'000ull)
            return detail::format_u64_large(destination, value);
        const auto high = value / 100'000'000;
        const auto low = value - high * 100'000'000;
        auto digits = detail::encode_sixteen_digits(static_cast<std::uint32_t>(high),
                                                    static_cast<std::uint32_t>(low));
        const auto length = detail::decimal_length64(value);
        digits = _mm_shuffle_epi8(digits, _mm_load_si128(
            reinterpret_cast<const __m128i*>(detail::trim_decimal16[length].data())));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(destination), digits);
        return destination + length;
    }
};


namespace detail {

// Two independent sixteen-digit strings, one per 128-bit half. The divisions
// use the same bounded, exact reciprocal products as encode_sixteen_digits.
// Each high/low input is an ordinary integer below 10^8.
[[nodiscard]] inline __m256i encode_two_sixteen_digit_strings(
    std::uint32_t first_high, std::uint32_t first_low,
    std::uint32_t second_high, std::uint32_t second_low) noexcept {
    const auto halves = _mm256_setr_epi32(
        first_high, 0, first_low, 0, second_high, 0, second_low, 0);
    const auto quotient = _mm256_srli_epi64(
        _mm256_mul_epu32(halves, _mm256_set1_epi32(109951163)), 40);
    const auto remainder = _mm256_sub_epi32(
        halves, _mm256_mullo_epi32(quotient, _mm256_set1_epi32(10000)));
    const auto groups32 = _mm256_or_si256(quotient, _mm256_slli_epi64(remainder, 32));
    const auto groups = _mm256_packus_epi32(groups32, groups32);
    const auto thousands = _mm256_srli_epi16(
        _mm256_mulhi_epu16(groups, _mm256_set1_epi16(8389)), 7);
    const auto hundreds = _mm256_srli_epi16(
        _mm256_mulhi_epu16(groups, _mm256_set1_epi16(5243)), 3);
    const auto tens = _mm256_srli_epi16(
        _mm256_mulhi_epu16(groups, _mm256_set1_epi16(-13107)), 3);
    const auto hundred_digits = _mm256_sub_epi16(
        hundreds, _mm256_mullo_epi16(thousands, _mm256_set1_epi16(10)));
    const auto ten_digits = _mm256_sub_epi16(
        tens, _mm256_mullo_epi16(hundreds, _mm256_set1_epi16(10)));
    const auto one_digits = _mm256_sub_epi16(
        groups, _mm256_mullo_epi16(tens, _mm256_set1_epi16(10)));
    const auto high_pairs = _mm256_unpacklo_epi16(thousands, hundred_digits);
    const auto low_pairs = _mm256_unpacklo_epi16(ten_digits, one_digits);
    return _mm256_add_epi8(
        _mm256_unpacklo_epi16(_mm256_packus_epi16(high_pairs, high_pairs),
                             _mm256_packus_epi16(low_pairs, low_pairs)),
        _mm256_set1_epi8('0'));
}

// The sixteen supplied ASCII digits encode value modulo 10^16; leading is
// floor(value / 10^16). The same 24-byte token capacity covers both branches.
[[nodiscard]] inline char* emit_u64_digit_tail(
    char* destination, std::uint64_t value, std::uint64_t leading,
    __m128i digits) noexcept {
    if (leading != 0) {
        const auto group = u64_leading_groups[leading];
        std::memcpy(destination, &group, 4);
        destination += group >> 32;
        _mm_storeu_si128(reinterpret_cast<__m128i*>(destination), digits);
        return destination + 16;
    }
    const auto length = decimal_length64(value);
    digits = _mm_shuffle_epi8(digits, _mm_load_si128(
        reinterpret_cast<const __m128i*>(trim_decimal16[length].data())));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(destination), digits);
    return destination + length;
}

} // namespace detail

// The scalar call preserves the previous formatter. The optional pair hook
// amortizes SIMD digit construction across two independent uint64 values.
// It inserts exactly the caller-supplied separator between tokens, but does
// not append a trailing delimiter. Capacity: 2 * token_capacity + 1 bytes.
// The old formatter remains available; there is no global default replacement.
struct u64_paired_avx2_formatter : u64_avx2_formatter {
    [[nodiscard]] CANARD_ALWAYS_INLINE inline char* format_pair(
        char* destination, value_type first, value_type second,
        char separator) const noexcept {
        if (first < 10'000 && second < 10'000) {
            destination = format_u32_compact(destination, static_cast<std::uint32_t>(first));
            *destination++ = separator;
            return format_u32_compact(destination, static_cast<std::uint32_t>(second));
        }
        constexpr value_type base = 10'000'000'000'000'000ull;
        if (first >= base && second >= base) {
            destination = detail::format_u64_large(destination, first);
            *destination++ = separator;
            return detail::format_u64_large(destination, second);
        }
        const auto first_top = first / base, second_top = second / base;
        const auto first_high = first / 100'000'000, second_high = second / 100'000'000;
        const auto digits = detail::encode_two_sixteen_digit_strings(
            static_cast<std::uint32_t>(first_high - first_top * 100'000'000),
            static_cast<std::uint32_t>(first - first_high * 100'000'000),
            static_cast<std::uint32_t>(second_high - second_top * 100'000'000),
            static_cast<std::uint32_t>(second - second_high * 100'000'000));
        destination = detail::emit_u64_digit_tail(
            destination, first, first_top, _mm256_castsi256_si128(digits));
        *destination++ = separator;
        return detail::emit_u64_digit_tail(
            destination, second, second_top, _mm256_extracti128_si256(digits, 1));
    }
};

}
#endif
#if defined(__GNUC__) || defined(__clang__)
#define CANARD_IO_FLATTEN [[gnu::flatten]]
#else
#define CANARD_IO_FLATTEN
#endif
namespace canard::io::detail {
// The target affects template arguments, not the definition of any instantiated
// class. Never pass a default-target object across differently targeted TUs:
// spell its execution type explicitly at that boundary.
#if defined(__AVX2__)
using target_execution = canard::execution::avx2;
#else
using target_execution = canard::execution::scalar;
#endif
template <typename Execution> struct codecs;
template <> struct codecs<canard::execution::scalar> {
    using ascii = decimal::ascii_scalar_codec;
    using wide = decimal::u64_scalar_codec;
    using formatter = decimal::u64_table_formatter;
};
#if defined(__AVX2__)
template <> struct codecs<canard::execution::avx2> {
    using ascii = decimal::ascii_avx2_codec;
    using wide = decimal::u64_shuffle_avx2_codec;
    using formatter = decimal::u64_paired_avx2_formatter;
};
#endif
} // namespace canard::io::detail
// ===== include/canard/io/delimited_reader.hpp =====
// ===== include/canard/io/protocols.hpp =====
// ===== include/canard/associated_types.hpp =====
#include <cstddef>

namespace canard {

template <typename T> using coordinate_type_t = typename T::coordinate_type;
template <typename T> using point_type_t = typename T::point_type;
template <typename T> using rectangle_type_t = typename T::rectangle_type;
template <typename T> using partition_type_t = typename T::partition_type;


template <typename T> using aggregate_type_t = typename T::aggregate_type;
template <typename T> using element_type_t = typename T::element_type;
template <typename T> using configuration_type_t = typename T::configuration_type;
template <typename T> using allocator_type_t = typename T::allocator_type;
template <typename T> using size_type_t = typename T::size_type;

// Direct associated-type projections: no cv/ref removal, fallback, or conversion.
// Missing members remain substitution failures in requires-expressions.
template <typename T> using value_type_t = typename T::value_type;

template <typename T> using monoid_type_t = typename T::monoid_type;

template <typename T> using tag_type_t = typename T::tag_type;

template <typename T> using summary_type_t = typename T::summary_type;

template <typename T> using update_type_t = typename T::update_type;

template <typename T> using action_type_t = typename T::action_type;

template <typename T> using leaf_type_t = typename T::leaf_type;

template <typename T> using branch_type_t = typename T::branch_type;

template <typename T> using word_type_t = typename T::word_type;

template <typename T> using representation_type_t = typename T::representation_type;

template <typename T> using execution_type_t = typename T::execution_type;

template <typename T> using delta_type_t = typename T::delta_type;

template <typename T> using prepared_action_t = typename T::prepared_action;

template <typename T> using subtree_change_t = typename T::subtree_change;

template <typename T> using local_change_t = typename T::local_change;

template <typename T> using edge_t = typename T::edge;

template <typename T> using field_t = typename T::field;

template <typename T> using pack_t = typename T::pack;

template <typename T> using fixed_multiplier_t = typename T::fixed_multiplier;

template <typename T> using blended_multiplier_t = typename T::blended_multiplier;

template <typename T, std::size_t Extent> using interval_t = typename T::template interval<Extent>;

template <typename T, std::size_t Count> using edit_type_t = typename T::template edit_type<Count>;

} // namespace canard
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>
namespace canard::io {
template <typename C>
concept unsigned64_codec = requires(const char* bytes, decimal::u64_field field) {
    requires std::same_as<value_type_t<C>, std::uint64_t>;
    { C::delimiters32(bytes) } noexcept -> std::same_as<std::uint32_t>;
    { C::decode(field) } noexcept -> std::same_as<std::uint64_t>;
    { C::decode_pair(field, field) } noexcept -> std::same_as<std::array<std::uint64_t, 2>>;
};
template <typename F, typename Word = value_type_t<F>>
concept integer_formatter = std::same_as<Word, value_type_t<F>> && requires(const F& f, char* p, Word x) {
    typename std::integral_constant<std::size_t, F::token_capacity>;
    requires (F::token_capacity > 0);
    { f(p, x) } noexcept -> std::same_as<char*>;
};
template <typename F, typename Word = value_type_t<F>>
concept paired_integer_formatter = integer_formatter<F, Word> && requires(const F& f, char* p, Word x) {
    { f.format_pair(p, x, x, char{}) } noexcept -> std::same_as<char*>;
};
} // namespace canard::io

#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>

namespace canard::io {

// Syntactic customization point. The semantic contract additionally requires
// exact uint64 conversion and the delimiter vocabulary in decimal_u64.hpp.
template <typename Codec>
concept u64_decimal_codec = unsigned64_codec<Codec>;

// Borrowing, trusted input reader. It consumes a caller-known number of fields:
// each field has 1..20 digits, fits uint64_t, and is separated by ONE whitespace
// byte. There is no leading whitespace, malformed-input validation or EOF loop.
// Missing final whitespace is allowed when view.data[view.size] is zero.
// The existing padded view supplies eight bytes before and 32 after the data.
// No whole-input token index or value staging buffer is built.
template <u64_decimal_codec Codec = decimal::u64_scalar_codec>
class delimited_reader {
    const char* window_;
    const char* cursor_;
    const char* end_;
    std::uint64_t delimiters_;

    [[nodiscard]] std::uint64_t scan() const noexcept {
        const auto low = Codec::delimiters32(window_);
        // Avoid a 64-byte load at a short tail: the public view promises only
        // 32 readable bytes after EOF. Padded bits are consumed only if the
        // final real token ends at EOF.
        if (end_ - window_ >= 32)
            return low | (std::uint64_t{Codec::delimiters32(window_ + 32)} << 32);
        return low;
    }

    [[nodiscard]] decimal::u64_field next_field() noexcept {
        while (delimiters_ == 0) {
            window_ += 64;
            delimiters_ = scan();
        }
        const unsigned bit = std::countr_zero(delimiters_);
        delimiters_ &= delimiters_ - 1;
        const char* first = cursor_;
        cursor_ = window_ + bit + 1;
        return {first, static_cast<unsigned>(cursor_ - first - 1)};
    }

  public:
    using value_type = std::uint64_t;
    using codec_type = Codec;

    explicit delimited_reader(padded_bytes_view bytes, Codec = {}) noexcept
        : window_(bytes.data), cursor_(bytes.data), end_(bytes.data + bytes.size),
          delimiters_(scan()) {}

    [[nodiscard]] value_type read() noexcept {
        return Codec::decode(next_field());
    }

    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<value_type, 2> read_pair() noexcept {
        decimal::u64_field first, second;
        const auto remaining = delimiters_ & (delimiters_ - 1);
        if (remaining != 0) {
            const unsigned left = std::countr_zero(delimiters_);
            const unsigned right = std::countr_zero(remaining);
            delimiters_ = remaining & (remaining - 1);
            first = {cursor_, static_cast<unsigned>(window_ + left - cursor_)};
            second = {window_ + left + 1, right - left - 1};
            cursor_ = window_ + right + 1;
        } else {
            // A pair crossing the window boundary uses the same field scanner;
            // decoding is still paired, with no copied byte fragments.
            first = next_field();
            second = next_field();
        }
        return Codec::decode_pair(first, second);
    }

    template <std::size_t Extent>
    void read_batch(std::span<value_type, Extent> destination) noexcept {
        std::size_t i = 0;
        for (; i + 1 < destination.size(); i += 2) {
            const auto pair = read_pair();
            destination[i] = pair[0];
            destination[i + 1] = pair[1];
        }
        if (i != destination.size())
            destination[i] = read();
    }

    [[nodiscard]] const char* position() const noexcept {
        return cursor_;
    }
};

template <unsigned64_codec Codec>
delimited_reader(padded_bytes_view, Codec) -> delimited_reader<Codec>;

} // namespace canard::io
#include <algorithm>
#include <array>
#include <optional>
#include <bit>
#include <utility>
#include <functional>

namespace canard::io::detail {
// One logical cursor. The optional cached wide scanner is refreshed on demand when
// another decoding path has advanced the logical cursor. Packed lookahead is not logical
// record consumption. The caller supplies trusted, padded, single-separated
// decimal fields. All the conversion routines are shared with the old API.
template <typename Execution> class record_decoder {
    using code = codecs<Execution>;
    using small_reader = basic_trusted_ascii_reader<typename code::ascii>;
    using wide_reader = delimited_reader<typename code::wide>;
    const char* cursor_;
    const char* end_;
    std::optional<wide_reader> wide_;

    CANARD_ALWAYS_INLINE wide_reader& wide() noexcept {
        if (!wide_ || wide_->position() != cursor_) {
            while (cursor_ < end_ && static_cast<unsigned char>(*cursor_) <= ' ') {
                ++cursor_;
            }
            wide_.emplace(padded_bytes_view{cursor_, static_cast<std::size_t>(end_ - cursor_)});
        }
        return *wide_;
    }
    CANARD_ALWAYS_INLINE small_reader begin_small() {
        return small_reader{padded_bytes_view{cursor_, static_cast<std::size_t>(end_ - cursor_)}};
    }
    template <typename S, std::size_t... I>
    CANARD_ALWAYS_INLINE schema_value_t<S> unpack(std::index_sequence<I...>) {
        using V = schema_value_t<S>;
        if constexpr ((field_traits<schema_field_t<S, I>>::small_unsigned && ...)) {
            static constexpr std::array<unsigned, sizeof...(I)> d{
                field_traits<schema_field_t<S, I>>::digits...};
            if constexpr (sizeof...(I) == 2) {
                auto r = begin_small();
                const auto v = r.template read_pair<d[0], d[1]>();
                cursor_ = r.position();
                return V{static_cast<field_value_t<schema_field_t<S, I>>>(v[I])...};
            } else if constexpr (sizeof...(I) == 3 && d[0] == 1 && d[1] <= 9 && d[2] <= 9) {
                auto r = begin_small();
                const auto v = r.template read_tagged_pair<d[1], d[2]>();
                cursor_ = r.position();
                return V{static_cast<field_value_t<schema_field_t<S, I>>>(v[I])...};
            } else if constexpr (sizeof...(I) == 3 && (d[0] + d[1] + d[2] + 3 <= 32)) {
                auto r = begin_small();
                const auto v = r.template read_triplet<d[0], d[1], d[2]>();
                cursor_ = r.position();
                return V{static_cast<field_value_t<schema_field_t<S, I>>>(v[I])...};
            } else if constexpr (sizeof...(I) == 4 && d[0] == 1 && d[1] <= 9 && d[2] <= 9 &&
                                 d[3] <= 9) {
                auto r = begin_small();
                const auto v = r.template read_tagged_triplet<d[1]>();
                cursor_ = r.position();
                return V{static_cast<field_value_t<schema_field_t<S, I>>>(v[I])...};
            } else if constexpr (sizeof...(I) == 4 &&
                                 ((field_traits<schema_field_t<S, I>>::digits <= 7) && ...)) {
                constexpr auto max_digits = [] {
                    unsigned result = 0;
                    for (auto digits : d) {
                        result = std::max(result, digits);
                    }
                    return result;
                }();
                auto r = begin_small();
                const auto v = r.template read_two_index_pairs<max_digits>();
                cursor_ = r.position();
                return V{static_cast<field_value_t<schema_field_t<S, I>>>(v[I])...};
            } else {
                return V{field<schema_field_t<S, I>>()...};
            }
        } else if constexpr (sizeof...(I) == 2 &&
                             ((std::same_as<field_value_t<schema_field_t<S, I>>, std::uint64_t>) &&
                              ...)) {
            const auto values = wide().read_pair();
            cursor_ = wide_->position();
            return V{values[I]...};
        } else {
            // Braced initialization guarantees left-to-right reads; function
            // argument evaluation order is never used to sequence input.
            return V{field<schema_field_t<S, I>>()...};
        }
    }

  public:
    explicit record_decoder(padded_bytes_view bytes) noexcept
        : cursor_(bytes.data), end_(bytes.data + bytes.size) {}
    void expect_token(std::string_view token) { expect_view_token(cursor_, end_, token); }
    [[nodiscard]] const char* position() const noexcept {
        return cursor_;
    }
    template <field_schema S> CANARD_ALWAYS_INLINE field_value_t<S> field() {
        using T = field_value_t<S>;
        if constexpr (field_traits<S>::small_unsigned && field_traits<S>::digits == 1) {
            // A semantic one-digit schema makes decimal multiply/add work
            // unnecessary. Like other trusted scalar reads, stop on the
            // separator so the next selected kernel owns whitespace handling.
            while (static_cast<unsigned char>(*cursor_) <= ' ') {
                ++cursor_;
            }
            return static_cast<T>(*cursor_++ - '0');
        } else if constexpr (field_traits<S>::small_unsigned) {
            auto r = begin_small();
            const auto v = r.template read_u32<field_traits<S>::digits>();
            cursor_ = r.position();
            return static_cast<T>(v);
        } else if constexpr (std::is_signed_v<T> && sizeof(T) <= 4) {
            auto r = begin_small();
            const auto v = r.read_i32();
            cursor_ = r.position();
            return static_cast<T>(v);
        } else if constexpr (std::is_unsigned_v<T>) {
            const auto value = wide().read();
            cursor_ = wide_->position();
            return static_cast<T>(value);
        } else {
            while (static_cast<unsigned char>(*cursor_) <= ' ')
                ++cursor_;
            const bool negative = *cursor_ == '-';
            cursor_ += negative;
            std::uint64_t x = 0;
            for (unsigned digit; (digit = static_cast<unsigned>(*cursor_ - '0')) < 10; ++cursor_)
                x = x * 10 + digit;
            if (cursor_ < end_)
                ++cursor_;
            const auto bits = negative ? std::uint64_t{0} - x : x;
            return std::bit_cast<std::int64_t>(bits);
        }
    }
    template <readable_schema S> CANARD_ALWAYS_INLINE schema_value_t<S> read() {
        if constexpr (field_schema<S>)
            return field<S>();
        else
            return unpack<S>(std::make_index_sequence<schema_traits<S>::arity>{});
    }
    template <readable_schema S, typename Destination>
    CANARD_ALWAYS_INLINE void read_block(Destination* out, std::size_t n) {
        if (n == 0) {
            return;
        }
        std::size_t i = 0;
        if constexpr (field_schema<S> && field_traits<S>::small_unsigned) {
            for (; i + 1 < n; i += 2) {
                auto [a, b] = read<record<S, S>>();
                out[i] = a;
                out[i + 1] = b;
            }
        } else if constexpr (!field_schema<S> && schema_traits<S>::arity == 2) {
            using A = schema_field_t<S, 0>;
            using B = schema_field_t<S, 1>;
            if constexpr (field_traits<A>::small_unsigned && field_traits<B>::small_unsigned &&
                          field_traits<A>::digits <= 7 && field_traits<B>::digits <= 7) {
                for (; i + 1 < n; i += 2) {
                    auto [a, b, c, d] = read<record<A, B, A, B>>();
                    out[i] = Destination{a, b};
                    out[i + 1] = Destination{c, d};
                }
            } else if constexpr (std::same_as<field_value_t<A>, std::uint64_t> &&
                                 std::same_as<field_value_t<B>, std::uint64_t>) {
                auto& parser = wide();
                for (; i < n; ++i) {
                    const auto v = parser.read_pair();
                    out[i] = Destination{v[0], v[1]};
                }
                cursor_ = parser.position();
            }
        }
        for (; i < n; ++i)
            out[i] = read<S>();
    }
    template <readable_schema S, typename T, typename Projection>
    CANARD_ALWAYS_INLINE void project_block(T* output, std::size_t count, Projection& project) {
        using V = schema_value_t<S>;
        if constexpr (!field_schema<S> && schema_traits<S>::arity == 2) {
            using A = schema_field_t<S, 0>;
            using B = schema_field_t<S, 1>;
            if constexpr (std::same_as<field_value_t<A>, std::uint64_t> &&
                          std::same_as<field_value_t<B>, std::uint64_t>) {
                if (!count)
                    return;
                auto& parser = wide();
                try {
                    for (std::size_t i = 0; i < count; ++i) {
                        const auto values = parser.read_pair();
                        output[i] = std::invoke(project, V{values[0], values[1]});
                    }
                } catch (...) {
                    cursor_ = parser.position();
                    throw;
                }
                cursor_ = parser.position();
                return;
            }
        }
        // Short bounded records benefit from jointly decoding more fields;
        // wide pairs instead fuse conversion/projection to avoid a staging pass.
        std::array<V, 8> input{};
        read_block<S>(input.data(), count);
        for (std::size_t i = 0; i < count; ++i) {
            output[i] = std::invoke(project, std::as_const(input[i]));
        }
    }
};

} // namespace canard::io::detail
// ===== include/canard/io/stream_reader.hpp =====
// ===== include/canard/io/source/buffered_file.hpp =====
// ===== include/canard/io/source/stdio.hpp =====
// ===== include/canard/io/transfer.hpp =====
#include <cstddef>
#include <system_error>
namespace canard::io {
struct transfer_result {
    std::size_t count = 0;
    std::error_code error{};
};
}
#include <cerrno>
#include <cstdio>
#include <stdexcept>
namespace canard::io {
// Borrows FILE*. A zero count without error means EOF.
class stdio_source {
    std::FILE* file_;
  public:
    explicit stdio_source(std::FILE* file = stdin) : file_(file) {
        if (!file)
            throw std::invalid_argument("canard: null FILE handle");
    }
    transfer_result read_some(char* destination, std::size_t capacity) noexcept {
        for (;;) {
            errno = 0;
            const auto count = std::fread(destination, 1, capacity, file_);
            if (!std::ferror(file_)) return {count, {}};
            const auto error = errno ? std::error_code(errno, std::generic_category())
                                     : std::make_error_code(std::errc::io_error);
            if (error == std::errc::interrupted) {
                std::clearerr(file_);
                if (count != 0) return {count, {}};
                continue;
            }
            return {count, error};
        }
    }
};
}
#include <array>
#include <cassert>
#include <concepts>
#include <cstring>
#include <stdexcept>
#include <utility>
namespace canard::io {
template <typename Source>
concept byte_source = requires(Source& source, char* p, std::size_t n) {
    { source.read_some(p, n) } noexcept -> std::same_as<transfer_result>;
};
// Views/pointers obtained from window() expire at the next ensure() that refills.
// A zero read is EOF; a short positive read is not necessarily EOF (pipes work).
template <byte_source Source = stdio_source, std::size_t Capacity = (1u << 16)>
class buffered_source {
    static_assert(Capacity >= 64);
    std::array<char, Capacity + 40> bytes_{};
    [[no_unique_address]] Source source_;
    std::size_t first_ = 0, last_ = 0;
    bool eof_ = false;
  public:
    explicit buffered_source(Source source = Source{}) : source_(std::move(source)) {}
    buffered_source(const buffered_source&) = delete;
    buffered_source& operator=(const buffered_source&) = delete;
    void ensure(std::size_t count) {
        if (count > Capacity)
            throw std::length_error("canard input window too small");
        if (last_ - first_ >= count || eof_)
            return;
        const auto remaining = last_ - first_;
        std::memmove(bytes_.data() + 8, bytes_.data() + 8 + first_, remaining);
        first_ = 0; last_ = remaining;
        while (last_ < count && !eof_) {
            const auto transfer = source_.read_some(bytes_.data() + 8 + last_, Capacity - last_);
            if (transfer.count > Capacity - last_)
                std::terminate();
            last_ += transfer.count;
            std::memset(bytes_.data() + 8 + last_, 0, 32);
            if (transfer.error)
                throw std::system_error(transfer.error, "canard input");
            if (transfer.count == 0)
                eof_ = true;
        }
    }
    [[nodiscard]] padded_bytes_view window() const & noexcept {
        return {bytes_.data() + 8 + first_, last_ - first_};
    }
    void window() const && = delete;
    void consume(std::size_t count) noexcept {
        assert(count <= last_ - first_);
        first_ += count;
    }
    [[nodiscard]] bool eof() const noexcept {
        return eof_ && first_ == last_;
    }
};
using buffered_file_source = buffered_source<stdio_source>;
}
#include <charconv>
#include <array>
#include <concepts>
#include <cstdint>
#include <stdexcept>
namespace canard::io {
// Checked whitespace-separated integer protocol. CRLF, repeated and leading
// whitespace are accepted. This is deliberately distinct from delimited_reader.
template <typename Source = buffered_file_source>
class stream_ascii_reader {
    Source* source_;
    static bool whitespace(unsigned char c) noexcept {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
    }
    void skip() {
        for (;;) {
            source_->ensure(1);
            const auto w = source_->window();
            std::size_t n = 0;
            while (n < w.size && whitespace(w.data[n]))
                ++n;
            source_->consume(n);
            if (n != w.size || w.size == 0)
                return;
        }
    }
  public:
    explicit stream_ascii_reader(Source& source) noexcept : source_(&source) {}
    template <std::integral T> requires (!std::same_as<T, bool>)
    [[nodiscard]] T read() {
        skip(); source_->ensure(32);
        const auto w = source_->window();
        if (w.size == 0)
            throw std::runtime_error("canard: unexpected end of integer input");
        std::size_t n = 0;
        while (n < w.size && !whitespace(w.data[n]) && n < 32)
            ++n;
        const std::size_t sign = w.data[0] == '-' ? 1 : 0;
        if (n > 20 + sign)
            throw std::runtime_error("canard: integer token too long");
        T value{};
        const auto result = std::from_chars(w.data, w.data + n, value);
        if (result.ec != std::errc{} || result.ptr != w.data + n)
            throw std::runtime_error("canard: invalid or out-of-range integer");
        source_->consume(n);
        return value;
    }
    template <unsigned Digits = 10> [[nodiscard]] std::uint32_t read_u32() {
        static_assert(Digits >= 1 && Digits <= 10); return read<std::uint32_t>();
    }
    [[nodiscard]] std::uint64_t read_u64() {
        return read<std::uint64_t>();
    }
    [[nodiscard]] std::int32_t read_i32() {
        return read<std::int32_t>();
    }
    [[nodiscard]] std::int64_t read_i64() {
        return read<std::int64_t>();
    }
    template <unsigned A = 9, unsigned B = 9>
    [[nodiscard]] std::array<std::uint32_t, 2> read_pair() {
        return {read_u32<A>(), read_u32<B>()};
    }
    template <unsigned A = 9, unsigned B = 9, unsigned C = 9>
    [[nodiscard]] std::array<std::uint32_t, 3> read_triplet() {
        return {read_u32<A>(), read_u32<B>(), read_u32<C>()};
    }
    template <unsigned A = 9, unsigned B = 9>
    [[nodiscard]] std::array<std::uint32_t, 3> read_tagged_pair() {
        return {read_u32<1>(), read_u32<A>(), read_u32<B>()};
    }
    template <unsigned A = 9>
    [[nodiscard]] std::array<std::uint32_t, 4> read_tagged_triplet() {
        return {read_u32<1>(), read_u32<A>(), read_u32<9>(), read_u32<9>()};
    }
    template <unsigned A = 6>
    [[nodiscard]] std::array<std::uint32_t, 4> read_two_index_pairs() {
        return {read_u32<A>(), read_u32<A>(), read_u32<A>(), read_u32<A>()};
    }
};
// Checked streaming counterpart of the exact-one-separator unsigned protocol.
// The codec is still a pure conversion policy. Refill happens outside conversion.
template <unsigned64_codec Codec = decimal::u64_scalar_codec, typename Source = buffered_file_source>
class delimited_stream_reader {
    Source* source_;
    struct field_result {
        decimal::u64_field field;
        std::size_t consumed;
    };
    static field_result field(padded_bytes_view w, std::size_t at) {
        auto n = at;
        while (n < w.size && w.data[n] >= '0' && w.data[n] <= '9' && n - at < 21)
            ++n;
        if (n == at || n - at > 20)
            throw std::runtime_error("canard: invalid decimal field");
        std::uint64_t checked;
        if (std::from_chars(w.data + at, w.data + n, checked).ec != std::errc{})
            throw std::runtime_error("canard: integer overflow");
        auto end = n;
        if (n < w.size) {
            const auto c = w.data[n];
            if (c != ' ' && c != '\n' && c != '\r' && c != '\t')
                throw std::runtime_error("canard: invalid separator");
            ++end;
        }
        return {{w.data + at, static_cast<unsigned>(n - at)}, end};
    }
  public:
    explicit delimited_stream_reader(Source& source, Codec = {}) noexcept : source_(&source) {}
    [[nodiscard]] std::uint64_t read() {
        source_->ensure(21);
        const auto f = field(source_->window(), 0);
        const auto value = Codec::decode(f.field);
        source_->consume(f.consumed);
        return value;
    }
    [[nodiscard]] std::array<std::uint64_t, 2> read_pair() {
        source_->ensure(42);
        const auto w = source_->window();
        const auto a = field(w, 0), b = field(w, a.consumed);
        const auto values = Codec::decode_pair(a.field, b.field);
        source_->consume(b.consumed);
        return values;
    }
};
}
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iterator>
#include <ranges>
#include <span>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace canard::io {

namespace input_mode {
struct checked_stream {};
struct trusted_view {};
} // namespace input_mode

namespace detail {

template <typename Mode, typename Execution> struct input_state;

template <typename Execution> struct input_state<input_mode::checked_stream, Execution> {
    buffered_file_source source;
    stream_ascii_reader<> parser;

    input_state() : source(), parser(source) {}
    explicit input_state(std::FILE* file) : source(stdio_source{file}), parser(source) {}

    void expect_token(std::string_view token) { expect_stream_token(source, token); }

    template <field_schema Schema> field_value_t<Schema> field() {
        const auto value = parser.template read<field_value_t<Schema>>();
        if constexpr (field_traits<Schema>::bounded_value) {
            if (value > field_traits<Schema>::maximum) {
                throw std::out_of_range("canard input bound");
            }
        }
        return value;
    }

    template <typename Schema, std::size_t... Index>
    schema_value_t<Schema> unpack(std::index_sequence<Index...>) {
        return schema_value_t<Schema>{field<schema_field_t<Schema, Index>>()...};
    }

    template <readable_schema Schema> schema_value_t<Schema> read() {
        if constexpr (field_schema<Schema>) {
            return field<Schema>();
        } else {
            return unpack<Schema>(std::make_index_sequence<schema_traits<Schema>::arity>{});
        }
    }

    template <typename Schema, typename T> void read_block(T* output, std::size_t count) {
        for (std::size_t index = 0; index < count; ++index) {
            output[index] = read<Schema>();
        }
    }

    template <typename Schema, typename T, typename Projection>
    void project_block(T* output, std::size_t count, Projection& project) {
        for (std::size_t index = 0; index < count; ++index) {
            output[index] = std::invoke(project, read<Schema>());
        }
    }
};

template <typename Execution>
struct input_state<input_mode::trusted_view, Execution> : record_decoder<Execution> {
    explicit input_state(padded_bytes_view bytes) : record_decoder<Execution>(bytes) {}
};

} // namespace detail

template <typename Mode = input_mode::checked_stream, typename Execution = detail::target_execution>
class reader;

// A counted, homogeneous, single-pass stream. Ordinary iterator use decodes
// exactly the current record: increment does NOT prefetch the next one. The
// bulk write(range, projection) overload may request a block explicitly. Such
// a projection must not itself access this reader (see docs/UNIFIED_IO.md).
template <typename Reader, typename Schema>
class records_view : public std::ranges::view_interface<records_view<Reader, Schema>> {
    Reader* input_ = nullptr;
    std::size_t remaining_ = 0;
    bool loaded_ = false;
    detail::schema_value_t<Schema> current_{};

  public:
    using value_type = detail::schema_value_t<Schema>;

    records_view() = default;
    records_view(Reader& input, std::size_t count) : input_(&input), remaining_(count) {}

    struct iterator {
        using iterator_concept = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type = records_view::value_type;
        using difference_type = std::ptrdiff_t;
        records_view* owner = nullptr;

        const value_type& operator*() const {
            if (!owner->loaded_) {
                try {
                    owner->current_ = owner->input_->template read<Schema>();
                    owner->loaded_ = true;
                } catch (...) {
                    owner->remaining_ = 0;
                    throw;
                }
            }
            return owner->current_;
        }

        iterator& operator++() {
            (void)operator*();
            --owner->remaining_;
            owner->loaded_ = false;
            return *this;
        }

        void operator++(int) {
            ++*this;
        }

        friend bool operator==(iterator current, std::default_sentinel_t) {
            return !current.owner || current.owner->remaining_ == 0;
        }
    };

    iterator begin() {
        return {this};
    }
    std::default_sentinel_t end() const {
        return {};
    }
    std::size_t size() const noexcept {
        return remaining_;
    }

    // Library bulk-consumption customization. Does not decode beyond count.
    CANARD_ALWAYS_INLINE std::size_t consume_block(std::span<value_type> block) {
        const auto count = std::min(remaining_, block.size());
        std::size_t offset = 0;
        if (count && loaded_) {
            block[0] = current_;
            loaded_ = false;
            offset = 1;
        }
        try {
            if (count > offset) {
                input_->template read<Schema>(block.subspan(offset, count - offset));
            }
        } catch (...) {
            remaining_ = 0;
            throw;
        }
        remaining_ -= count;
        return count;
    }

    template <typename T, std::size_t Extent, typename Projection>
    CANARD_ALWAYS_INLINE std::size_t consume_projected(std::span<T, Extent> block,
                                                       Projection& project) {
        const auto count =
            Extent == std::dynamic_extent ? std::min(remaining_, block.size()) : Extent;
        assert(count <= remaining_ && count <= 8);
        if (count == 0) {
            return 0;
        }
        std::size_t offset = 0;
        try {
            if (loaded_) {
                block[0] = std::invoke(project, std::as_const(current_));
                loaded_ = false;
                offset = 1;
            }
            input_->template project_block<Schema>(block.data() + offset, count - offset, project);
        } catch (...) {
            // The stream cannot be rolled back. Discard this view rather than
            // advertising a record count that no longer matches the cursor.
            remaining_ = 0;
            throw;
        }
        remaining_ -= count;
        return count;
    }
};

template <typename Mode, typename Execution>
class reader : private detail::input_state<Mode, Execution> {
    using state = detail::input_state<Mode, Execution>;
    template <typename R, typename S> friend class records_view;

    template <typename Schema, typename T, typename Projection>
    CANARD_ALWAYS_INLINE void project_block(T* output, std::size_t count, Projection& project) {
        state::template project_block<Schema>(output, count, project);
    }

  public:
    using execution_type = Execution;

    reader()
        requires std::default_initializable<state>
    = default;
    explicit reader(padded_bytes_view bytes)
        requires std::same_as<Mode, input_mode::trusted_view>
        : state(bytes) {}
    explicit reader(std::FILE* file)
        requires std::same_as<Mode, input_mode::checked_stream>
        : state(file) {}
    reader(const reader&) = delete;
    reader& operator=(const reader&) = delete;

    // Validate and consume one literal token without creating a second cursor.
    void read(io::expect literal) { state::expect_token(literal.token); }

    // Immediate: exactly one scalar or one record; never a future operation.
    template <typename... Fields>
        requires(sizeof...(Fields) > 0 &&
                 detail::readable_schema<detail::requested_schema_t<Fields...>>)
    [[nodiscard]] CANARD_ALWAYS_INLINE auto read() {
        return state::template read<detail::requested_schema_t<Fields...>>();
    }

    // Fill existing storage, optionally decoding through a narrower schema.
    // Arrays, vectors and spans share this overload.
    template <typename Schema = void, std::ranges::contiguous_range Range>
        requires(std::ranges::sized_range<Range> &&
                 !std::is_const_v<std::remove_reference_t<std::ranges::range_reference_t<Range>>>)
    CANARD_ALWAYS_INLINE void read(Range&& destination) {
        using selected_schema =
            std::conditional_t<std::is_void_v<Schema>, std::ranges::range_value_t<Range>, Schema>;
        static_assert(detail::readable_schema<selected_schema>);
        state::template read_block<selected_schema>(std::ranges::data(destination),
                                                    std::ranges::size(destination));
    }

    // Eager bulk read into existing records with a projection. The callback must
    // not itself access this reader. The optional
    // second callback argument is the destination index, not stream position.
    template <typename Schema, std::ranges::contiguous_range Range, typename Projection>
        requires std::ranges::sized_range<Range>
    CANARD_IO_FLATTEN void read(Range&& destination, Projection projection) {
        using value_type = detail::schema_value_t<Schema>;
        std::array<value_type, 8> block{};
        const auto size = std::ranges::size(destination);
        for (std::size_t offset = 0; offset < size;) {
            const auto count = std::min<std::size_t>(8, size - offset);
            state::template read_block<Schema>(block.data(), count);
            for (std::size_t index = 0; index < count; ++index) {
                if constexpr (std::invocable<Projection&, const value_type&, std::size_t>) {
                    destination[offset + index] =
                        std::invoke(projection, std::as_const(block[index]), offset + index);
                } else {
                    destination[offset + index] =
                        std::invoke(projection, std::as_const(block[index]));
                }
            }
            offset += count;
        }
    }

    // Output-iterator form: no default initialization or staging of an entire
    // container. Useful with back_inserter into reserved storage.
    template <typename Schema, std::weakly_incrementable Iterator>
        requires std::indirectly_writable<Iterator, detail::schema_value_t<Schema>>
    CANARD_IO_FLATTEN Iterator read(std::size_t count, Iterator output) {
        std::array<detail::schema_value_t<Schema>, 8> values{};
        while (count >= 8) {
            state::template read_block<Schema>(values.data(), 8);
            for (auto value : values) {
                *output++ = std::move(value);
            }
            count -= 8;
        }
        state::template read_block<Schema>(values.data(), count);
        for (std::size_t index = 0; index < count; ++index) {
            *output++ = std::move(values[index]);
        }
        return output;
    }

    template <typename... Fields>
        requires(sizeof...(Fields) > 0 &&
                 detail::readable_schema<detail::requested_schema_t<Fields...>>)
    [[nodiscard]] auto read(std::size_t count) & {
        return records_view<reader, detail::requested_schema_t<Fields...>>{*this, count};
    }

    template <typename... Fields> auto read(std::size_t) && = delete;

    [[nodiscard]] const char* position() const noexcept
        requires requires(const state& source) { source.position(); }
    {
        return state::position();
    }
};

reader() -> reader<>;
reader(std::FILE*) -> reader<>;
reader(padded_bytes_view) -> reader<input_mode::trusted_view>;

} // namespace canard::io
// ===== include/canard/io/writer.hpp =====

// ===== include/canard/io/output_buffer.hpp =====
#include <concepts>
#include <cstring>
#include <memory>
#include <span>
#include <utility>
namespace canard::io {
template <typename S>
concept byte_sink = (requires { requires S::assume_complete_writes; } &&
    requires(S& sink, const char* data, std::size_t n) { { sink.write_all(data, n) } noexcept; }) ||
    requires(S& sink, const char* data, std::size_t n) {
        { sink.write_some(data, n) } noexcept -> std::same_as<transfer_result>;
    };
// Shared byte ownership only. Decimal syntax and ISA are not part of this type.
// Explicit flush/finish reports errors; an error retains the unwritten suffix.
// Destruction attempts flush but cannot report errors: callers needing delivery
// confirmation must call finish() while the object is still alive.
template <byte_sink Sink, std::size_t Capacity = (1u << 16)>
class output_buffer {
  protected:
    static_assert(Capacity >= 64);
    static constexpr std::size_t capacity = Capacity;
    std::unique_ptr<char[]> storage_ = std::make_unique_for_overwrite<char[]>(Capacity);
    char* cursor_ = storage_.get();
    [[no_unique_address]] Sink sink_;
  public:
    explicit output_buffer(Sink sink = Sink{}) : sink_(std::move(sink)) {}
    output_buffer(const output_buffer&) = delete;
    output_buffer& operator=(const output_buffer&) = delete;
    ~output_buffer() noexcept { try { flush(); } catch (...) {} }
    void flush() {
        const auto size = static_cast<std::size_t>(cursor_ - storage_.get());
        if constexpr (requires { requires Sink::assume_complete_writes; }) {
            sink_.write_all(storage_.get(), size);
            cursor_ = storage_.get();
        } else {
            std::size_t sent = 0;
            while (sent != size) {
                const auto result = sink_.write_some(storage_.get() + sent, size - sent);
                // A sink must never report consuming bytes outside its input.
                if (result.count > size - sent)
                    std::terminate();
                sent += result.count;
                auto error = result.error;
                if (!error && result.count == 0)
                    error = std::make_error_code(std::errc::io_error);
                if (error) {
                    std::memmove(storage_.get(), storage_.get() + sent, size - sent);
                    cursor_ = storage_.get() + size - sent;
                    throw std::system_error(error, "canard output");
                }
            }
            cursor_ = storage_.get();
        }
    }
    void finish() {
        flush();
        if constexpr (requires { sink_.finish(); }) {
            if (auto error = sink_.finish())
                throw std::system_error(error, "canard output finish");
        }
    }
    [[nodiscard]] std::span<const char> pending_bytes() const & noexcept {
        return {storage_.get(), static_cast<std::size_t>(cursor_ - storage_.get())};
    }
    void pending_bytes() const && = delete;
    void write_bytes(std::span<const char> bytes) {
        while (!bytes.empty()) {
            auto room = static_cast<std::size_t>(storage_.get() + capacity - cursor_);
            if (room == 0) {
                flush();
                room = capacity;
            }
            const auto count = bytes.size() < room ? bytes.size() : room;
            std::memcpy(cursor_, bytes.data(), count);
            cursor_ += count;
            bytes = bytes.subspan(count);
        }
    }
};
}
// ===== include/canard/io/sink/stdio.hpp =====
#include <cerrno>
#include <cstdio>
#include <stdexcept>
namespace canard::io {
class stdio_sink {
    std::FILE* file_;
  public:
    static constexpr bool assume_complete_writes = false;
    explicit stdio_sink(std::FILE* file = stdout) : file_(file) {
        if (!file)
            throw std::invalid_argument("canard: null FILE handle");
    }
    transfer_result write_some(const char* data, std::size_t size) noexcept {
        for (;;) {
            errno = 0;
            const auto count = std::fwrite(data, 1, size, file_);
            if (!std::ferror(file_)) return {count, {}};
            const auto error = errno ? std::error_code(errno, std::generic_category())
                                     : std::make_error_code(std::errc::io_error);
            if (error == std::errc::interrupted) {
                std::clearerr(file_);
                if (count != 0) return {count, {}};
                continue;
            }
            return {count, error};
        }
    }
    std::error_code finish() noexcept {
        for (;;) {
            errno = 0;
            if (std::fflush(file_) == 0) return {};
            const auto error = errno ? std::error_code(errno, std::generic_category())
                                     : std::make_error_code(std::errc::io_error);
            if (error != std::errc::interrupted)
                return error;
            std::clearerr(file_);
        }
    }
};
}
#include <array>
#include <charconv>
#include <functional>
#include <ranges>
#include <span>
#include <string_view>
#include <tuple>
#include <utility>

namespace canard::io {

// One write means one record: fields separated by spaces, followed by newline.
// A range means one record per element. There is no delayed scalar queue:
// flush(), exceptions, interleaved types and online output retain normal order.
template <byte_sink Sink = stdio_sink, typename Execution = detail::target_execution>
class writer : private output_buffer<Sink> {
    using base = output_buffer<Sink>;
    using base::capacity;
    using base::cursor_;
    using base::storage_;
    using formatter = typename detail::codecs<Execution>::formatter;

    void ensure(std::size_t bytes) {
        if (storage_.get() + capacity - cursor_ < static_cast<std::ptrdiff_t>(bytes)) {
            base::flush();
        }
    }

    template <detail::integer T> CANARD_ALWAYS_INLINE char* token(char* destination, T value) {
        if constexpr (std::is_signed_v<T>) {
            return std::to_chars(destination, destination + 24, value).ptr;
        } else if constexpr (sizeof(T) <= 4) {
            return decimal::format_u32_compact(destination, value);
        } else {
            return decimal::format_u64_token(destination, value);
        }
    }

    template <typename T> static constexpr std::size_t record_capacity() {
        if constexpr (detail::is_tuple_v<T>) {
            return 1 + 24 * std::tuple_size_v<T>;
        } else {
            return 24;
        }
    }

    template <typename T> CANARD_ALWAYS_INLINE char* row(char* destination, const T& value) {
        if constexpr (detail::is_tuple_v<T>) {
            if constexpr (std::tuple_size_v<T> == 0) {
                *destination++ = '\n';
                return destination;
            }
            std::apply(
                [&](const auto&... fields) {
                    std::size_t count = 0;
                    ((destination = token(destination, fields),
                      *destination++ = (++count == sizeof...(fields) ? '\n' : ' ')),
                     ...);
                },
                value);
            return destination;
        } else {
            destination = token(destination, value);
            *destination++ = '\n';
            return destination;
        }
    }

    template <typename T, std::size_t Extent>
    CANARD_ALWAYS_INLINE void block(std::span<const T, Extent> values) {
        static_assert(record_capacity<T>() <= capacity / 8);
        // Reserve physical write capacity, not merely the decimal length.
        // Paired formatters need 2 * token_capacity + 1 plus the final newline.
        constexpr std::size_t stride = [] {
            if constexpr (std::same_as<T, std::uint64_t>)
                return std::size_t{formatter::token_capacity + 1};
            else if constexpr (std::same_as<T, std::uint32_t>)
                return std::size_t{16};
            else
                return record_capacity<T>();
        }();
        ensure(stride * values.size());
        char* destination = cursor_;
        if constexpr (std::same_as<T, std::uint64_t>) {
            const formatter format;
            std::size_t index = 0;
            if constexpr (requires { format.format_pair(destination, T{}, T{}, '\n'); }) {
                for (; index + 1 < values.size(); index += 2) {
                    destination =
                        format.format_pair(destination, values[index], values[index + 1], '\n');
                    *destination++ = '\n';
                }
            }
            for (; index < values.size(); ++index) {
                destination = format(destination, values[index]);
                *destination++ = '\n';
            }
        } else {
            for (const auto& value : values) {
                destination = row(destination, value);
            }
        }
        cursor_ = destination;
    }

  public:
    using execution_type = Execution;

    explicit writer(Sink sink = Sink{}) : base(std::move(sink)) {}
    explicit writer(std::FILE* file)
        requires std::same_as<Sink, stdio_sink>
        : base(stdio_sink{file}) {}
    using base::finish;
    using base::flush;
    using base::pending_bytes;

    // Ordinary integers use their full domain. An optional schema states a
    // value bound as a precondition, without changing the value's C++ type.
    template <typename Schema = void, detail::integer T> CANARD_ALWAYS_INLINE void write(T value) {
        ensure(24);
        if constexpr (!std::is_void_v<Schema>) {
            using field = detail::field_traits<Schema>;
            static_assert(field::bounded_value && field::small_unsigned);
            cursor_ =
                decimal::format_u32_line<field::maximum>(cursor_, storage_.get() + capacity, value);
        } else {
            cursor_ = row(cursor_, value);
        }
    }

    template <typename First, typename Second, typename... Rest>
        requires(detail::integer<std::remove_cvref_t<First>> &&
                 detail::integer<std::remove_cvref_t<Second>> &&
                 (detail::integer<std::remove_cvref_t<Rest>> && ...))
    CANARD_ALWAYS_INLINE void write(const First& first, const Second& second, const Rest&... rest) {
        static_assert(24 * (2 + sizeof...(Rest)) <= capacity);
        ensure(24 * (2 + sizeof...(Rest)));
        cursor_ = row(cursor_, std::tie(first, second, rest...));
    }

    template <typename T>
        requires(detail::is_tuple_v<T> && detail::integer_record<T>)
    CANARD_ALWAYS_INLINE void write(const T& value) {
        static_assert(record_capacity<T>() <= capacity);
        ensure(record_capacity<T>());
        cursor_ = row(cursor_, value);
    }

    // A labelled numeric row, e.g. write("b", bag_id, u, v). No line allocation.
    template <typename... Fields>
        requires(sizeof...(Fields) > 0 && (detail::integer<Fields> && ...))
    CANARD_ALWAYS_INLINE void write(std::string_view label, Fields... fields) {
        static_assert(1 + 24 * sizeof...(Fields) <= capacity);
        base::write_bytes({label.data(), label.size()});
        ensure(1 + 24 * sizeof...(Fields));
        *cursor_++ = ' ';
        cursor_ = row(cursor_, std::tie(fields...));
    }

    void write(std::string_view text) {
        base::write_bytes({text.data(), text.size()});
        ensure(1);
        *cursor_++ = '\n';
    }

    void write() {
        ensure(1);
        *cursor_++ = '\n';
    }

    // Bulk terminal operation. Counted input records opt into joint decoding
    // and projection. Callbacks run in record order and must not perform I/O
    // on this same reader or writer. The client needs no batch arrays, SIMD types or unrolling.
    template <std::ranges::input_range Range, typename Projection = std::identity>
        requires(!std::convertible_to<Range, std::string_view>)
    CANARD_IO_FLATTEN void write(Range&& values, Projection projection = {}) {
        using input_type = std::ranges::range_value_t<Range>;
        using value_type =
            std::remove_cvref_t<std::invoke_result_t<Projection&, const input_type&>>;
        static_assert(detail::integer_record<value_type>, "bulk output requires integer records");
        constexpr std::size_t batch = 8;
        std::array<value_type, batch> answers{};
        if constexpr (requires {
                          values.consume_projected(std::span<value_type>{answers}, projection);
                      }) {
            while (values.size() >= batch) {
                values.consume_projected(std::span<value_type, batch>{answers}, projection);
                block(std::span<const value_type, batch>{answers});
            }
            const auto count = values.consume_projected(std::span<value_type>{answers}, projection);
            block(std::span<const value_type>{answers.data(), count});
        } else {
            auto iterator = std::ranges::begin(values);
            const auto end = std::ranges::end(values);
            while (iterator != end) {
                std::size_t count = 0;
                for (; count < batch && iterator != end; ++count, ++iterator) {
                    answers[count] = std::invoke(projection, *iterator);
                }
                block(std::span<const value_type>{answers.data(), count});
            }
        }
    }
};

writer() -> writer<>;
writer(std::FILE*) -> writer<stdio_sink>;
template <byte_sink Sink> writer(Sink) -> writer<Sink>;

} // namespace canard::io
#ifdef CANARD_CLIENT_AVX2
#ifndef __AVX2__
#error "The optimized canard judge pipeline requires an AVX2 compilation target."
#endif
// ===== include/canard/io/source/checked_linux_mapped_file.hpp =====
#if !defined(__linux__)
#error "checked_linux_mapped_file is a Linux source adapter."
#endif
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <system_error>
#include <utility>
namespace canard::io {
// Maps the entire regular file, irrespective of its current descriptor offset.
// The descriptor is borrowed. Setup is checked once; readers still use trusted
// padded views. The file must not shrink or be concurrently modified afterward.
class checked_linux_mapped_file {
    void* mapping_ = MAP_FAILED;
    std::size_t mapping_size_ = 0, bytes_ = 0;
    const char* data_ = nullptr;
    static void fail(const char* what) {
        throw std::system_error(errno, std::generic_category(), what);
    }
  public:
    explicit checked_linux_mapped_file(int descriptor = STDIN_FILENO) {
        struct stat status{};
        if (::fstat(descriptor, &status) != 0)
            fail("canard fstat");
        if (!S_ISREG(status.st_mode) || status.st_size < 0)
            throw std::invalid_argument("canard mapping needs a regular file");
        const long page_size = ::sysconf(_SC_PAGESIZE);
        if (page_size < 32)
            throw std::runtime_error("canard: invalid system page size");
        const auto page = static_cast<std::size_t>(page_size);
        if (static_cast<std::uintmax_t>(status.st_size) >
            std::numeric_limits<std::size_t>::max() - 3 * page)
            throw std::length_error("canard mapping too large");
        bytes_ = static_cast<std::size_t>(status.st_size);
        const auto rounded = ((bytes_ + page - 1) / page) * page;
        mapping_size_ = rounded + 2 * page;
        mapping_ = ::mmap(nullptr, mapping_size_, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mapping_ == MAP_FAILED)
            fail("canard mmap reserve");
        data_ = static_cast<const char*>(mapping_) + page;
        if (bytes_ != 0 && ::mmap(const_cast<char*>(data_), bytes_, PROT_READ,
                                   MAP_PRIVATE | MAP_FIXED, descriptor, 0) == MAP_FAILED) {
            const int error = errno;
            ::munmap(mapping_, mapping_size_);
            mapping_ = MAP_FAILED;
            throw std::system_error(error, std::generic_category(), "canard mmap file");
        }
    }
    checked_linux_mapped_file(const checked_linux_mapped_file&) = delete;
    checked_linux_mapped_file& operator=(const checked_linux_mapped_file&) = delete;
    checked_linux_mapped_file(checked_linux_mapped_file&& other) noexcept
        : mapping_(std::exchange(other.mapping_, MAP_FAILED)), mapping_size_(other.mapping_size_),
          bytes_(std::exchange(other.bytes_, 0)), data_(std::exchange(other.data_, nullptr)) {}
    ~checked_linux_mapped_file() {
        if (mapping_ != MAP_FAILED) ::munmap(mapping_, mapping_size_);
    }
    [[nodiscard]] padded_bytes_view view() const & noexcept { return {data_, bytes_}; }
    void view() const && = delete;
};
}
// ===== include/canard/io/sink/posix.hpp =====
#include <cerrno>
#include <unistd.h>
namespace canard::io {
// Expert sink retaining the old successful, complete-write precondition.
class trusted_posix_sink {
    int descriptor_;
  public:
    static constexpr bool assume_complete_writes = true;
    explicit trusted_posix_sink(int descriptor = STDOUT_FILENO) noexcept : descriptor_(descriptor) {}
    void write_all(const char* data, std::size_t size) const noexcept {
        ::write(descriptor_, data, size);
    }
};
class posix_sink {
    int descriptor_;
  public:
    static constexpr bool assume_complete_writes = false;
    explicit posix_sink(int descriptor = STDOUT_FILENO) noexcept : descriptor_(descriptor) {}
    transfer_result write_some(const char* data, std::size_t size) const noexcept {
        ssize_t count;
        do { count = ::write(descriptor_, data, size); } while (count < 0 && errno == EINTR);
        if (count < 0) return {0, {errno, std::generic_category()}};
        return {static_cast<std::size_t>(count), {}};
    }
};
}
namespace canard::judge {
using execution = canard::execution::avx2;
class input
    : private canard::io::checked_linux_mapped_file,
      public canard::io::reader<canard::io::input_mode::trusted_view, canard::execution::avx2> {
    using file = canard::io::checked_linux_mapped_file;
    using reader_type =
        canard::io::reader<canard::io::input_mode::trusted_view, canard::execution::avx2>;

  public:
    input() : file(), reader_type(file::view()) {}
};
using output = canard::io::writer<canard::io::trusted_posix_sink, canard::execution::avx2>;
} // namespace canard::judge
#else
namespace canard::judge {
using execution = canard::execution::scalar;
using input = canard::io::reader<canard::io::input_mode::checked_stream, execution>;
using output = canard::io::writer<canard::io::stdio_sink, execution>;
} // namespace canard::judge
#endif
// ===== include/canard/graph/tree_decomposition_width_2.hpp =====
// ===== include/canard/kernel/tree_width_two.hpp =====
// ===== include/canard/structural/series_reduction_graph.hpp =====
#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <functional>
#include <span>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

namespace canard::structural {
// A half-edge's list position never changes. Series suppression reconnects the
// two surviving reverse half-edges in place. Dead list entries are scanned only
// when their vertex is eliminated (and at most once for dictionary seeding).
struct series_arc {
    using index_type = std::uint32_t;
    static constexpr index_type no_vertex = UINT32_MAX, no_arc = UINT32_MAX;
    index_type vertex_, next_, reverse_;
    series_arc() noexcept {}
    static series_arc pending(index_type u, index_type v) noexcept { return {u,v,0}; }
    std::array<index_type,2> pending_endpoints() const noexcept { return {vertex_,next_}; }
    series_arc(index_type vertex, index_type next, index_type reverse) noexcept
        : vertex_(vertex), next_(next), reverse_(reverse) {}
    index_type vertex() const noexcept { return vertex_; }
    index_type next() const noexcept { return next_; }
    index_type reverse() const noexcept { return reverse_; }
    void set_next(index_type next) noexcept { next_ = next; }
    void erase() noexcept { vertex_ = no_vertex; }
    void reconnect(index_type vertex, index_type reverse) noexcept {
        vertex_ = vertex; reverse_ = reverse;
    }
};

// Bounded representation, not a different algorithm. 20 vertex bits and 21
// incidence bits cover up to 1,048,575 vertices and 1,048,575 undirected edges.
template <unsigned VertexBits = 20, unsigned ArcBits = 21>
    requires(VertexBits > 0 && ArcBits > 0 && VertexBits < 32 && ArcBits < 32 &&
             VertexBits + 2 * ArcBits <= 64)
struct packed_series_arc {
    using index_type = std::uint32_t;
    static constexpr index_type no_vertex = (index_type{1} << VertexBits) - 1;
    static constexpr index_type no_arc = (index_type{1} << ArcBits) - 1;
    std::uint64_t bits_;
    packed_series_arc() noexcept {}
    static packed_series_arc pending(index_type u, index_type v) noexcept {
        packed_series_arc result;result.bits_=(std::uint64_t{u}<<32)|v;return result;
    }
    std::array<index_type,2> pending_endpoints() const noexcept { return {static_cast<index_type>(bits_>>32),static_cast<index_type>(bits_)}; }
    packed_series_arc(index_type vertex, index_type next, index_type reverse) noexcept
        : bits_(std::uint64_t{vertex} | (std::uint64_t{next} << VertexBits) |
                (std::uint64_t{reverse} << (VertexBits + ArcBits))) {}
    index_type vertex() const noexcept { return bits_ & no_vertex; }
    index_type next() const noexcept { return (bits_ >> VertexBits) & no_arc; }
    index_type reverse() const noexcept { return bits_ >> (VertexBits + ArcBits); }
    void set_next(index_type next) noexcept {
        bits_ = (bits_ & ~(std::uint64_t{no_arc} << VertexBits)) | (std::uint64_t{next} << VertexBits);
    }
    void erase() noexcept { bits_ |= no_vertex; }
    void reconnect(index_type vertex, index_type reverse) noexcept {
        bits_ = (bits_ & (std::uint64_t{no_arc} << VertexBits)) | vertex |
                (std::uint64_t{reverse} << (VertexBits + ArcBits));
    }
};
static_assert(sizeof(packed_series_arc<>) == 8);

template <std::uint32_t NoArc> struct series_vertex {
    using index_type = std::uint32_t;
    index_type degree_ = 0, head_ = NoArc - 1, xor_ = 0;
    index_type degree() const noexcept { return degree_; }
    index_type head() const noexcept { return head_; }
    index_type neighbor_xor() const noexcept { return xor_; }
    void set_head(index_type h) noexcept { head_ = h; }
    void insert_neighbor(index_type v) noexcept { ++degree_; xor_ ^= v; }
    void erase_neighbor(index_type v) noexcept { --degree_; xor_ ^= v; }
    void replace_neighbor(index_type a,index_type b) noexcept { xor_ ^= a ^ b; }
    void retire(index_type rank) noexcept { head_ = NoArc; degree_ = rank; }
};

template <unsigned VertexBits, unsigned ArcBits>
struct packed_series_vertex {
    using index_type = std::uint32_t;
    static constexpr index_type vertex_mask=(index_type{1}<<VertexBits)-1;
    static constexpr index_type arc_mask=(index_type{1}<<ArcBits)-1;
    static constexpr unsigned xor_shift=VertexBits+ArcBits;
    std::uint64_t bits_=std::uint64_t{arc_mask-1}<<VertexBits;
    index_type degree() const noexcept { return bits_ & vertex_mask; }
    index_type head() const noexcept { return (bits_>>VertexBits)&arc_mask; }
    index_type neighbor_xor() const noexcept { return bits_>>xor_shift; }
    void set_head(index_type h) noexcept {
        bits_=(bits_&~(std::uint64_t{arc_mask}<<VertexBits))|(std::uint64_t{h}<<VertexBits);
    }
    void insert_neighbor(index_type v) noexcept { bits_=(bits_+1)^(std::uint64_t{v}<<xor_shift); }
    void erase_neighbor(index_type v) noexcept { bits_=(bits_-1)^(std::uint64_t{v}<<xor_shift); }
    void replace_neighbor(index_type a,index_type b) noexcept { bits_^=std::uint64_t{a^b}<<xor_shift; }
    void retire(index_type rank) noexcept { bits_=rank|(std::uint64_t{arc_mask}<<VertexBits); }
};
static_assert(sizeof(packed_series_vertex<20,21>)==8);

template <typename Arc> struct series_vertex_for { using type=series_vertex<Arc::no_arc>; };
template <unsigned V,unsigned A> struct series_vertex_for<packed_series_arc<V,A>> {
    using type=std::conditional_t<(2*V+A<=64),packed_series_vertex<V,A>,series_vertex<packed_series_arc<V,A>::no_arc>>;
};

template <typename Edge>
concept series_edge = requires(const Edge& e) {
    requires std::integral<std::remove_cvref_t<decltype(std::get<0>(e))>>;
    requires std::integral<std::remove_cvref_t<decltype(std::get<1>(e))>>;
};

template <typename Arc = series_arc> class series_reduction_graph {
  public:
    using index_type = std::uint32_t;
    static constexpr index_type no_arc = Arc::no_arc, no_vertex = Arc::no_vertex;
    static constexpr index_type unbuilt = no_arc - 1;
    using vertex_state=typename series_vertex_for<Arc>::type;
    std::vector<vertex_state> vertices;
    std::vector<Arc> arcs;
    bool materialized = false;

    template <std::ranges::input_range Edges, typename Projection = std::identity>
        requires series_edge<std::remove_cvref_t<std::invoke_result_t<Projection&, std::ranges::range_reference_t<Edges>>>>
    series_reduction_graph(index_type n, Edges&& edges, Projection project = {}) : vertices(n) {
        if (n > no_vertex) throw std::length_error("canard: vertex representation exhausted");
        if constexpr (std::ranges::sized_range<Edges>) {
            const auto count = std::ranges::size(edges);
            if (count > no_arc / 2) throw std::length_error("canard: incidence representation exhausted");
            arcs.reserve(2 * static_cast<std::size_t>(count));
        }
        const auto append = [&](const auto& input_edge) {
            const auto edge = std::invoke(project, input_edge);
            const auto a = std::get<0>(edge), b = std::get<1>(edge);
            // Simple graph, in-range endpoints, and no parallel input edges are
            // semantic preconditions. Bounds/self-loops are checked in debug.
            assert(std::in_range<index_type>(a) && std::in_range<index_type>(b));
            const index_type u = static_cast<index_type>(a), v = static_cast<index_type>(b);
            assert(u < n && v < n && u != v);
            if (arcs.size() >= no_arc / 2)
                throw std::length_error("canard: incidence representation exhausted");
            arcs.push_back(Arc::pending(u,v));
            vertices[u].insert_neighbor(v); vertices[v].insert_neighbor(u);
        };
        using input_type = std::ranges::range_value_t<Edges>;
        if constexpr (requires(std::span<input_type> block) { edges.consume_block(block); edges.size(); }) {
            // Honor the foundation's counted-stream customization. Only eight
            // records are buffered; the projection must not read this stream.
            std::array<input_type,8> block{};
            while (edges.size()!=0) {
                const auto count=edges.consume_block(std::span<input_type>{block});
                for(std::size_t i=0;i<count;++i)append(block[i]);
            }
        } else {
            for(const auto& edge:edges)append(edge);
        }
    }
    // Pending input uses one Arc slot per undirected edge. Leaf peeling needs
    // only degrees and neighbor XORs. Materialize the remaining 2-core in the
    // SAME allocation, backwards, so unread pending records cannot be clobbered.
    // Unreachable dead slots need not be initialized or inspected.
    void materialize_core() {
        assert(!materialized);
        const auto m=arcs.size();arcs.resize(2*m);
        for(std::size_t i=m;i--!=0;) {
            const auto [u,v]=arcs[i].pending_endpoints();
            if(vertices[u].head()==no_arc || vertices[v].head()==no_arc)continue;
            const auto id=static_cast<index_type>(2*i);
            arcs[id]=Arc(v,vertices[u].head()==unbuilt?no_arc:vertices[u].head(),id+1);
            arcs[id+1]=Arc(u,vertices[v].head()==unbuilt?no_arc:vertices[v].head(),id);
            vertices[u].set_head(id);vertices[v].set_head(id+1);
        }
        materialized=true;
    }
    index_type peel_leaf(index_type u) noexcept {
        assert(!materialized && vertices[u].degree()==1);
        const auto v=vertices[u].neighbor_xor();
        vertices[v].erase_neighbor(u);
        return v;
    }
    index_type first(index_type u) const noexcept {
        auto e = vertices[u].head();
        while (e != no_arc && arcs[e].vertex() == no_vertex) e = arcs[e].next();
        return e;
    }
    std::array<index_type, 2> two(index_type u) const noexcept {
        const auto e = first(u);
        auto f = arcs[e].next();
        while (f != no_arc && arcs[f].vertex() == no_vertex) f = arcs[f].next();
        assert(f != no_arc);
        return {e, f};
    }
    bool adjacent(index_type u, index_type v) const noexcept {
        if (vertices[u].degree() > vertices[v].degree()) std::swap(u, v);
        for (auto e = vertices[u].head(); e != no_arc; e = arcs[e].next())
            if (arcs[e].vertex() == v) return true;
        return false;
    }
    // Bounded-degree lookup, unlinking dead entries as they are encountered.
    // Each dead half-edge is bypassed at most once, even when a once-large hub
    // later has small degree and is repeatedly queried during suppressions.
    bool adjacent_compact(index_type u, index_type v) noexcept {
        auto e = first(u);
        vertices[u].set_head(e);
        while (e != no_arc) {
            if (arcs[e].vertex() == v) return true;
            auto next = arcs[e].next();
            while (next != no_arc && arcs[next].vertex() == no_vertex) next = arcs[next].next();
            arcs[e].set_next(next);
            e = next;
        }
        return false;
    }
    // Enumerate current simple edges whose endpoints both exceed the limit.
    template <typename Function> void high_edges(index_type limit, Function&& function) const {
        for (index_type u = 0; u < vertices.size(); ++u) {
            if (vertices[u].degree() <= limit) continue;
            for (auto e = vertices[u].head(); e != no_arc; e = arcs[e].next()) {
                const auto v = arcs[e].vertex();
                if (v != no_vertex && u < v && vertices[v].degree() > limit) function(u, v);
            }
        }
    }
    template <typename Function> void live_edges(Function&& function) const {
        if(!materialized) {
            for(const auto& record:arcs) {
                auto [u,v]=record.pending_endpoints();
                if(vertices[u].head()!=no_arc && vertices[v].head()!=no_arc) {
                    if(u>v)std::swap(u,v);function(u,v);
                }
            }
            return;
        }
        for (index_type u = 0; u < vertices.size(); ++u)
            for (auto e = vertices[u].head(); e != no_arc; e = arcs[e].next()) {
                const auto v = arcs[e].vertex();
                if (v != no_vertex && u < v) function(u, v);
            }
    }
    void retire(index_type u, index_type rank) noexcept {
        vertices[u].retire(rank); // Degree storage becomes elimination ranks.
    }
    index_type remove_leaf(index_type u) noexcept {
        const auto e = first(u), v = arcs[e].vertex(), reverse = arcs[e].reverse();
        arcs[e].erase(); arcs[reverse].erase();
        vertices[v].erase_neighbor(u);
        return v;
    }
    void remove_triangle(index_type u, index_type e, index_type f) noexcept {
        const auto a = arcs[e].vertex(), b = arcs[f].vertex();
        arcs[arcs[e].reverse()].erase(); arcs[arcs[f].reverse()].erase();
        arcs[e].erase(); arcs[f].erase();
        vertices[a].erase_neighbor(u); vertices[b].erase_neighbor(u);
    }
    void suppress(index_type u, index_type e, index_type f) noexcept {
        const auto a = arcs[e].vertex(), b = arcs[f].vertex();
        const auto p = arcs[e].reverse(), q = arcs[f].reverse();
        arcs[p].reconnect(b, q); arcs[q].reconnect(a, p);
        arcs[e].erase(); arcs[f].erase();
        vertices[a].replace_neighbor(u,b);
        vertices[b].replace_neighbor(u,a);
    }
};
} // namespace canard::structural
// ===== include/canard/hash/edge_table.hpp =====
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
namespace canard::hash {
[[nodiscard]] inline std::uint64_t undirected_key(std::uint32_t u, std::uint32_t v) noexcept {
    if (u > v)
        std::swap(u, v);
    return (std::uint64_t{u} << 32) | v;
}
// Full-key comparison; deterministic hashing affects speed, never correctness.
// Back-shift deletion prevents tombstone accumulation. IDs must be nonzero.
class edge_table {
    std::vector<std::uint64_t> keys_;
    std::vector<std::uint32_t> values_;
    std::size_t size_ = 0;
    static std::uint64_t mix(std::uint64_t x) noexcept {
        x ^= x >> 30;
        x *= 0xbf58476d1ce4e5b9ULL;
        x ^= x >> 27;
        x *= 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    std::size_t bucket(std::uint64_t k) const noexcept {
        return mix(k) & (keys_.size() - 1);
    }
    void rehash(std::size_t capacity) {
        auto keys = std::move(keys_);
        auto values = std::move(values_);
        keys_.assign(capacity, 0);
        values_.assign(capacity, 0);
        size_ = 0;
        for (std::size_t i = 0; i < values.size(); ++i)
            if (values[i])
                insert(keys[i], values[i]);
    }

  public:
    explicit edge_table(std::size_t expected = 0) {
        const auto capacity = std::bit_ceil(std::max<std::size_t>(16, 2 * expected));
        keys_.resize(capacity);
        values_.resize(capacity);
    }
    edge_table(const edge_table&) = default;
    edge_table& operator=(const edge_table&) = default;
    edge_table(edge_table&& other) noexcept
        : keys_(std::move(other.keys_)), values_(std::move(other.values_)),
          size_(std::exchange(other.size_, 0)) {}
    edge_table& operator=(edge_table&& other) noexcept {
        if (this != &other) {
            keys_ = std::move(other.keys_);
            values_ = std::move(other.values_);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }
    [[nodiscard]] std::uint32_t find(std::uint64_t key) const noexcept {
        if (keys_.empty())
            return 0;
        auto i = bucket(key);
        const auto mask = keys_.size() - 1;
        while (values_[i]) {
            if (keys_[i] == key)
                return values_[i];
            i = (i + 1) & mask;
        }
        return 0;
    }
    void insert(std::uint64_t key, std::uint32_t id) {
        assert(id);
        if (keys_.empty())
            rehash(16);
        if ((size_ + 1) * 4 > keys_.size() * 3)
            rehash(keys_.size() * 2);
        auto i = bucket(key);
        const auto mask = keys_.size() - 1;
        while (values_[i]) {
            assert(keys_[i] != key);
            i = (i + 1) & mask;
        }
        keys_[i] = key;
        values_[i] = id;
        ++size_;
    }
    std::uint32_t erase(std::uint64_t key) noexcept {
        if (keys_.empty())
            return 0;
        auto hole = bucket(key);
        const auto mask = keys_.size() - 1;
        while (values_[hole] && keys_[hole] != key)
            hole = (hole + 1) & mask;
        if (!values_[hole])
            return 0;
        const auto id = values_[hole];
        --size_;
        auto next = (hole + 1) & mask;
        while (values_[next]) {
            const auto home = bucket(keys_[next]);
            if (((hole - home) & mask) < ((next - home) & mask)) {
                keys_[hole] = keys_[next];
                values_[hole] = values_[next];
                hole = next;
            }
            next = (next + 1) & mask;
        }
        values_[hole] = 0;
        return id;
    }
};
} // namespace canard::hash
#include <array>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace canard::kernel {
struct width_two_options {
    bool capture_obstruction = false;
    bool packed_incidence = true;
    std::uint32_t small_scan_limit = 8;
};
struct width_two_data {
    static constexpr std::uint32_t npos = UINT32_MAX;
    std::vector<std::array<std::uint32_t, 2>> later;
    std::vector<std::uint32_t> order;
    std::vector<std::uint32_t> residual_vertices;
    std::vector<std::array<std::uint32_t, 2>> residual_edges;
    std::uint32_t root = npos, width = 0;
    std::uint32_t high_edge_keys = 0, small_bags = 0;
    bool obstruction_captured = false;
    bool valid() const noexcept { return later.size() == order.size(); }
};

// Every surviving degree is nonincreasing. Thus an edge with two current
// high-degree endpoints cannot have been removed by a previous elimination.
// Only these pairs need a dictionary, and entries never need deletion.
template <typename Graph> class monotone_high_edges {
    Graph* graph_;
    std::uint32_t limit_;
    std::uint64_t last_ = UINT64_MAX;
    std::optional<hash::edge_table> table_;
  public:
    explicit monotone_high_edges(Graph& graph, std::uint32_t limit) noexcept : graph_(&graph), limit_(limit) {}
    bool contains_or_insert(std::uint32_t a, std::uint32_t b) {
        const auto key = hash::undirected_key(a, b);
        if (key == last_) return true;
        if (last_ == UINT64_MAX) {
            // One scan is linear in total incidence size. A repeated parallel
            // bundle then requires neither a dictionary nor another scan.
            last_ = key;
            return graph_->adjacent(a, b);
        }
        if (!table_) {
            std::size_t count = 0;
            graph_->high_edges(limit_, [&](auto, auto) { ++count; });
            table_.emplace(count);
            graph_->high_edges(limit_, [&](auto u, auto v) {
                table_->insert(hash::undirected_key(u, v), 1);
            });
        }
        last_ = key;
        if (table_->find(key)) return true;
        table_->insert(key, 1);
        return false;
    }
    std::uint32_t key_count() const noexcept {
        return table_ ? static_cast<std::uint32_t>(table_->size()) : (last_ != UINT64_MAX);
    }
};

template <typename Arc, std::ranges::input_range Edges, typename Projection = std::identity>
width_two_data eliminate_width_two(std::uint32_t n, Edges&& edges, width_two_options options, Projection project = {}) {
    structural::series_reduction_graph<Arc> graph(n, std::forward<Edges>(edges), std::move(project));
    width_two_data result;
    result.later.assign(n, {result.npos, result.npos});
    result.order.reserve(n);
    // Peel all leaves first: forests need no fills, and get width <= 1.
    for (std::uint32_t u = 0; u < n; ++u)
        if (graph.vertices[u].degree() <= 1) result.order.push_back(u);
    std::size_t read = 0;
    while (read != result.order.size()) {
        const auto u = result.order[read];
        if (graph.vertices[u].degree() != 0) {
            const auto v = graph.peel_leaf(u);
            result.later[u][0] = v;
            result.width = 1;
            if (graph.vertices[v].degree() == 1) result.order.push_back(v);
        }
        graph.retire(u, static_cast<std::uint32_t>(read++));
    }
    result.small_bags = static_cast<std::uint32_t>(read);
    // In the remaining 2-core, a vertex is enqueued exactly once: initially at
    // degree two or at its only 3 -> 2 transition. Queue storage is the witness.
    for (std::uint32_t u = 0; u < n; ++u)
        if (graph.vertices[u].head() != graph.no_arc && graph.vertices[u].degree() == 2)
            result.order.push_back(u);
    if (read != result.order.size()) graph.materialize_core();
    const auto scan_limit = std::max(std::uint32_t{2}, options.small_scan_limit);
    monotone_high_edges high(graph, scan_limit);
    while (read != result.order.size()) {
        const auto u = result.order[read];
        const auto degree = graph.vertices[u].degree();
        result.small_bags += degree < 2;
        if (degree == 1) {
            const auto v = graph.remove_leaf(u);
            result.later[u][0] = v;
            if (graph.vertices[v].degree() == 2) result.order.push_back(v);
        } else if (degree == 2) {
            const auto [e, f] = graph.two(u);
            const auto a = graph.arcs[e].vertex(), b = graph.arcs[f].vertex();
            result.later[u] = {a, b};
            result.width = 2;
            const auto da = graph.vertices[a].degree(), db = graph.vertices[b].degree();
            const bool duplicate = da <= 2
                ? da == 2 && graph.vertices[a].neighbor_xor() == (u ^ b)
                : db <= 2 ? db == 2 && graph.vertices[b].neighbor_xor() == (u ^ a)
                          : std::min(da, db) <= scan_limit
                              ? da <= db ? graph.adjacent_compact(a, b) : graph.adjacent_compact(b, a)
                          : high.contains_or_insert(a, b);
            if (duplicate) {
                graph.remove_triangle(u, e, f);
                if (graph.vertices[a].degree() == 2) result.order.push_back(a);
                if (graph.vertices[b].degree() == 2) result.order.push_back(b);
            } else {
                graph.suppress(u, e, f);
            }
        }
        graph.retire(u, static_cast<std::uint32_t>(read++));
    }
    result.high_edge_keys = high.key_count();
    if (!result.valid()) {
        if (options.capture_obstruction) {
            result.obstruction_captured = true;
            for (std::uint32_t u = 0; u < n; ++u)
                if (graph.vertices[u].head() != graph.no_arc)
                    result.residual_vertices.push_back(u);
            graph.live_edges([&](auto u, auto v) { result.residual_edges.push_back({u, v}); });
        }
        return result;
    }
    if (n != 0) result.root = result.order.back();
    for (auto& pair : result.later)
        if (pair[1] != result.npos &&
            graph.vertices[pair[0]].degree() > graph.vertices[pair[1]].degree())
            std::swap(pair[0], pair[1]);
    return result;
}
} // namespace canard::kernel
// ===== include/canard/format/owner.hpp =====
#include <format>
#include <utility>
namespace canard::format {
// Delegation preserves the description's parser. Formatting an owner never
// walks its values; use an explicitly constructed snapshot for content output.
template <typename Owner> struct description_formatter {
    using description_type = decltype(std::declval<const Owner&>().description());
    std::formatter<description_type, char> base;
    constexpr auto parse(std::format_parse_context& context) {
        return base.parse(context);
    }
    template <typename Context>
    auto format(const Owner& owner, Context& context) const {
        return base.format(owner.description(), context);
    }
};
}
#include <array>
#include <cassert>
#include <cstdint>
#include <format>
#include <ranges>
#include <span>
#include <utility>

namespace canard::graph {
struct width_two_bag {
    std::array<std::uint32_t, 3> vertices{};
    std::uint32_t count = 0;
    auto begin() const noexcept { return vertices.begin(); }
    auto end() const noexcept { return vertices.begin() + count; }
    std::size_t size() const noexcept { return count; }
    std::uint32_t operator[](std::size_t i) const noexcept { assert(i < count); return vertices[i]; }
};
struct width_two_description { std::uint32_t vertices, eliminated, width; bool valid; };
using width_two_options = kernel::width_two_options;
} // namespace canard::graph

namespace canard {
// An exact recognizer and owning elimination/decomposition witness for simple
// undirected graphs. Vertex and bag IDs are zero-based. Const queries allocate
// nothing; views borrow this owner. No recursion or separate input-edge allocation is used.
class tree_decomposition_width_2 {
    kernel::width_two_data data_;
  public:
    using vertex_type = std::uint32_t;
    using size_type = std::size_t;
    using bag_type = graph::width_two_bag;
    static constexpr vertex_type npos = UINT32_MAX;
    tree_decomposition_width_2() = default;
    template <std::ranges::input_range Edges>
        requires structural::series_edge<std::ranges::range_value_t<Edges>>
    explicit tree_decomposition_width_2(vertex_type n, Edges&& edges,
                                       graph::width_two_options options = {})
        : tree_decomposition_width_2(n, std::forward<Edges>(edges), std::identity{}, options) {}

    // Pure endpoint projection; counted I/O views may decode eight records at
    // once. Do not access that same reader from inside the projection.
    template <std::ranges::input_range Edges, typename Projection>
        requires std::invocable<Projection&, std::ranges::range_reference_t<Edges>> &&
                 structural::series_edge<std::remove_cvref_t<
                     std::invoke_result_t<Projection&, std::ranges::range_reference_t<Edges>>>>
    tree_decomposition_width_2(vertex_type n, Edges&& edges, Projection project,
                               graph::width_two_options options = {}) {
        if constexpr (std::ranges::sized_range<Edges>) {
            using packed = structural::packed_series_arc<>;
            if (options.packed_incidence && n <= packed::no_vertex &&
                std::ranges::size(edges) <= packed::no_arc / 2) {
                data_ = kernel::eliminate_width_two<packed>(n, std::forward<Edges>(edges), options, std::move(project));
                return;
            }
        }
        data_ = kernel::eliminate_width_two<structural::series_arc>(
            n, std::forward<Edges>(edges), options, std::move(project));
    }
    tree_decomposition_width_2(const tree_decomposition_width_2&) = default;
    tree_decomposition_width_2& operator=(const tree_decomposition_width_2&) = default;
    tree_decomposition_width_2(tree_decomposition_width_2&& other) noexcept
        : data_(std::exchange(other.data_, {})) {}
    tree_decomposition_width_2& operator=(tree_decomposition_width_2&& other) noexcept {
        if (this != &other) data_ = std::exchange(other.data_, {});
        return *this;
    }
    void swap(tree_decomposition_width_2& other) noexcept { std::swap(data_, other.data_); }
    friend void swap(tree_decomposition_width_2& a, tree_decomposition_width_2& b) noexcept { a.swap(b); }
    bool valid() const noexcept { return data_.valid(); }
    explicit operator bool() const noexcept { return valid(); }
    vertex_type vertex_count() const noexcept { return static_cast<vertex_type>(data_.later.size()); }
    vertex_type bag_count() const noexcept { return valid() ? vertex_count() : 0; }
    int width() const noexcept { return valid() ? static_cast<int>(data_.width) : -1; }
    vertex_type root() const noexcept { return data_.root; }
    std::span<const vertex_type> elimination_order() const noexcept { return data_.order; }
    std::span<const std::array<vertex_type, 2>> later_neighbors() const noexcept { return data_.later; }
    const std::array<vertex_type, 2>& later_neighbors(vertex_type u) const noexcept {
        assert(valid() && u < vertex_count()); return data_.later[u];
    }
    bag_type bag(vertex_type u) const noexcept {
        assert(valid() && u < vertex_count());
        const auto [a,b] = data_.later[u];
        return {{u,a,b}, 1u + (a != npos) + (b != npos)};
    }
    vertex_type parent(vertex_type u) const noexcept {
        assert(valid() && u < vertex_count());
        const auto a = data_.later[u][0];
        return a != npos ? a : u == root() ? npos : root();
    }
    auto bags() const & {
        return std::views::iota(vertex_type{0}, bag_count()) |
               std::views::transform([this](vertex_type u) { return bag(u); });
    }
    auto bags() const && = delete;
    bool has_obstruction() const noexcept { return data_.obstruction_captured; }
    std::span<const vertex_type> residual_vertices() const noexcept { return data_.residual_vertices; }
    std::span<const std::array<vertex_type, 2>> residual_edges() const noexcept { return data_.residual_edges; }
    vertex_type small_bag_count() const noexcept { return data_.small_bags; }
    vertex_type high_edge_key_count() const noexcept { return data_.high_edge_keys; }
    graph::width_two_description description() const noexcept {
        return {vertex_count(), static_cast<vertex_type>(data_.order.size()), data_.width, valid()};
    }
};
} // namespace canard

template <> struct std::formatter<canard::graph::width_two_description, char> {
    constexpr auto parse(std::format_parse_context& c) { return c.begin(); }
    template <typename Context> auto format(canard::graph::width_two_description x, Context& c) const {
        return std::format_to(c.out(), "tree_decomposition_width_2(vertices={}, eliminated={}, width={})",
                              x.vertices, x.eliminated, x.valid ? static_cast<int>(x.width) : -1);
    }
};
template <> struct std::formatter<canard::tree_decomposition_width_2, char>
    : canard::format::description_formatter<canard::tree_decomposition_width_2> {};
#include <cstdint>
#include <ranges>
#include <utility>

int main() {
    namespace io = canard::io;
    using u32 = std::uint32_t;
    using field = io::bounded<u32, 500000>;
    canard::judge::input input;
    canard::judge::output output;
    input.read(io::expect{"p"});
    input.read(io::expect{"tw"});
    const auto [n, m] = input.read<field, field>();
    auto edges = input.read<field, field>(m);
    canard::tree_decomposition_width_2 decomposition(n, edges, [](const auto& edge) {
        const auto [u, v] = edge;
        return std::pair{u - 1, v - 1};
    });
    if (!decomposition) {
        output.write(-1);
        output.finish();
        return 0;
    }
    output.write("s td", n, static_cast<u32>(decomposition.width()), n);
    for (u32 u = 0; u < n; ++u) {
        const auto [a,b] = decomposition.later_neighbors(u);
        if (b != decomposition.npos) output.write("b", u+1, u+1, a+1, b+1);
        else if (a != decomposition.npos) output.write("b", u+1, u+1, a+1);
        else output.write("b", u+1, u+1);
    }
    for (u32 u = 0; u < n; ++u) {
        const auto p = decomposition.parent(u);
        if (p != decomposition.npos) output.write(p+1, u+1);
    }
    output.finish();
}
