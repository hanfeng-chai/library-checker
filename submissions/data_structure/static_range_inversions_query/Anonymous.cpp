#line 1 "/home/andyli/OneDrive/lixiang/code/lib/test/library_checker/static_range_inversions_query.3.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

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
#define LB(c, x) distance(begin(c), lower_bound(c, x))
#define UB(c, x) distance(begin(c), upper_bound(c, x))
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
constexpr u64 ten(int t) { return t == 0 ? 1 : ten(t - 1) * 10; }
constexpr int get_lg(auto n) { return n <= 1 ? 1 : std::__bit_width(n - 1); }
constexpr auto Max(const auto& x) { return x; }
constexpr auto Min(const auto& x) { return x; }
constexpr auto Max(const auto& x, const auto& y, const auto&... arg) { return x < y ? Max(y, arg...) : Max(x, arg...); }
constexpr auto Min(const auto& x, const auto& y, const auto&... arg) { return x < y ? Min(x, arg...) : Min(y, arg...); }
constexpr bool chkmax(auto& d, const auto&... x) {
    auto t = Max(x...);
    return t > d ? d = t, true : false;
}
constexpr bool chkmin(auto& d, const auto&... x) {
    auto t = Min(x...);
    return t < d ? d = t, true : false;
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
    vi p(len(a));
    iota(all(p), 0);
    sort(p, [&](int i, int j) { return std::pair{a[i], i} < std::pair{a[j], j}; });
    return p;
}
vi sshift(const str& s, char c = 'a') {
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
    std::array<u32, 10000> O{};
    int x = 0;
    _for (i, 10)
        _for (j, 10)
            _for (k, 10)
                _for (l, 10)
                    O[x++] = i + j * 0x100 + k * 0x10000 + l * 0x1000000 + 0x30303030;
    return O;
}();
#line 3 "/home/andyli/lib/io.hpp"

#define FASTIO
#ifdef CHECK_EOF
#define ECHK0 *ip ? *ip++ : 0
#define ECHK1     \
    if (ch == EV) \
        return set(false), *this;
#define ECHK2 \
    if (!*ip) \
        return set(false), *this;
#define ECHK3 &&ch != -1
#define ECHK4 &&*ip
#define ECHK5     \
    if (ch == -1) \
        return set(false);
#define ECHK6 \
    if (!*ip) \
        return set(false);
#else
#define ECHK0 *ip++
#define ECHK1
#define ECHK2
#define ECHK3
#define ECHK4
#define ECHK5
#define ECHK6
#endif

#if defined(__unix__) && !defined(LX_DEBUG) && !defined(LX_LOCAL)
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
    bool status;
#ifndef LX_DEBUG
    char obuf[bufSize], *ip, *op = obuf;
#ifndef USE_MMAP
    char ibuf[bufSize], *eip;
#endif
#endif

#ifdef LX_DEBUG
    int getch() { return fgetc(in); }
    int getch_unchecked() { return getch(); }
    int unget(int c = -1) { return ungetc(c, in); }
    int peek() { return unget(getch()); }
    void input(FILE* f) { in = f, set(); }
    void skipws() {
        int ch = getch();
        while (blank(ch) ECHK3)
            ch = getch();
        unget(ch);
    }
    void ireadstr(char* s, usize n) { fread(s, 1, n, in); }
#elif defined(USE_MMAP)
    void skipws() {
        while (blank(*ip) ECHK4)
            ip++;
        ECHK6
    }
    int unget(int = 0) = delete;
    int getch() { return ECHK0; }
    int getch_unchecked() { return *ip++; }
    int peek() { return *ip; }
    void input(FILE* f) {
        struct stat st;
        int fd;
        in = f;
        if (in)
            fd = fileno(in), fstat(fd, &st), ip = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0), set();
    }
    void ireadstr(char* s, usize n) { memcpy(s, ip, n), ip += n; }
    static constexpr auto I = [] {
        std::array<u32, 0x10000> I{};
        fill(I, -1);
        _for (i, 10)
            _for (j, 10)
                I[i + j * 0x100 + 0x3030] = i * 10 + j;
        return I;
    }();
