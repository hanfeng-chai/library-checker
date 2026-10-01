#line 1 "library/test/yosupo/new/all_furthest_neighbors-SMAWK.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/all_furthest_neighbors"
#define GEOMETRY_REAL_TYPE ll
#line 2 "library/other/template.hpp"

#include <bits/stdc++.h>
#line 2 "library/template/macros.hpp"

#line 4 "library/template/macros.hpp"

#ifndef __COUNTER__
#define __COUNTER__ __LINE__
#endif

#define OVERLOAD5(a, b, c, d, e, ...) e
#define REP1_0(b, c) REP1_1(b, c)
#define REP1_1(b, c)                                                           \
    for (ll REP_COUNTER_##c = 0; REP_COUNTER_##c < (ll)(b); ++REP_COUNTER_##c)
#define REP1(b) REP1_0(b, __COUNTER__)
#define REP2(i, b) for (ll i = 0; i < (ll)(b); ++i)
#define REP3(i, a, b) for (ll i = (ll)(a); i < (ll)(b); ++i)
#define REP4(i, a, b, c) for (ll i = (ll)(a); i < (ll)(b); i += (ll)(c))
#define rep(...) OVERLOAD5(__VA_ARGS__, REP4, REP3, REP2, REP1)(__VA_ARGS__)
#define RREP2(i, a) for (ll i = (ll)(a)-1; i >= 0; --i)
#define RREP3(i, a, b) for (ll i = (ll)(b)-1; i >= (ll)(a); --i)
#define RREP4(i, a, b, c) for (ll i = (ll)(b)-1; i >= (ll)(a); i -= (ll)(c))
#define rrep(...) OVERLOAD5(__VA_ARGS__, RREP4, RREP3, RREP2)(__VA_ARGS__)
#define REPS2(i, b) for (ll i = 1; i <= (ll)(b); ++i)
#define REPS3(i, a, b) for (ll i = (ll)(a) + 1; i <= (ll)(b); ++i)
#define REPS4(i, a, b, c) for (ll i = (ll)(a) + 1; i <= (ll)(b); i += (ll)(c))
#define reps(...) OVERLOAD5(__VA_ARGS__, REPS4, REPS3, REPS2)(__VA_ARGS__)
#define RREPS2(i, a) for (ll i = (ll)(a); i > 0; --i)
#define RREPS3(i, a, b) for (ll i = (ll)(b); i > (ll)(a); --i)
#define RREPS4(i, a, b, c) for (ll i = (ll)(b); i > (ll)(a); i -= (ll)(c))
#define rreps(...) OVERLOAD5(__VA_ARGS__, RREPS4, RREPS3, RREPS2)(__VA_ARGS__)

#define each_for(...) for (auto&& __VA_ARGS__)
#define each_const(...) for (const auto& __VA_ARGS__)

#define all(v) std::begin(v), std::end(v)
#define rall(v) std::rbegin(v), std::rend(v)

#if __cpp_if_constexpr >= 201606L
#define IF_CONSTEXPR constexpr
#else
#define IF_CONSTEXPR
#endif

#define IO_BUFFER_SIZE (1 << 17)
#line 2 "library/template/alias.hpp"

#line 4 "library/template/alias.hpp"

using ll = long long;
using uint = unsigned int;
using ull = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
using ld = long double;
using PLL = std::pair<ll, ll>;
template<class T>
using prique = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template<class T> struct infinity {
    static constexpr T value = std::numeric_limits<T>::max() / 2;
    static constexpr T mvalue = std::numeric_limits<T>::lowest() / 2;
    static constexpr T max = std::numeric_limits<T>::max();
    static constexpr T min = std::numeric_limits<T>::lowest();
};

#if __cplusplus <= 201402L
template<class T> constexpr T infinity<T>::value;
template<class T> constexpr T infinity<T>::mvalue;
template<class T> constexpr T infinity<T>::max;
template<class T> constexpr T infinity<T>::min;
#endif

#if __cpp_variable_templates >= 201304L
template<class T> constexpr T INF = infinity<T>::value;
#endif

constexpr ll inf = infinity<ll>::value;
constexpr ld EPS = 1e-8;
constexpr ld PI = 3.1415926535897932384626;
#line 2 "library/template/type_traits.hpp"

#line 5 "library/template/type_traits.hpp"

template<class T, class... Args> struct function_traits_impl {
    using result_type = T;
    template<std::size_t idx>
    using argument_type =
        typename std::tuple_element<idx, std::tuple<Args...>>::type;
    using argument_tuple = std::tuple<Args...>;
    static constexpr std::size_t arg_size() { return sizeof...(Args); }
};

template<class> struct function_traits_helper;

template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...)> {
    using type = function_traits_impl<Res, Args...>;
};
template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...)&> {
    using type = function_traits_impl<Res, Args...>;
};
template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...) const> {
    using type = function_traits_impl<Res, Args...>;
};
template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...) const&> {
    using type = function_traits_impl<Res, Args...>;
};

#if __cpp_noexcept_function_type >= 201510L
template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...) noexcept> {
    using type = function_traits_impl<Res, Args...>;
};
template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...)& noexcept> {
    using type = function_traits_impl<Res, Args...>;
};
template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...) const noexcept> {
    using type = function_traits_impl<Res, Args...>;
};
template<class Res, class Tp, class... Args>
struct function_traits_helper<Res (Tp::*)(Args...) const& noexcept> {
    using type = function_traits_impl<Res, Args...>;
};
#endif

