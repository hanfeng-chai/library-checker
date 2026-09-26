// BEGIN bundled: problems/data_structure/static_range_sum_with_upper_bound/offline_value_sweep_fenwick.cpp
// BEGIN bundled: include/toy/ds.hpp

#include <bits/extc++.h>
#include <immintrin.h>

namespace toy {

class DisjointSetUnion {
    std::vector<int> parent_or_size;

public:
    explicit DisjointSetUnion(int n) : parent_or_size(n, -1) {}

    int leader(int x) {
        int root = x;
        while (parent_or_size[root] >= 0) root = parent_or_size[root];
        while (x != root) {
            int parent = parent_or_size[x];
            parent_or_size[x] = root;
            x = parent;
        }
        return root;
    }

    bool merge(int a, int b) {
        a = leader(a);
        b = leader(b);
        if (a == b) return false;
        if (parent_or_size[a] > parent_or_size[b]) std::swap(a, b);
        parent_or_size[a] += parent_or_size[b];
        parent_or_size[b] = a;
        return true;
    }

    bool same(int a, int b) { return leader(a) == leader(b); }
    int size(int x) { return -parent_or_size[leader(x)]; }
};

class RollbackUnionFind {
    struct Change {
        int parent;
        int child;
        int child_size;
    };

    std::vector<int> parent_or_size;
    std::vector<Change> history;

public:
    explicit RollbackUnionFind(int n, std::size_t capacity = 0)
        : parent_or_size(n, -1) {
        history.reserve(capacity);
    }

    int leader(int vertex) const {
        while (parent_or_size[vertex] >= 0)
            vertex = parent_or_size[vertex];
        return vertex;
    }

    bool same(int first, int second) const {
        return leader(first) == leader(second);
    }

    bool merge(int first, int second) {
        first = leader(first);
        second = leader(second);
        if (first == second) {
            history.push_back({-1, -1, 0});
            return false;
        }
        if (parent_or_size[first] > parent_or_size[second])
            std::swap(first, second);
        history.push_back({first, second, parent_or_size[second]});
        parent_or_size[first] += parent_or_size[second];
        parent_or_size[second] = first;
        return true;
    }

    void undo() {
        Change change = history.back();
        history.pop_back();
        if (change.parent < 0) return;
        parent_or_size[change.parent] -= change.child_size;
        parent_or_size[change.child] = change.child_size;
    }

    std::size_t snapshot() const { return history.size(); }

    void rollback(std::size_t state) {
        while (history.size() > state) undo();
    }
};

template<class T>
class FenwickTree {
    std::vector<T> data;

public:
    explicit FenwickTree(int n) : data(n + 1) {}

    template<class Range>
    explicit FenwickTree(const Range& values) : data(values.size() + 1) {
        for (int i = 0; i < (int)values.size(); ++i) data[i + 1] += values[i];
        for (int i = 1; i < (int)data.size(); ++i) {
            int parent = i + (i & -i);
            if (parent < (int)data.size()) data[parent] += data[i];
        }
    }

    void add(int index, T delta) {
        for (++index; index < (int)data.size(); index += index & -index)
            data[index] += delta;
    }

    T prefix_sum(int end) const {
        T result{};
        for (; end; end -= end & -end) result += data[end];
        return result;
    }

    T sum(int left, int right) const {
        return prefix_sum(right) - prefix_sum(left);
    }

    int lower_bound(T target) const {
        if (target <= T{}) return 0;
        int index = 0;
        for (int step = std::bit_floor((unsigned)data.size()); step; step >>= 1) {
            int next = index + step;
            if (next < (int)data.size() && data[next] < target) {
                index = next;
                target -= data[next];
            }
        }
        return index;
    }
};

class FenwickBitset {
    std::vector<uint64_t> bits;
    std::vector<int> block_counts;

    static uint64_t low_bits(unsigned width) {
        return (uint64_t{1} << width) - 1;
    }

    int prefix_blocks(int end) const {
        int result = 0;
        for (; end; end -= end & -end) result += block_counts[end - 1];
        return result;
    }

public:
    explicit FenwickBitset(int size)
        : bits((size + 63) / 64 + 1),
          block_counts((size + 63) / 64) {}

    bool contains(int index) const {
        return bits[index >> 6] >> (index & 63) & 1;
    }

    void set(int index, bool value) {
        int block = index >> 6;
        uint64_t mask = uint64_t{1} << (index & 63);
        if (bool(bits[block] & mask) == value) return;
        bits[block] ^= mask;
        int delta = value ? 1 : -1;
        for (++block; block <= (int)block_counts.size();
             block += block & -block)
            block_counts[block - 1] += delta;
    }

    int count(int left, int right) const {
        int left_block = left >> 6;
        int right_block = right >> 6;
        int result =
            std::popcount(bits[right_block] & low_bits(right & 63)) -
            std::popcount(bits[left_block] & low_bits(left & 63));
        return result + prefix_blocks(right_block) -
               prefix_blocks(left_block);
    }
};

template<class T, std::size_t Capacity>
class FixedCenteredDeque {
    std::array<T, 2 * Capacity + 1> data{};
    std::size_t left = Capacity;
    std::size_t right = Capacity;

public:
    bool empty() const { return left == right; }
    std::size_t size() const { return right - left; }

    void push_front(const T& value) { data[--left] = value; }
    void push_back(const T& value) { data[right++] = value; }
    void pop_front() { ++left; }
    void pop_back() { --right; }

    T& operator[](std::size_t index) { return data[left + index]; }
    const T& operator[](std::size_t index) const {
        return data[left + index];
    }
};

class PredecessorSet {
    std::vector<std::vector<uint64_t>> levels;
    int universe;

    static int next_in_word(const std::vector<uint64_t>& words, int index) {
        int word = index >> 6;
        if (word >= (int)words.size()) return -1;
        uint64_t candidates = words[word] & (~0ULL << (index & 63));
        if (candidates) return word * 64 + std::countr_zero(candidates);
        return -1;
    }

    static int previous_in_word(const std::vector<uint64_t>& words, int index) {
        if (index < 0) return -1;
        int word = std::min<int>(index >> 6, words.size() - 1);
        uint64_t mask = (index & 63) == 63 ? ~0ULL : (1ULL << ((index & 63) + 1)) - 1;
        uint64_t candidates = words[word] & mask;
        if (candidates) return word * 64 + 63 - std::countl_zero(candidates);
        return -1;
    }

public:
    explicit PredecessorSet(int n) : universe(n) {
        for (int size = n; ; size = (size + 63) / 64) {
            levels.emplace_back((size + 63) / 64);
            if (size <= 64) break;
        }
    }

