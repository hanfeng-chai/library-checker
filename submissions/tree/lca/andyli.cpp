#line 1 "/home/andyli/OneDrive/lixiang/code/r.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#pragma GCC optimize("-Ofast", "-funroll-loops")

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
#define FOR1(a) for (std::decay_t<decltype(a)> _ = 0; _ < (a); _++)
#define FOR2(i, a) for (std::decay_t<decltype(a)> i = 0; i < (a); i++)
#define FOR3(i, a, b) for (auto i = (a); i < (b); i++)
#define FOR4(i, a, b, c) for (auto i = (a); i < (b); i += (c))
#define FOR1_R(a) FOR2_R(i, a)
#define FOR2_R(i, a) for (auto i = (a); i--;)
#define FOR3_R(i, a, b) for (auto i = (b); i-- > (a);)
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define _for(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define _for_r(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
#define foreach(x, a) for (auto&& x: a)
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

class IO {
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

public:
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
        return std::apply([&](auto&... t) { (read(t), ...); }, t), *this;
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
            putch('-'), write(y = -y);
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

        u64 y = x;
        switch (y) {
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
    void print_range(auto f, auto l, char d = ' ') {
        if (f != l)
            for (write(*f++); f != l; write(*f++))
                putch(d);
    }
    template <tupleLike T>
    void write(T&& t) {
        [&]<auto... I>(std::index_sequence<I...>) {
            (..., (!I ? void() : putch(' '), write(std::get<I>(t))));
        }(std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>());
    }
    template <input_range R>
    requires (!std::same_as<range_value_t<R>, char>)
    void write(R&& r) { print_range(all(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.write(io); }
    void write(T&& t) { t.write(*this); }
    void writeln(auto&&... x) { write(FORWARD(x)...), print(); }
    void print() { putch('\n'); }
    void print(auto&& x, auto&&... y) {
        write(FORWARD(x));
        ((putch(' '), write(FORWARD(y))), ...);
        print();
    }
    template <std::input_iterator I>
    void displayArray(I f, I l, char d = ' ') { print_range(f, l, d), print(); }
    void displayArray(input_range auto&& r, char d = ' ') { displayArray(all(r), d); }
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
#line 4 "/home/andyli/OneDrive/lixiang/code/r.cpp"

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
#line 6 "/home/andyli/OneDrive/lixiang/code/r.cpp"

template <template <typename> typename ST, typename Monoid, int LG = 4>
struct Static_Range_Product {
    using M = Monoid;
    using X = M::value_type;

    int n;
    vc<X> a, pre, suf;
    ST<M> st;
    Static_Range_Product() = default;
    Static_Range_Product(int n) { build(n); }
    template <std::convertible_to<X> T>
    Static_Range_Product(const vc<T>& a) { build(a); }
    Static_Range_Product(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int i) { return M::unit(); });
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        a.resize(n);
        _for (i, n)
            a[i] = f(i);
        pre = suf = a;
        constexpr int mask = (1 << LG) - 1;
        _for (i, 1, n)
            if (i & mask)
                pre[i] = M::op(pre[i - 1], a[i]);
        _for_r (i, 1, n)
            if (i & mask)
                suf[i - 1] = M::op(a[i - 1], suf[i]);
        st.build(n >> LG, [&](int i) { return suf[i << LG]; });
    }
    void build2() {
        pre = suf = a;
        constexpr int mask = (1 << LG) - 1;
        _for (i, 1, n)
            if (i & mask)
                pre[i] = M::op(pre[i - 1], a[i]);
        _for_r (i, 1, n)
            if (i & mask)
                suf[i - 1] = M::op(a[i - 1], suf[i]);
        st.build(n >> LG, [&](int i) { return suf[i << LG]; });
    }
    X prod(int l, int r) const {
        if (l == r)
            return M::unit();
        r--;
        int x = l >> LG, y = r >> LG;
        if (x < y)
            return M::op(M::op(suf[l], st.prod(x + 1, y)), pre[r]);
        X t = a[l];
        _for (i, l + 1, r + 1)
            t = M::op(t, a[i]);
        return t;
    }
    int max_right(auto&& check, int l) const {
        ASSERT(0 <= l && l <= n && check(M::unit()));
        if (l == n)
            return n;
        return bsearch([&](int r) { return check(prod(l, r)); }, l, n + 1);
    }
    int min_left(auto&& check, int r) const {
        ASSERT(0 <= r && r <= n && check(M::unit()));
        if (r == 0)
            return 0;
        return bsearch([&](int l) { return check(prod(l, r)); }, r, -1);
    }
};

#line 3 "/home/andyli/lib/ds/sparse_table.hpp"

template <typename Monoid>
struct Sparse_Table {
    using M = Monoid;
    using X = M::value_type;

    int n, lg;
    vvc<X> st;

    Sparse_Table() = default;
    Sparse_Table(int n) { build(n); }
    template <std::convertible_to<X> T>
    Sparse_Table(const vc<T>& a) { build(a); }
    Sparse_Table(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int) { return M::unit(); });
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        lg = get_lg(n);
        st.resize(lg);
        st[0].resize(n);
        _for (i, n)
            st[0][i] = f(i);
        _for (i, lg - 1) {
            st[i + 1].resize(len(st[i]) - (1 << i));
            _for (j, len(st[i]) - (1 << i))
                st[i + 1][j] = M::op(st[i][j], st[i][j + (1 << i)]);
        }
    }
    X prod(int l, int r) const {
        if (l == r)
            return M::unit();
        if (l + 1 == r)
            return st[0][l];
        int k = 31 - __builtin_clz(r - l - 1);
        return M::op(st[k][l], st[k][r - (1 << k)]);
    }
    int max_right(auto&& check, int l) const {
        ASSERT(0 <= l && l <= n && check(M::unit()));
        if (l == n)
            return n;
        return bsearch([&](int r) { return check(prod(l, r)); }, l, n + 1);
    }
    int min_left(auto&& check, int r) const {
        ASSERT(0 <= r && r <= n && check(M::unit()));
        if (r == 0)
            return 0;
        return bsearch([&](int l) { return check(prod(l, r)); }, r, -1);
    }
};
#line 3 "/home/andyli/lib/monoid/min.hpp"

template <typename T>
struct Monoid_Min {
    using value_type = T;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) { return Min(a, b); }
    static constexpr X from_element(auto&& x) { return x; }
    static constexpr X unit() { return inf<X>; }
    static constexpr bool commute = true;
};
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
    class OutgoingEdges {
    public:
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
#line 3 "/home/andyli/lib/graph/tree.hpp"

template <typename G>
class Tree {
private:
    void dfs1(int u) {
        auto& size = rid;
        size[u] = 1;
        int l = g.indptr[u], r = g.indptr[u + 1];
        auto& csr = g.csr_edges;
        _for_r (i, l, r - 1)
            if (!dep[csr[i + 1]])
                swap(csr[i], csr[i + 1]);
        int max = 0;
        _for (i, l, r) {
            auto& v = csr[i];
            if (v == fa[u])
                continue;
            dep[v] = dep[u] + 1;
            if constexpr (G::is_weighted())
                wdep[v] = wdep[u] + v.cost;
            fa[v] = u;
            vtoe[v] = v.id;
            dfs1(v);
            size[u] += size[v];
            if (chkmax(max, size[v]))
                swap(csr[l], csr[i]);
        }
    }
    void dfs2(int u) {
        lid[u] = _id++;
        rid[u] += lid[u];
        id[lid[u]] = u;
        bool heavy = true;
        foreach (v, g[u])
            if (v != fa[u]) {
                top[v] = heavy ? top[u] : v;
                heavy = false;
                dfs2(v);
            }
    }

public:
    using graph_type = G;
    using cost_type = G::cost_type;
    int n, _id{};
    G& g;
    vi lid, rid, dep, top, fa, id, vtoe;
    vc<cost_type> wdep;
    Tree(G& g, int root = 0)
        : n(g.n), g(g),
          lid(n),
          rid(n),
          dep(n),
          top(n, root),
          fa(n, root),
          id(n),
          vtoe(n) {
        if constexpr (G::is_weighted())
            wdep.resize(n);
        build(root);
    }
    void build(int root) { dfs1(root), dfs2(root); }
    pi idx(int i) const { return {lid[i], rid[i]}; }
    bool in_subtree(int v, int u) const { return lid[u] <= lid[v] && lid[v] < rid[u]; }
    int size(int u) const { return rid[u] - lid[u]; }
    int size(int u, int r) const {
        if (u == r)
            return n;
        int v = jump(u, r, 1);
        if (in_subtree(u, v))
            return rid[u] - lid[u];
        return n - (rid[v] - lid[v]);
    }
    int e_to_v(int eid) const {
        auto&& e = g.edges[eid];
        return fa[e.from] == e.to ? e.from : e.to;
    }
    int v_to_e(int u) const { return vtoe[u]; }
    int elid(int u) const { return 2 * lid[u] - dep[u]; }
    int erid(int u) const { return 2 * rid[u] - dep[u] - 1; }
    vi child(int u) {
        vi r;
        foreach (v, g[u])
            if (v != fa[u])
                r.eb(v);
        return r;
    }
    virtual int lca(int u, int v) const {
        while (top[u] != top[v]) {
            if (lid[u] < lid[v])
                swap(u, v);
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }
    int lca(int u, int v, int r) const { return lca(u, v) ^ lca(u, r) ^ lca(v, r); }
    int k_ancestor(int u, int k) const {
        while (k > dep[u] - dep[top[u]]) {
            k -= dep[u] - dep[fa[top[u]]];
            u = fa[top[u]];
        }
        return id[lid[u] - k];
    }
    int jump(int u, int v, int k) const {
        int w = lca(u, v);
        if (dep[u] - dep[w] >= k)
            return k_ancestor(u, k);
        k = (dep[u] + dep[v] - dep[w] * 2) - k;
        if (k < 0)
            return -1;
        return k_ancestor(v, k);
    }
    bool in_path(int u, int v, int x) const {
        int w = lca(u, v);
        return (dep[x] <= dep[u] && dep[x] >= dep[w] && k_ancestor(u, dep[u] - dep[x]) == x) || (dep[x] <= dep[v] && dep[x] >= dep[w] && k_ancestor(v, dep[v] - dep[x]) == x);
    }
    int dist(int u, int v) const { return dep[u] + dep[v] - dep[lca(u, v)] * 2; }
    cost_type wdist(int u, int v) const requires (G::is_weighted())
    { return wdep[u] + wdep[v] - wdep[lca(u, v)] * 2; }
    vc<pi> path_decomposition(int u, int v, bool edge = false) const {
        vc<pi> up, down;
        while (top[u] != top[v])
            if (lid[u] < lid[v]) {
                down.eb(lid[top[v]], lid[v]);
                v = fa[top[v]];
            }
            else {
                up.eb(lid[u], lid[top[u]]);
                u = fa[top[u]];
            }
        if (lid[u] < lid[v])
            down.eb(lid[u] + edge, lid[v]);
        else if (lid[v] + edge <= lid[u])
            up.eb(lid[u], lid[v] + edge);
        up.insert(up.end(), rall(down));
        return up;
    }
    vi path(int u, int v) const {
        vi r;
        for (auto&& [a, b]: path_decomposition(u, v))
            if (a <= b)
                _for (i, a, b + 1)
                    r.eb(id[i]);
            else
                _for_r (i, b, a + 1)
                    r.eb(id[i]);
        return r;
    }
};
#line 3 "/home/andyli/lib/graph/fa_to_lid.hpp"

vi fa_to_lid(const auto& fa) {
    const int n = len(fa);
    vi lid(n);
    _for_r (i, 1, n)
        lid[fa[i]] += lid[i] + 1;
    _for (i, 1, n)
        lid[i] = std::exchange(lid[fa[i]], lid[fa[i]] - lid[i] - 1);
    return lid;
}
#line 84 "/home/andyli/OneDrive/lixiang/code/r.cpp"

int main() {
    dR(u32, n, q);
    vc<u32> fa(n);
    io.readArray(1 + all(fa));
    auto lid = fa_to_lid(fa);
    Static_Range_Product<Sparse_Table, Monoid_Min<int>> st;
    st.n = n;
    st.a.resize(n);
    _for (i, 1, n) {
        st.a[lid[i] - 1] = fa[i];
    }
    st.build2();
    _for (q) {
        dR(u32, u, v);
        u = lid[u], v = lid[v];
        if (u > v)
            swap(u, v);
        print(u32(st.prod(u, v)));
    }
    return 0;
}
