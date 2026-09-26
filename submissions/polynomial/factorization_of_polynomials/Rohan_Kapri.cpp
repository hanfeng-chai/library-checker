#line 1 "library/Template/template.hpp"
#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (int i = (int)(a); i < (int)(b); i++)
#define rrep(i, a, b) for (int i = (int)(b)-1; i >= (int)(a); i--)
#define ALL(v) (v).begin(), (v).end()
#define UNIQUE(v) sort(ALL(v)), (v).erase(unique(ALL(v)), (v).end())
#define SZ(v) (int)v.size()
#define MIN(v) *min_element(ALL(v))
#define MAX(v) *max_element(ALL(v))
#define LB(v, x) int(lower_bound(ALL(v), (x)) - (v).begin())
#define UB(v, x) int(upper_bound(ALL(v), (x)) - (v).begin())

using uint = unsigned int;
using ll = long long int;
using ull = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
const int inf = 0x3fffffff;
const ll INF = 0x1fffffffffffffff;

template <typename T, typename S = T> S SUM(const vector<T> &a) {
    return accumulate(ALL(a), S(0));
}
template <typename S, typename T = S> S POW(S a, T b) {
    S ret = 1, base = a;
    for (;;) {
        if (b & 1)
            ret *= base;
        b >>= 1;
        if (b == 0)
            break;
        base *= base;
    }
    return ret;
}
template <typename T> inline bool chmax(T &a, T b) {
    if (a < b) {
        a = b;
        return 1;
    }
    return 0;
}
template <typename T> inline bool chmin(T &a, T b) {
    if (a > b) {
        a = b;
        return 1;
    }
    return 0;
}
template <typename T, typename U> T ceil(T x, U y) {
    assert(y != 0);
    if (y < 0)
        x = -x, y = -y;
    return (x > 0 ? (x + y - 1) / y : x / y);
}
template <typename T, typename U> T floor(T x, U y) {
    assert(y != 0);
    if (y < 0)
        x = -x, y = -y;
    return (x > 0 ? x / y : (x - y + 1) / y);
}
template <typename T> int popcnt(T x) {
    return __builtin_popcountll(x);
}
template <typename T> int topbit(T x) {
    return (x == 0 ? -1 : 63 - __builtin_clzll(x));
}
template <typename T> int lowbit(T x) {
    return (x == 0 ? -1 : __builtin_ctzll(x));
}

template <class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p) {
    os << "P(" << p.first << ", " << p.second << ")";
    return os;
}
template <typename T> ostream &operator<<(ostream &os, const vector<T> &vec) {
    os << "{";
    for (int i = 0; i < vec.size(); i++) {
        os << vec[i] << (i + 1 == vec.size() ? "" : ", ");
    }
    os << "}";
    return os;
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &map_var) {
    os << "{";
    for (auto itr = map_var.begin(); itr != map_var.end(); itr++) {
        os << "(" << itr->first << ", " << itr->second << ")";
        itr++;
        if (itr != map_var.end())
            os << ", ";
        itr--;
    }
    os << "}";
    return os;
}
template <typename T> ostream &operator<<(ostream &os, const set<T> &set_var) {
    os << "{";
    for (auto itr = set_var.begin(); itr != set_var.end(); itr++) {
        os << *itr;
        ++itr;
        if (itr != set_var.end())
            os << ", ";
        itr--;
    }
    os << "}";
    return os;
}
#ifdef LOCAL
#define debug 1
#define show(...) _show(0, #__VA_ARGS__, __VA_ARGS__)
#else
#define debug 0
#define show(...) true
#endif
template <typename T> void _show(int i, T name) {
    cerr << '\n';
}
template <typename T1, typename T2, typename... T3>
void _show(int i, const T1 &a, const T2 &b, const T3 &...c) {
    for (; a[i] != ',' && a[i] != '\0'; i++)
        cerr << a[i];
    cerr << ":" << b << " ";
    _show(i + 1, a, c...);
}
#line 2 "library/Utility/fastio.hpp"
#include <unistd.h>
namespace fastio {
    static constexpr uint32_t SZ = 1 << 17;
    char ibuf[SZ];
    char obuf[SZ];
    char out[100];
// pointer of ibuf, obuf

    uint32_t pil = 0, pir = 0, por = 0;

    struct Pre {
        char num[10000][4];
        constexpr Pre() : num() {
            for (int i = 0; i < 10000; i++) {
                int n = i;
                for (int j = 3; j >= 0; j--) {
                    num[i][j] = n % 10 | '0';
                    n /= 10;
                }
            }
        }
    } constexpr pre;

    inline void load() {
        memmove(ibuf, ibuf + pil, pir - pil);
        pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
        pil = 0;
        if (pir < SZ)
            ibuf[pir++] = '\n';
    }

    inline void flush() {
        fwrite(obuf, 1, por, stdout);
        por = 0;
    }

    void rd(char &c) {
        do {
            if (pil + 1 > pir)
                load();
            c = ibuf[pil++];
        } while (isspace(c));
    }

    void rd(string &x) {
        x.clear();
        char c;
        do {
            if (pil + 1 > pir)
                load();
            c = ibuf[pil++];
        } while (isspace(c));
        do {
            x += c;
            if (pil == pir)
                load();
            c = ibuf[pil++];
        } while (!isspace(c));
    }

    template <typename T> void rd_real(T &x) {
        string s;
        rd(s);
        x = stod(s);
    }

