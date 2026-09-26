#line 1 "test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient"
#line 2 "cplibrary/default_code.hpp"

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
#define X first
#define Y second
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
template<class A, class B>
ostream& operator<<(ostream& os, const pair<A, B> &a) {
    os << "(" << a.first << ", " << a.second << ")";
    return os;
}
template <typename T>
concept PrintableContainer = requires(T& a) {
    a.begin();
    a.end();
} && !std::same_as<std::remove_cvref_t<T>, std::string> &&
     !std::same_as<std::remove_cvref_t<T>, std::string_view> &&
     !std::is_convertible_v<T, const char*>;
template <PrintableContainer T>
std::ostream& operator<<(std::ostream& os, const T& a) {
    os << "[ ";
    bool first = true;
    for (const auto& item : a) {
        if (!first) os << ", ";
        os << item;
        first = false;
    }
    return os << " ]";
}
#ifdef bbq
#include <experimental/iterator>
#define safe cerr<<__PRETTY_FUNCTION__<<" line "<<__LINE__<<" safe\n"
#define sepline sepline_() 
#define debug(a...) debug_(#a, a)
#define orange(a...) orange_(#a, a)
void debug_(auto s, auto ...a) {
    cerr << "\e[1;32m(" << s << ") = (";
    int f = 0;
    (..., (cerr << (f++ ? ", " : "") << a));
    cerr << ")\e[0m\n";
}
void orange_(auto s, auto L, auto R) {
    cerr << "\e[1;33m[ " << s << " ] = [ ";
    using namespace experimental;
    copy(L, R, make_ostream_joiner(cerr, ", "));
    cerr << " ]\e[0m\n";
}
void sepline_(int length = 50) {
    cerr << "\e[1;35m";
    cerr << string(length, '=');
    cerr << "\e[0m\n";
}
#else
#define safe ((void)0)
#define sepline safe
#define debug(...) safe
#define orange(...) safe
#endif

void chmax(auto &x, auto val) {
    x = max(x, val);
}

void chmin(auto &x, auto val) {
    x = min(x, val);
}

vector<int> count_array(const auto &container, int sz = -1) {
    if (sz == -1) sz = *ranges::max_element(container) + 1;
    vector<int> res(sz);
    for (auto x : container) ++res[x];
    return res;
}

template<class T>
void discretization(vector<T> &vals) {
    ranges::sort(vals);
    vals.erase(ranges::unique(vals).begin(), vals.end());
}
#line 3 "test.cpp"

#line 2 "cplibrary/Numeric/Binomial.hpp"

#line 2 "cplibrary/Numeric/internal_math.hpp"
// Reference: Atcoder Library https://github.com/atcoder/ac-library

#line 7 "cplibrary/Numeric/internal_math.hpp"
#include <type_traits>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace internal {

// @param m `1 <= m`
// @return x mod m
constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// Fast modular multiplication by barrett reduction
// Reference: https://en.wikipedia.org/wiki/Barrett_reduction
// NOTE: reconsider after Ice Lake
struct barrett {
    unsigned int _m;
    unsigned long long im;
    explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}
    unsigned int umod() const { return _m; }
    unsigned int modulo(unsigned long long z) const {
        if (_m == 1) return 0;
#ifdef _MSC_VER
        unsigned long long x;
        _umul128(z, im, &x);
#else
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
#endif
        unsigned long long y = x * _m;
        return (z - y + (z < y ? _m : 0));
    }
    unsigned int mul(unsigned int a, unsigned int b) const {
        return modulo((unsigned long long)a * b);
    }
    unsigned long long floor(unsigned long long z) const {
        if (_m == 1) return z;
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * _m;
        return (z < y ? x - 1 : x);
    }
    std::pair<unsigned long long, unsigned int> divmod(unsigned long long z) const {
        if (_m == 1) return {z, 0};
        unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
        unsigned long long y = x * _m;
        if (z < y) return {x - 1, z - y + _m};
        return {x, z - y};
    }
};

// @param n `0 <= n`
// @param m `1 <= m`
// @return `(x ** n) % m`
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

// Reference:
// M. Forisek and J. Jancina,
// Fast Primality Testing for Integers That Fit into a Machine Word
// @param n `0 <= n`
constexpr bool is_prime_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
template <int n> constexpr bool is_prime = is_prime_constexpr(n);

