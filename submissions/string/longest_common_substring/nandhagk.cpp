#if defined(__GNUC__) && !defined(__clang__)
#include <bits/stdc++.h>
#endif
// canard longest_common_substring: Linux regular-file stdin, AVX2 CPU required.
// GCC self-targets. With Clang, compile with -mavx2.
#if !defined(__AVX2__)
#if defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma GCC target("avx2")
#define __AVX2__ 1
#else
#error "Compile this AVX2 export with -mavx2, or use the portable export."
#endif
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC optimize("O3")
#endif
#define CANARD_CLIENT_AVX2 1
// Generated from canard headers and client. Edit the maintained sources.
// ===== clients/longest_common_substring.cpp =====
// ===== include/canard/judge/io.hpp =====
// Build adapter for these judge examples, not a different numeric API.
// The portable pipeline accepts pipes and checks input. The optimized pipeline
// promises regular-file input and trusted single-separated numeric records.
// Both use io::reader/io::writer; neither client chooses a codec or formatter.
// ===== include/canard/io.hpp =====
// The ordinary entry point: checked streaming input and checked buffered
// output. Execution is selected from the compilation target. Numeric schemas
// and record/range shapes choose kernels internally, never at runtime by ISA.
// ===== include/canard/io/reader.hpp =====

// ===== include/canard/io/schema.hpp =====

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

namespace canard::io {

// A schema, not a numeric wrapper. read<bounded<T, Max>>() returns an ordinary T.
// Checked input validates the bound; trusted input treats it as a precondition.
// Trusted input also promises at most decimal_digits(Maximum) digits: arbitrary
// extra leading zeroes need the checked source or a wider field schema.
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
// ===== include/canard/io/detail/record_decoder.hpp =====
// ===== include/canard/io/text.hpp =====
#include <cstddef>
#include <limits>
#include <ranges>
#include <utility>
namespace canard::io {
// A bounded whitespace-delimited text token, not an integer schema.
struct text_token {
    std::size_t maximum_size = std::numeric_limits<std::size_t>::max();
};
namespace detail {
inline bool text_whitespace(unsigned char c) noexcept {
    return c == ' ' || (c >= '\t' && c <= '\r');
}
} // namespace detail
// A single variable-length row, distinct from write(range)'s one-row-per-element.
template <std::ranges::view Range> struct text_row {
    Range values;
};
template <std::ranges::viewable_range Range> [[nodiscard]] auto row(Range&& values) {
    return text_row<std::views::all_t<Range>>{std::views::all(std::forward<Range>(values))};
}
} // namespace canard::io
#include <string_view>
#include <stdexcept>

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
    [[nodiscard]] const char* position() const noexcept {
        return cursor_;
    }
    [[nodiscard]] std::string_view read_text(text_token schema) {
        while (cursor_ < end_ && text_whitespace(static_cast<unsigned char>(*cursor_)))
            ++cursor_;
        const char* begin = cursor_;
        while (cursor_ < end_ && !text_whitespace(static_cast<unsigned char>(*cursor_)))
            ++cursor_;
        const auto count = static_cast<std::size_t>(cursor_ - begin);
        if (count > schema.maximum_size)
            throw std::length_error("canard: text token too long");
        if (!count)
            throw std::runtime_error("canard: unexpected end of text input");
        return {begin, count};
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
#include <string>
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
    [[nodiscard]] std::string read_text(text_token schema) {
        skip();
        std::string result;
        for (;;) {
            source_->ensure(1);
            const auto window = source_->window();
            std::size_t count = 0;
            while (count < window.size &&
                   !detail::text_whitespace(static_cast<unsigned char>(window.data[count])))
                ++count;
            if (count > schema.maximum_size - result.size())
                throw std::length_error("canard: text token too long");
            if (count)
                result.append(window.data, count);
            source_->consume(count);
            if (count < window.size || window.size == 0)
                break;
        }
        if (result.empty())
            throw std::runtime_error("canard: unexpected end of text input");
        return result;
    }
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

    [[nodiscard]] std::string read_text(text_token schema) {
        return parser.read_text(schema);
    }

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

    // Checked stream returns an owning string; trusted view borrows the mapped input.
    [[nodiscard]] auto read(text_token schema) {
        return state::read_text(schema);
    }

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

    void write(std::string_view text) {
        base::write_bytes({text.data(), text.size()});
        ensure(1);
        *cursor_++ = '\n';
    }

    void write() {
        ensure(1);
        *cursor_++ = '\n';
    }

    template <std::ranges::view Range> void write(text_row<Range> record) {
        bool first = true;
        for (auto&& value : record.values) {
            ensure(32);
            if (!first)
                *cursor_++ = ' ';
            cursor_ = token(cursor_, value);
            first = false;
        }
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
// ===== include/canard/string/longest_common_substring.hpp =====
// ===== include/canard/kernel/common_substring_seeds.hpp =====
// ===== include/canard/kernel/lcp_sum.hpp =====
// ===== include/canard/kernel/byte_string.hpp =====
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <utility>
#if defined(__AVX2__)
#include <immintrin.h>
#endif
namespace canard::kernel {
template <typename Execution> struct byte_string_operations;
template <> struct byte_string_operations<execution::scalar> {
    static std::pair<unsigned, unsigned> extrema(const unsigned char* s, std::size_t n) {
        unsigned lo = 255, hi = 0;
        for (std::size_t i = 0; i < n; ++i) {
            lo = std::min(lo, unsigned(s[i]));
            hi = std::max(hi, unsigned(s[i]));
        }
        return {lo, hi};
    }
    static std::size_t mismatch(const unsigned char* a, const unsigned char* b, std::size_t n) {
        std::size_t i = 0;
        while (i < n && a[i] == b[i])
            ++i;
        return i;
    }
    static void descending(std::int32_t* out, int start, int step, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i)
            out[i] = start - static_cast<int>(i) * step;
    }
};
#if defined(__AVX2__)
template <> struct byte_string_operations<execution::avx2> {
    static std::pair<unsigned, unsigned> extrema(const unsigned char* s, std::size_t n) {
        auto lo = _mm256_set1_epi8(-1), hi = _mm256_setzero_si256();
        std::size_t i = 0;
        for (; n - i >= 32; i += 32) {
            const auto x = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(s + i));
            lo = _mm256_min_epu8(lo, x);
            hi = _mm256_max_epu8(hi, x);
        }
        alignas(32) unsigned char a[32], b[32];
        _mm256_store_si256(reinterpret_cast<__m256i*>(a), lo);
        _mm256_store_si256(reinterpret_cast<__m256i*>(b), hi);
        unsigned mn = 255, mx = 0;
        for (unsigned j = 0; j < 32; ++j) {
            mn = std::min(mn, unsigned(a[j]));
            mx = std::max(mx, unsigned(b[j]));
        }
        for (; i < n; ++i) {
            mn = std::min(mn, unsigned(s[i]));
            mx = std::max(mx, unsigned(s[i]));
        }
        return {mn, mx};
    }
    static std::size_t mismatch(const unsigned char* a, const unsigned char* b, std::size_t n) {
        std::size_t i = 0;
        for (; n - i >= 32; i += 32) {
            const auto x = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a + i));
            const auto y = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b + i));
            const auto eq =
                static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpeq_epi8(x, y)));
            if (eq != ~std::uint32_t{})
                return i + std::countr_one(eq);
        }
        return i + byte_string_operations<execution::scalar>::mismatch(a + i, b + i, n - i);
    }
    static void descending(std::int32_t* out, int start, int step, std::size_t count) {
        std::size_t i = 0;
        // The vector path is used only for small certified periods (step <= 64).
        if (step <= 64) {
            auto v = _mm256_sub_epi32(_mm256_set1_epi32(start),
                                      _mm256_mullo_epi32(_mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7),
                                                         _mm256_set1_epi32(step)));
            for (; count - i >= 8; i += 8) {
                _mm256_storeu_si256(reinterpret_cast<__m256i*>(out + i), v);
                v = _mm256_sub_epi32(v, _mm256_set1_epi32(8 * step));
            }
        }
        for (; i < count; ++i)
            out[i] = start - static_cast<int>(i) * step;
    }
};
#endif
// Lexicographic comparison with explicit bounds; no padding precondition.
template <typename Execution, typename Symbol>
bool suffix_less(const Symbol* s, int n, int a, int b) {
    const auto len = static_cast<std::size_t>(std::min(n - a, n - b));
    if constexpr (sizeof(Symbol) == 1) {
        const auto* bytes = reinterpret_cast<const unsigned char*>(s);
        const auto at = byte_string_operations<Execution>::mismatch(bytes + a, bytes + b, len);
        return at < len ? s[a + at] < s[b + at] : a > b;
    } else {
        for (std::size_t i = 0; i < len; ++i)
            if (s[a + i] != s[b + i])
                return s[a + i] < s[b + i];
        return a > b;
    }
}
} // namespace canard::kernel
#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>

namespace canard::kernel {

// Most LCP extensions stop within a word. Avoid loading two entire SIMD vectors
// in that case; use the shared bounded SIMD matcher only for the longer prefix.
template <typename Execution>
std::size_t extend_lcp(const unsigned char* a, const unsigned char* b, std::size_t available) {
    if (available >= sizeof(std::uint64_t)) {
        std::uint64_t x, y;
        std::memcpy(&x, a, sizeof(x));
        std::memcpy(&y, b, sizeof(y));
        const auto difference = x ^ y;
        if (difference) {
            if constexpr (std::endian::native == std::endian::little)
                return std::countr_zero(difference) / 8;
            else
                return std::countl_zero(difference) / 8;
        }
        return sizeof(x) + byte_string_operations<Execution>::mismatch(
                               a + sizeof(x), b + sizeof(x), available - sizeof(x));
    }
    return byte_string_operations<execution::scalar>::mismatch(a, b, available);
}

// Precondition: order is the exact suffix permutation for s[0..n), excluding the
// empty suffix. This is Kasai's carry invariant with lexicographic successors.
// Only the sum is needed, so a sequential successor map replaces inverse ranks
// and no LCP result array is allocated. At most n matched bytes are added overall.
template <typename Execution>
std::uint64_t adjacent_lcp_sum(const unsigned char* s, int n, const int* order) {
    if (n < 2)
        return 0;
    auto successor = std::make_unique_for_overwrite<int[]>(static_cast<std::size_t>(n));
    for (int rank = 0; rank < n - 1; ++rank)
        successor[order[rank]] = order[rank + 1];
    successor[order[n - 1]] = n;

    std::uint64_t sum = 0;
    std::size_t matched = 0;
    for (int i = 0; i < n; ++i) {
        const int j = successor[i];
        if (j == n) {
            matched = 0;
            continue;
        }
        const auto limit = static_cast<std::size_t>(n - std::max(i, j));
        matched += extend_lcp<Execution>(s + i + matched, s + j + matched, limit - matched);
        sum += matched;
        if (matched)
            --matched;
    }
    return sum;
}

} // namespace canard::kernel
// ===== include/canard/string/common_substring_match.hpp =====
#include <cstddef>
namespace canard {
// Offsets, not borrowed references. Any maximum is permitted; ties are unspecified.
struct common_substring_match {
    std::size_t first = 0, second = 0, length = 0;
    [[nodiscard]] constexpr bool empty() const noexcept {
        return length == 0;
    }
    [[nodiscard]] constexpr std::size_t first_end() const noexcept {
        return first + length;
    }
    [[nodiscard]] constexpr std::size_t second_end() const noexcept {
        return second + length;
    }
    friend constexpr bool operator==(const common_substring_match&,
                                     const common_substring_match&) = default;
};
} // namespace canard
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>

