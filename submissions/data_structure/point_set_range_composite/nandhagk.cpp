// Point Set Range Composite: C++23, AVX2, single-threaded, trusted judge input.
// g++ -O2 -std=c++23 -DEVAL -DONLINE_JUDGE -march=native -o main main.cpp -I /opt/ac-library
// stdin must be a regular file: ./main < input.txt
// No optimization-level pragmas, target pragmas, forced NDEBUG, or runtime validation.
// Explicit vector code is AVX2. See README.md for contracts and derivation.


#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>
#include <span>
#include <utility>
#include <sys/mman.h>
#include <unistd.h>

#include <immintrin.h>

#ifndef __AVX2__
#error "AVX2 is required. Use -march=native on the judge, or -march=znver3."
#endif

namespace range_composite {

using u32 = std::uint32_t;
using u64 = std::uint64_t;
inline constexpr u32 modulus = 998'244'353;

struct encoded_affine_map {
    u32 slope;
    u32 intercept;
};

namespace montgomery {

inline constexpr u32 twice_modulus = 2 * modulus;
inline constexpr u32 negative_inverse = [] consteval {
    u32 inverse = 1;
    for (int step = 0; step < 5; ++step) {
        inverse *= 2 - modulus * inverse;
    }
    return -inverse;
}();
inline constexpr u32 one = (u64{1} << 32) % modulus;
inline constexpr u32 radix_squared = u64{one} * one % modulus;
static_assert(4 * u64{modulus} < (u64{1} << 32));
// The fused reduction below also needs its 64-bit numerator not to overflow.
static_assert(8 * u64{modulus} * modulus + u64{modulus} * 0xffff'ffffu
              < std::numeric_limits<u64>::max());

// All internal residues are Montgomery encoded and lie in [0, 2*modulus).
[[nodiscard]] constexpr u32 multiply(u32 a, u32 b) noexcept {
    const u64 product = u64{a} * b;
    const u32 correction = static_cast<u32>(product) * negative_inverse;
    return static_cast<u32>((product + u64{correction} * modulus) >> 32);
}

[[nodiscard]] constexpr u32 add(u32 a, u32 b) noexcept {
    const u32 sum = a + b;
    return sum >= twice_modulus ? sum - twice_modulus : sum;
}

[[nodiscard]] constexpr u32 subtract(u32 a, u32 b) noexcept {
    return a >= b ? a - b : a + twice_modulus - b;
}

// Reduce a*b+c*d together. The intermediate result is <3p; subtracting 2p
// when necessary restores the [0,2p) invariant.
[[nodiscard]] constexpr u32 multiply_sum(u32 a, u32 b, u32 c, u32 d) noexcept {
    const u64 sum = u64{a} * b + u64{c} * d;
    const u32 correction = static_cast<u32>(sum) * negative_inverse;
    const u32 result = static_cast<u32>((sum + u64{correction} * modulus) >> 32);
    return result >= twice_modulus ? result - twice_modulus : result;
}

[[nodiscard]] constexpr u32 encode(u32 value) noexcept {
    return multiply(value, radix_squared);
}

[[nodiscard]] constexpr u32 decode(u32 value) noexcept {
    const u32 result = multiply(value, 1);
    return result >= modulus ? result - modulus : result;
}

[[nodiscard]] constexpr u32 power(u32 base, u32 exponent) noexcept {
    u32 result = one;
    for (; exponent != 0; exponent >>= 1, base = multiply(base, base)) {
        if ((exponent & 1u) != 0) {
            result = multiply(result, base);
        }
    }
    return result;
}

// A radix-2^32 reduction needs the complete 64-bit product for each lane.
// AVX2 multiplies alternate 32-bit lanes, so use separate even/odd streams.
[[nodiscard]] inline __m256i multiply(__m256i a, __m256i b) noexcept {
    const auto even = _mm256_mul_epu32(a, b);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32),
                                     _mm256_srli_epi64(b, 32));
    const auto inverse = _mm256_set1_epi32(std::bit_cast<int>(negative_inverse));
    const auto prime = _mm256_set1_epi32(static_cast<int>(modulus));
    const auto even_correction = _mm256_mul_epu32(even, inverse);
    const auto odd_correction = _mm256_mul_epu32(odd, inverse);
    const auto even_sum = _mm256_add_epi64(
        even, _mm256_mul_epu32(even_correction, prime));
    const auto odd_sum = _mm256_add_epi64(
        odd, _mm256_mul_epu32(odd_correction, prime));
    return _mm256_blend_epi32(_mm256_srli_epi64(even_sum, 32), odd_sum, 0xaa);
}

