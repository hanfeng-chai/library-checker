
#pragma GCC optimize("fast-math")

#include <bits/stdc++.h>
#if __has_include("dbg.h")
#  include "dbg.h"
#else
#  define dbg(...) do {} while (0)
#endif

#define cp_lib_4th(_1, _2, _3, x, ...)  x
#define cp_lib_rep(i, l, r)             for (int i = (l); (i) < (r); ++(i))
#define cp_lib_rep0(i, r)               cp_lib_rep(i, 0, r)
#define rep(...)                        cp_lib_4th(__VA_ARGS__, cp_lib_rep, cp_lib_rep0, _)(__VA_ARGS__)
#define cp_lib_repr(i, r, l, ...)       for (int i = (r); (i) >= (l); --(i))
#define repr(...)                       cp_lib_repr(__VA_ARGS__, 0)
#define all(a)                          ::begin(a),::end(a)
#define trav(a, b)                      for (auto&& a : (b))

using namespace std;
using ll = long long;
using ld = long double;
[[maybe_unused]] static constexpr int INF = int(1e9 + 5);
[[maybe_unused]] static constexpr ll INFL = ll(INF) * INF;
template <class C> int sz(const C& c) { return int(::size(c)); }



#if __cpp_lib_remove_cvref < 201711L
template <class T> using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;
#endif
#if __cplusplus < 202002L
struct identity { template <class T> constexpr T&& operator()(T&& t) const noexcept { return forward<T>(t); }; };
template <class T> using iter_value_t = typename iterator_traits<remove_cvref_t<T>>::value_type;
#endif

namespace cp_lib_type_meta {
    template <class T, class = void> constexpr bool is_tuple_like = false;
    template <class T> constexpr bool is_tuple_like<T, void_t<tuple_element_t<0, T>>> = true;
}


namespace cp_lib_modint { struct ModIntTag {}; }
template <class T> constexpr bool IsModInt = is_base_of_v<cp_lib_modint::ModIntTag, T>;
#include <unistd.h>

namespace cp_lib_io {
    constexpr int BUF_SIZE = 1 << 20;
    constexpr array<array<char, 4>, 10'000> DIGITS = []{
        array<array<char, 4>, 10'000> digits{};
        for (int i = 3, d = 1; i >= 0; --i, d *= 10)
            rep(j, 10'000)
                digits[j][i] = char('0' + j / d % 10);
        return digits;
    }();
    array<char, BUF_SIZE> ibuf, obuf;
    char *iptr = data(ibuf), *iend = iptr, *optr = data(obuf);

    template <class T> constexpr bool is_std_array = false;
    template <class T, size_t I> constexpr bool is_std_array<array<T, I>> = true;

    void flush() {
        for (auto* p = begin(obuf); p != optr; p += write(STDOUT_FILENO, p, optr - p));
        optr = begin(obuf);
    }
    int _flush_atexit = []{ atexit(flush); return 0; }();

    void refill() {
        memmove(begin(ibuf), iptr, iend - iptr);
        iend -= iptr - begin(ibuf);
        iptr = begin(ibuf);
        iend += read(STDIN_FILENO, iend, end(ibuf) - 1 - iend);
        *iend = '\0';
    }

