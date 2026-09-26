#line 1 "library/test/yosupo/math/sum_of_exponential_times_polynomial.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial"
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
#if __cplusplus >= 201402L
#define rall(v) std::rbegin(v), std::rend(v)
#else
#define rall(v) v.rbegin(), v.rend()
#endif

#if __cpp_constexpr >= 201304L
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

#if __cpp_if_constexpr >= 201606L
#define IF_CONSTEXPR constexpr
#else
#define IF_CONSTEXPR
#endif

#define IO_BUFFER_SIZE 2048
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

template<std::size_t buf_size = IO_BUFFER_SIZE> class Reader {
private:
    int fd, idx, sz;
    bool state;
    std::array<char, buf_size> buffer;
    inline void read_buf() {
        sz = read(fd, buffer.begin(), buf_size);
        idx = 0;
        if (sz < 0) throw std::runtime_error("input failed");
    }

public:
    static constexpr int get_buf_size() { return buf_size; }
    Reader() noexcept : fd(0), idx(0), sz(0), state(true) {}
    Reader(int fd) noexcept : fd(fd), idx(0), sz(0), state(true) {}
    Reader(FILE* fp) noexcept : fd(fileno(fp)), idx(0), sz(0), state(true) {}

    class iterator {
    private:
        Reader* reader;

    public:
        using difference_type = void;
        using value_type = void;
        using pointer = void;
        using reference = void;
        using iterator_category = std::input_iterator_tag;

        iterator() : reader(nullptr) {}
        explicit iterator(Reader& reader) : reader(&reader) {}
        explicit iterator(Reader* reader) : reader(reader) {}

        iterator& operator++() {
            if (reader->idx == reader->sz) reader->read_buf();
            ++reader->idx;
            return *this;
        }
        iterator operator++(int) {
            iterator res = *this;
            ++(*this);
            return res;
        }
        char operator*() const {
            if (reader->idx == reader->sz) reader->read_buf();
            if (reader->idx < reader->sz) return reader->buffer[reader->idx];
            reader->state = false;
            return '\0';
        }
        bool rdstate() const { return reader->state; }
        void setstate(bool state) { reader->state = state; }
    };

    iterator begin() noexcept { return iterator(this); }
};

Reader<> reader(0);

