#include<bits/stdc++.h>
using namespace std;

using ld = long double;
using ll = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using i128 = __int128;
using u128 = unsigned __int128;
using f128 = __float128;
 
 
#define pii pair<int,int>
#define pll pair<ll,ll>
 
#define all(x) (x).begin(),(x).end()
#define rall(x) (x).rbegin(),(x).rend()
#define ars(x) (x),(x+n)
 
#define TIME  (1.0 * clock() / CLOCKS_PER_SEC)
 
#define For(i,a,b) for (int i=(a); i<(b); i++)
#define rep(i,a) For(i,0,a)
#define rev(i,a,b) for (int i=(a); i>(b); i--)
#define FOR(i,a,b) for (int i=(a); i<=(b); i++)
#define REP(i,a) FOR(i,1,a)
#define REV(i,a,b) for (int i=(a); i>=(b); i--)
 
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define fi first
#define se second
#define FT ios_base::sync_with_stdio(false); cin.tie(nullptr);
 
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

using vi=vector<int>;
using vll = vector<ll>;
template <class T>
using vc = vector<T>;
template <class T>
using vvc = vector<vc<T>>;
template <class T>
using vvvc = vector<vvc<T>>;
template <class T>
using vvvvc = vector<vvvc<T>>;
template <class T>
using vvvvvc = vector<vvvvc<T>>;
template <class T>
using pq = priority_queue<T>;
template <class T>
using pqg = priority_queue<T, vector<T>, greater<T>>;
 
#define vv(type, name, h, ...) \
  vector<vector<type>> name(h, vector<type>(__VA_ARGS__))
#define vvv(type, name, h, w, ...)   \
  vector<vector<vector<type>>> name( \
      h, vector<vector<type>>(w, vector<type>(__VA_ARGS__)))
#define vvvv(type, name, a, b, c, ...)       \
  vector<vector<vector<vector<type>>>> name( \
      a, vector<vector<vector<type>>>(       \
             b, vector<vector<type>>(c, vector<type>(__VA_ARGS__))))
 
//template <class T>
//using ods =
//   tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
 
template <typename T> bool chkmin(T &x,T y){return x>y?x=y,1:0;}
template <typename T> bool chkmax(T &x,T y){return x<y?x=y,1:0;}
 
template<class T> using pq = priority_queue<T>;
template<class T> using pqg = priority_queue<T, vector<T>, greater<T>>;
 
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
 
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ", "; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? ", " : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
 
void dbg_out() { cerr << endl; }
template<typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { __print(H); if (sizeof...(T)) cerr << ", "; dbg_out(T...); }
#define dbg(...) cerr << "[" << #__VA_ARGS__ << "]:"; dbg_out(__VA_ARGS__);

//---------------for faster io-----------------------------------
#pragma once

#include <unistd.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <bit>
#include <cstdint>
#include <cassert>
#include <numeric>
#include <type_traits>

namespace yosupo {

namespace internal {

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
using is_integral =
    typename std::conditional<std::is_integral<T>::value ||
                                  internal::is_signed_int128<T>::value ||
                                  internal::is_unsigned_int128<T>::value,
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

template <class T>
using is_integral_t = std::enable_if_t<is_integral<T>::value>;

template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

}  // namespace yosupo

namespace yosupo {

struct Scanner {
  public:
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;

    Scanner(FILE* fp) : fd(fileno(fp)) { line[0] = 127; }

    void read() {}
    template <class H, class... T> void read(H& h, T&... t) {
        bool f = read_single(h);
        assert(f);
        read(t...);
    }

    int read_unsafe() { return 0; }
    template <class H, class... T> int read_unsafe(H& h, T&... t) {
        bool f = read_single(h);
        if (!f) return 0;
        return 1 + read_unsafe(t...);
    }

    int close() { return ::close(fd); }

  private:
    static constexpr int SIZE = 1 << 15;

    int fd = -1;
    std::array<char, SIZE + 1> line;
    int st = 0, ed = 0;
    bool eof = false;

    bool read_single(std::string& ref) {
        if (!skip_space()) return false;
        ref = "";
        while (true) {
            char c = top();
            if (c <= ' ') break;
            ref += c;
            st++;
        }
        return true;
    }
    bool read_single(double& ref) {
        std::string s;
        if (!read_single(s)) return false;
        ref = std::stod(s);
        return true;
    }

