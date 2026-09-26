#line 1 "library/test/yosupo/new/two_square_sum.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/two_square_sum"
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
#define RREP3(i, a, b) for (ll i = (ll)(a)-1; i >= (ll)(b); --i)
#define RREP4(i, a, b, c) for (ll i = (ll)(a)-1; i >= (ll)(b); i -= (ll)(c))
#define rrep(...) OVERLOAD5(__VA_ARGS__, RREP4, RREP3, RREP2)(__VA_ARGS__)
#define REPS2(i, b) for (ll i = 1; i <= (ll)(b); ++i)
#define REPS3(i, a, b) for (ll i = (ll)(a) + 1; i <= (ll)(b); ++i)
#define REPS4(i, a, b, c) for (ll i = (ll)(a) + 1; i <= (ll)(b); i += (ll)(c))
#define reps(...) OVERLOAD5(__VA_ARGS__, REPS4, REPS3, REPS2)(__VA_ARGS__)
#define RREPS2(i, a) for (ll i = (ll)(a); i > 0; --i)
#define RREPS3(i, a, b) for (ll i = (ll)(a); i > (ll)(b); --i)
#define RREPS4(i, a, b, c) for (ll i = (ll)(a); i > (ll)(b); i -= (ll)(c))
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
    static_assert(size <= 128, "size must be less than or equal to 128");

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
    static_assert(size <= 128, "size must be less than or equal to 128");

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
    std::array<char, IO_BUFFER_SIZE + 1> buffer;
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

Printer<> print(1), eprint(2);

void prints() { print.print_char('\n'); }

template<class T> auto prints(const T& v) -> decltype(print << v, (void)0) {
    print << v;
    print.print_char('\n');
}

template<class Head, class... Tail>
auto prints(const Head& head, const Tail&... tail)
    -> decltype(print << head, (void)0) {
    print << head;
    print.print_char(' ');
    prints(tail...);
}

Printer<IO_BUFFER_SIZE, true> debug(1), edebug(2);

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
#line 2 "library/math/TwoSquareSum.hpp"

#line 2 "library/math/PollardRho.hpp"

#line 2 "library/random/Random.hpp"

#line 4 "library/random/Random.hpp"

template<class Engine> class Random {
private:
    Engine rnd;

public:
    using result_type = typename Engine::result_type;
    Random() : Random(std::random_device{}()) {}
    Random(result_type seed) : rnd(seed) {}
    result_type operator()() { return rnd(); }
    result_type min() const { return rnd.min(); }
    result_type max() const { return rnd.max(); }
    template<class IntType = ll> IntType uniform(IntType l, IntType r) {
        static_assert(std::is_integral<IntType>::value,
                      "template argument must be an integral type");
        assert(l <= r);
        return std::uniform_int_distribution<IntType>{l, r}(rnd);
    }
    template<class RealType = double>
    RealType uniform_real(RealType l, RealType r) {
        static_assert(std::is_floating_point<RealType>::value,
                      "template argument must be an floating point type");
        assert(l <= r);
        return std::uniform_real_distribution<RealType>{l, r}(rnd);
    }
    bool uniform_bool() { return uniform<int>(0, 1) == 1; }
    template<class T = ll> std::pair<T, T> uniform_pair(T l, T r) {
        assert(l < r);
        T a, b;
        do {
            a = uniform<T>(l, r);
            b = uniform<T>(l, r);
        } while (a == b);
        if (a > b) swap(a, b);
        return {a, b};
    }
    template<class T = ll> std::vector<T> choice(int n, T l, T r) {
        assert(l <= r);
        assert(T(n) <= (r - l + 1));
        std::set<T> res;
        while ((int)res.size() < n) res.insert(uniform<T>(l, r));
        return {res.begin(), res.end()};
    }
    template<class Iter> void shuffle(const Iter& first, const Iter& last) {
        std::shuffle(first, last, rnd);
    }
    template<class T> std::vector<T> permutation(T n) {
        std::vector<T> res(n);
        rep (i, n) res[i] = i;
        shuffle(all(res));
        return res;
    }
    template<class T = ll>
    std::vector<T> choice_shuffle(int n, T l, T r, bool sorted = true) {
        assert(l <= r);
        assert(T(n) <= (r - l + 1));
        std::vector<T> res(r - l + 1);
        rep (i, l, r + 1) res[i - l] = i;
        shuffle(all(res));
        res.erase(res.begin() + n, res.end());
        if (sorted) sort(all(res));
        return res;
    }
};

