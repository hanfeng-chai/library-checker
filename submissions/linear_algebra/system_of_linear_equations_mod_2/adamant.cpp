#line 1 "cp-algorithms-aux/verify/structures/bitpack/system_mod_2.test.cpp"
// @brief System of Linear Equations (Mod 2)
#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations_mod_2"
#pragma GCC optimize("O3,unroll-loops")
#line 1 "cp-algorithms-aux/cp-algo/structures/bit_array_util.hpp"


#line 1 "cp-algorithms-aux/cp-algo/structures/bit_array.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/bit.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/simd.hpp"


#include <experimental/simd>
#include <cstdint>
#include <cstddef>
#include <memory>

#if defined(__x86_64__) && !defined(CP_ALGO_DISABLE_AVX2)
#define CP_ALGO_SIMD_AVX2_TARGET _Pragma("GCC target(\"avx2\")")
#else
#define CP_ALGO_SIMD_AVX2_TARGET
#endif

#define CP_ALGO_SIMD_PRAGMA_PUSH \
    _Pragma("GCC push_options") \
    CP_ALGO_SIMD_AVX2_TARGET

CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo {
    template<typename T, size_t len>
    using simd [[gnu::vector_size(len * sizeof(T))]] = T;
    using i64x4 = simd<int64_t, 4>;
    using u64x4 = simd<uint64_t, 4>;
    using u32x8 = simd<uint32_t, 8>;
    using i32x4 = simd<int32_t, 4>;
    using u32x4 = simd<uint32_t, 4>;
    using i16x4 = simd<int16_t, 4>;
    using u8x32 = simd<uint8_t, 32>;
    using dx4 = simd<double, 4>;

    dx4 abs(dx4 a) {
        return dx4{
            std::abs(a[0]),
            std::abs(a[1]),
            std::abs(a[2]),
            std::abs(a[3])
        };
    }

    // https://stackoverflow.com/a/77376595
    // works for ints in (-2^51, 2^51)
    static constexpr dx4 magic = dx4() + (3ULL << 51);
    inline i64x4 lround(dx4 x) {
        return i64x4(x + magic) - i64x4(magic);
    }
    inline dx4 to_double(i64x4 x) {
        return dx4(x + i64x4(magic)) - magic;
    }

    inline dx4 round(dx4 a) {
        return dx4{
            std::nearbyint(a[0]),
            std::nearbyint(a[1]),
            std::nearbyint(a[2]),
            std::nearbyint(a[3])
        };
    }

    inline u64x4 low32(u64x4 x) {
        return x & uint32_t(-1);
    }
    inline auto swap_bytes(auto x) {
        return decltype(x)(__builtin_shufflevector(u32x8(x), u32x8(x), 1, 0, 3, 2, 5, 4, 7, 6));
    }
    inline u64x4 montgomery_reduce(u64x4 x, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        auto x_ninv = u64x4(_mm256_mul_epu32(__m256i(x), __m256i() + imod));
        x += u64x4(_mm256_mul_epu32(__m256i(x_ninv), __m256i() + mod));
#else
        auto x_ninv = u64x4(u32x8(low32(x)) * imod);
        x += x_ninv * uint64_t(mod);
#endif
        return swap_bytes(x);
    }

    inline u64x4 montgomery_mul(u64x4 x, u64x4 y, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        return montgomery_reduce(u64x4(_mm256_mul_epu32(__m256i(x), __m256i(y))), mod, imod);
#else
        return montgomery_reduce(x * y, mod, imod);
#endif
    }
    inline u32x8 montgomery_mul(u32x8 x, u32x8 y, uint32_t mod, uint32_t imod) {
        return u32x8(montgomery_mul(u64x4(x), u64x4(y), mod, imod)) |
               u32x8(swap_bytes(montgomery_mul(u64x4(swap_bytes(x)), u64x4(swap_bytes(y)), mod, imod)));
    }
    inline dx4 rotate_right(dx4 x) {
        static constexpr u64x4 shuffler = {3, 0, 1, 2};
        return __builtin_shuffle(x, shuffler);
    }

    template<std::size_t Align = 32>
    inline bool is_aligned(const auto* p) noexcept {
        return (reinterpret_cast<std::uintptr_t>(p) % Align) == 0;
    }

    template<class Target>
    inline Target& vector_cast(auto &&p) {
        return *reinterpret_cast<Target*>(std::assume_aligned<alignof(Target)>(&p));
    }
}
#pragma GCC pop_options