template<class F>
using function_traits = typename function_traits_helper<
    decltype(&std::remove_reference<F>::type::operator())>::type;

template<class F>
using function_result_type = typename function_traits<F>::result_type;
template<class F, std::size_t idx>
using function_argument_type =
    typename function_traits<F>::template argument_type<idx>;
template<class F>
using function_argument_tuple = typename function_traits<F>::argument_tuple;

template<class T>
using is_signed_int =
    std::integral_constant<bool, (std::is_integral<T>::value &&
                                  std::is_signed<T>::value) ||
                                     std::is_same<T, i128>::value>;
template<class T>
using is_unsigned_int =
    std::integral_constant<bool, (std::is_integral<T>::value &&
                                  std::is_unsigned<T>::value) ||
                                     std::is_same<T, u128>::value>;
template<class T>
using is_int = std::integral_constant<bool, is_signed_int<T>::value ||
                                                is_unsigned_int<T>::value>;
template<class T>
using make_signed_int = typename std::conditional<
    std::is_same<T, i128>::value || std::is_same<T, u128>::value,
    std::common_type<i128>, std::make_signed<T>>::type;
template<class T>
using make_unsigned_int = typename std::conditional<
    std::is_same<T, i128>::value || std::is_same<T, u128>::value,
    std::common_type<u128>, std::make_unsigned<T>>::type;


template<class T, class = void> struct is_range : std::false_type {};
template<class T>
struct is_range<
    T,
    decltype(all(std::declval<typename std::add_lvalue_reference<T>::type>()),
             (void)0)> : std::true_type {};

template<class T, bool = is_range<T>::value>
struct range_rank : std::integral_constant<std::size_t, 0> {};
template<class T>
struct range_rank<T, true>
    : std::integral_constant<std::size_t,
                             range_rank<typename T::value_type>::value + 1> {};

template<std::size_t size> struct int_least {
#ifndef IGNORE_TYPE_SIZE_ASSERT
    static_assert(size <= 128, "size must be less than or equal to 128");
#endif

    using type = typename std::conditional<
        size <= 8, std::int_least8_t,
        typename std::conditional<
            size <= 16, std::int_least16_t,
            typename std::conditional<
                size <= 32, std::int_least32_t,
                typename std::conditional<size <= 64, std::int_least64_t,
                                          i128>::type>::type>::type>::type;
};

template<std::size_t size> using int_least_t = typename int_least<size>::type;

template<std::size_t size> struct uint_least {
#ifndef IGNORE_TYPE_SIZE_ASSERT
    static_assert(size <= 128, "size must be less than or equal to 128");
#endif

    using type = typename std::conditional<
        size <= 8, std::uint_least8_t,
        typename std::conditional<
            size <= 16, std::uint_least16_t,
            typename std::conditional<
                size <= 32, std::uint_least32_t,
                typename std::conditional<size <= 64, std::uint_least64_t,
                                          u128>::type>::type>::type>::type;
};

template<std::size_t size> using uint_least_t = typename uint_least<size>::type;

template<class T>
using double_size_int = int_least<std::numeric_limits<T>::digits * 2 + 1>;
template<class T> using double_size_int_t = typename double_size_int<T>::type;
template<class T>
using double_size_uint = uint_least<std::numeric_limits<T>::digits * 2>;
template<class T> using double_size_uint_t = typename double_size_uint<T>::type;

template<class T>
using double_size =
    typename std::conditional<is_signed_int<T>::value, double_size_int<T>,
                              double_size_uint<T>>::type;
template<class T> using double_size_t = typename double_size<T>::type;
#line 2 "library/template/in.hpp"

#line 4 "library/template/in.hpp"
#include <unistd.h>
#line 8 "library/template/in.hpp"

template<std::size_t buf_size = IO_BUFFER_SIZE,
         std::size_t decimal_precision = 16>
class Scanner {
private:
    template<class, class = void> struct has_scan : std::false_type {};
    template<class T>
    struct has_scan<
        T, decltype(std::declval<T>().scan(std::declval<Scanner&>()), (void)0)>
        : std::true_type {};
    int fd;
    int idx, sz;
    bool state;
    std::array<char, buf_size + 1> buffer;
    inline char cur() {
        if (idx == sz) load();
        if (idx == sz) {
            state = false;
            return '\0';
        }
        return buffer[idx];
    }
    inline void next() {
        if (idx == sz) load();
        if (idx == sz) return;
        ++idx;
    }

public:
    inline void load() {
        assert(0 <= idx && idx <= sz && sz <= (int)buf_size);
        int len = sz - idx;
        if (idx < len) return;
        std::memcpy(buffer.begin(), buffer.begin() + idx, len);
        sz = len + read(fd, buffer.data() + len, buf_size - len);
        buffer[sz] = 0;
        idx = 0;
    }

    Scanner(int fd) : fd(fd), idx(0), sz(0), state(true) {}
    Scanner(FILE* fp) : fd(fileno(fp)), idx(0), sz(0), state(true) {}

    inline char scan_char() {
        if (idx == sz) load();
        return idx == sz ? '\0' : buffer[idx++];
    }

