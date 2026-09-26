#ifndef LOCAL
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#endif

#include <bits/stdc++.h>
namespace mitsuha {
    template <class T> bool chmin(T& x, const T& y) { return y >= x ? false : (x = y, true); }
    template <class T> bool chmax(T& x, const T& y) { return y <= x ? false : (x = y, true); }
    template <class T> constexpr T floor(const T x, const T y) { T q = x / y, r = x % y; return q - ((x ^ y) < 0 and (r != 0)); }
    template <class T> constexpr T ceil(const T x, const T y) { T q = x / y, r = x % y; return q + ((x ^ y) > 0 and (r != 0)); }
}

namespace mitsuha::macro {
#define IMPL_REPITER(cond) auto& begin() { return *this; } auto end() { return nullptr; } auto& operator*() { return _val; } auto& operator++() { return _val += _step, *this; } bool operator!=(std::nullptr_t) { return cond; }
    template <class Int, class IntL = Int, class IntStep = Int, std::enable_if_t<(std::is_signed_v<Int> == std::is_signed_v<IntL>), std::nullptr_t> = nullptr> struct rep_impl {
        Int _val; const Int _end, _step;
        rep_impl(Int n) : rep_impl(0, n) {}
        rep_impl(IntL l, Int r, IntStep step = 1) : _val(l), _end(r), _step(step) {}
        IMPL_REPITER((_val < _end))
    };
    template <class Int, class IntL = Int, class IntStep = Int, std::enable_if_t<(std::is_signed_v<Int> == std::is_signed_v<IntL>), std::nullptr_t> = nullptr> struct rrep_impl {
        Int _val; const Int _end, _step;
        rrep_impl(Int n) : rrep_impl(0, n) {}
        rrep_impl(IntL l, Int r) : _val(r - 1), _end(l), _step(-1) {}
        rrep_impl(IntL l, Int r, IntStep step) : _val(l + floor<Int>(r - l - 1, step) * step), _end(l), _step(-step) {}
        IMPL_REPITER((_val >= _end))
    };
#undef IMPL_REPITER
}

#include <unistd.h>
namespace mitsuha::io {
    static constexpr uint32_t SZ = 1 << 17;
    char ibuf[SZ], obuf[SZ], out[100];
    uint32_t pil = 0, pir = 0, por = 0;
    struct Pre {
        char num[10000][4];
        constexpr Pre() : num() {
            for (int i = 0; i < 10000; i++) {
                int n = i;
                for (int j = 3; j >= 0; j--) { num[i][j] = n % 10 | '0'; n /= 10; }
            }
        }
    } constexpr pre;

    inline void load() {
        memcpy(ibuf, ibuf + pil, pir - pil);
        pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
        pil = 0;
    }
    inline void flush() { fwrite(obuf, 1, por, stdout); por = 0; }

    void rd(char &c) { do { if (pil + 1 > pir) load(); c = ibuf[pil++]; } while (isspace(c)); }
    void rd(std::string &x) {
        x.clear(); char c;
        do { if (pil + 1 > pir) load(); c = ibuf[pil++]; } while (isspace(c));
        do { x += c; if (pil == pir) load(); if (pil == pir) break; c = ibuf[pil++]; } while (!isspace(c));
    }
    template <typename T> void rd_real(T &x) { std::string s; rd(s); x = stod(s); }
    template <typename T>
    void rd_integer(T &x) {
        if (pil + 100 > pir) load();
        char c;
        do { c = ibuf[pil++]; }while (c < '-');
        bool minus = 0;
        if constexpr (std::is_signed<T>::value || std::is_same_v<T, __int128>) { if (c == '-') { minus = 1, c = ibuf[pil++]; } }
        x = 0;
        while (c >= '0') { x = x * 10 + (c & 15), c = ibuf[pil++]; }
        if constexpr (std::is_signed<T>::value || std::is_same_v<T, __int128>) { if (minus) x = -x; }
    }

    void rd(int &x) { rd_integer(x); }
    void rd(long long &x) { rd_integer(x); }
    void rd(__int128 &x) { rd_integer(x); }
    void rd(unsigned int &x) { rd_integer(x); }
    void rd(unsigned long long &x) { rd_integer(x); }
    void rd(unsigned __int128 &x) { rd_integer(x); }
    void rd(double &x) { rd_real(x); }
    void rd(long double &x) { rd_real(x); }
    void rd(__float128 &x) { rd_real(x); }
    
