#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <immintrin.h>

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#endif

#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 11
#endif

// Deliberately unchecked: valid decimal input, one delimiter per token,
// x86-64 AVX2, and process-lifetime buffers are assumed for maximum speed.
namespace fastio_unsafe_impl {

using u32 = std::uint32_t;
using u64 = std::uint64_t;

constexpr auto make_right_align_masks() {
    std::array<std::array<char, 16>, 16> masks{};
    for (int digits = 0; digits < 16; ++digits) {
        for (int i = 0; i < 16; ++i) {
            masks[digits][i] = i < 16 - digits
                ? static_cast<char>(0x80)
                : static_cast<char>(i - (16 - digits));
        }
    }
    return masks;
}

alignas(16) inline constexpr auto right_align_masks = make_right_align_masks();

struct input {
public:
    input() {
#ifdef __linux__
        struct stat info {};
        if (::fstat(0, &info) == 0 && S_ISREG(info.st_mode) && info.st_size > 0) {
            const off_t current = ::lseek(0, 0, SEEK_CUR);
            const std::size_t file_size = static_cast<std::size_t>(info.st_size);
            const std::size_t page_size = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
            const std::size_t rounded_size =
                (file_size + page_size - 1) / page_size * page_size;
            const std::size_t reserved_size = rounded_size + page_size;

            char* region = static_cast<char*>(::mmap(
                nullptr, reserved_size, PROT_NONE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
            if (region != MAP_FAILED) {
                void* file_mapping = ::mmap(
                    region, rounded_size, PROT_READ,
                    MAP_PRIVATE | MAP_FIXED, 0, 0);
                void* zero_page = file_mapping == MAP_FAILED ? MAP_FAILED : ::mmap(
                    region + rounded_size, page_size, PROT_READ,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
                if (file_mapping != MAP_FAILED && zero_page != MAP_FAILED) {
                    const std::size_t offset = current > 0
                        ? std::min(static_cast<std::size_t>(current), file_size)
                        : 0;
                    cursor_ = region + offset;
                    return;
                }
                ::munmap(region, reserved_size);
            }
        }
#endif
        read_all_fallback();
    }

    input(const input&) = delete;
    input& operator=(const input&) = delete;

    char* cursor() const noexcept { return cursor_; }

private:
    void read_all_fallback() {
        std::size_t capacity = 1u << 20;
        std::size_t size = 0;
        char* buffer = static_cast<char*>(std::malloc(capacity + 32));
        if (buffer == nullptr) std::abort();

        for (;;) {
            if (size == capacity) {
                capacity *= 2;
                char* grown = static_cast<char*>(std::realloc(buffer, capacity + 32));
                if (grown == nullptr) std::abort();
                buffer = grown;
            }
            const std::size_t count = std::fread(
                buffer + size, 1, capacity - size, stdin);
            size += count;
            if (count == 0) break;
        }
        std::memset(buffer + size, 0, 32);
        cursor_ = buffer;
    }

    char* cursor_ = nullptr;
};

__attribute__((always_inline)) inline u64 parse_16_digits(__m128i digits) noexcept {
    const __m128i pair_weights = _mm_set1_epi16(0x010A);
    const __m128i quad_weights = _mm_set1_epi32(0x00010064);
    const __m128i oct_weights = _mm_set_epi32(1, 10000, 1, 10000);
    const __m128i pairs = _mm_maddubs_epi16(digits, pair_weights);
    const __m128i quads = _mm_madd_epi16(pairs, quad_weights);
    const __m128i products = _mm_mul_epu32(quads, oct_weights);
    const __m128i odd = _mm_srli_epi64(quads, 32);
    const __m128i octets = _mm_add_epi64(products, odd);
    const u64 high = static_cast<u64>(_mm_cvtsi128_si64(octets));
    const u64 low = static_cast<u64>(_mm_extract_epi64(octets, 1));
    return high * 100000000ULL + low;
}

__attribute__((always_inline)) inline u64 read_u64(char*& cursor) noexcept {
    const __m128i ascii_zero = _mm_set1_epi8('0');
    __m128i digits = _mm_sub_epi8(
        _mm_loadu_si128(reinterpret_cast<const __m128i*>(cursor)),
        ascii_zero);
    const u32 non_digit_mask = static_cast<u32>(_mm_movemask_epi8(digits));

    if (__builtin_expect(non_digit_mask != 0, 1)) {
        const int length = __builtin_ctz(non_digit_mask);
        digits = _mm_shuffle_epi8(
            digits,
            _mm_load_si128(reinterpret_cast<const __m128i*>(
                right_align_masks[static_cast<std::size_t>(length)].data())));
        cursor += length + 1;
        return parse_16_digits(digits);
    }

    u64 value = parse_16_digits(digits);
    cursor += 16;
    while (*cursor >= '0') {
        value = value * 10 + static_cast<unsigned>(*cursor & 15);
        ++cursor;
    }
    ++cursor;
    return value;
}

constexpr u32 pack4(char a, char b, char c, char d) noexcept {
    return static_cast<u32>(static_cast<unsigned char>(a)) |
           (static_cast<u32>(static_cast<unsigned char>(b)) << 8) |
           (static_cast<u32>(static_cast<unsigned char>(c)) << 16) |
           (static_cast<u32>(static_cast<unsigned char>(d)) << 24);
}

constexpr auto make_leading_groups() {
    std::array<u32, 10000> table{};
    for (int value = 0; value < 10000; ++value) {
        char a = static_cast<char>('0' + value / 1000);
        char b = static_cast<char>('0' + value / 100 % 10);
        char c = static_cast<char>('0' + value / 10 % 10);
        char d = static_cast<char>('0' + value % 10);
        if (value < 1000) a = ' ';
        if (value < 100) b = ' ';
        if (value < 10) c = ' ';
        if (value == 0) d = ' ';
        table[static_cast<std::size_t>(value)] = pack4(a, b, c, d);
    }
    return table;
}

constexpr auto make_padded_groups() {
    std::array<u32, 10000> table{};
    for (int value = 0; value < 10000; ++value) {
        table[static_cast<std::size_t>(value)] = pack4(
            static_cast<char>('0' + value / 1000),
            static_cast<char>('0' + value / 100 % 10),
            static_cast<char>('0' + value / 10 % 10),
            static_cast<char>('0' + value % 10));
    }
    return table;
}

inline constexpr auto leading_groups = make_leading_groups();
inline constexpr auto padded_groups = make_padded_groups();

struct output {
    output() = default;
    output(const output&) = delete;
    output& operator=(const output&) = delete;
    char* begin() noexcept { return buffer_.data(); }
    char* end() noexcept { return buffer_.data() + buffer_.size(); }

    __attribute__((noinline)) char* flush(char* cursor) noexcept {
        std::size_t remaining = static_cast<std::size_t>(cursor - buffer_.data());
        const char* data = buffer_.data();
#ifdef __linux__
        while (remaining != 0) {
            const ssize_t count = ::write(1, data, remaining);
            if (count > 0) {
                data += count;
                remaining -= static_cast<std::size_t>(count);
            } else if (count < 0 && errno == EINTR) {
                continue;
            } else {
                std::abort();
            }
        }
#else
        while (remaining != 0) {
            const std::size_t count = std::fwrite(data, 1, remaining, stdout);
            if (count == 0) std::abort();
            data += count;
            remaining -= count;
        }
#endif
        return buffer_.data();
    }

    void finish(char* cursor) noexcept {
        if (cursor != buffer_.data()) *cursor++ = '\n';
        (void)flush(cursor);
    }

    alignas(64) std::array<char, 1u << 19> buffer_;
};

__attribute__((always_inline)) inline void store_group(char*& cursor, u32 group) noexcept {
    std::memcpy(cursor, &group, sizeof(group));
    cursor += sizeof(group);
}

__attribute__((always_inline)) inline void emit_leading(char*& cursor, u64 value) noexcept {
    store_group(cursor, leading_groups[static_cast<std::size_t>(value)]);
}

__attribute__((always_inline)) inline void emit_padded(char*& cursor, u64 value) noexcept {
    store_group(cursor, padded_groups[static_cast<std::size_t>(value)]);
}

__attribute__((always_inline)) inline void write_u64(
    output& sink, char*& cursor, char* end, u64 value) noexcept {
    if (__builtin_expect(end - cursor < 24, 0)) cursor = sink.flush(cursor);

    if (value >= 1000000000000000ULL) {
        const u64 low8 = value % 100000000ULL;
        const u64 high = value / 100000000ULL;
        const u64 high_low4 = high % 10000;
        const u64 high_high = high / 10000;
        emit_leading(cursor, high_high / 10000);
        emit_padded(cursor, high_high % 10000);
        emit_padded(cursor, high_low4);
        emit_padded(cursor, low8 / 10000);
        emit_padded(cursor, low8 % 10000);
    } else if (value >= 100000000000ULL) {
        const u64 low8 = value % 100000000ULL;
        const u64 high = value / 100000000ULL;
        emit_leading(cursor, high / 10000);
        emit_padded(cursor, high % 10000);
        emit_padded(cursor, low8 / 10000);
        emit_padded(cursor, low8 % 10000);
    } else if (value >= 10000000ULL) {
        const u64 low8 = value % 100000000ULL;
        emit_leading(cursor, value / 100000000ULL);
        emit_padded(cursor, low8 / 10000);
        emit_padded(cursor, low8 % 10000);
    } else if (value >= 1000ULL) {
        emit_leading(cursor, value / 10000);
        emit_padded(cursor, value % 10000);
    } else if (value != 0) {
        emit_leading(cursor, value);
    } else {
        store_group(cursor, pack4(' ', ' ', ' ', '0'));
    }
}

} // namespace fastio_unsafe_impl

struct fastio_unsafe {
    fastio_unsafe() = default;
    fastio_unsafe(const fastio_unsafe&) = delete;
    fastio_unsafe& operator=(const fastio_unsafe&) = delete;

    fastio_unsafe_impl::input in;
    fastio_unsafe_impl::output out;
};

int main() {
    fastio_unsafe io;
    char* input_cursor = io.in.cursor();
    char* output_cursor = io.out.begin();
    char* const output_end = io.out.end();

    const fastio_unsafe_impl::u64 test_count =
        fastio_unsafe_impl::read_u64(input_cursor);
    constexpr fastio_unsafe_impl::u64 block_size =
        1u << FASTIO_UNSAFE_BLOCK_LOG;
    alignas(64) fastio_unsafe_impl::u64 sums[block_size];

    for (fastio_unsafe_impl::u64 base = 0;
         base < test_count; base += block_size) {
        const fastio_unsafe_impl::u64 count =
            std::min(block_size, test_count - base);
        for (fastio_unsafe_impl::u64 i = 0; i < count; ++i) {
            const fastio_unsafe_impl::u64 a =
                fastio_unsafe_impl::read_u64(input_cursor);
            const fastio_unsafe_impl::u64 b =
                fastio_unsafe_impl::read_u64(input_cursor);
            sums[i] = a + b;
        }
        for (fastio_unsafe_impl::u64 i = 0; i < count; ++i) {
            fastio_unsafe_impl::write_u64(
                io.out, output_cursor, output_end, sums[i]);
        }
    }
    io.out.finish(output_cursor);
}
