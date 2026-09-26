#include <iterator>
#include <optional>
#include <stack>
#include <utility>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <deque>
#include <bitset>
#include <iomanip>
#include <unordered_set>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <numeric>
#include <set>
#include <fstream>
#include <istream>
#include <type_traits>
#include <array>
#include <ostream>
#include <iostream>
#include <list>
#include <algorithm>

#ifndef KK2_TEMPLATE_PROCON_HPP
#define KK2_TEMPLATE_PROCON_HPP 1


#ifndef KK2_TEMPLATE_CONSTANT_HPP
#define KK2_TEMPLATE_CONSTANT_HPP 1

#ifndef KK2_TEMPLATE_TYPE_ALIAS_HPP
#define KK2_TEMPLATE_TYPE_ALIAS_HPP 1


using u32 = unsigned int;
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;

using pi = std::pair<int, int>;
using pl = std::pair<i64, i64>;
using pil = std::pair<int, i64>;
using pli = std::pair<i64, int>;

template <class T> using vc = std::vector<T>;
template <class T> using vvc = std::vector<vc<T>>;
template <class T> using vvvc = std::vector<vvc<T>>;
template <class T> using vvvvc = std::vector<vvvc<T>>;

template <class T> using pq = std::priority_queue<T>;
template <class T> using pqi = std::priority_queue<T, std::vector<T>, std::greater<T>>;

#endif // KK2_TEMPLATE_TYPE_ALIAS_HPP

template <class T> constexpr T infty = 0;
template <> constexpr int infty<int> = (1 << 30) - 123;
template <> constexpr i64 infty<i64> = (1ll << 62) - (1ll << 31);
template <> constexpr i128 infty<i128> = (i128(1) << 126) - (i128(1) << 63);
template <> constexpr u32 infty<u32> = infty<int>;
template <> constexpr u64 infty<u64> = infty<i64>;
template <> constexpr u128 infty<u128> = infty<i128>;
template <> constexpr double infty<double> = infty<i64>;
template <> constexpr long double infty<long double> = infty<i64>;

constexpr int mod = 998244353;
constexpr int modu = 1e9 + 7;
constexpr long double PI = 3.14159265358979323846;

#endif // KK2_TEMPLATE_CONSTANT_HPP
#ifndef KK2_TEMPLATE_FUNCTION_UTIL_HPP
#define KK2_TEMPLATE_FUNCTION_UTIL_HPP 1


#ifndef KK2_MATH_MONOID_MAX_HPP
#define KK2_MATH_MONOID_MAX_HPP 1


#ifndef KK2_TYPE_TRAITS_IO_HPP
#define KK2_TYPE_TRAITS_IO_HPP 1



