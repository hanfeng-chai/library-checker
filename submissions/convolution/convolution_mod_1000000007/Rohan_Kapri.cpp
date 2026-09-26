#line 1 ".verify-helper/cache/fft-shallow-roots-20260922-nnenmcu8/fused/prefix-107.cpp"
#include <bits/stdc++.h>
#include <immintrin.h>
#pragma GCC optimize("O3,no-unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt,fma")
#line 1 "cp-algo/math/fft.hpp"


#line 1 "cp-algo/math/ring.hpp"


#line 1 "cp-algo/number_theory/discrete_sqrt.hpp"


#line 1 "cp-algo/number_theory/modint.hpp"


#line 1 "cp-algo/math/common.hpp"


#line 5 "cp-algo/math/common.hpp"
#include <cassert>
#include <bit>
#line 9 "cp-algo/math/common.hpp"
namespace cp_algo::math {
#ifdef CP_ALGO_MAXN
    const int maxn = CP_ALGO_MAXN;
#else
    const int maxn = 1 << 19;
#endif
    const int magic = 64; // threshold for sizes to run the naive algo

    // Nonnegative 64-bit exponents, with an associative operation and its identity.
    // Windows >1 precompute odd powers only when that saves operations.
    template<int window = 1>
    auto bpow(auto const& x, auto n, auto const& one, auto op) {
        static_assert(window >= 1 && window <= 6);
        if constexpr(window > 1) {
            if(n == 0) {return one;}
            int bits = std::bit_width(uint64_t(n));
            auto low_bit = [&](int high) {
                int low = std::max(0, high - window + 1);
                while(!((n >> low) & 1)) {low++;}
                return low;
            };
            int first = low_bit(bits - 1);
            int cost = (1 << (window - 1)) + first;
            for(int j = first - 1; j >= 0;) {
                if(!((n >> j) & 1)) {j--;}
                else {cost++; j = low_bit(j) - 1;}
            }
            // Do not pay for the table when binary powering uses fewer operations.
            if(cost >= bits + std::popcount(uint64_t(n)) - 2) {return bpow<1>(x, n, one, op);}
            using T = std::decay_t<decltype(x)>;
            std::vector<T> odd;
            odd.reserve(1 << (window - 1));
            odd.push_back(x);
            auto square = op(x, x);
            while(odd.size() < size_t(1 << (window - 1))) {odd.push_back(op(odd.back(), square));}
            auto ans = odd[(n >> first) / 2];
            for(int j = first - 1; j >= 0;) {
                if(!((n >> j) & 1)) {ans = op(ans, ans); j--;}
                else {
                    int low = low_bit(j), length = j - low + 1;
                    auto digit = (n >> low) & ((1u << length) - 1);
                    for(int i = 0; i < length; i++) {ans = op(ans, ans);}
                    ans = op(ans, odd[digit / 2]);
                    j = low - 1;
                }
            }
            return ans;
        } else {
            if (n == 0) {
                return one;
            }
            auto ans = x;
            for(int j = std::bit_width<uint64_t>(n) - 2; ~j; j--) {
                ans = op(ans, ans);
                if((n >> j) & 1) {
                    ans = op(ans, x);
                }
            }
            return ans;
        }
    }
    template<int window = 1>
    auto bpow(auto x, auto n, auto ans) {
        return bpow<window>(x, n, ans, std::multiplies{});
    }
    template<typename T>
    T bpow(T const& x, auto n) {
        return bpow(x, n, T(1));
    }
    inline constexpr auto inv2(auto x) {
        assert(x % 2);
        std::make_unsigned_t<decltype(x)> y = 1;
        while(y * x != 1) {
            y *= 2 - x * y;
        }
        return y;
    }
}

#line 5 "cp-algo/number_theory/modint.hpp"
#include <cassert>
namespace cp_algo::math {

    template<typename modint, typename _Int>
    struct modint_base {
        using Int = _Int;
        using UInt = std::make_unsigned_t<Int>;
        static constexpr size_t bits = sizeof(Int) * 8;
        using Int2 = std::conditional_t<bits <= 32, int64_t, __int128_t>;
        using UInt2 = std::conditional_t<bits <= 32, uint64_t, __uint128_t>;
        constexpr static Int mod() {
            return modint::mod();
        }
        constexpr static UInt remod() {
            return modint::remod();
        }
        constexpr static UInt2 modmod() {
            return UInt2(mod()) * mod();
        }
        constexpr modint_base() = default;
        constexpr modint_base(Int2 rr) {
            to_modint().setr(UInt((rr + modmod()) % mod()));
        }
        constexpr modint inv() const {
            return bpow(to_modint(), mod() - 2);
        }
        modint operator - () const {
            modint neg;
            neg.r = std::min(-r, remod() - r);
            return neg;
        }
        modint& operator /= (const modint &t) {
            return to_modint() *= t.inv();
        }
        modint& operator *= (const modint &t) {
            r = UInt(UInt2(r) * t.r % mod());
            return to_modint();
        }
        modint& operator += (const modint &t) {
            r += t.r; r = std::min(r, r - remod());
            return to_modint();
        }
        modint& operator -= (const modint &t) {
            r -= t.r; r = std::min(r, r + remod());
            return to_modint();
        }
        modint operator + (const modint &t) const {return modint(to_modint()) += t;}
        modint operator - (const modint &t) const {return modint(to_modint()) -= t;}
        modint operator * (const modint &t) const {return modint(to_modint()) *= t;}
        modint operator / (const modint &t) const {return modint(to_modint()) /= t;}
        // Why <=> doesn't work?..
        auto operator == (const modint &t) const {return to_modint().getr() == t.getr();}
        auto operator != (const modint &t) const {return to_modint().getr() != t.getr();}
        auto operator <= (const modint &t) const {return to_modint().getr() <= t.getr();}
        auto operator >= (const modint &t) const {return to_modint().getr() >= t.getr();}
        auto operator < (const modint &t) const {return to_modint().getr() < t.getr();}
        auto operator > (const modint &t) const {return to_modint().getr() > t.getr();}
        Int rem() const {
            UInt R = to_modint().getr();
            return R - (R > (UInt)mod() / 2) * mod();
        }
        constexpr void setr(UInt rr) {
            r = rr;
        }
        constexpr UInt getr() const {
            return r;
        }

        // Only use these if you really know what you're doing!
        static uint64_t modmod8() {return uint64_t(8 * modmod());}
        void add_unsafe(UInt t) {r += t;}
        void pseudonormalize() {r = std::min(r, r - modmod8());}
        modint const& normalize() {
            if(r >= (UInt)mod()) {
                r %= mod();
            }
            return to_modint();
        }
        void setr_direct(UInt rr) {r = rr;}
        UInt getr_direct() const {return r;}
    protected:
        UInt r;
    private:
        constexpr modint& to_modint() {return static_cast<modint&>(*this);}
        constexpr modint const& to_modint() const {return static_cast<modint const&>(*this);}
    };
    template<typename modint>
    concept modint_type = std::is_base_of_v<modint_base<modint, typename modint::Int>, modint>;
    template<modint_type modint>
    decltype(std::cin)& operator >> (decltype(std::cin) &in, modint &x) {
        typename modint::UInt r;
        auto &res = in >> r;
        x.setr(r);
        return res;
    }
    template<modint_type modint>
    decltype(std::cout)& operator << (decltype(std::cout) &out, modint const& x) {
        return out << x.getr();
    }

    template<auto m>
    struct modint: modint_base<modint<m>, decltype(m)> {
        using Base = modint_base<modint<m>, decltype(m)>;
        using Base::Base;
        static constexpr Base::Int mod() {return m;}
        static constexpr Base::UInt remod() {return m;}
        auto getr() const {return Base::r;}
    };

    // Odd moduli up to a quarter of the unsigned word keep Montgomery residues lazily in
    // [0, 2 mod): remod() = 2 mod, and both 4 mod and ab + q * mod fit. Any other modulus keeps
    // fully reduced residues, remod() = mod, which is what the sums of modint_base need then:
    // an even one without the Montgomery form, a wide odd one with a reduction that subtracts
    // high words instead of adding double words that would overflow.
    template<typename Int = int>
    struct dynamic_modint: modint_base<dynamic_modint<Int>, Int> {
        using Base = modint_base<dynamic_modint<Int>, Int>;
        using Base::Base;

        // Out of line, so that the hot path stays as small as it was.
        [[gnu::noinline, gnu::cold]] static Base::UInt m_reduce_reduced(Base::UInt2 ab) {
            if(mod() % 2 == 0) {return typename Base::UInt(ab % mod());}
            // q * mod has the low word of ab, so the difference of the high words is exact.
            typename Base::UInt q = -(typename Base::UInt(ab) * inverse);
            auto high = typename Base::UInt(ab >> Base::bits);
            auto low = typename Base::UInt(typename Base::UInt2(q) * typename Base::UInt(mod()) >> Base::bits);
            return high >= low ? high - low : high - low + mod();
        }
        static Base::UInt m_reduce(Base::UInt2 ab) {
            if(imod() == 0) [[unlikely]] {return m_reduce_reduced(ab);}
            typename Base::UInt2 m = typename Base::UInt(ab) * imod();
            return typename Base::UInt((ab + m * mod()) >> Base::bits);
        }
        static Base::UInt m_transform(Base::UInt a) {
            if(mod() % 2 == 0) [[unlikely]] {
                return a;
            } else {
                return m_reduce(a * pw128());
            }
        }
        dynamic_modint& operator *= (const dynamic_modint &t) {
            Base::r = m_reduce(typename Base::UInt2(Base::r) * t.r);
            return *this;
        }
        void setr(Base::UInt rr) {
            Base::r = m_transform(rr);
        }
        Base::UInt getr() const {
            typename Base::UInt res = m_reduce(Base::r);
            return std::min(res, res - mod());
        }
        static Int mod() {return m;}
        static Base::UInt remod() {return rm;}
        static Base::UInt imod() {return im;}
        static Base::UInt2 pw128() {return r2;}
        static void switch_mod(Int nm) {
            m = nm;
            bool lazy = m % 2 && typename Base::UInt(m) <= typename Base::UInt(-1) / 4;
            rm = typename Base::UInt(m) * (lazy ? 2 : 1);
            inverse = m % 2 ? inv2(-m) : 0;
            im = lazy ? inverse : 0;
            r2 = static_cast<Base::UInt>(static_cast<Base::UInt2>(-1) % m + 1);
        }

        // Wrapper for temp switching
        auto static with_mod(Int tmp, auto callback) {
            struct scoped {
                Int prev = mod();
                ~scoped() {switch_mod(prev);}
            } _;
            switch_mod(tmp);
            return callback();
        }
    private:
        static thread_local Int m;
        // im: -1 / mod modulo 2^bits for lazy residues and 0 for reduced ones; inverse: the same
        // for every odd mod; rm: the value of remod().
        static thread_local Base::UInt im, r2, inverse, rm;
    };
    template<typename Int>
    Int thread_local dynamic_modint<Int>::m = 1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::im = -1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::r2 = 0;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::inverse = -1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::rm = 2;
}

#line 1 "cp-algo/random/rng.hpp"


#line 5 "cp-algo/random/rng.hpp"
namespace cp_algo::random {
    std::mt19937_64 gen(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    uint64_t rng() {
        return gen();
    }
}

#line 1 "cp-algo/math/affine.hpp"


#include <optional>
#line 5 "cp-algo/math/affine.hpp"
#include <cassert>
#line 7 "cp-algo/math/affine.hpp"
namespace cp_algo::math {
    // a * x + b
    template<typename base>
    struct lin {
        base a = 1, b = 0;
        std::optional<base> c;
        lin() {}
        lin(base b): a(0), b(b) {}
        lin(base a, base b): a(a), b(b) {}
        lin(base a, base b, base _c): a(a), b(b), c(_c) {}

        // polynomial product modulo x^2 - c
        lin operator * (const lin& t) {
            assert(c && t.c && *c == *t.c);
            return {a * t.b + b * t.a, b * t.b + a * t.a * (*c), *c};
        }

        // a * (t.a * x + t.b) + b
        lin apply(lin const& t) const {
            return {a * t.a, a * t.b + b};
        }

        void prepend(lin const& t) {
            *this = t.apply(*this);
        }

        base eval(base x) const {
            return a * x + b;
        }
    };

    // (ax+b) / (cx+d)
    template<typename base>
    struct linfrac {
        base a, b, c, d;
        linfrac(): a(1), b(0), c(0), d(1) {} // x, identity for composition
        linfrac(base a): a(a), b(1), c(1), d(0) {} // a + 1/x, for continued fractions
        linfrac(base a, base b, base c, base d): a(a), b(b), c(c), d(d) {}

        // composition of two linfracs
        linfrac operator * (linfrac t) const {
            return t.prepend(linfrac(*this));
        }

        linfrac operator-() const {
            return {-a, -b, -c, -d};
        }

        linfrac adj() const {
            return {d, -b, -c, a};
        }
        
        linfrac& prepend(linfrac const& t) {
            t.apply(a, c);
            t.apply(b, d);
            return *this;
        }

        // apply linfrac to A/B
        void apply(base &A, base &B) const {
            std::tie(A, B) = std::pair{a * A + b * B, c * A + d * B};
        }
    };
}

#line 6 "cp-algo/number_theory/discrete_sqrt.hpp"
namespace cp_algo::math {
    // https://en.wikipedia.org/wiki/Berlekamp-Rabin_algorithm
    template<modint_type base>
    std::optional<base> sqrt(base b) {
        if(b == base(0)) {
            return base(0);
        } else if(bpow(b, (b.mod() - 1) / 2) != base(1)) {
            return std::nullopt;
        } else {
            while(true) {
                base z = random::rng();
                if(z * z == b) {
                    return z;
                }
                lin<base> x(1, z, b); // x + z (mod x^2 - b)
                x = bpow(x, (b.mod() - 1) / 2, lin<base>(0, 1, b));
                if(x.a != base(0)) {
                    return x.a.inv();
                }
            }
        }
    }
}

#line 1 "cp-algo/number_theory/primality.hpp"


#line 6 "cp-algo/number_theory/primality.hpp"
namespace cp_algo::math {
    // https://en.wikipedia.org/wiki/Miller–Rabin_primality_test
    template<typename _Int>
    bool is_prime(_Int m) {
        using Int = std::make_signed_t<_Int>;
        using UInt = std::make_unsigned_t<Int>;
        if(m == 1 || m % 2 == 0) {
            return m == 2;
        }
        // m - 1 = 2^s * d
        int s = std::countr_zero(UInt(m - 1));
        auto d = (m - 1) >> s;
        using base = dynamic_modint<Int>;
        auto test = [&](base x) {
            x = bpow(x, d);
            if(std::abs(x.rem()) <= 1) {
                return true;
            }
            for(int i = 1; i < s && x != -1; i++) {
                x *= x;
            }
            return x == -1;
        };
        return base::with_mod(m, [&]() {
#ifdef CP_ALGO_NUMBER_THEORY_PRIMALITY_BASES_HPP
            uint16_t base2 = 7, base3 = 61;
            if (m != uint32_t(m)) {
                base2 = base_table1[uint32_t(m * 0xAD625B89) >> 18];
                base3 = base_table2[base2 >> 13];
            }
            return test(2) && test(base2) && test(base3);
#else
            return std::ranges::all_of(std::array{2, 325, 9375, 28178, 450775, 9780504, 1795265022}, test);
#endif
        });
    }
}

#line 1 "cp-algo/util/checkpoint.hpp"


#line 1 "cp-algo/util/big_alloc.hpp"



#line 14 "cp-algo/util/big_alloc.hpp"

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T, size_t Align = 32>
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

    template<typename T> using big_vector = std::vector<T, big_alloc<T>>;
    template<typename T> using big_basic_string = std::basic_string<T, std::char_traits<T>, big_alloc<T>>;
    template<typename T> using big_deque = std::deque<T, big_alloc<T>>;
    template<typename T> using big_stack = std::stack<T, big_deque<T>>;
    template<typename T> using big_queue = std::queue<T, big_deque<T>>;
    template<typename T> using big_priority_queue = std::priority_queue<T, big_vector<T>>;
    template<typename T> using big_forward_list = std::forward_list<T, big_alloc<T>>;
    using big_string = big_basic_string<char>;

    template<typename Key, typename Value, typename Compare = std::less<Key>>
    using big_map = std::map<Key, Value, Compare, big_alloc<std::pair<const Key, Value>>>;
    template<typename T, typename Compare = std::less<T>>
    using big_multiset = std::multiset<T, Compare, big_alloc<T>>;
    template<typename T, typename Compare = std::less<T>>
    using big_set = std::set<T, Compare, big_alloc<T>>;
}


#line 8 "cp-algo/util/checkpoint.hpp"
namespace cp_algo {
#ifdef CP_ALGO_CHECKPOINT
    big_map<big_string, double> checkpoints;
    double last;
#endif
    template<bool final = false>
    void checkpoint([[maybe_unused]] auto const& _msg) {
#ifdef CP_ALGO_CHECKPOINT
        big_string msg = _msg;
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
    template<bool final = false>
    void checkpoint() {
        checkpoint<final>("");
    }
}

#line 1 "cp-algo/math/cvector.hpp"


#line 1 "cp-algo/util/simd.hpp"


#include <experimental/simd>
#line 7 "cp-algo/util/simd.hpp"

#if defined(__x86_64__) && !defined(CP_ALGO_DISABLE_AVX2)
#define CP_ALGO_SIMD_AVX2_TARGET _Pragma("GCC target(\"avx2,fma\")")
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
    using u64x8 = simd<uint64_t, 8>;
    using u32x16 = simd<uint32_t, 16>;
    using i64x4 = simd<int64_t, 4>;
    using u64x4 = simd<uint64_t, 4>;
    using u32x8 = simd<uint32_t, 8>;
    using u16x16 = simd<uint16_t, 16>;
    using i32x4 = simd<int32_t, 4>;
    using u32x4 = simd<uint32_t, 4>;
    using u16x8 = simd<uint16_t, 8>;
    using u16x4 = simd<uint16_t, 4>;
    using i16x4 = simd<int16_t, 4>;
    using u8x32 = simd<uint8_t, 32>;
    using u8x16 = simd<uint8_t, 16>;
    using u8x8 = simd<uint8_t, 8>;
    using u8x4 = simd<uint8_t, 4>;
    using dx4 = simd<double, 4>;