    void assign(std::string_view bits) {
        for (auto& level : levels) std::fill(level.begin(), level.end(), 0);
        int index = 0;
        for (; index + 64 <= universe; index += 64) {
            __m256i ones = _mm256_set1_epi8('1');
            uint32_t low = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                _mm256_loadu_si256((const __m256i*)(bits.data() + index)), ones));
            uint32_t high = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                _mm256_loadu_si256((const __m256i*)(bits.data() + index + 32)), ones));
            levels[0][index / 64] = low | (uint64_t)high << 32;
        }
        for (; index < universe; ++index)
            if (bits[index] == '1') levels[0][index / 64] |= 1ULL << (index % 64);
        for (int level = 1; level < (int)levels.size(); ++level)
            for (int child = 0; child < (int)levels[level - 1].size(); ++child)
                if (levels[level - 1][child])
                    levels[level][child / 64] |= 1ULL << (child % 64);
    }

    bool contains(int x) const {
        return levels[0][x >> 6] >> (x & 63) & 1;
    }

    void insert(int x) {
        for (auto& level : levels) {
            uint64_t& word = level[x >> 6];
            uint64_t bit = 1ULL << (x & 63);
            if (word & bit) break;
            word |= bit;
            x >>= 6;
        }
    }

    void erase(int x) {
        for (auto& level : levels) {
            uint64_t& word = level[x >> 6];
            word &= ~(1ULL << (x & 63));
            if (word) break;
            x >>= 6;
        }
    }

    int next(int x) const {
        if (x >= universe) return -1;
        int position = x;
        for (int level = 0; level < (int)levels.size(); ++level) {
            int found = next_in_word(levels[level], position);
            if (found >= 0) {
                while (level--) {
                    uint64_t word = levels[level][found];
                    found = found * 64 + std::countr_zero(word);
                }
                return found < universe ? found : -1;
            }
            position = (position >> 6) + 1;
        }
        return -1;
    }

    int previous(int x) const {
        if (universe == 0 || x < 0) return -1;
        int position = std::min(x, universe - 1);
        for (int level = 0; level < (int)levels.size(); ++level) {
            int found = previous_in_word(levels[level], position);
            if (found >= 0) {
                while (level--) {
                    uint64_t word = levels[level][found];
                    found = found * 64 + 63 - std::countl_zero(word);
                }
                return found;
            }
            position = (position >> 6) - 1;
        }
        return -1;
    }
};

template <std::size_t MaxUniverse>
class BoundedPredecessorSet {
        static_assert(MaxUniverse <= (1ULL << 24));
        static constexpr std::size_t leaf_count = (MaxUniverse + 63) / 64;
        static constexpr std::size_t level1_count = (leaf_count + 63) / 64;
        static constexpr std::size_t level2_count = (level1_count + 63) / 64;

        std::array<uint64_t, leaf_count> leaf{};
        std::array<uint64_t, level1_count> level1{};
        std::array<uint64_t, level2_count> level2{};
        uint64_t root = 0;
        std::size_t assigned_leaves = 0;

    public:
        void assign(std::string_view bits) {
            level1.fill(0);
            level2.fill(0);
            root = 0;
            std::size_t index = 0;
            const __m256i ones = _mm256_set1_epi8('1');
            for (; index + 64 <= bits.size(); index += 64) {
                uint32_t low = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                    _mm256_loadu_si256((const __m256i*)(bits.data() + index)), ones));
                uint32_t high = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                    _mm256_loadu_si256((const __m256i*)(bits.data() + index + 32)), ones));
                leaf[index / 64] = low | (uint64_t)high << 32;
            }
            if (index < bits.size()) {
                leaf[index / 64] = 0;
                for (; index < bits.size(); ++index)
                    if (bits[index] == '1') leaf[index / 64] |= 1ULL << (index % 64);
            }
            std::size_t used_leaves = (bits.size() + 63) / 64;
            if (used_leaves < assigned_leaves)
                std::fill(leaf.begin() + used_leaves, leaf.begin() + assigned_leaves, 0);
            assigned_leaves = used_leaves;
            for (std::size_t i = 0; i < used_leaves; ++i)
                level1[i / 64] |= uint64_t(leaf[i] != 0) << (i % 64);
            for (std::size_t i = 0; i < (leaf_count + 63) / 64; ++i)
                level2[i / 64] |= uint64_t(level1[i] != 0) << (i % 64);
            for (std::size_t i = 0; i < (level1_count + 63) / 64; ++i)
                root |= uint64_t(level2[i] != 0) << i;
        }

        void insert(unsigned x) {
            leaf[x >> 6] |= 1ULL << (x & 63);
            level1[x >> 12] |= 1ULL << ((x >> 6) & 63);
            level2[x >> 18] |= 1ULL << ((x >> 12) & 63);
            root |= 1ULL << (x >> 18);
        }

        void erase(unsigned x) {
            if (!(leaf[x >> 6] &= ~(1ULL << (x & 63))))
                if (!(level1[x >> 12] &= ~(1ULL << ((x >> 6) & 63))))
                    if (!(level2[x >> 18] &= ~(1ULL << ((x >> 12) & 63))))
                        root &= ~(1ULL << (x >> 18));
        }

        bool contains(unsigned x) const {
            return leaf[x >> 6] >> (x & 63) & 1;
        }

        int successor(unsigned x) const {
            uint64_t word = leaf[x >> 6] & (~0ULL << (x & 63));
            if (word) return int((x >> 6) << 6 | std::countr_zero(word));
            word = level1[x >> 12] & (-2ULL << ((x >> 6) & 63));
            if (word) {
                unsigned answer = (x >> 12) << 6 | std::countr_zero(word);
                return int(answer << 6 | std::countr_zero(leaf[answer]));
            }
            word = level2[x >> 18] & (-2ULL << ((x >> 12) & 63));
            if (word) {
                unsigned answer = (x >> 18) << 6 | std::countr_zero(word);
                answer = answer << 6 | std::countr_zero(level1[answer]);
                return int(answer << 6 | std::countr_zero(leaf[answer]));
            }
            word = root & (-2ULL << (x >> 18));
            if (!word) return -1;
            unsigned answer = std::countr_zero(word);
            answer = answer << 6 | std::countr_zero(level2[answer]);
            answer = answer << 6 | std::countr_zero(level1[answer]);
            return int(answer << 6 | std::countr_zero(leaf[answer]));
        }

        int predecessor(unsigned x) const {
            uint64_t word = leaf[x >> 6] & ~(-2ULL << (x & 63));
            if (word) return int((x >> 6) << 6 | (63 - std::countl_zero(word)));
            word = level1[x >> 12] & ~(~0ULL << ((x >> 6) & 63));
            if (word) {
                unsigned answer = (x >> 12) << 6 | (63 - std::countl_zero(word));
                return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
            }
            word = level2[x >> 18] & ~(~0ULL << ((x >> 12) & 63));
            if (word) {
                unsigned answer = (x >> 18) << 6 | (63 - std::countl_zero(word));
                answer = answer << 6 | (63 - std::countl_zero(level1[answer]));
                return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
            }
            word = root & ~(~0ULL << (x >> 18));
            if (!word) return -1;
            unsigned answer = 63 - std::countl_zero(word);
            answer = answer << 6 | (63 - std::countl_zero(level2[answer]));
            answer = answer << 6 | (63 - std::countl_zero(level1[answer]));
            return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
        }
};

} // namespace toy
// END bundled: include/toy/ds.hpp
// BEGIN bundled: include/toy/io.hpp