    Scanner ignore(int n = 1) {
        if (idx + n > sz) load();
        idx += n;
        return *this;
    }

    inline void discard_space() {
        if (idx == sz) load();
        while (('\t' <= buffer[idx] && buffer[idx] <= '\r') ||
               buffer[idx] == ' ') {
            if (++idx == sz) load();
        }
    }
    void scan(char& a) {
        discard_space();
        a = scan_char();
    }
    void scan(bool& a) {
        discard_space();
        a = scan_char() != '0';
    }
    void scan(std::string& a) {
        discard_space();
        a.clear();
        while (cur() != '\0' && (buffer[idx] < '\t' || '\r' < buffer[idx]) &&
               buffer[idx] != ' ') {
            a += scan_char();
        }
    }
    template<std::size_t len> void scan(std::bitset<len>& a) {
        discard_space();
        if (idx + len > sz) load();
        rrep (i, len) a[i] = buffer[idx++] != '0';
    }
    template<class T,
             typename std::enable_if<is_signed_int<T>::value &&
                                     !has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        discard_space();
        if (buffer[idx] == '-') {
            ++idx;
            if (idx + 40 > sz &&
                (idx == sz || ('0' <= buffer[sz - 1] && buffer[sz - 1] <= '9')))
                load();
            a = 0;
            while ('0' <= buffer[idx] && buffer[idx] <= '9') {
                a = a * 10 - (buffer[idx++] - '0');
            }
        }
        else {
            if (idx + 40 > sz && '0' <= buffer[sz - 1] && buffer[sz - 1] <= '9')
                load();
            a = 0;
            while ('0' <= buffer[idx] && buffer[idx] <= '9') {
                a = a * 10 + (buffer[idx++] - '0');
            }
        }
    }
    template<class T,
             typename std::enable_if<is_unsigned_int<T>::value &&
                                     !has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        discard_space();
        if (idx + 40 > sz && '0' <= buffer[sz - 1] && buffer[sz - 1] <= '9')
            load();
        a = 0;
        while ('0' <= buffer[idx] && buffer[idx] <= '9') {
            a = a * 10 + (buffer[idx++] - '0');
        }
    }
    template<class T,
             typename std::enable_if<std::is_floating_point<T>::value &&
                                     !has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        discard_space();
        bool sgn = false;
        if (cur() == '-') {
            sgn = true;
            next();
        }
        a = 0;
        while ('0' <= cur() && cur() <= '9') {
            a = a * 10 + cur() - '0';
            next();
        }
        if (cur() == '.') {
            next();
            T n = 0, d = 1;
            for (int i = 0;
                 '0' <= cur() && cur() <= '9' && i < (int)decimal_precision;
                 ++i) {
                n = n * 10 + cur() - '0';
                d *= 10;
                next();
            }
            while ('0' <= cur() && cur() <= '9') next();
            a += n / d;
        }
        if (sgn) a = -a;
    }

private:
    template<std::size_t i, class... Args> void scan(std::tuple<Args...>& a) {
        if IF_CONSTEXPR (i < sizeof...(Args)) {
            scan(std::get<i>(a));
            scan<i + 1, Args...>(a);
        }
    }

public:
    template<class... Args> void scan(std::tuple<Args...>& a) {
        scan<0, Args...>(a);
    }
    template<class T, class U> void scan(std::pair<T, U>& a) {
        scan(a.first);
        scan(a.second);
    }
    template<class T,
             typename std::enable_if<is_range<T>::value &&
                                     !has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        for (auto&& i : a) scan(i);
    }
    template<class T,
             typename std::enable_if<has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        a.scan(*this);
    }

    void operator()() {}
    template<class Head, class... Args>
    void operator()(Head& head, Args&... args) {
        scan(head);
        operator()(args...);
    }

    template<class T> Scanner& operator>>(T& a) {
        scan(a);
        return *this;
    }

    explicit operator bool() const { return state; }

    friend Scanner& getline(Scanner& scan, std::string& a) {
        a.erase();
        char c;
        if ((c = scan.scan_char()) == '\n' || c == '\0') return scan;
        a += c;
        while ((c = scan.scan_char()) != '\n' && c != '\0') a += c;
        scan.state = true;
        return scan;
    }
};

Scanner<> scan(0);
#line 2 "library/template/out.hpp"

#line 8 "library/template/out.hpp"

struct NumberToString {
    char buf[10000][4];
    constexpr NumberToString() : buf{} {
        rep (i, 10000) {
            int n = i;
            rrep (j, 4) {
                buf[i][j] = (char)('0' + n % 10);
                n /= 10;
            }
        }
    }
} constexpr precalc_number_to_string;

template<std::size_t buf_size = IO_BUFFER_SIZE, bool debug = false>
class Printer {
private:
    template<class, bool = debug, class = void>
    struct has_print : std::false_type {};
    template<class T>
    struct has_print<T, false,
                     decltype(std::declval<T>().print(std::declval<Printer&>()),
                              (void)0)> : std::true_type {};
    template<class T>
    struct has_print<T, true,
                     decltype(std::declval<T>().debug(std::declval<Printer&>()),
                              (void)0)> : std::true_type {};
    int fd;
    std::size_t idx;
    std::array<char, buf_size> buffer;