    template <typename T> void rd_integer(T &x) {
        if (pil + 100 > pir)
            load();
        char c;
        do
            c = ibuf[pil++];
        while (c < '-');
        bool minus = 0;
        if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
            if (c == '-') {
                minus = 1, c = ibuf[pil++];
            }
        }
        x = 0;
        while ('0' <= c) {
            x = x * 10 + (c & 15), c = ibuf[pil++];
        }
        if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
            if (minus)
                x = -x;
        }
    }

    void rd(int &x) {
        rd_integer(x);
    }
    void rd(ll &x) {
        rd_integer(x);
    }
    void rd(i128 &x) {
        rd_integer(x);
    }
    void rd(uint &x) {
        rd_integer(x);
    }
    void rd(ull &x) {
        rd_integer(x);
    }
    void rd(u128 &x) {
        rd_integer(x);
    }
    void rd(double &x) {
        rd_real(x);
    }
    void rd(long double &x) {
        rd_real(x);
    }

    template <class T, class U> void rd(pair<T, U> &p) {
        return rd(p.first), rd(p.second);
    }
    template <size_t N = 0, typename T> void rd_tuple(T &t) {
        if constexpr (N < std::tuple_size<T>::value) {
            auto &x = std::get<N>(t);
            rd(x);
            rd_tuple<N + 1>(t);
        }
    }
    template <class... T> void rd(tuple<T...> &tpl) {
        rd_tuple(tpl);
    }

    template <size_t N = 0, typename T> void rd(array<T, N> &x) {
        for (auto &d : x)
            rd(d);
    }
    template <class T> void rd(vector<T> &x) {
        for (auto &d : x)
            rd(d);
    }

    void read() {}
    template <class H, class... T> void read(H &h, T &...t) {
        rd(h), read(t...);
    }

    void wt(const char c) {
        if (por == SZ)
            flush();
        obuf[por++] = c;
    }
    void wt(const string s) {
        for (char c : s)
            wt(c);
    }
    void wt(const char *s) {
        size_t len = strlen(s);
        for (size_t i = 0; i < len; i++)
            wt(s[i]);
    }

    template <typename T> void wt_integer(T x) {
        if (por > SZ - 100)
            flush();
        if (x < 0) {
            obuf[por++] = '-', x = -x;
        }
        int outi;
        for (outi = 96; x >= 10000; outi -= 4) {
            memcpy(out + outi, pre.num[x % 10000], 4);
            x /= 10000;
        }
        if (x >= 1000) {
            memcpy(obuf + por, pre.num[x], 4);
            por += 4;
        } else if (x >= 100) {
            memcpy(obuf + por, pre.num[x] + 1, 3);
            por += 3;
        } else if (x >= 10) {
            int q = (x * 103) >> 10;
            obuf[por] = q | '0';
            obuf[por + 1] = (x - q * 10) | '0';
            por += 2;
        } else
            obuf[por++] = x | '0';
        memcpy(obuf + por, out + outi + 4, 96 - outi);
        por += 96 - outi;
    }

    template <typename T> void wt_real(T x) {
        ostringstream oss;
        oss << fixed << setprecision(15) << double(x);
        string s = oss.str();
        wt(s);
    }

    void wt(int x) {
        wt_integer(x);
    }
    void wt(ll x) {
        wt_integer(x);
    }
    void wt(i128 x) {
        wt_integer(x);
    }
    void wt(uint x) {
        wt_integer(x);
    }
    void wt(ull x) {
        wt_integer(x);
    }
    void wt(u128 x) {
        wt_integer(x);
    }
    void wt(double x) {
        wt_real(x);
    }
    void wt(long double x) {
        wt_real(x);
    }

    template <class T, class U> void wt(const pair<T, U> val) {
        wt(val.first);
        wt(' ');
        wt(val.second);
    }
    template <size_t N = 0, typename T> void wt_tuple(const T t) {
        if constexpr (N < std::tuple_size<T>::value) {
            if constexpr (N > 0) {
                wt(' ');
            }
            const auto x = std::get<N>(t);
            wt(x);
            wt_tuple<N + 1>(t);
        }
    }
    template <class... T> void wt(tuple<T...> tpl) {
        wt_tuple(tpl);
    }
    template <class T, size_t S> void wt(const array<T, S> val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) {
            if (i)
                wt(' ');
            wt(val[i]);
        }
    }
    template <class T> void wt(const vector<T> val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) {
            if (i)
                wt(' ');
            wt(val[i]);
        }
    }

    void print() {
        wt('\n');
    }
    template <class Head, class... Tail> void print(Head &&head, Tail &&...tail) {
        wt(head);
        if (sizeof...(Tail))
            wt(' ');
        print(forward<Tail>(tail)...);
    }
    void __attribute__((destructor)) _d() {
        flush();
    }
} // namespace fastio

using fastio::flush;
using fastio::print;
using fastio::read;

inline void first(bool i = true) {
    print(i ? "first" : "second");
}
inline void Alice(bool i = true) {
    print(i ? "Alice" : "Bob");
}
inline void Takahashi(bool i = true) {
    print(i ? "Takahashi" : "Aoki");
}
inline void yes(bool i = true) {
    print(i ? "yes" : "no");
}
inline void Yes(bool i = true) {
    print(i ? "Yes" : "No");
}
inline void No() {
    print("No");
}
inline void YES(bool i = true) {
    print(i ? "YES" : "NO");
}
inline void NO() {
    print("NO");
}
inline void Yay(bool i = true) {
    print(i ? "Yay!" : ":(");
}
inline void Possible(bool i = true) {
    print(i ? "Possible" : "Impossible");
}
inline void POSSIBLE(bool i = true) {
    print(i ? "POSSIBLE" : "IMPOSSIBLE");
}

/**
 * @brief Fast IO
 */
#line 2 "library/Utility/random.hpp"

namespace Random {
    mt19937_64 randgen(chrono::steady_clock::now().time_since_epoch().count());
    using u64 = unsigned long long;
    u64 get() {
        return randgen();
    }
    template <typename T> T get(T L) { // [0,L]
        return get() % (L + 1);
    }
    template <typename T> T get(T L, T R) { // [L,R]
        return get(R - L) + L;
    }
    double uniform() {
        return double(get(1000000000)) / 1000000000;
    }
    string str(int n) {
        string ret;
        rep(i, 0, n) ret += get('a', 'z');
        return ret;
    }
    template <typename Iter> void shuffle(Iter first, Iter last) {
        if (first == last)
            return;
        int len = 1;
        for (auto it = first + 1; it != last; it++) {
            len++;
            int j = get(0, len - 1);
            if (j != len - 1)
                iter_swap(it, first + j);
        }
    }
    template <typename T> vector<T> select(int n, T L, T R) { // [L,R]
        if (n * 2 >= R - L + 1) {
            vector<T> ret(R - L + 1);
            iota(ALL(ret), L);
            shuffle(ALL(ret));
            ret.resize(n);
            return ret;
        } else {
            unordered_set<T> used;
            vector<T> ret;
            while (SZ(used) < n) {
                T x = get(L, R);
                if (!used.count(x)) {
                    used.insert(x);
                    ret.push_back(x);
                }
            }
            return ret;
        }
    }

    void relabel(int n, vector<pair<int, int>> &es) {
        shuffle(ALL(es));
        vector<int> ord(n);
        iota(ALL(ord), 0);
        shuffle(ALL(ord));
        for (auto &[u, v] : es)
            u = ord[u], v = ord[v];
    }
    template <bool directed, bool multi, bool self>
    vector<pair<int, int>> genGraph(int n, int m) {
        vector<pair<int, int>> cand, es;
        rep(u, 0, n) rep(v, 0, n) {
                if (!self and u == v)
                    continue;
                if (!directed and u > v)
                    continue;
                cand.push_back({u, v});
            }
        if (m == -1)
            m = get(SZ(cand));
        // chmin(m, SZ(cand));
        vector<int> ord;
        if (multi)
            rep(_, 0, m) ord.push_back(get(SZ(cand) - 1));
        else {
            ord = select(m, 0, SZ(cand) - 1);
        }
        for (auto &i : ord)
            es.push_back(cand[i]);
        relabel(n, es);
        return es;
    }
    vector<pair<int, int>> genTree(int n) {
        vector<pair<int, int>> es;
        rep(i, 1, n) es.push_back({get(i - 1), i});
        relabel(n, es);
        return es;
    }
}; // namespace Random

/**
 * @brief Random
 */
#line 4 "sol.cpp"

#line 2 "library/Math/fastdiv.hpp"

struct FastDiv {
    using u64 = uint64_t;
    using u128 = __uint128_t;
    constexpr FastDiv() : m(), s(), x() {}
    constexpr FastDiv(int _m)
            : m(_m), s(__lg(m - 1)), x(((u128(1) << (s + 64)) + m - 1) / m) {}
    constexpr int get() {
        return m;
    }
    constexpr friend u64 operator/(u64 n, const FastDiv &d) {
        return (u128(n) * d.x >> d.s) >> 64;
    }
    constexpr friend int operator%(u64 n, const FastDiv &d) {
        return n - n / d * d.m;
    }
    constexpr pair<u64, int> divmod(u64 n) const {
        u64 q = n / (*this);
        return {q, n - q * m};
    }
    int m, s;
    u64 x;
};

struct FastDiv64 {
    using u64 = uint64_t;
    using u128 = __uint128_t;
    u128 mod, mh, ml;
    explicit FastDiv64(u64 mod = 1) : mod(mod) {
        u128 m = u128(-1) / mod;
        if (m * mod + mod == u128(0))
            ++m;
        mh = m >> 64;
        ml = m & u64(-1);
    }
    u64 umod() const {
        return mod;
    }
    u64 modulo(u128 x) {
        u128 z = (x & u64(-1)) * ml;
        z = (x & u64(-1)) * mh + (x >> 64) * ml + (z >> 64);
        z = (x >> 64) * mh + (z >> 64);
        x -= z * mod;
        return x < mod ? x : x - mod;
    }
    u64 mul(u64 a, u64 b) {
        return modulo(u128(a) * b);
    }
};

