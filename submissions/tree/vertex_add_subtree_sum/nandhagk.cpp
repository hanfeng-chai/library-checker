// canard vertex_add_subtree_sum submission.
#if defined(__GNUC__) && !defined(__clang__)
#include <bits/stdc++.h>
#include <immintrin.h>
#pragma GCC target("avx2")
#ifndef __AVX2__
#define __AVX2__ 1
#endif
#endif
// Generated from canard headers and client. Edit the maintained sources.
// ===== clients/tree/vertex_add_subtree_sum.cpp =====
// ===== include/canard/io/source/checked_linux_mapped_file.hpp =====
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
// ===== include/canard/io/avx2/trusted_ascii_reader.hpp =====
// ===== include/canard/io/decimal_avx2.hpp =====
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
// ===== include/canard/io/detail/basic_trusted_ascii_reader.hpp =====
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
// ===== include/canard/io/writer.hpp =====

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
// ===== include/canard/io/detail/target.hpp =====
// ===== include/canard/execution.hpp =====
namespace canard::execution {
// Source-level portable operations. Compiler target flags still govern auto-vectorization.
struct scalar {};
// Explicit opt-in backend; no runtime dispatch or implicit fallback.
struct avx2 {};
}
// ===== include/canard/io/decimal/ascii_scalar.hpp =====
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
// ===== include/canard/io/output_buffer.hpp =====
// ===== include/canard/io/transfer.hpp =====
#include <cstddef>
#include <system_error>
namespace canard::io {
struct transfer_result {
    std::size_t count = 0;
    std::error_code error{};
};
}
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
// ===== include/canard/range/wide_segment_tree.hpp =====
// ===== include/canard/range/detail/wide_engine.hpp =====
// ===== include/canard/range/detail/wide_builder.hpp =====
// ===== include/canard/memory/wide_storage.hpp =====
// ===== include/canard/structural/wide_layout.hpp =====
// ===== include/canard/wide/configuration.hpp =====
// ===== include/canard/kernel/protocols.hpp =====
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

// Storage is an ownership/layout policy, orthogonal to representation and
// execution. The default tree owns one pair of contiguous block arrays.
// packed_forest makes wide_segment_tree a lightweight view into shared arenas.
namespace storage {
struct owning {};
struct packed_forest {};
} // namespace storage

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
    static_assert(Fanout >= 2 && Fanout <= 256 && std::has_single_bit(Fanout));
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
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <utility>

