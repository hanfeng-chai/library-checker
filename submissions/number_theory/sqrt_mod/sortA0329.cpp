#pragma GCC optimize("Ofast")

#include <ctime>   
#include <limits>  
#include <cstdio>  

namespace gsh {

class ClockTimer {
    std::clock_t start_time;
public:
    using value_type = std::clock_t;
    ClockTimer() { start_time = std::clock(); }
    void restart() { start_time = std::clock(); }
    value_type elapsed() const { return (std::clock() - start_time) * 1000 / CLOCKS_PER_SEC; }
    void print() const { std::printf("gsh::ClockTimer::print / %llu ms\n", static_cast<unsigned long long>(elapsed())); }
};

}  

#include <bit>              
#include <ctime>            

namespace gsh {

namespace itype {
    using i8 = char;
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
}  

namespace ftype {
    using f32 = float;
    using f64 = double;
    using flong = long double;
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

namespace internal {
    constexpr itype::u64 splitmix(itype::u64 x) {
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
    constexpr explicit Rand64(result_type value) : s0(value), s1(internal::splitmix(value)) {}
    constexpr result_type operator()() {
        itype::u64 t0 = s0, t1 = s1;
        const itype::u64 res = t0 + t1;
        t1 ^= t0;
        s0 = std::rotr(t0, 9) ^ t1 ^ (t1 << 14);
        s1 = std::rotr(t1, 28);
        return res;
    };
    constexpr void discard(unsigned long long z) {
        for (unsigned long long i = 0; i < z; ++i) operator()();
    }
    static constexpr result_type max() { return 18446744073709551615u; }
    static constexpr result_type min() { return 0; }
    constexpr void seed(result_type value = default_seed) { s0 = value, s1 = internal::splitmix(value); }
    friend constexpr bool operator==(Rand64 x, Rand64 y) { return x.s0 == y.s0 && x.s1 == y.s1; }
};


class Rand32 {
    itype::u64 val;
public:
    using result_type = itype::u32;
    static constexpr itype::usize word_size = sizeof(result_type) * 8;
    static constexpr result_type default_seed = 0xcafef00d;
    constexpr Rand32() : Rand32(default_seed) {}
    constexpr explicit Rand32(result_type value) : val(internal::splitmix((itype::u64) value << 32 | value)) {}
    constexpr result_type operator()() {
        itype::u64 x = val;
        const itype::i32 count = x >> 61;
        val = x * 0xcafef00dd15ea5e5;
        x ^= x >> 22;
        return x >> (22 + count);
    };
    constexpr void discard(unsigned long long z) {
        for (unsigned long long i = 0; i < z; ++i) val *= 0xcafef00dd15ea5e5;
    }
    static constexpr result_type max() { return 4294967295u; }
    static constexpr result_type min() { return 0; }
    constexpr void seed(result_type value = default_seed) { val = internal::splitmix((itype::u64) value << 32 | value); }
    friend constexpr bool operator==(Rand32 x, Rand32 y) { return x.val == y.val; }
};


class RandomDevice {
    static inline Rand64 engine{ static_cast<itype::u64>(std::time(nullptr)) };
public:
    using result_type = itype::u32;
    RandomDevice() {}
    RandomDevice(const RandomDevice&) = delete;
    ~RandomDevice() = default;
    void operator=(const RandomDevice&) = delete;
    ftype::f64 entropy() const noexcept { return 0.0; }
    static constexpr result_type max() { return 4294967295u; }
    static constexpr result_type min() { return 0; }
    result_type operator()() {
        itype::u64 a = internal::splitmix(static_cast<itype::u64>(std::time(nullptr)));
        itype::u64 b = internal::splitmix(static_cast<itype::u64>(std::clock()));
        return static_cast<result_type>(engine() ^ a ^ b);
    }
};


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

#include <type_traits>       
#include <bit>               
#include <initializer_list>  
   
#include <type_traits>        
#include <limits>             
#include <bit>                
#include <optional>           
    

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