    template <class T, class T2 = remove_cvref_t<T>>
    void print(T&& val) {
        if (end(obuf) - optr < 64)
            flush();

        if constexpr (is_same_v<T2, char>)
            *optr++ = val;
        else if constexpr (is_same_v<T2, bool> || is_same_v<T2, vector<bool>::reference>)
            return print(int(val));
        else if constexpr (is_integral_v<T2> && is_signed_v<T2>) {
            if (val < 0)
                *optr++ = '-';
            using U = make_unsigned_t<T2>;
            return print(U(val < 0 ? -U(val) : U(val)));
        } else if constexpr (is_integral_v<T2> && is_unsigned_v<T2>) {
            T2 val2 = val;
            array<char, 64> tmp;
            char* tptr = end(tmp);
            while (val2 >= 10'000)
                tptr -= 4, memcpy(tptr, &DIGITS[val2 % 10'000][0], 4), val2 /= T2(10'000);
            int d = (val2 >= 100 ? (val2 >= 1000 ? 4 : 3) : (val2 >= 10 ? 2 : 1));
            memcpy(optr, &DIGITS[val2][4 - d], d);
            memcpy(optr + d, tptr, end(tmp) - tptr);
            optr += d + int(end(tmp) - tptr);
        } else if constexpr (is_floating_point_v<T2>)
            optr += sprintf(optr, "%.30Lf", (long double)val);
        else if constexpr (is_convertible_v<T, string_view>) {
            string_view sv(val);
            if (sz(sv) + 1 <= end(obuf) - optr)
                memcpy(optr, data(sv), sz(sv)), optr += sz(sv);
            else {
                flush();
                for (auto *p = data(sv), *pe = p + sz(sv); p != pe; p += write(STDOUT_FILENO, p, pe - p));
            }
        } else if constexpr (IsModInt<T2>)
            return print(typename T2::Int(val));
        else if constexpr (cp_lib_type_meta::is_tuple_like<T2> && !is_std_array<T2>)
            return apply([](auto&&... items) { (print(items), ...); }, forward<T>(val));
        else {
            trav(item, val)
                print(item);
            return;
        }
        *optr++ = ' ';
    }

    template <class T>
    void read(T& val) {
        auto skip_ws = [] {
            do {
                for (; iptr != iend && *iptr <= ' '; ++iptr);
                if (iend - iptr < 64)
                    refill();
            } while (*iptr <= ' ');
        };
        auto read_other = [&](auto other) {
            read(other);
            return other;
        };

        if constexpr (is_same_v<T, char>)
            skip_ws(), val = *iptr++;
        else if constexpr (is_same_v<T, bool> || is_same_v<T, vector<bool>::reference>) {
            val = bool(read_other(uint8_t()));
        } else if constexpr (IsModInt<T>) {
            val = T(read_other(ll()));
        } else if constexpr (is_integral_v<T>) {
            skip_ws();
            if (is_signed_v<T> && *iptr == '-')
                ++iptr, val = T(-read_other(make_unsigned_t<T>()));
            else
                for (val = 0; iptr != iend && *iptr > ' '; val = T(10 * val + (*iptr++ & 15)));
        } else if constexpr (is_floating_point_v<T>)
            skip_ws(), val = T(strtold(iptr, &iptr));
        else if constexpr (is_same_v<T, string>) {
            skip_ws();
            val.clear();
            do {
                auto* after = find_if(iptr, iend, [](char c) { return c <= ' '; });
                val.append(iptr, after);
                if ((iptr = after) != iend)
                    break;
                refill();
            } while (iptr != iend);
        } else if constexpr (cp_lib_type_meta::is_tuple_like<T> && !is_std_array<T>)
            apply([](auto&... items) { (read(items), ...); }, val);
        else
            trav(item, val)
                read(item);
    }
}

using cp_lib_io::flush;

template <class... Args>
void print(Args&&... args) { (cp_lib_io::print(forward<Args>(args)), ...); }

template <class... Args>
void println(Args&&... args) {
    if (sizeof...(Args))
        (cp_lib_io::print(forward<Args>(args)), ...), *(cp_lib_io::optr - 1) = '\n';
    else
        print('\n'), --cp_lib_io::optr;
}

template <class... Args>
void read(Args&... args) { (cp_lib_io::read(args), ...); }

#ifdef CP_LIB_DEBUG
#define cp_lib_assert(expr) \
    do { if (!(expr)) { \
        ::cerr << "assertion failed: " << #expr << " (" << __FILE__ << ':' << __LINE__ << ")\n"; \
        ::abort(); \
    } } while (0)
#else
#define cp_lib_assert(expr)
#endif

template <class T, class E, class F = multiplies<T>>
constexpr T modpow(T b, E e, T one, F&& mul = F()) {
    for (; e; b = mul(b, b), e >>= 1)
        if (e & 1)
            one = mul(one, b);
    return one;
}

template <class T>
struct LazyReduction {
    T mod, mod2;

    using Wide = conditional_t<is_same_v<T, uint32_t>, uint64_t, unsigned __int128>;
    static constexpr T LIMIT = T(1) << (numeric_limits<T>::digits - 2);

    constexpr explicit LazyReduction(T mod_) : mod(mod_), mod2(2 * mod) {
        cp_lib_assert(1 <= mod && mod < LIMIT);
    }

    constexpr T add(T a, T b) const noexcept { return (a += b) >= mod2 ? a - mod2 : a; }
    constexpr T sub(T a, T b) const noexcept { return make_signed_t<T>(a -= b) < 0 ? a + mod2 : a; }
    constexpr T norm(T x) const noexcept { return x >= mod ? x - mod : x; }
};

template <class T>
struct MontgomeryReduction : LazyReduction<T> {
    using typename LazyReduction<T>::Wide;
    T inv, r2;

    constexpr explicit MontgomeryReduction(T mod_) noexcept
        : LazyReduction<T>(mod_), inv(mod_), r2(T(-Wide(mod_) % mod_)) {
        cp_lib_assert(mod_ % 2 && 3 <= mod_);
        rep(_, __builtin_ctz(numeric_limits<T>::digits) - 1)
            inv *= 2 - mod_ * inv;
    }

    constexpr T in(T x) const noexcept { cp_lib_assert(x < this->mod); return mul(x, r2); }
    constexpr T out(T x) const noexcept { return this->norm(reduce(x)); }
    constexpr T reduce(Wide x) const noexcept {
        return T((x - T(x) * inv * Wide(this->mod)) >> numeric_limits<T>::digits) + this->mod;
    }
    constexpr T mul(T a, T b) const noexcept { return reduce(Wide(a) * b); }
    constexpr T pow(T b, T e, T one) const noexcept { return modpow(b, e, one, [&](T x, T y) { return mul(x, y); }); }
};

template <class T>
struct BarrettReduction : LazyReduction<T> {
    using typename LazyReduction<T>::Wide;
    Wide m;

    constexpr explicit BarrettReduction(T mod_) noexcept
        : LazyReduction<T>(mod_), m(Wide(-1) / mod_) {}

    constexpr T reduce(Wide x) const noexcept {
        if constexpr (is_same_v<T, uint32_t>)
            return uint32_t(x - uint64_t(((unsigned __int128)(m) * x) >> 64) * this->mod);
        else {
            Wide ml = T(m), mh = T(m >> 64), xl = T(x), xh = T(x >> 64);
            auto hh = mh * xh, hl = mh * xl, lh = ml * xh, ll_ = ml * xl;
            auto carry = Wide(T(hl)) + T(lh) + (ll_ >> 64);
            auto upper = hh + (hl >> 64) + (lh >> 64) + (carry >> 64);
            return T(x - upper * this->mod);
        }
    }
    constexpr T mul(T a, T b) const noexcept { return reduce(Wide(a) * b); }
    constexpr T pow(T b, T e) const noexcept {
        return this->mod == 1 ? 0 : modpow(b, e, T(1), [&](T x, T y) { return mul(x, y); });
    }
};

template <class T, T... Witnesses>
constexpr bool miller_rabin(T n) noexcept {
    if (n < 2 || n % 6 % 4 != 1)
        return (n | 1) == 3;
    MontgomeryReduction mont(n);
    int s = __builtin_ctzll(n - 1);
    T d = (n - 1) >> s, one = mont.norm(mont.in(1)), neg_one = mont.norm(mont.in(n - 1));
    auto check = [&](uint32_t a) {
        if (a % n == 0)
            return true;
        T pe = mont.norm(mont.pow(mont.in(a % n), d, one));
        if (pe == one || pe == neg_one)
            return true;
        for (int i = s - 1; pe != neg_one && i >= 0; --i)
            pe = mont.norm(mont.mul(pe, pe));
        return pe == neg_one;
    };
    return (check(Witnesses) && ...);
}

template <class T>
constexpr bool is_prime(T n) noexcept {
    cp_lib_assert(n >= 0);
    if (n < MontgomeryReduction<uint32_t>::LIMIT)
        return miller_rabin<uint32_t, 2, 7, 61>(uint32_t(n));
    return miller_rabin<uint64_t, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(uint64_t(n));
}

template <class T, enable_if_t<is_unsigned_v<T>, int> = 0>
constexpr auto binary_gcd(T a, T b) noexcept {
    if (!a || !b)
        return a | b;
    T s = __builtin_ctzll(a | b), c = 0;
    for (a >>= __builtin_ctzll(a); b; b -= a) {
        b >>= __builtin_ctzll(b);
        if (a > b)
            c = a, a = b, b = c;
    }
    return a << s;
}

template <class T>
constexpr T pollard_rho(T n) noexcept {
    cp_lib_assert(n >= 3 && n % 2);
    MontgomeryReduction mont(n);
    for (T z = 2; ; z++) {
        T x = 0, y = mont.in(2), ys = 0, q = mont.in(1), g = 1;
        for (int r = 1; g == 1; r <<= 1) {
            x = y;
            rep(_, r)
                y = mont.add(mont.mul(y, y), z);
            for (int k = 0; g == 1 && k < r; k += 256) {
                ys = y;
                rep(_, min(256, r - k))
                    y = mont.add(mont.mul(y, y), z), q = mont.mul(q, mont.sub(x, y));
                g = binary_gcd(mont.out(q), n);
            }
        }

        if (g == n) {
            do {
                ys = mont.add(mont.mul(ys, ys), z);
                g = binary_gcd(mont.out(mont.sub(x, ys)), n);
            } while (g == 1);
        }

        if (g != n)
            return g;
    }
}

template <class T, class F>
constexpr auto factorize(T n, F&& f) {
    cp_lib_assert(n > 0);
    array<array<T, 2>, numeric_limits<T>::digits> fact{};
    int c = 0;
    if (__builtin_ctzll(n))
        fact[c] = {2, T(__builtin_ctzll(n))}, n >>= fact[c++][1];
    if (n != 1) {
        array<T, numeric_limits<T>::digits> q{};
        q[0] = n;
        for (int i = 0, r = 1, j = 0; i < r;) {
            if (is_prime(q[i])) {
                for (j = 0; j < c && fact[j][0] != q[i]; ++j);
                fact[j] = {q[i++], fact[j][1] + 1};
                c += j == c;
            } else {
                q[r] = q[i] < MontgomeryReduction<uint32_t>::LIMIT
                    ? T(pollard_rho(uint32_t(q[i])))
                    : T(pollard_rho(uint64_t(q[i])));
                q[i] /= q[r++];
            }
        }
    }
    return f(begin(fact), begin(fact) + c);
}

template <class T>
constexpr T euler_totient(T n) noexcept {
    return factorize(n, [&](auto it, auto it_end) {
        for (; it != it_end; ++it)
            n -= n / (*it)[0];
        return n;
    });
}

template <class T, class Next>
T power_tower(T mod, uint64_t h, Next&& next) {
    if (!h)
        return T(1) % mod;
    array<uint64_t, 64> mods{}, vals{};
    mods[0] = uint64_t(mod);
    int cnt = 1;
    for (; mods[cnt - 1] != 1 && cnt < h; ++cnt)
        mods[cnt] = euler_totient(mods[cnt - 1]);
    rep(i, cnt)
        vals[i] = next();

    uint64_t x = 1;
    auto pow = [&x](auto v, auto m) {
        using Int = decltype(m);
        BarrettReduction barrett(m);
        return modpow(v, Int(x), Int(1), [&](auto a, auto b) {
            auto c = typename decltype(barrett)::Wide(a) * b;
            if (c < m)
                return Int(c);
            auto d = barrett.reduce(c);
            return d < m ? d + m : d;
        });
    };

    repr(i, cnt - 1, 0)
        x = mods[i] < BarrettReduction<uint32_t>::LIMIT
            ? T(pow(uint32_t(vals[i]), uint32_t(mods[i])))
            : T(pow(vals[i], mods[i]));
    return T(x >= mod ? x - mods[0] : x);
}

template <class T>
T tetration(T a, T b, T mod) noexcept {
    return a ? power_tower(mod, b, [a]{ return a; }) : (1 - b % 2) % mod;
}

int main() {
    int q; read(q);
    while (q--) {
        int a, b, m; read(a, b, m);
        println(tetration(a, b, m));
    }
}
