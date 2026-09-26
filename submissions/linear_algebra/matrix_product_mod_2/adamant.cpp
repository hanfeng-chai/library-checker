#line 1 "cp-algorithms-aux/verify/structures/bitpack/prod_mod_2.test.cpp"
// @brief Matrix Product (Mod 2)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product_mod_2"
#pragma GCC optimize("Ofast,unroll-loops")
#define CP_ALGO_CHECKPOINT
#line 1 "cp-algorithms-aux/cp-algo/structures/bitpack.hpp"


#line 1 "cp-algorithms-aux/cp-algo/structures/bit_array.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/bit.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/simd.hpp"


#include <experimental/simd>
#include <cstdint>
#include <cstddef>
#include <memory>
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

    [[gnu::always_inline]] inline dx4 abs(dx4 a) {
    return a < 0 ? -a : a;
    }

    // https://stackoverflow.com/a/77376595
    // works for ints in (-2^51, 2^51)
    static constexpr dx4 magic = dx4() + (3ULL << 51);
    [[gnu::always_inline]] inline i64x4 lround(dx4 x) {
        return i64x4(x + magic) - i64x4(magic);
    }
    [[gnu::always_inline]] inline dx4 to_double(i64x4 x) {
        return dx4(x + i64x4(magic)) - magic;
    }

    [[gnu::always_inline]] inline dx4 round(dx4 a) {
        return dx4{
            std::nearbyint(a[0]),
            std::nearbyint(a[1]),
            std::nearbyint(a[2]),
            std::nearbyint(a[3])
        };
    }

    [[gnu::always_inline]] inline u64x4 low32(u64x4 x) {
        return x & uint32_t(-1);
    }
    [[gnu::always_inline]] inline auto swap_bytes(auto x) {
        return decltype(x)(__builtin_shufflevector(u32x8(x), u32x8(x), 1, 0, 3, 2, 5, 4, 7, 6));
    }
    [[gnu::target("avx2"), gnu::always_inline]] inline u64x4 montgomery_reduce(u64x4 x, uint32_t mod, uint32_t imod) {
        auto x_ninv = u64x4(_mm256_mul_epu32(__m256i(x), __m256i() + imod));
        x += u64x4(_mm256_mul_epu32(__m256i(x_ninv), __m256i() + mod));
        return swap_bytes(x);
    }

    [[gnu::target("avx2"), gnu::always_inline]] inline u64x4 montgomery_mul(u64x4 x, u64x4 y, uint32_t mod, uint32_t imod) {
        return montgomery_reduce(u64x4(_mm256_mul_epu32(__m256i(x), __m256i(y))), mod, imod);
    }
    [[gnu::always_inline]] inline u32x8 montgomery_mul(u32x8 x, u32x8 y, uint32_t mod, uint32_t imod) {
        return u32x8(montgomery_mul(u64x4(x), u64x4(y), mod, imod)) |
               u32x8(swap_bytes(montgomery_mul(u64x4(swap_bytes(x)), u64x4(swap_bytes(y)), mod, imod)));
    }
    [[gnu::always_inline]] inline dx4 rotate_right(dx4 x) {
        static constexpr u64x4 shuffler = {3, 0, 1, 2};
        return __builtin_shuffle(x, shuffler);
    }

    template<std::size_t Align = 32>
    [[gnu::always_inline]] inline bool is_aligned(const auto* p) noexcept {
        return (reinterpret_cast<std::uintptr_t>(p) % Align) == 0;
    }

    template<class Target>
    [[gnu::always_inline]] inline Target& vector_cast(auto &&p) {
        return *reinterpret_cast<Target*>(std::assume_aligned<alignof(Target)>(&p));
    }
}

#line 5 "cp-algorithms-aux/cp-algo/util/bit.hpp"
#include <array>
#include <bit>
namespace cp_algo {
    template<typename Uint>
    constexpr size_t bit_width = sizeof(Uint) * 8;

