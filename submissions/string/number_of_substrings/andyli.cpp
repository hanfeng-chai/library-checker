#line 1 "/home/andyli/OneDrive/lixiang/code/lib/test/library_checker/number_of_substrings.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"

#line 2 "/home/andyli/lib/all.hpp"
#if defined(LX_LOCAL)
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
template <typename T>
using vc = std::vector<T>;
template <typename T>
using vvc = vc<vc<T>>;
template <typename T>
using vvvc = vc<vvc<T>>;
using vi = vc<int>;
using vvi = vvc<int>;
using vvvi = vvvc<int>;
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
concept TupleLike = requires { typename std::tuple_element_t<0, std::decay_t<T>>; } && !requires (T t) { t[0]; };
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
auto concat(auto&& a, auto&&... b) {
    (a.insert(a.end(), all(b)), ...);
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
#line 3 "/home/andyli/lib/io/mmapreader.hpp"

#if defined(__unix__)
#include <sys/mman.h>
#include <sys/stat.h>

struct MmapReader {
    static constexpr int ev = 0;

    char* ip;
    MmapReader(FILE* f) {
        struct stat st;
        int fd;
        if (f)
            fd = fileno(f), fstat(fd, &st), ip = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    }
    MmapReader(std::string_view s): MmapReader(fopen(s.data(), "r")) {}
    int getch() { return *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() const { return *ip; }
    void ireadstr(char* s, usize n) {
        memcpy(s, ip, n);
        ip += n;
    }
    usize next_size(auto&& f) const {
        char* p = ip;
        while (!f(*p))
            p++;
        return p - ip;
    }
    void skipws() {
        while (*ip <= ' ')
            ip++;
    }
    void rd(char* s) {
        skipws();
        auto n = next_size([](char c) { return c <= ' '; });
        ireadstr(s, n);
        s[n] = 0;
    }
    void rd(str& s) {
        skipws();
        auto n = next_size([](char c) { return c <= ' '; });
        s.assign(ip, n);
        ip += n;
    }
    void readline(char* s) {
        auto n = next_size([](char c) { return c == '\n' || c == ev; });
        ireadstr(s, n), ip++;
        s[n] = 0;
    }
    void readline(str& s) {
        auto n = next_size([](char c) { return c == '\n' || c == ev; });
        s.assign(ip, n);
        ip += n + 1;
    }
    void readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
        s[n] = 0;
    }
    void readstr(str& s, usize n) {
        skipws();
        s.assign(ip, n);
        ip += n;
    }
    char*& raw_ip() { return ip; }
    template <int>
    void ensure() {}
};
#endif
#line 3 "/home/andyli/lib/io/freadreader.hpp"

struct FreadReader {
    static constexpr int ev = EOF;
    static constexpr usize bufSize = 1 << 20;

    FILE* in;
    char ibuf[bufSize + 8], *ip = ibuf, *eip = ibuf;
    bool eoi = false;
    FreadReader(FILE* f): in(f) {}
    FreadReader(std::string_view s): FreadReader(fopen(s.data(), "r")) {}
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
    int getch() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip; }
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
    void skipws() {
        while (peek() <= ' ')
            ip++;
    }
    void rd(char* s) {
        skipws();
        int ch = peek();
        while (ch > ' ')
            *s++ = ch, ip++, ch = peek();
        *s = 0;
    }
    void rd(str& s) {
        skipws();
        s.clear();
        int ch = peek();
        while (ch > ' ')
            s.push_back(ch), ip++, ch = peek();
    }
    void readline(char* s) {
        int ch = peek();
        while (ch != '\n' && ch != this->ev)
            *s++ = ch, ip++, ch = peek();
        *s = 0;
    }
    void readline(str& s) {
        s.clear();
        int ch = peek();
        while (ch != '\n' && ch != this->ev)
            s.push_back(ch), ip++, ch = peek();
    }
    void readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
        s[n] = 0;
    }
    void readstr(str& s, usize n) {
        skipws();
        s.resize(n);
        ireadstr(s.data(), n);
    }
    char*& raw_ip() { return ip; }
    template <int N>
    void ensure() {
        if (eip - ip < N) [[unlikely]]
            load();
    }
};
#line 3 "/home/andyli/lib/io/fwritewriter.hpp"