    inline dx4 abs(dx4 a) {
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

    // x - mod where x >= mod, for x in [0, 2 mod). Unsigned, so it holds up to mod < 2^31;
    // zero upper halves of 64-bit lanes stay zero.
    inline u32x8 reduce_once(u32x8 x, uint32_t mod) {
        auto y = x - mod;
        return x < y ? x : y;
    }
    inline u64x4 reduce_once(u64x4 x, uint32_t mod) {
        return u64x4(reduce_once(u32x8(x), mod));
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

#line 1 "cp-algo/util/complex.hpp"


#line 5 "cp-algo/util/complex.hpp"
#include <type_traits>
#line 7 "cp-algo/util/complex.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo {
    // Custom implementation, since std::complex is UB on non-floating types
    template<typename T>
    struct complex {
        using value_type = T;
        T x, y;
        inline constexpr complex(): x(), y() {}
        inline constexpr complex(T const& x): x(x), y() {}
        inline constexpr complex(T const& x, T const& y): x(x), y(y) {}
        inline complex& operator *= (T const& t) {x *= t; y *= t; return *this;}
        inline complex& operator /= (T const& t) {x /= t; y /= t; return *this;}
        inline complex operator * (T const& t) const {return complex(*this) *= t;}
        inline complex operator / (T const& t) const {return complex(*this) /= t;}
        inline complex& operator += (complex const& t) {x += t.x; y += t.y; return *this;}
        inline complex& operator -= (complex const& t) {x -= t.x; y -= t.y; return *this;}
        inline complex operator * (complex const& t) const {return {x * t.x - y * t.y, x * t.y + y * t.x};}
        inline complex operator / (complex const& t) const {return *this * t.conj() / t.norm();}
        inline complex operator + (complex const& t) const {return complex(*this) += t;}
        inline complex operator - (complex const& t) const {return complex(*this) -= t;}
        inline complex& operator *= (complex const& t) {return *this = *this * t;}
        inline complex& operator /= (complex const& t) {return *this = *this / t;}
        inline complex operator - () const {return {-x, -y};}
        inline complex conj() const {return {x, -y};}
        inline T norm() const {return x * x + y * y;}
        inline T abs() const {return std::sqrt(norm());}
        inline T const real() const {return x;}
        inline T const imag() const {return y;}
        inline T& real() {return x;}
        inline T& imag() {return y;}
        inline static constexpr complex polar(T r, T theta) {return {T(r * cos(theta)), T(r * sin(theta))};}
        inline auto operator <=> (complex const& t) const = default;
    };
    template<typename T> inline complex<T> conj(complex<T> const& x) {return x.conj();}
    template<typename T> inline T norm(complex<T> const& x) {return x.norm();}
    template<typename T> inline T abs(complex<T> const& x) {return x.abs();}
    template<typename T> inline T& real(complex<T> &x) {return x.real();}
    template<typename T> inline T& imag(complex<T> &x) {return x.imag();}
    template<typename T> inline T const real(complex<T> const& x) {return x.real();}
    template<typename T> inline T const imag(complex<T> const& x) {return x.imag();}
    template<typename T>
    inline constexpr complex<T> polar(T r, T theta) {
        return complex<T>::polar(r, theta);
    }
    template<typename T>
    inline std::ostream& operator << (std::ostream &out, complex<T> const& x) {
        return out << x.real() << ' ' << x.imag();
    }
}
#pragma GCC pop_options

#line 8 "cp-algo/math/cvector.hpp"
#include <numbers>
#line 10 "cp-algo/math/cvector.hpp"
#include <ranges>
#line 13 "cp-algo/math/cvector.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace stdx = std::experimental;
namespace cp_algo::math::fft {
    static constexpr size_t flen = 4;
    using ftype = double;
    using vftype = dx4;
    using point = complex<ftype>;
    using vpoint = complex<vftype>;
    static constexpr vftype vz = {};
    vpoint vi(vpoint const& r) {
        return {-imag(r), real(r)};
    }

    // Spectrum storage skips zero-filling on resize; every user writes before reading.
    template<class T>
    struct spectrum_alloc: big_alloc<T> {
        using big_alloc<T>::big_alloc;
        template<class U> struct rebind { using other = spectrum_alloc<U>; };
        template<class U> requires (std::is_same_v<U, vpoint>)
        void construct(U*) noexcept {}
    };
    using spectrum_vector = std::vector<vpoint, spectrum_alloc<vpoint>>;

    struct cvector {
        spectrum_vector r;
        cvector(size_t n) {
            n = std::max(flen, std::bit_ceil(n));
            r.assign(n / flen, vpoint{});
            prepare_roots(n / 16);
            checkpoint("cvector create");
        }

        vpoint& at(size_t k) {return r[k / flen];}
        vpoint at(size_t k) const {return r[k / flen];}
        template<class pt = point>
        inline void set(size_t k, pt const& t) {
            if constexpr(std::is_same_v<pt, point>) {
                real(r[k / flen])[k % flen] = real(t);
                imag(r[k / flen])[k % flen] = imag(t);
            } else {
                at(k) = t;
            }
        }
        template<class pt = point>
        inline pt get(size_t k) const {
            if constexpr(std::is_same_v<pt, point>) {
                return {real(r[k / flen])[k % flen], imag(r[k / flen])[k % flen]};
            } else {
                return at(k);
            }
        }

        size_t size() const {
            return flen * r.size();
        }
        static constexpr size_t eval_arg(size_t n) {
            if(n < pre_evals) {
                return eval_args[n];
            } else {
                return eval_arg(n / 2) | (n & 1) << (std::bit_width(n) - 1);
            }
        }
        static constexpr point eval_point(size_t n) {
            if(n % 2) {
                return -eval_point(n - 1);
            } else if(n % 4) {
                return eval_point(n - 2) * point(0, 1);
            } else if(n / 4 < pre_evals) {
                return evalp[n / 4];
            } else if(n / 4 - pre_evals < extra.size()) {
                return extra[n / 4 - pre_evals];
            } else {
                return polar<ftype>(1., std::numbers::pi / (ftype)std::bit_floor(n) * (ftype)eval_arg(n));
            }
        }
        static constexpr std::array<point, 32> roots = []() {
            std::array<point, 32> res;
            for(size_t i = 2; i < 32; i++) {
                res[i] = polar<ftype>(1., std::numbers::pi / (1ull << (i - 2)));
            }
            return res;
        }();
        static constexpr point root(size_t n) {
            return roots[std::bit_width(n)];
        }
        template<int step>
        static void exec_on_eval(size_t n, size_t k, auto &&callback) {
            callback(k, root(4 * step * n) * eval_point(step * k));
        }
        template<int step>
        static void exec_on_evals(size_t n, auto &&callback) {
            point factor=root(4*step*n);
            if constexpr(step==1 || step==2 || step==4){
                prepare_roots((step*n+3)/4);
                size_t i=0;
                if constexpr(step==1){
                    for(;i+4<=n;i+=4){
                        size_t k=i/4;
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                        point v=factor*e;
                        callback(i,v);callback(i+1,-v);
                        point iv(-imag(v),real(v));
                        callback(i+2,iv);callback(i+3,-iv);
                    }
                }else if constexpr(step==2){
                    for(;i+2<=n;i+=2){
                        size_t k=i/2;
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                        point v=factor*e;
                        callback(i,v);callback(i+1,point(-imag(v),real(v)));
                    }
                }
                for(;i<n;i++){
                    size_t index=step*i,k=index/4;
                    point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                    if(index&2)e=e*point(0,1);
                    if(index&1)e=-e;
                    callback(i,factor*e);
                }
            }else{
                for(size_t i=0;i<n;i++)callback(i,factor*eval_point(step*i));
            }
        }

        static void do_dot_iter(point rt, vpoint& Bv, vpoint const& Av, vpoint& res) {
            res += Av * Bv;
            real(Bv) = rotate_right(real(Bv));
            imag(Bv) = rotate_right(imag(Bv));
            auto x = real(Bv)[0], y = imag(Bv)[0];
            real(Bv)[0] = x * real(rt) - y * imag(rt);
            imag(Bv)[0] = x * imag(rt) + y * real(rt);
        }

        template<size_t fixed = 0>
        void dot(cvector const& t) {
            size_t n = fixed?fixed:this->size();
            exec_on_evals<1>(n / flen, [&](size_t k, point rt) __attribute__((always_inline)) {
                k *= flen;
                auto [Ax, Ay] = at(k);
                auto Cv = t.at(k);
                vpoint vrt = {vz + real(rt), vz + imag(rt)};
                auto Cr = Cv * vrt;
                vpoint res = vz;
                auto iter = [&]<int i>() __attribute__((always_inline)) {
                    auto wrap = [&](vftype original, vftype rotated) {
                        if constexpr(i == 0) {return original;}
                        else {return __builtin_shufflevector(rotated, original, 4-i,5-i,6-i,7-i);}
                    };
                    vpoint Cw = {wrap(real(Cv),real(Cr)),wrap(imag(Cv),imag(Cr))};
                    vpoint Av = {vz+Ax[i],vz+Ay[i]};
                    return Av*Cw;
                };
                auto p0=iter.template operator()<0>(), p1=iter.template operator()<1>();
                auto p2=iter.template operator()<2>(), p3=iter.template operator()<3>();
                res=(p0+p1)+(p2+p3);
                set(k, res);
            });
            checkpoint("dot");
        }
        // normalize=false leaves the inverse-transform scale for the caller.
        template<bool partial = true, bool normalize = true, size_t fixed = 0>
        void ifft() {
            size_t n = fixed?fixed:size();
            if constexpr (!partial) {
                prepare_roots(n / 4);
                point pi(0, 1);
                exec_on_evals<4>(n / 4, [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 4;
                    point v1 = conj(rt);
                    point v2 = v1 * v1;
                    point v3 = v1 * v2;
                    auto A = get(k);
                    auto B = get(k + 1);
                    auto C = get(k + 2);
                    auto D = get(k + 3);
                    set(k, (A + B) + (C + D));
                    set(k + 2, ((A + B) - (C + D)) * v2);
                    set(k + 1, ((A - B) - pi * (C - D)) * v1);
                    set(k + 3, ((A - B) + pi * (C - D)) * v3);
                });
            }
            bool parity = std::countr_zero(n) % 2;
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 2 * flen;
                    vpoint cvrt = {vz + real(rt), vz - imag(rt)};
                    auto B = at(k) - at(k + flen);
                    at(k) += at(k + flen);
                    at(k + flen) = B * cvrt;
                });
            }

            transform<true,fixed>(n, parity);
            checkpoint("ifft");
            if constexpr(normalize) {
                auto scale = vz + ftype(partial ? flen : 1) / ftype(n);
                for(size_t k = 0; k < n; k += flen) {
                    set(k, get<vpoint>(k) * scale);
                }
            }
        }
        template<bool partial = true, size_t fixed = 0>
        void fft() {
            size_t n = fixed?fixed:size();
            prepare_roots(n / (partial ? 16 : 4));
            bool parity = std::countr_zero(n) % 2;
            transform<false,fixed>(n, parity);
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 2 * flen;
                    vpoint vrt = {vz + real(rt), vz + imag(rt)};
                    auto t = at(k + flen) * vrt;
                    at(k + flen) = at(k) - t;
                    at(k) += t;
                });
            }
            if constexpr (!partial) {
                prepare_roots(n / 4);
                point pi(0, 1);
                exec_on_evals<4>(n / 4, [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 4;
                    point v1 = rt;
                    point v2 = v1 * v1;
                    point v3 = v1 * v2;
                    auto A = get(k);
                    auto B = get(k + 1) * v1;
                    auto C = get(k + 2) * v2;
                    auto D = get(k + 3) * v3;
                    set(k, (A + C) + (B + D));
                    set(k + 1, (A + C) - (B + D));
                    set(k + 2, (A - C) + pi * (B - D));
                    set(k + 3, (A - C) - pi * (B - D));
                });
            }
            checkpoint("fft");
        }
        static std::array<vpoint,4> transpose(std::array<vpoint,4> const&a){
            auto half=[](auto part){
                auto a0=__m256d(part(0)),a1=__m256d(part(1)),a2=__m256d(part(2)),a3=__m256d(part(3));
                auto t0=_mm256_unpacklo_pd(a0,a1),t1=_mm256_unpackhi_pd(a0,a1),t2=_mm256_unpacklo_pd(a2,a3),t3=_mm256_unpackhi_pd(a2,a3);
                return std::array<vftype,4>{vftype(_mm256_permute2f128_pd(t0,t2,0x20)),vftype(_mm256_permute2f128_pd(t1,t3,0x20)),vftype(_mm256_permute2f128_pd(t0,t2,0x31)),vftype(_mm256_permute2f128_pd(t1,t3,0x31))};
            };
            auto re=half([&](int k){return real(a[k]);}),im=half([&](int k){return imag(a[k]);});
            return {vpoint{re[0],im[0]},vpoint{re[1],im[1]},vpoint{re[2],im[2]},vpoint{re[3],im[3]}};
        }
        // Multiply groups of 16 points as polynomials modulo x^16 - rt: one radix-4 level on
        // each operand, a lane-parallel 4-coefficient Karatsuba product, one inverse level.
        // The weights rt, rt^2, rt^3 of a tile of groups are computed four groups per SIMD
        // operation ahead of the tile, so the product loop only broadcasts them from memory.
        static constexpr size_t dot_tile = 64;
        template<size_t fixed, bool square = false>void dot_fused16(cvector const& t,size_t offset,size_t length){
            constexpr size_t T=dot_tile;
            const size_t n=fixed?fixed:size();
            point factor=root(n);
            auto fr=_mm256_set1_pd(real(factor)),fi=_mm256_set1_pd(imag(factor));
            alignas(32) double tw[6][T];
            const bool ahead=offset+length<n;
            auto cmadd=[](vpoint x,vpoint y,vpoint c) __attribute__((always_inline)) {
                auto re=_mm256_fmadd_pd(__m256d(real(x)),__m256d(real(y)),_mm256_fnmadd_pd(__m256d(imag(x)),__m256d(imag(y)),__m256d(real(c))));
                auto im=_mm256_fmadd_pd(__m256d(real(x)),__m256d(imag(y)),_mm256_fmadd_pd(__m256d(imag(x)),__m256d(real(y)),__m256d(imag(c))));
                return vpoint{vftype(re),vftype(im)};
            };
            auto cmcross=[](vpoint x,vpoint y,vpoint p,vpoint q) __attribute__((always_inline)) {
                auto re=_mm256_sub_pd(_mm256_fmsub_pd(__m256d(real(x)),__m256d(real(y)),__m256d(real(p))),_mm256_fmadd_pd(__m256d(imag(x)),__m256d(imag(y)),__m256d(real(q))));
                auto im=_mm256_add_pd(_mm256_fmsub_pd(__m256d(real(x)),__m256d(imag(y)),__m256d(imag(p))),_mm256_fmsub_pd(__m256d(imag(x)),__m256d(real(y)),__m256d(imag(q))));
                return vpoint{vftype(re),vftype(im)};
            };
            // z * conj(v)
            auto mulconj=[](vpoint z,vpoint v) __attribute__((always_inline)) {
                auto re=_mm256_fmadd_pd(__m256d(real(z)),__m256d(real(v)),_mm256_mul_pd(__m256d(imag(z)),__m256d(imag(v))));
                auto im=_mm256_fmsub_pd(__m256d(imag(z)),__m256d(real(v)),_mm256_mul_pd(__m256d(real(z)),__m256d(imag(v))));
                return vpoint{vftype(re),vftype(im)};
            };
            for(size_t tile=offset;tile<offset+length;tile+=16*T){
                size_t k0=tile/16;
                auto const* e=reinterpret_cast<double const*>(k0<pre_evals?evalp.data()+k0:extra.data()+(k0-pre_evals));
                for(size_t g=0;g<T;g+=4){
                    auto x=_mm256_loadu_pd(e+2*g),y=_mm256_loadu_pd(e+2*g+4);
                    auto er=_mm256_permute4x64_pd(_mm256_unpacklo_pd(x,y),0xD8),ei=_mm256_permute4x64_pd(_mm256_unpackhi_pd(x,y),0xD8);
                    auto r1=_mm256_fmsub_pd(fr,er,_mm256_mul_pd(fi,ei)),i1=_mm256_fmadd_pd(fr,ei,_mm256_mul_pd(fi,er));
                    auto r2=_mm256_fmsub_pd(r1,r1,_mm256_mul_pd(i1,i1)),i2=_mm256_fmadd_pd(r1,i1,_mm256_mul_pd(i1,r1));
                    auto r3=_mm256_fmsub_pd(r1,r2,_mm256_mul_pd(i1,i2)),i3=_mm256_fmadd_pd(r1,i2,_mm256_mul_pd(i1,r2));
                    _mm256_store_pd(tw[0]+g,r1);_mm256_store_pd(tw[1]+g,i1);_mm256_store_pd(tw[2]+g,r2);
                    _mm256_store_pd(tw[3]+g,i2);_mm256_store_pd(tw[4]+g,r3);_mm256_store_pd(tw[5]+g,i3);
                }
                for(size_t g=0;g<T;g++){
                    size_t pos=tile+16*g;
                    if(ahead){
                        // Pull the next block of both operands towards the cache while this one is multiplied.
                        auto const* pa=reinterpret_cast<char const*>(r.data()+(pos+length)/flen);
                        auto const* pb=reinterpret_cast<char const*>(t.r.data()+(pos+length)/flen);
                        for(size_t c=0;c<4;c++){_mm_prefetch(pa+64*c,_MM_HINT_T2);_mm_prefetch(pb+64*c,_MM_HINT_T2);}
                    }
                    auto bc=[&](size_t c) __attribute__((always_inline)) {return vftype(_mm256_broadcast_sd(tw[c]+g));};
                    vpoint v1={bc(0),bc(1)},v2={bc(2),bc(3)},v3={bc(4),bc(5)};
                    auto forward=[&](cvector const& a) __attribute__((always_inline)) {
                        auto A=a.at(pos),B=a.at(pos+4)*v1,C=a.at(pos+8)*v2,D=a.at(pos+12)*v3;
                        return std::array<vpoint,4>{(A+C)+(B+D),(A+C)-(B+D),(A-C)+vi(B-D),(A-C)-vi(B-D)};
                    };
                    auto a=transpose(forward(*this));
                    auto b=[&] {if constexpr(square) return a; else return transpose(forward(t));}();
                    // The four residues are taken modulo x^4 - rt * {1, -1, i, -i}.
                    const auto flip_re=_mm256_set_pd(0.,-0.,-0.,0.),flip_im=_mm256_set_pd(-0.,0.,-0.,0.);
                    vpoint w={vftype(_mm256_xor_pd(_mm256_blend_pd(__m256d(real(v1)),__m256d(imag(v1)),0b1100),flip_re)),
                              vftype(_mm256_xor_pd(_mm256_blend_pd(__m256d(imag(v1)),__m256d(real(v1)),0b1100),flip_im))};
                    auto mul=[&](vpoint a0,vpoint a1,vpoint b0,vpoint b1) __attribute__((always_inline)) {
                        auto p=a0*b0,q=a1*b1;
                        return std::array<vpoint,2>{cmadd(w,q,p),cmcross(a0+a1,b0+b1,p,q)};
                    };
                    auto p=mul(a[0],a[2],b[0],b[2]),q=mul(a[1],a[3],b[1],b[3]);
                    auto m=mul(a[0]+a[1],a[2]+a[3],b[0]+b[1],b[2]+b[3]);
                    auto c=transpose({cmadd(w,q[1],p[0]),(m[0]-p[0])-q[0],p[1]+q[0],(m[1]-p[1])-q[1]});
                    auto A=c[0],B=c[1],C=c[2],D=c[3];
                    at(pos)=(A+B)+(C+D);at(pos+8)=mulconj((A+B)-(C+D),v2);
                    at(pos+4)=mulconj((A-B)-vi(C-D),v1);at(pos+12)=mulconj((A-B)+vi(C-D),v3);
                }
            }
        }
        // A product in two steps, this <- this * t modulo x^n - i up to the factor n / flen:
        // forward() on both operands, then multiply(t), which may be *this for a square.
        // The forward transform stops at groups of 16 points and dot_fused16 multiplies those,
        // which saves a pass over each operand and one over the result. That needs powers of
        // four, so a length 2 * 4^k takes its radix-two level at the top, modulo
        // x^(n/2) -+ sqrt(i); short lengths take the complete transform and the 4-point product.
        void forward() {
            if(!fused_leaves()) {return fft();}
            size_t n = size(), part = fused_part();
            if(part < n) {
                vpoint vrt = {vz + real(roots[4]), vz + imag(roots[4])};
                for(size_t k = 0; k < part; k += flen) {
                    auto t = at(k + part) * vrt;
                    at(k + part) = at(k) - t;
                    at(k) += t;
                }
            }
            for(size_t offset = 0; offset < n; offset += part) {
                transform<false, 0, 0, -1, true>(n, false, offset, part);
            }
            checkpoint("fft");
        }
        void multiply(cvector const& t) {
            if(!fused_leaves()) {dot(t); return ifft<true, false>();}
            size_t n = size(), part = fused_part();
            dot_fused16<0>(t, 0, n);
            checkpoint("dot");
            for(size_t offset = 0; offset < n; offset += part) {
                transform<true, 0, 0, -1, true>(n, false, offset, part);
            }
            if(part < n) {
                vpoint cvrt = {vz + real(roots[4]), vz - imag(roots[4])};
                for(size_t k = 0; k < part; k += flen) {
                    auto t = at(k) - at(k + part);
                    at(k) += at(k + part);
                    at(k + part) = t * cvrt;
                }
            }
            checkpoint("ifft");
        }
        // Radix-64 out-of-cache pass for n = 2^24, run as two tiled radix-8 stages.
        // With Fuse=1 (forward only) the first stage lifts u32 residues to Gaussian
        // coordinates on load, so the spectrum buffer is written once and never re-read
        // before the in-cache block phase. The index-keyed noise makes both branches see
        // the same representatives.
        struct fuse_args {
            const uint32_t* src = nullptr;
            size_t count = 0;
            uint64_t seed = 0;
            double a = 0, b = 0, a_over_p = 0, b_over_p = 0;
        };
        static constexpr size_t sweep_tile = 256;
        template<bool inverse, int Mode, size_t Tile, int Fuse = 0, bool Neg = false>
        void sweep8(fuse_args const& fa = {}) {
            constexpr size_t n = 1 << 24;
            static const std::array<std::array<point, 7>, 9> weights = []() {
                std::array<std::array<point, 7>, 9> table;
                for(size_t count: {size_t(1), size_t(8)}) for(size_t k = 0; k < count; k++) {
                    size_t rev = 0, x = k;
                    for(size_t c = count; c > 1; c >>= 1) {rev = (rev << 1) | (x & 1); x >>= 1;}
                    long double angle = std::numbers::pi_v<long double> * (1 + 4 * rev) / (16 * count);
                    if(k & 1) {angle -= std::numbers::pi_v<long double> / 4;}
                    for(size_t j = 1; j < 8; j++) {
                        long double a = j * angle;
                        if constexpr(Mode == 0) {table[(count - 1) / 7 + k][j - 1] = {double(cosl(a)), double(sinl(a))};}
                        else {table[(count - 1) / 7 + k][j - 1] = {double(sinl(a)), double(tanl(a / 2))};}
                    }
                }
                return table;
            }();
            auto lift = [&](size_t idx) __attribute__((always_inline)) -> vpoint {
                i32x4 bits{};
                if(idx + 4 <= fa.count) {std::memcpy(&bits, fa.src + idx, sizeof(bits));}
                else if(idx < fa.count) {for(size_t j = 0; j < fa.count - idx; j++) {bits[j] = int32_t(fa.src[idx + j]);}}
                else {return vpoint{vz, vz};}
                auto x = __builtin_convertvector(bits, vftype);
                u32x4 h = u32x4{uint32_t(idx), uint32_t(idx + 1), uint32_t(idx + 2), uint32_t(idx + 3)} ^ uint32_t(fa.seed);
                h *= 0x9E3779B1u; h ^= h >> 15; h *= 0x85EBCA77u; h ^= h >> 13; h *= 0xC2B2AE3Du; h ^= h >> 16;
                auto noise = __builtin_convertvector(i32x4(h), vftype) * 0x1p-32;
                auto q = round(x * fa.a_over_p + noise), t = round(x * fa.b_over_p + noise);
                auto re = x - q * fa.a - t * fa.b, im = t * fa.a - q * fa.b;
                return vpoint{re, Neg ? -im : im};
            };
            auto stage = [&]<bool top>(size_t offset, size_t length, size_t begin, size_t end) __attribute__((always_inline)) {
                size_t step = length / 8, k = offset / length;
                auto const& w = weights[(n / length - 1) / 7 + k];
                auto rot = [&]<size_t J>(vpoint z) __attribute__((always_inline)) {
                    auto c = w[J - 1];
                    if constexpr(Mode == 0) {return z * vpoint{vz + real(c), inverse ? vz - imag(c) : vz + imag(c)};}
                    else {
                        auto s = inverse ? vz - real(c) : vz + real(c), t = inverse ? vz - imag(c) : vz + imag(c);
                        auto x = vftype(_mm256_fnmadd_pd(__m256d(t), __m256d(imag(z)), __m256d(real(z))));
                        auto y = vftype(_mm256_fmadd_pd(__m256d(s), __m256d(x), __m256d(imag(z))));
                        return vpoint{vftype(_mm256_fnmadd_pd(__m256d(t), __m256d(y), __m256d(x))), y};
                    }
                };
                auto add = [](vpoint a, vpoint b) __attribute__((always_inline)) {
                    if constexpr(Mode != 2) {return a + b;}
                    else {return vpoint{vftype(_mm256_fmadd_pd(__m256d(real(a)), _mm256_set1_pd(1), __m256d(real(b)))), vftype(_mm256_fmadd_pd(__m256d(imag(a)), _mm256_set1_pd(1), __m256d(imag(b))))};}
                };
                auto sub = [](vpoint a, vpoint b) __attribute__((always_inline)) {
                    if constexpr(Mode != 2) {return a - b;}
                    else {return vpoint{vftype(_mm256_fmsub_pd(__m256d(real(a)), _mm256_set1_pd(1), __m256d(real(b)))), vftype(_mm256_fmsub_pd(__m256d(imag(a)), _mm256_set1_pd(1), __m256d(imag(b))))};}
                };
                auto d4 = [](vpoint a, vpoint b, vpoint c, vpoint d) __attribute__((always_inline)) {
                    auto s = a + c, t = a - c, u = b + d, v = vi(b - d);
                    if constexpr(inverse) {return std::array<vpoint, 4>{s + u, t - v, s - u, t + v};}
                    else {return std::array<vpoint, 4>{s + u, t + v, s - u, t - v};}
                };
                constexpr double q = 0.707106781186547524400844362104849039;
                auto r1 = [](vpoint z) __attribute__((always_inline)) {
                    if constexpr(inverse) {return vpoint{(real(z) + imag(z)) * q, (imag(z) - real(z)) * q};}
                    else {return vpoint{(real(z) - imag(z)) * q, (real(z) + imag(z)) * q};}
                };
                auto r3 = [](vpoint z) __attribute__((always_inline)) {
                    if constexpr(inverse) {return vpoint{(imag(z) - real(z)) * q, (-imag(z) - real(z)) * q};}
                    else {return vpoint{(-real(z) - imag(z)) * q, (real(z) - imag(z)) * q};}
                };
                std::array<vpoint*, 8> input, output;
                for(size_t j = 0; j < 8; j++) {input[j] = output[j] = r.data() + (offset + begin + j * step) / flen;}
                if(k & 1) {
                    constexpr std::array<size_t, 8> perm = {7, 6, 4, 5, 0, 1, 2, 3};
                    for(size_t j = 0; j < 8; j++) {
                        if constexpr(inverse) {input[j] = output[perm[j]];}
                        else {output[j] = input[perm[j]];}
                    }
                }
                constexpr bool fused_in = top && Fuse == 1 && !inverse;
                for(size_t j = 0; j < (end - begin) / flen; j++) {
                    size_t base = offset + begin + j * flen;
                    auto in = [&]<size_t S>() __attribute__((always_inline)) {
                        if constexpr(fused_in) {return lift(base + S * step);}
                        else {return input[S][j];}
                    };
                    if constexpr(!inverse) {
                        auto E = d4(in.template operator()<0>(), rot.template operator()<2>(in.template operator()<2>()), rot.template operator()<4>(in.template operator()<4>()), rot.template operator()<6>(in.template operator()<6>()));
                        auto O = d4(rot.template operator()<1>(in.template operator()<1>()), rot.template operator()<3>(in.template operator()<3>()), rot.template operator()<5>(in.template operator()<5>()), rot.template operator()<7>(in.template operator()<7>()));
                        auto a = O[0], b = r1(O[1]), c = vi(O[2]), d = r3(O[3]);
                        output[0][j] = add(E[0], a); output[1][j] = sub(E[0], a);
                        output[2][j] = add(E[2], c); output[3][j] = sub(E[2], c);
                        output[4][j] = add(E[1], b); output[5][j] = sub(E[1], b);
                        output[6][j] = add(E[3], d); output[7][j] = sub(E[3], d);
                    } else {
                        auto E = d4(add(input[0][j], input[1][j]), add(input[4][j], input[5][j]), add(input[2][j], input[3][j]), add(input[6][j], input[7][j]));
                        auto O = d4(sub(input[0][j], input[1][j]), r1(sub(input[4][j], input[5][j])), -vi(sub(input[2][j], input[3][j])), r3(sub(input[6][j], input[7][j])));
                        output[0][j] = E[0]; output[1][j] = rot.template operator()<1>(O[0]);
                        output[2][j] = rot.template operator()<2>(E[1]); output[3][j] = rot.template operator()<3>(O[1]);
                        output[4][j] = rot.template operator()<4>(E[2]); output[5][j] = rot.template operator()<5>(O[2]);
                        output[6][j] = rot.template operator()<6>(E[3]); output[7][j] = rot.template operator()<7>(O[3]);
                    }
                }
            };
            constexpr size_t h = n / 64;
            for(size_t j = 0; j < h; j += Tile) {
                size_t end = std::min(h, j + Tile);
                auto first = [&]() {for(size_t t = 0; t < 8; t++) {stage.template operator()<true>(0, n, j + t * h, end + t * h);}};
                auto second = [&]() {for(size_t k = 0; k < 8; k++) {stage.template operator()<false>(k * n / 8, n / 8, j, end);}};
                if constexpr(inverse) {second(); first();} else {first(); second();}
            }
        }
        // Product for n = 2^24: the top three stages split each input into 64 independent
        // blocks. Both forward transforms read the u32 inputs directly; each block then
        // completes its two forward transforms, the product, and the inverse before the
        // final three inverse stages combine the results.
        // Fusing the Gaussian lift into the first pass saves a write and a read of each
        // spectrum, but makes that pass compute-bound on the judge (measured slower there),
        // so it stays opt-in; by default both spectra are filled first and swept in place.
        static constexpr bool fuse_forward = false;
        template<bool Neg, bool Fused = fuse_forward, bool Square = false>
        void cache_product(cvector& b, fuse_args const& fa = {}, fuse_args const& fb = {}) {
            constexpr size_t n = 1 << 24, block = 1 << 18;
            prepare_roots(n / 16); prepare_shear_roots();
            if constexpr(Fused) {
                sweep8<false, 2, sweep_tile, 1, Neg>(fa);
                if constexpr(!Square) {b.sweep8<false, 2, sweep_tile, 1, Neg>(fb);}
            } else {
                sweep8<false, 2, sweep_tile>();
                if constexpr(!Square) {b.sweep8<false, 2, sweep_tile>();}
            }
            checkpoint("sweep forward");
            for(size_t offset = 0; offset < n; offset += block) {
                transform<false, n, block, 0, true>(n, false, offset, block);
                if constexpr(!Square) {b.transform<false, n, block, 0, true>(n, false, offset, block);}
                dot_fused16<n, Square>(b, offset, block);
                transform<true, n, block, 0, true>(n, false, offset, block);
            }
            checkpoint("blocks");
            sweep8<true, 2, sweep_tile>();
            checkpoint("sweep inverse");
        }
        static constexpr size_t pre_evals = 1 << 16;
        static const std::array<size_t, pre_evals> eval_args;
        static const std::array<point, pre_evals> evalp;
    private:
        // The power of four that the fused product works on: all of n = 4^k, half of n = 2 * 4^k.
        size_t fused_part() const {
            return size() >> (std::countr_zero(size()) % 2);
        }
        bool fused_leaves() const {
            return fused_part() >= 16 * dot_tile;
        }
        // Tile two radix-four stages together before descending into each child.
        template<bool inverse, size_t fixed = 0, size_t range_fixed=0, int top_fixed=-1,bool omit16=false>
        void transform(size_t input_n, bool parity, size_t range_offset=0, size_t range_length=0, int top_only=0) {
            if constexpr(range_fixed)range_length=range_fixed;
            if constexpr(top_fixed>=0)top_only=top_fixed;
            const size_t n=fixed?fixed:input_n;
            if constexpr(!range_fixed){prepare_roots(n/16);
            if constexpr(fixed==(1<<24))prepare_shear_roots();}
            size_t log_n=std::countr_zero(n);
            auto butterfly = [&](size_t offset,size_t length,size_t begin,size_t end) __attribute__((always_inline)) {
                if constexpr(omit16)if(length==16)return;
                size_t step=length/4,log_length=std::countr_zero(length),k=offset>>log_length;
                auto *p0=r.data()+(offset+begin)/flen,*p1=r.data()+(offset+begin+step)/flen,*p2=r.data()+(offset+begin+2*step)/flen,*p3=r.data()+(offset+begin+3*step)/flen;
                auto run=[&]<bool shear>() __attribute__((always_inline)) {
                    vpoint v1,v2,v3;vftype t1{},t2{},t3{};
                    if constexpr(shear && fixed==(1<<24)) {
                        auto const& c=shear_roots[((n>>log_length)-1)/3+k];
                        v1={vz,inverse?vz-real(c[0]):vz+real(c[0])};
                        v2={vz,inverse?vz-real(c[1]):vz+real(c[1])};
                        v3={vz,inverse?vz-real(c[2]):vz+real(c[2])};
                        t1=inverse?vz-imag(c[0]):vz+imag(c[0]);
                        t2=inverse?vz-imag(c[1]):vz+imag(c[1]);
                        t3=inverse?vz-imag(c[2]):vz+imag(c[2]);
                    }else {
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];point rt=roots[log_n+5-log_length]*e;
                        v1={vz+real(rt),inverse?vz-imag(rt):vz+imag(rt)};
                        if constexpr(shear)if(k&1){if constexpr(inverse)v1=vi(v1);else v1=-vi(v1);}
                        v2=v1*v1;v3=v1*v2;
                        if constexpr(shear){t1=imag(v1)/(vz+1.0+real(v1));t2=imag(v2)/(vz+1.0+real(v2));t3=imag(v3)/(vz+1.0+real(v3));}
                    }
                    auto rotate=[](vpoint z,vpoint v,vftype t) __attribute__((always_inline)) {
                        if constexpr(!shear)return z*v;
                        else {
                            auto x=vftype(_mm256_fnmadd_pd(__m256d(t),__m256d(imag(z)),__m256d(real(z))));
                            auto y=vftype(_mm256_fmadd_pd(__m256d(imag(v)),__m256d(x),__m256d(imag(z))));
                            return vpoint{vftype(_mm256_fnmadd_pd(__m256d(t),__m256d(y),__m256d(x))),y};
                        }
                    };
                    auto *i0=p0,*i1=p1,*i2=p2,*i3=p3,*o0=p0,*o1=p1,*o2=p2,*o3=p3;
                    if constexpr(shear)if(k&1) {
                        if constexpr(inverse){i0=p3;i1=p2;i2=p0;i3=p1;}
                        else{o0=p3;o1=p2;o2=p0;o3=p1;}
                    }
                    for(size_t j=0;j<(end-begin)/flen;j++) {
                        auto A=i0[j],B=i1[j],C=i2[j],D=i3[j];
                        if constexpr(inverse) {
                            o0[j]=(A+B)+(C+D);
                            o2[j]=rotate((A+B)-(C+D),v2,t2);
                            o1[j]=rotate((A-B)-vi(C-D),v1,t1);
                            o3[j]=rotate((A-B)+vi(C-D),v3,t3);
                        }else{
                            B=rotate(B,v1,t1);C=rotate(C,v2,t2);D=rotate(D,v3,t3);
                            o0[j]=(A+C)+(B+D);o1[j]=(A+C)-(B+D);
                            o2[j]=(A-C)+vi(B-D);o3[j]=(A-C)-vi(B-D);
                        }
                    }
                };
                if(length>=shear_min<fixed>)run.template operator()<true>();else run.template operator()<false>();
            };
            if(top_only){
                size_t offset=range_offset,length=range_length;
                if(top_only==1){butterfly(offset,length,0,length/4);return;}
                if(top_only==3){
                    size_t h=length/64;
                    for(size_t j=0;j<h;j+=512){
                        size_t end=std::min(h,j+512);
                        auto stage0=[&](){for(size_t t=0;t<16;t++)butterfly(offset,length,j+t*h,end+t*h);};
                        auto stage1=[&](){for(size_t q=0;q<4;q++)for(size_t t=0;t<4;t++)butterfly(offset+q*length/4,length/4,j+t*h,end+t*h);};
                        auto stage2=[&](){for(size_t q=0;q<16;q++)butterfly(offset+q*length/16,length/16,j,end);};
                        if constexpr(inverse){stage2();stage1();stage0();}else{stage0();stage1();stage2();}
                    }
                    return;
                }
                size_t step=length/16;
                for(size_t j=0;j<step;j+=256){
                    size_t end=std::min(step,j+256);
                    if constexpr(inverse){
                        for(size_t t=0;t<4;t++)butterfly(offset+t*length/4,length/4,j,end);
                        for(size_t t=0;t<4;t++)butterfly(offset,length,j+t*step,end+t*step);
                    }else{
                        for(size_t t=0;t<4;t++)butterfly(offset,length,j+t*step,end+t*step);
                        for(size_t t=0;t<4;t++)butterfly(offset+t*length/4,length/4,j,end);
                    }
                }
                return;
            }
            auto recurse = [&](auto &&self, size_t offset, size_t length) -> void {
                if(length < 4 * flen) {return;}
                if(length >= (1 << 15)) {
                    size_t step = length / 16;
                    if constexpr(inverse) {
                        for(size_t t = 0; t < 16; t++) {self(self, offset + t*step, step);}
                    }
                    for(size_t j = 0; j < step; j += 256) {
                        size_t end = std::min(step, j+256);
                        if constexpr(inverse) {
                            for(size_t t=0;t<4;t++) {butterfly(offset+t*length/4, length/4, j,end);}
                            for(size_t t=0;t<4;t++) {butterfly(offset,length,j+t*step,end+t*step);}
                        } else {
                            for(size_t t=0;t<4;t++) {butterfly(offset,length,j+t*step,end+t*step);}
                            for(size_t t=0;t<4;t++) {butterfly(offset+t*length/4,length/4,j,end);}
                        }
                    }
                    if constexpr(!inverse) {
                        for(size_t t = 0; t < 16; t++) {self(self, offset + t*step, step);}
                    }
                } else if(length >= (size_t(1) << (6 + parity))) {
                    auto finish=[&]<bool par>() {
                        constexpr size_t chunk=size_t(1)<<(6+par);
                        constexpr size_t bottom=size_t(1)<<(4+par);
                        auto small=[&]<size_t L>(auto&& self,size_t pos) __attribute__((always_inline)) -> void {
                            if constexpr(inverse && L>bottom) {
                                for(size_t q=0;q<4;q++)self.template operator()<L/4>(self,pos+q*(L/4));
                            }
                            butterfly(pos,L,0,L/4);
                            if constexpr(!inverse && L>bottom) {
                                for(size_t q=0;q<4;q++)self.template operator()<L/4>(self,pos+q*(L/4));
                            }
                        };
                        for(size_t leaf=offset;leaf<offset+length;leaf+=chunk){
                            if constexpr(inverse){
                                small.template operator()<chunk>(small,leaf);
                                size_t level=std::min<size_t>(std::countr_one(leaf+chunk-1),std::countr_zero(length));
                                for(size_t lvl=6+2+par;lvl<=level;lvl+=2){size_t len=size_t(1)<<lvl;butterfly(leaf & ~(len-1),len,0,len/4);}
                            }else{
                                size_t level=std::min<size_t>(std::countr_zero(n+leaf),std::countr_zero(length));
                                level-=level%2!=par;
                                for(size_t lvl=level;lvl>=6+2+par;lvl-=2){size_t len=size_t(1)<<lvl;butterfly(leaf & ~(len-1),len,0,len/4);}
                                small.template operator()<chunk>(small,leaf);
                            }
                        }
                    };
                    if(parity)finish.template operator()<true>();else finish.template operator()<false>();
                } else {
                    if constexpr(inverse) {
                        for(size_t leaf = offset + 3 * flen; leaf < offset + length; leaf += 4 * flen) {
                            size_t level = std::min<size_t>(std::countr_one(leaf + 3), std::countr_zero(length));
                            for(size_t lvl = 4 + parity; lvl <= level; lvl += 2) {
                                size_t len = size_t(1) << lvl;
                                butterfly(leaf & ~(len-1), len, 0, len / 4);
                            }
                        }
                    } else {
                        for(size_t leaf = offset; leaf < offset + length; leaf += 4 * flen) {
                            size_t level = std::min<size_t>(std::countr_zero(n + leaf), std::countr_zero(length));
                            level -= level % 2 != parity;
                            for(size_t lvl = level; lvl >= 4; lvl -= 2) {
                                size_t len = size_t(1) << lvl;
                                butterfly(leaf & ~(len-1), len, 0, len / 4);
                            }
                        }
                    }
                }
            };
            // Radix two is performed separately at the leaves.
            recurse(recurse, range_offset, range_length?range_length:n);
        }
        // Shortest butterfly that uses the shear rotation; the fixed 2^24 transform reads its
        // shear roots from a table, so they are worth using down to the last in-block level.
        template<size_t fixed> static constexpr size_t shear_min = fixed == (1 << 24) ? 64 : 256;
        static big_vector<std::array<point,3>> shear_roots;
        static void prepare_shear_roots() {
            constexpr size_t n=1<<24;
            if(!shear_roots.empty())return;
            shear_roots.resize((n/16-1)/3,std::array<point,3>{});
            for(size_t len=n;len>=shear_min<n>;len/=4) {
                size_t count=n/len,base=(count-1)/3;
                point factor=roots[29-std::countr_zero(len)];
                for(size_t k=0;k<count;k++) {
                    point rt=factor*(k<pre_evals?evalp[k]:extra[k-pre_evals]);
                    vpoint v1={vz+real(rt),vz+imag(rt)};
                    if(k&1)v1=-vi(v1);
                    vpoint v2=v1*v1,v3=v1*v2;
                    vftype t1=imag(v1)/(vz+1.0+real(v1)),t2=imag(v2)/(vz+1.0+real(v2)),t3=imag(v3)/(vz+1.0+real(v3));
                    shear_roots[base+k]={point(imag(v1)[0],t1[0]),point(imag(v2)[0],t2[0]),point(imag(v3)[0],t3[0])};
                }
            }
        }
        static big_vector<point> extra;
        // Keep the usual table small; cache additional roots for large transforms.
        static void prepare_roots(size_t n) {
            if(n <= pre_evals + extra.size()) {return;}
            size_t old = extra.size();
            extra.resize(std::bit_ceil(n) - pre_evals);
            static const std::array<point,256> coarse=[](){
                std::array<point,256> out;
                for(size_t i=0;i<256;i++)out[i]=polar<ftype>(1.,std::numbers::pi*double((eval_args[256+i]-1)/2)/512.0);
                return out;
            }();
            for(size_t h=pre_evals+old;h<pre_evals+extra.size();h*=2){
                for(size_t i=h;i<2*h;i+=256){
                    point fine=polar<ftype>(1.,std::numbers::pi/double(4*h)*double(eval_arg(4*i)));
                    for(size_t j=0;j<256;j++)extra[i+j-pre_evals]=coarse[j]*fine;
                }
            }
        }
    };

    big_vector<std::array<point,3>> cvector::shear_roots;
    big_vector<point> cvector::extra;

    const std::array<size_t, cvector::pre_evals> cvector::eval_args = []() {
        std::array<size_t, pre_evals> res = {};
        for(size_t i = 1; i < pre_evals; i++) {
            res[i] = res[i >> 1] | (i & 1) << (std::bit_width(i) - 1);
        }
        return res;
    }();
    const std::array<point, cvector::pre_evals> cvector::evalp = []() {
        std::array<point, pre_evals> res = {};
        res[0] = 1;
        for(size_t n = 1; n < pre_evals; n++) {
            res[n] = polar<ftype>(1., std::numbers::pi * ftype(eval_args[n]) / ftype(4 * std::bit_floor(n)));
        }
        return res;
    }();
}
#pragma GCC pop_options