namespace canard::kernel {
// Bounded reverse match: a and b point just past the compared ranges.
template <typename Execution>
std::size_t
extend_lcp_reverse(const unsigned char* a, const unsigned char* b, std::size_t available) {
    std::size_t matched = 0;
#if defined(__AVX2__)
    if constexpr (std::same_as<Execution, execution::avx2>) {
        while (available - matched >= 32) {
            const auto x = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a - matched - 32));
            const auto y = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b - matched - 32));
            const auto mask =
                static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpeq_epi8(x, y)));
            if (mask != ~std::uint32_t{})
                return matched + std::countl_one(mask);
            matched += 32;
        }
    }
#endif
    while (available - matched >= 8) {
        std::uint64_t x, y;
        std::memcpy(&x, a - matched - 8, 8);
        std::memcpy(&y, b - matched - 8, 8);
        const auto different = x ^ y;
        if (different) {
            if constexpr (std::endian::native == std::endian::little)
                return matched + std::countl_zero(different) / 8;
            else
                return matched + std::countr_zero(different) / 8;
        }
        matched += 8;
    }
    while (matched < available && a[-static_cast<std::ptrdiff_t>(matched) - 1] ==
                                      b[-static_cast<std::ptrdiff_t>(matched) - 1])
        ++matched;
    return matched;
}

// Exact bounded-work accelerator, NOT a rolling-hash answer. Fixed-length packed
// keys are injective. Hashing chooses a bucket; every probe compares the full
// key, and every occurrence is retained. nullopt requires an exact fallback.
template <typename Execution>
std::optional<common_substring_match> seeded_common_substring(std::string_view first,
                                                              std::string_view second,
                                                              unsigned lo,
                                                              unsigned hi,
                                                              common_substring_match best) {
    const bool swapped = first.size() > second.size();
    if (swapped) {
        std::swap(first, second);
        std::swap(best.first, best.second);
    }
    const auto n = first.size(), m = second.size();
    if (n < 256 || n > (std::size_t{1} << 21) || lo == hi)
        return std::nullopt;
    const auto* a = reinterpret_cast<const unsigned char*>(first.data());
    const auto* b = reinterpret_cast<const unsigned char*>(second.data());
    const unsigned alphabet = hi - lo + 1;
    const unsigned bits = std::bit_width(alphabet - 1);
    unsigned q = 1;
    std::uint64_t domain = alphabet;
    while (domain < 8 * n && (q + 1) * bits <= 32) {
        ++q;
        domain *= alphabet;
    }
    if (q > n)
        return std::nullopt;
    const unsigned key_bits = q * bits;
    const std::uint32_t mask =
        key_bits == 32 ? ~std::uint32_t{} : (std::uint32_t{1} << key_bits) - 1;
    auto key_at = [&](const unsigned char* p) {
        std::uint32_t key = 0;
        for (unsigned j = 0; j < q; ++j)
            key = (key << bits) | (p[j] - lo);
        return key;
    };
    const std::size_t positions = n - q + 1;
    // Sampling decides whether to try, never whether an answer is correct.
    std::array<std::uint32_t, 128> sample{};
    for (std::size_t i = 0; i < sample.size(); ++i)
        sample[i] = key_at(a + i * (positions - 1) / (sample.size() - 1));
    std::sort(sample.begin(), sample.end());
    unsigned duplicates = 0;
    for (std::size_t i = 1; i < sample.size(); ++i)
        duplicates += sample[i] == sample[i - 1];
    if (duplicates > sample.size() / 8)
        return std::nullopt;

    const std::size_t capacity = std::bit_ceil(2 * positions);
    const auto table_mask = capacity - 1;
    const unsigned shift = 32 - std::countr_zero(capacity);
    // [exact 32-bit key | head index plus one]; zero means an unoccupied slot.
    auto table = std::make_unique<std::uint64_t[]>(capacity);
    auto next = std::make_unique_for_overwrite<std::uint32_t[]>(positions);
    std::uint64_t probes = 0;
    const std::uint64_t probe_budget = 8 * (std::uint64_t{n} + m);
    auto bucket = [&](std::uint32_t key) -> std::size_t {
        return (key * std::uint32_t{0x9e3779b9}) >> shift;
    };
    std::uint32_t key = key_at(a);
    for (std::size_t i = 0; i < positions; ++i) {
        std::size_t slot = bucket(key);
        unsigned chain = 0;
        while (table[slot] && static_cast<std::uint32_t>(table[slot] >> 32) != key) {
            if (++probes > probe_budget || ++chain > 64)
                return std::nullopt;
            slot = (slot + 1) & table_mask;
        }
        next[i] = static_cast<std::uint32_t>(table[slot]);
        table[slot] = (std::uint64_t{key} << 32) | (i + 1);
        if (i + 1 < positions)
            key = ((key << bits) | (a[i + q] - lo)) & mask;
    }
    std::uint64_t work = 0;
    const std::uint64_t budget = 4 * (std::uint64_t{n} + m);
    for (std::size_t j = 0; j + q <= m;) {
        key = key_at(b + j);
        std::size_t slot = bucket(key);
        unsigned chain = 0;
        while (table[slot] && static_cast<std::uint32_t>(table[slot] >> 32) != key) {
            if (++probes > probe_budget || ++chain > 64)
                return std::nullopt;
            slot = (slot + 1) & table_mask;
        }
        auto position = static_cast<std::uint32_t>(table[slot]);
        while (position) {
            const std::size_t i = position - 1;
            if (++work > budget)
                return std::nullopt;
            const auto left_limit = std::min(i, j);
            const auto right_limit = std::min(n - i, m - j);
            if (left_limit + right_limit > best.length) {
                const auto right = q + extend_lcp<Execution>(a + i + q, b + j + q, right_limit - q);
                const auto left = extend_lcp_reverse<Execution>(a + i, b + j, left_limit);
                work += right + left;
                if (right + left > best.length)
                    best = {i - left, j - left, right + left};
                if (best.length == n) {
                    if (swapped)
                        std::swap(best.first, best.second);
                    return best;
                }
                if (work > budget)
                    return std::nullopt;
            }
            position = next[i];
        }
        // Every possible improvement still contains a sampled complete factor.
        j += best.length >= q ? best.length - q + 2 : 1;
    }
    if (best.length + 1 < q)
        return std::nullopt;
    if (swapped)
        std::swap(best.first, best.second);
    return best;
}

// Complete alphabet certification repairs a conservative range-width estimate,
// such as the alphabet {'a','z'}. Equality and LCE are preserved by this map.
template <typename Execution>
std::optional<common_substring_match> dense_seeded_common_substring(std::string_view first,
                                                                    std::string_view second,
                                                                    unsigned range,
                                                                    common_substring_match best) {
    if (std::min(first.size(), second.size()) < 256 || range <= 3)
        return std::nullopt;
    std::array<unsigned char, 256> present{}, code{};
    for (unsigned char c : first)
        present[c] |= 1;
    for (unsigned char c : second)
        present[c] |= 2;
    unsigned count = 0;
    bool common = false;
    for (unsigned c = 0; c < 256; ++c) {
        common |= present[c] == 3;
        if (present[c])
            code[c] = static_cast<unsigned char>(count++);
    }
    if (!common)
        return common_substring_match{};
    if (2 * count > range)
        return std::nullopt;
    auto text = std::make_unique_for_overwrite<char[]>(first.size() + second.size());
    for (std::size_t i = 0; i < first.size(); ++i)
        text[i] = static_cast<char>(code[static_cast<unsigned char>(first[i])]);
    for (std::size_t i = 0; i < second.size(); ++i)
        text[first.size() + i] = static_cast<char>(code[static_cast<unsigned char>(second[i])]);
    return seeded_common_substring<Execution>(
        std::string_view{text.get(), first.size()},
        std::string_view{text.get() + first.size(), second.size()},
        0,
        count - 1,
        best);
}
} // namespace canard::kernel
// ===== include/canard/kernel/common_substring_suffixes.hpp =====
// ===== include/canard/kernel/suffix_induction.hpp =====
// ===== include/canard/kernel/suffix_classification.hpp =====
#include <bit>
#include <cstdint>
#include <type_traits>
namespace canard::kernel {
// Positive symbols s[0..n), unique sentinel s[n]=0. n>0.
// Storage: ceil((n+1)/2) LMS slots; a zero-initialized (n+32)/32-word bitmap.
// Returns the beginning of the increasing LMS list ending at out's initial value.
template <typename Execution, typename Symbol>
int* gather_lms(const Symbol* s, int n, int* out, std::uint32_t* bitmap) {
    *--out = n;
    bitmap[n >> 5] |= std::uint32_t{1} << (n & 31);
    bool right = true;
    int end = n;
#if defined(__AVX2__)
    if constexpr (std::same_as<Execution, execution::avx2> && sizeof(Symbol) == 1) {
        const auto sign = _mm256_set1_epi8(static_cast<char>(0x80));
        while (end >= 33) {
            const int b = end - 32;
            const auto a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(s + b));
            const auto z = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(s + b + 1));
            const auto p = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(s + b - 1));
            const auto aa = _mm256_xor_si256(a, sign);
            std::uint32_t eq = _mm256_movemask_epi8(_mm256_cmpeq_epi8(a, z));
            std::uint32_t t =
                _mm256_movemask_epi8(_mm256_cmpgt_epi8(_mm256_xor_si256(z, sign), aa));
            t |= right ? eq & 0x80000000u : 0;
            // Parallel propagation across equal-character runs, with a scalar carry.
            auto e = eq;
            t |= e & (t >> 1);
            e &= e >> 1;
            t |= e & (t >> 2);
            e &= e >> 2;
            t |= e & (t >> 4);
            e &= e >> 4;
            t |= e & (t >> 8);
            e &= e >> 8;
            t |= e & (t >> 16);
            std::uint32_t lms = t & static_cast<std::uint32_t>(_mm256_movemask_epi8(
                                        _mm256_cmpgt_epi8(_mm256_xor_si256(p, sign), aa)));
            const unsigned shift = b & 31;
            bitmap[b >> 5] |= lms << shift;
            if (shift)
                bitmap[(b >> 5) + 1] |= lms >> (32 - shift);
            while (lms) {
                const int bit = 31 - std::countl_zero(lms);
                *--out = b + bit;
                lms ^= std::uint32_t{1} << bit;
            }
            right = (t & 1) != 0;
            end = b;
        }
    }