struct FwriteWriter {
    FILE* out;
    char obuf[1 << 20], *op = obuf;
    FwriteWriter(FILE* f): out(f) {}
    FwriteWriter(std::string_view s): FwriteWriter(fopen(s.data(), "w")) {}
    void putch(char c) {
        if (op == end(obuf))
            flush();
        putch_unchecked(c);
    }
    void putch_unchecked(char c) { *op++ = c; }
    void writestr(const char* s, usize n) {
        if (n >= usize(end(obuf) - op)) [[unlikely]]
            flush(), fwrite(s, 1, n, out);
        else
            memcpy(op, s, n), op += n;
    }
    void flush() { fwrite(obuf, 1, op - obuf, out), op = obuf; }
    template <int N>
    void ensure() {
        if (end(obuf) - op < N) [[unlikely]]
            flush();
    }
};
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
#line 6 "/home/andyli/lib/io/io.hpp"

#ifdef LX_DEBUG
#include <io/io2.hpp>
#endif

#ifndef FASTIO
#define FASTIO 1

template <typename Reader, typename Writer>
struct IO: Reader, Writer {
    u32 prec = 12;
    IO(FILE* i = stdin, FILE* o = stdout): Reader(i), Writer(o) {}
    ~IO() { this->flush(); }

    using Reader::getch;

    void read(auto&... x) { (rd(x), ...); }
    template <typename T = int>
    T read() {
        std::decay_t<T> x;
        return rd(x), x;
    }
    template <std::forward_iterator I>
    void readArray(I f, I l) {
        while (f != l)
            rd(*f++);
    }
    void readArray(forward_range auto&& r) { readArray(all(r)); }
    void zipread(auto&&... a) {
        _for (i, (len(a), ...))
            read(a[i]...);
    }

    void setprec(u32 n) { prec = n; }
    void write(auto&&... x) { (wt(FORWARD(x)), ...); }
    void writeln(auto&&... x) { write(FORWARD(x)..., '\n'); }
    void print() { wt('\n'); }
    void print(auto&&... x) { write(std::forward_as_tuple(FORWARD(x)...), '\n'); }

    template <typename I, typename T = std::iter_value_t<I>>
    static constexpr char default_delim = TupleLike<T> || input_range<T> ? '\n' : ' ';
    template <std::input_iterator I, std::sentinel_for<I> S>
    void print_range(I f, S l, char d = default_delim<I>) {
        if (f != l)
            for (wt(*f++); f != l; write(d, *f++)) {}
    }
    template <std::input_iterator I, std::sentinel_for<I> S>
    void displayArray(I f, S l, char d = default_delim<I>) { print_range(f, l, d), print(); }
    template <input_range R>
    void displayArray(R&& r, char d = default_delim<iterator_t<R>>) { displayArray(all(r), d); }

private:
    using Reader::rd;

    template <Unsigned T>
    void parse_int(T& x) {
        auto& ip = Reader::raw_ip();
        loop {
            u64 v;
            memcpy(&v, ip, 8);
            v -= 0x3030303030303030;
            if (v & 0x8080808080808080)
                break;
            v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0xffffffff;
            ip += 8;
            if constexpr (sizeof(T) < 8) {
                x = v;
                break;
            }
            x = 100000000 * x + v;
        }
        {
            u32 v;
            memcpy(&v, ip, 4);
            v -= 0x30303030;
            if (!(v & 0x80808080)) {
                v = (v * 10 + (v >> 8)) & 0xff00ff;
                v = (v * 100 + (v >> 16)) & 0xffff;
                x = 10000 * x + v;
                ip += 4;
            }
        }
        {
            u16 v;
            memcpy(&v, ip, 2);
            v -= 0x3030;
            if (!(v & 0x8080)) {
                v = (v * 10 + (v >> 8)) & 0xff;
                x = 100 * x + v;
                ip += 2;
            }
        }
        if (*ip >= '0')
            x = 10 * x + (*ip++ ^ 48);
    }
    template <Signed T>
    void rd(T& x) {
        this->skipws();
        Reader::template ensure<64>();
        make_unsigned_t<T> t{};
        if (this->peek() == '-') {
            this->getch_unchecked();
            parse_int(t);
            t = -t;
        }
        else
            parse_int(t);
        x = t;
    }
    void rd(Unsigned auto& x) {
        this->skipws();
        Reader::template ensure<64>();
        x = 0;
        parse_int(x);
    }
    void rd(std::floating_point auto& x) {
        static str s;
        rd(s);
        std::from_chars(s.begin().base(), s.end().base(), x);
    }
    void rd(char& c) {
        this->skipws();
        c = getch();
    }
    void rd(TupleLike auto& t) {
        std::apply([&](auto&... t) { read(t...); }, t);
    }
    void rd(forward_range auto&& r) { return readArray(FORWARD(r)); }
    template <typename T>
    requires requires (T& t, IO& io) { t.read(io); }
    void rd(T& t) { t.read(*this); }

