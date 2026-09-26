
#define CANARD_CLIENT_AVX2 1


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
// ===== include/canard/kernel/radix_minimum_avx2.hpp =====
#ifndef __AVX2__
#error "canard radix_minimum_avx2.hpp requires an explicit AVX2 compilation target"
#endif
// ===== include/canard/kernel/radix_minimum.hpp =====
#include <algorithm>
#include <concepts>
#include <span>

namespace canard::kernel {
template <typename Key, typename Value> struct radix_record {
    Key key;
    Value value;
};

// Execution specializations perform only this local reduction. The queue's
// monotonicity, bucket partition and tie semantics are independent of the ISA.
template <typename Key, typename Value, typename Execution> struct radix_minimum;

template <typename Key, typename Value>
struct radix_minimum<Key, Value, execution::scalar> {
    using record_type = radix_record<Key, Value>;
    // Precondition: records is nonempty.
    static Key minimum(std::span<const record_type> records) noexcept {
        Key result = records.front().key;
        for (const auto& entry : records.subspan(1))
            result = std::min(result, entry.key);
        return result;
    }
};
} // namespace canard::kernel
#include <immintrin.h>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace canard::kernel {
template <typename Key, typename Value>
struct radix_minimum<Key, Value, execution::avx2> {
    using record_type = radix_record<Key, Value>;
    static Key minimum(std::span<const record_type> records) noexcept {
        if constexpr (std::same_as<Key, std::uint64_t> &&
                      std::same_as<Value, std::uint32_t>) {
            static_assert(sizeof(record_type) == 16 && offsetof(record_type, key) == 0);
            if (records.size() < 8)
                return radix_minimum<Key, Value, execution::scalar>::minimum(records);
            // Only 64-bit lanes 0 and 2 contain keys; payload/padding lanes are
            // never used in the result. Biasing makes unsigned order signed.
            const auto bias = _mm256_set1_epi64x(std::numeric_limits<std::int64_t>::min());
            auto minimum = _mm256_set1_epi64x(std::numeric_limits<std::int64_t>::max());
            std::size_t i = 0;
            for (; i + 2 <= records.size(); i += 2) {
                auto next = _mm256_loadu_si256(
                    reinterpret_cast<const __m256i*>(records.data() + i));
                next = _mm256_xor_si256(next, bias);
                const auto less = _mm256_cmpgt_epi64(minimum, next);
                minimum = _mm256_blendv_epi8(minimum, next, less);
            }
            minimum = _mm256_xor_si256(minimum, bias);
            Key result = std::min(static_cast<Key>(_mm256_extract_epi64(minimum, 0)),
                                  static_cast<Key>(_mm256_extract_epi64(minimum, 2)));
            if (i < records.size())
                result = std::min(result, records[i].key);
            return result;
        } else if constexpr (std::same_as<Key, std::uint32_t> &&
                             std::same_as<Value, std::uint32_t>) {
            static_assert(sizeof(record_type) == 8 && offsetof(record_type, key) == 0);
            if (records.size() < 8)
                return radix_minimum<Key, Value, execution::scalar>::minimum(records);
            auto minimum = _mm256_set1_epi32(-1);
            std::size_t i = 0;
            for (; i + 4 <= records.size(); i += 4) {
                const auto next = _mm256_loadu_si256(
                    reinterpret_cast<const __m256i*>(records.data() + i));
                minimum = _mm256_min_epu32(minimum, next);
            }
            auto half = _mm_min_epu32(_mm256_castsi256_si128(minimum),
                                      _mm256_extracti128_si256(minimum, 1));
            half = _mm_min_epu32(half, _mm_shuffle_epi32(half, _MM_SHUFFLE(3, 2, 3, 2)));
            Key result = static_cast<Key>(_mm_cvtsi128_si32(half));
            for (; i < records.size(); ++i)
                result = std::min(result, records[i].key);
            return result;
        } else {
            // The explicit backend has the same API for other record layouts;
            // the local operation uses the portable reduction for those types.
            return radix_minimum<Key, Value, execution::scalar>::minimum(records);
        }
    }
};
} // namespace canard::kernel
// ===== include/canard/graph/k_shortest_walk.hpp =====
// ===== include/canard/index/shortest_walk_index.hpp =====
// ===== include/canard/kernel/nonnegative_shortest_tree.hpp =====
// ===== include/canard/structural/weighted_adjacency.hpp =====
// ===== include/canard/graph/weighted_edge.hpp =====
#include <concepts>
#include <cstdint>
#include <type_traits>

namespace canard::graph {
// The distance bound used by shortest_path_graph relies on <=32-bit weights.
template <typename T>
concept unsigned_edge_weight = std::unsigned_integral<T> && !std::same_as<T, bool> && sizeof(T) <= 4;

template <unsigned_edge_weight Weight = std::uint32_t> struct weighted_edge {
    using weight_type = Weight;
    std::uint32_t from, to;
    Weight weight;
    friend bool operator==(const weighted_edge&, const weighted_edge&) = default;
};

template <unsigned_edge_weight Weight = std::uint32_t> struct weighted_arc {
    std::uint32_t to;
    Weight weight;
    friend bool operator==(const weighted_arc&, const weighted_arc&) = default;
};
} // namespace canard::graph
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

