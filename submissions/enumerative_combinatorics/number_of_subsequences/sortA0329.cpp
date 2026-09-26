#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")
#if defined(EVAL) || defined(ONLINE_JUDGE)
#define NDEBUG
#endif
#include <bit>       
#include <cstdlib>   
#include <cstring>   
#include <unistd.h>  
#ifndef _WIN32
#include <sys/mman.h>  
#include <sys/stat.h>  
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
    using i128 = __int128_t;
    using u128 = __uint128_t;
    using isize = i32;
    using usize = u32;
    struct i4dig {
        i16 val;
        constexpr bool check() const noexcept { return val < 10000 && -val < 10000; }
    };
    struct u4dig {
        u16 val;
        constexpr bool check() const noexcept { return val < 10000; }
    };
    struct i8dig {
        i32 val;
        constexpr bool check() const noexcept { return val < 100000000 && -val < 100000000; }
    };
    struct u8dig {
        u32 val;
        constexpr bool check() const noexcept { return val < 100000000; }
    };
    struct i16dig {
        i64 val;
        constexpr bool check() const noexcept { return val < 10000000000000000 && -val < 10000000000000000; }
    };
    struct u16dig {
        u64 val;
        constexpr bool check() const noexcept { return val < 10000000000000000; }
    };
}  

namespace ftype {
    
    using f32 = float;
    using f64 = double;
    using f128 = __float128;
    using flong = long double;
}  

namespace ctype {
    using c8 = char;
    using wc = wchar_t;
    using utf8 = char8_t;
    using utf16 = char16_t;
    using utf32 = char32_t;
}  

namespace simd {

    using i8x32 = __attribute__((vector_size(32))) itype::i8;
    using u8x32 = __attribute__((vector_size(32))) itype::u8;
    using i16x16 = __attribute__((vector_size(32))) itype::i16;
    using u16x16 = __attribute__((vector_size(32))) itype::u16;
    using i32x8 = __attribute__((vector_size(32))) itype::i32;
    using u32x8 = __attribute__((vector_size(32))) itype::u32;
    using i64x4 = __attribute__((vector_size(32))) itype::i64;
    using u64x4 = __attribute__((vector_size(32))) itype::u64;
    
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
    
    using f32x16 = __attribute__((vector_size(64))) ftype::f32;
    using f64x8 = __attribute__((vector_size(64))) ftype::f64;

    template<class T, class U> constexpr T VectorCast(U x) {
        return __builtin_convertvector(x, T);
    }

}  

namespace internal {
    template<class T, class U> constexpr bool IsSame = false;
    template<class T> constexpr bool IsSame<T, T> = true;
    template<class T, class U, class... V> constexpr bool IsSameAny = IsSame<T, U> || IsSameAny<T, V...>;
    template<class T, class U> constexpr bool IsSameAny<T, U> = IsSame<T, U>;
}  

namespace simd {
    template<class T> concept Is256BitVector = internal::IsSameAny<T, i8x32, i16x16, i32x8, i64x4, u8x32, u16x16, u32x8, u64x4, f32x8, f64x4>;
    template<class T> concept Is512BitVector = internal::IsSameAny<T, i8x64, i16x32, i32x16, i64x8, u8x64, u16x32, u32x16, u64x8, f32x16, f64x8>;
    template<class T> concept IsVector = Is256BitVector<T> || Is512BitVector<T>;
}  

class Byte {
    itype::u8 b = 0;
public:
    friend constexpr Byte operator&(Byte l, Byte r) noexcept { return Byte::from_integer(l.b & r.b); }
    constexpr Byte& operator&=(Byte r) noexcept {
        b &= r.b;
        return *this;
    }
    friend constexpr Byte operator|(Byte l, Byte r) noexcept { return Byte::from_integer(l.b | r.b); }
    constexpr Byte& operator|=(Byte r) noexcept {
        b |= r.b;
        return *this;
    }
    friend constexpr Byte operator^(Byte l, Byte r) noexcept { return Byte::from_integer(l.b ^ r.b); }
    constexpr Byte& operator^=(Byte r) noexcept {
        b ^= r.b;
        return *this;
    }
    template<class IntType> friend constexpr Byte operator<<(Byte l, IntType r) noexcept { return Byte::from_integer(l.b << r); }
    template<class IntType> constexpr Byte& operator<<=(IntType r) noexcept {
        b <<= r;
        return *this;
    }
    template<class IntType> friend constexpr Byte operator>>(Byte l, IntType r) noexcept { return Byte::from_integer(l.b >> r); }
    template<class IntType> constexpr Byte& operator>>=(IntType r) noexcept {
        b >>= r;
        return *this;
    }
    friend constexpr Byte operator~(Byte l) noexcept { return Byte::from_integer(~l.b); }
    template<class IntType> constexpr IntType to_integer() noexcept { return static_cast<IntType>(b); }
    template<class IntType> static constexpr Byte from_integer(IntType l) noexcept {
        Byte res;
        res.b = static_cast<itype::u8>(l);
        return res;
    }
};  

}  