struct fixed_multiplier {
    __m256i value;
    __m256i quotient;
    explicit fixed_multiplier(u32 encoded) noexcept {
        const u32 scalar = decode(encoded);
        value = _mm256_set1_epi32(static_cast<int>(scalar));
        const u32 scaled = static_cast<u32>((u64{scalar} << 32) / modulus);
        quotient = _mm256_set1_epi32(std::bit_cast<int>(scaled));
    }
};
[[nodiscard]] inline __m256i multiply(__m256i a, fixed_multiplier b) noexcept {
    const auto even = _mm256_mul_epu32(a, b.quotient);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), b.quotient);
    const auto q = _mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa);
    return _mm256_sub_epi32(_mm256_mullo_epi32(a, b.value),
        _mm256_mullo_epi32(q, _mm256_set1_epi32(static_cast<int>(modulus))));
}

[[nodiscard]] inline __m256i add(__m256i a, __m256i b) noexcept {
    const auto sum = _mm256_add_epi32(a,b);
    return _mm256_min_epu32(sum,_mm256_sub_epi32(sum,_mm256_set1_epi32(static_cast<int>(twice_modulus))));
}

[[nodiscard]] inline __m256i load(const u32* address) noexcept {
    // The tree's arrays and every eight-element block are 32-byte aligned.
    return _mm256_load_si256(reinterpret_cast<const __m256i*>(address));
}

inline void store(u32* address, __m256i value) noexcept {
    _mm256_store_si256(reinterpret_cast<__m256i*>(address), value);
}

} // namespace montgomery

// Both numerator and denominator are Montgomery residues.
// denominator is nonzero modulo modulus.
struct fraction { u32 numerator, denominator; };

// A node stores EXCLUSIVE child-prefix inverses P_d^-1(x)=I_d*x+J_d.
// Entry zero is the identity. The full-node product is needed only during
// construction; its inverse is represented in the next layer when queried.
// Valid-input preconditions: 1<=N<=500000; all slopes are nonzero modulo p.
template<std::size_t Fanout = 16>
    requires (Fanout == 8 || Fanout == 16 || Fanout == 32)
class inverse_prefix_tree {
    static constexpr int digit_bits = std::countr_zero(Fanout);
    static constexpr int fanout = static_cast<int>(Fanout);
    static constexpr int digit_mask = fanout - 1;
    static constexpr int maximum_height = (32 + digit_bits - 1) / digit_bits;

    struct alignas(Fanout >= 16 ? 64 : 32) node {
        std::array<u32, Fanout> inverse_slope;
        std::array<u32, Fanout> inverse_offset;
    };
    static_assert(sizeof(node) == 2 * Fanout * sizeof(u32));

    struct storage_deleter {
        void* reservation = nullptr;
        std::size_t reserved_bytes = 0;
        void operator()(node* address) const noexcept {
            if (reservation != nullptr) ::munmap(reservation, reserved_bytes);
            else delete[] address;
        }
    };
    static_assert(std::is_trivially_destructible_v<node>);
    std::unique_ptr<node[], storage_deleter> nodes_{nullptr, storage_deleter{}};