    template <class T, class U> void rd(std::pair<T, U> &p) { return rd(p.first), rd(p.second); }
    template <size_t N = 0, typename T>
    void rd(T &t) { if constexpr (N < std::tuple_size<T>::value) { auto &x = std::get<N>(t); rd(x); rd<N + 1>(t); } }
    template <class... T> void rd(std::tuple<T...> &tpl) { rd(tpl); }
    template <size_t N = 0, typename T> void rd(std::array<T, N> &x) { for (auto &d: x) rd(d); }
    template <class T> void rd(std::vector<T> &x) { for (auto &d: x) rd(d); }

    void read() {}
    template <class H, class... T> void read(H &h, T &... t) { rd(h), read(t...); }

    void wt(const char c) { if (por == SZ) flush(); obuf[por++] = c; }
    void wt(const std::string &s) { for (char c: s) wt(c); }
    void wt(const char *s) { size_t len = strlen(s); for (size_t i = 0; i < len; i++) wt(s[i]); }
    template <typename T> void wt_integer(T x) {
        if (por > SZ - 100) flush();
        if (x < 0) { obuf[por++] = '-', x = -x; }
        int outi;
        for (outi = 96; x >= 10000; outi -= 4) { memcpy(out + outi, pre.num[x % 10000], 4); x /= 10000; }
        if (x >= 1000) { memcpy(obuf + por, pre.num[x], 4); por += 4; } 
        else if (x >= 100) { memcpy(obuf + por, pre.num[x] + 1, 3); por += 3; } 
        else if (x >= 10) { int q = (x * 103) >> 10; obuf[por] = q | '0'; obuf[por + 1] = (x - q * 10) | '0'; por += 2; } 
        else obuf[por++] = x | '0';
        memcpy(obuf + por, out + outi + 4, 96 - outi); por += 96 - outi;
    }
    template <typename T>
    void wt_real(T x) {
        std::ostringstream oss; oss << std::fixed << std::setprecision(15) << double(x);
        std::string s = oss.str(); wt(s);
    }
    void wt(int x) { wt_integer(x); }
    void wt(long long x) { wt_integer(x); }
    void wt(__int128 x) { wt_integer(x); }
    void wt(unsigned int x) { wt_integer(x); }
    void wt(unsigned long long x) { wt_integer(x); }
    void wt(unsigned __int128 x) { wt_integer(x); }
    void wt(double x) { wt_real(x); }
    void wt(long double x) { wt_real(x); }
    void wt(__float128 x) { wt_real(x); }
    
    template <class T, class U> void wt(const std::pair<T, U> val) { wt(val.first); wt(' '); wt(val.second); }
    template <size_t N = 0, typename T> void wt_tuple(const T t) {
        if constexpr (N < std::tuple_size<T>::value) {
            if constexpr (N > 0) { wt(' '); }
            const auto x = std::get<N>(t);
            wt(x); wt_tuple<N + 1>(t);
        }
    }
    template <class... T> void wt(std::tuple<T...> tpl) { wt_tuple(tpl); }
    template <class T, size_t S> void wt(const std::array<T, S> val) {
        auto n = val.size(); for (size_t i = 0; i < n; i++) { if (i) wt(' '); wt(val[i]); }
    }
    template <class T> void wt(const std::vector<T> val) {
        auto n = val.size(); for (size_t i = 0; i < n; i++) { if (i) wt(' '); wt(val[i]); }
    }

    void print() { wt('\n'); }
    template <class Head, class... Tail>
    void print(Head &&head, Tail &&... tail) { wt(head); if (sizeof...(Tail)) wt(' '); print(std::forward<Tail>(tail)...); }
    void __attribute__((destructor)) _d() { flush(); }
} // namespace mitsuha::io
namespace mitsuha{ using io::read; using io::print; using io::flush; }