namespace kk2 {

namespace type_traits {

struct istream_tag {};
struct ostream_tag {};

} // namespace type_traits

template <typename T> using is_standard_istream =
    typename std::conditional<std::is_same<T, std::istream>::value
                                  || std::is_same<T, std::ifstream>::value,
                              std::true_type,
                              std::false_type>::type;
template <typename T> using is_standard_ostream =
    typename std::conditional<std::is_same<T, std::ostream>::value
                                  || std::is_same<T, std::ofstream>::value,
                              std::true_type,
                              std::false_type>::type;
template <typename T> using is_user_defined_istream = std::is_base_of<type_traits::istream_tag, T>;
template <typename T> using is_user_defined_ostream = std::is_base_of<type_traits::ostream_tag, T>;

template <typename T> using is_istream =
    typename std::conditional<is_standard_istream<T>::value || is_user_defined_istream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_ostream =
    typename std::conditional<is_standard_ostream<T>::value || is_user_defined_ostream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_istream_t = std::enable_if_t<is_istream<T>::value>;
template <typename T> using is_ostream_t = std::enable_if_t<is_ostream<T>::value>;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_IO_HPP

namespace kk2 {

namespace monoid {

template <class S, class Compare = std::less<S>> struct Max {
    static constexpr bool commutative = true;
    using M = Max;
    S a;
    bool is_unit;

    Max() : a(S()), is_unit(true) {}
    Max(S a_) : a(a_), is_unit(false) {}
    operator S() const { return a; }

    inline static M op(M l, M r) {
        if (l.is_unit or r.is_unit) return l.is_unit ? r : l;
        return Compare{}(l.a, r.a) ? r : l;
    }

    inline static M unit() { return M(); }

    bool operator==(const M &rhs) const {
        return is_unit == rhs.is_unit and (is_unit or a == rhs.a);
    }

    bool operator!=(const M &rhs) const {
        return is_unit != rhs.is_unit or (!is_unit and a != rhs.a);
    }

    template <class OStream, is_ostream_t<OStream> * = nullptr>
    friend OStream &operator<<(OStream &os, const M &x) {
        if (x.is_unit) os << "-inf";
        else os << x.a;
        return os;
    }

    template <class IStream, is_istream_t<IStream> * = nullptr>
    friend IStream &operator>>(IStream &is, M &x) {
        is >> x.a;
        x.is_unit = false;
        return is;
    }
};

} // namespace monoid

} // namespace kk2

#endif // MATH_MONOID_MAX_HPP
#ifndef KK2_MATH_MONOID_MIN_HPP
#define KK2_MATH_MONOID_MIN_HPP 1



namespace kk2 {

namespace monoid {

template <class S, class Compare = std::less<S>> struct Min {
    static constexpr bool commutative = true;
    using M = Min;
    S a;
    bool is_unit;

    Min() : a(S()), is_unit(true) {}
    Min(S a_) : a(a_), is_unit(false) {}
    operator S() const { return a; }

    inline static M op(M l, M r) {
        if (l.is_unit or r.is_unit) return l.is_unit ? r : l;
        return Compare{}(l.a, r.a) ? l : r;
    }

    inline static M unit() { return M(); }

    bool operator==(const M &rhs) const {
        return is_unit == rhs.is_unit and (is_unit or a == rhs.a);
    }

    bool operator!=(const M &rhs) const {
        return is_unit != rhs.is_unit or (!is_unit and a != rhs.a);
    }

    template <class OStream, is_ostream_t<OStream> * = nullptr>
    friend OStream &operator<<(OStream &os, const M &x) {
        if (x.is_unit) os << "inf";
        else os << x.a;
        return os;
    }

    template <class IStream, is_istream_t<IStream> * = nullptr>
    friend IStream &operator>>(IStream &is, M &x) {
        is >> x.a;
        x.is_unit = false;
        return is;
    }
};

} // namespace monoid

} // namespace kk2

#endif // KK2_MATH_MONOID_MIN_HPP
#ifndef KK2_TYPE_TRAITS_CONTAINER_TRAITS_HPP
#define KK2_TYPE_TRAITS_CONTAINER_TRAITS_HPP 1



namespace kk2 {

template <typename T> struct is_vector : std::false_type {};
template <typename T, typename Alloc> struct is_vector<std::vector<T, Alloc>> : std::true_type {};

// コンテナかどうかを判定するtraits
template <typename T> struct is_container : std::false_type {};

// 基本的なコンテナ型の特殊化
template <typename T, typename Alloc> struct is_container<std::vector<T, Alloc>> : std::true_type {
};

template <typename CharT, typename Traits, typename Alloc>
struct is_container<std::basic_string<CharT, Traits, Alloc>> : std::true_type {};

template <typename T, std::size_t N> struct is_container<std::array<T, N>> : std::true_type {};

template <typename T, typename Alloc> struct is_container<std::deque<T, Alloc>> : std::true_type {};

template <typename T, typename Alloc> struct is_container<std::list<T, Alloc>> : std::true_type {};

// SFINAEでコンテナを判定するためのヘルパー
template <typename T> using is_container_t =
    typename std::enable_if_t<is_container<T>::value, std::nullptr_t>;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_CONTAINER_TRAITS_HPP

namespace kk2 {

template <class T, class... Sizes> auto make_vector(int first, Sizes... sizes) {
    if constexpr (sizeof...(sizes) == 0) {
        return std::vector<T>(first);
    } else {
        return std::vector<decltype(make_vector<T>(sizes...))>(first, make_vector<T>(sizes...));
    }
}

template <class T, class U> void fill_all(std::vector<T> &v, const U &x) {
    if constexpr (is_vector<T>::value) {
        for (auto &u : v) fill_all(u, x);
    } else {
        std::fill(v.begin(), v.end(), T(x));
    }
}

template <class T, class U> int iota_all(std::vector<T> &v, U x, int offset = 0) {
    if constexpr (is_vector<T>::value) {
        for (auto &u : v) offset += iota_all(u, x + offset);
    } else {
        for (auto &u : v) u = x++, ++offset;
    }
    return offset;
}

template <class C> int mysize(const C &c) { return size(c); }


// T: commutative monoid, F: (U, T) -> U
template <class U, class T, class F>
U all_monoid_prod(const std::vector<T> &v, U unit, const F &f) {
    U res = unit;
    if constexpr (is_vector<T>::value) {
        for (const auto &x : v) res = f(res, all_monoid_prod(x, unit, f));
    } else {
        for (const auto &x : v) res = f(res, x);
    }
    return res;
}

template <class U, class T> U all_sum(const std::vector<T> &v, U unit = U()) {
    return all_monoid_prod<U, T>(v, unit, [](U a, U b) { return a + b; });
}
template <class U, class T> U all_prod(const std::vector<T> &v, U unit = U(1)) {
    return all_monoid_prod<U, T>(v, unit, [](U a, U b) { return a * b; });
}
template <class U, class T> U all_xor(const std::vector<T> &v, U unit = U()) {
    return all_monoid_prod<U, T>(v, unit, [](U a, U b) { return a ^ b; });
}
template <class U, class T> U all_and(const std::vector<T> &v, U unit = U(-1)) {
    return all_monoid_prod<U, T>(v, unit, [](U a, U b) { return a & b; });
}
template <class U, class T> U all_or(const std::vector<T> &v, U unit = U()) {
    return all_monoid_prod<U, T>(v, unit, [](U a, U b) { return a | b; });
}
template <class U, class T> U all_min(const std::vector<T> &v) {
    return all_monoid_prod<monoid::Min<U>, T>(v, monoid::Min<U>::unit(), monoid::Min<U>::op);
}
template <class U, class T> U all_max(const std::vector<T> &v) {
    return all_monoid_prod<monoid::Max<U>, T>(v, monoid::Max<U>::unit(), monoid::Max<U>::op);
}
template <class U, class T> U all_gcd(const std::vector<T> &v, U unit = U()) {
    return all_monoid_prod<U, T>(v, unit, [](U a, U b) { return std::gcd(a, b); });
}
template <class U, class T> U all_lcm(const std::vector<T> &v, U unit = U(1)) {
    return all_monoid_prod<U, T>(v, unit, [](U a, U b) { return std::lcm(a, b); });
}

} // namespace kk2

#endif // KK2_TEMPLATE_FUNCTION_UTIL_HPP
#ifndef KK2_TEMPLATE_IO_UTIL_HPP
#define KK2_TEMPLATE_IO_UTIL_HPP 1



// なんかoj verifyはプロトタイプ宣言が落ちる

namespace impl {

struct read {
    template <class IStream, class T> inline static void all_read(IStream &is, T &x) { is >> x; }