    void allocate_nodes() {
#if !defined(RANGE_COMPOSITE_HUGE_PAGES) || RANGE_COMPOSITE_HUGE_PAGES
        constexpr std::size_t huge_page = 2u << 20;
        const std::size_t payload = node_count_ * sizeof(node);
        if (payload >= huge_page) {
            const auto page = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
            const std::size_t bytes = (payload + page - 1) / page * page;
            const std::size_t reserved = bytes + huge_page;
            void* reservation = ::mmap(nullptr, reserved, PROT_READ | PROT_WRITE,
                                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            const auto begin = reinterpret_cast<std::uintptr_t>(reservation);
            const auto aligned = (begin + huge_page - 1) / huge_page * huge_page;
            void* address = reinterpret_cast<void*>(aligned);
            // Advisory only: correctness does not require promotion to huge pages.
            ::madvise(address, bytes, MADV_HUGEPAGE);
            // Explicitly begin C++ object lifetimes in the mapped storage.
            auto* objects = ::new (address) node[node_count_];
            nodes_ = decltype(nodes_){objects, {reservation, reserved}};
            return;
        }
#endif
        auto ordinary = std::make_unique_for_overwrite<node[]>(node_count_);
        nodes_ = decltype(nodes_){ordinary.release(), storage_deleter{}};
    }
    std::array<node*, maximum_height> layers_{};
    std::size_t node_count_ = 0;
    int size_ = 0;
    int height_ = 0;

    // Entries before 'first' remain unchanged; the suffix receives
    // I <- scale*I, J <- scale*J + translation. All vector lanes are [0,2p).
    static void update_suffix(node& destination, int first,
                              montgomery::fixed_multiplier scale,
                              u32 translation) noexcept {
        using namespace montgomery;
        const auto shift = _mm256_set1_epi32(static_cast<int>(translation));
        const auto lane_numbers = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        for (int block = first & ~7; block < fanout; block += 8) {
            const auto mask = _mm256_cmpgt_epi32(lane_numbers,
                _mm256_set1_epi32(first - block - 1));
            auto* i = destination.inverse_slope.data() + block;
            auto* j = destination.inverse_offset.data() + block;
            const auto old_i = load(i);
            const auto old_j = load(j);
            store(i, _mm256_blendv_epi8(old_i, multiply(old_i, scale), mask));
            store(j, _mm256_blendv_epi8(old_j, add(multiply(old_j, scale), shift), mask));
        }
    }

    template<int Top>
    [[nodiscard]] fraction query_fixed(int left, int right, u32 x) const noexcept {
        using namespace montgomery;
        // Three independent chains: G_left^-1(x), G_right^-1(0), and
        // inverse_slope(G_right). The answer is their difference divided by
        // the last quantity. Shared high endpoint digits cancel beforehand.
        u32 inverse_left = x, inverse_right_offset = 0, right_inverse = one;
        [&]<std::size_t... Levels>(std::index_sequence<Levels...>) {
            (([&] {
                constexpr int level = static_cast<int>(Levels);
                const int le = left >> (level * digit_bits);
                const int re = right >> (level * digit_bits);
                const int ld = le & digit_mask;
                const int rd = re & digit_mask;
                const node& l = layers_[level][le >> digit_bits];
                const node& r = layers_[level][re >> digit_bits];
                inverse_left = add(multiply(l.inverse_slope[ld], inverse_left), l.inverse_offset[ld]);
                if constexpr (level == 0) {
                    inverse_right_offset = r.inverse_offset[rd];
                    right_inverse = r.inverse_slope[rd];
                } else {
                    inverse_right_offset = add(multiply(r.inverse_slope[rd], inverse_right_offset), r.inverse_offset[rd]);
                    right_inverse = multiply(right_inverse, r.inverse_slope[rd]);
                }
            }()), ...);
        }(std::make_index_sequence<Top + 1>{});
        return {subtract(inverse_left, inverse_right_offset), right_inverse};
    }

public:
    // Read-only lookahead: assignments pass (position,position), queries
    // pass (left,right). One allocated sentinel node per layer makes even
    // an exclusive endpoint on the next group boundary a valid address.
    void prefetch(int left, int right) const noexcept {
        for (int level = 0; level < 2; ++level, left >>= digit_bits, right >>= digit_bits) {
            if (level >= height_) break;
            const node& l = layers_[level][left >> digit_bits];
            const node& r = layers_[level][right >> digit_bits];
            __builtin_prefetch(l.inverse_slope.data(), 0, 3);
            __builtin_prefetch(l.inverse_offset.data(), 0, 3);
            __builtin_prefetch(r.inverse_slope.data(), 0, 3);
            __builtin_prefetch(r.inverse_offset.data(), 0, 3);
        }
    }

    // Input coefficients are already Montgomery encoded. Construction first
    // forms forward intercepts, batch-inverts node totals, then walks each
    // node backward to recover every exclusive prefix inverse.
    explicit inverse_prefix_tree(std::span<const encoded_affine_map> values) {
        using namespace montgomery;
        size_ = static_cast<int>(values.size());
        height_ = (std::bit_width(static_cast<u32>(size_)) - 1) / digit_bits + 1;
        std::array<int, maximum_height> child_counts{};
        std::array<std::size_t, maximum_height> offsets{};
        int children = size_;
        for (int level = 0; level < height_; ++level) {
            child_counts[level] = children;
            offsets[level] = node_count_;
            children = (children + fanout - 1) / fanout;
            node_count_ += static_cast<std::size_t>(children) + 1;
        }
        allocate_nodes();
        for (int level = 0; level < height_; ++level) {
            layers_[level] = nodes_.get() + offsets[level];
        }
        struct build_info { u32 a, b, prefix; };
        auto scratch = std::make_unique_for_overwrite<build_info[]>(node_count_);
        u32 product = one;
        for (int level = 0; level < height_; ++level) {
            children = child_counts[level];
            const int groups = (children + fanout - 1) / fanout;
            for (int group = 0; group < groups; ++group) {
                node& current = layers_[level][group];
                u32 prefix_a = one, prefix_b = 0;
                for (int digit = 0; digit < fanout; ++digit) {
                    const int child = group * fanout + digit;
                    u32 child_a = one, child_b = 0;
                    if (child < children) {
                        if (level == 0) {
                            child_a = values[child].slope;
                            child_b = values[child].intercept;
                        } else {
                            const auto previous = scratch[offsets[level - 1] + child];
                            child_a = previous.a;
                            child_b = previous.b;
                        }
                    }
                    current.inverse_offset[digit] = prefix_b;
                    prefix_a = multiply(child_a, prefix_a);
                    prefix_b = add(multiply(child_a, prefix_b), child_b);
                }
                scratch[offsets[level] + group] = {prefix_a, prefix_b, product};
                product = multiply(product, prefix_a);
            }
            layers_[level][groups].inverse_slope.fill(one);
            layers_[level][groups].inverse_offset.fill(0);
        }
        u32 inverse_product = power(product, modulus - 2);
        for (int level = height_; level-- != 0;) {
            const int children = child_counts[level];
            const int groups = (children + fanout - 1) / fanout;
            for (int group = groups; group-- != 0;) {
                node& current = layers_[level][group];
                const auto temp = scratch[offsets[level] + group];
                u32 inverse = multiply(inverse_product, temp.prefix);
                inverse_product = multiply(inverse_product, temp.a);
                for (int digit = fanout; digit-- != 0;) {
                    const int child = group * fanout + digit;
                    const u32 child_a = child < children
                        ? (level == 0 ? values[child].slope : scratch[offsets[level-1]+child].a) : one;
                    inverse = multiply(inverse, child_a);
                    current.inverse_slope[digit] = inverse;
                    current.inverse_offset[digit] = subtract(0, multiply(current.inverse_offset[digit], inverse));
                }
            }
        }
    }

    [[nodiscard]] int size() const noexcept { return size_; }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return node_count_ * sizeof(node);
    }