namespace mitsuha {
    template <class T, class ToKey, class CompKey = std::less<>, std::enable_if_t<std::conjunction_v<std::is_invocable<ToKey, T>, std::is_invocable_r<bool, CompKey, std::invoke_result_t<ToKey, T>, std::invoke_result_t<ToKey, T>>>, std::nullptr_t> = nullptr>
    auto lambda(const ToKey& to_key, const CompKey& comp_key = std::less<>()) {
        return [=](const T& x, const T& y) { return comp_key(to_key(x), to_key(y)); };
    }
    template <class Compare, std::enable_if_t<std::is_invocable_r_v<bool, Compare, int, int>, std::nullptr_t> = nullptr>
    std::vector<int> sorted_indices(int n, const Compare& compare) {
        std::vector<int> p(n);
        return std::iota(p.begin(), p.end(), 0), std::sort(p.begin(), p.end(), compare), p;
    }
    template <class ToKey, std::enable_if_t<std::is_invocable_v<ToKey, int>, std::nullptr_t> = nullptr>
    std::vector<int> sorted_indices(int n, const ToKey& to_key) { return sorted_indices(n, lambda<int>(to_key)); }

    template <typename T, typename Gen>
    auto generate_vector(int n, Gen generator) { std::vector<T> v(n); for (int i = 0; i < n; ++i) v[i] = generator(i); return v; }
    template <typename T> auto generate_range(T l, T r) { return generate_vector<T>(r - l, [l](int i) { return l + i; }); }
    template <typename T> auto generate_range(T n) { return generate_range(0, n); }

    template <class Iterable>
    void settify(Iterable& a) { std::sort(a.begin(), a.end()), a.erase(std::unique(a.begin(), a.end()), a.end()); }

    template <size_t D> struct Dim : std::array<int, D> {
        template <typename ...Ints> Dim(const Ints& ...ns) : std::array<int, D>::array{ static_cast<int>(ns)... } {}
    };
    template <typename ...Ints> Dim(const Ints& ...) -> Dim<sizeof...(Ints)>;
    template <class T, size_t D, size_t I = 0>
    auto ndvec(const Dim<D> &ns, const T& value = {}) {
        if constexpr (I + 1 < D) {
            return std::vector(ns[I], ndvec<T, D, I + 1>(ns, value));
        } else {
            return std::vector<T>(ns[I], value);
        }
    } 
} // namescape mitsuha

namespace mitsuha {
    using str = std::string;
    using int128 = __int128;
    using uint128 = unsigned __int128;
    template <class T> using min_priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;
    template <class T> using max_priority_queue = std::priority_queue<T, std::vector<T>, std::less<T>>;
}
namespace mitsuha { const std::string Yes = "Yes", No = "No", YES = "YES", NO = "NO"; }

#define Int(...) int __VA_ARGS__; read(__VA_ARGS__)
#define Ll(...) long long __VA_ARGS__; read(__VA_ARGS__)
#define Dbl(...) double __VA_ARGS__; read(__VA_ARGS__)
#define Chr(...) char __VA_ARGS__; read(__VA_ARGS__)
#define Str(...) string __VA_ARGS__; read(__VA_ARGS__)
#define Vt(type, name, size) vector<type> name(size); read(name)
#define Vvt(type, name, h, w) vector<vector<type>> name(h, vector<type>(w)); read(name)
#define die(...)  do { print(__VA_ARGS__); return; } while (false)
#define kill(...) do { print(__VA_ARGS__); return 0; } while (false)

#define Each(e, v) for (auto &&e : v)
#define CFor(e, v) for (const auto &e : v)
#define For(i, ...) CFor(i, mitsuha::macro::rep_impl(__VA_ARGS__))
#define Frr(i, ...) CFor(i, mitsuha::macro::rrep_impl(__VA_ARGS__))
#define Loop(n) for ([[maybe_unused]] const auto& _ : mitsuha::macro::rep_impl(n))

#define All(iterable) std::begin(iterable), std::end(iterable)
#define len(iterable) (long long) iterable.size()
#define elif else if
 
using namespace mitsuha;
using namespace std;

#ifdef LOCAL
/*-*/#include "library/debug/pprint.hpp"
#else
#define debug(...) void(0)
#endif
 
constexpr int iinf = std::numeric_limits<int>::max() / 2;
constexpr long long linf = std::numeric_limits<long long>::max() / 2;