namespace canard::structural {
enum class weighted_adjacency_kind { compressed_rows, functional_forward, functional_reverse };

struct weighted_adjacency_options {
    // A transposed functional layout has at most one stored outgoing arc per
    // vertex. The owner must reverse endpoints AND the recovered path.
    bool reverse_functional = true;
};

// A storage component, not an implicitly transposed public graph. The name
// stored_neighbors() and reversed() expose its orientation explicitly.
template <graph::unsigned_edge_weight Weight = std::uint32_t> class weighted_adjacency {
  public:
    using weight_type = Weight;
    using edge_type = graph::weighted_edge<Weight>;
    using arc_type = graph::weighted_arc<Weight>;
    using vertex_type = std::uint32_t;

  private:
    vertex_type vertices_ = 0, edges_ = 0;
    weighted_adjacency_kind kind_ = weighted_adjacency_kind::functional_forward;
    std::vector<vertex_type> offsets_;
    std::unique_ptr<arc_type[]> arcs_;

    [[nodiscard]] std::size_t arc_slots() const noexcept {
        return functional() ? vertices_ : edges_;
    }

  public:
    weighted_adjacency() = default;
    template <std::ranges::input_range Range>
        requires std::convertible_to<std::ranges::range_reference_t<Range>, edge_type>
    explicit weighted_adjacency(std::size_t vertices, Range&& edges,
                                weighted_adjacency_options options = {}) {
        constexpr auto maximum = std::numeric_limits<vertex_type>::max();
        if (vertices > maximum)
            throw std::length_error("canard weighted graph vertex domain exceeds uint32");
        vertices_ = static_cast<vertex_type>(vertices);
        std::vector<vertex_type> degrees(vertices + 1);
        std::vector<edge_type> growing;
        std::unique_ptr<edge_type[]> exact;
        std::size_t edge_count = 0;
        if constexpr (std::ranges::sized_range<Range>) {
            const auto count = std::ranges::size(edges);
            if (count > maximum)
                throw std::length_error("canard weighted graph edge count exceeds uint32");
            exact = std::make_unique_for_overwrite<edge_type[]>(static_cast<std::size_t>(count));
        }
        std::vector<std::uint64_t> incoming_seen(options.reverse_functional ? (vertices + 63) / 64 : 0);
        bool inverse_functional = options.reverse_functional;
        vertex_type degree_or = 0;
        for (auto&& item : edges) {
            const edge_type edge = item;
            if (edge.from >= vertices_ || edge.to >= vertices_)
                throw std::out_of_range("canard weighted graph edge endpoint");
            if constexpr (std::ranges::sized_range<Range>) {
                // A sized_range promises that its size equals the number of
                // elements. No growth check or value initialization is needed.
                exact[edge_count++] = edge;
            } else {
                if (growing.size() == maximum)
                    throw std::length_error("canard weighted graph edge count exceeds uint32");
                growing.push_back(edge);
                ++edge_count;
            }
            degree_or |= ++degrees[edge.from];
            if (inverse_functional) {
                const auto mask = std::uint64_t{1} << (edge.to % 64);
                auto& word = incoming_seen[edge.to / 64];
                inverse_functional = (word & mask) == 0;
                word |= mask;
            }
        }
        const std::span<const edge_type> staged{
            exact ? exact.get() : growing.data(), edge_count};
        edges_ = static_cast<vertex_type>(edge_count);
        if (degree_or <= 1 || inverse_functional) {
            const bool reverse = degree_or > 1;
            kind_ = reverse ? weighted_adjacency_kind::functional_reverse
                            : weighted_adjacency_kind::functional_forward;
            arcs_ = std::make_unique_for_overwrite<arc_type[]>(vertices_);
            std::fill_n(arcs_.get(), vertices_, arc_type{vertices_, Weight{0}});
            for (const auto& edge : staged)
                arcs_[reverse ? edge.to : edge.from] = {reverse ? edge.from : edge.to, edge.weight};
        } else {
            kind_ = weighted_adjacency_kind::compressed_rows;
            std::partial_sum(degrees.begin(), degrees.end(), degrees.begin());
            arcs_ = std::make_unique_for_overwrite<arc_type[]>(edges_);
            for (const auto& edge : staged)
                arcs_[--degrees[edge.from]] = {edge.to, edge.weight};
            offsets_ = std::move(degrees);
        }
    }

    weighted_adjacency(const weighted_adjacency& other)
        : vertices_(other.vertices_), edges_(other.edges_), kind_(other.kind_),
          offsets_(other.offsets_), arcs_(std::make_unique_for_overwrite<arc_type[]>(other.arc_slots())) {
        if (arc_slots() != 0)
            std::copy_n(other.arcs_.get(), arc_slots(), arcs_.get());
    }
    weighted_adjacency(weighted_adjacency&& other) noexcept { swap(other); }
    weighted_adjacency& operator=(const weighted_adjacency& other) {
        if (this != &other) {
            weighted_adjacency next(other);
            swap(next);
        }
        return *this;
    }
    weighted_adjacency& operator=(weighted_adjacency&& other) noexcept {
        if (this != &other) {
            weighted_adjacency next(std::move(other));
            swap(next);
        }
        return *this;
    }
    void swap(weighted_adjacency& other) noexcept {
        using std::swap;
        swap(vertices_, other.vertices_);
        swap(edges_, other.edges_);
        swap(kind_, other.kind_);
        offsets_.swap(other.offsets_);
        arcs_.swap(other.arcs_);
    }
    friend void swap(weighted_adjacency& a, weighted_adjacency& b) noexcept { a.swap(b); }
    [[nodiscard]] vertex_type vertex_count() const noexcept { return vertices_; }
    [[nodiscard]] vertex_type edge_count() const noexcept { return edges_; }
    [[nodiscard]] weighted_adjacency_kind kind() const noexcept { return kind_; }
    [[nodiscard]] bool functional() const noexcept {
        return kind_ != weighted_adjacency_kind::compressed_rows;
    }
    [[nodiscard]] bool reversed() const noexcept {
        return kind_ == weighted_adjacency_kind::functional_reverse;
    }
    // Precondition: vertex < vertex_count(). The span borrows this storage.
    [[nodiscard]] std::span<const arc_type> stored_neighbors(vertex_type vertex) const & noexcept {
        assert(vertex < vertices_);
        if (functional())
            return {arcs_.get() + vertex, arcs_[vertex].to == vertices_ ? 0u : 1u};
        return {arcs_.get() + offsets_[vertex], offsets_[vertex + 1] - offsets_[vertex]};
    }
    void stored_neighbors(vertex_type) const && = delete;
    [[nodiscard]] std::span<const vertex_type> offsets() const & noexcept { return offsets_; }
    void offsets() const && = delete;
    [[nodiscard]] std::span<const arc_type> stored_arcs() const & noexcept {
        return {arcs_.get(), arc_slots()};
    }
    void stored_arcs() const && = delete;
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return offsets_.capacity() * sizeof(vertex_type) + arc_slots() * sizeof(arc_type);
    }
    // Original edge orientation; storage order is not the original input order.
    [[nodiscard]] std::vector<edge_type> snapshot() const {
        std::vector<edge_type> result;
        result.reserve(edges_);
        for (vertex_type u = 0; u < vertices_; ++u)
            for (const auto arc : stored_neighbors(u))
                result.push_back(reversed() ? edge_type{arc.to, u, arc.weight}
                                            : edge_type{u, arc.to, arc.weight});
        return result;
    }
};
} // namespace canard::structural
// ===== include/canard/queue/monotone_radix_heap.hpp =====
#include <array>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard {
// Numeric monotone priorities and cheap index/record payloads. Inserting a key
// below last_key() is a contract violation. Equal keys are allowed, including
// after the queue becomes empty. clear(floor) starts a new monotone epoch.
// This is deliberately not a general comparison heap for nontrivial payloads.
template <std::unsigned_integral Key = std::uint64_t,
          typename Value = std::uint32_t,
          typename Execution = execution::scalar,
          std::size_t InlineCapacity = 32>
    requires(!std::same_as<Key, bool> && sizeof(Key) <= 8 &&
             std::is_trivially_copyable_v<Value> && std::default_initializable<Value>)
class monotone_radix_heap {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = kernel::radix_record<Key, Value>;
    using size_type = std::size_t;
    using execution_type = Execution;
    static constexpr unsigned key_bits = std::numeric_limits<Key>::digits;
    static constexpr std::size_t inline_capacity = InlineCapacity;

  private:
    std::array<std::vector<value_type>, key_bits> buckets_;
    std::vector<Value> zero_;
    std::array<value_type, InlineCapacity> inline_{};
    Key last_ = 0;
    std::uint64_t occupied_ = 0;
    std::size_t size_ = 0;
    bool small_ = InlineCapacity != 0;

    // Does not change the logical size. Commit the occupied bit only after a
    // successful allocation, so ordinary pushes have the strong guarantee.
    CANARD_ALWAYS_INLINE void put(value_type entry) {
        if (entry.key == last_) {
            zero_.push_back(entry.value);
        } else {
            const unsigned bucket = std::bit_width(static_cast<Key>(entry.key ^ last_)) - 1;
            buckets_[bucket].push_back(entry);
            occupied_ |= std::uint64_t{1} << bucket;
        }
    }

    void migrate() {
        try {
            for (std::size_t i = 0; i < size_; ++i)
                put(inline_[i]);
            small_ = false;
        } catch (...) {
            clear(); // Basic guarantee: a valid, empty heap after failed migration.
            throw;
        }
    }

    CANARD_ALWAYS_INLINE void refill() {
        assert(zero_.empty() && occupied_);
        const unsigned bucket = std::countr_zero(occupied_);
        auto& entries = buckets_[bucket];
        occupied_ &= occupied_ - 1;
        try {
            last_ = kernel::radix_minimum<Key, Value, Execution>::minimum(entries);
            // Every redistributed key has a strictly smaller bucket index.
            for (auto entry : entries)
                put(entry);
            entries.clear();
        } catch (...) {
            clear(); // No partially repaired partition escapes an allocation failure.
            throw;
        }
    }

    [[nodiscard]] std::size_t inline_minimum() const noexcept {
        std::size_t result = 0;
        for (std::size_t i = 1; i < size_; ++i)
            if (inline_[i].key < inline_[result].key)
                result = i;
        return result;
    }