/**
 * @brief Fast Division
 */
#line 2 "library/Math/comb.hpp"

template <typename T> T Inv(ll n) {
    static unsigned md;
    static vector<T> buf({0, 1});
    if (md != T::get_mod()) {
        md = T::get_mod();
        buf = vector<T>({0, 1});
    }
    assert(n > 0);
    n %= md;
    while (SZ(buf) <= n) {
        unsigned k = SZ(buf), q = (md + k - 1) / k;
        buf.push_back(buf[k * q - md] * q);
    }
    return buf[n];
}

template <typename T> T Fact(ll n, bool inv = 0) {
    static int md;
    static vector<T> buf({1, 1}), ibuf({1, 1});
    if (md != T::get_mod()) {
        md = T::get_mod();
        buf = ibuf = vector<T>({1, 1});
    }
    assert(n >= 0 and n < md);
    while (SZ(buf) <= n) {
        buf.push_back(buf.back() * SZ(buf));
        ibuf.push_back(ibuf.back() * Inv<T>(SZ(ibuf)));
    }
    return inv ? ibuf[n] : buf[n];
}

template <typename T> T nPr(int n, int r, bool inv = 0) {
    if (n < 0 || n < r || r < 0)
        return 0;
    return Fact<T>(n, inv) * Fact<T>(n - r, inv ^ 1);
}
template <typename T> T nCr(int n, int r, bool inv = 0) {
    if (n < 0 || n < r || r < 0)
        return 0;
    return Fact<T>(n, inv) * Fact<T>(r, inv ^ 1) * Fact<T>(n - r, inv ^ 1);
}
// sum = n, r tuples
template <typename T> T nHr(int n, int r, bool inv = 0) {
    return nCr<T>(n + r - 1, r - 1, inv);
}
// [x^n]C(x)^k
template <typename T> T Catalan(int n, int k) {
    if (k == 0)
        return (n == 0 ? 1 : 0);
    return T(k) * Inv<T>(n * 2 + k) * nCr<T>(n * 2 + k, n);
}
// sum = n, a nonzero tuples and b tuples
template <typename T> T choose(int n, int a, int b) {
    if (n == 0)
        return !a;
    return nCr<T>(n + b - 1, a + b - 1);
}

/**
 * @brief Combination
 */
#line 4 "library/Math/dynamic.hpp"

struct Fp {
    using u64 = uint64_t;
    uint v;
    static uint get_mod() {
        return _getmod();
    }
    static void set_mod(uint _m) {
        bar = FastDiv(_m);
    }
    Fp inv() const {
        int tmp, a = v, b = get_mod(), x = 1, y = 0;
        while (b) {
            tmp = a / b, a -= tmp * b;
            swap(a, b);
            x -= tmp * y;
            swap(x, y);
        }
        if (x < 0) {
            x += get_mod();
        }
        return x;
    }
    Fp() : v(0) {}
    Fp(ll x) {
        v = (x % get_mod() + get_mod()) % get_mod();
    }
    Fp operator-() const {
        return Fp() - *this;
    }
    Fp pow(ll t) {
        assert(t >= 0);
        Fp res = 1, b = *this;
        while (t) {
            if (t & 1)
                res *= b;
            b *= b;
            t >>= 1;
        }
        return res;
    }
    Fp &operator+=(const Fp &x) {
        v += x.v;
        if (v >= get_mod())
            v -= get_mod();
        return *this;
    }
    Fp &operator-=(const Fp &x) {
        v += get_mod() - x.v;
        if (v >= get_mod())
            v -= get_mod();
        return *this;
    }
    Fp &operator*=(const Fp &x) {
        v = (u64(v) * x.v) % bar;
        return *this;
    }
    Fp &operator/=(const Fp &x) {
        (*this) *= x.inv();
        return *this;
    }
    Fp operator+(const Fp &x) const {
        return Fp(*this) += x;
    }
    Fp operator-(const Fp &x) const {
        return Fp(*this) -= x;
    }
    Fp operator*(const Fp &x) const {
        return Fp(*this) *= x;
    }
    Fp operator/(const Fp &x) const {
        return Fp(*this) /= x;
    }
    bool operator==(const Fp &x) const {
        return v == x.v;
    }
    bool operator!=(const Fp &x) const {
        return v != x.v;
    }
    bool operator<(const Fp &x) const {
        return v < x.v;
    }
    friend istream &operator>>(istream &is, Fp &x) {
        return is >> x.v;
    }
    friend ostream &operator<<(ostream &os, const Fp &x) {
        return os << x.v;
    }

private:
    static FastDiv bar;
    static uint _getmod() {
        return bar.get();
    }
};
FastDiv Fp::bar(998244353);

void rd(Fp &x) {
    fastio::rd(x.v);
}
void wt(Fp x) {
    fastio::wt(x.v);
}

/**
 * @brief Dynamic Modint
 */
#line 2 "library/Math/miller.hpp"

struct m64 {
    using i64 = int64_t;
    using u64 = uint64_t;
    using u128 = __uint128_t;

    static u64 mod;
    static u64 r;
    static u64 n2;

    static u64 get_r() {
        u64 ret = mod;
        rep(_,0,5) ret *= 2 - mod * ret;
        return ret;
    }

    static void set_mod(u64 m) {
        assert(m < (1LL << 62));
        assert((m & 1) == 1);
        mod = m;
        n2 = -u128(m) % m;
        r = get_r();
        assert(r * mod == 1);
    }
    static u64 get_mod() { return mod; }

    u64 a;
    m64() : a(0) {}
    m64(const int64_t &b) : a(reduce((u128(b) + mod) * n2)){};

