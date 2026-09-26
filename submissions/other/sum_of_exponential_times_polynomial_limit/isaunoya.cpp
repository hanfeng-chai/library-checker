#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

/// @complexity Time: O(d log log(d + 2) + log n). Space: O(d).

/// @complexity Time: O(n) per evaluation at consecutive points.
/// Space: O(n).

namespace noya {

/// @brief Evaluate the degree < values.size() polynomial known at consecutive
/// points 0,1,... in O(n) over a field.
template <class T, class Integer>
T lagrange_consecutive(const std::vector<T> &values, Integer x) {
  static_assert(std::is_integral_v<Integer>);
  assert(!values.empty());
  if constexpr (std::is_signed_v<Integer>) {
    if (x >= 0 && std::uint64_t(x) < values.size()) {
      return values[std::size_t(x)];
    }
  } else if (x < values.size()) {
    return values[std::size_t(x)];
  }
  int n = int(values.size());
  T point = T(x);
  std::vector<T> prefix(n + 1, T(1));
  std::vector<T> suffix(n + 1, T(1));
  for (int index = 0; index < n; index++) {
    prefix[index + 1] = prefix[index] * (point - T(index));
  }
  for (int index = n - 1; index >= 0; index--) {
    suffix[index] = suffix[index + 1] * (point - T(index));
  }
  std::vector<T> inverse_factorial(n, T(1));
  T factorial = T(1);
  for (int value = 1; value < n; value++) {
    factorial *= T(value);
  }
  inverse_factorial[n - 1] = T(1) / factorial;
  for (int value = n - 1; value >= 1; value--) {
    inverse_factorial[value - 1] = inverse_factorial[value] * T(value);
  }
  T result{};
  for (int index = 0; index < n; index++) {
    T coefficient = prefix[index] * suffix[index + 1] *
                    inverse_factorial[index] * inverse_factorial[n - 1 - index];
    if ((n - 1 - index) & 1) {
      coefficient = -coefficient;
    }
    result += values[index] * coefficient;
  }
  return result;
}

/// @brief Return sum_{i=1}^n i^exponent over a field in O(exponent).
template <class T> T power_sum(std::uint64_t n, int exponent) {
  assert(exponent >= 0);
  auto power = [&](T value, int degree) {
    T result = T(1);
    while (degree > 0) {
      if (degree & 1) {
        result *= value;
      }
      value *= value;
      degree >>= 1;
    }
    return result;
  };
  std::vector<T> values(exponent + 2);
  for (int point = 1; point < int(values.size()); point++) {
    values[point] = values[point - 1] + power(T(point), exponent);
  }
  return lagrange_consecutive(values, n);
}

} // namespace noya