  public:
    monotone_radix_heap() = default;
    explicit monotone_radix_heap(Key floor) : last_(floor) {}
    monotone_radix_heap(const monotone_radix_heap&) = default;
    monotone_radix_heap(monotone_radix_heap&& other) noexcept {
        swap(other);
    }
    monotone_radix_heap& operator=(const monotone_radix_heap& other) {
        if (this != &other) {
            monotone_radix_heap next(other);
            swap(next);
        }
        return *this;
    }
    monotone_radix_heap& operator=(monotone_radix_heap&& other) noexcept {
        if (this != &other) {
            monotone_radix_heap next(std::move(other));
            swap(next);
        }
        return *this;
    }
    void swap(monotone_radix_heap& other) noexcept {
        using std::swap;
        swap(buckets_, other.buckets_);
        zero_.swap(other.zero_);
        swap(inline_, other.inline_);
        swap(last_, other.last_);
        swap(occupied_, other.occupied_);
        swap(size_, other.size_);
        swap(small_, other.small_);
    }
    friend void swap(monotone_radix_heap& a, monotone_radix_heap& b) noexcept {
        a.swap(b);
    }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] Key last_key() const noexcept { return last_; }

    CANARD_ALWAYS_INLINE void push(Key key, Value value) {
        assert(key >= last_);
        if constexpr (InlineCapacity != 0) {
            if (small_) {
                if (size_ < InlineCapacity) {
                    inline_[size_++] = {key, value};
                    return;
                }
                migrate();
            }
        }
        put({key, value});
        ++size_;
    }

    // Observing top may redistribute buckets and advance last_key() to the
    // current minimum. The returned record owns its cheap payload copy.
    [[nodiscard]] value_type top() {
        assert(!empty());
        if (small_) {
            const auto result = inline_[inline_minimum()];
            last_ = result.key;
            return result;
        }
        if (zero_.empty())
            refill();
        return {last_, zero_.back()};
    }

    [[nodiscard]] CANARD_ALWAYS_INLINE value_type pop() {
        assert(!empty());
        if (small_) {
            const auto index = inline_minimum();
            const auto result = inline_[index];
            inline_[index] = inline_[--size_];
            last_ = result.key;
            return result;
        }
        if (zero_.empty()) {
            const unsigned bucket = std::countr_zero(occupied_);
            auto& entries = buckets_[bucket];
            if (entries.size() == 1) {
                const auto result = entries.back();
                entries.clear();
                occupied_ &= occupied_ - 1;
                last_ = result.key;
                --size_;
                return result;
            }
            refill();
        }
        const auto result = value_type{last_, zero_.back()};
        zero_.pop_back();
        --size_;
        return result;
    }

    void clear(Key floor = 0) noexcept {
        for (auto& bucket : buckets_)
            bucket.clear();
        zero_.clear();
        last_ = floor;
        occupied_ = size_ = 0;
        small_ = InlineCapacity != 0;
    }
    // Dynamic payload capacity, excluding sizeof(*this) and allocator metadata.
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        std::size_t bytes = zero_.capacity() * sizeof(Value);
        for (const auto& bucket : buckets_)
            bytes += bucket.capacity() * sizeof(value_type);
        return bytes;
    }
};
} // namespace canard
#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace canard::kernel {
struct nonnegative_shortest_tree_result {
    static constexpr auto unreachable = std::numeric_limits<std::uint64_t>::max();
    static constexpr auto no_arc = std::numeric_limits<std::uint32_t>::max();
    std::vector<std::uint64_t> distance;
    std::vector<std::uint32_t> parent, tree_arc, settled;
};
// Run on the explicitly stored orientation, without sink suppression or early
// target termination. Parent and selected arc refer to the supplied storage.
// Strict relaxation makes parent-before-child settlement valid even at zero cost.
template <typename Adjacency, typename Execution = execution::scalar>
    requires graph::unsigned_edge_weight<typename Adjacency::weight_type>
[[nodiscard]] nonnegative_shortest_tree_result nonnegative_shortest_tree(
    const Adjacency& adjacency, std::uint32_t source,
    Execution = {}) {
    const auto n = adjacency.vertex_count();
    if (source >= n) throw std::out_of_range("canard shortest tree source");
    nonnegative_shortest_tree_result result;
    result.distance.assign(n, result.unreachable);
    result.parent.resize(n, n);
    result.tree_arc.resize(n, result.no_arc);
    result.settled.reserve(n);
    result.distance[source] = 0;
    monotone_radix_heap<std::uint64_t, std::uint32_t, Execution> queue;
    queue.push(0, source);
    const auto all = adjacency.stored_arcs();
    while (!queue.empty()) {
        const auto [d, u] = queue.pop();
        if (d != result.distance[u]) continue;
        result.settled.push_back(u);
        for (const auto& arc : adjacency.stored_neighbors(u)) {
            const auto v = arc.to;
            const auto candidate = d + static_cast<std::uint64_t>(arc.weight);
            if (candidate < result.distance[v]) {
                result.distance[v] = candidate;
                result.parent[v] = u;
                result.tree_arc[v] = static_cast<std::uint32_t>(&arc - all.data());
                queue.push(candidate, v);
            }
        }
    }
    return result;
}
} // namespace canard::kernel
// ===== include/canard/structural/persistent_leftist_heap.hpp =====
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <ranges>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard::structural {
// An append-only arena of persistent min heaps. Roots are arena-local indices;
// zero is empty. Meld preserves multiplicities, even when roots share nodes.
// References to nodes are invalidated by any operation that grows the arena.
template <std::unsigned_integral Key = std::uint64_t, typename Value = std::uint32_t>
    requires (!std::same_as<Key, bool> && std::is_trivially_copyable_v<Value> &&
              std::default_initializable<Value>)
class persistent_leftist_heap {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using index_type = std::uint32_t;
    using value_type = kernel::radix_record<Key, Value>;
    struct node_type {
        Key key{};
        Value value{};
        index_type left = 0, right = 0, rank = 0;
    };
  private:
    std::vector<node_type> nodes_{node_type{}};
    index_type append(node_type node) {
        if (nodes_.size() >= std::numeric_limits<index_type>::max())
            throw std::length_error("canard persistent heap index domain exhausted");
        const auto id = static_cast<index_type>(nodes_.size());
        nodes_.push_back(node);
        return id;
    }
  public:
    persistent_leftist_heap() = default;
    persistent_leftist_heap(const persistent_leftist_heap&) = default;
    persistent_leftist_heap& operator=(const persistent_leftist_heap&) = default;
    persistent_leftist_heap(persistent_leftist_heap&& other) : persistent_leftist_heap() { swap(other); }
    persistent_leftist_heap& operator=(persistent_leftist_heap&& other) {
        if (this != &other) { persistent_leftist_heap next(std::move(other)); swap(next); }
        return *this;
    }
    void swap(persistent_leftist_heap& other) noexcept { nodes_.swap(other.nodes_); }
    friend void swap(persistent_leftist_heap& a, persistent_leftist_heap& b) noexcept { a.swap(b); }
    explicit persistent_leftist_heap(std::size_t reserve_nodes) { reserve(reserve_nodes); }
    void reserve(std::size_t count) {
        if (count >= std::numeric_limits<index_type>::max())
            throw std::length_error("canard persistent heap capacity exceeds uint32");
        nodes_.reserve(count + 1);
    }
    [[nodiscard]] const node_type& operator[](index_type id) const noexcept {
        assert(id < nodes_.size());
        return nodes_[id];
    }
    [[nodiscard]] std::size_t node_count() const noexcept { return nodes_.size() - 1; }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept { return nodes_.capacity() * sizeof(node_type); }
    [[nodiscard]] index_type singleton(Key key, Value value) { return append({key, value, 0, 0, 1}); }

    // Floyd heap construction: linear time, one new node per supplied item.
    // Only fresh nodes are modified; every previously published root survives.
    template <std::ranges::input_range Range>
        requires std::convertible_to<std::ranges::range_reference_t<Range>, value_type>
    [[nodiscard]] index_type build(Range&& values) {
        const std::size_t first = nodes_.size();
        for (auto&& value : values) {
            const value_type entry = value;
            append({entry.key, entry.value, 0, 0, 1});
        }
        const auto count = nodes_.size() - first;
        if (count == 0) return 0;
        std::make_heap(nodes_.begin() + first, nodes_.end(),
                       [](const node_type& a, const node_type& b) { return a.key > b.key; });
        for (std::size_t i = count; i-- > 0;) {
            auto& node = nodes_[first + i];
            if (2 * i + 1 < count) node.left = static_cast<index_type>(first + 2 * i + 1);
            if (2 * i + 2 < count) node.right = static_cast<index_type>(first + 2 * i + 2);
            if (nodes_[node.left].rank < nodes_[node.right].rank) std::swap(node.left, node.right);
            node.rank = nodes_[node.right].rank + 1;
        }
        return static_cast<index_type>(first);
    }
    // No recursion or auxiliary stack. Unpublished copies temporarily form a
    // backwards chain; rebuilding restores each right edge and leftist rank.
    [[nodiscard]] index_type meld(index_type a, index_type b) {
        assert(a < nodes_.size() && b < nodes_.size());
        index_type chain = 0;
        while (a && b) {
            if (nodes_[b].key < nodes_[a].key) std::swap(a, b);
            auto node = nodes_[a];
            a = node.right;
            node.right = chain;
            chain = append(node);
        }
        index_type result = a ? a : b;
        while (chain) {
            auto& node = nodes_[chain];
            const auto previous = node.right;
            node.right = result;
            if (nodes_[node.left].rank < nodes_[node.right].rank) std::swap(node.left, node.right);
            node.rank = nodes_[node.right].rank + 1;
            result = chain;
            chain = previous;
        }
        return result;
    }
    [[nodiscard]] index_type insert(index_type root, Key key, Value value) {
        return meld(root, singleton(key, value));
    }
};
} // namespace canard::structural
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <span>
#include <utility>
#include <type_traits>
#include <vector>