#line 6 "cp-algorithms-aux/cp-algo/util/bit.hpp"
#include <array>
#include <bit>

#if defined(__x86_64__) && !defined(CP_ALGO_DISABLE_AVX2)
#define CP_ALGO_BIT_OPS_TARGET _Pragma("GCC target(\"avx2,bmi,bmi2,lzcnt,popcnt\")")
#else
#define CP_ALGO_BIT_OPS_TARGET _Pragma("GCC target(\"bmi,bmi2,lzcnt,popcnt\")")
#endif

#define CP_ALGO_BIT_PRAGMA_PUSH \
    _Pragma("GCC push_options") \
    CP_ALGO_BIT_OPS_TARGET

CP_ALGO_BIT_PRAGMA_PUSH
namespace cp_algo {
    template<typename Uint>
    constexpr size_t bit_width = sizeof(Uint) * 8;

    // n < 64
    uint64_t mask(size_t n) {
        return (1ULL << n) - 1;
    }
    size_t order_of_bit(auto x, size_t k) {
        return k ? std::popcount(x << (bit_width<decltype(x)> - k)) : 0;
    }
    inline size_t kth_set_bit(uint64_t x, size_t k) {
        return std::countr_zero(_pdep_u64(1ULL << k, x));
    }
    template<int fl = 0>
    void with_bit_floor(size_t n, auto &&callback) {
        if constexpr (fl >= 63) {
            return;
        } else if (n >> (fl + 1)) {
            with_bit_floor<fl + 1>(n, callback);
        } else {
            callback.template operator()<1ULL << fl>();
        }
    }
    void with_bit_ceil(size_t n, auto &&callback) {
        with_bit_floor(n, [&]<size_t N>() {
            if(N == n) {
                callback.template operator()<N>();
            } else {
                callback.template operator()<N << 1>();
            }
        });
    }

    inline uint32_t read_bits(char const* p) {
        return _mm256_movemask_epi8(__m256i(vector_cast<u8x32 const>(p[0]) + (127 - '0')));
    }
    inline uint64_t read_bits64(char const* p) {
        return read_bits(p) | (uint64_t(read_bits(p + 32)) << 32);
    }

    inline void write_bits(char *p, uint32_t bits) {
        static constexpr u8x32 shuffler = {
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1,
            2, 2, 2, 2, 2, 2, 2, 2,
            3, 3, 3, 3, 3, 3, 3, 3
        };
        auto shuffled = u8x32(_mm256_shuffle_epi8(__m256i() + bits, __m256i(shuffler)));
        static constexpr u8x32 mask = {
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128
        };
        for(int z = 0; z < 32; z++) {
            p[z] = shuffled[z] & mask[z] ? '1' : '0';
        }
    }
    inline void write_bits64(char *p, uint64_t bits) {
        write_bits(p, uint32_t(bits));
        write_bits(p + 32, uint32_t(bits >> 32));
    }
}
#pragma GCC pop_options

#line 1 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"