    template <class IStream, class T, class U>
    inline static void all_read(IStream &is, std::pair<T, U> &p) {
        all_read(is, p.first);
        all_read(is, p.second);
    }

    template <class IStream, class T> inline static void all_read(IStream &is, std::vector<T> &v) {
        for (T &x : v) all_read(is, x);
    }

    template <class IStream, class T, size_t F>
    inline static void all_read(IStream &is, std::array<T, F> &a) {
        for (T &x : a) all_read(is, x);
    }
};

struct write {
    template <class OStream, class T> inline static void all_write(OStream &os, const T &x) {
        os << x;
    }

    template <class OStream, class T, class U>
    inline static void all_write(OStream &os, const std::pair<T, U> &p) {
        all_write(os, p.first);
        all_write(os, ' ');
        all_write(os, p.second);
    }

    template <class OStream, class T>
    inline static void all_write(OStream &os, const std::vector<T> &v) {
        for (int i = 0; i < (int)v.size(); ++i) {
            if (i) all_write(os, ' ');
            all_write(os, v[i]);
        }
    }

    template <class OStream, class T, size_t F>
    inline static void all_write(OStream &os, const std::array<T, F> &a) {
        for (int i = 0; i < (int)F; ++i) {
            if (i) all_write(os, ' ');
            all_write(os, a[i]);
        }
    }
};

} // namespace impl

template <class IStream, class T, class U, kk2::is_istream_t<IStream> * = nullptr>
IStream &operator>>(IStream &is, std::pair<T, U> &p) {
    impl::read::all_read(is, p);
    return is;
}

template <class IStream, class T, kk2::is_istream_t<IStream> * = nullptr>
IStream &operator>>(IStream &is, std::vector<T> &v) {
    impl::read::all_read(is, v);
    return is;
}

template <class IStream, class T, size_t F, kk2::is_istream_t<IStream> * = nullptr>
IStream &operator>>(IStream &is, std::array<T, F> &a) {
    impl::read::all_read(is, a);
    return is;
}

template <class OStream, class T, class U, kk2::is_ostream_t<OStream> * = nullptr>
OStream &operator<<(OStream &os, const std::pair<T, U> &p) {
    impl::write::all_write(os, p);
    return os;
}

template <class OStream, class T, kk2::is_ostream_t<OStream> * = nullptr>
OStream &operator<<(OStream &os, const std::vector<T> &v) {
    impl::write::all_write(os, v);
    return os;
}

template <class OStream, class T, size_t F, kk2::is_ostream_t<OStream> * = nullptr>
OStream &operator<<(OStream &os, const std::array<T, F> &a) {
    impl::write::all_write(os, a);
    return os;
}

#endif // KK2_TEMPLATE_IO_UTIL_HPP
#ifndef KK2_TEMPLATE_MACROS_HPP
#define KK2_TEMPLATE_MACROS_HPP 1

#define rep1(a) for (long long _ = 0; _ < (long long)(a); ++_)
#define rep2(i, a) for (long long i = 0; i < (long long)(a); ++i)
#define rep3(i, a, b) for (long long i = (a); i < (long long)(b); ++i)
#define repi2(i, a) for (long long i = (a) - 1; i >= 0; --i)
#define repi3(i, a, b) for (long long i = (a) - 1; i >= (long long)(b); --i)
#define overload3(a, b, c, d, ...) d
#define rep(...) overload3(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define repi(...) overload3(__VA_ARGS__, repi3, repi2, rep1)(__VA_ARGS__)

#define fi first
#define se second
#define all(p) begin(p), end(p)

#endif // KK2_TEMPLATE_MACROS_HPP

struct FastIOSetUp {
    FastIOSetUp() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);
    }
} fast_io_set_up;

auto &kin = std::cin;
auto &kout = std::cout;
auto (*kendl)(std::ostream &) = std::endl<char, std::char_traits<char>>;

void Yes(bool b = 1) { kout << (b ? "Yes\n" : "No\n"); }
void No(bool b = 1) { kout << (b ? "No\n" : "Yes\n"); }
void YES(bool b = 1) { kout << (b ? "YES\n" : "NO\n"); }
void NO(bool b = 1) { kout << (b ? "NO\n" : "YES\n"); }
void yes(bool b = 1) { kout << (b ? "yes\n" : "no\n"); }
void no(bool b = 1) { kout << (b ? "no\n" : "yes\n"); }
template <class T, class S> inline bool chmax(T &a, const S &b) { return (a < b ? a = b, 1 : 0); }
template <class T, class S> inline bool chmin(T &a, const S &b) { return (a > b ? a = b, 1 : 0); }

std::istream &operator>>(std::istream &is, u128 &x) {
    std::string s;
    is >> s;
    x = 0;
    for (char c : s) {
        assert('0' <= c && c <= '9');
        x = x * 10 + c - '0';
    }
    return is;
}

std::istream &operator>>(std::istream &is, i128 &x) {
    std::string s;
    is >> s;
    bool neg = s[0] == '-';
    x = 0;
    for (int i = neg; i < (int)s.size(); i++) {
        assert('0' <= s[i] && s[i] <= '9');
        x = x * 10 + s[i] - '0';
    }
    if (neg) x = -x;
    return is;
}