using Random32 = Random<std::mt19937>;
Random32 rand32;
using Random64 = Random<std::mt19937_64>;
Random64 rand64;

/**
 * @brief Random
 * @docs docs/random/Random.md
 */
#line 2 "library/math/MontgomeryModInt.hpp"

#line 4 "library/math/MontgomeryModInt.hpp"

template<class T> class MontgomeryReduction {
    static_assert(std::is_integral<T>::value, "T must be integral");
    static_assert(std::is_unsigned<T>::value, "T must be unsigned");

private:
    using large_t = typename double_size_uint<T>::type;
    static constexpr int lg = std::numeric_limits<T>::digits;
    T mod;
    T r;
    T r2; // r^2 mod m
    T calc_minv() {
        T t = 0, res = 0;
        rep (i, lg) {
            if (~t & 1) {
                t += mod;
                res += static_cast<T>(1) << i;
            }
            t >>= 1;
        }
        return res;
    }
    T minv;

public:
    MontgomeryReduction(T v) { set_mod(v); }
    static constexpr int get_lg() { return lg; }
    void set_mod(T v) {
        assert(v > 0);
        assert(v & 1);
        assert(v <= std::numeric_limits<T>::max() / 2);
        mod = v;
        r = (-static_cast<T>(mod)) % mod;
        r2 = (-static_cast<large_t>(mod)) % mod;
        minv = calc_minv();
    }
    inline T get_mod() const { return mod; }
    inline T get_r() const { return r; }
    T reduce(large_t x) const {
        large_t tmp =
            (x + static_cast<large_t>(static_cast<T>(x) * minv) * mod) >> lg;
        return tmp >= mod ? tmp - mod : tmp;
    }
    T transform(large_t x) const { return reduce(x * r2); }
};

