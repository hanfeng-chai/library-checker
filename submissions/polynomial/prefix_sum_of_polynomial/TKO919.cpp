#line 1 "library/Template/template.hpp"
#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (int i = (int)(a); i < (int)(b); i++)
#define rrep(i, a, b) for (int i = (int)(b-1); i >= (int)(a); i--)
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

#ifdef LOCAL
#define show(...) _show(0, #__VA_ARGS__, __VA_ARGS__)
#else
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
template <class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p) {
    os << "P(" << p.first << ", " << p.second << ")";
    return os;
}
template <typename T, template <class> class C>
ostream &operator<<(ostream &os, const C<T> &v) {
    os << "[";
    for (auto d : v)
        os << d << ", ";
    os << "]";
    return os;
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
#line 3 "sol.cpp"

#line 2 "library/Math/modint.hpp"

template <unsigned mod = 1000000007> struct fp {
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

template <typename T> T Inv(ll n) {
    static const int md = T::get_mod();
    static vector<T> buf({0, 1});
    assert(n > 0);
    n %= md;
    while (SZ(buf) <= n) {
        int k = SZ(buf), q = (md + k - 1) / k;
        buf.push_back(buf[k * q - md] * q);
    }
    return buf[n];
}

template <typename T> T Fact(ll n, bool inv = 0) {
    static const int md = T::get_mod();
    static vector<T> buf({1, 1}), ibuf({1, 1});
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
template <typename T> T nHr(int n, int r, bool inv = 0) {
    return nCr<T>(n + r - 1, r, inv);
}

/**
 * @brief Modint
 */
#line 2 "library/Convolution/ntt.hpp"

template <typename T> struct NTT {
    static constexpr int rank2 = __builtin_ctzll(T::get_mod() - 1);
    std::array<T, rank2 + 1> root;  // root[i]^(2^i) == 1
    std::array<T, rank2 + 1> iroot; // root[i] * iroot[i] == 1

    std::array<T, std::max(0, rank2 - 2 + 1)> rate2;
    std::array<T, std::max(0, rank2 - 2 + 1)> irate2;

    std::array<T, std::max(0, rank2 - 3 + 1)> rate3;
    std::array<T, std::max(0, rank2 - 3 + 1)> irate3;

    NTT() {
        T g = 2;
        while (g.pow((T::get_mod() - 1) >> 1) == 1) {
            g += 1;
        }
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
                                (unsigned long long)(T::get_mod() + l.v - r.v) *
                                irot.v;
                            ;
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

                            a[i + offset] = a0 + a1 + a2 + a3;
                            a[i + offset + 1 * p] =
                                (a0 + (T::get_mod() - a1) + a2na3iimag) *
                                irot.v;
                            a[i + offset + 2 * p] =
                                (a0 + a1 + (T::get_mod() - a2) +
                                 (T::get_mod() - a3)) *
                                irot2.v;
                            a[i + offset + 3 * p] =
                                (a0 + (T::get_mod() - a1) +
                                 (T::get_mod() - a2na3iimag)) *
                                irot3.v;
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
                            a[i + offset] = a0 + a2 + a1 + a3;
                            a[i + offset + 1 * p] =
                                a0 + a2 + (2 * mod2 - (a1 + a3));
                            a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                            a[i + offset + 3 * p] =
                                a0 + na2 + (mod2 - a1na3imag);
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
#line 2 "library/FPS/fps.hpp"

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
    Poly<T> mult(const Poly<T> &a, const Poly<T> &b) {
        if (a.empty() or b.empty())
            return {};
        int as = a.size(), bs = b.size();
        int n = as + bs - 1;
        if (as <= 30 or bs <= 30) {
            if (as > 30)
                return mult(b, a);
            Poly<T> res(n);
            rep(i, 0, as) rep(j, 0, bs) res[i + j] += a[i] * b[j];
            return res;
        }
        int m = 1;
        while (m < n)
            m <<= 1;
        Poly<T> res(m);
        rep(i, 0, as) res[i] = a[i];
        res.NTT(0);
        if (a == b)
            rep(i, 0, m) res[i] *= res[i];
        else {
            Poly<T> c(m);
            rep(i, 0, bs) c[i] = b[i];
            c.NTT(0);
            rep(i, 0, m) res[i] *= c[i];
        }
        res.NTT(1);
        res.resize(n);
        return res;
    }
    Poly square() const {
        return Poly(mult(*this, *this));
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
    Poly operator/(const T &g) const {
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
        Poly g2 = g;
        reverse(ALL(*this));
        reverse(ALL(g2));
        int n = this->size() - g2.size() + 1;
        this->resize(n);
        g2.resize(n);
        *this *= g2.inv();
        this->resize(n);
        reverse(ALL(*this));
        shrink();
        return *this;
    }
    Poly &operator/=(const T &g) {
        rep(i, 0, this->size())(*this)[i] /= g;
        return *this;
    }
    Poly &operator%=(const Poly &g) {
        *this -= *this / g * g;
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
    Poly inv() const {
        const int n = this->size();
        Poly res(1);
        res.front() = T(1) / this->front();
        for (int k = 1; k < n; k <<= 1) {
            Poly f(k * 2), g(k * 2);
            rep(i, 0, min(n, k * 2)) f[i] = (*this)[i];
            rep(i, 0, k) g[i] = res[i];
            f.NTT(0);
            g.NTT(0);
            rep(i, 0, k * 2) f[i] *= g[i];
            f.NTT(1);
            rep(i, 0, k) {
                f[i] = 0;
                f[i + k] = -f[i + k];
            }
            f.NTT(0);
            rep(i, 0, k * 2) f[i] *= g[i];
            f.NTT(1);
            rep(i, 0, k) f[i] = res[i];
            swap(res, f);
        }
        res.resize(n);
        return res;
    }
    Poly exp() const {
        const int n = this->size();
        if (n == 1)
            return Poly({T(1)});
        Poly b(2), c(1), z1, z2(2);
        b[0] = c[0] = z2[0] = z2[1] = 1;
        b[1] = (*this)[1];
        for (int k = 2; k < n; k <<= 1) {
            Poly y = b;
            y.resize(k * 2);
            y.NTT(0);
            z1 = z2;
            Poly z(k);
            rep(i, 0, k) z[i] = y[i] * z1[i];
            z.NTT(1);
            rep(i, 0, k >> 1) z[i] = 0;
            z.NTT(0);
            rep(i, 0, k) z[i] *= -z1[i];
            z.NTT(1);
            c.insert(c.end(), z.begin() + (k >> 1), z.end());
            z2 = c;
            z2.resize(k * 2);
            z2.NTT(0);
            Poly x = *this;
            x.resize(k);
            x = x.diff();
            x.resize(k);
            x.NTT(0);
            rep(i, 0, k) x[i] *= y[i];
            x.NTT(1);
            Poly bb = b.diff();
            rep(i, 0, k - 1) x[i] -= bb[i];
            x.resize(k * 2);
            rep(i, 0, k - 1) {
                x[k + i] = x[i];
                x[i] = 0;
            }
            x.NTT(0);
            rep(i, 0, k * 2) x[i] *= z2[i];
            x.NTT(1);
            x.pop_back();
            x = x.inte();
            rep(i, k, min(n, k * 2)) x[i] += (*this)[i];
            rep(i, 0, k) x[i] = 0;
            x.NTT(0);
            rep(i, 0, k * 2) x[i] *= y[i];
            x.NTT(1);
            b.insert(b.end(), x.begin() + k, x.end());
        }
        b.resize(n);
        return b;
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
    void NTT(bool inv);
};

/**
 * @brief Formal Power Series (NTT-friendly mod)
 */
#line 7 "sol.cpp"
using Fp = fp<998244353>;
NTT<Fp> ntt;
template <> void Poly<Fp>::NTT(bool inv) {
    ntt.ntt(*this, inv);
}


template <typename T> vector<T> Bernoulli(int n) {
    Poly<T> f(n + 1);
    rep(i, 0, n + 1) f[i] = Fact<T>(i + 1, 1);
    f = f.inv();
    rep(i, 0, n + 1) f[i] *= Fact<T>(i);
    return f;
}

template <typename T> vector<T> Partition(int n) {
    Poly<T> f(n + 1);
    f[0] = 1;
    rep(k, 1, n + 1) {
        if (1LL * k * (3 * k + 1) / 2 <= n)
            f[1LL * k * (3 * k + 1) / 2] += (k & 1 ? -1 : 1);
        if (1LL * k * (3 * k - 1) / 2 <= n)
            f[1LL * k * (3 * k - 1) / 2] += (k & 1 ? -1 : 1);
    }
    return f.inv();
}

template <typename T> vector<T> StirlingNumber1st(int n) {
    if (n == 0)
        return Poly<T>({T(1)});
    Poly<T> f({T(0), T(1)});
    for (int LG = topbit(n) - 1; LG >= 0; LG--) {
        int m = n >> LG;
        f *= f.shift(m >> 1);
        if (m & 1)
            f = (f << 1) + f * T(m - 1);
    }
    rep(i, 0, n + 1) if ((n - i) & 1) f[i] = -f[i];
    return f;
}

template <typename T> vector<T> StirlingNumber2nd(int n) {
    if (n == 0)
        return Poly<T>({T(1)});
    Poly<T> f(n + 1), g(n + 1);
    rep(i, 0, n + 1) {
        f[i] = Fp(i).pow(n) * Fact<T>(i, 1);
        g[i] = Fact<T>(i, 1);
        if (i & 1)
            g[i] = -g[i];
    }
    f *= g;
    f.resize(n + 1);
    return f;
}

template <typename T> vector<T> Bell(int n) {
    Poly<T> f(n + 1);
    if (n)
        f[1] = 1;
    rep(i, 2, n + 1) f[i] = f[i - 1] / i;
    f = f.exp();
    T fac = 1;
    rep(i, 2, n + 1) fac *= i, f[i] *= fac;
    return f;
}

template <typename T> Poly<T> PrefixSum(Poly<T> &f) { // g(x)=sum_{y=0}^{x} f(y)
    if (f.empty())
        return {};
    int n = f.size();
    auto ber = Bernoulli<T>(n - 1);
    if (n >= 2)
        ber[1] = -ber[1];
    Poly<T> base(n);
    rep(i, 0, n) {
        ber[i] *= Fact<T>(i, 1);
        if (i)
            base[i] = f[i] * Fact<T>(i);
    }
    reverse(ALL(ber));
    base *= ber;
    Poly<T> ret(n + 1);
    ret[0] = f[0], ret[1] = f[0];
    rep(i, 1, n + 1) {
        ret[i] += base[i + n - 2] * Fact<T>(i, 1);
    }
    return ret;
}

int main() {
    int n;
    read(n);
    Poly<Fp> f(n);
    read(f);

    auto ret = PrefixSum(f).shift(-1);
    print(ret);
    return 0;
}
