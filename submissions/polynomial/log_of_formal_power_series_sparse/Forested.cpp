#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series_sparse"
// ============
#include <cassert>
#include <utility>
#include <vector>
// ============
#include <cassert>
#include <vector>

template <typename M>
M inv(int n) {
    static std::vector<M> data{M::raw(0), M::raw(1)};
    static constexpr unsigned MOD = M::get_mod();
    assert(0 < n);
    while ((int)data.size() <= n) {
        unsigned k = (unsigned)data.size();
        unsigned r = MOD / k + 1;
        data.push_back(M::raw(r) * data[k * r - MOD]);
    }
    return data[n];
}

template <typename M>
M fact(int n) {
    static std::vector<M> data{M::raw(1), M::raw(1)};
    assert(0 <= n);
    while ((int)data.size() <= n) {
        unsigned k = (unsigned)data.size();
        data.push_back(M::raw(k) * data.back());
    }
    return data[n];
}

template <typename M>
M inv_fact(int n) {
    static std::vector<M> data{M::raw(1), M::raw(1)};
    assert(0 <= n);
    while ((int)data.size() <= n) {
        unsigned k = (unsigned)data.size();
        data.push_back(inv<M>(k) * data.back());
    }
    return data[n];
}

template <typename M>
M binom(int n, int k) {
    assert(0 <= n);
    if (k < 0 || n < k) {
        return M::raw(0);
    }
    return fact<M>(n) * inv_fact<M>(k) * inv_fact<M>(n - k);
}

template <typename M>
M n_terms_sum_k(int n, int k) {
    assert(0 <= n && 0 <= k);
    if (n == 0) {
        return (k == 0 ? M::raw(1) : M::raw(0));
    }
    return binom<M>(n + k - 1, n - 1);
}
// ============
// O(n * (# of nonzero))
// g = log(f)
// g' = f' / f
// f g' = f'
// [x^(n-1)] f g' = [x^(n-1)] f'
// n g[n] + sum_(i>0) f[i] (n-i) g[n-i] = n f[n]
// g[n] = f[n] - 1/n sum_(i>0) (n-i) f[i] g[n-i]
template <typename T>
std::vector<T> fps_log_sparse(const std::vector<T> &f) {
    assert(!f.empty() && f[0] == T(1));
    int n = (int)f.size();
    std::vector<std::pair<int, T>> nonzero;
    for (int i = 1; i < n; ++i) {
        if (f[i] != T()) {
            nonzero.emplace_back(i, f[i]);
        }
    }
    std::vector<T> g = f;
    g[0] = T(0);
    for (int i = 1; i < n; ++i) {
        T sum;
        for (auto [j, val] : nonzero) {
            if (j > i) {
                break;
            }
            sum += T(i - j) * val * g[i - j];
        }
        g[i] -= inv<T>(i) * sum;
    }
    return g;
}
// ============
// ============

#include <cassert>
#include <iostream>
#include <type_traits>
// ============

#include <utility>