    std::size_t decimal_precision;

public:
    inline void print_char(char c) {
#if SHIO_LOCAL
        buffer[idx++] = c;
        if (idx == buf_size) flush();
#else
        if IF_CONSTEXPR (!debug) {
            buffer[idx++] = c;
            if (idx == buf_size) flush();
        }
#endif
    }
    inline void flush() {
        idx = write(fd, buffer.begin(), idx);
        idx = 0;
    }

    Printer(int fd) : fd(fd), idx(0), decimal_precision(16) {}
    Printer(FILE* fp) : fd(fileno(fp)), idx(0), decimal_precision(16) {}
    ~Printer() { flush(); }

    void set_decimal_precision(std::size_t decimal_precision) {
        this->decimal_precision = decimal_precision;
    }

    void print(char c) {
        if IF_CONSTEXPR (debug) print_char('\'');
        print_char(c);
        if IF_CONSTEXPR (debug) print_char('\'');
    }
    void print(bool b) { print_char((char)(b + '0')); }
    void print(const char* a) {
        if IF_CONSTEXPR (debug) print_char('"');
        for (; *a != '\0'; ++a) print_char(*a);
        if IF_CONSTEXPR (debug) print_char('"');
    }
    template<std::size_t len> void print(const char (&a)[len]) {
        if IF_CONSTEXPR (debug) print_char('"');
        for (auto i : a) print_char(i);
        if IF_CONSTEXPR (debug) print_char('"');
    }
    void print(const std::string& a) {
        if IF_CONSTEXPR (debug) print_char('"');
        for (auto i : a) print_char(i);
        if IF_CONSTEXPR (debug) print_char('"');
    }
    template<std::size_t len> void print(const std::bitset<len>& a) {
        rrep (i, len) print_char((char)(a[i] + '0'));
    }
    template<class T,
             typename std::enable_if<is_int<T>::value &&
                                     !has_print<T>::value>::type* = nullptr>
    void print(T a) {
        if (!a) {
            print_char('0');
            return;
        }
        if IF_CONSTEXPR (is_signed_int<T>::value) {
            if (a < 0) {
                print_char('-');
                using U = typename make_unsigned_int<T>::type;
                print(static_cast<U>(-static_cast<U>(a)));
                return;
            }
        }
        if (idx + 40 >= buf_size) flush();
        static char s[40];
        int t = 40;
        while (a >= 10000) {
            int i = a % 10000;
            a /= 10000;
            t -= 4;
            std::memcpy(s + t, precalc_number_to_string.buf[i], 4);
        }
        if (a >= 1000) {
            std::memcpy(buffer.begin() + idx, precalc_number_to_string.buf[a],
                        4);
            idx += 4;
        }
        else if (a >= 100) {
            std::memcpy(buffer.begin() + idx,
                        precalc_number_to_string.buf[a] + 1, 3);
            idx += 3;
        }
        else if (a >= 10) {
            std::memcpy(buffer.begin() + idx,
                        precalc_number_to_string.buf[a] + 2, 2);
            idx += 2;
        }
        else {
            buffer[idx++] = '0' | a;
        }
        std::memcpy(buffer.begin() + idx, s + t, 40 - t);
        idx += 40 - t;
    }
    template<class T,
             typename std::enable_if<std::is_floating_point<T>::value &&
                                     !has_print<T>::value>::type* = nullptr>
    void print(T a) {
        if (a == std::numeric_limits<T>::infinity()) {
            print("inf");
            return;
        }
        if (a == -std::numeric_limits<T>::infinity()) {
            print("-inf");
            return;
        }
        if (std::isnan(a)) {
            print("nan");
            return;
        }
        if (a < 0) {
            print_char('-');
            a = -a;
        }
        T b = a;
        if (b < 1) {
            print_char('0');
        }
        else {
            std::string s;
            while (b >= 1) {
                s += (char)('0' + (int)std::fmod(b, 10.0));
                b /= 10;
            }
            for (auto i = s.rbegin(); i != s.rend(); ++i) print_char(*i);
        }
        print_char('.');
        rep (decimal_precision) {
            a *= 10;
            print_char((char)('0' + (int)std::fmod(a, 10.0)));
        }
    }

private:
    template<std::size_t i, class... Args>
    void print(const std::tuple<Args...>& a) {
        if IF_CONSTEXPR (i < sizeof...(Args)) {
            if IF_CONSTEXPR (debug) print_char(',');
            print_char(' ');
            print(std::get<i>(a));
            print<i + 1, Args...>(a);
        }
    }

public:
    template<class... Args> void print(const std::tuple<Args...>& a) {
        if IF_CONSTEXPR (debug) print_char('(');
        if IF_CONSTEXPR (sizeof...(Args) != 0) print(std::get<0>(a));
        print<1, Args...>(a);
        if IF_CONSTEXPR (debug) print_char(')');
    }
    template<class T, class U> void print(const std::pair<T, U>& a) {
        if IF_CONSTEXPR (debug) print_char('(');
        print(a.first);
        if IF_CONSTEXPR (debug) print_char(',');
        print_char(' ');
        print(a.second);
        if IF_CONSTEXPR (debug) print_char(')');
    }
    template<class T,
             typename std::enable_if<is_range<T>::value &&
                                     !has_print<T>::value>::type* = nullptr>
    void print(const T& a) {
        if IF_CONSTEXPR (debug) print_char('{');
        for (auto i = std::begin(a); i != std::end(a); ++i) {
            if (i != std::begin(a)) {
                if IF_CONSTEXPR (debug) print_char(',');
                print_char(' ');
            }
            print(*i);
        }
        if IF_CONSTEXPR (debug) print_char('}');
    }
    template<class T, typename std::enable_if<has_print<T>::value &&
                                              !debug>::type* = nullptr>
    void print(const T& a) {
        a.print(*this);
    }
    template<class T, typename std::enable_if<has_print<T>::value &&
                                              debug>::type* = nullptr>
    void print(const T& a) {
        a.debug(*this);
    }

