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
    using f16 = _Float16;
    using f32 = float;
    using f64 = double;
    using f128 = __float128;
    using flong = long double;
}  

namespace ctype {
    using c8 = char;
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
    using f16x16 = __attribute__((vector_size(32))) ftype::f16;
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
    using f16x32 = __attribute__((vector_size(64))) ftype::f16;
    using f32x16 = __attribute__((vector_size(64))) ftype::f32;
    using f64x8 = __attribute__((vector_size(64))) ftype::f64;

}  

template<class T, class U> constexpr T SimdCast(U x) {
    return __builtin_convertvector(x, T);
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
        stream.reload(4);
        return internal::Parseu8(stream);
    }
};
template<> class Parser<itype::i8> {
public:
    template<class Stream> constexpr itype::i8 operator()(Stream& stream) const {
        stream.reload(4);
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
        stream.reload(9);
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
        stream.reload(9);
        return internal::Parseu8dig(stream);
    }
};
template<> class Parser<itype::i8dig> {
public:
    template<class Stream> constexpr itype::i8dig operator()(Stream& stream) const {
        stream.reload(9);
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
        if (eof == buf + Bufsize) [[likely]] {
            itype::u32 rem = eof - cur;
            std::memmove(buf, cur, rem);
            *(eof = buf + rem + read(fd, buf + rem, Bufsize - rem)) = '\0';
            cur = buf;
        } else if (eof == cur) {
            *(eof = buf + read(fd, buf, Bufsize)) = '\0';
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
    itype::i32 fd = 0;
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
        [[maybe_unused]] itype::i32 tmp = write(1, buf, cur - buf);
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
    static constexpr auto size(internal::same_ncvr<R> auto&& r) { return std::ranges::size(std::forward<R>(r)); }
    static constexpr auto ssize(internal::same_ncvr<R> auto&& r) { return std::ranges::ssize(std::forward<R>(r)); }
    static constexpr auto empty(internal::same_ncvr<R> auto&& r) { return std::ranges::empty(std::forward<R>(r)); }

    static constexpr auto begin(internal::same_ncvr<R> auto&& r) { return std::ranges::begin(std::forward<R>(r)); }
    static constexpr auto end(internal::same_ncvr<R> auto&& r) { return std::ranges::end(std::forward<R>(r)); }
    static constexpr auto cbegin(internal::same_ncvr<R> auto&& r) { return std::ranges::cbegin(std::forward<R>(r)); }
    static constexpr auto cend(internal::same_ncvr<R> auto&& r) { return std::ranges::cend(std::forward<R>(r)); }
    static constexpr auto rbegin(internal::same_ncvr<R> auto&& r) { return std::ranges::cbegin(std::forward<R>(r)); }
    static constexpr auto rend(internal::same_ncvr<R> auto&& r) { return std::ranges::cend(std::forward<R>(r)); }
    static constexpr auto crbegin(internal::same_ncvr<R> auto&& r) { return std::ranges::crbegin(std::forward<R>(r)); }
    static constexpr auto crend(internal::same_ncvr<R> auto&& r) { return std::ranges::crend(std::forward<R>(r)); }

    static constexpr auto mbegin(internal::same_ncvr<R> auto&& r) { return std::move_iterator(begin(std::forward<R>(r))); }
    static constexpr auto mend(internal::same_ncvr<R> auto&& r) { return std::move_sentinel(end(std::forward<R>(r))); }
    static constexpr auto mcbegin(internal::same_ncvr<R> auto&& r) { return std::move_iterator(cbegin(std::forward<R>(r))); }
    static constexpr auto mcend(internal::same_ncvr<R> auto&& r) { return std::move_sentinel(cend(std::forward<R>(r))); }
    static constexpr auto mrbegin(internal::same_ncvr<R> auto&& r) { return std::move_iterator(rbegin(std::forward<R>(r))); }
    static constexpr auto mrend(internal::same_ncvr<R> auto&& r) { return std::move_sentinel(rend(std::forward<R>(r))); }
    static constexpr auto mcrbegin(internal::same_ncvr<R> auto&& r) { return std::move_iterator(crbegin(std::forward<R>(r))); }
    static constexpr auto mcrend(internal::same_ncvr<R> auto&& r) { return std::move_sentinel(crend(std::forward<R>(r))); }

    static constexpr auto fbegin(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return begin(r);
        else return mbegin(r);
    }
    static constexpr auto fend(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return end(r);
        else return mend(r);
    }
    static constexpr auto fcbegin(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return cbegin(r);
        else return mcbegin(r);
    }
    static constexpr auto fcend(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return cend(r);
        else return mcend(r);
    }
    static constexpr auto frbegin(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return rbegin(r);
        else return mrbegin(r);
    }
    static constexpr auto frend(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return rend(r);
        else return mrend(r);
    }
    static constexpr auto fcrbegin(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return crbegin(r);
        else return mcrbegin(r);
    }
    static constexpr auto fcrend(internal::same_ncvr<R> auto&& r) {
        if constexpr (!std::ranges::borrowed_range<decltype(r)>) return crend(r);
        else return mcrend(r);
    }

    static constexpr auto data(internal::same_ncvr<R> auto&& r) { return std::ranges::data(std::forward<R>(r)); }
    static constexpr auto cdata(internal::same_ncvr<R> auto&& r) { return std::ranges::cdata(std::forward<R>(r)); }
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
#include <utility>          
  

namespace gsh {

namespace internal {
    template<typename T, typename U> concept less_builtin_ptr_cmp = requires(T&& t, U&& u) {
        { t < u } -> std::same_as<bool>;
    } && std::convertible_to<T, const volatile void*> && std::convertible_to<U, const volatile void*> && (!requires(T&& t, U&& u) { operator<(std::forward<T>(t), std::forward<U>(u)); } && !requires(T&& t, U&& u) { std::forward<T>(t).operator<(std::forward<U>(u)); });
}  
class Less {
public:
    template<class T, class U>
        requires std::totally_ordered_with<T, U>
    constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() < std::declval<U>())) {
        if constexpr (internal::less_builtin_ptr_cmp<T, U>) {
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
        if constexpr (internal::less_builtin_ptr_cmp<U, T>) {
            if (std::is_constant_evaluated()) return u < t;
            auto x = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<T>(t)));
            auto y = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<U>(u)));
            return y < x;
        } else return std::forward<U>(u) < std::forward<T>(t);
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


template<Range R> constexpr Arr<itype::u32> LongestIncreasingSubsequenceIndex(R&& r) {
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

template<RandomAccessRange R> constexpr Arr<itype::u32> EnumerateLongestCommonPrefixLength(R&& r) {
    using traits = RangeTraits<R>;
    const itype::u32 n = traits::size(r);
    Arr<itype::u32> res(n);
    res[0] = n;
    itype::u32 i = 1, j = 0;
    const auto itr = traits::begin(r);
    while (i != n) {
        while (i + j < n && *std::next(itr, j) == *std::next(itr, i + j)) ++j;
        res[i] = j;
        if (j == 0) {
            ++i;
            continue;
        }
        itype::u32 k = 1;
        while (res[k] < j - k) ++k;
        auto* const a = res.data() + i + 1;
        auto* const b = res.data() + 1;
        for (itype::u32 l = 0; l != k - 1; ++l) a[l] = b[l];
        i += k;
        j -= k;
    }
    return res;
}

}  
#include <bit>              
#include <ctime>            
  

namespace gsh {

namespace internal {
    constexpr itype::u64 Splitmix(itype::u64 x) {
        itype::u64 z = (x + 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
}  


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


class RandomDevice {
    Rand64 engine{ internal::Splitmix(static_cast<itype::u64>(std::time(nullptr))) };
public:
    using result_type = itype::u32;
    RandomDevice() {}
    RandomDevice(const RandomDevice&) = delete;
    ~RandomDevice() = default;
    void operator=(const RandomDevice&) = delete;
    constexpr ftype::f64 entropy() const noexcept { return 0.0; }
    static constexpr result_type max() { return 4294967295u; }
    static constexpr result_type min() { return 0; }
    result_type operator()() {
        itype::u64 a = internal::Splitmix(static_cast<itype::u64>(std::time(nullptr)));
        itype::u64 b = internal::Splitmix(static_cast<itype::u64>(std::clock()));
        return static_cast<result_type>(engine() ^ a ^ b);
    }
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


template<class URBG> constexpr itype::u32 Uniform32(URBG& g, itype::u32 max) {
    return (static_cast<itype::u64>(g() & 4294967295u) * max) >> 32;
}

template<class URBG> constexpr itype::u32 Uniform32(URBG& g, itype::u32 min, itype::u32 max) {
    return static_cast<itype::u32>((static_cast<itype::u64>(g() & 4294967295u) * (max - min)) >> 32) + min;
}

template<class URBG> constexpr itype::u64 Uniform64(URBG& g, itype::u64 max) {
    return (static_cast<itype::u128>(g()) * max) >> 64;
}

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

}  

#include <cstdio>

#ifdef EVAL
gsh::MmapReader r;
#else
gsh::BasicReader r;
#endif
gsh::BasicWriter w;
int main() {
    try {
        using namespace std;
        using namespace gsh;
        using namespace gsh::itype;
        using namespace gsh::ftype;
        using namespace gsh::ctype;
        static char S[500016];
        read(0, S, 500016);
        u32 N = (char*) memchr(&S[0], '\n', 500016) - S;
        for (u32 x : EnumerateLongestCommonPrefixLength(Subrange{ S, S + N })) {
            Formatter<u8dig>{}(w, u8dig{x});
            Formatter<c8>{}(w, ' ');
        }
    } catch (gsh::Exception& e) {
        printf("gsh::Exception was throwed: ");
        puts(e.what());
    }
}

