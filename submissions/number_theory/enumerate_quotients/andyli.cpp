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
constexpr u64 ten(int t) { return t == 0 ? 1 : ten(t - 1) * 10; }
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
    char ibuf[bufSize], *eip;
    bool eoi = false;
    void load() {
        if (eoi) [[unlikely]]
            return;
        int sz = eip - ip;
        memcpy(ibuf, ip, sz);
        eip = ibuf + sz + fread(ibuf + sz, 1, bufSize - sz, in);
        if (eip == ibuf + sz) [[unlikely]]
            eoi = true;
        ip = ibuf;
    }
    void skipws() {
        int ch = getch();
        while (blank(ch))
            ch = getch();
        unget();
    }
    int unget(int = 0) { return *ip--; }
    int getch() { return (ip == eip ? (eip = (ip = ibuf) + fread(ibuf, 1, bufSize, in)) : nullptr), ip == eip ? -1 : *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return (ip == eip ? (eip = (ip = ibuf) + fread(ibuf, 1, bufSize, in)) : nullptr), ip == eip ? -1 : *ip; }
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
    int unget(int = 0) = delete;
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
    static constexpr auto I = [] {
        std::array<u32, 0x10000> I{};
        fill(I, -1);
        _for (i, 10)
            _for (j, 10)
                I[i + j * 0x100 + 0x3030] = i * 10 + j;
        return I;
    }();
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
    template <Signed T>
    IO& read(T& x) {
        x = 0;
        make_unsigned_t<T> t;
        bool sign = false;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch))
            sign = ch == '-', ch = getch();
        if (eip - ip < 64) [[unlikely]]
            load();
        t = ch ^ 48;
#else
        while (!isdigit(*ip))
            sign = *ip++ == '-';
        t = *ip++ ^ 48;
#endif
        u16* tip = (u16*)ip;
        while (~I[*tip])
            t = t * 100 + I[*tip++];
        ip = (char*)tip;
        if (isdigit(*ip))
            t = t * 10 + (*ip++ ^ 48);
        x = sign ? -t : t;
        return *this;
    }
    IO& read(Unsigned auto& x) {
        x = 0;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch))
            ch = getch();
        if (eip - ip < 64) [[unlikely]]
            load();
        x = ch ^ 48;
#else
        while (!isdigit(*ip))
            ip++;
        x = *ip++ ^ 48;
#endif
        u16* tip = (u16*)ip;
        while (~I[*tip])
            x = x * 100 + I[*tip++];
        ip = (char*)tip;
        if (isdigit(*ip))
            x = x * 10 + (*ip++ ^ 48);
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
    template <Signed T>
    void write(T x) {
        make_unsigned_t<T> y = x;
        if (x < 0)
            write('-', y = -y);
        else
            write(y);
    }
    void write(std::unsigned_integral auto x) {
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
    }
    void write(u128 x) {
        if (end(obuf) - op < 64) [[unlikely]]
            flush();
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
#line 3 "/home/andyli/lib/math/fixpoint.hpp"

template <typename T>
T fixpoint(T x, auto&& f) {
    T xn = f(x);
    while (x < xn) {
        x = xn;
        xn = f(x);
    }
    while (xn < x) {
        x = xn;
        xn = f(x);
    }
    return x;
}
#line 3 "/home/andyli/lib/math/isqrt.hpp"

template <Unsigned T>
T isqrt(T a) {
    if (a < 4)
        return a > 0;
    auto guess = [&](T x) -> T { return sqrt(x); };
    auto next = [&](T x) { return (a / x + x) >> 1; };
    return fixpoint(guess(a), next);
}
template <Signed T>
T isqrt(T a) {
    ASSERT(a >= 0);
    return isqrt<std::make_unsigned_t<T>>(a);
}
#line 3 "/home/andyli/OneDrive/lixiang/code/q.cpp"

int main() {
    dR(u64, n);
    u32 sq = isqrt(n);
    u32 m = (u64(sq) * sq + sq <= n ? sq : sq - 1);
    print(sq + m);
    _for (i, m)
        io.write(i + 1), io.putch_unchecked(' ');
    _for_r (i, sq)
        io.write(n / (i + 1)), io.putch_unchecked(' ');
    print();
    return 0;
}