namespace canard::structural {

// Compact level-major geometry. No values, tags, arithmetic domain, or owner.
// Level zero contains blocks of leaves; a level-h slot covers B^h leaves.
template <typename Configuration = wide::configuration<>> class wide_layout {
  public:
    using size_type = std::size_t;
    static constexpr unsigned fanout = Configuration::fanout;
    static constexpr unsigned radix_bits = std::countr_zero(fanout);
    static constexpr unsigned max_height = std::max(
        1u,
        (static_cast<unsigned>(std::bit_width(Configuration::max_size - 1)) + radix_bits - 1) /
            radix_bits);

    template <unsigned Level>
    static constexpr size_type child_capacity = size_type{1} << (Level * radix_bits);
    template <unsigned Level>
    static constexpr unsigned active_entries = static_cast<unsigned>(std::min<size_type>(
        fanout, (Configuration::max_size + child_capacity<Level> - 1) / child_capacity<Level>));

  private:
    size_type size_ = 0;
    unsigned height_ = 0;
    std::array<size_type, max_height> counts_{};
    std::array<size_type, max_height> offsets_{}; // branch-array offsets; level 0 separate.
    size_type branch_count_ = 0;

  public:
    constexpr wide_layout() noexcept = default;
    explicit constexpr wide_layout(size_type n, unsigned minimum_height = 1) noexcept : size_(n) {
        // Preconditions: n <= Configuration::max_size, 1 <= minimum_height <= max_height.
        // Optional unary roots keep a fixed number of prefix levels addressable.
        // Zero is still a normal, height-zero size.
        if (n == 0)
            return;
        auto count = (n + fanout - 1) / fanout;
        counts_[0] = count;
        height_ = 1;
        while (count > 1 || height_ < minimum_height) {
            count = (count + fanout - 1) / fanout;
            counts_[height_] = count;
            offsets_[height_] = branch_count_;
            branch_count_ += count;
            ++height_;
        }
    }
    [[nodiscard]] constexpr size_type size() const noexcept {
        return size_;
    }
    [[nodiscard]] constexpr unsigned height() const noexcept {
        return height_;
    }
    [[nodiscard]] constexpr size_type blocks(unsigned level) const noexcept {
        return counts_[level];
    }
    [[nodiscard]] constexpr size_type offset(unsigned level) const noexcept {
        return offsets_[level];
    }
    [[nodiscard]] constexpr size_type leaf_blocks() const noexcept {
        return counts_[0];
    }
    [[nodiscard]] constexpr size_type branch_blocks() const noexcept {
        return branch_count_;
    }
    [[nodiscard]] constexpr size_type child_span(unsigned level) const noexcept {
        return size_type{1} << (level * radix_bits);
    }
    [[nodiscard]] constexpr unsigned valid_children(unsigned level,
                                                    size_type block) const noexcept {
        const auto children = level == 0 ? size_ : counts_[level - 1];
        return static_cast<unsigned>(std::min<size_type>(fanout, children - block * fanout));
    }
};

// A cover packet describes consecutive complete child slots, not payloads.
// Its logical interval is exactly [first, last), even at the truncated tail.
struct wide_cover_packet {
    unsigned level;
    std::size_t block;
    unsigned first_slot;
    unsigned last_slot;
    std::size_t first;
    std::size_t last;
};

namespace detail {
template <typename Layout, typename Visitor>
void visit_wide_cover_impl(const Layout& layout,
                           unsigned level,
                           std::size_t block,
                           std::size_t first,
                           std::size_t last,
                           Visitor& visit) {
    const auto span = layout.child_span(level);
    const auto base = block * Layout::fanout * span;
    const auto l = first / span;
    const auto r = (last - 1) / span;
    if (level == 0) {
        visit(wide_cover_packet{
            level, block, unsigned(l), unsigned(r + 1), base + first, base + last});
        return;
    }
    const auto full_first = (first + span - 1) / span;
    const auto full_last = last / span;
    if (l == r && (first % span || last % span)) {
        visit_wide_cover_impl(
            layout, level - 1, block * Layout::fanout + l, first % span, last - l * span, visit);
        return;
    }
    if (first % span)
        visit_wide_cover_impl(
            layout, level - 1, block * Layout::fanout + l, first % span, span, visit);
    if (full_first < full_last)
        visit(wide_cover_packet{level,
                                block,
                                unsigned(full_first),
                                unsigned(full_last),
                                base + full_first * span,
                                base + full_last * span});
    if (last % span)
        visit_wide_cover_impl(layout, level - 1, block * Layout::fanout + r, 0, last % span, visit);
}
} // namespace detail

// Increasing logical order; empty intervals produce no visits. This service
// can visit heavyweight external payloads without imposing a monoid on them.
template <typename Layout, typename Visitor>
void visit_cover(const Layout& layout, std::size_t first, std::size_t last, Visitor&& visitor) {
    if (first == last)
        return;
    detail::visit_wide_cover_impl(layout, layout.height() - 1, 0, first, last, visitor);
}

} // namespace canard::structural
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard::memory {

// Construct identity blocks without importing a sequence of leaf values.
// Used by accumulators whose representation starts with all blocks empty.
struct empty_blocks_t {};
inline constexpr empty_blocks_t empty_blocks{};

// Two typed, contiguous level-major arrays. Leaves do not pay for branch tags.
// Cached level pointers are derived state, rebased after every ownership
// change. Copies/moves never retain another owner's pointers. Ordinary tree
// operations perform no allocation.
template <typename Kernel, typename Configuration, typename Allocator = std::allocator<std::byte>>
class wide_storage {
  public:
    using layout_type = structural::wide_layout<Configuration>;
    using leaf_type = leaf_type_t<Kernel>;
    using branch_type = branch_type_t<Kernel>;
    using allocator_type = Allocator;
    using traits = std::allocator_traits<Allocator>;
    template <typename T> using rebound = typename traits::template rebind_alloc<T>;

  private:
    layout_type layout_;
    std::vector<leaf_type, rebound<leaf_type>> leaves_;
    std::vector<branch_type, rebound<branch_type>> branches_;
    std::array<branch_type*, layout_type::max_height> levels_{};

    void rebase() noexcept {
        levels_.fill(nullptr);
        if constexpr (std::same_as<leaf_type, branch_type>)
            levels_[0] = leaves_.data();
        for (unsigned level = 1; level < layout_.height(); ++level)
            levels_[level] = branches_.data() + layout_.offset(level);
    }


  public:
    explicit wide_storage(const Allocator& allocator = {})
        : leaves_(rebound<leaf_type>{allocator}), branches_(rebound<branch_type>{allocator}) {}

    wide_storage(layout_type layout, const leaf_type& empty_leaf,
                 const branch_type& empty_branch, const Allocator& allocator = {})
        : layout_(layout),
          leaves_(layout.leaf_blocks(), empty_leaf, rebound<leaf_type>{allocator}),
          branches_(layout.branch_blocks(), empty_branch, rebound<branch_type>{allocator}) {
        rebase();
    }
    wide_storage(layout_type layout, empty_blocks_t, const Kernel& kernel,
                 const Allocator& allocator = {})
        : wide_storage(layout, kernel.empty_leaf(), kernel.empty_branch(), allocator) {}

    wide_storage(const wide_storage& other)
        : wide_storage(other,
                       traits::select_on_container_copy_construction(other.get_allocator())) {}
    wide_storage(const wide_storage& other, const Allocator& allocator)
        : layout_(other.layout_), leaves_(other.leaves_, rebound<leaf_type>{allocator}),
          branches_(other.branches_, rebound<branch_type>{allocator}) {
        rebase();
    }
    wide_storage(wide_storage&& other) noexcept
        : layout_(std::exchange(other.layout_, {})), leaves_(std::move(other.leaves_)),
          branches_(std::move(other.branches_)) {
        rebase();
        other.rebase();
    }

    wide_storage& operator=(const wide_storage& other) {
        if (this == &other)
            return *this;
        const auto allocator = [&] {
            if constexpr (traits::propagate_on_container_copy_assignment::value)
                return other.get_allocator();
            else
                return get_allocator();
        }();
        wide_storage next{other, allocator}; // All throwing work precedes commit.
        if constexpr (traits::propagate_on_container_copy_assignment::value &&
                      !traits::propagate_on_container_swap::value) {
            // Adopting a different allocator forbids vector::swap here. Vector
            // move construction is noexcept: replace these non-const members
            // transparently after all allocation has succeeded.
            std::destroy_at(&leaves_);
            std::construct_at(&leaves_, std::move(next.leaves_));
            std::destroy_at(&branches_);
            std::construct_at(&branches_, std::move(next.branches_));
            layout_ = next.layout_;
            rebase();
        } else
            swap(next);
        return *this;
    }
    wide_storage& operator=(wide_storage&& other) {
        if (this == &other)
            return *this;
        if constexpr (traits::propagate_on_container_move_assignment::value) {
            leaves_ = std::move(other.leaves_);
            branches_ = std::move(other.branches_);
            layout_ = std::exchange(other.layout_, {});
            rebase();
            other.rebase();
        } else if (get_allocator() == other.get_allocator()) {
            wide_storage next{std::move(other)};
            swap(next);
        } else {
            // Unequal nonpropagating resources: stage a copy before committing.
            // This preserves both trees if allocation fails halfway through.
            wide_storage next{other, get_allocator()};
            swap(next);
            other.clear();
        }
        return *this;
    }
    void swap(wide_storage& other) noexcept(noexcept(leaves_.swap(other.leaves_)) &&
                                            noexcept(branches_.swap(other.branches_))) {
        // Standard precondition: equal resources unless allocator swap propagates.
        using std::swap;
        swap(layout_, other.layout_);
        leaves_.swap(other.leaves_);
        branches_.swap(other.branches_);
        rebase();
        other.rebase();
    }
    void clear() noexcept {
        leaves_.clear();
        branches_.clear();
        layout_ = {};
        rebase();
    }
    [[nodiscard]] Allocator get_allocator() const {
        return Allocator{leaves_.get_allocator()};
    }
    [[nodiscard]] const layout_type& layout() const noexcept {
        return layout_;
    }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return leaves_.size() * sizeof(leaf_type) + branches_.size() * sizeof(branch_type);
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return leaves_.capacity() * sizeof(leaf_type) + branches_.capacity() * sizeof(branch_type);
    }
    // Visit a block without type erasure. Homogeneous representations use one
    // level-pointer lookup; heterogeneous ones retain their distinct types.
    template <typename Function>
    decltype(auto) visit_node(unsigned level, std::size_t index, Function&& function) {
        if constexpr (std::same_as<leaf_type, branch_type>) {
            return std::forward<Function>(function)(levels_[level][index]);
        } else {
            if (level == 0)
                return std::forward<Function>(function)(leaves_[index]);
            return std::forward<Function>(function)(levels_[level][index]);
        }
    }
    template <unsigned Level> [[nodiscard]] decltype(auto) node(std::size_t index) noexcept {
        if constexpr (Level == 0)
            return (leaves_[index]);
        else
            return (levels_[Level][index]);
    }
    template <unsigned Level> [[nodiscard]] decltype(auto) node(std::size_t index) const noexcept {
        if constexpr (Level == 0)
            return (leaves_[index]);
        else
            return std::as_const(levels_[Level][index]);
    }
    [[nodiscard]] branch_type& branch(unsigned level, std::size_t index) noexcept {
        return levels_[level][index];
    }
    [[nodiscard]] leaf_type& leaf(std::size_t index) noexcept {
        return leaves_[index];
    }
    [[nodiscard]] const branch_type& branch(unsigned level, std::size_t index) const noexcept {
        return levels_[level][index];
    }
    [[nodiscard]] const leaf_type& leaf(std::size_t index) const noexcept {
        return leaves_[index];
    }
};

} // namespace canard::memory
#include <cassert>
#include <iterator>
#include <ranges>
#include <vector>
namespace canard::detail {
// Establishes summaries and optional normalized root frames. Storage only owns
// buffers; this builder alone knows import/fold/build_summary semantics.
template <typename Kernel, typename Configuration, typename Allocator = std::allocator<std::byte>>
class wide_builder {
    using storage_type = memory::wide_storage<Kernel, Configuration, Allocator>;
    using layout_type = structural::wide_layout<Configuration>;
    template <typename Iterator>
    static storage_type construct(Iterator first, std::size_t count, Kernel& kernel,
                                  const Allocator& allocator) {
        assert(count <= Configuration::max_size);
        storage_type storage{layout_type{count}, kernel.empty_leaf(), kernel.empty_branch(), allocator};
        if (count == 0)
            return storage;
        constexpr auto B = Configuration::fanout;
        for (std::size_t i = 0; i < count; ++i, ++first)
            kernel.write_slot(storage.leaf(i / B), unsigned(i % B),
                              kernel.import_value(static_cast<value_type_t<Kernel>>(*first)));
        const auto finalize = [&]<typename Block>(Block& block, bool root) {
            if constexpr (requires { kernel.build_summary(block, root); })
                return kernel.build_summary(block, root);
            else return kernel.fold(block, 0, B);
        };
        const auto& layout = storage.layout();
        for (unsigned level = 1; level < layout.height(); ++level) {
            for (std::size_t child = 0; child < layout.blocks(level - 1); ++child) {
                auto value = level == 1 ? finalize(storage.leaf(child), false)
                                       : finalize(storage.branch(level - 1, child), false);
                kernel.write_slot(storage.branch(level, child / B), unsigned(child % B), value);
            }
        }
        if constexpr (requires { kernel.build_summary(storage.leaf(0), true); }) {
            if (layout.height() == 1)
                finalize(storage.leaf(0), true);
            else finalize(storage.branch(layout.height() - 1, 0), true);
        }
        return storage;
    }
  public:
    template <std::ranges::input_range R>
    static storage_type build(R&& values, Kernel& kernel, const Allocator& allocator = {}) {
        if constexpr (std::ranges::sized_range<R>)
            return construct(std::ranges::begin(values), std::ranges::size(values), kernel, allocator);
        else if constexpr (std::ranges::forward_range<R>)
            return construct(std::ranges::begin(values), std::ranges::distance(values), kernel, allocator);
        else {
            using V = value_type_t<Kernel>;
            using rebound = typename std::allocator_traits<Allocator>::template rebind_alloc<V>;
            std::vector<V, rebound> staging{rebound{allocator}};
            for (auto&& value : values)
                staging.emplace_back(value);
            return construct(staging.begin(), staging.size(), kernel, allocator);
        }
    }
};
}
// ===== include/canard/kernel/wide_scalar.hpp =====
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
// ===== include/canard/utility/array.hpp =====
#include <array>
#include <utility>
#include <cstddef>
namespace canard::utility {
template <std::size_t N, typename T> [[nodiscard]] constexpr std::array<T, N> repeat(const T& x) {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return std::array<T, N>{(static_cast<void>(I), x)...};
    }(std::make_index_sequence<N>{});
}
}
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace canard::kernel {
namespace detail {
template <typename Action, unsigned B, bool Lazy> struct tag_storage {};
template <typename Action, unsigned B> struct tag_storage<Action, B, true> {
    std::array<tag_type_t<Action>, B> tags;
    std::uint64_t dirty = 0;
};
} // namespace detail

// Portable reference block implementation. It is generic in the scalar
// algebra; only this block layer knows the physical array representation.
// Other backends can replace fold/transform/repair without replacing traversal.
template <algebra::monoid Monoid, typename Action, unsigned Fanout>
    requires algebra::action_for<Action, Monoid>
struct wide_scalar {
    using value_type = value_type_t<Monoid>;
    using summary_type = value_type;
    using update_type = tag_type_t<Action>;
    using action_type = update_type;
    using prepared_action = action_type;
    static constexpr bool has_lazy = !std::same_as<Action, algebra::no_action>;
    static constexpr unsigned fanout = Fanout;

    struct alignas(64) leaf_type {
        std::array<summary_type, Fanout> values;
    };
    struct alignas(64) branch_type {
        std::array<summary_type, Fanout> values;
        [[no_unique_address]] detail::tag_storage<Action, Fanout, has_lazy> lazy;
    };

    // Generic repair returns a replacement subtotal. There is no subtraction,
    // group operation, delta-combination law, or equality test assumed here.
    struct local_change {};
    struct subtree_change {
        summary_type value;
    };

    [[no_unique_address]] Monoid monoid;
    [[no_unique_address]] Action action;

    static constexpr bool nothrow_mutation = algebra::nothrow_action_operations_v<Monoid, Action>;

    [[nodiscard]] leaf_type empty_leaf() const {
        return {utility::repeat<Fanout>(monoid.identity())};
    }
    [[nodiscard]] branch_type empty_branch() const {
        if constexpr (has_lazy)
            return {utility::repeat<Fanout>(monoid.identity()),
                    {utility::repeat<Fanout>(action.identity()), 0}};
        else
            return {utility::repeat<Fanout>(monoid.identity()), {}};
    }
    [[nodiscard]] summary_type identity() const noexcept(nothrow_mutation) {
        return monoid.identity();
    }
    [[nodiscard]] summary_type combine(const summary_type& x, const summary_type& y) const
        noexcept(nothrow_mutation) {
        return monoid.combine(x, y);
    }
    [[nodiscard]] summary_type import_value(const value_type& x) const noexcept(nothrow_mutation) {
        return x;
    }
    [[nodiscard]] value_type export_value(const summary_type& x) const noexcept(nothrow_mutation) {
        return x;
    }

    template <typename Block>
    [[nodiscard]] const summary_type& slot(const Block& block, unsigned i) const noexcept {
        return block.values[i];
    }
    template <typename Block>
    void write_slot(Block& block, unsigned i, const summary_type& x) const
        noexcept(nothrow_mutation) {
        block.values[i] = x;
    }

    template <unsigned Active = Fanout, typename Block>
    [[nodiscard]] summary_type fold(const Block& block, unsigned first, unsigned last) const
        noexcept(nothrow_mutation) {
        static_assert(Active <= Fanout);
        auto answer = identity();
        for (auto i = first; i < last; ++i)
            answer = combine(answer, slot(block, i));
        return answer;
    }

    [[nodiscard]] action_type identity_action() const noexcept(nothrow_mutation) {
        return action.identity();
    }
    [[nodiscard]] prepared_action prepare(const update_type& update) const
        noexcept(nothrow_mutation) {
        return update;
    }
    [[nodiscard]] prepared_action prepare_internal(const action_type& update) const
        noexcept(nothrow_mutation) {
        return update;
    }
    [[nodiscard]] action_type compose(const action_type& newer, const action_type& older) const
        noexcept(nothrow_mutation) {
        return action.compose(newer, older);
    }
    [[nodiscard]] summary_type map(const action_type& f,
                                   const summary_type& x,
                                   std::size_t count) const noexcept(nothrow_mutation) {
        return action.map(f, x, count);
    }

    [[nodiscard]] bool needs_materialization(const branch_type& node,
                                             unsigned child) const noexcept {
        if constexpr (has_lazy)
            return (node.lazy.dirty >> child) & 1;
        else
            return false;
    }
    [[nodiscard]] action_type child_frame(const branch_type& node, unsigned child) const
        noexcept(nothrow_mutation) {
        if constexpr (has_lazy)
            return node.lazy.tags[child];
        else
            return {};
    }
    void clear_frame(branch_type& node, unsigned child) const noexcept(nothrow_mutation) {
        if constexpr (has_lazy) {
            node.lazy.tags[child] = action.identity();
            node.lazy.dirty &= ~(std::uint64_t{1} << child);
        }
    }

    [[nodiscard]] local_change begin_changes() const noexcept {
        return {};
    }
    void account_local(local_change&, local_change) const noexcept {}
    [[nodiscard]] local_change
    repair_slot(branch_type& node, unsigned child, const subtree_change& change) const
        noexcept(nothrow_mutation) {
        write_slot(node, child, change.value);
        return {};
    }

    template <bool ReturnChange, typename Block>
    [[nodiscard]] subtree_change finish(const Block& node, local_change) const
        noexcept(nothrow_mutation) {
        if constexpr (ReturnChange)
            return {fold(node, 0, Fanout)};
        else
            return {identity()};
    }

    // Each addressed slot is an entire child with exactly child_length real
    // leaves. Traversal never sends a padded slot here.
    template <bool ReturnChange, unsigned Active = Fanout, typename Block>
    local_change apply_slots(Block& node,
                             unsigned first,
                             unsigned last,
                             const prepared_action& f,
                             std::size_t child_length) const noexcept(nothrow_mutation) {
        static_assert(Active <= Fanout);
        for (auto i = first; i < last; ++i)
            apply_slot(node, i, f, child_length);
        return {};
    }

    // A carried action exists only on a geometrically complete subtree.
    // All Fanout slots here represent real children. Apply incoming outside
    // the complete update interval and (update o incoming) inside it.
    template <typename Block>
    local_change apply_carried(Block& node,
                               unsigned first,
                               unsigned last,
                               const prepared_action& update,
                               const action_type& incoming,
                               std::size_t child_length) const noexcept(nothrow_mutation) {
        const auto after = compose(update, incoming);
        for (unsigned i = 0; i < Fanout; ++i)
            apply_slot(node, i, first <= i && i < last ? after : incoming, child_length);
        return {};
    }

    [[nodiscard]] subtree_change
    replace(leaf_type& node, unsigned child, const summary_type& value) const
        noexcept(nothrow_mutation) {
        write_slot(node, child, value);
        return finish<true>(node, {});
    }

  private:
    template <typename Block>
    void apply_slot(Block& node, unsigned child, const action_type& f, std::size_t length) const
        noexcept(nothrow_mutation) {
        node.values[child] = map(f, node.values[child], length);
        if constexpr (has_lazy && std::same_as<Block, branch_type>) {
            node.lazy.tags[child] = compose(f, node.lazy.tags[child]);
            node.lazy.dirty |= std::uint64_t{1} << child;
        }
    }
};

} // namespace canard::kernel

