#line 1 "/home/andyli/lib/test/library_checker/frequency_table_of_tree_distance.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"

#line 2 "/home/andyli/lib/all.hpp"
#if defined(LX_LOCAL) && !defined(CPH)
#include <timer.hpp>
#endif
#line 2 "/home/andyli/lib/types.hpp"
#include <bits/stdc++.h>

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = unsigned __int128;
using usize = std::uintptr_t;
using isize = std::intptr_t;
using i8 = signed char;
using i16 = short;
using i32 = int;
using i64 = long long;
using i128 = __int128;
using f64 = double;
using ld = long double;
#define vc std::vector
template <typename T>
using vvc = vc<vc<T>>;
template <typename T>
using vvvc = vc<vvc<T>>;
using vi = vc<int>;
using vvi = vc<vi>;
using vvvi = vc<vvi>;
using vcb = vc<u8>;
using pi = std::pair<int, int>;
using str = std::string;
template <typename T>
using PQ = std::priority_queue<T>;
template <typename T>
using PQG = std::priority_queue<T, vc<T>, std::greater<>>;
template <typename T>
struct make_unsigned: public std::make_unsigned<T> {};
template <>
struct make_unsigned<i128> {
    using type = u128;
};
template <typename T>
using make_unsigned_t = make_unsigned<T>::type;
template <typename T>
struct make_signed: public std::make_signed<T> {};
template <>
struct make_signed<u128> {
    using type = i128;
};
template <typename T>
using make_signed_t = make_signed<T>::type;
template <typename T>
concept tupleLike = requires { typename std::tuple_element_t<0, std::decay_t<T>>; } && !requires (T t) { t[0]; };
template <typename T>
concept Signed = std::signed_integral<T> || std::is_same_v<T, i128>;
template <typename T>
concept Unsigned = std::unsigned_integral<T> || std::is_same_v<T, u128>;
template <typename T>
concept Integer = Signed<T> || Unsigned<T>;
template <typename T>
concept Modint = requires { T::mod(); };
template <typename T>
concept StaticModint = requires { typename std::integral_constant<decltype(T::mod()), T::mod()>; };
#line 3 "/home/andyli/lib/template.hpp"
using namespace std::ranges;
using namespace std::literals;

// NOLINTBEGIN
#define CONC(a, b) CONC_(a, b)
#define CONC_(a, b) a##b
#define GET0(a, ...) a
#define GET1(a, b, ...) b
#define GET2(a, b, c, ...) c
#define GET3(a, b, c, d, ...) d
#define GET4(a, b, c, d, e, ...) e
#define GET5(a, b, c, d, e, f, ...) f
#define GET_LAST(...) CONC(GET, NARGS(__VA_ARGS__))(, __VA_ARGS__, , , , , , )
#define DROP_LAST1(a)
#define DROP_LAST2(a, b) a
#define DROP_LAST3(a, b, c) a, b
#define DROP_LAST4(a, b, c, d) a, b, c
#define DROP_LAST5(a, b, c, d, e) a, b, c, d
#define DROP_LAST(...) CONC(DROP_LAST, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define NARGS(...) GET5(__VA_ARGS__, 5, 4, 3, 2, 1, 0)
#define FOR1(a) for (std::decay_t<decltype(a)> _ = 0; _ < (a); _++)
#define FOR2(i, a) for (std::decay_t<decltype(a)> i = 0; i < (a); i++)
#define FOR3(i, a, b) for (auto i = (a); i < (b); i++)
#define FOR4(i, a, b, c) for (auto i = (a); i < (b); i += (c))
#define FOR2_R(i, a) for (auto i = (a); i--;)
#define FOR3_R(i, a, b) for (auto i = (b); i-- > (a);)
#define _for(...) GET4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define _for_r(...) GET3(__VA_ARGS__, FOR3_R, FOR2_R)(__VA_ARGS__)
#define M_ID(...) __VA_ARGS__
#define M_BRACKET(...) [__VA_ARGS__]
#define FOREACH_HELPER(...) GET4(__VA_ARGS__, M_BRACKET, M_BRACKET, M_BRACKET, M_ID)(__VA_ARGS__)
#define foreach(...) for (auto&& FOREACH_HELPER(DROP_LAST(__VA_ARGS__)): GET_LAST(__VA_ARGS__))
#define loop while (true)
[[maybe_unused]] struct {
    constexpr auto operator->*(auto&& f) const { return f(); }
} blk;
#define BLK blk->*[&]
#define lowbit(x) ((x) & (-(x)))
#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
#define LB(c, ...) distance(begin(c), lower_bound(c, __VA_ARGS__))
#define UB(c, ...) distance(begin(c), upper_bound(c, __VA_ARGS__))
#define UNIQUE(c) sort(c), (c).erase(std::unique(all(c)), end(c))
#define VEC(type, a, ...) auto a = vec<type>(__VA_ARGS__)
#define VECI(a, ...) auto a = veci(__VA_ARGS__)
#define FORWARD(x) std::forward<decltype(x)>(x)
#define eb emplace_back
#if defined(LX_LOCAL) || defined(ASSERTIONS)
#define ASSERT(...) assert(__VA_ARGS__)
#else
#define ASSERT(...) void()
#endif
// NOLINTEND

constexpr auto floor(auto&& x, auto&& y) { return x / y - (x % y && (x ^ y) < 0); }
constexpr auto ceil(auto&& x, auto&& y) { return floor(x + y - 1, y); }
constexpr auto divmod(auto x, auto y) {
    auto&& q = floor(x, y);
    return std::pair{q, x - q * y};
}
template <typename T = u64>
constexpr T ten(int t) { return t == 0 ? 1 : ten<T>(t - 1) * 10; }
constexpr int get_lg(auto n) { return n <= 1 ? 1 : std::__bit_width(n - 1); }
constexpr auto Max(const auto& x) { return x; }
constexpr auto Min(const auto& x) { return x; }
constexpr auto Max(const auto& x, const auto& y, const auto&... arg) { return x < y ? Max(y, arg...) : Max(x, arg...); }
constexpr auto Min(const auto& x, const auto& y, const auto&... arg) { return x < y ? Min(x, arg...) : Min(y, arg...); }
constexpr bool chkmax(auto& d, const auto&... x) {
    auto t = Max(x...);
    return d < t && (d = t, true);
}
constexpr bool chkmin(auto& d, const auto&... x) {
    auto t = Min(x...);
    return d > t && (d = t, true);
}
constexpr auto sum(input_range auto&& r) { return std::accumulate(all(r), range_value_t<decltype(r)>{}); }
constexpr auto sum(input_range auto&& r, auto init) { return std::accumulate(all(r), init); }
constexpr int len(auto&& x) { return size(x); }
template <typename T>
auto cumsum(const vc<T>& a) {
    int n = len(a);
    vc<T> b(n + 1);
    _for (i, n)
        b[i + 1] = b[i] + a[i];
    return b;
}
template <typename T>
auto cumsum(auto&& a) {
    int n = len(a);
    vc<T> b(n + 1);
    _for (i, n)
        b[i + 1] = b[i] + a[i];
    return b;
}
template <typename T>
auto vec(usize n, auto&&... s) {
    if constexpr (!sizeof...(s))
        return vc<T>(n);
    else
        return vc(n, vec<T>(s...));
}
auto veci(usize n, auto&&... s) {
    if constexpr (sizeof...(s) == 1)
        return vc(n, s...);
    else
        return vc(n, veci(s...));
}
template <typename T>
vi argsort(const vc<T>& a) {
    vi I(len(a));
    iota(all(I), 0);
    sort(I, [&](int i, int j) { return std::pair{a[i], i} < std::pair{a[j], j}; });
    return I;
}
template <typename T>
vc<T> rearrange(const vc<T>& a, const vi& I) {
    vc<T> b(len(a));
    _for (i, len(a))
        b[i] = a[I[i]];
    return b;
}
template <input_range R>
vi shift(R&& s, range_value_t<R> c) {
    vi a(len(s));
    _for (i, len(s))
        a[i] = s[i] - c;
    return a;
}
template <typename T>
T pop(vc<T>& q) {
    T r = std::move(q.back());
    q.pop_back();
    return r;
}
template <typename T>
T pop(std::deque<T>& q) {
    T r = std::move(q.front());
    q.pop_front();
    return r;
}
template <typename T, typename S, typename C>
T pop(std::priority_queue<T, S, C>& q) {
    T r = q.top();
    q.pop();
    return r;
}
template <typename T>
constexpr T inf = std::numeric_limits<T>::max() * 0.49;
template <>
constexpr f64 inf<f64> = inf<i64>;
template <>
constexpr ld inf<ld> = inf<i64>;
#line 3 "/home/andyli/lib/utility/itos_table.hpp"

constexpr auto itos_table = [] {
    std::array<char, 40000> O;
    char* p = O.data();
    _for (i, 10)
        _for (j, 10)
            _for (k, 10)
                _for (l, 10)
                    *p++ = 48 + i, *p++ = 48 + j, *p++ = 48 + k, *p++ = 48 + l;
    return O;
}();
#line 3 "/home/andyli/lib/io.hpp"

#ifdef LX_DEBUG
#include <io2.hpp>
#endif

#ifndef FASTIO
#define FASTIO 1

#if defined(__unix__) && !defined(LX_LOCAL)
#define USE_MMAP
#define EV 0
#include <sys/mman.h>
#include <sys/stat.h>
#else
#define EV (-1)
#endif

