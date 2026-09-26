#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

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

/// @complexity Time: Expected O(n log n) build, O(log n) point assignment,
/// and O(sqrt(n)) per rectangle sum or affine update; rectangle operations are
/// O(n) in the worst case. Space: O(n).

namespace noya {

/// @brief Offline dynamic weighted point set using a balanced two-dimensional
/// KD-tree. Every node stores its coordinate bounding box, active-point count,
/// weight sum, and a lazy affine tag. A rectangle operation stops at a fully
/// covered box, while points that will be inserted later start inactive, so
/// earlier affine updates neither count nor change them.
template <class Coordinate, class Value> class rectangle_affine_kd_tree {
public:
  using point_type = std::pair<Coordinate, Coordinate>;

  rectangle_affine_kd_tree() = default;

  rectangle_affine_kd_tree(const std::vector<point_type> &points,
                           const std::vector<Value> &values,
                           const std::vector<bool> &active) {
    build(points, values, active);
  }

  /// @brief Build from every coordinate that can ever be inserted. Inactive
  /// points ignore rectangle operations until point_set activates them.
  void build(const std::vector<point_type> &points,
             const std::vector<Value> &values,
             const std::vector<bool> &active) {
    assert(points.size() == values.size());
    assert(points.size() == active.size());
    points_ = points;
    rank_.assign(points.size(), 0);
    tree_.assign(std::max<std::size_t>(1, points.size() * 4), node{});
    std::vector<int> order(points.size());
    std::iota(order.begin(), order.end(), 0);
    if (!order.empty()) {
      build_at(1, 0, int(order.size()), 0, order, values, active);
    }
  }

  /// @brief Return the number of offline coordinate instances.
  int size() const { return int(points_.size()); }

  /// @brief Assign one point's weight and mark it active.
  void point_set(int index, const Value &value) {
    assert(0 <= index && index < size());
    point_set_at(1, 0, size(), rank_[index], value);
  }

  /// @brief Sum active weights in [left, right) x [bottom, top).
  Value rectangle_sum(const Coordinate &left, const Coordinate &right,
                      const Coordinate &bottom, const Coordinate &top) {
    if (size() == 0 || !(left < right) || !(bottom < top)) {
      return Value(0);
    }
    return rectangle_sum_at(1, 0, size(), left, right, bottom, top);
  }

  /// @brief Replace every active weight w in the half-open rectangle by
  /// multiplier * w + addend.
  void rectangle_apply(const Coordinate &left, const Coordinate &right,
                       const Coordinate &bottom, const Coordinate &top,
                       const Value &multiplier, const Value &addend) {
    if (size() == 0 || !(left < right) || !(bottom < top)) {
      return;
    }
    rectangle_apply_at(1, 0, size(), left, right, bottom, top, multiplier,
                       addend);
  }

private:
  struct node {
    Coordinate min_x{};
    Coordinate max_x{};
    Coordinate min_y{};
    Coordinate max_y{};
    Value sum = Value(0);
    int active_count = 0;
    Value lazy_multiplier = Value(1);
    Value lazy_addend = Value(0);
    bool has_lazy = false;
  };

  std::vector<point_type> points_;
  std::vector<int> rank_;
  std::vector<node> tree_;

  void build_at(int id, int left, int right, int dimension,
                std::vector<int> &order, const std::vector<Value> &values,
                const std::vector<bool> &active) {
    if (left + 1 == right) {
      int index = order[left];
      rank_[index] = left;
      const auto &[x, y] = points_[index];
      tree_[id].min_x = tree_[id].max_x = x;
      tree_[id].min_y = tree_[id].max_y = y;
      tree_[id].active_count = active[index] ? 1 : 0;
      tree_[id].sum = active[index] ? values[index] : Value(0);
      return;
    }
    int middle = (left + right) / 2;
    auto compare = [&](int first, int second) {
      const auto &a = points_[first];
      const auto &b = points_[second];
      if (dimension == 0) {
        if (a.first != b.first) {
          return a.first < b.first;
        }
        if (a.second != b.second) {
          return a.second < b.second;
        }
      } else {
        if (a.second != b.second) {
          return a.second < b.second;
        }
        if (a.first != b.first) {
          return a.first < b.first;
        }
      }
      return first < second;
    };
    std::nth_element(order.begin() + left, order.begin() + middle,
                     order.begin() + right, compare);
    build_at(id * 2, left, middle, dimension ^ 1, order, values, active);
    build_at(id * 2 + 1, middle, right, dimension ^ 1, order, values, active);
    pull(id);
  }

  void pull(int id) {
    const node &first = tree_[id * 2];
    const node &second = tree_[id * 2 + 1];
    tree_[id].min_x = std::min(first.min_x, second.min_x);
    tree_[id].max_x = std::max(first.max_x, second.max_x);
    tree_[id].min_y = std::min(first.min_y, second.min_y);
    tree_[id].max_y = std::max(first.max_y, second.max_y);
    tree_[id].sum = first.sum + second.sum;
    tree_[id].active_count = first.active_count + second.active_count;
  }

  void apply_node(int id, const Value &multiplier, const Value &addend) {
    node &current = tree_[id];
    current.sum = multiplier * current.sum +
                  addend * Value(current.active_count);
    if (current.has_lazy) {
      current.lazy_multiplier = multiplier * current.lazy_multiplier;
      current.lazy_addend = multiplier * current.lazy_addend + addend;
    } else {
      current.lazy_multiplier = multiplier;
      current.lazy_addend = addend;
      current.has_lazy = true;
    }
  }

  void push(int id) {
    node &current = tree_[id];
    if (!current.has_lazy) {
      return;
    }
    apply_node(id * 2, current.lazy_multiplier, current.lazy_addend);
    apply_node(id * 2 + 1, current.lazy_multiplier, current.lazy_addend);
    current.lazy_multiplier = Value(1);
    current.lazy_addend = Value(0);
    current.has_lazy = false;
  }

  static bool disjoint(const node &current, const Coordinate &left,
                       const Coordinate &right, const Coordinate &bottom,
                       const Coordinate &top) {
    return current.max_x < left || !(current.min_x < right) ||
           current.max_y < bottom || !(current.min_y < top);
  }

  static bool contained(const node &current, const Coordinate &left,
                        const Coordinate &right, const Coordinate &bottom,
                        const Coordinate &top) {
    return !(current.min_x < left) && current.max_x < right &&
           !(current.min_y < bottom) && current.max_y < top;
  }

  void point_set_at(int id, int left, int right, int position,
                    const Value &value) {
    if (left + 1 == right) {
      tree_[id].sum = value;
      tree_[id].active_count = 1;
      tree_[id].lazy_multiplier = Value(1);
      tree_[id].lazy_addend = Value(0);
      tree_[id].has_lazy = false;
      return;
    }
    push(id);
    int middle = (left + right) / 2;
    if (position < middle) {
      point_set_at(id * 2, left, middle, position, value);
    } else {
      point_set_at(id * 2 + 1, middle, right, position, value);
    }
    pull(id);
  }

  Value rectangle_sum_at(int id, int left_index, int right_index,
                         const Coordinate &left, const Coordinate &right,
                         const Coordinate &bottom, const Coordinate &top) {
    const node &current = tree_[id];
    if (disjoint(current, left, right, bottom, top)) {
      return Value(0);
    }
    if (contained(current, left, right, bottom, top)) {
      return current.sum;
    }
    push(id);
    int middle = (left_index + right_index) / 2;
    return rectangle_sum_at(id * 2, left_index, middle, left, right, bottom,
                            top) +
           rectangle_sum_at(id * 2 + 1, middle, right_index, left, right,
                            bottom, top);
  }

  void rectangle_apply_at(int id, int left_index, int right_index,
                          const Coordinate &left, const Coordinate &right,
                          const Coordinate &bottom, const Coordinate &top,
                          const Value &multiplier, const Value &addend) {
    const node &current = tree_[id];
    if (disjoint(current, left, right, bottom, top)) {
      return;
    }
    if (contained(current, left, right, bottom, top)) {
      apply_node(id, multiplier, addend);
      return;
    }
    push(id);
    int middle = (left_index + right_index) / 2;
    rectangle_apply_at(id * 2, left_index, middle, left, right, bottom, top,
                       multiplier, addend);
    rectangle_apply_at(id * 2 + 1, middle, right_index, left, right, bottom,
                       top, multiplier, addend);
    pull(id);
  }
};

} // namespace noya