// Linux x86-64 / GCC / C++23 only. Reader APIs encode token shape so fixed,
// bounded, full-range, and 128-bit inputs use independently benchmarked paths.
// Writer uses base-10^4 groups; signed 128-bit output is split at 10^19 so no
// compiler __divti3/__modti3 helper remains in the hot loop.
#include <bits/extc++.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef NDEBUG
#define toy_assert(expr) [[assume(expr)]]
#else
#define toy_assert(expr) assert(expr)
#endif

namespace toy {

using namespace std;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;
using isize = intptr_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

struct DirectMappingTag {};
inline constexpr DirectMappingTag direct_mapping;
using usize = uintptr_t;
using f32 = float;
using f64 = double;
using f80 = long double;

namespace detail {

inline void write_all(const char* p, usize n) {
    while (n) {
        isize z = ::write(1, p, n);
        if (z > 0) p += z, n -= z;
        else if (z < 0 && errno == EINTR) continue;
        else break;
    }
}

// High half of a 128x128 product, assembled from four 64x64 products.
inline u128 mulhi(u128 a, u128 b) {
    u64 al = (u64)a, ah = (u64)(a >> 64);
    u64 bl = (u64)b, bh = (u64)(b >> 64);
    u128 ll = (u128)al * bl, lh = (u128)al * bh;
    u128 hl = (u128)ah * bl, hh = (u128)ah * bh;
    u128 mid = (ll >> 64) + (u64)lh + (u64)hl;
    return hh + (lh >> 64) + (hl >> 64) + (mid >> 64);
}

// q=floor(x/10^19) via ceil(2^192/10^19); r=x-q*10^19.
inline pair<u64, u64> divmod_1e19(u128 x) {
    constexpr u64 B = 10'000'000'000'000'000'000ULL;
    constexpr u128 M = ((u128)0xd83c94fb6d2ac34aULL << 64) |
                       0x5663d3c7a0d865cbULL;
    if (x < B) return {0, (u64)x};
    u128 h = mulhi(x, M), s = x + h;
    u64 q = (u64)((s >> 64) + (s < x));
    return {q, (u64)(x - (u128)q * B)};
}

} // namespace detail

class Reader {
    const char* p;

