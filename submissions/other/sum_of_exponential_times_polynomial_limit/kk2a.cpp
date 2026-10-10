#include <stack>
#include <istream>
#include <cstdint>
#include <optional>
#include <cmath>
#include <vector>
#include <ostream>
#include <limits>
#include <unordered_map>
#include <map>
#include <bitset>
#include <bit>
#include <iostream>
#include <numeric>
#include <array>
#include <fstream>
#include <queue>
#include <ranges>
#include <cstdio>
#include <functional>
#include <deque>
#include <set>
#include <cstddef>
#include <utility>
#include <unordered_set>
#include <concepts>
#include <cassert>
#include <cctype>
#include <algorithm>
#include <type_traits>
#include <random>
#include <chrono>
#include <string>
#include <iterator>

// competitive-verifier: PROBLEM
// https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit

#ifndef KK2_MATH_MOD_POWER_SUM_HPP
#define KK2_MATH_MOD_POWER_SUM_HPP 1


#ifndef KK2_COMMON_TYPE_ALIAS_HPP
#define KK2_COMMON_TYPE_ALIAS_HPP 1


namespace kk2 {

using usize = std::size_t;
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;

#ifndef _MSC_VER
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

} // namespace kk2

#endif // KK2_COMMON_TYPE_ALIAS_HPP
#ifndef KK2_FPS_POLY_SAMPLE_POINT_EVALUATE_HPP
#define KK2_FPS_POLY_SAMPLE_POINT_EVALUATE_HPP 1


#ifndef KK2_COMMON_TYPE_ALIAS_HPP
#define KK2_COMMON_TYPE_ALIAS_HPP 1


namespace kk2 {

using usize = std::size_t;
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;

#ifndef _MSC_VER
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

} // namespace kk2

#endif // KK2_COMMON_TYPE_ALIAS_HPP
#ifndef KK2_MATH_MOD_COMB_HPP
#define KK2_MATH_MOD_COMB_HPP 1


#ifndef KK2_COMMON_TYPE_ALIAS_HPP
#define KK2_COMMON_TYPE_ALIAS_HPP 1


namespace kk2 {

using usize = std::size_t;
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;

#ifndef _MSC_VER
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

} // namespace kk2

#endif // KK2_COMMON_TYPE_ALIAS_HPP
#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
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

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP
#ifndef KK2_TYPE_TRAITS_MODINT_HPP
#define KK2_TYPE_TRAITS_MODINT_HPP 1



namespace kk2::modint {

template <class M>
concept Modular = requires(M x) {
    requires Integral<decltype(M::getmod())>;
    x.val();
    { x.inv() } -> std::same_as<M>;
};

} // namespace kk2::modint

#endif // KK2_TYPE_TRAITS_MODINT_HPP
#ifndef KK2_MATH_MOD_INV_TABLE_HPP
#define KK2_MATH_MOD_INV_TABLE_HPP 1



namespace kk2 {

/**
 * @brief `[1, n]`のmod逆元を列挙するテーブル
 *
 * @tparam mint
 */
template <class mint> struct InvTable {
    static inline std::vector<mint> _invs{0, 1};
    InvTable() = delete;

    static void set_upper(usize m) {
        if (_invs.size() > m) return;
        using index_type = std::make_unsigned_t<std::remove_cv_t<decltype(mint::getmod())>>;
        const index_type start = static_cast<index_type>(_invs.size());
        const index_type upper = static_cast<index_type>(m);
        const index_type mod = static_cast<index_type>(mint::getmod());
        _invs.resize(m + 1);
        // p = q * i + r
        // - q / r = 1 / i (mod p)
        for (index_type i = start; i <= upper; ++i)
            _invs[i] = (-_invs[mod % i]) * (mod / i);
    }

    template <UnsignedIntegral T> static inline mint inv(T n) {
        const usize index = static_cast<usize>(n);
        if (index >= _invs.size()) set_upper(index);
        return _invs[index];
    }

    template <SignedIntegral T> static inline mint inv(T n) {
        using U = std::make_unsigned_t<T>;
        const U un = static_cast<U>(n);
        if (n < 0) return -inv(U(0) - un);
        return inv(un);
    }

    static inline mint inv_unchecked(usize n) {
        return _invs[n];
    }
};

} // namespace kk2

#endif // KK2_MATH_MOD_INV_TABLE_HPP

namespace kk2 {

template <modint::Modular mint> struct Comb {
    static inline std::vector<mint> _fact{1}, _ifact{1};

    Comb() = delete;

  private:
    static void extend(usize m) {
        const usize n = _fact.size();
        m = std::min<usize>(m, mint::getmod() - 1);
        _fact.reserve(m + 1);
        _ifact.resize(m + 1);
        auto &_invs = InvTable<mint>::_invs;
        if (_invs.size() <= m) _invs.resize(m + 1);
        for (usize i = n; i <= m; i++) _fact.emplace_back(_fact.back() * i);
        _ifact[m] = _fact[m].inv();
        _invs[m] = _ifact[m] * _fact[m - 1];
        for (usize i = m; i > n; i--) {
            _ifact[i - 1] = _ifact[i] * i;
            _invs[i - 1] = _ifact[i - 1] * _fact[i - 2];
        }
    }

    static void ensure(usize n) {
        assert(n < mint::getmod());
        if (_fact.size() > n) return;
        extend(std::max<usize>(n, _fact.size() * 2));
    }

  public:
    static void set_upper(usize n) { ensure(n); }

    static mint fact(u32 n) {
        ensure(static_cast<usize>(n));
        return _fact[n];
    }

    static mint ifact(u32 n) {
        ensure(static_cast<usize>(n));
        return _ifact[n];
    }

    static mint inv(i32 n) {
        assert(n != 0);
        return InvTable<mint>::inv(n);
    }

    static mint binom(u32 n, u32 k) {
        if (k > n) return 0;
        return fact(n) * ifact(k) * ifact(n - k);
    }

    template <UnsignedIntegral T> static mint multinomial(const std::vector<T> &r) {
        u64 n = 0;
        for (const T x : r) n += x;
        assert(n < mint::getmod());
        mint res = fact(static_cast<u32>(n));
        for (const T x : r) res *= ifact(static_cast<u32>(x));
        return res;
    }

    static mint binom_naive(u32 n, u32 k) {
        if (k > n) return 0;
        mint res = 1;
        k = std::min(k, n - k);
        for (u32 i = 1; i <= k; i++) res *= inv(i) * (n--);
        return res;
    }

    static mint permu(u32 n, u32 k) {
        if (k > n) return 0;
        return fact(n) * ifact(n - k);
    }

    static mint homo(u32 n, u32 k) { return k == 0 ? 1 : binom(n + k - 1, k); }
};

} // namespace kk2

#endif // KK2_MATH_MOD_COMB_HPP
#ifndef KK2_TYPE_TRAITS_MODINT_HPP
#define KK2_TYPE_TRAITS_MODINT_HPP 1


#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
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

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP

namespace kk2::modint {

template <class M>
concept Modular = requires(M x) {
    requires Integral<decltype(M::getmod())>;
    x.val();
    { x.inv() } -> std::same_as<M>;
};

} // namespace kk2::modint

#endif // KK2_TYPE_TRAITS_MODINT_HPP

namespace kk2 {

/**
 * @brief Return `f(t)`, where `f(i) = y[i]` for `i = 0, ..., y.size() - 1`.
 */
template <modint::Modular mint> mint sample_point_evaluate(const std::vector<mint> &y, mint t) {
    if (y.empty()) return 0;
    assert(y.size() <= static_cast<usize>(mint::getmod()));

    const usize tval = static_cast<usize>(t.val());
    if (tval < y.size()) return y[tval];

    const i32 degree = static_cast<i32>(y.size()) - 1;
    Comb<mint>::set_upper(degree);

    std::vector<mint> prefix(y.size() + 1, mint(1));
    for (usize i = 0; i < y.size(); i++) prefix[i + 1] = prefix[i] * (t - mint(i));

    mint result = 0;
    mint suffix = 1;
    for (usize i = y.size(); i-- > 0;) {
        mint term = y[i] * Comb<mint>::ifact(static_cast<i32>(i))
                    * Comb<mint>::ifact(degree - static_cast<i32>(i));
        if ((degree - static_cast<i32>(i)) & 1) term = -term;
        result += term * prefix[i] * suffix;
        suffix *= t - mint(i);
    }
    return result;
}

} // namespace kk2