namespace canard::index {
// Immutable after construction. Owns the target potentials and persistent
// sidetrack heaps. The compact specialization discards the original graph;
// the witness specialization retains original edges, selected IDs and depths.
enum class shortest_walk_layout { sidetrack_heaps, functional_orbit };

struct sidetrack_edge { std::uint32_t to = 0, edge_id = 0; };
struct walk_witness_storage {
    std::vector<graph::weighted_edge<std::uint32_t>> edges;
    std::vector<std::uint32_t> tree_edge, depth;
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return edges.capacity() * sizeof(edges[0]) + (tree_edge.capacity() + depth.capacity()) * sizeof(std::uint32_t);
    }
};
struct no_walk_witness_storage {};
template <bool RetainWitness = false>
struct basic_shortest_walk_index_data {
    using payload_type = std::conditional_t<RetainWitness, sidetrack_edge, std::uint32_t>;
    using heap_type = structural::persistent_leftist_heap<std::uint64_t, payload_type>;
    [[no_unique_address]] std::conditional_t<RetainWitness, walk_witness_storage, no_walk_witness_storage> witness;
    [[nodiscard]] static std::uint32_t destination(payload_type value) noexcept {
        if constexpr (RetainWitness) return value.to;
        else return value;
    }
    static constexpr auto unreachable = std::numeric_limits<std::uint64_t>::max();
    std::uint32_t vertices = 0, edges = 0, target = 0;
    std::vector<std::uint64_t> distance;
    std::vector<std::uint32_t> roots;
    heap_type heap;
    std::size_t sidetracks = 0;
    shortest_walk_layout layout = shortest_walk_layout::sidetrack_heaps;
    std::vector<std::uint64_t> cycle_members;
    std::uint64_t cycle_weight = 0;

    [[nodiscard]] bool cyclic(std::uint32_t source) const noexcept {
        return !cycle_members.empty() && ((cycle_members[source / 64] >> (source % 64)) & 1);
    }

    basic_shortest_walk_index_data() = default;
    template <typename Adjacency, typename Execution>
    explicit basic_shortest_walk_index_data(const Adjacency& incoming,
                                      std::uint32_t target_vertex, Execution execution, bool functional_orbits = true)
        : vertices(incoming.vertex_count()), edges(incoming.edge_count()), target(target_vertex) {
        // With <=1 incoming arc per original vertex, reversing a walk gives
        // a unique orbit from t. Repeated visits occur exactly on its cycle.
        if constexpr (!RetainWitness) {
            if (functional_orbits && incoming.functional()) {
                layout = shortest_walk_layout::functional_orbit;
                distance.assign(vertices, unreachable);
                const auto arcs = incoming.stored_arcs();
                auto u = target;
                distance[u] = 0;
                while (arcs[u].to != vertices) {
                    const auto v = arcs[u].to;
                    const auto w = arcs[u].weight;
                    const auto candidate = distance[u] + static_cast<std::uint64_t>(w);
                    if (distance[v] != unreachable) {
                        cycle_weight = candidate - distance[v];
                        cycle_members.resize((std::size_t(vertices) + 63) / 64);
                        auto current = v;
                        do {
                            cycle_members[current / 64] |= std::uint64_t{1} << (current % 64);
                            current = arcs[current].to;
                        } while (current != v);
                        sidetracks = 1;
                        break;
                    }
                    distance[v] = candidate;
                    u = v;
                }
                return;
            }
        }
        auto tree = kernel::nonnegative_shortest_tree(incoming, target, execution);
        build_sidetracks(incoming, tree);
    }
    // Precondition: tree was produced from exactly this incoming storage and
    // target. This consuming overload lets one-shot clients stop on an
    // unreachable source before allocating any sidetrack heaps.
    template <typename Adjacency>
    basic_shortest_walk_index_data(const Adjacency& incoming,
                             std::uint32_t target_vertex,
                             kernel::nonnegative_shortest_tree_result&& tree)
        : vertices(incoming.vertex_count()), edges(incoming.edge_count()), target(target_vertex) {
        build_sidetracks(incoming, tree);
    }
  private:
    template <typename Adjacency>
    void build_sidetracks(const Adjacency& incoming,
                         kernel::nonnegative_shortest_tree_result& tree) {
        const auto all = incoming.stored_arcs();
        // Only arcs entering a reachable vertex can participate in a walk.
        std::vector<std::uint32_t> offsets(std::size_t(vertices) + 1);
        for (auto v : tree.settled)
            for (const auto& arc : incoming.stored_neighbors(v))
                if (tree.tree_arc[arc.to] != static_cast<std::uint32_t>(&arc - all.data()))
                    ++offsets[arc.to];
        std::partial_sum(offsets.begin(), offsets.end(), offsets.begin());
        sidetracks = offsets.back();
        auto local = std::make_unique_for_overwrite<typename heap_type::value_type[]>(sidetracks);
        for (auto v : tree.settled)
            for (const auto& arc : incoming.stored_neighbors(v)) {
                const auto u = arc.to;
                if (tree.tree_arc[u] == static_cast<std::uint32_t>(&arc - all.data())) continue;
                const auto delta = tree.distance[v] + static_cast<std::uint64_t>(arc.weight) - tree.distance[u];
                if constexpr (RetainWitness) local[--offsets[u]] = {delta, {v, arc.edge_id}};
                else local[--offsets[u]] = {delta, v};
            }
        roots.resize(vertices);
        heap.reserve(std::min(2 * sidetracks, std::size_t(std::numeric_limits<std::uint32_t>::max()) - 1));
        for (auto u : tree.settled) {
            const auto items = std::span<const typename heap_type::value_type>(local.get(), sidetracks).subspan(offsets[u], offsets[u + 1] - offsets[u]);
            const auto own = heap.build(items);
            const auto inherited = tree.parent[u] == vertices ? 0u : roots[tree.parent[u]];
            roots[u] = heap.meld(own, inherited);
        }
        if constexpr (RetainWitness) {
            witness.tree_edge.resize(vertices, tree.no_arc);
            witness.depth.resize(vertices);
            for (auto u : tree.settled) {
                if (tree.tree_arc[u] != tree.no_arc) {
                    witness.tree_edge[u] = all[tree.tree_arc[u]].edge_id;
                    witness.depth[u] = witness.depth[tree.parent[u]] + 1;
                }
            }
        }
        distance = std::move(tree.distance);
    }
  public:
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        const auto extra = [&] { if constexpr (RetainWitness) return witness.allocated_bytes(); else return std::size_t{0}; }();
        return extra + distance.capacity() * sizeof(std::uint64_t) + roots.capacity() * sizeof(std::uint32_t) + heap.allocated_bytes() + cycle_members.capacity() * sizeof(std::uint64_t);
    }
};
using shortest_walk_index_data = basic_shortest_walk_index_data<false>;
using shortest_walk_witness_index_data = basic_shortest_walk_index_data<true>;
} // namespace canard::index
// ===== include/canard/graph/shortest_walk_witness.hpp =====
// ===== include/canard/kernel/sidetrack_successors.hpp =====
#include <cstdint>

namespace canard::kernel {
enum class sidetrack_step { replace_last, append };
// The same heap-unfolding law drives length-only and witness streams. The mode
// tells provenance consumers whether the last sidetrack is replaced or retained.
// Every emitted delta is nonnegative; graph cycles are represented by append.
template <typename Index, typename Emit>
void sidetrack_successors(const Index& index, std::uint32_t source, std::uint32_t node_id, Emit emit) {
    if (node_id == 0) {
        const auto root = index.roots[source];
        if (root) emit(index.heap[root].key, root, sidetrack_step::append);
        return;
    }
    const auto& node = index.heap[node_id];
    if (node.left) emit(index.heap[node.left].key - node.key, node.left, sidetrack_step::replace_last);
    if (node.right) emit(index.heap[node.right].key - node.key, node.right, sidetrack_step::replace_last);
    const auto root = index.roots[Index::destination(node.value)];
    if (root) emit(index.heap[root].key, root, sidetrack_step::append);
}
} // namespace canard::kernel
// ===== include/canard/queue/monotone_enumerator.hpp =====
#include <cstddef>
#include <optional>
#include <utility>
#include <type_traits>