struct IO {
    static constexpr usize bufSize = 1 << 20;
    static constexpr bool isdigit(int c) { return '0' <= c && c <= '9'; }
    static constexpr bool blank(int c) { return c <= ' '; }

    u32 prec = 12;
    FILE *in, *out;
    char obuf[bufSize], *ip, *op = obuf;

#ifndef USE_MMAP
    char ibuf[bufSize + 8], *eip;
    bool eoi = false;
    void load() {
        if (eoi) [[unlikely]]
            return;
        usize sz = eip - ip;
        memcpy(ibuf, ip, sz);
        eip = ibuf + sz + fread(ibuf + sz, 1, bufSize - sz, in);
        if (eip != ibuf + bufSize) [[unlikely]]
            eoi = true;
        ip = ibuf;
    }
    void skipws() {
        int ch = getch();
        while (blank(ch))
            ch = getch();
        ip--;
    }
    int getch() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip; }
    void input(FILE* f) { in = f, ip = eip = ibuf; }
    void ireadstr(char* s, usize n) {
        if (usize len = eip - ip; n > len) [[unlikely]] {
            memcpy(s, ip, len);
            n -= len;
            s += len;
            ip = eip;
            fread(s, 1, n, in);
        }
        else
            memcpy(s, ip, n), ip += n;
    }
#else
    void skipws() {
        while (blank(*ip))
            ip++;
    }
    int getch() { return *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return *ip; }
    void input(FILE* f) {
        struct stat st;
        int fd;
        in = f;
        if (in)
            fd = fileno(in), fstat(fd, &st), ip = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    }
    void ireadstr(char* s, usize n) { memcpy(s, ip, n), ip += n; }
#endif
    void input(std::string_view s) { input(fopen(s.data(), "rb")); }
    void set(bool = true) {}
    IO(FILE* i = stdin, FILE* o = stdout) { input(i), output(o); }
    ~IO() { flush(); }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    IO& read(Args&... x) {
        (read(x), ...);
        return *this;
    }
    template <Unsigned T>
    void parse_int(T& x) {
        loop {
            u64 v;
            memcpy(&v, ip, 8);
            if ((v -= 0x3030303030303030) & 0x8080808080808080)
                break;
            v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0xffffffff;
            x = 100000000 * x + v;
            ip += 8;
        }
        {
            u32 v;
            memcpy(&v, ip, 4);
            if (!((v -= 0x30303030) & 0x80808080)) {
                v = (v * 10 + (v >> 8)) & 0xff00ff;
                v = (v * 100 + (v >> 16)) & 0xffff;
                x = 10000 * x + v;
                ip += 4;
            }
        }
        {
            u16 v;
            memcpy(&v, ip, 2);
            if (!((v -= 0x3030) & 0x8080)) {
                v = (v * 10 + (v >> 8)) & 0xff;
                x = 100 * x + v;
                ip += 2;
            }
        }
        if (isdigit(*ip))
            x = 10 * x + (*ip++ ^ 48);
    }
    template <Signed T>
    IO& read(T& x) {
        skipws();
#ifndef USE_MMAP
        if (eip - ip < 64) [[unlikely]]
            load();
#endif
        make_unsigned_t<T> t{};
        if (*ip == '-') {
            ip++;
            parse_int(t);
            t = -t;
        }
        else
            parse_int(t);
        x = t;
        return *this;
    }
    IO& read(Unsigned auto& x) {
        x = 0;
        skipws();
#ifndef USE_MMAP
        if (eip - ip < 64) [[unlikely]]
            load();
#endif
        parse_int(x);
        return *this;
    }
    IO& read(std::floating_point auto& x) {
        static str s;
        if (read(s))
            std::from_chars(s.begin().base(), s.end().base(), x);
        return *this;
    }
    template <typename T = int>
    T read() {
        std::decay_t<T> x;
        return read(x), x;
    }
    IO& read(char& ch) {
        skipws();
        ch = getch();
        return *this;
    }
#ifdef USE_MMAP
    usize next_size() const {
        char* ip = this->ip;
        while (!blank(*ip))
            ip++;
        return ip - this->ip;
    }
    IO& read(char* s) {
        skipws();
        auto n = next_size();
        ireadstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        auto n = next_size();
        s.assign(ip, n);
        ip += n;
        return *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.assign(ip, n);
        ip += n;
        return *this;
    }
#else
    IO& read(char* s) {
        skipws();
        int ch = peek();
        while (!blank(ch))
            *s++ = ch, getch_unchecked(), ch = peek();
        *s = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        int ch = peek();
        s.clear();
        while (!blank(ch))
            s.push_back(ch), getch_unchecked(), ch = peek();
        return *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.resize(n);
        ireadstr(s.data(), n);
        return *this;
    }