    static const char* map_stdin() {
        struct stat st{};
        fstat(0, &st);
        usize page = (usize)sysconf(_SC_PAGESIZE);
        usize size = (usize)st.st_size;
        usize mapped = (size + page - 1) & -page;
        void* base = mmap(nullptr, mapped + page, PROT_READ,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        toy_assert(base != MAP_FAILED);
        if (size) {
            void* file = mmap(base, size, PROT_READ,
                              MAP_PRIVATE | MAP_FIXED, 0, 0);
            toy_assert(file == base);
        }
        return (const char*)base;
    }

    static const char* map_stdin_direct() {
        struct stat st{};
        fstat(0, &st);
        void* base = mmap(nullptr, (usize)st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
        toy_assert(base != MAP_FAILED);
        return (const char*)base;
    }

    static const array<array<char, 16>, 17>& masks() {
        static constexpr auto a = [] {
            array<array<char, 16>, 17> result{};
            for (int n = 0; n <= 16; ++n)
                for (int i = 0; i < 16; ++i)
                    result[n][i] = i < 16 - n ? (char)0x80 : (char)(i - 16 + n);
            return result;
        }();
        return a;
    }

    static u64 parse16(__m128i x) {
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010a));
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));
        __m128i a = _mm_mul_epu32(x, _mm_set_epi32(1, 10000, 1, 10000));
        x = _mm_add_epi64(a, _mm_srli_epi64(x, 32));
        return (u64)_mm_cvtsi128_si64(x) * 100000000ULL +
               (u64)_mm_extract_epi64(x, 1);
    }

    static bool all_digit(u64 x) {
        return !(((x ^ 0x3030303030303030ULL) & 0xf0f0f0f0f0f0f0f0ULL));
    }

    static bool all_digit4(u32 x) {
        return !(((x ^ 0x30303030U) & 0xf0f0f0f0U));
    }

    static u32 parse4(u32 x) {
        x ^= 0x30303030U;
        x = (x * 10 + (x >> 8)) & 0x00ff00ffU;
        return (x * 100 + (x >> 16)) & 0x0000ffffU;
    }

    static u64 parse8(u64 x) {
        x ^= 0x3030303030303030ULL;
        x = (x * 10 + (x >> 8)) & 0x00ff00ff00ff00ffULL;
        x = (x * 100 + (x >> 16)) & 0x0000ffff0000ffffULL;
        return (x * 10000 + (x >> 32)) & 0x00000000ffffffffULL;
    }

    static u128 parse32(__m256i x) {
        x = _mm256_maddubs_epi16(x, _mm256_set1_epi16(0x010a));
        x = _mm256_madd_epi16(x, _mm256_set1_epi32(0x00010064));
        __m256i a = _mm256_mul_epu32(
            x, _mm256_set_epi32(1, 10000, 1, 10000, 1, 10000, 1, 10000));
        x = _mm256_add_epi64(a, _mm256_srli_epi64(x, 32));
        __m128i a0 = _mm256_castsi256_si128(x);
        __m128i a1 = _mm256_extracti128_si256(x, 1);
        u64 hi = (u64)_mm_cvtsi128_si64(a0) * 100000000ULL +
                 (u64)_mm_extract_epi64(a0, 1);
        u64 lo = (u64)_mm_cvtsi128_si64(a1) * 100000000ULL +
                 (u64)_mm_extract_epi64(a1, 1);
        return (u128)hi * 10000000000000000ULL + lo;
    }

    static const array<u64, 17>& powers10() {
        static constexpr auto a = [] {
            array<u64, 17> result{1};
            for (int i = 1; i <= 16; ++i) result[i] = result[i - 1] * 10;
            return result;
        }();
        return a;
    }

    [[gnu::always_inline]] static u64 parse_short16(__m128i x, u32 boundary,
                                                     int& digits) {
        digits = __builtin_ctz(boundary);
        x = _mm_shuffle_epi8(
            x, _mm_loadu_si128((const __m128i*)masks()[digits].data()));
        return parse16(x);
    }

    [[gnu::always_inline]] u64 read_swar_u64() {
        const char* q = p;
        u64 a, b;
        memcpy(&a, q, 8); memcpy(&b, q + 8, 8);
        if (all_digit(a)) {
            u64 v = parse8(a); q += 8;
            if (all_digit(b)) v = v * 100000000ULL + parse8(b), q += 8;
            while (*q >= '0') v = v * 10 + *q++ - '0';
            p = q + 1;
            return v;
        }
        u64 v = 0;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        p = q + 1;
        return v;
    }

    [[gnu::always_inline]] u64 read_swar4_u64() {
        const char* q = p;
        u32 block;
        memcpy(&block, q, 4);
        u64 value = 0;
        if (all_digit4(block)) {
            value = parse4(block);
            q += 4;
        }
        while (*q >= '0') value = value * 10 + *q++ - '0';
        p = q + 1;
        return value;
    }

    [[gnu::always_inline]] u64 read_scalar_u64() {
        u64 v = 0;
        while (*p >= '0') v = v * 10 + *p++ - '0';
        ++p;
        return v;
    }

    [[gnu::always_inline]] u64 read_simd_u64() {
        const char* q = p;
        __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                                  _mm_set1_epi8('0'));
        u32 boundary = (u32)_mm_movemask_epi8(x);
        if (__builtin_expect(boundary != 0, 1)) {
            int digits = __builtin_ctz(boundary);
            x = _mm_shuffle_epi8(
                x, _mm_loadu_si128((const __m128i*)masks()[digits].data()));
            p = q + digits + 1;
            return parse16(x);
        }
        u64 v = parse16(x); q += 16;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        p = q + 1;
        return v;
    }

    [[gnu::always_inline]] static u128 read_avx_u128(const char*& cursor) {
        const char* q = cursor;
        u128 v;
        __m256i x = _mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)q),
                                     _mm256_set1_epi8('0'));
        __m256i hi = _mm256_and_si256(x, _mm256_set1_epi8((char)0xf0));
        u32 bad = ~((u32)_mm256_movemask_epi8(
            _mm256_cmpeq_epi8(hi, _mm256_setzero_si256())));
        if (!bad) {
            v = parse32(x); q += 32;
            u64 tail = 0, scale = 1;
            while (*q >= '0') {
                tail = tail * 10 + *q++ - '0';
                scale *= 10;
            }
            v = v * scale + tail;
            cursor = q + 1;
            return v;
        }
        int n = __builtin_ctz(bad);
        if (n) {
            if (n <= 16) {
                __m128i lo = _mm256_castsi256_si128(x);
                lo = _mm_shuffle_epi8(
                    lo, _mm_loadu_si128((const __m128i*)masks()[n].data()));
                v = parse16(lo);
            } else {
                alignas(32) u8 b[64]{};
                _mm256_storeu_si256((__m256i*)(b + 32 - n), x);
                v = parse32(_mm256_load_si256((const __m256i*)b));
            }
            cursor = q + n + 1;
            return v;
        }
        v = 0;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        cursor = q + 1;
        return v;
    }

    [[gnu::always_inline]] static u128 read_staged_u128(const char*& cursor) {
        const char* q = cursor;
        __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                                  _mm_set1_epi8('0'));
        u32 boundary = (u32)_mm_movemask_epi8(x);
        if (boundary) {
            int digits;
            u128 value = parse_short16(x, boundary, digits);
            cursor = q + digits + 1;
            return value;
        }

        u128 value = parse16(x);
        q += 16;
        x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                         _mm_set1_epi8('0'));
        boundary = (u32)_mm_movemask_epi8(x);
        if (boundary) {
            int digits;
            u64 tail = parse_short16(x, boundary, digits);
            cursor = q + digits + 1;
            return value * powers10()[digits] + tail;
        }

        value = value * 10000000000000000ULL + parse16(x);
        q += 16;
        x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                         _mm_set1_epi8('0'));
        boundary = (u32)_mm_movemask_epi8(x);
        int digits;
        u64 tail = parse_short16(x, boundary, digits);
        cursor = q + digits + 1;
        return value * powers10()[digits] + tail;
    }

    template<int Digits, class U>
    [[gnu::always_inline]] U read_fixed_unsigned() {
        static_assert(1 <= Digits && Digits <= 37);
        U v;
        if constexpr (Digits <= 7) {
            v = 0;
            for (int i = 0; i < Digits; ++i) v = v * 10 + p[i] - '0';
        } else if constexpr (Digits == 8) {
            u64 x;
            memcpy(&x, p, 8);
            v = (U)parse8(x);
        } else if constexpr (Digits <= 16) {
            __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p),
                                      _mm_set1_epi8('0'));
            x = _mm_shuffle_epi8(
                x, _mm_loadu_si128((const __m128i*)masks()[Digits].data()));
            v = (U)parse16(x);
        } else if constexpr (Digits < 32) {
            if constexpr (Digits >= 24) {
                v = 0;
                int offset = 0;
                for (; offset + 8 <= Digits; offset += 8) {
                    u64 block;
                    memcpy(&block, p + offset, 8);
                    v = v * 100000000 + parse8(block);
                }
                for (; offset < Digits; ++offset) v = v * 10 + p[offset] - '0';
            } else {
                __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p),
                                          _mm_set1_epi8('0'));
                v = (U)parse16(x);
                for (int i = 16; i < Digits; ++i) v = v * 10 + p[i] - '0';
            }
        } else {
            __m256i x = _mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)p),
                                         _mm256_set1_epi8('0'));
            v = (U)parse32(x);
            for (int i = 32; i < Digits; ++i) v = v * 10 + p[i] - '0';
        }
        p += Digits + 1;
        return v;
    }

    template<class T>
    [[gnu::always_inline]] T read_signed(auto read_unsigned) {
        bool neg = *p == '-'; p += neg;
        auto v = (this->*read_unsigned)();
        using U = make_unsigned_t<T>;
        U magnitude = (U)v;
        return neg ? (T)(U(0) - magnitude) : (T)magnitude;
    }