    static u64 reduce(const u128 &b) {
        return (b + u128(u64(b) * u64(-r)) * mod) >> 64;
    }
    u64 get() const {
        u64 ret = reduce(a);
        return ret >= mod ? ret - mod : ret;
    }
    m64 &operator*=(const m64 &b) {
        a = reduce(u128(a) * b.a);
        return *this;
    }
    m64 operator*(const m64 &b) const { return m64(*this) *= b; }
    bool operator==(const m64 &b) const {
        return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
    }
    bool operator!=(const m64 &b) const {
        return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
    }
    m64 pow(u128 n) const {
        m64 ret(1), mul(*this);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
};
typename m64::u64 m64::mod, m64::r, m64::n2;

bool Miller(ll n){
    if(n<2 or (n&1)==0)return (n==2);
    m64::set_mod(n);
    ll d=n-1; while((d&1)==0)d>>=1;
    vector<ll> seeds;
    if(n<(1<<30))seeds={2, 7, 61};
    else seeds={2, 325, 9375, 28178, 450775, 9780504};
    for(auto& x:seeds){
        if(n<=x)break;
        ll t=d;
        m64 y=m64(x).pow(t);
        while(t!=n-1 and y!=1 and y!=n-1){
            y*=y;
            t<<=1;
        }
        if(y!=n-1 and (t&1)==0)return 0;
    } return 1;
}

/**
 * @brief Miller-Rabin
 */
#line 4 "library/Math/pollard.hpp"

vector<ll> Pollard(ll n) {
    if (n <= 1)
        return {};
    if (Miller(n))
        return {n};
    if ((n & 1) == 0) {
        vector<ll> v = Pollard(n >> 1);
        v.push_back(2);
        return v;
    }
    for (ll x = 2, y = 2, d;;) {
        ll c = Random::get(2LL, n - 1);
        do {
            x = (__int128_t(x) * x + c) % n;
            y = (__int128_t(y) * y + c) % n;
            y = (__int128_t(y) * y + c) % n;
            d = __gcd(x - y + n, n);
        } while (d == 1);
        if (d < n) {
            vector<ll> lb = Pollard(d), rb = Pollard(n / d);
            lb.insert(lb.end(), ALL(rb));
            return lb;
        }
    }
}

vector<pair<ll, int>> Pollard2(ll n) {
    auto ps = Pollard(n);
    sort(ALL(ps));
    using P = pair<ll, int>;
    vector<P> pes;
    for (auto &p : ps) {
        if (pes.empty() or pes.back().first != p) {
            pes.push_back({p, 1});
        } else {
            pes.back().second++;
        }
    }
    return pes;
}

vector<ll> EnumDivisors(ll n) {
    auto pes = Pollard2(n);
    vector<ll> ret;
    auto rec = [&](auto &rec, int id, ll d) -> void {
        if (id == SZ(pes)) {
            ret.push_back(d);
            return;
        }
        rec(rec, id + 1, d);
        rep(e, 0, pes[id].second) {
            d *= pes[id].first;
            rec(rec, id + 1, d);
        }
    };
    rec(rec, 0, 1);
    sort(ALL(ret));
    return ret;
}

/**
 * @brief Pollard-Rho
 */
#line 4 "library/Math/primitive.hpp"

ll mpow(ll a, ll t, ll m) {
    ll res = 1;
    FastDiv64 im(m);
    while (t) {
        if (t & 1)
            res = im.modulo(__int128_t(res) * a);
        a = im.modulo(__int128_t(a) * a);
        t >>= 1;
    }
    return res % m;
}
ll minv(ll a, ll m) {
    ll b = m, u = 1, v = 0;
    while (b) {
        ll t = a / b;
        a -= t * b;
        swap(a, b);
        u -= t * v;
        swap(u, v);
    }
    u = (u % m + m) % m;
    return u;
}
ll getPrimitiveRoot(ll p) {
    vector<ll> ps = Pollard(p - 1);
    sort(ALL(ps));
    rep(x, 1, inf) {
        for (auto &q : ps) {
            if (mpow(x, (p - 1) / q, p) == 1)
                goto fail;
        }
        return x;
        fail:;
    }
    assert(0);
}
template <typename T> T extgcd(T a, T b, T &p, T &q) {
    if (b == 0) {
        p = 1;
        q = 0;
        return a;
    }
    T d = extgcd(b, a % b, q, p);
    q -= a / b * p;
    return d;
}
template <typename T> pair<T, T> crt(vector<T> vs, vector<T> ms) {
    T V = vs[0], M = ms[0];
    rep(i, 1, vs.size()) {
        T p, q, v = vs[i], m = ms[i];
        if (M < m)
            swap(M, m), swap(V, v);
        T d = extgcd(M, m, p, q);
        if ((v - V) % d != 0)
            return {0, -1};
        T md = m / d, tmp = (v - V) / d % md * p % md;
        V += M * tmp;
        M *= md;
    }
    V = (V % M + M) % M;
    return {V, M};
}
ll garner(vector<ll> vs, vector<ll> p, int mod) {
    int sz = SZ(vs);
    vector<ll> kp(sz + 1), rmul(sz + 1, 1);
    p.push_back(mod);
    rep(i, 0, sz) {
        ll x = (vs[i] - kp[i]) * minv(rmul[i], p[i]) % p[i];
        if (x < 0)
            x += p[i];
        rep(j, i + 1, sz + 1) {
            kp[j] += rmul[j] * x;
            kp[j] %= p[j];
            rmul[j] *= p[i];
            rmul[j] %= p[j];
        }
    }
    return kp.back();
}

ll ModLog(ll a, ll b, ll p) {
    ll g = 1;
    for (ll t = p; t; t >>= 1)
        g = g * a % p;
    g = __gcd(g, p);
    ll t = 1, c = 0;
    for (; t % g; c++) {
        if (t == b)
            return c;
        t = t * a % p;
    }
    if (b % g)
        return -1;
    t /= g, b /= g;
    ll n = p / g, h = 0, gs = 1;
    for (; h * h < n; h++)
        gs = gs * a % n;
    unordered_map<ll, ll> bs;
    for (ll s = 0, e = b; s < h; bs[e] = ++s)
        e = e * a % n;
    for (ll s = 0, e = t; s < n;) {
        e = e * gs % n, s += h;
        if (bs.count(e)) {
            return c + s - bs[e];
        }
    }
    return -1;
}

ll TonelliShanks(ll a, ll p) {
    a %= p;
    if (a == 0)
        return 0;
    if (p == 2)
        return a;
    if (mpow(a, (p - 1) >> 1, p) != 1)
        return -1;
    ll b = 1;
    while (mpow(b, (p - 1) >> 1, p) == 1)
        b = Random::get(1LL, p - 1);

    ll q = p - 1, k = 0;
    while (q % 2 == 0) {
        q >>= 1;
        k++;
    }
    ll x = mpow(a, (q + 1) >> 1, p);
    b = mpow(b, q, p);
    k -= 2;
    while (mpow(x, 2, p) != a) {
        ll err = minv(a, p) * mpow(x, 2, p) % p;
        if (mpow(err, 1 << k, p) != 1)
            x = x * b % p;
        b = mpow(b, 2, p);
        k--;
    }
    return x;
}

ll mod_root(ll k, ll a, ll m) {
    if (a == 0)
        return k ? 0 : -1;
    if (m == 2)
        return a & 1;
    k %= m - 1;
    ll g = gcd(k, m - 1);
    if (mpow(a, (m - 1) / g, m) != 1)
        return -1;
    a = mpow(a, minv(k / g, (m - 1) / g), m);
    FastDiv64 im(m);

    auto _subroot = [&](ll p, int e, ll a) -> ll { // x^(p^e)==a(mod m)
        ll q = m - 1;
        int s = 0;
        while (q % p == 0) {
            q /= p;
            s++;
        }
        int d = s - e;
        ll pe = mpow(p, e, m),
                res = mpow(a, ((pe - 1) * minv(q, pe) % pe * q + 1) / pe, m), c = 1;
        while (mpow(c, (m - 1) / p, m) == 1)
            c++;
        c = mpow(c, q, m);
        map<ll, ll> mp;
        ll v = 1, block = sqrt(d * p) + 1,
                bs = mpow(c, mpow(p, s - 1, m - 1) * block % (m - 1), m);
        rep(i, 0, block + 1) mp[v] = i, v = im.modulo(i128(v) * bs);
        ll gs = minv(mpow(c, mpow(p, s - 1, m - 1), m), m);
        rep(i, 0, d) {
            ll err = im.modulo(i128(a) * minv(mpow(res, pe, m), m));
            ll pos = mpow(err, mpow(p, d - 1 - i, m - 1), m);
            rep(j, 0, block + 1) {
                if (mp.count(pos)) {
                    res = im.modulo(i128(res) *
                                    mpow(c,
                                         (block * mp[pos] + j) *
                                         mpow(p, i, m - 1) % (m - 1),
                                         m));
                    break;
                }
                pos = im.modulo(i128(pos) * gs);
            }
        }
        return res;
    };

    for (ll d = 2; d * d <= g; d++)
        if (g % d == 0) {
            int sz = 0;
            while (g % d == 0) {
                g /= d;
                sz++;
            }
            a = _subroot(d, sz, a);
        }
    if (g > 1)
        a = _subroot(g, 1, a);
    return a;
}

ull floor_root(ull a, ull k) {
    if (a <= 1 or k == 1)
        return a;
    if (k >= 64)
        return 1;
    if (k == 2)
        return sqrtl(a);
    constexpr ull LIM = -1;
    if (a == LIM)
        a--;
    auto mul = [&](ull &x, const ull &y) {
        if (x <= LIM / y)
            x *= y;
        else
            x = LIM;
    };
    auto pw = [&](ull x, ull t) -> ull {
        ull y = 1;
        while (t) {
            if (t & 1)
                mul(y, x);
            mul(x, x);
            t >>= 1;
        }
        return y;
    };
    ull ret = (k == 3 ? cbrt(a) - 1 : pow(a, nextafter(1 / double(k), 0)));
    while (pw(ret + 1, k) <= a)
        ret++;
    return ret;
}

/**
 * @brief Primitive Function
 */
#line 3 "library/Convolution/ntt.hpp"

template <typename T> struct NTT {
    static constexpr int rank2 = __builtin_ctzll(T::get_mod() - 1);
    std::array<T, rank2 + 1> root;  // root[i]^(2^i) == 1
    std::array<T, rank2 + 1> iroot; // root[i] * iroot[i] == 1

    std::array<T, std::max(0, rank2 - 2 + 1)> rate2;
    std::array<T, std::max(0, rank2 - 2 + 1)> irate2;

    std::array<T, std::max(0, rank2 - 3 + 1)> rate3;
    std::array<T, std::max(0, rank2 - 3 + 1)> irate3;

    NTT() {
        T g = getPrimitiveRoot(T::get_mod());
        root[rank2] = g.pow((T::get_mod() - 1) >> rank2);
        iroot[rank2] = root[rank2].inv();
        for (int i = rank2 - 1; i >= 0; i--) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }

        {
            T prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 2; i++) {
                rate2[i] = root[i + 2] * prod;
                irate2[i] = iroot[i + 2] * iprod;
                prod *= iroot[i + 2];
                iprod *= root[i + 2];
            }
        }
        {
            T prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 3; i++) {
                rate3[i] = root[i + 3] * prod;
                irate3[i] = iroot[i + 3] * iprod;
                prod *= iroot[i + 3];
                iprod *= root[i + 3];
            }
        }
    }

    void ntt(std::vector<T> &a, bool type = 0) {
        int n = int(a.size());
        int h = __builtin_ctzll((unsigned int)n);
        a.resize(1 << h);

        if (type) {
            int len = h; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
            while (len) {
                if (len == 1) {
                    int p = 1 << (h - len);
                    T irot = 1;
                    for (int s = 0; s < (1 << (len - 1)); s++) {
                        int offset = s << (h - len + 1);
                        for (int i = 0; i < p; i++) {
                            auto l = a[i + offset];
                            auto r = a[i + offset + p];
                            a[i + offset] = l + r;
                            a[i + offset + p] =
                                    ((unsigned long long)(T::get_mod() + l.v -
                                                          r.v) *
                                     irot.v) %
                                    T::get_mod();
                        }
                        if (s + 1 != (1 << (len - 1)))
                            irot *= irate2[__builtin_ctzll(~(unsigned int)(s))];
                    }
                    len--;
                } else {
                    // 4-base
                    int p = 1 << (h - len);
                    T irot = 1, iimag = iroot[2];
                    for (int s = 0; s < (1 << (len - 2)); s++) {
                        T irot2 = irot * irot;
                        T irot3 = irot2 * irot;
                        int offset = s << (h - len + 2);
                        for (int i = 0; i < p; i++) {
                            auto a0 = 1ULL * a[i + offset + 0 * p].v;
                            auto a1 = 1ULL * a[i + offset + 1 * p].v;
                            auto a2 = 1ULL * a[i + offset + 2 * p].v;
                            auto a3 = 1ULL * a[i + offset + 3 * p].v;

                            auto a2na3iimag =
                                    1ULL * T((T::get_mod() + a2 - a3) * iimag.v).v;

                            a[i + offset] = (a0 + a1 + a2 + a3) % T::get_mod();
                            a[i + offset + 1 * p] =
                                    (a0 + (T::get_mod() - a1) + a2na3iimag) *
                                    irot.v % T::get_mod();
                            a[i + offset + 2 * p] =
                                    (a0 + a1 + (T::get_mod() - a2) +
                                     (T::get_mod() - a3)) *
                                    irot2.v % T::get_mod();
                            a[i + offset + 3 * p] =
                                    (a0 + (T::get_mod() - a1) +
                                     (T::get_mod() - a2na3iimag)) *
                                    irot3.v % T::get_mod();
                        }
                        if (s + 1 != (1 << (len - 2)))
                            irot *= irate3[__builtin_ctzll(~(unsigned int)(s))];
                    }
                    len -= 2;
                }
            }
            T e = T(n).inv();
            for (auto &x : a)
                x *= e;
        } else {
            int len = 0; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
            while (len < h) {
                if (h - len == 1) {
                    int p = 1 << (h - len - 1);
                    T rot = 1;
                    for (int s = 0; s < (1 << len); s++) {
                        int offset = s << (h - len);
                        for (int i = 0; i < p; i++) {
                            auto l = a[i + offset];
                            auto r = a[i + offset + p] * rot;
                            a[i + offset] = l + r;
                            a[i + offset + p] = l - r;
                        }
                        if (s + 1 != (1 << len))
                            rot *= rate2[__builtin_ctzll(~(unsigned int)(s))];
                    }
                    len++;
                } else {
                    // 4-base
                    int p = 1 << (h - len - 2);
                    T rot = 1, imag = root[2];
                    for (int s = 0; s < (1 << len); s++) {
                        T rot2 = rot * rot;
                        T rot3 = rot2 * rot;
                        int offset = s << (h - len);
                        for (int i = 0; i < p; i++) {
                            auto mod2 = 1ULL * T::get_mod() * T::get_mod();
                            auto a0 = 1ULL * a[i + offset].v;
                            auto a1 = 1ULL * a[i + offset + p].v * rot.v;
                            auto a2 = 1ULL * a[i + offset + 2 * p].v * rot2.v;
                            auto a3 = 1ULL * a[i + offset + 3 * p].v * rot3.v;
                            auto a1na3imag =
                                    1ULL * T(a1 + mod2 - a3).v * imag.v;
                            auto na2 = mod2 - a2;
                            a[i + offset] = (a0 + a2 + a1 + a3) % T::get_mod();
                            a[i + offset + 1 * p] =
                                    (a0 + a2 + (2 * mod2 - (a1 + a3))) %
                                    T::get_mod();
                            a[i + offset + 2 * p] =
                                    (a0 + na2 + a1na3imag) % T::get_mod();
                            a[i + offset + 3 * p] =
                                    (a0 + na2 + (mod2 - a1na3imag)) % T::get_mod();
                        }
                        if (s + 1 != (1 << len))
                            rot *= rate3[__builtin_ctzll(~(unsigned int)(s))];
                    }
                    len += 2;
                }
            }
        }
    }
    vector<T> mult(const vector<T> &a, const vector<T> &b) {
        if (a.empty() or b.empty())
            return vector<T>();
        int as = a.size(), bs = b.size();
        int n = as + bs - 1;
        assert(n <= (1 << rank2));
        if (as <= 30 or bs <= 30) {
            if (as > 30)
                return mult(b, a);
            vector<T> res(n);
            rep(i, 0, as) rep(j, 0, bs) res[i + j] += a[i] * b[j];
            return res;
        }
        int m = 1;
        while (m < n)
            m <<= 1;
        vector<T> res(m);
        rep(i, 0, as) res[i] = a[i];
        ntt(res);
        if (a == b)
            rep(i, 0, m) res[i] *= res[i];
        else {
            vector<T> c(m);
            rep(i, 0, bs) c[i] = b[i];
            ntt(c);
            rep(i, 0, m) res[i] *= c[i];
        }
        ntt(res, 1);
        res.resize(n);
        return res;
    }
};