// @param b `1 <= b`
// @return pair(g, x) s.t. g = gcd(a, b), xa = g (mod b), 0 <= x < b/g
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};

    // Contracts:
    // [1] s - m0 * a = 0 (mod b)
    // [2] t - m1 * a = 0 (mod b)
    // [3] s * |m1| + t * |m0| <= b
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;

    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;  // |m1 * u| <= |m1| * s <= b

        // [3]:
        // (s - t * u) * |m1| + t * |m0 - m1 * u|
        // <= s * |m1| - t * u * |m1| + t * (|m0| + |m1| * u)
        // = s * |m1| + t * |m0| <= b

        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    // by [3]: |m0| <= b/g
    // by g != b: |m0| < b/g
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}
}  // namespace internal

namespace internal {

#ifndef _MSC_VER
    template <class T>
        using is_signed_int128 =
        typename std::conditional<std::is_same<T, __int128_t>::value ||
        std::is_same<T, __int128>::value,
        std::true_type,
        std::false_type>::type;

    template <class T>
        using is_unsigned_int128 =
        typename std::conditional<std::is_same<T, __uint128_t>::value ||
        std::is_same<T, unsigned __int128>::value,
        std::true_type,
        std::false_type>::type;

    template <class T>
        using make_unsigned_int128 =
        typename std::conditional<std::is_same<T, __int128_t>::value,
                 __uint128_t,
                 unsigned __int128>;

    template <class T>
        using is_integral = typename std::conditional<std::is_integral<T>::value ||
        is_signed_int128<T>::value ||
        is_unsigned_int128<T>::value,
        std::true_type,
        std::false_type>::type;

    template <class T>
        using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                std::is_signed<T>::value) ||
        is_signed_int128<T>::value,
        std::true_type,
        std::false_type>::type;

    template <class T>
        using is_unsigned_int =
        typename std::conditional<(is_integral<T>::value &&
                std::is_unsigned<T>::value) ||
        is_unsigned_int128<T>::value,
        std::true_type,
        std::false_type>::type;

    template <class T>
        using to_unsigned = typename std::conditional<
        is_signed_int128<T>::value,
        make_unsigned_int128<T>,
        typename std::conditional<std::is_signed<T>::value,
        std::make_unsigned<T>,
        std::common_type<T>>::type>::type;

#else

    template <class T> using is_integral = typename std::is_integral<T>;

    template <class T>
        using is_signed_int =
        typename std::conditional<is_integral<T>::value && std::is_signed<T>::value,
                 std::true_type,
                 std::false_type>::type;

    template <class T>
        using is_unsigned_int =
        typename std::conditional<is_integral<T>::value &&
        std::is_unsigned<T>::value,
        std::true_type,
        std::false_type>::type;

    template <class T>
        using to_unsigned = typename std::conditional<is_signed_int<T>::value,
              std::make_unsigned<T>,
              std::common_type<T>>::type;

#endif

    template <class T>
        using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

    template <class T>
        using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

    template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

#line 229 "cplibrary/Numeric/internal_math.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace internal {

    struct modint_base {};
    struct static_modint_base : modint_base {};

    template <class T> using is_modint = std::is_base_of<modint_base, T>;
    template <class T> using is_modint_t = std::enable_if_t<is_modint<T>::value>;

}  // namespace internal
#line 2 "cplibrary/Numeric/primitive_root.hpp"

#line 2 "cplibrary/Numeric/pollard_rho.hpp"

#line 2 "cplibrary/Numeric/miller_rabin.hpp"

template<typename T>
bool _miller_rabin(T a, T n) {
    if ((a = a % n) == 0) return 1;
    if ((n & 1) ^ 1) return n == 2;
    static auto mul = [&](T x, T y, T mod) {
        if constexpr (sizeof(T) == 4) return (long long)x * y % mod;
        else return (__int128)x * y % mod;
    };
    T t = std::countr_zero(std::make_unsigned_t<T>(n - 1)), x = 1;
    T tmp = (n - 1) >> t;
    for (; tmp; tmp >>= 1, a = mul(a, a, n))
        if(tmp & 1) x = mul(x, a, n);
    if (x == 1 || x == n - 1) return 1;
    while (--t)
        if ((x = mul(x, x, n)) == n - 1) return 1;
    return 0;
}