public:
    Reader(): p(map_stdin()) {}
    explicit Reader(DirectMappingTag): p(map_stdin_direct()) {}
    explicit Reader(const char* input): p(input) {}

    [[gnu::always_inline]] u32 read_digit() {
        u32 value = (u32)(*p - '0');
        p += 2;
        return value;
    }

    [[gnu::always_inline]] u32 read_u32() { return (u32)read_swar_u64(); }
    [[gnu::always_inline]] i32 read_i32() { return read_signed<i32>(&Reader::read_simd_u64); }
    [[gnu::always_inline]] u64 read_u64() { return read_simd_u64(); }
    [[gnu::always_inline]] i64 read_i64() { return read_signed<i64>(&Reader::read_simd_u64); }
    [[gnu::always_inline]] u128 read_u128() { return read_avx_u128(p); }
    [[gnu::always_inline]] i128 read_i128() {
        bool neg = *p == '-'; p += neg;
        u128 value = read_avx_u128(p);
        return neg ? (i128)(u128(0) - value) : (i128)value;
    }
    [[gnu::always_inline]] i128 read_staged_i128() {
        bool neg = *p == '-'; p += neg;
        u128 value = read_staged_u128(p);
        return neg ? (i128)(u128(0) - value) : (i128)value;
    }

    template<int Digits, class T = u64>
    [[gnu::always_inline]] T read_fixed() {
        static_assert(is_integral_v<T> || same_as<T, i128> || same_as<T, u128>);
        if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value = read_fixed_unsigned<Digits, u128>();
            return neg ? (i128)(u128(0) - value) : (i128)value;
        } else if constexpr (same_as<T, u128>) {
            return read_fixed_unsigned<Digits, u128>();
        } else if constexpr (is_signed_v<T>) {
            bool neg = *p == '-'; p += neg;
            using U = make_unsigned_t<T>;
            U value = read_fixed_unsigned<Digits, U>();
            return neg ? (T)(U(0) - value) : (T)value;
        } else {
            return read_fixed_unsigned<Digits, T>();
        }
    }

    template<int MaxDigits, class T = u64>
    [[gnu::always_inline]] T read_var() {
        static_assert(1 <= MaxDigits && MaxDigits <= 37);
        if constexpr (same_as<T, u128>) {
            if constexpr (MaxDigits <= 19) return (u128)read_simd_u64();
            else return read_u128();
        } else if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value;
            if constexpr (MaxDigits <= 18) value = read_simd_u64();
            else value = read_avx_u128(p);
            return neg ? (i128)(u128(0) - value) : (i128)value;
        }
        else if constexpr (is_signed_v<T>) {
            if constexpr (MaxDigits <= 2)
                return read_signed<T>(&Reader::read_scalar_u64);
            else return read_signed<T>(&Reader::read_simd_u64);
        } else {
            if constexpr (MaxDigits <= 2) return (T)read_scalar_u64();
            else return (T)read_simd_u64();
        }
    }

    template<int MaxDigits, class T = u64>
    [[gnu::always_inline]] T read_uniform() {
        static_assert(1 <= MaxDigits && MaxDigits <= 37);
        if constexpr (same_as<T, u128>) {
            if constexpr (MaxDigits <= 19) return (u128)read_simd_u64();
            else if constexpr (32 <= MaxDigits && MaxDigits <= 34)
                return read_avx_u128(p);
            else return read_staged_u128(p);
        } else if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value;
            if constexpr (MaxDigits <= 18) value = read_simd_u64();
            else if constexpr (32 <= MaxDigits && MaxDigits <= 36)
                value = read_avx_u128(p);
            else value = read_staged_u128(p);
            return neg ? (i128)(u128(0) - value) : (i128)value;
        } else {
            bool neg = false;
            if constexpr (is_signed_v<T>) neg = *p == '-', p += neg;
            u64 value;
            if constexpr (is_signed_v<T>) {
                if constexpr (MaxDigits <= 3) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 7) value = read_swar4_u64();
                else if constexpr (MaxDigits <= 13) value = read_swar_u64();
                else value = read_simd_u64();
            } else {
                if constexpr (MaxDigits <= 3) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 7) value = read_swar4_u64();
                else if constexpr (MaxDigits <= 12) value = read_swar_u64();
                else value = read_simd_u64();
            }
            using U = make_unsigned_t<T>;
            U magnitude = (U)value;
            if constexpr (is_signed_v<T>)
                return neg ? (T)(U(0) - magnitude) : (T)magnitude;
            else return (T)magnitude;
        }
    }

    template<u32 Mod>
    [[gnu::always_inline]] u32 read_mod() {
        u32 value = read_u32();
        toy_assert(value < Mod);
        return value;
    }

    template<class T = u32>
    [[gnu::always_inline]] T read_index(T size) {
        T value = read<T>();
        toy_assert(value < size);
        return value;
    }

    string_view read_token() {
        const char* begin = p;
        while (*p > ' ') ++p;
        string_view token(begin, p);
        ++p;
        return token;
    }

    [[gnu::always_inline]] string_view read_token(usize length) {
        string_view token(p, length);
        p += length;
        toy_assert(*p <= ' ');
        ++p;
        return token;
    }

    [[gnu::always_inline]] void skip_spaces() {
        while (*p <= ' ') ++p;
    }

    template<class T> [[gnu::always_inline]] T read() {
        if constexpr (same_as<T, i128>) return read_i128();
        else if constexpr (same_as<T, u128>) return read_u128();
        else if constexpr (is_signed_v<T> && sizeof(T) <= 4) return (T)read_i32();
        else if constexpr (is_signed_v<T>) return (T)read_i64();
        else if constexpr (sizeof(T) <= 4) return (T)read_u32();
        else return (T)read_u64();
    }
};

