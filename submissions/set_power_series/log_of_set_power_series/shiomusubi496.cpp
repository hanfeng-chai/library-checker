#line 1 "library/test/yosupo/set_power_series/log_of_set_power_series.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/log_of_set_power_series"
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
#line 2 "library/math/ModInt.hpp"

#line 4 "library/math/ModInt.hpp"

template<class T, T mod> class StaticModInt {
    static_assert(std::is_integral<T>::value, "T must be integral");
    static_assert(std::is_unsigned<T>::value, "T must be unsigned");
    static_assert(mod > 0, "mod must be positive");
    static_assert(mod <= std::numeric_limits<T>::max() / 2,
                  "mod * 2 must be less than or equal to T::max()");

private:
    using large_t = typename double_size_uint<T>::type;
    using signed_t = typename std::make_signed<T>::type;
    T val;
    static constexpr unsigned int inv1000000007[] = {
        0,         1,         500000004, 333333336, 250000002, 400000003,
        166666668, 142857144, 125000001, 111111112, 700000005};
    static constexpr unsigned int inv998244353[] = {
        0,         1,         499122177, 332748118, 748683265, 598946612,
        166374059, 855638017, 873463809, 443664157, 299473306};

    static constexpr ll mod_inv(ll a) {
        ll b = mod;
        ll x = 1, u = 0;
        ll t = 0, tmp = 0;
        while (b) {
            t = a / b;
            tmp = (a - t * b);
            a = b;
            b = tmp;
            tmp = (x - t * u);
            x = u;
            u = tmp;
        }
        if (x < 0) x += mod;
        return x;
    }

public:
    constexpr StaticModInt() : val(0) {}
    template<class U,
             typename std::enable_if<std::is_integral<U>::value &&
                                     std::is_signed<U>::value>::type* = nullptr>
    constexpr StaticModInt(U v) : val{} {
        v %= static_cast<signed_t>(mod);
        if (v < 0) v += static_cast<signed_t>(mod);
        val = static_cast<T>(v);
    }
    template<class U, typename std::enable_if<
                          std::is_integral<U>::value &&
                          std::is_unsigned<U>::value>::type* = nullptr>
    constexpr StaticModInt(U v) : val(v % mod) {}
    constexpr T get() const { return val; }
    static constexpr T get_mod() { return mod; }
    static constexpr StaticModInt raw(T v) {
        StaticModInt res;
        res.val = v;
        return res;
    }
    constexpr StaticModInt inv() const {
        if IF_CONSTEXPR (mod == 1000000007) {
            if (val <= 10) return inv1000000007[val];
        }
        else if IF_CONSTEXPR (mod == 998244353) {
            if (val <= 10) return inv998244353[val];
        }
        return mod_inv(val);
    }
    constexpr StaticModInt& operator++() {
        ++val;
        if (val == mod) val = 0;
        return *this;
    }
    constexpr StaticModInt operator++(int) {
        StaticModInt res = *this;
        ++*this;
        return res;
    }
    constexpr StaticModInt& operator--() {
        if (val == 0) val = mod;
        --val;
        return *this;
    }
    constexpr StaticModInt operator--(int) {
        StaticModInt res = *this;
        --*this;
        return res;
    }
    constexpr StaticModInt& operator+=(const StaticModInt& other) {
        val += other.val;
        if (val >= mod) val -= mod;
        return *this;
    }
    constexpr StaticModInt& operator-=(const StaticModInt& other) {
        if (val < other.val) val += mod;
        val -= other.val;
        return *this;
    }
    constexpr StaticModInt& operator*=(const StaticModInt& other) {
        large_t a = val;
        a *= other.val;
        a %= mod;
        val = a;
        return *this;
    }
    constexpr StaticModInt& operator/=(const StaticModInt& other) {
        *this *= other.inv();
        return *this;
    }
    friend constexpr StaticModInt operator+(const StaticModInt& lhs,
                                            const StaticModInt& rhs) {
        return StaticModInt(lhs) += rhs;
    }
    friend constexpr StaticModInt operator-(const StaticModInt& lhs,
                                            const StaticModInt& rhs) {
        return StaticModInt(lhs) -= rhs;
    }
    friend constexpr StaticModInt operator*(const StaticModInt& lhs,
                                            const StaticModInt& rhs) {
        return StaticModInt(lhs) *= rhs;
    }
    friend constexpr StaticModInt operator/(const StaticModInt& lhs,
                                            const StaticModInt& rhs) {
        return StaticModInt(lhs) /= rhs;
    }
    constexpr StaticModInt operator+() const { return StaticModInt(*this); }
    constexpr StaticModInt operator-() const { return StaticModInt() - *this; }
    friend constexpr bool operator==(const StaticModInt& lhs,
                                     const StaticModInt& rhs) {
        return lhs.val == rhs.val;
    }
    friend constexpr bool operator!=(const StaticModInt& lhs,
                                     const StaticModInt& rhs) {
        return lhs.val != rhs.val;
    }
    constexpr StaticModInt pow(ll a) const {
        StaticModInt v = *this, res = 1;
        while (a) {
            if (a & 1) res *= v;
            a >>= 1;
            v *= v;
        }
        return res;
    }
    template<class Pr> void print(Pr& a) const { a.print(val); }
    template<class Pr> void debug(Pr& a) const { a.print(val); }
    template<class Sc> void scan(Sc& a) {
        ll v;
        a.scan(v);
        *this = v;
    }
};

#if __cplusplus < 201703L
template<class T, T mod>
constexpr unsigned int StaticModInt<T, mod>::inv1000000007[];
template<class T, T mod>
constexpr unsigned int StaticModInt<T, mod>::inv998244353[];
#endif

template<unsigned int p> using static_modint = StaticModInt<unsigned int, p>;
using modint1000000007 = static_modint<1000000007>;
using modint998244353 = static_modint<998244353>;