template<typename T>
bool miller_rabin(T n) {
    if (n == 1) return false;
    static std::vector<T> _base[4] = {{2, 7, 61}, {2, 13, 23, 1662803}, {2, 3, 5, 7, 11, 13}, {2, 325, 9375, 28178, 450775, 9780504, 1795265022}};
    std::vector<T> base =
        (n < 4759123141ll) ? _base[0] :
        (n < 1122004669633ll) ? _base[1] :
        (n < 3474749660383ll) ? _base[2] : _base[3];
    for (T b : base)
        if (!_miller_rabin(b, n))
            return false;
    return true;
}
#line 4 "cplibrary/Numeric/pollard_rho.hpp"

/*
return an unsorted prime list
*/
template<typename T>
std::vector<T> pollard_rho(T n) {
    static auto mul = [&](T x, T y, T mod) {
        if constexpr (sizeof(T) == 4) return (long long)x * y % mod;
        else return (__int128)x * y % mod;
    };
    std::vector<T> res;
    auto factorize = [&](auto self, T cur) -> void {
        if (cur == 1) return;
        if (miller_rabin(cur)) return res.push_back(cur);
        if (cur % 2 == 0) {
            int cnt = std::countr_zero(std::make_unsigned_t<T>(cur));
            res.resize(res.size() + cnt, 2);
            return self(self, cur >> cnt);
        }
        T p = 2, q, i = 1, x = 0, y = 0, t = 0, ct = 87;
        #define f(x) ((mul(x, x, cur) + ct) % cur)
        while (t++ % 64 || std::__gcd(p, cur) == 1) {
            if (x == y) x = i++, y = f(x);
            q = mul(p, x < y ? y - x : x - y, cur);
            if (q) p = q;
            if (p == cur) ++ct;
            x = f(x), y = f(f(y));
        }
        T d = std::__gcd(p, cur);
        self(self, cur / d);
        self(self, d);
    };
    factorize(factorize, n);
    return res;
}
#line 2 "cplibrary/Numeric/mod_pow.hpp"

template<typename T>
T mod_pow(T a, long long n, T mod) {
    assert(n >= 0);
    if (mod == 1) return 0;
    a = ((a %= mod) < 0 ? a + mod : a);
    static auto mul = [&](T x, T y, T _mod) {
        if constexpr (sizeof(T) == 4) return (long long)x * y % _mod;
        else return (__int128)x * y % _mod;
    };
    T res = 1;
    for (; n; n >>= 1, a = mul(a, a, mod))
        if (n & 1)
            res = mul(res, a, mod);
    return res;
}
#line 5 "cplibrary/Numeric/primitive_root.hpp"

template<typename T>
T primitive_root(T p) {
    auto pfacs = pollard_rho(p - 1);
    std::ranges::sort(pfacs);
    pfacs.erase(std::ranges::unique(pfacs).begin(), pfacs.end());
    auto check = [&](T g) -> bool {
        for (auto pf : pfacs)
            if (mod_pow(g, (p - 1) / pf, p) == 1) return false;
        return true;
    };
    std::conditional_t<sizeof(T) == 4, std::mt19937, std::mt19937_64> rng(880301);
    while (1) {
        T x = rng() % (p - 1) + 1;
        if (check(x)) return x;
    }
    return -1;
}
#line 2 "cplibrary/Numeric/mod_inv.hpp"

template<typename T>
T mod_inv(T val, T mod) {
    if (mod == 0) return 0;
    mod = std::abs(mod);
    val %= mod;
    if (val < 0) val += mod;
    T a = val, b = mod, u = 1, v = 0, t;
    while (b > 0) {
        t = a / b;
        swap(a -= t * b, b), swap(u -= t * v, v);
    }
    if (u < 0) u += mod;
    return u;
}
#line 7 "cplibrary/Numeric/Binomial.hpp"