#endif
    IO& readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& readline(char* s) {
        int ch = getch();
        while (ch != '\n' && ch != EV)
            *s++ = ch, ch = getch();
        *s = 0;
        return *this;
    }
    IO& readline(str& s) {
        s.clear();
        int ch = getch();
        while (ch != '\n' && ch != EV)
            s.push_back(ch), ch = getch();
        return *this;
    }
    IO& read(tupleLike auto& t) {
        return std::apply([&](auto&... t) { read(t...); }, t), *this;
    }
    IO& read(forward_range auto&& r) { return readArray(FORWARD(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.read(io); }
    IO& read(T& t) { return t.read(*this), *this; }
    template <std::forward_iterator I>
    IO& readArray(I f, I l) {
        while (f != l)
            read(*f++);
        return *this;
    }
    IO& readArray(forward_range auto&& r) { return readArray(all(r)); }
    IO& zipread(auto&&... a) {
        _for (i, (len(a), ...))
            read(a[i]...);
        return *this;
    }

    void flush() { fwrite(obuf, 1, op - obuf, out), op = obuf; }
    void putch_unchecked(char c) { *op++ = c; }
    void putch(char c) { (op == end(obuf) ? flush() : void()), putch_unchecked(c); }
    void writestr(const char* s, usize n) {
        if (n >= usize(end(obuf) - op)) [[unlikely]]
            flush(), fwrite(s, 1, n, out);
        else
            memcpy(op, s, n), op += n;
    }
    void output(std::string_view s) { output(fopen(s.data(), "wb")); }
    void output(FILE* f) { out = f; }
    void setprec(u32 n = 6) { prec = n; }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    void write(Args&&... x) { (write(FORWARD(x)), ...); }
    void write() {}

    struct WriteInt {
        IO& io;
        template <int N = 4>
        void lead(u64 x) {
            if constexpr (N > 1)
                if (x < ten(N - 1)) {
                    lead<N - 1>(x);
                    return;
                }
            io.op = std::copy_n(&itos_table[x * 4 + (4 - N)], N, io.op);
        }
        template <int N>
        void wt4(u64 x) {
            if constexpr (N > 0) {
                io.op = std::copy_n(&itos_table[x / ten(N - 4) * 4], 4, io.op);
                wt4<N - 4>(x % ten(N - 4));
            }
        }
        template <int N = 4>
        void wt(u64 x) {
            if constexpr (N < 20)
                if (ten(N) <= x) {
                    wt<N + 4>(x);
                    return;
                }
            lead(x / ten(N - 4));
            wt4<N - 4>(x % ten(N - 4));
        }
        void write(std::unsigned_integral auto x) { wt(x); }
        void write(u128 x) {
            if (x < ten<u128>(16))
                wt(x);
            else if (x < ten<u128>(32)) {
                wt(x / ten<u128>(16));
                wt4<16>(x % ten<u128>(16));
            }
            else {
                wt(x / ten<u128>(32));
                x %= ten<u128>(32);
                wt4<16>(x / ten<u128>(16));
                wt4<16>(x % ten<u128>(16));
            }
        }
    };
    template <Signed T>
    void write(T x) {
        if (end(obuf) - op < 64) [[unlikely]]
            flush();
        make_unsigned_t<T> y = x;
        if (x < 0)
            *op++ = '-', y = -y;
        WriteInt{*this}.write(y);
    }
    void write(Unsigned auto x) {
        if (end(obuf) - op < 64) [[unlikely]]
            flush();
        WriteInt{*this}.write(x);
    }
    void write(char c) { putch(c); }
    void write(std::floating_point auto x) {
        static char buf[512];
        writestr(buf, std::to_chars(buf, buf + 512, x, std::chars_format::fixed, prec).ptr - buf);
    }
    void write(std::string_view s) { writestr(s.data(), s.size()); }
    template <typename I, typename T = std::iter_value_t<I>>
    static constexpr char default_delim = tupleLike<T> || input_range<T> ? '\n' : ' ';
    template <std::input_iterator I, std::sentinel_for<I> S>
    void print_range(I f, S l, char d = default_delim<I>) {
        if (f != l)
            for (write(*f++); f != l; write(d, *f++)) {}
    }
    template <tupleLike T>
    void write(T&& t) {
        std::apply([&](auto&& x, auto&&... y) { write(FORWARD(x)), (write(' ', FORWARD(y)), ...); }, FORWARD(t));
    }
    template <input_range R>
    requires (!std::same_as<range_value_t<R>, char>)
    void write(R&& r) { print_range(all(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.write(io); }
    void write(T&& t) { t.write(*this); }
    void writeln(auto&&... x) { write(FORWARD(x)...), print(); }
    void print() { putch('\n'); }
    void print(auto&&... x) { write(std::forward_as_tuple(FORWARD(x)...), '\n'); }
    template <std::input_iterator I, std::sentinel_for<I> S>
    void displayArray(I f, S l, char d = default_delim<I>) { print_range(f, l, d), print(); }
    template <input_range R>
    void displayArray(R&& r, char d = default_delim<iterator_t<R>>) { displayArray(all(r), d); }
    operator bool() const { return true; }
} io;
#define dR(type, ...) \
    type __VA_ARGS__; \
    io.read(__VA_ARGS__)
#define dRV(type, a, ...)      \
    VEC(type, a, __VA_ARGS__); \
    io.read(a)
#define STR(s, n) \
    str s;        \
    io.readstr(s, n)
void multipleTests(auto&& f) {
    dR(u32, q);
    _for (q)
        f();
}
void writeln(auto&&... x) { io.writeln(FORWARD(x)...); }
void print(auto&&... x) { io.print(FORWARD(x)...); }
void YES(bool v = true) { io.write(v ? "YES\n" : "NO\n"); }
inline void NO(bool v = true) { YES(!v); }
void Yes(bool v = true) { io.write(v ? "Yes\n" : "No\n"); }
inline void No(bool v = true) { Yes(!v); }
#endif
#line 2 "/home/andyli/lib/poly/ntt_avx2.hpp"
#include <immintrin.h>
#line 3 "/home/andyli/lib/math/primitive_root_constexpr.hpp"

constexpr u32 primitive_root_constexpr(u32 mod) {
    if (mod == 2)
        return 1;
    u64 ds[32]{};
    int idx = 0;
    u64 m = mod - 1;
    for (u64 i = 2; i * i <= m; i++)
        if (m % i == 0) {
            ds[idx++] = i;
            while (m % i == 0)
                m /= i;
        }
    if (m != 1)
        ds[idx++] = m;
    u32 pr = 1;
    loop {
        pr++;
        bool ok = true;
        _for (i, idx) {
            u64 a = pr, b = (mod - 1) / ds[i], r = 1;
            while (b) {
                if (b & 1)
                    r = r * a % mod;
                a = a * a % mod;
                b >>= 1;
            }
            if (r == 1) {
                ok = false;
                break;
            }
        }
        if (ok)
            return pr;
    }
}
#line 3 "/home/andyli/lib/utility/make_double_width.hpp"

template <typename T>
struct make_double_width {};
template <>
struct make_double_width<u8> {
    using type = u16;
};
template <>
struct make_double_width<u16> {
    using type = u32;
};
template <>
struct make_double_width<u32> {
    using type = u64;
};
template <>
struct make_double_width<u64> {
    using type = u128;
};
template <typename T>
using make_double_width_t = make_double_width<T>::type;
#line 3 "/home/andyli/lib/modint/montgomery_reduction.hpp"

template <typename T>
struct MontgomeryReduction {
    using int_type = T;
    using int_double_t = make_double_width_t<T>;
    static constexpr int base_width = std::numeric_limits<T>::digits;

    constexpr explicit MontgomeryReduction(T mod): _mod(mod), _mod2(mod * 2), _mod_neg_inv(inv_base(-mod)), _mbase((int_double_t(1) << base_width) % mod), _mbase2(int_double_t(_mbase) * _mbase % mod), _mbase3(int_double_t(_mbase2) * _mbase % mod) {}

    constexpr T mod() const { return _mod; }
    constexpr T mod2() const { return _mod2; }
    constexpr T mod_neg_inv() const { return _mod_neg_inv; }
    constexpr T mbase() const { return _mbase; }
    constexpr T mbase2() const { return _mbase2; }
    constexpr T mbase3() const { return _mbase3; }
    constexpr T reduce(int_double_t t) const { return T((t + int_double_t(T(t) * _mod_neg_inv) * _mod) >> base_width); }
    constexpr T shrink(T x) const { return x >= _mod2 ? x - _mod2 : x; }
    constexpr T strict_shrink(T x) const { return x >= _mod ? x - _mod : x; }
    static constexpr T inv_base(T x) {
        T y = 1;
        for (int i = 1; i < base_width; i *= 2)
            y *= 2 - x * y;
        return y;
    }

private:
    T _mod, _mod2, _mod_neg_inv, _mbase, _mbase2, _mbase3;
};
#line 5 "/home/andyli/lib/poly/ntt_avx2.hpp"

#define NTT_AVX2

// https://judge.yosupo.jp/submission/199421
using I256 = __m256i;
void store256(void* p, I256 x) { _mm256_store_si256((I256*)p, x); }
I256 load256(const void* p) { return _mm256_load_si256((const I256*)p); }
I256 shrk32(I256 x, I256 M) { return _mm256_min_epu32(x, _mm256_sub_epi32(x, M)); }
I256 dilt32(I256 x, I256 M) { return _mm256_min_epu32(x, _mm256_add_epi32(x, M)); }
I256 Ladd32(I256 x, I256 y, I256) { return _mm256_add_epi32(x, y); }
I256 Lsub32(I256 x, I256 y, I256 M) { return _mm256_add_epi32(_mm256_sub_epi32(x, y), M); }
I256 add32(I256 x, I256 y, I256 M) { return shrk32(_mm256_add_epi32(x, y), M); }
I256 sub32(I256 x, I256 y, I256 M) { return dilt32(_mm256_sub_epi32(x, y), M); }
I256 reduce(I256 a, I256 b, I256 niv, I256 M) {
    I256 c = _mm256_mul_epu32(a, niv), d = _mm256_mul_epu32(b, niv);
    c = _mm256_mul_epu32(c, M), d = _mm256_mul_epu32(d, M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(a, c), 32), _mm256_add_epi64(b, d), 0xaa);
}
I256 mul(I256 a, I256 b, I256 niv, I256 M) {
    return reduce(_mm256_mul_epu32(a, b), _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32)), niv, M);
}
I256 mul_bsm(I256 a, I256 b, I256 niv, I256 M) {
    return reduce(_mm256_mul_epu32(a, b), _mm256_mul_epu32(_mm256_srli_epi64(a, 32), b), niv, M);
}
I256 mul_bsmfxd(I256 a, I256 b, I256 bniv, I256 M) {
    I256 cc = _mm256_mul_epu32(a, bniv), dd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), bniv);
    I256 c = _mm256_mul_epu32(a, b), d = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), b);
    cc = _mm256_mul_epu32(cc, M), dd = _mm256_mul_epu32(dd, M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(c, cc), 32), _mm256_add_epi64(d, dd), 0xaa);
}
I256 mul_upd_rt(I256 a, I256 bu, I256 M) {
    I256 cc = _mm256_mul_epu32(a, bu), c = _mm256_mul_epu32(a, _mm256_srli_epi64(bu, 32));
    cc = _mm256_mul_epu32(cc, M);
    return shrk32(_mm256_srli_epi64(_mm256_add_epi64(c, cc), 32), M);
}

constexpr auto _mxlg = 26, _lg_itth = 6;
constexpr auto _itth = usize(1) << _lg_itth;
static_assert(_lg_itth % 2 == 0);

template <typename mint>
struct NTT_Info {
    static constexpr u32 mod = mint::mod();
    static constexpr MontgomeryReduction<u32> mr{mod};
    static constexpr mint pr = primitive_root_constexpr(mod);
    static constexpr int lvl = __builtin_ctz(mod - 1);

