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

#line 2 "library/Math/comb.hpp"

template <typename T> T Inv(ll n) {
    static int md;
    static vector<T> buf({0, 1});
    if (md != T::get_mod()) {
        md = T::get_mod();
        buf = vector<T>({0, 1});
    }
    assert(n > 0);
    n %= md;
    while (SZ(buf) <= n) {
        int k = SZ(buf), q = (md + k - 1) / k;
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
#line 6 "sol.cpp"
using Fp = fp<998244353>;
#line 2 "library/Math/dirichlet.hpp"

template <typename T> struct Dir {
    ll n;
    int SQ, sz;
    vector<T> dat;
    Dir() {}
    Dir(ll n) : n(n), SQ(sqrtl(n)), sz(SQ + n / (SQ + 1) + 1), dat(sz) {}
    T &operator[](int i) {
        return dat[i];
    }
    void pref() {
        rep(i, 0, sz - 1) dat[i + 1] += dat[i];
    }
    void diff() {
        rrep(i, 0, sz - 1) dat[i + 1] -= dat[i];
    }
    int idx(ll x) const {
        return (x <= SQ ? x : sz - n / x);
    }
    ll val(int id) const {
        return (id <= SQ ? id : n / (sz - id));
    }
};

template <typename T> Dir<T> mult(ll n, Dir<T> &a, Dir<T> &b) {
    int SQ = sqrtl(n);
    Dir<T> c(n);
    int thre = a.idx(max<int>(1, pow(n, 2. / 3)));
    auto get = [&](Dir<T> &A, Dir<T> &B) -> vector<pair<int, T>> {
        vector<pair<int, T>> ret;
        rep(i, 1, SQ + 1) if (A[i - 1] != A[i]) {
            T x = A[i] - A[i - 1];
            ret.push_back({i, x});
            rrep(j, thre + 1, c.sz) {
                ll k = c.val(j);
                int to = B.idx(k / i);
                if (to < i)
                    break;
                c[j] += x * B[to];
            }
        }
        rep(i, SQ + 1, thre + 1) {
            if (A[i - 1] != A[i]) {
                T x = A[i] - A[i - 1];
                ret.push_back({A.val(i), x});
            }
        }
        return ret;
    };
    auto posA = get(a, b);
    auto posB = get(b, a);
    for (auto &[v, x] : posA) {
        for (auto &[w, y] : posB) {
            int to = c.idx(ll(v) * w);
            if (to > thre)
                break;
            c[c.idx(v * w)] += x * y;
        }
    }
    rep(i, 1, thre + 1) {
        c[i] += c[i - 1];
    }

    rrep(i, thre + 1, c.sz) {
        ll x = c.val(i);
        int j = sqrtl(x);
        c[i] -= a[j] * b[j];
    }

    return c;
}

template <typename T> Dir<T> div(ll n, Dir<T> c, Dir<T> a) {
    T coe = T(1) / a[1];
    for (auto &x : a.dat)
        x *= coe;

    int SQ = sqrtl(n);
    int thre = a.idx(max<int>(1, pow(n, 2. / 3)));
    Dir<T> dc = c, da = a, db(n), b(n);
    dc.diff();
    da.diff();

    db[1] = c[1];
    rep(i, 2, thre + 1) {
        db[i] = dc[i] - da[i] * db[1];
        ll x = a.val(i);
        {
            int j = a.idx(x * x);
            if (j <= thre)
                dc[j] -= da[i] * db[i];
        }
        rep(j, 2, i) {
            int k = a.idx(x * a.val(j));
            if (k > thre)
                break;
            dc[k] -= da[i] * db[j] + da[j] * db[i];
        }
    }
    rep(i, 1, thre + 1) {
        b[i] = db[i] + b[i - 1];
    }
    rep(i, thre + 1, a.sz) {
        b[i] = c[i] - a[i] * b[1];
        ll x = a.val(i);
        {
            int j = a.idx(sqrtl(x));
            b[i] += a[j] * b[j];
        }
        rep(j, 2, i + 1) {
            int k = a.idx(x / j);
            if (j > k)
                break;
            b[i] -= da[j] * b[k] + a[k] * db[j];
        }
    }

    for (auto &x : b.dat)
        x *= coe;
    return b;
}

/**
 * @brief Dirichlet series
 */
#line 8 "sol.cpp"

void solve(int _rot) {
    // print("Case #"+to_string(_rot)+":");
    ll n;
    read(n);

    ll SQ = sqrtl(n);
    while (SQ * SQ + SQ <= n)
        SQ++;
    ll m = n / SQ;
    vector<ll> vs;
    rep(i, 1, m + 1) vs.push_back(n / (n / (i + 1) + 1));
    rrep(i, 1, SQ) vs.push_back(n / i);

    Dir<Fp> a(n), b(n);
    vector<Fp> inp(SZ(vs));
    rep(i, 0, SZ(vs)) read(inp[i].v);
    rrep(i, 0, SZ(vs) - 1) inp[i + 1] -= inp[i];
    rep(i, 0, SZ(vs)) {
        a.dat[a.idx(vs[i])] = inp[i];
    }
    a.pref();
    // rep(i, 0, SZ(vs)) read(inp[i].v);
    // rrep(i, 0, SZ(vs) - 1) inp[i + 1] -= inp[i];
    // rep(i, 0, SZ(vs)) {
    //     b.dat[b.idx(vs[i])] = inp[i];
    // }
    b.dat[1] = 1;
    b.pref();
    auto c = div(n, b, a);

    rep(i, 0, SZ(vs)) {
        Fp x = c[c.idx(vs[i])];
        if (i == SZ(vs) - 1)
            printf("%d\n", x.v);
        else
            printf("%d ", x.v);
    }
}

int main() {
    int t;
    read(t);
    rep(rot, 0, t) solve(rot + 1);
    return 0;
}