std::ostream &operator<<(std::ostream &os, u128 x) {
    if (x == 0) return os << '0';
    std::string s;
    while (x) {
        s.push_back('0' + x % 10);
        x /= 10;
    }
    std::reverse(s.begin(), s.end());
    return os << s;
}

std::ostream &operator<<(std::ostream &os, i128 x) {
    if (x == 0) return os << '0';
    if (x < 0) {
        os << '-';
        x = -x;
    }
    std::string s;
    while (x) {
        s.push_back('0' + x % 10);
        x /= 10;
    }
    std::reverse(s.begin(), s.end());
    return os << s;
}

#endif // KK2_TEMPLATE_PROCON_HPP
#ifndef KK2_TEMPLATE_DEBUG_HPP
#define KK2_TEMPLATE_DEBUG_HPP 1


#ifndef KK2_TYPE_TRAITS_MEMBER_HPP
#define KK2_TYPE_TRAITS_MEMBER_HPP 1



namespace kk2 {

#define HAS_MEMBER_FUNC(member)                                                                    \
    template <typename T, typename... Ts> struct has_member_func_##member##_impl {                 \
        template <typename U>                                                                      \
        static std::true_type check(decltype(std::declval<U>().member(std::declval<Ts>()...)) *);  \
        template <typename U> static std::false_type check(...);                                   \
        using type = decltype(check<T>(nullptr));                                                  \
    };                                                                                             \
    template <typename T, typename... Ts> struct has_member_func_##member                          \
        : has_member_func_##member##_impl<T, Ts...>::type {};                                      \
    template <typename T, typename... Ts> using has_member_func_##member##_t =                     \
        std::enable_if_t<has_member_func_##member<T, Ts...>::value>;                               \
    template <typename T, typename... Ts> using not_has_member_func_##member##_t =                 \
        std::enable_if_t<!has_member_func_##member<T, Ts...>::value>;

#define HAS_MEMBER_VAR(member)                                                                     \
    template <typename T> struct has_member_var_##member##_impl {                                  \
        template <typename U> static std::true_type check(decltype(std::declval<U>().member) *);   \
        template <typename U> static std::false_type check(...);                                   \
        using type = decltype(check<T>(nullptr));                                                  \
    };                                                                                             \
    template <typename T> struct has_member_var_##member                                           \
        : has_member_var_##member##_impl<T>::type {};                                              \
    template <typename T> using has_member_var_##member##_t =                                      \
        std::enable_if_t<has_member_var_##member<T>::value>;                                       \
    template <typename T> using not_has_member_var_##member##_t =                                  \
        std::enable_if_t<!has_member_var_##member<T>::value>;

HAS_MEMBER_FUNC(debug_output)
HAS_MEMBER_FUNC(val)


#undef HAS_MEMBER_FUNC
#undef HAS_MEMBER_VAR
} // namespace kk2

#endif // KK2_TYPE_TRAITS_MEMBER_HPP

namespace kk2 {

namespace debug {

#ifdef KK2

template <class OStream, is_ostream_t<OStream> *> void output(OStream &os);
template <class OStream, class T, is_ostream_t<OStream> *> void output(OStream &os, const T &t);
template <class OStream, class T, is_ostream_t<OStream> *>
void output(OStream &os, const std::vector<T> &v);
template <class OStream, class T, is_ostream_t<OStream> *>
void output(OStream &os, const std::vector<std::vector<T>> &d);
template <class OStream, class T, size_t F, is_ostream_t<OStream> *>
void output(OStream &os, const std::array<T, F> &a);
template <class OStream, class T, class U, is_ostream_t<OStream> *>
void output(OStream &os, const std::pair<T, U> &p);
template <class OStream, class T, is_ostream_t<OStream> *>
void output(OStream &os, const std::queue<T> &q);
template <class OStream, class T, class Container, class Compare, is_ostream_t<OStream> *>
void output(OStream &os, const std::priority_queue<T, Container, Compare> &q);
template <class OStream, class T, is_ostream_t<OStream> *>
void output(OStream &os, const std::deque<T> &d);
template <class OStream, class T, is_ostream_t<OStream> *>
void output(OStream &os, const std::stack<T> &s);
template <class OStream, class Key, class Compare, class Allocator, is_ostream_t<OStream> *>
void output(OStream &os, const std::set<Key, Compare, Allocator> &s);
template <class OStream, class Key, class Compare, class Allocator, is_ostream_t<OStream> *>
void output(OStream &os, const std::multiset<Key, Compare, Allocator> &s);
template <class OStream,
          class Key,
          class Hash,
          class KeyEqual,
          class Allocator,
          is_ostream_t<OStream> *>
void output(OStream &os, const std::unordered_set<Key, Hash, KeyEqual, Allocator> &s);
template <class OStream,
          class Key,
          class Hash,
          class KeyEqual,
          class Allocator,
          is_ostream_t<OStream> *>
void output(OStream &os, const std::unordered_multiset<Key, Hash, KeyEqual, Allocator> &s);
template <class OStream,
          class Key,
          class T,
          class Compare,
          class Allocator,
          is_ostream_t<OStream> *>
void output(OStream &os, const std::map<Key, T, Compare, Allocator> &m);
template <class OStream,
          class Key,
          class T,
          class Hash,
          class KeyEqual,
          class Allocator,
          is_ostream_t<OStream> *>
void output(OStream &os, const std::unordered_map<Key, T, Hash, KeyEqual, Allocator> &m);
template <class OStream, is_ostream_t<OStream> * = nullptr> void output(OStream &) {}

template <class OStream, class T, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const T &t) {
    if constexpr (has_member_func_debug_output<T, OStream &>::value) {
        t.debug_output(os);
    } else {
        os << t;
    }
}

template <class OStream, class T, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::vector<T> &v) {
    os << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        output(os, v[i]);
        if (i + 1 != (int)v.size()) os << ", ";
    }
    os << "]";
}