namespace canard {
// Best-first unfolding, NOT visited-state graph search: duplicate states retain
// multiplicity. Expand(entry, emit) emits (absolute_key, state) with keys >=
// entry.key. Expanding is deferred until the next request, avoiding work after
// the last requested item. An expander may own immutable shared preprocessing.
template <typename Expand, typename Execution = execution::scalar>
    requires (std::is_nothrow_move_constructible_v<Expand> && std::is_nothrow_move_assignable_v<Expand>)
class monotone_enumerator {
  public:
    using key_type = typename Expand::key_type;
    using state_type = typename Expand::state_type;
    using queue_type = monotone_radix_heap<key_type, state_type, Execution>;
    using value_type = typename queue_type::value_type;
  private:
    Expand expand_;
    queue_type queue_;
    std::optional<value_type> pending_;
  public:
    monotone_enumerator() = default;
    explicit monotone_enumerator(Expand expand, std::optional<value_type> initial)
        : expand_(std::move(expand)) {
        if (initial) queue_.push(initial->key, initial->value);
    }
    monotone_enumerator(const monotone_enumerator&) = default;
    monotone_enumerator& operator=(const monotone_enumerator& other)
        requires std::copy_constructible<Expand> {
        if (this != &other) {
            monotone_enumerator next(other);
            *this = std::move(next);
        }
        return *this;
    }
    monotone_enumerator(monotone_enumerator&& other) noexcept(std::is_nothrow_move_constructible_v<Expand>)
        : expand_(std::move(other.expand_)), queue_(std::move(other.queue_)),
          pending_(std::exchange(other.pending_, std::nullopt)) {}
    monotone_enumerator& operator=(monotone_enumerator&& other) noexcept(std::is_nothrow_move_assignable_v<Expand>) {
        if (this != &other) {
            expand_ = std::move(other.expand_);
            queue_ = std::move(other.queue_);
            pending_ = std::exchange(other.pending_, std::nullopt);
        }
        return *this;
    }
    [[nodiscard]] std::optional<value_type> next() {
        if (pending_) {
            const auto entry = *pending_;
            pending_.reset();
            try {
                expand_(entry, [this](key_type key, state_type state) { queue_.push(key, state); });
            } catch (...) {
                queue_.clear(); // Failed expansion invalidates only this cursor.
                throw;
            }
        }
        if (queue_.empty()) return std::nullopt;
        pending_ = queue_.pop();
        return pending_;
    }
    [[nodiscard]] const Expand& expander() const noexcept { return expand_; }
    [[nodiscard]] std::size_t frontier_size() const noexcept { return queue_.size(); }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept { return queue_.allocated_bytes(); }
};
} // namespace canard
// ===== include/canard/structural/persistent_chain.hpp =====
#include <cassert>
#include <concepts>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <utility>

namespace canard::structural {
// Append-only persistent sequences; index zero is the empty sequence. A record
// stores one value and an earlier prefix. IDs survive growth, references do not.
// Copying the arena explicitly snapshots every published sequence. Destruction
// is iterative, independent of the depth of any sequence.
template <typename Value>
    requires (std::is_trivially_copyable_v<Value> && std::default_initializable<Value>)
class persistent_chain {
  public:
    using value_type = Value;
    using index_type = std::size_t;
    struct node_type { Value value{}; index_type previous = 0, depth = 0; };
  private:
    std::vector<node_type> nodes_{node_type{}};
  public:
    persistent_chain() = default;
    persistent_chain(const persistent_chain&) = default;
    persistent_chain& operator=(const persistent_chain&) = default;
    persistent_chain(persistent_chain&& other) : persistent_chain() { swap(other); }
    persistent_chain& operator=(persistent_chain&& other) {
        if (this != &other) { persistent_chain next(std::move(other)); swap(next); }
        return *this;
    }
    void swap(persistent_chain& other) noexcept { nodes_.swap(other.nodes_); }
    friend void swap(persistent_chain& a, persistent_chain& b) noexcept { a.swap(b); }
    [[nodiscard]] index_type append(index_type previous, Value value) {
        if (previous >= nodes_.size()) throw std::out_of_range("canard persistent chain prefix");
        if (nodes_[previous].depth == std::numeric_limits<index_type>::max())
            throw std::length_error("canard persistent chain depth exhausted");
        const auto id = nodes_.size();
        nodes_.push_back({value, previous, nodes_[previous].depth + 1});
        return id;
    }
    [[nodiscard]] const node_type& operator[](index_type id) const noexcept {
        assert(id < nodes_.size()); return nodes_[id];
    }
    [[nodiscard]] std::size_t node_count() const noexcept { return nodes_.size() - 1; }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept { return nodes_.capacity() * sizeof(node_type); }
    [[nodiscard]] std::vector<Value> materialize(index_type tail) const {
        if (tail >= nodes_.size()) throw std::out_of_range("canard persistent chain tail");
        std::vector<Value> values(nodes_[tail].depth);
        auto i = values.size();
        while (tail) { values[--i] = nodes_[tail].value; tail = nodes_[tail].previous; }
        return values;
    }
};
} // namespace canard::structural
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