#endif // KK2_FPS_POLY_SAMPLE_POINT_EVALUATE_HPP
#ifndef KK2_MATH_MULTIPLICATIVE_FUNCTION_POW_TABLE_HPP
#define KK2_MATH_MULTIPLICATIVE_FUNCTION_POW_TABLE_HPP 1


#ifndef KK2_COMMON_TYPE_ALIAS_HPP
#define KK2_COMMON_TYPE_ALIAS_HPP 1


namespace kk2 {

using usize = std::size_t;
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;

#ifndef _MSC_VER
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

} // namespace kk2

#endif // KK2_COMMON_TYPE_ALIAS_HPP
#ifndef KK2_MATH_LPF_TABLE_HPP
#define KK2_MATH_LPF_TABLE_HPP 1


#ifndef KK2_MATH_MULTIPLICATIVE_FUNCTION_PRIME_COUNTING_HPP
#define KK2_MATH_MULTIPLICATIVE_FUNCTION_PRIME_COUNTING_HPP 1


#ifndef KK2_DATA_STRUCTURE_MY_BITSET_HPP
#define KK2_DATA_STRUCTURE_MY_BITSET_HPP 1


#ifndef KK2_BIT_BITCOUNT_HPP
#define KK2_BIT_BITCOUNT_HPP 1


#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
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

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP

namespace kk2 {

template <Integral T> constexpr int ctz(T x) {
    assert(x != T(0));

    if constexpr (sizeof(T) <= 4) {
        return __builtin_ctz(x);
    } else if constexpr (sizeof(T) <= 8) {
        return __builtin_ctzll(x);
    } else {
        if (x & 0xffffffffffffffff)
            return __builtin_ctzll((unsigned long long)(x & 0xffffffffffffffff));
        return 64 + __builtin_ctzll((unsigned long long)(x >> 64));
    }
}

template <Integral T> constexpr int lsb(T x) {
    assert(x != T(0));

    return ctz(x);
}

template <Integral T> constexpr int clz(T x) {
    assert(x != T(0));

    if constexpr (sizeof(T) <= 4) {
        return __builtin_clz(x);
    } else if constexpr (sizeof(T) <= 8) {
        return __builtin_clzll(x);
    } else {
        if (x >> 64) return __builtin_clzll((unsigned long long)(x >> 64));
        return 64 + __builtin_clzll((unsigned long long)(x & 0xffffffffffffffff));
    }
}

template <Integral T> constexpr int msb(T x) {
    assert(x != T(0));

    return sizeof(T) * 8 - 1 - clz(x);
}

template <Integral T> constexpr int popcount(T x) {

    if constexpr (sizeof(T) <= 4) {
        return __builtin_popcount(x);
    } else if constexpr (sizeof(T) <= 8) {
        return __builtin_popcountll(x);
    } else {
        return __builtin_popcountll((unsigned long long)(x >> 64))
               + __builtin_popcountll((unsigned long long)(x & 0xffffffffffffffff));
    }
}

}; // namespace kk2

#endif // KK2_BIT_BITCOUNT_HPP
#ifndef KK2_TYPE_TRAITS_IO_HPP
#define KK2_TYPE_TRAITS_IO_HPP 1


namespace kk2 {

namespace type_traits {

struct istream_tag {};
struct ostream_tag {};

} // namespace type_traits

template <typename T>
using is_standard_istream = typename std::conditional<std::is_same<T, std::istream>::value
                                                          || std::is_same<T, std::ifstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T>
using is_standard_ostream = typename std::conditional<std::is_same<T, std::ostream>::value
                                                          || std::is_same<T, std::ofstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T> using is_user_defined_istream = std::is_base_of<type_traits::istream_tag, T>;
template <typename T> using is_user_defined_ostream = std::is_base_of<type_traits::ostream_tag, T>;

template <typename T>
using is_istream =
    typename std::conditional<is_standard_istream<T>::value || is_user_defined_istream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_ostream =
    typename std::conditional<is_standard_ostream<T>::value || is_user_defined_ostream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_istream_t = std::enable_if_t<is_istream<T>::value>;
template <typename T> using is_ostream_t = std::enable_if_t<is_ostream<T>::value>;

template <class T>
concept StandardInputStream = is_standard_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept StandardOutputStream = is_standard_ostream<std::remove_cvref_t<T>>::value;

template <class T>
concept InputStream = is_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept OutputStream = is_ostream<std::remove_cvref_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_IO_HPP

namespace kk2 {

template <class Accessor> struct MonotoneRankRange {
    Accessor _accessor;

    struct StrideRange {
        Accessor _accessor;
        int _start, _end, _step;

        struct Iterator {
            using value_type = int;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;
            using reference = int;
            using pointer = void;

            int rank, end, step;
            mutable typename Accessor::MonotoneCursor cursor;

            Iterator(int rank_, int end_, int step_, const Accessor &accessor)
                : rank(rank_),
                  end(end_),
                  step(step_),
                  cursor(accessor.monotone_cursor()) {}

            int operator*() const { return cursor[rank]; }

            Iterator &operator++() {
                rank = step < end - rank ? rank + step : end;
                return *this;
            }

            Iterator operator++(int) {
                Iterator result = *this;
                ++*this;
                return result;
            }

            bool operator==(const Iterator &other) const { return rank == other.rank; }
        };

        Iterator begin() const { return Iterator(_start, _end, _step, _accessor); }
        Iterator end() const { return Iterator(_end, _end, _step, _accessor); }

        int size() const {
            if (_start == _end) return 0;
            return (_end - _start - 1) / _step + 1;
        }

        std::vector<int> to_vec() const {
            std::vector<int> result;
            result.reserve(size());
            for (int value : *this) result.push_back(value);
            return result;
        }
    };

    auto begin() const { return stride(0, 1).begin(); }
    auto end() const { return stride(0, 1).end(); }
    int size() const { return _accessor.size(); }

    int operator[](int rank) const { return _accessor[rank]; }

    auto monotone_cursor() const { return _accessor.monotone_cursor(); }

    StrideRange stride(int start, int step) const { return stride(start, step, size()); }

    StrideRange stride(int start, int step, int end) const {
        assert(0 <= start && start <= end && end <= size() && step > 0);
        return StrideRange{_accessor, start, end, step};
    }

    std::vector<int> to_vec() const {
        std::vector<int> result;
        result.reserve(size());
        for (int value : *this) result.push_back(value);
        return result;
    }
};

struct DynamicBitSet {
    struct RankSelect;

    using T = DynamicBitSet;
    using UInt = std::uint64_t;
    constexpr static int BLOCK_SIZE = sizeof(UInt) * 8;
    constexpr static int BLOCK_SIZE_LOG = __builtin_ctz(BLOCK_SIZE);
    constexpr static int BLOCK_MASK = BLOCK_SIZE - 1;
    constexpr static UInt ONE = 1;
    int n;
    std::vector<UInt> block;