namespace gsh {

template<class T> class Parser;

namespace internal {
    template<class Stream> constexpr itype::u8 Parseu8(Stream& stream) {
        itype::u32 v;
        std::memcpy(&v, stream.current(), 4);
        v ^= 0x30303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0) >> 3;
        v <<= (32 - (tmp << 3));
        stream.skip(tmp + 1);
        v = (v * 10 + (v >> 8)) & 0x00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff;
        return v;
    }
    template<class Stream> constexpr itype::u16 Parseu16(Stream& stream) {
        itype::u64 v;
        std::memcpy(&v, stream.current(), 8);
        v ^= 0x3030303030303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
        v <<= (64 - (tmp << 3));
        stream.skip(tmp + 1);
        v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
        v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
        return v;
    }
    template<class Stream> constexpr itype::u32 Parseu32(Stream& stream) {
        itype::u32 res = 0;
        {
            itype::u64 v;
            std::memcpy(&v, stream.current(), 8);
            if (!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                res = v;
                stream.skip(8);
            }
        }
        itype::u64 buf;
        std::memcpy(&buf, stream.current(), 8);
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
            std::memcpy(&v, stream.current(), 8);
            if (!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                stream.skip(8);
                itype::u64 u;
                std::memcpy(&u, stream.current(), 8);
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
        std::memcpy(&buf, stream.current(), 8);
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
    template<class Stream> constexpr itype::u8dig Parseu8dig(Stream& stream) {
        itype::u64 v;
        std::memcpy(&v, stream.current(), 8);
        v ^= 0x3030303030303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
        v <<= (64 - (tmp << 3));
        stream.skip(tmp + 1);
        v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
        v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
        return itype::u8dig{ static_cast<itype::u32>(v) };
    }
}  

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
        if (neg) stream.skip(1);
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
        if (neg) stream.skip(1);
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
        if (neg) stream.skip(1);
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
        if (neg) stream.skip(1);
        itype::i64 tmp = internal::Parseu64(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u8dig> {
public:
    template<class Stream> constexpr itype::u8dig operator()(Stream& stream) const {
        stream.reload(16);
        return internal::Parseu8dig(stream);
    }
};
template<> class Parser<itype::i8dig> {
public:
    template<class Stream> constexpr itype::i8dig operator()(Stream& stream) const {
        stream.reload(16);
        bool neg = *stream.current() == '-';
        if (neg) stream.skip(1);
        itype::i32 tmp = internal::Parseu8dig(stream).val;
        if (neg) tmp = -tmp;
        return itype::i8dig{ tmp };
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



template<class T> class Formatter;

namespace internal {
    template<int> constexpr auto InttoStr = [] {
        struct {
            ctype::c8 table[40004];
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
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
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
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
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
        auto copy1 = [&](itype::u64 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u64 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
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
    template<class Stream> constexpr void Formatu8dig(Stream& stream, itype::u8dig x) {
        const itype::u64 n = x.val;
        auto copy1 = [&](itype::u64 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u64 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 10000) copy1(n);
        else {
            copy1(n / 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu16dig(Stream& stream, itype::u16dig x) {
        const itype::u64 n = x.val;
        auto copy1 = [&](itype::u64 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u64 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
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
}  

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
template<> class Formatter<itype::u8dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u8dig n) const {
        stream.reload(8);
        internal::Formatu8dig(stream, n);
    }
};
template<> class Formatter<itype::i8dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i8dig n) const {
        stream.reload(9);
        *stream.current() = '-';
        stream.skip(n.val < 0);
        internal::Formatu8dig(stream, itype::u8dig{ n.val < 0 ? -n.val : n.val });
    }
};
template<> class Formatter<itype::u16dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u16dig n) const {
        stream.reload(16);
        internal::Formatu16dig(stream, n);
    }
};
template<> class Formatter<itype::i16dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i16dig n) const {
        stream.reload(17);
        *stream.current() = '-';
        stream.skip(n.val < 0);
        internal::Formatu16dig(stream, itype::u16dig{ n.val < 0 ? -n.val : n.val });
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
template<> class Formatter<const ctype::c8*> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, const ctype::c8* s) const {}
};

class BasicReader {
    constexpr static itype::u32 Bufsize = 1 << 18;
    itype::i32 fd = 0;
    ctype::c8 buf[Bufsize + 1];
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
class MmapReader {
    const itype::i32 fh;
    ctype::c8* buf;
    ctype::c8 *cur, *eof;
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
class StaticStrReader {
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

class BasicWriter {
    constexpr static itype::u32 Bufsize = 1 << 18;
    itype::i32 fd = 1;
    ctype::c8 buf[Bufsize];
    ctype::c8 *cur = buf, *eof = buf + Bufsize;
public:
    BasicWriter() {}
    BasicWriter(itype::i32 filehandle) : fd(filehandle) {}
    BasicWriter(const BasicWriter& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
        cur = buf + (rhs.cur - rhs.buf);
    }
    ~BasicWriter() { reload(); }
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
class StaticStrWriter {
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

}  

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

}  

#include <type_traits>         
#include <cstring>             
#include <cstdlib>             
#include <algorithm>           
     
#include <iterator>           
#include <algorithm>          
#include <initializer_list>   
#include <type_traits>        
#include <concepts>           
#include <cstring>            
#include <utility>            
#include <tuple>              
    
  
#include <type_traits>      
#include <concepts>         
#include <utility>          
#include <iterator>         
#include <ranges>           
#include <tuple>            
  

namespace gsh {

template<class R> concept Range = std::ranges::range<R>;
template<class R, class T> concept Rangeof = Range<R> && std::same_as<T, std::ranges::range_value_t<R>>;
template<class R> concept InputRange = std::ranges::input_range<R>;
template<class R, class T> concept OutputRange = Range<R> && std::ranges::output_range<R, T>;
template<class R> concept ForwardRange = std::ranges::forward_range<R>;
template<class R> concept BidirectionalRange = std::ranges::bidirectional_range<R>;
template<class R> concept RandomAccessRange = std::ranges::random_access_range<R>;
enum class RangeKind { Sized, Unsized };

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

    template<internal::same_ncvr<R> T> static constexpr auto size(T&& r) { return std::ranges::size(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto ssize(T&& r) { return std::ranges::ssize(std::forward<T>(r)); }
    template<internal::same_ncvr<R> T> static constexpr auto empty(T&& r) { return std::ranges::empty(std::forward<T>(r)); }

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
    constexpr D& get_ref() { return *reinterpret_cast<D*>(this); }
    constexpr const D& get_ref() const { return *reinterpret_cast<const D*>(this); }
    constexpr auto get_begin() { return get_ref().begin(); }
    constexpr auto get_begin() const { return get_ref().cbegin(); }
    constexpr auto get_end() { return get_ref().end(); }
    constexpr auto get_end() const { return get_ref().cend(); }
    constexpr auto get_rbegin() { return get_ref().rbegin(); }
    constexpr auto get_rbegin() const { return get_ref().crbegin(); }
    constexpr auto get_rend() { return get_ref().rend(); }
    constexpr auto get_rend() const { return get_ref().crend(); }
public:
    using derived_type = D;
    using value_type = V;
    constexpr derived_type copy() const& { return get_ref(); }
    constexpr derived_type copy() & { return get_ref(); }
    constexpr derived_type copy() && { return std::move(get_ref()); }
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
        for (const auto& el : get_ref())
            if (!f(el)) return false;
        return true;
    }
    constexpr bool all_of(const value_type& x) const {
        for (const auto& el : get_ref())
            if (!(el == x)) return false;
        return true;
    }
    template<std::predicate<value_type> Pred> constexpr bool any_of(Pred f) const {
        for (const auto& el : get_ref())
            if (f(el)) return true;
        return false;
    }
    constexpr bool any_of(const value_type& x) const {
        for (const auto& el : get_ref())
            if (el == x) return true;
        return false;
    }
    template<std::predicate<value_type> Pred> constexpr bool none_of(Pred f) const {
        for (const auto& el : get_ref())
            if (f(el)) return false;
        return true;
    }
    constexpr bool none_of(const value_type& x) const {
        for (const auto& el : get_ref())
            if (el == x) return false;
        return true;
    }
    constexpr bool contains(const value_type& x) const {
        for (const auto& el : get_ref())
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
        for (const auto& el : get_ref()) res += (el == x);
        return res;
    }
};

namespace internal {
    template<class T, class U> concept difference_from = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
    template<class From, class To> concept convertible_to_non_slicing = std::convertible_to<From, To> && !(std::is_pointer_v<std::decay_t<From>> && std::is_pointer_v<std::decay_t<To>> && !std::convertible_to<std::remove_pointer_t<std::decay_t<From>> (*)[], std::remove_pointer_t<std::decay_t<To>> (*)[]>);
    template<class T> concept pair_like =  std::tuple_size_v<std::remove_cvref_t<T>> == 2;
    template<class T, class U, class V> concept pair_like_convertible_from = !std::ranges::range<T> && !std::is_reference_v<T> && pair_like<T> && std::constructible_from<T, U, V> && convertible_to_non_slicing<U, std::tuple_element_t<0, T>> && std::convertible_to<V, std::tuple_element_t<1, T>>;
}  
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

}  

namespace std::ranges {
template<class I, class S, gsh::RangeKind K> constexpr bool enable_borrowed_range<gsh::Subrange<I, S, K>> = true;
}      
#include <type_traits>      
#include <limits>           
#include <utility>          
#include <iterator>         
#include <cstdlib>          
#include <new>              
  

namespace gsh {

template<class T, class... Args> constexpr T* ConstructAt(T* location, Args&&... args) {
    auto ptr = ::new (const_cast<void*>(static_cast<const volatile void*>(location))) T(std::forward<Args>(args)...);
    if constexpr (std::is_array_v<T>) return std::launder(location);
    else return ptr;
}
template<class T> constexpr void DestroyAt(T* location) {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        if constexpr (std::is_array_v<T>) {
            auto beg = std::begin(*location);
            auto end = std::end(*location);
            for (auto itr = beg; itr != end; ++itr) DestroyAt(itr);
        } else location->~T();
    }
}

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
}  

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
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type align, size_type n) { return a.allocate(align, n); }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint) {
        if constexpr (requires { a.allocate(n, hint); }) return a.allocate(n, hint);
        else return a.allocate(n);
    }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type align, size_type n, const_void_pointer hint) {
        if constexpr (requires { a.allocate(align, n, hint); }) return a.allocate(align, n, hint);
        else return a.allocate(align, n);
    }
    static constexpr void deallocate(Alloc& a, pointer p, size_type n) { a.deallocate(p, n); }
    [[nodiscard]] static constexpr pointer reallocate(Alloc& a, pointer p, size_type prev_size, size_type new_size) {
        if constexpr (requires { a.reallocate(p, prev_size, new_size); }) return a.reallocate(p, prev_size, new_size);
        else {
            deallocate(a, p, prev_size);
            return allocate(a, new_size, p);
        }
    }
    static constexpr size_type max_size(const Alloc& a) noexcept {
        if constexpr (requires { a.max_size(); }) return a.max_size();
        else return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
    template<class T, class... Args> static constexpr void construct(Alloc& a, T* p, Args&&... args) {
        if constexpr (requires { a.construct(p, std::forward<Args>(args)...); }) a.construct(p, std::forward<Args>(args)...);
        else ConstructAt(p, std::forward<Args>(args)...);
    }
    template<class T> static constexpr void destroy(Alloc& a, T* p) {
        if constexpr (requires { a.destroy(p); }) a.destroy(p);
        else DestroyAt(p);
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
    constexpr ~Allocator() = default;
    [[nodiscard]] constexpr T* allocate(size_type n) {
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(alignof(T))));
        else return static_cast<T*>(::operator new(sizeof(T) * n));
    }
    [[nodiscard]] constexpr T* allocate(size_type align, size_type n) { return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(align))); }
    constexpr void deallocate(T* p, size_type n) {
        if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, n, static_cast<std::align_val_t>(alignof(T)));
        else ::operator delete(p, n);
    }
    