template<class T, int id> class MontgomeryModInt {
private:
    using large_t = typename double_size_uint<T>::type;
    using signed_t = typename std::make_signed<T>::type;
    T val;

    static MontgomeryReduction<T> mont;

public:
    MontgomeryModInt() : val(0) {}
    template<class U, typename std::enable_if<
                          std::is_integral<U>::value &&
                          std::is_unsigned<U>::value>::type* = nullptr>
    MontgomeryModInt(U x)
        : val(mont.transform(
              x < (static_cast<large_t>(mont.get_mod()) << mont.get_lg())
                  ? x
                  : x % mont.get_mod())) {}
    template<class U,
             typename std::enable_if<std::is_integral<U>::value &&
                                     std::is_signed<U>::value>::type* = nullptr>
    MontgomeryModInt(U x)
        : MontgomeryModInt(static_cast<typename std::make_unsigned<U>::type>(
              x < 0 ? -x : x)) {
        if (x < 0 && val) val = mont.get_mod() - val;
    }

    T get() const { return mont.reduce(val); }
    static T get_mod() { return mont.get_mod(); }

    static void set_mod(T v) { mont.set_mod(v); }

    MontgomeryModInt operator+() const { return *this; }
    MontgomeryModInt operator-() const {
        MontgomeryModInt res;
        if (val) res.val = mont.get_mod() - val;
        return res;
    }
    MontgomeryModInt& operator++() {
        val += mont.get_r();
        if (val >= mont.get_mod()) val -= mont.get_mod();
        return *this;
    }
    MontgomeryModInt& operator--() {
        if (val < mont.get_r()) val += mont.get_mod();
        val -= mont.get_r();
        return *this;
    }
    MontgomeryModInt operator++(int) {
        MontgomeryModInt res = *this;
        ++*this;
        return res;
    }
    MontgomeryModInt operator--(int) {
        MontgomeryModInt res = *this;
        --*this;
        return res;
    }

    MontgomeryModInt& operator+=(const MontgomeryModInt& rhs) {
        val += rhs.val;
        if (val >= mont.get_mod()) val -= mont.get_mod();
        return *this;
    }
    MontgomeryModInt& operator-=(const MontgomeryModInt& rhs) {
        if (val < rhs.val) val += mont.get_mod();
        val -= rhs.val;
        return *this;
    }
    MontgomeryModInt& operator*=(const MontgomeryModInt& rhs) {
        val = mont.reduce(static_cast<large_t>(val) * rhs.val);
        return *this;
    }

    MontgomeryModInt pow(ull n) const {
        MontgomeryModInt res = 1, x = *this;
        while (n) {
            if (n & 1) res *= x;
            x *= x;
            n >>= 1;
        }
        return res;
    }
    MontgomeryModInt inv() const { return pow(mont.get_mod() - 2); }

    MontgomeryModInt& operator/=(const MontgomeryModInt& rhs) {
        return *this *= rhs.inv();
    }

    friend MontgomeryModInt operator+(const MontgomeryModInt& lhs,
                                      const MontgomeryModInt& rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }
    friend MontgomeryModInt operator-(const MontgomeryModInt& lhs,
                                      const MontgomeryModInt& rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }
    friend MontgomeryModInt operator*(const MontgomeryModInt& lhs,
                                      const MontgomeryModInt& rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }
    friend MontgomeryModInt operator/(const MontgomeryModInt& lhs,
                                      const MontgomeryModInt& rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    friend bool operator==(const MontgomeryModInt& lhs,
                           const MontgomeryModInt& rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const MontgomeryModInt& lhs,
                           const MontgomeryModInt& rhs) {
        return lhs.val != rhs.val;
    }

    template<class Pr> void print(Pr& a) const { a.print(mont.reduce(val)); }
    template<class Pr> void debug(Pr& a) const { a.print(mont.reduce(val)); }
    template<class Sc> void scan(Sc& a) {
        ll v;
        a.scan(v);
        *this = v;
    }
};

template<class T, int id>
MontgomeryReduction<T>
    MontgomeryModInt<T, id>::mont = MontgomeryReduction<T>(998244353);

using mmodint = MontgomeryModInt<unsigned int, -1>;

/**
 * @brief MontgomeryModInt(モンゴメリ乗算)
 * @docs docs/math/MontgomeryModInt.md
 */
#line 2 "library/math/MillerRabin.hpp"

#line 5 "library/math/MillerRabin.hpp"

constexpr ull base_miller_rabin_int[3] = {2, 7, 61};
constexpr ull base_miller_rabin_ll[7] = {2,      325,     9375,      28178,
                                         450775, 9780504, 1795265022};

template<class T> constexpr bool miller_rabin(ull n, const ull base[], int s) {
    if (T::get_mod() != n) T::set_mod(n);
    ull d = n - 1;
    while (~d & 1) d >>= 1;
    T e{1}, re{n - 1};
    rep (i, s) {
        ull a = base[i];
        if (a >= n) return true;
        ull t = d;
        T y = T(a).pow(t);
        while (t != n - 1 && y != e && y != re) {
            y *= y;
            t <<= 1;
        }
        if (y != re && !(t & 1)) return false;
    }
    return true;
}

constexpr bool is_prime_mr(ll n) {
    if (n == 2) return true;
    if (n < 2 || n % 2 == 0) return false;
    if (n < (1u << 31))
        return miller_rabin<MontgomeryModInt<unsigned int, -2>>(
            n, base_miller_rabin_int, 3);
    return miller_rabin<MontgomeryModInt<ull, -2>>(n, base_miller_rabin_ll, 7);
}

#if __cpp_variable_templates >= 201304L && __cpp_constexpr >= 201304L
template<ull n> constexpr bool is_prime_v = is_prime_mr(n);
#endif

/**
 * @brief MillerRabin(ミラーラビン素数判定)
 * @docs docs/math/MillerRabin.md
 */
#line 2 "library/string/RunLength.hpp"

#line 4 "library/string/RunLength.hpp"

template<class Cont, class Comp>
std::vector<std::pair<typename Cont::value_type, int>>
RunLength(const Cont& str, const Comp& cmp) {
    std::vector<std::pair<typename Cont::value_type, int>> res;
    if (str.size() == 0) return res;
    res.emplace_back(str[0], 1);
    rep (i, 1, str.size()) {
        if (cmp(res.back().first, str[i])) ++res.back().second;
        else res.emplace_back(str[i], 1);
    }
    return res;
}

template<class Cont>
std::vector<std::pair<typename Cont::value_type, int>>
RunLength(const Cont& str) {
    return RunLength(str, std::equal_to<typename Cont::value_type>());
}

/**
 * @brief RunLength(ランレングス圧縮)
 * @docs docs/string/RunLength.md
 */
#line 8 "library/math/PollardRho.hpp"

template<class T, class Rnd> ull pollard_rho(ull n, Rnd& rnd) {
    if (~n & 1) return 2;
    if (T::get_mod() != n) T::set_mod(n);
    T c, one = 1;
    auto f = [&](T x) -> T { return x * x + c; };
    constexpr int M = 128;
    while (1) {
        c = rnd.uniform(1ull, n - 1);
        T x = rnd.uniform(2ull, n - 1), y = x;
        ull g = 1;
        while (g == 1) {
            T p = one, tx = x, ty = y;
            rep (M) {
                x = f(x);
                y = f(f(y));
                p *= x - y;
            }
            g = gcd(p.get(), n);
            if (g == 1) continue;
            rep (M) {
                tx = f(tx);
                ty = f(f(ty));
                g = gcd((tx - ty).get(), n);
                if (g != 1) {
                    if (g != n) return g;
                    break;
                }
            }
        }
    }
    return -1;
}

template<class T = MontgomeryModInt<ull, -3>, class Rnd = Random64>
std::vector<ull> factorize(ull n, Rnd& rnd = rand64) {
    if (n == 1) return {};
    std::vector<ull> res;
    std::vector<ull> st = {n};
    while (!st.empty()) {
        ull t = st.back();
        st.pop_back();
        if (t == 1) continue;
        if (is_prime_mr(t)) {
            res.push_back(t);
            continue;
        }
        ull f = pollard_rho<T>(t, rnd);
        st.push_back(f);
        st.push_back(t / f);
    }
    std::sort(all(res));
    return res;
}

template<class T = MontgomeryModInt<ull, -3>, class Rnd = Random64>
std::vector<std::pair<ull, int>> expfactorize(ull n, Rnd& rnd = rand64) {
    auto f = factorize<T, Rnd>(n, rnd);
    return RunLength(f);
}

std::vector<ll> divisors_pr(ll n) {
    std::vector<ll> res;
    auto r = expfactorize(n);
    int m = r.size();
    rec_lambda([&](auto&& self, int k, ll d) -> void {
        if (k == m) {
            res.push_back(d);
            return;
        }
        ll t = d;
        rep (r[k].second) {
            self(k + 1, d);
            d *= r[k].first;
        }
        self(k + 1, d);
        d = t;
    })(0, 1);
    std::sort(all(res));
    return res;
}

/**
 * @brief PollardRho(素因数分解)
 * @docs docs/math/PollardRho.md
 */
#line 2 "library/math/GaussianInteger.hpp"

#line 4 "library/math/GaussianInteger.hpp"

template<class T> class GaussianInteger {
public:
    T real, imag;

    GaussianInteger() : real(0), imag(0) {}
    GaussianInteger(T real) : real(real), imag(0) {}
    GaussianInteger(T real, T imag) : real(real), imag(imag) {}

    GaussianInteger conj() const { return GaussianInteger(real, -imag); }
    T norm() const { return real * real + imag * imag; }
    GaussianInteger operator+() { return *this; }
    GaussianInteger operator-() const { return GaussianInteger(-real, -imag); }
    GaussianInteger operator+=(const GaussianInteger& rhs) {
        real += rhs.real;
        imag += rhs.imag;
        return *this;
    }
    GaussianInteger operator-=(const GaussianInteger& rhs) {
        real -= rhs.real;
        imag -= rhs.imag;
        return *this;
    }
    GaussianInteger operator*=(const GaussianInteger& rhs) {
        T tmp = real * rhs.real - imag * rhs.imag;
        imag = real * rhs.imag + imag * rhs.real;
        real = tmp;
        return *this;
    }
    friend GaussianInteger operator+(const GaussianInteger& lhs,
                                     const GaussianInteger& rhs) {
        return GaussianInteger(lhs) += rhs;
    }
    friend GaussianInteger operator-(const GaussianInteger& lhs,
                                     const GaussianInteger& rhs) {
        return GaussianInteger(lhs) -= rhs;
    }
    friend GaussianInteger operator*(const GaussianInteger& lhs,
                                     const GaussianInteger& rhs) {
        return GaussianInteger(lhs) *= rhs;
    }
    friend bool operator==(const GaussianInteger& lhs,
                           const GaussianInteger& rhs) {
        return lhs.real == rhs.real && lhs.imag == rhs.imag;
    }
    friend bool operator!=(const GaussianInteger& lhs,
                           const GaussianInteger& rhs) {
        return !(lhs == rhs);
    }
    template<class Pr> void print(Pr& pr) const {
        pr.print(real);
        pr.print(' ');
        pr.print(imag);
    }
    template<class Pr> void debug(Pr& pr) const {
        pr.print(real);
        pr.print_char('+');
        pr.print(imag);
        pr.print_char('i');
    }
    template<class Sc> void scan(Sc& sc) {
        sc.scan(real);
        sc.scan(imag);
    }
};

template<class T>
GaussianInteger<T> gcd(GaussianInteger<T> a, GaussianInteger<T> b) {
    while (b != GaussianInteger<T>()) {
        GaussianInteger<T> q = a * b.conj();
        T n = b.norm();
        {
            T tmp = q.real;
            q.real /= n;
            tmp = tmp - q.real * n;
            if (tmp * 2 > n) ++q.real;
            if (tmp * 2 < -n) --q.real;
        }
        {
            T tmp = q.imag;
            q.imag /= n;
            tmp = tmp - q.imag * n;
            if (tmp * 2 > n) ++q.imag;
            if (tmp * 2 < -n) --q.imag;
        }
        a -= b * q;
        std::swap(a, b);
    }
    return a;
}

/**
 * @brief GaussianInteger
 * @see https://maspypy.com/library-checker-gcd-of-gaussian-integers
 */
#line 6 "library/math/TwoSquareSum.hpp"

std::pair<ll, ll> two_square_sum_prime(ll p) {
    assert(p == 2 || p % 4 == 1);
    if (p == 2) return {1, 1};
    auto mod_pow_ll = [&](i128 x, ll e) -> i128 {
        i128 res = 1;
        while (e) {
            if (e & 1) (res *= x) %= p;
            (x *= x) %= p;
            e /= 2;
        }
        return res;
    };
    ll x = 1;
    while (1) {
        ++x;
        ll t = mod_pow_ll(x, (p - 1) / 4);
        if (i128{t} * t % p == p - 1) {
            x = t;
            break;
        }
    }
    GaussianInteger<i128> a(p, 0), b(x, 1);
    a = gcd(a, b);
    assert(a.norm() == p);
    return {a.real, a.imag};
}

template<class T> std::vector<std::pair<T, T>> two_square_sum(T a) {
    if (a < 0) return {};
    if (a == 0) return {{0, 0}};
    using G = GaussianInteger<T>;
    const auto& pfs = expfactorize(a);
    for (const auto& [p, e] : pfs) {
        if (p % 4 == 3 && e % 2 == 1) return {};
    }
    std::vector<G> res{{1, 0}};
    for (const auto& [p, e] : pfs) {
        if (p % 4 == 3) {
            const T x = my_pow(p, e / 2);
            for (auto& g : res) {
                g.real *= x;
                g.imag *= x;
            }
            continue;
        }
        auto [px, py] = two_square_sum_prime(p);
        G pg(px, py);
        std::vector<G> gpows(e + 1);
        gpows[0] = 1;
        rep (i, e) gpows[i + 1] = gpows[i] * pg;
        if (p == 2) {
            for (auto&& g : res) g *= gpows[e];
            continue;
        }
        std::vector<G> gs(e + 1);
        rep (i, e + 1) gs[i] = gpows[i] * gpows[e - i].conj();
        std::vector<G> nres;
        for (auto g : gs) for (auto i : res) nres.push_back(g * i);
        res = std::move(nres);
    }
    for (auto&& g : res) {
        while (g.real <= 0 || g.imag < 0) g = {-g.imag, g.real};
    }
    std::vector<std::pair<T, T>> ans(res.size());
    rep (i, res.size()) ans[i] = {res[i].real, res[i].imag};
    return ans;
}
#line 4 "library/test/yosupo/new/two_square_sum.test.cpp"
using namespace std;
int main() {
    int T; scan >> T;
    rep (T) {
        ll n; scan >> n;
        if (n == 0) {
            prints(1);
            prints(0, 0);
            continue;
        }
        auto res = two_square_sum(n);
        vector<pair<ll, ll>> ans;
        for (auto [a, b] : res) {
            rep (4) {
                swap(a, b);
                a = -a;
                if (a >= 0 && b >= 0) ans.emplace_back(a, b);
            }
        }
        prints(ans.size());
        for (auto p : ans) prints(p);
    }
}