#line 12 "cp-algo/math/ring.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::fft {
    size_t com_size(size_t as, size_t bs) {
        if(!as || !bs) {
            return 0;
        }
        return std::max(flen, std::bit_ceil(as + bs - 1) / 2);
    }
    // Whether a product of these sizes exceeds half of its padded length by a tail short enough
    // to be corrected naively, which lets mul_truncate halve its transform.
    constexpr size_t short_tail = 32;
    bool has_short_tail(size_t as, size_t bs) {
        size_t n = com_size(as, bs);
        return as + bs - 1 - n <= short_tail && as <= n && bs <= n;
    }
    // Convolution through an imaginary quadratic ring, for a prime modulus p < 2^31.
    // Let d be the smallest positive integer such that -d is a quadratic residue (d = 1 exactly
    // when p = 1 mod 4) and root^2 = -d. A residue x is written as re + im*sqrt(-d) with
    // re + im*root = x, (re, im) reduced by the lattice of representations of zero under the norm
    // re^2 + d*im^2, so that |re| and sqrt(d)*|im| are of order sqrt(p), and is embedded as the
    // complex number re + i*sqrt(d)*im. The product is then one complex convolution instead of
    // three real ones. For d = 1 (Gaussian integers) it is computed modulo x^n - i and x^n + i
    // (the latter through conjugation) and recombined modulo p, which needs i to lie in the ring;
    // for any other d it is a single transform of the full product length.
    constexpr uint64_t pow_mod(uint64_t x, uint64_t e, uint64_t p) {
        uint64_t res = 1;
        for(x %= p; e; e >>= 1, x = x * x % p) {if(e & 1) {res = res * x % p;}}
        return res;
    }
    // Smallest d > 0 with -d a quadratic residue modulo the odd prime p < 2^31, or 0.
    constexpr uint32_t least_negated_residue(uint64_t p) {
        for(uint32_t d = 1; d < 256 && d < p; d++) {
            if(pow_mod(p - d, (p - 1) / 2, p) == 1) {return d;}
        }
        return 0;
    }
    template<modint_type base>
    struct quadratic {
        static inline bool ready = false, available = false;
        static inline uint32_t d = 0, root = 0, prime = 0;
        // (a, b) and (a2, b2): reduced basis of the pairs (re, im) that represent zero.
        static inline int32_t a = 0, b = 0, a2 = 0, b2 = 0;
        // A fixed modulus settles d at compile time, so only the path in use is instantiated.
        static constexpr bool fixed_mod = requires {typename std::bool_constant<(base::mod(), true)>;};
        static constexpr uint32_t fixed_d = [] {
            if constexpr(fixed_mod) {return base::mod() > 2 && base::mod() % 2 ? least_negated_residue(base::mod()) : 0u;}
            else {return 0u;}
        }();
        static void init() {
            if(ready && prime == uint32_t(base::mod())) {return;}
            ready = true; available = false;
            uint64_t p = base::mod();
            prime = uint32_t(p);
            if(p < 3 || p >= (uint64_t(1) << 31) || !is_prime(p)) {return;}
            d = least_negated_residue(p);
            auto s = d ? cp_algo::math::sqrt(base(-int64_t(d))) : std::nullopt;
            if(!s) {return;}
            root = s->getr();
            // Lagrange reduction of (p, 0), (-root, 1) under the norm x^2 + d*y^2.
            using wide = __int128;
            wide x1 = p, y1 = 0, x2 = -wide(root), y2 = 1;
            auto norm = [&](wide x, wide y) {return x * x + d * y * y;};
            while(true) {
                if(norm(x1, y1) > norm(x2, y2)) {std::swap(x1, x2); std::swap(y1, y2);}
                wide dot = x1 * x2 + d * y1 * y2, len = norm(x1, y1);
                wide m = (2 * dot + (dot >= 0 ? len : -len)) / (2 * len);
                if(m == 0 || norm(x2 - m * x1, y2 - m * y1) >= norm(x2, y2)) {break;}
                x2 -= m * x1; y2 -= m * y1;
            }
            a = int32_t(x1); b = int32_t(y1);
            // In the Gaussian integers i*(a, b) = (-b, a) lies in the lattice as well.
            if(d == 1) {a2 = b; b2 = -a;}
            else {a2 = int32_t(x2); b2 = int32_t(y2);}
            assert(base(a) + base(b) * base(root) == base(0) && base(a2) + base(b2) * base(root) == base(0));
            assert(std::abs(int64_t(a) * b2 - int64_t(a2) * b) == int64_t(p));
            available = true;
        }
        // Points per transform: two branches of half the padded product length for d = 1, one
        // transform of the whole length otherwise. A short tail beyond that is computed naively
        // and taken back out, as in mul_truncate, which halves the transform: over the residues
        // for d = 1, where the branches together work modulo x^(2n) + 1, and in ring coordinates
        // for the single transform, which works modulo x^n - i.
        static size_t length(size_t as, size_t bs) {
            return (d == 1 ? com_size(as, bs) : 2 * com_size(as, bs)) >> has_short_tail(as, bs);
        }
        // Whether this path is used for operands of these sizes. With n = com_size(as, bs) it
        // takes six transforms of n points (three of 2n for d > 1), four for a square, where the
        // split representation takes seven and five, so it is preferred whenever it is exact,
        // unless a transform exceeds 2^24 points, the only length with a kernel tiled for data
        // outside the caches (which in turn does not let an operand wrap around).
        // Empirical rounding budget. Reusing a spectrum for a tiled square correlates its
        // errors, so that optimization uses a stricter budget below and otherwise keeps
        // independent lifts of the two operands.
        static bool usable(size_t as, size_t bs) {
            init();
            if(!available || std::min(as, bs) < size_t(magic)) {return false;}
            size_t need = as + bs - 1, n = length(as, bs);
            if(n > (1 << 24) || (n == (1 << 24) && d == 1 && std::max(as, bs) > n)) {return false;}
            using wide = unsigned __int128;
            return wide(need) * d * base::mod() * base::mod() <= wide(1) << 90;
        }
        // Lattice constants as doubles, hoisted out of the hot loops (the statics are 32-bit
        // integers and could alias the 32-bit output stream).
        struct lattice {
            // (c, e): the basis vector with the larger second coordinate, used to shrink im.
            double a, b, a2, b2, to_q, to_t, c, e, inv_e, root, scale, inv_scale, p;
            lattice(): a(quadratic::a), b(quadratic::b), a2(quadratic::a2), b2(quadratic::b2), p(base::mod()) {
                double det = a * b2 - a2 * b;
                to_q = b2 / det; to_t = -b / det;
                bool first = std::abs(b) >= std::abs(b2);
                c = first ? a : a2; e = first ? b : b2; inv_e = 1.0 / e;
                root = quadratic::root;
                scale = std::sqrt(double(d)); inv_scale = 1.0 / scale;
            }
        };
        // Map a rounded coordinate pair back to the residue re + root*im.
        template<bool unit>
        static u32x4 project(vpoint value, bool negative, lattice const& L) {
            const double p = L.p;
            auto R = round(real(value)), I = negative ? -imag(value) : imag(value);
            if constexpr(unit) {I = round(I);}
            else {I = round(I * L.inv_scale);}
            auto q = round(I * L.inv_e);
            auto U = vftype(_mm256_fnmadd_pd(__m256d(q), _mm256_set1_pd(L.c), __m256d(R)));
            auto V = vftype(_mm256_fnmadd_pd(__m256d(q), _mm256_set1_pd(L.e), __m256d(I)));
            auto h = vftype(_mm256_fmadd_pd(__m256d(V), _mm256_set1_pd(L.root), __m256d(U)));
            q = round(h * (1.0 / p));
            auto out = vftype(_mm256_fnmadd_pd(__m256d(q), _mm256_set1_pd(p), __m256d(h)));
            out = out < 0 ? out + p : out;
            return u32x4(_mm256_cvttpd_epi32(__m256d(out)));
        }
        // Lift residues to ring coordinates with stochastic rounding.
        // The xorshift stream is advanced once per 8 residues and restarted from the same
        // seed for both branches, so both see the same representatives.
        // With stream set the spectrum is written with non-temporal stores and left for
        // cache_product to transform: for n = 2^24 it is far larger than the caches and is next
        // read by a separate pass, so this saves the read-for-ownership of every destination line.
        // Coefficients from n on (d = 1 only) wrap around: x^n = i in the branch modulo x^n - i,
        // and the conjugated branch modulo x^n + i sees conj(-i * z) = i * conj(z).
        template<bool stream, bool unit>
        static void fill(cvector& c, auto const& x, auto const& upper, size_t n, bool negative, u64x4 state, bool transform = true) {
            const lattice L;
            auto const* src = reinterpret_cast<const uint32_t*>(std::data(x));
            size_t count = std::size(x);
            assert(count <= n && (std::empty(upper) || (unit && !stream && count == n)));
            c.r.resize(n / flen);
            auto* dst = reinterpret_cast<double*>(c.r.data());
            u32x8 words{};
            auto emit = [&]<bool wrap = false>(size_t i, i32x4 bits) __attribute__((always_inline)) {
                auto v = __builtin_convertvector(bits, vftype);
                if(i % 8 == 0) {state ^= state << 13; state ^= state >> 7; state ^= state << 17; words = u32x8(state);}
                i32x4 small = i % 8 ? i32x4(__builtin_shufflevector(words, words, 1, 3, 5, 7)) : i32x4(__builtin_shufflevector(words, words, 0, 2, 4, 6));
                auto noise = __builtin_convertvector(small, vftype) * 0x1p-32;
                auto q = round(v * L.to_q + noise), t = round(v * L.to_t + noise);
                auto re = v - q * L.a - t * L.a2, im = -(q * L.b) - t * L.b2;
                if constexpr(!unit) {im = im * L.scale;}
                if constexpr(stream) {
                    _mm256_stream_pd(dst + 2 * i, __m256d(re));
                    _mm256_stream_pd(dst + 2 * i + flen, __m256d(negative ? -im : im));
                } else if constexpr(wrap) {
                    c.r[(i - n) / flen] += vpoint{negative ? im : -im, re};
                } else {
                    c.r[i / flen] = vpoint{re, negative ? -im : im};
                }
            };
            size_t full = count / flen * flen;
            for(size_t i = 0; i < full; i += flen) {
                i32x4 bits;
                std::memcpy(&bits, src + i, sizeof(bits));
                emit(i, bits);
            }
            if(full < count) {
                i32x4 bits{};
                for(size_t j = full; j < count; j++) {bits[j - full] = int32_t(src[j]);}
                emit(full, bits);
            }
            if constexpr(stream) {_mm_sfence();}
            std::fill(c.r.begin() + (count + flen - 1) / flen, c.r.end(), vpoint{});
            auto const* wrapped = reinterpret_cast<const uint32_t*>(std::data(upper));
            for(size_t i = 0; i < std::size(upper); i += flen) {
                i32x4 bits{};
                for(size_t j = i; j < std::min(i + flen, std::size(upper)); j++) {bits[j - i] = int32_t(wrapped[j]);}
                emit.template operator()<true>(n + i, bits);
            }
            checkpoint("quadratic init");
            if constexpr(!stream) {if(transform) {c.forward();}}
        }
        // Lift a range of residues to ring coordinates, one point per coefficient, for the
        // reusable transforms below. Unlike fill it reads through the modint interface, so it
        // serves views and Montgomery storage alike, and it neither wraps nor streams.
        template<bool unit>
        static void lift(cvector& c, auto const& x, size_t n, bool negative, u64x4 state) {
            const lattice L;
            size_t total = std::size(x), count = std::min(n, total);
            assert(total <= 2 * n && (d == 1 || total <= n));
            c.r.resize(n / flen);
            u32x8 words{};
            size_t step = 0;
            auto emit = [&]<bool wrap>(size_t i, i32x4 bits) __attribute__((always_inline)) {
                auto v = __builtin_convertvector(bits, vftype);
                if(step % 2 == 0) {state ^= state << 13; state ^= state >> 7; state ^= state << 17; words = u32x8(state);}
                i32x4 small = step++ % 2 ? i32x4(__builtin_shufflevector(words, words, 1, 3, 5, 7))
                                         : i32x4(__builtin_shufflevector(words, words, 0, 2, 4, 6));
                auto noise = __builtin_convertvector(small, vftype) * 0x1p-32;
                auto q = round(v * L.to_q + noise), t = round(v * L.to_t + noise);
                auto re = v - q * L.a - t * L.a2, im = -(q * L.b) - t * L.b2;
                if constexpr(!unit) {im = im * L.scale;}
                // Coefficients from n on wrap around: x^n is i in this branch, -i in the other.
                if constexpr(wrap) {c.r[(i - n) / flen] += vpoint{negative ? im : -im, re};}
                else {c.r[i / flen] = vpoint{re, negative ? -im : im};}
            };
            // Residues are raw words exactly when the modulus is a compile-time constant; wider
            // storage than the residue needs is narrowed on the way in.
            constexpr bool plain = fixed_mod && std::ranges::contiguous_range<std::decay_t<decltype(x)>>;
            constexpr bool raw = plain && sizeof(base) == 4;
            constexpr bool wide = plain && sizeof(base) == 8;
            auto load = [&](size_t i, size_t upto) {
                i32x4 bits{};
                if constexpr(raw || wide) {
                    if(i + flen <= upto) {
                        if constexpr(raw) {
                            std::memcpy(&bits, reinterpret_cast<const uint32_t*>(std::data(x)) + i, sizeof(bits));
                        } else {
                            u64x4 words;
                            std::memcpy(&words, reinterpret_cast<const uint64_t*>(std::data(x)) + i, sizeof(words));
                            bits = __builtin_convertvector(words, i32x4);
                        }
                        return bits;
                    }
                }
                for(size_t j = i; j < std::min(i + flen, upto); j++) {bits[j - i] = int32_t(x[j].getr());}
                return bits;
            };
            for(size_t i = 0; i < count; i += flen) {emit.template operator()<false>(i, load(i, count));}
            std::fill(c.r.begin() + (count + flen - 1) / flen, c.r.end(), vpoint{});
            for(size_t i = n; i < total; i += flen) {emit.template operator()<true>(i, load(i, total));}
            checkpoint("quadratic init");
        }
        // Read a product out of its transformed branches, applying the inverse-transform scale.
        // For d = 1 the branches hold the product modulo x^n - i and modulo x^n + i, so the low
        // half of the result is their half-sum and the high half their half-difference over i.
        // The half of the result that the branches are recombined into is parked in the output
        // itself, which is always long enough: the high half is only wanted where the low half
        // has already been read back.
        static void recover(std::array<cvector, 2> const& parts, size_t n, double factor, auto& out, size_t k) {
            // Residues are raw 32-bit words exactly when the modulus is a compile-time constant,
            // which is what lets the recombination stay vectorized.
            constexpr bool plain = fixed_mod && std::ranges::contiguous_range<std::decay_t<decltype(out)>>;
            constexpr bool raw = plain && sizeof(base) == 4;
            constexpr bool wide = plain && sizeof(base) == 8;
            const lattice L;
            auto scale = vz + factor;
            size_t low = std::min(k, n);
            auto project_at = [&](cvector const& part, size_t i, bool negative) {
                if(d == 1) {return project<true>(part.at(i) * scale, negative, L);}
                else {return project<false>(part.at(i) * scale, negative, L);}
            };
            // Eight coefficients at a time, the width the recombination works in.
            auto project8 = [&](cvector const& part, size_t i, bool negative, size_t count) {
                auto lo8 = project_at(part, i, negative);
                auto hi8 = count > flen ? project_at(part, i + flen, negative) : u32x4{};
                return __builtin_shufflevector(lo8, hi8, 0, 1, 2, 3, 4, 5, 6, 7);
            };
            auto store8 = [&](size_t idx, u32x8 v, size_t count) {
                if constexpr(raw) {
                    if(count == 8) {std::memcpy(reinterpret_cast<uint32_t*>(std::data(out)) + idx, &v, sizeof(v)); return;}
                } else if constexpr(wide) {
                    if(count == 8) {
                        auto words = reinterpret_cast<uint64_t*>(std::data(out)) + idx;
                        auto lo4 = __builtin_convertvector(u32x4{v[0], v[1], v[2], v[3]}, u64x4);
                        auto hi4 = __builtin_convertvector(u32x4{v[4], v[5], v[6], v[7]}, u64x4);
                        std::memcpy(words, &lo4, sizeof(lo4));
                        std::memcpy(words + flen, &hi4, sizeof(hi4));
                        return;
                    }
                }
                for(size_t l = 0; l < count; l++) {out[idx + l].setr(typename base::UInt(v[l]));}
            };
            auto load8 = [&](size_t idx, size_t count) {
                u32x8 v{};
                if constexpr(raw) {
                    if(count == 8) {std::memcpy(&v, reinterpret_cast<uint32_t const*>(std::data(out)) + idx, sizeof(v)); return v;}
                } else if constexpr(wide) {
                    if(count == 8) {
                        auto words = reinterpret_cast<uint64_t const*>(std::data(out)) + idx;
                        u64x4 lo4, hi4;
                        std::memcpy(&lo4, words, sizeof(lo4));
                        std::memcpy(&hi4, words + flen, sizeof(hi4));
                        auto lo = __builtin_convertvector(lo4, u32x4), hi = __builtin_convertvector(hi4, u32x4);
                        return u32x8(__builtin_shufflevector(lo, hi, 0, 1, 2, 3, 4, 5, 6, 7));
                    }
                }
                for(size_t l = 0; l < count; l++) {v[l] = uint32_t(out[idx + l].getr());}
                return v;
            };
            for(size_t i = 0; i < low; i += 8) {
                store8(i, project8(parts[0], i, false, std::min<size_t>(8, low - i)), std::min<size_t>(8, low - i));
            }
            if(d > 1) {checkpoint("quadratic recover"); return;}
            const uint32_t mod32 = uint32_t(base::mod()), imod32 = -inv2<uint32_t>(base::mod());
            auto highmul = u32x8{} + uint32_t(((base(2) * base(root)).inv() * bpow(base(2), 32)).getr());
            for(size_t i = 0; i < low; i += 8) {
                size_t count = std::min<size_t>(8, low - i);
                auto minus = project8(parts[1], i, true, count);
                auto plus = load8(i, count);
                auto lo8 = reduce_once(plus + minus, mod32);
                lo8 = (lo8 + (lo8 & 1) * mod32) >> 1;
                auto hi8 = reduce_once(montgomery_mul(plus + mod32 - minus, highmul, mod32, imod32), mod32);
                store8(i, lo8, count);
                if(n + i < k) {store8(n + i, hi8, std::min<size_t>(count, k - n - i));}
            }
            checkpoint("quadratic recover");
        }
        // Cyclic product modulo x^k - 1, in place over a, for operands of exactly k coefficients.
        // The transform evaluates at the k-th roots of i, so twisting coefficient j by w^j with
        // w^k = -i turns the product it computes, modulo x^k - i, into the cyclic one; the
        // inverse twist is folded into the readback. A fold of the linear product would need
        // twice the transform.
        static void cyclic(auto& a, auto const& b, size_t k) {
            init();
            assert(available && std::popcount(k) == 1 && std::size(a) == k && std::size(b) == k);
            bool square = (void const*)std::data(a) == (void const*)std::data(b);
            // w^j from a table of every fourth power, built the way the root tables of the
            // transform are, and four consecutive powers per vector by one broadcast multiply.
            size_t groups = k / flen;
            size_t fine_bits = std::min<size_t>(8, std::countr_zero(std::max<size_t>(groups, 1)));
            size_t fine = size_t(1) << fine_bits;
            big_vector<point> low(fine), high((groups + fine - 1) >> fine_bits), step(groups);
            auto w = [&](size_t j) {return polar<ftype>(1., -std::numbers::pi * ftype(j) / ftype(2 * k));};
            for(size_t t = 0; t < low.size(); t++) {low[t] = w(flen * t);}
            for(size_t c = 0; c < high.size(); c++) {high[c] = w(flen * (c << fine_bits));}
            for(size_t c = 0; c < groups; c++) {step[c] = high[c >> fine_bits] * low[c & (fine - 1)];}
            vpoint quarter, quarter_conj;
            for(size_t l = 0; l < flen; l++) {
                point v = w(l);
                real(quarter)[l] = real(v); imag(quarter)[l] = imag(v);
                real(quarter_conj)[l] = real(v); imag(quarter_conj)[l] = -imag(v);
            }
            auto twiddle = [&](size_t j, bool inverse, ftype scale) {
                point t = step[j / flen];
                vpoint head = {vz + real(t) * scale, vz + (inverse ? -imag(t) : imag(t)) * scale};
                return head * (inverse ? quarter_conj : quarter);
            };
            auto run = [&]<bool unit>() {
                const lattice L;
                cvector A(k), B(0);
                lift<unit>(A, a, k, false, seed());
                if(!square) {lift<unit>(B, b, k, false, seed());}
                for(size_t j = 0; j < k; j += flen) {
                    auto w = twiddle(j, false, 1);
                    A.at(j) *= w;
                    if(!square) {B.at(j) *= w;}
                }
                A.forward();
                if(!square) {B.forward();}
                A.multiply(square ? A : B);
                for(size_t j = 0; j < k; j += flen) {
                    auto v = project<unit>(A.at(j) * twiddle(j, true, ftype(flen) / ftype(k)), false, L);
                    for(size_t l = 0; l < flen; l++) {a[j + l].setr(typename base::UInt(v[l]));}
                }
            };
            if(d == 1) {run.template operator()<true>();} else {run.template operator()<false>();}
            checkpoint("quadratic recover");
        }
        static u64x4 seed() {
            return u64x4{random::rng() | 1, random::rng() | 1, random::rng() | 1, random::rng() | 1};
        }
        // a <- a * b for d = 1, or a <- a * a with square set (b is not read then).
        // The first branch is stored in the upper half of a, so the part of a that wraps around
        // is set aside first.
        static void mul_branches(auto& a, auto const& b, bool square) {
            size_t as = std::size(a), bs = square ? as : std::size(b), need = as + bs - 1;
            size_t n = length(as, bs);
            assert(available && d == 1);
            // Coefficients from 2n on come back negated at the bottom; there are few of them.
            std::array<uint32_t, short_tail> high{};
            size_t tail = need > 2 * n ? need - 2 * n : 0;
            for(size_t i = 0; i < tail; i++) {
                auto const* x = reinterpret_cast<const uint32_t*>(std::data(a));
                auto const* y = square ? x : reinterpret_cast<const uint32_t*>(std::data(b));
                uint64_t sum = 0;
                for(size_t j = 2 * n + i - bs + 1; j < as; j++) {
                    sum = (sum + uint64_t(x[j]) * y[2 * n + i - j]) % prime;
                }
                high[i] = uint32_t(sum);
            }
            // Montgomery constants for the recombination of the two branches.
            const uint32_t mod32 = uint32_t(base::mod()), imod32 = -inv2<uint32_t>(base::mod());
            base r32 = bpow(base(2), 32);
            auto highmul = u32x8{} + uint32_t(((base(2) * base(root)).inv() * r32).getr());
            u64x4 seed_a = seed(), seed_b = seed();
            const lattice L;
            big_vector<base> a_upper(std::begin(a) + std::min(as, n), std::end(a));
            a.resize(2 * n);
            std::span<base const> a_lower = std::span(a).first(std::min(as, n));
            // A square may come with b aliasing the storage that a has just left.
            auto b_lower = square ? a_lower : std::span<base const>(b).first(std::min(bs, n));
            auto b_upper = square ? std::span<base const>(a_upper) : std::span<base const>(b).subspan(b_lower.size());
            cvector A(0), B(0);
            auto* out = reinterpret_cast<uint32_t*>(std::data(a));
            for(bool negative: {false, true}) {
                if(n == (1 << 24)) {
                    if constexpr(cvector::fuse_forward) {
                        cvector::fuse_args fa{out, as, seed_a[0], L.a, L.b, L.a / double(base::mod()), L.b / double(base::mod())};
                        cvector::fuse_args fb{reinterpret_cast<const uint32_t*>(std::data(b)), bs, seed_b[0], fa.a, fa.b, fa.a_over_p, fa.b_over_p};
                        if(negative) {A.template cache_product<true>(B, fa, fb);}
                        else {A.template cache_product<false>(B, fa, fb);}
                    } else {
                        fill<true, true>(A, a_lower, a_upper, n, negative, seed_a);
                        if(square && __uint128_t(need) * d * prime * prime <= (__uint128_t(1) << 88)) {
                            if(negative) {A.template cache_product<true, false, true>(A);}
                            else {A.template cache_product<false, false, true>(A);}
                        } else {
                            fill<true, true>(B, b_lower, b_upper, n, negative, seed_b);
                            if(negative) {A.template cache_product<true>(B);}
                            else {A.template cache_product<false>(B);}
                        }
                    }
                } else {
                    fill<false, true>(A, a_lower, a_upper, n, negative, seed_a);
                    if(!square) {fill<false, true>(B, b_lower, b_upper, n, negative, seed_b);}
                    A.multiply(square ? A : B);
                }
                auto scale = vz + double(flen) / double(n);
                for(size_t i = 0; i < n; i += 8) {
                    auto sum0 = project<true>(A.at(i) * scale, negative, L);
                    auto sum1 = project<true>(A.at(i + 4) * scale, negative, L);
                    auto sum = __builtin_shufflevector(sum0, sum1, 0, 1, 2, 3, 4, 5, 6, 7);
                    if(negative) {
                        u32x8 plus;
                        std::memcpy(&plus, out + n + i, sizeof(plus));
                        auto lo = plus + sum;
                        // Unsigned reduction: these sums pass 2^31 for moduli above 2^30.
                        lo = reduce_once(lo, mod32);
                        lo = (lo + (lo & 1) * base::mod()) >> 1;
                        auto hi = reduce_once(montgomery_mul(plus + base::mod() - sum, highmul, mod32, imod32), mod32);
                        std::memcpy(out + i, &lo, sizeof(lo));
                        std::memcpy(out + n + i, &hi, sizeof(hi));
                    } else {
                        std::memcpy(out + n + i, &sum, sizeof(sum));
                    }
                }
                checkpoint("quadratic recover");
            }
            a.resize(need);
            out = reinterpret_cast<uint32_t*>(std::data(a));
            for(size_t i = 0; i < tail; i++) {
                uint32_t sum = out[i] + high[i];
                out[i] = std::min(sum, sum - prime);
                out[2 * n + i] = high[i];
            }
        }
        // a <- a * b for d > 1, or a <- a * a with square set: i is not in the ring, so two
        // branches could only be recombined as complex numbers, which is slower and less exact
        // than one full transform.
        static void mul_single(auto& a, auto const& b, bool square) {
            size_t as = std::size(a), bs = square ? as : std::size(b), need = as + bs - 1;
            size_t n = length(as, bs), tail = need > n ? need - n : 0;
            assert(available && d > 1);
            const lattice L;
            cvector A(0), B(0);
            std::span<base const> none;
            // Coefficients from n on, in ring coordinates: they come back multiplied by i.
            std::array<point, short_tail> high{};
            auto wrapped = [&](cvector const& rhs) {
                for(size_t i = 0; i < tail; i++) {
                    for(size_t j = n + i - bs + 1; j < as; j++) {
                        high[i] += A.template get<point>(j) * rhs.template get<point>(n + i - j);
                    }
                }
            };
            if(n == (1 << 24)) {
                fill<true, false>(A, a, none, n, false, seed());
                if(square && __uint128_t(need) * d * prime * prime <= (__uint128_t(1) << 88)) {
                    wrapped(A);
                    A.template cache_product<false, false, true>(A);
                } else {
                    if(square) {fill<true, false>(B, a, none, n, false, seed());}
                    else {fill<true, false>(B, b, none, n, false, seed());}
                    wrapped(B);
                    A.template cache_product<false>(B);
                }
            } else {
                fill<false, false>(A, a, none, n, false, seed(), !tail);
                if(!square) {fill<false, false>(B, b, none, n, false, seed(), !tail);}
                if(tail) {
                    wrapped(square ? A : B);
                    A.forward();
                    if(!square) {B.forward();}
                }
                A.multiply(square ? A : B);
            }
            for(size_t i = 0; i < tail; i++) {
                A.set(i, A.template get<point>(i) - point(0, 1) * high[i] * (double(n) / double(flen)));
            }
            a.resize(n);
            auto* out = reinterpret_cast<uint32_t*>(std::data(a));
            auto scale = vz + double(flen) / double(n);
            for(size_t i = 0; i < n; i += flen) {
                auto sum = project<false>(A.at(i) * scale, false, L);
                std::memcpy(out + i, &sum, sizeof(sum));
            }
            checkpoint("quadratic recover");
            a.resize(need);
            out = reinterpret_cast<uint32_t*>(std::data(a));
            for(size_t i = 0; i < tail; i += flen) {
                vpoint lanes = {vz, vz};
                for(size_t j = i; j < std::min(tail, i + flen); j++) {
                    real(lanes)[j - i] = real(high[j]);
                    imag(lanes)[j - i] = imag(high[j]);
                }
                auto sum = project<false>(lanes, false, L);
                for(size_t j = i; j < std::min(tail, i + flen); j++) {out[n + j] = sum[j - i];}
            }
        }
        // Both routines read and write the storage as plain residues, which it is for modint<m>.
        // A runtime-modulus type keeps another form, so its operands are converted on the way.
        static void mul(auto& a, auto const& b, bool square) {
            static_assert(sizeof(std::decay_t<decltype(a[0])>) == 4);
            auto run = [&](auto const& rhs) {
                if constexpr(fixed_d == 1) {mul_branches(a, rhs, square);}
                else if constexpr(fixed_d > 1) {mul_single(a, rhs, square);}
                else if(d == 1) {mul_branches(a, rhs, square);}
                else {mul_single(a, rhs, square);}
            };
            if constexpr(fixed_mod) {run(b);}
            else {
                big_vector<base> plain;
                if(!square) {plain.assign(std::begin(b), std::end(b));}
                for(auto& x: plain) {x.setr_direct(x.getr());}
                for(auto& x: a) {x.setr_direct(x.getr());}
                run(plain);
                for(auto& x: a) {x.setr(x.getr_direct());}
            }
        }
    };

    // A polynomial transformed for repeated multiplication in the ring of quadratic<base>.
    //
    // capacity() is the number of product coefficients the transform represents, and both
    // operands of a product must fit in it. For d = 1 the polynomial is held as two conjugate
    // branches of capacity/2 points, modulo x^n - i and modulo x^n + i, whose moduli multiply
    // to x^(2n) + 1; for larger d as one transform of capacity points modulo x^n - i. Either
    // way a product of at most capacity coefficients does not wrap around.
    //
    // complete selects the layout. The default stops one radix level short, which the fused
    // product of multiply() finishes; the complete transform makes a product pointwise, which
    // is what an accumulated sum of products needs.
    //
    // A transform costs two doubles per coefficient, so the type is move-only and every copy is
    // spelled clone(). multiply() and square() consume the object and only read the other
    // operand, which is what lets one transform serve several products.
    //
    // A transform belongs to the modulus it was built under; a runtime modulus must not change
    // while one is alive.
    template<modint_type base, bool complete = false>
    struct spectrum {
        using ring = quadratic<base>;
        spectrum(auto const& a, size_t capacity): cap(std::max(2 * flen, std::bit_ceil(capacity))) {
            ring::init();
            assert(ring::available && std::size(a) <= cap);
            auto state = ring::seed();
            for(size_t j = 0; j < branches(); j++) {
                if(ring::d == 1) {ring::template lift<true>(parts[j], a, points(), j == 1, state);}
                else {ring::template lift<false>(parts[j], a, points(), false, state);}
                if constexpr(complete) {parts[j].template fft<false>();}
                else {parts[j].forward();}
            }
        }
        spectrum(spectrum&&) = default;
        spectrum& operator=(spectrum&&) = default;
        spectrum clone() const {return *this;}
        size_t capacity() const {return cap;}

        // out[0..k) = *this * other. Consumes *this and only reads other.
        void multiply(spectrum const& other, auto& out, size_t k) && requires(!complete) {
            assert(other.cap == cap && k <= cap);
            for(size_t j = 0; j < branches(); j++) {parts[j].multiply(other.parts[j]);}
            ring::recover(parts, points(), double(flen) / double(points()), out, k);
        }
        // out[0..k) = *this * *this, consuming *this.
        void square(auto& out, size_t k) && requires(!complete) {
            std::move(*this).multiply(*this, out, k);
        }
    private:
        spectrum(spectrum const&) = default;
        size_t branches() const {return ring::d == 1 ? 2 : 1;}
        size_t points() const {return ring::d == 1 ? cap / 2 : cap;}
        template<modint_type> friend struct product;
        std::array<cvector, 2> parts = {cvector(0), cvector(0)};
        size_t cap;
    };

    // A sum of products of transforms, read back once. Every term costs one pointwise pass and
    // the sum one inverse transform, which is what makes it worth keeping the operands around.
    template<modint_type base>
    struct product {
        using ring = quadratic<base>;
        using operand = spectrum<base, true>;
        explicit product(size_t capacity): cap(std::max(2 * flen, std::bit_ceil(capacity))) {
            ring::init();
            assert(ring::available);
            for(size_t j = 0; j < branches(); j++) {acc[j] = cvector(points());}
        }
        // *this += x * y.
        void add(operand const& x, operand const& y) {
            assert(x.cap == cap && y.cap == cap);
            for(size_t j = 0; j < branches(); j++) {
                for(size_t i = 0; i < points(); i += flen) {
                    acc[j].at(i) += x.parts[j].at(i) * y.parts[j].at(i);
                }
            }
            checkpoint("dot");
        }
        // out[0..k) = the accumulated sum, consuming *this.
        void recover(auto& out, size_t k) && {
            assert(k <= cap);
            for(size_t j = 0; j < branches(); j++) {acc[j].template ifft<false>();}
            ring::recover(acc, points(), 1.0, out, k);
        }
    private:
        size_t branches() const {return ring::d == 1 ? 2 : 1;}
        size_t points() const {return ring::d == 1 ? cap / 2 : cap;}
        std::array<cvector, 2> acc = {cvector(0), cvector(0)};
        size_t cap;
    };
}
#pragma GCC pop_options