namespace canard {
namespace graph::detail {
struct walk_step { std::uint32_t edge_id = 0; std::size_t prefix_edges = 0; };
using walk_history = structural::persistent_chain<walk_step>;
struct witness_frontier_state { std::uint32_t node = 0; std::size_t tail = 0; };
struct witness_sidetrack_expander {
    using key_type = std::uint64_t;
    using state_type = witness_frontier_state;
    using record_type = kernel::radix_record<key_type, state_type>;
    std::shared_ptr<const index::shortest_walk_witness_index_data> index;
    std::shared_ptr<walk_history> history;
    std::uint32_t source = 0;
    key_type maximum = std::numeric_limits<key_type>::max();
    witness_sidetrack_expander() = default;
    witness_sidetrack_expander(std::shared_ptr<const index::shortest_walk_witness_index_data> data,
                               std::uint32_t source_vertex, key_type cap)
        : index(std::move(data)), history(std::make_shared<walk_history>()), source(source_vertex), maximum(cap) {}
    // A copied stream gets an independent arena/frontier. Previously returned
    // handles keep their original arena, with no pointer or reference invalidation.
    witness_sidetrack_expander(const witness_sidetrack_expander& other)
        : index(other.index), history(other.history ? std::make_shared<walk_history>(*other.history) : nullptr),
          source(other.source), maximum(other.maximum) {}
    witness_sidetrack_expander& operator=(const witness_sidetrack_expander& other) {
        if (this != &other) { witness_sidetrack_expander next(other); *this = std::move(next); }
        return *this;
    }
    witness_sidetrack_expander(witness_sidetrack_expander&&) noexcept = default;
    witness_sidetrack_expander& operator=(witness_sidetrack_expander&&) noexcept = default;
    template <typename Emit>
    void operator()(record_type entry, Emit emit) const {
        kernel::sidetrack_successors(*index, source, entry.value.node,
            [&](key_type delta, std::uint32_t node, kernel::sidetrack_step step) {
                if (delta > maximum - entry.key) return;
                const auto prefix = step == kernel::sidetrack_step::replace_last
                    ? (*history)[entry.value.tail].previous : entry.value.tail;
                const auto previous = (*history)[prefix].value;
                const auto& metadata = index->witness;
                const auto current = prefix ? metadata.edges[previous.edge_id].to : source;
                const auto edge_id = index->heap[node].value.edge_id;
                const auto edge = metadata.edges[edge_id];
                assert(metadata.depth[current] >= metadata.depth[edge.from]);
                const auto segment = std::size_t(metadata.depth[current] - metadata.depth[edge.from]) + 1;
                const auto before = prefix ? previous.prefix_edges : std::size_t{0};
                const auto bound = std::numeric_limits<std::size_t>::max();
                if (segment > bound - before || metadata.depth[edge.to] > bound - before - segment)
                    throw std::length_error("canard walk edge count exceeds size_t");
                const auto tail = history->append(prefix, {edge_id, before + segment});
                emit(entry.key + delta, state_type{node, tail});
            });
    }
};
} // namespace graph::detail

template <typename Execution> class shortest_walks;

// An owning, compact walk handle. No edge list is constructed by enumeration.
// Stable IDs refer to original input order, including parallel edges. Repeated
// IDs represent repeated traversals. Methods require a non-moved-from handle.
// Extraction and advancing the producing cursor must not run concurrently.
class shortest_walk_witness {
    using index_type = index::shortest_walk_witness_index_data;
    std::shared_ptr<const index_type> index_;
    std::shared_ptr<const graph::detail::walk_history> history_;
    std::uint64_t weight_ = 0;
    std::size_t tail_ = 0, rank_ = 0;
    std::uint32_t source_ = 0;
    template <typename Execution> friend class shortest_walks;
    shortest_walk_witness(std::shared_ptr<const index_type> index,
                          std::shared_ptr<const graph::detail::walk_history> history,
                          std::uint32_t source, std::uint64_t weight, std::size_t tail, std::size_t rank)
        : index_(std::move(index)), history_(std::move(history)), weight_(weight), tail_(tail), rank_(rank), source_(source) {}
  public:
    using edge_id_type = std::uint32_t;
    using vertex_type = std::uint32_t;
    using edge_type = graph::weighted_edge<std::uint32_t>;
    shortest_walk_witness(const shortest_walk_witness&) = default;
    shortest_walk_witness& operator=(const shortest_walk_witness&) = default;
    shortest_walk_witness(shortest_walk_witness&&) noexcept = default;
    shortest_walk_witness& operator=(shortest_walk_witness&&) noexcept = default;
    [[nodiscard]] std::uint64_t weight() const noexcept { return weight_; }
    [[nodiscard]] std::size_t rank() const noexcept { return rank_; } // one-based
    [[nodiscard]] vertex_type source() const noexcept { return source_; }
    [[nodiscard]] vertex_type target() const noexcept { return index_->target; }
    [[nodiscard]] std::size_t sidetrack_count() const noexcept { return (*history_)[tail_].depth; }
    [[nodiscard]] std::size_t edge_count() const noexcept {
        if (!tail_) return index_->witness.depth[source_];
        const auto last = (*history_)[tail_].value;
        return last.prefix_edges + index_->witness.depth[index_->witness.edges[last.edge_id].to];
    }
    [[nodiscard]] const edge_type& edge(edge_id_type id) const & {
        return index_->witness.edges.at(id);
    }
    void edge(edge_id_type) const && = delete;
    [[nodiscard]] std::vector<edge_id_type> sidetrack_edge_ids() const {
        const auto steps = history_->materialize(tail_);
        std::vector<edge_id_type> ids;
        ids.reserve(steps.size());
        for (const auto step : steps) ids.push_back(step.edge_id);
        return ids;
    }
    // The bound is checked before any output. Extraction uses O(r) temporary
    // IDs for r sidetracks, plus the caller's output; tree segments are streamed.
    template <std::weakly_incrementable Output>
        requires std::indirectly_writable<Output, edge_id_type>
    Output copy_edge_ids(Output output, std::size_t max_edges = std::numeric_limits<std::size_t>::max()) const {
        if (edge_count() > max_edges) throw std::length_error("canard walk extraction limit");
        const auto steps = history_->materialize(tail_);
        const auto& metadata = index_->witness;
        auto current = source_;
        auto tree_to = [&](vertex_type destination) {
            while (current != destination) {
                const auto id = metadata.tree_edge[current];
                assert(id < metadata.edges.size());
                *output++ = id;
                current = metadata.edges[id].to;
            }
        };
        for (const auto step : steps) {
            const auto edge = metadata.edges[step.edge_id];
            tree_to(edge.from);
            *output++ = step.edge_id;
            current = edge.to;
        }
        tree_to(index_->target);
        return output;
    }
    [[nodiscard]] std::vector<edge_id_type> edge_ids(std::size_t max_edges = std::numeric_limits<std::size_t>::max()) const {
        const auto count = edge_count();
        if (count > max_edges) throw std::length_error("canard walk extraction limit");
        std::vector<edge_id_type> result(count);
        copy_edge_ids(result.begin(), max_edges);
        return result;
    }
    [[nodiscard]] std::vector<vertex_type> vertices(std::size_t max_edges = std::numeric_limits<std::size_t>::max()) const {
        const auto count = edge_count();
        if (count > max_edges || count == std::numeric_limits<std::size_t>::max())
            throw std::length_error("canard walk extraction limit");
        const auto ids = edge_ids(max_edges);
        std::vector<vertex_type> result;
        result.reserve(count + 1);
        result.push_back(source_);
        for (auto id : ids) result.push_back(index_->witness.edges[id].to);
        return result;
    }
};

// Independent consuming input view over implicit witnesses. Copies snapshot
// the enumeration and history; moving leaves an exhausted cursor. Handles keep
// preprocessing and provenance alive even after the cursor and owner die.
template <typename Execution = execution::scalar>
class shortest_walks : public std::ranges::view_interface<shortest_walks<Execution>> {
    using expander_type = graph::detail::witness_sidetrack_expander;
    monotone_enumerator<expander_type, Execution> enumeration_;
    std::size_t remaining_ = 0, produced_ = 0;
  public:
    using value_type = shortest_walk_witness;
    shortest_walks() = default;
    shortest_walks(std::shared_ptr<const index::shortest_walk_witness_index_data> index, std::uint32_t source,
                   std::size_t limit, std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max())
        : remaining_(limit) {
        if (!index || source >= index->vertices) throw std::out_of_range("canard walk source");
        std::optional<expander_type::record_type> initial;
        if (limit && index->distance[source] != index->unreachable && index->distance[source] <= maximum)
            initial = expander_type::record_type{index->distance[source], {0, 0}};
        enumeration_ = monotone_enumerator<expander_type, Execution>(expander_type{std::move(index), source, maximum}, initial);
    }
    shortest_walks(const shortest_walks&) = default;
    shortest_walks& operator=(const shortest_walks&) = default;
    shortest_walks(shortest_walks&& other) noexcept
        : enumeration_(std::move(other.enumeration_)), remaining_(std::exchange(other.remaining_, 0)), produced_(std::exchange(other.produced_, 0)) {}
    shortest_walks& operator=(shortest_walks&& other) noexcept {
        if (this != &other) {
            enumeration_ = std::move(other.enumeration_);
            remaining_ = std::exchange(other.remaining_, 0); produced_ = std::exchange(other.produced_, 0);
        }
        return *this;
    }
    [[nodiscard]] std::optional<value_type> next() {
        if (!remaining_) return std::nullopt;
        const auto entry = enumeration_.next();
        if (!entry) { remaining_ = 0; return std::nullopt; }
        --remaining_; ++produced_;
        const auto& expander = enumeration_.expander();
        return value_type{expander.index, expander.history, expander.source, entry->key, entry->value.tail, produced_};
    }
    [[nodiscard]] std::size_t produced() const noexcept { return produced_; }
    [[nodiscard]] std::size_t frontier_size() const noexcept { return enumeration_.frontier_size(); }
    [[nodiscard]] std::size_t history_node_count() const noexcept {
        const auto& h = enumeration_.expander().history; return h ? h->node_count() : 0;
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        const auto& h = enumeration_.expander().history;
        return enumeration_.allocated_bytes() + (h ? h->allocated_bytes() : 0);
    }
    class iterator {
        shortest_walks* owner_ = nullptr;
        std::optional<shortest_walk_witness> current_;
      public:
        using iterator_concept = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type = shortest_walk_witness;
        using difference_type = std::ptrdiff_t;
        iterator() = default;
        explicit iterator(shortest_walks& owner) : owner_(&owner), current_(owner.next()) {}
        const value_type& operator*() const { return *current_; }
        const value_type* operator->() const { return &*current_; }
        iterator& operator++() { current_ = owner_->next(); return *this; }
        void operator++(int) { ++*this; }
        friend bool operator==(const iterator& it, std::default_sentinel_t) noexcept { return !it.current_; }
    };
    iterator begin() { return iterator{*this}; }
    std::default_sentinel_t end() const noexcept { return {}; }
};
} // namespace canard
// ===== include/canard/structural/indexed_weighted_adjacency.hpp =====
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <ranges>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