    // E=replacement^-1 o old = scale*x+translation. At each level, conjugate
    // E by the preceding child prefix: t <- I*t + (1-scale)*J. Its slope is
    // unchanged. No old leaf values or modular inversions are needed here.
    [[gnu::always_inline]] inline void apply_delta(int position, u32 scale,
                                                 u32 translation, u32 one_minus_scale) noexcept {
        using namespace montgomery;
        const fixed_multiplier vector_scale{scale};
        for (int level = 0; level < height_; ++level, position >>= digit_bits) {
            node& current = layers_[level][position >> digit_bits];
            const int digit = position & digit_mask;
            translation = multiply_sum(current.inverse_slope[digit], translation,
                one_minus_scale, current.inverse_offset[digit]);
            update_suffix(current, digit + 1, vector_scale, translation);
        }
    }

    // Return a fraction representing f_{right-1}(...f_left(x)...), not its
    // final residue. The solver batch-resolves denominators after all queries.
    // Empty ranges also work; ordinary x must lie in [0,modulus).
    [[nodiscard]] fraction query(int left, int right, u32 x) const noexcept {
        using namespace montgomery;
        
        if (left == right) {
            return {encode(x),one};
        }
        x = encode(x);
        const int top = (std::bit_width(static_cast<u32>(left ^ right)) - 1) / digit_bits;
        switch (top) {
            case 0: return query_fixed<0>(left,right,x);
            case 1: return query_fixed<1>(left,right,x);
            case 2: return query_fixed<2>(left,right,x);
            case 3: return query_fixed<3>(left,right,x);
            case 4: return query_fixed<4>(left,right,x);
            case 5: return query_fixed<5>(left,right,x);
            default: return query_fixed<6>(left,right,x);
        }
    }
};

} // namespace range_composite