namespace canard::wide {
template <algebra::monoid Monoid, typename Action, unsigned Fanout>
    requires algebra::action_for<Action, Monoid>
struct kernel_binding<representation::ordinary, execution::scalar, Monoid, Action, Fanout> {
    using type = canard::kernel::wide_scalar<Monoid, Action, Fanout>;
};
} // namespace canard::wide
// ===== include/canard/kernel/wide_block_edit.hpp =====
#include <array>
#include <cstddef>

namespace canard::kernel {

// An edge change is interpreted by the block policy: it may be a replacement
// summary, an additive difference, or a change of coordinates.
template <typename Change> struct child_change {
    unsigned slot;
    Change change;
};

// One local update packet. Apply the prepared action to [first,last), and
// install BoundaryCount already-computed child changes outside that interval.
// Boundary slots are distinct and real. The packet describes work, not a
// run-time enumeration of how the block happened to be modified.
template <typename Change, std::size_t BoundaryCount> struct block_edit {
    unsigned first;
    unsigned last;
    std::array<child_change<Change>, BoundaryCount> boundaries;
};

} // namespace canard::kernel
// ===== include/canard/range/detail/wide_boundary_fold.hpp =====
#include <cstddef>
#include <utility>

namespace canard::detail {

// Shared two-boundary traversal. Reader owns the meaning of a block and of
// moving an observation through a parent edge. This file knows no tags, SIMD,
// sums, minima, fields, inverses, or payload layout.
template <typename Layout, typename Reader> class wide_boundary_fold {
    using value_type = summary_type_t<Reader>;
    static constexpr auto B = Layout::fanout;
    const Layout& layout_;
    const Reader& read_;

    template <unsigned Level>
    [[nodiscard]] value_type finish(std::size_t index, value_type value, std::size_t count) const {
        if constexpr (Reader::needs_lift && Level < Layout::max_height) {
            if (Level < layout_.height()) {
                value = read_.template lift<Level>(index / B, unsigned(index % B), value, count);
                return finish<Level + 1>(index / B, std::move(value), count);
            }
        }
        return value;
    }

    template <unsigned Level>
    [[nodiscard]] value_type join(std::size_t left,
                                  std::size_t right,
                                  value_type suffix,
                                  value_type prefix,
                                  std::size_t suffix_count,
                                  std::size_t prefix_count,
                                  std::size_t total_count) const {
        const auto l = unsigned(left % B), r = unsigned(right % B);
        if constexpr (Reader::needs_lift) {
            suffix = read_.template lift<Level>(left / B, l, suffix, suffix_count);
            prefix = read_.template lift<Level>(right / B, r, prefix, prefix_count);
        }
        if (left / B == right / B) {
            // No commutativity: suffix, middle, prefix are in that order.
            auto middle = read_.template fold_slots<Level>(left / B, l + 1, r);
            auto total = read_.combine(read_.combine(suffix, middle), prefix);
            return finish<Level + 1>(left / B, std::move(total), total_count);
        }
        if constexpr (Level + 1 < Layout::max_height) {
            suffix = read_.combine(suffix, read_.template fold_slots<Level>(left / B, l + 1, B));
            prefix = read_.combine(read_.template fold_slots<Level>(right / B, 0, r), prefix);
            constexpr auto span = Layout::template child_capacity<Level>;
            return join<Level + 1>(left / B,
                                   right / B,
                                   std::move(suffix),
                                   std::move(prefix),
                                   suffix_count + (B - l - 1) * span,
                                   prefix_count + r * span,
                                   total_count);
        } else
            std::unreachable();
    }

  public:
    wide_boundary_fold(const Layout& layout, const Reader& reader) noexcept
        : layout_(layout), read_(reader) {}

    [[nodiscard]] value_type operator()(std::size_t first, std::size_t last) const {
        if (first == last)
            return read_.identity();
        const auto left = first / B, right = (last - 1) / B;
        const auto l = unsigned(first % B), r = unsigned((last - 1) % B) + 1;
        if (left == right)
            return finish<1>(left, read_.template fold_slots<0>(left, l, r), last - first);
        if constexpr (Layout::max_height > 1) {
            auto suffix = read_.template fold_slots<0>(left, l, B);
            auto prefix = read_.template fold_slots<0>(right, 0, r);
            return join<1>(
                left, right, std::move(suffix), std::move(prefix), B - l, r, last - first);
        } else
            std::unreachable();
    }
};

} // namespace canard::detail
// ===== include/canard/range/detail/wide_upward_update.hpp =====
#include <array>
#include <cstddef>
#include <utility>