constexpr bool is_prime(unsigned n) {
    if (n == 0 || n == 1) {
        return false;
    }
    for (unsigned i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

constexpr unsigned mod_pow(unsigned x, unsigned y, unsigned mod) {
    unsigned ret = 1, self = x;
    while (y != 0) {
        if (y & 1) {
            ret = (unsigned)((unsigned long long)ret * self % mod);
        }
        self = (unsigned)((unsigned long long)self * self % mod);
        y /= 2;
    }
    return ret;
}

template <unsigned mod>
constexpr unsigned primitive_root() {
    static_assert(is_prime(mod), "`mod` must be a prime number.");
    if (mod == 2) {
        return 1;
    }

    unsigned primes[32] = {};
    int it = 0;
    {
        unsigned m = mod - 1;
        for (unsigned i = 2; i * i <= m; ++i) {
            if (m % i == 0) {
                primes[it++] = i;
                while (m % i == 0) {
                    m /= i;
                }
            }
        }
        if (m != 1) {
            primes[it++] = m;
        }
    }
    for (unsigned i = 2; i < mod; ++i) {
        bool ok = true;
        for (int j = 0; j < it; ++j) {
            if (mod_pow(i, (mod - 1) / primes[j], mod) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return i;
    }
    return 0;
}

// y >= 1
template <typename T>
constexpr T safe_mod(T x, T y) {
    x %= y;
    if (x < 0) {
        x += y;
    }
    return x;
}

// y != 0
template <typename T>
constexpr T floor_div(T x, T y) {
    if (y < 0) {
        x *= -1;
        y *= -1;
    }
    if (x >= 0) {
        return x / y;
    } else {
        return -((-x + y - 1) / y);
    }
}

// y != 0
template <typename T>
constexpr T ceil_div(T x, T y) {
    if (y < 0) {
        x *= -1;
        y *= -1;
    }
    if (x >= 0) {
        return (x + y - 1) / y;
    } else {
        return -(-x / y);
    }
}

// b >= 1
// returns (g, x) s.t. g = gcd(a, b), a * x = g (mod b), 0 <= x < b / g
// from ACL
template <typename T>
std::pair<T, T> extgcd(T a, T b) {
    a = safe_mod(a, b);
    T s = b, t = a, m0 = 0, m1 = 1;
    while (t) {
        T u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        std::swap(s, t);
        std::swap(m0, m1);
    }
    if (m0 < 0) {
        m0 += b / s;
    }
    return std::pair<T, T>(s, m0);
}

// b >= 1
// returns (g, x, y) s.t. g = gcd(a, b), a * x + b * y = g, 0 <= x < b / g, |y| < max(2, |a| / g)
template <typename T>
std::tuple<T, T, T> extgcd2(T a, T b) {
    T _a = safe_mod(a, b);
    T quot = (a - _a) / b;
    T x00 = 0, x01 = 1, y0 = b;
    T x10 = 1, x11 = -quot, y1 = _a;
    while (y1) {
        T u = y0 / y1;
        x00 -= u * x10;
        x01 -= u * x11;
        y0 -= u * y1;
        std::swap(x00, x10);
        std::swap(x01, x11);
        std::swap(y0, y1);
    }
    if (x00 < 0) {
        x00 += b / y0;
        x01 -= a / y0;
    }
    return std::tuple<T, T, T>(y0, x00, x01);
}

// gcd(x, m) == 1
template <typename T>
T inv_mod(T x, T m) {
    return extgcd(x, m).second;
}
// ============

template <unsigned mod>
struct ModInt {
    static_assert(mod != 0, "`mod` must not be equal to 0.");
    static_assert(mod < (1u << 31),
                  "`mod` must be less than (1u << 31) = 2147483648.");

    unsigned val;

    static constexpr unsigned get_mod() { return mod; }

    constexpr ModInt() : val(0) {}
    template <typename T, std::enable_if_t<std::is_signed_v<T>> * = nullptr>
    constexpr ModInt(T x)
        : val((unsigned)((long long)x % (long long)mod + (x < 0 ? mod : 0))) {}
    template <typename T, std::enable_if_t<std::is_unsigned_v<T>> * = nullptr>
    constexpr ModInt(T x) : val((unsigned)(x % mod)) {}

    static constexpr ModInt raw(unsigned x) {
        ModInt<mod> ret;
        ret.val = x;
        return ret;
    }

    constexpr unsigned get_val() const { return val; }

    constexpr ModInt operator+() const { return *this; }
    constexpr ModInt operator-() const { return ModInt<mod>(0u) - *this; }

    constexpr ModInt &operator+=(const ModInt &rhs) {
        val += rhs.val;
        if (val >= mod) val -= mod;
        return *this;
    }
    constexpr ModInt &operator-=(const ModInt &rhs) {
        val -= rhs.val;
        if (val >= mod) val += mod;
        return *this;
    }
    constexpr ModInt &operator*=(const ModInt &rhs) {
        val = (unsigned long long)val * rhs.val % mod;
        return *this;
    }
    constexpr ModInt &operator/=(const ModInt &rhs) {
        val = (unsigned long long)val * rhs.inv().val % mod;
        return *this;
    }

    friend constexpr ModInt operator+(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) += rhs;
    }
    friend constexpr ModInt operator-(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) -= rhs;
    }
    friend constexpr ModInt operator*(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) *= rhs;
    }
    friend constexpr ModInt operator/(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) /= rhs;
    }

    constexpr ModInt pow(unsigned long long x) const {
        ModInt<mod> ret = ModInt<mod>::raw(1);
        ModInt<mod> self = *this;
        while (x != 0) {
            if (x & 1) ret *= self;
            self *= self;
            x >>= 1;
        }
        return ret;
    }
    constexpr ModInt inv() const {
        static_assert(is_prime(mod), "`mod` must be a prime number.");
        assert(val != 0);
        return this->pow(mod - 2);
    }

    friend std::istream &operator>>(std::istream &is, ModInt<mod> &x) {
        long long val;
        is >> val;
        x.val = val % mod + (val < 0 ? mod : 0);
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const ModInt<mod> &x) {
        os << x.val;
        return os;
    }

    friend bool operator==(const ModInt &lhs, const ModInt &rhs) {
        return lhs.val == rhs.val;
    }

    friend bool operator!=(const ModInt &lhs, const ModInt &rhs) {
        return lhs.val != rhs.val;
    }
};

