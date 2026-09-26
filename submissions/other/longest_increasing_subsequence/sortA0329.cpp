
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
#endif

#include <cstdlib>  // std::exit
#include <cstring>  // std::memcpy, std::memmove
#include <utility>  // std::forward
#include <tuple>    // std::tuple, std::tuple_cat
#if __has_include(<unistd.h>)
#include <unistd.h>  // read, write
#endif
#ifndef _WIN32
#include <sys/mman.h>  // mmap
#include <sys/stat.h>  // stat, fstat
#endif


namespace gsh {

namespace itype {
    using i8 = signed char;
    using u8 = unsigned char;
    using i16 = short;
    using u16 = unsigned short;
    using i32 = int;
    using u32 = unsigned;
    using ilong = long;
    using ulong = unsigned long;
    using i64 = long long;
    using u64 = unsigned long long;
#ifdef __SIZEOF_INT128__
    using i128 = __int128_t;
    using u128 = __uint128_t;
#endif
    using isize = i32;
    using usize = u32;
}  // namespace itype

namespace ftype {
    //using f16 = _Float16;
    using f32 = float;
    using f64 = double;
#ifdef __SIZEOF_FLOAT128__
    using f128 = __float128;
#endif
    using flong = long double;
}  // namespace ftype

namespace ctype {
    using c8 = char;
    using wc = wchar_t;
    using utf8 = char8_t;
    using utf16 = char16_t;
    using utf32 = char32_t;
}  // namespace ctype

namespace internal {
    template<class T, class U> constexpr bool IsSame = false;
    template<class T> constexpr bool IsSame<T, T> = true;
    template<class T, class U, class... V> constexpr bool IsSameAny = IsSame<T, U> || IsSameAny<T, V...>;
    template<class T, class U> constexpr bool IsSameAny<T, U> = IsSame<T, U>;
}  // namespace internal

namespace simd {

#if defined __GNUC__
    using i8x32 = __attribute__((vector_size(32))) itype::i8;
    using u8x32 = __attribute__((vector_size(32))) itype::u8;
    using i16x16 = __attribute__((vector_size(32))) itype::i16;
    using u16x16 = __attribute__((vector_size(32))) itype::u16;
    using i32x8 = __attribute__((vector_size(32))) itype::i32;
    using u32x8 = __attribute__((vector_size(32))) itype::u32;
    using i64x4 = __attribute__((vector_size(32))) itype::i64;
    using u64x4 = __attribute__((vector_size(32))) itype::u64;
    //using f16x16 = __attribute__((vector_size(32))) ftype::f16;
    using f32x8 = __attribute__((vector_size(32))) ftype::f32;
    using f64x4 = __attribute__((vector_size(32))) ftype::f64;

    using i8x64 = __attribute__((vector_size(64))) itype::i8;
    using u8x64 = __attribute__((vector_size(64))) itype::u8;
    using i16x32 = __attribute__((vector_size(64))) itype::i16;
    using u16x32 = __attribute__((vector_size(64))) itype::u16;
    using i32x16 = __attribute__((vector_size(64))) itype::i32;
    using u32x16 = __attribute__((vector_size(64))) itype::u32;
    using i64x8 = __attribute__((vector_size(64))) itype::i64;
    using u64x8 = __attribute__((vector_size(64))) itype::u64;
    //using f16x32 = __attribute__((vector_size(64))) ftype::f16;
    using f32x16 = __attribute__((vector_size(64))) ftype::f32;
    using f64x8 = __attribute__((vector_size(64))) ftype::f64;

    template<class T, class U> constexpr T VectorCast(U x) {
        return __builtin_convertvector(x, T);
    }

    template<class T> concept Is256BitVector = internal::IsSameAny<T, i8x32, i16x16, i32x8, i64x4, u8x32, u16x16, u32x8, u64x4, f32x8, f64x4>;
    template<class T> concept Is512BitVector = internal::IsSameAny<T, i8x64, i16x32, i32x16, i64x8, u8x64, u16x32, u32x16, u64x8, f32x16, f64x8>;
    template<class T> concept IsVector = Is256BitVector<T> || Is512BitVector<T>;
#endif

}  // namespace simd

}  // namespace gsh

#include <bit>          // std::countr_zero

#include <type_traits>  // std::is_constant_evaluated


#define GSH_INTERNAL_SELECT1(a, ...)                      a
#define GSH_INTERNAL_SELECT2(a, b, ...)                   b
#define GSH_INTERNAL_SELECT3(a, b, c, ...)                c
#define GSH_INTERNAL_SELECT4(a, b, c, d, ...)             d
#define GSH_INTERNAL_SELECT5(a, b, c, d, e, ...)          e
#define GSH_INTERNAL_SELECT6(a, b, c, d, e, f, ...)       f
#define GSH_INTERNAL_SELECT7(a, b, c, d, e, f, g, ...)    g
#define GSH_INTERNAL_SELECT8(a, b, c, d, e, f, g, h, ...) h

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
template<class T>
    requires std::is_scalar_v<T>
GSH_INTERNAL_INLINE constexpr void ForceCalc([[maybe_unused]] const T& v) {
#if defined __GNUC__ || defined __INTEL_COMPILER
    if (!std::is_constant_evaluated()) __asm__ volatile("" ::"r"(v));
#endif
}
template<class T, class... Args> GSH_INTERNAL_INLINE constexpr void ForceCalc(const T& v, const Args&... args) {
    ForceCalc(v);
    ForceCalc(args...);
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
    template<class Stream> constexpr itype::u16 operator()(Stream& stream) const {
        stream.reload(8);
        return internal::Parseu4dig(stream);
    }
};
template<> class Parser<itype::i4dig> {
public:
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
    template<class Stream> constexpr itype::u32 operator()(Stream& stream) const {
        stream.reload(16);
        return internal::Parseu8dig(stream);
    }
};
template<> class Parser<itype::i8dig> {
public:
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
    ctype::c8* s;
    itype::u32 n;
public:
    constexpr Parser(ctype::c8* s_, itype::u32 n_ = 0xffffffff) noexcept : s(s_), n(n_) {}
    template<class Stream> constexpr void operator()(Stream& stream) const {
        if (n == 0xffffffff) {
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
        } else {
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
                    return;
                }
                stream.reload();
                avail = stream.avail();
            }
            MemoryCopy(c, stream.current(), rem);
            c += rem;
            stream.skip(rem);
            *c = '\0';
        }
    }
};

}  // namespace gsh