/**
 * @brief Number Theoretic Transform
 */
#line 3 "library/Math/modint.hpp"

template <unsigned mod = 1000000007> struct fp {
    static_assert(mod < uint(1) << 31);
    unsigned v;
    static constexpr int get_mod() {
        return mod;
    }
    constexpr unsigned inv() const {
        assert(v != 0);
        int x = v, y = mod, p = 1, q = 0, t = 0, tmp = 0;
        while (y > 0) {
            t = x / y;
            x -= t * y, p -= t * q;
            tmp = x, x = y, y = tmp;
            tmp = p, p = q, q = tmp;
        }
        if (p < 0)
            p += mod;
        return p;
    }
    constexpr fp(ll x = 0) : v(x >= 0 ? x % mod : (mod - (-x) % mod) % mod) {}
    fp operator-() const {
        return fp() - *this;
    }
    fp pow(ull t) {
        fp res = 1, b = *this;
        while (t) {
            if (t & 1)
                res *= b;
            b *= b;
            t >>= 1;
        }
        return res;
    }
    fp &operator+=(const fp &x) {
        if ((v += x.v) >= mod)
            v -= mod;
        return *this;
    }
    fp &operator-=(const fp &x) {
        if ((v += mod - x.v) >= mod)
            v -= mod;
        return *this;
    }
    fp &operator*=(const fp &x) {
        v = ull(v) * x.v % mod;
        return *this;
    }
    fp &operator/=(const fp &x) {
        if (x.v < 15000000) {
            return *this *= Inv<fp>(x.v);
        }
        v = ull(v) * x.inv() % mod;
        return *this;
    }
    fp operator+(const fp &x) const {
        return fp(*this) += x;
    }
    fp operator-(const fp &x) const {
        return fp(*this) -= x;
    }
    fp operator*(const fp &x) const {
        return fp(*this) *= x;
    }
    fp operator/(const fp &x) const {
        return fp(*this) /= x;
    }
    bool operator==(const fp &x) const {
        return v == x.v;
    }
    bool operator!=(const fp &x) const {
        return v != x.v;
    }
    friend istream &operator>>(istream &is, fp &x) {
        return is >> x.v;
    }
    friend ostream &operator<<(ostream &os, const fp &x) {
        return os << x.v;
    }
};