    size_t order_of_bit(auto x, size_t k) {
        return k ? std::popcount(x << (bit_width<decltype(x)> - k)) : 0;
    }
    [[gnu::target("bmi2")]]
    size_t kth_set_bit(uint64_t x, size_t k) {
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

    [[gnu::target("avx2"), gnu::always_inline]] inline uint32_t read_bits(char const* p) {
        return _mm256_movemask_epi8(__m256i(vector_cast<u8x32 const>(p[0]) + (127 - '0')));
    }
    [[gnu::always_inline]] inline uint64_t read_bits64(char const* p) {
        return read_bits(p) | (uint64_t(read_bits(p + 32)) << 32);
    }

    [[gnu::target("avx2"), gnu::always_inline]] inline void write_bits(char *p, uint32_t bits) {
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
    [[gnu::target("avx2"), gnu::always_inline]] inline void write_bits64(char *p, uint64_t bits) {
        write_bits(p, uint32_t(bits));
        write_bits(p + 32, uint32_t(bits >> 32));
    }
}

#line 1 "cp-algorithms-aux/cp-algo/util/bump_alloc.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"



#line 5 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"
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
}

#line 5 "cp-algorithms-aux/cp-algo/util/bump_alloc.hpp"
namespace cp_algo {
    template<class T, size_t max_len>
    struct bump_alloc {
        static char* buf;
        static size_t buf_ind;
        using value_type = T;
        template <class U> struct rebind { using other = bump_alloc<U, max_len>; };
        constexpr bool operator==(const bump_alloc&) const = default;
        constexpr bool operator!=(const bump_alloc&) const = default;
        bump_alloc() = default;
        template<class U> bump_alloc(const U&) {}
        T* allocate(size_t n) {
            buf_ind -= n * sizeof(T);
            buf_ind &= 0 - alignof(T);
            return (T*)(buf + buf_ind);
        }
        void deallocate(T*, size_t) {}
    };
    template<class T, size_t max_len>
    char* bump_alloc<T, max_len>::buf = big_alloc<char>().allocate(max_len * sizeof(T));
    template<class T, size_t max_len>
    size_t bump_alloc<T, max_len>::buf_ind = max_len * sizeof(T);
}

#line 5 "cp-algorithms-aux/cp-algo/structures/bit_array.hpp"
namespace cp_algo::structures {
    template<class Cont>
    struct _bit_array {
        static constexpr size_t width = bit_width<uint64_t>;
        size_t words, n;
        alignas(32) Cont data;

        _bit_array(): words(0), n(0), data() {}
        _bit_array(size_t N): words((N + width - 1) / width), n(N), data() {}

        uint64_t& word(size_t x) {
            return data[x];
        }
        uint64_t word(size_t x) const {
            return data[x];
        }
        void set(size_t x) {
            word(x / width) |= 1ULL << (x % width);
        }
        void reset(size_t x) {
            word(x / width) &= ~(1ULL << (x % width));
        }
        void reset() {
            for(auto& w: data) {
                w = 0;
            }
        }
        void flip(size_t x) {
            word(x / width) ^= 1ULL << (x % width);
        }
        bool test(size_t x) const {
            return (word(x / width) >> (x % width)) & 1;
        }
        bool operator[](size_t x) const {
            return test(x);
        }
        size_t size() const {
            return n;
        }
    };

    template<int N>
    struct bit_array: _bit_array<std::array<uint64_t, (N + 63) / 64>> {
        using Base = _bit_array<std::array<uint64_t, (N + 63) / 64>>;
        using Base::Base, Base::words, Base::data;
        bit_array(): Base(N) {}
    };
    struct dynamic_bit_array: _bit_array<std::vector<uint64_t>> {
        using Base = _bit_array<std::vector<uint64_t>>;
        using Base::Base, Base::words;
        dynamic_bit_array(size_t N): Base(N) {
            data.resize(words);
        }
    };

}

#line 7 "cp-algorithms-aux/cp-algo/structures/bitpack.hpp"
#include <string>
#line 9 "cp-algorithms-aux/cp-algo/structures/bitpack.hpp"
namespace cp_algo::structures {
    template<typename BitArray>
    struct _bitpack: BitArray {
        using Base = BitArray;
        using Base::Base, Base::width, Base::words, Base::data, Base::n, Base::word;
        auto operator <=> (_bitpack const& t) const = default;