    void operator()() {}
    template<class Head, class... Args>
    void operator()(const Head& head, const Args&... args) {
        print(head);
        operator()(args...);
    }

    template<class T> Printer& operator<<(const T& a) {
        print(a);
        return *this;
    }

    Printer& operator<<(Printer& (*pf)(Printer&)) { return pf(*this); }
};

template<std::size_t buf_size, bool debug>
Printer<buf_size, debug>& endl(Printer<buf_size, debug>& pr) {
    pr.print_char('\n');
    pr.flush();
    return pr;
}
template<std::size_t buf_size, bool debug>
Printer<buf_size, debug>& flush(Printer<buf_size, debug>& pr) {
    pr.flush();
    return pr;
}

struct SetPrec {
    int n;
    template<class Pr> void print(Pr& pr) const { pr.set_decimal_precision(n); }
    template<class Pr> void debug(Pr& pr) const { pr.set_decimal_precision(n); }
};
SetPrec setprec(int n) { return SetPrec{n}; };

#if SHIO_LOCAL

Printer<> printer(1), eprinter(2);

#else

class NullPrinter {
public:
    template<class T> NullPrinter& operator<<(const T&) { return *this; }
    NullPrinter& operator<<(NullPrinter& (*)(NullPrinter&)) { return *this; }
    void print_char(char) {}
    template<class T> void print(const T&) {}
    template<class... Args> void operator()(const Args&...) {}
    template<class T> void set_decimal_precision(T) {}
};

NullPrinter& endl(NullPrinter& pr) { return pr; }
NullPrinter& flush(NullPrinter& pr) { return pr; }

Printer<> printer(1);
NullPrinter eprinter;

#endif

void prints() { printer.print_char('\n'); }

template<class T> auto prints(const T& v) -> decltype(printer << v, (void)0) {
    printer << v;
    printer.print_char('\n');
}

template<class Head, class... Tail>
auto prints(const Head& head, const Tail&... tail)
    -> decltype(printer << head, (void)0) {
    printer << head;
    printer.print_char(' ');
    prints(tail...);
}

#ifdef SHIO_LOCAL

Printer<IO_BUFFER_SIZE, true> debug(1), edebug(2);

#else

NullPrinter debug, edebug;

#endif

void debugs() { debug.print_char('\n'); }

template<class T> auto debugs(const T& v) -> decltype(debug << v, (void)0) {
    debug << v;
    debug.print_char('\n');
}

template<class Head, class... Tail>
auto debugs(const Head& head, const Tail&... tail)
    -> decltype(debug << head, (void)0) {
    debug << head;
    debug.print_char(' ');
    debugs(tail...);
}
#line 2 "library/template/bitop.hpp"

#line 6 "library/template/bitop.hpp"

namespace bitop {

#define KTH_BIT(b, k) (((b) >> (k)) & 1)
#define POW2(k) (1ull << (k))

inline ull next_combination(int n, ull x) {
    if (n == 0) return 1;
    ull a = x & -x;
    ull b = x + a;
    return (x & ~b) / a >> 1 | b;
}

#define rep_comb(i, n, k)                                                      \
    for (ull i = (1ull << (k)) - 1; i < (1ull << (n));                         \
         i = bitop::next_combination((n), i))

inline constexpr int msb(ull x) {
    int res = x ? 0 : -1;
    if (x & 0xFFFFFFFF00000000) x &= 0xFFFFFFFF00000000, res += 32;
    if (x & 0xFFFF0000FFFF0000) x &= 0xFFFF0000FFFF0000, res += 16;
    if (x & 0xFF00FF00FF00FF00) x &= 0xFF00FF00FF00FF00, res += 8;
    if (x & 0xF0F0F0F0F0F0F0F0) x &= 0xF0F0F0F0F0F0F0F0, res += 4;
    if (x & 0xCCCCCCCCCCCCCCCC) x &= 0xCCCCCCCCCCCCCCCC, res += 2;
    return res + ((x & 0xAAAAAAAAAAAAAAAA) ? 1 : 0);
}

inline constexpr int ceil_log2(ull x) { return x ? msb(x - 1) + 1 : 0; }

inline constexpr ull reverse(ull x) {
    x = ((x & 0xAAAAAAAAAAAAAAAA) >> 1) | ((x & 0x5555555555555555) << 1);
    x = ((x & 0xCCCCCCCCCCCCCCCC) >> 2) | ((x & 0x3333333333333333) << 2);
    x = ((x & 0xF0F0F0F0F0F0F0F0) >> 4) | ((x & 0x0F0F0F0F0F0F0F0F) << 4);
    x = ((x & 0xFF00FF00FF00FF00) >> 8) | ((x & 0x00FF00FF00FF00FF) << 8);
    x = ((x & 0xFFFF0000FFFF0000) >> 16) | ((x & 0x0000FFFF0000FFFF) << 16);
    return (x >> 32) | (x << 32);
}

inline constexpr ull reverse(ull x, int n) { return reverse(x) >> (64 - n); }

} // namespace bitop