    struct WriteInt {
        Writer& writer;
        template <int N = 4>
        void lead(u64 x) {
            if constexpr (N > 1)
                if (x < ten(N - 1)) {
                    lead<N - 1>(x);
                    return;
                }
            writer.op = std::copy_n(&itos_table[x * 4 + (4 - N)], N, writer.op);
        }
        template <int N>
        void wt4(u64 x) {
            if constexpr (N > 0) {
                writer.op = std::copy_n(&itos_table[x / ten(N - 4) * 4], 4, writer.op);
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
    void wt(T x) {
        Writer::template ensure<64>();
        make_unsigned_t<T> y = x;
        if (x < 0)
            this->putch_unchecked('-'), y = -y;
        WriteInt{*this}.write(y);
    }
    void wt(Unsigned auto x) {
        Writer::template ensure<64>();
        WriteInt{*this}.write(x);
    }
    void wt(char c) { this->putch(c); }
    void wt(std::floating_point auto x) {
        static char buf[512];
        this->writestr(buf, std::to_chars(buf, buf + 512, x, std::chars_format::fixed, prec).ptr - buf);
    }
    void wt(std::string_view s) { this->writestr(s.data(), s.size()); }
    template <TupleLike T>
    void wt(T&& t) {
        std::apply([&](auto&& x, auto&&... y) { wt(FORWARD(x)), (write(' ', FORWARD(y)), ...); }, FORWARD(t));
    }
    template <input_range R>
    requires (!std::same_as<range_value_t<R>, char>)
    void wt(R&& r) { print_range(all(r)); }
    template <typename T>
    requires requires (T&& t, IO& io) { FORWARD(t).write(io); }
    void wt(T&& t) { FORWARD(t).write(*this); }
};

#if defined(__unix__) && !defined(LX_LOCAL)
using IO_t = IO<MmapReader, FwriteWriter>;
#else
using IO_t = IO<FreadReader, FwriteWriter>;
#endif

inline IO_t io;

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
inline void YES(bool v = true) { io.write(v ? "YES\n" : "NO\n"); }
inline void NO(bool v = true) { YES(!v); }
inline void Yes(bool v = true) { io.write(v ? "Yes\n" : "No\n"); }
inline void No(bool v = true) { Yes(!v); }
#endif  
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
#line 3 "/home/andyli/lib/ds/sparse_table.hpp"

template <typename Monoid>
struct Sparse_Table {
    using MX = Monoid;
    using X = MX::value_type;

    int n, lg;
    vvc<X> st;

    Sparse_Table() = default;
    Sparse_Table(int n) { build(n); }
    template <std::convertible_to<X> T>
    Sparse_Table(const vc<T>& a) { build(a); }
    Sparse_Table(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int) { return MX::unit(); });
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
                st[i + 1][j] = MX::op(st[i][j], st[i][j + (1 << i)]);
        }
    }
    X prod(int l, int r) const {
        if (l == r)
            return MX::unit();
        if (l + 1 == r)
            return st[0][l];
        int k = 31 - __builtin_clz(r - l - 1);
        return MX::op(st[k][l], st[k][r - (1 << k)]);
    }
    int max_right(auto&& check, int l) const {
        ASSERT(0 <= l && l <= n && check(MX::unit()));
        if (l == n)
            return n;
        return bsearch([&](int r) { return check(prod(l, r)); }, l, n + 1);
    }
    int min_left(auto&& check, int r) const {
        ASSERT(0 <= r && r <= n && check(MX::unit()));
        if (r == 0)
            return 0;
        return bsearch([&](int l) { return check(prod(l, r)); }, r, -1);
    }
};
#line 3 "/home/andyli/lib/ds/static_range_product.hpp"