    template<class T> class ModintTraits : public T {
        using base_type = T;
    public:
        using value_type = std::decay_t<decltype(base_type::mod())>;
        using modint_type = ModintTraits;
        constexpr static bool is_staticmod = !requires { base_type::set_mod(0); };
        constexpr ModintTraits() noexcept : T() {}
        template<class U> constexpr ModintTraits(U x) noexcept { operator=(x); }
        constexpr explicit operator value_type() const noexcept { return val(); }
        constexpr static void set_mod(value_type x) {
            static_assert(!is_staticmod, "ModintTraits::set_mod / Mod must be dynamic.");
            if (x <= 1) throw Exception("ModintTraits::set_mod / Mod must be at least 2.");
            if (x == mod()) return;
            base_type::set_mod(x);
        }
        constexpr value_type val() const noexcept { return base_type::val(); }
        constexpr static value_type mod() noexcept { return base_type::mod(); }
        template<class U> constexpr modint_type& operator=(U x) noexcept {
            static_assert(std::is_integral_v<U>, "ModintTraits::operator= / Only integer types can be assigned.");
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
        constexpr modint_type inv() const {
            value_type a = 1, b = 0, x = val(), y = mod();
            if (x == 0) throw Exception("ModintTraits::inv / Zero division is not possible.");
            while (true) {
                if (x <= 1) {
                    if (x == 0) [[unlikely]]
                        break;
                    else return modint_type::raw(a);
                }
                b += a * (y / x);
                y %= x;
                if (y <= 1) {
                    if (y == 0) [[unlikely]]
                        break;
                    else return modint_type::raw(mod() - b);
                }
                a += b * (x / y);
                x %= y;
            }
            throw Exception("ModintTraits::inv / Cannot calculate inverse element.");
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
            operator*=(x.inv());
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
        constexpr int jacobi() const noexcept {
            value_type a = val(), n = mod();
            if (a == 1) return 1;
            if (calc_gcd(a, n) != 1) return 0;
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
            if (n != 1) return 0;
            return res;
        }
        constexpr std::optional<modint_type> sqrt() const noexcept {
            const value_type vl = val(), md = mod();
            if (vl <= 1) return *this;
            auto get_min = [](modint_type x) {
                return x.val() > (mod() >> 1) ? -x : x;
            };
            if ((md & 0b11) == 3) {
                modint_type res = pow((md + 1) >> 2);
                if (res * res != *this) return std::nullopt;
                else return get_min(res);
            } else if ((md & 0b111) == 5) {
                modint_type res = pow((md + 3) >> 3);
                if constexpr (is_staticmod) {
                    constexpr modint_type p = modint_type::raw(2).pow((md - 1) >> 2);
                    res *= p;
                } else if (res * res != *this) res *= modint_type::raw(2).pow((md - 1) >> 2);
                if (res * res != *this) return std::nullopt;
                else return get_min(res);
            } else {
                if (legendre() != 1) return std::nullopt;
                value_type Q = md - 1;
                itype::u32 S = 0;
                while ((Q & 1) == 0) Q >>= 1, ++S;
                if (std::countr_zero(md - 1) < 6) {
                    modint_type z = 3;
                    while (z.legendre() != -1) ++z;
                    modint_type t = pow(Q), R = pow((Q + 1) / 2);
                    if (t.val() == 1) return R;
                    itype::u32 M = S;
                    modint_type c = z.pow(Q);
                    do {
                        modint_type U = t * t;
                        itype::u32 i = 1;
                        while (U.val() != 1) U = U * U, ++i;
                        modint_type b = c;
                        for (itype::u32 j = 0; j < (M - i - 1); ++j) b *= b;
                        M = i, c = b * b, t *= c, R *= b;
                    } while (t.val() != 1);
                    return get_min(R);
                } else {
                    modint_type a = 2;
                    while ((a * a - *this).legendre() != -1) ++a;
                    modint_type res1 = modint_type::raw(1), res2, pow1 = a, pow2 = modint_type::raw(1), w = a * a - *this;
                    value_type e = (md + 1) / 2;
                    while (true) {
                        if (e & 1) {
                            modint_type tmp = res1;
                            res1 = res1 * pow1 + res2 * pow2 * w;
                            res2 = tmp * pow2 + res2 * pow1;
                        }
                        e >>= 1;
                        if (e == 0) return get_min(res1);
                        modint_type tmp = pow1;
                        pow1 = pow1 * pow1 + pow2 * pow2 * w;
                        pow2 *= modint_type::raw(2) * tmp;
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
        constexpr void mul(modint_type x) noexcept { val_ = static_cast<itype::u128>(val_) * x.val_ % mod_; }
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
            const value_type a = (((M_ * val_) >> 32) * x.val_) >> 32;
            const value_type b = val_ * x.val_;
            const value_type c = a * mod_;
            const value_type d = b - c;
            const bool e = d < mod_;
            const value_type f = d - mod_;
            val_ = e ? d : f;
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

template<itype::u32 mod_ = 998244353> using StaticModint32 = internal::ModintTraits<internal::StaticModint32_impl<mod_>>;
template<itype::u64 mod_ = 998244353> using StaticModint64 = internal::ModintTraits<internal::StaticModint64_impl<mod_>>;
template<itype::u64 mod_ = 998244353> using StaticModint = std::conditional_t<(mod_ < (1ull << 32)), StaticModint32<mod_>, StaticModint64<mod_>>;
template<int id = 0> using DynamicModint32 = internal::ModintTraits<internal::DynamicModint32_impl<id>>;
template<int id = 0> using DynamicModint64 = internal::ModintTraits<internal::DynamicModint64_impl<id>>;
template<int id = 0> using DynamicModint = DynamicModint64<id>;

}  
    

namespace gsh {


constexpr itype::u32 IntSqrt(const itype::u32 x) {
    if (x < 2) return x;
    const itype::u32 log2x = std::bit_width(x) - 1;
    const itype::u32 log2y = log2x / 2u;
    itype::u32 y = 1 << log2y;
    itype::u32 y_squared = 1 << (2u * log2y);
    itype::i32 sqr_diff = x - y_squared;
    y += (sqr_diff / 3u) >> log2y;
    y_squared = y * y;
    sqr_diff = x - y_squared;
    y += sqr_diff / (2 * y);
    y_squared = y * y;
    sqr_diff = x - y_squared;
    if (sqr_diff >= 0) return y;
    y -= (-sqr_diff / (2 * y)) + 1;
    y_squared = y * y;
    sqr_diff = x - y_squared;
    y -= (sqr_diff < 0);
    return y;
}




template<class T, class U> constexpr auto GCD(T x, U y) {
    static_assert(!std::is_same_v<T, bool> && !std::is_same_v<U, bool> && std::is_integral_v<T> && std::is_integral_v<U>, "GCD / The input must be an integral type.");
    if constexpr (std::is_same_v<T, U>) {
        if constexpr (std::is_unsigned_v<T>) {
            return internal::calc_gcd(x, y);
        } else {
            return static_cast<T>(GCD<std::make_unsigned_t<T>, std::make_unsigned<T>>((x < 0 ? -x : x), (y < 0 ? -y : y)));
        }
    } else {
        return GCD<std::common_type_t<T, U>, std::common_type_t<T, U>>(x, y);
    }
}

template<class T, class... Args> auto GCD(T x, Args... y) {
    return GCD(x, GCD(y...));
}

template<class T, class U> auto LCM(T x, U y) {
    return static_cast<std::common_type_t<T, U>>(x < 0 ? -x : x) / GCD(x, y) * static_cast<std::common_type_t<T, U>>(y < 0 ? -y : y);
}

template<class T, class... Args> auto LCM(T x, Args... y) {
    return LCM(x, LCM(y...));
}

namespace internal {

    bool isPrime32(const itype::u32 x) {
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0 || x % 13 == 0 || x % 17 == 0 || x % 19 == 0 || x % 23 == 0 || x % 29 == 0 || x % 31 == 0 || x % 37 == 0 || x % 41 == 0 || x % 43 == 0) return x <= 43 && (x == 2 || x == 3 || x == 5 || x == 7 || x == 11 || x == 13 || x == 17 || x == 19 || x == 23 || x == 29 || x == 31 || x == 37 || x == 41 || x == 43);
        if (x < 47 * 47) return (x > 1);
        const static itype::u16 bases[] = { 1216, 1836,  8885,  4564, 10978, 5228, 15613, 13941, 1553, 173,   3615, 3144, 10065, 9259,  233,  2362, 6244,  6431, 10863, 5920, 6408, 6841, 22124, 2290,  45597, 6935,  4835, 7652, 1051, 445,  5807, 842,  1534, 22140, 1282, 1733, 347,   6311,  14081, 11157, 186,  703,  9862,  15490, 1720, 17816, 10433, 49185, 2535, 9158,  2143,  2840,  664,  29074, 24924, 1035, 41482, 1065,  10189, 8417,  130,  4551,  5159,  48886,
                                            786,  1938,  1013,  2139, 7171,  2143, 16873, 188,   5555, 42007, 1045, 3891, 2853,  23642, 148,  3585, 3027,  280,  3101,  9918, 6452, 2716, 855,   990,   1925,  13557, 1063, 6916, 4965, 4380, 587,  3214, 1808, 1036,  6356, 8191, 6783,  14424, 6929,  1002,  840,  422,  44215, 7753,  5799, 3415,  231,   2013,  8895, 2081,  883,   3855,  5577, 876,   3574,  1925, 1192,  865,   7376,  12254, 5952, 2516,  20463, 186,
                                            5411, 35353, 50898, 1084, 2127,  4305, 115,   7821,  1265, 16169, 1705, 1857, 24938, 220,   3650, 1057, 482,   1690, 2718,  4309, 7496, 1515, 7972,  3763,  10954, 2817,  3430, 1423, 714,  6734, 328,  2581, 2580, 10047, 2797, 155,  5951,  3817,  54850, 2173,  1318, 246,  1807,  2958,  2697, 337,   4871,  2439,  736,  37112, 1226,  527,   7531, 5418,  7242,  2421, 16135, 7015,  8432,  2605,  5638, 5161,  11515, 14949,
                                            748,  5003,  9048,  4679, 1915,  7652, 9657,  660,   3054, 15469, 2910, 775,  14106, 1749,  136,  2673, 61814, 5633, 1244,  2567, 4989, 1637, 1273,  11423, 7974,  7509,  6061, 531,  6608, 1088, 1627, 160,  6416, 11350, 921,  306,  18117, 1238,  463,   1722,  996,  3866, 6576,  6055,  130,  24080, 7331,  3922,  8632, 2706,  24108, 32374, 4237, 15302, 287,   2296, 1220,  20922, 3350,  2089,  562,  11745, 163,   11951 };
        using mint = DynamicModint32<-1>;
        mint::set_mod(x);
        const itype::u32 h = x * 0xad625b89;
        itype::u32 d = x - 1;
        mint cur = bases[h >> 24];
        itype::i32 s = std::countr_zero(d);
        d >>= s;
        cur = cur.pow(d);
        if (cur.val() == 1) return true;
        while (--s) {
            if (cur.val() == x - 1) return true;
            cur *= cur;
        }
        return cur.val() == x - 1;
    }

    bool isPrime64(const itype::u64 x) {
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0 || x % 13 == 0 || x % 17 == 0 || x % 19 == 0 || x % 23 == 0 || x % 29 == 0 || x % 31 == 0 || x % 37 == 0 || x % 41 == 0 || x % 43 == 0) return false;
        using mint = DynamicModint64<-1>;
        mint::set_mod(x);
        itype::u64 d = x - 1;
        const itype::i32 s = std::countr_zero(d);
        d >>= s;
        auto test = [&](itype::u64 a) -> bool {
            mint cur = mint(a).pow(d);
            if (cur.val() <= 1) return true;
            itype::i32 i = s;
            while (--i) {
                if (cur.val() == x - 1) return true;
                cur *= cur;
            }
            return cur.val() == x - 1;
        };
        if (x < 585226005592931977ull) {
            if (x < 7999252175582851ull) {
                if (x < 350269456337ull) return test(4230279247111683200ull) && test(14694767155120705706ull) && test(16641139526367750375ull);
                else if (x < 55245642489451ull) return test(2ull) && test(141889084524735ull) && test(1199124725622454117ull) && test(11096072698276303650ull);
                else return test(2ull) && test(4130806001517ull) && test(149795463772692060ull) && test(186635894390467037ull) && test(3967304179347715805ull);
            } else return test(2ull) && test(123635709730000ull) && test(9233062284813009ull) && test(43835965440333360ull) && test(761179012939631437ull) && test(1263739024124850375ull);
        } else return test(2ull) && test(325ull) && test(9375ull) && test(28178ull) && test(450775ull) && test(9780504ull) && test(1795265022ull);
    }

}  


bool isPrime(const itype::u64 x) {
    if (x < 2147483648) return internal::isPrime32(x);
    else return internal::isPrime64(x);
}

}  

#include <iostream>
#include <cassert>
#include <numeric>

using namespace gsh::itype;
using namespace gsh::ftype;
#include <unistd.h>
#include <bits/stdc++.h>
#include <immintrin.h>

class FastIstream : public std::ios_base {
    constexpr static int buffersize = (1 << 18) - 1;
    char buffer[buffersize + 1];
    char* cur = buffer;
    char* eof = buffer;
    inline void reload(ptrdiff_t w) {
        if (eof - w < cur) [[unlikely]] {
            if (eof == buffer + buffersize) [[likely]] {
                ptrdiff_t rem = eof - cur;
                std::memcpy(buffer, cur, rem);
                *(eof = buffer + rem + read(0, buffer + rem, buffersize - rem)) = '\0';
                cur = buffer;
            } else if (eof <= cur) {
                *(eof = buffer + read(0, buffer, buffersize)) = '\0';
                cur = buffer;
            }
        }
    }
public:
    FastIstream& operator>>(bool& n) {
        reload(2);
        n = *cur == '1';
        cur += 2;
        return *this;
    }
    FastIstream& operator>>(short& n) {
        reload(8);
        short neg = (*cur == '-') * -2 + 1;
        cur += neg == -1;
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u;
        int clz = std::countl_zero((tmp & 0x1010101010101010u) & (-(tmp & 0x1010101010101010u))) + 5;
        cur += (72 - clz) >> 3;
        tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
        tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
        n = (short) ((tmp * 0x271000000001ull) >> 32) * neg;
        return *this;
    }
    FastIstream& operator>>(unsigned short& n) {
        reload(8);
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u;
        int clz = std::countl_zero((tmp & 0x1010101010101010u) & (-(tmp & 0x1010101010101010u))) + 5;
        cur += (72 - clz) >> 3;
        tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
        tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
        n = (unsigned short) ((tmp * 0x271000000001ull) >> 32);
        return *this;
    }
    FastIstream& operator>>(unsigned int& n) {
        reload(16);
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u, tmp2 = tmp & 0x1010101010101010u;
        if (tmp2) {
            int clz = std::countl_zero(tmp2 & -tmp2) + 5;
            cur += (72 - clz) >> 3;
            tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (unsigned) ((tmp * 0x271000000001ull) >> 32);
        } else {
            cur += 8;
            tmp = (tmp * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (unsigned) ((tmp * 0x271000000001ull) >> 32);
            if (char c = *(cur++); c >= '0') {
                n = 10 * n + (c - '0');
                if ((c = *(cur++)) >= '0') n = 10 * n + (c - '0'), ++cur;
            }
        }
        return *this;
    }
    FastIstream& operator>>(int& n) {
        reload(16);
        int neg = (*cur == '-') * -2 + 1;
        cur += neg == -1;
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u, tmp2 = tmp & 0x1010101010101010u;
        if (tmp2) {
            int clz = std::countl_zero(tmp2 & -tmp2) + 5;
            cur += (72 - clz) >> 3;
            tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (int) ((tmp * 0x271000000001ull) >> 32);
        } else {
            cur += 8;
            tmp = (tmp * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (int) ((tmp * 0x271000000001ull) >> 32);
            if (char c = *(cur++); c >= '0') {
                n = 10 * n + (c - '0');
                if ((c = *(cur++)) >= '0') n = 10 * n + (c - '0'), ++cur;
            }
        }
        n *= neg;
        return *this;
    }
    FastIstream& operator>>(unsigned long long& n) {
        reload(32);
#ifndef __AVX512VL__
        n = 0;
        while (*cur >= '0') n = 10 * n + (*(cur++) - '0');
        ++cur;
#else
        unsigned long long tmp[3], tmp2[3];
        std::memcpy(tmp, cur, 24);
        int width;
        if ((tmp2[0] = (tmp[0] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
            width = std::countr_zero(tmp2[0]) - 4;
            n = ((((((tmp[0] << (64 - width)) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull) * 0x640001ull) >> 16 & 0x0000ffff0000ffffull) * 0x271000000001ull) >> 32;
            cur += (width >> 3) + 1;
        } else {
            __m256i tmp3;
            if ((tmp2[1] = (tmp[1] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
                width = 60 + std::countr_zero(tmp2[1]);
                if (width == 64) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, 0, tmp[0]);
                else tmp3 = _mm256_setr_epi64x(0, 0, tmp[0] << (128 - width), tmp[1] << (128 - width) | tmp[0] >> (width - 64));
            } else {
                width = 124 + std::countr_zero((tmp[2] ^= 0x3030303030303030) & 0x1010101010101010);
                if (width == 128) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, tmp[0], tmp[1]);
                else tmp3 = _mm256_setr_epi64x(0, tmp[0] << (192 - width), tmp[1] << (192 - width) | tmp[0] >> (width - 128), tmp[2] << (192 - width) | tmp[1] >> (width - 128));
            }
            cur += (width >> 3) + 1;
            alignas(32) unsigned long long res[4];
            _mm256_store_epi64(res, _mm256_srli_epi64(_mm256_mullo_epi64(_mm256_srli_epi32(_mm256_mullo_epi32(_mm256_srli_epi16(_mm256_mullo_epi16(_mm256_and_si256(tmp3, _mm256_set1_epi8(0x0f)), _mm256_set1_epi16(0xa01)), 8), _mm256_set1_epi32(0x640001)), 16), _mm256_set1_epi64x(0x271000000001)), 32));
            n = res[1] * 10000000000000000 + res[2] * 100000000 + res[3];
        }
#endif
        return *this;
    }
    FastIstream& operator>>(long long& n) {
        reload(32);
        long long neg = (*cur == '-') * -2 + 1;
        cur += neg == -1;
#ifndef __AVX512VL__
        n = 0;
        while (*cur >= '0') n = 10 * n + (*(cur++) - '0');
        ++cur;
        n *= neg;
#else
        unsigned long long tmp[3], tmp2[3];
        std::memcpy(tmp, cur, 24);
        int width;
        if ((tmp2[0] = (tmp[0] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
            width = std::countr_zero(tmp2[0]) - 4;
            n = neg * (((((((tmp[0] << (64 - width)) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull) * 0x640001ull) >> 16 & 0x0000ffff0000ffffull) * 0x271000000001ull) >> 32);
            cur += (width >> 3) + 1;
        } else {
            __m256i tmp3;
            if ((tmp2[1] = (tmp[1] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
                width = 60 + std::countr_zero(tmp2[1]);
                if (width == 64) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, 0, tmp[0]);
                else tmp3 = _mm256_setr_epi64x(0, 0, tmp[0] << (128 - width), tmp[1] << (128 - width) | tmp[0] >> (width - 64));
            } else {
                width = 124 + std::countr_zero((tmp[2] ^= 0x3030303030303030) & 0x1010101010101010);
                if (width == 128) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, tmp[0], tmp[1]);
                else tmp3 = _mm256_setr_epi64x(0, tmp[0] << (192 - width), tmp[1] << (192 - width) | tmp[0] >> (width - 128), tmp[2] << (192 - width) | tmp[1] >> (width - 128));
            }
            cur += (width >> 3) + 1;
            alignas(32) long long res[4];
            _mm256_store_epi64(res, _mm256_srli_epi64(_mm256_mullo_epi64(_mm256_srli_epi32(_mm256_mullo_epi32(_mm256_srli_epi16(_mm256_mullo_epi16(_mm256_and_si256(tmp3, _mm256_set1_epi8(0x0f)), _mm256_set1_epi16(0xa01)), 8), _mm256_set1_epi32(0x640001)), 16), _mm256_set1_epi64x(0x271000000001)), 32));
            n = neg * (res[1] * 10000000000000000 + res[2] * 100000000 + res[3]);
        }
#endif
        return *this;
    }
    FastIstream& operator>>(long& n) {
        long long x;
        operator>>(x);
        n = x;
        return *this;
    }
    FastIstream& operator>>(unsigned long& n) {
        unsigned long long x;
        operator>>(x);
        n = x;
        return *this;
    }
    friend FastIstream& operator>>(FastIstream& is, char& c) {
        is.reload(2);
        c = *is.cur;
        is.cur += 2;
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, unsigned char& c) {
        is.reload(2);
        c = *is.cur;
        is.cur += 2;
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, signed char& c) {
        is.reload(2);
        c = *is.cur;
        is.cur += 2;
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, char* s) {
        while (true) {
            while (*is.cur > ' ' && is.cur != is.eof) *(s++) = *is.cur, ++is.cur;
            if (is.cur == is.eof) is.reload(is.buffersize);
            else break;
        }
        ++is.cur;
        *s = '\0';
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, std::string& s) {
        s.clear();
        while (true) {
            char* st = is.cur;
            while (*is.cur > ' ' && is.cur != is.eof) ++is.cur;
            s += std::string_view(st, is.cur - st);
            if (is.cur == is.eof) is.reload(is.buffersize);
            else break;
        }
        ++is.cur;
        return is;
    }
    FastIstream& operator>>(float& f) {
        std::string s;
        (*this) >> s;
        std::from_chars(s.c_str(), s.c_str() + s.length(), f);
        return *this;
    }
    FastIstream& operator>>(double& f) {
        std::string s;
        (*this) >> s;
        std::from_chars(s.c_str(), s.c_str() + s.length(), f);
        return *this;
    }
    FastIstream& operator>>(long double& f) {
        std::string s;
        (*this) >> s;
        std::from_chars(s.c_str(), s.c_str() + s.length(), f);
        return *this;
    }
    template<std::ranges::range T> friend FastIstream& operator>>(FastIstream& is, T& x) {
        for (auto& v : x) is >> v;
        return is;
    }
    char getc() {
        reload(1);
        return *(cur++);
    }
    void seek(int n) {
        reload(n);
        cur += n;
    }
} fin;
class FastOstream : public std::ios_base {
    constexpr static int buffersize = 1 << 18;
    char buffer[buffersize];
    char* cur = buffer;
    inline void reload(ptrdiff_t w) {
        if (buffer + buffersize - w < cur) [[unlikely]] {
            [[maybe_unused]] int r = write(1, buffer, cur - buffer);
            cur = buffer;
        }
    }
    constexpr static std::array<unsigned, 10000> strtable = []() {
        std::array<unsigned, 10000> res;
        for (unsigned i = 0; i < 10000; ++i) {
            unsigned tmp[4];
            unsigned n = i;
            tmp[3] = (n % 10 + '0') << 24, n /= 10;
            tmp[2] = (n % 10 + '0') << 16, n /= 10;
            tmp[1] = (n % 10 + '0') << 8, n /= 10;
            tmp[0] = n % 10 + '0';
            res[i] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        }
        return res;
    }();
    constexpr static std::array<unsigned, 10000> strtable2 = []() {
        std::array<unsigned, 10000> res;
        for (unsigned i = 0; i < 10000; ++i) {
            unsigned tmp[4];
            unsigned n = i;
            if (i < 10) n *= 1000;
            else if (i < 100) n *= 100;
            else if (i < 1000) n *= 10;
            tmp[3] = (n % 10 + '0') << 24, n /= 10;
            tmp[2] = (n % 10 + '0') << 16, n /= 10;
            tmp[1] = (n % 10 + '0') << 8, n /= 10;
            tmp[0] = n % 10 + '0';
            res[i] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        }
        return res;
    }();
    template<class T> void putfloat(T f) {
        bool fixed = flags() & std::ios_base::fixed;
        bool scientific = flags() & std::ios_base::scientific;
        bool uppercase = flags() & std::ios_base::uppercase;
        if (fixed && scientific && (flags() & std::ios_base::showbase)) {
            std::memcpy(cur, (uppercase ? "0X" : "0x"), 2);
            cur += 2;
        }
        std::chars_format fmt = (fixed ? (scientific ? std::chars_format::hex : std::chars_format::fixed) : (scientific ? std::chars_format::scientific : std::chars_format::general));
        auto conv = [&]() {
            return std::to_chars(cur, buffer + buffersize, f, fmt, precision());
        };
        auto [ptr, ec] = conv();
        char* p;
        if (ec == std::errc::value_too_large) {
            reload(buffersize);
            p = cur;
            cur = conv().ptr;
        } else p = cur, cur = ptr;
        if (uppercase) {
            while (p != cur) {
                if (*p > '9') *p -= ('a' - 'A');
                ++p;
            }
        }
    }
public:
    FastOstream() {
        precision(6);
        setf(std::ios_base::showbase);
    }
    ~FastOstream() { reload(buffersize); }
    FastOstream& flush() {
        reload(buffersize);
        return *this;
    }
    char widen(char c) const { return c; }
    FastOstream& put(char c) {
        reload(1);
        *(cur++) = c;
        return *this;
    }
    FastOstream& operator<<(std::basic_ostream<FastOstream, void>& (*pf)(std::basic_ostream<FastOstream, void>&) );
    FastOstream& operator<<(std::basic_ios<FastOstream, void>& (*pf)(std::basic_ios<FastOstream, void>&) );
    FastOstream& operator<<(std::ios_base& (*pf)(std::ios_base&) ) {
        pf(*this);
        return *this;
    }
    FastOstream& operator<<(bool n) {
        if (ios_base::flags() & std::ios_base::boolalpha) {
            if (n) {
                reload(4);
                std::memcpy(cur, "true", 4);
                cur += 4;
            } else {
                reload(5);
                std::memcpy(cur, "false", 5);
                cur += 5;
            }
        } else {
            reload(1);
            *(cur++) = '0' + n;
        }
        return *this;
    }
    FastOstream& operator<<(unsigned short n) {
        reload(5);
        if (n >= 10000) {
            *(cur++) = '0' + n / 10000, n %= 10000;
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 1000) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 100) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n * 10];
            cur += 3;
        } else if (n >= 10) {
            *(cur++) = '0' + n / 10;
            *(cur++) = '0' + n % 10;
        } else *(cur++) = '0' + n;
        return *this;
    }
    FastOstream& operator<<(short n) {
        reload(6);
        if (n < 0) *(cur++) = '-', n = -n;
        if (n >= 10000) {
            *(cur++) = '0' + n / 10000, n %= 10000;
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 1000) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 100) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n * 10];
            cur += 3;
        } else if (n >= 10) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n * 100];
            cur += 2;
        } else *(cur++) = '0' + n;
        return *this;
    }
    FastOstream& operator<<(unsigned n) {
        reload(10);
        unsigned long long buf = 0;
        char d = 0;
        if (n >= 100000000) {
            d = 8;
            buf = static_cast<unsigned long long>(strtable[n % 10000]) << 32 | strtable[(n / 10000) % 10000];
            n /= 100000000;
        } else if (n >= 10000) {
            d = 4;
            buf = strtable[n % 10000];
            n /= 10000;
        }
        *reinterpret_cast<unsigned*>(cur) = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        *reinterpret_cast<unsigned long long*>(cur) = buf;
        cur += d;
        return *this;
    }
    FastOstream& operator<<(int n) {
        reload(11);
        if (n < 0) *(cur++) = '-', n = -n;
        unsigned long long buf = 0;
        char d = 0;
        if (n >= 100000000) {
            d = 8;
            buf = static_cast<unsigned long long>(strtable[n % 10000]) << 32 | strtable[(n / 10000) % 10000];
            n /= 100000000;
        } else if (n >= 10000) {
            d = 4;
            buf = strtable[n % 10000];
            n /= 10000;
        }
        *reinterpret_cast<unsigned*>(cur) = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        *reinterpret_cast<unsigned long long*>(cur) = buf;
        cur += d;
        return *this;
    }
    FastOstream& operator<<(unsigned long long n) {
        reload(20);
        static unsigned buf[4];
        int d = 0;
        if (n >= 10000000000000000) {
            d = 16;
            buf[3] = strtable[n % 10000], n /= 10000;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 1000000000000) {
            d = 12;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 100000000) {
            d = 8;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 10000) {
            d = 4;
            buf[0] = strtable[n % 10000], n /= 10000;
        }
        *(unsigned*) cur = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        std::memcpy(cur, buf, d);
        cur += d;
        return *this;
    }
    FastOstream& operator<<(long long n) {
        reload(21);
        if (n < 0) *(cur++) = '-', n = -n;
        static unsigned buf[4];
        char d = 0;
        if (n >= 10000000000000000) {
            d = 16;
            buf[3] = strtable[n % 10000], n /= 10000;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 1000000000000) {
            d = 12;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 100000000) {
            d = 8;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 10000) {
            d = 4;
            buf[0] = strtable[n % 10000], n /= 10000;
        }
        *(unsigned*) cur = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        std::memcpy(cur, buf, d);
        cur += d;
        return *this;
    }
    FastOstream& operator<<(long n) { return operator<<(static_cast<long long>(n)); }
    FastOstream& operator<<(unsigned long n) { return operator<<(static_cast<unsigned long long>(n)); }
    FastOstream& operator<<(float f) {
        reload(16);
        putfloat(f);
        return *this;
    }
    FastOstream& operator<<(double f) {
        reload(32);
        putfloat(f);
        return *this;
    }
    FastOstream& operator<<(long double f) {
        reload(64);
        putfloat(f);
        return *this;
    }
    FastOstream& operator<<(const void* p) {
        reload(18);
        if (flags() & std::ios_base::showbase) {
            *cur = '0';
            *(cur + 1) = flags() & std::ios_base::uppercase ? 'X' : 'x';
            cur += 2;
        }
        cur = std::to_chars(cur, buffer + buffersize, reinterpret_cast<unsigned long long>(p), 16).ptr;
        return *this;
    }
    FastOstream& operator<<(std::nullptr_t) {
        reload(7);
        std::memcpy(cur, "nullptr", 7);
        cur += 7;
        return *this;
    }
    friend FastOstream& operator<<(FastOstream& os, char c) {
        os.reload(1);
        *(os.cur++) = c;
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, signed char c) {
        os.reload(1);
        *(os.cur++) = c;
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, unsigned char c) {
        os.reload(1);
        *(os.cur++) = c;
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, const char* s) {
        size_t n = std::strlen(s);
        if (n >= os.buffersize) {
            os.reload(buffersize);
            write(1, s, n);
        } else {
            os.reload(n);
            std::memcpy(os.cur, s, n);
            os.cur += n;
        }
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, const std::string& s) {
        size_t n = s.length();
        if (n >= os.buffersize) {
            os.reload(buffersize);
            write(1, s.data(), n);
        } else {
            os.reload(n);
            std::memcpy(os.cur, s.data(), n);
            os.cur += n;
        }
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, std::string_view s) {
        size_t n = s.length();
        if (n >= os.buffersize) {
            os.reload(buffersize);
            write(1, s.data(), n);
        } else {
            os.reload(n);
            std::memcpy(os.cur, s.data(), n);
            os.cur += n;
        }
        return os;
    }
    template<std::ranges::range T> friend FastOstream& operator<<(FastOstream& os, const T& v) {
        size_t n = std::distance(std::ranges::begin(v), std::ranges::end(v)), cnt = 0;
        for (const auto& x : v) {
            os << x;
            if (++cnt != n) os << ' ';
        }
        return os;
    }
#if __GNUC__ >= 6
    friend FastOstream& operator<<(FastOstream& os, std::_Setprecision prec) {
        os.precision(prec._M_n);
        return os;
    }
#endif
} fout;
namespace std {
template<> class basic_ios<FastOstream, void> {
protected:
    FastOstream& ref;
public:
    basic_ios(FastOstream& r) : ref(r) {}
    char widen(char c) { return ref.widen(c); }
};
template<> class basic_ostream<FastOstream, void> : public basic_ios<FastOstream, void> {
public:
    basic_ostream(FastOstream& r) : basic_ios(r) {}
    basic_ostream& put(char c) {
        basic_ios::ref.put(c);
        return *this;
    }
    basic_ostream& flush() {
        basic_ios::ref.flush();
        return *this;
    }
};
}  
FastOstream& FastOstream::operator<<(std::basic_ostream<FastOstream, void>& (*pf)(std::basic_ostream<FastOstream, void>&) ) {
    std::basic_ostream<FastOstream, void> tmp(*this);
    pf(tmp);
    return *this;
}
FastOstream& FastOstream::operator<<(std::basic_ios<FastOstream, void>& (*pf)(std::basic_ios<FastOstream, void>&) ) {
    std::basic_ios<FastOstream, void> tmp(*this);
    pf(tmp);
    return *this;
}
int main() {
    u32 T;
    fin >> T;
    for (u32 i = 0; i != T; ++i) {
        u32 X, P;
        fin >> X >> P;
        if (P == 998244353) {
            using mint = gsh::StaticModint32<>;
            auto op = mint::raw(X).sqrt();
            if (op) {
                fout << *op << '\n';
            } else {
                fout << "-1\n";
            }
        } else {
            using mint = gsh::DynamicModint32<>;
            mint::set_mod(P);
            auto op = mint::raw(X).sqrt();
            if (op) {
                fout << *op << '\n';
            } else {
                fout << "-1\n";
            }
        }
    }
}

