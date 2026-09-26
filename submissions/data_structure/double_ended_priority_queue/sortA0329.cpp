
#if !defined(__clang__) && defined(__GNUC__)
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")
#endif
#ifdef EVAL
#define ONLINE_JUDGE
#endif
#ifdef ONLINE_JUDGE
#define NDEBUG
#else
#define NDEBUG
#endif


#include <cstdlib>  // std::exit
#include <cstring>  // std::memcpy, std::memmove
#include <utility>  // std::forward
#include <tuple>    // std::tuple, std::make_tuple
#if __has_include(<unistd.h>)
#include <unistd.h>  // read, write
#endif
#ifndef _WIN32
#include <sys/mman.h>  // mmap
#include <sys/stat.h>  // stat, fstat
#endif

#include <type_traits>
#include <cstdint>
#include <limits>
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif

namespace gsh {

namespace itype {
#if !defined(INT8_MAX) || !defined(UINT8_MAX)
    static_assert(false, "This library needs std::int8_t and std::uint8_t.");
#endif
#if !defined(INT16_MAX) || !defined(UINT16_MAX)
    static_assert(false, "This library needs std::int16_t and std::uint16_t.");
#endif
#if !defined(INT32_MAX) || !defined(UINT32_MAX)
    static_assert(false, "This library needs std::int32_t and std::uint32_t.");
#endif
#if !defined(INT64_MAX) || !defined(UINT64_MAX)
    static_assert(false, "This library needs std::int64_t and std::uint64_t.");
#endif
    using i8 = std::int8_t;
    using u8 = std::uint8_t;
    using i16 = std::int16_t;
    using u16 = std::uint16_t;
    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;
#ifdef __SIZEOF_INT128__
    using i128 = __int128_t;
    using u128 = __uint128_t;
#elif defined(_MSC_VER)
    class u128 {
        u64 a, b;
    public:
        constexpr u128() : a(0), b(0) {}
        constexpr u128(const u128&) = default;
        constexpr u128(u128&&) = default;
        constexpr u128& operator=(const u128&) = default;
        constexpr u128& operator=(u128&&) = default;
    };
    class i128 {
        i64 a;
        u64 b;
    public:
        constexpr u128() : a(0), b(0) {}
        constexpr u128(const u128&) = default;
        constexpr u128(u128&&) = default;
        constexpr u128& operator=(const u128&) = default;
        constexpr u128& operator=(u128&&) = default;
    };
#else
    static_assert(false, "This library needs __int128_t and __uint128_t.");
#endif
}  // namespace itype

namespace ftype {
    class InvalidFloat16Tag;
    class InvalidBfloat16Tag;
    class InvalidFloat128Tag;
#ifdef __STDCPP_FLOAT16_T__
    using f16 = std::float16_t;
#else
    using f16 = InvalidFloat16Tag;
#endif
#ifdef __STDCPP_FLOAT32_T__
    using f32 = std::float32_t;
#else
    static_assert(std::numeric_limits<float>::is_iec559, "There are no types compliant with IEC 559 binary32.");
    using f32 = float;
#endif
#ifdef __STDCPP_FLOAT64_T__
    using f64 = std::float64_t;
#else
    static_assert(std::numeric_limits<double>::is_iec559, "There are no types compliant with IEC 559 binary64.");
    using f64 = double;
#endif
#ifdef __STDCPP_FLOAT128_T__
    using f128 = std::float128_t;
#elif defined(__SIZEOF_FLOAT128__)
    using f128 = std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, __float128>;
#else
    using f128 = std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, InvalidFloat128Tag>;
#endif
#ifdef __STDCPP_BFLOAT16_T__
    using bf16 = std::bfloat16_t;
#else
    using bf16 = InvalidBfloat16Tag;
#endif
}  // namespace ftype

namespace ctype {
    using c8 = char;
    using wc = wchar_t;
    using utf8 = char8_t;
    using utf16 = char16_t;
    using utf32 = char32_t;
}  // namespace ctype

}  // namespace gsh

#include <bit>          // std::countr_zero
#include <ranges>       // std::ranges::forward_range


#define GSH_INTERNAL_SELECT1(a, ...)                         a
#define GSH_INTERNAL_SELECT2(a, b, ...)                      b
#define GSH_INTERNAL_SELECT3(a, b, c, ...)                   c
#define GSH_INTERNAL_SELECT4(a, b, c, d, ...)                d
#define GSH_INTERNAL_SELECT5(a, b, c, d, e, ...)             e
#define GSH_INTERNAL_SELECT6(a, b, c, d, e, f, ...)          f
#define GSH_INTERNAL_SELECT7(a, b, c, d, e, f, g, ...)       g
#define GSH_INTERNAL_SELECT8(a, b, c, d, e, f, g, h, ...)    h
#define GSH_INTERNAL_SELECT9(a, b, c, d, e, f, g, h, i, ...) i

#define GSH_INTERNAL_STR(s)       #s
#define GSH_INTERNAL_CONCAT(a, b) a##b
#define GSH_INTERNAL_VA_SIZE(...) GSH_INTERNAL_SELECT8(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
#if defined __clang__ || defined __INTEL_COMPILER
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(unroll n))
#elif defined __GNUC__
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(GCC unroll n))
#else
#define GSH_INTERNAL_UNROLL(n)
#endif
#ifdef __GNUC__
#define GSH_INTERNAL_INLINE   __attribute__((always_inline))
#define GSH_INTERNAL_NOINLINE __attribute__((noinline))
#elif defined _MSC_VER
#define GSH_INTERNAL_INLINE   [[msvc::forceinline]]
#define GSH_INTERNAL_NOINLINE [[msvc::noinline]]
#else
#define GSH_INTERNAL_INLINE
#define GSH_INTERNAL_NOINLINE
#endif
#ifdef __GNUC__
#define GSH_INTERNAL_RESTRICT __restrict__
#elif defined _MSC_VER
#define GSH_INTERNAL_RESTRICT __restrict
#else
#define GSH_INTERNAL_RESTRICT
#endif
#ifdef __clang__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma(GSH_INTERNAL_STR(clang attribute push(__attribute__((__VA_ARGS__)), apply_to = apply)))
#define GSH_INTERNAL_POP_ATTRIBUTE              _Pragma("clang attribute pop")
#elif defined __GNUC__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma("GCC push_options") _Pragma(GSH_INTERNAL_STR(GCC __VA_ARGS__))
#define GSH_INTERNAL_POP_ATTRIBUTE              _Pragma("GCC pop_options")
#else
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...)
#define GSH_INTERNAL_POP_ATTRIBUTE
#endif


namespace gsh {

[[noreturn]] void Unreachable() {
#if defined __GNUC__ || defined __clang__
    __builtin_unreachable();
#elif _MSC_VER
    __assume(false);
#else
    [[maybe_unused]] itype::u32 n = 1 / 0;
#endif
};
GSH_INTERNAL_INLINE constexpr void Assume(const bool f) {
    if (std::is_constant_evaluated()) return;
#if defined __clang__
    __builtin_assume(f);
#elif defined __GNUC__
    if (!f) __builtin_unreachable();
#elif _MSC_VER
    __assume(f);
#else
    if (!f) Unreachable();
#endif
}
template<bool Likely = true> GSH_INTERNAL_INLINE constexpr bool Expect(const bool f) {
    if (std::is_constant_evaluated()) return f;
#if defined __GNUC__ || defined __clang__
    return __builtin_expect(f, Likely);
#else
    if constexpr (Likely) {
        if (f) [[likely]]
            return true;
        else return false;
    } else {
        if (f) [[unlikely]]
            return false;
        else return true;
    }
#endif
}
GSH_INTERNAL_INLINE constexpr bool Unpredictable(const bool f) {
    if (std::is_constant_evaluated()) return f;
#if defined __clang__
    return __builtin_unpredictable(f);
#elif defined __GNUC__
    return __builtin_expect_with_probability(f, 1, 0.5);
#else
    return f;
#endif
}

class InPlaceTag {};
[[maybe_unused]] constexpr InPlaceTag InPlace;

template<class T>
    requires std::is_trivially_copyable_v<T>
GSH_INTERNAL_INLINE constexpr void MemorySet(T* p, ctype::c8 byte, itype::u32 len) {
    if (std::is_constant_evaluated()) {
        struct mem {
            ctype::c8 buf[sizeof(T)] = {};
        };
        mem init;
        for (itype::u32 i = 0; i != sizeof(T); ++i) init.buf[i] = byte;
        for (itype::u32 i = 0; i != len / sizeof(T); ++i) p[i] = std::bit_cast<T>(init);
        if (len % sizeof(T) != 0) {
            auto& ref = p[len / sizeof(T)];
            mem tmp = std::bit_cast<mem>(ref);
            for (itype::u32 i = 0; i != len % sizeof(T); ++i) tmp.buf[i] = byte;
            ref = std::bit_cast<T>(tmp);
        }
    } else std::memset(p, byte, len);
}
template<class T>
    requires std::is_trivially_copyable_v<T>
GSH_INTERNAL_INLINE constexpr itype::u32 MemoryChar(T* p, ctype::c8 byte, itype::u32 len) {
    if (std::is_constant_evaluated()) {
        struct mem {
            ctype::c8 buf[sizeof(T)] = {};
        };
        for (itype::u32 i = 0; i != len / sizeof(T); ++i) {
            mem tmp = std::bit_cast<mem>(p[i]);
            for (itype::u32 j = 0; j != sizeof(T); ++j) {
                if (tmp.buf[j] == byte) return i * sizeof(T) + j;
            }
        }
        if (len % sizeof(T) != 0) {
            mem tmp = std::bit_cast<mem>(p[len / sizeof(T)]);
            for (itype::u32 i = 0; i != len % sizeof(T); ++i) {
                if (tmp.buf[i] == byte) return len / sizeof(T) * sizeof(T) + i;
            }
        }
        return 0xffffffff;
    } else {
        const void* tmp = std::memchr(p, byte, len);
        return (tmp == nullptr ? 0xffffffff : static_cast<const ctype::c8*>(tmp) - reinterpret_cast<const ctype::c8*>(p));
    }
}
template<class T, class U>
    requires std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<U>
GSH_INTERNAL_INLINE constexpr void MemoryCopy(T* GSH_INTERNAL_RESTRICT dst, U* GSH_INTERNAL_RESTRICT src, itype::u32 len) {
    if (std::is_constant_evaluated()) {
        struct mem1 {
            ctype::c8 buf[sizeof(T)] = {};
        };
        struct mem2 {
            ctype::c8 buf[sizeof(U)] = {};
        };
        mem1 tmp1;
        mem2 tmp2;
        for (itype::u32 i = 0; i != len; ++i) {
            if (i % sizeof(U) == 0) tmp2 = std::bit_cast<mem2>(src[i / sizeof(U)]);
            tmp1.buf[i % sizeof(T)] = tmp2.buf[i % sizeof(U)];
            if ((i + 1) % sizeof(T) == 0) {
                dst[i / sizeof(T)] = std::bit_cast<T>(tmp1);
                tmp1 = mem1{};
            }
        }
        if (len % sizeof(T) != 0) {
            mem1 tmp3 = std::bit_cast<mem1>(dst[len / sizeof(T)]);
            for (itype::u32 i = 0; i != len % sizeof(T); ++i) tmp3.buf[i] = tmp1.buf[i];
            dst[len / sizeof(T)] = std::bit_cast<T>(tmp3);
        }
    } else std::memcpy(dst, src, len);
}
/*
template<class T, class U>
    requires std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<U>
GSH_INTERNAL_INLINE constexpr void MemoryMove(T* dst, U* src, itype::u32 len) {
    if (std::is_constant_evaluated()) {
    } else std::memmove(dst, src, len);
}
*/
GSH_INTERNAL_INLINE constexpr itype::u32 StrLen(const ctype::c8* p) {
    if (std::is_constant_evaluated()) {
        auto q = p;
        while (*q != '\0') ++q;
        return q - p;
    } else return std::strlen(p);
}

namespace internal {
    template<itype::u32 N, class First, class... Tail> class TypeAtImpl : public TypeAtImpl<N - 1, Tail...> {};
    template<class T, class... Types> class TypeAtImpl<0, T, Types...> {
    public:
        using type = T;
    };
}  // namespace internal
template<itype::u32 N, class... Types> using TypeAt = typename internal::TypeAtImpl<N, Types...>::type;

template<class... Types> class TypeArr {
public:
    constexpr static itype::u32 size() noexcept { return sizeof...(Types); }
    template<itype::u32 N> using type = std::conditional_t<(N < sizeof...(Types)), TypeAt<N, Types...>, void>;
};
template<> class TypeArr<> {
public:
    constexpr static itype::u32 size() noexcept { return 0; }
    template<itype::u32 N> using type = void;
};

}  // namespace gsh