#include <immintrin.h>
#include <array>
#include <bit>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace range_composite::io {

// Trusted Linux judge input: stdin is a regular file positioned at its start,
// with the official single-space/newline format. Mapping/allocation/write
// success is assumed. A readable guard page on each side makes vector and
// backwards eight-byte loads valid, including page-aligned EOF.
class input_buffer {
    void* mapping_ = MAP_FAILED;
    std::size_t mapping_size_ = 0;
    const char* cursor_ = nullptr;

    void map_regular_file() noexcept {
        struct stat status;
        ::fstat(STDIN_FILENO, &status);
        const auto bytes = static_cast<std::size_t>(status.st_size);
        constexpr std::size_t page = 4096;
        mapping_size_ = ((bytes + page - 1) & ~(page - 1)) + 2 * page;
        mapping_ = ::mmap(nullptr, mapping_size_, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        ::mmap(static_cast<char*>(mapping_)+page, bytes, PROT_READ, MAP_PRIVATE | MAP_FIXED, STDIN_FILENO, 0);
        cursor_ = static_cast<const char*>(mapping_) + page;
    }

    void skip_whitespace() {
        while (static_cast<unsigned char>(*cursor_) <= ' ') {
            ++cursor_;
        }
    }

    template<class Word>
    [[nodiscard]] static constexpr bool all_digits(Word word) noexcept {
        constexpr Word zeros = sizeof(Word) == 8
            ? static_cast<Word>(0x3030'3030'3030'3030ull) : Word{0x3030'3030u};
        constexpr Word high = sizeof(Word) == 8
            ? static_cast<Word>(0xf0f0'f0f0'f0f0'f0f0ull) : Word{0xf0f0'f0f0u};
        constexpr Word sixes = sizeof(Word) == 8
            ? static_cast<Word>(0x0606'0606'0606'0606ull) : Word{0x0606'0606u};
        return (word & high) == zeros && ((word + sixes) & high) == zeros;
    }

    template<class Word>
    [[nodiscard]] std::uint32_t read_unsigned() {
        static_assert(std::endian::native == std::endian::little);
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

    // Decode a known 1..9 digit field. Delimiter discovery is separate from
    // decimal arithmetic, allowing fields from the same record to overlap.
    static constexpr auto nibble_mask = [] consteval {
        std::array<std::uint64_t, 10> masks{};
        for (unsigned n=1; n<=9; ++n) masks[n] = 0x0f0f0f0f0f0f0f0full << (n<8 ? (8-n)*8 : 0);
        return masks;
    }();
    static constexpr std::array<unsigned,10> ninth{0,0,0,0,0,0,0,0,0,100000000};
    [[nodiscard]] static std::uint32_t parse_known(const char* data, unsigned length) noexcept {
        std::uint64_t digits;
        std::memcpy(&digits, data + length - 8, 8);
        digits &= nibble_mask[length];
        digits = digits * 10 + (digits >> 8);
        const auto eight = static_cast<std::uint32_t>((
            (digits & 0x0000'00ff'0000'00ffull) * 0x000f'4240'0000'0064ull +
            ((digits >> 16) & 0x0000'00ff'0000'00ffull) * 0x0000'2710'0000'0001ull) >> 32);
        return eight + static_cast<unsigned>(data[0]-'0') * ninth[length];
    }
    [[nodiscard, gnu::always_inline]] static inline std::array<std::uint32_t, 2>
    parse_pair(const char* a, unsigned la, const char* b, unsigned lb) noexcept {
        std::uint64_t ra, rb;
        std::memcpy(&ra, a + la - 8, 8);
        std::memcpy(&rb, b + lb - 8, 8);
        auto digits = _mm_set_epi64x(std::bit_cast<long long>(rb),std::bit_cast<long long>(ra));
        digits = _mm_and_si128(digits,_mm_set_epi64x(nibble_mask[lb], nibble_mask[la]));
        const auto pairs = _mm_maddubs_epi16(digits, _mm_set1_epi16(0x010a));
        const auto quads = _mm_madd_epi16(pairs, _mm_set1_epi32(0x00010064));
        const auto compact = _mm_packus_epi32(quads,quads);
        const auto eights = _mm_madd_epi16(compact,_mm_set1_epi32(0x00012710));
        return {static_cast<std::uint32_t>(_mm_cvtsi128_si32(eights)) + static_cast<unsigned>(a[0]-'0')*ninth[la],
                static_cast<std::uint32_t>(_mm_extract_epi32(eights,1)) + static_cast<unsigned>(b[0]-'0')*ninth[lb]};
    }
    [[nodiscard, gnu::always_inline]] static inline std::array<std::uint32_t,3>
    parse_three(const char* a,unsigned la,const char* b,unsigned lb,const char* c,unsigned lc) noexcept {
        std::uint64_t ra,rb,rc;
        std::memcpy(&ra,a+la-8,8); std::memcpy(&rb,b+lb-8,8); std::memcpy(&rc,c+lc-8,8);
        auto digits = _mm256_setr_epi64x(std::bit_cast<long long>(ra),std::bit_cast<long long>(rb),std::bit_cast<long long>(rc),0);
        digits = _mm256_and_si256(digits,_mm256_setr_epi64x(nibble_mask[la],nibble_mask[lb],nibble_mask[lc],0));
        const auto pairs = _mm256_maddubs_epi16(digits,_mm256_set1_epi16(0x010a));
        const auto quads = _mm256_madd_epi16(pairs,_mm256_set1_epi32(0x00010064));
        const auto compact = _mm256_packus_epi32(quads,quads);
        const auto eights = _mm256_madd_epi16(compact,_mm256_set1_epi32(0x00012710));
        return {static_cast<std::uint32_t>(_mm256_extract_epi32(eights,0)),
                static_cast<std::uint32_t>(_mm256_extract_epi32(eights,1))+static_cast<unsigned>(b[0]-'0')*ninth[lb],
                static_cast<std::uint32_t>(_mm256_extract_epi32(eights,4))+static_cast<unsigned>(c[0]-'0')*ninth[lc]};
    }
    [[nodiscard]] std::uint32_t delimiters() const noexcept {
        return static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpgt_epi8(
            _mm256_set1_epi8('0'), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cursor_)))));
    }

public:
    input_buffer() {
        map_regular_file();
    }
    input_buffer(const input_buffer&) = delete;
    input_buffer& operator=(const input_buffer&) = delete;
    ~input_buffer() {
        if (mapping_ != MAP_FAILED) {
            ::munmap(mapping_, mapping_size_);
        }
    }