// source: https://maspypy.github.io/library/mod/binomial.hpp
struct BinomialPrimePower {
    using barrett = internal::barrett;
    int p, e, pp, root, ord;
    std::vector<int> exp, log_fact, power;
    barrett bt_p, bt_pp;
    BinomialPrimePower(int _p, int _e) : p(_p), e(_e), power(e + 1, 1), bt_p(1), bt_pp(1) {
        for (int i = 0; i < e; ++i) power[i + 1] = power[i] * p;
        pp = power[e];
        bt_p = barrett(p), bt_pp = barrett(pp);
        std::vector<int> log;
        if (p == 2) {
            if (e <= 1) { return; }
            root = 5;
            ord = pp / 4;
            exp.assign(ord, 1);
            log.assign(pp, 0);
            for (int i = 0; i < ord - 1; ++i) { exp[i + 1] = (exp[i] * root) & (pp - 1); }
            for (int i = 0; i < ord; ++i) log[exp[i]] = log[pp - exp[i]] = i;
        }
        else {
            root = primitive_root(p);
            ord = pp / p * (p - 1);
            exp.assign(ord, 1);
            log.assign(pp, 0);
            for (int i = 0; i < ord - 1; ++i) { exp[i + 1] = bt_pp.mul(exp[i], root); }
            for (int i = 0; i < ord; ++i) log[exp[i]] = i;
        }
        log_fact.assign(pp, 0);
        for (int i = 1; i < pp; ++i) {
            log_fact[i] = log_fact[i - 1] + log[i];
            if (log_fact[i] >= ord) log_fact[i] -= ord;
        }
    }
    int C(long long n, long long i) {
        assert(n >= 0);
        if (i < 0 || i > n) return 0;
        long long a = i, b = n - i;
        if (pp == 2) { return ((a & b) == 0 ? 1 : 0); }
        int log = 0, cnt_p = 0, sgn = 0;
        if (e > 1) {
            while (n && cnt_p < e) {
                auto [n1, nr1] = bt_pp.divmod(n);
                auto [a1, ar1] = bt_pp.divmod(a);
                auto [b1, br1] = bt_pp.divmod(b);
                log += log_fact[nr1] - log_fact[ar1] - log_fact[br1];
                if (p > 2) sgn += (n1 & 1) + (a1 & 1) + (b1 & 1);
                else sgn += (((nr1 + 1) & 4) + ((ar1 + 1) & 4) + ((br1 + 1) & 4)) / 4;
                n = bt_p.floor(n), a = bt_p.floor(a), b = bt_p.floor(b);
                cnt_p += n - a - b;
            }
        }
        else {
            while (n && cnt_p < e) {
                auto [n1, nr1] = bt_pp.divmod(n);
                auto [a1, ar1] = bt_pp.divmod(a);
                auto [b1, br1] = bt_pp.divmod(b);
                log += log_fact[nr1] - log_fact[ar1] - log_fact[br1];
                if (p > 2)
                    sgn += (n1 & 1) + (a1 & 1) + (b1 & 1);
                else
                    sgn += ((nr1 + 1) >> 2 & 1) + ((ar1 + 1) >> 2 & 1) + ((br1 + 1) >> 2 & 1);
                n = n1, a = a1, b = b1;
                cnt_p += n - a - b;
            }
        }
        if (cnt_p >= e) return 0;
        log %= ord;
        if (log < 0) log += ord;
        int res = exp[log];
        if (sgn & 1) res = pp - res;
        return bt_pp.mul(power[cnt_p], res);
    }
};

struct Binomial {
    using barrett = internal::barrett;
    int mod;
    std::vector<BinomialPrimePower> BPP;
    std::vector<int> crt_coef;
    barrett bt;
    Binomial(int _mod) : mod(_mod), bt(mod) {
        auto pfacs = pollard_rho(mod);
        std::ranges::sort(pfacs);
        for (int i = 0, j = 0; i < int(pfacs.size()); i = j) {
            int pp = 1;
            while (j < int(pfacs.size()) && pfacs[i] == pfacs[j]) ++j, pp *= pfacs[i];
            BPP.emplace_back(pfacs[i], j - i);
            int other = mod / pp;
            crt_coef.push_back((long long)other * mod_inv(other, pp) % mod);
        }
    }
    int C(long long n, long long k) {
        assert(n >= 0);
        if (k < 0 || k > n) return 0;
        int ans = 0;
        for (int s = 0; s < int(crt_coef.size()); ++s)
            ans = bt.modulo(ans + (unsigned long long)(BPP[s].C(n, k)) * crt_coef[s]);
        return ans;
    }
};
#line 5 "test.cpp"

int main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int t, m;
    cin >> t >> m;
    Binomial binom(m);
    while (t--) {
        ll n, k;
        cin >> n >> k;
        cout << binom.C(n, k) << "\n";
    }
}