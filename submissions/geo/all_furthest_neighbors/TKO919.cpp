#include <bits/stdc++.h>

using namespace std;

#define rep(i, a, b) for (int i = (int)(a); i < (int)(b); i++)
#define rrep(i, a, b) for (int i = (int)(b) - 1; i >= (int)(a); i--)
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

template <typename T, typename S = T>
S SUM(const vector<T> &a)
{
    return accumulate(ALL(a), S(0));
}
template <typename S, typename T = S>
S POW(S a, T b)
{
    S ret = 1, base = a;
    for (;;)
    {
        if (b & 1)
            ret *= base;
        b >>= 1;
        if (b == 0)
            break;
        base *= base;
    }
    return ret;
}
template <typename T>
inline bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return 1;
    }
    return 0;
}
template <typename T>
inline bool chmin(T &a, T b)
{
    if (a > b)
    {
        a = b;
        return 1;
    }
    return 0;
}
template <typename T, typename U>
T ceil(T x, U y)
{
    assert(y != 0);
    if (y < 0)
        x = -x, y = -y;
    return (x > 0 ? (x + y - 1) / y : x / y);
}
template <typename T, typename U>
T floor(T x, U y)
{
    assert(y != 0);
    if (y < 0)
        x = -x, y = -y;
    return (x > 0 ? x / y : (x - y + 1) / y);
}
template <typename T>
int popcnt(T x)
{
    return __builtin_popcountll(x);
}
template <typename T>
int topbit(T x)
{
    return (x == 0 ? -1 : 63 - __builtin_clzll(x));
}
template <typename T>
int lowbit(T x)
{
    return (x == 0 ? -1 : __builtin_ctzll(x));
}