namespace canard::detail {

// Upward-only execution law. Block edits commute with ancestor frames; each
// edited subtree supplies a change certificate for its parent. Neither the
// scheduler nor its access adapter assumes a numeric interpretation of it.
// Unlike lazy descent, this schedule does not need level-specialized child
// lengths. A loop keeps the same hot block code and registers across levels.
template <typename Layout, typename Access> class wide_upward_update {
    using delta_type = delta_type_t<Access>;
    using action_type = action_type_t<Access>;
    using edge = edge_t<Access>;
    template <std::size_t Count> using edit = edit_type_t<Access, Count>;
    static constexpr auto B = Layout::fanout;
    const Layout& layout_;
    Access access_;

    void ascend(std::size_t child, unsigned level, delta_type change) const {
        for (; level < layout_.height() && !access_.is_identity(change); ++level) {
            change = access_.repair(level, child / B, unsigned(child % B), change);
            child /= B;
        }
        access_.commit_root(change);
    }

  public:
    wide_upward_update(const Layout& layout, Access access) noexcept
        : layout_(layout), access_(std::move(access)) {}

    void operator()(std::size_t first, std::size_t last, action_type update) const {
        auto left = first / B, right = (last - 1) / B;
        auto l = unsigned(first % B), r = unsigned((last - 1) % B) + 1;
        if (left == right) {
            const auto change = access_.edit_block(0, left, edit<0>{l, r, {}}, update);
            ascend(left, 1, change);
            return;
        }
        auto [dl, dr] =
            access_.edit_pair(0, left, edit<0>{l, B, {}}, right, edit<0>{0, r, {}}, update);
        for (unsigned level = 1;; ++level) {
            l = unsigned(left % B);
            r = unsigned(right % B);
            left /= B;
            right /= B;
            if (left == right) {
                const auto change = access_.edit_block(
                    level, left, edit<2>{l + 1, r, {edge{l, dl}, edge{r, dr}}}, update);
                ascend(left, level + 1, change);
                return;
            }
            auto changes = access_.edit_pair(level,
                                             left,
                                             edit<1>{l + 1, B, {edge{l, dl}}},
                                             right,
                                             edit<1>{0, r, {edge{r, dr}}},
                                             update);
            dl = std::move(changes.first);
            dr = std::move(changes.second);
        }
    }
};
} // namespace canard::detail
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard::detail {

// ONE block-summary engine for point trees and lazy trees. The kernel
// supplies a scalar reference or a packed/SIMD block implementation. Optional
// delta repair is local to the kernel; generic monoids return replacements.
template <typename Kernel, typename Configuration, typename Allocator> class wide_engine {
  public:
    using kernel_type = Kernel;
    using value_type = value_type_t<Kernel>;
    using summary_type = summary_type_t<Kernel>;
    using update_type = update_type_t<Kernel>;
    using action_type = action_type_t<Kernel>;
    using prepared_action = prepared_action_t<Kernel>;
    using subtree_change = subtree_change_t<Kernel>;
    using allocator_type = Allocator;
    using layout_type = structural::wide_layout<Configuration>;
    using storage_type = memory::wide_storage<Kernel, Configuration, Allocator>;
    static constexpr unsigned fanout = Configuration::fanout;
    static constexpr bool has_lazy = Kernel::has_lazy;
    static constexpr bool nothrow_mutation = Kernel::nothrow_mutation;
    static constexpr bool needs_lift = [] {
        if constexpr (requires { Kernel::needs_lift; })
            return Kernel::needs_lift;
        else
            return has_lazy;
    }();

  private:
    static constexpr auto B = fanout;
    [[no_unique_address]] Kernel kernel_;
    storage_type storage_;

    static Kernel construction_kernel(Kernel kernel) {
        if constexpr (requires { kernel.reset_build(); })
            kernel.reset_build();
        return kernel;
    }

    template <unsigned Level = 0, typename Function>
    decltype(auto) with_root(Function&& function) const {
        if (height() == Level + 1)
            return function.template operator()<Level>();
        if constexpr (Level + 1 < layout_type::max_height)
            return with_root<Level + 1>(std::forward<Function>(function));
        else
            std::unreachable();
    }

    template <unsigned Level, bool ReturnChange>
    auto
    transform(std::size_t index, unsigned first, unsigned last, const prepared_action& update) {
        return kernel_
            .template apply_slots<ReturnChange, layout_type::template active_entries<Level>>(
                storage_.template node<Level>(index),
                first,
                last,
                update,
                layout_type::template child_capacity<Level>);
    }

    template <unsigned Level>
    local_change_t<Kernel> descend(std::size_t index,
                                   unsigned child,
                                   std::size_t first,
                                   std::size_t last,
                                   const prepared_action& update) {
        auto& parent = storage_.template node<Level>(index);
        const auto changed = [&] {
            if (kernel_.needs_materialization(parent, child)) {
                const auto incoming = kernel_.child_frame(parent, child);
                kernel_.clear_frame(parent, child);
                return update_carried<Level - 1>(index * B + child, first, last, update, incoming);
            }
            return update_impl<Level - 1, true>(index * B + child, first, last, update);
        }();
        return kernel_.repair_slot(parent, child, changed);
    }

    template <unsigned Level>
    subtree_change update_carried(std::size_t index,
                                  std::size_t first,
                                  std::size_t last,
                                  const prepared_action& update,
                                  const action_type& incoming) {
        auto& node = storage_.template node<Level>(index);
        constexpr auto span = layout_type::template child_capacity<Level>;
        constexpr auto mask = span - 1;
        const auto full_first = unsigned((first + mask) / span);
        const auto full_last = std::max(full_first, unsigned(last / span));
        auto changes = kernel_.apply_carried(node, full_first, full_last, update, incoming, span);
        if constexpr (Level > 0) {
            const auto l = unsigned(first / span), r = unsigned((last - 1) / span);
            if (l == r) {
                if ((first & mask) || (last & mask))
                    kernel_.account_local(
                        changes, descend<Level>(index, l, first & mask, last - l * span, update));
            } else {
                if (first & mask)
                    kernel_.account_local(changes,
                                          descend<Level>(index, l, first & mask, span, update));
                if (last & mask)
                    kernel_.account_local(changes,
                                          descend<Level>(index, r, 0, last & mask, update));
            }
        }
        return kernel_.template finish<true>(node, changes);
    }

    template <unsigned Level>
    [[nodiscard]] summary_type prefix_impl(std::size_t index, std::size_t last) const
        requires(!needs_lift)
    {
        const auto& node = storage_.template node<Level>(index);
        if constexpr (Level == 0) {
            return kernel_.template fold<layout_type::template active_entries<0>>(
                node, 0, unsigned(last));
        } else {
            constexpr auto span = layout_type::template child_capacity<Level>;
            const auto child = unsigned(last / span);
            const auto remainder = last - std::size_t(child) * span;
            auto answer = kernel_.template fold<layout_type::template active_entries<Level>>(
                node, 0, child);
            if (remainder)
                answer = kernel_.combine(
                    answer, prefix_impl<Level - 1>(index * B + child, remainder));
            return answer;
        }
    }

    template <unsigned Level, bool ReturnChange>
    subtree_change update_impl(std::size_t index,
                               std::size_t first,
                               std::size_t last,
                               const prepared_action& update) {
        auto& node = storage_.template node<Level>(index);
        if constexpr (Level == 0) {
            const auto changed =
                transform<0, ReturnChange>(index, unsigned(first), unsigned(last), update);
            return kernel_.template finish<ReturnChange>(node, changed);
        } else {
            constexpr auto span = layout_type::template child_capacity<Level>;
            constexpr auto mask = span - 1;
            const auto l = unsigned(first / span), r = unsigned((last - 1) / span);
            auto changes = kernel_.begin_changes();
            if (l == r) {
                if ((first & mask) == 0 && (last & mask) == 0)
                    kernel_.account_local(changes,
                                          transform<Level, ReturnChange>(index, l, l + 1, update));
                else
                    kernel_.account_local(
                        changes, descend<Level>(index, l, first & mask, last - l * span, update));
            } else {
                if (first & mask)
                    kernel_.account_local(changes,
                                          descend<Level>(index, l, first & mask, span, update));
                if (last & mask)
                    kernel_.account_local(changes,
                                          descend<Level>(index, r, 0, last & mask, update));
                const auto full_first = unsigned((first + mask) / span);
                const auto full_last = unsigned(last / span);
                if (full_first < full_last)
                    kernel_.account_local(
                        changes,
                        transform<Level, ReturnChange>(index, full_first, full_last, update));
            }
            return kernel_.template finish<ReturnChange>(node, changes);
        }
    }

    template <unsigned Level> void materialize_child(std::size_t index, unsigned child) {
        if constexpr (has_lazy) {
            auto& parent = storage_.template node<Level>(index);
            if (kernel_.needs_materialization(parent, child)) {
                const auto prepared = kernel_.prepare_internal(kernel_.child_frame(parent, child));
                transform<Level - 1, false>(index * B + child, 0, B, prepared);
                kernel_.clear_frame(parent, child);
            }
        }
    }

    template <unsigned Level>
    void point_accumulate_impl(std::size_t index, std::size_t position, const value_type& delta)
        noexcept(noexcept(std::declval<Kernel&>().point_accumulate(
            std::declval<leaf_type_t<Kernel>&>(), unsigned{}, std::declval<const value_type&>())))
        requires requires(Kernel& k, leaf_type_t<Kernel>& leaf, const value_type& d) {
            k.point_accumulate(leaf, unsigned{}, d);
        }
    {
        auto& node = storage_.template node<Level>(index);
        if constexpr (Level == 0) {
            kernel_.point_accumulate(node, unsigned(position), delta);
        } else {
            constexpr auto span = layout_type::template child_capacity<Level>;
            const auto child = unsigned(position / span);
            kernel_.point_accumulate(node, child, delta);
            point_accumulate_impl<Level - 1>(index * B + child, position - child * span, delta);
        }
    }

    template <unsigned Level>
    subtree_change
    replace_impl(std::size_t index, std::size_t position, const summary_type& value) {
        auto& node = storage_.template node<Level>(index);
        if constexpr (Level == 0)
            return kernel_.replace(node, unsigned(position), value);
        else {
            constexpr auto span = layout_type::template child_capacity<Level>;
            const auto child = unsigned(position / span);
            materialize_child<Level>(index, child);
            const auto local_value = [&] {
                if constexpr (requires { kernel_.descend_value(node, child, value); })
                    return kernel_.descend_value(node, child, value);
                else
                    return value;
            }();
            const auto change =
                replace_impl<Level - 1>(index * B + child, position % span, local_value);
            const auto changes = kernel_.repair_slot(node, child, change);
            return kernel_.template finish<true>(node, changes);
        }
    }

    struct reader {
        using summary_type = summary_type_t<Kernel>;
        static constexpr bool needs_lift = wide_engine::needs_lift;
        const wide_engine& owner;
        [[nodiscard]] summary_type identity() const {
            return owner.kernel_.identity();
        }
        [[nodiscard]] summary_type combine(const summary_type& x, const summary_type& y) const {
            return owner.kernel_.combine(x, y);
        }
        template <unsigned Level>
        [[nodiscard]] summary_type
        fold_slots(std::size_t index, unsigned first, unsigned last) const {
            return owner.kernel_.template fold<layout_type::template active_entries<Level>>(
                owner.storage_.template node<Level>(index), first, last);
        }
        template <unsigned Level>
        [[nodiscard]] summary_type lift(std::size_t index,
                                        unsigned child,
                                        const summary_type& value,
                                        std::size_t count) const {
            const auto frame =
                owner.kernel_.child_frame(owner.storage_.template node<Level>(index), child);
            if constexpr (requires { owner.kernel_.map_nonempty(frame, value, count); })
                return owner.kernel_.map_nonempty(frame, value, count);
            else
                return owner.kernel_.map(frame, value, count);
        }
    };

    // Local block adapter for the upward execution law. There is no knowledge
    // of minima or translations in the boundary scheduler or this adapter.
    struct upward_access {
        using delta_type = subtree_change;
        using action_type = prepared_action;
        using edge = kernel::child_change<delta_type>;
        template <std::size_t Edits> using edit_type = kernel::block_edit<delta_type, Edits>;
        wide_engine& owner;
        bool is_identity(const delta_type& d) const {
            return owner.kernel_.change_is_identity(d);
        }
        void commit_root(const delta_type& d) const {
            owner.kernel_.commit_root(d);
        }
        template <std::size_t Edits>
        delta_type edit_block(unsigned level,
                              std::size_t index,
                              const edit_type<Edits>& edit,
                              action_type action) const {
            return owner.storage_.visit_node(level, index, [&](auto& block) {
                if constexpr (requires {
                                  owner.kernel_.template edit_block<B>(
                                      block, edit, action, unsigned{});
                              }) {
                    return owner.kernel_.template edit_block<B>(
                        block, edit, action, owner.layout().valid_children(level, index));
                } else {
                    auto changes = owner.kernel_.begin_changes();
                    if constexpr (Edits != 0)
                        for (const auto& boundary : edit.boundaries)
                            owner.kernel_.account_local(
                                changes,
                                owner.kernel_.repair_slot(block, boundary.slot, boundary.change));
                    if (edit.first < edit.last)
                        owner.kernel_.account_local(changes,
                                                    owner.kernel_.template apply_slots<true, B>(
                                                        block,
                                                        edit.first,
                                                        edit.last,
                                                        action,
                                                        owner.layout().child_span(level)));
                    return owner.kernel_.template finish<true>(block, changes);
                }
            });
        }
        template <std::size_t Edits>
        std::pair<delta_type, delta_type> edit_pair(unsigned level,
                                                    std::size_t left,
                                                    const edit_type<Edits>& left_edit,
                                                    std::size_t right,
                                                    const edit_type<Edits>& right_edit,
                                                    action_type action) const {
            // Both blocks are at the same level and are distinct. The optional
            // pair hook can stage independent writes before either wide load;
            // absent that hook the same packets execute sequentially.
            return owner.storage_.visit_node(level, left, [&](auto& lhs) {
                return owner.storage_.visit_node(level, right, [&](auto& rhs) {
                    if constexpr (requires {
                                      owner.kernel_.edit_pair(lhs,
                                                              left_edit,
                                                              rhs,
                                                              right_edit,
                                                              action,
                                                              unsigned{},
                                                              unsigned{});
                                  }) {
                        return owner.kernel_.edit_pair(lhs,
                                                       left_edit,
                                                       rhs,
                                                       right_edit,
                                                       action,
                                                       owner.layout().valid_children(level, left),
                                                       owner.layout().valid_children(level, right));
                    } else
                        return std::pair{edit_block(level, left, left_edit, action),
                                         edit_block(level, right, right_edit, action)};
                });
            });
        }
        delta_type
        repair(unsigned level, std::size_t index, unsigned slot, const delta_type& d) const {
            return owner.storage_.visit_node(level, index, [&](auto& block) {
                if constexpr (requires { owner.kernel_.repair_one(block, slot, d); })
                    return owner.kernel_.repair_one(block, slot, d);
                else
                    return owner.kernel_.template finish<true>(
                        block, owner.kernel_.repair_slot(block, slot, d));
            });
        }
    };

    // Shared ordered search for both directions. Carries are read-only and are
    // composed as (outer o local), never in the reverse chronological order.
    template <bool Rightward, unsigned Level, typename Predicate>
    std::size_t search(std::size_t index,
                       std::size_t base,
                       std::size_t boundary,
                       const action_type& outer,
                       summary_type& accumulator,
                       Predicate& predicate) const {
        constexpr auto span = layout_type::template child_capacity<Level>;
        const auto& node = storage_.template node<Level>(index);
        const auto active = layout().valid_children(Level, index);
        for (unsigned step = 0; step < active; ++step) {
            const auto slot = Rightward ? step : active - 1 - step;
            const auto first = base + slot * span;
            const auto last = std::min(size(), first + span);
            if constexpr (Rightward) {
                if (last <= boundary)
                    continue;
            } else {
                if (first >= boundary)
                    continue;
            }
            const bool full = Rightward ? first >= boundary : last <= boundary;
            if (full) {
                const auto part = kernel_.map(outer, kernel_.slot(node, slot), last - first);
                auto combined = Rightward ? kernel_.combine(accumulator, part)
                                          : kernel_.combine(part, accumulator);
                if (std::invoke(predicate, kernel_.export_value(combined))) {
                    accumulator = std::move(combined);
                    continue;
                }
                if constexpr (Level == 0)
                    return Rightward ? first : last;
            }
            if constexpr (Level > 0) {
                const auto carry = kernel_.compose(outer, kernel_.child_frame(node, slot));
                const auto result = search<Rightward, Level - 1>(
                    index * B + slot, first, boundary, carry, accumulator, predicate);
                if (result != (Rightward ? size() : 0))
                    return result;
            }
        }
        return Rightward ? size() : 0;
    }

    template <unsigned Level, typename Output>
    void write_values(std::size_t index, const action_type& outer, Output& output) const {
        const auto& node = storage_.template node<Level>(index);
        const auto active = layout().valid_children(Level, index);
        for (unsigned child = 0; child < active; ++child) {
            if constexpr (Level == 0) {
                *output = kernel_.export_value(kernel_.map(outer, kernel_.slot(node, child), 1));
                ++output;
            } else {
                const auto carry = kernel_.compose(outer, kernel_.child_frame(node, child));
                write_values<Level - 1>(index * B + child, carry, output);
            }
        }
    }

  public:
    template <std::ranges::input_range R>
    wide_engine(R&& values, Kernel kernel, const Allocator& allocator = {})
        : kernel_(construction_kernel(std::move(kernel))),
          storage_(wide_builder<Kernel, Configuration, Allocator>::build(
              std::forward<R>(values), kernel_, allocator)) {
        if constexpr (requires { Kernel::max_count; })
            static_assert(Configuration::max_size <= Kernel::max_count,
                          "The selected block kernel needs a smaller maximum count.");
    }
    wide_engine(const wide_engine&) = default;
    wide_engine(const wide_engine& other, const Allocator& allocator)
        : kernel_(other.kernel_), storage_(other.storage_, allocator) {}
    // Copying the (usually empty) algebra objects keeps a moved-from empty tree
    // in a valid domain. The owned O(n) arrays themselves are transferred.
    wide_engine(wide_engine&& other) noexcept(std::is_nothrow_copy_constructible_v<Kernel>)
        : kernel_(other.kernel_), storage_(std::move(other.storage_)) {}
    wide_engine& operator=(const wide_engine& other)
        requires(std::is_nothrow_copy_assignable_v<Kernel> &&
                 requires(storage_type& s) { s = std::as_const(s); })
    {
        if (this != &other) {
            storage_ = other.storage_;
            kernel_ = other.kernel_;
        }
        return *this;
    }
    wide_engine& operator=(wide_engine&& other)
        requires(std::is_nothrow_copy_assignable_v<Kernel>)
    {
        if (this != &other) {
            storage_ = std::move(other.storage_);
            kernel_ = other.kernel_;
        }
        return *this;
    }
    void swap(wide_engine& other) noexcept
        requires(std::is_nothrow_swappable_v<Kernel>)
    {
        using std::swap;
        storage_.swap(other.storage_);
        swap(kernel_, other.kernel_);
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return layout().size();
    }
    [[nodiscard]] bool empty() const noexcept {
        return size() == 0;
    }
    [[nodiscard]] unsigned height() const noexcept {
        return layout().height();
    }
    [[nodiscard]] const layout_type& layout() const noexcept {
        return storage_.layout();
    }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return storage_.storage_bytes();
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return storage_.allocated_bytes();
    }
    [[nodiscard]] Allocator get_allocator() const {
        return storage_.get_allocator();
    }
    [[nodiscard]] const Kernel& block_kernel() const noexcept {
        return kernel_;
    }

    [[nodiscard]] value_type fold(std::size_t first, std::size_t last) const {
        if constexpr (requires { kernel_.root_summary(); })
            if (first == 0 && last != 0 && last == size())
                return kernel_.export_value(kernel_.root_summary());
        if constexpr (!needs_lift) {
            if (first == last)
                return kernel_.export_value(kernel_.identity());
            const auto left = first / B, right = (last - 1) / B;
            const auto l = unsigned(first % B), r = unsigned((last - 1) % B) + 1;
            if (left == right)
                return kernel_.export_value(
                    kernel_.template fold<layout_type::template active_entries<0>>(
                        storage_.template node<0>(left), l, r));
            if (right - left <= 8 && layout().height() > 1) {
                auto answer = kernel_.template fold<layout_type::template active_entries<0>>(
                    storage_.template node<0>(left), l, B);
                for (auto block = left + 1; block < right; ++block) {
                    const auto& parent = storage_.template node<1>(block / B);
                    answer = kernel_.combine(answer, kernel_.slot(parent, unsigned(block % B)));
                }
                const auto tail = kernel_.template fold<layout_type::template active_entries<0>>(
                    storage_.template node<0>(right), 0, r);
                return kernel_.export_value(kernel_.combine(answer, tail));
            }
        }
        return kernel_.export_value(wide_boundary_fold{layout(), reader{*this}}(first, last));
    }
    [[nodiscard]] value_type prefix_fold(std::size_t last) const
        requires(!needs_lift)
    {
        if (last == 0)
            return kernel_.export_value(kernel_.identity());
        return with_root([&]<unsigned Level> {
            return kernel_.export_value(prefix_impl<Level>(0, last));
        });
    }
    [[nodiscard]] value_type get(std::size_t position) const {
        if constexpr (!needs_lift)
            return kernel_.export_value(
                kernel_.slot(storage_.template node<0>(position / B), unsigned(position % B)));
        else
            return fold(position, position + 1);
    }
    [[nodiscard]] value_type all_fold() const {
        if (empty())
            return kernel_.export_value(kernel_.identity());
        if constexpr (requires { kernel_.root_summary(); })
            return kernel_.export_value(kernel_.root_summary());
        return with_root([&]<unsigned Level> {
            return kernel_.export_value(
                kernel_.template fold<layout_type::template active_entries<Level>>(
                    storage_.template node<Level>(0), 0, layout().valid_children(Level, 0)));
        });
    }
    void set(std::size_t position, const value_type& value) noexcept
        requires(nothrow_mutation)
    {
        const auto encoded = kernel_.import_value(value);
        with_root([&]<unsigned Level> {
            const auto changed = replace_impl<Level>(0, position, encoded);
            if constexpr (requires { kernel_.commit_root(changed); })
                kernel_.commit_root(changed);
        });
    }
    void accumulate(std::size_t position, const value_type& delta)
        noexcept(noexcept(std::declval<Kernel&>().point_accumulate(
            std::declval<leaf_type_t<Kernel>&>(), unsigned{}, std::declval<const value_type&>())))
        requires requires(Kernel& k, leaf_type_t<Kernel>& leaf, const value_type& d) {
            k.point_accumulate(leaf, unsigned{}, d);
        }
    {
        with_root([&]<unsigned Level> { point_accumulate_impl<Level>(0, position, delta); });
    }
    [[nodiscard]] prepared_action prepare(const update_type& update) const
        requires(has_lazy)
    {
        return kernel_.prepare(update);
    }
    void apply(std::size_t first, std::size_t last, const update_type& update) noexcept
        requires(has_lazy && nothrow_mutation)
    {
        if (first == last)
            return;
        apply_prepared(first, last, kernel_.prepare(update));
    }
    void apply_prepared(std::size_t first, std::size_t last, const prepared_action& update) noexcept
        requires(has_lazy && nothrow_mutation)
    {
        if (first == last)
            return;
        if constexpr (requires { kernel_.apply_all(update); }) {
            if (first == 0 && last == size()) {
                kernel_.apply_all(update);
                return;
            }
        }
        if constexpr (requires { Kernel::upward_updates; }) {
            static_assert(Kernel::upward_updates);
            upward_access access{*this};
            wide_upward_update{layout(), access}(first, last, update);
        } else
            with_root([&]<unsigned Level> { update_impl<Level, false>(0, first, last, update); });
    }
    template <typename Predicate>
    [[nodiscard]] std::size_t max_right(std::size_t first, Predicate predicate) const {
        if (first == size())
            return first;
        auto accumulated = kernel_.identity();
        return with_root([&]<unsigned Level> {
            return search<true, Level>(
                0, 0, first, kernel_.identity_action(), accumulated, predicate);
        });
    }
    template <typename Predicate>
    [[nodiscard]] std::size_t min_left(std::size_t last, Predicate predicate) const {
        if (last == 0)
            return 0;
        auto accumulated = kernel_.identity();
        return with_root([&]<unsigned Level> {
            return search<false, Level>(
                0, 0, last, kernel_.identity_action(), accumulated, predicate);
        });
    }
    template <typename Output> Output materialize(Output output) const {
        if (!empty())
            with_root(
                [&]<unsigned Level> { write_values<Level>(0, kernel_.identity_action(), output); });
        return output;
    }
    [[nodiscard]] std::vector<value_type> snapshot() const {
        std::vector<value_type> result;
        result.reserve(size());
        materialize(std::back_inserter(result));
        return result;
    }
    template <std::ranges::input_range R>
    void rebuild(R&& values)
        requires(std::is_nothrow_swappable_v<Kernel>)
    {
        wide_engine next{std::forward<R>(values), kernel_, get_allocator()};
        swap(next);
    }
    void prefetch(std::size_t first, std::size_t last) const noexcept {
        if (first == last)
            return;
#if defined(__GNUC__) || defined(__clang__)
        const auto prefetch_block = [&](const auto& block) {
            if constexpr (requires { kernel_.prefetch(block); })
                kernel_.prefetch(block);
            else
                canard::detail::prefetch<0, 3>(&block);
        };
        auto l = first / B, r = (last - 1) / B;
        prefetch_block(storage_.leaf(l));
        if (r != l)
            prefetch_block(storage_.leaf(r));
        for (unsigned level = 1; level < height(); ++level) {
            l /= B;
            r /= B;
            prefetch_block(storage_.branch(level, l));
            if (r != l)
                prefetch_block(storage_.branch(level, r));
        }
#endif
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
#include <concepts>
#include <memory>
#include <iterator>
#include <ranges>
#include <span>
#include <utility>

namespace canard {

template <algebra::monoid Monoid,
          typename Configuration = wide::configuration<>,
          typename Allocator = std::allocator<std::byte>,
          typename StoragePolicy = wide::storage::owning>
class wide_segment_tree;

template <algebra::monoid Monoid, typename Configuration, typename Allocator>
    requires wide::supported_configuration<Configuration, Monoid, algebra::no_action>
class wide_segment_tree<Monoid, Configuration, Allocator, wide::storage::owning>
    : private detail::wide_engine<wide::kernel_for_t<Configuration, Monoid, algebra::no_action>, Configuration, Allocator> {
    using base = detail::wide_engine<wide::kernel_for_t<Configuration, Monoid, algebra::no_action>, Configuration, Allocator>;
    using kernel = wide::kernel_for_t<Configuration, Monoid, algebra::no_action>;

  public:
    using value_type = value_type_t<base>;
    using monoid_type = Monoid;
    using configuration_type = Configuration;
    using element_type = value_type;
    using aggregate_type = value_type;
    using size_type = std::size_t;
    using allocator_type = Allocator;
    static constexpr bool nothrow_mutation = base::nothrow_mutation;
    using base::size;
    using base::empty;
    using base::height;
    using base::storage_bytes;
    using base::allocated_bytes;
    using base::get_allocator;
    using base::fold;
    using base::prefix_fold;
    using base::get;
    using base::all_fold;
    using base::set;
    using base::accumulate;
    using base::max_right;
    using base::min_left;
    using base::materialize;
    using base::snapshot;
    using base::rebuild;
    using base::prefetch;
    [[nodiscard]] point_reference<wide_segment_tree> operator[](size_type position) & noexcept {
        return {*this, position};
    }
    [[nodiscard]] value_type operator[](size_type position) const & {
        return this->get(position);
    }
    void operator[](size_type) && = delete;
    void operator[](size_type) const && = delete;
    [[nodiscard]] interval_reference<wide_segment_tree> operator[](size_type first, size_type last) & noexcept {
        return {*this, first, last};
    }
    [[nodiscard]] interval_reference<const wide_segment_tree> operator[](size_type first, size_type last) const & noexcept {
        return {*this, first, last};
    }
    void operator[](size_type, size_type) && = delete;
    void operator[](size_type, size_type) const && = delete;
    void swap(wide_segment_tree& other) noexcept
        requires requires(base& value) { value.swap(value); } { base::swap(other); }
    [[nodiscard]] wide_tree_description description() const noexcept {
        return {this->size(), Configuration::fanout, this->height(), this->storage_bytes(), false};
    }


    template <std::ranges::input_range R>
    explicit wide_segment_tree(R&& values,
                               Monoid monoid = {},
                               Configuration = {},
                               const Allocator& allocator = {})
        : base(std::forward<R>(values), kernel{std::move(monoid), {}}, allocator) {}

    explicit wide_segment_tree(Monoid monoid = {},
                               Configuration configuration = {},
                               const Allocator& allocator = {})
        : wide_segment_tree(
              std::span<const value_type>{}, std::move(monoid), configuration, allocator) {}

    template <std::ranges::input_range R, typename Combine>
        requires std::same_as<Monoid, algebra::operation_monoid<value_type, Combine>>
    explicit wide_segment_tree(R&& values,
                               Combine combine,
                               value_type identity,
                               Configuration configuration = {},
                               const Allocator& allocator = {})
        : wide_segment_tree(std::forward<R>(values),
                            Monoid{std::move(combine), std::move(identity)},
                            configuration,
                            allocator) {}

    template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
    explicit wide_segment_tree(Iterator first,
                               Sentinel last,
                               Monoid monoid = {},
                               Configuration configuration = {},
                               const Allocator& allocator = {})
        : wide_segment_tree(
              std::ranges::subrange{first, last}, std::move(monoid), configuration, allocator) {}

    wide_segment_tree(const wide_segment_tree&) = default;
    wide_segment_tree(wide_segment_tree&&) = default;
    wide_segment_tree& operator=(const wide_segment_tree&) = default;
    wide_segment_tree& operator=(wide_segment_tree&&) = default;
    wide_segment_tree(const wide_segment_tree& other, const Allocator& allocator)
        : base(other, allocator) {}
    friend void swap(wide_segment_tree& a, wide_segment_tree& b) noexcept
        requires requires(base& x) {
            x.swap(x);
        }
    {
        a.base::swap(b);
    }
};

template <std::ranges::input_range R, algebra::monoid M>
wide_segment_tree(R&&, M) -> wide_segment_tree<M>;
template <std::ranges::input_range R, algebra::monoid M, typename C>
wide_segment_tree(R&&, M, C) -> wide_segment_tree<M, C>;
template <std::ranges::input_range R, algebra::monoid M, typename C, typename A>
wide_segment_tree(R&&, M, C, A) -> wide_segment_tree<M, C, A>;
template <std::ranges::input_range R, typename Combine, typename T>
    requires(!algebra::monoid<Combine>)
wide_segment_tree(R&&, Combine, T) -> wide_segment_tree<algebra::operation_monoid<T, Combine>>;
template <std::ranges::input_range R, typename Combine, typename T, typename C>
    requires(!algebra::monoid<Combine>)
wide_segment_tree(R&&, Combine, T, C)
    -> wide_segment_tree<algebra::operation_monoid<T, Combine>, C>;

template <std::input_iterator It, std::sentinel_for<It> S, algebra::monoid M>
wide_segment_tree(It, S, M) -> wide_segment_tree<M>;
template <std::input_iterator It, std::sentinel_for<It> S, algebra::monoid M, typename C>
wide_segment_tree(It, S, M, C) -> wide_segment_tree<M, C>;

} // namespace canard

template <canard::algebra::monoid M, typename C, typename Alloc, typename Storage>
struct std::formatter<canard::wide_segment_tree<M, C, Alloc, Storage>, char>
    : canard::format::description_formatter<canard::wide_segment_tree<M, C, Alloc, Storage>> {};
// ===== include/canard/wide/tiered_sum.hpp =====
// ===== include/canard/algebra/basic.hpp =====
#include <algorithm>
#include <concepts>
#include <limits>

namespace canard::algebra {

template <std::integral T> struct sum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return a + b;
    }
};

template <std::integral T> struct minimum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return std::numeric_limits<T>::max();
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return std::min(a, b);
    }
};