template <class OStream, class T, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::vector<std::vector<T>> &d) {
    os << "[\n";
    for (int i = 0; i < (int)d.size(); i++) {
        output(os, d[i]);
        output(os, "\n");
    }
    os << "]";
}

template <class OStream, class T, size_t F, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::array<T, F> &a) {
    os << "[";
    for (int i = 0; i < (int)F; i++) {
        output(os, a[i]);
        if (i + 1 != (int)F) os << ", ";
    }
    os << "]";
}

template <class OStream, class T, class U, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::pair<T, U> &p) {
    os << "(";
    output(os, p.first);
    os << ", ";
    output(os, p.second);
    os << ")";
}

template <class OStream, class T, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::queue<T> &q) {
    os << "[";
    std::queue<T> tmp = q;
    while (!tmp.empty()) {
        output(os, tmp.front());
        tmp.pop();
        if (!tmp.empty()) os << ", ";
    }
    os << "]";
}

template <class OStream, class T, class Container, class Compare, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::priority_queue<T, Container, Compare> &q) {
    os << "[";
    std::priority_queue<T, Container, Compare> tmp = q;
    while (!tmp.empty()) {
        output(os, tmp.top());
        tmp.pop();
        if (!tmp.empty()) os << ", ";
    }
    os << "]";
}

template <class OStream, class T, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::deque<T> &d) {
    os << "[";
    std::deque<T> tmp = d;
    while (!tmp.empty()) {
        output(os, tmp.front());
        tmp.pop_front();
        if (!tmp.empty()) os << ", ";
    }
    os << "]";
}

template <class OStream, class T, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::stack<T> &s) {
    os << "[";
    std::stack<T> tmp = s;
    std::vector<T> v;
    while (!tmp.empty()) {
        v.push_back(tmp.top());
        tmp.pop();
    }
    for (int i = (int)v.size() - 1; i >= 0; i--) {
        output(os, v[i]);
        if (i != 0) os << ", ";
    }
    os << "]";
}

template <class OStream,
          class Key,
          class Compare,
          class Allocator,
          is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::set<Key, Compare, Allocator> &s) {
    os << "{";
    std::set<Key, Compare, Allocator> tmp = s;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        output(os, *it);
        if (std::next(it) != tmp.end()) os << ", ";
    }
    os << "}";
}

template <class OStream,
          class Key,
          class Compare,
          class Allocator,
          is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::multiset<Key, Compare, Allocator> &s) {
    os << "{";
    std::multiset<Key, Compare, Allocator> tmp = s;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        output(os, *it);
        if (std::next(it) != tmp.end()) os << ", ";
    }
    os << "}";
}

template <class OStream,
          class Key,
          class Hash,
          class KeyEqual,
          class Allocator,
          is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::unordered_set<Key, Hash, KeyEqual, Allocator> &s) {
    os << "{";
    std::unordered_set<Key, Hash, KeyEqual, Allocator> tmp = s;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        output(os, *it);
        if (std::next(it) != tmp.end()) os << ", ";
    }
    os << "}";
}

template <class OStream,
          class Key,
          class Hash,
          class KeyEqual,
          class Allocator,
          is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::unordered_multiset<Key, Hash, KeyEqual, Allocator> &s) {
    os << "{";
    std::unordered_multiset<Key, Hash, KeyEqual, Allocator> tmp = s;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        output(os, *it);
        if (std::next(it) != tmp.end()) os << ", ";
    }
    os << "}";
}

template <class OStream,
          class Key,
          class T,
          class Compare,
          class Allocator,
          is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::map<Key, T, Compare, Allocator> &m) {
    os << "{";
    std::map<Key, T, Compare, Allocator> tmp = m;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        output(os, it->first);
        os << ": ";
        output(os, it->second);
        if (std::next(it) != tmp.end()) os << ", ";
    }
    os << "}";
}

template <class OStream,
          class Key,
          class T,
          class Hash,
          class KeyEqual,
          class Allocator,
          is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const std::unordered_map<Key, T, Hash, KeyEqual, Allocator> &m) {
    os << "{";
    std::unordered_map<Key, T, Hash, KeyEqual, Allocator> tmp = m;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        output(os, it->first);
        os << ": ";
        output(os, it->second);
        if (std::next(it) != tmp.end()) os << ", ";
    }
    os << "}";
}

template <class OStream, class T, class... Args, is_ostream_t<OStream> * = nullptr>
void output(OStream &os, const T &t, const Args &...args) {
    output(os, t);
    os << ' ';
    output(os, args...);
}

template <class OStream, is_ostream_t<OStream> * = nullptr> void outputln(OStream &os) {
    os << '\n';
    os.flush();
}

template <class OStream, class T, class... Args, is_ostream_t<OStream> * = nullptr>
void outputln(OStream &os, const T &t, const Args &...args) {
    output(os, t, args...);
    os << '\n';
    os.flush();
}