#endif
    for (int i = end - 1; i > 0; --i) {
        const bool st = s[i] < s[i + 1] || (s[i] == s[i + 1] && right);
        if (st && s[i - 1] > s[i]) {
            *--out = i;
            bitmap[i >> 5] |= std::uint32_t{1} << (i & 31);
        }
        right = st;
    }
    return out;
}
} // namespace canard::kernel
// ===== include/canard/kernel/suffix_certificates.hpp =====
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <memory>
#include <numeric>
#include <type_traits>
namespace canard::kernel {
// All certificates verify the complete input. Sampling only decides whether to try.
template <typename Execution>
unsigned short_period_suffixes(const unsigned char* s, int n, int* sa) {
    if (n < 256)
        return 0;
    std::array<int, 128> failure{};
    for (int i = 1; i < 128; ++i) {
        int j = failure[i - 1];
        while (j && s[i] != s[j])
            j = failure[j - 1];
        if (s[i] == s[j])
            ++j;
        failure[i] = j;
    }
    const int period = 128 - failure[127];
    if (period > 64 || byte_string_operations<Execution>::mismatch(s, s + period, n - period) !=
                           static_cast<std::size_t>(n - period))
        return 0;
    // A minimal prefix period that extends to the whole input is primitive.
    // Long suffixes of one residue share their first period symbols. Short tails
    // are separate entries; dropping those entries is incorrect for words like aba.
    std::array<int, 127> entries{};
    std::iota(entries.begin(), entries.begin() + period, 0);
    for (int j = 0; j < period - 1; ++j)
        entries[period + j] = n - period + 1 + j;
    std::sort(entries.begin(), entries.begin() + 2 * period - 1, [&](int a, int b) {
        const int al = std::min(period, n - a), bl = std::min(period, n - b),
                  len = std::min(al, bl);
        const auto at = byte_string_operations<Execution>::mismatch(s + a, s + b, len);
        return at < static_cast<std::size_t>(len) ? s[a + at] < s[b + at] : al < bl;
    });
    sa[0] = n;
    int* out = sa + 1;
    for (int e = 0; e < 2 * period - 1; ++e) {
        const int r = entries[e];
        if (r >= period) {
            *out++ = r;
            continue;
        }
        const int count = (n - period - r) / period + 1;
        byte_string_operations<Execution>::descending(out, r + (count - 1) * period, period, count);
        out += count;
    }
    return static_cast<unsigned>(period);
}

// On success replaces LMS input order by complete suffix order. On failure LMS
// is unchanged. s has n positive byte symbols and at least 32 zero padding bytes.
template <typename Execution>
bool prefix_order_lms(const unsigned char* s, int n, int alphabet, int* lms, int count) {
    const unsigned bits = std::bit_width(static_cast<unsigned>(alphabet - 2));
    if (!bits)
        return false;
    const unsigned position_bits = std::bit_width(static_cast<unsigned>(n));
    const unsigned key_bits = (64 - position_bits) / bits * bits;
    if (key_bits + position_bits < 20)
        return false;
    const unsigned prefix = key_bits / bits;
    const std::uint64_t position_mask = (std::uint64_t{1} << position_bits) - 1;
    auto key_at = [&](int at) {
        std::uint64_t key = 0;
        for (unsigned j = 0; j < prefix; ++j)
            key = (key << bits) | (j < static_cast<unsigned>(n - at) ? s[at + j] - 1u : 0u);
        return key;
    };
    const int sample_size = std::min(count, 256);
    std::array<std::uint64_t, 256> sample{};
    for (int i = 0; i < sample_size; ++i)
        sample[i] = key_at(lms[std::int64_t(i) * count / sample_size]);
    std::sort(sample.begin(), sample.begin() + sample_size);
    int duplicates = 0;
    for (int i = 1; i < sample_size; ++i)
        duplicates += sample[i] == sample[i - 1];
    if (duplicates > sample_size / 16)
        return false;
    auto words = std::make_unique_for_overwrite<std::uint64_t[]>(count);
    bool packed = false;
#if defined(__AVX2__)
    if constexpr (std::same_as<Execution, execution::avx2>) {
        if (bits == 5 && key_bits == 45) {
            const auto reverse = _mm256_setr_epi8(7,
                                                  6,
                                                  5,
                                                  4,
                                                  3,
                                                  2,
                                                  1,
                                                  0,
                                                  15,
                                                  14,
                                                  13,
                                                  12,
                                                  11,
                                                  10,
                                                  9,
                                                  8,
                                                  7,
                                                  6,
                                                  5,
                                                  4,
                                                  3,
                                                  2,
                                                  1,
                                                  0,
                                                  15,
                                                  14,
                                                  13,
                                                  12,
                                                  11,
                                                  10,
                                                  9,
                                                  8);
            const auto ones = _mm256_set1_epi8(1);
            int i = 0;
            for (; i + 4 <= count; i += 4) {
                const auto pos = _mm_loadu_si128(reinterpret_cast<const __m128i*>(lms + i));
                auto x = _mm256_i32gather_epi64(reinterpret_cast<const long long*>(s), pos, 1);
                x = _mm256_shuffle_epi8(_mm256_subs_epu8(x, ones), reverse);
                x = _mm256_or_si256(
                    _mm256_srli_epi64(_mm256_and_si256(x, _mm256_set1_epi64x(0x1f001f001f001f00ll)),
                                      3),
                    _mm256_and_si256(x, _mm256_set1_epi64x(0x001f001f001f001fll)));
                x = _mm256_or_si256(
                    _mm256_srli_epi64(_mm256_and_si256(x, _mm256_set1_epi64x(0x03ff000003ff0000ll)),
                                      6),
                    _mm256_and_si256(x, _mm256_set1_epi64x(0x000003ff000003ffll)));
                x = _mm256_or_si256(
                    _mm256_srli_epi64(_mm256_and_si256(x, _mm256_set1_epi64x(0x000fffff00000000ll)),
                                      12),
                    _mm256_and_si256(x, _mm256_set1_epi64x(0x00000000000fffffll)));
                auto ninth = _mm_i32gather_epi32(reinterpret_cast<const int*>(s + 8), pos, 1);
                ninth = _mm_subs_epu8(_mm_and_si128(ninth, _mm_set1_epi32(255)), _mm_set1_epi32(1));
                x = _mm256_or_si256(_mm256_slli_epi64(x, 5), _mm256_cvtepu32_epi64(ninth));
                x = _mm256_or_si256(
                    _mm256_sllv_epi64(x, _mm256_set1_epi64x(position_bits)),
                    _mm256_sub_epi64(_mm256_set1_epi64x(n), _mm256_cvtepu32_epi64(pos)));
                _mm256_storeu_si256(reinterpret_cast<__m256i*>(words.get() + i), x);
            }
            for (; i < count; ++i)
                words[i] = (key_at(lms[i]) << position_bits) | (n - lms[i]);
            packed = true;
        }
    }
#endif
    if (!packed) {
        std::uint64_t key = 0;
        int j = count - 1;
        words[j--] = 0; // sentinel
        for (int i = n - 1; i >= lms[0]; --i) {
            key = (key >> bits) | (std::uint64_t(s[i] - 1) << (key_bits - bits));
            if (j >= 0 && i == lms[j])
                words[j--] = (key << position_bits) | (n - i);
        }
    }
    const unsigned shift = key_bits + position_bits - 20;
    std::array<int, 1024> low{}, high{};
    for (int i = 0; i < count; ++i) {
        ++low[(words[i] >> shift) & 1023];
        ++high[(words[i] >> (shift + 10)) & 1023];
    }
    auto exclusive = [](auto& bins) {
        int total = 0;
        for (int& c : bins) {
            const int old = c;
            c = total;
            total += old;
        }
    };
    exclusive(low);
    exclusive(high);
    auto temporary = std::make_unique_for_overwrite<std::uint64_t[]>(count);
    for (int i = 0; i < count; ++i)
        temporary[low[(words[i] >> shift) & 1023]++] = words[i];
    for (int i = 0; i < count; ++i)
        words[high[(temporary[i] >> (shift + 10)) & 1023]++] = temporary[i];
    // A hard group-size limit preserves linear worst-case work even when a
    // misleading sample admits a very nonuniform distribution.
    for (int b = 0; b < count;) {
        int e = b + 1;
        while (e < count && words[e] >> shift == words[b] >> shift)
            ++e;
        if (e - b > 64)
            return false;
        std::sort(words.get() + b, words.get() + e);
        b = e;
    }
    for (int i = 1; i < count; ++i) {
        if (words[i] >> position_bits == words[i - 1] >> position_bits &&
            (words[i - 1] & position_mask) > prefix)
            return false;
    }
    // Equal zero-padded keys are accepted only when the shorter suffix ends
    // within the key. It is then a genuine prefix and is correctly ordered first.
    for (int i = 0; i < count; ++i)
        lms[i] = n - static_cast<int>(words[i] & position_mask);
    return true;
}
} // namespace canard::kernel
// ===== include/canard/kernel/suffix_reduction.hpp =====
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <type_traits>

namespace canard::kernel {

// Returns parity + 1, or zero when the complete certificate fails. Exactly one
// parity must contain a constant symbol strictly smaller than every other symbol.
// The sentinel is not included in n. No padding is needed for this check.
template <typename Execution, typename Symbol>
int alternating_separator(const Symbol* s, int n) {
    if (n < 2 || s[0] == s[1])
        return 0;
    const Symbol separator = std::min(s[0], s[1]);
    const int parity = s[0] == separator ? 0 : 1;
    int i = 0;
#if defined(__AVX2__)
    if constexpr (std::same_as<Execution, execution::avx2> && sizeof(Symbol) == 1) {
        const auto value = _mm256_set1_epi8(static_cast<char>(separator));
        const auto expected = parity == 0 ? 0x55555555u : 0xaaaaaaaau;
        const auto sign = _mm256_set1_epi8(static_cast<char>(0x80));
        const auto signed_value = _mm256_xor_si256(value, sign);
        for (; n - i >= 32; i += 32) {
            const auto x = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(s + i));
            const auto equal = static_cast<unsigned>(
                _mm256_movemask_epi8(_mm256_cmpeq_epi8(x, value)));
            const auto less = _mm256_movemask_epi8(
                _mm256_cmpgt_epi8(signed_value, _mm256_xor_si256(x, sign)));
            if (equal != expected || less != 0)
                return 0;
        }
    }
#endif
    for (; i < n; ++i) {
        if ((i & 1) == parity ? s[i] != separator : s[i] <= separator)
            return 0;
    }
    return parity + 1;
}

// Name COMPLETE LMS factors without an initial induced sort. A proper-prefix
// factor follows its extension: the shorter factor reaches an S-type LMS endpoint
// where the other is L-type. All-ones padding implements that reversed prefix
// order. A non-sentinel LMS endpoint is strictly below a preceding symbol, so it
// cannot equal the maximum padding digit. Distinct factors have distinct keys.
//
// Every factor must fit; the dictionary has at most 128 entries and eight probes
// per insertion. Exceeding a budget rejects the entire attempt. Hash collisions
// compare full keys and never stand in for equality. On rejection only names may
// have been modified; s and lms are unchanged. Byte input requires eight readable
// bytes from every LMS position (all internal byte texts have 32 padding bytes).
template <typename Symbol>
int direct_lms_names(const Symbol* s, int alphabet, const int* lms, int count, int* names) {
    if (alphabet < 2 || alphabet > 256 || count < 128)
        return 0;
    const unsigned bits = sizeof(Symbol) == 1
                              ? 8
                              : std::bit_width(static_cast<unsigned>(alphabet - 1));
    const int maximum = 64 / bits;
    for (int i = 0; i < count - 1; ++i) {
        if (lms[i + 1] - lms[i] + 1 > maximum)
            return 0;
    }

    std::array<std::uint64_t, 512> keys{};
    std::array<unsigned short, 512> ids{};
    std::array<std::uint64_t, 128> unique{};
    int used = 0;
    for (int i = 0; i < count; ++i) {
        const int end = i + 1 < count ? lms[i + 1] : lms[i];
        const unsigned length = static_cast<unsigned>(end - lms[i] + 1);
        const unsigned shift = 64 - length * bits;
        std::uint64_t key = 0;
        if constexpr (sizeof(Symbol) == 1) {
            std::memcpy(&key, s + lms[i], sizeof(key));
            if constexpr (std::endian::native == std::endian::little)
                key = std::byteswap(key);
            if (shift)
                key |= (std::uint64_t{1} << shift) - 1;
        } else {
            for (int j = lms[i]; j <= end; ++j)
                key = (key << bits) | s[j];
            if (shift)
                key = (key << shift) | ((std::uint64_t{1} << shift) - 1);
        }
        unsigned slot = static_cast<unsigned>((key * 0x9e3779b97f4a7c15ull) >> 55);
        unsigned probes = 0;
        while (ids[slot] && keys[slot] != key) {
            if (++probes == 8)
                return 0;
            slot = (slot + 1) & 511;
        }
        if (!ids[slot]) {
            if (used == 128)
                return 0;
            keys[slot] = key;
            unique[used] = key;
            ids[slot] = static_cast<unsigned short>(++used);
        }
        names[i] = ids[slot] - 1;
    }

    std::array<int, 128> permutation{}, ranks{};
    std::iota(permutation.begin(), permutation.begin() + used, 0);
    std::sort(permutation.begin(), permutation.begin() + used, [&](int a, int b) {
        return unique[a] < unique[b];
    });
    for (int i = 0; i < used; ++i)
        ranks[permutation[i]] = i;
    for (int i = 0; i < count; ++i)
        names[i] = ranks[names[i]];
    return used;
}