#else
    void skipws() {
        int ch = getch();
        while (blank(ch) ECHK3)
            ch = getch();
        ECHK5
        unget();
    }
    int unget(int = 0) { return *ip--; }
    int getch() { return (ip == eip ? (eip = (ip = ibuf) + fread(ibuf, 1, bufSize, in)) : nullptr), ip == eip ? -1 : *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return (ip == eip ? (eip = (ip = ibuf) + fread(ibuf, 1, bufSize, in)) : nullptr), ip == eip ? -1 : *ip; }
    void input(FILE* f) { in = f, ip = eip = ibuf, set(); }
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
#endif
    void input(std::string_view s) { input(fopen(s.data(), "rb")); }
    void set(bool s = true) { status = s; }
    IO(FILE* i = stdin, FILE* o = stdout) { input(i), output(o); }
    ~IO() { flush(); }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    IO& read(Args&... x) {
#ifdef CHECK_EOF
        (read(x) && ...);
#else
        (read(x), ...);
#endif
        return *this;
    }
    template <Signed T>
    IO& read(T& x) {
        x = 0;
        make_unsigned_t<T> t;
        bool sign = false;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch) ECHK3)
            sign = ch == '-', ch = getch();
        ECHK1
        t = 0;
        while (isdigit(ch))
            t = t * 10 + (ch ^ 48), ch = getch();
        unget(ch);
#else
        while (!isdigit(*ip) ECHK4)
            sign = *ip++ == '-';
        ECHK2
        t = *ip++ ^ 48;
        u16* tip = (u16*)ip;
        while (~I[*tip])
            t = t * 100 + I[*tip++];
        ip = (char*)tip;
        if (isdigit(*ip))
            t = t * 10 + (*ip++ ^ 48);
#endif
        x = sign ? -t : t;
        return *this;
    }
    IO& read(Unsigned auto& x) {
        x = 0;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch) ECHK3)
            ch = getch();
        ECHK1
        while (isdigit(ch))
            x = x * 10 + (ch ^ 48), ch = getch();
        unget(ch);
#else
        while (!isdigit(*ip) ECHK4)
            ip++;
        ECHK2
        x = *ip++ ^ 48;
        u16* tip = (u16*)ip;
        while (~I[*tip])
            x = x * 100 + I[*tip++];
        ip = (char*)tip;
        if (isdigit(*ip))
            x = x * 10 + (*ip++ ^ 48);
#endif
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
        ECHK1
        return *this;
    }