    template <class T,
              std::enable_if_t<std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& ref) {
        if (!skip_space<50>()) return false;
        ref = top();
        st++;
        return true;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& sref) {
        using U = internal::to_unsigned_t<T>;
        if (!skip_space<50>()) return false;
        bool neg = false;
        if (line[st] == '-') {
            neg = true;
            st++;
        }
        U ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        sref = neg ? -ref : ref;
        return true;
    }
    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<!std::is_same<U, char>::value>* = nullptr>
    bool read_single(U& ref) {
        if (!skip_space<50>()) return false;
        ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        return true;
    }

    bool reread() {
        if (ed - st >= 50) return true;
        if (st > SIZE / 2) {
            std::memmove(line.data(), line.data() + st, ed - st);
            ed -= st;
            st = 0;
        }
        if (eof) return false;
        auto u = ::read(fd, line.data() + ed, SIZE - ed);
        if (u == 0) {
            eof = true;
            line[ed] = '\0';
            u = 1;
        }
        ed += int(u);
        line[ed] = char(127);
        return true;
    }

    char top() {
        if (st == ed) {
            bool f = reread();
            assert(f);
        }
        return line[st];
    }

    template <int TOKEN_LEN = 0> bool skip_space() {
        while (true) {
            while (line[st] <= ' ') st++;
            if (ed - st > TOKEN_LEN) return true;
            if (st > ed) st = ed;
            for (auto i = st; i < ed; i++) {
                if (line[i] <= ' ') return true;
            }
            if (!reread()) return false;
        }
    }
};

struct Printer {
  public:
    template <char sep = ' ', bool F = false> void write() {}
    template <char sep = ' ', bool F = false, class H, class... T>
    void write(const H& h, const T&... t) {
        if (F) write_single(sep);
        write_single(h);
        write<true>(t...);
    }
    template <char sep = ' ', class... T> void writeln(const T&... t) {
        write<sep>(t...);
        write_single('\n');
    }

    Printer(FILE* _fp) : fd(fileno(_fp)) {}
    ~Printer() { flush(); }

    int close() {
        flush();
        return ::close(fd);
    }

    void flush() {
        if (pos) {
            auto res = ::write(fd, line.data(), pos);
            assert(res != -1);
            pos = 0;
        }
    }

  private:
    static std::array<std::array<char, 2>, 100> small;
    static std::array<unsigned long long, 20> tens;

    static constexpr size_t SIZE = 1 << 15;
    int fd;
    std::array<char, SIZE> line;
    size_t pos = 0;
    std::stringstream ss;

    template <class T,
              std::enable_if_t<std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        if (pos == SIZE) flush();
        line[pos++] = val;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        using U = internal::to_unsigned_t<T>;
        if (val == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();
        U uval = val;
        if (val < 0) {
            write_single('-');
            uval = -uval;
        }
        write_unsigned(uval);
    }

    template <class U, internal::is_unsigned_int_t<U>* = nullptr>
    void write_single(U uval) {
        if (uval == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();

        write_unsigned(uval);
    }

    static int calc_len(uint64_t x) {
        int i = ((63 - std::countl_zero(x)) * 3 + 3) / 10;
        if (x < tens[i])
            return i;
        else
            return i + 1;
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<2 >= sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<4 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        std::array<char, 8> buf;
        memcpy(buf.data() + 6, small[uval % 100].data(), 2);
        memcpy(buf.data() + 4, small[uval / 100 % 100].data(), 2);
        memcpy(buf.data() + 2, small[uval / 10000 % 100].data(), 2);
        memcpy(buf.data() + 0, small[uval / 1000000 % 100].data(), 2);

        if (uval >= 100000000) {
            if (uval >= 1000000000) {
                memcpy(line.data() + pos, small[uval / 100000000 % 100].data(),
                       2);
                pos += 2;
            } else {
                line[pos] = char('0' + uval / 100000000);
                pos++;
            }
            memcpy(line.data() + pos, buf.data(), 8);
            pos += 8;
        } else {
            size_t len = calc_len(uval);
            memcpy(line.data() + pos, buf.data() + (8 - len), len);
            pos += len;
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<8 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <
        class U,
        std::enable_if_t<internal::is_unsigned_int128<U>::value>* = nullptr>
    void write_unsigned(U uval) {
        static std::array<char, 50> buf;
        size_t len = 0;
        while (uval > 0) {
            buf[len++] = char((uval % 10) + '0');
            uval /= 10;
        }
        std::reverse(buf.begin(), buf.begin() + len);
        memcpy(line.data() + pos, buf.data(), len);
        pos += len;
    }

    void write_single(const std::string& s) {
        for (char c : s) write_single(c);
    }
    void write_single(const char* s) {
        size_t len = strlen(s);
        for (size_t i = 0; i < len; i++) write_single(s[i]);
    }
    template <class T> void write_single(const std::vector<T>& val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) {
            if (i) write_single(' ');
            write_single(val[i]);
        }
    }
};

std::array<std::array<char, 2>, 100> Printer::small = [] {
    std::array<std::array<char, 2>, 100> table;
    for (int i = 0; i <= 99; i++) {
        table[i][1] = char('0' + (i % 10));
        table[i][0] = char('0' + (i / 10 % 10));
    }
    return table;
}();
std::array<unsigned long long, 20> Printer::tens = [] {
    std::array<unsigned long long, 20> table;
    for (int i = 0; i < 20; i++) {
        table[i] = 1;
        for (int j = 0; j < i; j++) {
            table[i] *= 10;
        }
    }
    return table;
}();

}  // namespace yosupo
//sc.read(type) to enter input
//sc.write(type) to print out answer
//sc.writeln() to go break the line
using namespace yosupo;
Scanner sc(stdin);
Printer pr(stdout);
const int INF=1e9;
const ll INFI=1e15;
//----------Author: Nguyen Ho Nam,UIT, Saigon-----------------
#ifndef ATCODER_INTERNAL_MATH_HPP
#define ATCODER_INTERNAL_MATH_HPP 1

#include <utility>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace atcoder {

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