#line 6 "cp-algo/math/fft.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::fft {
    void mul_slow(auto &a, auto const& b, size_t k) {
        using base = std::decay_t<decltype(a[0])>;
        if(!std::empty(a) && std::data(a) == std::data(b)) {
            size_t n = std::min(k, std::size(a)), m = std::min(k, std::size(b));
            if(!m) {a.clear(); return;}
            a.resize(k);
            // Descending output only reads original coefficients at indices <=j.
            for(size_t j = k; j-- > 0;) {
                base sum = 0;
                size_t lo = j >= n ? j + 1 - n : 0, hi = std::min(j + 1, m);
                for(size_t i = lo; i < hi; i++) {
                    if(n == m && i > j - i) {break;}
                    auto term = a[i] * a[j - i];
                    sum += n == m && i != j - i ? term + term : term;
                }
                a[j] = sum;
            }
            return;
        }
        if(std::empty(a) || std::empty(b)) {
            a.clear();
        } else {
            size_t n = std::min(k, std::size(a));
            size_t m = std::min(k, std::size(b));
            size_t had = std::size(a);
            a.resize(k);
            // The loop below reads every coefficient it writes, so the growth is zeroed here
            // rather than relying on the caller's allocator to do it.
            if(k > had) {std::fill(std::begin(a) + had, std::end(a), base(0));}
            for(int j = int(k - 1); j >= 0; j--) {
                a[j] *= b[0];
                for(int i = std::max(j - (int)n, 0) + 1; i < std::min(j + 1, (int)m); i++) {
                    a[j] += a[j - i] * b[i];
                }
            }
        }
    }
    // Whether two ranges are backed by the same storage, which makes a product a square.
    bool same_storage(auto const& a, auto const& b) {
        if constexpr(std::ranges::contiguous_range<decltype(b)>) {
            return !std::empty(a) && (void const*)std::data(a) == (void const*)std::data(b);
        } else {
            return false;
        }
    }
    // The product of the truncated operands, keeping k coefficients. Short operands are cheaper
    // to multiply naively; every other product goes through the ring.
    void mul_truncate(auto &a, auto const& b, size_t k) {
        using base = std::decay_t<decltype(a[0])>;
        if(std::min({k, std::size(a), std::size(b)}) < magic) {
            mul_slow(a, b, k);
            return;
        }
        size_t as = std::min(k, std::size(a)), bs = std::min(k, std::size(b));
        assert(quadratic<base>::usable(as, bs) && "the ring needs an odd prime modulus below 2^31 and a product within its rounding bound");
        bool aliased = same_storage(a, b), square = aliased && as == bs;
        // Everything past the true length of the product is zero, written here rather than
        // left to the caller's allocator.
        size_t need = as + bs - 1, keep = std::min(k, need);
        auto pad = [&] {if(k > need) {std::fill(std::begin(a) + need, std::end(a), base(0));}};
        if constexpr(sizeof(base) == 4 && std::ranges::contiguous_range<decltype(b)>) {
            if(aliased && !square) {
                big_vector<base> copy(std::data(b), std::data(b) + bs);
                a.resize(as);
                quadratic<base>::mul(a, copy, false);
            } else {
                auto prefix = std::span<base const>(std::data(b), bs);
                a.resize(as);
                quadratic<base>::mul(a, prefix, square);
            }
            a.resize(k);
            pad();
        } else {
            // Wider storage or a non-contiguous operand: the reusable transform reads and writes
            // through the modint interface instead of the raw residues.
            size_t cap = std::bit_ceil(need);
            auto A = spectrum<base>(a | std::views::take(as), cap);
            a.resize(k);
            if(square) {std::move(A).square(a, keep);}
            else {std::move(A).multiply(spectrum<base>(b | std::views::take(bs), cap), a, keep);}
            pad();
        }
    }
    // Cyclic product modulo x^k - 1, in place over a.
    void cyclic_mul(auto &a, auto const& b, size_t k) {
        using base = std::decay_t<decltype(a[0])>;
        quadratic<base>::cyclic(a, b, k);
    }
    namespace impl {
        // Overlap-add for a short fixed operand; every block reuses its transform.
        void mul_unbalanced(auto &a, auto const& b) {
            using base = std::decay_t<decltype(a[0])>;
            auto x = std::span<base const>(a), y = std::span<base const>(b);
            if(x.size() < y.size()) {std::swap(x, y);}
            constexpr size_t length = 1 << 15;
            size_t step = length - y.size() + 1;
            auto fixed = spectrum<base>(y, length);
            std::decay_t<decltype(a)> result;
            // Accumulated into, so the zeros are written rather than assumed.
            result.assign(x.size() + y.size() - 1, base(0));
            big_vector<base> work(length);
            for(size_t start = 0; start < x.size(); start += step) {
                size_t count = std::min(step, x.size() - start);
                size_t need = count + y.size() - 1;
                spectrum<base>(x.subspan(start, count), length).multiply(fixed, work, need);
                for(size_t i = 0; i < need; i++) {result[start + i] += work[i];}
            }
            a = std::move(result);
        }
    }
    void mul(auto &a, auto const& b) {
        if(std::empty(a) || std::empty(b)) {a.clear(); return;}
        size_t small = std::min(std::size(a), std::size(b));
        size_t large = std::max(std::size(a), std::size(b));
        if(small >= magic && small <= 4096 && large >= (1 << 20) && large / small >= 64) {
            return impl::mul_unbalanced(a, b);
        }
        mul_truncate(a, b, std::size(a) + std::size(b) - 1);
    }
}
#pragma GCC pop_options