// Specialized induced sorting for the complete positive alphabet {1, 2}.
// Pure terminal 1-suffixes come first. Symbol-2 suffixes are produced by a FIFO
// induction from LMS predecessors; a reverse FIFO then produces symbol-1
// suffixes. This is the same stable L/S induction without empty-slot scans,
// sign encoding, or a clearing pass. The first unsorted-LMS pass is supported.
template <typename Symbol>
void binary_induce_suffixes(const Symbol* s,
                            int n,
                            const int* ends,
                            const int* lms,
                            int count,
                            int* sa) {
    sa[0] = n;
    int tail = n;
    while (tail > 0 && s[tail - 1] == 1) {
        --tail;
        sa[n - tail] = tail;
    }
    const int a_count = ends[1] - 1;
    int write = a_count + 1;
    if (tail > 0)
        sa[write++] = tail - 1;
    for (int i = 0; i < count; ++i) {
        const int p = lms[i];
        if (p != n)
            sa[write++] = p - 1;
    }
    for (int read = a_count + 1; read < write; ++read) {
        const int p = sa[read];
        if (p > 0 && s[p - 1] == 2)
            sa[write++] = p - 1;
    }
    write = a_count;
    for (int read = n; read > a_count; --read) {
        const int p = sa[read];
        if (p > 0 && s[p - 1] == 1)
            sa[write--] = p - 1;
    }
    for (int read = a_count; read > n - tail; --read) {
        const int p = sa[read];
        if (p > 0 && s[p - 1] == 1)
            sa[write--] = p - 1;
    }
}

} // namespace canard::kernel
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <memory>
#include <numeric>
#include <vector>
namespace canard {
enum class suffix_array_backend {
    empty,
    uniform,
    short_period,
    sparse_lms,
    prefix_induced,
    recursive_induced,
    separator_reduced,
    factor_reduced
};
struct suffix_array_options {
    bool prefix_certificates = true;
    bool short_periods = true;
    bool separator_reduction = true;
    bool factor_reduction = true;
    bool binary_induction = true;
};
struct suffix_array_description {
    std::size_t size = 0, storage_bytes = 0;
    suffix_array_backend backend = suffix_array_backend::empty;
    unsigned period = 0, lms_count = 0;
    unsigned factor_count = 0;
};
namespace kernel {
// Sign-coded predecessor eligibility. No random reads of a type array.
// Positive j+1 induces L; negative -(j+1) induces S. Zero is an empty slot.
template <typename Symbol>
void induce_suffixes(const Symbol* s,
                     int n,
                     int alphabet,
                     const int* ends,
                     const int* lms,
                     int count,
                     int* sa,
                     int* work,
                     bool binary = true) {
    if (binary && alphabet == 3) {
        binary_induce_suffixes(s, n, ends, lms, count, sa);
        return;
    }
    std::fill_n(sa, static_cast<std::size_t>(n) + 1, 0);
    std::copy_n(ends, alphabet, work);
    for (int i = count - 1; i >= 0; --i) {
        const int p = lms[i];
        sa[--work[s[p]]] = p + 1;
    }
    work[0] = 0;
    std::copy_n(ends, alphabet - 1, work + 1);
    for (int i = 0; i <= n; ++i) {
        const int p = sa[i];
        if (p > 1) {
            const int j = p - 2;
            const auto c = s[j];
            int v = j + 1;
            if (j == 0 || s[j - 1] < c)
                v = -v;
            sa[work[c]++] = v;
        }
    }
    std::copy_n(ends, alphabet, work);
    for (int i = n; i >= 0; --i) {
        const int p = sa[i];
        sa[i] = (p < 0 ? -p : p) - 1;
        if (p < -1) {
            const int j = -p - 2;
            const auto c = s[j];
            int v = j + 1;
            if (j > 0 && s[j - 1] <= c)
                v = -v;
            sa[--work[c]] = v;
        }
    }
}
// Positive bounded alphabet, unique zero sentinel. Output capacity n+1.
// At the byte root, 32 real zero-padding bytes enable bounded gather loads.
template <typename Execution, typename Symbol>
void induced_suffix_array(const Symbol* s,
                          int n,
                          int alphabet,
                          int* sa,
                          suffix_array_options options = {},
                          suffix_array_description* description = nullptr) {
    if (n == 0) {
        sa[0] = 0;
        return;
    }
    // A strict-minimum separator in exactly one parity can be deleted.
    // No suffix comparisons are lost: compare the remaining symbols, then
    // restore separator-starting suffixes before all other suffixes.
    if (options.separator_reduction && n >= 256) {
        const int classification = alternating_separator<Execution>(s, n);
        if (classification) {
            const int parity = classification - 1;
            const int offset = 1 - parity;
            const int m = (n + 1 - offset) / 2;
            std::vector<Symbol> reduced(static_cast<std::size_t>(m) + 32);
            for (int j = 0; j < m; ++j)
                reduced[j] = s[2 * j + offset];
            auto order = std::make_unique_for_overwrite<int[]>(m + 1);
            induced_suffix_array<Execution>(reduced.data(), m, alphabet, order.get(), options);
            sa[0] = n;
            int at = 1;
            if (((n - 1) & 1) == parity)
                sa[at++] = n - 1;
            for (int j = 1; j <= m; ++j) {
                const int position = 2 * order[j] + offset;
                if (position > 0)
                    sa[at++] = position - 1;
            }
            for (int j = 1; j <= m; ++j)
                sa[at++] = 2 * order[j] + offset;
            if (description)
                description->backend = suffix_array_backend::separator_reduced;
            return;
        }
    }
    auto positions = std::make_unique_for_overwrite<int[]>(n / 2 + 1);
    int* const last = positions.get() + n / 2 + 1;
    std::vector<std::uint32_t> bitmap((static_cast<std::size_t>(n) + 32) / 32);
    int* const lms = gather_lms<Execution>(s, n, last, bitmap.data());
    const int count = static_cast<int>(last - lms);
    if (description)
        description->lms_count = count;
    std::vector<int> buckets(static_cast<std::size_t>(2) * alphabet);
    int* const ends = buckets.data();
    int* const work = ends + alphabet;
    for (int i = 0; i <= n; ++i)
        ++ends[s[i]];
    std::partial_sum(ends, ends + alphabet, ends);
    if (count <= 64) {
        std::sort(lms, last, [&](int a, int b) { return suffix_less<Execution>(s, n, a, b); });
        induce_suffixes(s, n, alphabet, ends, lms, count, sa, work, options.binary_induction);
        if (description)
            description->backend = suffix_array_backend::sparse_lms;
        return;
    }
    if constexpr (sizeof(Symbol) == 1) {
        if (options.prefix_certificates &&
            prefix_order_lms<Execution>(
                reinterpret_cast<const unsigned char*>(s), n, alphabet, lms, count)) {
            induce_suffixes(s, n, alphabet, ends, lms, count, sa, work, options.binary_induction);
            if (description)
                description->backend = suffix_array_backend::prefix_induced;
            return;
        }
    }
    int* const direct_names = sa + count;
    const int distinct = options.factor_reduction
                             ? direct_lms_names(s, alphabet, lms, count, direct_names)
                             : 0;
    if (distinct) {
        std::vector<unsigned char> reduced(static_cast<std::size_t>(count) + 32);
        for (int i = 0; i < count; ++i)
            reduced[i] = static_cast<unsigned char>(direct_names[i]);
        auto recursive_options = options;
        recursive_options.prefix_certificates = false;
        induced_suffix_array<Execution>(reduced.data(), count - 1, distinct, sa, recursive_options);
        auto ordered = std::make_unique_for_overwrite<int[]>(count);
        for (int i = 0; i < count; ++i)
            ordered[i] = lms[sa[i]];
        induce_suffixes(
            s, n, alphabet, ends, ordered.get(), count, sa, work, options.binary_induction);
        if (description) {
            description->backend = suffix_array_backend::factor_reduced;
            description->factor_count = static_cast<unsigned>(distinct);
        }
        return;
    }
    induce_suffixes(s, n, alphabet, ends, lms, count, sa, work, options.binary_induction);
    int next = 0;
    for (int i = 0; i <= n; ++i) {
        const auto p = static_cast<unsigned>(sa[i]);
        if ((bitmap[p >> 5] >> (p & 31)) & 1u)
            sa[next++] = static_cast<int>(p);
    }
    // LMS positions cannot be adjacent. The tail of SA fits a half-index map.
    int* const names = sa + count;
    for (int i = 0; i < count - 1; ++i)
        names[lms[i] >> 1] = lms[i + 1] - lms[i] + 1;
    names[n >> 1] = 1;
    int rank = -1, previous = -1, previous_length = 0;
    for (int i = 0; i < count; ++i) {
        const int p = sa[i], length = names[p >> 1];
        const bool different =
            previous < 0 || length != previous_length ||
            std::memcmp(s + p, s + previous, static_cast<std::size_t>(length) * sizeof(Symbol)) !=
                0;
        rank += different;
        names[p >> 1] = rank;
        previous = p;
        previous_length = length;
    }
    // Safe forward compaction: floor(lms[i]/2) >= i.
    for (int i = 0; i < count; ++i)
        names[i] = names[lms[i] >> 1];
    if (rank + 1 == count) {
        for (int i = 0; i < count; ++i)
            sa[names[i]] = i;
    } else {
        auto recursive_options = options;
        recursive_options.prefix_certificates = false;
        recursive_options.short_periods = false;
        induced_suffix_array<Execution>(names, count - 1, rank + 1, sa, recursive_options);
    }
    auto ordered = std::make_unique_for_overwrite<int[]>(count);
    for (int i = 0; i < count; ++i)
        ordered[i] = lms[sa[i]];
    induce_suffixes(
        s, n, alphabet, ends, ordered.get(), count, sa, work, options.binary_induction);
    if (description)
        description->backend = suffix_array_backend::recursive_induced;
}
} // namespace kernel
} // namespace canard
#include <algorithm>
#include <memory>
#include <string_view>
#include <vector>

namespace canard::kernel {
// Unique separator 1 and sentinel 0 cannot equal any encoded data symbol.
template <typename Execution, typename Symbol>
common_substring_match suffix_common_substring(std::string_view first,
                                               std::string_view second,
                                               unsigned lo,
                                               unsigned hi,
                                               suffix_array_options options,
                                               common_substring_match best = {}) {
    const int split = static_cast<int>(first.size());
    const int start = split + 1;
    const int n = static_cast<int>(first.size() + second.size() + 1);
    std::vector<Symbol> text(static_cast<std::size_t>(n) + 32);
    for (int i = 0; i < split; ++i)
        text[i] = static_cast<Symbol>(static_cast<unsigned char>(first[i]) - lo + 2);
    text[split] = 1;
    for (std::size_t i = 0; i < second.size(); ++i)
        text[start + i] = static_cast<Symbol>(static_cast<unsigned char>(second[i]) - lo + 2);
    auto order = std::make_unique_for_overwrite<int[]>(static_cast<std::size_t>(n) + 1);
    induced_suffix_array<Execution>(
        text.data(), n, static_cast<int>(hi - lo + 3), order.get(), options);
    // Match original bytes even after a 16-bit sort. Release sorting storage
    // after its final use; no inverse ranks or materialized LCP array are needed.
    std::vector<Symbol>{}.swap(text);
    auto successor = std::make_unique_for_overwrite<int[]>(n);
    for (int rank = 1; rank < n; ++rank)
        successor[order[rank]] = order[rank + 1];
    successor[order[n]] = n;
    order.reset();
    const auto* a = reinterpret_cast<const unsigned char*>(first.data());
    const auto* b = reinterpret_cast<const unsigned char*>(second.data());
    std::size_t matched = 0;
    for (int i = 0; i < n; ++i) {
        const int j = successor[i];
        if (j == n || i == split || j == split) {
            matched = 0;
            continue;
        }
        if ((i < split) != (j < split)) {
            const auto x = static_cast<std::size_t>(i < split ? i : j);
            const auto y = static_cast<std::size_t>((i < split ? j : i) - start);
            const auto limit = std::min(first.size() - x, second.size() - y);
            if (limit > best.length) {
                matched += extend_lcp<Execution>(a + x + matched, b + y + matched, limit - matched);
                if (matched > best.length)
                    best = {x, y, matched};
            }
        }
        // Retain a decreasing LOWER bound across unextended pairs. Resetting it
        // on every same-document pair would lose Kasai's amortized guarantee.
        if (matched)
            --matched;
    }
    return best;
}
} // namespace canard::kernel
// ===== include/canard/kernel/sequence_common_substring.hpp =====
// ===== include/canard/kernel/sequence_lcp.hpp =====
#include <span>
#include <vector>