    u32 img{}, imgniv{};
    mint RT1[_mxlg]{};
    alignas(32) std::array<u32, 8> rt3[_mxlg - 2]{}, rt3i[_mxlg - 2]{}, bwbr{}, bwb{}, bwbi{}, rt4[_mxlg - 3]{}, rt4niv[_mxlg - 3]{}, rt4i[_mxlg - 3]{}, rt4iniv[_mxlg - 3]{}, pr2{}, pr4{}, pr2niv{}, pr4niv{}, pr2i{}, pr2iniv{}, pr4i{}, pr4iniv{};
    constexpr NTT_Info() {
        struct {
            std::array<mint, 8> rt4;
            std::array<mint, 8> rt4i;
        } tmp{};
        constexpr u32 niv = mr.mod_neg_inv();
        mint w[_mxlg - 1], y[_mxlg - 1];
        w[lvl - 2] = power(pr, (mod - 1) >> lvl), y[lvl - 2] = w[lvl - 2].inv();
        _for_r (i, lvl - 2) {
            w[i] = w[i + 1] * w[i + 1];
            y[i] = y[i + 1] * y[i + 1];
        }
        RT1[lvl - 1] = power(w[lvl - 2], 3);
        _for_r (i, lvl - 1)
            RT1[i] = RT1[i + 1] * RT1[i + 1];
        img = w[0].residue(), imgniv = img * niv;
        assignr(bwbr, 1, 0, 1, 0, 1);
        assignr(bwb, w[1], 0, w[0], 0, -w[0] * w[1]);
        assignr(bwbi, y[1], 0, y[0], 0, y[0] * y[1]);
        mint pr = 1, pri = 1;
        _for (i, lvl - 2) {
            const mint r = pr * w[i + 1], ri = pri * y[i + 1];
            const mint r2 = r * r, r2i = ri * ri;
            const mint r3 = r * r2, r3i = ri * r2i;
            rt3[i] = {r.residue() * niv, r.residue(), r2.residue() * niv, r2.residue(), r3.residue() * niv, r3.residue()};
            rt3i[i] = {ri.residue() * niv, ri.residue(), r2i.residue() * niv, r2i.residue(), r3i.residue() * niv, r3i.residue()};
            pr = pr * y[i + 1], pri = pri * w[i + 1];
        }
        pr = pri = 1;
        _for (i, lvl - 3) {
            const mint r = pr * w[i + 2], ri = pri * y[i + 2];
            tmp.rt4[0] = tmp.rt4i[0] = 1;
            _for (j, 7) {
                tmp.rt4[j + 1] = tmp.rt4[j] * r;
                tmp.rt4i[j + 1] = tmp.rt4i[j] * ri;
            }
            copyr(rt4[i], tmp.rt4);
            copyr(rt4i[i], tmp.rt4i);
            _for (j, 8) {
                rt4niv[i][j] = rt4[i][j] * niv;
                rt4iniv[i][j] = rt4i[i][j] * niv;
            }
            pr = pr * y[i + 2], pri = pri * w[i + 2];
        }
        assignr(pr2, 1, 1, 1, w[0], 1, 1, 1, w[0]);
        assignr(pr4, 1, 1, 1, 1, 1, w[1], w[0], w[0] * w[1]);
        const u32 nr2 = mod - mr.mbase2(), imgr2 = mint(img).residue();
        pr2i = {nr2, nr2, nr2, imgr2, nr2, nr2, nr2, imgr2};
        assignr(pr4i, 1, 1, 1, 1, 1, y[1], y[0], y[0] * y[1]);
        _for (j, 8) {
            pr2niv[j] = pr2[j] * niv, pr4niv[j] = pr4[j] * niv;
            pr2iniv[j] = pr2i[j] * niv, pr4iniv[j] = pr4i[j] * niv;
        }
    }

private:
    static constexpr void copyr(auto&& res, auto&& src) {
        _for (i, len(res))
            res[i] = src[i].residue();
    }
    static constexpr void assignr(auto&& res, auto&&... args) { res = {mint(args).residue()...}; }
};
struct U32Aligned {
    u32* p;
    U32Aligned(usize n) { p = new (std::align_val_t{32}) u32[n]; }
    ~U32Aligned() { ::operator delete[](p, std::align_val_t{32}); }
    U32Aligned(const U32Aligned&) = delete;
    U32Aligned& operator=(const U32Aligned&) = delete;
    operator u32*() const { return (u32*)p; }
};
template <typename mint>
void vec_dif(I256* const f, const usize n) {
    static constexpr NTT_Info<mint> info;
    alignas(32) std::array<u32, 8> st_1[_mxlg >> 1];
    const I256 Mod = _mm256_set1_epi32(info.mod), Mod2 = _mm256_set1_epi32(info.mr.mod2()), Niv = _mm256_set1_epi32(info.mr.mod_neg_inv());
    const I256 Img = _mm256_set1_epi32(info.img), ImgNiv = _mm256_set1_epi32(info.imgniv), id = _mm256_setr_epi32(0, 2, 0, 4, 0, 2, 0, 4);
    const int lgn = __builtin_ctzll(n);
    fill(st_1, st_1 + (lgn >> 1), info.bwb);
    const usize nn = n >> (lgn & 1), m = min(n, _itth), mm = min(nn, _itth);
    if (nn != n) {
        _for (i, nn) {
            const auto p0 = f + i, p1 = f + nn + i;
            const auto f0 = load256(p0), f1 = load256(p1);
            const auto g0 = add32(f0, f1, Mod2), g1 = Lsub32(f0, f1, Mod2);
            store256(p0, g0), store256(p1, g1);
        }
    }
    for (usize L = nn >> 2; L > 0; L >>= 2) {
        _for (i, L) {
            const auto p0 = f + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
            const auto f1 = load256(p1), f3 = load256(p3), f2 = load256(p2), f0 = load256(p0);
            const auto g3 = mul_bsmfxd(Lsub32(f1, f3, Mod2), Img, ImgNiv, Mod), g1 = add32(f1, f3, Mod2);
            const auto g0 = add32(f0, f2, Mod2), g2 = sub32(f0, f2, Mod2);
            const auto h0 = add32(g0, g1, Mod2), h1 = Lsub32(g0, g1, Mod2);
            const auto h2 = Ladd32(g2, g3, Mod2), h3 = Lsub32(g2, g3, Mod2);
            store256(p0, h0), store256(p1, h1), store256(p2, h2), store256(p3, h3);
        }
    }
    _for (j, 0, n, m) {
        int t = ((j == 0) ? min(_lg_itth, lgn) : __builtin_ctzll(j)) & -2, p = (t - 2) >> 1;
        for (usize L = (usize(1) << t) >> 2; L >= _itth; L >>= 2, t -= 2, p--) {
            auto rt = load256(st_1 + p);
            const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
            const auto r1Niv = _mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt, Niv), id);
            rt = mul_upd_rt(rt, load256(info.rt3 + __builtin_ctzll(~j >> t)), Mod);
            const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB), nr3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
            const auto r2Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_BBBB), nr3Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_DDDD);
            store256(st_1 + p, rt);
            _for (i, L) {
                const auto p0 = f + i + j, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                const auto f1 = load256(p1), f3 = load256(p3), f2 = load256(p2), f0 = load256(p0);
                const auto g1 = mul_bsmfxd(f1, r1, r1Niv, Mod), ng3 = mul_bsmfxd(f3, nr3, nr3Niv, Mod);
                const auto g2 = mul_bsmfxd(f2, r2, r2Niv, Mod), g0 = shrk32(f0, Mod2);
                const auto h3 = mul_bsmfxd(Ladd32(g1, ng3, Mod2), Img, ImgNiv, Mod), h1 = sub32(g1, ng3, Mod2);
                const auto h0 = add32(g0, g2, Mod2), h2 = sub32(g0, g2, Mod2);
                const auto u0 = Ladd32(h0, h1, Mod2), u1 = Lsub32(h0, h1, Mod2);
                const auto u2 = Ladd32(h2, h3, Mod2), u3 = Lsub32(h2, h3, Mod2);
                store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
            }
        }
        I256* const g = f + j;
        for (usize l = mm, L = mm >> 2; L; l = L, L >>= 2, t -= 2, p--) {
            auto rt = load256(st_1 + p);
            for (usize i = (j == 0 ? l : 0), k = (j + i) >> t; i < m; i += l, k++) {
                const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
                const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB);
                const auto nr3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
                _for (j, L) {
                    const auto p0 = g + i + j, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                    const auto f1 = load256(p1), f3 = load256(p3), f2 = load256(p2), f0 = load256(p0);
                    const auto g1 = mul_bsm(f1, r1, Niv, Mod), ng3 = mul_bsm(f3, nr3, Niv, Mod);
                    const auto g2 = mul_bsm(f2, r2, Niv, Mod), g0 = shrk32(f0, Mod2);
                    const auto h3 = mul_bsmfxd(Ladd32(g1, ng3, Mod2), Img, ImgNiv, Mod), h1 = sub32(g1, ng3, Mod2);
                    const auto h0 = add32(g0, g2, Mod2), h2 = sub32(g0, g2, Mod2);
                    const auto u0 = Ladd32(h0, h1, Mod2), u1 = Lsub32(h0, h1, Mod2);
                    const auto u2 = Ladd32(h2, h3, Mod2), u3 = Lsub32(h2, h3, Mod2);
                    store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                }
                rt = mul_upd_rt(rt, load256(info.rt3 + __builtin_ctzll(~k)), Mod);
            }
            store256(st_1 + p, rt);
        }
    }
}
template <typename mint, bool shrk = false>
void vec_dit(I256* const f, usize n) {
    static constexpr NTT_Info<mint> info;
    alignas(32) std::array<u32, 8> st_1[_mxlg >> 1];
    const I256 Mod = _mm256_set1_epi32(info.mod), Mod2 = _mm256_set1_epi32(info.mr.mod2()), Niv = _mm256_set1_epi32(info.mr.mod_neg_inv());
    const I256 Img = _mm256_set1_epi32(info.img), ImgNiv = _mm256_set1_epi32(info.imgniv), id = _mm256_setr_epi32(0, 2, 0, 4, 0, 2, 0, 4);
    const int lgn = __builtin_ctzll(n);
    fill(st_1, st_1 + (_lg_itth >> 1), info.bwbr);
    fill(st_1 + (_lg_itth >> 1), st_1 + (_mxlg >> 1), info.bwbi);
    const usize nn = n >> (lgn & 1), mm = min(nn, _itth);
    _for (j, 0, n, mm) {
        I256* const g = f + j;
        int t = 2, p = 0;
        for (usize l = 4, L = 1; l <= mm; L = l, l <<= 2, t += 2, p++) {
            auto rt = load256(st_1 + p);
            for (usize i = 0, k = j >> t; i < mm; i += l, k++) {
                const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
                const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB);
                const auto r3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
                _for (j, L) {
                    const auto p0 = g + i + j, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                    const auto f0 = load256(p0), f1 = load256(p1), f2 = load256(p2), f3 = load256(p3);
                    const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                    const auto g2 = add32(f2, f3, Mod2), g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod);
                    const auto h0 = Ladd32(g0, g2, Mod2), h1 = Ladd32(g1, g3, Mod2);
                    const auto h2 = Lsub32(g0, g2, Mod2), h3 = Lsub32(g1, g3, Mod2);
                    const auto u0 = shrk32(h0, Mod2), u1 = mul_bsm(h1, r1, Niv, Mod);
                    const auto u2 = mul_bsm(h2, r2, Niv, Mod), u3 = mul_bsm(h3, r3, Niv, Mod);
                    store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                }
                rt = mul_upd_rt(rt, load256(info.rt3i + __builtin_ctzll(~k)), Mod);
            }
            store256(st_1 + p, rt);
        }
        int tt = min(__builtin_ctzll(~(j >> _lg_itth)) + _lg_itth, lgn);
        for (usize L = _itth, l = L << 2; t <= tt; L = l, l <<= 2, t += 2, p++) {
            if ((j + _itth) == l) {
                if (shrk && l == n) {
                    _for (i, L) {
                        const auto p0 = f + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                        const auto f2 = load256(p2), f3 = load256(p3), f0 = load256(p0), f1 = load256(p1);
                        const auto g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod), g2 = add32(f2, f3, Mod2);
                        const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                        const auto h0 = add32(g0, g2, Mod2), h1 = add32(g1, g3, Mod2);
                        const auto h2 = sub32(g0, g2, Mod2), h3 = sub32(g1, g3, Mod2);
                        const auto u0 = shrk32(h0, Mod), u1 = shrk32(h1, Mod);
                        const auto u2 = shrk32(h2, Mod), u3 = shrk32(h3, Mod);
                        store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                    }
                }
                else {
                    _for (i, L) {
                        const auto p0 = f + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                        const auto f2 = load256(p2), f3 = load256(p3), f0 = load256(p0), f1 = load256(p1);
                        const auto g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod), g2 = add32(f2, f3, Mod2);
                        const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                        const auto h0 = add32(g0, g2, Mod2), h1 = add32(g1, g3, Mod2);
                        const auto h2 = sub32(g0, g2, Mod2), h3 = sub32(g1, g3, Mod2);
                        store256(p0, h0), store256(p1, h1), store256(p2, h2), store256(p3, h3);
                    }
                }
            }
            else {
                auto rt = load256(st_1 + p);
                const auto r1 = _mm256_permutevar8x32_epi32(rt, id);
                const auto r1Niv = _mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt, Niv), id);
                rt = mul_upd_rt(rt, load256(info.rt3i + __builtin_ctzll(~j >> t)), Mod);
                const auto r2 = _mm256_shuffle_epi32(r1, _MM_PERM_BBBB), r3 = _mm256_shuffle_epi32(r1, _MM_PERM_DDDD);
                const auto r2Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_BBBB), r3Niv = _mm256_shuffle_epi32(r1Niv, _MM_PERM_DDDD);
                store256(st_1 + p, rt);
                _for (i, L) {
                    const auto p0 = f + j + _itth - l + i, p1 = p0 + L, p2 = p1 + L, p3 = p2 + L;
                    const auto f0 = load256(p0), f1 = load256(p1), f2 = load256(p2), f3 = load256(p3);
                    const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
                    const auto g2 = add32(f2, f3, Mod2), g3 = mul_bsmfxd(Lsub32(f3, f2, Mod2), Img, ImgNiv, Mod);
                    const auto h0 = Ladd32(g0, g2, Mod2), h1 = Ladd32(g1, g3, Mod2);
                    const auto h2 = Lsub32(g0, g2, Mod2), h3 = Lsub32(g1, g3, Mod2);
                    const auto u0 = shrk32(h0, Mod2), u1 = mul_bsmfxd(h1, r1, r1Niv, Mod);
                    const auto u2 = mul_bsmfxd(h2, r2, r2Niv, Mod), u3 = mul_bsmfxd(h3, r3, r3Niv, Mod);
                    store256(p0, u0), store256(p1, u1), store256(p2, u2), store256(p3, u3);
                }
            }
        }
    }
    if (shrk && nn == n && n <= _itth) {
        _for (i, n) {
            const auto f0 = load256(f + i);
            store256(f + i, shrk32(f0, Mod));
        }
    }
    if (nn != n) {
        _for (i, nn) {
            const auto p0 = f + i, p1 = f + nn + i;
            const auto f0 = load256(p0), f1 = load256(p1);
            const auto g0 = add32(f0, f1, Mod2), g1 = sub32(f0, f1, Mod2);
            if constexpr (shrk) {
                const auto h0 = shrk32(g0, Mod), h1 = shrk32(g1, Mod);
                store256(p0, h0), store256(p1, h1);
            }
            else {
                store256(p0, g0), store256(p1, g1);
            }
        }
    }
}
//f[0,8) = fx * f[0,8) * g[0,8) (mod x^8 - ww)
[[gnu::always_inline]] inline void conv8(I256* f, const I256* g, I256 ww, I256 fx, I256 Niv, I256 Mod, I256 Mod2) {
    const auto raa = load256(f), rbb = load256(g);
    const auto taa = shrk32(raa, Mod2), bb = shrk32(mul_bsm(rbb, fx, Niv, Mod), Mod);
    const auto aw = shrk32(mul_bsm(taa, ww, Niv, Mod), Mod);
    const auto aa = shrk32(taa, Mod);
    const auto awa = _mm256_permute2x128_si256(aa, aw, 3);
    const auto b0 = _mm256_permute4x64_epi64(bb, 0x00), b1 = _mm256_shuffle_epi32(b0, _MM_PERM_CDAB);
    const auto a0 = aa, a1 = _mm256_srli_epi64(a0, 32);
    const auto aw7 = _mm256_alignr_epi8(aa, awa, 12);
    auto res00 = _mm256_mul_epu32(a0, b0);
    auto res01 = _mm256_mul_epu32(a1, b0);
    auto res10 = _mm256_mul_epu32(aw7, b1);
    auto res11 = _mm256_mul_epu32(a0, b1);
    const auto b2 = _mm256_permute4x64_epi64(bb, 0x55), b3 = _mm256_shuffle_epi32(b2, _MM_PERM_CDAB);
    const auto aw6 = _mm256_alignr_epi8(aa, awa, 8);
    const auto aw5 = _mm256_alignr_epi8(aa, awa, 4);
    res00 = _mm256_add_epi64(res00, _mm256_mul_epu32(aw6, b2));
    res01 = _mm256_add_epi64(res01, _mm256_mul_epu32(aw7, b2));
    res10 = _mm256_add_epi64(res10, _mm256_mul_epu32(aw5, b3));
    res11 = _mm256_add_epi64(res11, _mm256_mul_epu32(aw6, b3));
    const auto b4 = _mm256_permute4x64_epi64(bb, 0xaa), b5 = _mm256_shuffle_epi32(b4, _MM_PERM_CDAB);
    const auto aw3 = _mm256_alignr_epi8(awa, aw, 12);
    res00 = _mm256_add_epi64(res00, _mm256_mul_epu32(awa, b4));
    res01 = _mm256_add_epi64(res01, _mm256_mul_epu32(aw5, b4));
    res10 = _mm256_add_epi64(res10, _mm256_mul_epu32(aw3, b5));
    res11 = _mm256_add_epi64(res11, _mm256_mul_epu32(awa, b5));
    const auto b6 = _mm256_permute4x64_epi64(bb, 0xff), b7 = _mm256_shuffle_epi32(b6, _MM_PERM_CDAB);
    const auto aw2 = _mm256_alignr_epi8(awa, aw, 8);
    const auto aw1 = _mm256_alignr_epi8(awa, aw, 4);
    res00 = _mm256_add_epi64(res00, _mm256_mul_epu32(aw2, b6));
    res01 = _mm256_add_epi64(res01, _mm256_mul_epu32(aw3, b6));
    res10 = _mm256_add_epi64(res10, _mm256_mul_epu32(aw1, b7));
    res11 = _mm256_add_epi64(res11, _mm256_mul_epu32(aw2, b7));
    res00 = _mm256_add_epi64(res00, res10);
    res01 = _mm256_add_epi64(res01, res11);
    store256(f, shrk32(reduce(res00, res01, Niv, Mod), Mod2));
}
template <typename mint>
void vec_cvdt(I256* f, const I256* g, usize sz) {
    static constexpr NTT_Info<mint> info;
    const auto mod = info.mod, niv = info.mr.mod_neg_inv();
    const auto Fx = _mm256_set1_epi32(mint::from_int(mint(sz).inv().residue()).residue());
    const auto Niv = _mm256_set1_epi32(niv), Mod = _mm256_set1_epi32(mod), Mod2 = _mm256_set1_epi32(info.mr.mod2());
    mint rr = 1;
    _for (i, sz) {
        conv8(f + i, g + i, _mm256_set1_epi32(rr.raw()), Fx, Niv, Mod, Mod2);
        rr *= info.RT1[__builtin_ctzll(~i)];
    }
}
template <typename mint>
void conv(u32* f, u32* g, usize sz) {
    I256 *F = (I256*)f, *G = (I256*)g;
    vec_dif<mint>(F, sz >> 3);
    vec_dif<mint>(G, sz >> 3);
    vec_cvdt<mint>(F, G, sz >> 3);
    vec_dit<mint, true>(F, sz >> 3);
}
template <typename mint>
void conv(u32* f, usize sz) {
    I256 *F = (I256*)f;
    vec_dif<mint>(F, sz >> 3);
    vec_cvdt<mint>(F, F, sz >> 3);
    vec_dit<mint, true>(F, sz >> 3);
}
#line 3 "/home/andyli/lib/graph/base.hpp"