    // @param m `1 <= m`
    explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}

    // @return m
    unsigned int umod() const { return _m; }

    // @param a `0 <= a < m`
    // @param b `0 <= b < m`
    // @return `a * b % m`
    unsigned int mul(unsigned int a, unsigned int b) const {
        // [1] m = 1
        // a = b = im = 0, so okay

        // [2] m >= 2
        // im = ceil(2^64 / m)
        // -> im * m = 2^64 + r (0 <= r < m)
        // let z = a*b = c*m + d (0 <= c, d < m)
        // a*b * im = (c*m + d) * im = c*(im*m) + d*im = c*2^64 + c*r + d*im
        // c*r + d*im < m * m + m * im < m * m + 2^64 + m <= 2^64 + m * (m + 1) < 2^64 * 2
        // ((ab * im) >> 64) == c or c + 1
        unsigned long long z = a;
        z *= b;
#ifdef _MSC_VER
        unsigned long long x;
        _umul128(z, im, &x);
#else
        unsigned long long x =
            (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
#endif
        unsigned long long y = x * _m;
        return (unsigned int)(z - y + (z < y ? _m : 0));
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

// Compile time primitive root
// @param m must be prime
// @return primitive root (and minimum in now)
constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x > 1) {
        divs[cnt++] = x;
    }
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod_constexpr(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
template <int m> constexpr int primitive_root = primitive_root_constexpr(m);

// @param n `n < 2^32`
// @param m `1 <= m < 2^32`
// @return sum_{i=0}^{n-1} floor((ai + b) / m) (mod 2^64)
unsigned long long floor_sum_unsigned(unsigned long long n,
                                      unsigned long long m,
                                      unsigned long long a,
                                      unsigned long long b) {
    unsigned long long ans = 0;
    while (true) {
        if (a >= m) {
            ans += n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }

        unsigned long long y_max = a * n + b;
        if (y_max < m) break;
        // y_max < m * (n + 1)
        // floor(y_max / m) <= n
        n = (unsigned long long)(y_max / m);
        b = (unsigned long long)(y_max % m);
        std::swap(m, a);
    }
    return ans;
}

}  // namespace internal

}  // namespace atcoder

#endif  // ATCODER_INTERNAL_MATH_HPP
#ifndef ATCODER_MATH_HPP
#define ATCODER_MATH_HPP 1

#include <algorithm>
#include <cassert>
#include <tuple>
#include <vector>

namespace atcoder {

long long pow_mod(long long x, long long n, int m) {
    assert(0 <= n && 1 <= m);
    if (m == 1) return 0;
    internal::barrett bt((unsigned int)(m));
    unsigned int r = 1, y = (unsigned int)(internal::safe_mod(x, m));
    while (n) {
        if (n & 1) r = bt.mul(r, y);
        y = bt.mul(y, y);
        n >>= 1;
    }
    return r;
}

long long inv_mod(long long x, long long m) {
    assert(1 <= m);
    auto z = internal::inv_gcd(x, m);
    assert(z.first == 1);
    return z.second;
}

// (rem, mod)
std::pair<long long, long long> crt(const std::vector<long long>& r,
                                    const std::vector<long long>& m) {
    assert(r.size() == m.size());
    int n = int(r.size());
    // Contracts: 0 <= r0 < m0
    long long r0 = 0, m0 = 1;
    for (int i = 0; i < n; i++) {
        assert(1 <= m[i]);
        long long r1 = internal::safe_mod(r[i], m[i]), m1 = m[i];
        if (m0 < m1) {
            std::swap(r0, r1);
            std::swap(m0, m1);
        }
        if (m0 % m1 == 0) {
            if (r0 % m1 != r1) return {0, 0};
            continue;
        }
        // assume: m0 > m1, lcm(m0, m1) >= 2 * max(m0, m1)

        // (r0, m0), (r1, m1) -> (r2, m2 = lcm(m0, m1));
        // r2 % m0 = r0
        // r2 % m1 = r1
        // -> (r0 + x*m0) % m1 = r1
        // -> x*u0*g = r1-r0 (mod u1*g) (u0*g = m0, u1*g = m1)
        // -> x = (r1 - r0) / g * inv(u0) (mod u1)

        // im = inv(u0) (mod u1) (0 <= im < u1)
        long long g, im;
        std::tie(g, im) = internal::inv_gcd(m0, m1);

        long long u1 = (m1 / g);
        // |r1 - r0| < (m0 + m1) <= lcm(m0, m1)
        if ((r1 - r0) % g) return {0, 0};

        // u1 * u1 <= m1 * m1 / g / g <= m0 * m1 / g = lcm(m0, m1)
        long long x = (r1 - r0) / g % u1 * im % u1;

        // |r0| + |m0 * x|
        // < m0 + m0 * (u1 - 1)
        // = m0 + m0 * m1 / g - m0
        // = lcm(m0, m1)
        r0 += x * m0;
        m0 *= u1;  // -> lcm(m0, m1)
        if (r0 < 0) r0 += m0;
    }
    return {r0, m0};
}

long long floor_sum(long long n, long long m, long long a, long long b) {
    assert(0 <= n && n < (1LL << 32));
    assert(1 <= m && m < (1LL << 32));
    unsigned long long ans = 0;
    if (a < 0) {
        unsigned long long a2 = internal::safe_mod(a, m);
        ans -= 1ULL * n * (n - 1) / 2 * ((a2 - a) / m);
        a = a2;
    }
    if (b < 0) {
        unsigned long long b2 = internal::safe_mod(b, m);
        ans -= 1ULL * n * ((b2 - b) / m);
        b = b2;
    }
    return ans + internal::floor_sum_unsigned(n, m, a, b);
}

}  // namespace atcoder

#endif  // ATCODER_MATH_HPP
using namespace atcoder;
/*
   Primality tests-Miller Rabin 
   Reference:https://cp-algorithms.com/algebra/primality_tests.html
   Notes: higher bases for big number 
*/
bool check_composite(u64 n, u64 a, u64 d, int s) {
    u64 x = pow_mod(a, d, n);
    if (x == 1 || x == n - 1)
        return false;
    for (int r = 1; r < s; r++) {
        x = (u128)x * x % n;
        if (x == n - 1)
            return false;
    }
    return true;
}

bool MillerRabin(u64 n) {
    if (n < 2)
        return false;

    // Small primes to check divisibility
    for (u64 a : {2, 3, 5, 13, 19, 73, 193, 407521, 299210837}) {
        if (n % a == 0)
            return n == a;
    }

    int r = 0;
    u64 d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    // Larger bases for checking primality
    for (u64 a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (n == a)
            return true;
        if (check_composite(n, a, d, r))
            return false;
    }
    return true;
}
#line 2 "math-fast/gcd.hpp"

#include <algorithm>
using namespace std;

namespace BinaryGCDImpl {
using i8 = char;

u64 binary_gcd(u64 a, u64 b) {
  if (a == 0 || b == 0) return a + b;
  i8 n = __builtin_ctzll(a);
  i8 m = __builtin_ctzll(b);
  a >>= n;
  b >>= m;
  n = min(n, m);
  while (a != b) {
    u64 d = a - b;
    i8 s = __builtin_ctzll(d);
    bool f = a > b;
    b = f ? b : a;
    a = (f ? d : -d) >> s;
  }
  return a << n;
}

using u128 = __uint128_t;
// a > 0
int ctz128(u128 a) {
  u64 lo = a & u64(-1);
  return lo ? __builtin_ctzll(lo) : 64 + __builtin_ctzll(a >> 64);
}
u128 binary_gcd128(u128 a, u128 b) {
  if (a == 0 || b == 0) return a + b;
  i8 n = ctz128(a);
  i8 m = ctz128(b);
  a >>= n;
  b >>= m;
  n = min(n, m);
  while (a != b) {
    u128 d = a - b;
    i8 s = ctz128(d);
    bool f = a > b;
    b = f ? b : a;
    a = (f ? d : -d) >> s;
  }
  return a << n;
}

}  // namespace BinaryGCDImpl

long long binary_gcd(long long a, long long b) {
  return BinaryGCDImpl::binary_gcd(abs(a), abs(b));
}
__int128_t binary_gcd128(__int128_t a, __int128_t b) {
  if (a < 0) a = -a;
  if (b < 0) b = -b;
  return BinaryGCDImpl::binary_gcd128(a, b);
}

/**
 * @brief binary GCD
 */
/*
   Integer factorization (Floyd's cycle-finding algorithm)
   Reference:https://cp-algorithms.com/algebra/factorization.html
   Notes: Better performance in pollard rho function compare to web version
*/
u64 mult(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

u64 f(u64 x, u64 c, u64 mod) {
    return (mult(x, x, mod) + c) % mod;
}

u64 rho(u64 n, u64 x0 = 2, u64 c = 1) {
    if (n % 2 == 0)
        return 2;

    u64 x = x0;
    u64 y = x0;
    u64 g = 1;
    while (g == 1) {
        x = f(x, c, n);
        y = f(f(y, c, n), c, n);
        g = binary_gcd(abs((long long)x - (long long)y), n);
        if (g == n)
            return rho(n, x0 + 1, c + 1);
    }
    return g;
}

vector<u64> factorize(u64 n) {
    vector<u64> factors;
    if (n == 1)
        return factors;
    if (MillerRabin(n)) {
        factors.push_back(n);
        return factors;
    }
    u64 divisor = rho(n);
    auto div1 = factorize(divisor), div2 = factorize(n / divisor);
    factors.insert(factors.end(),all(div1));
    factors.insert(factors.end(),all(div2));
    sort(all(factors));
    return factors;
}
/*
   code from maspy but a little bit modify 
*/
vector<pair<u64, int>> factor(u64 n) {
  assert(n >= 1);
  vector<pair<u64, int>> pf;
  For(p, 2, 100) {
    if (p * p > n) break;
    if (n % p == 0) {
      ll e = 0;
      do { n /= p, e += 1; } while (n % p == 0);
      pf.eb(p, e);
    }
  }
  while (n > 1) { 
    auto prime=factorize(n);
    for(ll p:prime){
        ll e=0;
        while(n%p==0){
            n/=p; e+=1;
        }
        pf.eb(p,e);
    }
  }
  sort(all(pf));
  return pf;
}
/*
  reference:https://judge.yosupo.jp/submission/206982
*/
u64 PrimitiveRoot(u64 p){
   u64 x=0;
   auto mi=factorize(p-1);
   sort(all(mi));
   mi.resize(unique(all(mi))-mi.begin());
   for(int i=0;!i;){
      i=1; ++x;
      for(u64 f:mi){
         if(pow_mod(x,(p-1)/f,p)==1){
            i=0; 
            break;
         }
      }
   }
   return x;
}
// https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp
struct Barrett {
  using u32 = unsigned int;
  using i64 = long long;
  using u64 = unsigned long long;
  u32 m;
  u64 im;
  Barrett() : m(), im() {}
  Barrett(int n) : m(n), im(u64(-1) / m + 1) {}
  constexpr inline i64 quo(u64 n) {
    u64 x = u64((__uint128_t(n) * im) >> 64);
    u32 r = n - x * m;
    return m <= r ? x - 1 : x;
  }
  constexpr inline i64 rem(u64 n) {
    u64 x = u64((__uint128_t(n) * im) >> 64);
    u32 r = n - x * m;
    return m <= r ? r + m : r;
  }
  constexpr inline pair<i64, int> quorem(u64 n) {
    u64 x = u64((__uint128_t(n) * im) >> 64);
    u32 r = n - x * m;
    if (m <= r) return {x - 1, r + m};
    return {x, r};
  }
  constexpr inline i64 pow(u64 n, i64 p) {
    u32 a = rem(n), r = m == 1 ? 0 : 1;
    while (p) {
      if (p & 1) r = rem(u64(r) * a);
      a = rem(u64(a) * a);
      p >>= 1;
    }
    return r;
  }
  constexpr inline u32 mul(u32 a, u32 b) {
    return rem(u64(a) * b);
  }
};
/*
 references:https://maspypy.github.io/library/mod/binomial.hpp
*/
struct Binomial_PrimePower {
  int p, e;
  int pp;
  int root;
  int ord;
  vc<int> exp;
  vc<int> log_fact;
  vc<int> power;
  Barrett bt_p, bt_pp;

  Binomial_PrimePower(int p, int e) : p(p), e(e), power(e + 1, 1) {
    rep(i, e) power[i + 1] = power[i] * p;
    pp = power[e];
    bt_p = Barrett(p), bt_pp = Barrett(pp);
    vc<int> log;
    if (p == 2) {
      if (e <= 1) { return; }
      root = 5;
      ord = pp / 4;
      exp.assign(ord, 1);
      log.assign(pp, 0);
      rep(i, ord - 1) { exp[i + 1] = (exp[i] * root) & (pp - 1); }
      rep(i, ord) log[exp[i]] = log[pp - exp[i]] = i;
    } else {
      root = PrimitiveRoot(p);
      ord = pp / p * (p - 1);
      exp.assign(ord, 1);
      log.assign(pp, 0);
      rep(i, ord - 1) { exp[i + 1] = bt_pp.mul(exp[i], root); }
      rep(i, ord) log[exp[i]] = i;
    }
    log_fact.assign(pp, 0);
    For(i, 1, pp) {
      log_fact[i] = log_fact[i - 1] + log[i];
      if (log_fact[i] >= ord) log_fact[i] -= ord;
    }
  }
  int C(ll n, ll i) {
    assert(n >= 0);
    if (i < 0 || i > n) return 0;
    ll a = i, b = n - i;
    if (pp == 2) { return ((a & b) == 0 ? 1 : 0); }
    int log = 0, cnt_p = 0, sgn = 0;
    if (e > 1) {
      while (n && cnt_p < e) {
        auto [n1, nr1] = bt_pp.quorem(n);
        auto [a1, ar1] = bt_pp.quorem(a);
        auto [b1, br1] = bt_pp.quorem(b);
        log += log_fact[nr1] - log_fact[ar1] - log_fact[br1];
        if (p > 2) {
          sgn += (n1 & 1) + (a1 & 1) + (b1 & 1);
        } else {
          sgn += (((nr1 + 1) & 4) + ((ar1 + 1) & 4) + ((br1 + 1) & 4)) / 4;
        }
        n = bt_p.quo(n), a = bt_p.quo(a), b = bt_p.quo(b);
        cnt_p += n - a - b;
      }
    } else {
      while (n && cnt_p < e) {
        auto [n1, nr1] = bt_pp.quorem(n);
        auto [a1, ar1] = bt_pp.quorem(a);
        auto [b1, br1] = bt_pp.quorem(b);
        log += log_fact[nr1] - log_fact[ar1] - log_fact[br1];
        if (p > 2) {
          sgn += (n1 & 1) + (a1 & 1) + (b1 & 1);
        } else {
          sgn += ((nr1 + 1) >> 2 & 1) + ((ar1 + 1) >> 2 & 1)
                 + ((br1 + 1) >> 2 & 1);
        }
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
  int mod;
  vc<Binomial_PrimePower> BPP;
  vc<int> crt_coef;
  Barrett bt;

  Binomial(int mod) : mod(mod), bt(mod) {
    for (auto&& [p, e]: factor(mod)) {
      int pp = 1;
      rep(i,e) pp *= p;
      BPP.eb(Binomial_PrimePower(p, e));
      int other = mod / pp;
      crt_coef.eb(ll(other) * inv_mod(other, pp) % mod);
    }
  }

  int C(ll n, ll k) {
    assert(n >= 0);
    if (k < 0 || k > n) return 0;
    int ANS = 0;
    rep(s, crt_coef.size()) {
      ANS = bt.rem(ANS + u64(BPP[s].C(n, k)) * crt_coef[s]);
    }
    return ANS;
  }
};
void solve()
{
  u64 t,m,n,k; sc.read(t,m);
  Binomial comb(m);
  while(t--){
     sc.read(n,k);
     pr.write(comb.C(n,k));
     pr.writeln();
  }  
}

int main(){
FT;
int t=1; 
//sc.read(t); int tc=0;
while(t--)
{
//tc++;
//cout << "Case #" << tc << ": ";
solve();
// cerr << "Time elapsed: " << TIME << " s.\n";
}
}