inline constexpr int popcnt(ull x) noexcept {
#if __cplusplus >= 202002L
    return std::popcount(x);
#endif
    x = (x & 0x5555555555555555) + ((x >> 1) & 0x5555555555555555);
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x & 0x0f0f0f0f0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f0f0f0f0f);
    x = (x & 0x00ff00ff00ff00ff) + ((x >> 8) & 0x00ff00ff00ff00ff);
    x = (x & 0x0000ffff0000ffff) + ((x >> 16) & 0x0000ffff0000ffff);
    return (x & 0x00000000ffffffff) + ((x >> 32) & 0x00000000ffffffff);
}
#line 2 "library/template/func.hpp"

#line 6 "library/template/func.hpp"

template<class T, class U, class Comp = std::less<>>
inline constexpr bool chmin(T& a, const U& b,
                            Comp cmp = Comp()) noexcept(noexcept(cmp(b, a))) {
    return cmp(b, a) ? a = b, true : false;
}
template<class T, class U, class Comp = std::less<>>
inline constexpr bool chmax(T& a, const U& b,
                            Comp cmp = Comp()) noexcept(noexcept(cmp(a, b))) {
    return cmp(a, b) ? a = b, true : false;
}

inline constexpr ll gcd(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        const ll c = a;
        a = b;
        b = c % b;
    }
    return a;
}
inline constexpr ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }

inline constexpr bool is_prime(ll N) {
    if (N <= 1) return false;
    for (ll i = 2; i * i <= N; ++i) {
        if (N % i == 0) return false;
    }
    return true;
}
inline std::vector<ll> prime_factor(ll N) {
    std::vector<ll> res;
    for (ll i = 2; i * i <= N; ++i) {
        while (N % i == 0) {
            res.push_back(i);
            N /= i;
        }
    }
    if (N != 1) res.push_back(N);
    return res;
}

inline constexpr ll my_pow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res *= a;
        b >>= 1;
        a *= a;
    }
    return res;
}
inline constexpr ll mod_pow(ll a, ll b, ll mod) {
    assert(mod > 0);
    if (mod == 1) return 0;
    a %= mod;
    ll res = 1;
    while (b) {
        if (b & 1) (res *= a) %= mod;
        b >>= 1;
        (a *= a) %= mod;
    }
    return res;
}

inline PLL extGCD(ll a, ll b) {
    const ll n = a, m = b;
    ll x = 1, y = 0, u = 0, v = 1;
    ll t;
    while (b) {
        t = a / b;
        std::swap(a -= t * b, b);
        std::swap(x -= t * u, u);
        std::swap(y -= t * v, v);
    }
    if (x < 0) {
        x += m;
        y -= n;
    }
    return {x, y};
}
inline ll mod_inv(ll a, ll mod) {
    ll b = mod;
    ll x = 1, u = 0;
    ll t;
    while (b) {
        t = a / b;
        std::swap(a -= t * b, b);
        std::swap(x -= t * u, u);
    }
    if (x < 0) x += mod;
    assert(a == 1);
    return x;
}
#line 2 "library/template/util.hpp"

#line 6 "library/template/util.hpp"

template<class F> class RecLambda {
private:
    F f;

public:
    explicit constexpr RecLambda(F&& f_) : f(std::forward<F>(f_)) {}
    template<class... Args>
    constexpr auto operator()(Args&&... args)
        -> decltype(f(*this, std::forward<Args>(args)...)) {
        return f(*this, std::forward<Args>(args)...);
    }
};

template<class F> inline constexpr RecLambda<F> rec_lambda(F&& f) {
    return RecLambda<F>(std::forward<F>(f));
}


template<class Head, class... Tail> struct multi_dim_vector {
    using type = std::vector<typename multi_dim_vector<Tail...>::type>;
};
template<class T> struct multi_dim_vector<T> { using type = T; };

template<class T, class Arg>
constexpr std::vector<T> make_vec(int n, Arg&& arg) {
    return std::vector<T>(n, std::forward<Arg>(arg));
}
template<class T, class... Args>
constexpr typename multi_dim_vector<Args..., T>::type make_vec(int n,
                                                               Args&&... args) {
    return typename multi_dim_vector<Args..., T>::type(
        n, make_vec<T>(std::forward<Args>(args)...));
}