#ifdef USE_MMAP
    usize next_size() const {
        char* ip = this->ip;
        while (!blank(*ip) ECHK4)
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
        ECHK1
        while (!blank(ch))
            *s++ = ch, getch_unchecked(), ch = peek();
        *s = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        int ch = peek();
        ECHK1
        s.erase();
        while (!blank(ch))
            s.append(1, ch), getch_unchecked(), ch = peek();
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
        char* t = s;
        int ch = getch();
        while (ch != '\n' && ch != EV)
            *s++ = ch, ch = getch();
        *s = 0;
        if (s == t && ch == EV)
            set(false);
        return *this;
    }
    IO& readline(str& s) {
        s.erase();
        int ch = getch();
        while (ch != '\n' && ch != EV)
            s.append(1, ch), ch = getch();
        if (s.empty() && ch == EV)
            set(false);
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

#ifdef LX_DEBUG
    void flush() { fflush(out), set(); }
    void putch_unchecked(char c) { fputc(c, out); }
    void putch(char c) { putch_unchecked(c); }
    void writestr(const char* s, usize n) { fwrite(s, 1, n, out); }
#else
    void flush() { fwrite(obuf, 1, op - obuf, out), op = obuf; }
    void putch_unchecked(char c) { *op++ = c; }
    void putch(char c) { (op == end(obuf) ? flush() : void()), putch_unchecked(c); }
    void writestr(const char* s, usize n) {
        if (n >= usize(end(obuf) - op)) [[unlikely]]
            flush(), fwrite(s, 1, n, out);
        else
            memcpy(op, s, n), op += n;
    }
#endif
    void output(std::string_view s) { output(fopen(s.data(), "wb")); }
    void output(FILE* f) { out = f; }
    void setprec(u32 n = 6) { prec = n; }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    void write(Args&&... x) { (write(FORWARD(x)), ...); }
    void write() const {}
    template <Signed T>
    void write(T x) {
        make_unsigned_t<T> y = x;
        if (x < 0)
            write('-', y = -y);
        else
            write(y);
    }
    void write(std::unsigned_integral auto x) {
#ifndef LX_DEBUG
        if (end(obuf) - op < 64) [[unlikely]]
            flush();

        auto L = [&](int x) { return x == 1 ? 0 : ten(x - 1); };
        auto R = [&](int x) { return ten(x) - 1; };

        auto&& O = itos_table;
#define de(t)                            \
    case L(t)... R(t):                   \
        *(u32*)op = O[x / ten((t) - 4)]; \
        op += 4;                         \
        x %= ten((t) - 4);               \
        [[fallthrough]]

        switch (u64(x)) {
            de(18);
            de(14);
            de(10);
            de(6);
        case L(2)... R(2):
            *(u32*)op = O[x * 100];
            op += 2;
            break;

            de(17);
            de(13);
            de(9);
            de(5);
        case L(1)... R(1):
            *op++ = x ^ 48;
            break;

        default:
            *(u32*)op = O[x / ten(16)];
            op += 4;
            x %= ten(16);
            [[fallthrough]];
            de(16);
            de(12);
            de(8);
        case L(4)... R(4):
            *(u32*)op = O[x];
            op += 4;
            break;

            de(19);
            de(15);
            de(11);
            de(7);
        case L(3)... R(3):
            *(u32*)op = O[x * 10];
            op += 3;
            break;
        }
#undef de
#else
        write(u128(x));
#endif
    }
    void write(u128 x) {
#ifndef LX_DEBUG
        if (end(obuf) - op < 64) [[unlikely]]
            flush();
#endif
        static int s[40], t = 0;
        do
            s[t++] = x % 10, x /= 10;
        while (x);
        while (t)
            putch_unchecked(s[--t] ^ 48);
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
    operator bool() const { return status; }
} io;
#ifdef LX_LOCAL
IO err(nullptr, stderr);
#define dbg(x) err.print(#x, '=', x)
#else
#define dbg(x) \
    do {       \
    } while (false)
#endif
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
#line 3 "/home/andyli/lib/ds/offline/mo.hpp"

struct Mo {
    int n;
    vc<pi> Q;
    Mo(int n = 0): n(n) {}
    void add(int l, int r) { Q.eb(l, r); }
    static vi get_order(const vc<pi>& Q, int n) {
        int m = len(Q);
        vi I(m);
        iota(all(I), 0);
        int bs = sqrt(3) * n / sqrt(2 * m);
        chkmax(bs, 1);
        sort(I, [&](int i, int j) {
            int x = Q[i].first / bs, y = Q[j].first / bs;
            return x != y ? x < y : (x & 1 ? Q[i].second > Q[j].second : Q[i].second < Q[j].second);
        });
        auto cost = [&](int i, int j) -> int {
            return abs(Q[I[i]].first - Q[I[j]].first) + abs(Q[I[i]].second - Q[I[j]].second);
        };
        _for (i, m - 5) {
            if (cost(i, i + 2) + cost(i + 1, i + 3) < cost(i, i + 1) + cost(i + 2, i + 3)) {
                swap(I[i + 1], I[i + 2]);
            }
            if (cost(i, i + 3) + cost(i + 1, i + 4) < cost(i, i + 1) + cost(i + 3, i + 4)) {
                swap(I[i + 1], I[i + 3]);
            }
        }
        return I;
    }
    void calc(auto&& add_l, auto&& add_r, auto&& del_l, auto&& del_r, auto&& query) {
        auto I = get_order(Q, n);
        int l = 0, r = 0;
        foreach (i, I) {
            auto [ql, qr] = Q[i];
            while (l > ql)
                add_l(--l);
            while (r < qr)
                add_r(r++);
            while (l < ql)
                del_l(l++);
            while (r > qr)
                del_r(--r);
            query(i);
        }
    }
};
#line 3 "/home/andyli/lib/ds/fenwicktree.hpp"

template <typename Monoid>
struct FenwickTree {
    using M = Monoid;
    using X = M::value_type;

    int n;
    vc<X> a;
    X sum;
    FenwickTree() = default;
    FenwickTree(int n) { build(n); }
    template <std::convertible_to<X> T>
    FenwickTree(const vc<T>& a) { build(a); }
    FenwickTree(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        this->n = n;
        a.assign(n, M::unit());
        sum = M::unit();
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        a.assign(n, M::unit());
        _for (i, n)
            a[i] = f(i);
        _for (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[j - 1] = M::op(a[i - 1], a[j - 1]);
        }
        sum = prod(n);
    }
    void set(int i, const X& x) {
        multiply(i, M::inverse(get(i)));
        multiply(i, x);
    }
    void multiply(int i, const X& x) {
        sum = M::op(sum, x);
        for (i++; i <= n; i += lowbit(i))
            a[i - 1] = M::op(a[i - 1], x);
    }
    X get(int i) const { return prod(i, i + 1); }
    vc<X> get_all() const {
        vc<X> a = this->a;
        _for_r (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[j - 1] = M::op(a[j - 1], M::inverse(a[i - 1]));
        }
        return a;
    }
    X prod(int i) const {
        X r = M::unit();
        while (i) {
            r = M::op(r, a[i - 1]);
            i -= lowbit(i);
        }
        return r;
    }
    X prod(int l, int r) const {
        X vl = M::unit(), vr = M::unit();
        while (l < r) {
            vr = M::op(vr, a[r - 1]);
            r -= lowbit(r);
        }
        while (r < l) {
            vl = M::op(vl, a[l - 1]);
            l -= lowbit(l);
        }
        return M::op(vr, M::inverse(vl));
    }
    X prod_all() const { return sum; }
    int max_right(auto&& check, int l = 0) const {
        ASSERT(check(M::unit()));
        int i = l;
        X t = M::unit();
        int k = BLK {
            loop {
                if (i & 1)
                    t = M::op(t, M::inverse(a[--i]));
                if (i == 0)
                    return std::__lg(n) + 1;
                int k = __builtin_ctz(i) - 1;
                if (i + (1 << k) > n)
                    return k;
                if (!check(M::op(t, a[i + (1 << k) - 1])))
                    return k;
                t = M::op(t, M::inverse(a[i - 1]));
                i -= lowbit(i);
            }
        };
        while (k--) {
            if (i + (1 << k) <= n) {
                X nt = M::op(t, a[i + (1 << k) - 1]);
                if (check(nt)) {
                    t = nt;
                    i += 1 << k;
                }
            }
        }
        return i;
    }
    int min_left(auto&& check, int r) const {
        assert(check(M::unit()));
        int i = r;
        int k = 0;
        X t = M::unit();
        while (i && check(t)) {
            t = M::op(t, a[i - 1]);
            k = __builtin_ctz(i);
            i -= lowbit(i);
        }
        if (i == 0)
            return 0;
        while (k--) {
            X nt = M::op(t, M::inverse(a[i + (1 << k) - 1]));
            if (!check(nt)) {
                t = nt;
                i += 1 << k;
            }
        }
        return i + 1;
    }
    int kth(int x, int l = 0) {
        return max_right([&](const X& y) { return y <= x; }, l);
    }
};
#line 2 "/home/andyli/lib/monoid/add.hpp"

template <typename T>
struct Monoid_Add {
    using value_type = T;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return a + b; }
    static constexpr X inverse(const X& x) { return -x; }
    static constexpr X power(const X& x, auto n) { return n * x; }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return {}; }
    static constexpr bool commute = true;
};
#line 3 "/home/andyli/lib/math/binary_search.hpp"