#line 3 ".verify-helper/cache/fft-shallow-roots-20260922-nnenmcu8/fused/phasevector.hpp"
namespace cp_algo::math::fft {
    template<int Phase> struct phasevector {
        spectrum_vector r;
        phasevector(size_t n) {
            n = std::max(flen, std::bit_ceil(n));
            r.assign(n / flen, vpoint{});
            prepare_roots(n / 16);
            checkpoint("phasevector create");
        }

        vpoint& at(size_t k) {return r[k / flen];}
        vpoint at(size_t k) const {return r[k / flen];}
        template<class pt = point>
        inline void set(size_t k, pt const& t) {
            if constexpr(std::is_same_v<pt, point>) {
                real(r[k / flen])[k % flen] = real(t);
                imag(r[k / flen])[k % flen] = imag(t);
            } else {
                at(k) = t;
            }
        }
        template<class pt = point>
        inline pt get(size_t k) const {
            if constexpr(std::is_same_v<pt, point>) {
                return {real(r[k / flen])[k % flen], imag(r[k / flen])[k % flen]};
            } else {
                return at(k);
            }
        }

        size_t size() const {
            return flen * r.size();
        }
        static constexpr size_t eval_arg(size_t n) {
            if(n < pre_evals) {
                return eval_args[n];
            } else {
                return eval_arg(n / 2) | (n & 1) << (std::bit_width(n) - 1);
            }
        }
        static constexpr point eval_point(size_t n) {
            if(n % 2) {
                return -eval_point(n - 1);
            } else if(n % 4) {
                return eval_point(n - 2) * point(0, 1);
            } else if(n / 4 < pre_evals) {
                return evalp[n / 4];
            } else if(n / 4 - pre_evals < extra.size()) {
                return extra[n / 4 - pre_evals];
            } else {
                return polar<ftype>(1., std::numbers::pi / (ftype)std::bit_floor(n) * (ftype)eval_arg(n));
            }
        }
        static constexpr std::array<point, 32> roots = []() {
            std::array<point, 32> res;
            for(size_t i = 2; i < 32; i++) {
                res[i] = polar<ftype>(1., std::numbers::pi * (double(Phase) / 2) / (1ull << (i - 2)));
            }
            return res;
        }();
        static constexpr point root(size_t n) {
            return roots[std::bit_width(n)];
        }
        template<int step>
        static void exec_on_eval(size_t n, size_t k, auto &&callback) {
            callback(k, root(4 * step * n) * eval_point(step * k));
        }
        template<int step>
        static void exec_on_evals(size_t n, auto &&callback) {
            point factor=root(4*step*n);
            if constexpr(step==1 || step==2 || step==4){
                prepare_roots((step*n+3)/4);
                size_t i=0;
                if constexpr(step==1){
                    for(;i+4<=n;i+=4){
                        size_t k=i/4;
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                        point v=factor*e;
                        callback(i,v);callback(i+1,-v);
                        point iv(-imag(v),real(v));
                        callback(i+2,iv);callback(i+3,-iv);
                    }
                }else if constexpr(step==2){
                    for(;i+2<=n;i+=2){
                        size_t k=i/2;
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                        point v=factor*e;
                        callback(i,v);callback(i+1,point(-imag(v),real(v)));
                    }
                }
                for(;i<n;i++){
                    size_t index=step*i,k=index/4;
                    point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                    if(index&2)e=e*point(0,1);
                    if(index&1)e=-e;
                    callback(i,factor*e);
                }
            }else{
                for(size_t i=0;i<n;i++)callback(i,factor*eval_point(step*i));
            }
        }

        static void do_dot_iter(point rt, vpoint& Bv, vpoint const& Av, vpoint& res) {
            res += Av * Bv;
            real(Bv) = rotate_right(real(Bv));
            imag(Bv) = rotate_right(imag(Bv));
            auto x = real(Bv)[0], y = imag(Bv)[0];
            real(Bv)[0] = x * real(rt) - y * imag(rt);
            imag(Bv)[0] = x * imag(rt) + y * real(rt);
        }

        template<size_t fixed = 0>
        void dot(phasevector const& t) {
            size_t n = fixed?fixed:this->size();
            exec_on_evals<1>(n / flen, [&](size_t k, point rt) __attribute__((always_inline)) {
                k *= flen;
                auto [Ax, Ay] = at(k);
                auto Cv = t.at(k);
                vpoint vrt = {vz + real(rt), vz + imag(rt)};
                auto Cr = Cv * vrt;
                vpoint res = vz;
                auto iter = [&]<int i>() __attribute__((always_inline)) {
                    auto wrap = [&](vftype original, vftype rotated) {
                        if constexpr(i == 0) {return original;}
                        else {return __builtin_shufflevector(rotated, original, 4-i,5-i,6-i,7-i);}
                    };
                    vpoint Cw = {wrap(real(Cv),real(Cr)),wrap(imag(Cv),imag(Cr))};
                    vpoint Av = {vz+Ax[i],vz+Ay[i]};
                    return Av*Cw;
                };
                auto p0=iter.template operator()<0>(), p1=iter.template operator()<1>();
                auto p2=iter.template operator()<2>(), p3=iter.template operator()<3>();
                res=(p0+p1)+(p2+p3);
                set(k, res);
            });
            checkpoint("dot");
        }
        // normalize=false leaves the inverse-transform scale for the caller.
        template<bool partial = true, bool normalize = true, size_t fixed = 0>
        void ifft() {
            size_t n = fixed?fixed:size();
            if constexpr (!partial) {
                prepare_roots(n / 4);
                point pi(0, 1);
                exec_on_evals<4>(n / 4, [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 4;
                    point v1 = conj(rt);
                    point v2 = v1 * v1;
                    point v3 = v1 * v2;
                    auto A = get(k);
                    auto B = get(k + 1);
                    auto C = get(k + 2);
                    auto D = get(k + 3);
                    set(k, (A + B) + (C + D));
                    set(k + 2, ((A + B) - (C + D)) * v2);
                    set(k + 1, ((A - B) - pi * (C - D)) * v1);
                    set(k + 3, ((A - B) + pi * (C - D)) * v3);
                });
            }
            bool parity = std::countr_zero(n) % 2;
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 2 * flen;
                    vpoint cvrt = {vz + real(rt), vz - imag(rt)};
                    auto B = at(k) - at(k + flen);
                    at(k) += at(k + flen);
                    at(k + flen) = B * cvrt;
                });
            }

            transform<true,fixed>(n, parity);
            checkpoint("ifft");
            if constexpr(normalize) {
                auto scale = vz + ftype(partial ? flen : 1) / ftype(n);
                for(size_t k = 0; k < n; k += flen) {
                    set(k, get<vpoint>(k) * scale);
                }
            }
        }
        template<bool partial = true, size_t fixed = 0>
        void fft() {
            size_t n = fixed?fixed:size();
            prepare_roots(n / (partial ? 16 : 4));
            bool parity = std::countr_zero(n) % 2;
            transform<false,fixed>(n, parity);
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 2 * flen;
                    vpoint vrt = {vz + real(rt), vz + imag(rt)};
                    auto t = at(k + flen) * vrt;
                    at(k + flen) = at(k) - t;
                    at(k) += t;
                });
            }
            if constexpr (!partial) {
                prepare_roots(n / 4);
                point pi(0, 1);
                exec_on_evals<4>(n / 4, [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 4;
                    point v1 = rt;
                    point v2 = v1 * v1;
                    point v3 = v1 * v2;
                    auto A = get(k);
                    auto B = get(k + 1) * v1;
                    auto C = get(k + 2) * v2;
                    auto D = get(k + 3) * v3;
                    set(k, (A + C) + (B + D));
                    set(k + 1, (A + C) - (B + D));
                    set(k + 2, (A - C) + pi * (B - D));
                    set(k + 3, (A - C) - pi * (B - D));
                });
            }
            checkpoint("fft");
        }
        static std::array<vpoint,4> transpose(std::array<vpoint,4> const&a){
            auto half=[](auto part){
                auto a0=__m256d(part(0)),a1=__m256d(part(1)),a2=__m256d(part(2)),a3=__m256d(part(3));
                auto t0=_mm256_unpacklo_pd(a0,a1),t1=_mm256_unpackhi_pd(a0,a1),t2=_mm256_unpacklo_pd(a2,a3),t3=_mm256_unpackhi_pd(a2,a3);
                return std::array<vftype,4>{vftype(_mm256_permute2f128_pd(t0,t2,0x20)),vftype(_mm256_permute2f128_pd(t1,t3,0x20)),vftype(_mm256_permute2f128_pd(t0,t2,0x31)),vftype(_mm256_permute2f128_pd(t1,t3,0x31))};
            };
            auto re=half([&](int k){return real(a[k]);}),im=half([&](int k){return imag(a[k]);});
            return {vpoint{re[0],im[0]},vpoint{re[1],im[1]},vpoint{re[2],im[2]},vpoint{re[3],im[3]}};
        }
        // Multiply groups of 16 points as polynomials modulo x^16 - rt: one radix-4 level on
        // each operand, a lane-parallel 4-coefficient Karatsuba product, one inverse level.
        // The weights rt, rt^2, rt^3 of a tile of groups are computed four groups per SIMD
        // operation ahead of the tile, so the product loop only broadcasts them from memory.
        static constexpr size_t dot_tile = 64;
        template<size_t fixed, bool square = false>void dot_fused16(phasevector const& t,size_t offset,size_t length){
            constexpr size_t T=dot_tile;
            const size_t n=fixed?fixed:size();
            point factor=root(n);
            auto fr=_mm256_set1_pd(real(factor)),fi=_mm256_set1_pd(imag(factor));
            alignas(32) double tw[6][T];
            const bool ahead=offset+length<n;
            auto cmadd=[](vpoint x,vpoint y,vpoint c) __attribute__((always_inline)) {
                auto re=_mm256_fmadd_pd(__m256d(real(x)),__m256d(real(y)),_mm256_fnmadd_pd(__m256d(imag(x)),__m256d(imag(y)),__m256d(real(c))));
                auto im=_mm256_fmadd_pd(__m256d(real(x)),__m256d(imag(y)),_mm256_fmadd_pd(__m256d(imag(x)),__m256d(real(y)),__m256d(imag(c))));
                return vpoint{vftype(re),vftype(im)};
            };
            auto cmcross=[](vpoint x,vpoint y,vpoint p,vpoint q) __attribute__((always_inline)) {
                auto re=_mm256_sub_pd(_mm256_fmsub_pd(__m256d(real(x)),__m256d(real(y)),__m256d(real(p))),_mm256_fmadd_pd(__m256d(imag(x)),__m256d(imag(y)),__m256d(real(q))));
                auto im=_mm256_add_pd(_mm256_fmsub_pd(__m256d(real(x)),__m256d(imag(y)),__m256d(imag(p))),_mm256_fmsub_pd(__m256d(imag(x)),__m256d(real(y)),__m256d(imag(q))));
                return vpoint{vftype(re),vftype(im)};
            };
            // z * conj(v)
            auto mulconj=[](vpoint z,vpoint v) __attribute__((always_inline)) {
                auto re=_mm256_fmadd_pd(__m256d(real(z)),__m256d(real(v)),_mm256_mul_pd(__m256d(imag(z)),__m256d(imag(v))));
                auto im=_mm256_fmsub_pd(__m256d(imag(z)),__m256d(real(v)),_mm256_mul_pd(__m256d(real(z)),__m256d(imag(v))));
                return vpoint{vftype(re),vftype(im)};
            };
            for(size_t tile=offset;tile<offset+length;tile+=16*T){
                size_t k0=tile/16;
                auto const* e=reinterpret_cast<double const*>(k0<pre_evals?evalp.data()+k0:extra.data()+(k0-pre_evals));
                for(size_t g=0;g<T;g+=4){
                    auto x=_mm256_loadu_pd(e+2*g),y=_mm256_loadu_pd(e+2*g+4);
                    auto er=_mm256_permute4x64_pd(_mm256_unpacklo_pd(x,y),0xD8),ei=_mm256_permute4x64_pd(_mm256_unpackhi_pd(x,y),0xD8);
                    auto r1=_mm256_fmsub_pd(fr,er,_mm256_mul_pd(fi,ei)),i1=_mm256_fmadd_pd(fr,ei,_mm256_mul_pd(fi,er));
                    auto r2=_mm256_fmsub_pd(r1,r1,_mm256_mul_pd(i1,i1)),i2=_mm256_fmadd_pd(r1,i1,_mm256_mul_pd(i1,r1));
                    auto r3=_mm256_fmsub_pd(r1,r2,_mm256_mul_pd(i1,i2)),i3=_mm256_fmadd_pd(r1,i2,_mm256_mul_pd(i1,r2));
                    _mm256_store_pd(tw[0]+g,r1);_mm256_store_pd(tw[1]+g,i1);_mm256_store_pd(tw[2]+g,r2);
                    _mm256_store_pd(tw[3]+g,i2);_mm256_store_pd(tw[4]+g,r3);_mm256_store_pd(tw[5]+g,i3);
                }
                for(size_t g=0;g<T;g++){
                    size_t pos=tile+16*g;
                    if(ahead){
                        // Pull the next block of both operands towards the cache while this one is multiplied.
                        auto const* pa=reinterpret_cast<char const*>(r.data()+(pos+length)/flen);
                        auto const* pb=reinterpret_cast<char const*>(t.r.data()+(pos+length)/flen);
                        for(size_t c=0;c<4;c++){_mm_prefetch(pa+64*c,_MM_HINT_T2);_mm_prefetch(pb+64*c,_MM_HINT_T2);}
                    }
                    auto bc=[&](size_t c) __attribute__((always_inline)) {return vftype(_mm256_broadcast_sd(tw[c]+g));};
                    vpoint v1={bc(0),bc(1)},v2={bc(2),bc(3)},v3={bc(4),bc(5)};
                    auto forward=[&](phasevector const& a) __attribute__((always_inline)) {
                        auto A=a.at(pos),B=a.at(pos+4)*v1,C=a.at(pos+8)*v2,D=a.at(pos+12)*v3;
                        return std::array<vpoint,4>{(A+C)+(B+D),(A+C)-(B+D),(A-C)+vi(B-D),(A-C)-vi(B-D)};
                    };
                    auto a=transpose(forward(*this));
                    auto b=[&] {if constexpr(square) return a; else return transpose(forward(t));}();
                    // The four residues are taken modulo x^4 - rt * {1, -1, i, -i}.
                    const auto flip_re=_mm256_set_pd(0.,-0.,-0.,0.),flip_im=_mm256_set_pd(-0.,0.,-0.,0.);
                    vpoint w={vftype(_mm256_xor_pd(_mm256_blend_pd(__m256d(real(v1)),__m256d(imag(v1)),0b1100),flip_re)),
                              vftype(_mm256_xor_pd(_mm256_blend_pd(__m256d(imag(v1)),__m256d(real(v1)),0b1100),flip_im))};
                    auto mul=[&](vpoint a0,vpoint a1,vpoint b0,vpoint b1) __attribute__((always_inline)) {
                        auto p=a0*b0,q=a1*b1;
                        return std::array<vpoint,2>{cmadd(w,q,p),cmcross(a0+a1,b0+b1,p,q)};
                    };
                    auto p=mul(a[0],a[2],b[0],b[2]),q=mul(a[1],a[3],b[1],b[3]);
                    auto m=mul(a[0]+a[1],a[2]+a[3],b[0]+b[1],b[2]+b[3]);
                    auto c=transpose({cmadd(w,q[1],p[0]),(m[0]-p[0])-q[0],p[1]+q[0],(m[1]-p[1])-q[1]});
                    auto A=c[0],B=c[1],C=c[2],D=c[3];
                    at(pos)=(A+B)+(C+D);at(pos+8)=mulconj((A+B)-(C+D),v2);
                    at(pos+4)=mulconj((A-B)-vi(C-D),v1);at(pos+12)=mulconj((A-B)+vi(C-D),v3);
                }
            }
        }
        // A product in two steps, this <- this * t modulo x^n - i up to the factor n / flen:
        // forward() on both operands, then multiply(t), which may be *this for a square.
        // The forward transform stops at groups of 16 points and dot_fused16 multiplies those,
        // which saves a pass over each operand and one over the result. That needs powers of
        // four, so a length 2 * 4^k takes its radix-two level at the top, modulo
        // x^(n/2) -+ sqrt(i); short lengths take the complete transform and the 4-point product.
        void forward() {
            if(!fused_leaves()) {return fft();}
            size_t n = size(), part = fused_part();
            if(part < n) {
                vpoint vrt = {vz + real(roots[4]), vz + imag(roots[4])};
                for(size_t k = 0; k < part; k += flen) {
                    auto t = at(k + part) * vrt;
                    at(k + part) = at(k) - t;
                    at(k) += t;
                }
            }
            for(size_t offset = 0; offset < n; offset += part) {
                transform<false, 0, 0, -1, true>(n, false, offset, part);
            }
            checkpoint("fft");
        }
        void multiply(phasevector const& t) {
            if(!fused_leaves()) {dot(t); return ifft<true, false>();}
            size_t n = size(), part = fused_part();
            dot_fused16<0>(t, 0, n);
            checkpoint("dot");
            for(size_t offset = 0; offset < n; offset += part) {
                transform<true, 0, 0, -1, true>(n, false, offset, part);
            }
            if(part < n) {
                vpoint cvrt = {vz + real(roots[4]), vz - imag(roots[4])};
                for(size_t k = 0; k < part; k += flen) {
                    auto t = at(k) - at(k + part);
                    at(k) += at(k + part);
                    at(k + part) = t * cvrt;
                }
            }
            checkpoint("ifft");
        }
        // Radix-64 out-of-cache pass for n = 2^24, run as two tiled radix-8 stages.
        // With Fuse=1 (forward only) the first stage lifts u32 residues to Gaussian
        // coordinates on load, so the spectrum buffer is written once and never re-read
        // before the in-cache block phase. The index-keyed noise makes both branches see
        // the same representatives.
        struct fuse_args {
            const uint32_t* src = nullptr;
            size_t count = 0;
            uint64_t seed = 0;
            double a = 0, b = 0, a_over_p = 0, b_over_p = 0;
        };
        static constexpr size_t sweep_tile = 256;
        template<bool inverse, int Mode, size_t Tile, int Fuse = 0, bool Neg = false>
        void sweep8(fuse_args const& fa = {}) {
            constexpr size_t n = 1 << 24;
            static const std::array<std::array<point, 7>, 9> weights = []() {
                std::array<std::array<point, 7>, 9> table;
                for(size_t count: {size_t(1), size_t(8)}) for(size_t k = 0; k < count; k++) {
                    size_t rev = 0, x = k;
                    for(size_t c = count; c > 1; c >>= 1) {rev = (rev << 1) | (x & 1); x >>= 1;}
                    long double angle = std::numbers::pi_v<long double> * (1 + 4 * rev) / (16 * count);
                    if(k & 1) {angle -= std::numbers::pi_v<long double> / 4;}
                    for(size_t j = 1; j < 8; j++) {
                        long double a = j * angle;
                        if constexpr(Mode == 0) {table[(count - 1) / 7 + k][j - 1] = {double(cosl(a)), double(sinl(a))};}
                        else {table[(count - 1) / 7 + k][j - 1] = {double(sinl(a)), double(tanl(a / 2))};}
                    }
                }
                return table;
            }();
            auto lift = [&](size_t idx) __attribute__((always_inline)) -> vpoint {
                i32x4 bits{};
                if(idx + 4 <= fa.count) {std::memcpy(&bits, fa.src + idx, sizeof(bits));}
                else if(idx < fa.count) {for(size_t j = 0; j < fa.count - idx; j++) {bits[j] = int32_t(fa.src[idx + j]);}}
                else {return vpoint{vz, vz};}
                auto x = __builtin_convertvector(bits, vftype);
                u32x4 h = u32x4{uint32_t(idx), uint32_t(idx + 1), uint32_t(idx + 2), uint32_t(idx + 3)} ^ uint32_t(fa.seed);
                h *= 0x9E3779B1u; h ^= h >> 15; h *= 0x85EBCA77u; h ^= h >> 13; h *= 0xC2B2AE3Du; h ^= h >> 16;
                auto noise = __builtin_convertvector(i32x4(h), vftype) * 0x1p-32;
                auto q = round(x * fa.a_over_p + noise), t = round(x * fa.b_over_p + noise);
                auto re = x - q * fa.a - t * fa.b, im = t * fa.a - q * fa.b;
                return vpoint{re, Neg ? -im : im};
            };
            auto stage = [&]<bool top>(size_t offset, size_t length, size_t begin, size_t end) __attribute__((always_inline)) {
                size_t step = length / 8, k = offset / length;
                auto const& w = weights[(n / length - 1) / 7 + k];
                auto rot = [&]<size_t J>(vpoint z) __attribute__((always_inline)) {
                    auto c = w[J - 1];
                    if constexpr(Mode == 0) {return z * vpoint{vz + real(c), inverse ? vz - imag(c) : vz + imag(c)};}
                    else {
                        auto s = inverse ? vz - real(c) : vz + real(c), t = inverse ? vz - imag(c) : vz + imag(c);
                        auto x = vftype(_mm256_fnmadd_pd(__m256d(t), __m256d(imag(z)), __m256d(real(z))));
                        auto y = vftype(_mm256_fmadd_pd(__m256d(s), __m256d(x), __m256d(imag(z))));
                        return vpoint{vftype(_mm256_fnmadd_pd(__m256d(t), __m256d(y), __m256d(x))), y};
                    }
                };
                auto add = [](vpoint a, vpoint b) __attribute__((always_inline)) {
                    if constexpr(Mode != 2) {return a + b;}
                    else {return vpoint{vftype(_mm256_fmadd_pd(__m256d(real(a)), _mm256_set1_pd(1), __m256d(real(b)))), vftype(_mm256_fmadd_pd(__m256d(imag(a)), _mm256_set1_pd(1), __m256d(imag(b))))};}
                };
                auto sub = [](vpoint a, vpoint b) __attribute__((always_inline)) {
                    if constexpr(Mode != 2) {return a - b;}
                    else {return vpoint{vftype(_mm256_fmsub_pd(__m256d(real(a)), _mm256_set1_pd(1), __m256d(real(b)))), vftype(_mm256_fmsub_pd(__m256d(imag(a)), _mm256_set1_pd(1), __m256d(imag(b))))};}
                };
                auto d4 = [](vpoint a, vpoint b, vpoint c, vpoint d) __attribute__((always_inline)) {
                    auto s = a + c, t = a - c, u = b + d, v = vi(b - d);
                    if constexpr(inverse) {return std::array<vpoint, 4>{s + u, t - v, s - u, t + v};}
                    else {return std::array<vpoint, 4>{s + u, t + v, s - u, t - v};}
                };
                constexpr double q = 0.707106781186547524400844362104849039;
                auto r1 = [](vpoint z) __attribute__((always_inline)) {
                    if constexpr(inverse) {return vpoint{(real(z) + imag(z)) * q, (imag(z) - real(z)) * q};}
                    else {return vpoint{(real(z) - imag(z)) * q, (real(z) + imag(z)) * q};}
                };
                auto r3 = [](vpoint z) __attribute__((always_inline)) {
                    if constexpr(inverse) {return vpoint{(imag(z) - real(z)) * q, (-imag(z) - real(z)) * q};}
                    else {return vpoint{(-real(z) - imag(z)) * q, (real(z) - imag(z)) * q};}
                };
                std::array<vpoint*, 8> input, output;
                for(size_t j = 0; j < 8; j++) {input[j] = output[j] = r.data() + (offset + begin + j * step) / flen;}
                if(k & 1) {
                    constexpr std::array<size_t, 8> perm = {7, 6, 4, 5, 0, 1, 2, 3};
                    for(size_t j = 0; j < 8; j++) {
                        if constexpr(inverse) {input[j] = output[perm[j]];}
                        else {output[j] = input[perm[j]];}
                    }
                }
                constexpr bool fused_in = top && Fuse == 1 && !inverse;
                for(size_t j = 0; j < (end - begin) / flen; j++) {
                    size_t base = offset + begin + j * flen;
                    auto in = [&]<size_t S>() __attribute__((always_inline)) {
                        if constexpr(fused_in) {return lift(base + S * step);}
                        else {return input[S][j];}
                    };
                    if constexpr(!inverse) {
                        auto E = d4(in.template operator()<0>(), rot.template operator()<2>(in.template operator()<2>()), rot.template operator()<4>(in.template operator()<4>()), rot.template operator()<6>(in.template operator()<6>()));
                        auto O = d4(rot.template operator()<1>(in.template operator()<1>()), rot.template operator()<3>(in.template operator()<3>()), rot.template operator()<5>(in.template operator()<5>()), rot.template operator()<7>(in.template operator()<7>()));
                        auto a = O[0], b = r1(O[1]), c = vi(O[2]), d = r3(O[3]);
                        output[0][j] = add(E[0], a); output[1][j] = sub(E[0], a);
                        output[2][j] = add(E[2], c); output[3][j] = sub(E[2], c);
                        output[4][j] = add(E[1], b); output[5][j] = sub(E[1], b);
                        output[6][j] = add(E[3], d); output[7][j] = sub(E[3], d);
                    } else {
                        auto E = d4(add(input[0][j], input[1][j]), add(input[4][j], input[5][j]), add(input[2][j], input[3][j]), add(input[6][j], input[7][j]));
                        auto O = d4(sub(input[0][j], input[1][j]), r1(sub(input[4][j], input[5][j])), -vi(sub(input[2][j], input[3][j])), r3(sub(input[6][j], input[7][j])));
                        output[0][j] = E[0]; output[1][j] = rot.template operator()<1>(O[0]);
                        output[2][j] = rot.template operator()<2>(E[1]); output[3][j] = rot.template operator()<3>(O[1]);
                        output[4][j] = rot.template operator()<4>(E[2]); output[5][j] = rot.template operator()<5>(O[2]);
                        output[6][j] = rot.template operator()<6>(E[3]); output[7][j] = rot.template operator()<7>(O[3]);
                    }
                }
            };
            constexpr size_t h = n / 64;
            for(size_t j = 0; j < h; j += Tile) {
                size_t end = std::min(h, j + Tile);
                auto first = [&]() {for(size_t t = 0; t < 8; t++) {stage.template operator()<true>(0, n, j + t * h, end + t * h);}};
                auto second = [&]() {for(size_t k = 0; k < 8; k++) {stage.template operator()<false>(k * n / 8, n / 8, j, end);}};
                if constexpr(inverse) {second(); first();} else {first(); second();}
            }
        }
        // Product for n = 2^24: the top three stages split each input into 64 independent
        // blocks. Both forward transforms read the u32 inputs directly; each block then
        // completes its two forward transforms, the product, and the inverse before the
        // final three inverse stages combine the results.
        // Fusing the Gaussian lift into the first pass saves a write and a read of each
        // spectrum, but makes that pass compute-bound on the judge (measured slower there),
        // so it stays opt-in; by default both spectra are filled first and swept in place.
        static constexpr bool fuse_forward = false;
        template<bool Neg, bool Fused = fuse_forward, bool Square = false>
        void cache_product(phasevector& b, fuse_args const& fa = {}, fuse_args const& fb = {}) {
            constexpr size_t n = 1 << 24, block = 1 << 18;
            prepare_roots(n / 16); prepare_shear_roots();
            if constexpr(Fused) {
                sweep8<false, 2, sweep_tile, 1, Neg>(fa);
                if constexpr(!Square) {b.sweep8<false, 2, sweep_tile, 1, Neg>(fb);}
            } else {
                sweep8<false, 2, sweep_tile>();
                if constexpr(!Square) {b.sweep8<false, 2, sweep_tile>();}
            }
            checkpoint("sweep forward");
            for(size_t offset = 0; offset < n; offset += block) {
                transform<false, n, block, 0, true>(n, false, offset, block);
                if constexpr(!Square) {b.transform<false, n, block, 0, true>(n, false, offset, block);}
                dot_fused16<n, Square>(b, offset, block);
                transform<true, n, block, 0, true>(n, false, offset, block);
            }
            checkpoint("blocks");
            sweep8<true, 2, sweep_tile>();
            checkpoint("sweep inverse");
        }
        static constexpr size_t pre_evals = 1 << 16;
        static inline auto const& eval_args = cvector::eval_args;
        static inline auto const& evalp = cvector::evalp;
        // Experimental complete product: keep both forwards and the inverse of each
        // cache-sized block together, including for non-i branch phases.
        void product_blocks(phasevector& b) {
            size_t n=size(),part=fused_part(),block=part/16;
            prepare_roots(n/16);
            if(part<n) {
                vpoint rt={vz+real(roots[4]),vz+imag(roots[4])};
                for(size_t j=0;j<part;j+=flen) {
                    auto a1=at(j+part)*rt,b1=b.at(j+part)*rt;
                    at(j+part)=at(j)-a1;at(j)+=a1;
                    b.at(j+part)=b.at(j)-b1;b.at(j)+=b1;
                }
            }
            for(size_t offset=0;offset<n;offset+=part) {
                transform<false,0,0,2,true>(n,false,offset,part);
                b.template transform<false,0,0,2,true>(n,false,offset,part);
                for(size_t pos=offset;pos<offset+part;pos+=block) {
                    transform<false,0,0,0,true>(n,false,pos,block);
                    b.template transform<false,0,0,0,true>(n,false,pos,block);
                    dot_fused16<0>(b,pos,block);
                    transform<true,0,0,0,true>(n,false,pos,block);
                }
                transform<true,0,0,2,true>(n,false,offset,part);
            }
            if(part<n) {
                vpoint rt={vz+real(roots[4]),vz-imag(roots[4])};
                for(size_t j=0;j<part;j+=flen) {
                    auto high=at(j)-at(j+part);at(j)+=at(j+part);at(j+part)=high*rt;
                }
            }
        }
    private:
        // The power of four that the fused product works on: all of n = 4^k, half of n = 2 * 4^k.
        size_t fused_part() const {
            return size() >> (std::countr_zero(size()) % 2);
        }
        bool fused_leaves() const {
            return fused_part() >= 16 * dot_tile;
        }
        // Tile two radix-four stages together before descending into each child.
        template<bool inverse, size_t fixed = 0, size_t range_fixed=0, int top_fixed=-1,bool omit16=false>
        void transform(size_t input_n, bool parity, size_t range_offset=0, size_t range_length=0, int top_only=0) {
            if constexpr(range_fixed)range_length=range_fixed;
            if constexpr(top_fixed>=0)top_only=top_fixed;
            const size_t n=fixed?fixed:input_n;
            if constexpr(!range_fixed){prepare_roots(n/16);
            if constexpr(fixed==(1<<24))prepare_shear_roots();}
            size_t log_n=std::countr_zero(n);
            auto butterfly = [&](size_t offset,size_t length,size_t begin,size_t end) __attribute__((always_inline)) {
                if constexpr(omit16)if(length==16)return;
                size_t step=length/4,log_length=std::countr_zero(length),k=offset>>log_length;
                auto *p0=r.data()+(offset+begin)/flen,*p1=r.data()+(offset+begin+step)/flen,*p2=r.data()+(offset+begin+2*step)/flen,*p3=r.data()+(offset+begin+3*step)/flen;
                auto run=[&]<bool shear>() __attribute__((always_inline)) {
                    vpoint v1,v2,v3;vftype t1{},t2{},t3{};
                    if constexpr(shear && fixed==(1<<24)) {
                        auto const& c=shear_roots[((n>>log_length)-1)/3+k];
                        v1={vz,inverse?vz-real(c[0]):vz+real(c[0])};
                        v2={vz,inverse?vz-real(c[1]):vz+real(c[1])};
                        v3={vz,inverse?vz-real(c[2]):vz+real(c[2])};
                        t1=inverse?vz-imag(c[0]):vz+imag(c[0]);
                        t2=inverse?vz-imag(c[1]):vz+imag(c[1]);
                        t3=inverse?vz-imag(c[2]):vz+imag(c[2]);
                    }else {
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];point rt=roots[log_n+5-log_length]*e;
                        v1={vz+real(rt),inverse?vz-imag(rt):vz+imag(rt)};
                        if constexpr(shear)if(k&1){if constexpr(inverse)v1=vi(v1);else v1=-vi(v1);}
                        v2=v1*v1;v3=v1*v2;
                        if constexpr(shear){t1=imag(v1)/(vz+1.0+real(v1));t2=imag(v2)/(vz+1.0+real(v2));t3=imag(v3)/(vz+1.0+real(v3));}
                    }
                    auto rotate=[](vpoint z,vpoint v,vftype t) __attribute__((always_inline)) {
                        if constexpr(!shear)return z*v;
                        else {
                            auto x=vftype(_mm256_fnmadd_pd(__m256d(t),__m256d(imag(z)),__m256d(real(z))));
                            auto y=vftype(_mm256_fmadd_pd(__m256d(imag(v)),__m256d(x),__m256d(imag(z))));
                            return vpoint{vftype(_mm256_fnmadd_pd(__m256d(t),__m256d(y),__m256d(x))),y};
                        }
                    };
                    auto *i0=p0,*i1=p1,*i2=p2,*i3=p3,*o0=p0,*o1=p1,*o2=p2,*o3=p3;
                    if constexpr(shear)if(k&1) {
                        if constexpr(inverse){i0=p3;i1=p2;i2=p0;i3=p1;}
                        else{o0=p3;o1=p2;o2=p0;o3=p1;}
                    }
                    for(size_t j=0;j<(end-begin)/flen;j++) {
                        auto A=i0[j],B=i1[j],C=i2[j],D=i3[j];
                        if constexpr(inverse) {
                            o0[j]=(A+B)+(C+D);
                            o2[j]=rotate((A+B)-(C+D),v2,t2);
                            o1[j]=rotate((A-B)-vi(C-D),v1,t1);
                            o3[j]=rotate((A-B)+vi(C-D),v3,t3);
                        }else{
                            B=rotate(B,v1,t1);C=rotate(C,v2,t2);D=rotate(D,v3,t3);
                            o0[j]=(A+C)+(B+D);o1[j]=(A+C)-(B+D);
                            o2[j]=(A-C)+vi(B-D);o3[j]=(A-C)-vi(B-D);
                        }
                    }
                };
                if(length>=shear_min<fixed>)run.template operator()<true>();else run.template operator()<false>();
            };
            if(top_only){
                size_t offset=range_offset,length=range_length;
                if(top_only==1){butterfly(offset,length,0,length/4);return;}
                if(top_only==3){
                    size_t h=length/64;
                    for(size_t j=0;j<h;j+=512){
                        size_t end=std::min(h,j+512);
                        auto stage0=[&](){for(size_t t=0;t<16;t++)butterfly(offset,length,j+t*h,end+t*h);};
                        auto stage1=[&](){for(size_t q=0;q<4;q++)for(size_t t=0;t<4;t++)butterfly(offset+q*length/4,length/4,j+t*h,end+t*h);};
                        auto stage2=[&](){for(size_t q=0;q<16;q++)butterfly(offset+q*length/16,length/16,j,end);};
                        if constexpr(inverse){stage2();stage1();stage0();}else{stage0();stage1();stage2();}
                    }
                    return;
                }
                size_t step=length/16;
                for(size_t j=0;j<step;j+=256){
                    size_t end=std::min(step,j+256);
                    if constexpr(inverse){
                        for(size_t t=0;t<4;t++)butterfly(offset+t*length/4,length/4,j,end);
                        for(size_t t=0;t<4;t++)butterfly(offset,length,j+t*step,end+t*step);
                    }else{
                        for(size_t t=0;t<4;t++)butterfly(offset,length,j+t*step,end+t*step);
                        for(size_t t=0;t<4;t++)butterfly(offset+t*length/4,length/4,j,end);
                    }
                }
                return;
            }
            auto recurse = [&](auto &&self, size_t offset, size_t length) -> void {
                if(length < 4 * flen) {return;}
                if(length >= (1 << 15)) {
                    size_t step = length / 16;
                    if constexpr(inverse) {
                        for(size_t t = 0; t < 16; t++) {self(self, offset + t*step, step);}
                    }
                    for(size_t j = 0; j < step; j += 256) {
                        size_t end = std::min(step, j+256);
                        if constexpr(inverse) {
                            for(size_t t=0;t<4;t++) {butterfly(offset+t*length/4, length/4, j,end);}
                            for(size_t t=0;t<4;t++) {butterfly(offset,length,j+t*step,end+t*step);}
                        } else {
                            for(size_t t=0;t<4;t++) {butterfly(offset,length,j+t*step,end+t*step);}
                            for(size_t t=0;t<4;t++) {butterfly(offset+t*length/4,length/4,j,end);}
                        }
                    }
                    if constexpr(!inverse) {
                        for(size_t t = 0; t < 16; t++) {self(self, offset + t*step, step);}
                    }
                } else if(length >= (size_t(1) << (6 + parity))) {
                    auto finish=[&]<bool par>() {
                        constexpr size_t chunk=size_t(1)<<(6+par);
                        constexpr size_t bottom=size_t(1)<<(4+par);
                        auto small=[&]<size_t L>(auto&& self,size_t pos) __attribute__((always_inline)) -> void {
                            if constexpr(inverse && L>bottom) {
                                for(size_t q=0;q<4;q++)self.template operator()<L/4>(self,pos+q*(L/4));
                            }
                            butterfly(pos,L,0,L/4);
                            if constexpr(!inverse && L>bottom) {
                                for(size_t q=0;q<4;q++)self.template operator()<L/4>(self,pos+q*(L/4));
                            }
                        };
                        for(size_t leaf=offset;leaf<offset+length;leaf+=chunk){
                            if constexpr(inverse){
                                small.template operator()<chunk>(small,leaf);
                                size_t level=std::min<size_t>(std::countr_one(leaf+chunk-1),std::countr_zero(length));
                                for(size_t lvl=6+2+par;lvl<=level;lvl+=2){size_t len=size_t(1)<<lvl;butterfly(leaf & ~(len-1),len,0,len/4);}
                            }else{
                                size_t level=std::min<size_t>(std::countr_zero(n+leaf),std::countr_zero(length));
                                level-=level%2!=par;
                                for(size_t lvl=level;lvl>=6+2+par;lvl-=2){size_t len=size_t(1)<<lvl;butterfly(leaf & ~(len-1),len,0,len/4);}
                                small.template operator()<chunk>(small,leaf);
                            }
                        }
                    };
                    if(parity)finish.template operator()<true>();else finish.template operator()<false>();
                } else {
                    if constexpr(inverse) {
                        for(size_t leaf = offset + 3 * flen; leaf < offset + length; leaf += 4 * flen) {
                            size_t level = std::min<size_t>(std::countr_one(leaf + 3), std::countr_zero(length));
                            for(size_t lvl = 4 + parity; lvl <= level; lvl += 2) {
                                size_t len = size_t(1) << lvl;
                                butterfly(leaf & ~(len-1), len, 0, len / 4);
                            }
                        }
                    } else {
                        for(size_t leaf = offset; leaf < offset + length; leaf += 4 * flen) {
                            size_t level = std::min<size_t>(std::countr_zero(n + leaf), std::countr_zero(length));
                            level -= level % 2 != parity;
                            for(size_t lvl = level; lvl >= 4; lvl -= 2) {
                                size_t len = size_t(1) << lvl;
                                butterfly(leaf & ~(len-1), len, 0, len / 4);
                            }
                        }
                    }
                }
            };
            // Radix two is performed separately at the leaves.
            recurse(recurse, range_offset, range_length?range_length:n);
        }
        // Shortest butterfly that uses the shear rotation; the fixed 2^24 transform reads its
        // shear roots from a table, so they are worth using down to the last in-block level.
        template<size_t fixed> static constexpr size_t shear_min = fixed == (1 << 24) ? 64 : 256;
        static inline big_vector<std::array<point,3>> shear_roots;
        static void prepare_shear_roots() {
            constexpr size_t n=1<<24;
            if(!shear_roots.empty())return;
            shear_roots.resize((n/16-1)/3,std::array<point,3>{});
            for(size_t len=n;len>=shear_min<n>;len/=4) {
                size_t count=n/len,base=(count-1)/3;
                point factor=roots[29-std::countr_zero(len)];
                for(size_t k=0;k<count;k++) {
                    point rt=factor*(k<pre_evals?evalp[k]:extra[k-pre_evals]);
                    vpoint v1={vz+real(rt),vz+imag(rt)};
                    if(k&1)v1=-vi(v1);
                    vpoint v2=v1*v1,v3=v1*v2;
                    vftype t1=imag(v1)/(vz+1.0+real(v1)),t2=imag(v2)/(vz+1.0+real(v2)),t3=imag(v3)/(vz+1.0+real(v3));
                    shear_roots[base+k]={point(imag(v1)[0],t1[0]),point(imag(v2)[0],t2[0]),point(imag(v3)[0],t3[0])};
                }
            }
        }
        static inline big_vector<point> extra;
        // Keep the usual table small; cache additional roots for large transforms.
        static void prepare_roots(size_t n) {
            if(n <= pre_evals + extra.size()) {return;}
            size_t old = extra.size();
            extra.resize(std::bit_ceil(n) - pre_evals);
            static const std::array<point,256> coarse=[](){
                std::array<point,256> out;
                for(size_t i=0;i<256;i++)out[i]=polar<ftype>(1.,std::numbers::pi*double((eval_args[256+i]-1)/2)/512.0);
                return out;
            }();
            for(size_t h=pre_evals+old;h<pre_evals+extra.size();h*=2){
                for(size_t i=h;i<2*h;i+=256){
                    point fine=polar<ftype>(1.,std::numbers::pi/double(4*h)*double(eval_arg(4*i)));
                    for(size_t j=0;j<256;j++)extra[i+j-pre_evals]=coarse[j]*fine;
                }
            }
        }
    };

}
#line 5 ".verify-helper/cache/fft-shallow-roots-20260922-nnenmcu8/fused/candidate.hpp"
#include <sys/resource.h>
namespace shallow {
using namespace cp_algo;
using namespace cp_algo::math;
using namespace cp_algo::math::fft;
using W = uint32_t;
inline double now() { timespec t; clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
struct statistics { double fill=0, fft=0, project=0, crt=0, residual=0; };
inline statistics stats;
inline bool measure_error = false;
inline cvector reused_a(0),reused_b(0);
template<W P> struct arithmetic {
    static constexpr W inv = -inv2<W>(P);
    static constexpr W r32 = (uint64_t(1) << 32) % P;
    static u32x8 add(u32x8 x, u32x8 y) { return reduce_once(x+y,P); }
    static u32x8 sub(u32x8 x, u32x8 y) { return reduce_once(x+P-y,P); }
    static u32x8 half(u32x8 x) {return (x+(x&1)*P)>>1;}
    static u32x8 quarter(u32x8 x) {return half(half(x));}
    static u32x8 mul(u32x8 x, W y) {
        W ym = uint64_t(y)*r32%P;
        return reduce_once(montgomery_mul(x,u32x8{}+ym,P,inv),P);
    }
    static W power(W x, uint64_t e) { W y=1; for(;e;e>>=1,x=uint64_t(x)*x%P) if(e&1)y=uint64_t(y)*x%P; return y; }
};
inline u32x8 load(auto const& input, size_t offset) {
    u32x8 out{};
    if(offset+8<=input.size()) std::memcpy(&out, input.data()+offset, sizeof(out));
    else for(size_t j=offset;j<std::min(offset+8,input.size());j++)out[j-offset]=input[j].getr();
    return out;
}
inline void save(auto& output,size_t offset,u32x8 value) {std::memcpy(output.data()+offset,&value,sizeof(value));}

// A short geometric recurrence with regular re-anchoring, fused into lift/recovery.
struct ramp {
    double log_step;
    vftype lanes, step, value;
    ramp(double log_radius,size_t n):log_step(log_radius/n),lanes{1,std::exp(log_step),std::exp(2*log_step),std::exp(3*log_step)},step(vz+std::exp(4*log_step)) {}
    vftype at(size_t i) { if(i%256==0)value=lanes*std::exp(log_step*i);else value*=step;return value; }
};

template<W P, bool Weighted, class Vector>
void fill(Vector& c,auto const& input,size_t n,W alpha,bool negative,double log_radius,u64x4 state) {
    using Q=quadratic<modint<P>>;using A=arithmetic<P>;
    typename Q::lattice L;
    size_t blocks=(input.size()+n-1)/n;
    assert(blocks<=4);
    std::array<W,4> powers{1,alpha,0,0};
    powers[2]=uint64_t(alpha)*alpha%P;powers[3]=uint64_t(powers[2])*alpha%P;
    c.r.resize(n/flen);ramp weight(log_radius,n);
    for(size_t i=0;i<n;i+=8) {
        u32x8 v=load(input,i);
        for(size_t block=1;block<blocks;block++) {
            auto upper=load(input,i+block*n);
            if(powers[block]==1)v=A::add(v,upper);
            else if(powers[block]==P-1)v=A::sub(v,upper);
            else v=A::add(v,A::mul(upper,powers[block]));
        }
        state^=state<<13;state^=state>>7;state^=state<<17;u32x8 words=u32x8(state);
        for(int half=0;half<2;half++) {
            auto bits=half?__builtin_shufflevector(v,v,4,5,6,7):__builtin_shufflevector(v,v,0,1,2,3);
            auto noise_bits=half?__builtin_shufflevector(words,words,1,3,5,7):__builtin_shufflevector(words,words,0,2,4,6);
            auto x=__builtin_convertvector(i32x4(bits),vftype);
            auto noise=__builtin_convertvector(i32x4(noise_bits),vftype)*0x1p-32;
            auto q=round(x*L.to_q+noise),t=round(x*L.to_t+noise);
            auto re=x-q*L.a-t*L.a2,im=(-(q*L.b)-t*L.b2);
            if constexpr(Q::fixed_d!=1)im=im*L.scale;
            vpoint z={re,negative?-im:im};
            if constexpr(Weighted)z=z*weight.at(i+4*half);
            c.at(i+4*half)=z;
        }
    }
}

template<W P,int Phase,bool Weighted,bool Last=false>
void branch_to(auto const& a,auto const& b,size_t n,W alpha,bool negative,double log_radius,auto&& prepare,auto&& consume) {
    using Q=quadratic<modint<P>>;
    phasevector<Phase> A(0),B(0);
    A.r.swap(reused_a.r);B.r.swap(reused_b.r);
    double begin=now();
    fill<P,Weighted>(A,a,n,alpha,negative,log_radius,Q::seed());
    fill<P,Weighted>(B,b,n,alpha,negative,log_radius,Q::seed());
    stats.fill+=now()-begin;begin=now();
    if(n>=(1<<20))A.product_blocks(B);else {A.forward();B.forward();A.multiply(B);}
    stats.fft+=now()-begin;begin=now();
    if constexpr(Last){spectrum_vector tmp;tmp.swap(B.r);}
    prepare();typename Q::lattice L;
    auto scale=vz+double(flen)/n;ramp weight(-log_radius,n);
    for(size_t i=0;i<n;i+=8) {
        u32x4 parts[2];
        for(int half=0;half<2;half++){
        size_t pos=i+half*4;
        auto z=A.at(pos)*scale;
        if constexpr(Weighted)z=z*weight.at(pos);
        if(measure_error) {
            auto re=real(z),im=imag(z)*L.inv_scale;
            auto er=abs(re-round(re)),ei=abs(im-round(im));
            for(size_t j=0;j<flen;j++)stats.residual=std::max({stats.residual,er[j],ei[j]});
        }
        auto v=[&]{if constexpr(Q::fixed_d==1)return Q::template project<true>(z,negative,L);else return Q::template project<false>(z,negative,L);}();
        parts[half]=v;
        }
        consume(i,__builtin_shufflevector(parts[0],parts[1],0,1,2,3,4,5,6,7));
    }
    stats.project+=now()-begin;
    if constexpr(!Last){A.r.swap(reused_a.r);B.r.swap(reused_b.r);}
}

template<W P,int Phase,bool Weighted>
void branch(auto const& a,auto const& b,size_t n,W alpha,bool negative,double log_radius,auto& out) {
    branch_to<P,Phase,Weighted>(a,b,n,alpha,negative,log_radius,[&]{out.resize(n);},[&](size_t j,u32x8 value){save(out,j,value);});
}

template<W P,bool Scaled> void two(auto& a,auto const& b) {
    using M=modint<P>;using Q=quadratic<M>;using A=arithmetic<P>;Q::init();
    size_t need=a.size()+b.size()-1,n=std::max<size_t>(8,std::bit_ceil(need)/2);
    big_vector<M> plus;W root=Scaled?Q::root:1;
    W hi=A::power(uint64_t(2)*root%P,P-2);
    auto prepare=[&]{a.resize(2*n);};
    auto consume=[&](size_t j,u32x8 minus) {
        auto p=load(plus,j),lo=A::half(A::add(p,minus)),diff=A::sub(p,minus);
        auto high=[&]{if constexpr(Scaled)return A::mul(diff,hi);else return A::half(diff);}();
        save(a,j,lo);save(a,n+j,high);
    };
    if constexpr(Scaled) {
        double log_radius=.5*std::log(double(Q::d));
        branch<P,2,true>(a,b,n,root,false,log_radius,plus);
        branch_to<P,2,true,true>(a,b,n,P-root,true,log_radius,prepare,consume);
    } else {
        branch<P,0,false>(a,b,n,1,false,0,plus);
        branch_to<P,4,false,true>(a,b,n,P-1,false,0,prepare,consume);
    }
    a.resize(need);
}

template<W P,bool Corners> void four(auto& a,auto const& b) {
    using M=modint<P>;using Q=quadratic<M>;using A=arithmetic<P>;Q::init();assert(Q::d==1);
    size_t need=a.size()+b.size()-1,n=std::max<size_t>(8,std::bit_ceil(need)/4);
    std::array<big_vector<M>,3> values;W I=Q::root,S=Corners?(I+1)%P:1;
    W inv4=A::power(4,P-2),is=A::power(S,P-2);
    W c1=uint64_t(inv4)*is%P,c2=uint64_t(c1)*is%P,c3=uint64_t(c2)*is%P;
    auto prepare=[&]{a.resize(4*n);};
    auto consume=[&](size_t j,u32x8 s) {
        auto p=load(values[0],j),q=load(values[1],j),r=load(values[2],j);
        auto u=A::add(p,q),v=A::add(r,s),w=A::sub(p,q),z=A::mul(A::sub(r,s),P-I);
        save(a,j,A::quarter(A::add(u,v)));
        if constexpr(Corners) {
            save(a,n+j,A::mul(A::add(w,z),c1));save(a,2*n+j,A::mul(A::sub(u,v),c2));save(a,3*n+j,A::mul(A::sub(w,z),c3));
        } else {
            save(a,n+j,A::quarter(A::add(w,z)));save(a,2*n+j,A::quarter(A::sub(u,v)));save(a,3*n+j,A::quarter(A::sub(w,z)));
        }
    };
    if constexpr(Corners) {
        double log_radius=.5*std::log(2.0);
        branch<P,1,true>(a,b,n,S,false,log_radius,values[0]);
        branch<P,3,true>(a,b,n,P-S,true,log_radius,values[1]);
        branch<P,3,true>(a,b,n,(I+P-1)%P,false,log_radius,values[2]);
        branch_to<P,1,true,true>(a,b,n,(P+1-I)%P,true,log_radius,prepare,consume);
    } else {
        branch<P,0,false>(a,b,n,1,false,0,values[0]);
        branch<P,4,false>(a,b,n,P-1,false,0,values[1]);
        branch<P,2,false>(a,b,n,I,false,0,values[2]);
        branch_to<P,2,false,true>(a,b,n,P-I,true,0,prepare,consume);
    }
    a.resize(need);
}

template<W P> void apply(std::string const& mode,auto& a,auto const& b) {
    if(mode=="base")fft::mul(a,b);
    else if(mode=="unit2")two<P,false>(a,b);
    else if(mode=="scaled2")two<P,true>(a,b);
    else if constexpr(P==998244353) {
        if(mode=="unit4")four<P,false>(a,b);
        else if(mode=="corners4")four<P,true>(a,b);
        else throw std::runtime_error("unknown mode");
    } else throw std::runtime_error("unknown mode");
}
}
#line 6 ".verify-helper/cache/fft-shallow-roots-20260922-nnenmcu8/fused/prefix-107.cpp"

namespace cp_algo::math::fft {
using submission_base=modint<1000000007>;
void mul(big_vector<submission_base>& a,big_vector<submission_base> const& b) {
    using V=big_vector<submission_base>;
    auto small=std::min(a.size(),b.size()),large=std::max(a.size(),b.size());
    if(small<64 || has_short_tail(a.size(),b.size()) || (small<=4096 && large>=(1<<20) && large/small>=64)) {
        return mul<V,V>(a,b);
    }
    shallow::apply<1000000007>("unit2",a,b);
}
}

#include <sys/mman.h>
#include <sys/stat.h>

// ---------------------------------------------------------------
// Fast mmap‑based reader / buffered writer (from the __yzlf code)
// ---------------------------------------------------------------
namespace fastio {
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned;
using i64 = long long;
using u64 = unsigned long long;

constexpr std::size_t BUF_DEF_SIZE       = 262144;
constexpr std::size_t BUF_FLUSH_THRESHOLD = 32;
constexpr u64 E16 = 10000000000000000ULL;
constexpr u64 E12 = 1000000000000ULL;
constexpr u64 E8  = 100000000ULL;
constexpr u64 E4  = 10000ULL;

struct _io_t {
    u8  t_i[1 << 15];          // two‑digit decode table
    u32 t_o[10000];            // four ASCII digits packed in u32
    constexpr _io_t() : t_i{}, t_o{} {
        std::fill(t_i, t_i + (1 << 15), u8(-1));
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                t_i[0x3030 + 256 * j + i] = j + 10 * i;
        int idx = 0;
        for (int e0 = (48 << 0); e0 < (58 << 0); e0 += (1 << 0))
            for (int e1 = (48 << 8); e1 < (58 << 8); e1 += (1 << 8))
                for (int e2 = (48 << 16); e2 < (58 << 16); e2 += (1 << 16))
                    for (int e3 = (48 << 24); e3 < (58 << 24); e3 += (1 << 24))
                        t_o[idx++] = e0 ^ e1 ^ e2 ^ e3;
    }
    constexpr void get(char* s, u32 p) const { *reinterpret_cast<u32*>(s) = t_o[p]; }
};
constexpr _io_t _iot{};

struct Reader {
    char* bg;
    char* p;
    char* ed;
    explicit Reader(FILE* f = stdin) {
        int fd = fileno(f);
        struct stat st; fstat(fd, &st);
        bg = static_cast<char*>(mmap(nullptr, st.st_size + 4, PROT_READ, MAP_PRIVATE, fd, 0));
        madvise(bg, st.st_size + 4, MADV_SEQUENTIAL);
        p  = bg;
        ed = bg + st.st_size;
    }
    ~Reader() = default; // OS will clean the mapping