template<class T, int id> class DynamicModInt {
    static_assert(std::is_integral<T>::value, "T must be integral");
    static_assert(std::is_unsigned<T>::value, "T must be unsigned");

private:
    using large_t = typename double_size_uint<T>::type;
    using signed_t = typename std::make_signed<T>::type;
    T val;
    static T mod;

public:
    constexpr DynamicModInt() : val(0) {}
    template<class U,
             typename std::enable_if<std::is_integral<U>::value &&
                                     std::is_signed<U>::value>::type* = nullptr>
    constexpr DynamicModInt(U v) : val{} {
        v %= static_cast<signed_t>(mod);
        if (v < 0) v += static_cast<signed_t>(mod);
        val = static_cast<T>(v);
    }
    template<class U, typename std::enable_if<
                          std::is_integral<U>::value &&
                          std::is_unsigned<U>::value>::type* = nullptr>
    constexpr DynamicModInt(U v) : val(v % mod) {}
    T get() const { return val; }
    static T get_mod() { return mod; }
    static void set_mod(T v) {
        assert(v > 0);
        assert(v <= std::numeric_limits<T>::max() / 2);
        mod = v;
    }
    static DynamicModInt raw(T v) {
        DynamicModInt res;
        res.val = v;
        return res;
    }
    DynamicModInt inv() const { return mod_inv(val, mod); }
    DynamicModInt& operator++() {
        ++val;
        if (val == mod) val = 0;
        return *this;
    }
    DynamicModInt operator++(int) {
        DynamicModInt res = *this;
        ++*this;
        return res;
    }
    DynamicModInt& operator--() {
        if (val == 0) val = mod;
        --val;
        return *this;
    }
    DynamicModInt operator--(int) {
        DynamicModInt res = *this;
        --*this;
        return res;
    }
    DynamicModInt& operator+=(const DynamicModInt& other) {
        val += other.val;
        if (val >= mod) val -= mod;
        return *this;
    }
    DynamicModInt& operator-=(const DynamicModInt& other) {
        if (val < other.val) val += mod;
        val -= other.val;
        return *this;
    }
    DynamicModInt& operator*=(const DynamicModInt& other) {
        large_t a = val;
        a *= other.val;
        a %= mod;
        val = a;
        return *this;
    }
    DynamicModInt& operator/=(const DynamicModInt& other) {
        *this *= other.inv();
        return *this;
    }
    friend DynamicModInt operator+(const DynamicModInt& lhs,
                                   const DynamicModInt& rhs) {
        return DynamicModInt(lhs) += rhs;
    }
    friend DynamicModInt operator-(const DynamicModInt& lhs,
                                   const DynamicModInt& rhs) {
        return DynamicModInt(lhs) -= rhs;
    }
    friend DynamicModInt operator*(const DynamicModInt& lhs,
                                   const DynamicModInt& rhs) {
        return DynamicModInt(lhs) *= rhs;
    }
    friend DynamicModInt operator/(const DynamicModInt& lhs,
                                   const DynamicModInt& rhs) {
        return DynamicModInt(lhs) /= rhs;
    }
    DynamicModInt operator+() const { return DynamicModInt(*this); }
    DynamicModInt operator-() const { return DynamicModInt() - *this; }
    friend bool operator==(const DynamicModInt& lhs, const DynamicModInt& rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const DynamicModInt& lhs, const DynamicModInt& rhs) {
        return lhs.val != rhs.val;
    }
    DynamicModInt pow(ll a) const {
        DynamicModInt v = *this, res = 1;
        while (a) {
            if (a & 1) res *= v;
            a >>= 1;
            v *= v;
        }
        return res;
    }
    template<class Pr> void print(Pr& a) const { a.print(val); }
    template<class Pr> void debug(Pr& a) const { a.print(val); }
    template<class Sc> void scan(Sc& a) {
        ll v;
        a.scan(v);
        *this = v;
    }
};

template<class T, int id> T DynamicModInt<T, id>::mod = 998244353;

template<int id> using dynamic_modint = DynamicModInt<unsigned int, id>;
using modint = dynamic_modint<-1>;

/**
 * @brief ModInt
 * @docs docs/math/ModInt.md
 */
#line 2 "library/math/sps/Composite.hpp"

#line 2 "library/math/Combinatorics.hpp"

#line 5 "library/math/Combinatorics.hpp"

template<class T> class Combinatorics {
private:
    static std::vector<T> factorial;
    static std::vector<T> factinv;

public:
    static void init(ll n) {
        const int b = factorial.size();
        if (n < b) return;
        factorial.resize(n + 1);
        rep (i, b, n + 1) factorial[i] = factorial[i - 1] * i;
        factinv.resize(n + 1);
        factinv[n] = T(1) / factorial[n];
        rreps (i, b, n) factinv[i - 1] = factinv[i] * i;
    }
    static T fact(ll x) {
        if (x < 0) return 0;
        init(x);
        return factorial[x];
    }
    static T finv(ll x) {
        if (x < 0) return 0;
        init(x);
        return factinv[x];
    }
    static T inv(ll x) {
        if (x <= 0) return 0;
        init(x);
        return factorial[x - 1] * factinv[x];
    }
    static T perm(ll n, ll r) {
        if (r < 0 || r > n) return 0;
        init(n);
        return factorial[n] * factinv[n - r];
    }
    static T comb(ll n, ll r) {
        if (n < 0) return 0;
        if (r < 0 || r > n) return 0;
        init(n);
        return factorial[n] * factinv[n - r] * factinv[r];
    }
    static T homo(ll n, ll r) { return comb(n + r - 1, r); }
    static T small_perm(ll n, ll r) {
        if (r < 0 || r > n) return 0;
        T res = 1;
        reps (i, r) res *= n - r + i;
        return res;
    }
    static T small_comb(ll n, ll r) {
        if (r < 0 || r > n) return 0;
        chmin(r, n - r);
        init(r);
        T res = factinv[r];
        reps (i, r) res *= n - r + i;
        return res;
    }
    static T small_homo(ll n, ll r) { return small_comb(n + r - 1, r); }
};