namespace mitsuha{
struct has_mod_impl {
    template <class T>
    static auto check(T &&x) -> decltype(x.get_mod(), std::true_type{});
    template <class T>
    static auto check(...) -> std::false_type;
};

template <class T>
class has_mod : public decltype(has_mod_impl::check<T>(std::declval<T>())) {};

template <typename mint>
mint inv(int n) {
    static const int mod = mint::get_mod();
    static vector<mint> dat = {0, 1};
    assert(0 <= n);
    if (n >= mod) n %= mod;
    while (len(dat) <= n) {
        int k = len(dat);
        int q = (mod + k - 1) / k;
        dat.emplace_back(dat[k * q - mod] * mint::raw(q));
    }
    return dat[n];
}

template <typename mint>
mint fact(int n) {
    static const int mod = mint::get_mod();
    assert(0 <= n && n < mod);
    static vector<mint> dat = {1, 1};
    while (len(dat) <= n) dat.emplace_back(dat[len(dat) - 1] * mint::raw(len(dat)));
    return dat[n];
}

template <typename mint>
mint fact_inv(int n) {
    static vector<mint> dat = {1, 1};
    if (n < 0) return mint(0);
    while (len(dat) <= n) dat.emplace_back(dat[len(dat) - 1] * inv<mint>(len(dat)));
    return dat[n];
}

template <class mint, class... Ts>
mint fact_invs(Ts... xs) {
    return (mint(1) * ... * fact_inv<mint>(xs));
}

template <typename mint, class Head, class... Tail>
mint multinomial(Head &&head, Tail &&... tail) {
    return fact<mint>(head) * fact_invs<mint>(std::forward<Tail>(tail)...);
}

template <typename mint>
mint C_dense(int n, int k) {
    static vector<vector<mint>> C;
    static int H = 0, W = 0;
    auto calc = [&](int i, int j) -> mint {
        if (i == 0) return (j == 0 ? mint(1) : mint(0));
        return C[i - 1][j] + (j ? C[i - 1][j - 1] : 0);
    };
    if (W <= k) {
        for(int i = 0; i < H; ++i) {
            C[i].resize(k + 1);
            for(int j = W; j < k + 1; ++j) { C[i][j] = calc(i, j); }
        }
        W = k + 1;
    }
    if (H <= n) {
        C.resize(n + 1);
        for(int i = H; i < n + 1; ++i) {
            C[i].resize(W);
            for(int j = 0; j < W; ++j) { C[i][j] = calc(i, j); }
        }
        H = n + 1;
    }
    return C[n][k];
}

template <typename mint, bool large = false, bool dense = false>
mint C(long long n, long long k) {
    assert(n >= 0);
    if (k < 0 || n < k) return 0;
    if constexpr (dense) return C_dense<mint>(n, k);
    if constexpr (!large) return multinomial<mint>(n, k, n - k);
    k = min(k, n - k);
    mint x(1);
    for(int i = 0; i < k; ++i) x *= mint(n - i);
    return x * fact_inv<mint>(k);
}

template <typename mint, bool large = false>
mint C_inv(long long n, long long k) {
    assert(n >= 0);
    assert(0 <= k && k <= n);
    if (not large) return fact_inv<mint>(n) * fact<mint>(k) * fact<mint>(n - k);
    return mint(1) / C<mint, true>(n, k);
}

// [x^d](1-x)^{-n}
template <typename mint, bool large = false, bool dense = false>
mint C_negative(long long n, long long d) {
    assert(n >= 0);
    if (d < 0) return mint(0);
    if (n == 0) { return (d == 0 ? mint(1) : mint(0)); }
    return C<mint, large, dense>(n + d - 1, d);
}
} // namespace mitsuha

namespace mitsuha{
// odd mod.
// have rx instead of x
template <int id, typename U1, typename U2>
struct Mongomery_modint {
    using mint = Mongomery_modint;
    inline static U1 m, r, n2;
    static constexpr int W = numeric_limits<U1>::digits;

    static void set_mod(U1 mod) {
        assert(mod & 1 && mod <= U1(1) << (W - 2));
        m = mod, n2 = -U2(m) % m, r = m;
        Loop(5) r *= 2 - m * r;
        r = -r;
        assert(r * m == U1(-1));
    }
    static U1 reduce(U2 b) { return (b + U2(U1(b) * r) * m) >> W; }

