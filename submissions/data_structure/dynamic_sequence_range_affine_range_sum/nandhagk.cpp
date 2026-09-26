// canard avx2 pipeline. Generated from clients/dynamic_sequence_range_affine_range_sum.cpp.
#ifndef CANARD_CLIENT_AVX2
#define CANARD_CLIENT_AVX2 1
#endif
// Generated from canard headers and client. Edit the maintained sources.
// ===== clients/dynamic_sequence_range_affine_range_sum.cpp =====
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
// ===== include/canard/sequence/chunked_lazy_sequence.hpp =====
// ===== include/canard/kernel/sequence_scalar.hpp =====
// ===== include/canard/algebra/monoid.hpp =====
#include <concepts>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace canard::algebra {

// These concepts check EXPRESSIONS, not the semantic laws in docs/CONTRACTS.md.
// In particular, no syntactic trait is taken as proof of commutativity.
template <typename M>
concept monoid_expressions = requires(const M& m, const value_type_t<M>& x,
                                     const value_type_t<M>& y) {
    { m.identity() } -> std::same_as<value_type_t<M>>;
    { m.combine(x, y) } -> std::same_as<value_type_t<M>>;
};
// Compatibility concept used by the current value-owning structures.
template <typename M>
concept monoid = monoid_expressions<M> && std::copy_constructible<M> &&
                 std::copyable<value_type_t<M>>;

template <typename A, typename M>
concept action_for = monoid<M> && std::copy_constructible<A> &&
                     requires(const A& a,
                              const tag_type_t<A>& newer,
                              const tag_type_t<A>& older,
                              const value_type_t<M>& x,
                              std::size_t count) {
                         requires std::copyable<tag_type_t<A>>;
                         { a.identity() } -> std::same_as<tag_type_t<A>>;
                         { a.compose(newer, older) } -> std::same_as<tag_type_t<A>>;
                         { a.map(newer, x, count) } -> std::same_as<value_type_t<M>>;
                     };

// Convenient adaptation of an ordinary stateful callable and an explicit
// identity. The callable is stored without type erasure.
template <typename T, typename Combine> struct operation_monoid {
    using value_type = T;
    [[no_unique_address]] Combine operation;
    T unit;
    [[nodiscard]] T identity() const noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return unit;
    }
    [[nodiscard]] T combine(const T& x, const T& y) const
        noexcept(std::is_nothrow_invocable_r_v<T, const Combine&, const T&, const T&>) {
        return std::invoke(operation, x, y);
    }
};
template <typename Combine, typename T>
operation_monoid(Combine, T) -> operation_monoid<T, Combine>;

template <monoid M> struct reversed_monoid {
    using value_type = value_type_t<M>;
    [[no_unique_address]] M base;
    [[nodiscard]] value_type identity() const noexcept(noexcept(base.identity())) {
        return base.identity();
    }
    [[nodiscard]] value_type combine(const value_type& x, const value_type& y) const
        noexcept(noexcept(base.combine(y, x))) {
        return base.combine(y, x);
    }
};
template <typename M> reversed_monoid(M) -> reversed_monoid<M>;

// Componentwise product: a new payload composition does not need a new tree.
template <monoid Left, monoid Right> struct product_monoid {
    using value_type = std::pair<value_type_t<Left>, value_type_t<Right>>;
    [[no_unique_address]] Left left;
    [[no_unique_address]] Right right;
    [[nodiscard]] value_type identity() const
        noexcept(noexcept(left.identity()) && noexcept(right.identity()) &&
                 std::is_nothrow_move_constructible_v<decltype(left.identity())> &&
                 std::is_nothrow_move_constructible_v<decltype(right.identity())>) {
        return {left.identity(), right.identity()};
    }
    [[nodiscard]] value_type combine(const value_type& x, const value_type& y) const
        noexcept(noexcept(left.combine(x.first, y.first)) &&
                 noexcept(right.combine(x.second, y.second)) &&
                 std::is_nothrow_move_constructible_v<value_type>) {
        return {left.combine(x.first, y.first), right.combine(x.second, y.second)};
    }
};
template <typename L, typename R> product_monoid(L, R) -> product_monoid<L, R>;

template <typename Left, typename Right> struct product_action {
    using tag_type = std::pair<tag_type_t<Left>, tag_type_t<Right>>;
    [[no_unique_address]] Left left;
    [[no_unique_address]] Right right;
    [[nodiscard]] tag_type identity() const
        noexcept(noexcept(left.identity()) && noexcept(right.identity()) &&
                 std::is_nothrow_move_constructible_v<decltype(left.identity())> &&
                 std::is_nothrow_move_constructible_v<decltype(right.identity())>) {
        return {left.identity(), right.identity()};
    }
    [[nodiscard]] tag_type compose(const tag_type& newer, const tag_type& older) const
        noexcept(noexcept(left.compose(newer.first, older.first)) &&
                 noexcept(right.compose(newer.second, older.second)) &&
                 std::is_nothrow_move_constructible_v<tag_type>) {
        return {left.compose(newer.first, older.first), right.compose(newer.second, older.second)};
    }
    template <typename X, typename Y>
    [[nodiscard]] std::pair<X, Y>
    map(const tag_type& f, const std::pair<X, Y>& x, std::size_t count) const
        noexcept(noexcept(left.map(f.first, x.first, count)) &&
                 noexcept(right.map(f.second, x.second, count)) &&
                 std::is_nothrow_move_constructible_v<std::pair<X, Y>>) {
        return {left.map(f.first, x.first, count), right.map(f.second, x.second, count)};
    }
};
template <typename L, typename R> product_action(L, R) -> product_action<L, R>;

// Used only internally to erase action storage and propagation from point trees.
struct no_action {
    struct tag_type {};
    [[nodiscard]] constexpr tag_type identity() const noexcept {
        return {};
    }
    [[nodiscard]] constexpr tag_type compose(tag_type, tag_type) const noexcept {
        return {};
    }
    template <typename T>
    [[nodiscard]] constexpr T map(tag_type, const T& x, std::size_t) const
        noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return x;
    }
};

} // namespace canard::algebra
// ===== include/canard/kernel/sequence_leaf.hpp =====
#include <algorithm>
#include <array>
#include <cstddef>
namespace canard::kernel {
// Representation-neutral movement inside an owned, fixed-capacity leaf.
// No tags, fields, balancing, node allocation, or I/O in this component.
template <typename Word, unsigned Extent> struct sequence_leaf_operations {
    struct alignas(64) leaf_type {
        std::array<Word, Extent> values{};
    };
    static void
    copy_values(leaf_type& dst, unsigned at, const leaf_type& src, unsigned first, unsigned count) {
        std::copy_n(src.values.begin() + first, count, dst.values.begin() + at);
    }
    static void insert_value(leaf_type& leaf, unsigned at, unsigned size, const Word& value) {
        std::move_backward(
            leaf.values.begin() + at, leaf.values.begin() + size, leaf.values.begin() + size + 1);
        leaf.values[at] = value;
    }
    static void erase_value(leaf_type& leaf, unsigned at, unsigned size) {
        std::move(
            leaf.values.begin() + at + 1, leaf.values.begin() + size, leaf.values.begin() + at);
    }
    // Left=A B and right=C D become reverse(D) B and C reverse(A).
    // Reversing the enclosing whole-block interval then gives
    // A reverse(C) ... reverse(B) D. Leaves must be distinct, and both
    // replacement lengths must fit Extent. No semantic algebra is assumed.
    static void exchange_reversed_outer_parts(leaf_type& left,
                                              unsigned left_size,
                                              unsigned left_cut,
                                              leaf_type& right,
                                              unsigned right_size,
                                              unsigned right_cut) {
        const unsigned tail = right_size - right_cut;
        const unsigned replacement = tail + left_size - left_cut;
        std::array<Word, Extent> saved;
        std::copy_n(left.values.begin(), left_cut, saved.begin());
        if (tail > left_cut)
            std::move_backward(left.values.begin() + left_cut,
                               left.values.begin() + left_size,
                               left.values.begin() + replacement);
        else if (tail < left_cut)
            std::move(left.values.begin() + left_cut,
                      left.values.begin() + left_size,
                      left.values.begin() + tail);
        std::reverse_copy(right.values.begin() + right_cut,
                          right.values.begin() + right_size,
                          left.values.begin());
        std::reverse_copy(
            saved.begin(), saved.begin() + left_cut, right.values.begin() + right_cut);
    }
    // Same exchange as above, but preserve each leaf's physical orientation.
    // Neither full leaf needs to be reversed merely to edit an outer fragment.
    static void exchange_reversed_outer_parts_oriented(
        leaf_type& left, unsigned left_size, unsigned left_cut, bool left_reversed,
        leaf_type& right, unsigned right_size, unsigned right_cut, bool right_reversed) {
        const unsigned tail = right_size - right_cut;
        const unsigned inside = left_size - left_cut;
        std::array<Word, Extent> saved;
        const auto copy_fragment = [&](const Word* src, unsigned count, Word* dst) {
            if (left_reversed == right_reversed)
                std::reverse_copy(src, src + count, dst);
            else std::copy_n(src, count, dst);
        };
        copy_fragment(left.values.data() + (left_reversed ? inside : 0), left_cut, saved.data());
        if (!left_reversed && tail != left_cut) {
            if (tail > left_cut)
                std::move_backward(left.values.begin() + left_cut, left.values.begin() + left_size,
                                   left.values.begin() + tail + inside);
            else std::move(left.values.begin() + left_cut, left.values.begin() + left_size,
                           left.values.begin() + tail);
        }
        copy_fragment(right.values.data() + (right_reversed ? 0 : right_cut), tail,
                      left.values.data() + (left_reversed ? inside : 0));
        if (right_reversed && left_cut != tail) {
            if (left_cut > tail)
                std::move_backward(right.values.begin() + tail, right.values.begin() + right_size,
                                   right.values.begin() + left_cut + right_cut);
            else std::move(right.values.begin() + tail, right.values.begin() + right_size,
                           right.values.begin() + left_cut);
        }
        std::copy_n(saved.begin(), left_cut,
                    right.values.begin() + (right_reversed ? 0 : right_cut));
    }
    static void reverse_values(leaf_type& leaf, unsigned first, unsigned last) {
        std::reverse(leaf.values.begin() + first, leaf.values.begin() + last);
    }
};
} // namespace canard::kernel
// ===== include/canard/algebra/operation_traits.hpp =====
#include <type_traits>
#include <utility>
namespace canard::algebra {
template <monoid Monoid, typename Action>
    requires action_for<Action, Monoid>
inline constexpr bool nothrow_action_operations_v =
        std::is_nothrow_copy_constructible_v<value_type_t<Monoid>> &&
        std::is_nothrow_copy_assignable_v<value_type_t<Monoid>> &&
        std::is_nothrow_move_constructible_v<value_type_t<Monoid>> &&
        std::is_nothrow_move_assignable_v<value_type_t<Monoid>> &&
        std::is_nothrow_copy_constructible_v<tag_type_t<Action>> &&
        std::is_nothrow_copy_assignable_v<tag_type_t<Action>> &&
        std::is_nothrow_move_constructible_v<tag_type_t<Action>> &&
        std::is_nothrow_move_assignable_v<tag_type_t<Action>> &&
        noexcept(std::declval<const Monoid&>().identity()) &&
        noexcept(std::declval<const Monoid&>().combine(std::declval<const value_type_t<Monoid>&>(),
                                                       std::declval<const value_type_t<Monoid>&>())) &&
        noexcept(std::declval<const Action&>().identity()) &&
        noexcept(std::declval<const Action&>().compose(std::declval<const tag_type_t<Action>&>(),
                                                       std::declval<const tag_type_t<Action>&>())) &&
        noexcept(std::declval<const Action&>().map(
            std::declval<const tag_type_t<Action>&>(), std::declval<const value_type_t<Monoid>&>(), std::size_t{}));
}
// ===== include/canard/sequence/configuration.hpp =====
// ===== include/canard/kernel/protocols.hpp =====
#include <concepts>
#include <cstdint>
#include <span>
#include <utility>
#include <type_traits>