template <typename T>
struct Edge {
    int from, to;
    T cost;
    int id;
    operator int() const { return to; }
};
template <>
struct Edge<void> {
    int from, to, id;
    struct {
        char _[0];
        operator int() const { return 1; }
    } cost;
    operator int() const { return to; }
};

template <typename T = void, bool directed = false>
struct Graph {
    static constexpr bool is_directed() { return directed; }
    static constexpr bool is_weighted() { return !std::is_same_v<T, void>; }

    using cost_type = std::conditional_t<is_weighted(), T, int>;
    using edge_type = Edge<T>;

    int n{}, m{};
    vc<edge_type> edges;
    vi indptr;
    vc<edge_type> csr_edges;

    vi _deg, _indeg, _outdeg;
    struct OutgoingEdges {
        OutgoingEdges(const Graph* G, int l, int r): G(G), l(l), r(r) {}
        const edge_type* begin() const { return G->csr_edges.data() + l; }
        const edge_type* end() const { return G->csr_edges.data() + r; }

    private:
        const Graph* G;
        int l, r;
    };
    Graph() = default;
    Graph(int n): n(n) {}
    Graph(int n, int m): n(n) { edges.reserve(m); }
    void add(int from, int to, int id = -1) requires (!is_weighted())
    {
        if (id == -1)
            id = m;
        edges.eb(from, to, id), m++;
    }
    void add(int from, int to, cost_type cost, int id = -1) requires (is_weighted())
    {
        if (id == -1)
            id = m;
        edges.eb(from, to, cost, id), m++;
    }
    void build() {
        indptr.assign(n + 1, 0);
        foreach (e, edges) {
            indptr[e.from + 1]++;
            if constexpr (!directed)
                indptr[e.to + 1]++;
        }
        _for (i, n)
            indptr[i + 1] += indptr[i];
        auto counter = indptr;
        csr_edges.resize(indptr.back() + 1);
        foreach (e, edges) {
            csr_edges[counter[e.from]++] = e;
            if constexpr (!directed) {
                swap(e.from, e.to);
                csr_edges[counter[e.from]++] = e;
                swap(e.from, e.to);
            }
        }
    }
    OutgoingEdges operator[](int u) const { return {this, indptr[u], indptr[u + 1]}; }
    const vi& Deg() {
        if (_deg.empty())
            calc_deg();
        return _deg;
    }
    const vi& In_deg() {
        if (_indeg.empty())
            calc_deg_inout();
        return _indeg;
    }
    const vi& Out_deg() {
        if (_outdeg.empty())
            calc_deg_inout();
        return _outdeg;
    }
    int deg(int u) { return Deg()[u]; }
    int in_deg(int u) { return In_deg()[u]; }
    int out_deg(int u) { return Out_deg()[u]; }
    Graph reverse() const requires directed
    {
        Graph g0(n);
        foreach (e, edges) {
            if constexpr (is_weighted())
                g0.add(e.to, e.from, e.cost, e.id);
            else
                g0.add(e.to, e.from, e.id);
        }
        g0.build();
        return g0;
    }

#ifdef FASTIO
    void write(IO& io) const requires (!is_weighted())
    {
        io.print("from to id");
        _for (i, n)
            foreach (e, (*this)[i])
                io.print(e.from, e.to, e.id);
    }
    void write(IO& io) const requires (is_weighted())
    {
        io.print("from to cost id");
        _for (i, n)
            foreach (e, (*this)[i])
                io.print(e.from, e.to, e.cost, e.id);
    }
#endif

private:
    void calc_deg() {
        _deg.resize(n);
        foreach (e, edges)
            _deg[e.from]++, _deg[e.to]++;
    }
    void calc_deg_inout() {
        _indeg.resize(n);
        _outdeg.resize(n);
        foreach (e, edges)
            _indeg[e.to]++, _outdeg[e.from]++;
    }
};
template <typename G>
concept DirectedGraph = G::is_directed();
template <typename G>
concept UndirectedGraph = !DirectedGraph<G>;
template <typename G>
concept WeightedGraph = G::is_weighted();