std::vector<std::string> sep(const char *s) {
    std::vector<std::string> res;
    std::string now;
    int dep = 0;
    while (true) {
        if (*s == '\0') {
            res.emplace_back(now);
            break;
        }
        if (*s == '(' or *s == '[' or *s == '{') dep++;
        if (*s == ')' or *s == ']' or *s == '}') dep--;
        if (dep == 0 and *s == ',') {
            res.emplace_back(now);
            now.clear();
        } else if (!isspace(*s)) {
            now += *s;
        }
        s++;
    }
    return res;
}

void show_vars(const std::vector<std::string> &, int) {}

template <class T, class... Args>
void show_vars(const std::vector<std::string> &name, int pos, const T &t, const Args &...args) {
    // assert(pos < (int)name.size());
    output(std::cerr, name[pos++] + ":", t);
    if (sizeof...(args) > 0) output(std::cerr, ", ");
    show_vars(name, pos, args...);
}

#define kdebug(...)                                                                                \
    std::cerr << "line:" << __LINE__ << ' ';                                                       \
    kk2::debug::show_vars(kk2::debug::sep(#__VA_ARGS__), 0, __VA_ARGS__);                          \
    std::cerr << std::endl;

#define kput(s)                                                                                    \
    std::cerr << "line:" << __LINE__ << ' ';                                                       \
    kk2::debug::outputln(std::cerr, s);

#else

template <class OStream, class... Args, is_ostream_t<OStream> * = nullptr>
void output(OStream &, const Args &...) {}

template <class OStream, class... Args, is_ostream_t<OStream> * = nullptr>
void outputln(OStream &, const Args &...) {}

template <class... Args> void fix_warn(const Args &...) {}

#define kdebug(...) kk2::debug::fix_warn(__VA_ARGS__);

#define kput(s) kk2::debug::fix_warn(s)

#endif // KK2

} // namespace debug

} // namespace kk2

#endif // KK2_TEMPLATE_DEBUG_HPP
#ifndef KK2_SEGMENT_TREE_LAZY_HPP
#define KK2_SEGMENT_TREE_LAZY_HPP 1


namespace kk2 {

template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct LazySegmentTree {
  public:
    LazySegmentTree() : LazySegmentTree(0) {}

    LazySegmentTree(int n) : _n(n) {
        log = 0;
        while ((1ll << log) < _n) log++;
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        lz = std::vector<F>(size, id());
    }

    template <class... Args> LazySegmentTree(int n, Args... args)
        : LazySegmentTree(std::vector<S>(n, S(args...))) {}

    LazySegmentTree(const std::vector<S> &v) : _n(int(v.size())) {
        log = 0;
        while ((1ll << log) < _n) log++;
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        lz = std::vector<F>(size, id());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        build();
    }

    void build() {
        assert(!is_built);
        is_built = true;
        for (int i = size - 1; i >= 1; i--) { update(i); }
    }

    template <class... Args> void init_set(int p, Args... args) {
        assert(0 <= p && p < _n);
        assert(!is_built);
        d[p + size] = S(args...);
    }

    using Monoid = S;

    static S Op(S l, S r) { return op(l, r); }

    static S MonoidUnit() { return e(); }

    using Hom = F;

    static S Map(F f, S x) { return mapping(f, x); }

    static F Composition(F l, F r) { return composition(l, r); }

    static F HomUnit() { return id(); }

    template <class... Args> void set(int p, Args... args) {
        assert(0 <= p && p < _n);
        assert(is_built);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = S(args...);
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        assert(is_built);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        assert(is_built);
        if (l == r) return e();

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push(r >> i);
        }

        S sml = e(), smr = e();
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return op(sml, smr);
    }

    S all_prod() {
        assert(is_built);
        return d[1];
    }

    template <class... Args> void apply_point(int p, Args... args) {
        assert(0 <= p && p < _n);
        assert(is_built);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = mapping(F(args...), d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    template <class... Args> void apply_range(int l, int r, Args... args) {
        assert(0 <= l && l <= r && r <= _n);
        assert(is_built);
        if (l == r) return;
        F f = F(args...);

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    template <bool (*g)(S)> int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }

    template <class G> int max_right(int l, G g) {
        assert(0 <= l && l <= _n);
        assert(g(e()));
        assert(is_built);
        if (l == _n) return _n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*g)(S)> int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }

    template <class G> int min_left(int r, G g) {
        assert(0 <= r && r <= _n);
        assert(g(e()));
        assert(is_built);
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;
    std::vector<F> lz;
    bool is_built = false;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }

    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        if (k < size) lz[k] = composition(f, lz[k]);
    }

    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }
};

template <class A> using LazySegmentTreeS = LazySegmentTree<typename A::S,
                                                            A::S::op,
                                                            A::S::unit,
                                                            typename A::A,
                                                            A::act,
                                                            A::A::op,
                                                            A::A::unit>;

} // namespace kk2

#endif // KK2_SEGMENT_TREE_LAZY_HPP
#ifndef KK2_MATH_ACTION_AFFINE_SUMWITHSIZE_HPP
#define KK2_MATH_ACTION_AFFINE_SUMWITHSIZE_HPP 1

#ifndef KK2_MATH_GROUP_SUM_WITH_SIZE_HPP
#define KK2_MATH_GROUP_SUM_WITH_SIZE_HPP 1