namespace canard::kernel {
// These are syntactic protocols. The ordered algebra, chronology, padding and
// physical-memory laws are documented in docs/EXTENDING.md, not proven here.
template <typename K>
concept summary_kernel = requires(const K& k, const summary_type_t<K>& x,
                                  const value_type_t<K>& value) {
    { k.identity() } -> std::same_as<summary_type_t<K>>;
    { k.combine(x, x) } -> std::same_as<summary_type_t<K>>;
    k.import_value(value);
};
template <typename K>
concept wide_kernel = summary_kernel<K> && requires(K& k, const K& ck,
    leaf_type_t<K>& leaf, branch_type_t<K>& branch, const summary_type_t<K>& x,
    const action_type_t<K>& action, const subtree_change_t<K>& change,
    local_change_t<K>& local) {
    typename K::prepared_action;
    typename K::local_change;
    { K::has_lazy } -> std::convertible_to<bool>;
    { K::nothrow_mutation } -> std::convertible_to<bool>;
    { ck.empty_leaf() } -> std::same_as<leaf_type_t<K>>;
    { ck.empty_branch() } -> std::same_as<branch_type_t<K>>;
    { ck.export_value(x) } -> std::same_as<value_type_t<K>>;
    { ck.fold(leaf, 0u, 1u) } -> std::same_as<summary_type_t<K>>;
    { ck.fold(branch, 0u, 1u) } -> std::same_as<summary_type_t<K>>;
    ck.slot(leaf, 0u);
    ck.write_slot(leaf, 0u, x);
    ck.write_slot(branch, 0u, x);
    { ck.identity_action() } -> std::same_as<action_type_t<K>>;
    { ck.compose(action, action) } -> std::same_as<action_type_t<K>>;
    { ck.map(action, x, std::size_t{1}) } -> std::same_as<summary_type_t<K>>;
    { ck.needs_materialization(branch, 0u) } -> std::convertible_to<bool>;
    { ck.child_frame(branch, 0u) } -> std::same_as<action_type_t<K>>;
    ck.clear_frame(branch, 0u);
    { k.begin_changes() } -> std::same_as<local_change_t<K>>;
    { k.account_local(local, local) } -> std::same_as<void>;
    { k.repair_slot(branch, 0u, change) } -> std::same_as<local_change_t<K>>;
    { k.template finish<true>(leaf, local) } -> std::same_as<subtree_change_t<K>>;
    { k.template finish<true>(branch, local) } -> std::same_as<subtree_change_t<K>>;
    { k.replace(leaf, 0u, x) } -> std::same_as<subtree_change_t<K>>;
} && (!K::has_lazy || requires(K& k, leaf_type_t<K>& leaf, branch_type_t<K>& branch,
                             const update_type_t<K>& update, const prepared_action_t<K>& f,
                             const action_type_t<K>& incoming) {
    { k.prepare(update) } -> std::same_as<prepared_action_t<K>>;
    k.template apply_slots<true>(leaf, 0u, 1u, f, std::size_t{1});
    k.template apply_slots<true>(branch, 0u, 1u, f, std::size_t{1});
    { k.prepare_internal(incoming) } -> std::same_as<prepared_action_t<K>>;
    { k.apply_carried(leaf, 0u, 1u, f, incoming, std::size_t{1}) } -> std::same_as<local_change_t<K>>;
    { k.apply_carried(branch, 0u, 1u, f, incoming, std::size_t{1}) } -> std::same_as<local_change_t<K>>;
});

template <typename K>
concept assignment_kernel = summary_kernel<K> && requires(const K& k,
    leaf_type_t<K>& leaf, branch_type_t<K>& branch, summary_type_t<K>* output,
    const summary_type_t<K>& x) {
    typename K::monoid_type;
    { k.monoid() } -> std::same_as<const monoid_type_t<K>&>;
    { branch.tags[0] } -> std::same_as<std::uint32_t&>;
    { K::nothrow_mutation } -> std::convertible_to<bool>;
    { k.empty_leaf() } -> std::same_as<leaf_type_t<K>>;
    { k.empty_branch() } -> std::same_as<branch_type_t<K>>;
    { k.export_value(x) } -> std::same_as<value_type_t<K>>;
    { k.fold(leaf, 0u, 1u) } -> std::same_as<summary_type_t<K>>;
    { k.fold(branch, 0u, 1u) } -> std::same_as<summary_type_t<K>>;
    k.write_slot(leaf, 0u, x);
    k.slot(leaf, 0u);
    k.slot(branch, 0u);
    k.write_slot(branch, 0u, x);
    k.fill(leaf, 0u, 1u, x, std::uint32_t{});
    k.fill(branch, 0u, 1u, x, std::uint32_t{});
    k.reset(branch, 0u, 1u, x, x, 0u, 1u);
    k.make_powers(output, x, 1u);
    { k.repeat(output, 1u) } -> std::same_as<summary_type_t<K>>;
};

template <typename K>
concept sequence_kernel = summary_kernel<K> && requires(const K& k, leaf_type_t<K>& leaf,
    const summary_type_t<K>& x, const value_type_t<K>& value, const action_type_t<K>& action) {
    typename K::prepared_action;
    { K::has_lazy } -> std::convertible_to<bool>;
    { K::nothrow_mutation } -> std::convertible_to<bool>;
    { k.reversed(x) } -> std::same_as<summary_type_t<K>>;
    { k.export_summary(x) } -> std::same_as<value_type_t<K>>;
    { k.summarize(leaf, 0u, 1u) } -> std::same_as<summary_type_t<K>>;
    { k.identity_action() } -> std::same_as<action_type_t<K>>;
    { k.map(action, x, std::size_t{1}) } -> std::same_as<summary_type_t<K>>;
    k.copy_values(leaf, 0u, std::as_const(leaf), 0u, 1u);
    k.insert_value(leaf, 0u, 1u, k.import_value(value));
    k.erase_value(leaf, 0u, 1u);
    k.reverse_values(leaf, 0u, 1u);
    leaf.values[0] = k.import_value(value);
    { k.after_set(leaf, 1u, x, k.import_value(value), k.import_value(value)) }
        -> std::same_as<summary_type_t<K>>;
    { k.after_insert(leaf, 1u, x, k.import_value(value)) } -> std::same_as<summary_type_t<K>>;
    { k.after_erase(leaf, 1u, x, k.import_value(value)) } -> std::same_as<summary_type_t<K>>;
} && (!K::has_lazy || requires(const K& k, leaf_type_t<K>& leaf,
    const update_type_t<K>& update, const action_type_t<K>& action,
    const prepared_action_t<K>& f, const summary_type_t<K>& x) {
    { k.prepare(update) } -> std::same_as<prepared_action_t<K>>;
    { k.action_of(f) } -> std::convertible_to<action_type_t<K>>;
    { k.compose(action, action) } -> std::same_as<action_type_t<K>>;
    { k.materialize(leaf, 1u, action) } -> std::same_as<void>;
    { k.update_leaf(leaf, 1u, 0u, 1u, f, x) } -> std::same_as<summary_type_t<K>>;
});
template <typename K>
concept retained_assignment_kernel = assignment_kernel<K> && K::nothrow_mutation &&
    std::default_initializable<summary_type_t<K>> && std::copyable<summary_type_t<K>>;
// The current retained-slot owning sequence requires these storage properties.
// They are stronger than mathematical monoid requirements and diagnosed here.
template <typename K>
concept retained_sequence_kernel = sequence_kernel<K> && K::nothrow_mutation &&
    std::default_initializable<leaf_type_t<K>> &&
    std::default_initializable<summary_type_t<K>> &&
    std::default_initializable<action_type_t<K>> &&
    std::copyable<leaf_type_t<K>> && std::copyable<summary_type_t<K>>;

template <typename K>
concept static_block_kernel = requires(const K& k, const value_type_t<K>& x,
    const value_type_t<K>* input, value_type_t<K>* out, std::span<const value_type_t<K>> row) {
    { k.identity() } -> std::same_as<value_type_t<K>>;
    { k.combine(x, x) } -> std::same_as<value_type_t<K>>;
    k.scan(input, out, out);
    { k.fold_local(input, 0u, 1u) } -> std::same_as<value_type_t<K>>;
    k.combine_rows(row, input, out);
};
template <typename K>
concept prefix_block_kernel = requires(std::uint64_t* p) {
    typename K::leaf_type;
    typename K::branch_type;
    { K::empty_leaf() } -> std::same_as<leaf_type_t<K>>;
    { K::empty_branch() } -> std::same_as<branch_type_t<K>>;
    { K::add_suffix(p, 0u, std::uint64_t{}) } noexcept -> std::same_as<void>;
};
} // namespace canard::kernel
// ===== include/canard/wide/configuration.hpp =====
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace canard::wide {

namespace representation {
// Ordinary child summaries, with pending action tags for lazy trees.
struct ordinary {};
// Bounded signed-integer parent/child gaps and one absolute root coordinate.
// This describes an invariant and an update law, not an instruction set.
struct normalized_extremum {};
} // namespace representation
namespace execution = ::canard::execution; // Compatibility vocabulary.

// Open, compile-time customization point. Each admitted combination supplies
// a local block kernel, never an owner or a complete tree. No silent fallback.
template <typename Representation,
          typename Execution,
          typename Monoid,
          typename Action,
          unsigned Fanout>
struct kernel_binding {};

template <unsigned Fanout = 16,
          std::size_t MaxSize = (std::size_t{1} << 30),
          typename Representation = representation::ordinary,
          typename Execution = execution::scalar>
struct configuration {
    static_assert(Fanout >= 2 && Fanout <= 64 && std::has_single_bit(Fanout));
    static_assert(MaxSize > 0 && MaxSize <= std::numeric_limits<std::uint32_t>::max());
    static constexpr unsigned fanout = Fanout;
    static constexpr std::size_t max_size = MaxSize;
    using representation_type = Representation;
    using execution_type = Execution;
};

template <typename Configuration, typename Monoid, typename Action>
concept supported_configuration = requires {
    typename kernel_binding<representation_type_t<Configuration>,
                            execution_type_t<Configuration>,
                            Monoid,
                            Action,
                            Configuration::fanout>::type;
    requires canard::kernel::wide_kernel<typename kernel_binding<representation_type_t<Configuration>, execution_type_t<Configuration>, Monoid, Action, Configuration::fanout>::type>;
};

template <typename Configuration, typename Monoid, typename Action>
    requires supported_configuration<Configuration, Monoid, Action>
using kernel_for_t = typename kernel_binding<representation_type_t<Configuration>,
                                             execution_type_t<Configuration>,
                                             Monoid,
                                             Action,
                                             Configuration::fanout>::type;

} // namespace canard::wide
#include <cstddef>
#include <cstdint>
namespace canard::sequence {
template <unsigned LeafCapacity = 64,
          std::size_t MaxSize = (std::size_t{1} << 30),
          typename Representation = wide::representation::ordinary,
          typename Execution = wide::execution::scalar,
          unsigned OccupancyDivisor = 4,
          unsigned HeightTolerance = 1>
struct configuration {
    static_assert(LeafCapacity >= 8 && LeafCapacity <= 512 && std::has_single_bit(LeafCapacity));
    static_assert(MaxSize > 0 && MaxSize < (std::size_t{1} << 31));
    static constexpr unsigned leaf_capacity = LeafCapacity;
    static_assert(1 <= HeightTolerance && HeightTolerance <= 4);
    static constexpr unsigned height_tolerance = HeightTolerance;
    static_assert(OccupancyDivisor >= 2 && OccupancyDivisor <= LeafCapacity &&
                  std::has_single_bit(OccupancyDivisor));
    static constexpr unsigned occupancy_divisor = OccupancyDivisor;
    static constexpr std::size_t max_size = MaxSize;
    using representation_type = Representation;
    using execution_type = Execution;
};
template <typename Representation,
          typename Execution,
          typename Monoid,
          typename Action,
          unsigned Extent>
struct kernel_binding {};
template <typename Configuration, typename Monoid, typename Action>
concept supported_configuration = requires {
    typename kernel_binding<representation_type_t<Configuration>,
                            execution_type_t<Configuration>,
                            Monoid,
                            Action,
                            Configuration::leaf_capacity>::type;
    requires canard::kernel::sequence_kernel<typename kernel_binding<representation_type_t<Configuration>, execution_type_t<Configuration>, Monoid, Action, Configuration::leaf_capacity>::type>;
};
template <typename Configuration, typename Monoid, typename Action>
    requires supported_configuration<Configuration, Monoid, Action>
using kernel_for_t = typename kernel_binding<representation_type_t<Configuration>,
                                             execution_type_t<Configuration>,
                                             Monoid,
                                             Action,
                                             Configuration::leaf_capacity>::type;
} // namespace canard::sequence
#include <utility>
namespace canard::kernel {
// The general case explicitly stores both orders: reversal does NOT assume
// commutativity. Monoid/action objects are the same public canard algebra.
template <algebra::monoid Monoid, typename Action, unsigned Extent>
    requires (algebra::action_for<Action, Monoid> &&
              std::default_initializable<value_type_t<Monoid>>)
struct sequence_scalar : sequence_leaf_operations<value_type_t<Monoid>, Extent> {
    using value_type = value_type_t<Monoid>;
    using base = sequence_leaf_operations<value_type, Extent>;
    using leaf_type = leaf_type_t<base>;
    struct summary_type {
        value_type forward, backward;
    };
    using update_type = tag_type_t<Action>;
    using action_type = update_type;
    using prepared_action = action_type;
    static constexpr bool has_lazy = !std::same_as<Action, algebra::no_action>;
    static constexpr bool nothrow_mutation = algebra::nothrow_action_operations_v<Monoid, Action>;
    [[no_unique_address]] Monoid monoid;
    [[no_unique_address]] Action action;
    sequence_scalar(Monoid m, Action a) : monoid(std::move(m)), action(std::move(a)) {}
    [[nodiscard]] summary_type identity() const {
        return {monoid.identity(), monoid.identity()};
    }
    [[nodiscard]] summary_type combine(const summary_type& a, const summary_type& b) const {
        return {monoid.combine(a.forward, b.forward), monoid.combine(b.backward, a.backward)};
    }
    [[nodiscard]] summary_type reversed(summary_type a) const {
        std::swap(a.forward, a.backward);
        return a;
    }
    [[nodiscard]] value_type export_summary(const summary_type& a) const {
        return a.forward;
    }
    [[nodiscard]] value_type import_value(const value_type& a) const {
        return a;
    }
    [[nodiscard]] action_type identity_action() const {
        return action.identity();
    }
    [[nodiscard]] action_type compose(const action_type& a, const action_type& b) const {
        return action.compose(a, b);
    }
    [[nodiscard]] summary_type
    map(const action_type& f, const summary_type& s, std::size_t count) const {
        return {action.map(f, s.forward, count), action.map(f, s.backward, count)};
    }
    [[nodiscard]] prepared_action prepare(const update_type& f) const {
        return f;
    }
    [[nodiscard]] const action_type& action_of(const prepared_action& f) const {
        return f;
    }
    [[nodiscard]] summary_type summarize(const leaf_type& leaf, unsigned l, unsigned r) const {
        auto answer = identity();
        for (unsigned i = l; i < r; ++i)
            answer.forward = monoid.combine(answer.forward, leaf.values[i]);
        for (unsigned i = r; i-- > l;)
            answer.backward = monoid.combine(answer.backward, leaf.values[i]);
        return answer;
    }
    void materialize(leaf_type& leaf, unsigned size, const action_type& f) const {
        for (unsigned i = 0; i < size; ++i)
            leaf.values[i] = action.map(f, leaf.values[i], 1);
    }
    [[nodiscard]] summary_type update_leaf(leaf_type& leaf,
                                           unsigned size,
                                           unsigned l,
                                           unsigned r,
                                           const prepared_action& f,
                                           const summary_type&) const {
        for (unsigned i = l; i < r; ++i)
            leaf.values[i] = action.map(f, leaf.values[i], 1);
        return summarize(leaf, 0, size);
    }
    [[nodiscard]] summary_type after_insert(const leaf_type& leaf,
                                            unsigned size,
                                            const summary_type&,
                                            const value_type&) const {
        return summarize(leaf, 0, size);
    }
    [[nodiscard]] summary_type after_erase(const leaf_type& leaf,
                                           unsigned size,
                                           const summary_type&,
                                           const value_type&) const {
        return summarize(leaf, 0, size);
    }
    [[nodiscard]] summary_type after_set(const leaf_type& leaf,
                                         unsigned size,
                                         const summary_type&,
                                         const value_type&,
                                         const value_type&) const {
        return summarize(leaf, 0, size);
    }
};
} // namespace canard::kernel
namespace canard::sequence {
template <algebra::monoid Monoid, typename Action, unsigned Extent>
    requires (algebra::action_for<Action, Monoid> &&
              std::default_initializable<value_type_t<Monoid>>)
struct kernel_binding<wide::representation::ordinary,
                      wide::execution::scalar,
                      Monoid,
                      Action,
                      Extent> {
    using type = kernel::sequence_scalar<Monoid, Action, Extent>;
};
} // namespace canard::sequence
// ===== include/canard/kernel/sequence_affine_sum_common.hpp =====
// ===== include/canard/algebra/modular_types.hpp =====
#include <cstdint>

namespace canard::algebra {

template <std::uint32_t Modulus> struct affine_map {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    std::uint32_t multiplier = 1;
    std::uint32_t translation = 0;
    [[nodiscard]] constexpr std::uint32_t operator()(std::uint32_t x) const noexcept {
        return (std::uint64_t(multiplier) * x + translation) % Modulus;
    }
    friend constexpr bool operator==(affine_map, affine_map) = default;
};

template <std::uint32_t Modulus> struct modular_sum {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    using value_type = std::uint32_t;
    [[nodiscard]] constexpr value_type identity() const noexcept {
        return 0;
    }
    [[nodiscard]] constexpr value_type combine(value_type a, value_type b) const noexcept {
        const auto x = a + b;
        return x >= Modulus ? x - Modulus : x;
    }
};

template <std::uint32_t Modulus> struct affine_composition {
    using value_type = affine_map<Modulus>;
    [[nodiscard]] constexpr value_type identity() const noexcept {
        return {};
    }
    // Array order: combine(left, right) means apply left first, then right.
    [[nodiscard]] constexpr value_type combine(value_type left, value_type right) const noexcept {
        return {
            std::uint32_t(std::uint64_t(right.multiplier) * left.multiplier % Modulus),
            std::uint32_t((std::uint64_t(right.multiplier) * left.translation + right.translation) %
                          Modulus)};
    }
};

template <std::uint32_t Modulus> struct affine_on_sum {
    using tag_type = affine_map<Modulus>;
    [[nodiscard]] constexpr tag_type identity() const noexcept {
        return {};
    }
    [[nodiscard]] constexpr tag_type compose(tag_type newer, tag_type older) const noexcept {
        return affine_composition<Modulus>{}.combine(older, newer);
    }
    [[nodiscard]] constexpr std::uint32_t
    map(tag_type f, std::uint32_t x, std::size_t count) const noexcept {
        return (std::uint64_t(f.multiplier) * x +
                std::uint64_t(f.translation) * (count % Modulus)) %
               Modulus;
    }
};

} // namespace canard::algebra