template<class Iterator, std::size_t decimal_precision = 16> class Scanner {
public:
    using iterator_type = Iterator;

private:
    template<class, class = void> struct has_scan : std::false_type {};
    template<class T>
    struct has_scan<
        T, decltype(std::declval<T>().scan(std::declval<Scanner&>()), (void)0)>
        : std::true_type {};
    Iterator itr;

public:
    Scanner() = default;
    Scanner(const Iterator& itr) : itr(itr) {}

    char scan_char() {
        char c = *itr;
        ++itr;
        return c;
    }

    Scanner ignore(int n = 1) {
        rep (n) ++itr;
        return *this;
    }

    inline void discard_space() {
        while (('\t' <= *itr && *itr <= '\r') || *itr == ' ') ++itr;
    }
    void scan(char& a) {
        discard_space();
        a = *itr;
        ++itr;
    }
    void scan(bool& a) {
        discard_space();
        a = *itr != '0';
        ++itr;
    }
    void scan(std::string& a) {
        discard_space();
        a.clear();
        while ((*itr < '\t' || '\r' < *itr) && *itr != ' ' && *itr != '\0') {
            a += *itr;
            ++itr;
        }
    }
    template<std::size_t len> void scan(std::bitset<len>& a) {
        discard_space();
        rrep (i, len) {
            a[i] = *itr != '0';
            ++itr;
        }
    }
    template<class T,
             typename std::enable_if<is_signed_int<T>::value &&
                                     !has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        discard_space();
        if (*itr == '-') {
            ++itr;
            a = 0;
            while ('0' <= *itr && *itr <= '9') {
                a = a * 10 - (*itr - '0');
                ++itr;
            }
        }
        else {
            a = 0;
            while ('0' <= *itr && *itr <= '9') {
                a = a * 10 + (*itr - '0');
                ++itr;
            }
        }
    }
    template<class T,
             typename std::enable_if<is_unsigned_int<T>::value &&
                                     !has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        discard_space();
        a = 0;
        while ('0' <= *itr && *itr <= '9') {
            a = a * 10 + *itr - '0';
            ++itr;
        }
    }
    template<class T,
             typename std::enable_if<std::is_floating_point<T>::value &&
                                     !has_scan<T>::value>::type* = nullptr>
    void scan(T& a) {
        discard_space();
        bool sgn = false;
        if (*itr == '-') {
            sgn = true;
            ++itr;
        }
        a = 0;
        while ('0' <= *itr && *itr <= '9') {
            a = a * 10 + *itr - '0';
            ++itr;
        }
        if (*itr == '.') {
            ++itr;
            T n = 0, d = 1;
            for (int i = 0;
                 '0' <= *itr && *itr <= '9' && i < (int)decimal_precision;
                 ++i) {
                n = n * 10 + *itr - '0';
                d *= 10;
                ++itr;
            }
            while ('0' <= *itr && *itr <= '9') ++itr;
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
        each_for (i : a) scan(i);
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

    explicit operator bool() const { return itr.rdstate(); }

    friend Scanner& getline(Scanner& scan, std::string& a) {
        a.erase();
        char c;
        if ((c = scan.scan_char()) == '\n' || c == '\0') return scan;
        a += c;
        while ((c = scan.scan_char()) != '\n' && c != '\0') a += c;
        scan.itr.setstate(true);
        return scan;
    }
};

Scanner<Reader<>::iterator> scan(reader.begin());
#line 2 "library/template/out.hpp"

#line 8 "library/template/out.hpp"

template<std::size_t buf_size = IO_BUFFER_SIZE> class Writer {
private:
    int fd, idx;
    std::array<char, buf_size> buffer;
    inline void write_buf() {
        int num = write(fd, buffer.begin(), idx);
        idx = 0;
        if (num < 0) throw std::runtime_error("output failed");
    }

public:
    Writer() noexcept : fd(1), idx(0) {}
    Writer(int fd) noexcept : fd(fd), idx(0) {}
    Writer(FILE* fp) noexcept : fd(fileno(fp)), idx(0) {}

    ~Writer() { write_buf(); }

    class iterator {
    private:
        Writer* writer;

    public:
        using difference_type = void;
        using value_type = void;
        using pointer = void;
        using reference = void;
        using iterator_category = std::output_iterator_tag;

        iterator() noexcept : writer(nullptr) {}
        explicit iterator(Writer& writer) noexcept : writer(&writer) {}
        explicit iterator(Writer* writer) noexcept : writer(writer) {}

        iterator& operator++() {
            ++writer->idx;
            if (writer->idx == buf_size) writer->write_buf();
            return *this;
        }
        iterator operator++(int) {
            iterator res = *this;
            ++(*this);
            return res;
        }
        char& operator*() const { return writer->buffer[writer->idx]; }
        void flush() const { writer->write_buf(); }
    };

    iterator begin() noexcept { return iterator(this); }
};

Writer<> writer(1), ewriter(2);

template<class Iterator, bool debug = false> class Printer {
public:
    using iterator_type = Iterator;

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
    Iterator itr;
    std::size_t decimal_precision;

public:
    void print_char(char c) {
        *itr = c;
        ++itr;
    }

    void flush() { itr.flush(); }

    Printer() noexcept = default;
    explicit Printer(const Iterator& itr) noexcept
        : itr(itr), decimal_precision(16) {}

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
        std::string s;
        while (a) {
            s += (char)(a % 10 + '0');
            a /= 10;
        }
        for (auto i = s.rbegin(); i != s.rend(); ++i) print_char(*i);
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

template<class Iterator, bool debug>
Printer<Iterator, debug>& endl(Printer<Iterator, debug>& pr) {
    pr.print_char('\n');
    pr.flush();
    return pr;
}
template<class Iterator, bool debug>
Printer<Iterator, debug>& flush(Printer<Iterator, debug>& pr) {
    pr.flush();
    return pr;
}

struct SetPrec {
    int n;
    template<class Pr> void print(Pr& pr) const { pr.set_decimal_precision(n); }
    template<class Pr> void debug(Pr& pr) const { pr.set_decimal_precision(n); }
};
SetPrec setprec(int n) { return SetPrec{n}; };

Printer<Writer<>::iterator> print(writer.begin()), eprint(ewriter.begin());

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

#ifdef SHIO_LOCAL
Printer<Writer<>::iterator, true> debug(writer.begin()),
    edebug(ewriter.begin());
#else
char debug_iterator_character;
class DebugIterator {
public:
    DebugIterator() noexcept = default;
    DebugIterator& operator++() { return *this; }
    DebugIterator& operator++(int) { return *this; }
    char& operator*() const { return debug_iterator_character; }
    void flush() const {}
};
Printer<DebugIterator> debug, edebug;
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

inline CONSTEXPR int msb(ull x) {
    int res = x ? 0 : -1;
    if (x & 0xFFFFFFFF00000000) x &= 0xFFFFFFFF00000000, res += 32;
    if (x & 0xFFFF0000FFFF0000) x &= 0xFFFF0000FFFF0000, res += 16;
    if (x & 0xFF00FF00FF00FF00) x &= 0xFF00FF00FF00FF00, res += 8;
    if (x & 0xF0F0F0F0F0F0F0F0) x &= 0xF0F0F0F0F0F0F0F0, res += 4;
    if (x & 0xCCCCCCCCCCCCCCCC) x &= 0xCCCCCCCCCCCCCCCC, res += 2;
    return res + ((x & 0xAAAAAAAAAAAAAAAA) ? 1 : 0);
}

inline CONSTEXPR int ceil_log2(ull x) { return x ? msb(x - 1) + 1 : 0; }

inline CONSTEXPR ull reverse(ull x) {
    x = ((x & 0xAAAAAAAAAAAAAAAA) >> 1) | ((x & 0x5555555555555555) << 1);
    x = ((x & 0xCCCCCCCCCCCCCCCC) >> 2) | ((x & 0x3333333333333333) << 2);
    x = ((x & 0xF0F0F0F0F0F0F0F0) >> 4) | ((x & 0x0F0F0F0F0F0F0F0F) << 4);
    x = ((x & 0xFF00FF00FF00FF00) >> 8) | ((x & 0x00FF00FF00FF00FF) << 8);
    x = ((x & 0xFFFF0000FFFF0000) >> 16) | ((x & 0x0000FFFF0000FFFF) << 16);
    return (x >> 32) | (x << 32);
}

inline CONSTEXPR ull reverse(ull x, int n) { return reverse(x) >> (64 - n); }

} // namespace bitop

inline CONSTEXPR int popcnt(ull x) noexcept {
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

inline CONSTEXPR ll gcd(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        const ll c = a;
        a = b;
        b = c % b;
    }
    return a;
}
inline CONSTEXPR ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }

inline CONSTEXPR bool is_prime(ll N) {
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

inline CONSTEXPR ll my_pow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res *= a;
        b >>= 1;
        a *= a;
    }
    return res;
}
inline CONSTEXPR ll mod_pow(ll a, ll b, ll mod) {
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
        each_for (i : vec) i = get(i);
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
    T get() const { return val; }
    static constexpr T get_mod() { return mod; }
    static StaticModInt raw(T v) {
        StaticModInt res;
        res.val = v;
        return res;
    }
    StaticModInt inv() const {
        if IF_CONSTEXPR (mod == 1000000007) {
            if (val <= 10) return inv1000000007[val];
        }
        else if IF_CONSTEXPR (mod == 998244353) {
            if (val <= 10) return inv998244353[val];
        }
        return mod_inv(val, mod);
    }
    StaticModInt& operator++() {
        ++val;
        if (val == mod) val = 0;
        return *this;
    }
    StaticModInt operator++(int) {
        StaticModInt res = *this;
        ++*this;
        return res;
    }
    StaticModInt& operator--() {
        if (val == 0) val = mod;
        --val;
        return *this;
    }
    StaticModInt operator--(int) {
        StaticModInt res = *this;
        --*this;
        return res;
    }
    StaticModInt& operator+=(const StaticModInt& other) {
        val += other.val;
        if (val >= mod) val -= mod;
        return *this;
    }
    StaticModInt& operator-=(const StaticModInt& other) {
        if (val < other.val) val += mod;
        val -= other.val;
        return *this;
    }
    StaticModInt& operator*=(const StaticModInt& other) {
        large_t a = val;
        a *= other.val;
        a %= mod;
        val = a;
        return *this;
    }
    StaticModInt& operator/=(const StaticModInt& other) {
        *this *= other.inv();
        return *this;
    }
    friend StaticModInt operator+(const StaticModInt& lhs,
                                  const StaticModInt& rhs) {
        return StaticModInt(lhs) += rhs;
    }
    friend StaticModInt operator-(const StaticModInt& lhs,
                                  const StaticModInt& rhs) {
        return StaticModInt(lhs) -= rhs;
    }
    friend StaticModInt operator*(const StaticModInt& lhs,
                                  const StaticModInt& rhs) {
        return StaticModInt(lhs) *= rhs;
    }
    friend StaticModInt operator/(const StaticModInt& lhs,
                                  const StaticModInt& rhs) {
        return StaticModInt(lhs) /= rhs;
    }
    StaticModInt operator+() const { return StaticModInt(*this); }
    StaticModInt operator-() const { return StaticModInt() - *this; }
    friend bool operator==(const StaticModInt& lhs, const StaticModInt& rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const StaticModInt& lhs, const StaticModInt& rhs) {
        return lhs.val != rhs.val;
    }
    StaticModInt pow(ll a) const {
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
#line 2 "library/math/ExpPolySum.hpp"

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
        rreps (i, n, b) factinv[i - 1] = factinv[i] * i;
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
#line 2 "library/math/poly/SamplingPointsShift.hpp"

#line 2 "library/math/convolution/Convolution.hpp"

#line 5 "library/math/convolution/Convolution.hpp"

CONSTEXPR ull primitive_root_for_convolution(ull p) {
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

template<unsigned int p> class NthRoot {
private:
    static constexpr unsigned int lg = bitop::msb((p - 1) & (1 - p));
    unsigned int root[lg + 1];
    unsigned int inv_root[lg + 1];
    unsigned int rate[lg + 1];
    unsigned int inv_rate[lg + 1];

public:
    constexpr NthRoot() : root{}, inv_root{}, rate{}, inv_rate{} {
        root[lg] = mod_pow(primitive_root_for_convolution(p), (p - 1) >> lg, p);
        inv_root[lg] = mod_pow(root[lg], p - 2, p);
        rrep (i, lg) {
            root[i] = (ull)root[i + 1] * root[i + 1] % p;
            inv_root[i] = (ull)inv_root[i + 1] * inv_root[i + 1] % p;
        }
        ull r = 1;
        rep (i, 2, lg + 1) {
            rate[i - 2] = r * root[i] % p;
            r = r * inv_root[i] % p;
        }
        r = 1;
        rep (i, 2, lg + 1) {
            inv_rate[i - 2] = r * inv_root[i] % p;
            r = r * root[i] % p;
        }
    }
    static constexpr unsigned int get_lg() { return lg; }
    constexpr unsigned int get(int n) const { return root[n]; }
    constexpr unsigned int inv(int n) const { return inv_root[n]; }
    constexpr unsigned int get_rate(int n) const { return rate[n]; }
    constexpr unsigned int get_inv_rate(int n) const { return inv_rate[n]; }
};

template<unsigned int p> constexpr NthRoot<p> nth_root;

template<class T> void number_theoretic_transform(std::vector<T>& a) {
    int n = a.size();
    int lg = bitop::msb(n - 1) + 1;
    rrep (i, lg) {
        T z = T(1);
        rep (j, 1 << (lg - i - 1)) {
            int offset = j << (i + 1);
            rep (k, 1 << i) {
                T x = a[offset + k];
                T y = a[offset + k + (1 << i)] * z;
                a[offset + k] = x + y;
                a[offset + k + (1 << i)] = x - y;
            }
            if (j != (1 << (lg - i - 1)) - 1) {
                z *= nth_root<T::get_mod()>.get_rate(popcnt(j & ~(j + 1)));
            }
        }
    }
}
template<class T> void inverse_number_theoretic_transform(std::vector<T>& a) {
    int n = a.size();
    int lg = bitop::msb(n - 1) + 1;
    rep (i, lg) {
        T z = T(1);
        rep (j, 1 << (lg - i - 1)) {
            int offset = j << (i + 1);
            rep (k, 1 << i) {
                T x = a[offset + k];
                T y = a[offset + k + (1 << i)];
                a[offset + k] = x + y;
                a[offset + k + (1 << i)] = (x - y) * z;
            }
            if (j != (1 << (lg - i - 1)) - 1) {
                z *= nth_root<T::get_mod()>.get_inv_rate(popcnt(j & ~(j + 1)));
            }
        }
    }
    T inv_n = T(1) / n;
    each_for (x : a) x *= inv_n;
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

template<unsigned int p>
std::vector<static_modint<p>>
convolution_for_any_mod(const std::vector<static_modint<p>>& a,
                        const std::vector<static_modint<p>>& b);

template<unsigned int p>
std::vector<static_modint<p>>
convolution(const std::vector<static_modint<p>>& a,
            const std::vector<static_modint<p>>& b) {
    unsigned int n = a.size(), m = b.size();
    if (n == 0 || m == 0) return {};
    if (n <= 60 || m <= 60) return internal::convolution_naive(a, b);
    if (n + m - 1 > ((1 - p) & (p - 1))) return convolution_for_any_mod(a, b);
    if (n == m && a == b) return internal::convolution_pow2(a);
    return internal::convolution(a, b);
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

template<unsigned int p>
std::vector<static_modint<p>>
convolution_for_any_mod(const std::vector<static_modint<p>>& a,
                        const std::vector<static_modint<p>>& b) {
    int n = a.size(), m = b.size();
    assert(n + m - 1 <= (1 << 26));
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
    std::vector<static_modint<p>> res(n + m - 1);
    rep (i, n + m - 1) {
        ll t1 = c1[i];
        ll t2 = (c2[i] - t1 + MOD2) * INV1_2 % MOD2;
        if (t2 < 0) t2 += MOD2;
        ll t3 =
            ((c3[i] - t1 + MOD3) * INV1_3 % MOD3 - t2 + MOD3) * INV2_3 % MOD3;
        if (t3 < 0) t3 += MOD3;
        res[i] = static_modint<p>(t1 + (t2 + t3 * MOD2) % p * MOD1);
    }
    return res;
}

template<class T> void ntt_doubling_(std::vector<T>& a) {
    int n = a.size();
    auto b = a;
    inverse_number_theoretic_transform(b);
    const T z = internal::nth_root<T::get_mod()>.get(bitop::msb(n) + 1);
    T r = 1;
    rep (i, n) {
        b[i] *= r;
        r *= z;
    }
    number_theoretic_transform(b);
    std::copy(all(b), std::back_inserter(a));
}

template<unsigned int p> struct is_ntt_friendly : std::false_type {};

template<> struct is_ntt_friendly<998244353> : std::true_type {};

/**
 * @brief Convolution(畳み込み)
 * @docs docs/math/convolution/Convolution.md
 */
#line 2 "library/math/poly/FormalPowerSeries.hpp"

#line 2 "library/math/SqrtMod.hpp"

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
#line 5 "library/math/SqrtMod.hpp"

template<class T> ll sqrt_mod(ll a) {
    const ll p = T::get_mod();
    if (p == 2) return a;
    if (a == 0) return 0;
    if (T{a}.pow((p - 1) >> 1) != 1) return -1;
    T b = 2;
    while (T{b}.pow((p - 1) >> 1) == 1) ++b;
    ll s = 0, t = p - 1;
    while ((t & 1) == 0) t >>= 1, ++s;
    T x = T{a}.pow((t + 1) >> 1);
    T w = T{a}.pow(t);
    T v = T{b}.pow(t);
    while (w != 1) {
        ll k = 0;
        T y = w;
        while (y != 1) {
            y *= y;
            ++k;
        }
        T z = v;
        rep (s - k - 1) z *= z;
        x *= z;
        w *= z * z;
    }
    return std::min<ll>(x.get(), p - x.get());
}

ll sqrt_mod(ll a, ll p) {
    if (p == 2) return a;
    using mint = MontgomeryModInt<unsigned int, 493174342>;
    mint::set_mod(p);
    return sqrt_mod<mint>(a);
}

/**
 * @brief SqrtMod(平方剰余)
 * @docs docs/math/SqrtMod.md
 * @see https://37zigen.com/tonelli-shanks-algorithm/
 */
#line 7 "library/math/poly/FormalPowerSeries.hpp"

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
        rep (i, this->size()) (*this)[i] /= rhs;
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
                 AlwaysTrue && is_ntt_friendly<T::get_mod()>::value>::type* =
                 nullptr>
    FormalPowerSeries inv(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] != 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries res(1, (*this)[0].inv());
        for (int m = 1; m < deg; m <<= 1) {
            FormalPowerSeries f(2 * m);
            for (int i = 0; i < std::min(2 * m, (int)this->size()); i++)
                f[i] = (*this)[i];
            res.resize(2 * m);
            FormalPowerSeries dft = res;
            number_theoretic_transform(f);
            number_theoretic_transform(dft);
            rep (i, 2 * m) f[i] *= dft[i];
            inverse_number_theoretic_transform(f);
            std::fill(f.begin(), f.begin() + m, T{0});
            number_theoretic_transform(f);
            rep (i, 2 * m) dft[i] *= f[i];
            inverse_number_theoretic_transform(dft);
            rep (i, m, 2 * m) res[i] = -dft[i];
        }
        return res.prefix(deg);
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && !is_ntt_friendly<T::get_mod()>::value>::type* =
                 nullptr>
    FormalPowerSeries inv(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] != 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries res(1, (*this)[0].inv());
        for (int m = 1; m < deg; m <<= 1) {
            res = (res * 2 - (res * res * this->prefix(2 * m)).prefix(2 * m))
                      .prefix(2 * m);
        }
        return res.prefix(deg);
    }
    FormalPowerSeries log(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] == 1);
        if (deg == -1) deg = this->size();
        return (diff() * inv(deg)).prefix(deg - 1).integral();
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && is_ntt_friendly<T::get_mod()>::value>::type* =
                 nullptr>
    FormalPowerSeries exp(int deg = -1) const {
        assert(this->size() > 0 && (*this)[0] == 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries f(1, 1);
        FormalPowerSeries g(1, 1);
        FormalPowerSeries dft_f(1, 1);
        for (int m = 1; m < deg; m <<= 1) {
            FormalPowerSeries q = prefix(m).diff();
            q.resize(m);
            number_theoretic_transform(q);
            rep (i, m) q[i] *= dft_f[i];
            inverse_number_theoretic_transform(q);
            FormalPowerSeries s = f.diff();
            s.resize(m);
            rep (i, m) s[i] -= q[i];
            s.insert(s.begin(), (T)s.back());
            s.pop_back();
            FormalPowerSeries dft_g = g;
            s.resize(2 * m);
            dft_g.resize(2 * m);
            number_theoretic_transform(s);
            number_theoretic_transform(dft_g);
            rep (i, 2 * m) s[i] *= dft_g[i];
            inverse_number_theoretic_transform(s);
            FormalPowerSeries u =
                (prefix(2 * m) - (s.prefix(m) << (m - 1)).integral()) >> m;
            u.resize(2 * m);
            FormalPowerSeries dft_f_2 = f;
            dft_f_2.resize(2 * m);
            number_theoretic_transform(u);
            number_theoretic_transform(dft_f_2);
            rep (i, 2 * m) u[i] *= dft_f_2[i];
            inverse_number_theoretic_transform(u);
            f = f + (u.prefix(m) << m);
            if (2 * m < deg) {
                g.resize(2 * m);
                FormalPowerSeries dft_g_2 = g;
                FormalPowerSeries dft_f_2 = f;
                number_theoretic_transform(dft_g_2);
                number_theoretic_transform(dft_f_2);
                dft_f = dft_f_2;
                rep (i, 2 * m) dft_f_2[i] *= dft_g_2[i];
                inverse_number_theoretic_transform(dft_f_2);
                std::fill(dft_f_2.begin(), dft_f_2.begin() + m, T{0});
                number_theoretic_transform(dft_f_2);
                rep (i, 2 * m) dft_f_2[i] *= dft_g_2[i];
                inverse_number_theoretic_transform(dft_f_2);
                rep (i, m, 2 * m) g[i] = -dft_f_2[i];
            }
        }
        return f.prefix(deg);
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && !is_ntt_friendly<T::get_mod()>::value>::type* =
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
        if ((i128)(d)*k >= deg) {
            FormalPowerSeries res(deg);
            return res;
        }
        deg -= d * k;
        FormalPowerSeries res = (((*this >> d) / a).log(deg) * k).exp(deg);
        res *= a.pow(k);
        res <<= d * k;
        return res;
    }
    FormalPowerSeries sqrt(int deg = -1) const {
        if (deg == -1) deg = this->size();
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
        if (d & 1) return {};
        deg -= (d >> 1);
        if (deg <= 0) {
            FormalPowerSeries res(deg);
            return res;
        }
        FormalPowerSeries f = (*this >> d);
        T sq = sqrt_mod<T>(a.get());
        if (sq == -1) return {};
        FormalPowerSeries g(1, sq);
        for (int m = 1; m < deg; m <<= 1) {
            g = (g + (f.prefix(2 * m) * g.inv(2 * m)).prefix(2 * m)) / 2;
        }
        g.resize(deg);
        return g << (d >> 1);
    }
    FormalPowerSeries compose(FormalPowerSeries g, int deg = -1) const {
        if (this->empty()) return {};
        if (g.empty()) return {(*this)[0]};
        assert(g[0] == 0);
        int n = deg == -1 ? this->size() : deg;
        int m = 1 << (bitop::ceil_log2(
                          std::max<int>(1, std::sqrt(n / std::log2(n)))) +
                      1);
        FormalPowerSeries p = g.prefix(m), q = g >> m;
        p.shrink();
        q.shrink();
        int l = (n + m - 1) / m;
        std::vector<FormalPowerSeries> fs(this->size());
        rep (i, this->size()) fs[i] = FormalPowerSeries{(*this)[i]};
        FormalPowerSeries pd = p.diff();
        int z = 0;
        while (z < (int)pd.size() && pd[z] == T{0}) z++;
        if (z == (int)pd.size()) {
            FormalPowerSeries ans;
            rrep (i, l) {
                ans = ((ans * q) << m).prefix(n - i * m) +
                      FormalPowerSeries{(*this)[i]};
            }
            return ans;
        }
        pd = (pd >> z).inv(n);
        FormalPowerSeries t = p;
        for (int k = 1; fs.size() > 1; k <<= 1) {
            std::vector<FormalPowerSeries> nfs((fs.size() + 1) / 2);
            t.resize(1 << (bitop::ceil_log2(t.size()) + 1));
            number_theoretic_transform(t);
            rep (i, fs.size() / 2) {
                nfs[i] = std::move(fs[2 * i]);
                fs[2 * i + 1].resize(t.size());
                number_theoretic_transform(fs[2 * i + 1]);
                rep (j, t.size()) fs[2 * i + 1][j] *= t[j];
                inverse_number_theoretic_transform(fs[2 * i + 1]);
                if ((int)fs[2 * i + 1].size() > n) fs[2 * i + 1].resize(n);
                nfs[i] += fs[2 * i + 1];
            }
            if (fs.size() & 1) nfs.back() = std::move(fs.back());
            fs = std::move(nfs);
            if (fs.size() > 1) {
                rep (i, t.size()) t[i] *= t[i];
                inverse_number_theoretic_transform(t);
                if ((int)t.size() > n) t.resize(n);
            }
        }
        FormalPowerSeries fp = fs[0].prefix(n);
        FormalPowerSeries res = fp;
        int n2 = 1 << (bitop::ceil_log2(n) + 1);
        FormalPowerSeries qpow(n2);
        qpow[0] = 1;
        q.resize(n2);
        number_theoretic_transform(q);
        pd.resize(n2);
        number_theoretic_transform(pd);
        rep (i, 1, l) {
            if ((n - i * m) * 4 <= n2) {
                while ((n - i * m) * 4 <= n2) {
                    n2 /= 2;
                }
                inverse_number_theoretic_transform(q);
                q.resize(n - i * m);
                q.resize(n2);
                number_theoretic_transform(q);
                inverse_number_theoretic_transform(pd);
                pd.resize(n - i * m);
                pd.resize(n2);
                number_theoretic_transform(pd);
            }
            qpow.resize(n - i * m);
            qpow.resize(n2);
            number_theoretic_transform(qpow);
            rep (j, n2) qpow[j] *= q[j];
            inverse_number_theoretic_transform(qpow);
            qpow.resize(n - i * m);

            fp = fp.diff() >> z;
            fp.resize(n - i * m);
            fp.resize(n2);
            number_theoretic_transform(fp);
            rep (j, n2) fp[j] *= pd[j];
            inverse_number_theoretic_transform(fp);
            fp.resize(n - i * m);

            res += ((qpow * fp).prefix(n - i * m) * Comb::finv(i)) << (i * m);
        }
        return res;
    }
    FormalPowerSeries compinv(int deg = -1) const {
        assert(this->size() >= 2 && (*this)[0] == 0 && (*this)[1] != 0);
        if (deg == -1) deg = this->size();
        FormalPowerSeries fd = diff();
        FormalPowerSeries x{0, 1};
        FormalPowerSeries res{0, (*this)[1].inv()};
        for (int m = 2; m < deg; m <<= 1) {
            auto tmp = prefix(2 * m).compose(res);
            auto d = tmp.diff();
            auto gd = res.diff();
            res -=
                ((tmp - x) * (d.inv(2 * m) * gd).prefix(2 * m)).prefix(2 * m);
        }
        return res.prefix(deg);
    }
    template<bool AlwaysTrue = true,
             typename std::enable_if<
                 AlwaysTrue && is_ntt_friendly<T::get_mod()>::value>::type* =
                 nullptr>
    FormalPowerSeries& ntt_doubling() {
        ntt_doubling_(*this);
        return *this;
    }
};

/**
 * @brief FormalPowerSeries(形式的冪級数)
 * @docs docs/math/poly/FormalPowerSeries.md
 * @see https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
 */
#line 2 "library/math/poly/TaylorShift.hpp"

#line 7 "library/math/poly/TaylorShift.hpp"

template<class T, class Comb = Combinatorics<T>>
FormalPowerSeries<T> taylor_shift(FormalPowerSeries<T> f, T a) {
    const int n = f.size();
    Comb::init(n);
    rep (i, n) f[i] *= Comb::fact(i);
    FormalPowerSeries<T> g(n);
    T p = 1;
    rep (i, n) {
        g[n - 1 - i] = p * Comb::finv(i);
        p *= a;
    }
    f *= g;
    f >>= n - 1;
    rep (i, n) f[i] *= Comb::finv(i);
    return f;
}

/**
 * @brief TaylorShift
 * @docs docs/math/poly/TaylorShift.md
 */
#line 8 "library/math/poly/SamplingPointsShift.hpp"

template<class T, class Comb = Combinatorics<T>>
std::vector<T> sampling_points_shift(std::vector<T> a, int m, T t) {
    const int n = a.size();
    Comb::init(std::max(n, m));
    FormalPowerSeries<T> f(n), g(n);
    rep (i, n) f[i] = i & 1 ? -Comb::finv(i) : Comb::finv(i);
    rep (i, n) g[i] = a[i] * Comb::finv(i);
    f = (f * g).prefix(n);
    rep (i, n) f[i] *= Comb::fact(i);
    T p = 1;
    rep (i, n) {
        g[n - 1 - i] = p * Comb::finv(i);
        p *= t--;
    }
    f = (f * g) >> (n - 1);
    rep (i, n) f[i] *= Comb::finv(i);
    g.resize(m);
    rep (i, m) g[i] = Comb::finv(i);
    f = (f * g).prefix(m);
    rep (i, m) f[i] *= Comb::fact(i);
    return std::vector<T>(f);
}

template<class T, class Comb = Combinatorics<T>>
T sampling_points_shift(std::vector<T> a, T t) {
    const int n = a.size();
    Comb::init(n - 1);
    std::vector<T> cum(n, 1);
    rep (i, n - 1) cum[i + 1] = cum[i] * (i - t);
    T res = 0, cur = 1;
    rrep (i, n) {
        res += a[i] * cum[i] * cur * (i & 1 ? -Comb::finv(i) : Comb::finv(i)) * Comb::finv(n - i - 1);
        cur *= i - t;
    }
    return res;
}

/**
 * @brief SamplingPointsShift(標本点シフト)
 * @docs docs/math/poly/SamplingPointsShift.md
 */
#line 2 "library/math/PowEnumerate.hpp"

#line 2 "library/math/PrimeFactor.hpp"

#line 4 "library/math/PrimeFactor.hpp"

class PrimeFactor {
private:
    int MAX;
    std::vector<int> era, primes;

public:
    PrimeFactor(int MAX) : MAX(MAX), era(MAX + 1, -1), primes() {
        rep (i, 2, MAX + 1) {
            if (era[i] == -1) {
                era[i] = i;
                primes.push_back(i);
            }
            each_const (p : primes) {
                if (i * p > MAX) break;
                era[i * p] = p;
            }
        }
    }
    bool is_prime(int x) const { return era[x] == x; }
    int get_factor(int x) const { return era[x]; }
    std::vector<int> factorize(int x) const {
        std::vector<int> res;
        for (; x > 1; x /= era[x]) res.push_back(era[x]);
        return res;
    }
    const std::vector<int>& get_primes() const& { return primes; }
    std::vector<int> get_primes() && { return std::move(primes); }
};

class IsPrime {
private:
    int MAX;
    std::vector<bool> era;
    std::vector<int> primes;

public:
    IsPrime(int MAX) : MAX(MAX), era(MAX + 1, true), primes() {
        era[0] = era[1] = false;
        rep (i, 2, MAX + 1) {
            if (era[i]) primes.push_back(i);
            each_const (p : primes) {
                if (i * p > MAX) break;
                era[i * p] = false;
            }
        }
    }
    bool is_prime(int x) { return era[x]; }
    const std::vector<int>& get_primes() const& { return primes; }
    std::vector<int> get_primes() && { return std::move(primes); }
};

/**
 * @brief PrimeFactor(エラトステネスの篩)
 * @docs docs/math/PrimeFactor.md
 */
#line 5 "library/math/PowEnumerate.hpp"

// 1/a[i]   O(n + log p)
template<class T> std::vector<T> inv_enumerate(std::vector<T> a) {
    int n = a.size();
    std::vector<T> res(n + 1);
    res[0] = 1;
    rep (i, n) res[i + 1] = res[i] * a[i];
    T iv = 1 / res[n];
    res.pop_back();
    rrep (i, n) {
        res[i] *= iv;
        iv *= a[i];
    }
    return res;
}

// 0^m, 1^m, ..., n^m   O(n log m / log n)
template<class T> std::vector<T> pow_enumerate(int n, ll m) {
    if (m == 0) return std::vector<T>(n + 1, 1);
    std::vector<T> res(n + 1);
    res[0] = 0;
    res[1] = 1;
    PrimeFactor pf(n);
    rep (i, 2, n + 1) {
        if (pf.is_prime(i)) res[i] = T{i}.pow(m);
        else {
            ll x = pf.get_factor(i);
            res[i] = res[x] * res[i / x];
        }
    }
    return res;
}

/**
 * @brief PowEnumerate
 * @docs docs/math/PowEnumerate.md
 */
#line 7 "library/math/ExpPolySum.hpp"

template<class T, class Comb = Combinatorics<T>> T exp_poly_sum(T r, int d, ll n = -1) {
    if (n == -1) assert(r != 1);
    auto cum = pow_enumerate<T>(d + 1, d);
    Comb::init(d + 1);
    T c = 0;
    {
        T sm = 0, cur = 1;
        rep (i, d + 2) {
            sm += cur * cum[i];
            cur *= r;
            cum[i] = sm;
        }
    }
    {
        T t = 1;
        rep (i, d + 1) {
            c += t * Comb::comb(d + 1, i) * cum[d - i];
            t *= -r;
        }
        c /= (1 - r).pow(d + 1);
    }
    if (n == -1) return c;
    if (n == 0) return 0;
    if (r == 1) return sampling_points_shift<T, Comb>(cum, n - 1);
    cum.pop_back();
    T iv = 1 / r, cur = 1;
    rep (i, d + 1) {
        cum[i] = (c - cum[i]) * cur;
        cur *= iv;
    }
    return c - r.pow(n - 1) * sampling_points_shift<T, Comb>(cum, n - 1);
}

/**
 * @brief ExpPolySum($\sum_{i=0}^{\infty}r^ii^d$)
 * @docs docs/math/ExpPolySum.md
 * @see https://kyopro-friends.hatenablog.com/entry/2020/03/11/073122
 */
#line 5 "library/test/yosupo/math/sum_of_exponential_times_polynomial.test.cpp"
using namespace std;
using mint = modint998244353;
int main() {
    mint r; scan >> r;
    ll d, n; scan >> d >> n;
    prints(exp_poly_sum(r, d, n));
}