    U1 x;
    Mongomery_modint() : x(0) {}
    Mongomery_modint(U1 x) : x(reduce(U2(x) * n2)){};
    U1 val() const {
        U1 y = reduce(x);
        return y >= m ? y - m : y;
    }
    mint &operator+=(mint y) {
        x = ((x += y.x) >= m ? x - m : x);
        return *this;
    }
    mint &operator-=(mint y) {
        x -= (x >= y.x ? y.x : y.x - m);
        return *this;
    }
    mint &operator*=(mint y) {
        x = reduce(U2(x) * y.x);
        return *this;
    }
    mint operator+(mint y) const { return mint(*this) += y; }
    mint operator-(mint y) const { return mint(*this) -= y; }
    mint operator*(mint y) const { return mint(*this) *= y; }
    bool operator==(mint y) const {
        return (x >= m ? x - m : x) == (y.x >= m ? y.x - m : y.x);
    }
    bool operator!=(mint y) const { return not operator==(y); }
    mint pow(long long n) const {
        assert(n >= 0);
        mint y = 1, z = *this;
        for (; n; n >>= 1, z *= z)
            if (n & 1) y *= z;
        return y;
    }
};

template <int id>
using Mongomery_modint_32 = Mongomery_modint<id, unsigned int, unsigned long long>;
template <int id>
using Mongomery_modint_64 = Mongomery_modint<id, unsigned long long, __uint128_t>;
} // namespace mitsuha

namespace mitsuha{
bool primetest(const unsigned long long x) {
    assert(x < 1ULL << 62);
    if (x == 2 or x == 3 or x == 5 or x == 7) return true;
    if (x % 2 == 0 or x % 3 == 0 or x % 5 == 0 or x % 7 == 0) return false;
    if (x < 121) return x > 1;
    const unsigned long long d = (x - 1) >> ((x - 1) == 0 ? -1 : __builtin_ctzll(x - 1));

    using mint = Mongomery_modint_64<202311020>;

    mint::set_mod(x);
    const mint one(1ULL), minus_one(x - 1);
    auto ok = [&](unsigned long long a) -> bool {
        auto y = mint(a).pow(d);
        unsigned long long t = d;
        while (y != one && y != minus_one && t != x - 1) y *= y, t <<= 1;
        if (y != minus_one && t % 2 == 0) return false;
        return true;
    };
    if (x < (1ULL << 32)) {
        for (unsigned long long a: {2, 7, 61})
            if (!ok(a)) return false;
    } else {
        for (unsigned long long a: {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
            if (!ok(a)) return false;
        }
    }
    return true;
}
} // namespace mitsuha

namespace mitsuha{
unsigned long long RNG_64() {
    static uint64_t x_
            = uint64_t(chrono::duration_cast<chrono::nanoseconds>(
                    chrono::high_resolution_clock::now().time_since_epoch())
                               .count())
              * 10150724397891781847ULL;
    x_ ^= x_ << 7;
    return x_ ^= x_ >> 9;
}

unsigned long long RNG(unsigned long long lim) { return RNG_64() % lim; }

long long RNG(long long l, long long r) { return l + RNG_64() % (r - l); }
} // namespace mitsuha

namespace mitsuha{
template <typename mint>
long long rho(long long n, long long c) {
    assert(n > 1);
    const mint cc(c);
    auto f = [&](mint x) { return x * x + cc; };
    mint x = 1, y = 2, z = 1, q = 1;
    long long g = 1;
    const long long m = 1LL << (__lg(n) / 5);
    for (long long r = 1; g == 1; r <<= 1) {
        x = y;
        Loop(r) y = f(y);
        for (long long k = 0; k < r && g == 1; k += m) {
            z = y;
            Loop(min(m, r - k)) y = f(y), q *= x - y;
            g = gcd(q.val(), n);
        }
    }
    if (g == n) do {
            z = f(z);
            g = gcd((x - z).val(), n);
        } while (g == 1);
    return g;
}

long long find_prime_factor(long long n) {
    assert(n > 1);
    if (primetest(n)) return n;
    Loop(100) {
        long long m = 0;
        if (n < (1 << 30)) {
            using mint = Mongomery_modint_32<20231025>;
            mint::set_mod(n);
            m = rho<mint>(n, RNG(0, n));
        } else {
            using mint = Mongomery_modint_64<20231025>;
            mint::set_mod(n);
            m = rho<mint>(n, RNG(0, n));
        }
        if (primetest(m)) return m;
        n = m;
    }
    assert(0);
    return -1;
}

// returns sorted
vector<pair<long long, int>> factor(long long n) {
    assert(n >= 1);
    vector<pair<long long, int>> pf;
    For(p, 2, 100) {
        if (p * p > n) break;
        if (n % p == 0) {
            long long e = 0;
            do { n /= p, e += 1; } while (n % p == 0);
            pf.emplace_back(p, e);
        }
    }
    while (n > 1) {
        long long p = find_prime_factor(n);
        long long e = 0;
        do { n /= p, e += 1; } while (n % p == 0);
        pf.emplace_back(p, e);
    }
    sort(pf.begin(), pf.end());
    return pf;
}

vector<pair<long long, int>> factor_by_lpf(long long n, vector<int>& lpf) {
    vector<pair<long long, int>> res;
    while (n > 1) {
        int p = lpf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        res.emplace_back(p, e);
    }
    return res;
}
} // namespace mitsuha

namespace mitsuha{
// https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp
struct Barrett {
    unsigned int m;
    unsigned long long im;
    explicit Barrett(unsigned int m = 1) : m(m), im((unsigned long long)(-1) / m + 1) {}
    unsigned int umod() const { return m; }
    unsigned int modulo(unsigned long long z) {
        if (m == 1) return 0;
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * m;
        return (z - y + (z < y ? m : 0));
    }
    unsigned long long floor(unsigned long long z) {
        if (m == 1) return z;
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * m;
        return (z < y ? x - 1 : x);
    }
    pair<unsigned long long, unsigned int> divmod(unsigned long long z) {
        if (m == 1) return {z, 0};
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * m;
        if (z < y) return {x - 1, z - y + m};
        return {x, z - y};
    }
    unsigned int mul(unsigned int a, unsigned int b) { return modulo((unsigned long long)(a) * b); }
};

struct Barrett_64 {
    unsigned __int128 mod, mh, ml;