template <Integer T>
T bsearch(auto&& check, T ok, T ng) {
    while (std::abs(ok - ng) > 1) {
        T x = (ng + ok) >> 1;
        (check(x) ? ok : ng) = x;
    }
    return ok;
}
template <std::floating_point T>
T bsearch(auto&& check, T ok, T ng, int iter = 100) {
    _for (iter) {
        T x = (ng + ok) / 2;
        (check(x) ? ok : ng) = x;
    }
    return (ng + ok) / 2;
}
#line 5 "/home/andyli/lib/ds/fenwicktree_01.hpp"

template <template <typename> typename FenwickTree>
struct FenwickTree_01Base {
    int n, m;
    vc<u64> dat;
    FenwickTree<Monoid_Add<int>> ft;
    FenwickTree_01Base() = default;
    FenwickTree_01Base(int n) { build(n); }
    template <std::convertible_to<bool> T>
    FenwickTree_01Base(const vc<T>& a) { build(a); }
    FenwickTree_01Base(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        this->n = n;
        m = (n + 63) >> 6;
        dat.assign(m, 0);
        ft.build(m);
    }
    template <std::convertible_to<bool> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        m = (n + 63) >> 6;
        dat.assign(m, 0);
        _for (i, n)
            dat[i >> 6] |= u64(f(i)) << (i & 63);
        ft.build(m, [&](int i) { return std::popcount(dat[i]); });
    }
    void add(int i) {
        dat[i >> 6] |= 1ULL << (i & 63);
        ft.multiply(i >> 6, 1);
    }
    void remove(int i) {
        dat[i >> 6] &= ~(1ULL << (i & 63));
        ft.multiply(i >> 6, -1);
    }
    void multiply(int i, int x) {
        if (x == 1)
            add(i);
        else
            remove(i);
    }
    bool get(int i) const { return dat[i >> 6] >> (i & 63) & 1; }
    vc<bool> get_all() const {
        vc<bool> a(n);
        _for (i, n)
            a[i] = get(i);
        return a;
    }
    int prod(int i) const {
        int ans = ft.prod(i >> 6);
        ans += std::popcount(dat[i >> 6] & ((1ULL << (i & 63)) - 1));
        return ans;
    }
    int prod(int l, int r) const {
        int ans = ft.prod(l >> 6, r >> 6);
        ans += std::popcount(dat[r >> 6] & ((1ULL << (r & 63)) - 1));
        ans -= std::popcount(dat[l >> 6] & ((1ULL << (l & 63)) - 1));
        return ans;
    }
    int prod_all() const { return ft.prod_all(); }
    int kth(int k, int l = 0) {
        if (k >= prod_all())
            return n;
        k += std::popcount(dat[l >> 6] & ((1ULL << (l & 63)) - 1));
        l >>= 6;
        int mi = 0;
        int i = ft.max_right([&](int x) { return x <= k ? (chkmax(mi, x), true) : false; }, l);
        if (i == m)
            return n;
        k -= mi;
        u64 x = dat[i];
        int p = std::popcount(x);
        if (p <= k)
            return n;
        k = bsearch([&](int mi) { return (p - std::popcount(x >> mi)) <= k; }, 0, 64);
        return 64 * i + k;
    }
    int next(int i) {
        int j = i >> 6;
        i &= 63;
        u64 x = dat[j] & ~((1ULL << i) - 1);
        if (x)
            return 64 * j + __builtin_ctzll(x);
        j = ft.kth(0, j + 1);
        if (j == m || !dat[j])
            return n;
        return 64 * j + __builtin_ctzll(dat[j]);
    }
    int prev(int i) {
        if (i == n)
            i--;
        int j = i >> 6;
        i &= 63;
        u64 x = dat[j];
        if (i < 63)
            x &= (1ULL << (i + 1)) - 1;
        if (x)
            return 64 * j + 63 - __builtin_clzll(x);
        j = ft.min_left([&](int x) { return x <= 0; }, j) - 1;
        if (j == -1)
            return -1;
        return 64 * j + 63 - __builtin_clzll(dat[j]);
    }
};
using FenwickTree_01 = FenwickTree_01Base<FenwickTree>;
#line 3 "/home/andyli/lib/ds/sqrtfenwicktree.hpp"