template <unsigned mod> void rd(fp<mod> &x) {
    fastio::rd(x.v);
}
template <unsigned mod> void wt(fp<mod> x) {
    fastio::wt(x.v);
}

/**
 * @brief Modint
 */
#line 4 "library/Convolution/arbitrary.hpp"

using M1 = fp<469762049>;  // 2^26
using M2 = fp<1811939329>; // 2^26
using M3 = fp<2013265921>; // 2^27
NTT<M1> N1;
NTT<M2> N2;
NTT<M3> N3;
constexpr uint r_12 = M2(M1::get_mod()).inv();
constexpr uint r_13 = M3(M1::get_mod()).inv();
constexpr uint r_23 = M3(M2::get_mod()).inv();
constexpr uint r_1323 = M3(ll(r_13) * r_23).v;
constexpr ll w1 = M1::get_mod();
constexpr ll w2 = ll(w1) * M2::get_mod();
static constexpr ll ARBITRARY_NAIVE_WORK = 1LL << 15;
static constexpr ll POLY_DIV_NAIVE_WORK = 1LL << 23;
static constexpr int POLY_GCD_NAIVE_DEG = 16384;

template <typename T> vector<T> ArbitraryMultll(vector<ll> a, vector<ll> b) {
    if (a.empty() or b.empty())
        return {};

    int n = a.size() + b.size() - 1;
    vector<T> res(n);

    ll ami = MIN(a), bmi = MIN(b);
    for (auto &x : a)
        x -= ami;
    for (auto &x : b)
        x -= bmi;

    vector<ll> ruia(SZ(a) + 1), ruib(SZ(b) + 1);
    rep(i, 0, SZ(a)) ruia[i + 1] = ruia[i] + a[i];
    rep(i, 0, SZ(b)) ruib[i + 1] = ruib[i] + b[i];

    rep(k, 0, SZ(res)) {
        ll l = max(0, k - SZ(b) + 1), r = min(SZ(a), k + 1);
        res[k] += ll(r - l) * ami * bmi;
        res[k] += ami * (ruib[k - l + 1] - ruib[k - r + 1]);
        res[k] += bmi * (ruia[r] - ruia[l]);
    }

    vector<ll> vals[3];
    vector<M1> a1(ALL(a)), b1(ALL(b)), c1 = N1.mult(a1, b1);
    vector<M2> a2(ALL(a)), b2(ALL(b)), c2 = N2.mult(a2, b2);
    vector<M3> a3(ALL(a)), b3(ALL(b)), c3 = N3.mult(a3, b3);

    for (M1 x : c1)
        vals[0].push_back(x.v);
    for (M2 x : c2)
        vals[1].push_back(x.v);
    for (M3 x : c3)
        vals[2].push_back(x.v);

    rep(i, 0, n) {
        ll p = vals[0][i];
        ll q = (vals[1][i] + M2::get_mod() - p) * r_12 % M2::get_mod();
        ll r = ((vals[2][i] + M3::get_mod() - p) * r_1323 +
                (M3::get_mod() - q) * r_23) %
               M3::get_mod();
        res[i] += T(r) * w2 + T(q) * w1 + p;
    }
    return res;
}

template <typename T>
vector<T> ArbitraryMult(const vector<T> &a, const vector<T> &b) {
    if (a.empty() or b.empty())
        return {};

    if (1LL * a.size() * b.size() <= ARBITRARY_NAIVE_WORK) {
        vector<T> res(a.size() + b.size() - 1);
        rep(i, 0, a.size()) rep(j, 0, b.size())
                res[i + j] += a[i] * b[j];
        return res;
    }

    vector<ll> A(a.size()), B(b.size());
    rep(i, 0, a.size()) A[i] = a[i].v;
    rep(i, 0, b.size()) B[i] = b[i].v;
    return ArbitraryMultll<T>(move(A), move(B));
}

/**
 * @brief Arbitrary Mod Convolution
 */
#line 2 "library/FPS/arbitraryfps.hpp"