namespace canard::structural {
// CSR with stable input-order edge identities. The explicit transpose flag
// changes stored orientation, never original_edges() or the IDs. Unlike the
// compact weighted_adjacency this storage does not collapse functional rows.
template <graph::unsigned_edge_weight Weight = std::uint32_t>
class indexed_weighted_adjacency {
  public:
    using weight_type = Weight;
    using vertex_type = std::uint32_t;
    using edge_type = graph::weighted_edge<Weight>;
    struct arc_type { vertex_type to; Weight weight; std::uint32_t edge_id; };
  private:
    vertex_type vertices_ = 0;
    bool transposed_ = false;
    std::vector<vertex_type> offsets_;
    std::vector<arc_type> arcs_;
    std::vector<edge_type> edges_;
  public:
    indexed_weighted_adjacency() = default;
    template <std::ranges::input_range Range>
        requires std::convertible_to<std::ranges::range_reference_t<Range>, edge_type>
    explicit indexed_weighted_adjacency(std::size_t vertices, Range&& input, bool transpose = false)
        : transposed_(transpose) {
        constexpr auto maximum = std::numeric_limits<vertex_type>::max();
        if (vertices > maximum) throw std::length_error("canard indexed graph vertex domain exceeds uint32");
        vertices_ = static_cast<vertex_type>(vertices);
        offsets_.resize(vertices + 1);
        if constexpr (std::ranges::sized_range<Range>) {
            const auto count = std::ranges::size(input);
            if (count > maximum) throw std::length_error("canard indexed graph edge domain exceeds uint32");
            edges_.reserve(static_cast<std::size_t>(count));
        }
        for (auto&& item : input) {
            const edge_type edge = item;
            if (edge.from >= vertices_ || edge.to >= vertices_)
                throw std::out_of_range("canard indexed graph endpoint");
            if (edges_.size() == maximum) throw std::length_error("canard indexed graph edge domain exhausted");
            ++offsets_[transpose ? edge.to : edge.from];
            edges_.push_back(edge);
        }
        std::partial_sum(offsets_.begin(), offsets_.end(), offsets_.begin());
        arcs_.resize(edges_.size());
        for (std::size_t i = 0; i < edges_.size(); ++i) {
            const auto edge = edges_[i];
            const auto u = transpose ? edge.to : edge.from;
            arcs_[--offsets_[u]] = {transpose ? edge.from : edge.to, edge.weight, static_cast<vertex_type>(i)};
        }
    }
    indexed_weighted_adjacency(const indexed_weighted_adjacency&) = default;
    indexed_weighted_adjacency& operator=(const indexed_weighted_adjacency& other) {
        if (this != &other) { indexed_weighted_adjacency next(other); swap(next); }
        return *this;
    }
    indexed_weighted_adjacency(indexed_weighted_adjacency&& other) noexcept { swap(other); }
    indexed_weighted_adjacency& operator=(indexed_weighted_adjacency&& other) noexcept {
        if (this != &other) { indexed_weighted_adjacency next(std::move(other)); swap(next); }
        return *this;
    }
    void swap(indexed_weighted_adjacency& other) noexcept {
        using std::swap; swap(vertices_, other.vertices_); swap(transposed_, other.transposed_);
        offsets_.swap(other.offsets_); arcs_.swap(other.arcs_); edges_.swap(other.edges_);
    }
    friend void swap(indexed_weighted_adjacency& a, indexed_weighted_adjacency& b) noexcept { a.swap(b); }
    [[nodiscard]] vertex_type vertex_count() const noexcept { return vertices_; }
    [[nodiscard]] vertex_type edge_count() const noexcept { return static_cast<vertex_type>(arcs_.size()); }
    [[nodiscard]] bool reversed() const noexcept { return transposed_; }
    [[nodiscard]] bool functional() const noexcept { return false; }
    [[nodiscard]] std::span<const arc_type> stored_arcs() const & noexcept { return arcs_; }
    void stored_arcs() const && = delete;
    [[nodiscard]] std::span<const arc_type> stored_neighbors(vertex_type vertex) const & noexcept {
        assert(vertex < vertices_);
        return std::span<const arc_type>(arcs_).subspan(offsets_[vertex], offsets_[vertex + 1] - offsets_[vertex]);
    }
    void stored_neighbors(vertex_type) const && = delete;
    [[nodiscard]] std::span<const edge_type> original_edges() const & noexcept { return edges_; }
    void original_edges() const && = delete;
    [[nodiscard]] std::vector<edge_type> release_original_edges() noexcept { return std::exchange(edges_, {}); }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return offsets_.capacity() * sizeof(vertex_type) + arcs_.capacity() * sizeof(arc_type) + edges_.capacity() * sizeof(edge_type);
    }
};
} // namespace canard::structural
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <utility>