#ifdef FASTIO
template <typename T = void, bool directed = false>
auto read_graph(int n, int m, int off = 1) {
    Graph<T, directed> g(n, m);
    _for (m) {
        dR(int, a, b), a -= off, b -= off;
        if constexpr (g.is_weighted()) {
            dR(T, c);
            g.add(a, b, c);
        }
        else
            g.add(a, b);
    }
    g.build();
    return g;
}
template <typename T = void, bool directed = false>
auto read_tree(int n, int off = 1) { return read_graph<T, directed>(n, n - 1, off); }
#endif
#line 3 "/home/andyli/lib/graph/bfs01.hpp"

template <typename T>
struct BFS01Result1 {
    vc<T> dis;
    vi par;
};
template <typename T>
struct BFS01Result2 {
    vc<T> dis;
    vi par;
    vi root;
};
template <typename T>
auto bfs01(const auto& g, int s = 0) {
    const int n = g.n;
    vc dis(n, inf<T>);
    vi par(n, -1);
    std::deque<int> q;
    dis[s] = 0;
    q.eb(s);
    while (!q.empty()) {
        int u = pop(q);
        foreach (v, g[u])
            if (chkmin(dis[v], dis[u] + v.cost)) {
                par[v] = u;
                if (v.cost)
                    q.eb(v);
                else
                    q.emplace_front(v);
            }
    }
    return BFS01Result1{std::move(dis), std::move(par)};
}
template <typename T>
auto bfs01(const auto& g, const vi& s) {
    const int n = g.n;
    vc dis(n, inf<T>);
    vi par(n, -1);
    vi root(n, -1);
    std::deque<int> q;
    foreach (s, s) {
        dis[s] = 0;
        root[s] = s;
        q.eb(s);
    }
    while (!q.empty()) {
        int u = pop(q);
        foreach (v, g[u])
            if (chkmin(dis[v], dis[u] + v.cost)) {
                par[v] = u;
                root[v] = root[u];
                if (v.cost)
                    q.eb(v);
                else
                    q.emplace_front(v);
            }
    }
    return BFS01Result2{std::move(dis), std::move(par), std::move(root)};
}
#line 3 "/home/andyli/lib/graph/centroid_decomposition.hpp"

void centroid_decomposition_0_dfs(vi& par, vi& vs, auto&& f) {
    const int n = len(vs);
    ASSERT(n >= 1);
    int c = -1;
    vi sz(n, 1);
    _for_r (i, n) {
        if (sz[i] >= ceil(n, 2)) {
            c = i;
            break;
        }
        sz[par[i]] += sz[i];
    }
    vi col(n);
    vi V{c};
    int nc = 1;
    _for (u, 1, n) {
        if (par[u] == c) {
            V.eb(u);
            col[u] = nc++;
        }
    }
    if (c > 0) {
        for (int u = par[c]; u != -1; u = par[u]) {
            V.eb(u);
            col[u] = nc;
        }
        nc++;
    }
    _for (u, n) {
        if (u != c && !col[u]) {
            V.eb(u);
            col[u] = col[par[u]];
        }
    }
    vi indptr(nc + 1);
    _for (i, n)
        indptr[col[i] + 1]++;
    _for (i, nc)
        indptr[i + 1] += indptr[i];
    auto counter = indptr;
    vi ord(n);
    foreach (u, V)
        ord[counter[col[u]]++] = u;
    vi new_idx(n);
    _for (i, n)
        new_idx[ord[i]] = i;
    vi name(n);
    _for (i, n)
        name[new_idx[i]] = vs[i];
    {
        vi tmp(n, -1);
        _for (i, 1, n) {
            int a = new_idx[i], b = new_idx[par[i]];
            if (a > b)
                swap(a, b);
            tmp[b] = a;
        }
        par = std::move(tmp);
    }
    f(par, name, indptr);
    _for (i, 1, nc) {
        int l = indptr[i], r = indptr[i + 1];
        vi par1(r - l);
        vi name1(r - l);
        _for (i, l, r)
            name1[i - l] = name[i];
        _for (i, l, r)
            par1[i - l] = max(par[i] - l, -1);
        centroid_decomposition_0_dfs(par1, name1, f);
    }
}
void centroid_decomposition_1_dfs(vi& par, vi& vs, auto&& f) {
    const int n = len(vs);
    ASSERT(n > 1);
    if (n == 2)
        return;
    int c = -1;
    vi sz(n, 1);
    _for_r (i, n) {
        if (sz[i] >= ceil(n, 2)) {
            c = i;
            break;
        }
        sz[par[i]] += sz[i];
    }
    vi col(n, -1);
    int take = 0;
    vi ord(n, -1);
    ord[c] = 0;
    int p = 1;
    _for (u, 1, n) {
        if (par[u] == c && take + sz[u] <= floor(n - 1, 2)) {
            ord[u] = p++;
            col[u] = 0;
            take += sz[u];
        }
    }
    _for (u, 1, n)
        if (!col[par[u]]) {
            ord[u] = p++;
            col[u] = 0;
        }
    int n0 = p - 1;
    for (int u = par[c]; u != -1; u = par[u]) {
        ord[u] = p++;
        col[u] = 1;
    }
    _for (u, n)
        if (u != c && ord[u] == -1) {
            ord[u] = p++;
            col[u] = 1;
        }
    ASSERT(p == n);
    int n1 = n - 1 - n0;
    vi par0(n0 + 1, -1), par1(n1 + 1, -1), par2(n, -1);
    vi V0(n0 + 1), V1(n1 + 1), V2(n);
    _for (u, n) {
        int i = ord[u];
        V2[i] = vs[u];
        if (col[u] != 1)
            V0[i] = vs[u];
        if (col[u] != 0)
            V1[max(i - n0, 0)] = vs[u];
    }
    _for (u, 1, n) {
        int a = ord[u], b = ord[par[u]];
        if (a > b)
            swap(a, b);
        par2[b] = a;
        if (col[u] != 1 && col[par[u]] != 1)
            par0[b] = a;
        if (col[u] != 0 && col[par[u]] != 0)
            par1[max(b - n0, 0)] = max(a - n0, 0);
    }
    f(par2, V2, n0, n1);
    centroid_decomposition_1_dfs(par0, V0, f);
    centroid_decomposition_1_dfs(par1, V1, f);
}
template <int MODE>
void centroid_decomposition(const auto& g, auto&& f) {
    const int n = g.n;
    if (n == 1)
        return;
    vi V(n), par(n, -1);
    int l = 0, r = 0;
    V[r++] = 0;
    while (l < r) {
        int u = V[l++];
        foreach (v, g[u])
            if (v != par[u])
                V[r++] = v, par[v] = u;
    }
    vi new_idx(n);
    _for (i, n)
        new_idx[V[i]] = i;
    vi tmp(n, -1);
    _for (i, 1, n)
        tmp[new_idx[i]] = new_idx[par[i]];
    par = std::move(tmp);
    if constexpr (MODE == 0)
        centroid_decomposition_0_dfs(par, V, f);
    else
        centroid_decomposition_1_dfs(par, V, f);
}
#line 3 "/home/andyli/lib/poly/ntt.hpp"