template <typename T> struct Poly : vector<T> {
    Poly(int n = 0) {
        this->assign(n, T());
    }
    Poly(const initializer_list<T> f) : vector<T>::vector(f) {}
    Poly(const vector<T> &f) {
        this->assign(ALL(f));
    }
    int deg() const {
        return this->size() - 1;
    }
    T eval(const T &x) {
        T res;
        for (int i = this->size() - 1; i >= 0; i--)
            res *= x, res += this->at(i);
        return res;
    }
    Poly rev() const {
        Poly res = *this;
        reverse(ALL(res));
        return res;
    }
    void shrink() {
        while (!this->empty() and this->back() == 0)
            this->pop_back();
    }
    Poly operator>>(ll sz) const {
        if ((int)this->size() <= sz)
            return {};
        Poly ret(*this);
        ret.erase(ret.begin(), ret.begin() + sz);
        return ret;
    }
    Poly operator<<(ll sz) const {
        Poly ret(*this);
        ret.insert(ret.begin(), sz, T(0));
        return ret;
    }
    Poly inv() const {
        assert(this->front() != 0);
        const int n = this->size();
        Poly res(1);
        res.front() = T(1) / this->front();
        for (int k = 1; k < n; k <<= 1) {
            Poly g = res, h = *this;
            h.resize(k * 2);
            res.resize(k * 2);
            g = (g.square() * h);
            g.resize(k * 2);
            rep(i, k, min(k * 2, n)) res[i] -= g[i];
        }
        res.resize(n);
        return res;
    }
    Poly square() const {
        return Poly(mult(*this, *this));
    }
    Poly &div_naive(const Poly &g) {
        int n = this->size(), m = g.size();
        Poly q(n - m + 1);
        T inv = g.back().inv();

        for (int i = n - 1; i >= m - 1; i--) {
            T c = (*this)[i] * inv;
            q[i - m + 1] = c;
            if (c == T())
                continue;
            rep(j, 0, m - 1)
                (*this)[i - m + 1 + j] -= c * g[j];
        }

        *this = move(q);
        shrink();
        return *this;
    }
    Poly &mod_naive(const Poly &g) {
        int n = this->size(), m = g.size();
        if (n < m) {
            shrink();
            return *this;
        }

        T inv = g.back().inv();
        for (int i = n - 1; i >= m - 1; i--) {
            T c = (*this)[i] * inv;
            if (c == T())
                continue;
            rep(j, 0, m - 1)
                (*this)[i - m + 1 + j] -= c * g[j];
        }

        this->resize(m - 1);
        shrink();
        return *this;
    }
    Poly operator-() const {
        return Poly() - *this;
    }
    Poly operator+(const Poly &g) const {
        return Poly(*this) += g;
    }
    Poly operator+(const T &g) const {
        return Poly(*this) += g;
    }
    Poly operator-(const Poly &g) const {
        return Poly(*this) -= g;
    }
    Poly operator-(const T &g) const {
        return Poly(*this) -= g;
    }
    Poly operator*(const Poly &g) const {
        return Poly(*this) *= g;
    }
    Poly operator*(const T &g) const {
        return Poly(*this) *= g;
    }
    Poly operator/(const Poly &g) const {
        return Poly(*this) /= g;
    }
    Poly operator%(const Poly &g) const {
        return Poly(*this) %= g;
    }
    pair<Poly, Poly> divmod(const Poly &g) const {
        Poly q = *this / g, r = *this - g * q;
        r.shrink();
        return {q, r};
    }
    Poly &operator+=(const Poly &g) {
        if (g.size() > this->size())
            this->resize(g.size());
        rep(i, 0, g.size()) {
            (*this)[i] += g[i];
        }
        return *this;
    }
    Poly &operator+=(const T &g) {
        if (this->empty())
            this->push_back(0);
        (*this)[0] += g;
        return *this;
    }
    Poly &operator-=(const Poly &g) {
        if (g.size() > this->size())
            this->resize(g.size());
        rep(i, 0, g.size()) {
            (*this)[i] -= g[i];
        }
        return *this;
    }
    Poly &operator-=(const T &g) {
        if (this->empty())
            this->push_back(0);
        (*this)[0] -= g;
        return *this;
    }
    Poly &operator*=(const Poly &g) {
        *this = mult(*this, g);
        return *this;
    }
    Poly &operator*=(const T &g) {
        rep(i, 0, this->size())(*this)[i] *= g;
        return *this;
    }
    Poly &operator/=(const Poly &g) {
        if (g.size() > this->size()) {
            this->clear();
            return *this;
        }

        int n = this->size() - g.size() + 1;
        if (1LL * n * g.size() <= POLY_DIV_NAIVE_WORK)
            return div_naive(g);

        Poly g2 = g;
        reverse(ALL(*this));
        reverse(ALL(g2));
        this->resize(n);
        g2.resize(n);
        *this *= g2.inv();
        this->resize(n);
        reverse(ALL(*this));
        shrink();
        return *this;
    }

    Poly &operator%=(const Poly &g) {
        if (g.size() > this->size()) {
            shrink();
            return *this;
        }

        int n = this->size() - g.size() + 1;
        if (1LL * n * g.size() <= POLY_DIV_NAIVE_WORK)
            return mod_naive(g);

        *this -= *this / g * g;
        this->resize(g.size() - 1);
        shrink();
        return *this;
    }
    Poly diff() const {
        Poly res(this->size() - 1);
        rep(i, 0, res.size()) res[i] = (*this)[i + 1] * (i + 1);
        return res;
    }
    Poly inte() const {
        Poly res(this->size() + 1);
        for (int i = res.size() - 1; i; i--)
            res[i] = (*this)[i - 1] / i;
        return res;
    }
    Poly log() const {
        assert(this->front() == 1);
        const int n = this->size();
        Poly res = diff() * inv();
        res = res.inte();
        res.resize(n);
        return res;
    }
    Poly exp() const {
        assert(this->front() == 0);
        const int n = this->size();
        Poly res(1), g(1);
        res.front() = g.front() = 1;
        for (int k = 1; k < n; k <<= 1) {
            g = (g + g - g.square() * res);
            g.resize(k);
            Poly q = *this;
            q.resize(k);
            q = q.diff();
            Poly w = (q + g * (res.diff() - res * q)), t = *this;
            w.resize(k * 2 - 1);
            t.resize(k * 2);
            res = (res + res * (t - w.inte()));
            res.resize(k * 2);
        }
        res.resize(n);
        return res;
    }
    Poly shift(const int &c) const {
        const int n = this->size();
        Poly res = *this, g(n);
        g[0] = 1;
        rep(i, 1, n) g[i] = g[i - 1] * c / i;
        vector<T> fact(n, 1);
        rep(i, 0, n) {
            if (i)
                fact[i] = fact[i - 1] * i;
            res[i] *= fact[i];
        }
        res = res.rev();
        res *= g;
        res.resize(n);
        res = res.rev();
        rep(i, 0, n) res[i] /= fact[i];
        return res;
    }
    Poly pow(ll t) {
        if (t == 0) {
            Poly res(this->size());
            res[0] = 1;
            return res;
        }
        int n = this->size(), k = 0;
        while (k < n and (*this)[k] == 0)
            k++;
        Poly res(n);
        if (__int128_t(t) * k >= n)
            return res;
        n -= t * k;
        Poly g(n);
        T c = (*this)[k], ic = c.inv();
        rep(i, 0, n) g[i] = (*this)[i + k] * ic;
        g = g.log();
        for (auto &x : g)
            x *= t;
        g = g.exp();
        c = c.pow(t);
        rep(i, 0, n) res[i + t * k] = g[i] * c;
        return res;
    }
    vector<T> mult(const vector<T> &a, const vector<T> &b) const;
};

/**
 * @brief Formal Power Series (Arbitrary mod)
 */
#line 8 "sol.cpp"
template <>
vector<Fp> Poly<Fp>::mult(const vector<Fp> &a, const vector<Fp> &b) const {
    return ArbitraryMult(a, b);
}

#line 2 "library/FPS/halfgcd.hpp"

namespace HalfGCD{
    template<typename T>using P=array<T,2>;
    template<typename T>using Mat=array<T,4>;
    template<typename T>P<T> operator*(const Mat<T>& a,const P<T>& b){
        P<T> ret={a[0]*b[0]+a[1]*b[1],a[2]*b[0]+a[3]*b[1]};
        rep(i,0,2)ret[i].shrink();
        return ret;
    }
    template<typename T>Mat<T> operator*(const Mat<T>& a,const Mat<T>& b){
        Mat<T> ret={a[0]*b[0]+a[1]*b[2],a[0]*b[1]+a[1]*b[3],
                    a[2]*b[0]+a[3]*b[2],a[2]*b[1]+a[3]*b[3]};
        rep(i,0,4)ret[i].shrink();
        return ret;
    }