#include <map>
#include <deque>
#include <vector>
#include <string>
#line 9 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"
#include <iostream>

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T, std::size_t Align = 32>
    class big_alloc {
        static_assert( Align >= alignof(void*), "Align must be at least pointer-size");
        static_assert(std::popcount(Align) == 1, "Align must be a power of two");
    public:
        using value_type = T;
        template <class U> struct rebind { using other = big_alloc<U, Align>; };
        constexpr bool operator==(const big_alloc&) const = default;
        constexpr bool operator!=(const big_alloc&) const = default;

        big_alloc() noexcept = default;
        template <typename U, std::size_t A>
        big_alloc(const big_alloc<U, A>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t n) {
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align = std::max<std::size_t>(alignof(T),  Align);
#if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) {
                void* raw = mmap(nullptr, padded,
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                madvise(raw, padded, MADV_HUGEPAGE);
                madvise(raw, padded, MADV_POPULATE_WRITE);
                return static_cast<T*>(raw);
            }
#endif
            return static_cast<T*>(::operator new(padded, std::align_val_t(align)));
        }

        void deallocate(T* p, std::size_t n) noexcept {
            if (!p) return;
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align  = std::max<std::size_t>(alignof(T),  Align);
    #if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) { munmap(p, padded); return; }
    #endif
            ::operator delete(p, padded, std::align_val_t(align));
        }

    private:
        static constexpr std::size_t MEGABYTE = 1 << 20;
        static constexpr std::size_t round_up(std::size_t x) noexcept {
            return (x + Align - 1) / Align * Align;
        }
    };

    template<typename T>
    using big_vector = std::vector<T, big_alloc<T>>;
    template<typename T>
    using big_basic_string = std::basic_string<T, std::char_traits<T>, big_alloc<T>>;
    template<typename T>
    using big_deque = std::deque<T, big_alloc<T>>;
    template<typename Key, typename Value, typename Compare = std::less<Key>>
    using big_map = std::map<Key, Value, Compare, big_alloc<std::pair<const Key, Value>>>;
    using big_string = big_basic_string<char>;
}

#line 5 "cp-algorithms-aux/cp-algo/structures/bit_array.hpp"
#include <cassert>
namespace cp_algo::structures {
    template<typename C>
    concept Resizable = requires(C& c, std::size_t n) { c.resize(n); };

    template<class Cont>
    struct _bit_array {
        using word_t = typename Cont::value_type;
        static constexpr size_t width = bit_width<word_t>;
        size_t words, n;
        alignas(32) Cont data;

        constexpr void resize(size_t N) {
            n = N;
            words = (n + width - 1) / width;
            if constexpr (Resizable<Cont>) {
                data.resize(words);
            } else {
                assert(std::size(data) >= words);
            }
        }

        constexpr _bit_array(): data() {
            if constexpr (!Resizable<Cont>) {
                resize(std::size(data) * width);
            } else {
                resize(0);
            }
        }
        constexpr _bit_array(size_t N): data() {
            resize(N);
        }

        constexpr word_t& word(size_t x) {
            return data[x];
        }
        constexpr word_t word(size_t x) const {
            return data[x];
        }
        constexpr void set_all(word_t val = -1) {
            for(auto& w: data) {w = val;}
        }
        constexpr void reset() {
            set_all(0);
        }
        constexpr void set(size_t x) {
            word(x / width) |= 1ULL << (x % width);
        }
        constexpr void reset(size_t x) {
            word(x / width) &= ~(1ULL << (x % width));
        }
        constexpr void flip(size_t x) {
            word(x / width) ^= 1ULL << (x % width);
        }
        constexpr bool test(size_t x) const {
            return (word(x / width) >> (x % width)) & 1;
        }
        constexpr bool operator[](size_t x) const {
            return test(x);
        }
        constexpr size_t size() const {
            return n;
        }
        
        auto operator <=> (_bit_array const& t) const = default;
        
        constexpr _bit_array& xor_hint(_bit_array const& t, size_t hint) {
            for(size_t i = hint / width; i < std::size(data); i++) {
                data[i] ^= t.data[i];
            }
            return *this;
        }
        constexpr _bit_array& operator ^= (_bit_array const& t) {
            return xor_hint(t, 0);
        }
        constexpr _bit_array operator ^ (_bit_array const& t) const {
            return _bit_array(*this) ^= t;
        }
    };