template <std::integral T> struct maximum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return std::numeric_limits<T>::lowest();
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return std::max(a, b);
    }
};

template <std::integral T> struct add_to_sum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    // Newer is applied AFTER older. Addition happens to commute; others do not.
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept {
        return newer + older;
    }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        return x + f * static_cast<T>(count);
    }
};

template <std::integral T> struct add_to_extremum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept {
        return newer + older;
    }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        // Length distinguishes a real INT_MAX/INT_MIN leaf from an empty span.
        return count == 0 ? x : x + f;
    }
};

} // namespace canard::algebra
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>
#if defined(__AVX2__) || defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

namespace canard::wide::representation {
template <unsigned LeafFanout = 16>
struct tiered_sum {
    static_assert(LeafFanout >= 2 && LeafFanout <= 64 && std::has_single_bit(LeafFanout));
    static constexpr unsigned leaf_fanout = LeafFanout;
};
} // namespace canard::wide::representation

namespace canard::wide::detail {
template <typename T> struct is_tiered_sum : std::false_type {};
template <unsigned L> struct is_tiered_sum<representation::tiered_sum<L>> : std::true_type {};
template <typename T> inline constexpr bool is_tiered_sum_v = is_tiered_sum<T>::value;
} // namespace canard::wide::detail