struct query {
  int type = 0;
  std::array<int, 6> argument{};
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int initial_size, query_count;
  std::cin >> initial_size >> query_count;
  using mint = atcoder::modint998244353;
  using tree_type = noya::rectangle_affine_kd_tree<int, mint>;
  std::vector<tree_type::point_type> points;
  std::vector<mint> values;
  std::vector<bool> active;
  points.reserve(initial_size + query_count);
  values.reserve(initial_size + query_count);
  active.reserve(initial_size + query_count);
  for (int index = 0; index < initial_size; index++) {
    int x, y, weight;
    std::cin >> x >> y >> weight;
    points.emplace_back(x, y);
    values.emplace_back(weight);
    active.push_back(true);
  }

  std::vector<query> queries(query_count);
  for (query &current : queries) {
    std::cin >> current.type;
    int count = current.type == 0 ? 3 : current.type == 1 ? 2 :
                current.type == 2 ? 4 : 6;
    for (int index = 0; index < count; index++) {
      std::cin >> current.argument[index];
    }
    if (current.type == 0) {
      int x = current.argument[0];
      int y = current.argument[1];
      int weight = current.argument[2];
      current.argument[0] = int(points.size());
      current.argument[1] = weight;
      points.emplace_back(x, y);
      values.emplace_back(0);
      active.push_back(false);
    }
  }

  tree_type tree(points, values, active);
  for (const query &current : queries) {
    const auto &a = current.argument;
    if (current.type <= 1) {
      tree.point_set(a[0], mint(a[1]));
    } else if (current.type == 2) {
      std::cout << tree.rectangle_sum(a[0], a[2], a[1], a[3]).val()
                << '\n';
    } else {
      tree.rectangle_apply(a[0], a[2], a[1], a[3], mint(a[4]), mint(a[5]));
    }
  }
}