template <unsigned mod>
void debug(ModInt<mod> x) {
    std::cerr << x.val;
}
// ============
// ============
#include <bits/stdc++.h>
#define OVERRIDE(a, b, c, d, ...) d
#define REP2(i, n) for (i32 i = 0; i < (i32)(n); ++i)
#define REP3(i, m, n) for (i32 i = (i32)(m); i < (i32)(n); ++i)
#define REP(...) OVERRIDE(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define PER2(i, n) for (i32 i = (i32)(n)-1; i >= 0; --i)
#define PER3(i, m, n) for (i32 i = (i32)(n)-1; i >= (i32)(m); --i)
#define PER(...) OVERRIDE(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define ALL(x) begin(x), end(x)
#define LEN(x) (i32)(x.size())
using namespace std;
using u32 = unsigned int;
using u64 = unsigned long long;
using i32 = signed int;
using i64 = signed long long;
using f64 = double;
using f80 = long double;
using pi = pair<i32, i32>;
using pl = pair<i64, i64>;
template <typename T>
using V = vector<T>;
template <typename T>
using VV = V<V<T>>;
template <typename T>
using VVV = V<V<V<T>>>;
template <typename T>
using VVVV = V<V<V<V<T>>>>;
template <typename T>
using PQR = priority_queue<T, V<T>, greater<T>>;
template <typename T>
bool chmin(T &x, const T &y) {
    if (x > y) {
        x = y;
        return true;
    }
    return false;
}
template <typename T>
bool chmax(T &x, const T &y) {
    if (x < y) {
        x = y;
        return true;
    }
    return false;
}
template <typename T>
i32 lob(const V<T> &arr, const T &v) {
    return (i32)(lower_bound(ALL(arr), v) - arr.begin());
}
template <typename T>
i32 upb(const V<T> &arr, const T &v) {
    return (i32)(upper_bound(ALL(arr), v) - arr.begin());
}
template <typename T>
V<i32> argsort(const V<T> &arr) {
    V<i32> ret(arr.size());
    iota(ALL(ret), 0);
    sort(ALL(ret), [&](i32 i, i32 j) -> bool {
        if (arr[i] == arr[j]) {
            return i < j;
        } else {
            return arr[i] < arr[j];
        }
    });
    return ret;
}
#ifdef INT128
using u128 = __uint128_t;
using i128 = __int128_t;
#endif
[[maybe_unused]] constexpr i32 INF = 1000000100;
[[maybe_unused]] constexpr i64 INF64 = 3000000000000000100;
struct SetUpIO {
    SetUpIO() {
#ifdef FAST_IO
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
#endif
        cout << fixed << setprecision(15);
    }
} set_up_io;
void scan(char &x) { cin >> x; }
void scan(u32 &x) { cin >> x; }
void scan(u64 &x) { cin >> x; }
void scan(i32 &x) { cin >> x; }
void scan(i64 &x) { cin >> x; }
void scan(string &x) { cin >> x; }
template <typename T>
void scan(V<T> &x) {
    for (T &ele : x) {
        scan(ele);
    }
}
void read() {}
template <typename Head, typename... Tail>
void read(Head &head, Tail &...tail) {
    scan(head);
    read(tail...);
}
#define CHAR(...)     \
    char __VA_ARGS__; \
    read(__VA_ARGS__);