namespace canard {

template <typename T, typename Configuration, typename Allocator>
    requires(wide::detail::is_tiered_sum_v<representation_type_t<Configuration>> &&
             std::same_as<execution_type_t<Configuration>, execution::avx2> &&
             std::integral<T> && sizeof(T) == 8)
class wide_segment_tree<algebra::sum<T>,
                        Configuration,
                        Allocator,
                        wide::storage::owning> {
    static constexpr unsigned B = Configuration::fanout;
    static constexpr unsigned L = representation_type_t<Configuration>::leaf_fanout;
    static_assert(B >= 2 && B <= 256 && std::has_single_bit(B));
    static constexpr unsigned max_levels = 16;

  public:
    using value_type = T;
    using monoid_type = algebra::sum<value_type>;
    using configuration_type = Configuration;
    using element_type = value_type;
    using aggregate_type = value_type;
    using size_type = std::size_t;
    using allocator_type = Allocator;
    static constexpr bool nothrow_mutation = true;

  private:
    std::vector<value_type> data_;
    std::array<size_type, max_levels> offset_{};
    std::array<size_type, max_levels> count_{};
    size_type size_ = 0;
    unsigned levels_ = 0;
    value_type total_ = 0;

    [[nodiscard]] const value_type* level(unsigned h) const noexcept { return data_.data() + offset_[h]; }
    [[nodiscard]] value_type* level(unsigned h) noexcept { return data_.data() + offset_[h]; }

    [[nodiscard]] static __attribute__((target("avx2"))) value_type
    sum_slots(const value_type* p, unsigned first, unsigned last) noexcept {
        __m256i acc = _mm256_setzero_si256();
        for (; first + 4 <= last; first += 4)
            acc = _mm256_add_epi64(
                acc, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(p + first)));
        const __m128i halves =
            _mm_add_epi64(_mm256_castsi256_si128(acc), _mm256_extracti128_si256(acc, 1));
        const __m128i reduced = _mm_add_epi64(halves, _mm_unpackhi_epi64(halves, halves));
        value_type result = static_cast<value_type>(_mm_cvtsi128_si64(reduced));
        while (first < last)
            result += p[first++];
        return result;
    }