template<class T, class Comp = std::less<T>> class compressor {
private:
    std::vector<T> dat;
    Comp cmp;
    bool sorted = false;

public:
    compressor() : compressor(Comp()) {}
    compressor(const Comp& cmp) : cmp(cmp) {}
    compressor(const std::vector<T>& vec, bool f = false,
               const Comp& cmp = Comp())
        : dat(vec), cmp(cmp) {
        if (f) build();
    }
    compressor(std::vector<T>&& vec, bool f = false, const Comp& cmp = Comp())
        : dat(std::move(vec)), cmp(cmp) {
        if (f) build();
    }
    compressor(std::initializer_list<T> il, bool f = false,
               const Comp& cmp = Comp())
        : dat(all(il)), cmp(cmp) {
        if (f) build();
    }
    void reserve(int n) {
        assert(!sorted);
        dat.reserve(n);
    }
    void push_back(const T& v) {
        assert(!sorted);
        dat.push_back(v);
    }
    void push_back(T&& v) {
        assert(!sorted);
        dat.push_back(std::move(v));
    }
    template<class... Args> void emplace_back(Args&&... args) {
        assert(!sorted);
        dat.emplace_back(std::forward<Args>(args)...);
    }
    void push(const std::vector<T>& vec) {
        assert(!sorted);
        const int n = dat.size();
        dat.resize(n + vec.size());
        rep (i, vec.size()) dat[n + i] = vec[i];
    }
    int build() {
        assert(!sorted);
        sorted = true;
        std::sort(all(dat), cmp);
        dat.erase(std::unique(all(dat),
                              [&](const T& a, const T& b) -> bool {
                                  return !cmp(a, b) && !cmp(b, a);
                              }),
                  dat.end());
        return dat.size();
    }
    const T& operator[](int k) const& {
        assert(sorted);
        assert(0 <= k && k < (int)dat.size());
        return dat[k];
    }
    int get(const T& val) const {
        assert(sorted);
        auto itr = std::lower_bound(all(dat), val, cmp);
        assert(itr != dat.end() && !cmp(val, *itr));
        return itr - dat.begin();
    }
    int lower_bound(const T& val) const {
        assert(sorted);
        auto itr = std::lower_bound(all(dat), val, cmp);
        return itr - dat.begin();
    }
    int upper_bound(const T& val) const {
        assert(sorted);
        auto itr = std::upper_bound(all(dat), val, cmp);
        return itr - dat.begin();
    }
    bool contains(const T& val) const {
        assert(sorted);
        return std::binary_search(all(dat), val, cmp);
    }
    std::vector<int> pressed(const std::vector<T>& vec) const {
        assert(sorted);
        std::vector<int> res(vec.size());
        rep (i, vec.size()) res[i] = get(vec[i]);
        return res;
    }
    void press(std::vector<T>& vec) const {
        assert(sorted);
        for (auto&& i : vec) i = get(i);
    }
    int size() const {
        assert(sorted);
        return dat.size();
    }
};
#line 2 "library/geometry/Point.hpp"

#line 2 "library/geometry/template.hpp"

#line 4 "library/geometry/template.hpp"

#ifdef GEOMETRY_EPS
constexpr ld geom_eps = GEOMETRY_EPS;
#else
constexpr ld geom_eps = EPS;
#endif

#ifdef GEOMETRY_REAL_TYPE
using Real = GEOMETRY_REAL_TYPE;
// a <=> b  :  cmp(a, b) <=> 0
inline int cmp(Real a, Real b) {
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}
#else
using Real = ld;
// a <=> b  :  cmp(a, b) <=> 0
inline int cmp(ld a, ld b) {
    if (a > b + geom_eps) return 1;
    if (a < b - geom_eps) return -1;
    return 0;
}
#endif

#ifdef GEOMETRY_ANGLE_TYPE
using angle_t = GEOMETRY_ANGLE_TYPE;
#else
using angle_t = ld;
#endif
#line 4 "library/geometry/Point.hpp"