#include <ranges>       // std::ranges::forward_range


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
            if (std::is_constant_evaluated()) {
                rem = n % 10000000000000000;
                return n / 10000000000000000;
            }
            itype::u64 res;
            __asm__("divq %[v]" : "=a"(res), "=d"(rem) : [v] "r"(10000000000000000), "a"(static_cast<itype::u64>(n)), "d"(static_cast<itype::u64>(n >> 64)));
            return res;
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
template<> class Formatter<bool> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, bool b) const {
        stream.reload(1);
        *stream.current() = '0' + b;
        stream.skip(1);
    }
};
template<> class Formatter<const ctype::c8*> {
    itype::u32 n;
public:
    constexpr Formatter() : n(0xffffffff) {}
    constexpr Formatter(itype::u32 len) : n(len) {}
    template<class Stream> constexpr void operator()(Stream& stream, const ctype::c8* s) const {
        itype::u32 len = n == 0xffffffff ? StrLen(s) : n;
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

namespace internal {
    template<class T, class U> constexpr bool FormatableTupleImpl = false;
    template<class T, std::size_t... I> constexpr bool FormatableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Formatter<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}  // namespace internal
template<class T> concept FormatableTuple = requires { std::tuple_size<T>::value; } && internal::FormatableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<FormatableTuple T> class Formatter<T> {
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
template<class R> concept FormatableRange = std::ranges::forward_range<R> && requires { sizeof(Formatter<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<FormatableRange R>
    requires(!FormatableTuple<R>)
class Formatter<R> {
    template<class Stream, class T, class... Args> constexpr void print(Stream&& stream, T&& r, Args&&... args) const {
        auto first = std::ranges::begin(r);
        auto last = std::ranges::end(r);
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
        } else if constexpr (simd::Is256BitVector<T>) {
            struct chars {
                ctype::c8 c[32];
            };
            auto y = std::bit_cast<chars>(x);
            return internal::HashBytes(y.c, 32);
        } else if constexpr (simd::Is512BitVector<T>) {
            struct chars {
                ctype::c8 c[64];
            };
            auto y = std::bit_cast<chars>(x);
            return internal::HashBytes(y.c, 64);
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
    template<class D> class IstreamInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
        constexpr const D& derived() const { return *static_cast<const D*>(this); }
    public:
        constexpr auto read() { return std::tuple{}; }
        template<class T, class... Types> constexpr auto read() {
            if constexpr (sizeof...(Types) == 0) return Parser<T>{}(derived());
            else if constexpr (sizeof...(Types) == 1) {
                auto res = Parser<T>{}(derived());
                return std::tuple_cat(std::tuple(std::move(res)), std::tuple(read<Types...>()));
            } else {
                auto res = Parser<T>{}(derived());
                return std::tuple_cat(std::tuple(std::move(res)), read<Types...>());
            }
        }
    };
    template<class D> class OstreamInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
        constexpr const D& derived() const { return *static_cast<const D*>(this); }
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

#include <algorithm>       // std::lower_bound
#include <cmath>           // std::sqrt
#include <limits>          // std::numeric_limits

#include <iterator>          // std::reverse_iterator, std::iterator_traits, std::input_iterator, std::distance

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
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n) { return a.allocate(n); }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, size_type align) { return a.allocate(align, n); }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint) {
        if constexpr (requires { a.allocate(n, hint); }) return a.allocate(n, hint);
        else return a.allocate(n);
    }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, size_type align, const_void_pointer hint) {
        if constexpr (requires { a.allocate(align, n, hint); }) return a.allocate(align, n, hint);
        else return a.allocate(align, n);
    }
    static constexpr void deallocate(Alloc& a, pointer p, size_type n) { a.deallocate(p, n); }
    static constexpr void deallocate(Alloc& a, pointer p, size_type n, size_type align) { a.deallocate(p, n, align); }
    [[nodiscard]] static constexpr pointer reallocate(Alloc& a, pointer p, size_type prev_size, size_type new_size) {
        if constexpr (requires { a.reallocate(p, prev_size, new_size); }) return a.reallocate(p, prev_size, new_size);
        else {
            deallocate(a, p, prev_size);
            return allocate(a, new_size, p);
        }
    }
    [[nodiscard]] static constexpr pointer reallocate(Alloc& a, pointer p, size_type prev_size, size_type new_size, size_type align) {
        if constexpr (requires { a.reallocate(p, prev_size, new_size, align); }) return a.reallocate(p, prev_size, new_size, align);
        else {
            deallocate(a, p, prev_size, align);
            return allocate(a, new_size, p, align);
        }
    }
    static constexpr size_type max_size(const Alloc& a) noexcept {
        if constexpr (requires { a.max_size(); }) return a.max_size();
        else return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
    template<class T, class... Args> static constexpr void construct(Alloc& a, T* p, Args&&... args) {
        if constexpr (requires { a.construct(p, std::forward<Args>(args)...); }) a.construct(p, std::forward<Args>(args)...);
        else std::construct_at(p, std::forward<Args>(args)...);
    }
    template<class T> static constexpr void destroy(Alloc& a, T* p) {
        if constexpr (requires { a.destroy(p); }) a.destroy(p);
        else std::destroy_at(p);
    }
    static constexpr Alloc select_on_container_copy_construction(const Alloc& a) {
        if constexpr (requires { a.select_on_container_copy_construction(); }) return a.select_on_container_copy_construction();
        else return a;
    }
};

template<class T> class Allocator {
public:
    using value_type = T;
    using propagate_on_container_move_assignment = std::true_type;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using is_always_equal = std::true_type;
    constexpr Allocator() noexcept = default;
    constexpr Allocator(const Allocator&) noexcept = default;
    template<class U> constexpr Allocator(const Allocator<U>&) noexcept {}
    [[nodiscard]] constexpr T* allocate(size_type n) {
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(alignof(T))));
        else return static_cast<T*>(::operator new(sizeof(T) * n));
    }
    [[nodiscard]] constexpr T* allocate(size_type n, size_type align) { return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(align))); }
    constexpr void deallocate(T* p, [[maybe_unused]] size_type n) {
#ifdef __cpp_sized_deallocation
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, n, static_cast<std::align_val_t>(alignof(T)));
        else ::operator delete(p, n);
#else
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, static_cast<std::align_val_t>(alignof(T)));
        else ::operator delete(p);
#endif
    }
    constexpr void deallocate(T* p, [[maybe_unused]] size_type n, size_type align) {
#ifdef __cpp_sized_deallocation
        ::operator delete(p, n, static_cast<std::align_val_t>(align));
#else
        ::operator delete(p, static_cast<std::align_val_t>(align));
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
        if (std::is_constant_evaluated()) {
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(alignof(T))));
            else return static_cast<T*>(::operator new(sizeof(T) * n));
        }
        constexpr itype::u32 align = __STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(T) ? alignof(T) : __STDCPP_DEFAULT_NEW_ALIGNMENT__;
        void* ptr = static_cast<void*>(buf + *cnt);
        std::size_t space = end - static_cast<ctype::c8*>(ptr);
        std::align(align, sizeof(T) * n, ptr, space);
        if (ptr == nullptr) throw Exception("gsh::PoolAllocator::allocate / Failed to allocate memory.");
        T* res = static_cast<T*>(ptr);
        *cnt = static_cast<ctype::c8*>(ptr) - buf;
        return res;
    }
    [[nodiscard]] constexpr T* allocate(size_type n, size_type align) {
        if (std::is_constant_evaluated()) return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(align)));
        void* ptr = static_cast<void*>(buf + *cnt);
        std::size_t space = end - static_cast<ctype::c8*>(ptr);
        std::align(align, sizeof(T) * n, ptr, space);
        if (ptr == nullptr) throw Exception("gsh::PoolAllocator::allocate / Failed to allocate memory.");
        T* res = static_cast<T*>(ptr);
        *cnt = static_cast<ctype::c8*>(ptr) - buf;
        return res;
    }
    constexpr void deallocate(T* p, [[maybe_unused]] size_type n) {
        if (std::is_constant_evaluated()) {
#ifdef __cpp_sized_deallocation
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, n, static_cast<std::align_val_t>(alignof(T)));
            else ::operator delete(p, n);
#else
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, static_cast<std::align_val_t>(alignof(T)));
            else ::operator delete(p);
#endif
        }
    }
    constexpr void deallocate(T* p, [[maybe_unused]] size_type n, size_type align) {
        if (std::is_constant_evaluated()) {
#ifdef __cpp_sized_deallocation
            ::operator delete(p, n, static_cast<std::align_val_t>(align));
#else
            ::operator delete(p, static_cast<std::align_val_t>(align));
#endif
        }
    }
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

}  // namespace gsh