template <typename Monoid>
struct SqrtFenwickTree {
    using M = Monoid;
    using X = M::value_type;

    int n, m, bs;
    vc<X> dat1;
    vvc<X> dat2;
    SqrtFenwickTree() = default;
    SqrtFenwickTree(int n) { build(n); }
    template <std::convertible_to<X> T>
    SqrtFenwickTree(const vc<T>& a) { build(a); }
    SqrtFenwickTree(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        this->n = n;
        bs = sqrt(n);
        m = ceil(n, bs);
        dat1.assign(m + 1, M::unit());
        dat2.resize(m);
        _for (i, m - 1)
            dat2[i].assign(bs + 1, M::unit());
        dat2.back().assign(n - bs * (m - 1) + 1, M::unit());
        if (n % bs == 0)
            dat1.eb(M::unit()), dat2.eb(vc<X>{M::unit()});
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        bs = sqrt(n);
        m = ceil(n, bs);
        dat1.assign(m + 1, M::unit());
        dat2.resize(m);
        _for (i, m) {
            int l = i * bs, r = min(n, (i + 1) * bs);
            dat2.assign(r - l + 1, M::unit());
            _for (j, l, r) {
                dat2[i][j + 1 - l] = M::op(dat2[i][j - l], f(j));
            }
            dat1[i + 1] = M::op(dat1[i], dat2[i].back());
        }
    }
    void set(int i, const X& x) {
        multiply(i, M::inverse(get(i)));
        multiply(i, x);
    }
    void multiply(int i, const X& x) {
        int p = i / bs;
        int l = p * bs, r = min(n, (p + 1) * bs);
        _for (j, i, r)
            dat2[p][j + 1 - l] = M::op(dat2[p][j + 1 - l], x);
        _for (j, p, m)
            dat1[j + 1] = M::op(dat1[j + 1], x);
    }
    X get(int i) const {
        int p = i / bs;
        return M::op(dat2[p][i + 1 - p * bs], M::inverse(dat2[p][i - p * bs]));
    }
    vc<X> get_all() const {
        vc<X> a(n);
        _for (i, n)
            a[i] = get(i);
        return a;
    }
    X prod(int i) const {
        int p = i / bs;
        return M::op(dat1[p], dat2[p][i - p * bs]);
    }
    X prod(int l, int r) const { return M::op(prod(r), M::inverse(prod(l))); }
    X prod_all() const { return dat1.back(); }
};
#line 4 "/home/andyli/lib/ds/sqrtfenwicktree_01.hpp"