    explicit Barrett_64(unsigned long long mod = 1) : mod(mod) {
        unsigned __int128 m = (unsigned __int128)(-1) / mod;
        if (m * mod + mod == (unsigned __int128)(0)) ++m;
        mh = m >> 64;
        ml = m & (unsigned long long)(-1);
    }

    unsigned long long umod() const { return mod; }

    unsigned long long modulo(unsigned __int128 x) {
        unsigned __int128 z = (x & (unsigned long long)(-1)) * ml;
        z = (x & (unsigned long long)(-1)) * mh + (x >> 64) * ml + (z >> 64);
        z = (x >> 64) * mh + (z >> 64);
        x -= z * mod;
        return x < mod ? x : x - mod;
    }

    unsigned long long mul(unsigned long long a, unsigned long long b) { return modulo((unsigned __int128)(a) * b); }
};
} // namespace mitsuha

namespace mitsuha{
unsigned int mod_pow(int a, long long n, int mod) {
    assert(n >= 0);
    a = ((a %= mod) < 0 ? a + mod : a);
    if ((mod & 1) && (mod < (1 << 30))) {
        using mint = Mongomery_modint_32<202311021>;
        mint::set_mod(mod);
        return mint(a).pow(n).val();
    }
    Barrett bt(mod);
    int r = 1;
    while (n) {
        if (n & 1) r = bt.mul(r, a);
        a = bt.mul(a, a), n >>= 1;
    }
    return r;
}

unsigned long long mod_pow_64(long long a, long long n, unsigned long long mod) {
    assert(n >= 0);
    a = ((a %= mod) < 0 ? a + mod : a);
    if ((mod & 1) && (mod < (1ULL << 62))) {
        using mint = Mongomery_modint_64<202311021>;
        mint::set_mod(mod);
        return mint(a).pow(n).val();
    }
    Barrett_64 bt(mod);
    long long r = 1;
    while (n) {
        if (n & 1) r = bt.mul(r, a);
        a = bt.mul(a, a), n >>= 1;
    }
    return r;
}
} // namespace mitsuha

namespace mitsuha{
int primitive_root(int p) {
    auto pf = factor(p - 1);
    auto is_ok = [&](int g) -> bool {
        for (auto&& [q, e]: pf)
            if (mod_pow(g, (p - 1) / q, p) == 1) return false;
        return true;
    };
    while (1) {
        int x = RNG(1, p);
        if (is_ok(x)) return x;
    }
    return -1;
}

long long primitive_root_64(long long p) {
    auto pf = factor(p - 1);
    auto is_ok = [&](long long g) -> bool {
        for (auto&& [q, e]: pf)
            if (mod_pow_64(g, (p - 1) / q, p) == 1) return false;
        return true;
    };
    while (1) {
        long long x = RNG(1, p);
        if (is_ok(x)) return x;
    }
    return -1;
}
} // namespace mitsuha

namespace mitsuha{
template <int id>
struct Dynamic_Modint {
    static constexpr bool is_modint = true;
    using mint = Dynamic_Modint;
    unsigned int val;
    static Barrett bt;
    static unsigned int umod() { return bt.umod(); }