template <template <typename> typename ST, typename Monoid, int LG = 4>
struct Static_Range_Product {
    using MX = Monoid;
    using X = MX::value_type;

    int n;
    vc<X> a, pre, suf;
    ST<MX> st;
    Static_Range_Product() = default;
    Static_Range_Product(int n) { build(n); }
    template <std::convertible_to<X> T>
    Static_Range_Product(const vc<T>& a) { build(a); }
    Static_Range_Product(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int i) { return MX::unit(); });
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
                pre[i] = MX::op(pre[i - 1], a[i]);
        _for_r (i, 1, n)
            if (i & mask)
                suf[i - 1] = MX::op(a[i - 1], suf[i]);
        st.build(n >> LG, [&](int i) { return suf[i << LG]; });
    }
    X prod(int l, int r) const {
        if (l == r)
            return MX::unit();
        r--;
        int x = l >> LG, y = r >> LG;
        if (x < y)
            return MX::op(MX::op(suf[l], st.prod(x + 1, y)), pre[r]);
        X t = a[l];
        _for (i, l + 1, r + 1)
            t = MX::op(t, a[i]);
        return t;
    }
    int max_right(auto&& check, int l) const {
        ASSERT(0 <= l && l <= n && check(MX::unit()));
        if (l == n)
            return n;
        return bsearch([&](int r) { return check(prod(l, r)); }, l, n + 1);
    }
    int min_left(auto&& check, int r) const {
        ASSERT(0 <= r && r <= n && check(MX::unit()));
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
    static constexpr bool commute() { return true; }
};
#line 5 "/home/andyli/lib/string/suffix_array.hpp"

// When the i-th suffix in lexicographical order starts at the j-th character,
// sa[i] = j, isa[j] = i
struct Suffix_Array {
    int n;
    vi sa, isa, LCP;
    Static_Range_Product<Sparse_Table, Monoid_Min<int>> st;

    Suffix_Array(std::string_view s) {
        n = len(s);
        char first = min(s), last = max(s);
        sa = calc_suffix_array(s, first, last);
        calc_lcp(s);
    }
    Suffix_Array(const vi& s) {
        n = len(s);
        sa = calc_suffix_array(s);
        calc_lcp(s);
    }
    void build_lcp() { st.build(LCP); }
    int lcp(int i, int j) {
        ASSERT(st.n);
        if (i == n || j == n)
            return 0;
        if (i == j)
            return n - i;
        i = isa[i], j = isa[j];
        if (i > j)
            swap(i, j);
        return st.prod(i, j);
    }
    // [l, r) such that lcp with s[i:] is at least k
    pi lcp_range(int i, int k) {
        ASSERT(st.n);
        if (i == n)
            return {0, n};
        i = isa[i];
        auto check = [&](int x) { return x >= k; };
        return {st.min_left(check, i), st.max_right(check, i) + 1};
    }
    int compare(int l0, int r0, int l1, int r1) {
        int n0 = r0 - l0, n1 = r1 - l1;
        int m = lcp(l0, l1);
        if (m >= min(n0, n1))
            return n0 - n1;
        return isa[l0] - isa[l1];
    }
    i64 count_substrings() {
        i64 ret = i64(n) * (n + 1) / 2;
        _for (i, n - 1)
            ret -= LCP[i];
        return ret;
    }