    inline void skip_space() { while (*p <= ' ') ++p; }

    template <typename T>
    Reader& operator>>(T& x) {
        skip_space(); bool neg = false;
        if constexpr (std::is_signed_v<T>) { neg = (*p == '-'); p += neg; }
        x = *p++ - '0';
        while (true) {
            u32 y = _iot.t_i[*reinterpret_cast<u16*>(p)];
            if (y > 99) break;
            x = x * 100 + y; p += 2;
        }
        if (*p > ' ') x = x * 10 + (*p++ & 15);
        if (neg) x = -x; return *this;
    }
};

struct Writer {
    char* bg; char* p; char* ed; FILE* f;
    explicit Writer(FILE* fi = stdout, std::size_t sz = BUF_DEF_SIZE) : f(fi) {
        bg = new char[sz]; p = bg; ed = bg + sz - BUF_FLUSH_THRESHOLD;
    }
    ~Writer() { flush(); delete[] bg; }
    inline void flush() { fwrite(bg, 1, p - bg, f); p = bg; }
    inline void chk() { if (p > ed) flush(); }

    inline void put_block(u32 x) { _iot.get(p, x); p += 4; }

    inline void put2(u32 x) {
        if (x > 9) { _iot.get(p, x * 100); p += 2; }
        else *p++ = char('0' + x);
    }
    inline void put4(u32 x) {
        if (x > 99) { if (x > 999) put_block(x); else { _iot.get(p, x * 10); p += 3; } }
        else put2(x);
    }
    inline void write_u32(u32 x) {
        if (x >= E8) { put2(x / E8); x %= E8; put_block(x / E4); put_block(x % E4); }
        else if (x >= E4) { put4(x / E4); put_block(x % E4); }
        else put4(x);
        chk();
    }
    inline void write_u64(u64 x) {
        if (x >= E8) {
            u64 q0 = x / E8, r0 = x % E8;
            if (x >= E16) { u64 q1 = q0 / E8, r1 = q0 % E8; put4(q1); put_block(r1 / E4); put_block(r1 % E4); }
            else if (x >= E12) { put4(q0 / E4); put_block(q0 % E4); }
            else put4(q0);
            put_block(r0 / E4); put_block(r0 % E4);
        } else if (x >= E4) { put4(x / E4); put_block(x % E4); }
        else put4(static_cast<u32>(x));
        chk();
    }