namespace gsh {

namespace itype {
    struct i4dig;
    struct u4dig;
    struct i8dig;
    struct u8dig;
    struct i16dig;
    struct u16dig;
}  // namespace itype

template<class T> class Parser;

namespace internal {
    template<class Stream> constexpr itype::u8 Parseu8(Stream& stream) {
        itype::u32 v;
        MemoryCopy(&v, stream.current(), 4);
        v ^= 0x30303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0) >> 3;
        v <<= (32 - (tmp << 3));
        v = (v * 10 + (v >> 8)) & 0x00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff;
        stream.skip(tmp + 1);
        return v;
    }
    template<class Stream> constexpr itype::u16 Parseu16(Stream& stream) {
        itype::u64 v;
        MemoryCopy(&v, stream.current(), 8);
        v ^= 0x3030303030303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
        v <<= (64 - (tmp << 3));
        v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
        v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
        stream.skip(tmp + 1);
        return v;
    }
    template<class Stream> constexpr itype::u32 Parseu32(Stream& stream) {
        itype::u32 res = 0;
        {
            itype::u64 v;
            MemoryCopy(&v, stream.current(), 8);
            if (!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                res = v;
                stream.skip(8);
            }
        }
        itype::u64 buf;
        MemoryCopy(&buf, stream.current(), 8);
        {
            itype::u32 v = buf;
            if (!((v ^= 0x30303030) & 0xf0f0f0f0)) {
                buf >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res = 10000 * res + v;
                stream.skip(4);
            }
        }
        {
            itype::u16 v = buf;
            if (!((v ^= 0x3030) & 0xf0f0)) {
                buf >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res = 100 * res + v;
                stream.skip(2);
            }
        }
        {
            const ctype::c8 v = ctype::c8(buf) ^ 0x30;
            const bool f = !(v & 0xf0);
            res = f ? 10 * res + v : res;
            stream.skip(f + 1);
        }
        return res;
    };
    template<class Stream> constexpr itype::u64 Parseu64(Stream& stream) {
        itype::u64 res = 0;
        {
            itype::u64 v;
            MemoryCopy(&v, stream.current(), 8);
            if (!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                stream.skip(8);
                itype::u64 u;
                MemoryCopy(&u, stream.current(), 8);
                if (!((u ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                    v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                    u = (u * 10 + (u >> 8)) & 0x00ff00ff00ff00ff;
                    v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                    u = (u * 100 + (u >> 16)) & 0x0000ffff0000ffff;
                    v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                    u = (u * 10000 + (u >> 32)) & 0x00000000ffffffff;
                    res = v * 100000000 + u;
                    stream.skip(8);
                } else {
                    v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                    v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                    v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                    res = v;
                }
            }
        }
        itype::u64 buf;
        MemoryCopy(&buf, stream.current(), 8);
        {
            itype::u32 v = buf;
            if (!((v ^= 0x30303030) & 0xf0f0f0f0)) {
                buf >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res = 10000 * res + v;
                stream.skip(4);
            }
        }
        {
            itype::u16 v = buf;
            if (!((v ^= 0x3030) & 0xf0f0)) {
                buf >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res = 100 * res + v;
                stream.skip(2);
            }
        }
        {
            const ctype::c8 v = ctype::c8(buf) ^ 0x30;
            const bool f = !(v & 0xf0);
            res = f ? 10 * res + v : res;
            stream.skip(f + 1);
        }
        return res;
    }
    template<class Stream> constexpr itype::u128 Parseu128(Stream& stream) {
        itype::u128 res = 0;
        GSH_INTERNAL_UNROLL(4)
        for (itype::u32 i = 0; i != 4; ++i) {
            itype::u64 v;
            MemoryCopy(&v, stream.current(), 8);
            if (((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) != 0) break;
            v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
            if (i == 0) res = v;
            else res = res * 100000000 + v;
            stream.skip(8);
        }
        itype::u64 buf;
        MemoryCopy(&buf, stream.current(), 8);
        itype::u64 res2 = 0, pw = 1;
        {
            itype::u32 v = buf;
            if (!((v ^= 0x30303030) & 0xf0f0f0f0)) {
                buf >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res2 = v;
                pw = 10000;
                stream.skip(4);
            }
        }
        {
            itype::u16 v = buf;
            if (!((v ^= 0x3030) & 0xf0f0)) {
                buf >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res2 = res2 * 100 + v;
                pw *= 100;
                stream.skip(2);
            }
        }
        {
            const ctype::c8 v = ctype::c8(buf) ^ 0x30;
            const bool f = (v & 0xf0) == 0;
            const volatile auto tmp1 = pw * 10, tmp2 = res2 * 10 + v;
            const auto tmp3 = tmp1, tmp4 = tmp2;
            pw = f ? tmp3 : pw;
            res2 = f ? tmp4 : res2;
            stream.skip(f + 1);
        }
        return res * pw + res2;
    }
    template<class Stream> constexpr itype::u16 Parseu4dig(Stream& stream) {
        itype::u32 v;
        MemoryCopy(&v, stream.current(), 4);
        v ^= 0x30303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0) >> 3;
        v <<= (32 - (tmp << 3));
        v = (v * 10 + (v >> 8)) & 0x00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff;
        stream.skip(tmp + 1);
        return v;
    }
    template<class Stream> constexpr itype::u32 Parseu8dig(Stream& stream) {
        itype::u64 v;
        MemoryCopy(&v, stream.current(), 8);
        v ^= 0x3030303030303030;
        const itype::u64 msk = v & 0xf0f0f0f0f0f0f0f0;
        itype::i32 tmp = std::countr_zero(msk) >> 3;
        v <<= (64 - (tmp << 3));
        v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
        v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
        stream.skip(tmp + 1);
        return v;
    }
}  // namespace internal

template<> class Parser<itype::u8> {
public:
    template<class Stream> constexpr itype::u8 operator()(Stream& stream) const {
        stream.reload(8);
        return internal::Parseu8(stream);
    }
};
template<> class Parser<itype::i8> {
public:
    template<class Stream> constexpr itype::i8 operator()(Stream& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i8 tmp = internal::Parseu8(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u16> {
public:
    template<class Stream> constexpr itype::u16 operator()(Stream& stream) const {
        stream.reload(8);
        return internal::Parseu16(stream);
    }
};
template<> class Parser<itype::i16> {
public:
    template<class Stream> constexpr itype::i16 operator()(Stream& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i16 tmp = internal::Parseu16(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u32> {
public:
    template<class Stream> constexpr itype::u32 operator()(Stream& stream) const {
        stream.reload(16);
        return internal::Parseu32(stream);
    }
};
template<> class Parser<itype::i32> {
public:
    template<class Stream> constexpr itype::i32 operator()(Stream& stream) const {
        stream.reload(16);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i32 tmp = internal::Parseu32(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u64> {
public:
    template<class Stream> constexpr itype::u64 operator()(Stream& stream) const {
        stream.reload(32);
        return internal::Parseu64(stream);
    }
};
template<> class Parser<itype::i64> {
public:
    template<class Stream> constexpr itype::i64 operator()(Stream& stream) const {
        stream.reload(32);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i64 tmp = internal::Parseu64(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u128> {
public:
    template<class Stream> constexpr itype::u128 operator()(Stream& stream) const {
        stream.reload(64);
        return internal::Parseu128(stream);
    }
};
template<> class Parser<itype::i128> {
public:
    template<class Stream> constexpr itype::i128 operator()(Stream& stream) const {
        stream.reload(64);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i128 tmp = internal::Parseu128(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u4dig> {
public:
    using value_type = itype::u16;
    template<class Stream> constexpr itype::u16 operator()(Stream& stream) const {
        stream.reload(8);
        return internal::Parseu4dig(stream);
    }
};
template<> class Parser<itype::i4dig> {
public:
    using value_type = itype::i16;
    template<class Stream> constexpr itype::i16 operator()(Stream& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i16 tmp = internal::Parseu4dig(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u8dig> {
public:
    using value_type = itype::u32;
    template<class Stream> constexpr itype::u32 operator()(Stream& stream) const {
        stream.reload(16);
        return internal::Parseu8dig(stream);
    }
};
template<> class Parser<itype::i8dig> {
public:
    using value_type = itype::i32;
    template<class Stream> constexpr itype::i32 operator()(Stream& stream) const {
        stream.reload(16);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i32 tmp = internal::Parseu8dig(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<ctype::c8> {
public:
    template<class Stream> constexpr ctype::c8 operator()(Stream& stream) const {
        stream.reload(2);
        ctype::c8 tmp = *stream.current();
        stream.skip(2);
        return tmp;
    }
};
template<> class Parser<ctype::c8*> {
public:
    using value_type = void;
    template<class Stream> constexpr ctype::c8* operator()(Stream& stream, ctype::c8* s) const {
        stream.reload(16);
        ctype::c8* c = s;
        while (true) {
            const ctype::c8* e = stream.current();
            while (*e >= '!') ++e;
            const itype::u32 len = e - stream.current();
            MemoryCopy(c, stream.current(), len);
            stream.skip(len);
            c += len;
            if (stream.avail() == 0) stream.reload();
            else break;
        }
        stream.skip(1);
        *c = '\0';
        return s;
    }
    template<class Stream> constexpr ctype::c8* operator()(Stream& stream, ctype::c8* s, itype::u32 n) const {
        itype::u32 rem = n;
        ctype::c8* c = s;
        itype::u32 avail = stream.avail();
        while (avail <= rem) {
            MemoryCopy(c, stream.current(), avail);
            c += avail;
            rem -= avail;
            stream.skip(avail);
            if (rem == 0) {
                *c = '\0';
                return s;
            }
            stream.reload();
            avail = stream.avail();
        }
        MemoryCopy(c, stream.current(), rem);
        c += rem;
        stream.skip(rem + 1);
        *c = '\0';
        return s;
    }
};

namespace internal {
    template<class T, class P, class Stream, class... Args> struct ParsingIterator {
        using value_type = T;
        using difference_type = itype::i32;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::input_iterator_tag;
        itype::u32 n;
        P* ref;
        Stream* stream;
        std::tuple<Args...>* args;
        constexpr ParsingIterator(itype::u32 m, P* r, Stream* s, std::tuple<Args...>* a) noexcept : n(m), ref(r), stream(s), args(a) {}
        GSH_INTERNAL_INLINE friend constexpr bool operator==(const ParsingIterator& a, const ParsingIterator& b) noexcept { return a.n == b.n; }
        GSH_INTERNAL_INLINE constexpr ParsingIterator& operator++() noexcept { return ++n, *this; }
        GSH_INTERNAL_INLINE constexpr ParsingIterator operator++(int) noexcept { return { n++, *ref, *stream, *args }; }
        GSH_INTERNAL_INLINE constexpr T operator*() const {
            return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) -> T {
                return (*ref)(*stream, std::get<I>(*args)...);
            }(std::make_integer_sequence<itype::u32, sizeof...(Args)>());
        }
    };
}  // namespace internal
template<class R> concept ParsableRange = std::ranges::forward_range<R> && requires { sizeof(Parser<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<ParsableRange R> class Parser<R> {
public:
    template<class Stream, class... Args> constexpr R operator()(Stream&& stream, itype::u32 len, Args&&... args) const {
        Parser<std::ranges::range_value_t<R>> p;
        std::tuple<Args...> a(std::forward<Args>(args)...);
        using iter = internal::ParsingIterator<std::ranges::range_value_t<R>, decltype(p), std::remove_cvref_t<Stream>, Args...>;
        return R(iter(0, &p, &stream, &a), iter(len, nullptr, nullptr, nullptr));
    }
};

/*
namespace internal {
    template<class T, class U> constexpr bool ParsableTupleImpl = false;
    template<class T, std::size_t... I> constexpr bool ParsableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Parser<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}  // namespace internal
template<class T> concept ParsableTuple = requires { std::tuple_size<T>::value; } && internal::ParsableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<ParsableTuple T>
    requires(!ParsableRange<T>)
class Parser<T> {
public:
    template<class Stream> constexpr T operator()(Stream&& stream) const {}
};
*/

}  // namespace gsh

#include <charconv>       // std::to_chars, std::chars_format, std::errc


namespace gsh {

class Exception {
    char str[512];
    char* cur = str;
    void write(const char* x) {
        for (int i = 0; i != 512; ++i, ++cur) {
            if (x[i] == '\0') break;
            *cur = x[i];
        }
    }
    void write(long long x) {
        if (x == 0) *(cur++) = '0';
        else {
            if (x < 0) {
                *(cur++) = '-';
                x = -x;
            }
            char buf[20];
            int i = 0;
            while (x != 0) buf[i++] = x % 10 + '0', x /= 10;
            while (i--) *(cur++) = buf[i];
        }
    }
    template<class T, class... Args> void generate_message(T x, Args... args) {
        write(x);
        if constexpr (sizeof...(Args) > 0) generate_message(args...);
    }
public:
    Exception() noexcept { *cur = '\0'; }
    Exception(const Exception& x) noexcept {
        for (int i = 0; i != 512; ++i) str[i] = x.str[i];
        cur = x.cur;
    }
    explicit Exception(const char* what_arg) noexcept {
        for (int i = 0; i != 512; ++i, ++cur) {
            *cur = what_arg[i];
            if (what_arg[i] == '\0') break;
        }
    }
    template<class... Args> explicit Exception(Args... args) noexcept {
        generate_message(args...);
        *cur = '\0';
    }
    Exception& operator=(const Exception& x) noexcept {
        for (int i = 0; i != 512; ++i) str[i] = x.str[i];
        cur = x.cur;
        return *this;
    }
    const char* what() const noexcept { return str; }
};

}  // namespace gsh


namespace gsh {

namespace itype {
    struct i4dig;
    struct u4dig;
    struct i8dig;
    struct u8dig;
    struct i16dig;
    struct u16dig;
}  // namespace itype

template<class T> class Formatter;

namespace internal {
    template<itype::u32> constexpr auto InttoStr = [] {
        struct {
            ctype::c8 table[40004] = {};
        } res;
        for (itype::u32 i = 0; i != 10000; ++i) {
            res.table[4 * i + 0] = (i / 1000 + '0');
            res.table[4 * i + 1] = (i / 100 % 10 + '0');
            res.table[4 * i + 2] = (i / 10 % 10 + '0');
            res.table[4 * i + 3] = (i % 10 + '0');
        }
        return res;
    }();
    template<class Stream> constexpr void Formatu16(Stream& stream, itype::u16 n) {
        auto copy1 = [&](itype::u16 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u16 x) {
            MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 10000) copy1(n);
        else {
            copy1(n / 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu32(Stream& stream, itype::u32 n) {
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 100000000) {
            if (n < 10000) copy1(n);
            else {
                copy1(n / 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 100000000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu64(Stream& stream, itype::u64 n) {
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 10000000000000000) {
            if (n < 1000000000000) {
                if (n < 100000000) {
                    if (n < 10000) copy1(n);
                    else {
                        copy1(n / 10000);
                        copy2(n % 10000);
                    }
                } else {
                    copy1(n / 100000000);
                    copy2(n / 10000 % 10000);
                    copy2(n % 10000);
                }
            } else {
                copy1(n / 1000000000000);
                copy2(n / 100000000 % 10000);
                copy2(n / 10000 % 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 10000000000000000);
            copy2(n / 1000000000000 % 10000);
            copy2(n / 100000000 % 10000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu128(Stream& stream, itype::u128 n) {
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        auto div_1e16 = [&](itype::u64& rem) -> itype::u64 {
#if defined(__GNUC__) && defined(__x86_64__)
            if constexpr (sizeof(void*) == 8) {
                if (std::is_constant_evaluated()) {
                    itype::u64 res = n / 10000000000000000;
                    rem = n - 10000000000000000 * res;
                    return res;
                }
                itype::u64 res;
                __asm__("divq %[v]" : "=a"(res), "=d"(rem) : [v] "r"(10000000000000000), "a"(static_cast<itype::u64>(n)), "d"(static_cast<itype::u64>(n >> 64)));
                return res;
            } else {
                itype::u64 res = n / 10000000000000000;
                rem = n - 10000000000000000 * res;
                return res;
            }
#else
            itype::u64 res = n / 10000000000000000;
            rem = n - 10000000000000000 * res;
            return res;
#endif
        };
        constexpr itype::u128 t = static_cast<itype::u128>(10000000000000000) * 10000000000000000;
        if (n >= t) {
            const itype::u32 dv = n / t;
            n -= dv * t;
            if (dv >= 10000) {
                copy1(dv / 10000);
                copy2(dv % 10000);
            } else copy1(dv);
            itype::u64 a, b = 0;
            a = div_1e16(b);
            const itype::u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
            copy2(c / 10000), copy2(c % 10000);
            copy2(d / 10000), copy2(d % 10000);
            copy2(e / 10000), copy2(e % 10000);
            copy2(f / 10000), copy2(f % 10000);
        } else {
            itype::u64 a, b = 0;
            a = div_1e16(b);
            const itype::u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
            const itype::u32 g = c / 10000, h = c % 10000, i = d / 10000, j = d % 10000, k = e / 10000, l = e % 10000, m = f / 10000, n = f % 10000;
            if (a == 0) {
                if (e == 0) {
                    if (m == 0) copy1(n);
                    else copy1(m), copy2(n);
                } else {
                    if (k == 0) copy1(l), copy2(m), copy2(n);
                    else copy1(k), copy2(l), copy2(m), copy2(n);
                }
            } else {
                if (c == 0) {
                    if (i == 0) copy1(j), copy2(k), copy2(l), copy2(m), copy2(n);
                    else copy1(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                } else {
                    if (g == 0) copy1(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                    else copy1(g), copy2(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                }
            }
        }
    }
    template<class Stream> constexpr void Formatu4dig(Stream& stream, itype::u16 x) {
        itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
        MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
        stream.skip(4 - off);
    }
    template<class Stream> constexpr void Formatu8dig(Stream& stream, itype::u32 x) {
        const itype::u32 n = x;
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 10000) copy1(n);
        else {
            copy1(n / 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu16dig(Stream& stream, itype::u64 x) {
        const itype::u64 n = x;
        auto copy1 = [&](itype::u64 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u64 x) {
            MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 1000000000000) {
            if (n < 100000000) {
                if (n < 10000) copy1(n);
                else {
                    copy1(n / 10000);
                    copy2(n % 10000);
                }
            } else {
                copy1(n / 100000000);
                copy2(n / 10000 % 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 1000000000000);
            copy2(n / 100000000 % 10000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
    }
}  // namespace internal

template<> class Formatter<itype::u16> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u16 n) const {
        stream.reload(8);
        internal::Formatu16(stream, n);
    }
};
template<> class Formatter<itype::i16> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i16 n) const {
        stream.reload(8);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu16(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u32> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u32 n) const {
        stream.reload(16);
        internal::Formatu32(stream, n);
    }
};
template<> class Formatter<itype::i32> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i32 n) const {
        stream.reload(16);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu32(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u64> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u64 n) const {
        stream.reload(32);
        internal::Formatu64(stream, n);
    }
};
template<> class Formatter<itype::i64> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i64 n) const {
        stream.reload(32);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu64(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u128> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u128 n) const {
        stream.reload(64);
        internal::Formatu128(stream, n);
    }
};
template<> class Formatter<itype::i128> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i128 n) const {
        stream.reload(64);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu128(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u4dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u16 n) const {
        stream.reload(4);
        internal::Formatu4dig(stream, n);
    }
};
template<> class Formatter<itype::i4dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i16 n) const {
        stream.reload(5);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu4dig(stream, static_cast<itype::u16>(n < 0 ? -n : n));
    }
};
template<> class Formatter<itype::u8dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u32 n) const {
        stream.reload(8);
        internal::Formatu8dig(stream, n);
    }
};
template<> class Formatter<itype::i8dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i32 n) const {
        stream.reload(9);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu8dig(stream, static_cast<itype::u32>(n < 0 ? -n : n));
    }
};
template<> class Formatter<itype::u16dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u64 n) const {
        stream.reload(16);
        internal::Formatu16dig(stream, n);
    }
};
template<> class Formatter<itype::i16dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i64 n) const {
        stream.reload(17);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu16dig(stream, static_cast<itype::u64>(n < 0 ? -n : n));
    }
};
template<> class Formatter<ctype::c8> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, ctype::c8 c) const {
        stream.reload(1);
        *stream.current() = c;
        stream.skip(1);
    }
};
namespace internal {
    template<class T> class FloatFormatter {
    public:
        template<class Stream> constexpr void FormatFloat(Stream& stream, T f, std::chars_format fmt, itype::i32 precision) {
            stream.reload(32);
            auto [ptr, err] = std::to_chars(stream.current(), stream.current() + stream.avail(), f, fmt, precision);
            if (err != std::errc{}) [[unlikely]] {
                stream.reload();
                auto [ptr, err] = std::to_chars(stream.current(), stream.current() + stream.avail(), f, fmt, precision);
                if (err != std::errc{}) throw Exception("gsh::Formatter<ftype::f32>::operator() / The value is too large.");
                stream.skip(ptr - stream.current());
            } else {
                stream.skip(ptr - stream.current());
            }
        }
    };
    template<> class FloatFormatter<ftype::InvalidFloat16Tag> {};
    template<> class FloatFormatter<ftype::InvalidBfloat16Tag> {};
    template<> class FloatFormatter<ftype::InvalidFloat128Tag> {};
#ifdef __SIZEOF_FLOAT128__
    template<> class FloatFormatter<__float128> {};
#endif
}  // namespace internal
template<> class Formatter<ftype::f16> : public internal::FloatFormatter<ftype::f16> {};
template<> class Formatter<ftype::f32> : public internal::FloatFormatter<ftype::f32> {};
template<> class Formatter<ftype::f64> : public internal::FloatFormatter<ftype::f64> {};
template<> class Formatter<ftype::f128> : public internal::FloatFormatter<ftype::f128> {};
template<> class Formatter<ftype::bf16> : public internal::FloatFormatter<ftype::bf16> {};
template<> class Formatter<bool> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, bool b) const {
        stream.reload(1);
        *stream.current() = '0' + b;
        stream.skip(1);
    }
};
template<> class Formatter<const ctype::c8*> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, const ctype::c8* s) const { operator()(stream, s, StrLen(s)); }
    template<class Stream> constexpr void operator()(Stream&& stream, const ctype::c8* s, itype::u32 len) const {
        itype::u32 avail = stream.avail();
        if (avail >= len) [[likely]] {
            MemoryCopy(stream.current(), s, len);
            stream.skip(len);
        } else {
            MemoryCopy(stream.current(), s, avail);
            len -= avail;
            s += avail;
            stream.skip(avail);
            while (len != 0) {
                stream.reload();
                avail = stream.avail();
                const itype::u32 tmp = len < avail ? len : avail;
                MemoryCopy(stream.current(), s, tmp);
                len -= tmp;
                s += tmp;
                stream.skip(tmp);
            }
        }
    }
};
template<> class Formatter<ctype::c8*> : public Formatter<const ctype::c8*> {};

template<class R> concept FormatableRange = std::ranges::forward_range<R> && requires { sizeof(Formatter<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<FormatableRange R> class Formatter<R> {
    template<class Stream, class T, class... Args> constexpr void print(Stream&& stream, T&& r, Args&&... args) const {
        auto first = std::ranges::begin(r);
        auto last = std::ranges::end(r);
        if (first == last) return;
        Formatter<std::decay_t<std::ranges::range_value_t<R>>> formatter;
        while (true) {
            formatter(stream, *first, args...);
            ++first;
            if (first != last) {
                Formatter<ctype::c8>{}(stream, ' ');
            } else break;
        }
    }
public:
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, R& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const R& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, R&& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const R&& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
};

namespace internal {
    template<class T, class U> constexpr bool FormatableTupleImpl = false;
    template<class T, std::size_t... I> constexpr bool FormatableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Formatter<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}  // namespace internal
template<class T> concept FormatableTuple = requires { std::tuple_size<T>::value; } && internal::FormatableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<FormatableTuple T>
    requires(!FormatableRange<T>)
class Formatter<T> {
    template<std::size_t I, class Stream, class U, class... Args> constexpr void print_element(Stream&& stream, U&& x, Args&&... args) const {
        using std::get;
        using element_type = std::decay_t<std::tuple_element_t<I, T>>;
        if constexpr (requires { x.template get<I>(); }) Formatter<element_type>{}(stream, x.template get<I>(), args...);
        else Formatter<element_type>{}(stream, get<I>(x), args...);
        if constexpr (I < std::tuple_size<T>::value - 1) {
            Formatter<ctype::c8>{}(stream, ' ');
            print_element<I + 1>(std::forward<Stream>(stream), x, std::forward<Args>(args)...);
        }
    }
    template<class Stream, class U, class... Args> constexpr void print(Stream&& stream, U&& x, Args&&... args) const {
        if constexpr (std::tuple_size<T>::value != 0) print_element<0>(std::forward<Stream>(stream), x, std::forward<Args>(args)...);
    }
public:
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, T& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const T& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, T&& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
    template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const T&& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
};

}  // namespace gsh

#include <concepts>                // std::totally_ordered_with, std::same_as, std::integral, std::floating_point

#include <cstddef>                 // std::nullptr_t

#include <typeindex>               // std::hash


namespace gsh {

namespace internal {
    template<class T> constexpr bool IsReferenceWrapper = false;
    template<class U> constexpr bool IsReferenceWrapper<std::reference_wrapper<U>> = true;
    // https://en.cppreference.com/w/cpp/utility/functional/invoke
    template<class C, class Pointed, class Object, class... Args> GSH_INTERNAL_INLINE constexpr decltype(auto) InvokeMemPtr(Pointed C::*member, Object&& object, Args&&... args) {
        using object_t = std::remove_cvref_t<Object>;
        constexpr bool is_member_function = std::is_function_v<Pointed>;
        constexpr bool is_wrapped = IsReferenceWrapper<object_t>;
        constexpr bool is_derived_object = std::is_same_v<C, object_t> || std::is_base_of_v<C, object_t>;
        if constexpr (is_member_function) {
            if constexpr (is_derived_object) return (std::forward<Object>(object).*member)(std::forward<Args>(args)...);
            else if constexpr (is_wrapped) return (object.get().*member)(std::forward<Args>(args)...);
            else return ((*std::forward<Object>(object)).*member)(std::forward<Args>(args)...);
        } else {
            static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
            if constexpr (is_derived_object) return std::forward<Object>(object).*member;
            else if constexpr (is_wrapped) return object.get().*member;
            else return (*std::forward<Object>(object)).*member;
        }
    }
}  // namespace internal
template<class F, class... Args> GSH_INTERNAL_INLINE constexpr std::invoke_result_t<F, Args...> Invoke(F&& f, Args&&... args) noexcept(std::is_nothrow_invocable_v<F, Args...>) {
    if constexpr (std::is_member_function_pointer_v<std::remove_cvref_t<F>>) return internal::InvokeMemPtr(f, std::forward<Args>(args)...);
    else return std::forward<F>(f)(std::forward<Args>(args)...);
}

namespace internal {
    template<typename T, typename U> concept LessPtrCmp = requires(T&& t, U&& u) {
        { t < u } -> std::same_as<bool>;
    } && std::convertible_to<T, const volatile void*> && std::convertible_to<U, const volatile void*> && (!requires(T&& t, U&& u) { operator<(std::forward<T>(t), std::forward<U>(u)); } && !requires(T&& t, U&& u) { std::forward<T>(t).operator<(std::forward<U>(u)); });
}  // namespace internal
class Less {
public:
    template<class T, class U>
        requires std::totally_ordered_with<T, U>
    GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() < std::declval<U>())) {
        if constexpr (internal::LessPtrCmp<T, U>) {
            if (std::is_constant_evaluated()) return t < u;
            auto x = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<T>(t)));
            auto y = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<U>(u)));
            return x < y;
        } else return std::forward<T>(t) < std::forward<U>(u);
    }
    using is_transparent = void;
};
class Greater {
public:
    template<class T, class U>
        requires std::totally_ordered_with<T, U>
    GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<U>() < std::declval<T>())) {
        if constexpr (internal::LessPtrCmp<U, T>) {
            if (std::is_constant_evaluated()) return u < t;
            auto x = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<T>(t)));
            auto y = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<U>(u)));
            return y < x;
        } else return std::forward<U>(u) < std::forward<T>(t);
    }
    using is_transparent = void;
};
class EqualTo {
public:
    template<class T, class U>
        requires std::equality_comparable_with<T, U>
    GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() == std::declval<U>())) {
        return std::forward<T>(t) == std::forward<U>(u);
    }
    using is_transparent = void;
};

class Identity {
public:
    template<class T> [[nodiscard]]
    GSH_INTERNAL_INLINE constexpr T&& operator()(T&& t) const noexcept {
        return std::forward<T>(t);
    }
    using is_transparent = void;
};

template<class F> class SwapArgs : public F {
public:
    constexpr SwapArgs() noexcept(std::is_nothrow_default_constructible_v<F>) : F() {}
    constexpr SwapArgs(const F& f) noexcept(std::is_nothrow_copy_constructible_v<F>) : F(f) {}
    constexpr SwapArgs(F&& f) noexcept(std::is_nothrow_move_constructible_v<F>) : F(std::move(f)) {}
    constexpr SwapArgs& operator=(const F& f) noexcept(std::is_nothrow_copy_assignable_v<F>) {
        F::operator=(f);
        return *this;
    }
    constexpr SwapArgs& operator=(F&& f) noexcept(std::is_nothrow_move_assignable_v<F>) {
        F::operator=(std::move(f));
        return *this;
    }
    constexpr SwapArgs& operator=(const SwapArgs&) noexcept(std::is_nothrow_copy_assignable_v<F>) = default;
    constexpr SwapArgs& operator=(SwapArgs&&) noexcept(std::is_nothrow_move_assignable_v<F>) = default;
    template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
    template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) const noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
};

template<class F, class... G> class BindFront {
    [[no_unique_address]] F func;
    [[no_unique_address]] BindFront<G...> bind;
    constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F> && noexcept(BindFront<G...>())) : func(), bind() {}
    template<class Arg, class... Args>
        requires(sizeof...(Args) == sizeof...(G))
    constexpr BindFront(Arg&& arg, Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Arg> && noexcept(BindFront<G...>(std::forward<Args>(args)...))) : func(std::forward<Arg>(arg)),
                                                                                                                                                                 bind(std::forward<Args>(args)...) {}
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) & noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(bind, Invoke(func, std::forward<Args>(args)...)); }
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) && noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(std::move(bind), Invoke(std::move(func), std::forward<Args>(args)...)); }
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(bind, Invoke(func, std::forward<Args>(args)...)); }
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const&& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(std::move(bind), Invoke(std::move(func), std::forward<Args>(args)...)); }
};
template<class F> class BindFront<F> : public F {
public:
    constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F>) : F() {}
    template<class... Args> constexpr BindFront(Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Args...>) : F(std::forward<Args>(args)...) {}
};