    constexpr Allocator& operator=(const Allocator&) = default;
    template<class U> friend constexpr bool operator==(const Allocator&, const Allocator<U>&) noexcept { return true; }
};

template<itype::u32 Size> class MemoryPool {
    template<class T> friend class PoolAllocator;
    itype::u32 cnt = 0;
    itype::u32 ref = 0;
    itype::u8 buf[Size];
public:
    constexpr ~MemoryPool() noexcept(false) {
        if (ref != 0) throw Exception("gsh::MemoryPool::~MemoryPool / There are some gsh::PoolAllocator tied to this object have not yet been destroyed.");
    }
};
template<class T> class PoolAllocator {
    template<class U> friend class PoolAllocator;
    itype::u32* cnt;
    itype::u32* ref;
    itype::u8* buf;
public:
    using value_type = T;
    using propagate_on_container_copy_assignmant = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using size_type = itype::u32;
    using difference_type = itype::i32;
    using is_always_equal = std::false_type;
    constexpr PoolAllocator() noexcept : cnt(nullptr), ref(nullptr), buf(nullptr) {}
    constexpr PoolAllocator(const PoolAllocator& a) : cnt(a.cnt), ref(a.ref), buf(a.buf) { ++*ref; }
    template<class U> constexpr PoolAllocator(const PoolAllocator<U>& a) : cnt(a.cnt), ref(a.ref), buf(a.buf) { ++*ref; }
    template<itype::u32 Size> constexpr PoolAllocator(MemoryPool<Size>& p) noexcept : cnt(&p.cnt), ref(&p.ref), buf(p.buf) { ++*ref; }
    constexpr ~PoolAllocator() noexcept {
        if (ref != nullptr) --*ref;
    }
    constexpr T* allocate(size_type n) {
        auto res = reinterpret_cast<T*>(buf + *cnt);
        *cnt += sizeof(T) * n;
        return res;
    }
    constexpr void deallocate(T*, size_type) {}
    constexpr PoolAllocator& operator=(const PoolAllocator& a) {
        if (ref != nullptr) --*ref;
        cnt = a.cnt, ref = a.ref, buf = a.buf;
        ++*ref;
        return *this;
    }
    template<class U> friend constexpr bool operator==(const PoolAllocator& a, const PoolAllocator<U>& b) { return a.cnt == b.cnt && a.ref == b.ref && a.buf == b.buf; }
};

}  
     