using SqrtFenwickTree_01 = FenwickTree_01Base<SqrtFenwickTree>;
#line 7 "/home/andyli/OneDrive/lixiang/code/lib/test/library_checker/static_range_inversions_query.3.test.cpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    dRV(pi, Q, q);
    auto I = argsort(a);
    vi b(n);
    _for (i, n)
        b[I[i]] = i;
    a = std::move(b);
    FenwickTree_01 ft(n);
    vc<i64> pre(n + 1), suf(n + 1);
    _for (i, n) {
        pre[i + 1] = pre[i] + ft.prod(a[i], n);
        ft.add(a[i]);
    }
    ft.build(n);
    _for_r (i, n) {
        suf[i] = suf[i + 1] + ft.prod(a[i]);
        ft.add(a[i]);
    }
    auto Im = Mo::get_order(Q, n);
    int l = 0, r = 0;
    vvc<std::tuple<int, int, int>> QL(n + 1), QR(n + 1);
    vc<i64> ans(q);
    foreach (i, Im) {
        auto [ql, qr] = Q[i];
        if (l > ql) {
            QR[r].eb(ql, l, ~i);
            ans[i] += suf[ql] - suf[l];
            l = ql;
        }
        if (r < qr) {
            QL[l].eb(r, qr, ~i);
            ans[i] += pre[qr] - pre[r];
            r = qr;
        }
        if (l < ql) {
            QR[r].eb(l, ql, i);
            ans[i] += suf[ql] - suf[l];
            l = ql;
        }
        if (r > qr) {
            QL[l].eb(qr, r, i);
            ans[i] += pre[qr] - pre[r];
            r = qr;
        }
    }
    SqrtFenwickTree_01 sft(n);
    _for (i, n) {
        foreach (l, r, j, QL[i]) {
            i64 s = 0;
            _for (k, l, r)
                s += sft.prod(a[k], n);
            if (j >= 0)
                ans[j] += s;
            else
                ans[~j] -= s;
        }
        sft.add(a[i]);
    }
    sft.build(n);
    _for_r (i, n) {
        foreach (l, r, j, QR[i + 1]) {
            i64 s = 0;
            _for (k, l, r)
                s += sft.prod(a[k]);
            if (j >= 0)
                ans[j] += s;
            else
                ans[~j] -= s;
        }
        sft.add(a[i]);
    }
    _for (i, q - 1)
        ans[Im[i + 1]] += ans[Im[i]];
    io.displayArray(ans, '\n');
    return 0;
}