template<class T> class CustomizedHash;

namespace internal {
    template<class T> concept Nocvref = std::same_as<T, std::remove_cv_t<T>> && !std::is_reference_v<T>;
    constexpr itype::u64 MixIntegers(itype::u64 a, itype::u64 b) {
        itype::u128 tmp = static_cast<itype::u128>(a) * b;
        return static_cast<itype::u64>(tmp) ^ static_cast<itype::u64>(tmp >> 64);
    }
    constexpr itype::u64 HashBytes(const ctype::c8* ptr, itype::u32 len) noexcept {
        constexpr itype::u64 m = 0xc6a4a7935bd1e995;
        constexpr itype::u64 seed = 0xe17a1465;
        constexpr itype::u32 r = 47;
        itype::u64 h = seed ^ (len * m);
        const itype::u32 n_blocks = len / 8;
        for (itype::u64 i = 0; i < n_blocks; ++i) {
            itype::u64 k;
            const auto p = ptr + i * 8;
            if (std::is_constant_evaluated()) {
                k = 0;
                for (itype::u32 j = 0; j != 8; ++j) k |= static_cast<itype::u64>(p[j]) << (8 * j);
            } else {
                for (int j = 0; j != 8; ++j) *(reinterpret_cast<ctype::c8*>(&k) + j) = *(p + j);
            }
            k *= m;
            k ^= k >> r;
            k *= m;
            h ^= k;
            h *= m;
        }
        const auto data8 = ptr + n_blocks * 8;
        switch (len & 7u) {
        case 7 : h ^= static_cast<itype::u64>(data8[6]) << 48U; [[fallthrough]];
        case 6 : h ^= static_cast<itype::u64>(data8[5]) << 40U; [[fallthrough]];
        case 5 : h ^= static_cast<itype::u64>(data8[4]) << 32U; [[fallthrough]];
        case 4 : h ^= static_cast<itype::u64>(data8[3]) << 24U; [[fallthrough]];
        case 3 : h ^= static_cast<itype::u64>(data8[2]) << 16U; [[fallthrough]];
        case 2 : h ^= static_cast<itype::u64>(data8[1]) << 8U; [[fallthrough]];
        case 1 :
            h ^= static_cast<itype::u64>(data8[0]);
            h *= m;
            [[fallthrough]];
        default : break;
        }
        h ^= h >> r;
        return h;
    }
    constexpr itype::u64 HashBytes(const ctype::c8* ptr) noexcept {
        auto last = ptr;
        while (*last != '\0') ++last;
        return HashBytes(ptr, last - ptr);
    }
    template<class T> concept StdHashCallable = requires(T x) {
        { std::hash<T>{}(x) } -> std::integral;
    };
    template<class T> concept CustomizedHashCallable = requires(T x) {
        { CustomizedHash<T>{}(x) } -> std::integral;
    };
}  // namespace internal

// https://raw.githubusercontent.com/martinus/unordered_dense/v1.3.0/include/ankerl/unordered_dense.h
class Hash {
public:
    template<class T>
        requires internal::CustomizedHashCallable<T>
    constexpr itype::u64 operator()(const T& x) const {
        return static_cast<itype::u64>(CustomizedHash<T>{}(x));
    }
    template<class T>
        requires internal::CustomizedHashCallable<T>
    constexpr itype::u64 operator()(const T& x, const CustomizedHash<T>& h) const {
        return static_cast<itype::u64>(h(x));
    }
    template<class T>
        requires(!internal::CustomizedHashCallable<T> && !std::is_volatile_v<T>)
    constexpr itype::u64 operator()(const T& x) const {
        if constexpr (std::same_as<T, std::nullptr_t>) return operator()(static_cast<void*>(x));
        else if constexpr (std::is_pointer_v<T>) {
            static_assert(sizeof(x) == 4 || sizeof(x) == 8);
            if constexpr (sizeof(x) == 8) return operator()(std::bit_cast<itype::u64>(x));
            else return operator()(std::bit_cast<itype::u32>(x));
        } else if constexpr (std::same_as<T, itype::u64>) return internal::MixIntegers(x, 0x9e3779b97f4a7c15);
        else if constexpr (std::same_as<T, itype::u128>) {
            itype::u64 a = internal::MixIntegers(static_cast<itype::u64>(x), 0x9e3779b97f4a7c15);
            itype::u64 b = internal::MixIntegers(static_cast<itype::u64>(x >> 64), 12638153115695167455ull);
            return a ^ b;
        } else if constexpr (std::integral<T>) {
            static_assert(sizeof(T) <= 16);
            if constexpr (sizeof(T) <= 8) return operator()(static_cast<itype::u64>(x));
            else return operator()(static_cast<itype::u128>(x));
        } else if constexpr (std::floating_point<T>) {
            static_assert(sizeof(T) <= 16);
            if constexpr (sizeof(T) == 2) return operator()(std::bit_cast<itype::u16>(x));
            else if constexpr (sizeof(T) == 4) return operator()(std::bit_cast<itype::u32>(x));
            else if constexpr (sizeof(T) == 8) return operator()(std::bit_cast<itype::u64>(x));
            else if constexpr (sizeof(T) == 16) return operator()(std::bit_cast<itype::u128>(x));
            else if constexpr (sizeof(T) < 8) {
                struct a {
                    ctype::c8 b[sizeof(T)];
                };
                struct c {
                    a d;
                    ctype::c8 e[8 - sizeof(T)]{};
                } f;
                f.d = std::bit_cast<a>(x);
                return operator()(std::bit_cast<itype::u64>(f));
            } else {
                struct a {
                    struct b {
                        ctype::c8 c[sizeof(T)];
                    } d;
                    ctype::c8 e[16 - sizeof(T)]{};
                } f;
                f.d = std::bit_cast<a::b>(x);
                return operator()(std::bit_cast<itype::u128>(f));
            }
        } else if constexpr (internal::StdHashCallable<std::remove_cvref_t<T>>) return static_cast<itype::u64>(std::hash<std::remove_cvref_t<T>>{}(static_cast<std::remove_cvref_t<T>>(x)));
        else {
            static_assert((std::declval<T>(), false), "Cannot find the appropriate hash function.");
            return 0ull;
        }
    }
    using is_transparent = void;
};

class Plus {
public:
    template<class T, class U> constexpr decltype(auto) operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) + std::forward<U>(u))) { return std::forward<T>(t) + std::forward<U>(u); }
    using is_transparent = void;
};
class Negate {
public:
    template<class T> constexpr decltype(auto) operator()(T&& t) const noexcept(noexcept(-std::forward<T>(t))) { return -std::forward<T>(t); }
    using is_transparent = void;
};

}  // namespace gsh