template <class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p)
{
    os << "P(" << p.first << ", " << p.second << ")";
    return os;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &vec)
{
    os << "{";
    for (int i = 0; i < vec.size(); i++)
    {
        os << vec[i] << (i + 1 == vec.size() ? "" : ", ");
    }
    os << "}";
    return os;
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &map_var)
{
    os << "{";
    for (auto itr = map_var.begin(); itr != map_var.end(); itr++)
    {
        os << "(" << itr->first << ", " << itr->second << ")";
        itr++;
        if (itr != map_var.end())
            os << ", ";
        itr--;
    }
    os << "}";
    return os;
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &set_var)
{
    os << "{";
    for (auto itr = set_var.begin(); itr != set_var.end(); itr++)
    {
        os << *itr;
        ++itr;
        if (itr != set_var.end())
            os << ", ";
        itr--;
    }
    os << "}";
    return os;
}
template <typename T>
ostream &operator<<(ostream &os, const multiset<T> &set_var)
{
    os << "{";
    for (auto itr = set_var.begin(); itr != set_var.end(); itr++)
    {
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
#define show(...) _show(0, #__VA_ARGS__, __VA_ARGS__)
#else
#define show(...) true
#endif
template <typename T>
void _show(int i, T name)
{
    cerr << endl;
}
template <typename T1, typename T2, typename... T3>
void _show(int i, const T1 &a, const T2 &b, const T3 &...c)
{
    for (; a[i] != ',' && a[i] != '\0'; i++)
        cerr << a[i];
    cerr << ":" << b << " ";
    _show(i + 1, a, c...);
}

#include <unistd.h>

namespace fastio
{
    static constexpr uint32_t SZ = 1 << 17;
    char ibuf[SZ];
    char obuf[SZ];
    char out[100];
    // pointer of ibuf, obuf

    uint32_t pil = 0, pir = 0, por = 0;

    struct Pre
    {
        char num[10000][4];
        constexpr Pre() : num()
        {
            for (int i = 0; i < 10000; i++)
            {
                int n = i;
                for (int j = 3; j >= 0; j--)
                {
                    num[i][j] = n % 10 | '0';
                    n /= 10;
                }
            }
        }
    } constexpr pre;

    inline void load()
    {
        memmove(ibuf, ibuf + pil, pir - pil);
        pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
        pil = 0;
        if (pir < SZ)
            ibuf[pir++] = '\n';
    }

    inline void flush()
    {
        fwrite(obuf, 1, por, stdout);
        por = 0;
    }

    void rd(char &c)
    {
        do
        {
            if (pil + 1 > pir)
                load();
            c = ibuf[pil++];
        } while (isspace(c));
    }

    void rd(string &x)
    {
        x.clear();
        char c;
        do
        {
            if (pil + 1 > pir)
                load();
            c = ibuf[pil++];
        } while (isspace(c));
        do
        {
            x += c;
            if (pil == pir)
                load();
            c = ibuf[pil++];
        } while (!isspace(c));
    }

    template <typename T>
    void rd_real(T &x)
    {
        string s;
        rd(s);
        x = stod(s);
    }

    template <typename T>
    void rd_integer(T &x)
    {
        if (pil + 100 > pir)
            load();
        char c;
        do
            c = ibuf[pil++];
        while (c < '-');
        bool minus = 0;
        if constexpr (is_signed<T>::value || is_same_v<T, i128>)
        {
            if (c == '-')
            {
                minus = 1, c = ibuf[pil++];
            }
        }
        x = 0;
        while ('0' <= c)
        {
            x = x * 10 + (c & 15), c = ibuf[pil++];
        }
        if constexpr (is_signed<T>::value || is_same_v<T, i128>)
        {
            if (minus)
                x = -x;
        }
    }

    void rd(int &x)
    {
        rd_integer(x);
    }
    void rd(ll &x)
    {
        rd_integer(x);
    }
    void rd(i128 &x)
    {
        rd_integer(x);
    }
    void rd(uint &x)
    {
        rd_integer(x);
    }
    void rd(ull &x)
    {
        rd_integer(x);
    }
    void rd(u128 &x)
    {
        rd_integer(x);
    }
    void rd(double &x)
    {
        rd_real(x);
    }
    void rd(long double &x)
    {
        rd_real(x);
    }

    template <class T, class U>
    void rd(pair<T, U> &p)
    {
        return rd(p.first), rd(p.second);
    }
    template <size_t N = 0, typename T>
    void rd_tuple(T &t)
    {
        if constexpr (N < std::tuple_size<T>::value)
        {
            auto &x = std::get<N>(t);
            rd(x);
            rd_tuple<N + 1>(t);
        }
    }
    template <class... T>
    void rd(tuple<T...> &tpl)
    {
        rd_tuple(tpl);
    }

    template <size_t N = 0, typename T>
    void rd(array<T, N> &x)
    {
        for (auto &d : x)
            rd(d);
    }
    template <class T>
    void rd(vector<T> &x)
    {
        for (auto &d : x)
            rd(d);
    }

    void read() {}
    template <class H, class... T>
    void read(H &h, T &...t)
    {
        rd(h), read(t...);
    }

    void wt(const char c)
    {
        if (por == SZ)
            flush();
        obuf[por++] = c;
    }
    void wt(const string s)
    {
        for (char c : s)
            wt(c);
    }
    void wt(const char *s)
    {
        size_t len = strlen(s);
        for (size_t i = 0; i < len; i++)
            wt(s[i]);
    }

    template <typename T>
    void wt_integer(T x)
    {
        if (por > SZ - 100)
            flush();
        if (x < 0)
        {
            obuf[por++] = '-', x = -x;
        }
        int outi;
        for (outi = 96; x >= 10000; outi -= 4)
        {
            memcpy(out + outi, pre.num[x % 10000], 4);
            x /= 10000;
        }
        if (x >= 1000)
        {
            memcpy(obuf + por, pre.num[x], 4);
            por += 4;
        }
        else if (x >= 100)
        {
            memcpy(obuf + por, pre.num[x] + 1, 3);
            por += 3;
        }
        else if (x >= 10)
        {
            int q = (x * 103) >> 10;
            obuf[por] = q | '0';
            obuf[por + 1] = (x - q * 10) | '0';
            por += 2;
        }
        else
            obuf[por++] = x | '0';
        memcpy(obuf + por, out + outi + 4, 96 - outi);
        por += 96 - outi;
    }

    template <typename T>
    void wt_real(T x)
    {
        ostringstream oss;
        oss << fixed << setprecision(15) << double(x);
        string s = oss.str();
        wt(s);
    }

    void wt(int x)
    {
        wt_integer(x);
    }
    void wt(ll x)
    {
        wt_integer(x);
    }
    void wt(i128 x)
    {
        wt_integer(x);
    }
    void wt(uint x)
    {
        wt_integer(x);
    }
    void wt(ull x)
    {
        wt_integer(x);
    }
    void wt(u128 x)
    {
        wt_integer(x);
    }
    void wt(double x)
    {
        wt_real(x);
    }
    void wt(long double x)
    {
        wt_real(x);
    }

    template <class T, class U>
    void wt(const pair<T, U> val)
    {
        wt(val.first);
        wt(' ');
        wt(val.second);
    }
    template <size_t N = 0, typename T>
    void wt_tuple(const T t)
    {
        if constexpr (N < std::tuple_size<T>::value)
        {
            if constexpr (N > 0)
            {
                wt(' ');
            }
            const auto x = std::get<N>(t);
            wt(x);
            wt_tuple<N + 1>(t);
        }
    }
    template <class... T>
    void wt(tuple<T...> tpl)
    {
        wt_tuple(tpl);
    }
    template <class T, size_t S>
    void wt(const array<T, S> val)
    {
        auto n = val.size();
        for (size_t i = 0; i < n; i++)
        {
            if (i)
                wt(' ');
            wt(val[i]);
        }
    }
    template <class T>
    void wt(const vector<T> val)
    {
        auto n = val.size();
        for (size_t i = 0; i < n; i++)
        {
            if (i)
                wt(' ');
            wt(val[i]);
        }
    }

    void print()
    {
        wt('\n');
    }
    template <class Head, class... Tail>
    void print(Head &&head, Tail &&...tail)
    {
        wt(head);
        if (sizeof...(Tail))
            wt(' ');
        print(forward<Tail>(tail)...);
    }
    void __attribute__((destructor)) _d()
    {
        flush();
    }
} // namespace fastio

using fastio::flush;
using fastio::print;
using fastio::read;

inline void first(bool i = true)
{
    print(i ? "first" : "second");
}
inline void Alice(bool i = true)
{
    print(i ? "Alice" : "Bob");
}
inline void Takahashi(bool i = true)
{
    print(i ? "Takahashi" : "Aoki");
}
inline void yes(bool i = true)
{
    print(i ? "yes" : "no");
}
inline void Yes(bool i = true)
{
    print(i ? "Yes" : "No");
}
inline void No()
{
    print("No");
}
inline void YES(bool i = true)
{
    print(i ? "YES" : "NO");
}
inline void NO()
{
    print("NO");
}
inline void Yay(bool i = true)
{
    print(i ? "Yay!" : ":(");
}
inline void Possible(bool i = true)
{
    print(i ? "Possible" : "Impossible");
}
inline void POSSIBLE(bool i = true)
{
    print(i ? "POSSIBLE" : "IMPOSSIBLE");
}

/**
 * @brief Fast IO
 */

namespace Random
{
    mt19937_64 randgen(chrono::steady_clock::now().time_since_epoch().count());
    using u64 = unsigned long long;
    u64 get()
    {
        return randgen();
    }
    template <typename T>
    T get(T L)
    { // [0,L]

        return get() % (L + 1);
    }
    template <typename T>
    T get(T L, T R)
    { // [L,R]

        return get(R - L) + L;
    }
    double uniform()
    {
        return double(get(1000000000)) / 1000000000;
    }
    string str(int n)
    {
        string ret;
        rep(i, 0, n) ret += get('a', 'z');
        return ret;
    }
    template <typename Iter>
    void shuffle(Iter first, Iter last)
    {
        if (first == last)
            return;
        int len = 1;
        for (auto it = first + 1; it != last; it++)
        {
            len++;
            int j = get(0, len - 1);
            if (j != len - 1)
                iter_swap(it, first + j);
        }
    }
    template <typename T>
    vector<T> select(int n, T L, T R)
    { // [L,R]

        if (n * 2 >= R - L + 1)
        {
            vector<T> ret(R - L + 1);
            iota(ALL(ret), L);
            shuffle(ALL(ret));
            ret.resize(n);
            return ret;
        }
        else
        {
            unordered_set<T> used;
            vector<T> ret;
            while (SZ(used) < n)
            {
                T x = get(L, R);
                if (!used.count(x))
                {
                    used.insert(x);
                    ret.push_back(x);
                }
            }
            return ret;
        }
    }

    void relabel(int n, vector<pair<int, int>> &es)
    {
        shuffle(ALL(es));
        vector<int> ord(n);
        iota(ALL(ord), 0);
        shuffle(ALL(ord));
        for (auto &[u, v] : es)
            u = ord[u], v = ord[v];
    }
    template <bool directed, bool multi, bool self>
    vector<pair<int, int>> genGraph(int n, int m)
    {
        vector<pair<int, int>> cand, es;
        rep(u, 0, n) rep(v, 0, n)
        {
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
        else
        {
            ord = select(m, 0, SZ(cand) - 1);
        }
        for (auto &i : ord)
            es.push_back(cand[i]);
        relabel(n, es);
        return es;
    }
    vector<pair<int, int>> genTree(int n)
    {
        vector<pair<int, int>> es;
        rep(i, 1, n) es.push_back({get(i - 1), i});
        relabel(n, es);
        return es;
    }
}; // namespace Random

struct FastDiv
{
    using u64 = uint64_t;
    using u128 = __uint128_t;
    constexpr FastDiv() : m(), s(), x() {}
    constexpr FastDiv(int _m)
        : m(_m), s(__lg(m - 1)), x(((u128(1) << (s + 64)) + m - 1) / m) {}
    constexpr int get()
    {
        return m;
    }
    constexpr friend u64 operator/(u64 n, const FastDiv &d)
    {
        return (u128(n) * d.x >> d.s) >> 64;
    }
    constexpr friend int operator%(u64 n, const FastDiv &d)
    {
        return n - n / d * d.m;
    }
    constexpr pair<u64, int> divmod(u64 n) const
    {
        u64 q = n / (*this);
        return {q, n - q * m};
    }
    int m, s;
    u64 x;
};

struct FastDiv64
{
    using u64 = uint64_t;
    using u128 = __uint128_t;
    u128 mod, mh, ml;
    explicit FastDiv64(u64 mod = 1) : mod(mod)
    {
        u128 m = u128(-1) / mod;
        if (m * mod + mod == u128(0))
            ++m;
        mh = m >> 64;
        ml = m & u64(-1);
    }
    u64 umod() const
    {
        return mod;
    }
    u64 modulo(u128 x)
    {
        u128 z = (x & u64(-1)) * ml;
        z = (x & u64(-1)) * mh + (x >> 64) * ml + (z >> 64);
        z = (x >> 64) * mh + (z >> 64);
        x -= z * mod;
        return x < mod ? x : x - mod;
    }
    u64 mul(u64 a, u64 b)
    {
        return modulo(u128(a) * b);
    }
};

template <unsigned mod = 1000000007>
struct fp
{
    unsigned v;
    static constexpr int get_mod()
    {
        return mod;
    }
    constexpr unsigned inv() const
    {
        assert(v != 0);
        int x = v, y = mod, p = 1, q = 0, t = 0, tmp = 0;
        while (y > 0)
        {
            t = x / y;
            x -= t * y, p -= t * q;
            tmp = x, x = y, y = tmp;
            tmp = p, p = q, q = tmp;
        }
        if (p < 0)
            p += mod;
        return p;
    }
    constexpr fp() : v(0) {}
    // template <typename T>
    constexpr fp(ll x) : v(x >= 0 ? x % mod : (mod - (-x) % mod) % mod) {}
    fp operator-() const
    {
        return fp() - *this;
    }
    fp pow(ull t)
    {
        fp res = 1, b = *this;
        while (t)
        {
            if (t & 1)
                res *= b;
            b *= b;
            t >>= 1;
        }
        return res;
    }
    fp &operator+=(const fp &x)
    {
        if ((v += x.v) >= mod)
            v -= mod;
        return *this;
    }
    fp &operator-=(const fp &x)
    {
        if ((v += mod - x.v) >= mod)
            v -= mod;
        return *this;
    }
    fp &operator*=(const fp &x)
    {
        v = ull(v) * x.v % mod;
        return *this;
    }
    fp &operator/=(const fp &x)
    {
        v = ull(v) * x.inv() % mod;
        return *this;
    }
    fp operator+(const fp &x) const
    {
        return fp(*this) += x;
    }
    fp operator-(const fp &x) const
    {
        return fp(*this) -= x;
    }
    fp operator*(const fp &x) const
    {
        return fp(*this) *= x;
    }
    fp operator/(const fp &x) const
    {
        return fp(*this) /= x;
    }
    bool operator==(const fp &x) const
    {
        return v == x.v;
    }
    bool operator!=(const fp &x) const
    {
        return v != x.v;
    }
    friend istream &operator>>(istream &is, fp &x)
    {
        return is >> x.v;
    }
    friend ostream &operator<<(ostream &os, const fp &x)
    {
        return os << x.v;
    }
};

template <unsigned mod>
void rd(fp<mod> &x)
{
    fastio::rd(x.v);
}
template <unsigned mod>
void wt(fp<mod> x)
{
    fastio::wt(x.v);
}

vector<int> sieve(int N)
{
    vector<int> isp(N + 1);
    int n, sq = ceil(sqrt(N));
    for (int z = 1; z <= 5; z += 4)
    {
        for (int y = z; y <= sq; y += 6)
        {
            for (int x = 1; x <= sq and (n = 4 * x * x + y * y) <= N; ++x)
            {
                isp[n] ^= 1;
            }
            for (int x = y + 1; x <= sq and (n = 3 * x * x - y * y) <= N;
                 x += 2)
            {
                isp[n] ^= 1;
            }
        }
    }
    for (int z = 2; z <= 4; z += 2)
    {
        for (int y = z; y <= sq; y += 6)
        {
            for (int x = 1; x <= sq and (n = 3 * x * x + y * y) <= N; x += 2)
            {
                isp[n] ^= 1;
            }
            for (int x = y + 1; x <= sq and (n = 3 * x * x - y * y) <= N;
                 x += 2)
            {
                isp[n] ^= 1;
            }
        }
    }
    for (int y = 3; y <= sq; y += 6)
    {
        for (int z = 1; z <= 2; ++z)
        {
            for (int x = z; x <= sq and (n = 4 * x * x + y * y) <= N; x += 3)
            {
                isp[n] ^= 1;
            }
        }
    }
    for (int n = 5; n <= sq; ++n)
        if (isp[n])
        {
            for (int k = n * n; k <= N; k += n * n)
            {
                isp[k] = false;
            }
        }
    isp[2] = isp[3] = true;

    vector<int> ret;
    for (int i = 2; i <= N; i++)
        if (isp[i])
        {
            ret.push_back(i);
        }
    return ret;
}

template <typename T>
T Inv(ll n)
{
    static int md;
    static vector<T> buf({0, 1});
    if (md != T::get_mod())
    {
        md = T::get_mod();
        buf = vector<T>({0, 1});
    }
    assert(n > 0);
    n %= md;
    while (SZ(buf) <= n)
    {
        int k = SZ(buf), q = (md + k - 1) / k;
        buf.push_back(buf[k * q - md] * q);
    }
    return buf[n];
}

template <typename T>
T Fact(ll n, bool inv = 0)
{
    static int md;
    static vector<T> buf({1, 1}), ibuf({1, 1});
    if (md != T::get_mod())
    {
        md = T::get_mod();
        buf = ibuf = vector<T>({1, 1});
    }
    assert(n >= 0 and n < md);
    while (SZ(buf) <= n)
    {
        buf.push_back(buf.back() * SZ(buf));
        ibuf.push_back(ibuf.back() * Inv<T>(SZ(ibuf)));
    }
    return inv ? ibuf[n] : buf[n];
}

template <typename T>
T nPr(int n, int r, bool inv = 0)
{
    if (n < 0 || n < r || r < 0)
        return 0;
    return Fact<T>(n, inv) * Fact<T>(n - r, inv ^ 1);
}
template <typename T>
T nCr(int n, int r, bool inv = 0)
{
    if (n < 0 || n < r || r < 0)
        return 0;
    return Fact<T>(n, inv) * Fact<T>(r, inv ^ 1) * Fact<T>(n - r, inv ^ 1);
}
// sum = n, r tuples
template <typename T>
T nHr(int n, int r, bool inv = 0)
{
    return nCr<T>(n + r - 1, r - 1, inv);
}
// sum = n, a nonzero tuples and b tuples
template <typename T>
T choose(int n, int a, int b)
{
    if (n == 0)
        return !a;
    return nCr<T>(n + b - 1, a + b - 1);
}

struct m64
{
    using i64 = int64_t;
    using u64 = uint64_t;
    using u128 = __uint128_t;

    static u64 mod;
    static u64 r;
    static u64 n2;

    static u64 get_r()
    {
        u64 ret = mod;
        rep(_, 0, 5) ret *= 2 - mod * ret;
        return ret;
    }

    static void set_mod(u64 m)
    {
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
    m64(const int64_t &b) : a(reduce((u128(b) + mod) * n2)) {};

    static u64 reduce(const u128 &b)
    {
        return (b + u128(u64(b) * u64(-r)) * mod) >> 64;
    }
    u64 get() const
    {
        u64 ret = reduce(a);
        return ret >= mod ? ret - mod : ret;
    }
    m64 &operator*=(const m64 &b)
    {
        a = reduce(u128(a) * b.a);
        return *this;
    }
    m64 operator*(const m64 &b) const { return m64(*this) *= b; }
    bool operator==(const m64 &b) const
    {
        return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
    }
    bool operator!=(const m64 &b) const
    {
        return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
    }
    m64 pow(u128 n) const
    {
        m64 ret(1), mul(*this);
        while (n > 0)
        {
            if (n & 1)
                ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
};
typename m64::u64 m64::mod, m64::r, m64::n2;

bool Miller(ll n)
{
    if (n < 2 or (n & 1) == 0)
        return (n == 2);
    m64::set_mod(n);
    ll d = n - 1;
    while ((d & 1) == 0)
        d >>= 1;
    vector<ll> seeds;
    if (n < (1 << 30))
        seeds = {2, 7, 61};
    else
        seeds = {2, 325, 9375, 28178, 450775, 9780504};
    for (auto &x : seeds)
    {
        if (n <= x)
            break;
        ll t = d;
        m64 y = m64(x).pow(t);
        while (t != n - 1 and y != 1 and y != n - 1)
        {
            y *= y;
            t <<= 1;
        }
        if (y != n - 1 and (t & 1) == 0)
            return 0;
    }
    return 1;
}

vector<ll> Pollard(ll n)
{
    if (n <= 1)
        return {};
    if (Miller(n))
        return {n};
    if ((n & 1) == 0)
    {
        vector<ll> v = Pollard(n >> 1);
        v.push_back(2);
        return v;
    }
    for (ll x = 2, y = 2, d;;)
    {
        ll c = Random::get(2LL, n - 1);
        do
        {
            x = (__int128_t(x) * x + c) % n;
            y = (__int128_t(y) * y + c) % n;
            y = (__int128_t(y) * y + c) % n;
            d = __gcd(x - y + n, n);
        } while (d == 1);
        if (d < n)
        {
            vector<ll> lb = Pollard(d), rb = Pollard(n / d);
            lb.insert(lb.end(), ALL(rb));
            return lb;
        }
    }
}

vector<pair<ll, int>> Pollard2(ll n)
{
    auto ps = Pollard(n);
    sort(ALL(ps));
    using P = pair<ll, int>;
    vector<P> pes;
    for (auto &p : ps)
    {
        if (pes.empty() or pes.back().first != p)
        {
            pes.push_back({p, 1});
        }
        else
        {
            pes.back().second++;
        }
    }
    return pes;
}

vector<ll> EnumDivisors(vector<pair<ll, int>> &pes)
{
    vector<ll> ret;
    auto rec = [&](auto &rec, int id, ll d) -> void
    {
        if (id == SZ(pes))
        {
            ret.push_back(d);
            return;
        }
        rec(rec, id + 1, d);
        rep(e, 0, pes[id].second)
        {
            d *= pes[id].first;
            rec(rec, id + 1, d);
        }
    };
    rec(rec, 0, 1);
    sort(ALL(ret));
    return ret;
}

struct UnionFind
{
    vector<int> par;
    int n;
    UnionFind() {}
    UnionFind(int _n) : par(_n, -1), n(_n) {}
    int root(int x) { return par[x] < 0 ? x : par[x] = root(par[x]); }
    bool same(int x, int y) { return root(x) == root(y); }
    int size(int x) { return -par[root(x)]; }
    bool unite(int x, int y)
    {
        x = root(x), y = root(y);
        if (x == y)
            return false;
        if (size(x) > size(y))
            swap(x, y);
        par[y] += par[x];
        par[x] = y;
        n--;
        return true;
    }
};

template <typename F>
vector<int> MonotoneMinima(int R, int C, F cmp)
{
    vector<int> ret(R);
    auto rec = [&](auto &f, vector<int> target) -> void
    {
        int m = target.size();
        if (m == 0)
            return;
        vector<int> even;
        for (int i = 1; i < m; i += 2)
            even.push_back(target[i]);
        f(f, even);
        int cur = 0;
        for (int i = 0; i < m; i += 2)
        {
            ret[target[i]] = cur;
            int end = C - 1;
            if (i != m - 1)
                end = ret[even[i / 2]];
            while (cur < end)
            {
                cur++;
                if (cmp(target[i], ret[target[i]], cur))
                    ret[target[i]] = cur;
            }
        }
    };
    vector<int> tmp(R);
    iota(ALL(tmp), 0);
    rec(rec, tmp);
    return ret;
}

void solve()
{
    int n;
    read(n);
    vector<ll> X(n), Y(n);
    rep(i, 0, n) read(X[i], Y[i]);
    rep(i, 0, n)
    {
        X.push_back(X[i]);
        Y.push_back(Y[i]);
    }

    auto dist = [&](int i, int j) -> ll
    {
        return (X[i] - X[j]) * (X[i] - X[j]) + (Y[i] - Y[j]) * (Y[i] - Y[j]);
    };
    auto F = [&](int i, int j, int k) -> bool
    {
        if (j < i)
            return 1;
        if (k >= i + n)
            return 0;
        return dist(i, j) < dist(i, k);
    };
    auto ret = MonotoneMinima(n, n * 2, F);
    rep(i, 0, n) ret[i] %= n;
    print(ret);
}

int main()
{
    int T;
    read(T);
    while (T--)
        solve();
    return 0;
}

// g++ -std=gnu++20 -O3 -Wall -Wextra -DLOCAL -mtune=native -ftrapv -o a.out a.cpp -I .