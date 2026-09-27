// clang-format off
// verification-helper: PROBLEM https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum
// clang-format on

#include <cassert>
#include <utility>
#include <vector>


#include <algorithm>
#include <concepts>
#include <functional>
#include <limits>

namespace yosupo {

template <class T>
concept monoid = requires(T& x, typename T::S s) {
    { x.op(s, s) } -> std::same_as<typename T::S>;
    { x.e() } -> std::same_as<typename T::S>;
};

template <class _S, class OP> struct Monoid {
    using S = _S;
    OP op;
    S e() { return _e; }
    Monoid(S e, OP _op = OP()) : op(_op), _e(e) {}

  private:
    S _e;
};

template <monoid Monoid> struct ReversibleMonoid {
    ReversibleMonoid(const Monoid& _monoid) : monoid(_monoid) {}
    struct S {
        typename Monoid::S val;
        typename Monoid::S rev;
    };
    S op(S a, S b) {
        return {monoid.op(a.val, b.val), monoid.op(b.rev, a.rev)};
    }
    S e() { return {monoid.e(), monoid.e()}; }

  private:
    Monoid monoid;
};

template <class _S> struct Max {
    using S = _S;
    Max(S e = std::numeric_limits<S>::min()) : _e(e) {}
    S op(S a, S b) { return std::max(a, b); }
    S e() { return _e; }

  private:
    S _e;
};
template <class _S> struct Min {
    using S = _S;
    Min(S e = std::numeric_limits<S>::max()) : _e(e) {}
    S op(S a, S b) { return std::min(a, b); }
    S e() { return _e; }

  private:
    S _e;
};

template <class S> using Sum = Monoid<S, std::plus<S>>;
template <class S> using Prod = Monoid<S, std::multiplies<S>>;

template <class T>
concept static_top_tree_dp = requires(T t) {
    requires monoid<decltype(T::path)>;
    requires monoid<decltype(T::point)>;
    {
        t.add_vertex(std::declval<typename decltype(T::point)::S>(),
                     std::declval<int>())
    } -> std::same_as<typename decltype(T::path)::S>;
    {
        t.add_edge(std::declval<typename decltype(T::path)::S>())
    } -> std::same_as<typename decltype(T::point)::S>;
};

}  // namespace yosupo

#include <stdio.h>
#include <unistd.h>
#include <array>
#include <bit>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>
#include <type_traits>



namespace yosupo {

namespace internal {

template <class T>
using is_signed_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value ||
                                  std::is_same<T, __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value ||
                                  std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value,
                              __uint128_t,
                              unsigned __int128>;

template <class T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value ||
                                  internal::is_signed_int128<T>::value ||
                                  internal::is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                                                 std::is_signed<T>::value) ||
                                                    is_signed_int128<T>::value,
                                                std::true_type,
                                                std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<(is_integral<T>::value &&
                               std::is_unsigned<T>::value) ||
                                  is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<
    is_signed_int128<T>::value,
    make_unsigned_int128<T>,
    typename std::conditional<std::is_signed<T>::value,
                              std::make_unsigned<T>,
                              std::common_type<T>>::type>::type;

template <class T>
using is_integral_t = std::enable_if_t<is_integral<T>::value>;

template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

}  // namespace yosupo

namespace yosupo {

struct Scanner {
  public:
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;

    Scanner(FILE* fp) : fd(fileno(fp)) { line[0] = 127; }

    void read() {}
    template <class H, class... T> void read(H& h, T&... t) {
        bool f = read_single(h);
        assert(f);
        read(t...);
    }

    int read_unsafe() { return 0; }
    template <class H, class... T> int read_unsafe(H& h, T&... t) {
        bool f = read_single(h);
        if (!f) return 0;
        return 1 + read_unsafe(t...);
    }

    int close() { return ::close(fd); }

  private:
    static constexpr int SIZE = 1 << 15;

    int fd = -1;
    std::array<char, SIZE + 1> line;
    int st = 0, ed = 0;
    bool eof = false;