    Writer& operator<<(u32 x) { write_u32(x); return *this; }
    Writer& operator<<(u64 x) { write_u64(x); return *this; }
    Writer& operator<<(int x) { if (x < 0) { *p++ = '-'; write_u32(-static_cast<u32>(x)); } else write_u32(static_cast<u32>(x)); return *this; }
    Writer& operator<<(long long x) { if (x < 0) { *p++ = '-'; write_u64(-static_cast<u64>(x)); } else write_u64(static_cast<u64>(x)); return *this; }
    Writer& operator<<(char c) { *p++ = c; chk(); return *this; }
};

inline Reader in;
inline Writer out;
} // namespace fastio

using base=cp_algo::math::modint<1000000007>;
int main(){
    using namespace fastio;
    u32 n,m;in>>n>>m;
    cp_algo::big_vector<base> a,b(m);
    if(n&&m)a.reserve(std::bit_ceil(size_t(n)+m-1));a.resize(n);
    for(auto &x:a){u32 v;in>>v;x.setr(v);}
    for(auto &x:b){u32 v;in>>v;x.setr(v);}
    cp_algo::checkpoint("read");
    if(n&&m)cp_algo::math::fft::mul(a,b);else a.clear();
    for(size_t i=0;i<a.size();i++){out<<static_cast<u32>(a[i].getr());out<<(i+1==a.size()?'\n':' ');}
    if(a.empty())out<<'\n';
    out.flush();cp_algo::checkpoint("write");cp_algo::checkpoint<true>();
}