    [[nodiscard]] value_type prefix_impl(size_type last) const noexcept {
        if (last == 0)
            return 0;
        if (last == size_)
            return total_;
        value_type answer = 0;
        auto remainder = static_cast<unsigned>(last % L);
        auto block = last / L;
        if (remainder) {
            answer += sum_slots(level(0) + block * L, 0, remainder);
        }
        auto index = block;
        for (unsigned h = 1; h < levels_ && index; ++h) {
            const auto slot = static_cast<unsigned>(index % B);
            index /= B;
            if (slot)
                answer += sum_slots(level(h) + index * B, 0, slot);
        }
        return answer;
    }

  public:
    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    explicit wide_segment_tree(R&& values,
                               monoid_type = {},
                               Configuration = {},
                               const Allocator& = {}) {
        size_ = static_cast<size_type>(std::ranges::distance(values));
        if (size_ == 0)
            return;
        count_[0] = size_;
        offset_[0] = 0;
        size_type total_cells = size_;
        auto previous = size_;
        levels_ = 1;
        auto divisor = size_type{L};
        while (previous > 1) {
            const auto current = (previous + divisor - 1) / divisor;
            offset_[levels_] = total_cells;
            count_[levels_] = current;
            total_cells += current;
            previous = current;
            divisor = B;
            ++levels_;
        }
        data_.assign(total_cells, value_type{});
        auto out = data_.begin();
        for (auto&& x : values) {
            const auto value = static_cast<value_type>(x);
            *out++ = value;
            total_ += value;
        }
        // level 1 summarizes L raw elements; higher levels summarize B entries.
        for (unsigned h = 1; h < levels_; ++h) {
            const auto fanout = h == 1 ? L : B;
            const auto* src = level(h - 1);
            auto* dst = level(h);
            const auto src_count = count_[h - 1];
            for (size_type i = 0; i < count_[h]; ++i) {
                const auto first = static_cast<unsigned>(i * fanout);
                const auto last = static_cast<unsigned>(std::min<size_type>(src_count, (i + 1) * fanout));
                dst[i] = sum_slots(src, first, last);
            }
        }
    }