template<class T>
std::vector<T> Combinatorics<T>::factorial = std::vector<T>(1, 1);
template<class T>
std::vector<T> Combinatorics<T>::factinv = std::vector<T>(1, 1);

/**
 * @brief Combinatorics
 * @docs docs/math/Combinatorics.md
 */
#line 2 "library/math/convolution/SubsetConvolution.hpp"

#line 4 "library/math/convolution/SubsetConvolution.hpp"

namespace internal {

template<class T, int L> std::vector<std::array<T, L>> ranked_zeta(const std::vector<T>& a) {
    int n = a.size();
    std::vector<std::array<T, L>> b(n);
    rep (i, n) b[i][popcnt(i)] = a[i];
    for (int i = 1; i < n; i <<= 1) {
        rep (j, 0, n, i << 1) {
            rep (k, j, j + i) {
                rep (l, L) b[k + i][l] += b[k][l];
            }
        }
    }
    return b;
}
template<class T, int L> std::vector<T> ranked_moebius(std::vector<std::array<T, L>> b) {
    int n = b.size();
    for (int i = 1; i < n; i <<= 1) {
        rep (j, 0, n, i << 1) {
            rep (k, j, j + i) {
                rep (l, L) b[k + i][l] -= b[k][l];
            }
        }
    }
    std::vector<T> a(n);
    rep (i, n) a[i] = b[i][popcnt(i)];
    return a;
}

} // namespace internal

template<class T, int L>
std::vector<T> subset_convolution_pow2(const std::vector<T>& a) {
    int n = a.size(), m = bitop::ceil_log2(n);
    std::vector<std::array<T, L + 1>> a2 = internal::ranked_zeta<T, L + 1>(a);
    rep (k, n) {
        auto& f = a2[k];
        const auto g = a2[k];
        rrep (i, m + 1) {
            T sm = 0;
            rep (j, i + 1) sm += f[j] * g[i - j];
            f[i] = sm;
        }
    }
    return internal::ranked_moebius<T, L + 1>(a2);
}

template<class T, int L = 0>
std::vector<T> subset_convolution(const std::vector<T>& a,
                                  const std::vector<T>& b) {
    if (a.size() > (1 << L)) {
        return subset_convolution<T, std::min<int>(L + 1, 30)>(a, b);
    }
    if (a == b) {
        return subset_convolution_pow2<T, L>(a);
    }
    int n = a.size(), m = bitop::ceil_log2(n);
    std::vector<std::array<T, L + 1>> a2 = internal::ranked_zeta<T, L + 1>(a);
    std::vector<std::array<T, L + 1>> b2 = internal::ranked_zeta<T, L + 1>(b);
    rep (k, n) {
        auto& f = a2[k];
        const auto& g = b2[k];
        int c = popcnt(k);
        rrep (i, std::min<int>(c * 2, m) + 1) {
            T sm = 0;
            rep (j, std::max<int>(i - c, 0), i + 1) sm += f[j] * g[i - j];
            f[i] = sm;
        }
    }
    return internal::ranked_moebius<T, L + 1>(a2);
}

/**
 * @brief SubsetConvolution
 * @docs docs/math/convolution/SubsetConvolution.md
 */
#line 2 "library/math/poly/FormalPowerSeries.hpp"

#line 2 "library/math/convolution/Convolution.hpp"

#line 5 "library/math/convolution/Convolution.hpp"

constexpr ull primitive_root_for_convolution(ull p) {
    if (p == 2) return 1;
    if (p == 998244353) return 3;
    if (p == 469762049) return 3;
    if (p == 1811939329) return 11;
    if (p == 2013265921) return 11;
    rep (g, 2, p) {
        if (mod_pow(g, (p - 1) >> 1, p) != 1) return g;
    }
    return -1;
}