namespace gsh {

template<class T>
    requires std::same_as<T, std::remove_cv_t<T>>
class ArrInitTag {};
template<class T = void> constexpr ArrInitTag<T> ArrInit;
class ArrNoInitTag {};
constexpr ArrNoInitTag ArrNoInit;

template<class T, class Allocator = Allocator<T>>
    requires std::same_as<T, typename AllocatorTraits<Allocator>::value_type> && std::same_as<T, std::remove_cv_t<T>>
class Arr : public ViewInterface<Arr<T, Allocator>, T> {
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
    size_type len = 0;
public:
    constexpr Arr() noexcept(noexcept(Allocator())) : Arr(Allocator()) {}
    constexpr explicit Arr(const allocator_type& a) noexcept : alloc(a) {}
    constexpr explicit Arr(size_type n, const allocator_type& a = Allocator()) : alloc(a) {
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n;
        for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i);
    }
    constexpr explicit Arr(ArrNoInitTag, size_type n, const allocator_type& a = Allocator()) : alloc(a) {
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n;
    }
    constexpr explicit Arr(const size_type n, const value_type& value, const allocator_type& a = Allocator()) : alloc(a) {
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n;
        for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i, value);
    }
    template<std::input_iterator InputIter> constexpr Arr(InputIter first, InputIter last, const allocator_type& a = Allocator()) : alloc(a) {
        const size_type n = std::distance(first, last);
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n;
        size_type i = 0;
        for (InputIter itr = first; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
    }
    constexpr Arr(const Arr& x) : Arr(x, traits::select_on_container_copy_construction(x.alloc)) {}
    constexpr Arr(Arr&& x) noexcept : alloc(std::move(x.alloc)), ptr(x.ptr), len(x.len) { x.ptr = nullptr, x.len = 0; }
    constexpr Arr(const Arr& x, const allocator_type& a) : alloc(a), len(x.len) {
        if (len == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, len);
        for (size_type i = 0; i != len; ++i) traits::construct(alloc, ptr + i, *(x.ptr + i));
    }
    constexpr Arr(Arr&& x, const allocator_type& a) : alloc(a) {
        if (traits::is_always_equal || x.get_allocator() == a) {
            ptr = x.ptr, len = x.len;
            x.ptr = nullptr, x.len = 0;
        } else {
            if (x.len == 0) [[unlikely]]
                return;
            len = x.len;
            ptr = traits::allocate(alloc, len);
            for (size_type i = 0; i != len; ++i) traits::construct(alloc, ptr + i, std::move(*(x.ptr + i)));
            traits::deallocate(x.alloc, x.ptr, x.len);
            x.ptr = nullptr, x.len = 0;
        }
    }
    constexpr Arr(std::initializer_list<value_type> il, const allocator_type& a = Allocator()) : Arr(il.begin(), il.end(), a) {}
    template<ForwardRange R>
        requires(!std::same_as<Arr, std::remove_cvref_t<R>>)
    constexpr Arr(R&& r, const allocator_type& a = Allocator()) : Arr(RangeTraits<R>::begin(r), RangeTraits<R>::end(r), a) {}
    constexpr ~Arr() {
        if (len != 0) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
    }
    constexpr Arr& operator=(const Arr& x) {
        if (&x == this) return *this;
        for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        if (traits::propagate_on_container_copy_assignment::value || len != x.len) {
            if (len != 0) traits::deallocate(alloc, ptr, len);
            if constexpr (traits::propagate_on_container_copy_assignment::value) alloc = x.alloc;
            ptr = traits::allocate(alloc, x.len);
        }
        len = x.len;
        for (size_type i = 0; i != len; ++i) *(ptr + i) = *(x.ptr + i);
        return *this;
    }
    constexpr Arr& operator=(Arr&& x) noexcept(traits::propagate_on_container_move_assignment::value || traits::is_always_equal::value) {
        if (&x == this) return *this;
        if (len != 0) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
        if constexpr (traits::propagate_on_container_move_assignment::value) alloc = std::move(x.alloc);
        ptr = x.ptr, len = x.len;
        x.ptr = nullptr, x.len = 0;
        return *this;
    }
    constexpr Arr& operator=(std::initializer_list<value_type> init) {
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
        if (len == sz) return;
        if (sz == 0) {
            clear();
            return;
        }
        const pointer new_ptr = traits::allocate(alloc, sz);
        const size_type mn = len < sz ? len : sz;
        if (len != 0) {
            for (size_type i = 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
        ptr = new_ptr;
        for (size_type i = len; i < sz; ++i) traits::construct(alloc, ptr + i);
        len = sz;
    }
    constexpr void resize(const size_type sz, const value_type& c) {
        if (len == sz) return;
        if (sz == 0) {
            clear();
            return;
        }
        const pointer new_ptr = traits::allocate(alloc, sz);
        const size_type mn = len < sz ? len : sz;
        if (len != 0) {
            for (size_type i = 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
        ptr = new_ptr;
        for (size_type i = len; i < sz; ++i) traits::construct(alloc, ptr + i, c);
        len = sz;
    }
    constexpr void resize(const size_type sz, ArrNoInitTag) {
        if (len == sz) return;
        if (sz == 0) {
            clear();
            return;
        }
        const pointer new_ptr = traits::allocate(alloc, sz);
        const size_type mn = len < sz ? len : sz;
        if (len != 0) {
            for (size_type i = 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
        ptr = new_ptr;
        len = sz;
    }
    [[nodiscard]] constexpr bool empty() const noexcept { return len == 0; }
    GSH_INTERNAL_INLINE constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr reference at(const size_type n) {
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::at / The index is out of range. ( n=", n, ", size=", len, " )");
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr const_reference at(const size_type n) const {
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::at / The index is out of range. ( n=", n, ", size=", len, " )");
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr reference at_unchecked(const size_type n) { return *(ptr + n); }
    GSH_INTERNAL_INLINE constexpr const_reference at_unchecked(const size_type n) const { return *(ptr + n); }
    constexpr pointer data() noexcept { return ptr; }
    constexpr const_pointer data() const noexcept { return ptr; }
    constexpr reference front() { return *ptr; }
    constexpr const_reference front() const { return *ptr; }
    constexpr reference back() { return *(ptr + len - 1); }
    constexpr const_reference back() const { return *(ptr + len - 1); }
    template<std::input_iterator InputIter> constexpr void assign(const InputIter first, const InputIter last) {
        const size_type n = std::distance(first, last);
        if (n == 0) {
            clear();
        } else if (len == n) {
            InputIter itr = first;
            for (size_type i = 0; i != len; ++itr, ++i) *(ptr + i) = *itr;
        } else {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
            ptr = traits::allocate(alloc, n);
            len = n;
            InputIter itr = first;
            for (size_type i = 0; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
        }
    }
    constexpr void assign(const size_type n, const value_type& t) {
        if (n == 0) {
            clear();
        } else if (len == n) {
            for (size_type i = 0; i != len; ++i) *(ptr + i) = t;
        } else {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
            ptr = traits::allocate(alloc, n);
            len = n;
            for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i, t);
        }
    }
    constexpr void assign(const size_type n, ArrNoInitTag) {
        clear();
        if (n != 0) {
            ptr = traits::allocator(alloc, n);
            len = n;
        }
    }
    constexpr void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
    constexpr void swap(Arr& x) noexcept(traits::propagate_on_container_swap::value || traits::is_always_equal::value) {
        using std::swap;
        swap(ptr, x.ptr);
        swap(len, x.len);
        if constexpr (traits::propagate_on_container_swap::value) swap(alloc, x.alloc);
    }
    constexpr void clear() {
        if (len != 0) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
            ptr = nullptr, len = 0;
        }
    }
    constexpr allocator_type get_allocator() const noexcept { return alloc; }
    friend constexpr bool operator==(const Arr& x, const Arr& y) {
        if (x.len != y.len) return false;
        bool res = true;
        for (size_type i = 0; i != x.len;) {
            const bool f = *(x.ptr + i) == *(y.ptr + i);
            res &= f;
            i = f ? i + 1 : x.len;
        }
        return res;
    }
    friend constexpr auto operator<=>(const Arr& x, const Arr& y) { return std::lexicographical_compare_three_way(x.begin(), x.end(), y.begin(), y.end()); }
    friend constexpr void swap(Arr& x, Arr& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
template<std::input_iterator InputIter, class Alloc = Allocator<typename std::iterator_traits<InputIter>::value_type>> Arr(InputIter, InputIter, Alloc = Alloc()) -> Arr<typename std::iterator_traits<InputIter>::value_type, Alloc>;
template<Range R, class Alloc = Allocator<typename RangeTraits<R>::value_type>> Arr(R, Alloc = Alloc()) -> Arr<typename RangeTraits<R>::value_type, Alloc>;

template<class T, itype::u32 N>
    requires std::same_as<T, std::remove_cv_t<T>>
class StaticArr : public ViewInterface<StaticArr<T, N>, T> {
    union {
        T elems[(N == 0 ? 1 : N)];
    };
public:
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    constexpr StaticArr() noexcept(noexcept(value_type{})) : elems{} {}
    constexpr StaticArr(ArrNoInitTag) noexcept {}
    template<class U, class... Args> constexpr StaticArr(ArrInitTag<U>, Args&&... args) : elems{ static_cast<U>(std::forward<Args>(args))... } {
        static_assert(std::same_as<T, U>, "gsh::StaticArr::StaticArr / The type specified in gsh::ArrInitTag is different from value_type.");
        static_assert(sizeof...(Args) == N, "gsh::StaticArr::StaticArr / The number of arguments is greater than the length of the array.");
    }
    template<class... Args> constexpr StaticArr(ArrInitTag<void>, Args&&... args) : elems{ static_cast<T>(std::forward<Args>(args))... } { static_assert(sizeof...(Args) <= N, "gsh::StaticArr::StaticArr / The number of arguments is greater than the length of the array."); }
    constexpr explicit StaticArr(const value_type& value) {
        for (itype::u32 i = 0; i != N; ++i) std::construct_at(elems + i, value);
    }
    template<std::input_iterator InputIter> constexpr explicit StaticArr(InputIter first) {
        for (itype::u32 i = 0; i != N; ++first, ++i) std::construct_at(elems + i, *first);
    }
    template<std::input_iterator InputIter> constexpr StaticArr(InputIter first, InputIter last) {
        const itype::u32 n = std::distance(first, last);
        if (n != N) throw gsh::Exception("gsh::StaticArr::StaticArr / The size of the given range differs from the size of the array.");
        for (itype::u32 i = 0; i != N; ++first, ++i) std::construct_at(elems + i, *first);
    }
    template<Rangeof<value_type> R> constexpr StaticArr(R&& r) : StaticArr(RangeTraits<R>::begin(r), RangeTraits<R>::end(r)) {}
    constexpr StaticArr(const value_type (&a)[N]) {
        for (itype::u32 i = 0; i != N; ++i) std::construct_at(elems + i, a[i]);
    }
    constexpr StaticArr(value_type (&&a)[N]) {
        for (itype::u32 i = 0; i != N; ++i) std::construct_at(elems + i, std::move(a[i]));
    }
    constexpr StaticArr(const StaticArr& x) {
        for (itype::u32 i = 0; i != N; ++i) std::construct_at(elems + i, x.elems[i]);
    }
    constexpr StaticArr(StaticArr&& y) {
        for (itype::u32 i = 0; i != N; ++i) std::construct_at(elems + i, std::move(y.elems[i]));
    }
    constexpr StaticArr(std::initializer_list<value_type> il) : StaticArr(il.begin(), il.end()) {}
    template<ForwardRange R>
        requires(!std::same_as<StaticArr, std::remove_cvref_t<R>>)
    constexpr StaticArr(R&& r) : StaticArr(RangeTraits<R>::begin(r), RangeTraits<R>::end(r)) {}
    constexpr ~StaticArr() noexcept {
        if constexpr (!std::is_trivially_destructible_v<value_type>)
            for (itype::u32 i = 0; i != N; ++i) std::destroy_at(elems + i);
    }
    constexpr StaticArr& operator=(const StaticArr& x) {
        for (itype::u32 i = 0; i != N; ++i) elems[i] = x.elems[i];
        return *this;
    }
    constexpr StaticArr& operator=(StaticArr&& x) noexcept {
        for (itype::u32 i = 0; i != N; ++i) elems[i] = std::move(x.elems[i]);
        return *this;
    }
    constexpr StaticArr& operator=(std::initializer_list<value_type> init) {
        assign(init.begin(), init.end());
        return *this;
    }
    constexpr iterator begin() noexcept { return elems; }
    constexpr const_iterator begin() const noexcept { return elems; }
    constexpr iterator end() noexcept { return elems + N; }
    constexpr const_iterator end() const noexcept { return elems + N; }
    constexpr const_iterator cbegin() const noexcept { return elems; }
    constexpr const_iterator cend() const noexcept { return elems + N; }
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(elems + N); }
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(elems + N); }
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(elems); }
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(elems); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(elems + N); }
    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(elems); }
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }
    [[nodiscard]] constexpr bool empty() const noexcept { return N != 0; }
    GSH_INTERNAL_INLINE constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::operator[] / The index is out of range. ( n=", n, ", size=", N, " )");
#endif
        return elems[n];
    }
    GSH_INTERNAL_INLINE constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::operator[] / The index is out of range. ( n=", n, ", size=", N, " )");
#endif
        return elems[n];
    }
    GSH_INTERNAL_INLINE constexpr reference at(const size_type n) {
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::at / The index is out of range. ( n=", n, ", size=", N, " )");
        return elems[n];
    }
    GSH_INTERNAL_INLINE constexpr const_reference at(const size_type n) const {
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::at / The index is out of range. ( n=", n, ", size=", N, " )");
        return elems[n];
    }
    GSH_INTERNAL_INLINE constexpr reference at_unchecked(const size_type n) { return elems[n]; }
    GSH_INTERNAL_INLINE constexpr const_reference at_unchecked(const size_type n) const { return elems[n]; }
    constexpr pointer data() noexcept { return elems; }
    constexpr const_pointer data() const noexcept { return elems; }
    constexpr reference front() { return elems[0]; }
    constexpr const_reference front() const { return elems[0]; }
    constexpr reference back() { return elems[N - 1]; }
    constexpr const_reference back() const { return elems[N - 1]; }
    template<std::input_iterator InputIter> constexpr void assign(InputIter first) {
        for (itype::u32 i = 0; i != N; ++first, ++i) elems[i] = *first;
    }
    template<std::input_iterator InputIter> constexpr void assign(InputIter first, const InputIter last) {
        const itype::u32 n = std::distance(first, last);
        if (n != N) throw gsh::Exception("gsh::StaticArr::assign / The size of the given range differs from the size of the array.");
        for (itype::u32 i = 0; i != N; ++first, ++i) elems[i] = *first;
    }
    constexpr void assign(const value_type& value) {
        for (itype::u32 i = 0; i != N; ++i) elems[i] = value;
    }
    constexpr void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
    constexpr void swap(StaticArr& x) {
        using std::swap;
        for (itype::u32 i = 0; i != N; ++i) swap(elems[i], x.elems[i]);
    }
    friend constexpr bool operator==(const StaticArr& x, const StaticArr& y) {
        bool res = true;
        for (size_type i = 0; i != N;) {
            const bool f = x.elems[i] == y.elems[i];
            res &= f;
            i = f ? i + 1 : N;
        }
        return res;
    }
    friend constexpr auto operator<=>(const StaticArr& x, const StaticArr& y) { return std::lexicographical_compare_three_way(x.begin(), x.end(), y.begin(), y.end()); }
    friend constexpr void swap(StaticArr& x, StaticArr& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
template<class U, class... Args> StaticArr(ArrInitTag<U>, Args...) -> StaticArr<std::conditional_t<std::is_void_v<U>, std::common_type_t<Args...>, U>, sizeof...(Args)>;

}  // namespace gsh

namespace std {
template<class T, gsh::itype::u32 N> struct tuple_size<gsh::StaticArr<T, N>> : integral_constant<size_t, N> {};
template<std::size_t M, class T, gsh::itype::u32 N> struct tuple_element<M, gsh::StaticArr<T, N>> {
    static_assert(M < N, "std::tuple_element<gsh::StaticArr<T, N>> / The index is out of range.");
    using type = T;
};
}  // namespace std

namespace gsh {
template<std::size_t M, class T, itype::u32 N> const T& get(const StaticArr<T, N>& a) {
    static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
    return a[M];
}
template<std::size_t M, class T, itype::u32 N> T& get(StaticArr<T, N>& a) {
    static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
    return a[M];
}
template<std::size_t M, class T, itype::u32 N> T&& get(StaticArr<T, N>&& a) {
    static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
    return std::move(a[M]);
}
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
    constexpr Vec() noexcept(noexcept(Allocator())) : Vec(Allocator()) {}
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
        return *(ptr + n);
    }
    GSH_INTERNAL_INLINE constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Vec::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
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
    GSH_INTERNAL_INLINE constexpr reference at_unchecked(const size_type n) { return *(ptr + n); }
    GSH_INTERNAL_INLINE constexpr const_reference at_unchecked(const size_type n) const { return *(ptr + n); }
    constexpr pointer data() noexcept { return ptr; }
    constexpr const_pointer data() const noexcept { return ptr; }
    constexpr reference front() { return *ptr; }
    constexpr const_reference front() const { return *ptr; }
    constexpr reference back() { return *(ptr + len - 1); }
    constexpr const_reference back() const { return *(ptr + len - 1); }
    template<std::input_iterator InputIter> constexpr void assign(const InputIter first, const InputIter last) {
        const size_type n = std::distance(first, last);
        if (n > cap) {
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, cap);
            ptr = traits::allocate(alloc, n);
            cap = n;
            InputIter itr = first;
            for (size_type i = 0; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
        } else if (n > len) {
            size_type i = 0;
            InputIter itr = first;
            for (; i != len; ++itr, ++i) *(ptr + i) = *itr;
            for (; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
        } else {
            for (size_type i = n; i != len; ++i) traits::destroy(alloc, ptr + i);
            InputIter itr = first;
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
                for (size_type i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
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

template<class T, class U> constexpr std::common_type_t<T, U> Min(const T& a, const U& b) {
    return a < b ? a : b;
}
template<class T, class... Args> constexpr auto Min(const T& x, const Args&... args) {
    return Min(x, Min(args...));
}
template<class T, class U> constexpr std::common_type_t<T, U> Max(const T& a, const U& b) {
    return a < b ? b : a;
}
template<class T, class... Args> constexpr auto Max(const T& x, const Args&... args) {
    return Max(x, Max(args...));
}
template<class T, class U> constexpr bool Chmin(T& a, const U& b) {
    const bool f = b < a;
    a = f ? b : a;
    return f;
}
template<class T, class... Args> constexpr bool Chmin(T& a, const Args&... b) {
    return Chmin(a, Min(b...));
}
template<class T, class U> constexpr bool Chmax(T& a, const U& b) {
    const bool f = a < b;
    a = f ? b : a;
    return f;
}
template<class T, class... Args> constexpr bool Chmax(T& a, const Args&... b) {
    return Chmax(a, Max(b...));
}

template<ForwardRange R, class Proj = Identity, std::indirect_strict_weak_order<std::projected<typename RangeTraits<R>::iterator, Proj>> Comp = Less>
    requires std::indirectly_copyable_storable<typename RangeTraits<R>::iterator, typename RangeTraits<R>::value_type*>
constexpr typename RangeTraits<R>::value_type MinValue(R&& r, Comp&& comp = {}, Proj&& proj = {}) {
    using traits = RangeTraits<R>;
    auto first = traits::begin(r);
    auto last = traits::end(r);
    if (first == last) throw Exception("gsh::Min / The input is empty.");
    auto res = *(first++);
    for (; first != last; ++first) {
        const bool f = Invoke(comp, Invoke(proj, static_cast<const decltype(res)&>(*first)), Invoke(proj, res));
        if constexpr (std::is_trivial_v<decltype(res)>) res = f ? *first : res;
        else if (f) res = *first;
    }
    return res;
}
template<ForwardRange R, class Proj = Identity, std::indirect_strict_weak_order<std::projected<typename RangeTraits<R>::iterator, Proj>> Comp = Less>
    requires std::indirectly_copyable_storable<typename RangeTraits<R>::iterator, typename RangeTraits<R>::value_type*>
constexpr typename RangeTraits<R>::value_type MaxValue(R&& r, Comp&& comp = {}, Proj&& proj = {}) {
    if constexpr (std::same_as<std::remove_cvref_t<Comp>, Less>) return MinValue(std::forward<R>(r), Greater(), proj);
    else if constexpr (std::same_as<std::remove_cvref_t<Comp>, Greater>) return MinValue(std::forward<R>(r), Less(), proj);
    else return MinValue(std::forward<R>(r), SwapArgs(std::forward<Comp>(comp)), proj);
}

namespace internal {
    template<class F, class T, class I, class U> concept IndirectlyBinaryLeftFoldableImpl = std::movable<T> && std::movable<U> && std::convertible_to<T, U> && std::invocable<F&, U, std::iter_reference_t<I>> && std::assignable_from<U&, std::invoke_result_t<F&, U, std::iter_reference_t<I>>>;
    template<class F, class T, class I> concept IndirectlyBinaryLeftFoldable = std::copy_constructible<F> && std::indirectly_readable<I> && std::invocable<F&, T, std::iter_reference_t<I>> && std::convertible_to<std::invoke_result_t<F&, T, std::iter_reference_t<I>>, std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>> && IndirectlyBinaryLeftFoldableImpl<F, T, I, std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>>;
}  // namespace internal
template<ForwardRange R, class T = typename RangeTraits<R>::value_type, internal::IndirectlyBinaryLeftFoldable<T, typename RangeTraits<R>::iterator> F = Plus> constexpr auto Fold(R&& r, T init = {}, F&& f = {}) {
    for (auto&& x : std::forward<R>(r)) init = Invoke(f, std::move(init), std::forward<decltype(x)>(x));
    return init;
}

template<BidirectionalRange R>
    requires std::permutable<typename RangeTraits<R>::iterator>
void Reverse(R&& r) {
    std::ranges::reverse(std::forward<R>(r));
}

namespace internal {
    template<class T, class Proj = Identity> void SortUnsigned8(T* const p, const itype::u32 n, Proj&& proj = {}) {
        static itype::u32 cnt[1 << 8];
        std::memset(cnt, 0, sizeof(cnt));
        for (itype::u32 i = 0; i != n; ++i) ++cnt[Invoke(proj, p[i]) & 0xff];
        for (itype::u32 i = 0; i != (1 << 8) - 1; ++i) cnt[i + 1] += cnt[i];
        Arr<T> tmp(n);
        for (itype::u32 i = n; i--;) tmp[--cnt[Invoke(proj, p[i]) & 0xffff]] = std::move(p[i]);
        for (itype::u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
    }
    template<class T, class Proj = Identity> void SortUnsigned16(T* const p, const itype::u32 n, Proj&& proj = {}) {
        static itype::u32 cnt[1 << 16];
        std::memset(cnt, 0, sizeof(cnt));
        for (itype::u32 i = 0; i != n; ++i) ++cnt[Invoke(proj, p[i]) & 0xffff];
        for (itype::u32 i = 0; i != (1 << 16) - 1; ++i) cnt[i + 1] += cnt[i];
        Arr<T> tmp(n);
        for (itype::u32 i = n; i--;) tmp[--cnt[Invoke(proj, p[i]) & 0xffff]] = std::move(p[i]);
        for (itype::u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
    }
    template<class T, class Proj = Identity> void SortUnsigned32(T* const p, const itype::u32 n, Proj&& proj = {}) {
        static itype::u32 cnt1[1 << 16], cnt2[1 << 16];
        std::memset(cnt1, 0, sizeof(cnt1));
        std::memset(cnt2, 0, sizeof(cnt2));
        for (itype::u32 i = 0; i != n; ++i) {
            auto tmp = Invoke(proj, p[i]);
            const itype::u16 a = tmp & 0xffff;
            const itype::u16 b = tmp >> 16 & 0xffff;
            ++cnt1[a];
            ++cnt2[b];
        }
        for (itype::u32 i = 0; i != (1 << 16) - 1; ++i) {
            cnt1[i + 1] += cnt1[i];
            cnt2[i + 1] += cnt2[i];
        }
        Arr<T> tmp(n);
        for (itype::u32 i = n; i--;) tmp[--cnt1[Invoke(proj, p[i]) & 0xffff]] = std::move(p[i]);
        for (itype::u32 i = n; i--;) p[--cnt2[Invoke(proj, tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
    }
    template<class T, class Proj = Identity> void SortUnsigned64(T* const p, const itype::u32 n, Proj&& proj = {}) {
        static itype::u32 cnt1[1 << 16], cnt2[1 << 16], cnt3[1 << 16], cnt4[1 << 16];
        std::memset(cnt1, 0, sizeof(cnt1));
        std::memset(cnt2, 0, sizeof(cnt2));
        std::memset(cnt3, 0, sizeof(cnt3));
        std::memset(cnt4, 0, sizeof(cnt4));
        for (itype::u32 i = 0; i != n; ++i) {
            auto tmp = Invoke(proj, p[i]);
            const itype::u16 a = tmp & 0xffff;
            const itype::u16 b = tmp >> 16 & 0xffff;
            const itype::u16 c = tmp >> 32 & 0xffff;
            const itype::u16 d = tmp >> 48 & 0xffff;
            ++cnt1[a];
            ++cnt2[b];
            ++cnt3[c];
            ++cnt4[d];
        }
        for (itype::u32 i = 0; i != (1 << 16) - 1; ++i) {
            cnt1[i + 1] += cnt1[i];
            cnt2[i + 1] += cnt2[i];
            cnt3[i + 1] += cnt3[i];
            cnt4[i + 1] += cnt4[i];
        }
        Arr<T> tmp(n);
        for (itype::u32 i = n; i--;) tmp[--cnt1[Invoke(proj, p[i]) & 0xffff]] = std::move(p[i]);
        for (itype::u32 i = n; i--;) p[--cnt2[Invoke(proj, tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
        for (itype::u32 i = n; i--;) tmp[--cnt3[Invoke(proj, p[i]) >> 32 & 0xffff]] = std::move(p[i]);
        for (itype::u32 i = n; i--;) p[--cnt4[Invoke(proj, tmp[i]) >> 48 & 0xffff]] = std::move(tmp[i]);
    }
    class Revmsb {
        template<class T> constexpr auto operator()(T&& x) const {
            using result_type = std::make_unsigned_t<T>;
            return std::bit_cast<result_type>(x) ^ (result_type(1) << (sizeof(T) * 8 - 1));
        }
    };
    template<class T, class Comp, class Proj>
        requires std::is_scalar_v<T>
    GSH_INTERNAL_INLINE constexpr void CompSwap(T* const p, itype::u32 a, itype::u32 b, Comp&& comp = {}, Proj&& proj = {}) {
        bool f = Invoke(comp, Invoke(proj, p[a]), Invoke(proj, p[b]));
        const auto tmp_a = p[a], tmp_b = p[b];
        p[a] = f ? tmp_a : tmp_b;
        p[b] = f ? tmp_b : tmp_a;
    }
    template<class T, class Comp, class Proj> GSH_INTERNAL_NOINLINE constexpr void CompSwap(T* const p, itype::u32 a, itype::u32 b, Comp&& comp = {}, Proj&& proj = {}) {
        using std::swap;
        if (!Invoke(comp, Invoke(proj, p[a]), Invoke(proj, p[b]))) swap(p[a], p[b]);
    }
    template<itype::u32 N, class T, class Comp = Less, class Proj = Identity> GSH_INTERNAL_NOINLINE constexpr void SortBlock(T* const p, itype::u32 len, Comp&& comp = {}, Proj&& proj = {}) {
        if constexpr (N <= 8) Assume(len == 1);
        for (itype::u32 i = 0; i != len; ++i) {
            T* const q = p + i * N;
            // clang-format off
#ifdef F
#define GSH_INTERNAL_DEFINED_MACRO_F
#pragma push_macro("F")
#undef F
#endif
#define F(A,B) CompSwap(q,A,B,comp,proj);
if constexpr(N==2){F(0,1)}
if constexpr(N==3){F(0,2)F(0,1)F(1,2)}
if constexpr(N==4){F(0,2)F(1,3)F(0,1)F(2,3)F(1,2)}
if constexpr(N==5){F(0,3)F(1,4)F(0,2)F(1,3)F(0,1)F(2,4)F(1,2)F(3,4)F(2,3)}
if constexpr(N==6){F(0,5)F(1,3)F(2,4)F(1,2)F(3,4)F(0,3)F(2,5)F(0,1)F(2,3)F(4,5)F(1,2)F(3,4)}
if constexpr(N==7){F(0,6)F(2,3)F(4,5)F(0,2)F(1,4)F(3,6)F(0,1)F(2,5)F(3,4)F(1,2)F(4,6)F(2,3)F(4,5)F(1,2)F(3,4)F(5,6)}
if constexpr(N==8){F(0,2)F(1,3)F(4,6)F(5,7)F(0,4)F(1,5)F(2,6)F(3,7)F(0,1)F(2,3)F(4,5)F(6,7)F(2,4)F(3,5)F(1,4)F(3,6)F(1,2)F(3,4)F(5,6)}
if constexpr(N==9){F(0,3)F(1,7)F(2,5)F(4,8)F(0,7)F(2,4)F(3,8)F(5,6)F(0,2)F(1,3)F(4,5)F(7,8)F(1,4)F(3,6)F(5,7)F(0,1)F(2,4)F(3,5)F(6,8)F(2,3)F(4,5)F(6,7)F(1,2)F(3,4)F(5,6)}
if constexpr(N==10){F(0,8)F(1,9)F(2,7)F(3,5)F(4,6)F(0,2)F(1,4)F(5,8)F(7,9)F(0,3)F(2,4)F(5,7)F(6,9)F(0,1)F(3,6)F(8,9)F(1,5)F(2,3)F(4,8)F(6,7)F(1,2)F(3,5)F(4,6)F(7,8)F(2,3)F(4,5)F(6,7)F(3,4)F(5,6)}
if constexpr(N==11){F(0,9)F(1,6)F(2,4)F(3,7)F(5,8)F(0,1)F(3,5)F(4,10)F(6,9)F(7,8)F(1,3)F(2,5)F(4,7)F(8,10)F(0,4)F(1,2)F(3,7)F(5,9)F(6,8)F(0,1)F(2,6)F(4,5)F(7,8)F(9,10)F(2,4)F(3,6)F(5,7)F(8,9)F(1,2)F(3,4)F(5,6)F(7,8)F(2,3)F(4,5)F(6,7)}
if constexpr(N==12){F(0,8)F(1,7)F(2,6)F(3,11)F(4,10)F(5,9)F(0,1)F(2,5)F(3,4)F(6,9)F(7,8)F(10,11)F(0,2)F(1,6)F(5,10)F(9,11)F(0,3)F(1,2)F(4,6)F(5,7)F(8,11)F(9,10)F(1,4)F(3,5)F(6,8)F(7,10)F(1,3)F(2,5)F(6,9)F(8,10)F(2,3)F(4,5)F(6,7)F(8,9)F(4,6)F(5,7)F(3,4)F(5,6)F(7,8)}
if constexpr(N==13){F(0,12)F(1,10)F(2,9)F(3,7)F(5,11)F(6,8)F(1,6)F(2,3)F(4,11)F(7,9)F(8,10)F(0,4)F(1,2)F(3,6)F(7,8)F(9,10)F(11,12)F(4,6)F(5,9)F(8,11)F(10,12)F(0,5)F(3,8)F(4,7)F(6,11)F(9,10)F(0,1)F(2,5)F(6,9)F(7,8)F(10,11)F(1,3)F(2,4)F(5,6)F(9,10)F(1,2)F(3,4)F(5,7)F(6,8)F(2,3)F(4,5)F(6,7)F(8,9)F(3,4)F(5,6)}
if constexpr(N==14){F(0,1)F(2,3)F(4,5)F(6,7)F(8,9)F(10,11)F(12,13)F(0,2)F(1,3)F(4,8)F(5,9)F(10,12)F(11,13)F(0,4)F(1,2)F(3,7)F(5,8)F(6,10)F(9,13)F(11,12)F(0,6)F(1,5)F(3,9)F(4,10)F(7,13)F(8,12)F(2,10)F(3,11)F(4,6)F(7,9)F(1,3)F(2,8)F(5,11)F(6,7)F(10,12)F(1,4)F(2,6)F(3,5)F(7,11)F(8,10)F(9,12)F(2,4)F(3,6)F(5,8)F(7,10)F(9,11)F(3,4)F(5,6)F(7,8)F(9,10)F(6,7)}
if constexpr(N==15){F(1,2)F(3,10)F(4,14)F(5,8)F(6,13)F(7,12)F(9,11)F(0,14)F(1,5)F(2,8)F(3,7)F(6,9)F(10,12)F(11,13)F(0,7)F(1,6)F(2,9)F(4,10)F(5,11)F(8,13)F(12,14)F(0,6)F(2,4)F(3,5)F(7,11)F(8,10)F(9,12)F(13,14)F(0,3)F(1,2)F(4,7)F(5,9)F(6,8)F(10,11)F(12,13)F(0,1)F(2,3)F(4,6)F(7,9)F(10,12)F(11,13)F(1,2)F(3,5)F(8,10)F(11,12)F(3,4)F(5,6)F(7,8)F(9,10)F(2,3)F(4,5)F(6,7)F(8,9)F(10,11)F(5,6)F(7,8)}
if constexpr(N==16){F(0,13)F(1,12)F(2,15)F(3,14)F(4,8)F(5,6)F(7,11)F(9,10)F(0,5)F(1,7)F(2,9)F(3,4)F(6,13)F(8,14)F(10,15)F(11,12)F(0,1)F(2,3)F(4,5)F(6,8)F(7,9)F(10,11)F(12,13)F(14,15)F(0,2)F(1,3)F(4,10)F(5,11)F(6,7)F(8,9)F(12,14)F(13,15)F(1,2)F(3,12)F(4,6)F(5,7)F(8,10)F(9,11)F(13,14)F(1,4)F(2,6)F(5,8)F(7,10)F(9,13)F(11,14)F(2,4)F(3,6)F(9,12)F(11,13)F(3,5)F(6,8)F(7,9)F(10,12)F(3,4)F(5,6)F(7,8)F(9,10)F(11,12)F(6,7)F(8,9)}
            // clang-format on
            static_assert(N <= 16);
#undef F
#ifdef GSH_INTERNAL_DEFINED_MACRO_F
#undef GSH_INTERNAL_DEFINED_MACRO_F
#pragma pop_macro("F")
#endif
        }
    }
}  // namespace internal
template<Range R, class Comp = Less, class Proj = Identity>
    requires std::sortable<std::ranges::iterator_t<R>, Comp, Proj>
constexpr void Sort(R&& r, Comp&& comp = {}, Proj&& proj = {}) {
    if constexpr (!PointerObtainable<R>) {
        Arr tmp(std::move_iterator(std::ranges::begin(r)), std::move_sentinel(std::ranges::end(r)));
        Sort(tmp, std::forward<Comp>(comp), std::forward<Proj>(proj));
        for (itype::u32 i = 0; auto&& el : r) el = std::move(tmp[i++]);
        return;
    } else {
        const itype::u32 n = std::ranges::size(r);
        auto* const p = std::ranges::data(r);
        using value_type = std::remove_cvref_t<decltype(*p)>;
        if (false && !std::is_constant_evaluated()) {
            constexpr bool is_less = std::same_as<std::remove_cvref_t<Comp>, Less>;
            constexpr bool is_greater = std::same_as<std::remove_cvref_t<Comp>, Greater>;
            if constexpr ((is_less || is_greater) && std::is_default_constructible_v<value_type>) {
                using inv_result = std::remove_cvref_t<std::invoke_result_t<Proj, std::ranges::range_value_t<R>>>;
                if constexpr (std::same_as<inv_result, itype::u8> || std::same_as<inv_result, itype::i8> || std::same_as<inv_result, ctype::c8>) {
                    if (n >= 200) {
                        if constexpr (std::is_unsigned_v<inv_result>) internal::SortUnsigned8(p, n, std::forward<Proj>(proj));
                        else internal::SortUnsigned8(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
                        if constexpr (is_greater) Reverse(std::forward<R>(r));
                        return;
                    }
                }
                if constexpr (std::same_as<inv_result, itype::u16> || std::same_as<inv_result, itype::i16>) {
                    if (n >= 1000) {
                        if constexpr (std::is_unsigned_v<inv_result>) internal::SortUnsigned16(p, n, std::forward<Proj>(proj));
                        else internal::SortUnsigned16(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
                        if constexpr (is_greater) Reverse(std::forward<R>(r));
                        return;
                    }
                }
                if constexpr (std::same_as<inv_result, itype::u32> || std::same_as<inv_result, itype::i32>) {
                    if (n >= 2000) {
                        if constexpr (std::is_unsigned_v<inv_result>) internal::SortUnsigned32(p, n, std::forward<Proj>(proj));
                        else internal::SortUnsigned32(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
                        if constexpr (is_greater) Reverse(std::forward<R>(r));
                        return;
                    }
                }
                if constexpr (std::same_as<inv_result, itype::u64> || std::same_as<inv_result, itype::i64>) {
                    if (n >= 4000) {
                        if constexpr (std::is_unsigned_v<inv_result>) internal::SortUnsigned64(p, n, std::forward<Proj>(proj));
                        else internal::SortUnsigned64(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
                        if constexpr (is_greater) Reverse(std::forward<R>(r));
                        return;
                    }
                }
            }
        }
        const itype::u32 minrun = n <= 16 ? 32 : ((n >> (std::bit_width(n) - 4)) + ((n & ((1ull << (std::bit_width(n) - 4)) - 1)) != 0)) << std::has_single_bit(n);
        const itype::u32 rem = n % minrun, blocks = n / minrun;
        switch (rem) {
        case 2 : internal::SortBlock<2>(p, 1, comp, proj); break;
        case 3 : internal::SortBlock<3>(p, 1, comp, proj); break;
        case 4 : internal::SortBlock<4>(p, 1, comp, proj); break;
        case 5 : internal::SortBlock<5>(p, 1, comp, proj); break;
        case 6 : internal::SortBlock<6>(p, 1, comp, proj); break;
        case 7 : internal::SortBlock<7>(p, 1, comp, proj); break;
        case 8 : internal::SortBlock<8>(p, 1, comp, proj); break;
        case 9 : internal::SortBlock<9>(p, 1, comp, proj); break;
        case 10 : internal::SortBlock<10>(p, 1, comp, proj); break;
        case 11 : internal::SortBlock<11>(p, 1, comp, proj); break;
        case 12 : internal::SortBlock<12>(p, 1, comp, proj); break;
        case 13 : internal::SortBlock<13>(p, 1, comp, proj); break;
        case 14 : internal::SortBlock<14>(p, 1, comp, proj); break;
        case 15 : internal::SortBlock<15>(p, 1, comp, proj); break;
        case 16 : internal::SortBlock<16>(p, 1, comp, proj); break;
        default : break;
        }
        if (minrun == 32) return;
        switch (minrun) {
        case 9 : internal::SortBlock<9>(p + rem, blocks, comp, proj); break;
        case 10 : internal::SortBlock<10>(p + rem, blocks, comp, proj); break;
        case 11 : internal::SortBlock<11>(p + rem, blocks, comp, proj); break;
        case 12 : internal::SortBlock<12>(p + rem, blocks, comp, proj); break;
        case 13 : internal::SortBlock<13>(p + rem, blocks, comp, proj); break;
        case 14 : internal::SortBlock<14>(p + rem, blocks, comp, proj); break;
        case 15 : internal::SortBlock<15>(p + rem, blocks, comp, proj); break;
        case 16 : internal::SortBlock<16>(p + rem, blocks, comp, proj); break;
        default : Unreachable();
        }
        itype::u32 first[24];
        itype::u32 size[24];
        first[0] = p;
        size[0] = rem;
        itype::u32 cnt = 1;
        for (itype::u32 i = 0; i != blocks; ++i) {
            first[cnt] = p + rem + minrun * i;
            size[cnt] = minrun;
            ++cnt;
            while (cnt >= 2 && size[cnt - 2] >= size[cnt - 1]) {}
        }
    }
}

template<ForwardRange R, class T, class Proj = Identity, std::indirect_strict_weak_order<const T*, std::projected<typename RangeTraits<R>::iterator, Proj>> Comp = Less> constexpr auto LowerBound(R&& r, const T& value, Comp&& comp = {}, Proj&& proj = {}) {
    using traits = RangeTraits<R>;
    auto st = traits::begin(r);
    for (auto len = traits::size(r) + 1; len > 1;) {
        auto half = len / 2;
        len -= half;
        auto tmp = std::next(st, half);
        auto md = std::next(tmp, -1);
        st = Invoke(comp, Invoke(proj, *md), value) ? tmp : st;
    }
    return st;
}
template<ForwardRange R, class T, class Proj = Identity, std::indirect_strict_weak_order<const T*, std::projected<typename RangeTraits<R>::iterator, Proj>> Comp = Less> constexpr auto UpperBound(R&& r, const T& value, Comp&& comp = {}, Proj&& proj = {}) {
    using traits = RangeTraits<R>;
    auto st = traits::begin(r);
    for (auto len = traits::size(r) + 1; len > 1;) {
        auto half = len / 2;
        len -= half;
        auto tmp = std::next(st, half);
        auto md = std::next(tmp, -1);
        st = !static_cast<bool>(Invoke(comp, value, Invoke(proj, *md))) ? tmp : st;
    }
    return st;
}

template<ForwardRange R, class Proj = Identity, class Comp = Less> constexpr Arr<itype::u32> LongestIncreasingSubsequence(R&& r, Comp&& comp = {}, Proj&& proj = {}) {
    using T = typename RangeTraits<R>::value_type;
    Arr<T> dp(RangeTraits<R>::size(r));
    Arr<itype::u32> idx(dp.size());
    itype::u32 i = 0;
    T *begin = dp.data(), *last = dp.data();
    for (auto&& x : r) {
        if (begin == last || Invoke(comp, *(last - 1), Invoke(proj, x))) [[unlikely]] {
            idx[i++] = last - begin;
            *last = x;
            ++last;
        } else {
            T* loc = LowerBound(Subrange{ begin, last - 1 }, x, comp, proj);
            idx[i++] = loc - begin;
            *loc = x;
        }
    }
    itype::u32 cnt = last - begin - 1;
    Arr<itype::u32> res(last - begin);
    for (itype::u32 i = dp.size(); i--;) {
        if (idx[i] == cnt) res[cnt--] = i;
    }
    return res;
}

template<RandomAccessRange R> constexpr Arr<itype::u32> LongestCommonPrefix(R&& r) {
    using traits = RangeTraits<R>;
    const itype::u32 n = traits::size(r);
    Arr<itype::u32> res(n);
    if (n == 0) return res;
    res[0] = n;
    const auto itr = traits::begin(r);
    itype::u32 i = 1, j = 0;
    while (i != n) {
        while (i + j < n && *std::next(itr, j) == *std::next(itr, i + j)) ++j;
        res[i] = j;
        if (j == 0) {
            ++i;
            continue;
        }
        itype::u32 k = 1;
        while (k < j && k + res[k] < j) ++k;
        const auto a = res.data() + i + 1;
        const auto b = res.data() + 1;
        for (itype::u32 l = 0; l != k - 1; ++l) a[l] = b[l];
        i += k;
        j -= k;
    }
    return res;
}

template<class T = itype::u64, Rangeof<itype::u32> R> constexpr T CountDistinctSubsequences(R&& r) {
    using traits = RangeTraits<R>;
    const itype::u32 n = traits::size(r);
    if (n == 0) return 0;
    Arr<itype::u64> s(n);
    for (itype::u32 i = 0; itype::u32 x : r) {
        s[i] = static_cast<itype::u64>(i) << 32 | x;
        ++i;
    }
    internal::SortUnsigned32(s.data(), n);
    Arr<itype::u32> rank(n);
    rank[s[0] >> 32] = 0;
    itype::u32 cnt = 0, end = s[0];
    for (itype::u32 i = 1; i != n; ++i) {
        cnt += end != static_cast<itype::u32>(s[i]);
        rank[s[i] >> 32] = cnt;
        end = s[i];
    }
    Arr<itype::u32> last(cnt + 1, n);
    Arr<T> dp(n + 1);
    dp[0] = 1;
    const T m = 2;
    for (itype::u32 i = 0; i != n; ++i) {
        dp[i + 1] = m * dp[i] - dp[last[rank[i]]];
        last[rank[i]] = i;
    }
    return dp[n] - dp[0];
}

template<Range R> constexpr auto Majority(R&& r) {
    using traits = RangeTraits<R>;
    itype::u32 c = 0;
    itype::u32 len = 0;
    auto i = traits::begin(r);
    auto j = traits::end(r), k = j;
    for (; i != j; ++i) {
        ++len;
        if (c == 0) k = i, c = 1;
        else c += static_cast<itype::u32>(static_cast<bool>(*i == *k)) * 2 - 1;
    }
    c = 0;
    for (i = traits::begin(r); i != j; ++i) c += static_cast<bool>(*i == *k);
    return (2 * c >= len ? k : j);
}

class Mo {
    struct rg {
        itype::u32 l, r;
        constexpr rg(itype::u32 a, itype::u32 b) : l(a), r(b) {}
    };
    Vec<rg> qu;
public:
    constexpr Mo() {}
    constexpr void reserve(itype::u32 q) { qu.reserve(q); }
    constexpr void query(itype::u32 l, itype::u32 r) { qu.emplace_back(l, r); }
    template<class F1, class F2, class F3> void run(F1&& add, F2&& del, F3&& slv) const { solve(add, add, del, del, std::forward<F3>(slv)); }
    template<class F1, class F2, class F3, class F4, class F5> void run(F1 addl, F2 addr, F3 dell, F4 delr, F5 slv) const {
        const itype::u32 Q = qu.size();
        itype::u32 N = 0;
        for (itype::u32 i = 0; i != Q; ++i) N = N < qu[i].r ? qu[i].r : N;
        itype::u32 width = 1.1 * std::sqrt(static_cast<ftype::f64>(3ull * N * N) / (2 * Q));
        width += width == 0;
        Arr<itype::u32> cnt(N + 1), buf(Q), block(Q), idx(Q);
        for (itype::u32 i = 0; i != Q; ++i) ++cnt[qu[i].r];
        for (itype::u32 i = 0; i != N; ++i) cnt[i + 1] += cnt[i];
        for (itype::u32 i = 0; i != Q; ++i) buf[--cnt[qu[i].r]] = i;
        cnt.assign(N / width + 2, 0);
        for (itype::u32 i = 0; i != Q; ++i) block[i] = qu[i].l / width;
        for (itype::u32 i = 0; i != Q; ++i) ++cnt[block[i]];
        for (itype::u32 i = 0; i != cnt.size() - 1; ++i) cnt[i + 1] += cnt[i];
        for (itype::u32 i = 0; i != Q; ++i) idx[--cnt[block[buf[i]]]] = buf[i];
        for (itype::u32 i = 0; i < cnt.size() - 1; i += 2) {
            const itype::u32 l = cnt[i], r = cnt[i + 1];
            for (itype::u32 j = 0; j != (r - l) / 2; ++j) {
                const itype::u32 t = idx[l + j];
                idx[l + j] = idx[r - j - 1], idx[r - j - 1] = t;
            }
        }
        itype::u32 nl = 0, nr = 0;
        for (itype::u32 i : idx) {
            while (nl > qu[i].l) Invoke(addl, --nl);
            while (nr < qu[i].r) Invoke(addr, nr++);
            while (nl < qu[i].l) Invoke(dell, nl++);
            while (nr > qu[i].r) Invoke(delr, --nr);
            Invoke(slv, i);
        }
    }
};

}  // namespace gsh

#include <ctime>            // std::time, std::clock
#include <source_location>  // std::source_location


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
    static constexpr itype::usize word_size = sizeof(result_type) * 8;
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
    static constexpr itype::usize word_size = sizeof(result_type) * 8;
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
    constexpr void reload() { x = URBG::operator()(), cnt = 0; }
    constexpr void init() {
        for (itype::u32 i = 0; i != Size; ++i) buf[i] = URBG::operator()();
        x = URBG::operator()(), cnt = 0;
    }
    constexpr URBG::result_type operator()() { return x ^ buf[cnt++]; }
};
template<itype::u32 Size> using RandBuffer32 = RandBuffer<Size, Rand32>;
template<itype::u32 Size> using RandBuffer64 = RandBuffer<Size, Rand64>;

// @brief Generate 32bit uniform random numbers in [0, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u32 Uniform32(URBG& g, itype::u32 max) {
    return (static_cast<itype::u64>(g() & 4294967295u) * max) >> 32;
}
// @brief Generate 32bit uniform random numbers in [min, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u32 Uniform32(URBG& g, itype::u32 min, itype::u32 max) {
    return static_cast<itype::u32>((static_cast<itype::u64>(g() & 4294967295u) * (max - min)) >> 32) + min;
}
// @brief Generate 64bit uniform random numbers in [0, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u64 Uniform64(URBG& g, itype::u64 max) {
    return (static_cast<itype::u128>(g()) * max) >> 64;
}
// @brief Generate 64bit uniform random numbers in [min, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u64 Uniform64(URBG& g, itype::u64 min, itype::u64 max) {
    return static_cast<itype::u64>((static_cast<itype::u128>(g()) * (max - min)) >> 64) + min;
}

template<class URBG> constexpr itype::u32 UnbiasedUniform32(URBG& g, itype::u32 max) {
    itype::u32 mask = ~0u;
    --max;
    mask >>= std::countl_zero(max | 1);
    itype::u32 x;
    do {
        x = g() & mask;
    } while (x > max);
    return x;
}
template<class URBG> constexpr itype::u32 UnbiasedUniform32(URBG& g, itype::u32 min, itype::u32 max) {
    return min + UnbiasedUniform32(g, max - min);
}
template<class URBG> constexpr itype::u64 UnbiasedUniform64(URBG& g, itype::u64 max) {
    itype::u64 mask = ~0ull;
    --max;
    mask >>= std::countl_zero(max | 1);
    itype::u64 x;
    do {
        x = g() & mask;
    } while (x > max);
    return x;
}
template<class URBG> constexpr itype::u32 UnbiasedUniform64(URBG& g, itype::u64 min, itype::u64 max) {
    return min + UnbiasedUniform64(g, max - min);
}

//https://speakerdeck.com/hole/rand01?slide=31
template<class URBG> constexpr ftype::f32 Canocicaled32(URBG& g) {
    return std::bit_cast<ftype::f32>((127u << 23) | (static_cast<itype::u32>(g()) & 0x7fffff)) - 1.0f;
}
template<class URBG> constexpr ftype::f32 Uniformf32(URBG& g, ftype::f32 max) {
    return canocicaled32(g) * max;
}
template<class URBG> constexpr ftype::f32 Uniformf32(URBG& g, ftype::f32 min, ftype::f32 max) {
    return canocicaled32(g) * (max - min) + min;
}
template<class URBG> constexpr ftype::f64 Canocicaled64(URBG& g) {
    return std::bit_cast<ftype::f64>((1023ull << 52) | (g() & 0xfffffffffffffull)) - 1.0;
}
template<class URBG> constexpr ftype::f64 Uniformf64(URBG& g, ftype::f64 max) {
    return canocicaled64(g) * max;
}
template<class URBG> constexpr ftype::f64 Uniformf64(URBG& g, ftype::f64 min, ftype::f64 max) {
    return canocicaled64(g) * (max - min) + min;
}

}  // namespace gsh

#include <cstdio>  // std::printf


namespace gsh {

class ClockTimer {
    std::clock_t start_time;
public:
    ClockTimer() { start_time = std::clock(); }
    void restart() { start_time = std::clock(); }
    std::clock_t elapsed() const { return (std::clock() - start_time) * 1000 / CLOCKS_PER_SEC; }
    void print() const { std::printf("gsh::ClockTimer::print / %llu ms\n", static_cast<unsigned long long>(elapsed())); }
};

}  // namespace gsh


#if 0 && !defined ONLINE_JUDGE
#include <fcntl.h>
gsh::BasicReader r(open("in.txt", O_RDONLY));
gsh::BasicWriter w(open("out.txt", O_WRONLY | O_TRUNC));
#else
#if defined(ONLINE_JUDGE)
gsh::MmapReader rd;
#else
gsh::BasicReader rd;
#endif
gsh::BasicWriter wt;
#endif
void Main() {
    using namespace std;
    using namespace gsh;
    using namespace gsh::itype;
    using namespace gsh::ftype;
    using namespace gsh::ctype;
    u32 N = rd.read<u8dig>();
    Arr<u32> A(N);
    for (u32 i = 0; i != N; ++i) A[i] = rd.read<u32>();
    auto res = LongestIncreasingSubsequence(A);
    wt.writeln(res.size());
    for (u32 x : res) {
        Formatter<u8dig>{}(wt, x);
        Formatter<c8>{}(wt, ' ');
    }
    Formatter<c8>{}(wt, '\n');
    /*
    //internal::StaticModint32Impl<998244353> mint;
    internal::DynamicModint32Impl mint;
    mint.set(998244353);
    //mint.set((51ull << 53) + 1);
    //mint.set(4 * 123456789ull + 3);
    auto a = mint.build(2u), b = mint.build(3u), c = mint.build(5u);
    ClockTimer t;
    [&]() __attribute__((noinline)) {
        for (u32 i = 0; i != 100000000; ++i) {
            auto d = mint.neg(mint.inc(mint.sub(mint.dec(mint.add(mint.mul(a, a), b)), c)));
            auto e = mint.neg(mint.inc(mint.sub(mint.dec(mint.add(mint.mul(b, b), c)), a)));
            auto f = mint.neg(mint.inc(mint.sub(mint.dec(mint.add(mint.mul(c, c), a)), b)));
            a = d, b = e, c = f;
        }
        R"(
        for (u32 i = 0; i != 200000; ++i) {
            auto tmp_a = mint.sqrt(a);
            auto tmp_b = mint.sqrt(b);
            auto tmp_c = mint.sqrt(c);
            if (tmp_a) assert(mint.same(mint.mul(*tmp_a, *tmp_a), a));
            if (tmp_b) assert(mint.same(mint.mul(*tmp_b, *tmp_b), b));
            if (tmp_c) assert(mint.same(mint.mul(*tmp_c, *tmp_c), c));
            a = mint.mul(tmp_a ? tmp_a.val() : a, a);
            b = mint.mul(tmp_b ? tmp_b.val() : b, b);
            c = mint.mul(tmp_c ? tmp_c.val() : c, c);
        }
        )";
    }
    ();
    t.print();
    Formatter<u64>{}(w, mint.val(a));
    Formatter<c8>{}(w, '\n');
    Formatter<u64>{}(w, mint.val(b));
    Formatter<c8>{}(w, '\n');
    Formatter<u64>{}(w, mint.val(c));
    Formatter<c8>{}(w, '\n');
    */
}
int main() {
#ifdef ONLINE_JUDGE
    Main();
    wt.reload();
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