    explicit wide_segment_tree(monoid_type = {}, Configuration = {}, const Allocator& = {}) {}

    [[nodiscard]] size_type size() const noexcept { return size_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] unsigned height() const noexcept { return levels_; }
    [[nodiscard]] size_type storage_bytes() const noexcept { return data_.size() * sizeof(value_type); }
    [[nodiscard]] size_type allocated_bytes() const noexcept { return data_.capacity() * sizeof(value_type); }
    [[nodiscard]] Allocator get_allocator() const { return {}; }

    void accumulate(size_type position, value_type delta) noexcept {
        level(0)[position] += delta;
        total_ += delta;
        auto index = position / L;
        if (levels_ > 1)
            level(1)[index] += delta;
        for (unsigned h = 2; h < levels_; ++h) {
            index /= B;
            level(h)[index] += delta;
        }
    }

    [[nodiscard]] value_type get(size_type position) const noexcept { return level(0)[position]; }
    [[nodiscard]] value_type all_fold() const noexcept { return total_; }
    [[nodiscard]] value_type prefix_fold(size_type last) const noexcept { return prefix_impl(last); }

    [[nodiscard]] value_type fold(size_type first, size_type last) const noexcept {
        if (first == last)
            return 0;
        if (first == 0)
            return prefix_impl(last);
        if (last == size_)
            return total_ - prefix_impl(first);

        value_type answer = 0;
        // Raw leaf boundaries.
        while (first < last && first % L)
            answer += level(0)[first++];
        while (first < last && last % L)
            answer += level(0)[--last];
        if (first == last)
            return answer;
        first /= L;
        last /= L;

        for (unsigned h = 1; first < last; ++h) {
            if (h + 1 == levels_)
                return answer + sum_slots(level(h), static_cast<unsigned>(first), static_cast<unsigned>(last));
            while (first < last && first % B)
                answer += level(h)[first++];
            while (first < last && last % B)
                answer += level(h)[--last];
            first /= B;
            last /= B;
        }
        return answer;
    }

    void prefetch(size_type first, size_type last) const noexcept {
        if (first == last)
            return;
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(level(0) + first, 0, 2);
        __builtin_prefetch(level(0) + last - 1, 0, 2);
#endif
    }
};

} // namespace canard
// ===== include/canard/wide/sum_avx2.hpp =====
// ===== include/canard/kernel/wide_sum_avx2.hpp =====
#include <cstdint>
#include <immintrin.h>
#include <utility>

namespace canard::kernel {
template <unsigned Fanout>
struct wide_sum_avx2 : wide_scalar<algebra::sum<std::int64_t>, algebra::no_action, Fanout> {
    using base = wide_scalar<algebra::sum<std::int64_t>, algebra::no_action, Fanout>;
    using typename base::summary_type;
    wide_sum_avx2(algebra::sum<std::int64_t> monoid = {}, algebra::no_action action = {})
        : base{std::move(monoid), std::move(action)} {}

    template <typename Block>
    void point_accumulate(Block& block, unsigned slot, const std::int64_t& delta) const noexcept {
        block.values[slot] += delta;
    }

    template <unsigned Active = Fanout, typename Block>
    [[nodiscard]] __attribute__((target("avx2"))) summary_type
    fold(const Block& block, unsigned first, unsigned last) const noexcept {
        static_assert(Active <= Fanout);
        const auto* p = block.values.data();
        __m256i acc = _mm256_setzero_si256();
        for (; first + 4 <= last; first += 4)
            acc = _mm256_add_epi64(acc, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(p + first)));
        const __m128i halves = _mm_add_epi64(_mm256_castsi256_si128(acc), _mm256_extracti128_si256(acc, 1));
        const __m128i total = _mm_add_epi64(halves, _mm_unpackhi_epi64(halves, halves));
        std::int64_t scalar = _mm_cvtsi128_si64(total);
        while (first < last) scalar += p[first++];
        return scalar;
    }
};
} // namespace canard::kernel
namespace canard::wide {
template <unsigned Fanout>
    requires(Fanout == 4 || Fanout == 8 || Fanout == 16 || Fanout == 32 || Fanout == 64 || Fanout == 128 || Fanout == 256)
struct kernel_binding<representation::ordinary,
                      execution::avx2,
                      algebra::sum<std::int64_t>,
                      algebra::no_action,
                      Fanout> {
    using type = kernel::wide_sum_avx2<Fanout>;
};
} // namespace canard::wide
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
static constexpr u64 query_bit = u64{1} << 63;
static constexpr u64 position_mask = (u64{1} << 19) - 1;
static inline u64 pack_pair(u32 low, u32 high) noexcept { return u64(low) | (u64(high) << 32); }
using tiered_profile = canard::wide::configuration<32, 500000,
    canard::wide::representation::tiered_sum<16>, canard::execution::avx2>;
using chain_profile = canard::wide::configuration<64, 500000,
    canard::wide::representation::ordinary, canard::execution::avx2>;

int main() {
    canard::io::checked_linux_mapped_file file;
    canard::io::avx2_ascii_reader input{file.view()};
    canard::io::writer<canard::io::trusted_posix_sink, canard::execution::avx2> output;
    const auto nq = input.read_pair<6, 6>();
    const u32 n = nq[0], q = nq[1];

    auto values = std::make_unique_for_overwrite<i64[]>(n);
    u32 i = 0;
    for (; i + 1 < n; i += 2) {
        const auto x = input.read_pair<10, 10>(); values[i] = x[0]; values[i + 1] = x[1];
    }
    if (i < n) values[i] = input.read_u32<10>();

    auto meta = std::make_unique_for_overwrite<u64[]>(n);
    meta[0] = pack_pair(0, 1);
    bool is_chain = true;
    for (i = 1; i + 1 < n; i += 2) {
        const auto p = input.read_pair<6, 6>();
        is_chain &= p[0] == i - 1 && p[1] == i;
        meta[i] = pack_pair(p[0], 1); meta[i + 1] = pack_pair(p[1], 1);
    }
    if (i < n) {
        const auto p = input.read_u32<6>(); is_chain &= p == i - 1; meta[i] = pack_pair(p, 1);
    }

    std::vector<u64> answers;
    answers.reserve(q / 2 + 16);

    // A chain's subtree is exactly the suffix [u,n), so avoid building Euler metadata.
    if (is_chain) {
        canard::wide_segment_tree tree{
            std::span<const i64>{values.get(), n}, canard::algebra::sum<i64>{}, chain_profile{}};
        values.reset(); meta.reset();
        u64 total = static_cast<u64>(tree.all_fold());
        for (u32 k = 0; k < q; ++k) {
            const auto type = input.read_u32<1>();
            if (type == 0) {
                const auto ux = input.read_pair<6, 10>();
                tree.accumulate(ux[0], static_cast<i64>(ux[1])); total += ux[1];
            } else {
                const auto u = input.read_u32<6>();
                answers.push_back(total - static_cast<u64>(tree.prefix_fold(u)));
            }
        }
        output.write(answers); output.finish(); return 0;
    }

    // Because p[v] < v, reverse vertex order is a topological order for subtree sizes.
    for (u32 v = n; v-- > 1;) {
        const auto p = u32(meta[v]);
        meta[p] += u64(u32(meta[v] >> 32)) << 32;
    }

    // Stable child-order preorder without storing adjacency: cursor[p] owns the next position
    // inside p's already-known subtree interval.
    auto cursor = std::make_unique_for_overwrite<u32[]>(n);
    auto euler = std::make_unique_for_overwrite<i64[]>(n);
    cursor[0] = 1; euler[0] = values[0]; meta[0] = pack_pair(0, n);
    for (u32 v = 1; v < n; ++v) {
        const auto m = meta[v];
        const u32 p = u32(m), subtree_size = u32(m >> 32), first = cursor[p];
        cursor[p] = first + subtree_size; cursor[v] = first + 1;
        euler[first] = values[v]; meta[v] = pack_pair(first, first + subtree_size);
    }
    cursor.reset(); values.reset();

    // Resolve vertex IDs to Euler intervals while parsing. Each online operation then fits in
    // one 64-bit record: [query bit | delta/end | position]. This removes metadata lookups from
    // the hot wide-tree loop without reordering operations.
    auto operations = std::make_unique_for_overwrite<u64[]>(q);
    u32 answer_count = 0;
    for (u32 k = 0; k < q; ++k) {
        const auto type = input.read_u32<1>();
        if (type == 0) {
            const auto ux = input.read_pair<6, 10>();
            operations[k] = u64(u32(meta[ux[0]])) | (u64(ux[1]) << 19);
        } else {
            const auto u = input.read_u32<6>(); const auto m = meta[u];
            operations[k] = query_bit | u64(u32(m)) | (u64(u32(m >> 32)) << 19);
            ++answer_count;
        }
    }
    meta.reset();

    canard::wide_segment_tree tree{
        std::span<const i64>{euler.get(), n}, canard::algebra::sum<i64>{}, tiered_profile{}};
    euler.reset();
    answers.reserve(answer_count);
    for (u32 k = 0; k < q; ++k) {
        const auto operation = operations[k];
        const auto first = u32(operation & position_mask);
        if (operation & query_bit) {
            const auto last = u32((operation >> 19) & position_mask);
            answers.push_back(static_cast<u64>(tree.fold(first, last)));
        } else {
            tree.accumulate(first, static_cast<i64>((operation >> 19) & 0x3fffffffULL));
        }
    }

    output.write(answers); output.finish();
}