namespace internal {

template<class T> class NthRoot {
private:
    static constexpr unsigned int lg =
        bitop::msb((T::get_mod() - 1) & (1 - T::get_mod()));
    T root[lg + 1], inv_root[lg + 1];
    T rate[lg + 1], inv_rate[lg + 1];
    T rate3[lg + 1], inv_rate3[lg + 1];

public:
    constexpr NthRoot() : root{}, inv_root{}, rate{}, inv_rate{}, rate3{}, inv_rate3{} {
        root[lg] = T{primitive_root_for_convolution(T::get_mod())}.pow(
            (T::get_mod() - 1) >> lg);
        inv_root[lg] = root[lg].inv();
        rrep (i, lg) {
            root[i] = root[i + 1] * root[i + 1];
            inv_root[i] = inv_root[i + 1] * inv_root[i + 1];
        }
        T r = 1, ir = 1;
        rep (i, 2, lg + 1) {
            rate[i - 2] = r * root[i];
            inv_rate[i - 2] = ir * inv_root[i];
            r *= inv_root[i];
            ir *= root[i];
        }
        r = ir = 1;
        rep (i, 3, lg + 1) {
            rate3[i - 3] = r * root[i];
            inv_rate3[i - 3] = ir * inv_root[i];
            r *= inv_root[i];
            ir *= root[i];
        }
    }
    static constexpr unsigned int get_lg() { return lg; }
    constexpr T get(int n) const { return root[n]; }
    constexpr T inv(int n) const { return inv_root[n]; }
    constexpr T get_rate(int n) const { return rate[n]; }
    constexpr T get_inv_rate(int n) const { return inv_rate[n]; }
    constexpr T get_rate3(int n) const { return rate3[n]; }
    constexpr T get_inv_rate3(int n) const { return inv_rate3[n]; }
};

template<class T> void number_theoretic_transform(std::vector<T>& a) {
    static constexpr NthRoot<T> nth_root;
    static constexpr ull MOD = T::get_mod();
    static constexpr ull MOD2 = MOD * MOD;
    int n = a.size();
    for (int i = n >> 1; i > 0; i >>= 1) {
        if (i == 1) {
            T z = T::raw(1);
            rep (j, 0, n, i << 1) {
                rep (k, i) {
                    const T x = a[j + k];
                    const T y = a[j + i + k] * z;
                    a[j + k] = x + y;
                    a[j + i + k] = x - y;
                }
                z *= nth_root.get_rate(popcnt(j & ~(j + (i << 1))));
            }
        }
        else {
            i >>= 1;
            T z = 1, y = nth_root.get(2);
            rep (j, 0, n, i << 2) {
                T z2 = z * z, z3 = z2 * z;
                rep (k, i) {
                    ull a0 = a[j + k].get();
                    ull a1 = a[j + k + i].get() * (ull)z.get();
                    ull a2 = a[j + k + i * 2].get() * (ull)z2.get();
                    ull a3 = a[j + k + i * 3].get() * (ull)z3.get();
                    ull tmp = T(a1 + MOD2 - a3).get() * (ull)y.get();
                    a[j + k] = a0 + a2 + a1 + a3;
                    a[j + k + i] = a0 + a2 + MOD2 * 2 - a1 - a3;
                    a[j + k + i * 2] = a0 + MOD2 - a2 + tmp;
                    a[j + k + i * 3] = a0 + MOD2 * 2 - a2 - tmp;
                }
                z *= nth_root.get_rate3(popcnt(j & ~(j + (i << 2))));
            }
        }
    }
}

template<class T> void inverse_number_theoretic_transform(std::vector<T>& a) {
    static constexpr NthRoot<T> nth_root;
    static constexpr ull MOD = T::get_mod();
    int n = a.size();
    for (int i = 1; i < n; i <<= 1) {
        if (i == n / 2) {
            T z = T::raw(1);
            rep (j, 0, n, i << 1) {
                rep (k, i) {
                    const T x = a[j + k];
                    const T y = a[j + i + k];
                    a[j + k] = x + y;
                    a[j + i + k] = (x - y) * z;
                }
                z *= nth_root.get_inv_rate(popcnt(j & ~(j + (i << 1))));
            }
        }
        else {
            T z = 1, y = nth_root.inv(2);
            rep (j, 0, n, i << 2) {
                T z2 = z * z, z3 = z2 * z;
                rep (k, i) {
                    ull a0 = a[j + k].get();
                    ull a1 = a[j + k + i].get();
                    ull a2 = a[j + k + i * 2].get();
                    ull a3 = a[j + k + i * 3].get();
                    ull tmp = (a2 + MOD - a3) * (ull)y.get() % MOD;
                    a[j + k] = a0 + a2 + a1 + a3;
                    a[j + k + i] = (a0 + MOD - a1 + tmp) * (ull)z.get();
                    a[j + k + i * 2] = (a0 + a1 + 2 * MOD - a2 - a3) * (ull)z2.get();
                    a[j + k + i * 3] = (a0 + 2 * MOD - a1 - tmp) * (ull)z3.get();
                }
                z *= nth_root.get_inv_rate3(popcnt(j & ~(j + (i << 2))));
            }
            i <<= 1;
        }
    }
    T inv_n = T(1) / n;
    for (auto&& x : a) x *= inv_n;
}

template<class T>
std::vector<T> convolution_naive(const std::vector<T>& a,
                                 const std::vector<T>& b) {
    int n = a.size(), m = b.size();
    std::vector<T> c(n + m - 1);
    rep (i, n)
        rep (j, m) c[i + j] += a[i] * b[j];
    return c;
}

template<class T> std::vector<T> convolution_pow2(std::vector<T> a) {
    int n = a.size() * 2 - 1;
    int lg = bitop::msb(n - 1) + 1;
    if (n - (1 << (lg - 1)) <= 5) {
        --lg;
        int m = a.size() - (1 << (lg - 1));
        std::vector<T> a1(a.begin(), a.begin() + m), a2(a.begin() + m, a.end());
        std::vector<T> c(n);
        std::vector<T> c1 = convolution_naive(a1, a1);
        std::vector<T> c2 = convolution_naive(a1, a2);
        std::vector<T> c3 = convolution_pow2(a2);
        rep (i, c1.size()) c[i] += c1[i];
        rep (i, c2.size()) c[i + m] += c2[i] * 2;
        rep (i, c3.size()) c[i + m * 2] += c3[i];
        return c;
    }
    int m = 1 << lg;
    a.resize(m);
    number_theoretic_transform(a);
    rep (i, m) a[i] *= a[i];
    inverse_number_theoretic_transform(a);
    a.resize(n);
    return a;
}

template<class T>
std::vector<T> convolution(std::vector<T> a, std::vector<T> b) {
    int n = a.size() + b.size() - 1;
    int lg = bitop::ceil_log2(n);
    int m = 1 << lg;
    if (n - (1 << (lg - 1)) <= 5) {
        --lg;
        if (a.size() < b.size()) std::swap(a, b);
        int m = n - (1 << lg);
        std::vector<T> a1(a.begin(), a.begin() + m), a2(a.begin() + m, a.end());
        std::vector<T> c(n);
        std::vector<T> c1 = convolution_naive(a1, b);
        std::vector<T> c2 = convolution(a2, b);
        rep (i, c1.size()) c[i] += c1[i];
        rep (i, c2.size()) c[i + m] += c2[i];
        return c;
    }
    a.resize(m);
    b.resize(m);
    number_theoretic_transform(a);
    number_theoretic_transform(b);
    rep (i, m) a[i] *= b[i];
    inverse_number_theoretic_transform(a);
    a.resize(n);
    return a;
}

} // namespace internal

