#line 1 "/home/andyli/OneDrive/lixiang/code/r.cpp"

// #define CHECK_EOF
// #define LX_DEBUG
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
    static constexpr char default_delim = tupleLike<T> || (input_range<T> && !requires (range_value_t<T> x) { {x} -> std::same_as<char&>; }) ? '\n' : ' ';
    template <std::input_iterator I, std::sentinel_for<I> S>
    void print_range(I f, S l, char d = default_delim<I>) {
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
#line 5 "/home/andyli/OneDrive/lixiang/code/r.cpp"

#line 3 "/home/andyli/lib/poly/fft.hpp"

namespace CFFT {
    template <typename T>
    struct Cp {
        T x{}, y{};
        Cp operator+(const Cp& c) const { return {x + c.x, y + c.y}; }
        Cp& operator+=(const Cp& c) { return *this = *this + c; }
        Cp operator-(const Cp& c) const { return {x - c.x, y - c.y}; }
        Cp& operator-=(const Cp& c) { return *this = *this - c; }
        Cp operator*(const Cp& c) const { return {x * c.x - y * c.y, x * c.y + y * c.x}; }
        Cp operator*=(const Cp& c) { return *this = *this * c; }
        Cp operator*(T z) const { return {x * z, y * z}; }
        Cp operator*=(T z) { return *this = *this * z; }
        Cp operator-() const { return {-x, -y}; }
        Cp conj() const { return {x, -y}; }
        Cp rotl() const { return {-y, x}; }
    };
    using C = Cp<f64>;
    vc<C> w;

    void setw(int k) {
        if (len(w) >= (1 << --k))
            return;
        w.resize(1 << k);
        w[0] = {1, 0};
        for (int i = 1; i < (1 << k); i <<= 1) {
            auto arg = std::numbers::pi / (i << 1);
            w[i] = {cos(arg), sin(arg)};
        }
        _for (i, 1, 1 << k)
            w[i] = w[i & (i - 1)] * w[lowbit(i)];
    }
    void fft(vc<C>& a, int k) {
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
        int u = 1 << (k & 1), v = 1 << (k - 2 - (k & 1));
        C imag = w[1];
        while (v) {
            for (int j0 = 0, j1 = v, j2 = j1 + v, j3 = j2 + v; j0 < v; j0++, j1++, j2++, j3++) {
                C t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                C t0p2 = t0 + t2, t1p3 = t1 + t3;
                C t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
            }
            _for (jh, 1, u) {
                C ww = w[jh], xx = w[jh << 1], wx = ww * xx;
                for (int j0 = (jh * v) << 2, j1 = j0 + v, j2 = j1 + v, j3 = j2 + v, je = j1; j0 < je; j0++, j1++, j2++, j3++) {
                    C t0 = a[j0], t1 = a[j1] * xx, t2 = a[j2] * ww, t3 = a[j3] * wx;
                    C t0p2 = t0 + t2, t1p3 = t1 + t3;
                    C t0m2 = t0 - t2, t1m3 = (t1 - t3) * w[1];
                    a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
                }
            }
            u <<= 2, v >>= 2;
        }
    }
    void ifft(vc<C>& a, int k) {
        if (len(a) <= 1)
            return;
        if (k == 1) {
            a[0] += std::exchange(a[1], a[0] - a[1]);
            return;
        }
        int u = 1 << (k - 2), v = 1;
        C imag = w[1].conj();
        while (u) {
            for (int j0 = 0, j1 = v, j2 = j1 + v, j3 = j2 + v; j0 < v; j0++, j1++, j2++, j3++) {
                C t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                C t0p1 = t0 + t1, t2p3 = t2 + t3;
                C t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
                a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
                a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
            }
            _for (jh, 1, u) {
                C ww = w[jh].conj(), xx = w[jh << 1].conj(), yy = w[(jh << 1) + 1].conj();
                for (int j0 = (jh * v) << 2, j1 = j0 + v, j2 = j1 + v, j3 = j2 + v, je = j1; j0 < je; j0++, j1++, j2++, j3++) {
                    C t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    C t0p1 = t0 + t1, t2p3 = t2 + t3;
                    C t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                    a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                    a[j1] = t0m1 + t2m3, a[j3] = (t0m1 - t2m3) * ww;
                }
            }
            u >>= 2, v <<= 2;
        }
        if (k & 1) {
            u = 1 << (k - 1);
            _for (j, u)
                a[j] += std::exchange(a[j + u], a[j] - a[j + u]);
        }
    }
} // namespace CFFT
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
#line 8 "/home/andyli/OneDrive/lixiang/code/r.cpp"

// 0: neg = false, dat = {}
struct bigint {
    using M = bigint;

    static constexpr int logD = 7;
    static constexpr u32 D = 1 << 28, B = 1 << 14;
    bool neg = false;
    vi dat;

    bigint() = default;
    bigint(bool n, vi d): neg(n), dat(std::move(d)) {}

    template <Signed T>
    bigint(T x) {
        make_unsigned_t<T> y = x;
        if (x < 0)
            neg = true, y = -y;
        dat = _from_int(y);
    }
    template <Unsigned T>
    bigint(T x): dat(_from_int(x)) {}

    bigint(std::string_view s) {
        if (s == "0")
            return;
        if (s[0] == '-')
            s = s.substr(1), neg = true;
        for (int ie = len(s); 0 < ie; ie -= logD) {
            int is = max(0, ie - logD);
            int x = 0;
            _for (i, is, ie) {
                x = x * 16 + (s[i] >= 'A' ? s[i] - 'A' + 10 : s[i] - '0');
            }
            dat.eb(x);
        }
    }
#ifdef FASTIO
    void read(IO& io) {
        static str s;
        io.read(s);
        *this = bigint(s);
    }
    void write(IO& io) const {
        if (dat.empty()) {
            io.putch('0');
            return;
        }
        if (neg)
            io.putch('-');
        _for_r (i, len(dat))
            io.write(_itos(dat[i], i != len(dat) - 1));
    }
#endif
    friend M operator+(const M& lhs, const M& rhs) {
        if (lhs.neg == rhs.neg)
            return {lhs.neg, _add(lhs.dat, rhs.dat)};
        if (_leq(lhs.dat, rhs.dat)) {
            auto c = _sub(rhs.dat, lhs.dat);
            bool n = rhs.neg && !c.empty();
            return {n, std::move(c)};
        }
        auto c = _sub(lhs.dat, rhs.dat);
        bool n = lhs.neg && !c.empty();
        return {n, std::move(c)};
    }
    friend M operator-(const M& lhs, const M& rhs) {
        if (rhs.dat.empty())
            return lhs;
        if (lhs.neg == !rhs.neg)
            return {lhs.neg, _add(lhs.dat, rhs.dat)};
        if (_leq(lhs.dat, rhs.dat)) {
            auto c = _sub(rhs.dat, lhs.dat);
            bool n = !rhs.neg && !c.empty();
            return {n, std::move(c)};
        }
        auto c = _sub(lhs.dat, rhs.dat);
        bool n = lhs.neg && !c.empty();
        return {n, std::move(c)};
    }

    friend M operator*(const M& lhs, const M& rhs) {
        auto c = _mul(lhs.dat, rhs.dat);
        bool n = (lhs.neg ^ rhs.neg) && !c.empty();
        return {n, std::move(c)};
    }
    friend auto divmod(const M& lhs, const M& rhs) {
        auto dm = _divmod_newton(lhs.dat, rhs.dat);
        bool dn = lhs.neg != rhs.neg && !dm.first.empty();
        bool mn = lhs.neg && !dm.second.empty();
        return std::pair{M{dn, std::move(dm.first)}, M{mn, std::move(dm.second)}};
    }
    friend M operator/(const M& lhs, const M& rhs) { return divmod(lhs, rhs).first; }
    friend M operator%(const M& lhs, const M& rhs) { return divmod(lhs, rhs).second; }

    M& operator+=(const M& rhs) { return *this = *this + rhs; }
    M& operator-=(const M& rhs) { return *this = *this - rhs; }
    M& operator*=(const M& rhs) { return *this = *this * rhs; }
    M& operator/=(const M& rhs) { return *this = *this / rhs; }
    M& operator%=(const M& rhs) { return *this = *this % rhs; }

    M& operator++() { return *this += 1; }
    M operator++(int) {
        M t = *this;
        ++*this;
        return t;
    }
    M& operator--() { return *this -= 1; }
    M operator--(int) {
        M t = *this;
        --*this;
        return t;
    }

    M operator-() const {
        if (dat.empty())
            return *this;
        return {!neg, dat};
    }
    M operator+() const { return *this; }
    friend M abs(const M& m) { return {false, m.dat}; }

    friend bool operator==(const M& lhs, const M& rhs) = default;
    friend bool operator<(const M& lhs, const M& rhs) { return lhs != rhs && _neq_lt(lhs, rhs); }
    friend bool operator<=(const M& lhs, const M& rhs) { return !(rhs < lhs); }
    friend bool operator>(const M& lhs, const M& rhs) { return rhs < lhs; }
    friend bool operator>=(const M& lhs, const M& rhs) { return !(lhs < rhs); }

    template <typename T>
    T to_int() const {
        T r = _to_int<T>(dat);
        return neg ? -r : r;
    }

private:
    static bool _lt(const vi& a, const vi& b) {
        if (len(a) != len(b))
            return len(a) < len(b);
        _for_r (i, len(a))
            if (a[i] != b[i])
                return a[i] < b[i];
        return false;
    }
    static bool _leq(const vi& a, const vi& b) { return a == b || _lt(a, b); }
    static bool _neq_lt(const M& lhs, const M& rhs) {
        if (lhs.neg != rhs.neg)
            return lhs.neg;
        return _lt(lhs.dat, rhs.dat) ^ lhs.neg;
    }
    static bool _is_one(const vi& a) { return len(a) == 1 && a[0] == 1; }
    static void _shrink(vi& a) {
        while (!a.empty() && !a.back())
            a.pop_back();
    }
    static vi _add(const vi& a, const vi& b) {
        vi c(max(len(a), len(b)) + 1);
        _for (i, max(len(a), len(b))) {
            if (i < len(a))
                c[i] += a[i];
            if (i < len(b))
                c[i] += b[i];
            if (c[i] >= D)
                c[i] -= D, c[i + 1]++;
        }
        _shrink(c);
        return c;
    }
    static vi _sub(vi a, const vi& b) {
        int t = 0;
        _for (i, len(a)) {
            if (i < len(b))
                t += b[i];
            a[i] -= t;
            t = 0;
            if (a[i] < 0)
                a[i] += D, t = 1;
        }
        _shrink(a);
        return a;
    }
    static vi _mul_naive(const vi& a, const vi& b) {
        auto c = convolution_naive<int, u64>(a, b);
        vi r(len(a) + len(b) + 2);
        u64 x = 0;
        _for (i, len(c)) {
            x += c[i];
            r[i] = x % D, x /= D;
        }
        int i = len(c);
        while (x)
            r[i++] = x % D, x /= D;
        _shrink(r);
        return r;
    }
    static vi _mul(const vi& a, const vi& b) {
        using namespace CFFT;
        if (a.empty() || b.empty())
            return {};
        if (_is_one(a))
            return b;
        if (_is_one(b))
            return a;
        int n = len(a), m = len(b);
        if (min(n, m) <= 30)
            return _mul_naive(a, b);
        int k = get_lg(n + m), sz = 1 << k;
        vc<C> c(sz), d(sz);
        _for (i, n)
            c[i] = {f64(a[i] % B), f64(a[i] / B)};
        _for (i, m)
            d[i] = {f64(b[i] % B), f64(b[i] / B)};
        setw(k);
        fft(c, k), fft(d, k);
        f64 fx = 1.0 / sz, fy = fx * 0.25;
        c[0] = {c[0].x * d[0].x + c[0].y * d[0].y, c[0].x * d[0].y + c[0].y * d[0].x};
        c[0] *= fx;
        c[1] *= d[1] * fx;
        for (int k = 2, m = 3; k < sz; k <<= 1, m <<= 1)
            for (int i = k, j = i + k - 1; i < m; i++, j--) {
                C oi = c[i] + c[j].conj(), hi = c[i] - c[j].conj();
                C oj = d[i] + d[j].conj(), hj = d[i] - d[j].conj();
                C r0 = oi * oj - hi * hj * ((i & 1) ? -w[i >> 1] : w[i >> 1]);
                C r1 = oj * hi + oi * hj;
                c[i] = (r0 + r1) * fy;
                c[j] = (r0 - r1).conj() * fy;
            }
        ifft(c, k);
        vi r(n + m + 2);
        u64 x = 0;
        _for (i, n + m) {
            x += i64(c[i].x + 0.5) + i64(c[i].y + 0.5) * B;
            r[i] = x % D, x /= D;
        }
        int i = n + m;
        while (x)
            r[i++] = x % D, x /= D;
        _shrink(r);
        return r;
    }
    static auto _divmod_li(const vi& a, const vi& b) {
        i64 va = _to_int<i64>(a), vb = b[0];
        return std::pair{_from_int(va / vb), _from_int(va % vb)};
    }
    static auto _divmod_i64(const vi& a, const vi& b) {
        i64 va = _to_int<i64>(a), vb = _to_int<i64>(b);
        return std::pair{_from_int(va / vb), _from_int(va % vb)};
    }
    static auto _divmod_1e8(const vi& a, const vi& b) {
        if (b[0] == 1)
            return std::pair{a, vi{}};
        if (len(a) <= 2)
            return _divmod_li(a, b);
        vi quo(len(a));
        i64 d = 0;
        int b0 = b[0];
        _for_r (i, len(a)) {
            d = d * D + a[i];
            int q = d / b0, r = d % b0;
            quo[i] = q, d = r;
        }
        _shrink(quo);
        return std::pair{std::move(quo), d ? vi{int(d)} : vi{}};
    }
    static auto _divmod_naive(const vi& a, const vi& b) {
        ASSERT(!b.empty());
        if (len(b) == 1)
            return _divmod_1e8(a, b);
        if (max(len(a), len(b)) <= 2)
            return _divmod_i64(a, b);
        if (_lt(a, b))
            return std::pair{vi{}, a};
        int norm = D / (b.back() + 1);
        vi x = _mul(a, {norm});
        vi y = _mul(b, {norm});
        int yb = y.back();
        vi quo(len(x) - len(y) + 1);
        vi rem(x.end() - len(y), x.end());
        _for_r (i, len(quo)) {
            if (len(rem) == len(y)) {
                if (_leq(y, rem))
                    quo[i] = 1, rem = _sub(rem, y);
            }
            else if (len(rem) > len(y)) {
                i64 rb = i64(rem[len(rem) - 1]) * D + rem[len(rem) - 2];
                int q = rb / yb;
                vi yq = _mul(y, {q});
                while (_lt(rem, yq))
                    q--, yq = _sub(yq, y);
                rem = _sub(rem, yq);
                while (_leq(y, rem))
                    q++, rem = _sub(rem, y);
                quo[i] = q;
            }
            if (i)
                rem.insert(rem.begin(), x[i - 1]);
        }
        _shrink(quo), _shrink(rem);
        auto [q2, r2] = _divmod_1e8(rem, {norm});
        return std::pair{std::move(quo), std::move(q2)};
    }
    static vi _calc_inv(const vi& a, int deg) {
        int k = deg, c = len(a);
        while (k > 64)
            k = (k + 1) >> 1;
        vi z(c + k + 1);
        z.back() = 1;
        z = _divmod_naive(z, a).first;
        while (k < deg) {
            vi s = _mul(z, z);
            s.insert(s.begin(), 0);
            int d = min(c, 2 * k + 1);
            vi t(a.end() - d, a.end()), u = _mul(s, t);
            u.erase(u.begin(), u.begin() + d);
            vi w(k + 1), w2 = _add(z, z);
            w.insert(w.end(), all(w2));
            z = _sub(w, u);
            z.erase(z.begin());
            k <<= 1;
        }
        z.erase(z.begin(), z.begin() + k - deg);
        return z;
    }
    static std::pair<vi, vi> _divmod_newton(const vi& a, const vi& b) {
        ASSERT(!b.empty());
        if (len(b) <= 64 || len(a) - len(b) <= 64)
            return _divmod_naive(a, b);
        int norm = D / (b.back() + 1);
        vi x = _mul(a, {norm}), y = _mul(b, {norm});
        int s = len(x), t = len(y);
        int deg = s - t + 2;
        vi z = _calc_inv(y, deg);
        vi q = _mul(x, z);
        q.erase(q.begin(), q.begin() + t + deg);
        vi yq = _mul(y, {q});
        while (_lt(x, yq))
            q = _sub(q, {1}), yq = _sub(yq, y);
        vi r = _sub(x, yq);
        while (_leq(y, r))
            q = _add(q, {1}), r = _sub(r, y);
        _shrink(q), _shrink(r);
        auto [q2, r2] = _divmod_1e8(r, {norm});
        return {std::move(q), std::move(q2)};
    }
    static str _itos(int x, bool zero_padding) {
        str r;
        _for (i, logD) {
            int t = x & 15;
            r += char(t < 10 ? '0' + t : 'A' + t - 10);
            x >>= 4;
        }
        if (!zero_padding)
            while (!r.empty() && r.back() == '0')
                r.pop_back();
        reverse(r);
        return r;
    }
    static vi _from_int(Integer auto x) {
        vi r;
        while (x)
            r.eb(x % D), x /= D;
        return r;
    }
    template <typename T>
    static T _to_int(const vi& a) {
        T r{};
        _for_r (i, len(a))
            r = r * D + a[i];
        return r;
    }
};

int main() {
    multipleTests([&] {
        dR(bigint, a, b);
        print(a * b);
    });
    return 0;
}