namespace noya {

namespace exponential_polynomial_sum_detail {

template <class Mint>
std::vector<Mint> monomial_values(int degree) {
  int count = degree + 1;
  std::vector<Mint> values;
  values.reserve(count + 1);
  if (degree == 0) {
    values.assign(count, Mint(1));
    return values;
  }

  std::vector<int> least_prime(count);
  std::vector<int> primes;
  for (int value = 2; value < count; value++) {
    if (least_prime[value] == 0) {
      least_prime[value] = value;
      primes.push_back(value);
    }
    for (int prime : primes) {
      if (prime > least_prime[value] || prime * std::int64_t(value) >= count) {
        break;
      }
      least_prime[prime * value] = prime;
    }
  }

  values.resize(count);
  if (count > 1) {
    values[1] = Mint(1);
  }
  for (int value = 2; value < count; value++) {
    if (least_prime[value] == value) {
      values[value] = Mint(value).pow(degree);
    } else {
      int factor = least_prime[value];
      values[value] = values[factor] * values[value / factor];
    }
  }
  return values;
}

template <class Mint>
Mint limit_from_values(Mint ratio, int degree,
                       const std::vector<Mint> &values) {
  assert(ratio != Mint(1));
  std::vector<Mint> inverse_factorial(degree + 2, Mint(1));
  Mint factorial = 1;
  for (int value = 1; value <= degree + 1; value++) {
    factorial *= Mint(value);
  }
  inverse_factorial[degree + 1] = Mint(1) / factorial;
  for (int value = degree + 1; value > 0; value--) {
    inverse_factorial[value - 1] =
        inverse_factorial[value] * Mint(value);
  }

  Mint ratio_power = 1;
  Mint reverse_ratio_power = ratio.pow(degree);
  Mint inverse_ratio = Mint(1) / ratio;
  Mint prefix_sum = 0;
  Mint answer = 0;
  for (int point = 0; point <= degree; point++) {
    prefix_sum += ratio_power * values[point];
    Mint term = inverse_factorial[degree - point] *
                inverse_factorial[point + 1] * reverse_ratio_power *
                prefix_sum;
    if ((degree - point) & 1) {
      term = -term;
    }
    answer += term;
    ratio_power *= ratio;
    reverse_ratio_power *= inverse_ratio;
  }
  return answer * factorial / (Mint(1) - ratio).pow(degree + 1);
}

} // namespace exponential_polynomial_sum_detail

/// @brief Return sum_{i>=0} r^i i^d over a finite field for r != 1, with
/// 0^0=1. A degree-d polynomial is determined by its values at 0..d. Writing
/// it in the consecutive-point Lagrange basis and summing each basis polynomial
/// against the geometric series reduces the answer to factorial weights and
/// prefix sums of r^i i^d.
template <class Mint>
Mint exponential_polynomial_sum_limit(Mint ratio, int degree) {
  assert(degree >= 0);
  assert(degree + 1 < Mint::mod());
  assert(ratio != Mint(1));
  if (ratio == Mint{}) {
    return degree == 0 ? Mint(1) : Mint{};
  }
  std::vector<Mint> values =
      exponential_polynomial_sum_detail::monomial_values<Mint>(degree);
  return exponential_polynomial_sum_detail::limit_from_values(
      ratio, degree, values);
}

/// @brief Return sum_{i=0}^{n-1} r^i i^d, with 0^0=1. For r=1 the prefix
/// sum is a degree-(d+1) polynomial in n. Otherwise let C be the infinite
/// weighted sum; r^{-n}(S(n)-C) is a degree-d polynomial, so d+2 sampled
/// prefix sums and one consecutive-point interpolation determine S(n), even
/// for very large n.
template <class Mint>
Mint exponential_polynomial_sum(Mint ratio, int degree, std::uint64_t count) {
  assert(degree >= 0);
  assert(degree + 1 < Mint::mod());
  if (count == 0) {
    return Mint{};
  }
  if (ratio == Mint{}) {
    return degree == 0 ? Mint(1) : Mint{};
  }

  std::vector<Mint> values =
      exponential_polynomial_sum_detail::monomial_values<Mint>(degree);
  if (ratio == Mint(1)) {
    values.insert(values.begin(), Mint{});
    for (int point = 1; point <= degree + 1; point++) {
      values[point] += values[point - 1];
    }
    return lagrange_consecutive(values, count);
  }

  Mint limit = exponential_polynomial_sum_detail::limit_from_values(
      ratio, degree, values);
  Mint prefix_sum = 0;
  Mint ratio_power = 1;
  Mint inverse_ratio_power = 1;
  Mint inverse_ratio = Mint(1) / ratio;
  values.push_back(Mint{});
  for (int point = 0; point <= degree; point++) {
    Mint monomial = values[point];
    values[point] = inverse_ratio_power * (prefix_sum - limit);
    prefix_sum += ratio_power * monomial;
    ratio_power *= ratio;
    inverse_ratio_power *= inverse_ratio;
  }
  values[degree + 1] = inverse_ratio_power * (prefix_sum - limit);
  return limit + ratio.pow(count) * lagrange_consecutive(values, count);
}

} // namespace noya

/// @complexity Time: O(1) arithmetic, O(log exponent) power/inverse.
/// Space: O(1).

#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace atcoder {