#define U32(...)     \
    u32 __VA_ARGS__; \
    read(__VA_ARGS__);
#define U64(...)     \
    u64 __VA_ARGS__; \
    read(__VA_ARGS__);
#define I32(...)     \
    i32 __VA_ARGS__; \
    read(__VA_ARGS__);
#define I64(...)     \
    i64 __VA_ARGS__; \
    read(__VA_ARGS__);
#define STR(...)        \
    string __VA_ARGS__; \
    read(__VA_ARGS__);
#define VEC(type, name, size) \
    V<type> name(size);       \
    read(name);
#define VVEC(type, name, size1, size2)    \
    VV<type> name(size1, V<type>(size2)); \
    read(name);
// ============
// ============
#include <cstdio>
#include <cstring>
#include <type_traits>
#include <utility>

// unable to read INT_MIN (int), LLONG_MIN (long long)
class Reader {
    FILE *fp;
    static constexpr int BUF = 1 << 18;
    char buf[BUF];
    char *pl, *pr;

    void reread() {
        int wd = pr - pl;
        std::memcpy(buf, pl, wd);
        pl = buf;
        pr = buf + wd;
        pr += std::fread(pr, 1, BUF - wd, fp);
    }

    char skip() {
        char ch = *pl++;
        while (ch <= ' ') {
            ch = *pl++;
        }
        return ch;
    }

    template <typename T>
    void read_unsigned(T &x) {
        if (pr - pl < 64) {
            reread();
        }
        x = 0;
        char ch = skip();
        while ('0' <= ch) {
            x = 10 * x + (0xf & ch);
            ch = *pl++;
        }
    }
    template <typename T>
    void read_signed(T &x) {
        if (pr - pl < 64) {
            reread();
        }
        x = 0;
        bool neg = false;
        char ch = skip();
        if (ch == '-') {
            ch = *pl++;
            neg = true;
        }
        while ('0' <= ch) {
            x = 10 * x + (0xf & ch);
            ch = *pl++;
        }
        if (neg) {
            x = -x;
        }
    }

    void read_single(int &x) { read_signed(x); }
    void read_single(unsigned &x) { read_unsigned(x); }
    void read_single(long &x) { read_signed(x); }
    void read_single(unsigned long &x) { read_signed(x); }
    void read_single(long long &x) { read_signed(x); }
    void read_single(unsigned long long &x) { read_unsigned(x); }

public:
    Reader(FILE *fp) : fp(fp), pl(buf), pr(buf) { reread(); }

    void read() {}
    template <typename Head, typename... Tail>
    void read(Head &head, Tail &...tail) {
        read_single(head);
        read(tail...);
    }
};

struct NumberToString {
    char buf[10000][4];
    constexpr NumberToString() : buf() {
        for (int i = 0; i < 10000; ++i) {
            int n = i;
            for (int j = 3; j >= 0; --j) {
                buf[i][j] = '0' + n % 10;
                n /= 10;
            }
        }
    }
} constexpr number_to_string_precalc;

class Writer {
    FILE *fp;
    static constexpr int BUF = 1 << 18;
    char buf[BUF];
    char *ptr;

    void write_u32(unsigned x) {
        if ((buf + BUF - ptr) < 32) {
            flush();
        }
        static char sml[12];
        int t = 8;
        while (x >= 10000) {
            unsigned n = x % 10000;
            x /= 10000;
            std::memcpy(sml + t, number_to_string_precalc.buf[n], 4);
            t -= 4;
        }
        if (x >= 1000) {
            std::memcpy(ptr, number_to_string_precalc.buf[x], 4);
            ptr += 4;
        } else if (x >= 100) {
            std::memcpy(ptr, number_to_string_precalc.buf[x] + 1, 3);
            ptr += 3;
        } else if (x >= 10) {
            unsigned q = (x * 103) >> 10;
            *ptr++ = q | '0';
            *ptr++ = (x - 10 * q) | '0';
        } else {
            *ptr++ = '0' | x;
        }
        std::memcpy(ptr, sml + (t + 4), 8 - t);
        ptr += 8 - t;
    }