// ===== include/canard/numeric/montgomery32.hpp =====
#include <cstdint>
#include <limits>
namespace canard::numeric {
// Expert arithmetic policy. Words are deliberately raw: encode/decode mark
// representation changes; operators below preserve lazy residues in [0,2p).
// No primality assumption is needed for arithmetic or power().
template <std::uint32_t Modulus> struct montgomery32 {
    using word_type = std::uint32_t;
    using u32 = word_type;
    using u64 = std::uint64_t;
    static constexpr u32 modulus = Modulus;
    static_assert(Modulus > 1 && (Modulus & 1) && Modulus < (u32{1} << 30),
                  "Requires odd 1 < modulus < 2^30.");
    static constexpr u32 twice_modulus = 2 * modulus;
    static constexpr u32 negative_inverse = [] consteval {
        u32 inverse = 1;
        for (int step = 0; step < 5; ++step) {
            inverse *= 2 - modulus * inverse;
        }
        return -inverse;
    }();
    static constexpr u32 one = (u64{1} << 32) % modulus;
    static constexpr u32 radix_squared = u64{one} * one % modulus;
    static_assert(4 * u64{modulus} < (u64{1} << 32));
    // The fused reduction below also needs its 64-bit numerator not to overflow.
    static_assert(8 * u64{modulus} * modulus + u64{modulus} * 0xffff'ffffu <
                  std::numeric_limits<u64>::max());

    // REDC primitive: both words lie in [0, 2*modulus). Usually both are
    // encoded, producing an encoded result. One encoded and one ordinary
    // operand deliberately produce an ordinary result (encoded scaling).
    [[nodiscard]] static constexpr u32 multiply(u32 a, u32 b) noexcept {
        const u64 product = u64{a} * b;
        const u32 correction = static_cast<u32>(product) * negative_inverse;
        return static_cast<u32>((product + u64{correction} * modulus) >> 32);
    }

    [[nodiscard]] static constexpr u32 add(u32 a, u32 b) noexcept {
        const u32 sum = a + b;
        return sum >= twice_modulus ? sum - twice_modulus : sum;
    }

    [[nodiscard]] static constexpr u32 subtract(u32 a, u32 b) noexcept {
        return a >= b ? a - b : a + twice_modulus - b;
    }

    // Reduce a*b+c*d together. Both products must have the same encoding
    // degree: either encoded*encoded or encoded*ordinary for both terms.
    // The intermediate result is <3p; subtracting 2p
    // when necessary restores the [0,2p) invariant.
    [[nodiscard]] static constexpr u32 multiply_sum(u32 a, u32 b, u32 c, u32 d) noexcept {
        const u64 sum = u64{a} * b + u64{c} * d;
        const u32 correction = static_cast<u32>(sum) * negative_inverse;
        const u32 result = static_cast<u32>((sum + u64{correction} * modulus) >> 32);
        return result >= twice_modulus ? result - twice_modulus : result;
    }

    [[nodiscard]] static constexpr u32 encode(u32 value) noexcept {
        return multiply(value, radix_squared);
    }

    [[nodiscard]] static constexpr u32 decode(u32 value) noexcept {
        const u32 result = multiply(value, 1);
        return result >= modulus ? result - modulus : result;
    }

    [[nodiscard]] static constexpr u32 power(u32 base, u32 exponent) noexcept {
        u32 result = one;
        for (; exponent != 0; exponent >>= 1, base = multiply(base, base)) {
            if ((exponent & 1u) != 0) {
                result = multiply(result, base);
            }
        }
        return result;
    }
};
// Primality is checked at compile time only by algorithms requiring inversion.
[[nodiscard]] consteval bool is_prime32(std::uint32_t n) {
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (std::uint32_t d = 3; std::uint64_t{d} * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

// Optional public scalar value type; exactly one word. The hot storage kernels
// use the policy above directly rather than aliasing arrays of these objects.
template <std::uint32_t Modulus> class montgomery_value {
    using arithmetic = montgomery32<Modulus>;
    std::uint32_t encoded_;
    struct encoded_tag {};
    constexpr montgomery_value(encoded_tag, std::uint32_t x) noexcept : encoded_(x) {}

  public:
    // Precondition: canonical < Modulus. No run-time checks or division here.
    explicit constexpr montgomery_value(std::uint32_t canonical = 0) noexcept
        : encoded_(arithmetic::encode(canonical)) {}
    [[nodiscard]] constexpr std::uint32_t value() const noexcept {
        return arithmetic::decode(encoded_);
    }
    friend constexpr montgomery_value operator+(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::add(a.encoded_, b.encoded_)};
    }
    friend constexpr montgomery_value operator-(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::subtract(a.encoded_, b.encoded_)};
    }
    friend constexpr montgomery_value operator*(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::multiply(a.encoded_, b.encoded_)};
    }
    friend constexpr bool operator==(montgomery_value a, montgomery_value b) noexcept {
        return a.value() == b.value();
    }
};
} // namespace canard::numeric
#include <cstdint>
namespace canard::kernel {
// Shared representation for scalar and AVX2 sequence execution. Sums/values
// are ordinary residues in [0,2p); only lazy coefficients are encoded.
template <std::uint32_t Modulus, unsigned Extent>
struct sequence_affine_sum_common : sequence_leaf_operations<std::uint32_t, Extent> {
    using field = numeric::montgomery32<Modulus>;
    using value_type = std::uint32_t;
    using summary_type = std::uint32_t;
    using base = sequence_leaf_operations<value_type, Extent>;
    using leaf_type = leaf_type_t<base>;
    using update_type = algebra::affine_map<Modulus>;
    struct action_type {
        value_type multiplier, translation;
    };
    static constexpr bool has_lazy = true;
    static constexpr bool reversal_invariant_summary = true;
    static constexpr bool nothrow_mutation = true;
    static constexpr std::size_t max_size = 2ull * Modulus - 1;
    sequence_affine_sum_common(algebra::modular_sum<Modulus>, algebra::affine_on_sum<Modulus>) {}
    [[nodiscard]] static summary_type identity() noexcept {
        return 0;
    }
    [[nodiscard]] static summary_type combine(summary_type a, summary_type b) noexcept {
        return field::add(a, b);
    }
    [[nodiscard]] static summary_type reversed(summary_type a) noexcept {
        return a;
    }
    [[nodiscard]] static value_type export_summary(summary_type a) noexcept {
        return a >= Modulus ? a - Modulus : a;
    }
    [[nodiscard]] static value_type import_value(value_type a) noexcept {
        return a;
    }
    [[nodiscard]] static action_type identity_action() noexcept {
        return {field::one, 0};
    }
    [[nodiscard]] static action_type encode(update_type f) noexcept {
        return {field::encode(f.multiplier), field::encode(f.translation)};
    }
    [[nodiscard]] static action_type compose(action_type a, action_type b) noexcept {
        return {field::multiply(a.multiplier, b.multiplier),
                field::add(field::multiply(a.multiplier, b.translation), a.translation)};
    }
    [[nodiscard]] static summary_type
    map(action_type f, summary_type s, std::size_t count) noexcept {
        return field::multiply_sum(f.multiplier, s, f.translation, static_cast<unsigned>(count));
    }
    [[nodiscard]] static summary_type remove_prefix_summary(summary_type total,
                                                            summary_type prefix) noexcept {
        return field::subtract(total, prefix);
    }
    [[nodiscard]] static summary_type remove_suffix_summary(summary_type total,
                                                            summary_type suffix) noexcept {
        return field::subtract(total, suffix);
    }
    [[nodiscard]] static summary_type
    after_reverse(const leaf_type&, unsigned, unsigned, unsigned, summary_type old) noexcept {
        return old;
    }
    [[nodiscard]] static summary_type
    after_insert(const leaf_type&, unsigned, summary_type old, value_type value) noexcept {
        return field::add(old, value);
    }
    [[nodiscard]] static summary_type
    after_erase(const leaf_type&, unsigned, summary_type old, value_type value) noexcept {
        return field::subtract(old, value);
    }
    // Erasure can leave a uniform affine frame pending. Remove the observed
    // value from the already-mapped summary, but keep all remaining words raw.
    [[nodiscard]] static summary_type after_erase_pending(
        const leaf_type&, unsigned, summary_type old,
        value_type raw, action_type incoming) noexcept {
        return field::subtract(old, map(incoming, raw, 1));
    }
    [[nodiscard]] static summary_type after_set(const leaf_type&,
                                                unsigned,
                                                summary_type old,
                                                value_type before,
                                                value_type after) noexcept {
        return field::add(old, field::subtract(after, before));
    }
};
template <std::uint32_t Modulus, unsigned Extent>
struct sequence_affine_sum_scalar : sequence_affine_sum_common<Modulus, Extent> {
    using base = sequence_affine_sum_common<Modulus, Extent>;
    using field = field_t<base>;
    using leaf_type = leaf_type_t<base>;
    using summary_type = summary_type_t<base>;
    using update_type = update_type_t<base>;
    using action_type = action_type_t<base>;
    using prepared_action = action_type;
    using base::base;
    [[nodiscard]] static prepared_action prepare(update_type f) noexcept {
        return base::encode(f);
    }
    [[nodiscard]] static action_type action_of(prepared_action f) noexcept {
        return f;
    }
    [[nodiscard]] static summary_type
    summarize(const leaf_type& leaf, unsigned l, unsigned r) noexcept {
        std::uint64_t total = 0;
        for (unsigned i = l; i < r; ++i)
            total += leaf.values[i];
        return total % Modulus;
    }
    static void materialize(leaf_type& leaf, unsigned size, action_type f) noexcept {
        for (unsigned i = 0; i < size; ++i)
            leaf.values[i] = base::map(f, leaf.values[i], 1);
    }
    [[nodiscard]] static summary_type update_leaf(leaf_type& leaf,
                                                  unsigned,
                                                  unsigned l,
                                                  unsigned r,
                                                  prepared_action f,
                                                  summary_type old) noexcept {
        summary_type before = 0, after = 0;
        for (unsigned i = l; i < r; ++i) {
            before = field::add(before, leaf.values[i]);
            leaf.values[i] = base::map(f, leaf.values[i], 1);
            after = field::add(after, leaf.values[i]);
        }
        return field::add(old, field::subtract(after, before));
    }
};
} // namespace canard::kernel
namespace canard::sequence {
template <std::uint32_t P, unsigned Extent>
    requires(P > 1 && (P & 1) != 0 && P < (1u << 30))
struct kernel_binding<wide::representation::ordinary,
                      wide::execution::scalar,
                      algebra::modular_sum<P>,
                      algebra::affine_on_sum<P>,
                      Extent> {
    using type = kernel::sequence_affine_sum_scalar<P, Extent>;
};
} // namespace canard::sequence
// ===== include/canard/sequence/detail/chunked_engine.hpp =====
// ===== include/canard/kernel/capabilities.hpp =====
namespace canard::kernel {
// Optional hooks have full semantic fallback contracts in docs/EXTENDING.md.
// Passing a concept checks only the spelling/types, not mathematical laws.
template <typename K>
concept carried_leaf_update = sequence_kernel<K> && requires(const K& k,
    leaf_type_t<K>& leaf, const prepared_action_t<K>& f,
    const action_type_t<K>& incoming, const summary_type_t<K>& sum) {
    { k.update_leaf_carried(leaf, 1u, 0u, 1u, f, incoming, sum) }
        -> std::same_as<summary_type_t<K>>;
};
template <typename K>
concept residual_summaries = sequence_kernel<K> && requires(const K& k,
    const summary_type_t<K>& total, const summary_type_t<K>& prefix) {
    { k.remove_prefix_summary(total, prefix) } -> std::same_as<summary_type_t<K>>;
    { k.remove_suffix_summary(total, prefix) } -> std::same_as<summary_type_t<K>>;
};
} // namespace canard::kernel
// ===== include/canard/sequence/detail/boundary_repair.hpp =====
#include <algorithm>
#include <array>
#include <cstdint>
namespace canard::detail {
template <typename Owner> struct chunked_boundary_repair {
    using handle = std::uint32_t;
    using topology_access = typename Owner::topology_access;
    using topology = typename Owner::topology;
    static constexpr unsigned B = Owner::B;
    static constexpr unsigned minimum_occupancy = Owner::minimum_occupancy;
    static constexpr unsigned path_capacity = Owner::path_capacity;
    // Repair the two edge leaves in place. Do not detach and then reinsert
    // leaves whose positions do not change: their recorded spines need only
    // one bottom-up summary repair. A merge deletes one leaf and its parent.
    static handle concatenate(Owner& owner, handle a, handle b) {
        auto& nodes_ = owner.nodes_;
        auto& leaves_ = owner.leaves_;
        auto& kernel_ = owner.kernel_;
        if (!a)
            return b;
        if (!b)
            return a;
        topology_access access{owner};
        topology tree{access};
        if (nodes_[a].back >= minimum_occupancy && nodes_[b].front >= minimum_occupancy)
            return tree.join(a, b);
        std::array<handle, path_capacity> apath, bpath;
        unsigned an = 0, bn = 0;
        handle x = a, y = b;
        while (!owner.is_leaf(x)) {
            owner.push_branch(x);
            apath[an++] = x;
            x = nodes_[x].right;
        }
        while (!owner.is_leaf(y)) {
            owner.push_branch(y);
            bpath[bn++] = y;
            y = nodes_[y].left;
        }
        owner.push_leaf(x);
        owner.push_leaf(y);
        const unsigned nx = nodes_[x].length, ny = nodes_[y].length, total = nx + ny;
        auto& left = leaves_[nodes_[x].leaf];
        auto& right = leaves_[nodes_[y].leaf];
        if (total <= B) {
            const auto combined = kernel_.combine(nodes_[x].summary, nodes_[y].summary);
            kernel_.copy_values(left, nx, right, 0, ny);
            nodes_[x].length = total;
            nodes_[x].front = nodes_[x].back = std::min<unsigned>(total, minimum_occupancy);
            nodes_[x].summary = combined;
            owner.release_leaf(y);
            if (bn == 0)
                b = 0;
            else {
                const auto parent = bpath[--bn];
                b = nodes_[parent].right;
                nodes_.release(parent);
                while (bn) {
                    const auto parent = bpath[--bn];
                    nodes_[parent].left = b;
                    b = tree.rebalance(parent);
                }
            }
            while (an)
                owner.repair_branch(apath[--an]);
            return concatenate(owner, a, b);
        }
        const unsigned target = total / 2;
        if (nx < target) {
            const unsigned moved = target - nx;
            kernel_.copy_values(left, nx, right, 0, moved);
            std::move(
                right.values.begin() + moved, right.values.begin() + ny, right.values.begin());
        } else if (nx > target) {
            const unsigned moved = nx - target;
            std::move_backward(
                right.values.begin(), right.values.begin() + ny, right.values.begin() + ny + moved);
            kernel_.copy_values(right, 0, left, target, moved);
        }
        nodes_[x].length = target;
        nodes_[y].length = total - target;
        owner.repair_leaf(x);
        owner.repair_leaf(y);
        while (an)
            owner.repair_branch(apath[--an]);
        while (bn)
            owner.repair_branch(bpath[--bn]);
        return tree.join(a, b);
    }
 };
}
// ===== include/canard/sequence/detail/topology_access.hpp =====
#include <cstdint>
namespace canard::detail {
template <typename Owner>
struct chunked_topology_access {
    using handle = std::uint32_t;
    using size_type = typename Owner::size_type;
        Owner& owner;
        unsigned height(handle x) const {
            return x ? owner.nodes_[x].height : 0;
        }
        size_type length(handle x) const {
            return x ? owner.nodes_[x].length : 0;
        }
        bool is_leaf(handle x) const {
            return owner.nodes_[x].leaf != 0;
        }
        handle left(handle x) const {
            return owner.nodes_[x].left;
        }
        handle right(handle x) const {
            return owner.nodes_[x].right;
        }
        void set_left(handle x, handle child) const {
            owner.nodes_[x].left = child;
        }
        void set_right(handle x, handle child) const {
            owner.nodes_[x].right = child;
        }
        void push(handle x) const {
            owner.push_branch(x);
        }
        void repair(handle x) const {
            owner.repair_branch(x);
        }
        handle make_branch(handle a, handle b) const {
            return owner.make_branch(a, b);
        }
        handle reuse_branch(handle x, handle a, handle b) const {
            owner.nodes_[x].left = a;
            owner.nodes_[x].right = b;
            owner.repair_branch(x);
            return x;
        }
        void release_branch(handle x) const {
            owner.nodes_.release(x);
        }
        auto split_leaf(handle x, size_type k) const {
            return owner.split_leaf(x, k);
        }
    };
}
// ===== include/canard/memory/indexed_pool.hpp =====
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace canard::memory {
// Stable handles, not stable references. Growth may relocate slots; a caller
// must not retain references across acquire/ensure_available. Index 0 is null.
// The free-list capacity always covers the slots capacity, including after a
// copy, so release performs no allocation. Released slot objects are retained
// until reuse or pool destruction.
template <typename T> class indexed_pool {
    std::vector<T> slots_;
    std::vector<std::uint32_t> free_;

  public:
    indexed_pool() {
        reserve(16);
        slots_.emplace_back();
    }
    indexed_pool(const indexed_pool& other) : slots_(other.slots_), free_(other.free_) {
        free_.reserve(slots_.capacity());
    }
    indexed_pool(indexed_pool&&) noexcept = default;
    indexed_pool& operator=(const indexed_pool& other) {
        if (this != &other) {
            indexed_pool next{other};
            swap(next);
        }
        return *this;
    }
    indexed_pool& operator=(indexed_pool&&) noexcept = default;
    void swap(indexed_pool& other) noexcept {
        slots_.swap(other.slots_);
        free_.swap(other.free_);
    }
    void reserve(std::size_t count) {
        if (count <= slots_.capacity())
            return;
        free_.reserve(count);
        slots_.reserve(count);
    }
    void ensure_available(std::size_t count) {
        const auto needed = std::max<std::size_t>(1, slots_.size()) +
                            (count > free_.size() ? count - free_.size() : 0);
        if (needed > slots_.capacity())
            reserve(std::max(needed, 2 * slots_.capacity()));
        if (slots_.empty())
            slots_.emplace_back();
    }
    [[nodiscard]] std::uint32_t acquire() {
        const auto id = acquire_retained();
        slots_[id] = T{};
        return id;
    }
    // Returned objects are initialized, but a recycled slot retains its old
    // value. A leaf owner overwrites all active positions before publishing it.
    [[nodiscard]] std::uint32_t acquire_retained() {
        if (!free_.empty()) {
            const auto id = free_.back();
            free_.pop_back();
            return id;
        }
        ensure_available(1);
        const auto id = static_cast<std::uint32_t>(slots_.size());
        slots_.emplace_back();
        return id;
    }
    void release(std::uint32_t id) noexcept {
        free_.push_back(id);
    }
    [[nodiscard]] T& operator[](std::uint32_t id) noexcept {
        return slots_[id];
    }
    [[nodiscard]] const T& operator[](std::uint32_t id) const noexcept {
        return slots_[id];
    }
    [[nodiscard]] std::size_t live_slots() const noexcept {
        return slots_.empty() ? 0 : slots_.size() - 1 - free_.size();
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return slots_.capacity() * sizeof(T) + free_.capacity() * sizeof(std::uint32_t);
    }
};
} // namespace canard::memory
// ===== include/canard/structural/height_balanced_rope.hpp =====
// ===== include/canard/structural/rope_access.hpp =====
#include <concepts>
#include <cstdint>
#include <utility>
namespace canard::structural {
template <typename A>
concept rope_access = requires(const A& a, typename A::handle h, typename A::size_type n) {
    { a.height(h) } -> std::convertible_to<unsigned>;
    { a.length(h) } -> std::same_as<typename A::size_type>;
    { a.is_leaf(h) } -> std::convertible_to<bool>;
    { a.left(h) } -> std::same_as<typename A::handle>;
    { a.right(h) } -> std::same_as<typename A::handle>;
    a.set_left(h, h); a.set_right(h, h); a.push(h); a.repair(h);
    { a.make_branch(h, h) } -> std::same_as<typename A::handle>;
    { a.reuse_branch(h, h, h) } -> std::same_as<typename A::handle>;
    a.release_branch(h);
    a.split_leaf(h, n);
};
} // namespace canard::structural
#include <algorithm>
#include <cstdint>
#include <utility>
#include <span>
namespace canard::structural {
// Leaf-oriented height-balanced split/join scheduler. MaxImbalance=1 is
// strict AVL; a fixed larger tolerance trades height for fewer rotations. Access owns metadata,
// payloads, lazy interpretation, and storage lifetime. This file knows none of them. Every tree
// passed to join obeys the selected height-balance bound; every non-leaf has two children.
template <typename Access, unsigned MaxImbalance = 1> class height_balanced_rope {
    static_assert(1 <= MaxImbalance && MaxImbalance <= 4);
    using handle = std::uint32_t;
    Access& access_;
    handle rotate_right(handle x) {
        const auto y = access_.left(x);
        access_.push(y);
        access_.set_left(x, access_.right(y));
        access_.set_right(y, x);
        access_.repair(x);
        access_.repair(y);
        return y;
    }
    handle rotate_left(handle x) {
        const auto y = access_.right(x);
        access_.push(y);
        access_.set_right(x, access_.left(y));
        access_.set_left(y, x);
        access_.repair(x);
        access_.repair(y);
        return y;
    }

  public:
    explicit height_balanced_rope(Access& access) : access_(access) {
        static_assert(rope_access<Access>, "Incomplete height-balanced rope access protocol");
    }
    // Precondition: the child-height difference is at most MaxImbalance+1, the node is
    // pushed, and both children are valid. The caller may retain only handles.
    CANARD_ALWAYS_INLINE handle rebalance(handle x) {
        const auto a = access_.left(x), b = access_.right(x);
        if (access_.height(a) > access_.height(b) + MaxImbalance) {
            access_.push(a);
            if (access_.height(access_.left(a)) < access_.height(access_.right(a)))
                access_.set_left(x, rotate_left(a));
            return rotate_right(x);
        }
        if (access_.height(b) > access_.height(a) + MaxImbalance) {
            access_.push(b);
            if (access_.height(access_.right(b)) < access_.height(access_.left(b)))
                access_.set_right(x, rotate_right(b));
            return rotate_left(x);
        }
        access_.repair(x);
        return x;
    }

  private:
    // Only the height-mismatched case needs a recursive descent. Keep that
    // out of the compatible-height join so callers can reuse a spare directly.
    // Precondition: both trees are nonempty and their height difference exceeds
    // MaxImbalance. ha/hb are their current heights.
    CANARD_NOINLINE handle
    join_unbalanced(handle a, handle b, handle spare, unsigned ha, unsigned hb) {
        if (ha > hb + MaxImbalance) {
            access_.push(a);
            const auto child = join(access_.right(a), b, spare);
            access_.set_right(a, child);
            return rebalance(a);
        }
        access_.push(b);
        const auto child = join(a, access_.left(b), spare);
        access_.set_left(b, child);
        return rebalance(b);
    }

  public:
    CANARD_ALWAYS_INLINE handle join(handle a, handle b, handle spare = 0) {
        if (!a || !b) {
            if (spare)
                access_.release_branch(spare);
            return a ? a : b;
        }
        const auto ha = access_.height(a), hb = access_.height(b);
        if (ha > hb + MaxImbalance || hb > ha + MaxImbalance)
            return join_unbalanced(a, b, spare, ha, hb);
        return spare ? access_.reuse_branch(spare, a, b) : access_.make_branch(a, b);
    }
    std::pair<handle, handle> split(handle x, std::uint32_t position) {
        if (position == 0)
            return {0, x};
        if (position == access_.length(x))
            return {x, 0};
        if (access_.is_leaf(x))
            return access_.split_leaf(x, position);
        access_.push(x);
        const auto a = access_.left(x), b = access_.right(x), n = access_.length(a);
        if (position == n) {
            access_.release_branch(x);
            return {a, b};
        }
        if (position < n) {
            const auto [first, second] = split(a, position);
            return {first, join(second, b, x)};
        }
        const auto [first, second] = split(b, position - n);
        return {join(a, first, x), second};
    }
    // Reconstruct a split from an already exposed root-to-leaf path.
    // All path branches must already be pushed. The nonempty replacement parts
    // occupy the old leaf's position; at least one part must be nonempty.
    // RefreshPath also repairs the initially retained spine after leaf content
    // or length changes. All other repairs are performed by join, once.
    //
    // At most one branch becomes unused: after the first separation both parts
    // are nonempty, so later branches are reused by join. Its handle stays live
    // and must be consumed as a join spare or explicitly released by the caller.
    struct split_result {
        handle first, second, spare;
    };
    template <bool RefreshPath = false>
    split_result split_from_leaf_retained(handle leaf,
                                          handle first,
                                          handle second,
                                          std::span<const handle> path) {
        handle spare = 0;
        auto child = leaf;
        for (auto i = path.size(); i--;) {
            const auto parent = path[i];
            const auto left = access_.left(parent), right = access_.right(parent);
            if (child == left) {
                if (!first) {
                    if constexpr (RefreshPath)
                        access_.repair(parent);
                    second = parent;
                } else if (!second) {
                    second = right;
                    spare = parent;
                } else
                    second = join(second, right, parent);
            } else {
                if (!second) {
                    if constexpr (RefreshPath)
                        access_.repair(parent);
                    first = parent;
                } else if (!first) {
                    first = left;
                    spare = parent;
                } else
                    first = join(left, first, parent);
            }
            child = parent;
        }
        return {first, second, spare};
    }
    template <bool RefreshPath = false>
    std::pair<handle, handle> split_from_leaf(handle leaf,
                                             handle first,
                                             handle second,
                                             std::span<const handle> path) {
        auto result = split_from_leaf_retained<RefreshPath>(leaf, first, second, path);
        if (result.spare)
            access_.release_branch(result.spare);
        return {result.first, result.second};
    }
    // Returns {remaining tree, detached boundary leaf}. No allocation.
    template <bool Front> std::pair<handle, handle> pop_leaf(handle x) {
        if (access_.is_leaf(x))
            return {0, x};
        access_.push(x);
        const auto child = Front ? access_.left(x) : access_.right(x);
        const auto [rest, leaf] = pop_leaf<Front>(child);
        if (!rest) {
            const auto other = Front ? access_.right(x) : access_.left(x);
            access_.release_branch(x);
            return {other, leaf};
        }
        if constexpr (Front)
            access_.set_left(x, rest);
        else
            access_.set_right(x, rest);
        return {rebalance(x), leaf};
    }
};
} // namespace canard::structural
// ===== include/canard/sequence/detail/rope_boundary_fold.hpp =====
// ===== include/canard/memory/local_stack.hpp =====
#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>
#include <type_traits>