    template<size_t N>
    using bit_array = _bit_array<std::array<uint64_t, (N + 63) / 64>>;
    using dynamic_bit_array = _bit_array<big_vector<uint64_t>>;

}

#line 9 "cp-algorithms-aux/cp-algo/structures/bit_array_util.hpp"
#include <span>
CP_ALGO_BIT_PRAGMA_PUSH
namespace cp_algo::structures {
    template<typename BitArray>
    constexpr void from_string(BitArray& arr, std::span<char> bits) {
        arr.resize(std::size(bits));
        for(size_t i = 0; i < arr.words; i++) {
            arr.word(i) = read_bits64(bits.data() + i * BitArray::width);
        }
    }

    template<typename BitArray>
    constexpr big_string to_string(BitArray const& arr) {
        big_string res(arr.words * BitArray::width, '0');
        for(size_t i = 0; i < arr.words; i++) {
            write_bits64(res.data() + i * BitArray::width, arr.word(i));
        }
        res.resize(arr.n);
        return res;
    }

    template<typename BitArray>
    constexpr size_t count(BitArray const& arr, size_t n) {
        size_t res = 0;
        for(size_t i = 0; i < n / BitArray::width; i++) {
            res += std::popcount(arr.word(i));
        }
        if (n % BitArray::width) {
            res += std::popcount(arr.word(n / BitArray::width) & mask(n % BitArray::width));
        }
        return res;
    }
    
    template<typename BitArray>
    constexpr size_t count(BitArray const& arr) {
        return count(arr, arr.n);
    }
    
    template<typename BitArray>
    constexpr size_t ctz(BitArray const& arr) {
        size_t res = 0;
        size_t i = 0;
        while(i < arr.words && arr.word(i) == 0) {
            res += BitArray::width;
            i++;
        }
        if(i < arr.words) {
            res += std::countr_zero(arr.word(i));
        }
        return std::min(res, arr.n);
    }
}
#pragma GCC pop_options

#line 5 "cp-algorithms-aux/verify/structures/bitpack/system_mod_2.test.cpp"
#include <bits/stdc++.h>

using namespace std;
using namespace cp_algo::structures;

const int maxn = (1 << 12) + 1;
bit_array<maxn> a[maxn];

void solve() {
    size_t n, m;
    cin >> n >> m;
    vector<string> As(n);
    for(size_t i = 0; i < n; i++) {
        cin >> As[i];
    }
    string bs;
    cin >> bs;
    for(size_t i = 0; i < n; i++) {
        As[i] += bs[i];
        from_string(a[i], As[i]);
    }
    vector<size_t> lead(n);
    auto vars = views::iota((size_t)0, m + 1);
    set<size_t> free(begin(vars), end(vars));
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < i; j++) {
            if(a[i][lead[j]]) {
                a[i].xor_hint(a[j], lead[j]);
            }
        }
        lead[i] = ctz(a[i]);
        if(lead[i] == m) {
            cout << -1 << "\n";
            return;
        }
        if(lead[i] > m) {
            continue;
        }
        free.erase(lead[i]);
        for(size_t j = 0; j < i; j++) {
            if(a[j][lead[i]]) {
                a[j].xor_hint(a[i], lead[i]);
            }
        }
    }
    bit_array<maxn> x[maxn];
    for(auto [j, pj]: views::enumerate(free)) {
        x[j].set(pj);
        for(size_t i = 0; i < n; i++) {
            if(lead[i] < m && a[i][pj]) {
                x[j].set(lead[i]);
            }
        }
    }
    size_t rk = size(free) - 1;
    swap(x[0], x[rk]);
    cout << rk << "\n";
    for(size_t i = 0; i <= rk; i++) {
        cout << to_string(x[i]).substr(0, m) << "\n";
    }
}

signed main() {
    //freopen("input.txt", "r", stdin);
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    while(t--) {
        solve();
    }
}