        _bitpack(std::string &bits): _bitpack(std::size(bits)) {
            bits += std::string(-std::size(bits) % width, '0');
            for(size_t i = 0; i < words; i++) {
                word(i) = read_bits64(bits.data() + i * width);
            }
        }

        _bitpack& xor_hint(_bitpack const& t, size_t hint) {
            for(size_t i = hint / width; i < std::size(data); i++) {
                data[i] ^= t.data[i];
            }
            return *this;
        }
        _bitpack& operator ^= (_bitpack const& t) {
            return xor_hint(t, 0);
        }
        _bitpack operator ^ (_bitpack const& t) const {
            return _bitpack(*this) ^= t;
        }

        std::string to_string() const {
            std::string res(words * width, '0');
            for(size_t i = 0; i < words; i++) {
                write_bits64(res.data() + i * width, word(i));
            }
            res.resize(n);
            return res;
        }

        size_t ctz() const {
            size_t res = 0;
            size_t i = 0;
            while(i < words && word(i) == 0) {
                res += width;
                i++;
            }
            if(i < words) {
                res += std::countr_zero(word(i));
            }
            return std::min(res, n);
        }
    };

    template<int N>
    using bitpack = _bitpack<bit_array<N>>;
    using dynamic_bitpack = _bitpack<dynamic_bit_array>;
}

#line 1 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"


#line 4 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
#include <chrono>
#line 6 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
#include <map>
namespace cp_algo {
    std::map<std::string, double> checkpoints;
    template<bool final = false>
    void checkpoint([[maybe_unused]] std::string const& msg = "") {
#ifdef CP_ALGO_CHECKPOINT
        static double last = 0;
        double now = (double)clock() / CLOCKS_PER_SEC;
        double delta = now - last;
        last = now;
        if(msg.size() && !final) {
            checkpoints[msg] += delta;
        }
        if(final) {
            for(auto const& [key, value] : checkpoints) {
                std::cerr << key << ": " << value * 1000 << " ms\n";
            }
            std::cerr << "Total: " << now * 1000 << " ms\n";
        }
#endif
    }
}

#line 7 "cp-algorithms-aux/verify/structures/bitpack/prod_mod_2.test.cpp"
#include <bits/stdc++.h>

using namespace std;

const int maxn = 1 << 12;
const size_t K = 8;

using bitpack = cp_algo::structures::bitpack<maxn>;

bitpack a[maxn], b[maxn], c[maxn];
bitpack precalc[1 << K];

void process_precalc(int i) {
    for(size_t j = 0; j < K; j++) {
        int step = 1 << j;
        for(int k = 0; k < step; k++) {
            precalc[k + step] = precalc[k] ^ b[i + j];
        }
    }
}

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    cp_algo::checkpoint("init");
    string row;
    for(int i = 0; i < n; i++) {
        cin >> row;
        a[i] = row;
    }
    for(auto &it: b) {
        it = bitpack(k);
    }
    for(int i = 0; i < m; i++) {
        cin >> row;
        b[i] = row;
    }
    for(auto &it: c) {
        it = bitpack(k);
    }
    for(auto &it: precalc) {
        it = bitpack(k);
    }
    cp_algo::checkpoint("read");
    const int width = bitpack::width;
    for(int j = 0; j < m; j += width) {
        for(int offset = 0; offset < width; offset += K) {
            process_precalc(j + offset);
            for(int i = 0; i < n; i++) {
                c[i] ^= precalc[uint8_t(a[i].word(j / width) >> offset)];
            }
        }
    }
    cp_algo::checkpoint("mul");
    for(int i = 0; i < n; i++) {
        cout << c[i].to_string() << "\n";
    }
    cp_algo::checkpoint("write");
    cp_algo::checkpoint<1>();
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
