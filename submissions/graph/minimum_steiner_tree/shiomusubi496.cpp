#line 1 "library/test/yosupo/new/minimum_steiner_tree.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/minimum_steiner_tree"
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
#line 2 "library/graph/Graph.hpp"

#line 4 "library/graph/Graph.hpp"

template<class T = int> struct edge {
    int from, to;
    T cost;
    int idx;
    edge() : from(-1), to(-1) {}
    edge(int f, int t, const T& c = 1, int i = -1)
        : from(f), to(t), cost(c), idx(i) {}
    edge(int f, int t, T&& c, int i = -1)
        : from(f), to(t), cost(std::move(c)), idx(i) {}
    operator int() const { return to; }
    friend bool operator<(const edge<T>& lhs, const edge<T>& rhs) {
        return lhs.cost < rhs.cost;
    }
    friend bool operator>(const edge<T>& lhs, const edge<T>& rhs) {
        return lhs.cost > rhs.cost;
    }
};

template<class T = int> using Edges = std::vector<edge<T>>;
template<class T = int> using GMatrix = std::vector<std::vector<T>>;

template<class T = int> class Graph : public std::vector<std::vector<edge<T>>> {
private:
    using Base = std::vector<std::vector<edge<T>>>;

public:
    int edge_id = 0;
    using Base::Base;
    int edge_size() const { return edge_id; }
    int add_edge(int a, int b, const T& c, bool is_directed = false) {
        assert(0 <= a && a < (int)this->size());
        assert(0 <= b && b < (int)this->size());
        (*this)[a].emplace_back(a, b, c, edge_id);
        if (!is_directed) (*this)[b].emplace_back(b, a, c, edge_id);
        return edge_id++;
    }
    int add_edge(int a, int b, bool is_directed = false) {
        assert(0 <= a && a < (int)this->size());
        assert(0 <= b && b < (int)this->size());
        (*this)[a].emplace_back(a, b, 1, edge_id);
        if (!is_directed) (*this)[b].emplace_back(b, a, 1, edge_id);
        return edge_id++;
    }
};

template<class T> GMatrix<T> ListToMatrix(const Graph<T>& G) {
    const int N = G.size();
    auto res = make_vec<T>(N, N, infinity<T>::value);
    rep (i, N) res[i][i] = 0;
    rep (i, N) {
        for (const auto& e : G[i]) res[i][e.to] = e.cost;
    }
    return res;
}

template<class T> Edges<T> UndirectedListToEdges(const Graph<T>& G) {
    const int V = G.size();
    const int E = G.edge_size();
    Edges<T> Ed(E);
    rep (i, V) {
        for (const auto& e : G[i]) Ed[e.idx] = e;
    }
    return Ed;
}

template<class T> Edges<T> DirectedListToEdges(const Graph<T>& G) {
    const int V = G.size();
    const int E = std::accumulate(
        all(G), 0, [](int a, const std::vector<edge<T>>& v) -> int {
            return a + v.size();
        });
    Edges<T> Ed(G.edge_size());
    Ed.reserve(E);
    rep (i, V) {
        for (const auto& e : G[i]) {
            if (Ed[e.idx] == -1) Ed[e.idx] = e;
            else Ed.push_back(e);
        }
    }
    return Ed;
}

template<class T> Graph<T> ReverseGraph(const Graph<T>& G) {
    const int V = G.size();
    Graph<T> res(V);
    rep (i, V) {
        for (const auto& e : G[i]) {
            res[e.to].emplace_back(e.to, e.from, e.cost, e.idx);
        }
    }
    res.edge_id = G.edge_size();
    return res;
}


struct unweighted_edge {
    template<class... Args> unweighted_edge(const Args&...) {}
    operator int() { return 1; }
};

using UnweightedGraph = Graph<unweighted_edge>;

/**
 * @brief Graph-template
 * @docs docs/graph/Graph.md
 */
#line 2 "library/graph/mst/MinimumSteinerTree.hpp"

#line 2 "library/data-struct/unionfind/UnionFind.hpp"