namespace canard::memory {

// Bounded automatic-storage stack. Only live entries construct a T, so query
// frames do not require default-constructible or cheap-to-construct summaries.
// No allocation. Capacity and nonempty access are caller preconditions.
template <std::destructible T, std::size_t Capacity> class local_stack {
    static_assert(Capacity > 0);
    struct alignas(T) slot {
        std::byte bytes[sizeof(T)];
    };
    std::array<slot, Capacity> slots_;
    std::size_t size_ = 0;

    T* live(std::size_t i) noexcept {
        return std::launder(reinterpret_cast<T*>(slots_[i].bytes));
    }
    const T* live(std::size_t i) const noexcept {
        return std::launder(reinterpret_cast<const T*>(slots_[i].bytes));
    }

  public:
    using value_type = T;
    local_stack() noexcept = default;
    local_stack(const local_stack&) = delete;
    local_stack& operator=(const local_stack&) = delete;
    ~local_stack() {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            while (!empty()) {
                pop_back();
            }
        }
    }
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    T& emplace(Args&&... args) {
        assert(size_ < Capacity);
        auto* value = std::construct_at(reinterpret_cast<T*>(slots_[size_].bytes),
                                       std::forward<Args>(args)...);
        ++size_;
        return *value;
    }
    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }
    T& back() noexcept {
        assert(!empty());
        return *live(size_ - 1);
    }
    const T& back() const noexcept {
        assert(!empty());
        return *live(size_ - 1);
    }
    void pop_back() noexcept {
        assert(!empty());
        std::destroy_at(live(--size_));
    }
};

} // namespace canard::memory
#include <cstdint>
#include <array>

namespace canard::detail {

// One read-only branch observation in the requested orientation. A pending
// reversal changes the order and the orientation of both children, not values.
struct oriented_rope_children {
    std::uint32_t first;
    std::uint32_t second;
    unsigned first_length;
    bool reversed;
};

// Ordered two-boundary fold. The reader supplies summaries, leaf observation
// and optional local observation frames. Only framed readers keep deferred
// accumulators; the action-free schedule retains no path stack. Neither path
// knows sums, inverses, packed words, mutable pointers or SIMD instructions.
template <typename Reader> class rope_boundary_fold {
    using summary_type = summary_type_t<Reader>;
    using handle = std::uint32_t;
    Reader read_;

    // Save an accumulator only when crossing a local observation frame.
    // Clean levels stay in the two register accumulators. Lifting happens
    // after combining child pieces, in inner-to-outer chronological order.
    [[nodiscard]] summary_type fold_with_lifts(handle x, unsigned first, unsigned last) const {
        struct frame {
            handle parent;
            unsigned count;
            summary_type outside;
        };
        std::array<handle, Reader::path_capacity> common;
        unsigned common_size = 0;
        const unsigned result_length = last - first;
        const auto finish = [&](summary_type result) {
            for (unsigned i = common_size; i-- > 0;)
                result = read_.lift(common[i], result, result_length);
            return result;
        };
        bool flip = false;
        for (;;) {
            if (first == 0 && last == read_.length(x))
                return finish(read_.whole(x, flip));
            if (read_.is_leaf(x))
                return finish(read_.leaf_fold(x, first, last, flip));
            if (read_.needs_lift(x))
                common[common_size++] = x;
            const auto branch = read_.children(x, flip);
            flip = branch.reversed;
            if (last <= branch.first_length) {
                x = branch.first;
                continue;
            }
            if (first >= branch.first_length) {
                x = branch.second;
                first -= branch.first_length;
                last -= branch.first_length;
                continue;
            }
            handle left = branch.first, right = branch.second;
            last -= branch.first_length;
            bool left_flip = flip, right_flip = flip;
            memory::local_stack<frame, Reader::path_capacity> left_path, right_path;
            auto suffix = read_.identity(), prefix = read_.identity();
            for (;;) {
                const bool left_done = first == 0 || read_.is_leaf(left);
                const bool right_done = last == read_.length(right) || read_.is_leaf(right);
                if (left_done && right_done)
                    break;
                if (!left_done) {
                    if (read_.needs_lift(left)) {
                        left_path.emplace(left, read_.length(left) - first, suffix);
                        suffix = read_.identity();
                    }
                    const auto child = read_.children(left, left_flip);
                    left_flip = child.reversed;
                    if (first < child.first_length) {
                        suffix = read_.combine(read_.whole(child.second, left_flip), suffix);
                        left = child.first;
                    } else {
                        first -= child.first_length;
                        left = child.second;
                    }
                }
                if (!right_done) {
                    if (read_.needs_lift(right)) {
                        right_path.emplace(right, last, prefix);
                        prefix = read_.identity();
                    }
                    const auto child = read_.children(right, right_flip);
                    right_flip = child.reversed;
                    if (last <= child.first_length) {
                        right = child.first;
                    } else {
                        prefix = read_.combine(prefix, read_.whole(child.first, right_flip));
                        last -= child.first_length;
                        right = child.second;
                    }
                }
            }
            const auto left_piece = first == 0
                ? read_.whole(left, left_flip)
                : read_.leaf_fold(left, first, read_.length(left), left_flip);
            const auto right_piece = last == read_.length(right)
                ? read_.whole(right, right_flip)
                : read_.leaf_fold(right, 0, last, right_flip);
            suffix = read_.combine(left_piece, suffix);
            prefix = read_.combine(prefix, right_piece);
            while (!left_path.empty() || !right_path.empty()) {
                if (!left_path.empty()) {
                    const auto& saved = left_path.back();
                    suffix = read_.combine(read_.lift(saved.parent, suffix, saved.count),
                                           saved.outside);
                    left_path.pop_back();
                }
                if (!right_path.empty()) {
                    const auto& saved = right_path.back();
                    prefix = read_.combine(saved.outside,
                                           read_.lift(saved.parent, prefix, saved.count));
                    right_path.pop_back();
                }
            }
            return finish(read_.combine(suffix, prefix));
        }
    }

  public:
    explicit rope_boundary_fold(Reader reader) : read_(reader) {}

    // Precondition: x is nonempty and 0 <= first < last <= length(x).
    [[nodiscard]] summary_type operator()(handle x, unsigned first, unsigned last) const {
        if constexpr (Reader::has_frames)
            return fold_with_lifts(x, first, last);
        bool flip = false;
        while (!read_.is_leaf(x)) {
            if (first == 0 && last == read_.length(x))
                return read_.whole(x, flip);
            const auto branch = read_.children(x, flip);
            const auto count = branch.first_length;
            flip = branch.reversed;
            if (last <= count) {
                x = branch.first;
                continue;
            }
            if (first >= count) {
                x = branch.second;
                first -= count;
                last -= count;
                continue;
            }

            handle left = branch.first, right = branch.second;
            last -= count;
            bool left_flip = flip, right_flip = flip;
            auto suffix = read_.identity(), prefix = read_.identity();
            // Interleave the independent endpoint paths. Covered right siblings
            // prepend to the left accumulator; left siblings append to the right.
            while (!read_.is_leaf(left) || !read_.is_leaf(right)) {
                if (!read_.is_leaf(left)) {
                    const auto child = read_.children(left, left_flip);
                    left_flip = child.reversed;
                    if (first < child.first_length) {
                        suffix = read_.combine(read_.whole(child.second, left_flip), suffix);
                        left = child.first;
                    } else {
                        left = child.second;
                        first -= child.first_length;
                    }
                }
                if (!read_.is_leaf(right)) {
                    const auto child = read_.children(right, right_flip);
                    right_flip = child.reversed;
                    if (last <= child.first_length) {
                        right = child.first;
                    } else {
                        prefix = read_.combine(prefix, read_.whole(child.first, right_flip));
                        right = child.second;
                        last -= child.first_length;
                    }
                }
            }
            suffix = read_.combine(
                read_.leaf_fold(left, first, read_.length(left), left_flip), suffix);
            prefix = read_.combine(prefix, read_.leaf_fold(right, 0, last, right_flip));
            return read_.combine(suffix, prefix);
        }
        return read_.leaf_fold(x, first, last, flip);
    }
};

} // namespace canard::detail
#include <algorithm>
#include <cassert>
#include <bit>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>
#include <stdexcept>