namespace gsh {

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
    constexpr explicit Arr(size_type n, const Allocator& a = Allocator()) : alloc(a) {
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n;
        if constexpr (!std::is_trivially_default_constructible_v<value_type>)
            for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i);
        else std::memset(ptr, 0, sizeof(value_type) * n);
    }
    constexpr explicit Arr(const size_type n, const value_type& value, const allocator_type& a = Allocator()) : alloc(a) {
        if (n == 0) [[unlikely]]
            return;
        ptr = traits::allocate(alloc, n);
        len = n;
        for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i, value);
    }
    template<std::input_iterator InputIter> constexpr Arr(const InputIter first, const InputIter last, const allocator_type& a = Allocator()) : alloc(a) {
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
        if (std::is_trivially_copy_constructible_v<value_type> && !std::is_constant_evaluated()) {
            std::memcpy(ptr, x.ptr, sizeof(value_type) * len);
        } else {
            for (size_type i = 0; i != len; ++i) traits::construct(alloc, ptr + i, *(x.ptr + i));
        }
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
            if constexpr (std::is_trivially_move_constructible_v<value_type> && !std::is_constant_evaluated()) {
                std::memcpy(ptr, x.ptr, sizeof(value_type) * len);
            } else {
                for (size_type i = 0; i != len; ++i) traits::construct(alloc, ptr + i, std::move(*(x.ptr + i)));
            }
            traits::deallocate(x.alloc, x.ptr, x.len);
            x.ptr = nullptr, x.len = 0;
        }
    }
    constexpr Arr(std::initializer_list<value_type> il, const allocator_type& a = Allocator()) : Arr(il.begin(), il.end(), a) {}
    template<Rangeof<value_type> R> constexpr Arr(R&& r, const allocator_type& a = Allocator()) : Arr(RangeTraits<R>::fbegin(r), RangeTraits<R>::fend(r), a) {}
    constexpr ~Arr() {
        if (len != 0) {
            if constexpr (!std::is_trivially_destructible_v<value_type>)
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
    }
    constexpr Arr& operator=(const Arr& x) {
        if (&x == this) return *this;
        if constexpr (!std::is_trivially_destructible_v<value_type>)
            for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        if (traits::propagate_on_container_copy_assignment::value || len != x.len) {
            if (len != 0) traits::deallocate(alloc, ptr, len);
            if constexpr (traits::propagate_on_container_copy_assignment::value) alloc = x.alloc;
            ptr = traits::allocate(alloc, x.len);
        }
        len = x.len;
        if (std::is_trivially_copy_assignable_v<value_type> && !std::is_constant_evaluated()) {
            std::memcpy(ptr, x.ptr, sizeof(value_type) * len);
        } else {
            for (size_type i = 0; i != len; ++i) *(ptr + i) = *(x.ptr + i);
        }
        return *this;
    }
    constexpr Arr& operator=(Arr&& x) noexcept(traits::propagate_on_container_move_assignment::value || traits::is_always_equal::value) {
        if (&x == this) return *this;
        if (len != 0) {
            if constexpr (!std::is_trivially_destructible_v<value_type>)
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
            if (std::is_trivially_move_constructible_v<value_type> && !std::is_constant_evaluated()) {
                std::memcpy(new_ptr, ptr, sizeof(value_type) * mn);
            } else {
                for (size_type i = 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
            }
            if constexpr (!std::is_trivially_destructible_v<value_type>)
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
        ptr = new_ptr;
        if constexpr (!std::is_trivially_default_constructible_v<value_type>)
            for (size_type i = len; i < sz; ++i) traits::construct(alloc, ptr + i);
        else if (len < sz) std::memset(ptr + len, 0, sizeof(value_type) * (sz - len));
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
            if (std::is_trivially_move_constructible_v<value_type> && !std::is_constant_evaluated()) {
                std::memcpy(new_ptr, ptr, sizeof(value_type) * mn);
            } else {
                for (size_type i = 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
            }
            if constexpr (!std::is_trivially_destructible_v<value_type>)
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
        }
        ptr = new_ptr;
        for (size_type i = len; i < sz; ++i) traits::construct(alloc, ptr + i, c);
        len = sz;
    }
    [[nodiscard]] constexpr bool empty() const noexcept { return len == 0; }
    __attribute__((always_inline)) constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
        return *(ptr + n);
    }
    __attribute__((always_inline)) constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
        return *(ptr + n);
    }
    __attribute__((always_inline)) constexpr reference at(const size_type n) {
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::at / The index is out of range. ( n=", n, ", size=", len, " )");
        return *(ptr + n);
    }
    __attribute__((always_inline)) constexpr const_reference at(const size_type n) const {
        if (n >= len) [[unlikely]]
            throw gsh::Exception("gsh::Arr::at / The index is out of range. ( n=", n, ", size=", len, " )");
        return *(ptr + n);
    }
    __attribute__((always_inline)) constexpr reference at_unchecked(const size_type n) { return *(ptr + n); }
    constexpr const_reference at_unchecked(const size_type n) const { return *(ptr + n); }
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
            if constexpr (!std::is_trivially_destructible_v<value_type>)
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
        } else if (n != 0) {
            if constexpr (!std::is_trivially_destructible_v<value_type>)
                for (size_type i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
            traits::deallocate(alloc, ptr, len);
            ptr = traits::allocate(alloc, n);
            len = n;
            for (size_type i = 0; i != n; ++i) traits::construct(alloc, ptr + i, t);
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
            if constexpr (!std::is_trivially_destructible_v<value_type>)
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

template<class T>
    requires std::same_as<T, std::remove_cv_t<T>>
class ArrInitTag {};
template<class T = void> constexpr ArrInitTag<T> ArrInit;
class ArrNoInitTag {};
constexpr ArrNoInitTag ArrNoInit;

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
        static_assert(std::is_same_v<T, U>, "gsh::StaticArr::StaticArr / The type specified in gsh::ArrInitTag is different from value_type.");
        static_assert(sizeof...(Args) <= N, "gsh::StaticArr::StaticArr / The number of arguments is greater than the length of the array.");
    }
    template<class... Args> constexpr StaticArr(ArrInitTag<void>, Args&&... args) : elems{ static_cast<T>(std::forward<Args>(args))... } { static_assert(sizeof...(Args) <= N, "gsh::StaticArr::StaticArr / The number of arguments is greater than the length of the array."); }
    constexpr explicit StaticArr(const value_type& value) {
        for (itype::u32 i = 0; i != N; ++i) ConstructAt(elems + i, value);
    }
    template<std::input_iterator InputIter> constexpr explicit StaticArr(InputIter first) {
        for (itype::u32 i = 0; i != N; ++first, ++i) ConstructAt(elems + i, *first);
    }
    template<std::input_iterator InputIter> constexpr StaticArr(InputIter first, const InputIter last) {
        const itype::u32 n = std::distance(first, last);
        if (n != N) throw gsh::Exception("gsh::StaticArr::StaticArr / The size of the given range differs from the size of the array.");
        for (itype::u32 i = 0; i != N; ++first, ++i) ConstructAt(elems + i, *first);
    }
    template<Rangeof<value_type> R> constexpr StaticArr(R&& r) : StaticArr(RangeTraits<R>::fbegin(r), RangeTraits<R>::fend(r)) {}
    constexpr StaticArr(const value_type (&a)[N]) {
        for (itype::u32 i = 0; i != N; ++i) ConstructAt(elems + i, a[i]);
    }
    constexpr StaticArr(value_type (&&a)[N]) {
        for (itype::u32 i = 0; i != N; ++i) ConstructAt(elems + i, std::move(a[i]));
    }
    constexpr StaticArr(const StaticArr& x) {
        for (itype::u32 i = 0; i != N; ++i) ConstructAt(elems + i, x.elems[i]);
    }
    constexpr StaticArr(StaticArr&& y) {
        for (itype::u32 i = 0; i != N; ++i) ConstructAt(elems + i, std::move(y.elems[i]));
    }
    constexpr StaticArr(std::initializer_list<value_type> il) : StaticArr(il.begin(), il.end()) {}
    constexpr ~StaticArr() noexcept {
        if constexpr (!std::is_trivially_destructible_v<value_type>)
            for (itype::u32 i = 0; i != N; ++i) DestroyAt(elems + i);
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
    __attribute__((always_inline)) constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::operator[] / The index is out of range. ( n=", n, ", size=", N, " )");
#endif
        return elems[n];
    }
    __attribute__((always_inline)) constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::operator[] / The index is out of range. ( n=", n, ", size=", N, " )");
#endif
        return elems[n];
    }
    __attribute__((always_inline)) constexpr reference at(const size_type n) {
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::at / The index is out of range. ( n=", n, ", size=", N, " )");
        return elems[n];
    }
    __attribute__((always_inline)) constexpr const_reference at(const size_type n) const {
        if (n >= N) [[unlikely]]
            throw gsh::Exception("gsh::StaticArr::at / The index is out of range. ( n=", n, ", size=", N, " )");
        return elems[n];
    }
    __attribute__((always_inline)) constexpr reference at_unchecked(const size_type n) { return elems[n]; }
    constexpr const_reference at_unchecked(const size_type n) const { return elems[n]; }
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

}  

namespace std {
template<class T, gsh::itype::u32 N> struct tuple_size<gsh::StaticArr<T, N>> : integral_constant<size_t, N> {};
template<std::size_t M, class T, gsh::itype::u32 N> struct tuple_element<M, gsh::StaticArr<T, N>> {
    static_assert(M < N, "std::tuple_element<gsh::StaticArr<T, N>> / The index is out of range.");
    using type = T;
};
}  

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
}  
       
#include <concepts>         
#include <type_traits>      
#include <utility>          
#include <cstddef>          
#include <bit>              
  

namespace gsh {

namespace internal {
    template<typename T, typename U> concept LessPtrCmp = requires(T&& t, U&& u) {
        { t < u } -> std::same_as<bool>;
    } && std::convertible_to<T, const volatile void*> && std::convertible_to<U, const volatile void*> && (!requires(T&& t, U&& u) { operator<(std::forward<T>(t), std::forward<U>(u)); } && !requires(T&& t, U&& u) { std::forward<T>(t).operator<(std::forward<U>(u)); });
}  
class Less {
public:
    template<class T, class U>
        requires std::totally_ordered_with<T, U>
    constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() < std::declval<U>())) {
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
    constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<U>() < std::declval<T>())) {
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
    constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() == std::declval<U>())) {
        return std::forward<T>(t) == std::forward<U>(u);
    }
    using is_transparent = void;
};

class Identity {
public:
    template<class T> [[nodiscard]]
    constexpr T&& operator()(T&& t) const noexcept {
        return std::forward<T>(t);
    }
    using is_transparent = void;
};

}  

namespace std {
template<class T> class hash;
}

namespace gsh {

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
}  


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
    template<class T, class U> constexpr auto operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) + std::forward<U>(u))) -> decltype(std::forward<T>(t) + std::forward<U>(u)) { return std::forward<T>(t) + std::forward<U>(u); }
    using is_transparent = void;
};
class Negate {
public:
    template<class T> constexpr auto operator()(T&& t) const noexcept(noexcept(-std::forward<T>(t))) -> decltype(-std::forward<T>(t)) { return -std::forward<T>(t); }
    using is_transparent = void;
};

}  
  

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