namespace internal {

// @param m `1 <= m`
// @return x mod m
constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// Fast modular multiplication by barrett reduction
// Reference: https://en.wikipedia.org/wiki/Barrett_reduction
// NOTE: reconsider after Ice Lake
struct barrett {
    unsigned int _m;
    unsigned long long im;

    // @param m `1 <= m`
    explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}

    // @return m
    unsigned int umod() const { return _m; }

    // @param a `0 <= a < m`
    // @param b `0 <= b < m`
    // @return `a * b % m`
    unsigned int mul(unsigned int a, unsigned int b) const {
        // [1] m = 1
        // a = b = im = 0, so okay

        // [2] m >= 2
        // im = ceil(2^64 / m)
        // -> im * m = 2^64 + r (0 <= r < m)
        // let z = a*b = c*m + d (0 <= c, d < m)
        // a*b * im = (c*m + d) * im = c*(im*m) + d*im = c*2^64 + c*r + d*im
        // c*r + d*im < m * m + m * im < m * m + 2^64 + m <= 2^64 + m * (m + 1) < 2^64 * 2
        // ((ab * im) >> 64) == c or c + 1
        unsigned long long z = a;
        z *= b;
#ifdef _MSC_VER
        unsigned long long x;
        _umul128(z, im, &x);
#else
        unsigned long long x =
            (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
#endif
        unsigned long long y = x * _m;
        return (unsigned int)(z - y + (z < y ? _m : 0));
    }
};

// @param n `0 <= n`
// @param m `1 <= m`
// @return `(x ** n) % m`
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

// Reference:
// M. Forisek and J. Jancina,
// Fast Primality Testing for Integers That Fit into a Machine Word
// @param n `0 <= n`
constexpr bool is_prime_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
template <int n> constexpr bool is_prime = is_prime_constexpr(n);

// @param b `1 <= b`
// @return pair(g, x) s.t. g = gcd(a, b), xa = g (mod b), 0 <= x < b/g
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};

    // Contracts:
    // [1] s - m0 * a = 0 (mod b)
    // [2] t - m1 * a = 0 (mod b)
    // [3] s * |m1| + t * |m0| <= b
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;

    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;  // |m1 * u| <= |m1| * s <= b

        // [3]:
        // (s - t * u) * |m1| + t * |m0 - m1 * u|
        // <= s * |m1| - t * u * |m1| + t * (|m0| + |m1| * u)
        // = s * |m1| + t * |m0| <= b

        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    // by [3]: |m0| <= b/g
    // by g != b: |m0| < b/g
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}

// Compile time primitive root
// @param m must be prime
// @return primitive root (and minimum in now)
constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
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
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod_constexpr(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
template <int m> constexpr int primitive_root = primitive_root_constexpr(m);

// @param n `n < 2^32`
// @param m `1 <= m < 2^32`
// @return sum_{i=0}^{n-1} floor((ai + b) / m) (mod 2^64)
unsigned long long floor_sum_unsigned(unsigned long long n,
                                      unsigned long long m,
                                      unsigned long long a,
                                      unsigned long long b) {
    unsigned long long ans = 0;
    while (true) {
        if (a >= m) {
            ans += n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }

        unsigned long long y_max = a * n + b;
        if (y_max < m) break;
        // y_max < m * (n + 1)
        // floor(y_max / m) <= n
        n = (unsigned long long)(y_max / m);
        b = (unsigned long long)(y_max % m);
        std::swap(m, a);
    }
    return ans;
}

}  // namespace internal

}  // namespace atcoder

namespace atcoder {

namespace internal {

#ifndef _MSC_VER
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
using is_integral = typename std::conditional<std::is_integral<T>::value ||
                                                  is_signed_int128<T>::value ||
                                                  is_unsigned_int128<T>::value,
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

#else

template <class T> using is_integral = typename std::is_integral<T>;

template <class T>
using is_signed_int =
    typename std::conditional<is_integral<T>::value && std::is_signed<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<is_integral<T>::value &&
                                  std::is_unsigned<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<is_signed_int<T>::value,
                                              std::make_unsigned<T>,
                                              std::common_type<T>>::type;

#endif

template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

}  // namespace atcoder

namespace atcoder {

namespace internal {

struct modint_base {};
struct static_modint_base : modint_base {};

template <class T> using is_modint = std::is_base_of<modint_base, T>;
template <class T> using is_modint_t = std::enable_if_t<is_modint<T>::value>;

}  // namespace internal

template <int m, std::enable_if_t<(1 <= m)>* = nullptr>
struct static_modint : internal::static_modint_base {
    using mint = static_modint;