    // One AVX2 delimiter mask per record; decimal reductions use packed
    // byte-to-pair, pair-to-quad, and quad-to-eight-digit multiply-adds.
    // A ninth leading digit is added separately. Field lengths are trusted.
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 2> residue_pair() {
        skip_whitespace();
        auto mask = delimiters();
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a,b] = parse_pair(cursor_, first, cursor_ + first + 1, second - first - 1);
        cursor_ += second;
        ++cursor_;
        return {a,b};
    }
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 4> operation_record() {
        skip_whitespace();
        auto mask = delimiters();
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned fourth = std::countr_zero(mask);
        const auto type = static_cast<unsigned>(cursor_[0] - '0');
        const auto [position,a,b] = parse_three(cursor_+2,second-2,
            cursor_+second+1,third-second-1,cursor_+third+1,fourth-third-1);
        cursor_ += fourth;
        ++cursor_;
        return {type,position,a,b};
    }
    [[nodiscard]] std::uint32_t index() { return read_unsigned<std::uint32_t>(); }
    [[nodiscard]] std::uint32_t residue() { return read_unsigned<std::uint64_t>(); }
    [[nodiscard]] std::uint32_t operation_type() {
        skip_whitespace();
        const auto result = static_cast<unsigned>(*cursor_++ - '0');
        return result;
    }
};