    DynamicBitSet(int n_ = 0, bool x = 0) : n(n_) {
        UInt val = x ? -1 : 0;
        block.assign((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG, val);
        if (n & BLOCK_MASK) block.back() >>= BLOCK_SIZE - (n & BLOCK_MASK);
        // fit the last block
    }

    DynamicBitSet(const std::string &s) : n(s.size()) {
        block.resize((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        set(s);
    }

    inline int size() const { return n; }

    int word_count() const { return block.size(); }

    UInt &word(int i) {
        assert(0 <= i && i < word_count());
        return block[i];
    }

    const UInt &word(int i) const {
        assert(0 <= i && i < word_count());
        return block[i];
    }

    UInt *data() { return block.data(); }

    const UInt *data() const { return block.data(); }

    T &clear_unused_bits() {
        if ((n & BLOCK_MASK) && !block.empty()) block.back() &= (ONE << (n & BLOCK_MASK)) - 1;
        return *this;
    }

    T &inplace_combine_top(const T &rhs) {
        if (this == &rhs) {
            T copy = rhs;
            return inplace_combine_top(copy);
        }
        int old_n = n;
        n += rhs.n;
        block.resize((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        int offset = old_n & BLOCK_MASK;
        int word_offset = old_n >> BLOCK_SIZE_LOG;
        if (offset == 0) {
            std::copy(rhs.block.begin(), rhs.block.end(), block.begin() + word_offset);
        } else {
            for (int i = 0; i < rhs.word_count(); ++i) {
                block[word_offset + i] |= rhs.block[i] << offset;
                if (word_offset + i + 1 < word_count()) {
                    block[word_offset + i + 1] = rhs.block[i] >> (BLOCK_SIZE - offset);
                }
            }
        }
        return *this;
    }

    T combine_top(const T &rhs) const { return T(*this).inplace_combine_top(rhs); }

    T &inplace_combine_bottom(const T &rhs) {
        T result = rhs;
        result.inplace_combine_top(*this);
        *this = std::move(result);
        return *this;
    }

    T combine_bottom(const T &rhs) const { return T(*this).inplace_combine_bottom(rhs); }

    void set(int i, bool x = true) {
        assert(0 <= i && i < n);
        if (x) block[i >> BLOCK_SIZE_LOG] |= ONE << (i & BLOCK_MASK);
        else block[i >> BLOCK_SIZE_LOG] &= ~(ONE << (i & BLOCK_MASK));
    }

    void reset(int i) { set(i, false); }

    T &set_all(bool x = true) {
        std::fill(block.begin(), block.end(), x ? ~UInt(0) : UInt(0));
        if (x && (n & BLOCK_MASK)) block.back() &= (ONE << (n & BLOCK_MASK)) - 1;
        return *this;
    }

    T &reset_all() { return set_all(false); }

    void set(const std::string &s) {
        assert((int)s.size() == n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            int r = n - (i << BLOCK_SIZE_LOG), l = std::max(0, r - BLOCK_SIZE);
            block[i] = 0;
            for (int j = l; j < r; j++) block[i] = (block[i] << 1) | (s[j] - '0');
        }
    }

    void set_reversed(const std::string &s) {
        assert((int)s.size() == n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            int l = i << BLOCK_SIZE_LOG, r = std::min(n, l + BLOCK_SIZE);
            block[i] = 0;
            for (int j = r - 1; j >= l; --j) block[i] = (block[i] << 1) | (s[j] - '0');
        }
    }

    struct BitReference {
        std::vector<UInt> &block;
        int idx;

      public:
        BitReference(std::vector<UInt> &block_, int idx_) : block(block_), idx(idx_) {}

        operator bool() const { return (block[idx >> BLOCK_SIZE_LOG] >> (idx & BLOCK_MASK)) & 1; }

        template <InputStream IStream> friend IStream &operator>>(IStream &is, BitReference a) {
            bool c;
            is >> c;
            a = c;
            return is;
        }

        BitReference &operator=(bool x) {
            if (x) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            else block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator=(const BitReference &other) {
            if (other) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            else block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator&=(bool x) {
            if (!x) block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator&=(const BitReference &other) {
            if (!other) block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator|=(bool x) {
            if (x) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator|=(const BitReference &other) {
            if (other) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator^=(bool x) {
            if (x) block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator^=(const BitReference &other) {
            if (other) block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &flip() {
            block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator~() {
            block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        bool val() const { return (block[idx >> BLOCK_SIZE_LOG] >> (idx & BLOCK_MASK)) & 1; }
    };

    BitReference operator[](int i) {
        assert(0 <= i && i < n);
        return BitReference(block, i);
    }

    bool operator[](int i) const {
        assert(0 <= i && i < n);
        return (block[i >> BLOCK_SIZE_LOG] >> (i & BLOCK_MASK)) & 1;
    }

    bool is_pinned(int i) const {
        assert(0 <= i && i < n);
        return (block[i >> BLOCK_SIZE_LOG] >> (i & BLOCK_MASK)) & 1;
    }

    T &operator=(const std::string &s) {
        set(s);
        return *this;
    }

    T &flip() {
        for (UInt &x : block) x = ~x;
        if (n & BLOCK_MASK) block.back() &= (ONE << (n & BLOCK_MASK)) - 1;
        return *this;
    }

    T &flip(int i) {
        assert(0 <= i && i < n);
        block[i >> BLOCK_SIZE_LOG] ^= ONE << (i & BLOCK_MASK);
        return *this;
    }

    int ctz() const { return find_next(0); }

    int clz() const {
        int last = find_prev(n - 1);
        return last == -1 ? n : n - 1 - last;
    }

    int find_next(int i) const {
        if (i < 0) i = 0;
        if (i >= n) return n;
        int j = i >> BLOCK_SIZE_LOG;
        UInt bits = block[j] & (~UInt(0) << (i & BLOCK_MASK));
        while (true) {
            if (bits) return std::min(n, j * BLOCK_SIZE + (int)std::countr_zero(bits));
            if (++j == word_count()) return n;
            bits = block[j];
        }
    }

    int find_next_zero(int i) const {
        if (i < 0) i = 0;
        if (i >= n) return n;
        int j = i >> BLOCK_SIZE_LOG;
        UInt bits = ~block[j] & (~UInt(0) << (i & BLOCK_MASK));
        while (true) {
            if (bits) return std::min(n, j * BLOCK_SIZE + (int)std::countr_zero(bits));
            if (++j == word_count()) return n;
            bits = ~block[j];
        }
    }

    int find_prev(int i) const {
        if (i >= n) i = n - 1;
        if (i < 0) return -1;
        int j = i >> BLOCK_SIZE_LOG;
        int offset = i & BLOCK_MASK;
        UInt bits = block[j] & (~UInt(0) >> (BLOCK_MASK - offset));
        while (true) {
            if (bits) return j * BLOCK_SIZE + (BLOCK_MASK - std::countl_zero(bits));
            if (j-- == 0) return -1;
            bits = block[j];
        }
    }

    int popcount() const {
        int res = 0;
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            res += kk2::popcount(block[i]);
        }
        return res;
    }

    T &operator~() { return flip(); }

    T &operator&=(const T &other) {
        assert(n == other.n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            block[i] &= other.block[i];
        }
        return *this;
    }

    T &operator|=(const T &other) {
        assert(n == other.n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            block[i] |= other.block[i];
        }
        return *this;
    }

    T &operator^=(const T &other) {
        assert(n == other.n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            block[i] ^= other.block[i];
        }
        return *this;
    }

    T &inplace_or_repeated(const T &pattern) {
        assert(pattern.n > 0 && (pattern.n & BLOCK_MASK) == 0);
        int pattern_words = pattern.word_count();
        for (int begin = 0; begin < word_count(); begin += pattern_words) {
            int size = std::min(pattern_words, word_count() - begin);
            for (int i = 0; i < size; ++i) block[begin + i] |= pattern.block[i];
        }
        return clear_unused_bits();
    }

    friend T operator&(const T &lhs, const T &rhs) { return T(lhs) &= rhs; }

    friend T operator|(const T &lhs, const T &rhs) { return T(lhs) |= rhs; }

    friend T operator^(const T &lhs, const T &rhs) { return T(lhs) ^= rhs; }

    friend bool operator==(const T &lhs, const T &rhs) {
        if (lhs.n != rhs.n) return false;
        for (int i = 0; i < (lhs.n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            if (lhs.block[i] != rhs.block[i]) return false;
        }
        return true;
    }

    friend bool operator!=(const T &lhs, const T &rhs) { return !(lhs == rhs); }

    operator bool() const {
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            if (block[i]) return true;
        }
        return false;
    }

    std::string to_string(UInt x) const { return std::bitset<BLOCK_SIZE>(x).to_string(); }

    std::string to_string() const {
        std::vector<std::string> tmp((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            tmp[i] = to_string(block[i]);
        }
        if (n & BLOCK_MASK) {
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
            tmp.back().resize(n & BLOCK_MASK);
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
        }
        std::string res;
        for (int i = (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i--;) { res += tmp[i]; }
        return res;
    }

    std::string to_reversed_string() const {
        std::vector<std::string> tmp((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            tmp[i] = to_string(block[i]);
        }
        if (n & BLOCK_MASK) {
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
            tmp.back().resize(n & BLOCK_MASK);
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
        }
        std::string res;
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            std::reverse(std::begin(tmp[i]), std::end(tmp[i]));
            res += tmp[i];
        }
        return res;
    }

    template <OutputStream OStream> friend OStream &operator<<(OStream &os, const T &bs) {
        return os << bs.to_string();
    }

    template <InputStream IStream> friend IStream &operator>>(IStream &is, T &bs) {
        std::string s;
        is >> s;
        bs.set_reversed(s);
        return is;
    }
};

struct DynamicBitSet::RankSelect {
  private:
    DynamicBitSet _bits;
    std::vector<int> _prefix;

    UInt selected_word(int word) const { return _bits.word(word); }

  public:
    RankSelect() : _prefix(1) {}

    explicit RankSelect(DynamicBitSet bits, bool value = true)
        : _bits(std::move(bits)),
          _prefix(_bits.word_count() + 1) {
        for (int word = 0; word < _bits.word_count(); ++word) {
            UInt selected = value ? _bits.word(word) : ~_bits.word(word);
            if (word + 1 == _bits.word_count() && (_bits.size() & BLOCK_MASK)) {
                selected &= (ONE << (_bits.size() & BLOCK_MASK)) - 1;
            }
            _bits.word(word) = selected;
            _prefix[word + 1] = _prefix[word] + std::popcount(selected);
        }
    }

    int bit_size() const { return _bits.size(); }

    bool contains(int index) const {
        assert(0 <= index && index < bit_size());
        return _bits[index];
    }

    int rank(int end) const {
        assert(0 <= end && end <= bit_size());
        int word = end >> BLOCK_SIZE_LOG;
        int result = _prefix[word];
        if (end & BLOCK_MASK) {
            UInt mask = (ONE << (end & BLOCK_MASK)) - 1;
            result += std::popcount(selected_word(word) & mask);
        }
        return result;
    }

    int select(int rank) const {
        assert(0 <= rank && rank < size());
        int word =
            (int)(std::upper_bound(_prefix.begin(), _prefix.end(), rank) - _prefix.begin()) - 1;
        UInt selected = selected_word(word);
        int local_rank = rank - _prefix[word];
        while (local_rank--) selected &= selected - 1;
        return word * BLOCK_SIZE + std::countr_zero(selected);
    }

    int find_next(int index) const { return _bits.find_next(index); }

    struct MonotoneCursor {
      private:
        const RankSelect *_index;
        int _last_rank = -1;
        int _word = 0;

      public:
        explicit MonotoneCursor(const RankSelect &index) : _index(&index) {}

        int operator[](int rank) {
            assert(0 <= rank && _last_rank <= rank && rank < _index->size());
            _last_rank = rank;
            while (_index->_prefix[_word + 1] <= rank) ++_word;
            UInt selected = _index->selected_word(_word);
            int local_rank = rank - _index->_prefix[_word];
            while (local_rank--) selected &= selected - 1;
            return _word * BLOCK_SIZE + std::countr_zero(selected);
        }
    };

    int size() const { return _prefix.back(); }

    int operator[](int rank) const { return select(rank); }

    MonotoneCursor monotone_cursor() const & { return MonotoneCursor(*this); }
    MonotoneCursor monotone_cursor() const && = delete;

    struct RangeAccessor {
        const RankSelect *index;

        using MonotoneCursor = RankSelect::MonotoneCursor;

        int size() const { return index->size(); }
        int operator[](int rank) const { return (*index)[rank]; }
        MonotoneCursor monotone_cursor() const { return index->monotone_cursor(); }
    };

    using Range = MonotoneRankRange<RangeAccessor>;

    Range range() const & { return Range{RangeAccessor{this}}; }
    Range range() const && = delete;

    auto begin() const { return range().begin(); }
    auto end() const { return range().end(); }
    auto stride(int start, int step) const & { return range().stride(start, step); }
    auto stride(int start, int step, int end) const & { return range().stride(start, step, end); }
    auto stride(int, int) const && = delete;
    auto stride(int, int, int) const && = delete;
    std::vector<int> to_vec() const { return range().to_vec(); }
};

} // namespace kk2

#endif // KK2_DATA_STRUCTURE_MY_BITSET_HPP
#ifndef KK2_MATH_ENUMERATE_QUOTIENTS_HPP
#define KK2_MATH_ENUMERATE_QUOTIENTS_HPP 1


#ifndef KK2_MATH_SQRT_FLOOR_HPP
#define KK2_MATH_SQRT_FLOOR_HPP 1


#ifndef KK2_MATH_FRAC_FLOOR_HPP
#define KK2_MATH_FRAC_FLOOR_HPP 1


namespace kk2 {

// floor(x) = ceil(x) - 1 (for all x not in Z) ...(1)
// floor(x) = -ceil(-x)   (for all x)          ...(2)

// return floor(a / b)
template <typename T, typename U> constexpr T fracfloor(T a, U b) {
    assert(b != 0);
    if (a % b == 0) return a / b;
    if (a >= 0) return a / b;

    // floor(x) = -ceil(-x)      by (2)
    //          = -floor(-x) - 1 by (1)
    return -((-a) / b) - 1;
}

// return ceil(a / b)
template <typename T, typename U> constexpr T fracceil(T a, U b) {
    assert(b != 0);
    if (a % b == 0) return a / b;
    if (a >= 0) return a / b + 1;

    // ceil(x) = -floor(-x)      by (2)
    return -((-a) / b);
}

} // namespace kk2

#endif // KK2_MATH_FRAC_FLOOR_HPP

namespace kk2 {

template <typename T> T sqrt_floor(T n) {
    assert(n >= 0);
    if (n == T(0)) return 0;
    T x = std::sqrt(n);
    if (x == T(0)) ++x;
    while (x > kk2::fracfloor(n, x)) --x;
    while (x + 1 <= kk2::fracfloor(n, x + 1)) ++x;
    return x;
}

template <typename T> T sqrt_ceil(T n) {
    assert(n >= 0);
    if (n <= T(1)) return n;
    T x = std::sqrt(n);
    if (x == T(0)) ++x;
    while (x < kk2::fracceil(n, x)) ++x;
    while (x - 1 >= kk2::fracceil(n, x - 1)) --x;
    return x;
}

} // namespace kk2

#endif // KK2_MATH_SQRT_FLOOR_HPP

namespace kk2 {

template <class T> struct EnumerateQuotients {
    T n;
    int sqrt_n;
    std::vector<T> res;

    EnumerateQuotients(T n) : n(n), sqrt_n(sqrt_floor(n)) {
        res.resize(sqrt_n + n / (sqrt_n + 1));
        std::iota(res.begin(), res.begin() + sqrt_n, 1);
        for (T i = n / (sqrt_n + 1), j = sqrt_n; i; --i, ++j) res[j] = n / i;
    }

    const std::vector<T> &get() const { return res; }

    int size() const { return res.size(); }

    const T &operator[](int i) const { return res[i]; }

    int idx(T x) const {
        if (x <= sqrt_n) return x - 1;
        return size() - n / x;
    }
};

} // namespace kk2

#endif // KK2_MATH_ENUMERATE_QUOTIENTS_HPP

namespace kk2 {

namespace internal {

inline std::vector<int> prime_counting_base_primes(int n) {
    DynamicBitSet composite(n + 1);
    std::vector<int> primes;
    for (int p = 2; p <= n; ++p) {
        if (composite[p]) continue;
        primes.push_back(p);
        if (1LL * p * p <= n) {
            for (long long q = 1LL * p * p; q <= n; q += p) composite.set((int)q);
        }
    }
    return primes;
}

} // namespace internal

long long prime_counting(long long n) {
    if (n < 2) return 0;
    EnumerateQuotients<long long> eq(n);
    std::vector<int> primes = internal::prime_counting_base_primes(eq.sqrt_n);
    std::vector<long long> dp(eq.size());
    for (int i = 0; i < eq.size(); ++i) dp[i] = eq[i] - 1;
    for (const long long p : primes) {
        for (int i = eq.size() - 1;; --i) {
            if (eq[i] < p * p) break;
            dp[i] -= dp[eq.idx(eq[i] / p)] - dp[p - 2];
        }
    }
    return dp.back();
}

} // namespace kk2

#endif // KK2_MATH_MULTIPLICATIVE_FUNCTION_PRIME_COUNTING_HPP

namespace kk2 {

struct LPFTable {
  private:
    static inline std::vector<u32> _primes{2}, _lpf{0, 1, 2};

  public:
    LPFTable() = delete;

    static void set_upper(usize m) {
        if (_lpf.size() > m) return;
        m = std::max<usize>(2 * _lpf.size(), m);
        assert(m <= static_cast<usize>(std::numeric_limits<u32>::max()));
        usize reserve_target = static_cast<usize>(prime_counting(static_cast<i64>(m)));
        if (_primes.capacity() < reserve_target) _primes.reserve(reserve_target);
        _lpf.resize(m + 1);
        const u32 upper = static_cast<u32>(m);
        for (usize index = 2; index <= m; ++index) {
            const u32 i = static_cast<u32>(index);
            if (_lpf[index] == 0) {
                _lpf[index] = i;
                _primes.emplace_back(i);
            }
            for (const u32 p : _primes) {
                const u64 pi = static_cast<u64>(p) * i;
                if (pi > upper) break;
                const usize product = static_cast<usize>(pi);
                if (_lpf[index] < p) break;
                _lpf[product] = p;
            }
        }
    }

    static const std::vector<u32> &primes() { return _primes; }

    template <typename It> struct PrimeIt {
        It bg, ed;
        PrimeIt(It bg_, It ed_) : bg(bg_), ed(ed_) {}
        It begin() const { return bg; }
        It end() const { return ed; }
        usize size() const { return static_cast<usize>(ed - bg); }
        u32 operator[](usize i) const { return bg[i]; }
        std::vector<u32> to_vec() const { return std::vector<u32>(bg, ed); }
    };

    static auto primes(usize n) {
        if (n >= _lpf.size()) set_upper(n);
        const u32 upper = static_cast<u32>(n);
        return PrimeIt(_primes.begin(), std::upper_bound(_primes.begin(), _primes.end(), upper));
    }

    static u32 lpf(u32 n) {
        assert(n > 1);
        if (static_cast<usize>(n) >= _lpf.size()) set_upper(static_cast<usize>(n));
        return _lpf[n];
    }

    static bool isprime(u32 n) {
        assert(n > 0);
        if (static_cast<usize>(n) >= _lpf.size()) set_upper(static_cast<usize>(n));
        return n != 1 and _lpf[n] == n;
    }
};

} // namespace kk2


#endif // KK2_MATH_LPF_TABLE_HPP
#ifndef KK2_MATH_POW_HPP
#define KK2_MATH_POW_HPP 1


namespace kk2 {

template <class S, class T, class U> constexpr S pow(T x, U n) {
    assert(n >= 0);
    S r = 1, y = x;
    while (n) {
        if (n & 1) r *= y;
        if (n >>= 1) y *= y;
    }
    return r;
}

} // namespace kk2

#endif // KK2_MATH_POW_HPP

namespace kk2 {

/**
 * @brief Compute the values `0^e, 1^e, ..., n^e.`
 *
 * As usual for formal power series and modular arithmetic, 0^0 is defined
 * to be 1.
 */
template <class T> std::vector<T> pow_table(u32 e, u32 n) {
    std::vector<T> result(static_cast<usize>(n) + 1);

    result[0] = (e == 0 ? T(1) : T(0));
    if (n == 0) return result;
    result[1] = T(1);
    if (n == 1) return result;

    LPFTable::set_upper(static_cast<usize>(n));
    for (usize index = 2; index <= n; ++index) {
        const u32 i = static_cast<u32>(index);
        const u32 p = LPFTable::lpf(i);
        if (p == i) {
            result[index] = pow<T>(T(i), e);
        } else {
            result[index] = result[index / p] * result[p];
        }
    }
    return result;
}

} // namespace kk2

#endif // KK2_MATH_MULTIPLICATIVE_FUNCTION_POW_TABLE_HPP
#ifndef KK2_MATH_MOD_COMB_HPP
#define KK2_MATH_MOD_COMB_HPP 1


#ifndef KK2_COMMON_TYPE_ALIAS_HPP
#define KK2_COMMON_TYPE_ALIAS_HPP 1


namespace kk2 {

using usize = std::size_t;
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;

#ifndef _MSC_VER
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

} // namespace kk2

#endif // KK2_COMMON_TYPE_ALIAS_HPP
#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
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

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP
#ifndef KK2_TYPE_TRAITS_MODINT_HPP
#define KK2_TYPE_TRAITS_MODINT_HPP 1



namespace kk2::modint {

template <class M>
concept Modular = requires(M x) {
    requires Integral<decltype(M::getmod())>;
    x.val();
    { x.inv() } -> std::same_as<M>;
};

} // namespace kk2::modint

#endif // KK2_TYPE_TRAITS_MODINT_HPP
#ifndef KK2_MATH_MOD_INV_TABLE_HPP
#define KK2_MATH_MOD_INV_TABLE_HPP 1



namespace kk2 {

/**
 * @brief `[1, n]`のmod逆元を列挙するテーブル
 *
 * @tparam mint
 */
template <class mint> struct InvTable {
    static inline std::vector<mint> _invs{0, 1};
    InvTable() = delete;

    static void set_upper(usize m) {
        if (_invs.size() > m) return;
        using index_type = std::make_unsigned_t<std::remove_cv_t<decltype(mint::getmod())>>;
        const index_type start = static_cast<index_type>(_invs.size());
        const index_type upper = static_cast<index_type>(m);
        const index_type mod = static_cast<index_type>(mint::getmod());
        _invs.resize(m + 1);
        // p = q * i + r
        // - q / r = 1 / i (mod p)
        for (index_type i = start; i <= upper; ++i)
            _invs[i] = (-_invs[mod % i]) * (mod / i);
    }

    template <UnsignedIntegral T> static inline mint inv(T n) {
        const usize index = static_cast<usize>(n);
        if (index >= _invs.size()) set_upper(index);
        return _invs[index];
    }

    template <SignedIntegral T> static inline mint inv(T n) {
        using U = std::make_unsigned_t<T>;
        const U un = static_cast<U>(n);
        if (n < 0) return -inv(U(0) - un);
        return inv(un);
    }

    static inline mint inv_unchecked(usize n) {
        return _invs[n];
    }
};

} // namespace kk2

#endif // KK2_MATH_MOD_INV_TABLE_HPP

namespace kk2 {

template <modint::Modular mint> struct Comb {
    static inline std::vector<mint> _fact{1}, _ifact{1};

    Comb() = delete;

  private:
    static void extend(usize m) {
        const usize n = _fact.size();
        m = std::min<usize>(m, mint::getmod() - 1);
        _fact.reserve(m + 1);
        _ifact.resize(m + 1);
        auto &_invs = InvTable<mint>::_invs;
        if (_invs.size() <= m) _invs.resize(m + 1);
        for (usize i = n; i <= m; i++) _fact.emplace_back(_fact.back() * i);
        _ifact[m] = _fact[m].inv();
        _invs[m] = _ifact[m] * _fact[m - 1];
        for (usize i = m; i > n; i--) {
            _ifact[i - 1] = _ifact[i] * i;
            _invs[i - 1] = _ifact[i - 1] * _fact[i - 2];
        }
    }

    static void ensure(usize n) {
        assert(n < mint::getmod());
        if (_fact.size() > n) return;
        extend(std::max<usize>(n, _fact.size() * 2));
    }

  public:
    static void set_upper(usize n) { ensure(n); }

    static mint fact(u32 n) {
        ensure(static_cast<usize>(n));
        return _fact[n];
    }

    static mint ifact(u32 n) {
        ensure(static_cast<usize>(n));
        return _ifact[n];
    }

    static mint inv(i32 n) {
        assert(n != 0);
        return InvTable<mint>::inv(n);
    }

    static mint binom(u32 n, u32 k) {
        if (k > n) return 0;
        return fact(n) * ifact(k) * ifact(n - k);
    }

    template <UnsignedIntegral T> static mint multinomial(const std::vector<T> &r) {
        u64 n = 0;
        for (const T x : r) n += x;
        assert(n < mint::getmod());
        mint res = fact(static_cast<u32>(n));
        for (const T x : r) res *= ifact(static_cast<u32>(x));
        return res;
    }

    static mint binom_naive(u32 n, u32 k) {
        if (k > n) return 0;
        mint res = 1;
        k = std::min(k, n - k);
        for (u32 i = 1; i <= k; i++) res *= inv(i) * (n--);
        return res;
    }

    static mint permu(u32 n, u32 k) {
        if (k > n) return 0;
        return fact(n) * ifact(n - k);
    }

    static mint homo(u32 n, u32 k) { return k == 0 ? 1 : binom(n + k - 1, k); }
};

} // namespace kk2

#endif // KK2_MATH_MOD_COMB_HPP
#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
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

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP
#ifndef KK2_TYPE_TRAITS_MODINT_HPP
#define KK2_TYPE_TRAITS_MODINT_HPP 1



namespace kk2::modint {

template <class M>
concept Modular = requires(M x) {
    requires Integral<decltype(M::getmod())>;
    x.val();
    { x.inv() } -> std::same_as<M>;
};

} // namespace kk2::modint

#endif // KK2_TYPE_TRAITS_MODINT_HPP

namespace kk2 {

/**
 * @brief Return `sum_{i=0}^{n-1} i^k`.
 * @see https://kk2a.github.io/library/math_mod/power_sum.hpp.html#sum-of-monomial
 */
template <modint::Modular mint, UnsignedIntegral T1, UnsignedIntegral T2>
mint sum_of_monomial(T1 n, T2 k) {
    if (n == 0) return 0;
    if (k == 0) return mint(n);
    std::vector<mint> value = pow_table<mint>(k, k);
    std::vector<mint> sum(static_cast<usize>(k) + 2);
    for (usize i = 0; i <= k; ++i) sum[i + 1] = sum[i] + value[i];
    return sample_point_evaluate(sum, mint(n));
}

/**
 * @brief Return `sum_{i=0}^{n-1} f(i)` from the samples `f(0), ..., f(k)`.
 * @see https://kk2a.github.io/library/math_mod/power_sum.hpp.html#sum-of-polynomial-samples
 */
template <modint::Modular mint, UnsignedIntegral T>
mint sum_of_polynomial_samples(const std::vector<mint> &samples, T n) {
    assert(!samples.empty());
    assert(samples.size() < static_cast<usize>(mint::getmod()));

    if (n <= samples.size()) {
        return std::ranges::fold_left(
            samples | std::views::take(static_cast<usize>(n)), mint(0), std::plus{});
    }

    std::vector<mint> prefix_sum(samples.size() + 1);
    for (usize i = 0; i < samples.size(); ++i) prefix_sum[i + 1] = prefix_sum[i] + samples[i];

    return sample_point_evaluate(prefix_sum, mint(n));
}

/**
 * @brief Return the rational-function value of `sum_{i=0}^infty r^i f(i)` from its samples.
 * @see https://kk2a.github.io/library/math_mod/power_sum.hpp.html#sum-of-geometric-polynomial-samples-infinite
 */
template <modint::Modular mint>
mint sum_of_geometric_polynomial_samples(mint r, const std::vector<mint> &samples) {
    assert(!samples.empty());
    assert(samples.size() < static_cast<usize>(mint::getmod()));
    assert(r != mint(1));
    if (r == mint(0)) return samples[0];

    const usize k = samples.size() - 1;
    const usize m = k + 1;
    InvTable<mint>::set_upper(m);

    const mint minus_r = -r;
    const mint inverse_minus_r = minus_r.inv();
    mint denominator_coefficient = mint(m) * minus_r.pow(k);
    mint denominator_prefix = (mint(1) - r).pow(m) - minus_r.pow(m);
    mint r_power = 1;
    mint numerator = 0;
    for (usize i = 0; i <= k; ++i, r_power *= r) {
        numerator += samples[i] * r_power * denominator_prefix;
        if (i != k) {
            const usize j = k - i;
            denominator_prefix -= denominator_coefficient;
            denominator_coefficient *=
                mint(j) * InvTable<mint>::inv_unchecked(m - j + 1) * inverse_minus_r;
        }
    }
    return numerator / (mint(1) - r).pow(m);
}

/**
 * @brief Return `sum_{i=0}^{n-1} r^i f(i)` from the samples `f(0), ..., f(k)`.
 * @see https://kk2a.github.io/library/math_mod/power_sum.hpp.html#sum-of-geometric-polynomial-samples-finite
 */
template <modint::Modular mint, UnsignedIntegral T>
mint sum_of_geometric_polynomial_samples(mint r, T n, const std::vector<mint> &samples) {
    assert(!samples.empty());
    assert(samples.size() < static_cast<usize>(mint::getmod()));
    if (n == 0) return mint(0);
    if (r == mint(0)) return samples[0];
    if (r == mint(1)) return sum_of_polynomial_samples<mint>(samples, n);

    if (n <= samples.size()) {
        mint res = 0;
        mint rp = 1;
        for (usize i = 0; i < static_cast<usize>(n); ++i, rp *= r) res += rp * samples[i];
        return res;
    }

    Comb<mint>::set_upper(samples.size());
    const mint infinite_sum = sum_of_geometric_polynomial_samples(r, samples);
    const mint ir = r.inv();
    std::vector<mint> tail_samples(samples.size());
    tail_samples[0] = -infinite_sum;
    for (usize i = 0; i + 1 < samples.size(); ++i) {
        tail_samples[i + 1] = ir * (tail_samples[i] + samples[i]);
    }

    return infinite_sum + r.pow(n) * sample_point_evaluate(tail_samples, mint(n));
}

/**
 * @brief Return `sum_{i=0}^{n-1} r^i i^k`.
 * @see https://kk2a.github.io/library/math_mod/power_sum.hpp.html#sum-of-geometric-monomial-finite
 */
template <modint::Modular mint, UnsignedIntegral T1, UnsignedIntegral T2>
mint sum_of_geometric_monomial(mint r, T1 n, T2 k) {
    return sum_of_geometric_polynomial_samples(r, n, pow_table<mint>(k, k));
}

/**
 * @brief Return the rational-function value of `sum_{i=0}^infty r^i i^k`.
 * @see https://kk2a.github.io/library/math_mod/power_sum.hpp.html#sum-of-geometric-monomial-infinite
 */
template <modint::Modular mint, UnsignedIntegral T>
mint sum_of_geometric_monomial(mint r, T k) {
    return sum_of_geometric_polynomial_samples(r, pow_table<mint>(k, k));
}

} // namespace kk2

#endif // KK2_MATH_MOD_POWER_SUM_HPP
#ifndef KK2_MODINT_MONT_HPP
#define KK2_MODINT_MONT_HPP 1


#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
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

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP
#ifndef KK2_TYPE_TRAITS_IO_HPP
#define KK2_TYPE_TRAITS_IO_HPP 1


namespace kk2 {

namespace type_traits {

struct istream_tag {};
struct ostream_tag {};

} // namespace type_traits

template <typename T>
using is_standard_istream = typename std::conditional<std::is_same<T, std::istream>::value
                                                          || std::is_same<T, std::ifstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T>
using is_standard_ostream = typename std::conditional<std::is_same<T, std::ostream>::value
                                                          || std::is_same<T, std::ofstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T> using is_user_defined_istream = std::is_base_of<type_traits::istream_tag, T>;
template <typename T> using is_user_defined_ostream = std::is_base_of<type_traits::ostream_tag, T>;

template <typename T>
using is_istream =
    typename std::conditional<is_standard_istream<T>::value || is_user_defined_istream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_ostream =
    typename std::conditional<is_standard_ostream<T>::value || is_user_defined_ostream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_istream_t = std::enable_if_t<is_istream<T>::value>;
template <typename T> using is_ostream_t = std::enable_if_t<is_ostream<T>::value>;

template <class T>
concept StandardInputStream = is_standard_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept StandardOutputStream = is_standard_ostream<std::remove_cvref_t<T>>::value;

template <class T>
concept InputStream = is_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept OutputStream = is_ostream<std::remove_cvref_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_IO_HPP

namespace kk2 {

template <int p> struct LazyMontgomeryModInt {
    using mint = LazyMontgomeryModInt;
    using i32 = int32_t;
    using i64 = int64_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    static constexpr u32 get_r() {
        u32 ret = p;
        for (int i = 0; i < 4; ++i) ret *= 2 - p * ret;
        return ret;
    }

    static constexpr u32 r = get_r();
    static constexpr u32 n2 = -u64(p) % p;
    static_assert(r * p == 1, "invalid, r * p != 1");
    static_assert(p < (1 << 30), "invalid, p >= 2 ^ 30");
    static_assert((p & 1) == 1, "invalid, p % 2 == 0");

    u32 _v;

    constexpr LazyMontgomeryModInt() : _v(0) {}

    template <Integral T> constexpr LazyMontgomeryModInt(T b) : _v(reduce(u64(b % p + p) * n2)) {}

    static constexpr u32 reduce(const u64 &b) { return (b + u64(u32(b) * u32(-r)) * p) >> 32; }
    constexpr mint &operator++() { return *this += 1; }
    constexpr mint &operator--() { return *this -= 1; }

    constexpr mint operator++(int) {
        mint ret = *this;
        *this += 1;
        return ret;
    }

    constexpr mint operator--(int) {
        mint ret = *this;
        *this -= 1;
        return ret;
    }

    constexpr mint &operator+=(const mint &b) {
        if (i32(_v += b._v - 2 * p) < 0) _v += 2 * p;
        return *this;
    }

    constexpr mint &operator-=(const mint &b) {
        if (i32(_v -= b._v) < 0) _v += 2 * p;
        return *this;
    }

    constexpr mint &operator*=(const mint &b) {
        _v = reduce(u64(_v) * b._v);
        return *this;
    }

    constexpr mint &operator/=(const mint &b) {
        *this *= b.inv();
        return *this;
    }


    constexpr bool operator==(const mint &b) const {
        return (_v >= p ? _v - p : _v) == (b._v >= p ? b._v - p : b._v);
    }

    constexpr bool operator!=(const mint &b) const {
        return (_v >= p ? _v - p : _v) != (b._v >= p ? b._v - p : b._v);
    }

    constexpr mint operator-() const { return mint() - mint(*this); }
    constexpr mint operator+() const { return mint(*this); }
    friend constexpr mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
    friend constexpr mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
    friend constexpr mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
    friend constexpr mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }

    template <class T> constexpr mint pow(T n) const {
        mint ret(1), mul(*this);
        while (n > 0) {
            if (n & 1) ret *= mul;
            if (n >>= 1) mul *= mul;
        }
        return ret;
    }

    constexpr mint inv() const {
        assert(*this != mint(0));
        return pow(p - 2);
    }

    template <OutputStream OStream> friend OStream &operator<<(OStream &os, const mint &x) {
        return os << x.val();
    }

    template <InputStream IStream> friend IStream &operator>>(IStream &is, mint &x) {
        i64 t;
        is >> t;
        x = mint(t);
        return (is);
    }

    constexpr u32 val() const {
        u32 ret = reduce(_v);
        return ret >= p ? ret - p : ret;
    }

    static constexpr u32 getmod() { return p; }
};

template <int p> using Mont = LazyMontgomeryModInt<p>;

using mont998 = Mont<998244353>;
using mont107 = Mont<1000000007>;

} // namespace kk2

#endif // KK2_MODINT_MONT_HPP
#ifndef KK2_TEMPLATE_TEMPLATE_HPP
#define KK2_TEMPLATE_TEMPLATE_HPP 1


#ifndef KK2_TEMPLATE_CONSTANT_HPP
#define KK2_TEMPLATE_CONSTANT_HPP 1

#ifndef KK2_TEMPLATE_TYPE_ALIAS_HPP
#define KK2_TEMPLATE_TYPE_ALIAS_HPP 1


using i32 = int;
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
#ifndef KK2_TEMPLATE_FASTIO_HPP
#define KK2_TEMPLATE_FASTIO_HPP 1


#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
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

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP
#ifndef KK2_TYPE_TRAITS_IO_HPP
#define KK2_TYPE_TRAITS_IO_HPP 1


namespace kk2 {

namespace type_traits {

struct istream_tag {};
struct ostream_tag {};

} // namespace type_traits

template <typename T>
using is_standard_istream = typename std::conditional<std::is_same<T, std::istream>::value
                                                          || std::is_same<T, std::ifstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T>
using is_standard_ostream = typename std::conditional<std::is_same<T, std::ostream>::value
                                                          || std::is_same<T, std::ofstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T> using is_user_defined_istream = std::is_base_of<type_traits::istream_tag, T>;
template <typename T> using is_user_defined_ostream = std::is_base_of<type_traits::ostream_tag, T>;

template <typename T>
using is_istream =
    typename std::conditional<is_standard_istream<T>::value || is_user_defined_istream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_ostream =
    typename std::conditional<is_standard_ostream<T>::value || is_user_defined_ostream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_istream_t = std::enable_if_t<is_istream<T>::value>;
template <typename T> using is_ostream_t = std::enable_if_t<is_ostream<T>::value>;

template <class T>
concept StandardInputStream = is_standard_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept StandardOutputStream = is_standard_ostream<std::remove_cvref_t<T>>::value;

template <class T>
concept InputStream = is_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept OutputStream = is_ostream<std::remove_cvref_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_IO_HPP

namespace kk2 {

namespace fastio {

struct Scanner : type_traits::istream_tag {
  private:
    static constexpr size_t INPUT_BUF = 1 << 17;
    size_t pos = 0, end = 0;
    bool is_eof = false;
    static char buf[INPUT_BUF];
    FILE *fp;

  public:
    Scanner() : fp(stdin) {}

    Scanner(const char *file) : fp(fopen(file, "r")) {}

    ~Scanner() {
        if (fp != stdin) fclose(fp);
    }

    char now() {
        if (is_eof) return '\0';
        if (pos == end) {
            end = fread(buf, 1, INPUT_BUF, fp);
            if (end != INPUT_BUF) buf[end] = '\0';
            if (end == 0) is_eof = true;
            pos = 0;
        }
        return buf[pos];
    }

    void skip_space() {
        while (isspace(now())) ++pos;
    }

    template <UnsignedIntegral T> T next_unsigned_integral() {
        skip_space();
        T res{};
        while (isdigit(now())) {
            res = res * 10 + (now() - '0');
            ++pos;
        }
        return res;
    }

    template <SignedIntegral T> T next_signed_integral() {
        skip_space();
        if (now() == '-') {
            ++pos;
            return T(-next_unsigned_integral<typename to_unsigned<T>::type>());
        } else return (T)next_unsigned_integral<typename to_unsigned<T>::type>();
    }

    char next_char() {
        skip_space();
        auto res = now();
        ++pos;
        return res;
    }

    std::string next_string() {
        skip_space();
        std::string res;
        while (true) {
            char c = now();
            if (isspace(c) or c == '\0') break;
            res.push_back(now());
            ++pos;
        }
        return res;
    }

    template <UnsignedIntegral T> Scanner &operator>>(T &x) {
        x = next_unsigned_integral<T>();
        return *this;
    }

    template <SignedIntegral T> Scanner &operator>>(T &x) {
        x = next_signed_integral<T>();
        return *this;
    }

    Scanner &operator>>(char &x) {
        x = next_char();
        return *this;
    }

    Scanner &operator>>(std::string &x) {
        x = next_string();
        return *this;
    }
};

struct endl_struct_t {};

struct Printer : type_traits::ostream_tag {
  private:
    static char helper[10000][5];
    static char leading_zero[10000][5];
    constexpr static size_t OUTPUT_BUF = 1 << 17;
    static char buf[OUTPUT_BUF];
    size_t pos = 0;
    FILE *fp;

    template <class T> static constexpr void div_mod(T &a, T &b, T mod) {
        a = b / mod;
        b -= a * mod;
    }

    static void init() {
        buf[0] = '\0';
        for (size_t i = 0; i < 10000; ++i) {
            leading_zero[i][0] = i / 1000 + '0';
            leading_zero[i][1] = i / 100 % 10 + '0';
            leading_zero[i][2] = i / 10 % 10 + '0';
            leading_zero[i][3] = i % 10 + '0';
            leading_zero[i][4] = '\0';

            size_t j = 0;
            if (i >= 1000) helper[i][j++] = i / 1000 + '0';
            if (i >= 100) helper[i][j++] = i / 100 % 10 + '0';
            if (i >= 10) helper[i][j++] = i / 10 % 10 + '0';
            helper[i][j++] = i % 10 + '0';
            helper[i][j] = '\0';
        }
    }

  public:
    Printer() : fp(stdout) { init(); }

    Printer(const char *file) : fp(fopen(file, "w")) { init(); }

    ~Printer() {
        write();
        if (fp != stdout) fclose(fp);
    }

    void write() {
        fwrite(buf, 1, pos, fp);
        pos = 0;
    }

    void flush() {
        write();
        fflush(fp);
    }

    void put_char(char c) {
        if (pos == OUTPUT_BUF) write();
        buf[pos++] = c;
    }

    void put_cstr(const char *s) {
        while (*s) put_char(*(s++));
    }

    void put_u32(uint32_t x) {
        uint32_t y;
        if (x >= 100000000) { // 10^8
            div_mod<uint32_t>(y, x, 100000000);
            put_cstr(helper[y]);
            div_mod<uint32_t>(y, x, 10000);
            put_cstr(leading_zero[y]);
            put_cstr(leading_zero[x]);
        } else if (x >= 10000) { // 10^4
            div_mod<uint32_t>(y, x, 10000);
            put_cstr(helper[y]);
            put_cstr(leading_zero[x]);
        } else put_cstr(helper[x]);
    }

    void put_i32(int32_t x) {
        if (x < 0) {
            put_char('-');
            put_u32(-x);
        } else put_u32(x);
    }

    void put_u64(uint64_t x) {
        uint64_t y;
        if (x >= 1000000000000ull) { // 10^12
            div_mod<uint64_t>(y, x, 1000000000000ull);
            put_u32(y);
            div_mod<uint64_t>(y, x, 100000000ull);
            put_cstr(leading_zero[y]);
            div_mod<uint64_t>(y, x, 10000ull);
            put_cstr(leading_zero[y]);
            put_cstr(leading_zero[x]);
        } else if (x >= 10000ull) { // 10^4
            div_mod<uint64_t>(y, x, 10000ull);
            put_u32(y);
            put_cstr(leading_zero[x]);
        } else put_cstr(helper[x]);
    }

    void put_i64(int64_t x) {
        if (x < 0) {
            put_char('-');
            put_u64(-x);
        } else put_u64(x);
    }

    void put_u128(__uint128_t x) {
        constexpr static __uint128_t pow10_10 = 10000000000ull;
        constexpr static __uint128_t pow10_20 = pow10_10 * pow10_10;

        __uint128_t y;
        if (x >= pow10_20) { // 10^20
            div_mod<__uint128_t>(y, x, pow10_20);
            put_u64(uint64_t(y));
            div_mod<__uint128_t>(y, x, __uint128_t(10000000000000000ull));
            put_cstr(leading_zero[y]);
            div_mod<__uint128_t>(y, x, __uint128_t(1000000000000ull));
            put_cstr(leading_zero[y]);
            div_mod<__uint128_t>(y, x, __uint128_t(100000000ull));
            put_cstr(leading_zero[y]);
            div_mod<__uint128_t>(y, x, __uint128_t(10000ull));
            put_cstr(leading_zero[y]);
            put_cstr(leading_zero[x]);
        } else if (x >= __uint128_t(10000)) { // 10^4
            div_mod<__uint128_t>(y, x, __uint128_t(10000));
            put_u64(uint64_t(y));
            put_cstr(leading_zero[x]);
        } else put_cstr(helper[x]);
    }

    void put_i128(__int128_t x) {
        if (x < 0) {
            put_char('-');
            put_u128(-x);
        } else put_u128(x);
    }

    template <UnsignedIntegral T> Printer &operator<<(T x) {
        if constexpr (sizeof(T) <= 4) put_u32(x);
        else if constexpr (sizeof(T) <= 8) put_u64(x);
        else put_u128(x);
        return *this;
    }

    template <SignedIntegral T> Printer &operator<<(T x) {
        if constexpr (sizeof(T) <= 4) put_i32(x);
        else if constexpr (sizeof(T) <= 8) put_i64(x);
        else put_i128(x);
        return *this;
    }

    Printer &operator<<(char x) {
        put_char(x);
        return *this;
    }

    Printer &operator<<(const std::string &x) {
        for (char c : x) put_char(c);
        return *this;
    }

    Printer &operator<<(const char *x) {
        put_cstr(x);
        return *this;
    }

    // std::cout << std::endl; は関数ポインタを渡しているらしい
    Printer &operator<<(endl_struct_t) {
        put_char('\n');
        flush();
        return *this;
    }
};

char Scanner::buf[Scanner::INPUT_BUF];
char Printer::buf[Printer::OUTPUT_BUF];
char Printer::helper[10000][5];
char Printer::leading_zero[10000][5];

} // namespace fastio

#if defined(INTERACTIVE) || defined(USE_STDIO)
auto &kin = std::cin;
auto &kout = std::cout;
auto (*kendl)(std::ostream &) = std::endl<char, std::char_traits<char>>;
#else
fastio::Scanner kin;
fastio::Printer kout;
fastio::endl_struct_t kendl;
#endif

} // namespace kk2

#endif // KK2_TEMPLATE_FASTIO_HPP
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

template <kk2::InputStream IStream, class T, class U>
IStream &operator>>(IStream &is, std::pair<T, U> &p) {
    impl::read::all_read(is, p);
    return is;
}

template <kk2::InputStream IStream, class T> IStream &operator>>(IStream &is, std::vector<T> &v) {
    impl::read::all_read(is, v);
    return is;
}

template <kk2::InputStream IStream, class T, size_t F>
IStream &operator>>(IStream &is, std::array<T, F> &a) {
    impl::read::all_read(is, a);
    return is;
}

template <kk2::OutputStream OStream, class T, class U>
OStream &operator<<(OStream &os, const std::pair<T, U> &p) {
    impl::write::all_write(os, p);
    return os;
}

template <kk2::OutputStream OStream, class T>
OStream &operator<<(OStream &os, const std::vector<T> &v) {
    impl::write::all_write(os, v);
    return os;
}

template <kk2::OutputStream OStream, class T, size_t F>
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

#endif // KK2_TEMPLATE_MACROS_HPP

using kk2::kendl;
using kk2::kin;
using kk2::kout;

void Yes(bool b = 1) { kout << (b ? "Yes\n" : "No\n"); }
void No(bool b = 1) { kout << (b ? "No\n" : "Yes\n"); }
void YES(bool b = 1) { kout << (b ? "YES\n" : "NO\n"); }
void NO(bool b = 1) { kout << (b ? "NO\n" : "YES\n"); }
void yes(bool b = 1) { kout << (b ? "yes\n" : "no\n"); }
void no(bool b = 1) { kout << (b ? "no\n" : "yes\n"); }
template <class T, class S> inline bool chmax(T &a, const S &b) { return (a < b ? a = b, 1 : 0); }
template <class T, class S> inline bool chmin(T &a, const S &b) { return (a > b ? a = b, 1 : 0); }

#endif // KK2_TEMPLATE_TEMPLATE_HPP

int main() {
    using mint = kk2::mont998;

    mint r;
    u64 d;
    kin >> r >> d;
    kout << kk2::sum_of_geometric_monomial(r, d) << kendl;
    return 0;
}
// Author: kk2
// converted by https://github.com/kk2a/cpp-bundle
// 2026-10-03 02:08:47