#line 4 "library/data-struct/unionfind/UnionFind.hpp"

class UnionFind {
private:
    int n;
    std::vector<int> par;

public:
    UnionFind() : UnionFind(0) {}
    UnionFind(int n) : n(n), par(n, -1) {}
    int find(int x) {
        assert(0 <= x && x < n);
        return par[x] < 0 ? x : par[x] = find(par[x]);
    }
    std::pair<int, int> merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return {x, -1};
        if (par[x] > par[y]) std::swap(x, y);
        par[x] += par[y];
        par[y] = x;
        return {x, y};
    }
    bool same(int x, int y) { return find(x) == find(y); }
    int size(int x) { return -par[find(x)]; }
    std::vector<std::vector<int>> groups() {
        std::vector<std::vector<int>> res(n);
        rep (i, n) res[find(i)].push_back(i);
        res.erase(
            remove_if(all(res),
                      [](const std::vector<int>& v) { return v.empty(); }),
            res.end());
        return res;
    }
    bool is_root(int x) const {
        assert(0 <= x && x < n);
        return par[x] < 0;
    }
};

/**
 * @brief UnionFind
 * @docs docs/data-struct/unionfind/UnionFind.md
 */
#line 6 "library/graph/mst/MinimumSteinerTree.hpp"

template<class T> std::pair<T, Edges<T>> minimum_steiner_tree(Graph<T> G, std::vector<int> X) {
    int N = G.size(), K = X.size();
    std::vector<std::vector<T>> dp(1 << K, std::vector<T>(N, infinity<T>::value));
    std::vector<Edges<T>> prv1(1 << K, Edges<T>(N));
    std::vector<std::vector<int>> prv2(1 << K, std::vector<int>(N, -1));
    rep (i, N) {
        dp[0][i] = 0;
        prv2[0][i] = -2;
    }
    rep (i, K) {
        dp[1 << i][X[i]] = 0;
        prv2[1 << i][X[i]] = -2;
    }
    prique<std::pair<T, int>> que;
    rep (S, 1, 1 << K) {
        for (ll U = (S - 1) & S; U > (S ^ U); U = (U - 1) & S) {
            rep (i, N) {
                if (chmin(dp[S][i], dp[U][i] + dp[S ^ U][i])) {
                    prv2[S][i] = U;
                }
            }
        }
        rep (i, N) que.emplace(dp[S][i], i);
        while (!que.empty()) {
            T c = std::move(que.top().first);
            int v = que.top().second;
            que.pop();
            if (dp[S][v] != c) continue;
            for (const auto& e : G[v]) {
                if (chmin(dp[S][e.to], c + e.cost)) {
                    prv1[S][e.to] = e;
                    prv2[S][e.to] = -1;
                    que.emplace(dp[S][e.to], e.to);
                }
            }
        }
    }
    std::queue<std::pair<int, int>> que2;
    Edges<T> res;
    int v = std::min_element(all(dp.back())) - dp.back().begin();
    que2.emplace((1 << K) - 1, v);
    while (!que2.empty()) {
        auto [S, v] = que2.front();
        que2.pop();
        if (prv2[S][v] >= 0) {
            que2.emplace(prv2[S][v], v);
            que2.emplace(S ^ prv2[S][v], v);
        }
        else if (prv2[S][v] == -1) {
            res.push_back(prv1[S][v]);
            que2.emplace(S, prv1[S][v].from);
        }
    }
    return {*std::min_element(all(dp.back())), res};
}
#line 5 "library/test/yosupo/new/minimum_steiner_tree.test.cpp"
using namespace std;
int main() {
    int N, M; scan >> N >> M;
    Graph<ll> G(N);
    rep (M) {
        int a, b; scan >> a >> b;
        ll c; scan >> c;
        G.add_edge(a, b, c);
    }
    int K; scan >> K;
    vector<int> X(K); scan >> X;
    auto res = minimum_steiner_tree(G, X);
    vector<int> idx;
    rep (i, res.second.size()) idx.push_back(res.second[i].idx);
    prints(res.first, res.second.size());
    prints(idx);
}