    template<typename T>Mat<T> HGCD(P<T> a){
        int m=(SZ(a[0])+1)>>1;
        if(SZ(a[1])<=m){
            Mat<T> ret;
            ret[0]={1},ret[3]={1};
            return ret;
        }
        auto R=HGCD(P<T>{a[0]>>m,a[1]>>m});
        a=R*a;
        if(SZ(a[1])<=m)return R;
        Mat<T> Q;
        Q[1]={1},Q[2]={1},Q[3]=-(a[0]/a[1]);
        R=Q*R,a=Q*a;
        if(SZ(a[1])<=m)return R;
        int k=2*m+1-SZ(a[0]);
        auto H=HGCD(P<T>{a[0]>>k,a[1]>>k});
        return H*R;
    }
    template<typename T>Mat<T> InnerGCD(P<T> a){
        if(SZ(a[0])<SZ(a[1])){
            auto M=InnerGCD(P<T>{a[1],a[0]});
            swap(M[0],M[1]);
            swap(M[2],M[3]);
            return M;
        }
        auto m0=HGCD(a);
        a=m0*a;
        if(a[1].empty())return m0;
        Mat<T> Q;
        Q[1]={1},Q[2]={1},Q[3]=-(a[0]/a[1]);
        m0=Q*m0,a=Q*a;
        if(a[1].empty())return m0;
        return InnerGCD(a)*m0;
    }
    template <typename T> T gcd(T a, T b) {
        a.shrink();
        b.shrink();

        if (a.size() < b.size())
            swap(a, b);

        while (!b.empty() and a.deg() > POLY_GCD_NAIVE_DEG) {
            P<T> p({a, b});
            p = HGCD(p) * p;
            a = move(p[0]);
            b = move(p[1]);

            if (b.empty())
                break;

            if (a.size() < b.size())
                swap(a, b);

            a %= b;
            swap(a, b);
        }

        while (!b.empty()) {
            a.mod_naive(b);
            swap(a, b);
        }

        if (!a.empty()) {
            auto coeff = a.back().inv();
            for (auto &x : a)
                x *= coeff;
        }
        return a;
    }
    template<typename T>pair<bool,T> PolyInv(const T& a,const T& b){
        P<T> p({a,b});
        auto M=InnerGCD(p);
        T g=(M*p)[0];
        if(g.size()!=1)return {false,{}};
        P<T> x({T({1}),b});
        auto ret=(M*x)[0]%b;
        auto coeff=g[0].inv();
        for(auto& x:ret)x*=coeff;
        return {true,ret};
    }
}

/**
 * @brief Half GCD
*/
#line 14 "sol.cpp"

namespace FactorizePoly {
    template <typename T> Poly<T> powmod(const Poly<T> &f, ll n, const Poly<T> &g) {
        Poly<T> ret({1}), base = f;
        while (n) {
            if (n & 1) {
                ret *= base;
                ret %= g;
            }
            n >>= 1;
            if (!n)
                break;
            base *= base;
            base %= g;
        }
        return ret;
    }

    template <typename T>
    Poly<T> equalDegreePower(const Poly<T> &a, int degree, const Poly<T> &m) {
        int p = T::get_mod();
        auto x = powmod(a, (p - 1) / 2, m);
        Poly<T> ret({1});
        rep(i, 0, degree) {
            ret *= x;
            ret %= m;
            if (i + 1 < degree)
                x = powmod(x, p, m);
        }
        return ret;
    }

    template <typename T>
    Poly<T> equalDegreeTraceFp2(const Poly<T> &a, int degree, const Poly<T> &m) {
        Poly<T> ret, cur = a;
        cur %= m;
        rep(i, 0, degree) {
            ret += cur;
            if (i + 1 < degree) {
                cur *= cur;
                cur %= m;
            }
        }
        return ret;
    }

    template <typename T> vector<Poly<T>> EDF(Poly<T> &f, int d) {
        int p = T::get_mod();
        if (f.deg() < d)
            return {};
        if (f.deg() == d)
            return {f};

        for (;;) {
            Poly<T> base(SZ(f));
            rep(i, 0, SZ(f)) base[i] = Random::get(p - 1);

            Poly<T> rem;
            if (p == 2) {
                rem = equalDegreeTraceFp2(base, d, f);
            } else {
                rem = equalDegreePower(base, d, f);
                rem[0] -= 1;
            }

            auto g = HalfGCD::gcd(rem, f);
            if (g.deg() > 0 and g != f) {
                auto ret = EDF(g, d);
                auto fg = f / g;
                auto add = EDF(fg, d);
                ret.insert(ret.end(), ALL(add));
                return ret;
            }
        }
    }
    template <typename T> vector<Poly<T>> CantorZassenhaus(Poly<T> &f) {
        auto cur = f;
        vector<Poly<T>> ret;
        ll p = T::get_mod(), d = 1;
        Poly<T> x({0, 1}), xp = x;
        for (;;) {
            if (cur.deg() < d * 2) {
                if (cur.deg())
                    ret.push_back(cur);
                break;
            }
            xp = powmod(xp, p, cur);
            auto rem = xp - x;
            auto g = HalfGCD::gcd(rem, cur);
            auto add = EDF(g, d);
            ret.insert(ret.end(), ALL(add));
            cur /= g;
            d++;
        }
        return ret;
    }
    template <typename T> Poly<T> pth_root_poly(const Poly<T> &f) {
        int p = T::get_mod();
        Poly<T> g(f.deg() / p + 1);
        rep(i, 0, SZ(f)) {
            if (i % p == 0) {
                g[i / p] = f[i];
            } else {
                assert(f[i] == T(0));
            }
        }
        return g;
    }
    template <typename T> vector<Poly<T>> SquarefreeDecomposition(Poly<T> f) {
        int p = T::get_mod();
        f.shrink();
        vector<Poly<T>> ret;
        if (f.deg() <= 0) {
            return ret;
        }
        auto add_part = [&](int mult, Poly<T> g) {
            while (SZ(ret) < mult) {
                ret.push_back(Poly<T>({1}));
            }
            ret[mult - 1] *= g;
        };

        auto df = f.diff();
        df.shrink();
        if (df.empty()) {
            auto h = pth_root_poly(f);
            auto sub = SquarefreeDecomposition(h);
            rep(i, 0, SZ(sub)) add_part((i + 1) * p, sub[i]);
            return ret;
        }

        auto c = HalfGCD::gcd(f, df);
        auto w = f / c;
        int ptr = 1;
        while (w.deg() > 0) {
            auto y = HalfGCD::gcd(w, c);
            auto z = w / y;
            add_part(ptr, z);
            w = y;
            c /= y;
            c.shrink();
            ptr++;
        }

        if (c.deg() > 0) {
            auto h = pth_root_poly(c);
            auto sub = SquarefreeDecomposition(h);
            rep(j, 0, SZ(sub)) add_part((j + 1) * p, sub[j]);
        }
        return ret;
    }
    template <typename T> vector<Poly<T>> run(Poly<T> &f) { // f: monic
        auto dec = SquarefreeDecomposition(f);
        vector<Poly<T>> ret;
        rep(i, 0, SZ(dec)) {
            auto add = CantorZassenhaus(dec[i]);
            rep(_, 0, i + 1) ret.insert(ret.end(), ALL(add));
        }
        return ret;
    }
}; // namespace FactorizePoly

#line 2 "library/FPS/prodofpolys.hpp"

template<typename T>Poly<T> ProdOfPolys(vector<Poly<T>>& fs){
    if(fs.empty())return Poly<T>({T(1)});
    sort(ALL(fs),[&](Poly<T>& a,Poly<T>& b){return a.size()<b.size();});
    deque<Poly<T>> deq;
    for(auto& f:fs)deq.push_back(f);
    while(deq.size()>1){
        deq.push_back(deq[0]*deq[1]);
        deq.pop_front();
        deq.pop_front();
    }
    return deq[0];
}

int main() {
    int n = 100, p = 998244353;
    read(n, p);
    // assert(p != 2);
    Fp::set_mod(p);
    Poly<Fp> f(n + 1);
    read(f);

    auto ret = FactorizePoly::run(f);
    map<Poly<Fp>, int> mp;
    for (auto &g : ret)
        mp[g]++;
    print(SZ(mp));
    for (auto &[g, e] : mp) {
        print(e, SZ(g) - 1, g);
    }
    return 0;
}