namespace kk2 {

namespace group {

template <class S, class T = S> struct SumWithSize {
    static constexpr bool commutative = true;
    using M = SumWithSize;
    S a;
    T size;

    SumWithSize() : a(S()), size(0) {}
    SumWithSize(S a_, S size_ = T(1)) : a(a_), size(size_) {}
    operator S() const { return a; }
    inline static M op(M l, M r) { return M(l.a + r.a, l.size + r.size); }
    inline static M inv(M x) { return M(-x.a, -x.size); }
    inline static M unit() { return M(); }
    bool operator==(const M &rhs) const { return a == rhs.a and size == rhs.size; }
    bool operator!=(const M &rhs) const { return a != rhs.a or size != rhs.size; }

    template <class OStream, is_ostream_t<OStream> * = nullptr>
    friend OStream &operator<<(OStream &os, const M &x) {
        return os << x.a << " " << x.size;
    }

    template <class IStream, is_istream_t<IStream> * = nullptr>
    friend IStream &operator>>(IStream &is, M &x) {
        is >> x.a;
        x.size = T(1);
        return is;
    }
};

} // namespace group

} // namespace kk2

#endif // KK2_MATH_GROUP_SUM_WITH_SIZE_HPP
#ifndef KK2_MATH_MONOID_AFFINE_HPP
#define KK2_MATH_MONOID_AFFINE_HPP 1


namespace kk2 {

namespace monoid {

template <class S> struct Affine {
    static constexpr bool commutative = false;
    using M = Affine;
    S a, b; // x \mapsto ax + b

    Affine() : a(S(1)), b(S(0)) {};
    Affine(S a, S b) : a(a), b(b) {}
    inline S eval(S x) const { return a * x + b; }
    // l \circ r
    inline static M op(M l, M r) { return M(l.a * r.a, l.a * r.b + l.b); }
    inline static M unit() { return M(); }
    inline static M inv(M f) { return M(S(1) / f.a, -f.b / f.a); }
    bool operator==(const M &rhs) const { return a == rhs.a and b == rhs.b; }
    bool operator!=(const M &rhs) const { return a != rhs.a or b != rhs.b; }

    template <class OStream, is_ostream_t<OStream> * = nullptr>
    friend OStream &operator<<(OStream &os, const M &x) {
        return os << x.a << " " << x.b;
    }

    template <class IStream, is_istream_t<IStream> * = nullptr>
    friend IStream &operator>>(IStream &is, M &x) {
        return is >> x.a >> x.b;
    }
};

} // namespace monoid

} // namespace kk2

#endif // KK2_MATH_MONOID_AFFINE_HPP

namespace kk2 {

namespace action {

template <class T, class U> struct AffineSumWithSize {
    using A = monoid::Affine<T>;
    using S = group::SumWithSize<T, U>;

    inline static S act(A f, S x) { return S(f.a * x.a + f.b * x.size, x.size); }
};

} // namespace action

} // namespace kk2

#endif // KK2_MATH_ACTION_AFFINE_SUMWITHSIZE_HPP
#ifndef KK2_MODINT_MODINT_HPP
#define KK2_MODINT_MODINT_HPP 1


#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1



namespace kk2 {

#ifndef _MSC_VER

template <typename T> using is_signed_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value
                                  or std::is_same<T, __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_signed =
    typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T> using to_unsigned =
    typename std::conditional<is_signed_int128<T>::value,
                              make_unsigned_int128<T>,
                              typename std::conditional<std::is_signed<T>::value,
                                                        std::make_unsigned<T>,
                                                        std::common_type<T>>::type>::type;

#else

template <typename T> using is_integral = std::enable_if_t<std::is_integral<T>::value>;
template <typename T> using is_signed = std::enable_if_t<std::is_signed<T>::value>;
template <typename T> using is_unsigned = std::enable_if_t<std::is_unsigned<T>::value>;
template <typename T> using to_unsigned = std::make_unsigned<T>;

#endif // _MSC_VER

template <typename T> using is_integral_t = std::enable_if_t<is_integral<T>::value>;
template <typename T> using is_signed_t = std::enable_if_t<is_signed<T>::value>;
template <typename T> using is_unsigned_t = std::enable_if_t<is_unsigned<T>::value>;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP

namespace kk2 {

template <int p> struct ModInt {
    using mint = ModInt;

  public:
    static int Mod;

    constexpr static unsigned int getmod() {
        if (p > 0) return p;
        else return Mod;
    }

    static void setmod(int Mod_) {
        assert(1 <= Mod_);
        Mod = Mod_;
    }

    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }

    constexpr ModInt() : _v(0) {}

    template <class T, is_integral_t<T> * = nullptr> constexpr ModInt(T v) {
        if constexpr (is_signed<T>::value) {
            v = v % (long long)(getmod());
            if (v < 0) v += getmod();
            _v = v;
        } else if constexpr (is_unsigned<T>::value) {
            _v = v %= getmod();
        } else {
            ModInt();
        }
    }

    unsigned int val() const { return _v; }

    mint &operator++() {
        _v++;
        if (_v == getmod()) _v = 0;
        return *this;
    }

    mint &operator--() {
        if (_v == 0) _v = getmod();
        _v--;
        return *this;
    }

    mint operator++(int) {
        mint result = *this;
        ++*this;
        return result;
    }

    mint operator--(int) {
        mint result = *this;
        --*this;
        return result;
    }

    mint &operator+=(const mint &rhs) {
        _v += rhs._v;
        if (_v >= getmod()) _v -= getmod();
        return *this;
    }

    mint &operator-=(const mint &rhs) {
        _v += getmod() - rhs._v;
        if (_v >= getmod()) _v -= getmod();
        return *this;
    }

    mint &operator*=(const mint &rhs) {
        unsigned long long z = _v;
        z *= rhs._v;
        z %= getmod();
        _v = z;
        return *this;
    }