template <typename mint>
struct NTT {
    static constexpr u32 mod = mint::mod();
    static constexpr mint pr = primitive_root_constexpr(mod);
    static constexpr int lvl = __builtin_ctz(mod - 1);
    mint dw[lvl], dy[lvl];
    constexpr NTT() {
        if (lvl < 3)
            return;
        mint w[lvl], y[lvl];
        w[lvl - 1] = power(pr, (mod - 1) >> lvl);
        y[lvl - 1] = w[lvl - 1].inv();
        _for_r (i, 1, lvl - 1)
            w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
        dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
        _for (i, 3, lvl) {
            dw[i] = dw[i - 1] * y[i - 2] * w[i];
            dy[i] = dy[i - 1] * w[i - 2] * y[i];
        }
    }
};
template <typename mint>
void fft4(vc<mint>& a, int k) {
    constexpr NTT<mint> ntt;
    if (len(a) <= 1)
        return;
    if (k == 1) {
        a[0] += std::exchange(a[1], a[0] - a[1]);
        return;
    }
    if (k & 1) {
        int v = 1 << (k - 1);
        _for (j, v)
            a[j] += std::exchange(a[j + v], a[j] - a[j + v]);
    }
    int u = 1 << (2 + (k & 1));
    int v = 1 << (k - 2 - (k & 1));
    mint one(1);
    mint imag = ntt.dw[1];
    while (v) {
        for (int j0 = 0, j1 = v, j2 = j1 + v, j3 = j2 + v; j0 < v; j0++, j1++, j2++, j3++) {
            mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
            mint t0p2 = t0 + t2, t1p3 = t1 + t3;
            mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
            a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
            a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
        }
        mint ww = one, xx = one * ntt.dw[2], wx = one;
        for (int jh = 4; jh < u;) {
            ww = xx * xx, wx = ww * xx;
            for (int j0 = jh * v, j1 = j0 + v, j2 = j1 + v, j3 = j2 + v, je = j1; j0 < je; j0++, j1++, j2++, j3++) {
                mint t0 = a[j0], t1 = a[j1] * xx, t2 = a[j2] * ww, t3 = a[j3] * wx;
                mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
            }
            xx *= ntt.dw[__builtin_ctz(jh += 4)];
        }
        u <<= 2;
        v >>= 2;
    }
}
template <typename mint>
void ifft4(vc<mint>& a, int k) {
    constexpr NTT<mint> ntt;
    if (len(a) <= 1)
        return;
    if (k == 1) {
        a[0] += std::exchange(a[1], a[0] - a[1]);
        return;
    }
    int u = 1 << (k - 2);
    int v = 1;
    mint one(1);
    mint imag = ntt.dy[1];
    while (u) {
        for (int j0 = 0, j1 = v, j2 = j1 + v, j3 = j2 + v; j0 < v; j0++, j1++, j2++, j3++) {
            mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
            mint t0p1 = t0 + t1, t2p3 = t2 + t3;
            mint t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
            a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
            a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
        }
        mint ww = one, xx = one * ntt.dy[2], yy = one;
        u <<= 2;
        for (int jh = 4; jh < u;) {
            ww = xx * xx, yy = xx * imag;
            for (int j0 = jh * v, j1 = j0 + v, j2 = j1 + v, j3 = j2 + v, je = j1; j0 < je; j0++, j1++, j2++, j3++) {
                mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                a[j1] = t0m1 + t2m3, a[j3] = (t0m1 - t2m3) * ww;
            }
            xx *= ntt.dy[__builtin_ctz(jh += 4)];
        }
        u >>= 4, v <<= 2;
    }
    if (k & 1) {
        u = 1 << (k - 1);
        _for (j, u)
            a[j] += std::exchange(a[j + u], a[j] - a[j + u]);
    }
}
#line 3 "/home/andyli/lib/math/mod_inverse.hpp"

template <std::unsigned_integral T, typename S = std::make_signed_t<T>>
constexpr std::tuple<S, S, T> bezout(T x, T y) {
    bool t = x < y;
    if (t)
        swap(x, y);
    if (y == 0) {
        if (x == 0)
            return {};
        if (t)
            return {0, 1, x};
        return {1, 0, x};
    }
    S s0 = 1, s1 = 0, t0 = 0, t1 = 1;
    loop {
        auto [q, r] = divmod(x, y);
        if (r == 0) {
            if (t)
                return {t1, s1, y};
            return {s1, t1, y};
        }
        S s2 = s0 - S(q) * s1, t2 = t0 - S(q) * t1;
        x = y;
        y = r;
        s0 = s1;
        s1 = s2;
        t0 = t1;
        t1 = t2;
    }
}
template <std::unsigned_integral T>
constexpr T mod_inverse(T x, T m) {
    auto [s, t, g] = bezout(x, m);
    ASSERT(g == 1);
    return s < 0 ? T(s) + m : s;
}
#line 2 "/home/andyli/lib/math/power.hpp"

template <typename T>
constexpr T power(T a, auto b) {
    T ans{1};
    for (; b; b >>= 1, a *= a)
        if (b & 1)
            ans *= a;
    return ans;
}
#line 4 "/home/andyli/lib/modint/modint_common.hpp"

template <typename T>
auto val(const T& x) {
    if constexpr (Modint<T>)
        return x.val();
    else
        return x;
}
#line 4 "/home/andyli/lib/modint/montgomery.hpp"

template <typename Context>
struct MontgomeryModInt {
    using mint = MontgomeryModInt;
    using int_type = Context::int_type;
    using mr_type = Context::mr_type;
    using int_double_t = mr_type::int_double_t;

    constexpr MontgomeryModInt() = default;
    constexpr MontgomeryModInt(Signed auto y) {
        using S = make_signed_t<int_type>;
        S v = y % S(mod());
        x = mr().reduce(mr().mbase2() * int_double_t(v < 0 ? v + mod() : v));
    }
    constexpr MontgomeryModInt(Unsigned auto y) { x = mr().reduce(mr().mbase2() * int_double_t(y % mod())); }
    constexpr int_type val() const { return mr().strict_shrink(mr().reduce(x)); }
    constexpr int_type residue() const { return mr().strict_shrink(x); }
    static constexpr int_type mod() { return mr().mod(); }
    constexpr mint& operator++() {
        x = mr().shrink(x + mr().mbase());
        return *this;
    }
    constexpr mint operator++(int) {
        mint r = *this;
        ++*this;
        return r;
    }
    constexpr mint& operator+=(const mint& rhs) {
        x = mr().shrink(x + rhs.x);
        return *this;
    }
    constexpr mint& operator--() {
        x = mr().shrink(x + mod() - mr().mbase());
        return *this;
    }
    constexpr mint operator--(int) {
        mint r = *this;
        --*this;
        return r;
    }
    constexpr mint& operator-=(const mint& rhs) {
        x = mr().shrink(x + mr().mod2() - rhs.x);
        return *this;
    }
    constexpr mint& operator*=(const mint& rhs) {
        x = mr().reduce(int_double_t(x) * rhs.x);
        return *this;
    }
    constexpr mint inv() const { return from_raw(mr().reduce(int_double_t(mr().mbase3()) * mod_inverse(x, mod()))); }
    constexpr mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }
    constexpr mint operator+() const { return *this; }
    constexpr mint operator-() const { return from_raw(!x ? 0 : mr().mod2() - x); }
    friend constexpr mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    friend constexpr bool operator==(const mint& lhs, const mint& rhs) { return mr().strict_shrink(lhs.x) == mr().strict_shrink(rhs.x); }
    friend constexpr auto operator<=>(const mint& lhs, const mint& rhs) { return lhs.val() <=> rhs.val(); }

    static constexpr mint from_raw(int_type x) {
        mint r;
        r.x = x;
        return r;
    }
    static constexpr mint from_int(int_type x) { return from_raw(mr().reduce(mr().mbase2() * int_double_t(x))); }
    constexpr int_type raw() const { return x; }
#ifdef FASTIO
    void read(IO& io) {
        static int_type x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const { io.write(val()); }
#endif
    static constexpr const mr_type& mr() { return Context::montgomery_reduction(); }

private:
    int_type x{};
};
template <Unsigned T, T Mod>
requires (Mod % 2 == 1 && Mod <= std::numeric_limits<T>::max() / 4)
struct StaticMontgomeryReductionContext {
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;
    static constexpr const mr_type& montgomery_reduction() { return _reduction; }

private:
    static constexpr auto _reduction = mr_type(Mod);
};