namespace canard::kernel {
// Only canonical fixed-width unsigned codes enter this matcher. User-object
// representations are never compared. The first unequal byte lies within the
// first unequal symbol, independently of host endianness.
template <typename Execution, typename Symbol>
    requires(std::same_as<Symbol, std::uint8_t> || std::same_as<Symbol, std::uint16_t> ||
             std::same_as<Symbol, std::uint32_t>)
std::size_t extend_encoded_lcp(const Symbol* a, const Symbol* b, std::size_t available) {
    return extend_lcp<Execution>(reinterpret_cast<const unsigned char*>(a),
                                 reinterpret_cast<const unsigned char*>(b),
                                 available * sizeof(Symbol)) /
           sizeof(Symbol);
}

// Next(i) is the lexicographic successor of i, or n. Emit receives the text
// position and its exact LCP. Static callbacks allow either inverse-rank or
// successor-map storage without a runtime mode in the hot loop.
template <typename Next, typename Extend, typename Emit>
void scan_sequence_lcps(std::size_t n, Next&& next, Extend&& extend, Emit&& emit) {
    std::size_t matched = 0;
    for (std::size_t i = 0; i < n; ++i) {
        const auto j = static_cast<std::size_t>(next(i));
        if (j == n) {
            matched = 0;
            continue;
        }
        const auto limit = n - std::max(i, j);
        matched += extend(i + matched, j + matched, limit - matched);
        emit(i, matched);
        if (matched)
            --matched;
    }
}

template <typename Extend>
std::vector<int> sequence_lcp_array(std::span<const int> order, Extend&& extend) {
    const auto n = order.size();
    if (n < 2)
        return {};
    std::vector<int> rank(n), result(n - 1);
    for (std::size_t i = 0; i < n; ++i)
        rank[order[i]] = static_cast<int>(i);
    scan_sequence_lcps(
        n,
        [&](std::size_t i) {
            const auto r = static_cast<std::size_t>(rank[i]) + 1;
            return r == n ? n : static_cast<std::size_t>(order[r]);
        },
        std::forward<Extend>(extend),
        [&](std::size_t i, std::size_t matched) { result[rank[i]] = static_cast<int>(matched); });
    return result;
}

template <typename Extend>
std::uint64_t sequence_lcp_sum(std::span<const int> order, Extend&& extend) {
    const auto n = order.size();
    if (n < 2)
        return 0;
    auto successor = std::make_unique_for_overwrite<int[]>(n);
    for (std::size_t r = 0; r + 1 < n; ++r)
        successor[order[r]] = order[r + 1];
    successor[order.back()] = static_cast<int>(n);
    std::uint64_t sum = 0;
    scan_sequence_lcps(
        n,
        [&](std::size_t i) { return successor[i]; },
        std::forward<Extend>(extend),
        [&](std::size_t, std::size_t matched) { sum += matched; });
    return sum;
}
} // namespace canard::kernel
#include <functional>

namespace canard::kernel {
// Equality alone supplies no ordered/hashable alphabet. A direct dynamic
// program avoids quadratic joint-alphabet discovery when one range is short.
// O(n*m) equality calls, one row of min(n,m)+1 integers; positions stay in the
// caller's two original ranges. The equality relation must be an equivalence.
template <typename First, typename Second, typename Equal>
common_substring_match equivalence_common_substring(const First& first,
                                                    const Second& second,
                                                    Equal& equal,
                                                    bool certificates = true) {
    if (!first.size() || !second.size())
        return {};
    if (certificates) {
        const auto bound = std::min(first.size(), second.size());
        std::size_t matched = 0;
        while (matched < bound && std::invoke(equal, first[matched], second[matched]))
            ++matched;
        if (matched == bound)
            return {0, 0, bound};
    }
    const auto solve = [&](const auto& outer, const auto& inner, bool swapped) {
        std::vector<int> row(inner.size() + 1);
        common_substring_match best{};
        for (std::size_t i = 0; i < outer.size(); ++i) {
            for (std::size_t j = inner.size(); j; --j) {
                row[j] = std::invoke(equal, outer[i], inner[j - 1]) ? row[j - 1] + 1 : 0;
                const auto length = static_cast<std::size_t>(row[j]);
                if (length > best.length)
                    best = {i + 1 - length, j - length, length};
            }
        }
        if (swapped)
            std::swap(best.first, best.second);
        return best;
    };
    return first.size() >= second.size() ? solve(first, second, false) : solve(second, first, true);
}

// Wide-alphabet companion to the specialized byte seed/suffix implementation.
// Data codes are jointly assigned in [0,alphabet); separator 1 and sentinel 0
// are outside both input alphabets after the +2 translation.
template <typename Execution, typename SortSymbol, typename Symbol>
common_substring_match sequence_common_substring(std::span<const Symbol> first,
                                                 std::span<const Symbol> second,
                                                 std::uint32_t alphabet,
                                                 suffix_array_options options,
                                                 common_substring_match best = {}) {
    const int split = static_cast<int>(first.size());
    const int start = split + 1;
    const int n = static_cast<int>(first.size() + second.size() + 1);
    std::vector<SortSymbol> text(static_cast<std::size_t>(n) + 32);
    for (int i = 0; i < split; ++i)
        text[i] = static_cast<SortSymbol>(first[i] + 2u);
    text[split] = 1;
    for (std::size_t i = 0; i < second.size(); ++i)
        text[start + i] = static_cast<SortSymbol>(second[i] + 2u);
    auto order = std::make_unique_for_overwrite<int[]>(static_cast<std::size_t>(n) + 1);
    induced_suffix_array<Execution>(
        text.data(), n, static_cast<int>(alphabet) + 2, order.get(), options);
    std::vector<SortSymbol>{}.swap(text);
    auto successor = std::make_unique_for_overwrite<int[]>(n);
    for (int rank = 1; rank < n; ++rank)
        successor[order[rank]] = order[rank + 1];
    successor[order[n]] = n;
    order.reset();
    std::size_t matched = 0;
    for (int i = 0; i < n; ++i) {
        const int j = successor[i];
        if (j == n || i == split || j == split) {
            matched = 0;
            continue;
        }
        if ((i < split) != (j < split)) {
            const auto x = static_cast<std::size_t>(i < split ? i : j);
            const auto y = static_cast<std::size_t>((i < split ? j : i) - start);
            const auto limit = std::min(first.size() - x, second.size() - y);
            if (limit > best.length) {
                matched += extend_encoded_lcp<Execution>(
                    first.data() + x + matched, second.data() + y + matched, limit - matched);
                if (matched > best.length)
                    best = {x, y, matched};
            }
        }
        // Unextended pairs retain a decreasing LOWER bound. Resetting at every
        // same-document pair would discard the linear-time carry invariant.
        if (matched)
            --matched;
    }
    return best;
}
} // namespace canard::kernel
// ===== include/canard/string/encoded_sequence.hpp =====
// ===== include/canard/string/sequence_acquisition.hpp =====
// ===== include/canard/string/alphabet.hpp =====
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <utility>

namespace canard {

template <typename T>
concept sequence_execution = std::same_as<T, execution::scalar> || std::same_as<T, execution::avx2>;

namespace alphabet {
// Plain char is an unsigned byte by default, consistently with string_view.
// Supply an explicit comparator to request different character semantics.
struct symbol_less {
    template <typename T, typename U>
        requires requires(const T& a, const U& b) { std::less<>{}(a, b); }
    constexpr bool operator()(const T& a, const U& b) const {
        if constexpr (std::same_as<T, char> && std::same_as<U, char>)
            return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
        else
            return std::less<>{}(a, b);
    }
};

template <typename Compare = symbol_less, typename Projection = std::identity> struct ordered {
    [[no_unique_address]] Compare compare{};
    [[no_unique_address]] Projection project{};
};
template <typename C> ordered(C) -> ordered<C>;
template <typename C, typename P> ordered(C, P) -> ordered<C, P>;

// Values, after projection, must be integers in [0, size). No discovery/sort.
template <typename Projection = std::identity> struct dense {
    std::uint32_t size;
    [[no_unique_address]] Projection project{};
};
dense(std::uint32_t) -> dense<>;
template <typename P> dense(std::uint32_t, P) -> dense<P>;

struct default_hash {
    template <typename T>
        requires requires(const T& x) { std::hash<T>{}(x); }
    std::size_t operator()(const T& x) const {
        return std::hash<T>{}(x);
    }
};
// Equal keys MUST have equal hashes. Collisions are resolved by equal, never
// treated as evidence of equality. Does not specify an external alphabet order.
template <typename Hash = default_hash,
          typename Equal = std::equal_to<>,
          typename Projection = std::identity>
struct hashed {
    [[no_unique_address]] Hash hash{};
    [[no_unique_address]] Equal equal{};
    [[no_unique_address]] Projection project{};
};
template <typename H> hashed(H) -> hashed<H>;
template <typename H, typename Q> hashed(H, Q) -> hashed<H, Q>;
template <typename H, typename Q, typename P> hashed(H, Q, P) -> hashed<H, Q, P>;

// Equality-only, deterministic quadratic worst case. Use hashed when possible.
template <typename Equal = std::equal_to<>, typename Projection = std::identity> struct equivalent {
    [[no_unique_address]] Equal equal{};
    [[no_unique_address]] Projection project{};
};
template <typename Q> equivalent(Q) -> equivalent<Q>;
template <typename Q, typename P> equivalent(Q, P) -> equivalent<Q, P>;
} // namespace alphabet

namespace detail {
template <typename T> inline constexpr int alphabet_kind = -1;
template <typename C, typename P> inline constexpr int alphabet_kind<alphabet::ordered<C, P>> = 0;
template <typename P> inline constexpr int alphabet_kind<alphabet::dense<P>> = 1;
template <typename H, typename Q, typename P>
inline constexpr int alphabet_kind<alphabet::hashed<H, Q, P>> = 2;
template <typename Q, typename P>
inline constexpr int alphabet_kind<alphabet::equivalent<Q, P>> = 3;

template <std::size_t N> std::string_view terminated_array_view(const char (&text)[N]) {
    std::size_t n = 0;
    while (n < N && text[n] != '\0')
        ++n;
    return {text, n};
}

template <typename R>
inline constexpr bool character_array =
    std::is_array_v<std::remove_reference_t<R>> &&
    std::same_as<std::remove_cv_t<std::remove_extent_t<std::remove_reference_t<R>>>, char>;
template <typename T>
inline constexpr bool byte_symbol = std::same_as<T, char> || std::same_as<T, unsigned char> ||
                                    std::same_as<T, char8_t> || std::same_as<T, std::byte>;
template <typename R>
concept contiguous_byte_range =
    std::ranges::contiguous_range<R> && std::ranges::sized_range<R> &&
    byte_symbol<std::ranges::range_value_t<R>> &&
    (!std::is_volatile_v<std::remove_reference_t<std::ranges::range_reference_t<R>>>);

template <contiguous_byte_range R> std::string_view byte_view(R&& range) {
    const auto n = static_cast<std::size_t>(std::ranges::size(range));
    if (!n)
        return {};
    return {reinterpret_cast<const char*>(std::ranges::data(range)), n};
}

template <typename R, typename P, bool = std::same_as<P, std::identity>> struct sequence_key {
    using type = std::remove_cvref_t<std::invoke_result_t<P&, std::ranges::range_reference_t<R>>>;
};
template <typename R, typename P> struct sequence_key<R, P, true> {
    // In particular, vector<bool>'s proxy becomes bool, not a retained proxy.
    using type = std::ranges::range_value_t<R>;
};
template <typename R, typename P> using sequence_key_t = typename sequence_key<R, P>::type;

template <typename R, typename P>
concept projectable_sequence =
    std::ranges::input_range<R> && std::regular_invocable<P&, std::ranges::range_reference_t<R>>;

template <typename A, typename K>
concept key_order = requires(A& a, const K& x, const K& y) {
    { std::invoke(a.compare, x, y) } -> std::convertible_to<bool>;
};
template <typename A, typename K>
concept key_equivalence = requires(A& a, const K& x, const K& y) {
    { std::invoke(a.equal, x, y) } -> std::convertible_to<bool>;
};
template <typename A, typename K>
concept key_hash = key_equivalence<A, K> && requires(A& a, const K& x) {
    { std::invoke(a.hash, x) } -> std::convertible_to<std::size_t>;
};

template <typename A, typename K>
concept alphabet_for_key =
    (alphabet_kind<A> == 0 && key_order<A, K>) ||
    (alphabet_kind<A> == 1 && (std::integral<K> || std::same_as<K, std::byte>)) ||
    (alphabet_kind<A> == 2 && key_hash<A, K>) || (alphabet_kind<A> == 3 && key_equivalence<A, K>);
} // namespace detail

template <typename A>
concept sequence_alphabet = detail::alphabet_kind<A> >= 0;
template <typename A>
concept ordered_sequence_alphabet = detail::alphabet_kind<A> == 0 || detail::alphabet_kind<A> == 1;
template <typename R, typename A>
concept sequence_alphabet_for =
    sequence_alphabet<A> && detail::projectable_sequence<R, decltype(std::declval<A>().project)> &&
    detail::alphabet_for_key<A, detail::sequence_key_t<R, decltype(std::declval<A>().project)>>;

} // namespace canard
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