namespace canard::detail {
// Online sequence owner/maintenance engine. Leaf kernels specify algebra,
// representation and execution; height_balanced_rope schedules structural changes.
template <typename Kernel, typename Configuration> class chunked_engine {
  public:
    using value_type = value_type_t<Kernel>;
    using summary_type = summary_type_t<Kernel>;
    using update_type = update_type_t<Kernel>;
    using action_type = action_type_t<Kernel>;
    using prepared_action = prepared_action_t<Kernel>;
    using leaf_type = leaf_type_t<Kernel>;
    using size_type = std::uint32_t;
    static constexpr unsigned leaf_capacity = Configuration::leaf_capacity;
    static constexpr bool nothrow_mutation = Kernel::nothrow_mutation;
    static constexpr bool has_lazy = Kernel::has_lazy;

  private:
    using handle = std::uint32_t;
    static constexpr unsigned B = leaf_capacity;
    static constexpr unsigned height_tolerance = Configuration::height_tolerance;
    // At least one factor of two in leaf count for every tolerance+1 levels.
    // This bound also covers legacy strict-AVL configurations and tiny trees.
    static constexpr unsigned path_capacity =
        (height_tolerance + 1) * std::bit_width(Configuration::max_size) + 1;
    static constexpr unsigned minimum_occupancy = B / Configuration::occupancy_divisor;
    static constexpr unsigned reversed_flag = 1, dirty_flag = 2;
    struct node {
        handle left = 0, right = 0;
        size_type length = 0;
        handle leaf = 0;
        [[no_unique_address]] action_type action;
        summary_type summary;
        using occupancy_type =
            std::conditional_t<(minimum_occupancy < 256), std::uint8_t, std::uint16_t>;
        occupancy_type front = 0, back = 0;
        std::uint8_t height = 0, flags = 0;
    };
    [[no_unique_address]] Kernel kernel_;
    memory::indexed_pool<node> nodes_;
    memory::indexed_pool<leaf_type> leaves_;
    handle root_ = 0;

    // This is the complete topology interface. The scheduler cannot access
    // value arrays or infer the meaning of an action or a summary.
    friend struct chunked_topology_access<chunked_engine>;
    using topology_access = chunked_topology_access<chunked_engine>;
    using topology = structural::height_balanced_rope<topology_access, height_tolerance>;
    [[nodiscard]] size_type length(handle x) const noexcept {
        return x ? nodes_[x].length : 0;
    }
    [[nodiscard]] unsigned height(handle x) const noexcept {
        return x ? nodes_[x].height : 0;
    }
    [[nodiscard]] bool is_leaf(handle x) const noexcept {
        return nodes_[x].leaf != 0;
    }
    [[nodiscard]] summary_type aggregate(handle x) const {
        return x ? nodes_[x].summary : kernel_.identity();
    }
    handle new_leaf() {
        const auto payload = leaves_.acquire_retained();
        const auto id = nodes_.acquire();
        auto& n = nodes_[id];
        n.leaf = payload;
        n.height = 1;
        n.action = kernel_.identity_action();
        n.summary = kernel_.identity();
        return id;
    }
    void release_leaf(handle x) noexcept {
        leaves_.release(nodes_[x].leaf);
        nodes_.release(x);
    }
    void repair_leaf(handle x, const summary_type& summary) {
        auto& n = nodes_[x];
        n.front = n.back = std::min<unsigned>(n.length, minimum_occupancy);
        n.summary = summary;
    }
    void repair_leaf(handle x) {
        const auto& n = nodes_[x];
        repair_leaf(x, kernel_.summarize(leaves_[n.leaf], 0, n.length));
    }
    [[nodiscard]] std::pair<summary_type, summary_type> partition_summary(
        const leaf_type& leaf, unsigned length, unsigned at, const summary_type& total) const {
        if constexpr (requires(const summary_type& part) {
                          kernel_.remove_prefix_summary(total, part);
                          kernel_.remove_suffix_summary(total, part);
                      }) {
            if (at <= length - at) {
                const auto first = kernel_.summarize(leaf, 0, at);
                return {first, kernel_.remove_prefix_summary(total, first)};
            }
            const auto second = kernel_.summarize(leaf, at, length);
            return {kernel_.remove_suffix_summary(total, second), second};
        } else
            return {kernel_.summarize(leaf, 0, at), kernel_.summarize(leaf, at, length)};
    }
    void repair_branch(handle x) {
        auto& n = nodes_[x];
        const auto& a = nodes_[n.left];
        const auto& b = nodes_[n.right];
        assert(n.left && n.right && n.flags == 0);
        n.length = a.length + b.length;
        n.height = 1 + std::max(a.height, b.height);
        n.front = a.front;
        n.back = b.back;
        n.summary = kernel_.combine(a.summary, b.summary);
    }
    handle make_branch(handle a, handle b) {
        const auto x = nodes_.acquire();
        auto& n = nodes_[x];
        n.left = a;
        n.right = b;
        n.action = kernel_.identity_action();
        repair_branch(x);
        return x;
    }
    void reverse_node(handle x) {
        if (!x)
            return;
        auto& n = nodes_[x];
        n.flags ^= reversed_flag;
        std::swap(n.left, n.right);
        std::swap(n.front, n.back);
        n.summary = kernel_.reversed(n.summary);
    }
    void apply_node(handle x, const action_type& action) {
        auto& n = nodes_[x];
        n.summary = kernel_.map(action, n.summary, n.length);
        if (n.flags & dirty_flag)
            n.action = kernel_.compose(action, n.action);
        else
            n.action = action;
        n.flags |= dirty_flag;
    }
    CANARD_ALWAYS_INLINE void push_branch(handle x) {
        auto& n = nodes_[x];
        assert(!n.leaf);
        if (n.flags & reversed_flag) {
            reverse_node(n.left);
            reverse_node(n.right);
        }
        if constexpr (has_lazy)
            if (n.flags & dirty_flag) {
                apply_node(n.left, n.action);
                apply_node(n.right, n.action);
            }
        n.flags = 0;
    }
    // Value actions and physical orientation are independent. Uniform
    // pointwise actions commute with reversal, so moving boundary fragments
    // needs current values but does not require forward physical order.
    void materialize_leaf_action(handle x) {
        if constexpr (has_lazy) {
            auto& n = nodes_[x];
            if (n.flags & dirty_flag) {
                kernel_.materialize(leaves_[n.leaf], n.length, n.action);
                n.flags &= ~dirty_flag;
            }
        }
    }
    void push_leaf(handle x) {
        materialize_leaf_action(x);
        auto& n = nodes_[x];
        if (n.flags & reversed_flag)
            kernel_.reverse_values(leaves_[n.leaf], 0, n.length);
        n.flags = 0;
    }
    std::pair<handle, handle> split_leaf(handle x, unsigned at) {
        push_leaf(x);
        const auto old_size = nodes_[x].length;
        const auto summaries =
            partition_summary(leaves_[nodes_[x].leaf], old_size, at, nodes_[x].summary);
        const auto y = new_leaf(); // retain no references across pool growth
        kernel_.copy_values(leaves_[nodes_[y].leaf], 0, leaves_[nodes_[x].leaf], at, old_size - at);
        nodes_[x].length = at;
        nodes_[y].length = old_size - at;
        repair_leaf(x, summaries.first);
        repair_leaf(y, summaries.second);
        return {x, y};
    }
    friend struct chunked_boundary_repair<chunked_engine>;
    handle concatenate(handle a, handle b) {
        return chunked_boundary_repair<chunked_engine>::concatenate(*this, a, b);
    }
    handle insert_impl(handle x, unsigned position, const value_type& value) {
        if (!x) {
            x = new_leaf();
            auto& n = nodes_[x];
            n.length = n.front = n.back = 1;
            leaves_[n.leaf].values[0] = kernel_.import_value(value);
            n.summary = kernel_.summarize(leaves_[n.leaf], 0, 1);
            return x;
        }
        if (is_leaf(x)) {
            if constexpr (has_lazy)
                materialize_leaf_action(x);
            else
                push_leaf(x);
            if (nodes_[x].length == B) {
                const auto [a, b] = split_leaf(x, B / 2);
                handle l = a, r = b;
                if (position <= B / 2)
                    l = insert_impl(a, position, value);
                else
                    r = insert_impl(b, position - B / 2, value);
                return make_branch(l, r);
            }
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            const auto word = kernel_.import_value(value);
            const bool flip = has_lazy && (n.flags & reversed_flag);
            const auto previous = flip ? kernel_.reversed(n.summary) : n.summary;
            kernel_.insert_value(leaf, flip ? n.length - position : position, n.length, word);
            ++n.length;
            n.front = n.back = std::min<unsigned>(n.length, minimum_occupancy);
            const auto next = kernel_.after_insert(leaf, n.length, previous, word);
            n.summary = flip ? kernel_.reversed(next) : next;
            return x;
        }
        push_branch(x);
        const unsigned n = length(nodes_[x].left);
        if (position < n) {
            const auto child = insert_impl(nodes_[x].left, position, value);
            nodes_[x].left = child;
        } else {
            const auto child = insert_impl(nodes_[x].right, position - n, value);
            nodes_[x].right = child;
        }
        topology_access access{*this};
        return topology{access}.rebalance(x);
    }
    handle erase_impl(handle x, unsigned position) {
        if (is_leaf(x)) {
            constexpr bool supports_pending_erase = has_lazy && requires(
                const leaf_type& leaf, const summary_type& summary, const action_type& action) {
                kernel_.after_erase_pending(leaf, 0u, summary, leaf.values[0], action);
            };
            if constexpr (has_lazy && !supports_pending_erase)
                materialize_leaf_action(x);
            else if constexpr (!has_lazy)
                push_leaf(x);
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            const bool flip = has_lazy && (n.flags & reversed_flag);
            if (flip)
                position = n.length - 1 - position;
            const auto previous = flip ? kernel_.reversed(n.summary) : n.summary;
            const auto old = leaf.values[position];
            kernel_.erase_value(leaf, position, n.length);
            --n.length;
            if (!n.length) {
                release_leaf(x);
                return 0;
            }
            n.front = n.back = std::min<unsigned>(n.length, minimum_occupancy);
            const auto next = [&] {
                if constexpr (supports_pending_erase)
                    if (n.flags & dirty_flag)
                        return kernel_.after_erase_pending(leaf, n.length, previous, old, n.action);
                return kernel_.after_erase(leaf, n.length, previous, old);
            }();
            n.summary = flip ? kernel_.reversed(next) : next;
            return x;
        }
        push_branch(x);
        const unsigned n = length(nodes_[x].left);
        if (position < n) {
            const auto child = erase_impl(nodes_[x].left, position);
            nodes_[x].left = child;
        } else {
            const auto child = erase_impl(nodes_[x].right, position - n);
            nodes_[x].right = child;
        }
        const auto a = nodes_[x].left, b = nodes_[x].right;
        if (!a || !b) {
            nodes_.release(x);
            return a ? a : b;
        }
        if (nodes_[a].back < minimum_occupancy || nodes_[b].front < minimum_occupancy) {
            nodes_.release(x);
            return concatenate(a, b);
        }
        topology_access access{*this};
        topology tree{access};
        if (height(a) > height(b) + height_tolerance + 1 ||
            height(b) > height(a) + height_tolerance + 1) {
            nodes_.release(x);
            return tree.join(a, b);
        }
        return tree.rebalance(x);
    }
    struct plain_reader {
        using summary_type = summary_type_t<Kernel>;
        static constexpr bool has_frames = false;
        const chunked_engine& owner;

        [[nodiscard]] bool is_leaf(handle x) const noexcept {
            return owner.is_leaf(x);
        }
        [[nodiscard]] unsigned length(handle x) const noexcept {
            return owner.length(x);
        }
        [[nodiscard]] summary_type identity() const {
            return owner.kernel_.identity();
        }
        [[nodiscard]] summary_type combine(const summary_type& a, const summary_type& b) const {
            return owner.kernel_.combine(a, b);
        }
        [[nodiscard]] summary_type whole(handle x, bool flip) const {
            const auto& summary = owner.nodes_[x].summary;
            return flip ? owner.kernel_.reversed(summary) : summary;
        }
        [[nodiscard]] oriented_rope_children children(handle x, bool flip) const noexcept {
            const auto& n = owner.nodes_[x];
            const auto a = flip ? n.right : n.left;
            const auto b = flip ? n.left : n.right;
            return {a, b, owner.length(a), flip != bool(n.flags & reversed_flag)};
        }
        [[nodiscard]] summary_type
        leaf_fold(handle x, unsigned first, unsigned last, bool flip) const {
            const auto& n = owner.nodes_[x];
            const auto& leaf = owner.leaves_[n.leaf];
            flip ^= bool(n.flags & reversed_flag);
            if (flip)
                return owner.kernel_.reversed(
                    owner.kernel_.summarize(leaf, n.length - last, n.length - first));
            return owner.kernel_.summarize(leaf, first, last);
        }
    };

    struct lazy_reader : plain_reader {
        static constexpr bool has_frames = true;
        static constexpr unsigned path_capacity = chunked_engine::path_capacity;
        [[nodiscard]] bool needs_lift(handle x) const noexcept {
            return this->owner.nodes_[x].flags & dirty_flag;
        }
        [[nodiscard]] summary_type lift(handle x, const summary_type& value, unsigned count) const {
            return this->owner.kernel_.map(this->owner.nodes_[x].action, value, count);
        }
        [[nodiscard]] summary_type
        leaf_fold(handle x, unsigned first, unsigned last, bool flip) const {
            auto value = plain_reader::leaf_fold(x, first, last, flip);
            return needs_lift(x) ? lift(x, value, last - first) : value;
        }
    };

    void apply_impl(handle x, unsigned l, unsigned r, const prepared_action& prepared) {
        if (l == 0 && r == length(x)) {
            apply_node(x, kernel_.action_of(prepared));
            return;
        }
        if (is_leaf(x)) {
            auto& n = nodes_[x];
            const bool flip = n.flags & reversed_flag;
            if (flip) {
                const auto first = n.length - r;
                r = n.length - l;
                l = first;
            }
            const auto previous = flip ? kernel_.reversed(n.summary) : n.summary;
            const auto update = [&] {
                if constexpr (kernel::carried_leaf_update<Kernel>) {
                    if (n.flags & dirty_flag) {
                        const auto next = kernel_.update_leaf_carried(
                            leaves_[n.leaf], n.length, l, r, prepared, n.action, previous);
                        n.flags &= ~dirty_flag;
                        return next;
                    }
                }
                materialize_leaf_action(x);
                return kernel_.update_leaf(leaves_[n.leaf], n.length, l, r, prepared, previous);
            }();
            n.summary = flip ? kernel_.reversed(update) : update;
            return;
        }
        push_branch(x);
        const auto a = nodes_[x].left, b = nodes_[x].right, n = length(a);
        if (r <= n)
            apply_impl(a, l, r, prepared);
        else if (l >= n)
            apply_impl(b, l - n, r - n, prepared);
        else {
            apply_impl(a, l, n, prepared);
            apply_impl(b, 0, r - n, prepared);
        }
        repair_branch(x);
    }
    void set_impl(handle x, unsigned position, const value_type& value) {
        if (is_leaf(x)) {
            if constexpr (has_lazy)
                materialize_leaf_action(x);
            else
                push_leaf(x);
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            const bool flip = has_lazy && (n.flags & reversed_flag);
            if (flip)
                position = n.length - 1 - position;
            const auto previous = flip ? kernel_.reversed(n.summary) : n.summary;
            const auto old = leaf.values[position], word = kernel_.import_value(value);
            leaf.values[position] = word;
            const auto next = kernel_.after_set(leaf, n.length, previous, old, word);
            n.summary = flip ? kernel_.reversed(next) : next;
            return;
        }
        push_branch(x);
        const auto a = nodes_[x].left, b = nodes_[x].right, n = length(a);
        if (position < n)
            set_impl(a, position, value);
        else
            set_impl(b, position - n, value);
        repair_branch(x);
    }
    // Reverse an interval using the two captured boundary paths. Exchange only the pieces OUTSIDE
    // the requested interval between the two boundary blocks (in reverse order), then reverse the
    // enclosing interval of whole blocks. No fragmentation or occupancy repair is necessary when
    // both replacement sizes fit.
    CANARD_NOINLINE handle
    reverse_blocks(handle x, handle a, handle b, unsigned l, unsigned r, unsigned left_length) {
        std::array<handle, path_capacity> apath, bpath;
        unsigned an = 0, bn = 0;
        handle u = a, v = b;
        unsigned i = l, j = r - left_length - 1;
        // Independent spines: expose both incrementally rather than completing
        // one dependent descent before starting the other. No lookahead beyond
        // the current operation, speculative mutation, or cached node pointers.
        const auto descend = [&](handle& at, unsigned& position, auto& path, unsigned& count) {
            push_branch(at);
            path[count++] = at;
            const auto left = nodes_[at].left, right = nodes_[at].right;
            const unsigned n = nodes_[left].length;
            const bool go_right = position >= n;
            at = go_right ? right : left;
            position -= go_right ? n : 0;
        };
        while (!is_leaf(u) && !is_leaf(v)) {
            descend(u, i, apath, an);
            descend(v, j, bpath, bn);
        }
        while (!is_leaf(u))
            descend(u, i, apath, an);
        while (!is_leaf(v))
            descend(v, j, bpath, bn);
        ++j;
        const auto nu = nodes_[u].length, nv = nodes_[v].length;
        const auto tail = nv - j, newu = tail + nu - i, newv = j + i;
        topology_access access{*this};
        topology tree{access};
        if (newu < minimum_occupancy || newv < minimum_occupancy || newu > B || newv > B) {
            const auto left_parts = i ? split_leaf(u, i) : std::pair<handle, handle>{0, u};
            const auto right_parts = j < nv ? split_leaf(v, j) : std::pair<handle, handle>{v, 0};
            const auto [first, left] =
                tree.split_from_leaf(u, left_parts.first, left_parts.second, {apath.data(), an});
            const auto [right, last] =
                tree.split_from_leaf(v, right_parts.first, right_parts.second, {bpath.data(), bn});
            reverse_node(left);
            reverse_node(right);
            const auto middle = tree.join(right, left, x);
            return concatenate(concatenate(first, middle), last);
        }
        if (i || tail) {
            if constexpr (requires(leaf_type& leaf) {
                              kernel_.exchange_reversed_outer_parts_oriented(
                                  leaf, 0u, 0u, false, leaf, 0u, 0u, false);
                          }) {
                materialize_leaf_action(u);
                materialize_leaf_action(v);
                auto& left = leaves_[nodes_[u].leaf];
                auto& right = leaves_[nodes_[v].leaf];
                const bool left_flip = nodes_[u].flags & reversed_flag;
                const bool right_flip = nodes_[v].flags & reversed_flag;
                const auto parts = [&](const leaf_type& leaf, unsigned n, unsigned at,
                                       const summary_type& total, bool flip) {
                    if (!flip)
                        return partition_summary(leaf, n, at, total);
                    const auto [a, b] =
                        partition_summary(leaf, n, n - at, kernel_.reversed(total));
                    return std::pair{kernel_.reversed(b), kernel_.reversed(a)};
                };
                const auto left_summaries = parts(left, nu, i, nodes_[u].summary, left_flip);
                const auto right_summaries = parts(right, nv, j, nodes_[v].summary, right_flip);
                const auto new_left_summary =
                    kernel_.combine(kernel_.reversed(right_summaries.second), left_summaries.second);
                const auto new_right_summary =
                    kernel_.combine(right_summaries.first, kernel_.reversed(left_summaries.first));
                kernel_.exchange_reversed_outer_parts_oriented(left, nu, i, left_flip,
                                                               right, nv, j, right_flip);
                nodes_[u].length = newu;
                nodes_[v].length = newv;
                repair_leaf(u, new_left_summary);
                repair_leaf(v, new_right_summary);
            } else {
                push_leaf(u);
                push_leaf(v);
                auto& left = leaves_[nodes_[u].leaf];
                auto& right = leaves_[nodes_[v].leaf];
                const auto left_summaries = partition_summary(left, nu, i, nodes_[u].summary);
                const auto right_summaries = partition_summary(right, nv, j, nodes_[v].summary);
                const auto new_left_summary =
                    kernel_.combine(kernel_.reversed(right_summaries.second), left_summaries.second);
                const auto new_right_summary =
                    kernel_.combine(right_summaries.first, kernel_.reversed(left_summaries.first));
                kernel_.exchange_reversed_outer_parts(left, nu, i, right, nv, j);
                nodes_[u].length = newu;
                nodes_[v].length = newv;
                repair_leaf(u, new_left_summary);
                repair_leaf(v, new_right_summary);
            }
        }
        // Refresh only the retained initial spines; reconstructed ancestors are
        // repaired by join. Do not repair both complete paths first.
        const auto [first, left, spare_left] =
            tree.template split_from_leaf_retained<true>(u, 0, u, {apath.data(), an});
        const auto [right, last, spare_right] =
            tree.template split_from_leaf_retained<true>(v, v, 0, {bpath.data(), bn});
        reverse_node(left);
        reverse_node(right);
        const auto middle = tree.join(right, left, x);
        return tree.join(tree.join(first, middle, spare_left), last, spare_right);
    }
    handle reverse_impl(handle x, unsigned l, unsigned r) {
        if (l == 0 && r == length(x)) {
            reverse_node(x);
            return x;
        }
        if (is_leaf(x)) {
            if constexpr (requires { Kernel::reversal_invariant_summary; }) {
                static_assert(Kernel::reversal_invariant_summary);
                // A permutation-invariant summary remains valid even with an
                // unmaterialized uniform pointwise action. Preserve both flags.
                auto& n = nodes_[x];
                if (n.flags & reversed_flag) {
                    const auto first = n.length - r;
                    r = n.length - l;
                    l = first;
                }
                kernel_.reverse_values(leaves_[n.leaf], l, r);
                return x;
            }
            push_leaf(x);
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            kernel_.reverse_values(leaf, l, r);
            if constexpr (requires { kernel_.after_reverse(leaf, n.length, l, r, n.summary); })
                repair_leaf(x, kernel_.after_reverse(leaf, n.length, l, r, n.summary));
            else
                repair_leaf(x);
            return x;
        }
        push_branch(x);
        const auto a = nodes_[x].left, b = nodes_[x].right, n = length(a);
        topology_access access{*this};
        topology tree{access};
        if (r <= n || l >= n) {
            if (r <= n) {
                const auto child = reverse_impl(a, l, r);
                nodes_[x].left = child;
            } else {
                const auto child = reverse_impl(b, l - n, r - n);
                nodes_[x].right = child;
            }
            const auto left = nodes_[x].left, right = nodes_[x].right;
            if (nodes_[left].back < minimum_occupancy || nodes_[right].front < minimum_occupancy) {
                nodes_.release(x);
                return concatenate(left, right);
            }
            if (height(left) > height(right) + height_tolerance + 1 ||
                height(right) > height(left) + height_tolerance + 1) {
                nodes_.release(x);
                return tree.join(left, right);
            }
            return tree.rebalance(x);
        }
        return reverse_blocks(x, a, b, l, r, n);
    }
    template <typename Output>
    void materialize_impl(handle x, const action_type& outer, bool flip, Output& out) const {
        const auto& n = nodes_[x];
        const auto action = [&] {
            if constexpr (has_lazy) {
                if (n.flags & dirty_flag)
                    return kernel_.compose(outer, n.action);
            }
            return outer;
        }();
        const bool reverse = flip != bool(n.flags & reversed_flag);
        if (n.leaf) {
            for (unsigned i = 0; i < n.length; ++i) {
                const unsigned j = reverse ? n.length - 1 - i : i;
                const auto one = kernel_.summarize(leaves_[n.leaf], j, j + 1);
                *out++ = kernel_.export_summary(kernel_.map(action, one, 1));
            }
        } else {
            materialize_impl(flip ? n.right : n.left, action, reverse, out);
            materialize_impl(flip ? n.left : n.right, action, reverse, out);
        }
    }
    bool validate_impl(handle x, bool first, bool last, bool flip) const {
        const auto& n = nodes_[x];
        if (n.leaf)
            return n.length > 0 && n.length <= B && n.height == 1 &&
                   n.front == std::min<unsigned>(n.length, minimum_occupancy) &&
                   n.back == std::min<unsigned>(n.length, minimum_occupancy) &&
                   (first || last || n.length >= minimum_occupancy);
        if (!n.left || !n.right)
            return false;
        const auto& a = nodes_[n.left];
        const auto& b = nodes_[n.right];
        const bool reversed = n.flags & reversed_flag;
        if (n.length != a.length + b.length || n.height != 1 + std::max(a.height, b.height) ||
            std::abs(int(a.height) - int(b.height)) > int(height_tolerance) ||
            n.front != (reversed ? a.back : a.front) || n.back != (reversed ? b.front : b.back))
            return false;
        const bool child_flip = flip != reversed;
        return validate_impl(flip ? n.right : n.left, first, false, child_flip) &&
               validate_impl(flip ? n.left : n.right, false, last, child_flip);
    }
    handle build_balanced(const std::vector<handle>& leaves, unsigned l, unsigned r) {
        if (r - l == 1)
            return leaves[l];
        const auto m = (l + r) / 2;
        const auto a = build_balanced(leaves, l, m), b = build_balanced(leaves, m, r);
        return make_branch(a, b);
    }

  public:
    explicit chunked_engine(Kernel kernel) : kernel_(std::move(kernel)) {
        if constexpr (requires { Kernel::max_size; })
            static_assert(Configuration::max_size <= Kernel::max_size);
    }
    template <std::ranges::input_range R>
    chunked_engine(R&& values, Kernel kernel) : chunked_engine(std::move(kernel)) {
        std::vector<handle> leaves;
        if constexpr (std::ranges::sized_range<R>) {
            assert(std::ranges::size(values) <= Configuration::max_size);
            const auto count = (std::ranges::size(values) + B - 1) / B;
            nodes_.reserve(2 * count + 32);
            leaves_.reserve(count + 16);
            leaves.reserve(count);
        }
        handle current = 0;
        for (auto&& value : values) {
            if (!current || nodes_[current].length == B) {
                if (current)
                    repair_leaf(current);
                current = new_leaf();
                leaves.push_back(current);
            }
            auto& n = nodes_[current];
            leaves_[n.leaf].values[n.length++] = kernel_.import_value(value);
        }
        if (current) {
            repair_leaf(current);
            root_ = build_balanced(leaves, 0, leaves.size());
        }
        assert(size() <= Configuration::max_size);
    }
    chunked_engine(const chunked_engine&) = default;
    chunked_engine& operator=(const chunked_engine& other) {
        if (this != &other) {
            chunked_engine next{other};
            swap(next);
        }
        return *this;
    }
    chunked_engine(chunked_engine&& other) noexcept(std::is_nothrow_copy_constructible_v<Kernel>)
        : kernel_(other.kernel_), nodes_(std::move(other.nodes_)),
          leaves_(std::move(other.leaves_)), root_(std::exchange(other.root_, 0)) {}
    chunked_engine& operator=(chunked_engine&& other) {
        if (this != &other) {
            kernel_ = other.kernel_;
            nodes_ = std::move(other.nodes_);
            leaves_ = std::move(other.leaves_);
            root_ = std::exchange(other.root_, 0);
        }
        return *this;
    }
    void swap(chunked_engine& other) noexcept
        requires(std::is_nothrow_swappable_v<Kernel>)
    {
        using std::swap;
        swap(kernel_, other.kernel_);
        nodes_.swap(other.nodes_);
        leaves_.swap(other.leaves_);
        swap(root_, other.root_);
    }
    [[nodiscard]] size_type size() const noexcept {
        return length(root_);
    }
    [[nodiscard]] bool empty() const noexcept {
        return !root_;
    }
    [[nodiscard]] unsigned height() const noexcept {
        return height(root_);
    }
    [[nodiscard]] std::size_t leaf_count() const noexcept {
        return leaves_.live_slots();
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return nodes_.allocated_bytes() + leaves_.allocated_bytes();
    }
    [[nodiscard]] value_type fold(size_type first, size_type last) const {
        assert(first <= last && last <= size());
        if (first == last)
            return kernel_.export_summary(kernel_.identity());
        if constexpr (has_lazy)
            return kernel_.export_summary(
                rope_boundary_fold{lazy_reader{{*this}}}(root_, first, last));
        else
            return kernel_.export_summary(
                rope_boundary_fold{plain_reader{*this}}(root_, first, last));
    }
    [[nodiscard]] value_type all_fold() const {
        return kernel_.export_summary(aggregate(root_));
    }
    [[nodiscard]] value_type get(size_type position) const {
        return fold(position, position + 1);
    }
    void insert(size_type position, const value_type& value) {
        assert(position <= size() && size() < Configuration::max_size);
        root_ = insert_impl(root_, position, value);
    }
    void erase(size_type position) {
        assert(position < size());
        root_ = erase_impl(root_, position);
    }
    void set(size_type position, const value_type& value) noexcept
        requires(nothrow_mutation)
    {
        assert(position < size());
        set_impl(root_, position, value);
    }
    void apply(size_type first, size_type last, const update_type& update) noexcept
        requires(has_lazy && nothrow_mutation)
    {
        assert(first <= last && last <= size());
        if (first != last)
            apply_impl(root_, first, last, kernel_.prepare(update));
    }
    void reverse(size_type first, size_type last) {
        assert(first <= last && last <= size());
        if (last - first > 1)
            root_ = reverse_impl(root_, first, last);
    }
    template <typename Output> Output materialize(Output out) const {
        if (root_)
            materialize_impl(root_, kernel_.identity_action(), false, out);
        return out;
    }
    [[nodiscard]] std::vector<value_type> snapshot() const {
        std::vector<value_type> result;
        result.reserve(size());
        materialize(std::back_inserter(result));
        return result;
    }
    void reserve_slots(std::size_t node_slots, std::size_t leaf_slots) {
        if (node_slots >= (std::uint64_t{1} << 32) || leaf_slots >= (std::uint64_t{1} << 32))
            throw std::length_error("canard: slot handle range exceeded");
        nodes_.reserve(node_slots);
        leaves_.reserve(leaf_slots);
    }
    void clear() requires std::is_nothrow_swappable_v<Kernel> {
        chunked_engine next{kernel_};
        swap(next); // Releases retained payload objects after successful empty construction.
    }
    template <std::ranges::input_range R>
    void rebuild(R&& values) requires std::is_nothrow_swappable_v<Kernel> {
        chunked_engine next{std::forward<R>(values), kernel_};
        swap(next);
    }
    [[nodiscard]] bool check_invariants() const {
        return !root_ || validate_impl(root_, true, true, false);
    }
};
} // namespace canard::detail
// ===== include/canard/range/maintained_reference.hpp =====
#include <cstddef>
#include <concepts>
#include <format>
#include <string_view>
#include <utility>