namespace gsh {

namespace internal {
    template<class D> class IstreamInterface;
}  // namespace internal

template<class D, class Types, class... Args> class ParsingChain;

class NoParsingResult {
    template<class D, class Types, class... Args> friend class ParsingChain;
    constexpr NoParsingResult() noexcept {}
    NoParsingResult(const NoParsingResult&) = delete;
    NoParsingResult(NoParsingResult&&) = delete;
};
class CustomParser {
    ~CustomParser() = delete;
};

template<class D, class... Types, class... Args> class ParsingChain<D, TypeArr<Types...>, Args...> {
    friend class internal::IstreamInterface<D>;
    template<class D2, class Types2, class... Args2> friend class ParsingChain;
    D& ref;
    [[no_unique_address]] std::tuple<Args...> args;
    GSH_INTERNAL_INLINE constexpr ParsingChain(D& r, std::tuple<Args...>&& a) noexcept : ref(r), args(std::move(a)) {}
    template<class... Options>
        requires(sizeof...(Args) < sizeof...(Types))
    GSH_INTERNAL_INLINE constexpr auto next_chain(Options&&... options) const noexcept {
        return ParsingChain<D, TypeArr<Types...>, Args..., std::tuple<Options...>>(ref, std::tuple_cat(args, std::make_tuple(std::forward_as_tuple(std::forward<Options>(options)...))));
    };
public:
    ParsingChain() = delete;
    ParsingChain(const ParsingChain&) = delete;
    ParsingChain(ParsingChain&&) = delete;
    ParsingChain& operator=(const ParsingChain&) = delete;
    ParsingChain& operator=(ParsingChain&&) = delete;
    template<class... Options>
        requires(sizeof...(Args) == 0)
    [[nodiscard]] constexpr auto option(Options&&... options) const noexcept {
        return next_chain(std::forward<Options>(options)...);
    }
    template<class... Options>
        requires(sizeof...(Args) != 0)
    [[nodiscard]] constexpr auto operator()(Options&&... options) const noexcept {
        return next_chain(std::forward<Options>(options)...);
    }
    template<std::size_t N> friend constexpr decltype(auto) get(const ParsingChain& chain) {
        auto get_result = [](auto&& parser, auto&&... args) GSH_INTERNAL_INLINE -> decltype(auto) {
            if constexpr (std::is_void_v<std::invoke_result_t<decltype(parser), decltype(args)...>>) {
                Invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
                return NoParsingResult{};
            } else {
                return Invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
            }
        };
        using value_type = typename TypeArr<Types...>::template type<N>;
        if constexpr (N < sizeof...(Args)) {
            if constexpr (std::same_as<CustomParser, value_type>) {
                return std::apply([get_result, &chain](auto&& parser, auto&&... args) -> decltype(auto) { return get_result(std::forward<decltype(parser)>(parser), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
            } else {
                return std::apply([get_result, &chain](auto&&... args) -> decltype(auto) { return get_result(Parser<value_type>(), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
            }
        } else {
            return get_result(Parser<value_type>(), chain.ref);
        }
    }
    constexpr void ignore() const noexcept {
        [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
            (..., get<I>(*this));
        }(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
    }
    template<class T> constexpr operator T() const noexcept {
        static_assert(sizeof...(Types) == 1);
        return static_cast<T>(get<0>(*this));
    }
    constexpr decltype(auto) val() const noexcept {
        static_assert(sizeof...(Types) == 1);
        return get<0>(*this);
    }
    template<class... To>
        requires(sizeof...(To) == 0 || sizeof...(To) == sizeof...(Types))
    constexpr auto bind() const noexcept {
        if constexpr (sizeof...(To) == 0) {
            return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
                return std::tuple{ get<I>(*this)... };
            }(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
        } else {
            return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
                return std::tuple<To...>{ static_cast<To>(get<I>(*this))... };
            }(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
        }
    }
};

}  // namespace gsh

namespace std {
template<class D, class... Types, class... Args> class tuple_size<gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>> : public integral_constant<size_t, sizeof...(Types)> {};
template<size_t N, class D, class... Types, class... Args> class tuple_element<N, gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>> {
public:
    using type = decltype(get<N>(std::declval<const gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>&>()));
};
}  // namespace std

namespace gsh {

namespace internal {
    template<class D> class IstreamInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
    public:
        template<class T, class... Types> [[nodiscard]] constexpr auto read() { return ParsingChain<D, TypeArr<T, Types...>>(derived(), std::tuple<>()); }
    };
    template<class D> class OstreamInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
    public:
        template<class Sep> constexpr void write_sep(Sep&&) {}
        template<class Sep, class T, class... Args> constexpr void write_sep(Sep&& sep, T&& x, Args&&... args) {
            Formatter<std::decay_t<T>>{}(derived(), std::forward<T>(x));
            if constexpr (sizeof...(Args) != 0) {
                Formatter<std::decay_t<Sep>>{}(derived(), sep);
                write_sep(std::forward<Sep>(sep), std::forward<Args>(args)...);
            }
        }
        template<class... Args> constexpr void write(Args&&... args) { write_sep(' ', std::forward<Args>(args)...); }
        template<class Sep, class... Args> constexpr void writeln_sep(Sep&& sep, Args&&... args) {
            write_sep(std::forward<Sep>(sep), std::forward<Args>(args)...);
            Formatter<ctype::c8>{}(derived(), '\n');
        }
        template<class... Args> constexpr void writeln(Args&&... args) {
            write_sep(' ', std::forward<Args>(args)...);
            Formatter<ctype::c8>{}(derived(), '\n');
        }
    };
}  // namespace internal

template<itype::u32 Bufsize = (1 << 17)> class BasicReader : public internal::IstreamInterface<BasicReader<Bufsize>> {
    itype::i32 fd = 0;
    ctype::c8 buf[Bufsize + 1] = {};
    ctype::c8 *cur = buf, *eof = buf;
public:
    BasicReader() {}
    BasicReader(itype::i32 filehandle) : fd(filehandle) {}
    BasicReader(const BasicReader& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
        cur = buf + (rhs.cur - rhs.buf);
        eof = buf + (rhs.cur - rhs.eof);
    }
    BasicReader& operator=(const BasicReader& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
        cur = buf + (rhs.cur - rhs.buf);
        eof = buf + (rhs.cur - rhs.eof);
        return *this;
    }
    void reload() {
        if (eof == buf + Bufsize || eof == cur || [&] {
                auto p = cur;
                while (*p >= '!') ++p;
                return p;
            }() == eof) [[likely]] {
            itype::u32 rem = eof - cur;
            std::memmove(buf, cur, rem);
            *(eof = buf + rem + read(fd, buf + rem, Bufsize - rem)) = '\0';
            cur = buf;
        }
    }
    void reload(itype::u32 len) {
        if (avail() < len) [[unlikely]]
            reload();
    }
    itype::u32 avail() const { return eof - cur; }
    const ctype::c8* current() const { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class MmapReader : public internal::IstreamInterface<MmapReader> {
    [[maybe_unused]] const itype::i32 fh;
    [[maybe_unused]] ctype::c8 *buf, *cur, *eof;
public:
    MmapReader() : fh(0) {
#ifdef _WIN32
        write(1, "gsh::MmapReader / gsh::MmapReader is not available for Windows.\n", 64);
        std::exit(1);
#else
        struct stat st;
        fstat(0, &st);
        buf = reinterpret_cast<ctype::c8*>(mmap(nullptr, st.st_size + 64, PROT_READ, MAP_PRIVATE, 0, 0));
        cur = buf;
        eof = buf + st.st_size;
#endif
    }
    void reload() const {}
    void reload(itype::u32) const {}
    itype::u32 avail() const { return eof - cur; }
    const ctype::c8* current() const { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class StaticStrReader : public internal::IstreamInterface<StaticStrReader> {
    const ctype::c8* cur;
public:
    constexpr StaticStrReader() {}
    constexpr StaticStrReader(const ctype::c8* c) : cur(c) {}
    constexpr void reload() const {}
    constexpr void reload(itype::u32) const {}
    constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
    constexpr const ctype::c8* current() { return cur; }
    constexpr void skip(itype::u32 n) { cur += n; }
};

template<itype::u32 Bufsize = (1 << 17)> class BasicWriter : public internal::OstreamInterface<BasicWriter<Bufsize>> {
    itype::i32 fd = 1;
    ctype::c8 buf[Bufsize + 1] = {};
    ctype::c8 *cur = buf, *eof = buf + Bufsize;
public:
    BasicWriter() {}
    BasicWriter(itype::i32 filehandle) : fd(filehandle) {}
    BasicWriter(const BasicWriter& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
        cur = buf + (rhs.cur - rhs.buf);
    }
    BasicWriter& operator=(const BasicWriter& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
        cur = buf + (rhs.cur - rhs.buf);
        return *this;
    }
    void reload() {
        [[maybe_unused]] itype::i32 tmp = write(fd, buf, cur - buf);
        cur = buf;
    }
    void reload(itype::u32 len) {
        if (eof - cur < len) [[unlikely]]
            reload();
    }
    itype::u32 avail() const { return eof - cur; }
    ctype::c8* current() { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class StaticStrWriter : public internal::OstreamInterface<StaticStrWriter> {
    ctype::c8* cur;
public:
    constexpr StaticStrWriter() {}
    constexpr StaticStrWriter(ctype::c8* c) : cur(c) {}
    constexpr void reload() const {}
    constexpr void reload(itype::u32) const {}
    constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
    constexpr ctype::c8* current() { return cur; }
    constexpr void skip(itype::u32 n) { cur += n; }
};

}  // namespace gsh

#include <iterator>          // std::reverse_iterator, std::iterator_traits, std::input_iterator, std::distance
#include <algorithm>         // std::lexicographical_compare_three_way
#include <initializer_list>  // std::initializer_list


namespace gsh {

template<class R> concept Range = std::ranges::range<R>;
template<class R, class T> concept Rangeof = Range<R> && std::same_as<T, std::ranges::range_value_t<R>>;
template<class R> concept InputRange = std::ranges::input_range<R>;
template<class R, class T> concept OutputRange = Range<R> && std::ranges::output_range<R, T>;
template<class R> concept ForwardRange = std::ranges::forward_range<R>;
template<class R> concept BidirectionalRange = std::ranges::bidirectional_range<R>;
template<class R> concept RandomAccessRange = std::ranges::random_access_range<R>;
enum class RangeKind { Sized, Unsized };
template<class R> concept PointerObtainable = requires(R r) { std::ranges::data(r); };

namespace internal {
    template<class T, class U> concept same_ncvr = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
}
template<Range R> class RangeTraits {
public:
    using value_type = std::ranges::range_value_t<R>;
    using iterator = std::ranges::iterator_t<R>;
    using sentinel = std::ranges::sentinel_t<R>;
    using const_iterator = decltype(std::ranges::cbegin(std::declval<R&>()));
    using const_sentinel = decltype(std::ranges::cend(std::declval<R&>()));
    using size_type = std::ranges::range_size_t<R>;
    using difference_type = std::ranges::range_difference_t<R>;
    using reference = std::ranges::range_reference_t<R>;
    using const_reference = std::common_reference_t<const std::iter_value_t<iterator>&&, std::iter_reference_t<iterator>>;
    using rvalue_reference = std::ranges::range_rvalue_reference_t<R>;
    using range_type = std::remove_cvref_t<R>;
    constexpr static RangeKind range_kind = std::ranges::sized_range<R> ? RangeKind::Sized : RangeKind::Unsized;
    constexpr static bool pointer_obtainable = requires(R r) { std::ranges::data(r); };
    constexpr static bool is_borrowed_range = std::ranges::borrowed_range<R>;

    template<internal::same_ncvr<R> T> static constexpr itype::u32 size(T&& r) { return std::ranges::size(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr itype::u32 ssize(T&& r) { return std::ranges::ssize(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr bool empty(T&& r) { return std::ranges::empty(std::forward<T>(r)); }

    template<internal::same_ncvr<R> T> static constexpr auto begin(T&& r) { return std::ranges::begin(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto end(T&& r) { return std::ranges::end(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto cbegin(T&& r) { return std::ranges::cbegin(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto cend(T&& r) { return std::ranges::cend(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto rbegin(T&& r) { return std::ranges::rbegin(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto rend(T&& r) { return std::ranges::rend(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto crbegin(T&& r) { return std::ranges::crbegin(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto crend(T&& r) { return std::ranges::crend(std::forward<T>(r)); }

    template<internal::same_ncvr<R> T> static constexpr auto mbegin(T&& r) { return std::move_iterator(begin(std::forward<T>(r))); }
    template<internal::same_ncvr<R> T> static constexpr auto mend(T&& r) { return std::move_sentinel(end(std::forward<T>(r))); }
    template<internal::same_ncvr<R> T> static constexpr auto mcbegin(T&& r) { return std::move_iterator(cbegin(std::forward<T>(r))); }
    template<internal::same_ncvr<R> T> static constexpr auto mcend(T&& r) { return std::move_sentinel(cend(std::forward<T>(r))); }
    template<internal::same_ncvr<R> T> static constexpr auto mrbegin(T&& r) { return std::move_iterator(rbegin(std::forward<T>(r))); }
    template<internal::same_ncvr<R> T> static constexpr auto mrend(T&& r) { return std::move_sentinel(rend(std::forward<T>(r))); }
    template<internal::same_ncvr<R> T> static constexpr auto mcrbegin(T&& r) { return std::move_iterator(crbegin(std::forward<T>(r))); }
    template<internal::same_ncvr<R> T> static constexpr auto mcrend(T&& r) { return std::move_sentinel(crend(std::forward<T>(r))); }

    template<internal::same_ncvr<R> T> static constexpr auto fbegin(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return begin(std::forward<T>(r));
        else return mbegin(std::forward<T>(r));
    }
    template<internal::same_ncvr<R> T> static constexpr auto fend(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return end(std::forward<T>(r));
        else return mend(std::forward<T>(r));
    }
    template<internal::same_ncvr<R> T> static constexpr auto fcbegin(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return cbegin(std::forward<T>(r));
        else return mcbegin(std::forward<T>(r));
    }
    template<internal::same_ncvr<R> T> static constexpr auto fcend(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return cend(std::forward<T>(r));
        else return mcend(std::forward<T>(r));
    }
    template<internal::same_ncvr<R> T> static constexpr auto frbegin(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return rbegin(std::forward<T>(r));
        else return mrbegin(std::forward<T>(r));
    }
    template<internal::same_ncvr<R> T> static constexpr auto frend(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return rend(std::forward<T>(r));
        else return mrend(std::forward<T>(r));
    }
    template<internal::same_ncvr<R> T> static constexpr auto fcrbegin(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return crbegin(std::forward<T>(r));
        else return mcrbegin(std::forward<T>(r));
    }
    template<internal::same_ncvr<R> T> static constexpr auto fcrend(T&& r) {
        if constexpr (!std::ranges::borrowed_range<std::remove_cvref_t<T>>) return crend(std::forward<T>(r));
        else return mcrend(std::forward<T>(r));
    }

    template<internal::same_ncvr<R> T> static constexpr auto data(T&& r) { return std::ranges::data(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto cdata(T&& r) { return std::ranges::cdata(std::forward<T>(r)); }
};

template<class D, class V>
    requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
class ViewInterface;

template<class Iter> class SlicedRange : public ViewInterface<SlicedRange<Iter>, std::iter_value_t<Iter>> {
public:
    using iterator = Iter;
    using value_type = std::iter_value_t<Iter>;
    static_assert(std::sentinel_for<iterator, iterator>, "gsh::SlicedRange / The iterator cannot behave as sentinel.");
private:
    iterator first, last;
public:
    constexpr SlicedRange(iterator beg, iterator end) : first(beg), last(end) {}
    constexpr iterator begin() const { return first; }
    constexpr iterator end() const { return last; }
    constexpr auto rbegin() const { return std::reverse_iterator{ last }; }
    constexpr auto rend() const { return std::reverse_iterator{ first }; }
};

template<class D, class V>
    requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
class ViewInterface {
    constexpr D& derived() { return *static_cast<D*>(this); }
    constexpr const D& derived() const { return *static_cast<const D*>(this); }
    constexpr auto get_begin() { return derived().begin(); }
    constexpr auto get_begin() const { return derived().cbegin(); }
    constexpr auto get_end() { return derived().end(); }
    constexpr auto get_end() const { return derived().cend(); }
    constexpr auto get_rbegin() { return derived().rbegin(); }
    constexpr auto get_rbegin() const { return derived().crbegin(); }
    constexpr auto get_rend() { return derived().rend(); }
    constexpr auto get_rend() const { return derived().crend(); }
public:
    using derived_type = D;
    using value_type = V;
    constexpr derived_type copy() const& { return derived(); }
    constexpr derived_type copy() & { return derived(); }
    constexpr derived_type copy() && { return std::move(derived()); }
    constexpr auto slice(itype::u32 a, itype::u32 b) {
        auto beg = std::next(get_begin(), a);
        auto end = std::next(beg, b - a);
        return SlicedRange{ beg, end };
    }
    constexpr auto slice(itype::u32 a, itype::u32 b) const {
        auto beg = std::next(get_begin(), a);
        auto end = std::next(beg, b - a);
        return SlicedRange{ beg, end };
    }
    constexpr auto slice(itype::u32 a) { return SlicedRange{ std::next(get_begin(), a), get_end() }; }
    constexpr auto slice(itype::u32 a) const { return SlicedRange{ std::next(get_begin(), a), get_end() }; }
    template<std::predicate<value_type> Pred> constexpr bool all_of(Pred f) const {
        for (const auto& el : derived())
            if (!f(el)) return false;
        return true;
    }
    constexpr bool all_of(const value_type& x) const {
        for (const auto& el : derived())
            if (!(el == x)) return false;
        return true;
    }
    template<std::predicate<value_type> Pred> constexpr bool any_of(Pred f) const {
        for (const auto& el : derived())
            if (f(el)) return true;
        return false;
    }
    constexpr bool any_of(const value_type& x) const {
        for (const auto& el : derived())
            if (el == x) return true;
        return false;
    }
    template<std::predicate<value_type> Pred> constexpr bool none_of(Pred f) const {
        for (const auto& el : derived())
            if (f(el)) return false;
        return true;
    }
    constexpr bool none_of(const value_type& x) const {
        for (const auto& el : derived())
            if (el == x) return false;
        return true;
    }
    constexpr bool contains(const value_type& x) const {
        for (const auto& el : derived())
            if (el == x) return true;
        return false;
    }
    constexpr auto find(const value_type& x) const {
        const auto end = get_end();
        for (auto itr = get_begin(); itr != end; ++itr)
            if (*itr == x) return itr;
        return end;
    }
    constexpr itype::u32 count(const value_type& x) const {
        itype::u32 res = 0;
        for (const auto& el : derived()) res += (el == x);
        return res;
    }
};

namespace internal {
    template<class T, class U> concept difference_from = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
    template<class From, class To> concept convertible_to_non_slicing = std::convertible_to<From, To> && !(std::is_pointer_v<std::decay_t<From>> && std::is_pointer_v<std::decay_t<To>> && !std::convertible_to<std::remove_pointer_t<std::decay_t<From>> (*)[], std::remove_pointer_t<std::decay_t<To>> (*)[]>);
    template<class T> concept pair_like = /* tuple-like<T> && */ std::tuple_size_v<std::remove_cvref_t<T>> == 2;
    template<class T, class U, class V> concept pair_like_convertible_from = !std::ranges::range<T> && !std::is_reference_v<T> && pair_like<T> && std::constructible_from<T, U, V> && convertible_to_non_slicing<U, std::tuple_element_t<0, T>> && std::convertible_to<V, std::tuple_element_t<1, T>>;
}  // namespace internal
template<std::input_or_output_iterator I, std::sentinel_for<I> S = I, RangeKind K = std::sized_sentinel_for<S, I> ? RangeKind::Sized : RangeKind::Unsized>
    requires(K == RangeKind::Sized || !std::sized_sentinel_for<S, I>)
class Subrange : public ViewInterface<Subrange<I, S, K>, std::iter_value_t<I>> {
    I itr;
    S sent;
    static constexpr bool StoreSize = (K == RangeKind::Sized && !std::sized_sentinel_for<S, I>);
    struct empty_sz {};
    [[no_unique_address]] std::conditional_t<StoreSize, std::make_unsigned_t<std::iter_difference_t<I>>, empty_sz> sz;
public:
    constexpr Subrange() = default;
    constexpr Subrange(internal::convertible_to_non_slicing<I> auto i, S s)
        requires(!StoreSize)
      : itr(i),
        sent(s) {}
    constexpr Subrange(internal::convertible_to_non_slicing<I> auto i, S s, std::make_unsigned_t<std::iter_difference_t<I>> n)
        requires(K == RangeKind::Sized)
      : itr(i),
        sent(s) {
        if constexpr (StoreSize) sz = n;
    }
    template<internal::difference_from<Subrange> R>
        requires std::ranges::borrowed_range<R> && internal::convertible_to_non_slicing<std::ranges::iterator_t<R>, I>
                 && std::convertible_to<std::ranges::sentinel_t<R>, S>
                 constexpr Subrange(R&& r)
                     requires(!StoreSize || std::ranges::sized_range<R>)
      : itr(std::ranges::begin(r)),
        sent(std::ranges::end(r)) {}
    template<std::ranges::borrowed_range R>
        requires internal::convertible_to_non_slicing<std::ranges::iterator_t<R>, I>
                 && std::convertible_to<std::ranges::sentinel_t<R>, S>
                 constexpr Subrange(R&& r, std::make_unsigned_t<std::iter_difference_t<I>> n)
                     requires(K == RangeKind::Sized)
      : Subrange{ std::ranges::begin(r), std::ranges::end(r), n } {}
    template<internal::difference_from<Subrange> PairLike>
        requires internal::pair_like_convertible_from<PairLike, const I&, const S&>
    constexpr operator PairLike() const {
        return PairLike(itr, sent);
    }
    constexpr I begin() const
        requires std::copyable<I>
    {
        return itr;
    }
    [[nodiscard]] constexpr I begin()
        requires(!std::copyable<I>)
    {
        return std::move(itr);
    }
    constexpr S end() const { return sent; }
    constexpr bool empty() const { return itr == sent; }
    constexpr I data() const
        requires(std::is_pointer_v<I> && std::copyable<I>)
    {
        return itr;
    }
    constexpr I data() const
        requires(std::is_pointer_v<I> && !std::copyable<I>)
    {
        return std::move(itr);
    }
    [[nodiscard]] constexpr Subrange next(std::iter_difference_t<I> n = 1) const&
        requires std::forward_iterator<I>
    {
        auto tmp = *this;
        tmp.advance(n);
        return tmp;
    }
    [[nodiscard]] constexpr Subrange next(std::iter_difference_t<I> n = 1) && {
        advance(n);
        return std::move(*this);
    }
    [[nodiscard]] constexpr Subrange prev(std::iter_difference_t<I> n = 1) const
        requires std::bidirectional_iterator<I>
    {
        auto tmp = *this;
        tmp.advance(-n);
        return tmp;
    }
    constexpr Subrange& advance(std::iter_difference_t<I> n) {
        if constexpr (StoreSize) {
            auto d = n - std::ranges::advance(itr, n, sent);
            if (d >= 0) sz -= static_cast<std::make_unsigned_t<std::remove_cvref_t<decltype(d)>>>(d);
            else sz += static_cast<std::make_unsigned_t<std::remove_cvref_t<decltype(d)>>>(d);
            return *this;
        } else {
            std::ranges::advance(itr, n, sent);
            return *this;
        }
    }
};
template<std::input_or_output_iterator I, std::sentinel_for<I> S> Subrange(I, S) -> Subrange<I, S>;
template<std::input_or_output_iterator I, std::sentinel_for<I> S> Subrange(I, S, std::make_unsigned_t<std::iter_difference_t<I>>) -> Subrange<I, S, RangeKind::Sized>;
template<std::ranges::borrowed_range R> Subrange(R&&) -> Subrange<std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, (std::ranges::sized_range<R> || std::sized_sentinel_for<std::ranges::sentinel_t<R>, std::ranges::iterator_t<R>>) ? RangeKind::Sized : RangeKind::Unsized>;
template<std::ranges::borrowed_range R> Subrange(R&&, std::make_unsigned_t<std::ranges::range_difference_t<R>>) -> Subrange<std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, RangeKind::Sized>;

}  // namespace gsh

namespace std::ranges {
template<class I, class S, gsh::RangeKind K> constexpr bool enable_borrowed_range<gsh::Subrange<I, S, K>> = true;
}

#include <new>          // ::operator new
#include <memory>       // std::construct_at, std::destroy_at


namespace gsh {

namespace internal {
    template<class T, class U> struct GetPtr {
        using type = U*;
    };
    template<class T, class U>
        requires requires { typename T::pointer; }
    struct GetPtr<T, U> {
        using type = typename T::pointer;
    };
    template<class T, class U> struct RepFirst {};
    template<template<class, class...> class SomeTemplate, class U, class T, class... Types> struct RepFirst<SomeTemplate<T, Types...>, U> {
        using type = SomeTemplate<U, Types...>;
    };
    template<class T, class U> struct Rebind {
        using type = typename RepFirst<T, U>::type;
    };
    template<class T, class U>
        requires requires { typename T::template rebind<U>; }
    struct Rebind<T, U> {
        using type = typename T::template rebind<U>;
    };
    template<class T, class U> struct GetRebindPtr {
        using type = typename Rebind<T, U>::type;
    };
    template<class T, class U> struct GetRebindPtr<T*, U> {
        using type = U;
    };
    template<class T, class U, class V> struct GetConstPtr {
        using type = typename GetRebindPtr<U, const V*>::type;
    };
    template<class T, class U, class V>
        requires requires { typename T::const_pointer; }
    struct GetConstPtr<T, U, V> {
        using type = typename T::const_pointer;
    };
    template<class T, class U> struct GetVoidPtr {
        using type = typename GetRebindPtr<U, void*>::type;
    };
    template<class T, class U>
        requires requires { typename T::void_pointer; }
    struct GetVoidPtr<T, U> {
        using type = typename T::void_pointer;
    };
    template<class T, class U> struct GetConstVoidPtr {
        using type = typename GetRebindPtr<U, const void*>::type;
    };
    template<class T, class U>
        requires requires { typename T::const_void_pointer; }
    struct GetConstVoidPtr<T, U> {
        using type = typename T::const_void_pointer;
    };
    template<class T> struct GetDifferenceTypeSub {
        using type = itype::i32;
    };
    template<class T>
        requires requires { typename T::difference_type; }
    struct GetDifferenceTypeSub<T> {
        using type = typename T::difference_type;
    };
    template<class T, class U> struct GetDifferenceType {
        using type = typename GetDifferenceTypeSub<U>::type;
    };
    template<class T, class U>
        requires requires { typename T::difference_type; }
    struct GetDifferenceType<T, U> {
        using type = typename T::difference_type;
    };
    template<class T, class U> struct GetSizeType {
        using type = std::make_unsigned_t<U>;
    };
    template<class T, class U>
        requires requires { typename T::size_type; }
    struct GetSizeType<T, U> {
        using type = typename T::size_type;
    };
    template<class T> struct IsPropCopy {
        using type = std::false_type;
    };
    template<class T>
        requires requires { typename T::propagate_on_container_copy_assignment; }
    struct IsPropCopy<T> {
        using type = typename T::propagate_on_container_copy_assignment;
    };
    template<class T> struct IsPropMove {
        using type = std::false_type;
    };
    template<class T>
        requires requires { typename T::propagate_on_container_move_assignment; }
    struct IsPropMove<T> {
        using type = typename T::propagate_on_container_move_assignment;
    };
    template<class T> struct IsPropSwap {
        using type = std::false_type;
    };
    template<class T>
        requires requires { typename T::propagate_on_container_swap; }
    struct IsPropSwap<T> {
        using type = typename T::propagate_on_container_swap;
    };
    template<class T> struct IsAlwaysEqual {
        using type = typename std::is_empty<T>::type;
    };
    template<class T>
        requires requires { typename T::is_always_equal; }
    struct IsAlwaysEqual<T> {
        using type = typename T::is_always_equal;
    };
    template<class T, class U> struct RebindAlloc {
        using type = typename internal::RepFirst<T, U>::type;
    };
    template<class T, class U>
        requires requires { typename T::template rebind<U>::other; }
    struct RebindAlloc<T, U> {
        using type = typename T::template rebind<U>::other;
    };
}  // namespace internal

template<class Alloc> class AllocatorTraits {
public:
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename internal::GetPtr<Alloc, value_type>::type;
    using const_pointer = typename internal::GetConstPtr<Alloc, pointer, value_type>::type;
    using void_pointer = typename internal::GetVoidPtr<Alloc, pointer>::type;
    using const_void_pointer = typename internal::GetConstVoidPtr<Alloc, pointer>::type;
    using difference_type = typename internal::GetDifferenceType<Alloc, pointer>::type;
    using size_type = typename internal::GetSizeType<Alloc, difference_type>::type;
    using propagate_on_container_copy_assignment = typename internal::IsPropCopy<Alloc>::type;
    using propagate_on_container_move_assignment = typename internal::IsPropMove<Alloc>::type;
    using propagate_on_container_swap = typename internal::IsPropSwap<Alloc>::type;
    using is_always_equal = typename internal::IsAlwaysEqual<Alloc>::type;
    template<class U> using rebind_alloc = typename internal::RebindAlloc<Alloc, U>::type;
    template<class U> using rebind_traits = AllocatorTraits<typename internal::RebindAlloc<Alloc, U>::type>;
private:
    constexpr static bool with_hint = requires(Alloc& a, size_type n, const_void_pointer hint) { a.allocate(n, hint); };
    constexpr static bool aligned_with_hint = requires(Alloc& a, size_type n, std::align_val_t align, const_void_pointer hint) { a.allocate(n, align, hint); };
    template<class... Args> constexpr static bool constructible = requires(Alloc& a, pointer p, Args&&... args) { a.construct(p, std::forward<Args>(args)...); };
    constexpr static bool destructible = requires(Alloc& a, pointer p) { a.destroy(p); };
    constexpr static bool selectable = requires(Alloc& a) { a.select_on_container_copy_construction(); };
public:
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n) noexcept(noexcept(a.allocate(n))) { return a.allocate(n); }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, std::align_val_t align) noexcept(noexcept(a.allocate(n, align))) { return a.allocate(n, align); }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint) noexcept(noexcept(a.allocate(n, hint)))
        requires(with_hint)
    {
        return a.allocate(n, hint);
    }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer) noexcept(noexcept(a.allocate(n)))
        requires(!with_hint)
    {
        return a.allocate(n);
    }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, std::align_val_t align, const_void_pointer hint) noexcept(noexcept(a.allocate(n, align, hint)))
        requires(aligned_with_hint)
    {
        return a.allocate(n, align, hint);
    }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, std::align_val_t align, const_void_pointer) noexcept(noexcept(a.allocate(align, n)))
        requires(!aligned_with_hint)
    {
        return a.allocate(align, n);
    }
    static constexpr void deallocate(Alloc& a, pointer p, size_type n) noexcept(noexcept(a.deallocate(p, n))) { a.deallocate(p, n); }
    static constexpr void deallocate(Alloc& a, pointer p, size_type n, std::align_val_t align) noexcept(noexcept(a.deallocate(p, n, align))) { a.deallocate(p, n, align); }
    static constexpr size_type max_size(const Alloc& a) noexcept {
        if constexpr (requires { a.max_size(); }) return a.max_size();
        else return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
    template<class T, class... Args> static constexpr void construct(Alloc& a, T* p, Args&&... args) noexcept(noexcept(a.construct(p, std::forward<Args>(args)...)))
        requires(constructible<Args...>)
    {
        a.construct(p, std::forward<Args>(args)...);
    }
    template<class T, class... Args> static constexpr void construct(Alloc&, T* p, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
        requires(!constructible<Args...>)
    {
        std::construct_at(p, std::forward<Args>(args)...);
    }
    template<class T> static constexpr void destroy(Alloc& a, T* p) noexcept(noexcept(a.destroy(p)))
        requires(destructible)
    {
        return a.destroy(p);
    }
    template<class T> static constexpr void destroy(Alloc&, T* p) noexcept(std::is_nothrow_destructible_v<T>)
        requires(!destructible)
    {
        return std::destroy_at(p);
    }
    static constexpr Alloc select_on_container_copy_construction(const Alloc& a) noexcept(noexcept(a.select_on_container_copy_construction()))
        requires(selectable)
    {
        return a.select_on_container_copy_construction();
    }
    static constexpr Alloc select_on_container_copy_construction(const Alloc& a) noexcept(std::is_nothrow_copy_constructible_v<Alloc>)
        requires(!selectable)
    {
        return a;
    }
};

template<class T> class Allocator {
public:
    using value_type = T;
    using propagate_on_container_move_assignment = std::true_type;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using is_always_equal = std::true_type;
    constexpr Allocator() noexcept {}
    constexpr Allocator(const Allocator&) noexcept {}
    template<class U> constexpr Allocator(const Allocator<U>&) noexcept {}
    [[nodiscard]] constexpr T* allocate(size_type n) {
        if (std::is_constant_evaluated()) return std::allocator<T>().allocate(n);
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(alignof(T))));
        else return static_cast<T*>(::operator new(sizeof(T) * n));
    }
    [[nodiscard]] T* allocate(size_type n, std::align_val_t align) { return static_cast<T*>(::operator new(sizeof(T) * n, align)); }
    constexpr void deallocate(T* p, [[maybe_unused]] size_type n) noexcept {
        if (std::is_constant_evaluated()) return std::allocator<T>().deallocate(p, n);
#ifdef __cpp_sized_deallocation
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, n, static_cast<std::align_val_t>(alignof(T)));
        else ::operator delete(p, n);
#else
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, static_cast<std::align_val_t>(alignof(T)));
        else ::operator delete(p);
#endif
    }
    void deallocate(T* p, [[maybe_unused]] size_type n, std::align_val_t align) noexcept {
#ifdef __cpp_sized_deallocation
        ::operator delete(p, n, align);
#else
        ::operator delete(p, align);
#endif
    }
    constexpr Allocator& operator=(const Allocator&) = default;
    template<class U> friend constexpr bool operator==(const Allocator&, const Allocator<U>&) noexcept { return true; }
};

template<itype::u32 Size> class MemoryPool {
    template<class T> friend class PoolAllocator;
    itype::u32 cnt = 0;
    itype::u32 ref = 0;
    ctype::c8 buf[Size];
public:
    constexpr ~MemoryPool() noexcept(false) {
        if (ref != 0) throw Exception("gsh::MemoryPool::~MemoryPool / There are some gsh::PoolAllocator tied to this object have not yet been destroyed.");
    }
};
template<class T> class PoolAllocator {
    template<class U> friend class PoolAllocator;
    itype::u32* cnt;
    itype::u32* ref;
    ctype::c8 *buf, *end;
public:
    using value_type = T;
    using propagate_on_container_copy_assignmant = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using is_always_equal = std::false_type;
    constexpr PoolAllocator() noexcept : cnt(nullptr), ref(nullptr), buf(nullptr), end(nullptr) {}
    constexpr PoolAllocator(const PoolAllocator& a) noexcept : cnt(a.cnt), ref(a.ref), buf(a.buf), end(a.end) { ++*ref; }
    template<class U> constexpr PoolAllocator(const PoolAllocator<U>& a) noexcept : cnt(a.cnt), ref(a.ref), buf(a.buf), end(a.end) { ++*ref; }
    template<itype::u32 Size> constexpr PoolAllocator(MemoryPool<Size>& p) noexcept : cnt(&p.cnt), ref(&p.ref), buf(p.buf), end(p.buf + Size) { ++*ref; }
    constexpr ~PoolAllocator() noexcept {
        if (ref != nullptr) --*ref;
    }
    [[nodiscard]] constexpr T* allocate(size_type n) {
        if (std::is_constant_evaluated()) return Allocator<T>().allocate(n);
        constexpr itype::u32 align = __STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(T) ? alignof(T) : __STDCPP_DEFAULT_NEW_ALIGNMENT__;
        void* ptr = static_cast<void*>(buf + *cnt);
        std::size_t space = end - static_cast<ctype::c8*>(ptr);
        std::align(align, sizeof(T) * n, ptr, space);
        if (ptr == nullptr) throw Exception("gsh::PoolAllocator::allocate / Failed to allocate memory.");
        T* res = static_cast<T*>(ptr);
        *cnt = static_cast<ctype::c8*>(ptr) - buf;
        return res;
    }
    [[nodiscard]] T* allocate(size_type n, std::align_val_t align) {
        void* ptr = static_cast<void*>(buf + *cnt);
        std::size_t space = end - static_cast<ctype::c8*>(ptr);
        std::align(static_cast<std::size_t>(align), sizeof(T) * n, ptr, space);
        if (ptr == nullptr) throw Exception("gsh::PoolAllocator::allocate / Failed to allocate memory.");
        T* res = static_cast<T*>(ptr);
        *cnt = static_cast<ctype::c8*>(ptr) - buf;
        return res;
    }
    constexpr void deallocate(T* p, [[maybe_unused]] size_type n) noexcept {
        if (std::is_constant_evaluated()) return Allocator<T>().deallocate(p, n);
    }
    void deallocate(T*, size_type, std::align_val_t) noexcept {}
    constexpr PoolAllocator& operator=(const PoolAllocator& a) noexcept {
        if (ref != nullptr) --*ref;
        cnt = a.cnt, ref = a.ref, buf = a.buf, end = a.end;
        ++*ref;
        return *this;
    }
    template<itype::u32 Size> constexpr PoolAllocator& operator=(MemoryPool<Size>& p) noexcept {
        if (ref != nullptr) --*ref;
        cnt = &p.cnt, ref = &p.ref, buf = p.buf, end = p.buf + Size;
        ++*ref;
        return *this;
    }
    template<class U> friend constexpr bool operator==(const PoolAllocator& a, const PoolAllocator<U>& b) noexcept { return a.cnt == b.cnt && a.ref == b.ref && a.buf == b.buf && a.end == b.end; }
};

template<class T> class SingleAllocator {
    T* buffer[24] = {};
    itype::u32 x = 0xffffffff, y = 0;
    T** del = nullptr;
    itype::u32 end = 0;
    [[no_unique_address]] Allocator<T> alloc;
    [[no_unique_address]] Allocator<T*> del_alloc;
    using traits = AllocatorTraits<Allocator<T>>;
    using del_alloc_traits = AllocatorTraits<Allocator<T*>>;
public:
    using value_type = T;
    using propagate_on_container_copy_assignmant = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using is_always_equal = std::false_type;
    constexpr SingleAllocator() noexcept {}
    constexpr SingleAllocator(const SingleAllocator&) noexcept {}
    template<class U> constexpr SingleAllocator(const SingleAllocator<U>&) noexcept {}
    constexpr ~SingleAllocator() noexcept {
        for (itype::u32 i = 0; i != x + 1; ++i) traits::deallocate(alloc, buffer[i], 1 << i);
        if (del != nullptr) del_alloc_traits::deallocate(del_alloc, del, (1u << (x + 1)) - 1);
    }
    constexpr SingleAllocator& operator=(const SingleAllocator&) noexcept {}
    constexpr T* allocate(itype::u32) {
        if (y == (1u << (x + 1)) >> 1) {
            if (end != 0) [[likely]] {
                return del[--end];
            } else {
                x += 1, y = 0;
                buffer[x] = traits::allocate(alloc, 1 << x);
                T** new_del = del_alloc_traits::allocate(del_alloc, (1 << (x + 1)) - 1);
                if (del != nullptr) [[likely]] {
                    for (itype::u32 i = 0; i != end; ++i) new_del[i] = del[i];
                    del_alloc_traits::deallocate(del_alloc, del, (1 << x) - 1);
                }
                del = new_del;
            }
        }
        return &buffer[x][y++];
    }
    constexpr void deallocate(T* p, itype::u32) noexcept { del[end++] = p; }
    constexpr itype::u32 max_size() const noexcept { return (1 << 24) - 1; }
    constexpr SingleAllocator select_on_container_copy_construction() const noexcept { return {}; }
    template<class U> friend constexpr bool operator==(const SingleAllocator&, const SingleAllocator<U>&) noexcept { return false; }
};

template<class Alloc> class SharedAllocator {
    static inline Alloc alloc;
    using traits = AllocatorTraits<Alloc>;
public:
    using value_type = typename traits::value_type;
    using propagate_on_container_copy_assignmant = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using size_type = typename traits::size_type;
    using difference_type = typename traits::difference_type;
    using is_always_equal = std::true_type;
    using allocator_type = Alloc;
    template<class U> class rebind {
    public:
        ~rebind() = delete;
        using other = SharedAllocator<typename traits::template rebind_alloc<U>>;
    };
    constexpr SharedAllocator() noexcept {}
    constexpr SharedAllocator(const SharedAllocator&) noexcept {}
    template<class T> constexpr SharedAllocator(const SharedAllocator<T>&) noexcept {}
    constexpr SharedAllocator& operator=(const SharedAllocator&) noexcept {}
    template<class... Args> auto allocate(Args&&... args) noexcept(noexcept(alloc.allocate(std::forward<Args>(args)...))) { return alloc.allocate(std::forward<Args>(args)...); }
    template<class... Args> void deallocate(Args&&... args) noexcept(noexcept(alloc.deallocate(std::forward<Args>(args)...))) { return alloc.deallocate(std::forward<Args>(args)...); }
    size_type max_size() const noexcept { return alloc.max_size(); }
    static Alloc& get_allocator() noexcept { return alloc; }
    template<class T> friend constexpr bool operator==(const SharedAllocator&, const SharedAllocator<T>&) noexcept { return true; }
};

template<class Alloc> class ConstexprAllocator {
    [[no_unique_address]] Alloc alloc;
    using traits = AllocatorTraits<Alloc>;
public:
    using allocator_type = Alloc;
    using value_type = typename traits::value_type;
    using pointer = typename traits::pointer;
    using const_pointer = typename traits::const_pointer;
    using void_pointer = typename traits::void_pointer;
    using const_void_pointer = typename traits::const_void_pointer;
    using difference_type = typename traits::difference_type;
    using size_type = typename traits::size_type;
    using propagate_on_container_copy_assignment = typename traits::propagate_on_container_copy_assignment;
    using propagate_on_container_move_assignment = typename traits::propagate_on_container_move_assignment;
    using propagate_on_container_swap = typename traits::propagate_on_container_swap;
    using is_always_equal = typename traits::is_always_equal;
    template<class U> class rebind {
    public:
        ~rebind() = delete;
        using other = ConstexprAllocator<typename traits::template rebind_alloc<U>>;
    };
    constexpr ConstexprAllocator() noexcept(noexcept(Alloc())) {}
    constexpr ConstexprAllocator(const ConstexprAllocator&) noexcept(std::is_nothrow_copy_constructible_v<Alloc>) = default;
    constexpr ConstexprAllocator(ConstexprAllocator&&) noexcept(std::is_nothrow_move_constructible_v<Alloc>) = default;
    template<class U> constexpr ConstexprAllocator(const ConstexprAllocator<U>& a) noexcept(std::is_nothrow_constructible_v<Alloc, const U&>) : alloc(a.alloc) {}
    template<class U> constexpr ConstexprAllocator(ConstexprAllocator<U>&& a) noexcept(std::is_nothrow_constructible_v<Alloc, U&&>) : alloc(std::move(a.alloc)) {}
    constexpr ConstexprAllocator& operator=(const ConstexprAllocator& a) noexcept(std::is_nothrow_copy_assignable_v<Alloc>) {
        alloc = a.alloc;
        return *this;
    }
    constexpr ConstexprAllocator& operator=(ConstexprAllocator&& a) noexcept(std::is_nothrow_move_assignable_v<Alloc>) {
        alloc = std::move(a.alloc);
        return *this;
    }
    template<class... Args> constexpr auto allocate(Args&&... args) noexcept(noexcept(alloc.allocate(std::forward<Args>(args)...))) {
        if (std::is_constant_evaluated()) return Allocator<value_type>().allocate(std::forward<Args>(args)...);
        else return alloc.allocate(std::forward<Args>(args)...);
    }
    template<class... Args> constexpr void deallocate(Args&&... args) noexcept(noexcept(alloc.deallocate(std::forward<Args>(args)...))) {
        if (std::is_constant_evaluated()) return Allocator<value_type>().deallocate(std::forward<Args>(args)...);
        else return alloc.deallocate(std::forward<Args>(args)...);
    }
    constexpr size_type max_size() const noexcept {
        if (std::is_constant_evaluated()) return AllocatorTraits<Allocator<value_type>>::max_size();
        else return alloc.max_size();
    }
    constexpr Alloc& get_allocator() noexcept { return alloc; }
    template<class U> friend constexpr bool operator==(const ConstexprAllocator& a, const ConstexprAllocator<U>& b) noexcept(noexcept(a.alloc == b.alloc)) { return a.alloc == b.alloc; }
};

}  // namespace gsh


namespace gsh {

template<class T, class Allocator = Allocator<T>>
    requires std::is_same_v<T, typename AllocatorTraits<Allocator>::value_type> && (!std::is_const_v<T>)
class Vec : public ViewInterface<Vec<T, Allocator>, T> {
    using traits = AllocatorTraits<Allocator>;
public:
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename traits::pointer;
    using const_pointer = typename traits::const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    [[no_unique_address]] allocator_type alloc;
    pointer ptr = nullptr;
    size_type len = 0, cap = 0;
public:
    constexpr Vec() noexcept(noexcept(Allocator())) {}
    constexpr explicit Vec(const allocator_type& a) noexcept : alloc(a) {}
    constexpr explicit Vec(size_type n, const Allocator& a = Allocator()) : alloc(a) {
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n, cap = n;
        for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i);
    }
    constexpr explicit Vec(const size_type n, const value_type& value, const allocator_type& a = Allocator()) : alloc(a) {
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n, cap = n;
        for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i, value);
    }
    template<std::input_iterator InputIter> constexpr Vec(const InputIter first, const InputIter last, const allocator_type& a = Allocator()) : alloc(a) {
        const size_type n = std::distance(first, last);
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n, cap = n;
        size_type i = 0;
        for (InputIter itr = first; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
    }
    constexpr Vec(const Vec& x) : Vec(x, traits::select_on_container_copy_construction(x.alloc)) {}
    constexpr Vec(Vec&& x) noexcept : alloc(std::move(x.alloc)), ptr(x.ptr), len(x.len), cap(x.cap) { x.ptr = nullptr, x.len = 0, x.cap = 0; }
    constexpr Vec(const Vec& x, const allocator_type& a) : alloc(a), len(x.len), cap(x.len) {
        if (len == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, cap);
        for (size_type i = 0; i != len; ++i) traits::construct(alloc, ptr + i, *(x.ptr + i));
    }
    constexpr Vec(Vec&& x, const allocator_type& a) : alloc(a) {
        if (traits::is_always_equal || x.get_allocator() == a) {
            ptr = x.ptr, len = x.len, cap = x.cap;
            x.ptr = nullptr, x.len = 0, x.cap = 0;
        } else {
            if (x.len == 0) [[unlikely]]
                return;
            len = x.len, cap = x.cap;
            ptr = traits::allocate(alloc, len);
            for (size_type i = 0; i != len; ++i) traits::construct(alloc, ptr + i, std::move(*(x.ptr + i)));
            traits::deallocate(x.alloc, x.ptr, x.cap);
            x.ptr = nullptr, x.len = 0, x.cap = 0;
        }
    }
    constexpr Vec(std::initializer_list<value_type> il, const allocator_type& a = Allocator()) : Vec(il.begin(), il.end(), a) {}
    template<ForwardRange R>
        requires(!std::same_as<Vec, std::remove_cvref_t<R>>)
    constexpr Vec(R&& r, const allocator_type& a = Allocator()) : Vec(RangeTraits<R>::begin(r), RangeTraits<R>::end(r), a) {}
    constexpr ~Vec() {
        if (cap != 0) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, cap);
        }
    }
    constexpr Vec& operator=(const Vec& x) {
        if (&x == this) return *this;
        for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        if (traits::propagate_on_container_copy_assignment::value || cap < x.len) {
            if (cap != 0) traits::deallocate(alloc, ptr, cap);
            if constexpr (traits::propagate_on_container_copy_assignment::value) alloc = x.alloc;
            cap = x.len;
            ptr = traits::allocate(alloc, cap);
        }
        len = x.len;
        for (size_type i = 0; i != len; ++i) *(ptr + i) = *(x.ptr + i);
        return *this;
    }
    constexpr Vec& operator=(Vec&& x) noexcept(traits::propagate_on_container_move_assignment::value || traits::is_always_equal::value) {
        if (&x == this) return *this;
        if (cap != 0) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, cap);
        }
        if constexpr (traits::propagate_on_container_move_assignment::value) alloc = std::move(x.alloc);
        ptr = x.ptr, len = x.len, cap = x.cap;
        x.ptr = nullptr, x.len = 0, x.cap = 0;
        return *this;
    }
    constexpr Vec& operator=(std::initializer_list<value_type> init) {
        assign(init.begin(), init.end());
        return *this;
    }
    constexpr iterator begin() noexcept { return ptr; }
    constexpr const_iterator begin() const noexcept { return ptr; }
    constexpr iterator end() noexcept { return ptr + len; }
    constexpr const_iterator end() const noexcept { return ptr + len; }
    constexpr const_iterator cbegin() const noexcept { return ptr; }
    constexpr const_iterator cend() const noexcept { return ptr + len; }
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(ptr + len); }
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(ptr + len); }
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(ptr); }
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(ptr); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(ptr + len); }
    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(ptr); }
    constexpr size_type size() const noexcept { return len; }
    constexpr size_type max_size() const noexcept {
        const auto tmp = traits::max_size(alloc);
        return tmp < 2147483647 ? tmp : 2147483647;
    }
    constexpr void resize(const size_type sz) {
        if (cap < sz) {
            const pointer new_ptr = traits::allocate(alloc, sz);
            if (cap != 0) {
                for (size_type i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
                traits::deallocate(alloc, ptr, cap);
            }
            ptr = new_ptr;
            for (size_type i = len; i != sz; ++i) traits::construct(alloc, ptr + i);
            len = sz, cap = sz;
        } else if (len < sz) {
            for (size_type i = len; i != sz; ++i) traits::construct(alloc, ptr + i);
            len = sz;
        } else {
            for (size_type i = sz; i != len; ++i) traits::destroy(alloc, ptr + i);
            len = sz;
        }
    }
    constexpr void resize(const size_type sz, const value_type& c) {
        if (cap < sz) {
            const pointer new_ptr = traits::allocate(sz);
            if (cap != 0) {
                for (size_type i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
                traits::deallocate(alloc, ptr, cap);
            }
            ptr = new_ptr;
            for (size_type i = len; i != sz; ++i) traits::construct(alloc, *(ptr + i), c);
            len = sz, cap = sz;
        } else if (len < sz) {
            for (size_type i = len; i != sz; ++i) traits::construct(alloc, *(ptr + i), c);
            len = sz;
        } else {
            for (size_type i = sz; i != len; ++i) traits::destroy(alloc, ptr + i);
            len = sz;
        }
    }
    constexpr size_type capacity() const noexcept { return cap; }
    [[nodiscard]] constexpr bool empty() const noexcept { return len == 0; }
    constexpr void reserve(const size_type n) {
        if (n > cap) {
            const pointer new_ptr = traits::allocate(alloc, n);
            if (cap != 0) {
                for (size_type i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
                traits::deallocate(alloc, ptr, cap);
            }
            ptr = new_ptr, cap = n;
        }
    }
    constexpr void shrink_to_fit() {
        if (len == 0) {
            if (cap != 0) traits::deallocate(alloc, ptr, cap);
            ptr = nullptr, cap = 0;
            return;
        }
        if (len != cap) {
            const pointer new_ptr = traits::allocate(alloc, len);
            for (size_type i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, cap);
            ptr = new_ptr, cap = len;
        }
    }
    GSH_INTERNAL_INLINE constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Vec::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
        Assume(n < len);
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Vec::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
        Assume(n < len);
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr reference at(const size_type n) {
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Vec::at / The index is out of range. ( n=", n, ", size=", len, " )");
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr const_reference at(const size_type n) const {
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Vec::at / The index is out of range. ( n=", n, ", size=", len, " )");
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr reference at_unchecked(const size_type n) noexcept {
        Assume(n < len);
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr const_reference at_unchecked(const size_type n) const noexcept {
        Assume(n < len);
        return *(ptr + n);
    }
    constexpr pointer data() noexcept { return ptr; }
    constexpr const_pointer data() const noexcept { return ptr; }
    constexpr reference front() noexcept { return *ptr; }
    constexpr const_reference front() const noexcept { return *ptr; }
    constexpr reference back() noexcept { return *(ptr + len - 1); }
    constexpr const_reference back() const noexcept { return *(ptr + len - 1); }
    template<std::ranges::range R> constexpr void assign(R&& r) {
        const size_type n = std::ranges::size(r);
        if (n > cap) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, cap);
            ptr = traits::allocate(alloc, n);
            cap = n;
            auto itr = std::ranges::begin(r);
            for (size_type i = 0; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
        } else if (n > len) {
            size_type i = 0;
            auto itr = std::ranges::begin(r);
            for (; i != len; ++itr, ++i) *(ptr + i) = *itr;
            for (; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
        } else {
            for (size_type i = n; i != len; ++i) traits::destroy(alloc, ptr + i);
            auto itr = std::ranges::begin(r);
            for (size_type i = 0; i != n; ++itr, ++i) *(ptr + i) = *itr;
        }
        len = n;
    }
    constexpr void assign(const size_type n, const value_type& t) {
        if (n > cap) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, cap);
            ptr = traits::allocate(alloc, n);
            cap = n;
            for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i, t);
        } else if (n > len) {
            size_type i = 0;
            for (; i != len; ++i) *(ptr + i) = t;
            for (; i != n; ++i) traits::construct(alloc, ptr + i, t);
        } else {
            for (size_type i = n; i != len; ++i) traits::destroy(alloc, ptr + i);
            for (size_type i = 0; i != n; ++i) *(ptr + i) = t;
        }
        len = n;
    }
    constexpr void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
private:
    constexpr void extend_one() {
        if (len == cap) {
            const pointer new_ptr = traits::allocate(alloc, cap * 2 + 8);
            if (cap != 0) {
                for (size_type i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
                traits::deallocate(alloc, ptr, cap);
            }
            ptr = new_ptr, cap = cap * 2 + 8;
        }
    }
public:
    constexpr void push_back(const T& x) {
        extend_one();
        traits::construct(alloc, ptr + (len++), x);
    }
    constexpr void push_back(T&& x) {
        extend_one();
        traits::construct(alloc, ptr + (len++), std::move(x));
    }
    template<class... Args> constexpr reference emplace_back(Args&&... args) {
        extend_one();
        traits::construct(alloc, ptr + len, std::forward<Args>(args)...);
        return *(ptr + (len++));
    }
    constexpr void pop_back() {
#ifndef NDEBUG
        if (len == 0) [[unlikely]]
            throw gsh::Exception("gsh::Vec::pop_back / The container is empty.");
#endif
        traits::destroy(alloc, ptr + (--len));
    }
    /*
    constexpr iterator insert(const const_iterator position, const value_type& x);
    constexpr iterator insert(const const_iterator position, value_type&& x);
    constexpr iterator insert(const const_iterator position, const size_type n, const value_type& x);
    template<class InputIter> constexpr iterator insert(const const_iterator position, const InputIter first, const InputIter last);
    constexpr iterator insert(const const_iterator position, const std::initializer_list<value_type> il);
    template<class... Args> constexpr iterator emplace(const_iterator position, Args&&... args);
    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);
    */
    constexpr void swap(Vec& x) noexcept(traits::propagate_on_container_swap::value || traits::is_always_equal::value) {
        using std::swap;
        swap(ptr, x.ptr);
        swap(len, x.len);
        swap(cap, x.cap);
        if constexpr (traits::propagate_on_container_swap::value) swap(alloc, x.alloc);
    }
    constexpr void clear() {
        for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        len = 0;
    }
    constexpr void reset() {
        if (cap != 0) {
            traits::deallocate(alloc, ptr, cap);
            ptr = nullptr, len = 0, cap = 0;
        }
    }
    constexpr allocator_type get_allocator() const noexcept { return alloc; }
    friend constexpr bool operator==(const Vec& x, const Vec& y) {
        if (x.len != y.len) return false;
        bool res = true;
        for (size_type i = 0; i != x.len;) {
            const bool f = *(x.ptr + i) == *(y.ptr + i);
            res &= f;
            i = f ? i + 1 : x.len;
        }
        return res;
    }
    friend constexpr auto operator<=>(const Vec& x, const Vec& y) { return std::lexicographical_compare_three_way(x.begin(), x.end(), y.begin(), y.end()); }
    friend constexpr void swap(Vec& x, Vec& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
template<std::input_iterator InputIter, class Alloc = Allocator<typename std::iterator_traits<InputIter>::value_type>> Vec(InputIter, InputIter, Alloc = Alloc()) -> Vec<typename std::iterator_traits<InputIter>::value_type, Alloc>;
template<Range R, class Alloc = Allocator<typename RangeTraits<R>::value_type>> Vec(R, Alloc = Alloc()) -> Vec<typename RangeTraits<R>::value_type, Alloc>;


}  // namespace gsh


namespace gsh {

/*
template<class T, class Comp = Less, class Alloc = ConstexprAllocator<SharedAllocator<SingleAllocator<T>>>> class SkewHeap {
    struct node {
        using traits = AllocatorTraits<typename AllocatorTraits<Alloc>::template rebind_alloc<node>>;
        T x;
        node *l, *r;
        constexpr node(const T& x_, node* l_, node* r_) noexcept(std::is_nothrow_copy_constructible_v<T>) : x(x_), l(l_), r(r_) {}
        constexpr node(T&& x_, node* l_, node* r_) noexcept(std::is_nothrow_move_constructible_v<T>) : x(std::move(x_)), l(l_), r(r_) {}
        constexpr node* copy(auto& alloc) {
            node* res = traits::allocate(alloc, 1);
            traits::construct(alloc, res, x, nullptr, nullptr);
            if (l != nullptr) res->l = l->copy(alloc);
            if (r != nullptr) res->r = r->copy(alloc);
            return res;
        }
        constexpr node* move(auto& alloc) {
            node* res = traits::allocate(alloc, 1);
            traits::construct(alloc, std::move(res), x, nullptr, nullptr);
            if (l != nullptr) res->l = l->copy(alloc);
            if (r != nullptr) res->r = r->copy(alloc);
            return res;
        }
        constexpr void destroy(auto& alloc) noexcept {
            if (l != nullptr) {
                l->destroy(alloc);
                traits::destroy(alloc, l);
                traits::deallocate(alloc, l, 1);
            }
            if (r != nullptr) {
                r->destroy(alloc);
                traits::destroy(alloc, r);
                traits::deallocate(alloc, r, 1);
            }
        }
    };
    GSH_INTERNAL_INLINE constexpr static node* merge_nodes(auto& comp, node* p, node* q) noexcept(std::is_nothrow_invocable_v<decltype(comp), const T&, const T&>) {
        if (p == nullptr) return q;
        if (q == nullptr) return p;
        node* res = nullptr;
        node *curp = p, *curq = q;
        node** prev = &res;
        while (curp != nullptr) {
            if (Invoke(comp, static_cast<const T&>(curq->x), static_cast<const T&>(curp->x))) [[unlikely]] {
                auto tmp = curp;
                curp = curq, curq = tmp;
            }
            node* tmp = curp->r;
            curp->r = curp->l;
            *prev = curp;
            prev = &(curp->l);
            curp = tmp;
        }
        *prev = curq;
        return res;
    }
    [[no_unique_address]] AllocatorTraits<Alloc>::template rebind_alloc<node> node_alloc;
    using traits = AllocatorTraits<decltype(node_alloc)>;
    [[no_unique_address]] Comp comp_func;
    node* root = nullptr;
    itype::u32 sz = 0;
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using compare_type = Comp;
    using allocator_type = Alloc;
    constexpr SkewHeap() noexcept {}
    constexpr explicit SkewHeap(const Comp& comp, const Alloc& alloc = Alloc()) : node_alloc(alloc), comp_func(comp) {}
    constexpr explicit SkewHeap(const Alloc& alloc) : node_alloc(alloc) {}
    template<class InputIterator> constexpr SkewHeap(InputIterator first, InputIterator last, const Comp& comp = Comp(), const Alloc& alloc = Alloc()) : node_alloc(alloc), comp_func(comp) {}
    template<class InputIterator> SkewHeap(InputIterator first, InputIterator last, const Alloc& alloc) : SkewHeap(first, last, Comp(), alloc) {}
    constexpr SkewHeap(const SkewHeap& x) : node_alloc(traits::select_on_container_copy_construction(x.node_alloc)), comp_func(x.comp_func), sz(x.sz) {
        if (x.root != nullptr) root = x.root->copy(node_alloc);
    }
    constexpr SkewHeap(SkewHeap&& y) : node_alloc(std::move(y.node_alloc)), comp_func(std::move(y.comp_func)), root(y.root), sz(y.sz) { y.root = nullptr; }
    constexpr SkewHeap(const SkewHeap& x, const Alloc& alloc) : node_alloc(alloc), comp_func(x.comp_func), sz(x.sz) {
        if (x.root != nullptr) root = x.root->copy(node_alloc);
    }
    constexpr SkewHeap(SkewHeap&& y, const Alloc& alloc) : node_alloc(alloc), comp_func(y.comp_func), sz(y.sz) {
        if constexpr (typename traits::is_always_equal()) root = y.root, y.root = nullptr;
        else if (node_alloc == y.node_alloc) root = y.root, y.root = nullptr;
        else root = y.root->move(node_alloc);
    }
    constexpr SkewHeap(std::initializer_list<value_type> init, const Comp& comp = Comp(), const Alloc& alloc = Alloc()) : SkewHeap(init.begin(), init.end(), comp, alloc) {}
    constexpr SkewHeap(std::initializer_list<value_type> init, const Alloc& alloc) : SkewHeap(init.begin(), init.end(), Comp(), alloc) {}
    constexpr ~SkewHeap() noexcept {
        if (root != nullptr) {
            root->destroy(node_alloc);
            traits::destroy(node_alloc, root);
            traits::deallocate(node_alloc, root, 1);
        }
    }
    constexpr void push(const T& x) {
        node* p = traits::allocate(node_alloc, 1);
        traits::construct(node_alloc, p, x, nullptr, nullptr);
        root = merge_nodes(comp_func, root, p);
        ++sz;
    }
    constexpr void push(T&& x) {
        node* p = traits::allocate(node_alloc, 1);
        traits::construct(node_alloc, p, std::move(x), nullptr, nullptr);
        root = merge_nodes(comp_func, root, p);
        ++sz;
    }
    constexpr void pop() {
        node *l = root->l, *r = root->r;
        traits::destroy(node_alloc, root);
        traits::deallocate(node_alloc, root, 1);
        root = merge_nodes(comp_func, l, r);
        --sz;
    }
    constexpr reference top() noexcept { return root->x; }
    constexpr const_reference top() const noexcept { return root->x; }
    constexpr itype::u32 size() const noexcept { return sz; }
    constexpr void merge(SkewHeap&& h) {
        root = merge_nodes(comp_func, root, h.root);
        h.root = nullptr;
    }
};
*/

template<class T, class Comp = Less, class Alloc = Allocator<T>> class DoubleEndedHeap {
    Vec<T, Alloc> data;
    [[no_unique_address]] Comp comp_func;
    itype::u32 mx = 0;
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using compare_type = Comp;
    using allocator_type = Alloc;
    constexpr DoubleEndedHeap() noexcept {}
    constexpr explicit DoubleEndedHeap(const Comp& comp, const Alloc& alloc = Alloc()) : data(alloc), comp_func(comp) {}
    constexpr explicit DoubleEndedHeap(const Alloc& alloc) : data(alloc) {}
    template<class InputIterator> constexpr DoubleEndedHeap(InputIterator first, InputIterator last, const Comp& comp = Comp(), const Alloc& alloc = Alloc()) : data(first, last, alloc), comp_func(comp) { make_heap(); }
    template<class InputIterator> DoubleEndedHeap(InputIterator first, InputIterator last, const Alloc& alloc) : data(first, last, alloc) { make_heap(); }
    constexpr DoubleEndedHeap(const DoubleEndedHeap& x) = default;
    constexpr DoubleEndedHeap(DoubleEndedHeap&& y) noexcept = default;
    constexpr DoubleEndedHeap(const DoubleEndedHeap& x, const Alloc& alloc) : data(x.data, alloc), comp_func(x.comp_func), mx(x.mx) {}
    constexpr DoubleEndedHeap(DoubleEndedHeap&& y, const Alloc& alloc) : data(std::move(y.data), alloc), comp_func(y.comp_func), mx(y.mx) {}
    constexpr DoubleEndedHeap(std::initializer_list<value_type> init, const Comp& comp = Comp(), const Alloc& alloc = Alloc()) : data(init, alloc), comp_func(comp) { make_heap(); }
    constexpr DoubleEndedHeap(std::initializer_list<value_type> init, const Alloc& alloc) : data(init, alloc) { make_heap(); }
    constexpr DoubleEndedHeap& operator=(const DoubleEndedHeap&) = default;
    constexpr DoubleEndedHeap& operator=(DoubleEndedHeap&&) noexcept(std::is_nothrow_move_assignable_v<Comp>) = default;
private:
    constexpr static bool nothrow_op = std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T> && std::is_nothrow_invocable_v<Comp, T&, T&>;
    GSH_INTERNAL_INLINE constexpr bool is_min_level(itype::u32 idx) const noexcept {
        Assume(idx + 1 != 0);
        return std::bit_width(idx + 1) & 1;
    }
    GSH_INTERNAL_INLINE constexpr void set_mx() noexcept(nothrow_op) {
        if (data.size() >= 3) [[likely]]
            mx = 1 + Invoke(comp_func, data[1], data[2]);
        else mx = data.size() == 2;
    }
    template<bool Min, bool SetMax> GSH_INTERNAL_INLINE constexpr void push_down(itype::u32 idx) noexcept(nothrow_op) {
        itype::u32 lim = (data.size() + 1) / 4 - 1;
        auto comp = [&](auto&& a, auto&& b) GSH_INTERNAL_INLINE {
            if constexpr (Min) return static_cast<bool>(Invoke(comp_func, a, b));
            else return static_cast<bool>(Invoke(comp_func, b, a));
        };
        itype::u32 cur = idx;
        T tmp = std::move(data[idx]);
        while (true) {
            itype::u32 grdch = (cur + 1) * 4 - 1;
            if (cur >= lim) [[unlikely]] {
                itype::u32 ch = (cur + 1) * 2 - 1;
                if (grdch < data.size()) [[unlikely]] {
                    itype::u32 m = ch + comp(data[ch + 1], data[ch]);
                    switch (data.size() - grdch) {
                    case 3 :
                        {
                            itype::u32 n = grdch + 1 + comp(data[grdch + 2], data[grdch + 1]);
                            m = comp(data[m], data[grdch]) ? m : grdch;
                            m = comp(data[m], data[n]) ? m : n;
                            break;
                        }
                    case 2 :
                        {
                            itype::u32 n = grdch + comp(data[grdch + 1], data[grdch]);
                            m = comp(data[m], data[n]) ? m : n;
                            break;
                        }
                    case 1 :
                        {
                            m = comp(data[m], data[grdch]) ? m : grdch;
                            break;
                        }
                    default : Unreachable();
                    };
                    if (m < grdch) {
                        if (comp(data[m], tmp)) {
                            data[cur] = std::move(data[m]);
                            data[m] = std::move(tmp);
                        } else {
                            data[cur] = std::move(tmp);
                        }
                    } else {
                        itype::u32 p = (m + 1) / 2 - 1;
                        if (comp(data[m], tmp)) {
                            data[cur] = std::move(data[m]);
                            if (comp(data[p], tmp)) {
                                data[m] = std::move(data[p]);
                                data[p] = std::move(tmp);
                            } else {
                                data[m] = std::move(tmp);
                            }
                        } else {
                            data[cur] = std::move(tmp);
                        }
                    }
                } else if (ch >= data.size()) [[likely]] {
                    data[cur] = std::move(tmp);
                } else if (ch < data.size() - 1) [[likely]] {
                    bool f = comp(data[ch + 1], data[ch]);
                    T m = std::move(f ? data[ch + 1] : data[ch]);
                    bool g = comp(m, tmp);
                    data[cur] = std::move(g ? m : tmp);
                    data[ch + f] = std::move(g ? tmp : m);
                } else if (comp(data[ch], tmp)) {
                    data[cur] = std::move(data[ch]);
                    data[ch] = std::move(tmp);
                } else {
                    data[cur] = std::move(tmp);
                }
                if constexpr (SetMax) {
                    Assume(data.size() >= 3);
                    set_mx();
                }
                return;
            }
            itype::u32 a = grdch + comp(data[grdch + 1], data[grdch]);
            itype::u32 b = grdch + 2 + comp(data[grdch + 3], data[grdch + 2]);
            itype::u32 c = a + comp(data[b], data[a]) * (b - a);
            itype::u32 p = (c + 1) / 2 - 1;
            if (!comp(data[c], tmp)) {
                data[cur] = std::move(tmp);
                if constexpr (SetMax) {
                    Assume(data.size() >= 3);
                    set_mx();
                }
                return;
            }
            data[cur] = std::move(data[c]);
            cur = c;
            bool f = comp(data[p], tmp);
            T tmp2 = data[p];
            data[p] = std::move(f ? tmp : tmp2);
            tmp = std::move(f ? tmp2 : tmp);
        }
    }
    GSH_INTERNAL_INLINE constexpr void pop_front_impl() noexcept(nothrow_op) {
        if (data.size() <= 3) [[unlikely]] {
            switch (data.size()) {
            case 0 : break;
            case 1 : mx = 0; break;
            case 2 :
                {
                    if (Invoke(comp_func, data[1], data[0])) {
                        auto tmp = std::move(data[0]);
                        data[0] = std::move(data[1]);
                        data[1] = std::move(tmp);
                    }
                    mx = 1;
                    break;
                }
            case 3 :
                {
                    itype::u32 m = 1 + Invoke(comp_func, data[2], data[1]);
                    if (Invoke(comp_func, data[m], data[0])) {
                        auto tmp = std::move(data[0]);
                        data[0] = std::move(data[m]);
                        data[m] = std::move(tmp);
                    }
                    mx = 1 + Invoke(comp_func, data[1], data[2]);
                    break;
                }
            default : Unreachable();
            }
            return;
        }
        push_down<true, true>(0);
    }
    GSH_INTERNAL_INLINE constexpr void pop_back_impl() noexcept(nothrow_op) {
        if (data.size() <= 3) [[unlikely]] {
            set_mx();
            return;
        }
        push_down<false, true>(mx);
    }
    constexpr void make_heap() noexcept(nothrow_op) {
        if (data.size() <= 1) [[unlikely]]
            return;
        itype::u32 lim1 = data.size() / 2;
        if (data.size() % 2 == 0) {
            --lim1;
            itype::u32 ch = (lim1 + 1) * 2 - 1;
            if (Invoke(comp_func, data[lim1], data[ch]) ^ is_min_level(lim1)) {
                auto tmp = std::move(data[lim1]);
                data[lim1] = std::move(data[ch]);
                data[ch] = std::move(tmp);
            }
        }
        itype::u32 lim2 = data.size() / 4;
        Assume(lim2 + 1 != 0);
        itype::u32 lr = std::bit_floor(lim2 + 1) * 2 - 1;
        lr = lim1 < lr ? lim1 : lr;
        bool lim2_min = is_min_level(lim2);
        for (itype::u32 i = lim2_min ? lim2 : lr, j = lim2_min ? lr : lim1; i < j; ++i) {
            itype::u32 ch = (i + 1) * 2 - 1;
            itype::u32 m = ch + static_cast<bool>(Invoke(comp_func, data[ch + 1], data[ch]));
            bool f = Invoke(comp_func, data[i], data[m]);
            T tmp1 = std::move(data[i]);
            T tmp2 = std::move(data[m]);
            data[i] = std::move(f ? tmp1 : tmp2);
            data[m] = std::move(f ? tmp2 : tmp1);
        }
        for (itype::u32 i = lim2_min ? lr : lim2, j = lim2_min ? lim1 : lr; i < j; ++i) {
            itype::u32 ch = (i + 1) * 2 - 1;
            itype::u32 m = ch + static_cast<bool>(Invoke(comp_func, data[ch], data[ch + 1]));
            bool f = Invoke(comp_func, data[m], data[i]);
            T tmp1 = std::move(data[i]);
            T tmp2 = std::move(data[m]);
            data[i] = std::move(f ? tmp1 : tmp2);
            data[m] = std::move(f ? tmp2 : tmp1);
        }
        for (itype::u32 i = lim2; i--;) {
            if (is_min_level(i)) {
                push_down<true, false>(i);
            } else {
                push_down<false, false>(i);
            }
        }
        set_mx();
    }
    constexpr void push_up() noexcept(nothrow_op) {
        const itype::u32 idx = data.size() - 1;
        if (idx <= 2) [[unlikely]] {
            if (Invoke(comp_func, data[idx], data[0])) {
                auto tmp = std::move(data[idx]);
                data[idx] = std::move(data[0]);
                data[0] = std::move(tmp);
            }
            set_mx();
            return;
        }
        itype::u32 p = ((idx + 1) >> 1) - 1;
        if (is_min_level(idx)) {
            if (Invoke(comp_func, data[p], data[idx])) {
                // push_up_max(p)
                T tmp = std::move(data[idx]);
                data[idx] = std::move(data[p]);
                itype::u32 cur = p;
                while (cur > 2 && Invoke(comp_func, data[p = ((cur + 1) / 4) - 1], tmp)) {
                    data[cur] = std::move(data[p]);
                    cur = p;
                }
                data[cur] = std::move(tmp);
                Assume(data.size() >= 3);
                set_mx();
            } else {
                // push_up_min(idx)
                T tmp = std::move(data[idx]);
                itype::u32 cur = idx;
                while (Invoke(comp_func, tmp, data[p = ((cur + 1) / 4) - 1])) {
                    data[cur] = std::move(data[p]);
                    cur = p;
                    if (cur == 0) [[unlikely]]
                        break;
                }
                data[cur] = std::move(tmp);
            }
        } else {
            if (Invoke(comp_func, data[idx], data[p])) {
                // push_up_min(p)
                T tmp = std::move(data[idx]);
                data[idx] = std::move(data[p]);
                itype::u32 cur = p;
                while (cur != 0 && Invoke(comp_func, tmp, data[p = ((cur + 1) / 4) - 1])) {
                    data[cur] = std::move(data[p]);
                    cur = p;
                }
                data[cur] = std::move(tmp);
            } else {
                // push_up_max(idx)
                T tmp = std::move(data[idx]);
                itype::u32 cur = idx;
                while (Invoke(comp_func, data[p = ((cur + 1) / 4) - 1], tmp)) {
                    data[cur] = std::move(data[p]);
                    cur = p;
                    if (cur <= 2) [[unlikely]] {
                        data[cur] = std::move(tmp);
                        Assume(data.size() >= 3);
                        set_mx();
                        return;
                    }
                }
                data[cur] = std::move(tmp);
            }
        }
    }
public:
    template<Range R> constexpr void assign(R&& r) {
        data.assign(std::forward<R>(r));
        make_heap();
    }
    constexpr const_reference top() const noexcept { return data[0]; }
    constexpr const_reference front() const noexcept { return data[0]; }
    constexpr const_reference back() const noexcept { return data[mx]; }
    [[nodiscard]] constexpr bool empty() const noexcept { return data.empty(); }
    constexpr itype::u32 size() const noexcept { return data.size(); }
    constexpr void reserve(itype::u32 n) { data.reserve(n); }
    constexpr void push(const T& x) {
        data.push_back(x);
        push_up();
    }
    constexpr void push(T&& x) {
        data.push_back(std::move(x));
        push_up();
    }
    template<class... Args> constexpr void emplace(Args&&... args) {
        data.emplace_back(std::forward<Args>(args)...);
        push_up();
    }
    constexpr void pop() noexcept(nothrow_op) { pop_front(); }
    constexpr void pop_front() noexcept(nothrow_op) {
        data[0] = std::move(data.back());
        data.pop_back();
        pop_front_impl();
    }
    constexpr void pop_back() noexcept(nothrow_op) {
        data[mx] = std::move(data.back());
        data.pop_back();
        pop_back_impl();
    }
    constexpr void replace(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) { replace_front(x); }
    constexpr void replace(T&& x) noexcept(nothrow_op) { replace_front(std::move(x)); }
    constexpr void replace_front(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
        data[0] = x;
        pop_front_impl();
    }
    constexpr void replace_front(T&& x) noexcept(nothrow_op) {
        data[0] = std::move(x);
        pop_front_impl();
    }
    constexpr void replace_back(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
        data[mx] = x;
        pop_back_impl();
    }
    constexpr void replace_back(T&& x) noexcept(nothrow_op) {
        data[mx] = std::move(x);
        pop_back_impl();
    }
    constexpr void pushpop(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) { pushpop_front(x); }
    constexpr void pushpop(T&& x) noexcept(nothrow_op) { pushpop_front(std::move(x)); }
    constexpr void pushpop_front(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
        if (Invoke(comp_func, data[0], x)) {
            data[0] = x;
            pop_front_impl();
        }
    }
    constexpr void pushpop_front(T&& x) noexcept(nothrow_op) {
        if (Invoke(comp_func, data[0], x)) {
            data[0] = std::move(x);
            pop_front_impl();
        }
    }
    constexpr void pushpop_back(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
        if (Invoke(comp_func, x, data[mx])) {
            data[mx] = x;
            pop_back_impl();
        }
    }
    constexpr void pushpop_back(T&& x) noexcept(nothrow_op) {
        if (Invoke(comp_func, x, data[mx])) {
            data[mx] = std::move(x);
            pop_back_impl();
        }
    }
};

}  // namespace gsh

#include <source_location>  // std::source_location


#define ALL(V)       std::ranges::begin(V), std::ranges::end(V)
#define RALL(V)      std::ranges::rbegin(V), std::ranges::rend(V)
#define ALLMID(V, n) std::ranges::begin(V), std::ranges::next(std::ranges::begin(V), n), std::ranges::end(V)
#define VALUE(...)   (([&]() __VA_ARGS__)())
// clang-format off
#define RET_WITH(...) { __VA_ARGS__; return; } []{}
#define RETV_WITH(val, ...) { __VA_ARGS__; return val; } []{}
#define BRK_WITH(...) { __VA_ARGS__; break; } []{}
#define CTN_WITH(...) { __VA_ARGS__; continue; } []{}
#define EXT_WITH(...) { __VA_ARGS__; std::exit(0); } []{}

#define GSH_INTERNAL_ARGS0() ()
#define GSH_INTERNAL_ARGS1(a) (auto&& a)
#define GSH_INTERNAL_ARGS2(a, b) (auto&& a, auto&& b)
#define GSH_INTERNAL_ARGS3(a, b, c) (auto&& a, auto&& b, auto&& c)
#define GSH_INTERNAL_ARGS4(a, b, c, d) (auto&& a, auto&& b, auto&& c, auto&& d)
#define GSH_INTERNAL_ARGS5(a, b, c, d, e) (auto&& a, auto&& b, auto&& c, auto&& d, auto&& e)
#define GSH_INTERNAL_ARGS6(a, b, c, d, e, f) (auto&& a, auto&& b, auto&& c, auto&& d, auto&& e, auto&& f)
#define GSH_INTERNAL_ARGS7(a, b, c, d, e, f, g) (auto&& a, auto&& b, auto&& c, auto&& d, auto&& e, auto&& f, auto&& g)
#define GSH_INTERNAL_ARGS(...) GSH_INTERNAL_SELECT8(__VA_ARGS__, GSH_INTERNAL_ARGS7, GSH_INTERNAL_ARGS6, GSH_INTERNAL_ARGS5, GSH_INTERNAL_ARGS4, GSH_INTERNAL_ARGS3, GSH_INTERNAL_ARGS2, GSH_INTERNAL_ARGS1, GSH_INTERNAL_ARGS0)(__VA_ARGS__)
#define GSH_INTERNAL_LAMBDA1(...) [&]() { return (__VA_ARGS__); }
#define GSH_INTERNAL_LAMBDA2(args, ...) [&] GSH_INTERNAL_ARGS##args { return (__VA_ARGS__); }
#define LAMBDA(...) GSH_INTERNAL_SELECT3(__VA_ARGS__, GSH_INTERNAL_LAMBDA2, GSH_INTERNAL_LAMBDA1)(__VA_ARGS__)
// clang-format on

/*
#define REP(var_name, ...)  for ([[maybe_unused]] auto var_name : gsh::Step(__VA_ARGS__))
#define RREP(var_name, ...) for (auto var_name : gsh::Step(__VA_ARGS__) | std::ranges::reverse)
*/

namespace gsh {
namespace internal {
    void AssertPrint(const ctype::c8* message, std::source_location loc) {
        BasicWriter<2048> w(2);
        w.write("\e[2m[from gsh::internal::Assert] \e[0mDuring the execution of \e[1m\e[3m'");
        w.write(loc.function_name());
        w.write("'\e[0m\n");
        w.write(loc.file_name());
        w.write(':');
        w.write(loc.line());
        w.write(':');
        w.write(loc.column());
        w.write(':');
        w.write(" \e[31mAssertion Failed:\e[0m \e[1m\e[3m'");
        w.write(message);
        w.write("'\e[0m\n");
        w.reload();
    }
    GSH_INTERNAL_INLINE constexpr void Assert(const bool cond, const ctype::c8* message, std::source_location loc = std::source_location::current()) {
        if (!cond) [[unlikely]] {
            if (std::is_constant_evaluated()) {
                throw 0;
            } else {
                AssertPrint(message, loc);
                std::exit(1);
            }
        }
    }
}  // namespace internal
}  // namespace gsh
#define GSH_INTERNAL_ASSERT1(cond)          gsh::internal::Assert(cond, #cond)
#define GSH_INTERNAL_ASSERT2(cond, message) gsh::internal::Assert(cond, message)
#define ASSERT(...)                         GSH_INTERNAL_SELECT3(__VA_ARGS__, GSH_INTERNAL_ASSERT2, GSH_INTERNAL_ASSERT1)(__VA_ARGS__)

#include <ctime>   // std::clock_t, std::clock, CLOCKS_PER_SEC


namespace gsh {

class Timer {
    std::clock_t start_time;
public:
    Timer() { start_time = std::clock(); }
    void restart() { start_time = std::clock(); }
    itype::u64 elapsed() const { return static_cast<itype::u64>(std::clock() - start_time) * 1000 / CLOCKS_PER_SEC; }
};

template<> class Formatter<Timer> {
public:
    template<class Stream> void operator()(Stream&& stream, const Timer& t) {
        Formatter<itype::u64>{}(stream, t.elapsed());
        Formatter<ctype::c8>{}(stream, 'm');
        Formatter<ctype::c8>{}(stream, 's');
    }
};

}  // namespace gsh


namespace gsh {

namespace internal {
    constexpr itype::u64 Splitmix(itype::u64 x) {
        itype::u64 z = (x + 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
}  // namespace internal

// @brief 64bit pseudo random number generator using xoroshiro128+
class Rand64 {
    itype::u64 s0, s1;
public:
    using result_type = itype::u64;
    static constexpr itype::u32 word_size = sizeof(result_type) * 8;
    static constexpr result_type default_seed = 0xcafef00dd15ea5e5;
    constexpr Rand64() : Rand64(default_seed) {}
    constexpr explicit Rand64(result_type value) : s0(value), s1(internal::Splitmix(value)) {}
    constexpr result_type operator()() {
        itype::u64 t0 = s0, t1 = s1;
        const itype::u64 res = t0 + t1;
        t1 ^= t0;
        s0 = std::rotr(t0, 9) ^ t1 ^ (t1 << 14);
        s1 = std::rotr(t1, 28);
        return res;
    };
    constexpr void discard(itype::u64 z) {
        for (itype::u64 i = 0; i < z; ++i) operator()();
    }
    static constexpr result_type max() { return 18446744073709551615u; }
    static constexpr result_type min() { return 0; }
    constexpr void seed(result_type value = default_seed) { s0 = value, s1 = internal::Splitmix(value); }
    friend constexpr bool operator==(Rand64 x, Rand64 y) { return x.s0 == y.s0 && x.s1 == y.s1; }
};

// @brief 32bit pseudo random number generator using Permuted congruential generator
class Rand32 {
    itype::u64 val;
public:
    using result_type = itype::u32;
    static constexpr itype::u32 word_size = sizeof(result_type) * 8;
    static constexpr result_type default_seed = 0xcafef00d;
    constexpr Rand32() : Rand32(default_seed) {}
    constexpr explicit Rand32(result_type value) : val(internal::Splitmix((itype::u64) value << 32 | value)) {}
    constexpr result_type operator()() {
        itype::u64 x = val;
        const itype::i32 count = x >> 61;
        val = x * 0xcafef00dd15ea5e5;
        x ^= x >> 22;
        return x >> (22 + count);
    };
    constexpr void discard(itype::u64 z) {
        itype::u64 pow = 0xcafef00dd15ea5e5;
        while (z != 0) {
            if (z & 1) val *= pow;
            z >>= 1;
            pow *= pow;
        }
    }
    static constexpr result_type max() { return 4294967295u; }
    static constexpr result_type min() { return 0; }
    constexpr void seed(result_type value = default_seed) { val = internal::Splitmix((itype::u64) value << 32 | value); }
    friend constexpr bool operator==(Rand32 x, Rand32 y) { return x.val == y.val; }
};

// @brief Generate random numbers from std::time, std::clock, and std::source_location.
class RandomDevice {
    Rand64 engine;
    constexpr itype::u64 from_time() {
        if (!std::is_constant_evaluated()) {
            itype::u64 a = internal::Splitmix(static_cast<itype::u64>(std::time(nullptr)));
            itype::u64 b = Hash{}(internal::Splitmix(static_cast<itype::u64>(std::clock())));
            return a ^ b;
        } else return 0x9e3779b97f4a7c15;
    }
    constexpr itype::u64 from_compile_time() {
        itype::u64 a = internal::Splitmix(Hash{}(internal::HashBytes(__DATE__)));
        itype::u64 b = Hash{}(internal::Splitmix(internal::HashBytes(__TIME__)));
        itype::u64 c = internal::Splitmix(internal::Splitmix(internal::HashBytes(__TIMESTAMP__)));
        return internal::Splitmix(internal::MixIntegers(a, c)) ^ b;
    }
    constexpr itype::u64 from_location(const std::source_location& loc) {
        itype::u64 a = Hash{}(internal::Splitmix(loc.column()));
        itype::u64 b = internal::Splitmix(loc.line());
        itype::u64 c = Hash{}(internal::HashBytes(loc.file_name()));
        itype::u64 d = internal::HashBytes(loc.function_name());
        return internal::MixIntegers(a, d) ^ internal::Splitmix(internal::MixIntegers(b, c));
    }
    constexpr itype::u64 get_val(const std::source_location& loc) { return internal::Splitmix(from_time()) ^ from_location(loc) ^ (Hash{}(from_compile_time())); }
public:
    using result_type = itype::u64;
    constexpr RandomDevice(const std::source_location& loc = std::source_location::current()) : engine(internal::Splitmix(get_val(loc))) {}
    constexpr RandomDevice(const RandomDevice&) = default;
    constexpr RandomDevice& operator=(const RandomDevice&) = default;
    constexpr ftype::f64 entropy() const noexcept { return 0.0; }
    static constexpr result_type max() { return 0xffffffffffffffff; }
    static constexpr result_type min() { return 0; }
    constexpr result_type operator()(const std::source_location& loc = std::source_location::current()) { return engine() ^ get_val(loc); }
};

template<itype::u32 Size, class URBG> class RandBuffer : public URBG {
    typename URBG::result_type buf[Size];
    itype::u32 x, cnt;
public:
    constexpr RandBuffer() { init(); }
    constexpr explicit RandBuffer(URBG::result_type value) : URBG(value) { init(); }
    constexpr void reload() { x = Invoke(static_cast<URBG&>(*this)), cnt = 0; }
    constexpr void init() {
        for (itype::u32 i = 0; i != Size; ++i) buf[i] = Invoke(static_cast<URBG&>(*this));
        x = Invoke(static_cast<URBG&>(*this)), cnt = 0;
    }
    constexpr URBG::result_type operator()() { return x ^ buf[cnt++]; }
};
template<itype::u32 Size> using RandBuffer32 = RandBuffer<Size, Rand32>;
template<itype::u32 Size> using RandBuffer64 = RandBuffer<Size, Rand64>;

// @brief Generate 32bit uniform random numbers in [0, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u32 Uniform32(URBG&& g, itype::u32 max) {
    return (static_cast<itype::u64>(Invoke(g) & 4294967295u) * max) >> 32;
}
// @brief Generate 32bit uniform random numbers in [min, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u32 Uniform32(URBG&& g, itype::u32 min, itype::u32 max) {
    return static_cast<itype::u32>((static_cast<itype::u64>(Invoke(g) & 4294967295u) * (max - min)) >> 32) + min;
}
// @brief Generate 64bit uniform random numbers in [0, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u64 Uniform64(URBG&& g, itype::u64 max) {
    return (static_cast<itype::u128>(Invoke(g)) * max) >> 64;
}
// @brief Generate 64bit uniform random numbers in [min, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u64 Uniform64(URBG&& g, itype::u64 min, itype::u64 max) {
    return static_cast<itype::u64>((static_cast<itype::u128>(Invoke(g)) * (max - min)) >> 64) + min;
}

template<RandomAccessRange R, class URBG> constexpr void Shuffle(R&& r, URBG&& g) {
    itype::u32 sz = std::ranges::size(r);
    auto itr = std::ranges::begin(r);
    for (itype::u32 i = 0; i != sz; ++i, ++itr) {
        std::ranges::swap(*itr, *std::ranges::next(itr, Uniform32(g, sz - i)));
    }
}


template<class URBG> constexpr itype::u32 UnbiasedUniform32(URBG&& g, itype::u32 max) {
    itype::u32 mask = ~0u;
    --max;
    mask >>= std::countl_zero(max | 1);
    itype::u32 x;
    do {
        x = Invoke(g) & mask;
    } while (x > max);
    return x;
}
template<class URBG> constexpr itype::u32 UnbiasedUniform32(URBG&& g, itype::u32 min, itype::u32 max) {
    return min + UnbiasedUniform32(g, max - min);
}
template<class URBG> constexpr itype::u64 UnbiasedUniform64(URBG&& g, itype::u64 max) {
    itype::u64 mask = ~0ull;
    --max;
    mask >>= std::countl_zero(max | 1);
    itype::u64 x;
    do {
        x = Invoke(g) & mask;
    } while (x > max);
    return x;
}
template<class URBG> constexpr itype::u32 UnbiasedUniform64(URBG&& g, itype::u64 min, itype::u64 max) {
    return min + UnbiasedUniform64(g, max - min);
}

//https://speakerdeck.com/hole/rand01?slide=31
template<class URBG> constexpr ftype::f32 Canocicaled32(URBG&& g) {
    return std::bit_cast<ftype::f32>((127u << 23) | (static_cast<itype::u32>(Invoke(g)) & 0x7fffff)) - 1.0f;
}
template<class URBG> constexpr ftype::f32 Uniformf32(URBG&& g, ftype::f32 max) {
    return Canocicaled32(g) * max;
}
template<class URBG> constexpr ftype::f32 Uniformf32(URBG&& g, ftype::f32 min, ftype::f32 max) {
    return Canocicaled32(g) * (max - min) + min;
}
template<class URBG> constexpr ftype::f64 Canocicaled64(URBG&& g) {
    return std::bit_cast<ftype::f64>((1023ull << 52) | (static_cast<itype::u64>(Invoke(g)) & 0xfffffffffffffull)) - 1.0;
}
template<class URBG> constexpr ftype::f64 Uniformf64(URBG&& g, ftype::f64 max) {
    return Canocicaled64(g) * max;
}
template<class URBG> constexpr ftype::f64 Uniformf64(URBG&& g, ftype::f64 min, ftype::f64 max) {
    return Canocicaled64(g) * (max - min) + min;
}

template<class URBG> constexpr bool Bernoulli(URBG&& g) {
    return Invoke(g) & 1;
}
template<class URBG> constexpr bool Bernoulli32(URBG&& g, ftype::f32 p) {
    return Canocicaled32(g) < p;
}
template<class URBG> constexpr bool Bernoulli64(URBG&& g, ftype::f64 p) {
    return Canocicaled64(g) < p;
}

}  // namespace gsh

#include <queue>

#if 0 && !defined ONLINE_JUDGE
#include <fcntl.h>
gsh::BasicReader rd(open("Test/in.txt", O_RDONLY));
gsh::BasicWriter wt(open("Test/out.txt", O_WRONLY | O_TRUNC));
#else
#if defined(ONLINE_JUDGE)
gsh::MmapReader rd;
#else
gsh::BasicReader rd;
#endif
#if 1 || !defined ONLINE_JUDGE
gsh::BasicWriter wt;
#else
#define NO_WT
#endif
#endif
void Main() {
    using namespace std;
    using namespace gsh;
    using namespace gsh::itype;
    using namespace gsh::ftype;
    using namespace gsh::ctype;
    auto [N, Q] = rd.read<u8dig, u8dig>();
    DoubleEndedHeap<i32> q;
    q.reserve(1000000);
    if (N == 0) rd.skip(1);
    q.assign(std::views::iota(0u, N) | views::transform([&](itype::u32) -> i32 { return rd.read<i32>().val(); }));
    for (u32 i = 0; i != Q; ++i) {
        c8 t = rd.read<c8>();
        if (t == '0') {
            q.push(rd.read<i32>().val());
        } else if (t == '1') {
            wt.writeln(q.front());
            q.pop_front();
        } else {
            wt.writeln(q.back());
            q.pop_back();
        }
    }
}
int main() {
#ifdef ONLINE_JUDGE
    Main();
#ifndef NO_WT
    wt.reload();
#endif
#else
    try {
        Main();
        wt.reload();
    } catch (gsh::Exception& e) {
        wt.writeln("gsh::Exception was throwed:", e.what());
        wt.reload();
        return 1;
    }
#endif
}