class Point {
public:
    Real x, y;
    Point() : x(0), y(0) {}
    Point(Real x, Real y) : x(x), y(y) {}
    Point& operator+=(const Point& p) {
        x += p.x;
        y += p.y;
        return *this;
    }
    Point& operator-=(const Point& p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    Point& operator*=(Real a) {
        x *= a;
        y *= a;
        return *this;
    }
    Point& operator/=(Real a) {
        x /= a;
        y /= a;
        return *this;
    }
    Point operator+() const { return *this; }
    Point operator-() const { return Point(-x, -y); }
    friend Point operator+(const Point& p1, const Point& p2) {
        return Point(p1) += p2;
    }
    friend Point operator-(const Point& p1, const Point& p2) {
        return Point(p1) -= p2;
    }
    friend Point operator*(const Point& p, Real a) { return Point(p) *= a; }
    friend Point operator*(Real a, const Point& p) { return Point(p) *= a; }
    friend Point operator/(const Point& p, Real a) { return Point(p) /= a; }
    friend bool operator==(const Point& p1, const Point& p2) {
        return cmp(p1.x, p2.x) == 0 && cmp(p1.y, p2.y) == 0;
    }
    friend bool operator!=(const Point& p1, const Point& p2) {
        return !(p1 == p2);
    }
    friend bool operator<(const Point& p1, const Point& p2) {
        return cmp(p1.x, p2.x) < 0 ||
               (cmp(p1.x, p2.x) == 0 && cmp(p1.y, p2.y) < 0);
    }
    friend bool operator>(const Point& p1, const Point& p2) { return p2 < p1; }
    friend bool operator<=(const Point& p1, const Point& p2) {
        return !(p2 < p1);
    }
    friend bool operator>=(const Point& p1, const Point& p2) {
        return !(p1 < p2);
    }
    friend bool comp_arg(const Point& p1, const Point& p2) {
        // -pi < theta <= pi
        int a1 = p1.y < 0 ? 0 : p1.y > 0 ? 2 : p1.x >= 0 ? 1 : 3;
        int a2 = p2.y < 0 ? 0 : p2.y > 0 ? 2 : p2.x >= 0 ? 1 : 3;
        if (a1 != a2) return a1 < a2;
        return cross(p1, p2) > 0;
    }
    Real norm() const { return x * x + y * y; }
    friend Real norm(const Point& p) { return p.norm(); }
#ifndef GEOMETRY_REAL_TYPE
    Real abs() const { return sqrt(norm()); }
    friend Real abs(const Point& p) { return p.abs(); }
    inline angle_t arg() const { return atan2((ld)y, (ld)x); }
    friend angle_t arg(const Point& p) { return p.arg(); }
    Point& rotate(angle_t theta) {
        Real c = cos(theta), s = sin(theta);
        Real nx = x * c - y * s, ny = x * s + y * c;
        x = nx;
        y = ny;
        return *this;
    }
    friend Point rotate(const Point& p, angle_t theta) {
        return Point(p).rotate(theta);
    }
#endif
    Point& rotate90() {
        Real nx = -y, ny = x;
        x = nx;
        y = ny;
        return *this;
    }
    friend Point rotate90(const Point& p) { return Point(p).rotate90(); }
    // inner product(内積), p1 * p2 = |p1| * |p2| * cos(theta)
    friend Real dot(const Point& p1, const Point& p2) {
        return p1.x * p2.x + p1.y * p2.y;
    }
    // outer product(外積), p1 ^ p2 = |p1| * |p2| * sin(theta)
    friend Real cross(const Point& p1, const Point& p2) {
        return p1.x * p2.y - p1.y * p2.x;
    }
    template<class Sc> void scan(Sc& scan) { scan >> x >> y; }
    template<class Pr> void print(Pr& print) const { print << x << ' ' << y; }
    template<class Pr> void debug(Pr& print) const {
        print.print_char('(');
        print << x;
        print.print_char(',');
        print << y;
        print.print_char(')');
    }
};

#ifndef GEOMETRY_REAL_TYPE
Real distance(const Point& p1, const Point& p2) { return abs(p1 - p2); }
#endif

enum class CCW {
    COUNTER_CLOCKWISE = 1,
    CLOCKWISE = -1,
    ONLINE_BACK = 2,
    ONLINE_FRONT = -2,
    ON_SEGMENT = 0,
};

CCW ccw(const Point& p0, const Point& p1, const Point& p2) {
    Point a = p1 - p0, b = p2 - p0;
    if (cmp(cross(a, b), 0) > 0) return CCW::COUNTER_CLOCKWISE;
    if (cmp(cross(a, b), 0) < 0) return CCW::CLOCKWISE;
    if (cmp(dot(a, b), 0) < 0) return CCW::ONLINE_BACK;
    if (a.norm() < b.norm()) return CCW::ONLINE_FRONT;
    return CCW::ON_SEGMENT;
}
#line 2 "library/dp/SMAWK.hpp"

#line 4 "library/dp/SMAWK.hpp"

template<class F> std::vector<int> smawk_comp(int H, int W, F&& cmp) {
    std::vector<int> row(H), col(W);
    std::iota(all(row), 0);
    std::iota(all(col), 0);
    return rec_lambda([&](auto&& self, const std::vector<int>& row,
                          const std::vector<int>& col) -> std::vector<int> {
        const int n = row.size();
        if (n == 0) return {};
        std::vector<int> ncol;
        ncol.reserve(n);
        for (int i : col) {
            while (!ncol.empty() && cmp(row[ncol.size() - 1], i, ncol.back()))
                ncol.pop_back();
            if ((int)ncol.size() < n) ncol.push_back(i);
        }
        std::vector<int> row_odd;
        row_odd.reserve(n / 2 + 1);
        rep (i, 1, n, 2) row_odd.push_back(row[i]);
        const std::vector<int> ans = self(row_odd, ncol);
        std::vector<int> res(n);
        rep (i, row_odd.size()) res[i * 2 + 1] = ans[i];
        int j = 0;
        rep (i, 0, n, 2) {
            const int last = i == n - 1 ? ncol.back() : res[i + 1];
            res[i] = ncol[j];
            while (ncol[j] < last) {
                ++j;
                if (cmp(row[i], ncol[j], res[i])) res[i] = ncol[j];
            }
        }
        return res;
    })(row, col);
}

template<class F> std::vector<int> smawk(int H, int W, F&& f) {
    return smawk_comp(H, W,
                      [&](int i, int j, int k) { return f(i, j) < f(i, k); });
}

/**
 * @brief SMAWK
 * @docs docs/dp/SMAWK.md
 */
#line 6 "library/test/yosupo/new/all_furthest_neighbors-SMAWK.test.cpp"
using namespace std;
int main() {
    int T; scan >> T;
    rep (T) {
        int n; scan >> n;
        vector<Point> p(n); scan >> p;
        p.resize(2 * n);
        rep (i, n) p[i + n] = p[i];
        auto idx = smawk_comp(n, 2 * n, [&](int i, int j, int k) -> bool {
            if (j < i || j > i + n || k < i || k > i + n) {
                return std::max<int>(i - j, j - i - n) < std::max<int>(i - k, k - i - n);
            }
            return norm(p[i] - p[j]) > norm(p[i] - p[k]);
        });
        rep (i, n) idx[i] %= n;
        prints(idx);
    }
}