using internal::inverse_number_theoretic_transform;
using internal::number_theoretic_transform;

template<class T>
std::vector<T>
convolution_for_any_mod(const std::vector<T>& a,
                        const std::vector<T>& b);

template<unsigned int p>
std::vector<static_modint<p>>
convolution(const std::vector<static_modint<p>>& a,
            const std::vector<static_modint<p>>& b) {
    unsigned int n = a.size(), m = b.size();
    if (n == 0 || m == 0) return {};
    if (n <= 60 || m <= 60) return internal::convolution_naive(a, b);
    if (n + m - 1 <= ((1 - p) & (p - 1))) {
        if (n == m && a == b) return internal::convolution_pow2(a);
        return internal::convolution(a, b);
    }
    return convolution_for_any_mod(a, b);
}

template<unsigned int p>
std::vector<ll> convolution(const std::vector<ll>& a,
                            const std::vector<ll>& b) {
    int n = a.size(), m = b.size();
    std::vector<static_modint<p>> a2(n), b2(m);
    rep (i, n) a2[i] = a[i];
    rep (i, m) b2[i] = b[i];
    auto c2 = convolution(a2, b2);
    std::vector<ll> c(c2.size());
    rep (i, c2.size()) c[i] = c2[i].get();
    return c;
}

template<class T>
std::vector<T>
convolution_for_any_mod(const std::vector<T>& a,
                        const std::vector<T>& b) {
    int n = a.size(), m = b.size();
    assert(n + m - 1 <= (1 << 26));
    if (n == 0 || m == 0) return {};
    std::vector<ll> a2(n), b2(m);
    rep (i, n) a2[i] = a[i].get();
    rep (i, m) b2[i] = b[i].get();
    static constexpr ll MOD1 = 469762049;
    static constexpr ll MOD2 = 1811939329;
    static constexpr ll MOD3 = 2013265921;
    static constexpr ll INV1_2 = mod_pow(MOD1, MOD2 - 2, MOD2);
    static constexpr ll INV1_3 = mod_pow(MOD1, MOD3 - 2, MOD3);
    static constexpr ll INV2_3 = mod_pow(MOD2, MOD3 - 2, MOD3);
    auto c1 = convolution<MOD1>(a2, b2);
    auto c2 = convolution<MOD2>(a2, b2);
    auto c3 = convolution<MOD3>(a2, b2);
    std::vector<T> res(n + m - 1);
    rep (i, n + m - 1) {
        ll t1 = c1[i];
        ll t2 = (c2[i] - t1 + MOD2) * INV1_2 % MOD2;
        if (t2 < 0) t2 += MOD2;
        ll t3 =
            ((c3[i] - t1 + MOD3) * INV1_3 % MOD3 - t2 + MOD3) * INV2_3 % MOD3;
        if (t3 < 0) t3 += MOD3;
        res[i] = (t1 + T(t2 + t3 * MOD2) * MOD1);
    }
    return res;
}

template<int id>
std::vector<dynamic_modint<id>>
convolution(const std::vector<dynamic_modint<id>>& a,
            const std::vector<dynamic_modint<id>>& b) {
    return convolution_for_any_mod(a, b);
}

std::vector<ll> convolution_ll(const std::vector<ll>& a, const std::vector<ll>& b) {
    int n = a.size(), m = b.size();
    assert(n + m - 1 <= (1 << 26));
    if (n == 0 || m == 0) return {};
    static constexpr ll MOD1 = 469762049;
    static constexpr ll MOD2 = 1811939329;
    static constexpr ll MOD3 = 2013265921;
    static constexpr ll INV1_2 = mod_pow(MOD1, MOD2 - 2, MOD2);
    static constexpr ll INV1_3 = mod_pow(MOD1, MOD3 - 2, MOD3);
    static constexpr ll INV2_3 = mod_pow(MOD2, MOD3 - 2, MOD3);
    auto c1 = convolution<MOD1>(a, b);
    auto c2 = convolution<MOD2>(a, b);
    auto c3 = convolution<MOD3>(a, b);
    std::vector<ll> res(n + m - 1);
    rep (i, n + m - 1) {
        ll t1 = c1[i];
        ll t2 = (c2[i] - t1 + MOD2) * INV1_2 % MOD2;
        if (t2 < 0) t2 += MOD2;
        ll t3 =
            ((c3[i] - t1 + MOD3) * INV1_3 % MOD3 - t2 + MOD3) * INV2_3 % MOD3;
        if (t3 < 0) t3 += MOD3;
        res[i] = t1 + (t2 + t3 * MOD2) * MOD1;
    }
    return res;
}

template<class T> void ntt_doubling_(std::vector<T>& a, std::vector<T> b) {
    static constexpr internal::NthRoot<T> nth_root;
    int n = a.size();
    const T z = nth_root.get(bitop::msb(n) + 1);
    T r = 1;
    rep (i, n) {
        b[i] *= r;
        r *= z;
    }
    number_theoretic_transform(b);
    a.reserve(2 * n);
    a.insert(a.end(), all(b));
}

template<class T> void ntt_doubling_(std::vector<T>& a) {
    static constexpr internal::NthRoot<T> nth_root;
    int n = a.size();
    auto b = a;
    inverse_number_theoretic_transform(b);
    const T z = nth_root.get(bitop::msb(n) + 1);
    T r = 1;
    rep (i, n) {
        b[i] *= r;
        r *= z;
    }
    number_theoretic_transform(b);
    a.reserve(2 * n);
    a.insert(a.end(), all(b));
}

template<unsigned int p> 
using is_ntt_friendly = std::integral_constant<bool, (1 << 23) <= ((1 - p) & (p - 1))>;