    static int get_mod() { return (int)(bt.umod()); }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = Barrett(m);
    }

    static Dynamic_Modint raw(unsigned int v) {
        Dynamic_Modint x;
        x.val = v;
        return x;
    }
    Dynamic_Modint() : val(0) {}
    Dynamic_Modint(unsigned int x) : val(bt.modulo(x)) {}
    Dynamic_Modint(unsigned long long x) : val(bt.modulo(x)) {}
    Dynamic_Modint(int x) : val((x %= get_mod()) < 0 ? x + get_mod() : x) {}
    Dynamic_Modint(long long x) : val((x %= get_mod()) < 0 ? x + get_mod() : x) {}

    mint& operator+=(const mint& rhs) {
        val = (val += rhs.val) < umod() ? val : val - umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        val = (val += umod() - rhs.val) < umod() ? val : val - umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        val = bt.mul(val, rhs.val);
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inverse(); }
    mint operator-() const { return mint() - *this; }
    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x, n >>= 1;
        }
        return r;
    }
    mint inverse() const {
        int x = val, mod = get_mod();
        int a = x, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b), swap(u -= t * v, v);
        }
        if (u < 0) u += mod;
        return u;
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs.val != rhs.val;
    }
    static pair<int, int>& get_ntt() {
        static pair<int, int> p = {-1, -1};
        return p;
    }
    static void set_ntt_info() {
        int mod = get_mod();
        int k = ((mod - 1) == 0 ? -1 : __builtin_ctz(mod - 1));
        int r = primitive_root(mod);
        r = mod_pow(r, (mod - 1) >> k, mod);
        get_ntt() = {k, r};
    }
    static pair<int, int> ntt_info() { return get_ntt(); }
    static bool can_ntt() { return ntt_info().first != -1; }
};

template <int id>
void rd(Dynamic_Modint<id>& x) {
  io::rd(x.val);
  x.val %= Dynamic_Modint<id>::umod();
}
template <int id>
void wt(Dynamic_Modint<id> x) {
  io::wt(x.val);
}

using dmint = Dynamic_Modint<-1>;
template <int id>
Barrett Dynamic_Modint<id>::bt;
} // namespace mitsuha

namespace mitsuha{
int det_mod(vector<vector<int>> A, int mod) {
    Barrett bt(mod);
    const int n = len(A);
    long long det = 1;
    For(i, n) {
        For(j, i, n) {
            if (A[j][i] == 0) continue;
            if (i != j) { swap(A[i], A[j]), det = mod - det; }
            break;
        }
        For(j, i + 1, n) {
            while (A[i][i] != 0) {
                long long c = mod - A[j][i] / A[i][i];
                Frr(k, i, n) { A[j][k] = bt.modulo(A[j][k] + A[i][k] * c); }
                swap(A[i], A[j]), det = mod - det;
            }
            swap(A[i], A[j]), det = mod - det;
        }
    }
    For(i, n) det = bt.mul(det, A[i][i]);
    return det % mod;
}

template <typename mint>
mint det(vector<vector<mint>>& A) {
    const int n = len(A);
    vector<vector<int>> B(n, vector<int>(n));
    For(i, n) For(j, n) B[i][j] = A[i][j].val;
    return det_mod(B, mint::get_mod());
}
} // namespace mitsuha

int main(){

    Int(n, m);

    dmint::set_mod(m);
    Vvt(dmint, a, n, n);
    print(det(a));
    
    #ifdef LOCAL
        auto __time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        print(); print(localtime(&__time)->tm_min, localtime(&__time)->tm_sec);
    #endif
    return 0;
}