template<usize N = 1 << 19, bool Compact = false>
class Writer {
    alignas(64) array<char, N> storage;
    char* begin = storage.data();
    char* p = begin;
    char* end = begin + N;

    static constexpr u32 packed_digits(int x) {
        return (u32)('0' + x / 1000) |
               (u32)('0' + x / 100 % 10) << 8 |
               (u32)('0' + x / 10 % 10) << 16 |
               (u32)('0' + x % 10) << 24;
    }

    static const array<u32, 10000>& lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) result[x] = packed_digits(x);
            return result;
        }();
        return a;
    }

    static const array<u32, 10000>& first_lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) {
                u32 value = packed_digits(x);
                if (x < 1000) value = (value & 0xffffff00U) | ' ';
                if (x < 100) value = (value & 0xffff00ffU) | (u32)' ' << 8;
                if (x < 10) value = (value & 0xff00ffffU) | (u32)' ' << 16;
                if (x == 0) value = (value & 0x00ffffffU) | (u32)' ' << 24;
                result[x] = value;
            }
            return result;
        }();
        return a;
    }

    static const array<u32, 10000>& negative_lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) {
                u32 value = packed_digits(x);
                if (x < 1000) value = (value & 0xffffff00U) | ' ';
                if (x < 100) value = (value & 0xffff00ffU) | (u32)' ' << 8;
                if (x < 10) value = (value & 0xff00ffffU) | (u32)' ' << 16;
                unsigned offset = x >= 100 ? 0 : x >= 10 ? 8 : x ? 16 : 24;
                result[x] = (value & ~(0xffU << offset)) | (u32)'-' << offset;
            }
            return result;
        }();
        return a;
    }

    template<u64 Magic, int Shift>
    [[gnu::always_inline]] static u64 reciprocal_div(u64 x) {
        return (u64)(((u128)x * Magic) >> 64) >> Shift;
    }

    [[gnu::always_inline]] static u64 div_1e4(u64 x) {
        return reciprocal_div<0x346dc5d63886594bULL, 11>(x);
    }

    [[gnu::always_inline]] static u64 div_1e8(u64 x) {
        return reciprocal_div<0xabcc77118461cefdULL, 26>(x);
    }

    [[gnu::always_inline]] static u64 div_1e12(u64 x) {
        return reciprocal_div<0x232f33025bd42233ULL, 37>(x);
    }

    [[gnu::always_inline]] static u64 div_1e16(u64 x) {
        return reciprocal_div<0x39a5652fb1137857ULL, 51>(x);
    }

    [[gnu::always_inline]] void ensure(usize n) {
        if ((usize)(end - p) < n) flush();
    }

    [[gnu::always_inline]] static void group(char*& cursor, u32 s) {
        memcpy(cursor, &s, 4); cursor += 4;
    }

    [[gnu::always_inline]] static void first_group(char*& cursor, u64 x) {
        if constexpr (Compact) {
            unsigned skip = 3 - (x >= 10) - (x >= 100) - (x >= 1000);
            u32 s = lut()[x] >> (skip * 8);
            memcpy(cursor, &s, 4);
            cursor += 4 - skip;
        } else {
            group(cursor, x ? first_lut()[x]
                            : (u32)' ' | (u32)' ' << 8 |
                              (u32)' ' << 16 | (u32)'0' << 24);
        }
    }

    [[gnu::always_inline]] static void u64_raw(char*& cursor, u64 x) {
        const auto& table = lut();
        if constexpr (!Compact) {
            if (x >= 10'000'000'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                u64 middle = high % 10000;
                u64 top = high / 10000;
                first_group(cursor, top / 10000);
                group(cursor, table[top % 10000]);
                group(cursor, table[middle]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 1'000'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                first_group(cursor, high / 10000);
                group(cursor, table[high % 10000]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 100'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                first_group(cursor, x / 100'000'000ULL);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 10000ULL) {
                first_group(cursor, x / 10000);
                group(cursor, table[x % 10000]);
            } else if (x) {
                first_group(cursor, x);
            } else {
                group(cursor, (u32)' ' | (u32)' ' << 8 |
                              (u32)' ' << 16 | (u32)'0' << 24);
            }
        } else if (x > 9999'9999'9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            u64 q3 = div_1e12(x), q4 = div_1e16(x);
            first_group(cursor, q4);
            group(cursor, table[q3 - q4 * 10000]);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999'9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            first_group(cursor, q3);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            first_group(cursor, q2);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999ULL) {
            u64 q1 = div_1e4(x);
            first_group(cursor, q1);
            group(cursor, table[x - q1 * 10000]);
        } else {
            first_group(cursor, x);
        }
    }

    template<u64 Max>
    [[gnu::always_inline]] static void compact_u64_bounded(char*& cursor, u64 x) {
        static_assert(Max <= numeric_limits<u64>::max());
        const auto& table = lut();
        if constexpr (Max <= 9'999ULL) {
            first_group(cursor, x);
        } else if constexpr (Max <= 99'999'999ULL) {
            u64 q1 = div_1e4(x);
            if (q1) first_group(cursor, q1), group(cursor, table[x - q1 * 10000]);
            else first_group(cursor, x);
        } else if constexpr (Max <= 999'999'999'999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            if (q2) {
                first_group(cursor, q2);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q1) {
                first_group(cursor, q1);
                group(cursor, table[x - q1 * 10000]);
            } else {
                first_group(cursor, x);
            }
        } else if constexpr (Max <= 9'999'999'999'999'999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            if (q3) {
                first_group(cursor, q3);
                group(cursor, table[q2 - q3 * 10000]);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q2) {
                first_group(cursor, q2);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q1) {
                first_group(cursor, q1);
                group(cursor, table[x - q1 * 10000]);
            } else {
                first_group(cursor, x);
            }
        } else {
            u64_raw(cursor, x);
        }
    }

    template<unsigned Digits>
    [[gnu::always_inline]] static void u64_fixed(char*& cursor, u64 x) {
        static_assert(1 <= Digits && Digits <= 20);
        constexpr unsigned groups = (Digits + 3) / 4;
        constexpr unsigned leading = Digits - 4 * (groups - 1);
        auto first_fixed = [&](u64 value) {
            u32 first = lut()[value] >> ((4 - leading) * 8);
            memcpy(cursor, &first, 4);
            cursor += leading;
        };
        const auto& table = lut();
        if constexpr (groups == 1) {
            first_fixed(x);
        } else if constexpr (groups == 2) {
            u64 q1 = div_1e4(x);
            first_fixed(q1);
            group(cursor, table[x - q1 * 10000]);
        } else if constexpr (groups == 3) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            first_fixed(q2);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if constexpr (groups == 4) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            first_fixed(q3);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            u64 q3 = div_1e12(x), q4 = div_1e16(x);
            first_fixed(q4);
            group(cursor, table[q3 - q4 * 10000]);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        }
    }

    template<unsigned Digits>
    [[gnu::always_inline]] static void i128_fixed(char*& cursor, i128 x) {
        static_assert(1 <= Digits && Digits <= 39);
        bool negative = x < 0;
        u128 magnitude = negative ? u128(0) - (u128)x : (u128)x;
        if (negative) *cursor++ = '-';
        if constexpr (Digits <= 20) {
            u64_fixed<Digits>(cursor, (u64)magnitude);
        } else {
            auto [high, low] = detail::divmod_1e19(magnitude);
            u64_fixed<Digits - 19>(cursor, high);
            fixed19(cursor, low);
        }
    }

    [[gnu::always_inline]] static void i64_raw(char*& cursor, u64 x) {
        if constexpr (Compact) {
            *cursor++ = '-';
            u64_raw(cursor, x);
        } else {
            const auto& next = lut();
            const auto& negative = negative_lut();
            if (x >= 10'000'000'000'000'000'000ULL) {
                *cursor++ = '-'; u64_raw(cursor, x);
            } else if (x > 999'9999'9999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x);
                u64 q3 = div_1e12(x), q4 = div_1e16(x);
                group(cursor, negative[q4]);
                group(cursor, next[q3 - q4 * 10000]);
                group(cursor, next[q2 - q3 * 10000]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999'9999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
                group(cursor, negative[q3]);
                group(cursor, next[q2 - q3 * 10000]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x);
                group(cursor, negative[q2]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999ULL) {
                u64 q1 = div_1e4(x);
                group(cursor, negative[q1]);
                group(cursor, next[x - q1 * 10000]);
            } else {
                group(cursor, negative[x]);
            }
        }
    }

    [[gnu::always_inline]] static void fixed19(char*& cursor, u64 x) {
        const auto& next = lut();
        u64 q1 = div_1e4(x), q2 = div_1e8(x);
        u64 q3 = div_1e12(x), q4 = div_1e16(x);
        u32 first = next[q4] >> 8;
        memcpy(cursor, &first, 4); cursor += 3;
        group(cursor, next[q3 - q4 * 10000]);
        group(cursor, next[q2 - q3 * 10000]);
        group(cursor, next[q1 - q2 * 10000]);
        group(cursor, next[x - q1 * 10000]);
    }

public:
    ~Writer() { flush(); }
    void flush() { detail::write_all(begin, p - begin); p = begin; }
    void put(char c) {
        ensure(1);
        *p++ = c;
    }
    void write(string_view s) {
        while (!s.empty()) {
            usize n = min<usize>(s.size(), end - p);
            if (!n) {
                flush();
                continue;
            }
            memcpy(p, s.data(), n);
            p += n;
            s.remove_prefix(n);
        }
    }

    [[gnu::always_inline]] void write_padded_u32(u32 x) {
        ensure(16);
        char* cursor = p;
        *cursor++ = ' ';
        if (x > 99'999'999U) {
            u32 high = x / 100'000'000U;
            u32 middle = x / 10'000U % 10'000U;
            group(cursor, first_lut()[high]);
            group(cursor, lut()[middle]);
            group(cursor, lut()[x % 10'000U]);
        } else if (x > 9'999U) {
            u32 high = x / 10'000U;
            group(cursor, first_lut()[high]);
            group(cursor, lut()[x % 10'000U]);
        } else if (x) {
            group(cursor, first_lut()[x]);
        } else {
            group(cursor, (u32)' ' | (u32)' ' << 8 |
                          (u32)' ' << 16 | (u32)'0' << 24);
        }
        p = cursor;
    }

    [[gnu::always_inline]] void write_token_u32_6(u32 x) {
        toy_assert(x < 1'000'000U);
        ensure(8);
        char* cursor = p;
        *cursor++ = ' ';
        if (x >= 10'000U) {
            u32 high = x / 10'000U;
            u32 digits = lut()[high];
            if (high >= 10) {
                digits >>= 16;
                memcpy(cursor, &digits, 2);
                cursor += 2;
            } else {
                *cursor++ = (char)(digits >> 24);
            }
            group(cursor, lut()[x % 10'000U]);
        } else if (x) {
            group(cursor, first_lut()[x]);
        } else {
            group(cursor, (u32)' ' | (u32)' ' << 8 |
                          (u32)' ' << 16 | (u32)'0' << 24);
        }
        p = cursor;
    }

    [[gnu::always_inline]] void writeln(u64 x) {
        ensure(24);
        char* cursor = p;
        u64_raw(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    [[gnu::always_inline]] void writeln_i64(i64 x) {
        ensure(24);
        char* cursor = p;
        if (x < 0)
            i64_raw(cursor, u64(0) - (u64)x);
        else
            u64_raw(cursor, (u64)x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void writeln_fixed(u64 x) {
        ensure(24);
        char* cursor = p;
        u64_fixed<Digits>(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void write_token_fixed(u64 x) {
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        u64_fixed<Digits>(cursor, x);
        p = cursor;
    }
    template<u64 Max>
    [[gnu::always_inline]] void writeln_bounded(u64 x) {
        toy_assert(x <= Max);
        ensure(24);
        char* cursor = p;
        if constexpr (Compact) compact_u64_bounded<Max>(cursor, x);
        else u64_raw(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    [[gnu::always_inline]] void write_token(u64 x) {
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        u64_raw(cursor, x);
        p = cursor;
    }
    template<u64 Max>
    [[gnu::always_inline]] void write_token_bounded(u64 x) {
        toy_assert(x <= Max);
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        if constexpr (Compact) compact_u64_bounded<Max>(cursor, x);
        else u64_raw(cursor, x);
        p = cursor;
    }
    [[gnu::always_inline]] void writeln(i128 x) {
        ensure(48); bool neg = x < 0; u128 v = neg ? u128(0) - (u128)x : (u128)x;
        auto [q, r] = detail::divmod_1e19(v);
        char* cursor = p;
        if (neg) {
            if (q) i64_raw(cursor, q), fixed19(cursor, r); else i64_raw(cursor, r);
        } else {
            if (q) u64_raw(cursor, q), fixed19(cursor, r); else u64_raw(cursor, r);
        }
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void writeln_fixed(i128 x) {
        ensure(48);
        char* cursor = p;
        i128_fixed<Digits>(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void write_token_fixed(i128 x) {
        ensure(48);
        char* cursor = p;
        *cursor++ = ' ';
        i128_fixed<Digits>(cursor, x);
        p = cursor;
    }
};

template<usize N = 1 << 19>
using CompactWriter = Writer<N, true>;

} // namespace toy
// END bundled: include/toy/io.hpp
// BEGIN bundled: include/toy/sort.hpp

#include <bits/extc++.h>

namespace toy {

template<unsigned KeyBits = 32, unsigned RadixBits = 16,
         std::random_access_iterator Iterator, class Key>
void radix_sort_u32(Iterator first, Iterator last, Key key) {
    static_assert(1 <= RadixBits && RadixBits <= 16);
    static_assert(RadixBits < KeyBits && KeyBits <= 2 * RadixBits);
    constexpr uint32_t bucket_count = uint32_t(1) << RadixBits;
    constexpr uint32_t mask = bucket_count - 1;
    using Value = std::iter_value_t<Iterator>;

    std::size_t count = last - first;
    if (count < bucket_count / 8) {
        std::stable_sort(first, last, [&](const Value& left, const Value& right) {
            return key(left) < key(right);
        });
        return;
    }

    std::array<uint32_t, bucket_count> low_count{}, high_count{};
    for (const Value& value : std::ranges::subrange(first, last)) {
        uint32_t current = key(value);
        if constexpr (KeyBits < 32)
            assert(current < (uint32_t(1) << KeyBits));
        ++low_count[current & mask];
        ++high_count[(current >> RadixBits) & mask];
    }
    for (uint32_t i = 1; i < bucket_count; ++i) {
        low_count[i] += low_count[i - 1];
        high_count[i] += high_count[i - 1];
    }

    std::vector<Value> temporary(count);
    for (std::size_t i = count; i--;) {
        uint32_t current = key(first[i]);
        temporary[--low_count[current & mask]] = std::move(first[i]);
    }
    for (std::size_t i = count; i--;) {
        uint32_t current = key(temporary[i]);
        first[--high_count[(current >> RadixBits) & mask]] =
            std::move(temporary[i]);
    }
}

template<unsigned KeyBits = 64, unsigned RadixBits = 16,
         std::random_access_iterator Iterator, class Key>
void radix_sort_u64(Iterator first, Iterator last, Key key) {
    static_assert(1 <= RadixBits && RadixBits <= 16);
    static_assert(RadixBits < KeyBits && KeyBits <= 64);
    constexpr uint32_t bucket_count = uint32_t(1) << RadixBits;
    constexpr uint64_t mask = bucket_count - 1;
    constexpr unsigned passes = (KeyBits + RadixBits - 1) / RadixBits;
    using Value = std::iter_value_t<Iterator>;

    std::size_t count = last - first;
    if (count < bucket_count / 8) {
        std::stable_sort(first, last, [&](const Value& left, const Value& right) {
            return key(left) < key(right);
        });
        return;
    }

    std::vector<Value> temporary(count);
    bool in_temporary = false;
    for (unsigned pass = 0; pass < passes; ++pass) {
        unsigned shift = pass * RadixBits;
        std::array<uint32_t, bucket_count> frequencies{};
        if (!in_temporary) {
            for (std::size_t i = 0; i < count; ++i)
                ++frequencies[key(first[i]) >> shift & mask];
        } else {
            for (const Value& value : temporary)
                ++frequencies[key(value) >> shift & mask];
        }
        for (uint32_t i = 1; i < bucket_count; ++i)
            frequencies[i] += frequencies[i - 1];
        if (!in_temporary) {
            for (std::size_t i = count; i--;)
                temporary[--frequencies[key(first[i]) >> shift & mask]] =
                    std::move(first[i]);
        } else {
            for (std::size_t i = count; i--;)
                first[--frequencies[key(temporary[i]) >> shift & mask]] =
                    std::move(temporary[i]);
        }
        in_temporary = !in_temporary;
    }
    if (in_temporary)
        std::move(temporary.begin(), temporary.end(), first);
}

} // namespace toy
// END bundled: include/toy/sort.hpp

namespace {
struct Aggregate {
    toy::u64 sum = 0;
    int count = 0;

    Aggregate& operator+=(const Aggregate& other) {
        sum += other.sum;
        count += other.count;
        return *this;
    }

    friend Aggregate operator-(Aggregate left, const Aggregate& right) {
        left.sum -= right.sum;
        left.count -= right.count;
        return left;
    }
};

struct Item {
    toy::u32 value;
    int position;
};

struct Query {
    int left;
    int right;
    toy::u32 bound;
    int index;
};
}

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i)
        items[i] = {input.read_uniform<10, toy::u32>(), i};
    std::vector<Query> queries(query_count);
    for (int i = 0; i < query_count; ++i) {
        queries[i].left = input.read_uniform<6, toy::u32>();
        queries[i].right = input.read_uniform<6, toy::u32>();
        queries[i].bound = input.read_uniform<10, toy::u32>();
        queries[i].index = i;
    }
    toy::radix_sort_u32<30, 15>(
        items.begin(), items.end(), [](const Item& item) { return item.value; });
    toy::radix_sort_u32<30, 15>(
        queries.begin(), queries.end(),
        [](const Query& query) { return query.bound; });

    toy::FenwickTree<Aggregate> tree(n);
    std::vector<Aggregate> answers(query_count);
    int inserted = 0;
    for (const Query& query : queries) {
        while (inserted < n && items[inserted].value <= query.bound) {
            tree.add(items[inserted].position,
                     {items[inserted].value, 1});
            ++inserted;
        }
        answers[query.index] = tree.sum(query.left, query.right);
    }
    for (const Aggregate& answer : answers) {
        output.write_token((toy::u64)answer.count);
        output.write_token(answer.sum);
        output.put('\n');
    }
}
// END bundled: problems/data_structure/static_range_sum_with_upper_bound/offline_value_sweep_fenwick.cpp