template<class T>
struct is_ntt_friendly_modint : std::false_type {};

template<unsigned int p>
struct is_ntt_friendly_modint<static_modint<p>> : is_ntt_friendly<p> {};

/**
 * @brief Convolution(畳み込み)
 * @docs docs/math/convolution/Convolution.md
 */
#line 6 "library/math/poly/FormalPowerSeries.hpp"

template<class T> class FormalPowerSeries : public std::vector<T> {
private:
    using Base = std::vector<T>;
    using Comb = Combinatorics<T>;

public:
    using Base::Base;
    FormalPowerSeries(const Base& v) : Base(v) {}
    FormalPowerSeries(Base&& v) : Base(std::move(v)) {}

    FormalPowerSeries& shrink() {
        while (!this->empty() && this->back() == T{0}) this->pop_back();
        return *this;
    }

    T eval(T x) const {
        T res = 0;
        rrep (i, this->size()) {
            res *= x;
            res += (*this)[i];
        }
        return res;
    }

    FormalPowerSeries prefix(int deg) const {
        assert(0 <= deg);
        if (deg < (int)this->size()) {
            return FormalPowerSeries(this->begin(), this->begin() + deg);
        }
        FormalPowerSeries res(*this);
        res.resize(deg);
        return res;
    }

    FormalPowerSeries operator+() const { return *this; }
    FormalPowerSeries operator-() const {
        FormalPowerSeries res(this->size());
        rep (i, this->size()) res[i] = -(*this)[i];
        return res;
    }
    FormalPowerSeries& operator<<=(int n) {
        this->insert(this->begin(), n, T{0});
        return *this;
    }
    FormalPowerSeries& operator>>=(int n) {
        this->erase(this->begin(),
                    this->begin() + std::min(n, (int)this->size()));
        return *this;
    }
    friend FormalPowerSeries operator<<(const FormalPowerSeries& lhs, int rhs) {
        return FormalPowerSeries(lhs) <<= rhs;
    }
    friend FormalPowerSeries operator>>(const FormalPowerSeries& lhs, int rhs) {
        return FormalPowerSeries(lhs) >>= rhs;
    }
    FormalPowerSeries& operator+=(const FormalPowerSeries& rhs) {
        if (this->size() < rhs.size()) this->resize(rhs.size());
        rep (i, rhs.size()) (*this)[i] += rhs[i];
        return *this;
    }
    FormalPowerSeries& operator-=(const FormalPowerSeries& rhs) {
        if (this->size() < rhs.size()) this->resize(rhs.size());
        rep (i, rhs.size()) (*this)[i] -= rhs[i];
        return *this;
    }
    friend FormalPowerSeries operator+(const FormalPowerSeries& lhs,
                                       const FormalPowerSeries& rhs) {
        return FormalPowerSeries(lhs) += rhs;
    }
    friend FormalPowerSeries operator-(const FormalPowerSeries& lhs,
                                       const FormalPowerSeries& rhs) {
        return FormalPowerSeries(lhs) -= rhs;
    }
    friend FormalPowerSeries operator*(const FormalPowerSeries& lhs,
                                       const FormalPowerSeries& rhs) {
        return FormalPowerSeries(convolution(lhs, rhs));
    }
    FormalPowerSeries& operator*=(const FormalPowerSeries& rhs) {
        return *this = *this * rhs;
    }
    FormalPowerSeries& operator*=(const T& rhs) {
        rep (i, this->size()) (*this)[i] *= rhs;
        return *this;
    }
    friend FormalPowerSeries operator*(const FormalPowerSeries& lhs,
                                       const T& rhs) {
        return FormalPowerSeries(lhs) *= rhs;
    }
    friend FormalPowerSeries operator*(const T& lhs,
                                       const FormalPowerSeries& rhs) {
        return FormalPowerSeries(rhs) *= lhs;
    }
    FormalPowerSeries& operator/=(const T& rhs) {
        const T irhs = 1 / rhs;
        rep (i, this->size()) (*this)[i] *= irhs;
        return *this;
    }
    friend FormalPowerSeries operator/(const FormalPowerSeries& lhs,
                                       const T& rhs) {
        return FormalPowerSeries(lhs) /= rhs;
    }

    FormalPowerSeries rev() const {
        FormalPowerSeries res(*this);
        std::reverse(all(res));
        return res;
    }

    friend FormalPowerSeries div(FormalPowerSeries lhs, FormalPowerSeries rhs) {
        lhs.shrink();
        rhs.shrink();
        if (lhs.size() < rhs.size()) {
            return FormalPowerSeries{};
        }
        int n = lhs.size() - rhs.size() + 1;
        if (rhs.size() <= 32) {
            FormalPowerSeries res(n);
            T iv = rhs.back().inv();
            rrep (i, n) {
                T d = lhs[i + rhs.size() - 1] * iv;
                res[i] = d;
                rep (j, rhs.size()) lhs[i + j] -= d * rhs[j];
            }
            return res;
        }
        return (lhs.rev().prefix(n) * rhs.rev().inv(n)).prefix(n).rev();
    }
    friend FormalPowerSeries operator%(FormalPowerSeries lhs,
                                       FormalPowerSeries rhs) {
        lhs.shrink();
        rhs.shrink();
        if (lhs.size() < rhs.size()) {
            return lhs;
        }
        int n = lhs.size() - rhs.size() + 1;
        if (rhs.size() <= 32) {
            T iv = rhs.back().inv();
            rrep (i, n) {
                T d = lhs[i + rhs.size() - 1] * iv;
                rep (j, rhs.size()) lhs[i + j] -= d * rhs[j];
            }
            return lhs.shrink();
        }
        return (lhs - div(lhs, rhs) * rhs).shrink();
    }
    friend std::pair<FormalPowerSeries, FormalPowerSeries>
    divmod(FormalPowerSeries lhs, FormalPowerSeries rhs) {
        lhs.shrink();
        rhs.shrink();
        if (lhs.size() < rhs.size()) {
            return {FormalPowerSeries{}, lhs};
        }
        int n = lhs.size() - rhs.size() + 1;
        if (rhs.size() <= 32) {
            FormalPowerSeries res(n);
            T iv = rhs.back().inv();
            rrep (i, n) {
                T d = lhs[i + rhs.size() - 1] * iv;
                res[i] = d;
                rep (j, rhs.size()) lhs[i + j] -= d * rhs[j];
            }
            return {res, lhs.shrink()};
        }
        FormalPowerSeries q = div(lhs, rhs);
        return {q, (lhs - q * rhs).shrink()};
    }
    FormalPowerSeries& operator%=(const FormalPowerSeries& rhs) {
        return *this = *this % rhs;
    }

    FormalPowerSeries diff() const {
        if (this->empty()) return {};
        FormalPowerSeries res(this->size() - 1);
        rep (i, res.size()) res[i] = (*this)[i + 1] * (i + 1);
        return res;
    }
    FormalPowerSeries integral() const {
        FormalPowerSeries res(this->size() + 1);
        res[0] = 0;
        Comb::init(this->size());
        rep (i, this->size()) res[i + 1] = (*this)[i] * Comb::inv(i + 1);
        return res;
    }

    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && is_ntt_friendly_modint<T>::value>::type* =
                 nullptr>
    FormalPowerSeries inv(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] != 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries f(1, (*this)[0].inv());
        for (int m = 1; m < deg; m <<= 1) {
            FormalPowerSeries t = this->prefix(2 * m);
            f.resize(2 * m);
            FormalPowerSeries dft_f = f;
            number_theoretic_transform(t);
            number_theoretic_transform(dft_f);
            rep (i, 2 * m) t[i] *= dft_f[i];
            inverse_number_theoretic_transform(t);
            std::fill(t.begin(), t.begin() + m, T{0});
            number_theoretic_transform(t);
            rep (i, 2 * m) dft_f[i] *= t[i];
            inverse_number_theoretic_transform(dft_f);
            rep (i, m, 2 * m) f[i] = -dft_f[i];
        }
        return f.prefix(deg);
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && !is_ntt_friendly_modint<T>::value>::type* =
                 nullptr>
    FormalPowerSeries inv(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] != 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries res(1, (*this)[0].inv());
        for (int m = 1; m < deg; m <<= 1) {
            res = res * 2 - (res * res * this->prefix(2 * m)).prefix(2 * m);
        }
        return res.prefix(deg);
    }
    FormalPowerSeries log(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] == 1);
        if (deg == -1) deg = this->size();
        return (diff().prefix(deg - 1) * inv(deg - 1))
            .prefix(deg - 1)
            .integral();
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && is_ntt_friendly_modint<T>::value>::type* =
                 nullptr>
    FormalPowerSeries exp(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] == 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries df = this->diff();
        FormalPowerSeries f(1, 1);
        FormalPowerSeries g(1, 1);
        FormalPowerSeries dft_f = f;
        number_theoretic_transform(dft_f);
        for (int m = 1; m < deg; m <<= 1) {
            dft_f.ntt_doubling(f);
            f.resize(2 * m);
            g.resize(2 * m);
            FormalPowerSeries dft_g = g;
            number_theoretic_transform(dft_g);
            FormalPowerSeries t = df.prefix(2 * m);
            number_theoretic_transform(t);
            rep (i, 2 * m) t[i] *= dft_f[i];
            inverse_number_theoretic_transform(t);
            std::fill(t.begin(), t.begin() + m - 1, T{0});
            number_theoretic_transform(t);
            rep (i, 2 * m) t[i] *= dft_g[i];
            inverse_number_theoretic_transform(t);
            std::fill(t.begin(), t.begin() + m - 1, T{0});
            t = t.prefix(2 * m - 1).integral();
            number_theoretic_transform(t);
            rep (i, 2 * m) t[i] *= dft_f[i];
            inverse_number_theoretic_transform(t);
            rep (i, m, 2 * m) f[i] = t[i];
            if (2 * m < deg) {
                dft_f = f;
                number_theoretic_transform(dft_f);
                FormalPowerSeries t = dft_f;
                rep (i, 2 * m) t[i] *= dft_g[i];
                inverse_number_theoretic_transform(t);
                std::fill(t.begin(), t.begin() + m, T{0});
                number_theoretic_transform(t);
                rep (i, 2 * m) t[i] *= dft_g[i];
                inverse_number_theoretic_transform(t);
                rep (i, m, 2 * m) g[i] = -t[i];
            }
        }
        return f.prefix(deg);
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && !is_ntt_friendly_modint<T>::value>::type* =
                 nullptr>
    FormalPowerSeries exp(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] == 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries res(1, 1);
        for (int m = 1; m < deg; m <<= 1) {
            res = (res * (prefix(2 * m) - res.log(2 * m)) + res).prefix(2 * m);
        }
        return res.prefix(deg);
    }
    FormalPowerSeries pow(ll k, int deg = -1) const {
        if (deg == -1) deg = this->size();
        if (deg == 0) return {};
        if (k == 0) {
            FormalPowerSeries res(deg);
            res[0] = 1;
            return res;
        }
        if (k == 1) return prefix(deg);
        if (k == 2) return (*this * *this).prefix(deg);
        T a;
        int d = -1;
        rep (i, this->size()) {
            if ((*this)[i] != 0) {
                a = (*this)[i];
                d = i;
                break;
            }
        }
        if (d == -1) {
            FormalPowerSeries res(deg);
            return res;
        }
        if ((i128)d * k >= deg) {
            FormalPowerSeries res(deg);
            return res;
        }
        deg -= d * k;
        FormalPowerSeries res = (((*this >> d) / a).log(deg) * k).exp(deg);
        res *= a.pow(k);
        res <<= d * k;
        return res;
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && is_ntt_friendly_modint<T>::value>::type* =
                 nullptr>
    FormalPowerSeries& ntt_doubling() {
        ntt_doubling_(*this);
        return *this;
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && is_ntt_friendly_modint<T>::value>::type* =
                 nullptr>
    FormalPowerSeries& ntt_doubling(const std::vector<T>& b) {
        ntt_doubling_(*this, b);
        return *this;
    }
};

/**
 * @brief FormalPowerSeries(形式的冪級数)
 * @docs docs/math/poly/FormalPowerSeries.md
 * @see https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
 */
#line 7 "library/math/sps/Composite.hpp"

template<class T, class Comb = Combinatorics<T>>
std::vector<T> fps_composite_of_sps(FormalPowerSeries<T> f, std::vector<T> s) {
    int n = s.size(), m = bitop::ceil_log2(n);
    assert(s[0] == 0);
    assert(n == (1 << m));
    f.resize(m + 1);
    Comb::init(m + 1);
    rep (i, m + 1) f[i] *= Comb::fact(i);
    std::vector<T> a(1 << m, f[m]);
    rrep (i, m) {
        rrep (j, m - i) {
            std::vector<T> b(1 << j), c(1 << j);
            rep (k, 1 << j) b[k] = a[k];
            rep (k, 1 << j) c[k] = s[k + (1 << j)];
            std::vector<T> d = subset_convolution(b, c);
            rep (k, 1 << j) a[k + (1 << j)] = d[k];
        }
        a[0] = f[i];
    }
    return a;
}

template<class T, class Comb = Combinatorics<T>>
std::vector<T> polynomial_composite_of_sps(FormalPowerSeries<T> f, std::vector<T> s) {
    Comb::init(f.size());
    int n = s.size(), m = bitop::ceil_log2(n);
    FormalPowerSeries<T> g(m + 1);
    T c = s[0];
    rep (i, m + 1) {
        T d = 1;
        rep (j, i, f.size()) {
            g[i] += f[j] * Comb::comb(j, i) * d;
            d *= c;
        }
    }
    s[0] = 0;
    return fps_composite_of_sps(g, s);
}

template<class T, class Comb = Combinatorics<T>>
std::vector<T> power_projection_of_sps(std::vector<T> s, std::vector<T> w, int M) {
    int n = s.size(), m = bitop::ceil_log2(n);
    assert(n == (1 << m));
    assert(s.size() == w.size());
    T c = s[0];
    s[0] = 0;
    FormalPowerSeries<T> f(m + 1);
    rep (i, m) {
        f[i] = w[0];
        FormalPowerSeries<T> a(1 << (m - 1 - i));
        rep (j, m - i) {
            std::vector<T> c(1 << j), d(1 << j);
            rep (k, 1 << j) c[k] = s[k + (1 << j)];
            rep (k, 1 << j) d[k] = w[(2 << j) - 1 - k];
            std::vector<T> b = subset_convolution(d, c);
            rep (k, 1 << j) a[k] += b[(1 << j) - 1 - k];
        }
        w = a;
    }
    f[m] = w[0];
    Comb::init(M);
    FormalPowerSeries<T> g(M);
    rep (i, m + 1) {
        T d = 1;
        rep (j, i, M) {
            g[j] += f[i] * Comb::perm(j, i) * d;
            d *= c;
        }
    }
    return g;
}

template<class T>
std::vector<T> exp_of_sps(std::vector<T> s) {
    int n = s.size(), m = bitop::ceil_log2(n);
    assert(s[0] == 0);
    assert(n == (1 << m));
    std::vector<T> a(1 << m, 1);
    rrep (i, m) {
        int j = m - i - 1;
        std::vector<T> b(1 << j), c(1 << j);
        rep (k, 1 << j) b[k] = a[k];
        rep (k, 1 << j) c[k] = s[k + (1 << j)];
        std::vector<T> d = subset_convolution(b, c);
        rep (k, 1 << j) a[k + (1 << j)] = d[k];
    }
    return a;
}

template<class T, int L = 0>
std::vector<T> div_of_sps(const std::vector<T>& a, const std::vector<T>& b) {
    if ((int)a.size() > (1 << L)) {
        return div_of_sps<T, std::min<int>(L + 1, 30)>(a, b);
    }
    int n = a.size();
    assert(b[0] == 1);
    auto f = internal::ranked_zeta<T, L + 1>(a);
    auto g = internal::ranked_zeta<T, L + 1>(b);
    rep (i, n) {
        rep (j, L + 1) {
            rep (k, 1, L + 1 - j) f[i][j + k] -= f[i][j] * g[i][k];
        }
    }
    return internal::ranked_moebius<T, L + 1>(f);
}

template<class T>
std::vector<T> log_of_sps(std::vector<T> s) {
    int n = s.size(), m = bitop::ceil_log2(n);
    assert(s[0] == 1);
    assert(n == (1 << m));
    std::vector<T> a(n);
    rep (i, m) {
        std::vector<T> b(1 << i);
        std::vector<T> c(1 << i);
        rep (j, 1 << i) b[j] = s[j + (1 << i)];
        rep (j, 1 << i) c[j] = s[j];
        std::vector<T> d = div_of_sps(b, c);
        rep (j, 1 << i) a[j + (1 << i)] = d[j];
    }
    return a;
}
#line 5 "library/test/yosupo/set_power_series/log_of_set_power_series.test.cpp"
using namespace std;
using mint = modint998244353;
int main() {
    int N; scan >> N;
    vector<mint> a(1 << N); scan >> a;
    prints(log_of_sps(a));
}