template <Unsigned T>
struct DynamicMontgomeryReductionContext {
    using int_type = T;
    using mr_type = MontgomeryReduction<T>;

    struct Guard {
        Guard(const Guard&) = delete;
        Guard& operator=(const Guard&) = delete;
        ~Guard() { _reduction_env.pop_back(); }

    private:
        friend DynamicMontgomeryReductionContext;
        Guard() = default;
    };
    [[nodiscard]] static Guard set_mod(T mod) {
        ASSERT(mod % 2 == 1 && mod <= std::numeric_limits<T>::max() / 4);
        _reduction_env.eb(mod);
        return {};
    }
    static constexpr const mr_type& montgomery_reduction() { return _reduction_env.back(); }

private:
    static inline vc<mr_type> _reduction_env;
};

template <u32 Mod>
using MMInt = MontgomeryModInt<StaticMontgomeryReductionContext<u32, Mod>>;
template <u64 Mod>
using MMInt64 = MontgomeryModInt<StaticMontgomeryReductionContext<u64, Mod>>;

using MMInt998244353 = MMInt<998244353>;
using MMInt1000000007 = MMInt<1000000007>;

#define SetMMod(T, mod)                               \
    using ctx = DynamicMontgomeryReductionContext<T>; \
    auto _guard = ctx::set_mod(mod);                  \
    using mint = MontgomeryModInt<ctx>
#line 3 "/home/andyli/lib/poly/arbitrary_ntt.hpp"

namespace ArbitraryNTT {
    constexpr u32 m0 = 167772161;
    constexpr u32 m1 = 469762049;
    constexpr u32 m2 = 754974721;
    using mint0 = MMInt<m0>;
    using mint1 = MMInt<m1>;
    using mint2 = MMInt<m2>;
    constexpr u32 r01 = mint1(m0).inv().val();
    constexpr u32 r02 = mint2(m0).inv().val();
    constexpr u32 r12 = mint2(m1).inv().val();
    constexpr u32 r02r12 = u64(r02) * r12 % m2;
    constexpr u64 w1 = m0;
    constexpr u64 w2 = u64(m0) * m1;
    template <typename T>
    void crt(auto&& c0, auto&& c1, auto&& c2, auto&& r, u64 w1, u64 w2) {
        _for (i, len(r)) {
            u64 n1 = val(c1[i]), n2 = val(c2[i]), a = val(c0[i]);
            u64 b = (n1 + m1 - a) * r01 % m1;
            u64 c = ((n2 + m2 - a) * r02r12 + (m2 - b) * r12) % m2;
            r[i] = a + b * w1 + T(c) * w2;
        }
    }
} // namespace ArbitraryNTT
#line 3 "/home/andyli/lib/poly/convolution_naive.hpp"

template <typename T, typename U = T>
vc<U> convolution_naive(const vc<T>& a, const vc<T>& b) {
    int n = len(a), m = len(b);
    if (n > m)
        return convolution_naive<T, U>(b, a);
    if (!n)
        return {};
    vc<U> r(n + m - 1);
    _for (i, n)
        _for (j, m)
            r[i + j] += U(a[i]) * b[j];
    return r;
}
#line 3 "/home/andyli/lib/poly/convolution_karatsuba.hpp"

template <typename T, typename U = T>
vc<U> convolution_karatsuba(const vc<T>& f, const vc<T>& g) {
    int n = len(f), m = len(g);
    if (min(n, m) <= 30)
        return convolution_naive<T, U>(f, g);
    int mi = ceil(max(n, m), 2);
    vc<U> f1, f2, g1, g2;
    if (n < mi)
        f1 = {all(f)};
    else {
        f1 = {f.begin(), f.begin() + mi};
        f2 = {mi + all(f)};
    }
    if (m < mi)
        g1 = {all(g)};
    else {
        g1 = {g.begin(), g.begin() + mi};
        g2 = {mi + all(g)};
    }
    auto a = convolution_karatsuba(f1, g1);
    auto b = convolution_karatsuba(f2, g2);
    _for (i, len(f2))
        f1[i] += f2[i];
    _for (i, len(g2))
        g1[i] += g2[i];
    auto c = convolution_karatsuba(f1, g1);
    vc<U> r(n + m - 1);
    _for (i, len(a))
        r[i] += a[i], c[i] -= a[i];
    _for (i, len(b))
        r[2 * mi + i] += b[i], c[i] -= b[i];
    if (c.back() == 0)
        c.pop_back();
    _for (i, len(c))
        r[mi + i] += c[i];
    return r;
}
#line 5 "/home/andyli/lib/poly/convolution.hpp"

template <Modint mint>
vc<mint> convolution_ntt(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    int k = get_lg(n + m - 1), sz = 1 << k;
#ifdef NTT_AVX2
    if constexpr (sizeof(mint) == 4) {
        U32Aligned f(sz), g(sz);
        _for (i, n)
            f[i] = a[i].val();
        memset(f + n, 0, (sz - n) << 2);
        _for (i, m)
            g[i] = b[i].val();
        memset(g + m, 0, (sz - m) << 2);
        conv<mint>(f, g, sz);
        vc<mint> r(n + m - 1);
        _for (i, n + m - 1)
            r[i] = mint::from_int(f[i]);
        return r;
    }
#endif
    vc<mint> f(sz), g(sz);
    _for (i, n)
        f[i] = a[i];
    _for (i, m)
        g[i] = b[i];
    fft4(f, k);
    fft4(g, k);
    _for (i, sz)
        f[i] *= g[i];
    ifft4(f, k);
    f.resize(n + m - 1);
    mint iv = mint(sz).inv();
    foreach (x, f)
        x *= iv;
    return f;
}
template <typename T, typename U = T>
vc<U> convolution_garner(const vc<T>& a, const vc<T>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
#ifdef NTT_AVX2
    int k = get_lg(n + m - 1), sz = 1 << k;
    U32Aligned f0(sz), f1(sz), f2(sz), g0(sz), g1(sz), g2(sz);
    _for (i, n) {
        f0[i] = val(a[i]) % mint0::mod();
        f1[i] = val(a[i]) % mint1::mod();
        f2[i] = val(a[i]) % mint2::mod();
    }
    memset(f0 + n, 0, (sz - n) << 2);
    memset(f1 + n, 0, (sz - n) << 2);
    memset(f2 + n, 0, (sz - n) << 2);
    _for (i, m) {
        g0[i] = val(b[i]) % mint0::mod();
        g1[i] = val(b[i]) % mint1::mod();
        g2[i] = val(b[i]) % mint2::mod();
    }
    memset(g0 + m, 0, (sz - m) << 2);
    memset(g1 + m, 0, (sz - m) << 2);
    memset(g2 + m, 0, (sz - m) << 2);
    conv<mint0>(f0, g0, sz);
    conv<mint1>(f1, g1, sz);
    conv<mint2>(f2, g2, sz);
#else
    vc<mint0> a0(n), b0(m);
    vc<mint1> a1(n), b1(m);
    vc<mint2> a2(n), b2(m);
    _for (i, n)
        a0[i] = val(a[i]), a1[i] = val(a[i]), a2[i] = val(a[i]);
    _for (i, m)
        b0[i] = val(b[i]), b1[i] = val(b[i]), b2[i] = val(b[i]);
    auto f0 = convolution_ntt(a0, b0);
    auto f1 = convolution_ntt(a1, b1);
    auto f2 = convolution_ntt(a2, b2);
#endif
    vc<U> r(n + m - 1);
    if constexpr (Modint<T>)
        crt<u64>(f0, f1, f2, r, w1 % T::mod(), w2 % T::mod());
    else
        crt<U>(f0, f1, f2, r, w1, w2);
    return r;
}
template <Integer T, typename U = i64>
vc<U> convolution(const vc<T>& a, const vc<T>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) <= 100)
        return convolution_karatsuba<T, U>(a, b);
    return convolution_garner<T, U>(a, b);
}
template <Modint mint>
vc<mint> convolution(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) > 40)
        if constexpr (StaticModint<mint> && __builtin_ctzll(mint::mod() - 1) >= 20)
            return convolution_ntt(a, b);
    return min(n, m) <= 40 ? convolution_karatsuba(a, b) : convolution_garner(a, b);
}
#line 4 "/home/andyli/lib/graph/tree_all_distances.hpp"

vc<i64> tree_all_distances(const auto& g) {
    const int n = g.n;
    vc<i64> ans(n);
    ans[0] = n;
    ans[1] = n + n - 2;
    auto f = [&](auto&& par, auto&& V, int n1, int n2) {
        int n = len(V);
        vi dist(n);
        _for (i, 1, n)
            dist[i] = dist[par[i]] + 1;
        int mx = max(dist);
        vi f(mx + 1), g(mx + 1);
        _for (i, 1, n1 + 1)
            f[dist[i]]++;
        _for (i, n1 + 1, n1 + n2 + 1)
            g[dist[i]]++;
        while (f.back() == 0)
            f.pop_back();
        while (g.back() == 0)
            g.pop_back();
        auto r = convolution<int, i64>(f, g);
        _for (i, len(r))
            ans[i] += r[i] * 2;
    };
    centroid_decomposition<1>(g, f);
    return ans;
}
#line 6 "/home/andyli/lib/test/library_checker/frequency_table_of_tree_distance.test.cpp"

int main() {
    dR(int, n);
    auto g = read_tree(n, 0);
    auto ans = tree_all_distances(g);
    ans.erase(ans.begin());
    foreach (x, ans)
        x >>= 1;
    print(ans);
    return 0;
}