namespace canard::detail {
inline constexpr std::size_t sequence_size_limit =
    static_cast<std::size_t>(std::numeric_limits<int>::max() - 1);

inline void check_sequence_size(std::size_t n) {
    if (n > sequence_size_limit)
        throw std::length_error("canard sequence: input too long");
}

// Borrow projected objects only through stable forward references and simple
// identity/member projections. Arbitrary projected values are snapshotted once.
// The returned references live only within the synchronous algorithm call.
template <typename R, typename P>
inline constexpr bool borrow_sequence_keys =
    std::ranges::forward_range<R> &&
    (std::same_as<P, std::identity> || std::is_member_object_pointer_v<P>) &&
    std::is_lvalue_reference_v<std::invoke_result_t<P&, std::ranges::range_reference_t<R>>> &&
    (!std::is_volatile_v<
        std::remove_reference_t<std::invoke_result_t<P&, std::ranges::range_reference_t<R>>>>) &&
    (!std::integral<sequence_key_t<R, P>>) && (!std::same_as<sequence_key_t<R, P>, std::byte>);

template <typename K, bool Borrow> class acquired_sequence_keys {
    using stored_type = std::conditional_t<Borrow, const K*, K>;
    std::vector<stored_type> values_;

  public:
    using key_type = K;
    template <std::ranges::input_range R, typename P>
    void append(R&& range, P& project, std::size_t maximum = sequence_size_limit) {
        if constexpr (std::ranges::sized_range<R>) {
            const auto count = std::ranges::size(range);
            if (count > maximum - values_.size())
                throw std::length_error("canard sequence: input too long");
            values_.reserve(values_.size() + static_cast<std::size_t>(count));
        }
        for (auto&& item : range) {
            if (values_.size() == maximum)
                throw std::length_error("canard sequence: input too long");
            if constexpr (Borrow)
                values_.push_back(std::addressof(std::invoke(project, item)));
            else
                values_.emplace_back(std::invoke(project, std::forward<decltype(item)>(item)));
        }
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return values_.size();
    }
    [[nodiscard]] decltype(auto) operator[](std::size_t i) const {
        if constexpr (Borrow)
            return *values_[i];
        else
            return values_[i];
    }
};

// Stable random-access input needs neither a payload copy nor a reference
// vector. Restrict this path to identity/member projections: arbitrary computed
// keys are evaluated once by the materializing acquisition above.
template <typename R, typename P>
inline constexpr bool indexed_sequence_keys =
    std::ranges::random_access_range<R> && std::ranges::sized_range<R> &&
    std::is_lvalue_reference_v<std::ranges::range_reference_t<R>> &&
    (!std::is_volatile_v<
        std::remove_reference_t<std::invoke_result_t<P&, std::ranges::range_reference_t<R>>>>) &&
    std::is_lvalue_reference_v<std::invoke_result_t<P&, std::ranges::range_reference_t<R>>> &&
    (std::same_as<P, std::identity> || std::is_member_object_pointer_v<P>);

template <typename R, typename P> class indexed_sequence_key_view {
    std::ranges::iterator_t<R> first_{};
    P* project_ = nullptr;
    std::size_t size_ = 0;

  public:
    using key_type = sequence_key_t<R, P>;
    void append(R&& range, P& project, std::size_t maximum = sequence_size_limit) {
        const auto count = std::ranges::size(range);
        if (count > maximum)
            throw std::length_error("canard sequence: input too long");
        first_ = std::ranges::begin(range);
        project_ = std::addressof(project);
        size_ = static_cast<std::size_t>(count);
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }
    [[nodiscard]] const key_type& operator[](std::size_t i) const {
        return std::invoke(*project_, first_[static_cast<std::ranges::range_difference_t<R>>(i)]);
    }
};

template <typename R, typename P>
using acquired_keys_t =
    std::conditional_t<indexed_sequence_keys<R, P>,
                       indexed_sequence_key_view<R, P>,
                       acquired_sequence_keys<sequence_key_t<R, P>, borrow_sequence_keys<R, P>>>;

template <typename R, typename A>
concept retainable_sequence_for =
    sequence_alphabet_for<R, A> &&
    (indexed_sequence_keys<R, decltype(std::declval<A>().project)> ||
     borrow_sequence_keys<R, decltype(std::declval<A>().project)> ||
     (std::move_constructible<sequence_key_t<R, decltype(std::declval<A>().project)>> &&
      std::constructible_from<sequence_key_t<R, decltype(std::declval<A>().project)>,
                              std::invoke_result_t<decltype(std::declval<A>().project)&,
                                                   std::ranges::range_reference_t<R>>>));

template <typename R, typename A> inline constexpr bool native_sequence_equality = false;
template <std::ranges::contiguous_range R, typename C>
    requires std::ranges::sized_range<R> && std::integral<std::ranges::range_value_t<R>> &&
                 std::has_unique_object_representations_v<std::ranges::range_value_t<R>> &&
                 (!std::is_volatile_v<std::remove_reference_t<std::ranges::range_reference_t<R>>>)
inline constexpr bool native_sequence_equality<R, alphabet::ordered<C, std::identity>> =
    std::same_as<C, alphabet::symbol_less> || std::same_as<C, std::less<>> ||
    std::same_as<C, std::less<std::ranges::range_value_t<R>>> || std::same_as<C, std::ranges::less>;

template <typename Keys, typename A>
void validate_dense_sequence(const Keys& keys, const A& policy) {
    if constexpr (alphabet_kind<A> == 1) {
        if (policy.size > sequence_size_limit || (!policy.size && keys.size()))
            throw std::invalid_argument("canard sequence: invalid alphabet size");
        for (std::size_t i = 0; i < keys.size(); ++i) {
            if constexpr (std::is_signed_v<typename Keys::key_type>)
                if (keys[i] < 0)
                    throw std::out_of_range("canard sequence: negative alphabet code");
            if constexpr (sizeof(typename Keys::key_type) > sizeof(std::uint64_t)) {
                if (keys[i] >= static_cast<typename Keys::key_type>(policy.size))
                    throw std::out_of_range("canard sequence: symbol outside alphabet");
            }
            if (static_cast<std::uint64_t>(keys[i]) >= policy.size)
                throw std::out_of_range("canard sequence: symbol outside alphabet");
        }
    }
}

template <typename R, typename S> void check_joint_sequence_sizes(R& first, S& second) {
    constexpr auto maximum = sequence_size_limit - 1;
    if constexpr (std::ranges::sized_range<R>) {
        const auto n = std::ranges::size(first);
        if (n > maximum)
            throw std::length_error("canard LCS: combined input too long");
        if constexpr (std::ranges::sized_range<S>) {
            if (std::ranges::size(second) > maximum - static_cast<std::size_t>(n))
                throw std::length_error("canard LCS: combined input too long");
        }
    } else if constexpr (std::ranges::sized_range<S>) {
        if (std::ranges::size(second) > maximum)
            throw std::length_error("canard LCS: combined input too long");
    }
}

// Combining different projected key types implicitly (e.g. signed/unsigned)
// could change symbol order. A projection must explicitly provide a common type.
template <typename R, typename S, typename A>
concept joint_sequence_alphabet_for =
    retainable_sequence_for<R, A> && retainable_sequence_for<S, A> &&
    std::same_as<sequence_key_t<R, decltype(std::declval<A>().project)>,
                 sequence_key_t<S, decltype(std::declval<A>().project)>>;

// Each side retains its own optimal acquisition: e.g. borrow move-only
// records on one side while owning single-pass values on the other. The
// conditional expression preserves references, or returns values for proxies.
template <typename First, typename Second> class joined_sequence_key_view {
    const First& first_;
    const Second& second_;

  public:
    using key_type = typename First::key_type;
    joined_sequence_key_view(const First& first, const Second& second)
        : first_(first), second_(second) {}
    [[nodiscard]] std::size_t size() const noexcept {
        return first_.size() + second_.size();
    }
    [[nodiscard]] decltype(auto) operator[](std::size_t index) const {
        return index < first_.size() ? first_[index] : second_[index - first_.size()];
    }
};

template <typename A, typename X, typename Y> bool equivalent_keys(A& a, const X& x, const Y& y) {
    if constexpr (alphabet_kind<A> == 0)
        return !std::invoke(a.compare, x, y) && !std::invoke(a.compare, y, x);
    else if constexpr (alphabet_kind<A> == 1)
        return x == y;
    else
        return std::invoke(a.equal, x, y);
}
} // namespace canard::detail
#include <algorithm>
#include <array>
#include <bit>
#include <numeric>
#include <span>
#include <unordered_map>
#include <variant>