    void write_u64(unsigned long long x) {
        if ((buf + BUF - ptr) < 32) {
            flush();
        }
        if (x >= 10000000000000000) {
            unsigned long long z = x % 100000000;
            x /= 100000000;
            unsigned long long y = x % 100000000;
            x /= 100000000;
            if (x >= 1000) {
                std::memcpy(ptr, number_to_string_precalc.buf[x], 4);
                ptr += 4;
            } else if (x >= 100) {
                std::memcpy(ptr, number_to_string_precalc.buf[x] + 1, 3);
                ptr += 3;
            } else if (x >= 10) {
                unsigned q = (x * 103) >> 10;
                *ptr++ = q | '0';
                *ptr++ = (x - 10 * q) | '0';
            } else {
                *ptr++ = '0' | x;
            }
            std::memcpy(ptr, number_to_string_precalc.buf[y / 10000], 4);
            std::memcpy(ptr + 4, number_to_string_precalc.buf[y % 10000], 4);
            std::memcpy(ptr + 8, number_to_string_precalc.buf[z / 10000], 4);
            std::memcpy(ptr + 12, number_to_string_precalc.buf[z % 10000], 4);
            ptr += 16;
        } else {
            static char sml[12];
            int t = 8;
            while (x >= 10000) {
                unsigned long long n = x % 10000;
                x /= 10000;
                std::memcpy(sml + t, number_to_string_precalc.buf[n], 4);
                t -= 4;
            }
            if (x >= 1000) {
                std::memcpy(ptr, number_to_string_precalc.buf[x], 4);
                ptr += 4;
            } else if (x >= 100) {
                std::memcpy(ptr, number_to_string_precalc.buf[x] + 1, 3);
                ptr += 3;
            } else if (x >= 10) {
                unsigned q = (x * 103) >> 10;
                *ptr++ = q | '0';
                *ptr++ = (x - 10 * q) | '0';
            } else {
                *ptr++ = '0' | x;
            }
            std::memcpy(ptr, sml + (t + 4), 8 - t);
            ptr += 8 - t;
        }
    }

    void write_char(char c) {
        if (ptr == buf + BUF) {
            flush();
        }
        *ptr++ = c;
    }

    template <typename T>
    void write_unsigned(T x) {
        if constexpr (std::is_same_v<T, unsigned long long> ||
                      std::is_same_v<T, unsigned long>) {
            write_u64(x);
        } else {
            write_u32(x);
        }
    }

    template <typename T>
    void write_signed(T x) {
        std::make_unsigned_t<T> y = x;
        if (x < 0) {
            write_char('-');
            y = -y;
        }
        write_unsigned(y);
    }

    void write_single(int x) { write_signed(x); }
    void write_single(unsigned x) { write_unsigned(x); }
    void write_single(long x) { write_signed(x); }
    void write_single(unsigned long x) { write_unsigned(x); }
    void write_single(long long x) { write_signed(x); }
    void write_single(unsigned long long x) { write_unsigned(x); }
    void write_single(char c) { write_char(c); }

public:
    Writer(FILE *fp) : fp(fp), ptr(buf) {}
    ~Writer() { flush(); }

    void flush() {
        std::fwrite(buf, 1, ptr - buf, fp);
        ptr = buf;
    }

    void write() {}
    template <typename Head, typename... Tail>
    void write(Head &&head, Tail &&...tail) {
        write_single(head);
        if (sizeof...(Tail)) {
            write_char(' ');
        }
        write(std::forward<Tail>(tail)...);
    }

    template <typename... T>
    void writeln(T &&...t) {
        write(std::forward<T>(t)...);
        write_char('\n');
    }
};

Reader rd(stdin);
Writer wr(stdout);
// ============

int main() {
    using M = ModInt<998244353>;
    i32 n, k;
    rd.read(n, k);
    V<M> f(n);
    REP(i, k) {
        i32 pos;
        M val;
        rd.read(pos, val.val);
        f[pos] = val;
    }
    V<M> g = fps_log_sparse(f);
    REP(i, n) {
        wr.write(g[i].val);
        wr.write(" \n"[i + 1 == n]);
    }
}