    mint &operator/=(const mint &rhs) { return *this = *this * rhs.inv(); }
    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }
    friend mint operator+(const mint &lhs, const mint &rhs) { return mint(lhs) += rhs; }
    friend mint operator-(const mint &lhs, const mint &rhs) { return mint(lhs) -= rhs; }
    friend mint operator*(const mint &lhs, const mint &rhs) { return mint(lhs) *= rhs; }
    friend mint operator/(const mint &lhs, const mint &rhs) { return mint(lhs) /= rhs; }
    friend bool operator==(const mint &lhs, const mint &rhs) { return lhs._v == rhs._v; }
    friend bool operator!=(const mint &lhs, const mint &rhs) { return lhs._v != rhs._v; }

    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }

    mint inv() const {
        long long s = getmod(), t = _v;
        long long m0 = 0, m1 = 1;

        while (t) {
            long long u = s / t;
            s -= t * u;
            m0 -= m1 * u;

            std::swap(s, t);
            std::swap(m0, m1);
        }
        if (m0 < 0) m0 += getmod() / s;
        return m0;
    }

    template <class OStream, is_ostream_t<OStream> * = nullptr>
    friend OStream &operator<<(OStream &os, const mint &mint_) {
        os << mint_._v;
        return os;
    }

    template <class IStream, is_istream_t<IStream> * = nullptr>
    friend IStream &operator>>(IStream &is, mint &mint_) {
        long long x;
        is >> x;
        mint_ = mint(x);
        return is;
    }

  private:
    unsigned int _v;
};

template <int p> int ModInt<p>::Mod = 998244353;

using mint998 = ModInt<998244353>;
using mint107 = ModInt<1000000007>;

} // namespace kk2

#endif // KK2_MODINT_MODINT_HPP
#ifndef KK2_OTHERS_COORDINATE_COMPRESSION_HPP
#define KK2_OTHERS_COORDINATE_COMPRESSION_HPP 1


namespace kk2 {

// Coordinate Compression
template <typename S = int> struct CC {
    std::vector<S> xs;
    bool initialized;

    CC() : initialized(false) {}

    CC(const std::vector<S> &xs_) : xs(xs_), initialized(false) {}

    void add(S x) {
        xs.push_back(x);
        initialized = false;
    }

    void add(const std::vector<S> &ys) {
        std::copy(std::begin(ys), std::end(ys), std::back_inserter(xs));
        initialized = false;
    }

    void build() {
        std::sort(std::begin(xs), std::end(xs));
        xs.erase(std::unique(std::begin(xs), std::end(xs)), std::end(xs));
        initialized = true;
    }

    S operator[](int i) {
        if (!initialized) build();
        return xs[i];
    }

    int size() {
        if (!initialized) build();
        return xs.size();
    }

    int get(S x) {
        if (!initialized) build();
        return std::upper_bound(std::begin(xs), std::end(xs), x) - std::begin(xs) - 1;
    }

    std::vector<int> get(const std::vector<S> &ys) {
        std::vector<int> ret(ys.size());
        for (int i = 0; i < (int)ys.size(); ++i) ret[i] = get(ys[i]);
        return ret;
    }

    int operator()(S x) { return get(x); }

    std::vector<int> operator()(const std::vector<S> &ys) { return get(ys); }

    int lower(S x) {
        if (!initialized) build();
        return std::lower_bound(std::begin(xs), std::end(xs), x) - std::begin(xs);
    }

    int upper(S x) {
        if (!initialized) build();
        return std::upper_bound(std::begin(xs), std::end(xs), x) - std::begin(xs);
    }

    bool exist(S x) {
        if (!initialized) build();
        int idx = lower(x);
        return idx < (int)xs.size() && xs[idx] == x;
    }
};

} // namespace kk2

#endif // KK2_OTHERS_COORDINATE_COMPRESSION_HPP
using namespace std;

void solve() {
    using mint = kk2::mint998;
    using A = kk2::action::AffineSumWithSize<mint, mint>;
    int n, q;
    kin >> n >> q;
    vc<array<int, 5>> queries(q);
    kk2::CC<int> cc;
    rep (i, q) {
        auto &x = queries[i];
        kin >> x[0];
        if (x[0] == 0) {
            kin >> x[1] >> x[2] >> x[3] >> x[4];
            cc.add(x[1]), cc.add(x[2]);
        } else if (x[0] == 1) {
            kin >> x[1] >> x[2];
            cc.add(x[1]), cc.add(x[2]);
        }
    }
    cc.build();
    rep (i, q) {
        auto &x = queries[i];
        if (x[0] == 0) {
            x[1] = cc.get(x[1]);
            x[2] = cc.get(x[2]);
        } else if (x[0] == 1) {
            x[1] = cc.get(x[1]);
            x[2] = cc.get(x[2]);
        }
    }
    kk2::LazySegmentTreeS<A> seg(cc.size());
    rep (i, cc.size() - 1) seg.init_set(i, 0, cc[i + 1] - cc[i]);
    seg.build();

    for (auto query : queries) {
        if (query[0] == 0) {
            auto [_, l, r, a, b] = query;
            seg.apply_range(l, r, a, b);
        } else {
            auto [_0, l, r, _1, _2] = query;
            kout << seg.prod(l, r).a << "\n";
        }
    }
}

int main() {
#ifdef KK2
    int t = 1;
#else
    int t = 1;
#endif
    // kin >> t;
    rep (t) solve();

    return 0;
}
// Author: kk2
// converted by https://github.com/kk2a/cpp-bundle
// 2025-07-25 20:45:55