    bool read_single(std::string& ref) {
        if (!skip_space()) return false;
        ref = "";
        while (true) {
            char c = top();
            if (c <= ' ') break;
            ref += c;
            st++;
        }
        return true;
    }
    bool read_single(double& ref) {
        std::string s;
        if (!read_single(s)) return false;
        ref = std::stod(s);
        return true;
    }

    template <class T,
              std::enable_if_t<std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& ref) {
        if (!skip_space<50>()) return false;
        ref = top();
        st++;
        return true;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& sref) {
        using U = internal::to_unsigned_t<T>;
        if (!skip_space<50>()) return false;
        bool neg = false;
        if (line[st] == '-') {
            neg = true;
            st++;
        }
        U ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        sref = neg ? -ref : ref;
        return true;
    }
    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<!std::is_same<U, char>::value>* = nullptr>
    bool read_single(U& ref) {
        if (!skip_space<50>()) return false;
        ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        return true;
    }

    bool reread() {
        if (ed - st >= 50) return true;
        if (st > SIZE / 2) {
            std::memmove(line.data(), line.data() + st, ed - st);
            ed -= st;
            st = 0;
        }
        if (eof) return false;
        auto u = ::read(fd, line.data() + ed, SIZE - ed);
        if (u == 0) {
            eof = true;
            line[ed] = '\0';
            u = 1;
        }
        ed += int(u);
        line[ed] = char(127);
        return true;
    }

    char top() {
        if (st == ed) {
            bool f = reread();
            assert(f);
        }
        return line[st];
    }

    template <int TOKEN_LEN = 0> bool skip_space() {
        while (true) {
            while (line[st] <= ' ') st++;
            if (ed - st > TOKEN_LEN) return true;
            if (st > ed) st = ed;
            for (auto i = st; i < ed; i++) {
                if (line[i] <= ' ') return true;
            }
            if (!reread()) return false;
        }
    }
};

struct Printer {
  public:
    template <char sep = ' ', bool F = false> void write() {}
    template <char sep = ' ', bool F = false, class H, class... T>
    void write(const H& h, const T&... t) {
        if (F) write_single(sep);
        write_single(h);
        write<true>(t...);
    }
    template <char sep = ' ', class... T> void writeln(const T&... t) {
        write<sep>(t...);
        write_single('\n');
    }

    Printer(FILE* _fp) : fd(fileno(_fp)) {}
    ~Printer() { flush(); }

    int close() {
        flush();
        return ::close(fd);
    }

    void flush() {
        if (pos) {
            auto res = ::write(fd, line.data(), pos);
            assert(res != -1);
            pos = 0;
        }
    }

  private:
    static std::array<std::array<char, 2>, 100> small;
    static std::array<unsigned long long, 20> tens;

    static constexpr size_t SIZE = 1 << 15;
    int fd;
    std::array<char, SIZE> line;
    size_t pos = 0;
    std::stringstream ss;