namespace canard {
enum class sequence_encoding_method { bounded, integer_radix, comparison, hashed, equality };
struct sequence_encoding_description {
    std::size_t size = 0;
    std::uint32_t alphabet_size = 0;
    unsigned symbol_bytes = 1;
    sequence_encoding_method method = sequence_encoding_method::bounded;
};
namespace detail {
struct encoded_sequence_storage {
    std::variant<std::vector<std::uint8_t>, std::vector<std::uint16_t>, std::vector<std::uint32_t>>
        symbols;
    std::uint32_t alphabet = 0;
    sequence_encoding_method method = sequence_encoding_method::bounded;
};
template <typename ValueAt>
encoded_sequence_storage pack_sequence_values(std::size_t n,
                                              std::uint32_t alphabet,
                                              sequence_encoding_method method,
                                              ValueAt&& value_at) {
    check_sequence_size(n);
    encoded_sequence_storage out;
    out.alphabet = alphabet;
    out.method = method;
    const auto fill = [&]<typename Code>() {
        std::vector<Code> codes(n);
        for (std::size_t i = 0; i < n; ++i)
            codes[i] = static_cast<Code>(value_at(i));
        out.symbols = std::move(codes);
    };
    if (alphabet <= 256)
        fill.template operator()<std::uint8_t>();
    else if (alphabet <= 65536)
        fill.template operator()<std::uint16_t>();
    else
        fill.template operator()<std::uint32_t>();
    return out;
}
// Defer this helper until a generic encoding actually needs it. Eager vector/
// variant instantiation in a byte-only TU perturbs unrelated emitted functions.
template <typename = void>
encoded_sequence_storage pack_sequence_codes(std::vector<std::uint32_t> labels,
                                             std::uint32_t alphabet,
                                             sequence_encoding_method method) {
    encoded_sequence_storage out;
    out.alphabet = alphabet;
    out.method = method;
    if (alphabet <= 256) {
        std::vector<std::uint8_t> bytes(labels.size());
        std::ranges::copy(labels, bytes.begin());
        out.symbols = std::move(bytes);
    } else if (alphabet <= 65536) {
        std::vector<std::uint16_t> words(labels.size());
        std::ranges::copy(labels, words.begin());
        out.symbols = std::move(words);
    } else {
        out.symbols = std::move(labels);
    }
    return out;
}

template <typename K> struct unsigned_sequence_key {
    using type = std::make_unsigned_t<K>;
};
template <> struct unsigned_sequence_key<bool> {
    using type = std::uint8_t;
};
template <> struct unsigned_sequence_key<std::byte> {
    using type = std::uint8_t;
};
template <typename K> using unsigned_sequence_key_t = typename unsigned_sequence_key<K>::type;

template <typename K, typename C>
inline constexpr bool radix_key_order =
    (std::integral<K> || std::same_as<K, std::byte>) && sizeof(K) <= sizeof(std::uint64_t) &&
    (std::same_as<C, alphabet::symbol_less> || std::same_as<C, std::less<>> ||
     std::same_as<C, std::less<K>> || std::same_as<C, std::ranges::less>);

template <typename K, typename C> unsigned_sequence_key_t<K> ordered_integer_key(K key) {
    using U = unsigned_sequence_key_t<K>;
    U word = static_cast<U>(key);
    if constexpr (std::is_signed_v<K> &&
                  !(std::same_as<K, char> && std::same_as<C, alphabet::symbol_less>))
        word ^= U{1} << (std::numeric_limits<U>::digits - 1);
    return word;
}

template <typename Keys, typename C>
encoded_sequence_storage radix_encode_keys(const Keys& keys, C) {
    using K = typename Keys::key_type;
    using U = unsigned_sequence_key_t<K>;
    const auto n = keys.size();
    check_sequence_size(n);
    if (!n)
        return {};
    std::vector<U> words(n);
    U lo = std::numeric_limits<U>::max(), hi = 0;
    for (std::size_t i = 0; i < n; ++i) {
        const U word = ordered_integer_key<K, C>(static_cast<K>(keys[i]));
        words[i] = word;
        lo = std::min(lo, word);
        hi = std::max(hi, word);
    }
    const auto distance = static_cast<std::uint64_t>(static_cast<U>(hi - lo));
    // Bounded counting is both cheaper and exact for a reasonably dense span.
    // The span budget makes this O(n), even for sparse 64-bit input domains.
    if (distance <= 65535 && distance <= 4 * n + 256) {
        std::vector<std::uint32_t> ranks(static_cast<std::size_t>(distance) + 1);
        for (U word : words)
            ranks[static_cast<std::size_t>(word - lo)] = 1;
        std::uint32_t alphabet = 0;
        for (auto& rank : ranks) {
            const auto present = rank;
            rank = alphabet;
            alphabet += present;
        }
        return pack_sequence_values(
            n, alphabet, sequence_encoding_method::bounded, [&](std::size_t i) {
                return ranks[static_cast<std::size_t>(words[i] - lo)];
            });
    }
    std::vector<std::uint32_t> labels(n), order(n), work(n);
    std::iota(order.begin(), order.end(), 0u);
    // All high bits above lo^hi agree throughout the interval [lo,hi].
    const unsigned passes = (std::bit_width(static_cast<U>(lo ^ hi)) + 7) / 8;
    for (unsigned pass = 0; pass < passes; ++pass) {
        std::array<std::size_t, 256> positions{};
        const unsigned shift = pass * 8;
        for (auto index : order)
            ++positions[(words[index] >> shift) & 255u];
        std::size_t offset = 0;
        for (auto& position : positions) {
            const auto count = position;
            position = offset;
            offset += count;
        }
        for (auto index : order)
            work[positions[(words[index] >> shift) & 255u]++] = index;
        order.swap(work);
    }
    std::uint32_t rank = 0;
    labels[order.front()] = 0;
    for (std::size_t i = 1; i < n; ++i) {
        rank += words[order[i - 1]] != words[order[i]];
        labels[order[i]] = rank;
    }
    return pack_sequence_codes(
        std::move(labels), rank + 1, sequence_encoding_method::integer_radix);
}

template <typename Keys, sequence_alphabet A>
encoded_sequence_storage encode_sequence_keys(const Keys& keys, A& policy) {
    using K = typename Keys::key_type;
    const auto n = keys.size();
    check_sequence_size(n);
    if constexpr (alphabet_kind<A> == 0) {
        using Compare = decltype(policy.compare);
        if constexpr (radix_key_order<K, Compare>) {
            return radix_encode_keys(keys, policy.compare);
        } else {
            std::vector<std::uint32_t> order(n), labels(n);
            std::iota(order.begin(), order.end(), 0u);
            std::sort(order.begin(), order.end(), [&](auto a, auto b) {
                return std::invoke(policy.compare, keys[a], keys[b]);
            });
            std::uint32_t alphabet = 0;
            for (std::size_t i = 0; i < n; ++i) {
                if (!i || std::invoke(policy.compare, keys[order[i - 1]], keys[order[i]]))
                    ++alphabet;
                labels[order[i]] = alphabet - 1;
            }
            return pack_sequence_codes(
                std::move(labels), alphabet, sequence_encoding_method::comparison);
        }
    } else if constexpr (alphabet_kind<A> == 1) {
        if (policy.size > sequence_size_limit || (!policy.size && n))
            throw std::invalid_argument("canard sequence: invalid alphabet size");
        // The width is known before reading any symbol; do not allocate a
        // transient uint32_t array merely to narrow it immediately afterwards.
        return pack_sequence_values(
            n, policy.size, sequence_encoding_method::bounded, [&](std::size_t i) {
                if constexpr (std::is_signed_v<K>) {
                    if (keys[i] < 0)
                        throw std::out_of_range("canard sequence: negative alphabet code");
                }
                if constexpr (sizeof(K) > sizeof(std::uint64_t)) {
                    if (keys[i] >= static_cast<K>(policy.size))
                        throw std::out_of_range("canard sequence: symbol outside alphabet");
                }
                const auto value = static_cast<std::uint64_t>(keys[i]);
                if (value >= policy.size)
                    throw std::out_of_range("canard sequence: symbol outside alphabet");
                return value;
            });
    } else if constexpr (alphabet_kind<A> == 2) {
        // Keys remain in the acquisition buffer. The dictionary owns integer
        // positions, not copies of possibly heavy or noncopyable user objects.
        auto hash = [&](std::uint32_t i) -> std::size_t {
            return std::invoke(policy.hash, keys[i]);
        };
        auto equal = [&](std::uint32_t a, std::uint32_t b) {
            return std::invoke(policy.equal, keys[a], keys[b]);
        };
        std::unordered_map<std::uint32_t, std::uint32_t, decltype(hash), decltype(equal)>
            dictionary(0, hash, equal);
        dictionary.reserve(n);
        std::vector<std::uint32_t> labels(n);
        for (std::size_t i = 0; i < n; ++i) {
            auto [where, inserted] = dictionary.try_emplace(
                static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(dictionary.size()));
            (void)inserted;
            labels[i] = where->second;
        }
        return pack_sequence_codes(std::move(labels),
                                   static_cast<std::uint32_t>(dictionary.size()),
                                   sequence_encoding_method::hashed);
    } else {
        std::vector<std::uint32_t> representatives, labels(n);
        for (std::size_t i = 0; i < n; ++i) {
            const auto found = std::ranges::find_if(representatives, [&](auto j) {
                return std::invoke(policy.equal, keys[i], keys[j]);
            });
            const auto rank = static_cast<std::uint32_t>(found - representatives.begin());
            if (found == representatives.end())
                representatives.push_back(static_cast<std::uint32_t>(i));
            labels[i] = rank;
        }
        return pack_sequence_codes(std::move(labels),
                                   static_cast<std::uint32_t>(representatives.size()),
                                   sequence_encoding_method::equality);
    }
}
} // namespace detail

template <bool Ordered = true> class joint_encoded_sequences;

// Owns immutable compact class IDs, not the original elements or policy. The
// boolean distinguishes meaningful ordered ranks from arbitrary equality IDs.
template <bool Ordered = true> class encoded_sequence {
    detail::encoded_sequence_storage storage_;
    template <bool> friend class joint_encoded_sequences;

  public:
    static constexpr bool preserves_order = Ordered;
    encoded_sequence() = default;
    template <std::ranges::input_range R, sequence_alphabet A = alphabet::ordered<>>
        requires detail::retainable_sequence_for<R, A> && (Ordered == ordered_sequence_alphabet<A>)
    explicit encoded_sequence(R&& range, A policy = {}) {
        detail::acquired_keys_t<R, decltype(policy.project)> keys;
        keys.append(std::forward<R>(range), policy.project);
        storage_ = detail::encode_sequence_keys(keys, policy);
    }
    encoded_sequence(const encoded_sequence&) = default;
    encoded_sequence(encoded_sequence&& other) noexcept : storage_(std::move(other.storage_)) {
        other.storage_ = {};
    }
    encoded_sequence& operator=(const encoded_sequence& other) {
        if (this != &other) {
            encoded_sequence copy(other);
            swap(copy);
        }
        return *this;
    }
    encoded_sequence& operator=(encoded_sequence&& other) noexcept {
        if (this != &other) {
            encoded_sequence moved(std::move(other));
            swap(moved);
        }
        return *this;
    }
    void swap(encoded_sequence& other) noexcept {
        std::swap(storage_, other.storage_);
    }
    friend void swap(encoded_sequence& a, encoded_sequence& b) noexcept {
        a.swap(b);
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return std::visit([](const auto& codes) { return codes.size(); }, storage_.symbols);
    }
    [[nodiscard]] bool empty() const noexcept {
        return size() == 0;
    }
    [[nodiscard]] std::uint32_t alphabet_size() const noexcept {
        return storage_.alphabet;
    }
    [[nodiscard]] unsigned symbol_bytes() const noexcept {
        return std::visit(
            [](const auto& codes) {
                return static_cast<unsigned>(
                    sizeof(typename std::remove_cvref_t<decltype(codes)>::value_type));
            },
            storage_.symbols);
    }
    [[nodiscard]] sequence_encoding_description describe() const noexcept {
        return {size(), alphabet_size(), symbol_bytes(), storage_.method};
    }
    [[nodiscard]] std::uint32_t operator[](std::size_t index) const noexcept {
        return std::visit([&](const auto& codes) -> std::uint32_t { return codes[index]; },
                          storage_.symbols);
    }
    [[nodiscard]] std::uint32_t at(std::size_t index) const {
        if (index >= size())
            throw std::out_of_range("canard encoded_sequence: index");
        return (*this)[index];
    }
    template <typename F> decltype(auto) visit(F&& function) const& {
        return std::visit(
            [&](const auto& codes) -> decltype(auto) {
                using T = typename std::remove_cvref_t<decltype(codes)>::value_type;
                return std::invoke(std::forward<F>(function), std::span<const T>{codes});
            },
            storage_.symbols);
    }
    template <typename F> void visit(F&&) const&& = delete;
};
template <std::ranges::input_range R, sequence_alphabet A = alphabet::ordered<>>
    requires detail::retainable_sequence_for<R, A>
encoded_sequence(R&&, A = {}) -> encoded_sequence<ordered_sequence_alphabet<A>>;

// One encoding over BOTH ranges; independent encodings are not interchangeable.
template <bool Ordered> class joint_encoded_sequences {
    encoded_sequence<Ordered> symbols_;
    std::size_t split_ = 0;

  public:
    static constexpr bool preserves_order = Ordered;
    joint_encoded_sequences() = default;
    template <std::ranges::input_range R,
              std::ranges::input_range S,
              sequence_alphabet A = alphabet::ordered<>>
        requires detail::joint_sequence_alphabet_for<R, S, A> &&
                 (Ordered == ordered_sequence_alphabet<A>)
    explicit joint_encoded_sequences(R&& first, S&& second, A policy = {}) {
        detail::check_joint_sequence_sizes(first, second);
        constexpr auto maximum = detail::sequence_size_limit - 1;
        detail::acquired_keys_t<R, decltype(policy.project)> a;
        detail::acquired_keys_t<S, decltype(policy.project)> b;
        a.append(std::forward<R>(first), policy.project, maximum);
        split_ = a.size();
        b.append(std::forward<S>(second), policy.project, maximum - split_);
        detail::joined_sequence_key_view keys{a, b};
        symbols_.storage_ = detail::encode_sequence_keys(keys, policy);
    }
    joint_encoded_sequences(const joint_encoded_sequences&) = default;
    joint_encoded_sequences(joint_encoded_sequences&& other) noexcept
        : symbols_(std::move(other.symbols_)), split_(std::exchange(other.split_, 0)) {}
    joint_encoded_sequences& operator=(const joint_encoded_sequences& other) {
        if (this != &other) {
            joint_encoded_sequences copy(other);
            swap(copy);
        }
        return *this;
    }
    joint_encoded_sequences& operator=(joint_encoded_sequences&& other) noexcept {
        if (this != &other) {
            joint_encoded_sequences moved(std::move(other));
            swap(moved);
        }
        return *this;
    }
    void swap(joint_encoded_sequences& other) noexcept {
        symbols_.swap(other.symbols_);
        std::swap(split_, other.split_);
    }
    friend void swap(joint_encoded_sequences& a, joint_encoded_sequences& b) noexcept {
        a.swap(b);
    }
    [[nodiscard]] std::size_t first_size() const noexcept {
        return split_;
    }
    [[nodiscard]] std::size_t second_size() const noexcept {
        return symbols_.size() - split_;
    }
    [[nodiscard]] std::uint32_t alphabet_size() const noexcept {
        return symbols_.alphabet_size();
    }
    [[nodiscard]] sequence_encoding_description describe() const noexcept {
        return symbols_.describe();
    }
    template <typename F> decltype(auto) visit(F&& function) const& {
        return symbols_.visit([&](auto codes) -> decltype(auto) {
            return std::invoke(
                std::forward<F>(function), codes.first(split_), codes.subspan(split_));
        });
    }
    template <typename F> void visit(F&&) const&& = delete;
};
template <std::ranges::input_range R,
          std::ranges::input_range S,
          sequence_alphabet A = alphabet::ordered<>>
    requires detail::joint_sequence_alphabet_for<R, S, A>
joint_encoded_sequences(R&&, S&&, A = {}) -> joint_encoded_sequences<ordered_sequence_alphabet<A>>;
} // namespace canard
#include <concepts>
#include <limits>
#include <stdexcept>
#include <string_view>