namespace canard {

// A copied proxy copies the handle. Assigning a proxy writes a VALUE, never
// silently rebinds a handle. Snapshot the RHS before starting maintenance.
template <typename Owner> class point_reference {
    Owner* owner_;
    std::size_t position_;

  public:
    using value_type = value_type_t<Owner>;
    point_reference(Owner& owner, std::size_t position) noexcept
        : owner_(&owner), position_(position) {}
    point_reference(const point_reference&) noexcept = default;
    [[nodiscard]] value_type value() const {
        return owner_->get(position_);
    }
    operator value_type() const {
        return value();
    }
    const point_reference& operator=(const value_type& replacement) const noexcept(noexcept(owner_->set(position_, replacement)))
        requires requires(Owner& owner, const value_type& value) { owner.set(std::size_t{}, value); }
    {
        owner_->set(position_, replacement);
        return *this;
    }
    const point_reference& operator=(const point_reference& other) const
        requires requires(Owner& owner, const value_type& value) { owner.set(std::size_t{}, value); }
    {
        const auto replacement = other.value();
        owner_->set(position_, replacement);
        return *this;
    }
    template <typename Other>
    const point_reference& operator=(const point_reference<Other>& other) const
        requires(std::same_as<value_type, value_type_t<Other>> &&
                 requires(Owner& owner, const value_type& value) { owner.set(std::size_t{}, value); })
    {
        const auto replacement = other.value();
        owner_->set(position_, replacement);
        return *this;
    }
    template <typename Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); }
    {
        owner_->apply(position_, position_ + 1, action);
    }
};

template <typename Owner> class interval_reference {
    Owner* owner_;
    std::size_t first_, last_;

  public:
    interval_reference(Owner& owner, std::size_t first, std::size_t last) noexcept
        : owner_(&owner), first_(first), last_(last) {}
    [[nodiscard]] auto fold() const {
        return owner_->fold(first_, last_);
    }
    template <typename Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); }
    {
        owner_->apply(first_, last_, action);
    }
    template <typename Value>
    void assign(const Value& value) const
        noexcept(noexcept(owner_->assign(first_, last_, value)))
        requires requires(Owner& owner) { owner.assign(first_, last_, value); } {
        owner_->assign(first_, last_, value);
    }
    void reverse() const noexcept(noexcept(owner_->reverse(first_, last_)))
        requires requires(Owner& owner) { owner.reverse(first_, last_); } {
        owner_->reverse(first_, last_);
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return last_ - first_;
    }
};

struct wide_tree_description {
    std::size_t size;
    unsigned fanout;
    unsigned height;
    std::size_t storage_bytes;
    bool lazy;
};

} // namespace canard

// One read per format operation. Numeric presentation comes from the value's
// own formatter. Public facades include this header, not an optional plugin.
template <typename Owner>
    requires std::formattable<canard::value_type_t<Owner>, char>
struct std::formatter<canard::point_reference<Owner>, char>
    : std::formatter<canard::value_type_t<Owner>, char> {
    template <typename Context>
    auto format(const canard::point_reference<Owner>& reference, Context& context) const {
        const auto snapshot = reference.value();
        return std::formatter<canard::value_type_t<Owner>, char>::format(snapshot, context);
    }
};

template <> struct std::formatter<canard::wide_tree_description, char> {
    constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }
    template <typename Context>
    auto format(canard::wide_tree_description x, Context& context) const {
        return std::format_to(context.out(),
                              "{}(size={}, fanout={}, height={}, storage={} bytes)",
                              x.lazy ? "wide_lazy_segment_tree" : "wide_segment_tree",
                              x.size,
                              x.fanout,
                              x.height,
                              x.storage_bytes);
    }
};
#include <span>
// ===== include/canard/algebra/modular.hpp =====
// ===== include/canard/format/affine_map.hpp =====
#include <format>
#include <string_view>
// The public scalar's formatter is exposed by its own header. Arithmetic-only
// internal kernels do not include this header or formatting/transport support.
template <std::uint32_t P>
struct std::formatter<canard::algebra::affine_map<P>, char>
    : std::formatter<std::string_view, char> {
    template <typename Context>
    auto format(canard::algebra::affine_map<P> f, Context& context) const {
        const auto text = std::format("({}x + {})", f.multiplier, f.translation);
        return std::formatter<std::string_view, char>::format(text, context);
    }
};
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
namespace canard {
struct chunked_sequence_description {
    std::size_t size;
    unsigned leaf_capacity;
    unsigned height;
    std::size_t leaf_count;
    std::size_t allocated_bytes;
    bool lazy = true;
};
template <algebra::monoid Monoid,
          typename Action,
          typename Configuration = sequence::configuration<>>
    requires(algebra::action_for<Action, Monoid> &&
             sequence::supported_configuration<Configuration, Monoid, Action> &&
             kernel::retained_sequence_kernel<sequence::kernel_for_t<Configuration, Monoid, Action>>)
class chunked_lazy_sequence
    : private detail::chunked_engine<sequence::kernel_for_t<Configuration, Monoid, Action>,
                                    Configuration> {
    using kernel = sequence::kernel_for_t<Configuration, Monoid, Action>;
    using base = detail::chunked_engine<kernel, Configuration>;

  public:
    using value_type = value_type_t<base>;
    using tag_type = tag_type_t<Action>;
    using configuration_type = Configuration;
    using monoid_type = Monoid;
    using action_policy_type = Action;
    using element_type = value_type;
    using aggregate_type = value_type;
    using update_type = tag_type;
    using size_type = size_type_t<base>;
    static constexpr bool nothrow_mutation = base::nothrow_mutation;
    using base::size;
    using base::empty;
    [[nodiscard]] unsigned height() const noexcept {
        return base::height();
    }
    using base::leaf_count;
    using base::allocated_bytes;
    using base::fold;
    using base::all_fold;
    using base::get;
    using base::insert;
    using base::erase;
    using base::set;
    using base::apply;
    using base::reverse;
    using base::materialize;
    using base::snapshot;
    using base::check_invariants;
    using base::reserve_slots;
    using base::clear;
    using base::rebuild;

    void swap(chunked_lazy_sequence& other) noexcept
        requires requires(base& value) { value.swap(value); } { base::swap(other); }
    friend void swap(chunked_lazy_sequence& left, chunked_lazy_sequence& right)
        noexcept(noexcept(left.swap(right))) requires requires { left.swap(right); } { left.swap(right); }

    template <std::ranges::input_range R>
    explicit chunked_lazy_sequence(R&& values,
                                   Monoid monoid = {},
                                   Action action = {},
                                   Configuration = {})
        : base(std::forward<R>(values), kernel{std::move(monoid), std::move(action)}) {}
    explicit chunked_lazy_sequence(Monoid monoid = {}, Action action = {}, Configuration = {})
        : base(kernel{std::move(monoid), std::move(action)}) {}
    [[nodiscard]] point_reference<chunked_lazy_sequence> operator[](size_type position) & noexcept {
        return {*this, position};
    }
    [[nodiscard]] value_type operator[](size_type position) const & {
        return this->get(position);
    }
    void operator[](size_type) && = delete;
    void operator[](size_type) const && = delete;
    [[nodiscard]] interval_reference<chunked_lazy_sequence> operator[](size_type first, size_type last) & noexcept {
        return {*this, first, last};
    }
    [[nodiscard]] interval_reference<const chunked_lazy_sequence> operator[](size_type first, size_type last) const & noexcept {
        return {*this, first, last};
    }
    void operator[](size_type, size_type) && = delete;
    void operator[](size_type, size_type) const && = delete;
    [[nodiscard]] chunked_sequence_description description() const noexcept {
        return {this->size(),
                Configuration::leaf_capacity,
                this->height(),
                this->leaf_count(),
                this->allocated_bytes()};
    }
};
template <std::ranges::input_range R, algebra::monoid M, typename A, typename C>
chunked_lazy_sequence(R&&, M, A, C) -> chunked_lazy_sequence<M, A, C>;
template <std::ranges::input_range R, algebra::monoid M, typename A>
chunked_lazy_sequence(R&&, M, A) -> chunked_lazy_sequence<M, A>;
} // namespace canard