namespace canard {
struct with_walk_witness_t {};
inline constexpr with_walk_witness_t with_walk_witness{};

struct shortest_walk_options {
    // Applies to compact length-only preprocessing. Witness indexes retain
    // explicit edge identities and always use the general sidetrack layout.
    bool functional_orbits = true;
};

namespace graph::detail {
template <graph::unsigned_edge_weight Weight, std::ranges::input_range Range>
[[nodiscard]] structural::weighted_adjacency<Weight> incoming_adjacency(std::size_t vertices, Range& edges) {
    using edge_type = graph::weighted_edge<Weight>;
    auto reversed = std::ranges::ref_view{edges} |
        std::views::transform([](const auto& item) {
            const edge_type edge = item;
            return edge_type{edge.to, edge.from, edge.weight};
        });
    return structural::weighted_adjacency<Weight>{vertices, reversed, {.reverse_functional = false}};
}
template <bool RetainWitness = false>
struct sidetrack_expander {
    using key_type = std::uint64_t;
    using state_type = std::uint32_t;
    using record_type = kernel::radix_record<key_type, state_type>;
    std::shared_ptr<const index::basic_shortest_walk_index_data<RetainWitness>> index;
    state_type source = 0;
    key_type maximum = std::numeric_limits<key_type>::max();
    // Costs beyond UINT64_MAX are outside this stream's representable domain;
    // their nonnegative descendants cannot become representable again.
    template <typename Emit>
    void operator()(record_type entry, Emit emit) const {
        auto add = [&](key_type delta, state_type state) {
            if (delta <= maximum - entry.key)
                emit(entry.key + delta, state);
        };
        if (index->layout == index::shortest_walk_layout::functional_orbit) {
            if (index->cyclic(source)) add(index->cycle_weight, 0);
            return;
        }
        kernel::sidetrack_successors(*index, source, entry.value,
            [&](key_type delta, state_type node, kernel::sidetrack_step) { add(delta, node); });
    }
};
} // namespace graph::detail

// A consuming input view. next() and iteration share one cursor. Copying
// explicitly snapshots its frontier; ordinary use moves this view instead.
// The shared immutable index is kept alive independently of the public owner.
template <typename Execution = execution::scalar, bool RetainWitness = false>
class shortest_walk_lengths : public std::ranges::view_interface<shortest_walk_lengths<Execution, RetainWitness>> {
    using expander_type = graph::detail::sidetrack_expander<RetainWitness>;
    monotone_enumerator<expander_type, Execution> enumeration_;
    std::size_t remaining_ = 0, produced_ = 0;
  public:
    using value_type = std::uint64_t;
    shortest_walk_lengths() = default;
    shortest_walk_lengths(std::shared_ptr<const index::basic_shortest_walk_index_data<RetainWitness>> index,
                          std::uint32_t source, std::size_t limit, std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max())
        : remaining_(limit) {
        if (!index || source >= index->vertices) throw std::out_of_range("canard walk source");
        std::optional<typename expander_type::record_type> initial;
        if (limit && index->distance[source] != index->unreachable && index->distance[source] <= maximum)
            initial = typename expander_type::record_type{index->distance[source], 0};
        enumeration_ = monotone_enumerator<expander_type, Execution>(expander_type{std::move(index), source, maximum}, initial);
    }
    // One-shot constructor: consumes the edge range, computes target distances,
    // and stops immediately when this source cannot reach the target. Use the
    // separate shortest_walk_index owner to amortize preprocessing over sources.
    template <std::ranges::input_range Range>
        requires (!RetainWitness) && requires { typename std::ranges::range_value_t<Range>::weight_type; } &&
                 graph::unsigned_edge_weight<typename std::ranges::range_value_t<Range>::weight_type> &&
                 std::convertible_to<std::ranges::range_reference_t<Range>,
                     graph::weighted_edge<typename std::ranges::range_value_t<Range>::weight_type>>
    shortest_walk_lengths(std::size_t vertices, Range&& edges, std::uint32_t source,
                          std::uint32_t target, std::size_t limit, Execution execution = {},
                          shortest_walk_options options = {}) {
        using weight_type = typename std::ranges::range_value_t<Range>::weight_type;
        if (source >= vertices || target >= vertices) throw std::out_of_range("canard walk endpoint");
        const auto incoming = graph::detail::incoming_adjacency<weight_type>(vertices, edges);
        if (limit == 0) return;
        std::shared_ptr<const index::basic_shortest_walk_index_data<RetainWitness>> data;
        if (options.functional_orbits && incoming.functional()) {
            data = std::make_shared<index::shortest_walk_index_data>(incoming, target, execution, true);
        } else {
            auto tree = kernel::nonnegative_shortest_tree(incoming, target, execution);
            if (tree.distance[source] == tree.unreachable) return;
            data = std::make_shared<index::shortest_walk_index_data>(incoming, target, std::move(tree));
        }
        *this = shortest_walk_lengths{std::move(data), source, limit};
    }
    shortest_walk_lengths(const shortest_walk_lengths&) = default;
    shortest_walk_lengths& operator=(const shortest_walk_lengths&) = default;
    shortest_walk_lengths(shortest_walk_lengths&& other) noexcept
        : enumeration_(std::move(other.enumeration_)),
          remaining_(std::exchange(other.remaining_, 0)), produced_(std::exchange(other.produced_, 0)) {}
    shortest_walk_lengths& operator=(shortest_walk_lengths&& other) noexcept {
        if (this != &other) {
            enumeration_ = std::move(other.enumeration_);
            remaining_ = std::exchange(other.remaining_, 0);
            produced_ = std::exchange(other.produced_, 0);
        }
        return *this;
    }
    [[nodiscard]] std::optional<value_type> next() {
        if (remaining_ == 0) return std::nullopt;
        const auto entry = enumeration_.next();
        if (!entry) { remaining_ = 0; return std::nullopt; }
        --remaining_; ++produced_;
        return entry->key;
    }
    [[nodiscard]] std::size_t produced() const noexcept { return produced_; }
    [[nodiscard]] std::size_t frontier_size() const noexcept { return enumeration_.frontier_size(); }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept { return enumeration_.allocated_bytes(); }
    class iterator {
        shortest_walk_lengths* owner_ = nullptr;
        std::optional<std::uint64_t> current_;
      public:
        using iterator_concept = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type = std::uint64_t;
        using difference_type = std::ptrdiff_t;
        iterator() = default;
        explicit iterator(shortest_walk_lengths& owner) : owner_(&owner), current_(owner.next()) {}
        value_type operator*() const { return *current_; }
        iterator& operator++() { current_ = owner_->next(); return *this; }
        void operator++(int) { ++*this; }
        friend bool operator==(const iterator& it, std::default_sentinel_t) noexcept { return !it.current_; }
    };
    iterator begin() { return iterator{*this}; }
    std::default_sentinel_t end() const noexcept { return {}; }
};

template <std::ranges::input_range Range, typename Execution = execution::scalar>
shortest_walk_lengths(std::size_t, Range&&, std::uint32_t, std::uint32_t, std::size_t,
                      Execution = {}, shortest_walk_options = {}) -> shortest_walk_lengths<Execution>;

// Fixed-target preprocessing. Copies share immutable data; streams own their
// frontier and retain the data, so interleaved sources and owner destruction are safe.
template <graph::unsigned_edge_weight Weight = std::uint32_t, typename Execution = execution::scalar, bool RetainWitness = false>
class shortest_walk_index {
    std::shared_ptr<const index::basic_shortest_walk_index_data<RetainWitness>> data_;
  public:
    using weight_type = Weight;
    using execution_type = Execution;
    using edge_type = graph::weighted_edge<Weight>;
    using vertex_type = std::uint32_t;
    using lengths_type = shortest_walk_lengths<Execution, RetainWitness>;
    static constexpr bool retains_witness = RetainWitness;
    shortest_walk_index() = default;
    template <std::ranges::input_range Range>
        requires std::convertible_to<std::ranges::range_reference_t<Range>, edge_type>
    explicit shortest_walk_index(std::size_t vertices, Range&& edges, vertex_type target, Execution execution = {}, shortest_walk_options options = {}) {
        if (target >= vertices) throw std::out_of_range("canard walk target");
        if constexpr (RetainWitness) {
            auto normalized = std::ranges::ref_view{edges} | std::views::transform([](const auto& item) {
                const edge_type edge = item;
                return graph::weighted_edge<std::uint32_t>{edge.from, edge.to, static_cast<std::uint32_t>(edge.weight)};
            });
            structural::indexed_weighted_adjacency<std::uint32_t> incoming(vertices, normalized, true);
            auto data = std::make_shared<index::shortest_walk_witness_index_data>(incoming, target, execution, false);
            data->witness.edges = incoming.release_original_edges();
            data_ = std::move(data);
        } else {
            const auto incoming = graph::detail::incoming_adjacency<Weight>(vertices, edges);
            data_ = std::make_shared<index::shortest_walk_index_data>(incoming, target, execution, options.functional_orbits);
        }
    }
    template <std::ranges::input_range Range>
        requires RetainWitness && std::convertible_to<std::ranges::range_reference_t<Range>, edge_type>
    explicit shortest_walk_index(std::size_t vertices, Range&& edges, vertex_type target,
                                 with_walk_witness_t, Execution execution = {}, shortest_walk_options options = {})
        : shortest_walk_index(vertices, std::forward<Range>(edges), target, execution, options) {}
    [[nodiscard]] shortest_walks<Execution> walks(vertex_type source,
        std::size_t limit = std::numeric_limits<std::size_t>::max(),
        std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max()) const requires RetainWitness {
        return shortest_walks<Execution>{data_, source, limit, maximum};
    }
    // One-based rank. Enumerates k candidates, not k materialized edge lists.
    [[nodiscard]] std::optional<shortest_walk_witness> kth_walk(vertex_type source, std::size_t k,
        std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max()) const requires RetainWitness {
        if (k == 0) throw std::invalid_argument("canard kth_walk rank is one-based");
        auto stream = walks(source, k, maximum);
        std::optional<shortest_walk_witness> result;
        for (std::size_t i = 0; i < k; ++i) {
            result = stream.next();
            if (!result) break;
        }
        return result;
    }
    [[nodiscard]] vertex_type vertex_count() const noexcept { return data_ ? data_->vertices : 0; }
    [[nodiscard]] vertex_type edge_count() const noexcept { return data_ ? data_->edges : 0; }
    [[nodiscard]] std::optional<vertex_type> target() const noexcept { return data_ ? std::optional{data_->target} : std::nullopt; }
    [[nodiscard]] std::optional<std::uint64_t> distance(vertex_type source) const {
        if (!data_ || source >= data_->vertices) throw std::out_of_range("canard walk source");
        const auto d = data_->distance[source];
        return d == data_->unreachable ? std::nullopt : std::optional{d};
    }
    [[nodiscard]] lengths_type lengths(vertex_type source, std::size_t limit = std::numeric_limits<std::size_t>::max(),
                                       std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max()) const {
        return lengths_type{data_, source, limit, maximum};
    }
    [[nodiscard]] index::shortest_walk_layout layout() const noexcept {
        return data_ ? data_->layout : index::shortest_walk_layout::sidetrack_heaps;
    }
    [[nodiscard]] std::size_t sidetrack_count() const noexcept { return data_ ? data_->sidetracks : 0; }
    [[nodiscard]] std::size_t heap_node_count() const noexcept { return data_ ? data_->heap.node_count() : 0; }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept { return data_ ? data_->allocated_bytes() : 0; }
    void swap(shortest_walk_index& other) noexcept { data_.swap(other.data_); }
    friend void swap(shortest_walk_index& a, shortest_walk_index& b) noexcept { a.swap(b); }
};
template <std::ranges::input_range Range, typename Execution = execution::scalar>
    requires (!std::same_as<Execution, with_walk_witness_t>)
shortest_walk_index(std::size_t, Range&&, std::uint32_t, Execution = {}, shortest_walk_options = {})
    -> shortest_walk_index<typename std::ranges::range_value_t<Range>::weight_type, Execution>;
template <std::ranges::input_range Range, typename Execution = execution::scalar>
shortest_walk_index(std::size_t, Range&&, std::uint32_t, with_walk_witness_t, Execution = {}, shortest_walk_options = {})
    -> shortest_walk_index<typename std::ranges::range_value_t<Range>::weight_type, Execution, true>;
} // namespace canard
#include <cstdint>
#include <ranges>

int main() {
    namespace io = canard::io;
    using u32 = std::uint32_t;
    using count_field = io::bounded<u32, 300000>;
    using vertex_field = io::bounded<u32, 299999>;
    using weight_field = io::bounded<u32, 10000000>;
    canard::judge::input input;
    canard::judge::output output;
    const auto [n, m, source, target, k] =
        input.read<count_field, count_field, vertex_field, vertex_field, count_field>();
    auto edges = input.read<vertex_field, vertex_field, weight_field>(m) |
        std::views::transform([](const auto& fields) {
            const auto [from, to, weight] = fields;
            return canard::graph::weighted_edge{from, to, weight};
        });
    canard::shortest_walk_lengths lengths{n, edges, source, target, k, canard::judge::execution{}};
    output.write(lengths);
    output.write(std::views::iota(lengths.produced(), std::size_t{k}) |
                 std::views::transform([](std::size_t) { return -1; }));
    output.finish();
}
