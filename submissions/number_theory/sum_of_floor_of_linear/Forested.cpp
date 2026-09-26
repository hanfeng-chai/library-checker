// ============
#include <cstdio>
#include <cstring>
#include <string>
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
    
    void write_string(const std::string &s) {
        for (char c : s) {
            write_char(c);
        }
    }

    void write_single(int x) { write_signed(x); }
    void write_single(unsigned x) { write_unsigned(x); }
    void write_single(long x) { write_signed(x); }
    void write_single(unsigned long x) { write_unsigned(x); }
    void write_single(long long x) { write_signed(x); }
    void write_single(unsigned long long x) { write_unsigned(x); }
    void write_single(char c) { write_char(c); }
    void write_single(const std::string &s) { write_string(s); }

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
void scan(f64 &x) { cin >> x; }
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
#define F64(...)     \
    f64 __VA_ARGS__; \
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

using ll = long long;

ll floor_sum(ll n, ll m, ll a, ll b) {
    ll ans = 0;
    while (n) {
        ans += n * (n - 1) / 2 * (a / m) + b / m * n;
        a %= m;
        b %= m;
        ll mx = a * n + b;
        if (mx < m) break;
        n = mx / m;
        b = mx % m;
        swap(a, m);
    }
    return ans;
}

void solve() {
    i32 n, m, a, b;
    rd.read(n, m, a, b);
    wr.writeln(floor_sum(n, m, a, b));
}

int main() {
    i32 t = 1;
    rd.read(t);
    while (t--) {
        solve();
    }
}