template <> struct std::formatter<canard::chunked_sequence_description, char> {
    constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }
    template <typename Context>
    auto format(canard::chunked_sequence_description x, Context& context) const {
        return std::format_to(context.out(),
                              "{}(size={}, leaf_capacity={}, height={}, "
                              "leaves={}, allocated={} bytes)",
                              x.lazy ? "chunked_lazy_sequence" : "chunked_sequence",
                              x.size,
                              x.leaf_capacity,
                              x.height,
                              x.leaf_count,
                              x.allocated_bytes);
    }
};

template <canard::algebra::monoid M, typename A, typename C>
struct std::formatter<canard::chunked_lazy_sequence<M, A, C>, char>
    : canard::format::description_formatter<canard::chunked_lazy_sequence<M, A, C>> {};
// ===== include/canard/sequence/avx2.hpp =====
// ===== include/canard/kernel/sequence_affine_sum_avx2.hpp =====
#include <cstring>
// ===== include/canard/kernel/sequence_leaf_avx2.hpp =====
// ===== include/canard/simd/sequence_avx2.hpp =====
// ===== include/canard/simd/avx2.hpp =====
// A deliberately small, concrete AVX2 vocabulary; not a std::simd emulation.
// No dispatch, allocation, bounds checks, or implicit scalar conversions.
#include <bit>
#include <cstdint>
#include <type_traits>
#include <immintrin.h>
#ifndef __AVX2__
#error "This backend requires AVX2 (e.g. -march=znver3 or -march=native)."
#endif
namespace canard::simd {
struct mask32x8;
namespace detail {
struct native_access;
}
class u32x8 {
    __m256i bits_;
    explicit u32x8(__m256i bits) noexcept : bits_(bits) {}
    friend struct detail::native_access;

  public:
    using value_type = std::uint32_t;
    using mask_type = mask32x8;
    static constexpr int size() noexcept {
        return 8;
    }
    static constexpr int alignment = 32;
    // Default construction, like an intrinsic register, leaves lanes uninitialized.
    u32x8() = default;
    explicit u32x8(value_type value) noexcept
        : bits_(_mm256_set1_epi32(std::bit_cast<int>(value))) {}
};
// A broadcast invariant encoded in the type, not re-checked at runtime.
// Fixed-factor arithmetic can exploit identical lanes without extra shuffles.
class broadcast_u32x8 {
    u32x8 value_;

  public:
    broadcast_u32x8() = default;
    explicit broadcast_u32x8(std::uint32_t value) noexcept : value_(value) {}
    [[nodiscard]] u32x8 vector() const noexcept {
        return value_;
    }
};
struct mask32x8 {
  private:
    __m256i bits_;
    explicit mask32x8(__m256i bits) noexcept : bits_(bits) {}
    friend struct detail::native_access;

  public:
    // Each lane is either all-zero or all-one, never an arbitrary byte mask.
    mask32x8() = default;
};
namespace detail {
struct native_access {
    static __m256i get(u32x8 v) noexcept {
        return v.bits_;
    }
    static __m256i get(mask32x8 v) noexcept {
        return v.bits_;
    }
    static u32x8 words(__m256i v) noexcept {
        return u32x8{v};
    }
    static mask32x8 mask(__m256i v) noexcept {
        return mask32x8{v};
    }
};
} // namespace detail
static_assert(sizeof(u32x8) == 32 && alignof(u32x8) == 32);
static_assert(sizeof(mask32x8) == 32);
static_assert(std::is_trivially_copyable_v<u32x8>);

// Full-width operations. Alignment and eight readable/writable words are
// preconditions. select() does NOT make an earlier memory access conditional.
[[nodiscard]] inline u32x8 load_aligned(const std::uint32_t* address) noexcept {
    return detail::native_access::words(
        _mm256_load_si256(reinterpret_cast<const __m256i*>(address)));
}
inline void store_aligned(std::uint32_t* address, u32x8 value) noexcept {
    _mm256_store_si256(reinterpret_cast<__m256i*>(address), detail::native_access::get(value));
}
[[nodiscard]] inline u32x8 select(mask32x8 mask, u32x8 when_true, u32x8 when_false) noexcept {
    using access = detail::native_access;
    return access::words(
        _mm256_blendv_epi8(access::get(when_false), access::get(when_true), access::get(mask)));
}
// Lane k is selected iff k >= first. Negative first selects every lane;
// first >= 8 selects none. Precondition: first > INT_MIN.
[[nodiscard]] inline mask32x8 lanes_from(int first) noexcept {
    return detail::native_access::mask(_mm256_cmpgt_epi32(_mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7),
                                                          _mm256_set1_epi32(first - 1)));
}
[[nodiscard]] inline u32x8 operator+(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_add_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 operator-(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_sub_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 min(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_min_epu32(access::get(a), access::get(b)));
}
// Explicitly distinguish multiplication modulo 2^32 from the high half of
// the widening product. Neither of these functions is modular-field multiply.
[[nodiscard]] inline u32x8 multiply_low(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_mullo_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 multiply_high(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    const auto av = access::get(a), bv = access::get(b);
    const auto even = _mm256_mul_epu32(av, bv);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(av, 32), _mm256_srli_epi64(bv, 32));
    return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa));
}
// Same high-half multiplication with the second operand known to be a
// broadcast. AVX2's odd-lane stream can reuse it without a right shift.
[[nodiscard]] inline u32x8 multiply_high(u32x8 a, broadcast_u32x8 b) noexcept {
    using access = detail::native_access;
    const auto av = access::get(a), bv = access::get(b.vector());
    const auto even = _mm256_mul_epu32(av, bv);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(av, 32), bv);
    return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa));
}
} // namespace canard::simd
#include <algorithm>
namespace canard::simd {
// Copies [first,last) in reverse order to a disjoint destination of equal length.
// Unaligned addresses are supported. No padding is required: vector accesses
// stay entirely inside the ranges, with a scalar tail of at most seven words.
inline void reverse_copy_u32(const std::uint32_t* first,
                             const std::uint32_t* last,
                             std::uint32_t* output) noexcept {
    const auto order = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    while (last - first >= 8) {
        last -= 8;
        const auto words = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(last));
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(output),
                           _mm256_permutevar8x32_epi32(words, order));
        output += 8;
    }
    while (last != first)
        *output++ = *--last;
}