    template <class T,
              std::enable_if_t<std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        if (pos == SIZE) flush();
        line[pos++] = val;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        using U = internal::to_unsigned_t<T>;
        if (val == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();
        U uval = val;
        if (val < 0) {
            write_single('-');
            uval = -uval;
        }
        write_unsigned(uval);
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<!std::is_same<char, U>::value>* = nullptr>
    void write_single(U uval) {
        if (uval == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();

        write_unsigned(uval);
    }

    static int calc_len(uint64_t x) {
        int i = ((63 - std::countl_zero(x)) * 3 + 3) / 10;
        if (x < tens[i])
            return i;
        else
            return i + 1;
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<2 >= sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<4 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        std::array<char, 8> buf;
        memcpy(buf.data() + 6, small[uval % 100].data(), 2);
        memcpy(buf.data() + 4, small[uval / 100 % 100].data(), 2);
        memcpy(buf.data() + 2, small[uval / 10000 % 100].data(), 2);
        memcpy(buf.data() + 0, small[uval / 1000000 % 100].data(), 2);

        if (uval >= 100000000) {
            if (uval >= 1000000000) {
                memcpy(line.data() + pos, small[uval / 100000000 % 100].data(),
                       2);
                pos += 2;
            } else {
                line[pos] = char('0' + uval / 100000000);
                pos++;
            }
            memcpy(line.data() + pos, buf.data(), 8);
            pos += 8;
        } else {
            size_t len = calc_len(uval);
            memcpy(line.data() + pos, buf.data() + (8 - len), len);
            pos += len;
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<8 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <
        class U,
        std::enable_if_t<internal::is_unsigned_int128<U>::value>* = nullptr>
    void write_unsigned(U uval) {
        static std::array<char, 50> buf;
        size_t len = 0;
        while (uval > 0) {
            buf[len++] = char((uval % 10) + '0');
            uval /= 10;
        }
        std::reverse(buf.begin(), buf.begin() + len);
        memcpy(line.data() + pos, buf.data(), len);
        pos += len;
    }

    void write_single(const std::string& s) {
        for (char c : s) write_single(c);
    }
    void write_single(const char* s) {
        size_t len = strlen(s);
        for (size_t i = 0; i < len; i++) write_single(s[i]);
    }
    template <class T> void write_single(const std::vector<T>& val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) {
            if (i) write_single(' ');
            write_single(val[i]);
        }
    }
};

inline std::array<std::array<char, 2>, 100> Printer::small = [] {
    std::array<std::array<char, 2>, 100> table;
    for (int i = 0; i <= 99; i++) {
        table[i][1] = char('0' + (i % 10));
        table[i][0] = char('0' + (i / 10 % 10));
    }
    return table;
}();
inline std::array<unsigned long long, 20> Printer::tens = [] {
    std::array<unsigned long long, 20> table;
    for (int i = 0; i < 20; i++) {
        table[i] = 1;
        for (int j = 0; j < i; j++) {
            table[i] *= 10;
        }
    }
    return table;
}();

}  // namespace yosupo

#include <ranges>

namespace yosupo {

template <class T> struct FlattenVector {
    std::vector<T> v;
    std::vector<int> start;
    FlattenVector(int n, const std::vector<std::pair<int, T>>& _v)
        : start(n + 1) {
        for (const auto& x : _v) {
            start[x.first + 1]++;
        }
        for (int i = 1; i <= n; i++) {
            start[i] += start[i - 1];
        }
        v = std::vector<T>(start[n]);

        auto pos = start;
        for (const auto& x : _v) {
            v[pos[x.first]] = x.second;
            pos[x.first]++;
        }
    }

    auto at(int i) {
        return v | std::ranges::views::take(start[i + 1]) |
               std::ranges::views::drop(start[i]);
    }
};

}  // namespace yosupo

#include <cstdlib>


#include <cmath>
#include <initializer_list>
#include <numeric>



namespace yosupo {
using std::countr_zero;

inline int countr_zero(unsigned __int128 x) {
    auto lo = (unsigned long long)(x);
    auto hi = (unsigned long long)(x >> 64);
    return lo ? std::countr_zero(lo) : 64 + std::countr_zero(hi);
}

template <class T>
    requires requires(T x) {
        { x.countr_zero() } -> std::same_as<int>;
    }
int countr_zero(T x) {
    return x.countr_zero();
}

}  // namespace yosupo


namespace yosupo {

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using i128 = __int128;
using u128 = unsigned __int128;

}  // namespace yosupo

namespace yosupo {

// sign
template <class T>
    requires std::is_integral_v<T>
int sgn(T x) {
    if (x == 0) return 0;
    return x > 0 ? 1 : -1;
}
inline int sgn(__int128 x) {
    if (x == 0) return 0;
    return x > 0 ? 1 : -1;
}
// for custom class
template <class T>
    requires requires(T x) {
        { x.sgn() } -> std::same_as<int>;
    }
int sgn(T x) {
    return x.sgn();
}

// abs
inline i128 abs(i128 x) { return x < 0 ? -x : x; }
template <class T>
    requires requires(T x) {
        { x.abs() } -> std::same_as<T>;
    }
T abs(T x) {
    return x.abs();
}

// gcd
using std::gcd;
inline u128 gcd(u128 a, u128 b) {
    if (a == 0) return b;
    if (b == 0) return a;
    int shift;
    {
        int a_bsf = countr_zero(a);
        a >>= a_bsf;
        int b_bsf = countr_zero(b);
        b >>= b_bsf;
        shift = std::min(a_bsf, b_bsf);
    }
    while (a != b) {
        if (a > b) std::swap(a, b);
        b -= a;
        b >>= countr_zero(b);
    }
    return (a << shift);
}
inline i128 gcd(i128 a, i128 b) { return gcd((u128)abs(a), (u128)abs(b)); }
template <class T>
    requires requires(T x) {
        { T::gcd(x, x) } -> std::same_as<T>;
    }
T gcd(T x, T y) {
    return T::gcd(x, y);
}

template <class T> T floor_div(T x, T y) {
    auto d = x / y;
    auto r = x % y;
    if (r == 0) return d;
    if ((r > 0) == (y > 0)) return d;
    return d - 1;
}
template <class T> T ceil_div(T x, T y) {
    auto d = x / y;
    auto r = x % y;
    if (r == 0) return d;
    if ((r > 0) == (y > 0)) return d + 1;
    return d;
}

// x * inv_u32(x) = 1 (mod 2^32)
inline constexpr u32 inv_u32(const u32 x) {
    u32 inv = 1;
    for (int i = 0; i < 5; i++) {
        inv *= 2u - inv * x;
    }
    return inv;
}

inline constexpr u64 pow_mod_u64(u64 x, u64 n, u64 m) {
    if (m == 1) return 0;
    u64 r = 1;
    u64 y = x % m;
    while (n) {
        if (n & 1) r = (u64)((u128)(1) * r * y % m);
        y = (u64)((u128)(1) * y * y % m);
        n >>= 1;
    }
    return r;
}

inline constexpr u32 smallest_primitive_root(u32 m) {
    if (m == 2) return 1;

    u32 divs[20] = {};
    int cnt = 0;
    u32 x = (m - 1) / 2;
    for (int i = 2; (u64)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x > 1) {
        divs[cnt++] = x;
    }
    for (u32 g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod_u64(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

inline bool is_prime(u32 n) {
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    u64 d = n - 1;
    while (d % 2 == 0) d /= 2;
    for (u64 a : {2, 7, 61}) {
        if (a % n == 0) return true;
        u64 t = d;
        u64 y = pow_mod_u64(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = (u64)((u128)(1) * y * y % n);
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
inline bool is_prime(u64 n) {
    if (n <= std::numeric_limits<u32>::max()) {
        return is_prime((u32)n);
    }
    if (n % 2 == 0) return false;
    u64 d = n - 1;
    while (d % 2 == 0) d /= 2;
    for (u64 a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (a % n == 0) return true;
        u64 t = d;
        u64 y = pow_mod_u64(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = (u64)((u128)(1) * y * y % n);
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
inline bool is_prime(i32 n) {
    if (n <= 1) return false;
    return is_prime((u32)n);
}
inline bool is_prime(i64 n) {
    if (n <= 1) return false;
    return is_prime((u64)n);
}

inline i64 pollard_single(i64 n) {
    if (is_prime(n)) return n;
    if (n % 2 == 0) return 2;
    long long st = 0;
    auto f = [&](i64 x) { return (i64)(((u128)(x)*x + st) % n); };
    while (true) {
        st++;
        i64 x = st, y = f(x);
        while (true) {
            i64 p = gcd((y - x + n), n);
            if (p == 0 || p == n) break;
            if (p != 1) return p;
            x = f(x);
            y = f(f(y));
        }
    }
}

inline std::vector<i64> factor(i64 n) {
    if (n == 1) return {};
    i64 x = pollard_single(n);
    if (x == n) return {x};
    auto f0 = factor(x), f1 = factor(n / x);
    f0.insert(f0.end(), f1.begin(), f1.end());
    return f0;
}

}  // namespace yosupo

namespace yosupo {

template <i32 MOD> struct ModInt {
    static_assert(MOD % 2 && 1 <= MOD && MOD <= (1 << 30) - 1,
                  "mod must be odd and at most 2^30 - 1");

    static constexpr i32 mod() { return MOD; }

    constexpr ModInt() : x(0) {}

    // It's OK because _x * B2 < 2**32 * MOD
    constexpr ModInt(u32 _x) : x(reduce_mul(_x, B2)) {}

    constexpr ModInt(std::signed_integral auto _x)
        : ModInt((u32)(_x % MOD + MOD)) {}
    constexpr ModInt(std::unsigned_integral auto _x)
        : ModInt((u32)(_x % MOD)) {}

    constexpr i32 val() const {
        u32 y = reduce_mul(x, 1);
        return y < MOD ? y : y - MOD;
    }

    constexpr ModInt operator+() const { return *this; }
    constexpr ModInt operator-() const { return ModInt() -= *this; }

    constexpr ModInt& operator+=(const ModInt& rhs) {
        x += rhs.x;
        x = std::min(x, x - 2 * MOD);
        return *this;
    }
    constexpr friend ModInt operator+(const ModInt& lhs, const ModInt& rhs) {
        return ModInt(lhs) += rhs;
    }

    constexpr ModInt& operator-=(const ModInt& rhs) {
        x += 2 * MOD - rhs.x;
        x = std::min(x, x - 2 * MOD);
        return *this;
    }
    constexpr friend ModInt operator-(const ModInt& lhs, const ModInt& rhs) {
        return ModInt(lhs) -= rhs;
    }

    constexpr ModInt& operator*=(const ModInt& rhs) {
        x = reduce_mul(x, rhs.x);
        return *this;
    }
    constexpr friend ModInt operator*(const ModInt& lhs, const ModInt& rhs) {
        return ModInt(lhs) *= rhs;
    }

    constexpr ModInt& operator/=(const ModInt& rhs) {
        return *this *= rhs.inv();
    }
    constexpr friend ModInt operator/(const ModInt& lhs, const ModInt& rhs) {
        return ModInt(lhs) /= rhs;
    }

    friend bool operator==(const ModInt& lhs, const ModInt& rhs) {
        return std::min(lhs.x, lhs.x - MOD) == std::min(rhs.x, rhs.x - MOD);
    }

    constexpr ModInt pow(u64 n) const {
        ModInt v = *this, r = 1;
        while (n) {
            if (n & 1) r *= v;
            v *= v;
            n >>= 1;
        }
        return r;
    }
    constexpr ModInt inv() const {
        // TODO: for non-prime
        return pow(MOD - 2);
    }

    std::string dump() const { return std::to_string(val()); }

  private:
    u32 x;  // [0, 2 * MOD)

    static constexpr u32 B = ((u64(1) << 32)) % MOD;
    static constexpr u32 B2 = u64(1) * B * B % MOD;
    static constexpr u32 INV = -inv_u32(MOD);

    // Input: (l * r) must be no more than (2^32 * MOD)
    // Output: ((l * r) / 2^32) % MOD
    static constexpr u32 reduce_mul(u32 l, u32 r) {
        u64 x = u64(1) * l * r;
        x += u64(u32(x) * INV) * MOD;
        return u32(x >> 32);
    }
};

using ModInt998244353 = ModInt<998244353>;
using ModInt1000000007 = ModInt<1000000007>;

}  // namespace yosupo

#include <bitset>


namespace yosupo {

template <static_top_tree_dp TreeDP> struct StaticTopTree {
    using Point = typename decltype(TreeDP::point)::S;
    using Path = typename decltype(TreeDP::path)::S;

    int n;
    std::vector<std::pair<int, int>> edges;
    TreeDP& dp;

    StaticTopTree(int _n, TreeDP& _dp)
        : n(_n),
          dp(_dp),
          points(n + 1, dp.point.e()),
          node_ids(n, {n, -1, -1}) {
        edges.reserve(2 * (n - 1));
    }

    void add_edge(int u, int v) {
        edges.push_back({u, v});
        edges.push_back({v, u});
    }

    // compress / rake
    template <class D> struct Inner {
        std::pair<D, D> d;
        int par;
    };
    std::vector<Inner<Path>> compressed;
    std::vector<Inner<Point>> raked;

    // compress / rake / leaf
    template <class D> struct Node {
        D p;
        int* par;
    };
    template <bool RAKE, class D>
    Node<D> merge(const Node<D>& l,
                  const Node<D>& r,
                  std::vector<Inner<D>>& nodes) {
        int id = int(nodes.size());
        *l.par = 2 * id;
        *r.par = 2 * id + 1;
        nodes.push_back({{l.p, r.p}, -1});
        if constexpr (RAKE) {
            return {dp.point.op(l.p, r.p), &nodes[id].par};
        } else {
            return {dp.path.op(l.p, r.p), &nodes[id].par};
        }
    }

    void init(int r = 0) {
        FlattenVector tree(n, edges);
        std::vector<int> topo;
        {
            std::vector<int> parent(n, -1);
            topo.reserve(n);
            topo.push_back(r);
            for (int i = 0; i < n; i++) {
                int u = topo[i];
                for (int v : tree.at(u)) {
                    if (v == parent[u]) continue;
                    parent[v] = u;
                    topo.push_back(v);
                }
            }
            edges.erase(remove_if(edges.begin(), edges.end(),
                                  [&](auto edge) {
                                      return parent[edge.second] != edge.first;
                                  }),
                        edges.end());
            tree = FlattenVector(n, edges);
        }

        std::vector<int> heavy_child(n, -1);
        std::vector<u64> mask(n);

        std::vector<std::pair<int, Node<Path>>> b;
        b.reserve(n);
        auto build_compress = [&](int u) {
            b.clear();
            auto merge_last = [&]() {
                auto y = b.back();
                b.pop_back();
                auto x = b.back();
                b.pop_back();
                b.push_back({std::max(x.first, y.first) + 1,
                             merge<false>(x.second, y.second, compressed)});
            };
            while (u != -1) {
                b.push_back({std::countr_zero(std::bit_ceil(mask[u])),
                             {dp.add_vertex(points[u], u), &node_ids[u].c_id}});
                while (true) {
                    int len = int(b.size());
                    if (len >= 3 && (b[len - 3].first == b[len - 2].first ||
                                     b[len - 3].first <= b[len - 1].first)) {
                        auto last = b.back();
                        b.pop_back();
                        merge_last();
                        b.push_back(last);
                    } else if (len >= 2 &&
                               b[len - 2].first <= b[len - 1].first) {
                        merge_last();
                    } else {
                        break;
                    }
                }
                u = heavy_child[u];
            }
            while (b.size() != 1) {
                merge_last();
            }
            return b.back().second.p;
        };

        compressed.reserve(n - 1);
        raked.reserve(n - 1);

        const int MAX_H = 128;
        std::array<Node<Point>, MAX_H> q;
        std::bitset<MAX_H> has = {};
        for (int u : topo | std::ranges::views::reverse) {
            u64 sum_rake = 0;
            for (int v : tree.at(u)) {
                sum_rake += std::bit_ceil(mask[v]) << 1;
            }
            mask[u] = std::bit_ceil(sum_rake);
            for (int v : tree.at(u)) {
                int d = std::countr_zero(
                    std::bit_ceil(sum_rake - (std::bit_ceil(mask[v]) << 1)));
                u64 s =
                    ((mask[v] + (u64(1) << d) - 1) >> d << d) + (u64(1) << d);
                if (s <= mask[u]) {
                    mask[u] = s;
                    heavy_child[u] = v;
                }
            }

            for (int v : tree.at(u)) {
                if (v == heavy_child[u]) continue;
                int d = std::countr_zero(
                    std::bit_ceil(sum_rake - (std::bit_ceil(mask[v]) << 1)));
                Point point = dp.add_edge(build_compress(v));
                Node<Point> data = {point, &node_ids[v].r_id};
                while (has.test(d)) {
                    has.reset(d);
                    data = merge<true>(q[d], data, raked);
                    d++;
                }
                q[d] = data;
                has.set(d);
            }
            int d = int(has._Find_first());
            if (d == int(has.size())) continue;

            Node data = q[d];
            has.reset(d);
            while (true) {
                d = int(has._Find_first());
                if (d == int(has.size())) break;
                has.reset(d);
                data = merge<true>(q[d], data, raked);
            }
            points[u] = data.p;

            for (int v0 : tree.at(u)) {
                if (v0 == heavy_child[u]) continue;
                int v = v0;
                while (v != -1) {
                    node_ids[v].h_par = u;
                    node_ids[v].r_id = node_ids[v0].r_id;
                    v = heavy_child[v];
                }
            }
        }
        build_compress(r);
    }

    std::vector<Point> points;
    struct ID {
        int h_par, c_id, r_id;
    };
    std::vector<ID> node_ids;

    Point update(int u) {
        auto up_compress = [&](int id, auto p) {
            while (id >= 0) {
                if (id % 2 == 0) {
                    compressed[id / 2].d.first = p;
                } else {
                    compressed[id / 2].d.second = p;
                }
                p = dp.path.op(compressed[id / 2].d.first, compressed[id / 2].d.second);
                id = compressed[id / 2].par;
            }
            return p;
        };
        auto up_rake = [&](int id, auto p) {
            while (id >= 0) {
                if (id % 2 == 0) {
                    raked[id / 2].d.first = p;
                } else {
                    raked[id / 2].d.second = p;
                }
                p = dp.point.op(raked[id / 2].d.first, raked[id / 2].d.second);
                id = raked[id / 2].par;
            }
            return p;
        };
        while (u != n) {
            auto [h_par, c_id, r_id] = node_ids[u];
            Point p =
                dp.add_edge(up_compress(c_id, dp.add_vertex(points[u], u)));
            points[h_par] = up_rake(r_id, p);
            u = h_par;
        }
        return points[n];
    }

    Path path_prod(int u) {
        Path path = dp.path.e();
        Point point = points[u];
        while (true) {
            auto [h_par, c_id, r_id] = node_ids[u];
            Path l = dp.path.e(), r = dp.path.e();
            {
                int id = c_id;
                while (id >= 0) {
                    if (id % 2 == 0) {
                        r = dp.path.op(r, compressed[id / 2].d.second);
                    } else {
                        l = dp.path.op(compressed[id / 2].d.first, l);
                    }
                    id = compressed[id / 2].par;
                }
            }
            point = dp.point.op(point, dp.add_edge(r));
            path = dp.path.op(l, dp.path.op(dp.add_vertex(point, u), path));
            if (h_par == n) return path;
            point = dp.point.e();
            {
                int id = r_id;
                while (id >= 0) {
                    if (id % 2 == 0) {
                        point = dp.point.op(point, raked[id / 2].d.second);
                    } else {
                        point = dp.point.op(raked[id / 2].d.first, point);
                    }
                    id = raked[id / 2].par;
                }
            }
            u = h_par;
        }
    }
};

}  // namespace yosupo

using mint = yosupo::ModInt998244353;

inline static yosupo::Scanner sc(stdin);
inline static yosupo::Printer pr(stdout);

struct Path {
    mint a, b, ans, cnt;  // f(x) = ax + b
};
struct PathMonoid {
    using S = Path;
    Path op(Path x, Path y) {
        return Path(x.a * y.a, x.b + x.a * y.b,
                    x.ans + x.a * y.ans + x.b * y.cnt, x.cnt + y.cnt);
    }
    Path e() { return Path(mint(1), mint(0), mint(0), mint(0)); }
};

struct TreeDP {
    struct Point {
        mint ans, cnt;
    };
    struct PointMonoid {
        using S = Point;
        Point op(Point x, Point y) { return Point(x.ans + y.ans, x.cnt + y.cnt); }
        Point e() { return Point(mint(0), mint(0)); }
    };
    PointMonoid point = PointMonoid();    

    yosupo::ReversibleMonoid<PathMonoid> path =
        yosupo::ReversibleMonoid(PathMonoid());
    using Path = typename decltype(path)::S;

    struct Vertex {
        mint a, b, val; //, cnt;
    };
    std::vector<Vertex> f;
    Path add_vertex(Point x, int u) {
        x.ans += f[u].val;
        //x.cnt += f[u].cnt;
        //auto p = PathMonoid::S(f[u].a, f[u].b, f[u].a * x.ans + f[u].b * x.cnt, x.cnt);
        //return {p, p};

        x.cnt += mint(1);
        auto up = PathMonoid::S(f[u].a, f[u].b, f[u].a * x.ans + f[u].b * x.cnt,
                               x.cnt);
        auto down = PathMonoid::S(f[u].a, f[u].b, x.ans,
                                x.cnt);
        return {up, down};
    }
    Point add_edge(Path x) { return Point(x.val.ans, x.val.cnt); }
};

// int main() {
//     int n, q;
//     sc.read(n, q);

//     TreeDP dp;
//     dp.f = std::vector<TreeDP::Vertex>(n + n - 1);
//     for (int u = 0; u < n; u++) {
//         int x;
//         sc.read(x);
//         dp.f[u].a = mint(1);
//         dp.f[u].b = mint(0);
//         dp.f[u].val = x;
//         dp.f[u].cnt = mint(1);
//     }

//     yosupo::StaticTopTree<TreeDP> tr(n + n - 1, dp);
//     struct E {
//         int to;
//         mint a, b;
//     };
//     std::vector<std::pair<int, E>> edges;
//     for (int i = 0; i < n - 1; i++) {
//         int u, v;
//         sc.read(u, v);
//         int a, b;
//         sc.read(a, b);
//         tr.add_edge(u, n + i);
//         tr.add_edge(n + i, v);
//         edges.push_back({u, {v, a, b}});
//         edges.push_back({v, {u, a, b}});

//         dp.f[n + i].a = a;
//         dp.f[n + i].b = b;
//         dp.f[n + i].val = 0;
//         dp.f[n + i].cnt = 0;
//     }
//     tr.init();

//     struct Query {
//         int t;
//         int u;
//         mint a, b;
//     };
//     std::vector<Query> queries;
//     for (int ph = 0; ph < q; ph++) {
//         int t;
//         sc.read(t);

//         if (t == 0) {
//             int u, x;
//             sc.read(u, x);
//             dp.f[u].val = x;
//             tr.update(u);
//         } else {
//             int e, a, b;
//             sc.read(e, a, b);
//             dp.f[n + e].a = a;
//             dp.f[n + e].b = b;
//             tr.update(n + e);
//         }
//         int r;
//         sc.read(r);
//         auto ans = tr.path_prod(r);
//         pr.writeln(ans.rev.ans.val());
//     }

//     return 0;
// }

int main() {
    int n, q;
    sc.read(n, q);

    TreeDP dp;
    dp.f = std::vector<TreeDP::Vertex>(n);
    for (int u = 0; u < n; u++) {
        int x;
        sc.read(x);
        dp.f[u].val = x;
    }

    yosupo::StaticTopTree<TreeDP> tr(n, dp);
    struct E {
        int to;
        mint a, b;
    };
    std::vector<std::pair<int, E>> edges;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        sc.read(u, v);
        int a, b;
        sc.read(a, b);
        tr.add_edge(u, v);
        edges.push_back({u, {v, a, b}});
        edges.push_back({v, {u, a, b}});
    }
    std::vector<int> par(n, -1);
    {
        auto tree = yosupo::FlattenVector(n, edges);
        std::vector<int> topo;
        topo.reserve(n);
        topo.push_back(0);
        dp.f[0].a = mint(1);
        for (int i = 0; i < n; i++) {
            int u = topo[i];
            for (auto e : tree.at(u)) {
                int v = e.to;
                if (v == par[u]) continue;
                par[v] = u;
                dp.f[v].a = e.a;
                dp.f[v].b = e.b;
                topo.push_back(v);
            }
        }
    }
    tr.init();

    struct Query {
        int t;
        int u;
        mint a, b;
    };
    std::vector<Query> queries;
    for (int ph = 0; ph < q; ph++) {
        int t;
        sc.read(t);

        if (t == 0) {
            int u, x;
            sc.read(u, x);
            dp.f[u].val = x;
            tr.update(u);
        } else {
            int e, a, b;
            sc.read(e, a, b);
            int u = edges[2 * e].first;
            int v = edges[2 * e].second.to;
            if (par[v] == u) std::swap(u, v);
            assert(par[u] == v);
            dp.f[u].a = a;
            dp.f[u].b = b;
            tr.update(u);
        }

        int r;
        sc.read(r);
        auto ans = tr.path_prod(r);
        pr.writeln(ans.rev.ans.val());
    }

    return 0;
}