namespace canard {
struct longest_common_substring_options {
    bool certificates = true;
    bool seed_filter = true;
    suffix_array_options suffixes{};
};
// Exact unsigned-byte semantics, including embedded NUL. Inputs are neither
// modified nor retained. Empty answers have all-zero offsets. Worst-case O(n+m)
// time and temporary memory; every unsuccessful seed attempt falls back to SA-IS.
template <typename Execution = execution::scalar>
    requires(std::same_as<Execution, execution::scalar> || std::same_as<Execution, execution::avx2>)
[[nodiscard]] common_substring_match
longest_common_substring(std::string_view first,
                         std::string_view second,
                         Execution = {},
                         longest_common_substring_options options = {}) {
    constexpr auto maximum = static_cast<std::size_t>(std::numeric_limits<int>::max() - 2);
    if (first.size() > maximum || second.size() > maximum - first.size())
        throw std::length_error("canard longest_common_substring: combined input too long");
    if (first.empty() || second.empty())
        return {};
    const auto* a = reinterpret_cast<const unsigned char*>(first.data());
    const auto* b = reinterpret_cast<const unsigned char*>(second.data());
    const auto bound = std::min(first.size(), second.size());
    common_substring_match best{};
    if (options.certificates) {
        best.length = kernel::extend_lcp<Execution>(a, b, bound);
        if (best.length == bound)
            return best;
        if (first.size() != second.size() &&
            kernel::extend_lcp<Execution>(
                a + first.size() - bound, b + second.size() - bound, bound) == bound)
            return {first.size() - bound, second.size() - bound, bound};
    }
    const auto [alo, ahi] = kernel::byte_string_operations<Execution>::extrema(a, first.size());
    const auto [blo, bhi] = kernel::byte_string_operations<Execution>::extrema(b, second.size());
    if (options.certificates && (ahi < blo || bhi < alo))
        return {};
    if (options.certificates && (alo == ahi || blo == bhi)) {
        const bool swapped = alo != ahi;
        const auto* text = swapped ? a : b;
        const auto length = swapped ? first.size() : second.size();
        const auto uniform_length = swapped ? second.size() : first.size();
        const auto symbol = swapped ? blo : alo;
        std::size_t run = 0;
        best = {};
        for (std::size_t i = 0; i < length; ++i) {
            run = text[i] == symbol ? run + 1 : 0;
            if (run > best.length) {
                best = {0, i + 1 - run, run};
                if (run == uniform_length)
                    break;
            }
        }
        if (swapped)
            std::swap(best.first, best.second);
        return best;
    }
    const auto lo = std::min(alo, blo), hi = std::max(ahi, bhi);
    if (options.seed_filter) {
        if (auto result = kernel::seeded_common_substring<Execution>(first, second, lo, hi, best))
            return *result;
        if (auto result =
                kernel::dense_seeded_common_substring<Execution>(first, second, hi - lo + 1, best))
            return *result;
    }
    if (hi - lo <= 253)
        return kernel::suffix_common_substring<Execution, unsigned char>(
            first, second, lo, hi, options.suffixes, best);
    return kernel::suffix_common_substring<Execution, std::uint16_t>(
        first, second, lo, hi, options.suffixes, best);
}

// Pre-encoded pairs share one equivalence-class domain by construction. Never
// zip independently compressed sequences: identical numbers could mean unequal
// input symbols. Arbitrary class order is sufficient for LCS.
template <bool Ordered, sequence_execution Execution = execution::scalar>
[[nodiscard]] common_substring_match
longest_common_substring(const joint_encoded_sequences<Ordered>& input,
                         Execution = {},
                         longest_common_substring_options options = {}) {
    if (input.alphabet_size() > static_cast<std::uint32_t>(std::numeric_limits<int>::max() - 2))
        throw std::invalid_argument("canard LCS: alphabet too large for separator");
    if (options.certificates && input.alphabet_size() == 1)
        return {0, 0, std::min(input.first_size(), input.second_size())};
    return input.visit([&](auto first, auto second) -> common_substring_match {
        if (first.empty() || second.empty())
            return {};
        if constexpr (sizeof(typename decltype(first)::element_type) == 1) {
            return longest_common_substring(
                detail::byte_view(first), detail::byte_view(second), Execution{}, options);
        } else {
            common_substring_match best{};
            const auto bound = std::min(first.size(), second.size());
            if (options.certificates) {
                best.length =
                    kernel::extend_encoded_lcp<Execution>(first.data(), second.data(), bound);
                if (best.length == bound)
                    return best;
                if (first.size() != second.size() &&
                    kernel::extend_encoded_lcp<Execution>(first.data() + first.size() - bound,
                                                          second.data() + second.size() - bound,
                                                          bound) == bound)
                    return {first.size() - bound, second.size() - bound, bound};
            }
            if (input.alphabet_size() <= 65534)
                return kernel::sequence_common_substring<Execution, std::uint16_t>(
                    first, second, input.alphabet_size(), options.suffixes, best);
            return kernel::sequence_common_substring<Execution, std::uint32_t>(
                first, second, input.alphabet_size(), options.suffixes, best);
        }
    });
}

template <std::ranges::input_range R,
          std::ranges::input_range S,
          sequence_alphabet A,
          sequence_execution Execution = execution::scalar>
    requires detail::joint_sequence_alphabet_for<R, S, A> ||
             (detail::contiguous_byte_range<R> && detail::contiguous_byte_range<S> &&
              std::same_as<A, alphabet::ordered<>>)
[[nodiscard]] common_substring_match
    longest_common_substring(R&& first,
                             S&& second,
                             A policy,
                             Execution = {},
                             longest_common_substring_options options = {}) {
    if constexpr (detail::contiguous_byte_range<R> && detail::contiguous_byte_range<S> &&
                  std::same_as<A, alphabet::ordered<>>) {
        return longest_common_substring(
            detail::byte_view(first), detail::byte_view(second), Execution{}, options);
    } else if constexpr (detail::alphabet_kind<A> == 3) {
        detail::check_joint_sequence_sizes(first, second);
        detail::acquired_keys_t<R, decltype(policy.project)> a;
        detail::acquired_keys_t<S, decltype(policy.project)> b;
        constexpr auto maximum = detail::sequence_size_limit - 1;
        a.append(std::forward<R>(first), policy.project, maximum);
        b.append(std::forward<S>(second), policy.project, maximum - a.size());
        return kernel::equivalence_common_substring(a, b, policy.equal, options.certificates);
    } else {
        const joint_encoded_sequences input{
            std::forward<R>(first), std::forward<S>(second), std::move(policy)};
        return longest_common_substring(input, Execution{}, options);
    }
}

// Preserve the pre-existing null-terminated char-array convention for default
// calls, including mixed literal/container calls. Explicit spans and explicit
// alphabet overloads use the full supplied range, including embedded NULs.
namespace detail {
template <typename R> decltype(auto) default_sequence_range(R&& range) {
    if constexpr (character_array<R>) {
        const auto* data = std::ranges::data(range);
        const auto* end = data + std::ranges::size(range);
        return std::string_view{data, static_cast<std::size_t>(std::find(data, end, '\0') - data)};
    } else {
        return std::forward<R>(range);
    }
}
} // namespace detail

template <std::ranges::input_range R,
          std::ranges::input_range S,
          sequence_execution Execution = execution::scalar>
    requires(!(std::same_as<std::remove_cvref_t<R>, std::string_view> &&
               std::same_as<std::remove_cvref_t<S>, std::string_view>)) &&
            (detail::joint_sequence_alphabet_for<R, S, alphabet::ordered<>> ||
             (detail::contiguous_byte_range<R> && detail::contiguous_byte_range<S>))
[[nodiscard]] common_substring_match longest_common_substring(
    R&& first, S&& second, Execution = {}, longest_common_substring_options options = {}) {
    return longest_common_substring(detail::default_sequence_range(std::forward<R>(first)),
                                    detail::default_sequence_range(std::forward<S>(second)),
                                    alphabet::ordered<>{},
                                    Execution{},
                                    options);
}
} // namespace canard
#include <string_view>
#include <tuple>
int main() {
    canard::judge::input input;
    canard::judge::output output;
    const auto first = input.read(canard::io::text_token{500000});
    const auto second = input.read(canard::io::text_token{500000});
    const auto answer = canard::longest_common_substring(
        std::string_view{first}, std::string_view{second}, canard::judge::execution{});
    output.write(std::tuple{answer.first, answer.first_end(), answer.second, answer.second_end()});
    output.finish();
}