inline void reverse_u32(std::uint32_t* first, std::uint32_t* last) noexcept {
    const auto order = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    while (last - first >= 16) {
        last -= 8;
        const auto a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first));
        const auto b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(last));
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(first),
                            _mm256_permutevar8x32_epi32(b, order));
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(last),
                            _mm256_permutevar8x32_epi32(a, order));
        first += 8;
    }
    std::reverse(first, last);
}
} // namespace canard::simd
#include <cstdint>
#include <cstring>
namespace canard::kernel {
// Ordered 32-bit leaf movement, independent of arithmetic and lazy actions.
template <unsigned Extent>
struct sequence_leaf_u32_avx2 : sequence_leaf_operations<std::uint32_t, Extent> {
    using base = sequence_leaf_operations<std::uint32_t, Extent>;
    using leaf_type = leaf_type_t<base>;
    // Same ordered movement contract as the scalar leaf operation. Save the
    // first prefix already reversed, vector-copy the other outer fragment,
    // and use memmove only for the potentially overlapping interior shift.
    static void exchange_reversed_outer_parts(leaf_type& left,
                                              unsigned left_size,
                                              unsigned left_cut,
                                              leaf_type& right,
                                              unsigned right_size,
                                              unsigned right_cut) noexcept {
        const unsigned tail = right_size - right_cut;
        alignas(32) std::uint32_t saved[Extent];
        simd::reverse_copy_u32(left.values.data(), left.values.data() + left_cut, saved);
        if (tail != left_cut)
            std::memmove(left.values.data() + tail,
                         left.values.data() + left_cut,
                         (left_size - left_cut) * sizeof(std::uint32_t));
        simd::reverse_copy_u32(right.values.data() + right_cut,
                              right.values.data() + right_size,
                              left.values.data());
        std::memcpy(right.values.data() + right_cut, saved, left_cut * sizeof(std::uint32_t));
    }
    static void exchange_reversed_outer_parts_oriented(
        leaf_type& left, unsigned left_size, unsigned left_cut, bool left_reversed,
        leaf_type& right, unsigned right_size, unsigned right_cut, bool right_reversed) noexcept {
        const unsigned tail = right_size - right_cut;
        const unsigned inside = left_size - left_cut;
        alignas(32) std::uint32_t saved[Extent];
        const auto copy_fragment = [&](const std::uint32_t* src, unsigned count, std::uint32_t* dst) {
            if (left_reversed == right_reversed)
                simd::reverse_copy_u32(src, src + count, dst);
            else std::memcpy(dst, src, count * sizeof(std::uint32_t));
        };
        copy_fragment(left.values.data() + (left_reversed ? inside : 0), left_cut, saved);
        if (!left_reversed && tail != left_cut)
            std::memmove(left.values.data() + tail, left.values.data() + left_cut,
                         inside * sizeof(std::uint32_t));
        copy_fragment(right.values.data() + (right_reversed ? 0 : right_cut), tail,
                      left.values.data() + (left_reversed ? inside : 0));
        if (right_reversed && left_cut != tail)
            std::memmove(right.values.data() + left_cut, right.values.data() + tail,
                         right_cut * sizeof(std::uint32_t));
        std::memcpy(right.values.data() + (right_reversed ? 0 : right_cut), saved,
                    left_cut * sizeof(std::uint32_t));
    }
    static void reverse_values(leaf_type& leaf, unsigned first, unsigned last) noexcept {
        simd::reverse_u32(leaf.values.data() + first, leaf.values.data() + last);
    }
};
} // namespace canard::kernel
// ===== include/canard/kernel/affine_sum_arithmetic_avx2.hpp =====
// ===== include/canard/algebra/affine_sum_types.hpp =====
#include <array>
#include <cstddef>
#include <cstdint>
namespace canard::algebra {
// Input-facing coefficients: x -> multiplier * x + translation, modulo p.
// Both coefficients must be canonical residues, but multiplier may be zero.
struct affine_update {
    std::uint32_t multiplier;
    std::uint32_t translation;
};

// Same action in Montgomery representation. The field is part of the type.
template <typename Field> struct encoded_affine_update {
    using word_type = word_type_t<Field>;
    word_type multiplier;
    word_type translation;
};

// Ordinary residues, not encoded ones. Each lane may lie in [0, 2p).
template <std::size_t Fanout> struct alignas(Fanout >= 16 ? 64 : 32) affine_sum_leaf {
    std::array<std::uint32_t, Fanout> sums;
};

// Each slot summarizes one child. sums[] already includes that slot's lazy
// action; the child's own block does not include it until the action is pushed.
// Lazy coefficients are encoded. Only a clean multiplier has the high bit set.
template <std::size_t Fanout> struct alignas(Fanout >= 16 ? 64 : 32) affine_sum_branch {
    std::array<std::uint32_t, Fanout> sums;
    std::array<std::uint32_t, Fanout> lazy_multipliers;
    std::array<std::uint32_t, Fanout> lazy_translations;
};

}
// ===== include/canard/numeric/montgomery32_avx2.hpp =====
namespace canard::numeric {
template <std::uint32_t Modulus> struct montgomery32_avx2 {
    using scalar = montgomery32<Modulus>;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using pack_type = simd::u32x8;
    static constexpr auto modulus = Modulus;
    static constexpr auto negative_inverse = scalar::negative_inverse;
    // A radix-2^32 reduction needs the complete 64-bit product for each lane.
    // AVX2 multiplies alternate 32-bit lanes, so use separate even/odd streams.
    [[nodiscard]] static inline simd::u32x8 multiply(simd::u32x8 left, simd::u32x8 right) noexcept {
        using access = simd::detail::native_access;
        const auto a = access::get(left), b = access::get(right);
        const auto even = _mm256_mul_epu32(a, b);
        const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
        const auto inverse = _mm256_set1_epi32(std::bit_cast<int>(negative_inverse));
        const auto prime = _mm256_set1_epi32(static_cast<int>(modulus));
        const auto even_correction = _mm256_mul_epu32(even, inverse);
        const auto odd_correction = _mm256_mul_epu32(odd, inverse);
        const auto even_sum = _mm256_add_epi64(even, _mm256_mul_epu32(even_correction, prime));
        const auto odd_sum = _mm256_add_epi64(odd, _mm256_mul_epu32(odd_correction, prime));
        return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even_sum, 32), odd_sum, 0xaa));
    }

    // The factor is supplied encoded; internally Shoup-style reciprocal
    // reduction uses its canonical ordinary value. Input and output vectors
    // retain their representation (encoded or ordinary). The reciprocal is
    // prepared ONCE for all uses of this multiplier.
    class blended_multiplier;
    class fixed_multiplier {
        friend class blended_multiplier;
        simd::u32x8 factor_;
        simd::broadcast_u32x8 quotient_;

      public:
        explicit fixed_multiplier(u32 encoded) noexcept {
            const u32 value = scalar::decode(encoded);
            factor_ = simd::u32x8{value};
            quotient_ = simd::broadcast_u32x8{static_cast<u32>((u64{value} << 32) / modulus)};
        }
        [[nodiscard]] simd::u32x8 operator()(simd::u32x8 values) const noexcept {
            const auto q = simd::multiply_high(values, quotient_);
            return simd::multiply_low(values, factor_) -
                   simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    // Each lane chooses one of two already-prepared factors. Multiplication
    // retains its input representation and returns a lazy residue below 2p.
    // No divisions or reciprocal preparations occur inside the vector loop.
    class blended_multiplier {
        simd::u32x8 factor_, quotient_;

      public:
        blended_multiplier(simd::mask32x8 selected,
                           const fixed_multiplier& yes,
                           const fixed_multiplier& no) noexcept
            : factor_(simd::select(selected, yes.factor_, no.factor_)),
              quotient_(simd::select(selected, yes.quotient_.vector(), no.quotient_.vector())) {}
        [[nodiscard]] simd::u32x8 operator()(simd::u32x8 values) const noexcept {
            auto q = simd::multiply_high(values, quotient_);
            return simd::multiply_low(values, factor_) -
                   simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    [[nodiscard]] static simd::u32x8 add(simd::u32x8 a, simd::u32x8 b) noexcept {
        const auto sum = a + b;
        return simd::min(sum, sum - simd::u32x8{scalar::twice_modulus});
    }
};
} // namespace canard::numeric
// ===== include/canard/simd/reduction_avx2.hpp =====
#include <cstdint>

namespace canard::simd {

// Lane k is selected exactly when first <= k < last. Negative boundaries and
// boundaries beyond lane 7 are permitted. first must be greater than INT_MIN.
[[nodiscard]] inline mask32x8 lanes_between(int first, int last) noexcept {
    using access = detail::native_access;
    return access::mask(
        _mm256_andnot_si256(access::get(lanes_from(last)), access::get(lanes_from(first))));
}

[[nodiscard]] inline u32x8 operator&(u32x8 left, u32x8 right) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_and_si256(access::get(left), access::get(right)));
}

// Exact sum of eight unsigned 32-bit lanes, without 32-bit overflow.
[[nodiscard]] inline std::uint64_t reduce_add_widened(u32x8 values) noexcept {
    const auto raw = detail::native_access::get(values);
    const auto pairs = _mm256_add_epi64(_mm256_and_si256(raw, _mm256_set1_epi64x(0xffff'ffffu)),
                                        _mm256_srli_epi64(raw, 32));
    const auto halves =
        _mm_add_epi64(_mm256_castsi256_si128(pairs), _mm256_extracti128_si256(pairs, 1));
    const auto total = _mm_add_epi64(halves, _mm_srli_si128(halves, 8));
    return static_cast<std::uint64_t>(_mm_cvtsi128_si64(total));
}

// Sum an arbitrary contiguous range without reading beyond its end.
// The mathematical total must fit in uint64_t.
[[nodiscard]] inline std::uint64_t sum_widened_u32(const std::uint32_t* first,
                                                   const std::uint32_t* last) noexcept {
    auto even = _mm256_setzero_si256(), odd = _mm256_setzero_si256();
    const auto low = _mm256_set1_epi64x(0xffff'ffffu);
    for (; last - first >= 8; first += 8) {
        const auto values = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first));
        even = _mm256_add_epi64(even, _mm256_and_si256(values, low));
        odd = _mm256_add_epi64(odd, _mm256_srli_epi64(values, 32));
    }
    const auto total = _mm256_add_epi64(even, odd);
    const auto halves =
        _mm_add_epi64(_mm256_castsi256_si128(total), _mm256_extracti128_si256(total, 1));
    std::uint64_t answer = static_cast<std::uint64_t>(
        _mm_cvtsi128_si64(_mm_add_epi64(halves, _mm_srli_si128(halves, 8))));
    for (; first != last; ++first)
        answer += *first;
    return answer;
}

} // namespace canard::simd
#include <array>
#include <cstddef>
#include <cstdint>

namespace canard::algebra {

template <typename Field, std::size_t Fanout>
    requires(Fanout == 8 || Fanout == 16 || Fanout == 32)
struct affine_sum_kernel {
    using word_type = word_type_t<Field>;
    using pack = simd::u32x8;
    using vectors = numeric::montgomery32_avx2<Field::modulus>;
    using leaf_type = affine_sum_leaf<Fanout>;
    using branch_type = affine_sum_branch<Fanout>;
    using action_type = encoded_affine_update<Field>;

    static constexpr word_type clean_bit = word_type{1} << 31;
    static constexpr word_type value_bits = clean_bit - 1;
    static constexpr word_type clean_multiplier = clean_bit | Field::one;

    // The expensive fixed-factor preparation is done once per update (or push),
    // then reused for all sum, multiplier, and translation vectors it touches.
    struct prepared_action {
        action_type action;
        fixed_multiplier_t<vectors> scale;

        explicit prepared_action(action_type encoded) noexcept
            : action(encoded), scale(encoded.multiplier) {}
    };

    [[nodiscard]] static word_type reduce(pack values) noexcept {
        return static_cast<word_type>(simd::reduce_add_widened(values) % Field::modulus);
    }

    // A uint64_t value congruent to the selected sum modulo p. For Fanout=32
    // intermediate lanes may already be reduced modulo 2p; this is not an
    // exact integer sum. Several such results can be added before one final %p.
    template <unsigned Active = Fanout>
    [[nodiscard]] static std::uint64_t
    sum_range_widened(const word_type* sums, unsigned first, unsigned last) noexcept {
        static_assert(0 < Active && Active <= Fanout);
        pack total{0};
        for (unsigned block = 0; block < Active; block += pack::size()) {
            const auto selected =
                simd::lanes_between(static_cast<int>(first) - static_cast<int>(block),
                                    static_cast<int>(last) - static_cast<int>(block));
            total = accumulate(total,
                               simd::select(selected, simd::load_aligned(sums + block), pack{0}));
        }
        return simd::reduce_add_widened(total);
    }
    template <unsigned Active = Fanout>
    [[nodiscard]] static word_type
    sum_range(const word_type* sums, unsigned first, unsigned last) noexcept {
        return sum_range_widened<Active>(sums, first, last) % Field::modulus;
    }

    [[nodiscard]] static bool has_pending_action(const branch_type& node, unsigned child) noexcept {
        return (node.lazy_multipliers[child] & clean_bit) == 0;
    }

    [[nodiscard]] static action_type pending_action(const branch_type& node,
                                                    unsigned child) noexcept {
        return {node.lazy_multipliers[child] & value_bits, node.lazy_translations[child]};
    }

    static void clear_action(branch_type& node, unsigned child) noexcept {
        node.lazy_multipliers[child] = clean_multiplier;
        node.lazy_translations[child] = 0;
    }

    // REDC((bR)*ordinary_sum + (cR)*ordinary_length) is an ordinary sum.
    // Mixing representations here is intentional: neither the sum nor the
    // length needs encoding, and there is no decoding at each query level.
    [[nodiscard]] static word_type
    apply_to_sum(action_type action, word_type ordinary_sum, unsigned length) noexcept {
        return Field::multiply_sum(action.multiplier, ordinary_sum, action.translation, length);
    }

    template <bool ReturnDelta, unsigned Active = Fanout>
    CANARD_ALWAYS_INLINE static inline word_type
    apply(leaf_type& node, unsigned first, unsigned last, const prepared_action& update) noexcept {
        return transform<false, ReturnDelta, Active>(
            node.sums.data(), nullptr, nullptr, first, last, update, 1);
    }

    template <bool ReturnDelta, unsigned Active = Fanout>
    CANARD_ALWAYS_INLINE static inline word_type apply(branch_type& node,
                                                         unsigned first,
                                                         unsigned last,
                                                         const prepared_action& update,
                                                         unsigned child_length) noexcept {
        return transform<true, ReturnDelta, Active>(node.sums.data(),
                                                    node.lazy_multipliers.data(),
                                                    node.lazy_translations.data(),
                                                    first,
                                                    last,
                                                    update,
                                                    child_length);
    }

    // Materialize the incoming action everywhere and the new action only on
    // [first, last), using their composition for those lanes. Return only the
    // new action's delta, because the parent already includes the incoming one.
    template <bool MaintainTags>
    CANARD_ALWAYS_INLINE static inline word_type
    transform_carried(word_type* sums,
                      word_type* multipliers,
                      word_type* translations,
                      unsigned first,
                      unsigned last,
                      const prepared_action& update,
                      action_type incoming,
                      unsigned child_length) noexcept {
        const auto [b, c] = update.action;
        const prepared_action before{incoming};
        if constexpr (MaintainTags) {
            if (first == last) {
                return transform<true, false>(
                    sums, multipliers, translations, 0, Fanout, before, child_length);
            }
        }
        const prepared_action after{
            action_type{Field::multiply(b, incoming.multiplier),
                        Field::add(Field::multiply(b, incoming.translation), c)}};
        const pack old_offset{Field::multiply(incoming.translation, child_length)};
        const pack new_offset{Field::multiply(after.action.translation, child_length)};
        pack previous_total{0};
        for (unsigned block = 0; block < Fanout; block += pack::size()) {
            const auto selected =
                simd::lanes_between(int(first) - int(block), int(last) - int(block));
            const blended_multiplier_t<vectors> scale{selected, after.scale, before.scale};
            auto previous = simd::load_aligned(sums + block);
            previous_total = accumulate(previous_total, simd::select(selected, previous, pack{0}));
            simd::store_aligned(
                sums + block,
                vectors::add(scale(previous), simd::select(selected, new_offset, old_offset)));
            if constexpr (MaintainTags) {
                const auto old_b = simd::load_aligned(multipliers + block) & pack{value_bits};
                const auto old_c = simd::load_aligned(translations + block);
                const auto offset = simd::select(
                    selected, pack{after.action.translation}, pack{incoming.translation});
                simd::store_aligned(multipliers + block, scale(old_b));
                simd::store_aligned(translations + block, vectors::add(scale(old_c), offset));
            }
        }
        const unsigned length = (last - first) * child_length;
        // H = update o incoming. Delta = (H.b-G.b)*S + (H.c-G.c)*L.
        // Reusing these coefficients avoids first reducing G(S,L).
        return apply_to_sum({Field::subtract(after.action.multiplier, incoming.multiplier),
                             Field::subtract(after.action.translation, incoming.translation)},
                            reduce(previous_total),
                            length);
    }

    CANARD_ALWAYS_INLINE static inline word_type apply_carried(leaf_type& node,
                                                                 unsigned first,
                                                                 unsigned last,
                                                                 const prepared_action& update,
                                                                 action_type incoming) noexcept {
        return transform_carried<false>(
            node.sums.data(), nullptr, nullptr, first, last, update, incoming, 1);
    }
    CANARD_ALWAYS_INLINE static inline word_type apply_carried(branch_type& node,
                                                                 unsigned first,
                                                                 unsigned last,
                                                                 const prepared_action& update,
                                                                 action_type incoming,
                                                                 unsigned length) noexcept {
        return transform_carried<true>(node.sums.data(),
                                       node.lazy_multipliers.data(),
                                       node.lazy_translations.data(),
                                       first,
                                       last,
                                       update,
                                       incoming,
                                       length);
    }

    template <bool ReturnDelta>
    CANARD_ALWAYS_INLINE static inline word_type apply_values(
        word_type* sums, unsigned first, unsigned last, const prepared_action& update) noexcept {
        return transform<false, ReturnDelta>(sums, nullptr, nullptr, first, last, update, 1);
    }

  private:
    // For B<=16, each lane accumulates at most two values below 2p.
    // Since 4p<2^32, raw addition is exact; reduce only after widening.
    [[nodiscard]] static pack accumulate(pack a, pack b) noexcept {
        if constexpr (Fanout <= 16)
            return a + b;
        else
            return vectors::add(a, b);
    }
    // Every memory operation is full-width and aligned. Masks select values,
    // not memory accesses: all Fanout entries must exist even in a padded node.
    template <bool MaintainTags, bool ReturnDelta, unsigned Active = Fanout>
    CANARD_ALWAYS_INLINE static inline word_type transform(word_type* sums,
                                                             word_type* multipliers,
                                                             word_type* translations,
                                                             unsigned first,
                                                             unsigned last,
                                                             const prepared_action& update,
                                                             unsigned child_length) noexcept {
        const auto [b, c] = update.action;
        const pack sum_offset{Field::multiply(c, child_length)};
        const pack tag_offset{c};
        pack previous_total{0};

        for (unsigned block = 0; block < Active; block += pack::size()) {
            const auto selected =
                simd::lanes_between(static_cast<int>(first) - static_cast<int>(block),
                                    static_cast<int>(last) - static_cast<int>(block));
            auto* destination = sums + block;
            const auto previous = simd::load_aligned(destination);
            if constexpr (ReturnDelta) {
                previous_total =
                    accumulate(previous_total, simd::select(selected, previous, pack{0}));
            }
            const auto next = vectors::add(update.scale(previous), sum_offset);
            simd::store_aligned(destination, simd::select(selected, next, previous));

            if constexpr (MaintainTags) {
                auto* scales = multipliers + block;
                auto* shifts = translations + block;
                const auto old_scales = simd::load_aligned(scales);
                const auto old_shifts = simd::load_aligned(shifts);
                const auto next_scales = update.scale(old_scales & pack{value_bits});
                const auto next_shifts = vectors::add(update.scale(old_shifts), tag_offset);
                simd::store_aligned(scales, simd::select(selected, next_scales, old_scales));
                simd::store_aligned(shifts, simd::select(selected, next_shifts, old_shifts));
            }
        }
        if constexpr (ReturnDelta) {
            // New total - old total = (b - 1) * old total + c * covered length.
            return apply_to_sum({Field::subtract(b, Field::one), c},
                                reduce(previous_total),
                                (last - first) * child_length);
        } else {
            return 0;
        }
    }
};

} // namespace canard::algebra
namespace canard::kernel {
template <std::uint32_t Modulus, unsigned Extent>
struct sequence_affine_sum_avx2 : sequence_affine_sum_common<Modulus, Extent> {
    using base = sequence_affine_sum_common<Modulus, Extent>;
    using field = field_t<base>;
    static constexpr unsigned chunk = Extent < 32 ? Extent : 32;
    using arithmetic = algebra::affine_sum_kernel<field, chunk>;
    using leaf_type = leaf_type_t<base>;
    using summary_type = summary_type_t<base>;
    using update_type = update_type_t<base>;
    using action_type = action_type_t<base>;
    using prepared_action = prepared_action_t<arithmetic>;
    using base::base;
    [[nodiscard]] static prepared_action prepare(update_type f) noexcept {
        const auto a = base::encode(f);
        return prepared_action{{a.multiplier, a.translation}};
    }
    [[nodiscard]] static action_type action_of(const prepared_action& f) noexcept {
        return {f.action.multiplier, f.action.translation};
    }
    [[nodiscard]] static summary_type
    summarize(const leaf_type& leaf, unsigned l, unsigned r) noexcept {
        return simd::sum_widened_u32(leaf.values.data() + l, leaf.values.data() + r) % Modulus;
    }
    // Separate full chunks so their all-lane masks disappear at compile time.
    static void
    transform(leaf_type& leaf, unsigned l, unsigned r, const prepared_action& f) noexcept {
        if (l % chunk) {
            const auto at = l / chunk * chunk;
            const auto end = std::min(at + chunk, r);
            arithmetic::template apply_values<false>(leaf.values.data() + at, l - at, end - at, f);
            l = end;
        }
        for (; l + chunk <= r; l += chunk)
            arithmetic::template apply_values<false>(leaf.values.data() + l, 0, chunk, f);
        if (l < r)
            arithmetic::template apply_values<false>(leaf.values.data() + l, 0, r - l, f);
    }
    static void materialize(leaf_type& leaf, unsigned size, action_type f) noexcept {
        transform(leaf, 0, size, prepared_action{{f.multiplier, f.translation}});
    }
    [[nodiscard]] static summary_type update_leaf(leaf_type& leaf,
                                                  unsigned,
                                                  unsigned l,
                                                  unsigned r,
                                                  const prepared_action& f,
                                                  summary_type old) noexcept {
        const auto before = summarize(leaf, l, r);
        transform(leaf, l, r, f);
        return field::add(old, field::subtract(base::map(action_of(f), before, r - l), before));
    }
    // Materialize the older frame and the new partial update in one pass
    // over the leaf's value regions. The supplied summary already includes
    // incoming. Its correction is (after - incoming) on the selected region.
    [[nodiscard]] static summary_type update_leaf_carried(
        leaf_type& leaf, unsigned size, unsigned l, unsigned r,
        const prepared_action& update, action_type incoming, summary_type old) noexcept {
        const auto prior_part = summarize(leaf, l, r);
        const auto after = base::compose(action_of(update), incoming);
        const prepared_action before{{incoming.multiplier, incoming.translation}};
        const prepared_action next{{after.multiplier, after.translation}};
        transform(leaf, 0, l, before);
        transform(leaf, l, r, next);
        transform(leaf, r, size, before);
        const action_type change{
            field::subtract(after.multiplier, incoming.multiplier),
            field::subtract(after.translation, incoming.translation)};
        return field::add(old, base::map(change, prior_part, r - l));
    }
    using movement = sequence_leaf_u32_avx2<Extent>;
    static void exchange_reversed_outer_parts(leaf_type& left, unsigned left_size,
                                              unsigned left_cut, leaf_type& right,
                                              unsigned right_size, unsigned right_cut) noexcept {
        movement::exchange_reversed_outer_parts(left, left_size, left_cut,
                                                right, right_size, right_cut);
    }
    static void exchange_reversed_outer_parts_oriented(
        leaf_type& left, unsigned left_size, unsigned left_cut, bool left_reversed,
        leaf_type& right, unsigned right_size, unsigned right_cut, bool right_reversed) noexcept {
        movement::exchange_reversed_outer_parts_oriented(
            left, left_size, left_cut, left_reversed, right, right_size, right_cut, right_reversed);
    }
    static void reverse_values(leaf_type& leaf, unsigned first, unsigned last) noexcept {
        movement::reverse_values(leaf, first, last);
    }

};
} // namespace canard::kernel
namespace canard::sequence {
template <std::uint32_t P, unsigned Extent>
    requires(P > 1 && (P & 1) != 0 && P < (1u << 30))
struct kernel_binding<wide::representation::ordinary,
                      wide::execution::avx2,
                      algebra::modular_sum<P>,
                      algebra::affine_on_sum<P>,
                      Extent> {
    using type = kernel::sequence_affine_sum_avx2<P, Extent>;
};
} // namespace canard::sequence
#include <vector>
#ifndef CANARD_SEQUENCE_CAPACITY
#define CANARD_SEQUENCE_CAPACITY 512
#endif
#ifndef CANARD_SEQUENCE_OCCUPANCY
#define CANARD_SEQUENCE_OCCUPANCY 8
#endif
#ifndef CANARD_SEQUENCE_BALANCE
#define CANARD_SEQUENCE_BALANCE 4
#endif
namespace io = canard::io;
using u32 = std::uint32_t;
using index_field = io::bounded<u32, 999999>;
using opcode = io::bounded<u32, 9>;
using number = io::bounded<u32, 999999999>;
using client_execution = canard::judge::execution;

namespace {
constexpr std::uint32_t modulus = 998244353;
#ifdef CANARD_SEQUENCE_SCALAR
using execution = canard::wide::execution::scalar;
#else
using execution = client_execution;
#endif
using profile = canard::sequence::configuration<CANARD_SEQUENCE_CAPACITY,
                                                1000000,
                                                canard::wide::representation::ordinary,
                                                execution,
                                                CANARD_SEQUENCE_OCCUPANCY,
                                                CANARD_SEQUENCE_BALANCE>;
} // namespace
int main() {
    canard::judge::input input;
    canard::judge::output output;
    const auto [n, q] = input.read<index_field, index_field>();
    std::vector<std::uint32_t> initial(n);
    input.read<number>(std::span{initial});
    canard::chunked_lazy_sequence sequence{initial,
                                           canard::algebra::modular_sum<modulus>{},
                                           canard::algebra::affine_on_sum<modulus>{},
                                           profile{}};
    for (unsigned op = 0; op < q; ++op) {
        const auto type = input.read<opcode>();
        if (type == 0) {
            const auto [i, x] = input.read<number, number>();
            sequence.insert(i, x);
        } else if (type == 1) {
            const auto i = input.read<index_field>();
            sequence.erase(i);
        } else {
            const auto [l, r] = input.read<number, number>();
            if (type == 2)
                sequence.reverse(l, r);
            else if (type == 3) {
                const auto [b, c] = input.read<number, number>();
                sequence.apply(l, r, {b, c});
            } else
                output.write(sequence.fold(l, r));
        }
    }
    output.finish();
}