    static void induced_sort(const vi& a, int val_range, vi& sa, const vcb& sl,
      const vi& lms_idx) {
        vi l(val_range), r(val_range);
        foreach (c, a) {
            if (c + 1 < val_range)
                l[c + 1]++;
            r[c]++;
        }
        partial_sum(all(l), l.begin());
        partial_sum(all(r), r.begin());
        fill(sa, -1);
        _for_r (i, len(lms_idx))
            sa[--r[a[lms_idx[i]]]] = lms_idx[i];
        foreach (i, sa)
            if (i >= 1 && sl[i - 1])
                sa[l[a[i - 1]]++] = i - 1;
        fill(r, 0);
        foreach (c, a)
            r[c]++;
        partial_sum(all(r), r.begin());
        for (int k = len(sa) - 1, i = sa[k]; k >= 1; i = sa[--k])
            if (i >= 1 && !sl[i - 1])
                sa[--r[a[i - 1]]] = i - 1;
    }
    vi SA_IS(const vi& a, int val_range) {
        const int n = len(a);
        vi sa(n), lms_idx;
        vcb sl(n);
        _for_r (i, n - 1) {
            sl[i] = (a[i] > a[i + 1] || (a[i] == a[i + 1] && sl[i + 1]));
            if (sl[i] && !sl[i + 1])
                lms_idx.eb(i + 1);
        }
        reverse(lms_idx);
        induced_sort(a, val_range, sa, sl, lms_idx);
        vi new_lms_idx(len(lms_idx)), lms_vec(len(lms_idx));
        int k = 0;
        _for (i, n)
            if (!sl[sa[i]] && sa[i] >= 1 && sl[sa[i] - 1])
                new_lms_idx[k++] = sa[i];
        int cur = 0;
        sa[n - 1] = cur;
        _for (k, 1, len(new_lms_idx)) {
            int i = new_lms_idx[k - 1], j = new_lms_idx[k];
            if (a[i] != a[j]) {
                sa[j] = ++cur;
                continue;
            }
            for (int p = i + 1, q = j + 1;; p++, q++) {
                if (a[p] != a[q]) {
                    sa[j] = ++cur;
                    break;
                }
                if ((!sl[p] && sl[p - 1]) || (!sl[q] && sl[q - 1])) {
                    sa[j] = (sl[p] || !sl[p - 1] || sl[q] || !sl[q - 1] ? ++cur : cur);
                    break;
                }
            }
        }
        _for (i, len(lms_idx))
            lms_vec[i] = sa[lms_idx[i]];
        if (cur + 1 < len(lms_idx)) {
            auto lms_sa = SA_IS(lms_vec, cur + 1);
            _for (i, len(lms_idx))
                new_lms_idx[i] = lms_idx[lms_sa[i]];
        }
        induced_sort(a, val_range, sa, sl, new_lms_idx);
        return sa;
    }
    vi calc_suffix_array(std::string_view s, char first, char last) {
        vi a(len(s) + 1);
        _for (i, len(s))
            a[i] = s[i] - first + 1;
        auto ret = SA_IS(a, last - first + 2);
        ret.erase(ret.begin());
        return ret;
    }
    vi calc_suffix_array(const vi& s) {
        vi ss(s);
        UNIQUE(ss);
        vi a(len(s) + 1);
        _for (i, len(s))
            a[i] = LB(ss, s[i]) + 1;
        auto ret = SA_IS(a, len(ss) + 2);
        ret.erase(ret.begin());
        return ret;
    }
    void calc_lcp(auto&& s) {
        int n = len(s), k = 0;
        isa.resize(n);
        LCP.resize(n);
        _for (i, n)
            isa[sa[i]] = i;
        _for (i, n) {
            if (isa[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[isa[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k])
                k++;
            LCP[isa[i]] = k;
            if (k)
                k--;
        }
        LCP.pop_back();
    }
};
#line 5 "/home/andyli/OneDrive/lixiang/code/lib/test/library_checker/number_of_substrings.test.cpp"

int main() {
    dR(str, s);
    int n = len(s);
    Suffix_Array SA(s);
    print(i64(n) * i64(n + 1) / 2 - sum(SA.LCP, 0LL));
    return 0;
}