namespace internal {
    template<class T, bool Rev = false, class Proj = Identity> void SortUnsigned32(T* const p, const itype::u32 n, Proj proj = {}) {
        static itype::u32 cnt1[1 << 16], cnt2[1 << 16];
        std::memset(cnt1, 0, sizeof(cnt1));
        std::memset(cnt2, 0, sizeof(cnt2));
        for (itype::u32 i = 0; i != n; ++i) {
            auto tmp = proj(p[i]);
            const itype::u16 a = tmp & 0xffff;
            const itype::u16 b = tmp >> 16 & 0xffff;
            ++cnt1[a];
            ++cnt2[b];
        }
        for (itype::u32 i = 0; i != (1 << 16) - 1; ++i) {
            cnt1[i + 1] += cnt1[i];
            cnt2[i + 1] += cnt2[i];
        }
        T* const tmp = reinterpret_cast<T*>(std::malloc(sizeof(T) * n));
        if constexpr (Rev) {
            for (itype::u32 i = 0; i != n; ++i) tmp[--cnt1[proj(p[i]) & 0xffff]] = std::move(p[i]);
            for (itype::u32 i = 0; i != n; ++i) p[--cnt2[proj(tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
        } else {
            for (itype::u32 i = n; i != 0;) --i, tmp[--cnt1[proj(p[i]) & 0xffff]] = std::move(p[i]);
            for (itype::u32 i = n; i != 0;) --i, p[--cnt2[proj(tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
        }
        std::free(tmp);
    }
    template<class T, bool Rev = false, class Proj = Identity> void SortUnsigned64(T* const p, const itype::u32 n, Proj proj = {}) {
        static itype::u32 cnt1[1 << 16], cnt2[1 << 16], cnt3[1 << 16], cnt4[1 << 16];
        std::memset(cnt1, 0, sizeof(cnt1));
        std::memset(cnt2, 0, sizeof(cnt2));
        std::memset(cnt3, 0, sizeof(cnt3));
        std::memset(cnt4, 0, sizeof(cnt4));
        for (itype::u32 i = 0; i != n; ++i) {
            auto tmp = proj(p[i]);
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
        T* const tmp = reinterpret_cast<T*>(std::malloc(sizeof(T) * n));
        if constexpr (Rev) {
            for (itype::u32 i = 0; i != n; ++i) tmp[--cnt1[proj(p[i]) & 0xffff]] = std::move(p[i]);
            for (itype::u32 i = 0; i != n; ++i) p[--cnt2[proj(tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
            for (itype::u32 i = 0; i != n; ++i) tmp[--cnt3[proj(p[i]) >> 32 & 0xffff]] = std::move(p[i]);
            for (itype::u32 i = 0; i != n; ++i) p[--cnt4[proj(tmp[i]) >> 48 & 0xffff]] = std::move(tmp[i]);
        } else {
            for (itype::u32 i = n; i != 0;) --i, tmp[--cnt1[proj(p[i]) & 0xffff]] = std::move(p[i]);
            for (itype::u32 i = n; i != 0;) --i, p[--cnt2[proj(tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
            for (itype::u32 i = n; i != 0;) --i, tmp[--cnt3[proj(p[i]) >> 32 & 0xffff]] = std::move(p[i]);
            for (itype::u32 i = n; i != 0;) --i, p[--cnt4[proj(tmp[i]) >> 48 & 0xffff]] = std::move(tmp[i]);
        }
        std::free(tmp);
    }
}  


template<Range R> constexpr Arr<itype::u32> LongestIncreasingSubsequence(R&& r) {
    using T = typename RangeTraits<R>::value_type;
    Arr<T> dp(RangeTraits<R>::size(r));
    Arr<itype::u32> idx(dp.size());
    itype::u32 i = 0;
    T *begin = dp.data(), *last = dp.data();
    for (const T& x : r) {
        T* loc = std::lower_bound(begin, last, x);
        idx[i++] = loc - begin;
        last += (loc == last);
        *loc = x;
    }
    itype::u32 cnt = last - begin - 1;
    Arr<itype::u32> res(last - begin);
    for (itype::u32 i = dp.size(); i != 0;)
        if (idx[--i] == cnt) res[cnt--] = i;
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


}  
#include <type_traits>        
#include <limits>             
#include <bit>                
    
  
#include <type_traits>        
#include <utility>            
#include <initializer_list>   
#include <compare>            
#include <tuple>              


namespace gsh {

class InPlaceTag {};
constexpr InPlaceTag InPlace;

}  
       
  

namespace gsh {

class NullOpt {};
constexpr NullOpt Null;
template<class T> class Option {
    template<class U> friend class Option;
public:
    using value_type = T;
private:
    alignas(value_type) Byte buffer[sizeof(value_type)];
    bool has = false;
    template<class... Args> constexpr void construct(Args&&... args) { new (buffer) value_type(std::forward<Args>(args)...); }
    constexpr void destroy() {
        if constexpr (!std::is_trivially_destructible_v<value_type>) delete reinterpret_cast<value_type*>(buffer);
    }
    constexpr value_type& ref() { return *reinterpret_cast<value_type*>(buffer); }
    constexpr const value_type& ref() const { return *reinterpret_cast<const value_type*>(buffer); }
    template<class U> constexpr static bool is_explicit = !std::is_convertible_v<U, T>;
    constexpr static bool noexcept_swapable = []() {
        using std::swap;
        return noexcept(swap(std::declval<value_type&>(), std::declval<value_type&>()));
    }();
public:
    constexpr Option() noexcept {}
    constexpr ~Option() noexcept {
        if (has) destroy();
    }
    constexpr Option(NullOpt) noexcept {}
    constexpr Option(const Option& rhs) {
        if (rhs.has) {
            has = true;
            construct(rhs.ref());
        } else {
            has = false;
        }
    }
    constexpr Option(Option&& rhs) noexcept {
        if (rhs.has) {
            has = true;
            construct(std::move(rhs.ref()));
        } else {
            has = false;
        }
    }
    template<class... Args> constexpr explicit Option(InPlaceTag, Args&&... args) {
        has = true;
        construct(std::forward<Args>(args)...);
    }
    template<class U, class... Args> constexpr explicit Option(InPlaceTag, std::initializer_list<U> il, Args&&... args) {
        has = true;
        construct(il, std::forward<Args>(args)...);
    }
    template<class U = T>
        requires requires(U&& rhs) { static_cast<value_type>(std::forward<U>(rhs)); }
    explicit(is_explicit<U>) constexpr Option(U&& rhs) {
        has = true;
        construct(std::forward<U>(rhs));
    }
    template<class U> explicit(is_explicit<U>) constexpr Option(const Option<U>& rhs) {
        if (rhs.has) {
            has = true;
            construct(static_cast<value_type>(rhs.ref()));
        } else {
            has = false;
        }
    }
    template<class U> explicit(is_explicit<U>) constexpr Option(Option<U>&& rhs) {
        if (rhs.has) {
            has = true;
            construct(static_cast<value_type>(std::move(rhs.ref())));
        } else {
            has = false;
        }
    }
    constexpr Option& operator=(NullOpt) noexcept {
        if (has) {
            destroy();
            has = false;
        }
    }
    constexpr Option& operator=(const Option& rhs) {
        if (has) destroy();
        if (rhs.has) {
            has = true;
            construct(rhs.ref());
        } else {
            has = false;
        }
    }
    template<class... Args> constexpr T& emplace(Args&&... args) {
        if (has) destroy();
        has = true;
        construct(std::forward<Args>(args)...);
        return ref();
    }
    template<class U, class... Args> constexpr T& emplace(std::initializer_list<U> il, Args&&... args) {
        if (has) destroy();
        has = true;
        construct(il, std::forward<Args>(args)...);
        return ref();
    }
    constexpr void swap(Option& rhs) noexcept(noexcept_swapable) {
        using std::swap;
        swap(has, rhs.has);
        swap(ref(), rhs.ref());
    }
    constexpr void reset() noexcept {
        if (has) {
            destroy();
            has = false;
        }
    }
    constexpr T& operator*() & { return ref(); }
    constexpr T&& operator*() && { return std::move(ref()); }
    constexpr const T& operator*() const& { return ref(); }
    constexpr const T&& operator*() const&& { return std::move(ref()); }
    constexpr const T* operator->() const { return reinterpret_cast<const T*>(buffer); }
    constexpr T* operator->() { return reinterpret_cast<T*>(buffer); }
    constexpr explicit operator bool() const noexcept { return has; }
    constexpr bool has_value() const noexcept { return has; }
    constexpr const T& value() const& {
        if (!has) throw gsh::Exception("gsh::Option::value / The object doesn't have value.");
        return ref();
    }
    constexpr T& value() & {
        if (!has) throw gsh::Exception("gsh::Option::value / The object doesn't have value.");
        return ref();
    }
    constexpr T&& value() && {
        if (!has) throw gsh::Exception("gsh::Option::value / The object doesn't have value.");
        return std::move(ref());
    }
    constexpr const T&& value() const&& {
        if (!has) throw gsh::Exception("gsh::Option::value / The object doesn't have value.");
        return std::move(ref());
    }
    template<class U> constexpr T value_or(U&& v) const& { return has ? ref() : static_cast<T>(std::forward<U>(v)); }
    template<class U> constexpr T value_or(U&& v) && { return has ? static_cast<T>(std::move(ref())) : static_cast<T>(std::forward<U>(v)); }
};
template<class T> Option(T) -> Option<T>;

template<class T, class U> constexpr bool operator==(const Option<T>& x, const Option<U>& y) {
    if (x.has_value() && y.has_value()) return *x == *y;
    else return x.has_value() ^ y.has_value();
}
template<class T> constexpr bool operator==(const Option<T>& x, NullOpt) noexcept {
    return !x.has_value();
}
template<class T, class U> constexpr bool operator==(const Option<T>& x, const U& y) {
    return x.has_value() && (*x == y);
}
template<class T, class U> constexpr bool operator==(const T& x, const Option<U>& y) {
    return y.has_value() && (x == *y);
}
template<class T, std::three_way_comparable_with<T> U> constexpr std::compare_three_way_result_t<T, U> operator<=>(const Option<T>& x, const Option<U>& y) {
    return x.has_value() && y.has_value() ? *x <=> *y : x.has_value() <=> y.has_value();
}
template<class T> constexpr std::strong_ordering operator<=>(const Option<T>& x, NullOpt) noexcept {
    return x.has_value <=> false;
}
template<class T, std::three_way_comparable_with<T> U> constexpr std::compare_three_way_result_t<T, U> operator<=>(const Option<T>& x, const U& y) {
    return x.has_value() ? *x <=> y : std::strong_ordering::less;
}

}  

namespace std {
template<class T> struct tuple_size<gsh::Option<T>> : integral_constant<size_t, 2> {};
template<class T> struct tuple_element<0, gsh::Option<T>> {
    using type = T;
};
template<class T> struct tuple_element<1, gsh::Option<T>> {
    using type = bool;
};
}  

namespace gsh {
template<std::size_t N, class T> auto get(const Option<T>& x) {
    if constexpr (N == 0) {
        if (x.has_value()) return *x;
        else return T();
    } else return x.has_value();
}
template<std::size_t N, class T> auto get(Option<T>& x) {
    if constexpr (N == 0) {
        if (x.has_value()) return *x;
        else return T();
    } else return x.has_value();
}
template<std::size_t N, class T> auto get(Option<T>&& x) {
    if constexpr (N == 0) {
        if (x.has_value()) return std::move(*x);
        else return T();
    } else return x.has_value();
}
}  

namespace gsh {

namespace internal {

    
    template<class T> constexpr T calc_gcd(T x, T y) {
        if (x == 0 || y == 0) [[unlikely]]
            return x | y;
        const itype::i32 n = std::countr_zero(x);
        const itype::i32 m = std::countr_zero(y);
        const itype::i32 l = n < m ? n : m;
        x >>= n;
        y >>= m;
        while (x != y) {
            const T s = y < x ? x - y : y - x;
            const itype::i32 t = std::countr_zero(s);
            y = y < x ? y : x;
            x = s >> t;
        }
        return x << l;
    }

    template<class T> class ModintImpl : public T {
        using base_type = T;
    public:
        using value_type = std::decay_t<decltype(base_type::mod())>;
        using modint_type = ModintImpl;
        constexpr static bool is_staticmod = !requires { base_type::set_mod(0); };
        constexpr ModintImpl() noexcept : T() {}
        template<class U> constexpr ModintImpl(U x) noexcept { operator=(x); }
        constexpr explicit operator value_type() const noexcept { return val(); }
        constexpr static void set_mod(value_type x) {
            static_assert(!is_staticmod, "gsh::internal::ModintImpl::set_mod / Mod must be dynamic.");
            if (x <= 1) throw Exception("gsh::internal::ModintImpl::set_mod / Mod must be at least 2.");
            if (x == mod()) return;
            base_type::set_mod(x);
        }
        constexpr value_type val() const noexcept { return base_type::val(); }
        constexpr static value_type mod() noexcept { return base_type::mod(); }
        template<class U> constexpr modint_type& operator=(U x) noexcept {
            static_assert(std::is_integral_v<U>, "gsh::internal::ModintImpl::operator= / Only integer types can be assigned.");
            if constexpr (std::is_unsigned_v<U>) {
                if constexpr (std::is_same_v<U, unsigned long long> || std::is_same_v<U, unsigned long>) base_type::assign(static_cast<itype::u64>(x));
                else base_type::assign(static_cast<itype::u32>(x));
            } else {
                if (x < 0) {
                    if constexpr (std::is_same_v<U, long long> || std::is_same_v<U, long>) base_type::assign(static_cast<itype::u64>(-x));
                    else base_type::assign(static_cast<itype::u32>(-x));
                    base_type::neg();
                } else {
                    if constexpr (std::is_same_v<U, long long> || std::is_same_v<U, long>) base_type::assign(static_cast<itype::u64>(x));
                    else base_type::assign(static_cast<itype::u32>(x));
                }
            }
            return *this;
        }
        constexpr static modint_type raw(value_type x) noexcept {
            modint_type res;
            res.rawassign(x);
            return res;
        }
        template<class Istream> friend Istream& operator>>(Istream& ist, modint_type& x) {
            value_type n;
            ist >> n;
            x = n;
            return ist;
        }
        template<class Ostream> friend Ostream& operator<<(Ostream& ost, modint_type x) { return ost << x.val(); }
        constexpr Option<modint_type> inv() const noexcept {
            value_type a = 1, b = 0, x = val(), y = mod();
            if (x == 0) return Null;
            while (true) {
                if (x <= 1) {
                    if (x == 0) [[unlikely]]
                        break;
                    else return modint_type(a);
                }
                b += a * (y / x);
                y %= x;
                if (y <= 1) {
                    if (y == 0) [[unlikely]]
                        break;
                    else return modint_type(mod() - b);
                }
                a += b * (x / y);
                x %= y;
            }
            return Null;
        }
        constexpr modint_type pow(itype::u64 e) const noexcept {
            modint_type res = modint_type::raw(1), pow = *this;
            while (e) {
                modint_type tmp = pow * pow;
                if (e & 1) res *= pow;
                pow = tmp;
                e >>= 1;
            }
            return res;
        }
        constexpr modint_type operator+() const noexcept { return *this; }
        constexpr modint_type operator-() const noexcept {
            modint_type res = *this;
            res.neg();
            return res;
        }
        constexpr modint_type& operator++() noexcept {
            base_type::inc();
            return *this;
        }
        constexpr modint_type& operator--() noexcept {
            base_type::dec();
            return *this;
        }
        constexpr modint_type operator++(int) noexcept {
            modint_type copy = *this;
            operator++();
            return copy;
        }
        constexpr modint_type operator--(int) noexcept {
            modint_type copy = *this;
            operator--();
            return copy;
        }
        constexpr modint_type& operator+=(modint_type x) noexcept {
            base_type::add(x);
            return *this;
        }
        constexpr modint_type& operator-=(modint_type x) noexcept {
            base_type::sub(x);
            return *this;
        }
        constexpr modint_type& operator*=(modint_type x) noexcept {
            base_type::mul(x);
            return *this;
        }
        constexpr modint_type& operator/=(modint_type x) {
            auto iv = x.inv();
            if (!iv) throw gsh::Exception("gsh::internal::ModintImpl::operator/= / Cannot calculate inverse.");
            operator*=(*iv);
            return *this;
        }
        friend constexpr modint_type operator+(modint_type l, modint_type r) noexcept { return modint_type(l) += r; }
        friend constexpr modint_type operator-(modint_type l, modint_type r) noexcept { return modint_type(l) -= r; }
        friend constexpr modint_type operator*(modint_type l, modint_type r) noexcept { return modint_type(l) *= r; }
        friend constexpr modint_type operator/(modint_type l, modint_type r) { return modint_type(l) /= r; }
        friend constexpr bool operator==(modint_type l, modint_type r) noexcept { return l.val() == r.val(); }
        friend constexpr bool operator!=(modint_type l, modint_type r) noexcept { return l.val() != r.val(); }
        constexpr int legendre() const noexcept {
            value_type res = pow((mod() - 1) >> 1).val();
            return (res <= 1 ? static_cast<int>(res) : -1);
        }
        constexpr int jacobi(bool skip_calc_gcd = false) const noexcept {
            value_type a = val(), n = mod();
            if (a == 1) return 1;
            if (!skip_calc_gcd && calc_gcd(a, n) != 1) return 0;
            int res = 1;
            while (a != 0) {
                while (!(a & 1) && a != 0) {
                    a >>= 1;
                    res = ((n & 0b111) == 3 || (n & 0b111) == 5) ? -res : res;
                }
                res = ((a & 0b11) == 3 || (n & 0b11) == 3) ? -res : res;
                value_type tmp = n;
                n = a;
                a = tmp;
                a %= n;
            }
            return n == 1 ? res : 0;
        }
        constexpr Option<modint_type> sqrt() const noexcept {
            const value_type vl = val(), md = mod();
            if (vl <= 1) return *this;
            auto get_min = [](modint_type x) {
                return x.val() > (mod() >> 1) ? -x : x;
            };
            if ((md & 0b11) == 3) {
                modint_type res = pow((md + 1) >> 2);
                if (res * res != *this) return Null;
                else return get_min(res);
            } else if ((md & 0b111) == 5) {
                modint_type res = pow((md + 3) >> 3);
                if constexpr (is_staticmod) {
                    constexpr modint_type p = modint_type::raw(2).pow((md - 1) >> 2);
                    res *= p;
                } else if (res * res != *this) res *= modint_type::raw(2).pow((md - 1) >> 2);
                if (res * res != *this) return Null;
                else return get_min(res);
            } else {
                const itype::u32 S = std::countr_zero(md - 1);
                const value_type Q = (md - 1) >> S;
                const itype::u32 W = std::bit_width(md);
                if (S * S <= 12 * W) {
                    const modint_type tmp = pow(Q / 2);
                    modint_type R = tmp * (*this), t = R * tmp;
                    if (t.val() == 1) return R;
                    modint_type u = t;
                    for (itype::u32 i = 0; i != S - 1; ++i) u *= u;
                    if (u.val() != 1) return Null;
                    const modint_type z = [&]() {
                        if (md % 3 == 2) return modint_type::raw(3);
                        if (auto x = md % 5; x == 2 || x == 3) return modint_type::raw(5);
                        if (auto x = md % 7; x == 3 || x == 5 || x == 6) return modint_type::raw(7);
                        if (auto x = md % 11; x == 2 || x == 6 || x == 7 || x == 8 || x == 10) return modint_type(11);
                        if (auto x = md % 13; x == 2 || x == 5 || x == 6 || x == 7 || x == 8 || x == 11) return modint_type(13);
                        for (const itype::u32 x : { 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 }) {
                            if (modint_type(x).legendre() == -1) return modint_type(x);
                        }
                        modint_type z = 101;
                        while (z.legendre() != -1) z += 2;
                        return z;
                    }();
                    itype::u32 M = S;
                    modint_type c = z.pow(Q);
                    do {
                        modint_type U = t * t;
                        itype::u32 i = 1;
                        while (U.val() != 1) U = U * U, ++i;
                        modint_type b = c;
                        for (itype::u32 j = 0, k = M - i - 1; j < k; ++j) b *= b;
                        M = i, c = b * b, t *= c, R *= b;
                    } while (t.val() != 1);
                    return get_min(R);
                } else {
                    if (legendre() != 1) return Null;
                    modint_type a = 2;
                    while ((a * a - *this).legendre() != -1) ++a;
                    modint_type res1 = modint_type::raw(1), res2, pow1 = a, pow2 = modint_type::raw(1), w = a * a - *this;
                    value_type e = (md + 1) / 2;
                    while (true) {
                        const modint_type tmp2 = pow2 * w;
                        if (e & 1) {
                            const modint_type tmp = res1;
                            res1 = res1 * pow1 + res2 * tmp2;
                            res2 = tmp * pow2 + res2 * pow1;
                        }
                        e >>= 1;
                        if (e == 0) return get_min(res1);
                        const modint_type tmp = pow1;
                        pow1 = pow1 * pow1 + pow2 * tmp2;
                        pow2 *= tmp + tmp;
                    }
                }
            }
        }
    };

    template<itype::u32 mod_> class StaticModint32_impl {
        using value_type = itype::u32;
        using modint_type = StaticModint32_impl;
        value_type val_ = 0;
    protected:
        constexpr StaticModint32_impl() noexcept {}
        constexpr value_type val() const noexcept { return val_; }
        static constexpr value_type mod() noexcept { return mod_; }
        constexpr void assign(itype::u32 x) noexcept { val_ = x % mod_; }
        constexpr void assign(itype::u64 x) noexcept { val_ = x % mod_; }
        constexpr void rawassign(value_type x) noexcept { val_ = x; }
        constexpr void neg() noexcept { val_ = (val_ == 0 ? 0 : mod_ - val_); }
        constexpr void inc() noexcept { val_ = (val_ == mod_ - 1 ? 0 : val_ + 1); }
        constexpr void dec() noexcept { val_ = (val_ == 0 ? mod_ - 1 : val_ - 1); }
        constexpr void add(modint_type x) noexcept {
            if (mod_ - val_ > x.val_) val_ += x.val_;
            else val_ = x.val_ - (mod_ - val_);
        }
        constexpr void sub(modint_type x) noexcept {
            if (val_ >= x.val_) val_ -= x.val_;
            else val_ = mod_ - (x.val_ - val_);
        }
        constexpr void mul(modint_type x) noexcept { val_ = static_cast<itype::u64>(val_) * x.val_ % mod_; }
    };

    template<itype::u64 mod_> class StaticModint64_impl {
        using value_type = itype::u64;
        using modint_type = StaticModint64_impl;
        value_type val_ = 0;
    protected:
        constexpr StaticModint64_impl() noexcept {}
        constexpr value_type val() const noexcept { return val_; }
        static constexpr value_type mod() noexcept { return mod_; }
        constexpr void assign(itype::u32 x) noexcept {
            if constexpr (mod_ < (1ull << 32)) val_ = x % mod_;
            else val_ = x;
        }
        constexpr void assign(itype::u64 x) noexcept { val_ = x % mod_; }
        constexpr void rawassign(value_type x) noexcept { val_ = x; }
        constexpr void neg() noexcept { val_ = (val_ == 0 ? 0 : mod_ - val_); }
        constexpr void inc() noexcept { val_ = (val_ == mod_ - 1 ? 0 : val_ + 1); }
        constexpr void dec() noexcept { val_ = (val_ == 0 ? mod_ - 1 : val_ - 1); }
        constexpr void add(modint_type x) noexcept {
            if (mod_ - val_ > x.val_) val_ += x.val_;
            else val_ = x.val_ - (mod_ - val_);
        }
        constexpr void sub(modint_type x) noexcept {
            if (val_ >= x.val_) val_ -= x.val_;
            else val_ = mod_ - (x.val_ - val_);
        }
        constexpr void mul(modint_type x) noexcept {
            if constexpr (mod_ < (1ull << 63)) {
                constexpr itype::u128 M_ = std::numeric_limits<itype::u128>::max() / mod_ + std::has_single_bit(mod_);
                const value_type a = (((M_ * val_) >> 64) * x.val_) >> 64;
                const value_type b = val_ * x.val_;
                const value_type c = a * mod_;
                const value_type d = b - c;
                const bool e = d < mod_;
                const value_type f = d - mod_;
                val_ = e ? d : f;
            } else {
                constexpr itype::u128 M_ = std::numeric_limits<itype::u128>::max() / mod_ + std::has_single_bit(mod_);
                const value_type a = (((M_ * val_) >> 64) * x.val_) >> 64;
                const itype::u128 b = (itype::u128) val_ * x.val_;
                const itype::u128 c = (itype::u128) a * mod_;
                const itype::u128 d = b - c;
                const bool e = d < mod_;
                const itype::u128 f = d - mod_;
                val_ = e ? d : f;
            }
        }
    };

    template<int id> class DynamicModint32_impl {
        using value_type = itype::u32;
        using modint_type = DynamicModint32_impl;
        static inline value_type mod_ = 0;
        static inline itype::u64 M_ = 0;
        value_type val_ = 0;
    protected:
        DynamicModint32_impl() noexcept {}
        static void set_mod(value_type newmod) noexcept {
            mod_ = newmod;
            M_ = std::numeric_limits<itype::u64>::max() / mod_ + std::has_single_bit(mod_);
        }
        value_type val() const noexcept { return val_; }
        static value_type mod() noexcept { return mod_; }
        void assign(itype::u32 x) noexcept { val_ = x % mod_; }
        void assign(itype::u64 x) noexcept { val_ = x % mod_; }
        void rawassign(value_type x) noexcept { val_ = x; }
        void neg() noexcept { val_ = (val_ == 0 ? 0 : mod_ - val_); }
        void inc() noexcept { val_ = (val_ == mod_ - 1 ? 0 : val_ + 1); }
        void dec() noexcept { val_ = (val_ == 0 ? mod_ - 1 : val_ - 1); }
        void add(modint_type x) noexcept {
            const auto tmp = val_ + x.val_;
            const auto tmp2 = mod_ - val_;
            val_ = tmp2 > x.val_ ? tmp : x.val_ - tmp2;
        }
        void sub(modint_type x) noexcept {
            if (val_ >= x.val_) val_ -= x.val_;
            else val_ = mod_ - (x.val_ - val_);
        }
        void mul(modint_type x) noexcept {
            const itype::u64 a = (itype::u64) val_ * x.val_;
            const itype::u64 b = ((itype::u128) M_ * a) >> 64;
            const itype::u64 c = a - b * mod_;
            val_ = c < mod_ ? c : c - mod_;
            
        }
    };

    template<int id> class DynamicModint64_impl {
        using value_type = itype::u64;
        using modint_type = DynamicModint64_impl;
        static inline value_type mod_ = 0;
        static inline itype::u128 M_ = 0;
        value_type val_ = 0;
    protected:
        DynamicModint64_impl() noexcept {}
        static void set_mod(value_type newmod) noexcept {
            mod_ = newmod;
            M_ = std::numeric_limits<itype::u128>::max() / mod_ + std::has_single_bit(mod_);
        }
        value_type val() const noexcept { return val_; }
        static value_type mod() noexcept { return mod_; }
        void assign(itype::u32 x) noexcept { val_ = x % mod_; }
        void assign(itype::u64 x) noexcept { val_ = x % mod_; }
        void rawassign(value_type x) noexcept { val_ = x; }
        void neg() noexcept { val_ = (val_ == 0 ? 0 : mod_ - val_); }
        void inc() noexcept { val_ = (val_ == mod_ - 1 ? 0 : val_ + 1); }
        void dec() noexcept { val_ = (val_ == 0 ? mod_ - 1 : val_ - 1); }
        void add(modint_type x) noexcept {
            const auto tmp = val_ + x.val_;
            const auto tmp2 = mod_ - val_;
            val_ = tmp2 > x.val_ ? tmp : x.val_ - tmp2;
        }
        void sub(modint_type x) noexcept {
            if (val_ >= x.val_) val_ -= x.val_;
            else val_ = mod_ - (x.val_ - val_);
        }
        void mul(modint_type x) noexcept {
            const value_type a = (((M_ * val_) >> 64) * x.val_) >> 64;
            const value_type b = val_ * x.val_;
            const value_type c = a * mod_;
            const value_type d = b - c;
            const bool e = d < mod_;
            const value_type f = d - mod_;
            val_ = e ? d : f;
        }
    };

}  

template<itype::u32 mod_ = 998244353> using StaticModint32 = internal::ModintImpl<internal::StaticModint32_impl<mod_>>;
template<itype::u64 mod_ = 998244353> using StaticModint64 = internal::ModintImpl<internal::StaticModint64_impl<mod_>>;
template<itype::u64 mod_ = 998244353> using StaticModint = std::conditional_t<(mod_ < (1ull << 32)), StaticModint32<mod_>, StaticModint64<mod_>>;
template<int id = 0> using DynamicModint32 = internal::ModintImpl<internal::DynamicModint32_impl<id>>;
template<int id = 0> using DynamicModint64 = internal::ModintImpl<internal::DynamicModint64_impl<id>>;
template<int id = 0> using DynamicModint = DynamicModint64<id>;

}  

#include <cstdio>

#if false
#include <fcntl.h>
gsh::BasicReader r(open("in.txt", O_RDONLY));
gsh::BasicWriter w(open("out.txt", O_WRONLY | O_TRUNC));
#else
gsh::BasicReader r;
gsh::BasicWriter w;
#endif
int main() {
    try {
        using namespace std;
        using namespace gsh;
        using namespace gsh::itype;
        using namespace gsh::ftype;
        using namespace gsh::ctype;
        u32 N = Parser<u8dig>{}(r).val;
        Arr<u32> A(N);
        for (u32 i = 0; i != N; ++i) A[i] = Parser<u32>{}(r);
        Formatter<u32>{}(w, CountDistinctSubsequences<StaticModint32<>>(A).val());
    } catch (gsh::Exception& e) {
        printf("gsh::Exception was throwed: ");
        puts(e.what());
    }
}