class output_buffer {
    static constexpr std::size_t capacity = 1u << 16;
    static constexpr auto four_digits = [] consteval {
        std::array<std::array<char, 4>, 10'000> result{};
        for (unsigned value = 0; value < result.size(); ++value) {
            result[value] = {static_cast<char>('0' + value / 1000),
                static_cast<char>('0' + value / 100 % 10),
                static_cast<char>('0' + value / 10 % 10),
                static_cast<char>('0' + value % 10)};
        }
        return result;
    }();
    std::unique_ptr<char[]> storage_ = std::make_unique_for_overwrite<char[]>(capacity);
    char* cursor_ = storage_.get();

public:
    output_buffer() = default;
    output_buffer(const output_buffer&) = delete;
    output_buffer& operator=(const output_buffer&) = delete;
    ~output_buffer() {
        flush();
    }

    void flush() {
        ::write(STDOUT_FILENO, storage_.get(), static_cast<std::size_t>(cursor_-storage_.get()));
        cursor_ = storage_.get();
    }

    void write(std::uint32_t value) {
        char* const end = storage_.get() + capacity;
        if (end - cursor_ < 16) {
            flush();
        }
        // Most answers have nine digits. Emit their eight low digits with
        // two fixed-size copies; use to_chars only for a short leading group.
        auto append_four = [this](unsigned group) noexcept {
            std::memcpy(cursor_, four_digits[group].data(), 4);
            cursor_ += 4;
        };
        if (value >= 100'000'000) {
            unsigned high = value / 100'000'000;
            value %= 100'000'000;
            *cursor_++ = static_cast<char>('0' + high);
            append_four(value / 10'000);
            append_four(value % 10'000);
        } else if (value >= 10'000) {
            cursor_ = std::to_chars(cursor_, end, value / 10'000).ptr;
            append_four(value % 10'000);
        } else {
            cursor_ = std::to_chars(cursor_, end, value).ptr;
        }
        *cursor_++ = '\n';
    }
};

} // namespace range_composite::io


#include <utility>

namespace {
using namespace range_composite;

#ifndef RANGE_COMPOSITE_FANOUT
inline constexpr std::size_t chosen_fanout = 16;
#else
inline constexpr std::size_t chosen_fanout = RANGE_COMPOSITE_FANOUT;
#endif

// N<=500000 leaves the high position bit available for the operation tag.
// All fields are initialized; there is no union, aliasing, or inactive member.
enum class operation_kind : u32 { assignment = 0, query = 1u << 31 };
struct operation {
    u32 tagged_position;
    u32 first_operand;  // Assignment: new slope, then correction scale. Query: right.
    u32 second_operand; // Assignment: new intercept, then correction offset. Query: x.
    u32 inverse_slope;  // Prefix product -> new-slope inverse -> 1-correction_scale.

    [[nodiscard]] bool is_assignment() const noexcept {
        return (tagged_position & std::to_underlying(operation_kind::query)) == 0;
    }
    [[nodiscard]] int position() const noexcept {
        return static_cast<int>(tagged_position & ~std::to_underlying(operation_kind::query));
    }
};
static_assert(sizeof(operation) == 16);

void solve() {
    using namespace montgomery;
    io::input_buffer input;
    io::output_buffer output;
    const u32 n = input.index();
    const u32 q = input.index();
    auto initial = std::make_unique_for_overwrite<encoded_affine_map[]>(n);
    for (auto& value : std::span{initial.get(), n}) {
        const auto [a,b] = input.residue_pair();
        value = {encode(a),encode(b)};
    }
    auto operations = std::make_unique_for_overwrite<operation[]>(q);
    auto update_indices = std::make_unique_for_overwrite<u32[]>(q);
    u32 update_count = 0;
    u32 product = one;
    u32 query_count = 0;
    for (auto& current : std::span{operations.get(), q}) {
        const auto [type,position,a,b] = input.operation_record();
        if (type == 0) {
            const u32 slope = encode(a);
            const u32 intercept = encode(b);
            current = {position, slope, intercept, product};
            product = multiply(product, slope);
            update_indices[update_count++] = static_cast<u32>(&current - operations.get());
        } else {
            const u32 right = a;
            const u32 x = b;
            current = {position | std::to_underlying(operation_kind::query), right, x, 0};
            ++query_count;
        }
    }
    if (update_count != 0) {
        u32 inverse_product = power(product, modulus - 2);
        for (std::size_t index = update_count; index-- != 0;) {
            auto& current = operations[update_indices[index]];
            const u32 prefix = current.inverse_slope;
            current.inverse_slope = multiply(inverse_product, prefix);
            inverse_product = multiply(inverse_product, current.first_operand);
        }
    }
    inverse_prefix_tree<chosen_fanout> tree(std::span<const encoded_affine_map>{initial.get(), n});
    // Precompute only local leaf corrections. This does not execute any tree
    // operation. The chronological history is independent of query answers.
    for (u32 index = 0; index < update_count; ++index) {
        if (index + 12 < update_count) {
            __builtin_prefetch(initial.get() + operations[update_indices[index + 12]].position(), 0, 3);
        }
        auto& current = operations[update_indices[index]];
        auto& old = initial[current.position()];
        const u32 scale = multiply(old.slope, current.inverse_slope);
        const u32 translation = multiply(subtract(old.intercept, current.second_operand), current.inverse_slope);
        old = {current.first_operand, current.second_operand};
        current.first_operand = scale;
        current.second_operand = translation;
        current.inverse_slope = subtract(one, scale);
    }
    update_indices.reset();
    initial.reset();
    struct answer { u32 numerator, denominator, prefix; };
    auto answers = std::make_unique_for_overwrite<answer[]>(query_count);
    u32 answer_product = one;
    u32 answer_count = 0;
    // Preparing inverses and prefetching addresses do not reorder operations.
    // Four-operation lookahead hides the latency of the two largest layers.
    for (u32 index = 0; index < q; ++index) {
        const auto& current = operations[index];
        if (index + 4 < q) {
            const auto& future = operations[index + 4];
            tree.prefetch(future.position(), future.is_assignment() ? future.position() : static_cast<int>(future.first_operand));
        }
        if (current.is_assignment()) {
            tree.apply_delta(current.position(),
                current.first_operand, current.second_operand, current.inverse_slope);
        } else {
            const auto result = tree.query(current.position(),
                static_cast<int>(current.first_operand), current.second_operand);
            answers[answer_count++] = {result.numerator,result.denominator,answer_product};
            answer_product = multiply(answer_product,result.denominator);
        }
    }
    // Start the inverse accumulator in ORDINARY representation. Multiplying
    // it by an encoded denominator/prefix keeps it ordinary, so resolving a
    // fraction needs one multiplication and no per-answer Montgomery decode.
    u32 answer_inverse = decode(power(answer_product, modulus - 2));
    for (std::size_t index = query_count; index-- != 0;) {
        auto& current = answers[index];
        const u32 inverse = multiply(answer_inverse,current.prefix);
        answer_inverse = multiply(answer_inverse,current.denominator);
        const u32 result = multiply(current.numerator,inverse);
        current.numerator = result >= modulus ? result - modulus : result;
    }
    for (const auto& current : std::span{answers.get(), query_count}) {
        output.write(current.numerator);
    }
    output.flush();
}
} // namespace

int main() { solve(); }