  public:
    static constexpr int mod() { return m; }
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }

    static_modint() : _v(0) {}
    template <class T, internal::is_signed_int_t<T>* = nullptr>
    static_modint(T v) {
        long long x = (long long)(v % (long long)(umod()));
        if (x < 0) x += umod();
        _v = (unsigned int)(x);
    }
    template <class T, internal::is_unsigned_int_t<T>* = nullptr>
    static_modint(T v) {
        _v = (unsigned int)(v % umod());
    }

    int val() const { return _v; }

    mint& operator++() {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator--() {
        if (_v == 0) _v = umod();
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

    mint& operator+=(const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        _v -= rhs._v;
        if (_v >= umod()) _v += umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        unsigned long long z = _v;
        z *= rhs._v;
        _v = (unsigned int)(z % umod());
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }

    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }

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
        if (prime) {
            assert(_v);
            return pow(umod() - 2);
        } else {
            auto eg = internal::inv_gcd(_v, m);
            assert(eg.first == 1);
            return eg.second;
        }
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs._v != rhs._v;
    }

  private:
    unsigned int _v;
    static constexpr unsigned int umod() { return m; }
    static constexpr bool prime = internal::is_prime<m>;
};

template <int id> struct dynamic_modint : internal::modint_base {
    using mint = dynamic_modint;

  public:
    static int mod() { return (int)(bt.umod()); }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = internal::barrett(m);
    }
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }

    dynamic_modint() : _v(0) {}
    template <class T, internal::is_signed_int_t<T>* = nullptr>
    dynamic_modint(T v) {
        long long x = (long long)(v % (long long)(mod()));
        if (x < 0) x += mod();
        _v = (unsigned int)(x);
    }
    template <class T, internal::is_unsigned_int_t<T>* = nullptr>
    dynamic_modint(T v) {
        _v = (unsigned int)(v % mod());
    }

    int val() const { return _v; }

    mint& operator++() {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator--() {
        if (_v == 0) _v = umod();
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

    mint& operator+=(const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        _v += mod() - rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        _v = bt.mul(_v, rhs._v);
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }

    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }

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
        auto eg = internal::inv_gcd(_v, mod());
        assert(eg.first == 1);
        return eg.second;
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs._v != rhs._v;
    }

  private:
    unsigned int _v;
    static internal::barrett bt;
    static unsigned int umod() { return bt.umod(); }
};
template <int id> internal::barrett dynamic_modint<id>::bt(998244353);

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<-1>;

namespace internal {

template <class T>
using is_static_modint = std::is_base_of<internal::static_modint_base, T>;

template <class T>
using is_static_modint_t = std::enable_if_t<is_static_modint<T>::value>;

template <class> struct is_dynamic_modint : public std::false_type {};
template <int id>
struct is_dynamic_modint<dynamic_modint<id>> : public std::true_type {};

template <class T>
using is_dynamic_modint_t = std::enable_if_t<is_dynamic_modint<T>::value>;

}  // namespace internal

}  // namespace atcoder

namespace noya {

/// @brief Alias for atcoder::modint998244353.
using mint998 = atcoder::modint998244353;
/// @brief Alias for atcoder::modint1000000007.
using mint107 = atcoder::modint1000000007;

/// @brief Alias for atcoder::static_modint with a custom modulus.
template<const int MOD>
using mint = atcoder::static_modint<MOD>;

} // namespace noya

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int ratio, degree;
  std::cin >> ratio >> degree;
  std::cout << noya::exponential_polynomial_sum_limit(
                   noya::mint998(ratio), degree)
                   .val()
            << '\n';
}
