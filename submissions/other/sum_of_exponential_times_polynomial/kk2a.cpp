#include <queue>
#include <cstddef>
#include <fstream>
#include <chrono>
#include <memory>
#include <utility>
#include <type_traits>
#include <map>
#include <ranges>
#include <numeric>
#include <ostream>
#include <string>
#include <array>
#include <concepts>
#include <optional>
#include <deque>
#include <cassert>
#include <cstdint>
#include <set>
#include <limits>
#include <tuple>
#include <cctype>
#include <bit>
#include <algorithm>
#include <bitset>
#include <unordered_map>
#include <istream>
#include <unordered_set>
#include <iterator>
#include <cstdio>
#include <vector>
#include <stack>
#include <functional>
#include <iostream>
#include <random>
#include <cmath>

// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial

#ifndef KK2_FPS_FPS_NTT_FRIENDLY_HPP
#define KK2_FPS_FPS_NTT_FRIENDLY_HPP 1

#ifndef KK2_MATH_MOD_BUTTERFLY_HPP
#define KK2_MATH_MOD_BUTTERFLY_HPP 1


#ifndef KK2_MATH_MOD_PRIMITIVE_ROOT_HPP
#define KK2_MATH_MOD_PRIMITIVE_ROOT_HPP 1

#ifndef KK2_MATH_MOD_POW_MOD_HPP
#define KK2_MATH_MOD_POW_MOD_HPP 1


namespace kk2 {

template <class S, class T, class U> constexpr S pow_mod(T x, U n, T m) {
    assert(n >= 0);
    if (m == 1) return S(0);
    S _m = m, r = 1;
    S y = x % _m;
    if (y < 0) y += _m;
    while (n) {
        if (n & 1) r = (r * y) % _m;
        if (n >>= 1) y = (y * y) % _m;
    }
    return r;
}

} // namespace kk2

#endif // KK2_MATH_MOD_POW_MOD_HPP

namespace kk2 {

constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    if (m == 1107296257) return 10;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) { x /= i; }
        }
    }
    if (x > 1) { divs[cnt++] = x; }
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod<long long>(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

template <int m> static constexpr int primitive_root = primitive_root_constexpr(m);

} // namespace kk2

#endif // KK2_MATH_MOD_PRIMITIVE_ROOT_HPP

namespace kk2 {

namespace detail {

template <class mint> int butterfly_log_size(std::size_t n) {
    assert((n > 0 && (n & (n - 1)) == 0) && "butterfly size must be a power of two");
    assert(n < (std::size_t{1} << 30) && "butterfly size is too large");
    assert((static_cast<std::uintmax_t>(mint::getmod()) - 1) % n == 0
           && "butterfly size is not supported by the modulus");
    return __builtin_ctz(static_cast<unsigned int>(n));
}

} // namespace detail

template <class FPS, class mint = typename FPS::value_type> void butterfly(FPS &a) {
    int h = detail::butterfly_log_size<mint>(a.size());
    static int g = primitive_root<mint::getmod()>;
    static bool first = true;
    static mint sum_e2[30]; // sum_e[i] = ies[0] * ... * ies[i - 1] * es[i]
    static mint sum_e3[30];
    static mint es[30], ies[30]; // es[i]^(2^(2+i)) == 1
    if (first) {
        first = false;
        int cnt2 = __builtin_ctz(mint::getmod() - 1);
        mint e = mint(g).pow((mint::getmod() - 1) >> cnt2), ie = e.inv();
        for (int i = cnt2; i >= 2; i--) {
            // e^(2^i) == 1
            es[i - 2] = e;
            ies[i - 2] = ie;
            e *= e;
            ie *= ie;
        }
        mint now = 1;
        for (int i = 0; i <= cnt2 - 2; i++) {
            sum_e2[i] = es[i] * now;
            now *= ies[i];
        }
        now = 1;
        for (int i = 0; i <= cnt2 - 3; i++) {
            sum_e3[i] = es[i + 1] * now;
            now *= ies[i + 1];
        }
    }

    int len = 0;
    while (len < h) {
        if (h - len == 1) {
            int p = 1 << (h - len - 1);
            mint rot = 1;
            for (int s = 0; s < (1 << len); s++) {
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p] * rot;
                    a[i + offset] = l + r;
                    a[i + offset + p] = l - r;
                }
                if (s + 1 != (1 << len)) rot *= sum_e2[__builtin_ctz(~(unsigned int)(s))];
            }
            len++;
        } else {
            int p = 1 << (h - len - 2);
            mint rot = 1, imag = es[0];
            for (int s = 0; s < (1 << len); s++) {
                mint rot2 = rot * rot;
                mint rot3 = rot2 * rot;
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto a0 = a[i + offset];
                    auto a1 = a[i + offset + p] * rot;
                    auto a2 = a[i + offset + p * 2] * rot2;
                    auto a3 = a[i + offset + p * 3] * rot3;
                    auto a1na3imag = (a1 - a3) * imag;
                    a[i + offset] = a0 + a2 + a1 + a3;
                    a[i + offset + p] = a0 + a2 - a1 - a3;
                    a[i + offset + p * 2] = a0 - a2 + a1na3imag;
                    a[i + offset + p * 3] = a0 - a2 - a1na3imag;
                }
                if (s + 1 != (1 << len)) rot *= sum_e3[__builtin_ctz(~(unsigned int)(s))];
            }
            len += 2;
        }
    }
}

template <class FPS, class mint = typename FPS::value_type> void butterfly_inv(FPS &a) {
    int n = int(a.size());
    int h = detail::butterfly_log_size<mint>(a.size());
    static constexpr int g = primitive_root<mint::getmod()>;
    static bool first = true;
    static mint sum_ie2[30]; // sum_ie[i] = es[0] * ... * es[i - 1] * ies[i]
    static mint sum_ie3[30];
    static mint es[30], ies[30]; // es[i]^(2^(2+i)) == 1
    static mint invn[30];
    if (first) {
        first = false;
        int cnt2 = __builtin_ctz(mint::getmod() - 1);
        mint e = mint(g).pow((mint::getmod() - 1) >> cnt2), ie = e.inv();
        for (int i = cnt2; i >= 2; i--) {
            // e^(2^i) == 1
            es[i - 2] = e;
            ies[i - 2] = ie;
            e *= e;
            ie *= ie;
        }
        mint now = 1;
        for (int i = 0; i <= cnt2 - 2; i++) {
            sum_ie2[i] = ies[i] * now;
            now *= es[i];
        }
        now = 1;
        for (int i = 0; i <= cnt2 - 3; i++) {
            sum_ie3[i] = ies[i + 1] * now;
            now *= es[i + 1];
        }

        invn[0] = 1;
        invn[1] = mint::getmod() / 2 + 1;
        for (int i = 2; i < 30; i++) invn[i] = invn[i - 1] * invn[1];
    }
    int len = h;
    while (len) {
        if (len == 1) {
            int p = 1 << (h - len);
            mint irot = 1;
            for (int s = 0; s < (1 << (len - 1)); s++) {
                int offset = s << (h - len + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p];
                    a[i + offset] = l + r;
                    a[i + offset + p] = (l - r) * irot;
                }
                if (s + 1 != (1 << (len - 1))) irot *= sum_ie2[__builtin_ctz(~(unsigned int)(s))];
            }
            len--;
        } else {
            int p = 1 << (h - len);
            mint irot = 1, iimag = ies[0];
            for (int s = 0; s < (1 << ((len - 2))); s++) {
                mint irot2 = irot * irot;
                mint irot3 = irot2 * irot;
                int offset = s << (h - len + 2);
                for (int i = 0; i < p; i++) {
                    auto a0 = a[i + offset];
                    auto a1 = a[i + offset + p];
                    auto a2 = a[i + offset + p * 2];
                    auto a3 = a[i + offset + p * 3];
                    auto a2na3iimag = (a2 - a3) * iimag;

                    a[i + offset] = a0 + a1 + a2 + a3;
                    a[i + offset + p] = (a0 - a1 + a2na3iimag) * irot;
                    a[i + offset + p * 2] = (a0 + a1 - a2 - a3) * irot2;
                    a[i + offset + p * 3] = (a0 - a1 - a2na3iimag) * irot3;
                }
                if (s + 1 != (1 << (len - 2))) irot *= sum_ie3[__builtin_ctz(~(unsigned int)(s))];
            }
            len -= 2;
        }
    }

    for (int i = 0; i < n; i++) a[i] *= invn[h];
}

template <class FPS, class mint = typename FPS::value_type> void doubling(FPS &a) {
    int n = a.size();
    detail::butterfly_log_size<mint>(a.size() * 2);
    auto b = a;
    int z = 1;
    butterfly_inv(b);
    mint r = 1, zeta = mint(primitive_root<mint::getmod()>).pow((mint::getmod() - 1) / (n << 1));
    for (int i = 0; i < n; i++) {
        b[i] *= r;
        r *= zeta;
    }
    butterfly(b);
    std::copy(b.begin(), b.end(), std::back_inserter(a));
}

} // namespace kk2

#endif // KK2_MATH_MOD_BUTTERFLY_HPP
#ifndef KK2_FPS_FPS_BASE_HPP
#define KK2_FPS_FPS_BASE_HPP 1


#ifndef KK2_MATH_MOD_INV_TABLE_HPP
#define KK2_MATH_MOD_INV_TABLE_HPP 1


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

template <modint::Modular mint> struct Comb;

/**
 * @brief `[1, n]`のmod逆元を列挙するテーブル
 *
 * @tparam mint
 */
template <class mint> struct InvTable {
    static inline std::vector<mint> _invs{0, 1};

  private:
    static inline mint _factorial = 1;

    static void set_factorial(const mint &factorial) {
        _factorial = factorial;
    }

    template <modint::Modular> friend struct Comb;

  public:
    InvTable() = delete;

    static void set_upper(int m) {
        if ((int)_invs.size() > m) return;
        int start = _invs.size();
        _invs.resize(m + 1);
        const mint factorial_start = _factorial;
        for (int i = start; i <= m; ++i) {
            _factorial *= i;
            _invs[i] = _factorial;
        }
        mint inverse_factorial = _factorial.inv();
        for (int i = m; i > start; --i) {
            _invs[i] = inverse_factorial * _invs[i - 1];
            inverse_factorial *= i;
        }
        _invs[start] = inverse_factorial * factorial_start;
    }

    static inline mint inv(int n) {
        bool neg = n < 0;
        if (neg) n = -n;
        if (n >= (int)_invs.size()) set_upper(n);
        return neg ? -_invs[n] : _invs[n];
    }
};

} // namespace kk2

#endif // KK2_MATH_MOD_INV_TABLE_HPP
#ifndef KK2_TYPE_TRAITS_FPS_HPP
#define KK2_TYPE_TRAITS_FPS_HPP 1


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

namespace kk2::fps {

namespace category {

struct arbitrary_modulus {};
struct ntt_friendly_modulus {};

struct ordinary {};
struct exponential_generating {};
struct set_power_series {};

struct univariate {};
struct bivariate {};
struct multivariate {};

} // namespace category

// Compatibility forwarding alias. The canonical modint constraint lives in
// type_traits/modint.hpp; keeping this name avoids breaking existing FPS code.
template <class M>
concept Modular = modint::Modular<M>;

template <class F>
concept FormalPowerSeries = requires(const F &f, int i) {
    typename F::value_type;
    typename F::modulus_category;
    typename F::series_category;
    { f.size() } -> std::integral;
    f[i];
} && std::ranges::range<const F>;

template <class F>
concept NTTFriendlyFormalPowerSeries =
    FormalPowerSeries<F>
    && std::same_as<typename F::modulus_category, category::ntt_friendly_modulus>;

template <class F>
concept ArbitraryModulusFormalPowerSeries =
    FormalPowerSeries<F> && std::same_as<typename F::modulus_category, category::arbitrary_modulus>;

template <class F>
concept OrdinaryFormalPowerSeries =
    FormalPowerSeries<F> && std::same_as<typename F::series_category, category::ordinary>;

template <class F>
concept ExponentialGeneratingFunction =
    FormalPowerSeries<F>
    && std::same_as<typename F::series_category, category::exponential_generating>;

template <class F>
concept SetPowerSeries =
    FormalPowerSeries<F> && std::same_as<typename F::series_category, category::set_power_series>;

template <class F>
concept UnivariateFormalPowerSeries = FormalPowerSeries<F> && requires {
    typename F::variable_category;
} && std::same_as<typename F::variable_category, category::univariate>;

template <class F>
concept ModularUnivariateFormalPowerSeries =
    UnivariateFormalPowerSeries<F> && modint::Modular<typename F::value_type>;

template <class F>
concept UnivariateNTTFriendlyFormalPowerSeries =
    NTTFriendlyFormalPowerSeries<F> && UnivariateFormalPowerSeries<F>;

template <class F>
concept UnivariateArbitraryModulusFormalPowerSeries =
    ArbitraryModulusFormalPowerSeries<F> && UnivariateFormalPowerSeries<F>;

template <class F>
concept BivariateFormalPowerSeries = FormalPowerSeries<F> && requires {
    typename F::variable_category;
} && std::same_as<typename F::variable_category, category::bivariate>;

template <class F>
concept MultivariateFormalPowerSeries = FormalPowerSeries<F> && requires {
    typename F::variable_category;
} && std::same_as<typename F::variable_category, category::multivariate>;

// Short names for the categories that are commonly used in algorithms.
template <class F>
concept SPS = SetPowerSeries<F>;

template <class F>
concept EGF = ExponentialGeneratingFunction<F>;

template <class F>
concept Bivariate = BivariateFormalPowerSeries<F>;

template <class F>
concept Multivariate = MultivariateFormalPowerSeries<F>;

} // namespace kk2::fps

#endif // KK2_TYPE_TRAITS_FPS_HPP
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
#ifndef KK2_FPS_OPERATIONS_DIVISION_HPP
#define KK2_FPS_OPERATIONS_DIVISION_HPP 1


#ifndef KK2_TYPE_TRAITS_FPS_HPP
#define KK2_TYPE_TRAITS_FPS_HPP 1


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

namespace kk2::fps {

namespace category {

struct arbitrary_modulus {};
struct ntt_friendly_modulus {};

struct ordinary {};
struct exponential_generating {};
struct set_power_series {};

struct univariate {};
struct bivariate {};
struct multivariate {};

} // namespace category

// Compatibility forwarding alias. The canonical modint constraint lives in
// type_traits/modint.hpp; keeping this name avoids breaking existing FPS code.
template <class M>
concept Modular = modint::Modular<M>;

template <class F>
concept FormalPowerSeries = requires(const F &f, int i) {
    typename F::value_type;
    typename F::modulus_category;
    typename F::series_category;
    { f.size() } -> std::integral;
    f[i];
} && std::ranges::range<const F>;

template <class F>
concept NTTFriendlyFormalPowerSeries =
    FormalPowerSeries<F>
    && std::same_as<typename F::modulus_category, category::ntt_friendly_modulus>;

template <class F>
concept ArbitraryModulusFormalPowerSeries =
    FormalPowerSeries<F> && std::same_as<typename F::modulus_category, category::arbitrary_modulus>;

template <class F>
concept OrdinaryFormalPowerSeries =
    FormalPowerSeries<F> && std::same_as<typename F::series_category, category::ordinary>;

template <class F>
concept ExponentialGeneratingFunction =
    FormalPowerSeries<F>
    && std::same_as<typename F::series_category, category::exponential_generating>;

template <class F>
concept SetPowerSeries =
    FormalPowerSeries<F> && std::same_as<typename F::series_category, category::set_power_series>;

template <class F>
concept UnivariateFormalPowerSeries = FormalPowerSeries<F> && requires {
    typename F::variable_category;
} && std::same_as<typename F::variable_category, category::univariate>;

template <class F>
concept ModularUnivariateFormalPowerSeries =
    UnivariateFormalPowerSeries<F> && modint::Modular<typename F::value_type>;

template <class F>
concept UnivariateNTTFriendlyFormalPowerSeries =
    NTTFriendlyFormalPowerSeries<F> && UnivariateFormalPowerSeries<F>;

template <class F>
concept UnivariateArbitraryModulusFormalPowerSeries =
    ArbitraryModulusFormalPowerSeries<F> && UnivariateFormalPowerSeries<F>;

template <class F>
concept BivariateFormalPowerSeries = FormalPowerSeries<F> && requires {
    typename F::variable_category;
} && std::same_as<typename F::variable_category, category::bivariate>;

template <class F>
concept MultivariateFormalPowerSeries = FormalPowerSeries<F> && requires {
    typename F::variable_category;
} && std::same_as<typename F::variable_category, category::multivariate>;

// Short names for the categories that are commonly used in algorithms.
template <class F>
concept SPS = SetPowerSeries<F>;

template <class F>
concept EGF = ExponentialGeneratingFunction<F>;

template <class F>
concept Bivariate = BivariateFormalPowerSeries<F>;

template <class F>
concept Multivariate = MultivariateFormalPowerSeries<F>;

} // namespace kk2::fps

#endif // KK2_TYPE_TRAITS_FPS_HPP
#ifndef KK2_FPS_FPS_SPARSITY_DETECTOR_HPP
#define KK2_FPS_FPS_SPARSITY_DETECTOR_HPP 1


namespace kk2 {

enum class FPSOperation {
    CONVOLUTION,
    LOG,
    POWER,
    DIVISION,
    POLYNOMIAL_DIVISION,
    INVERSE,
    EXP,
    SQRT
};

namespace fps::sparsity_detail {

// E(n): the leading FFT evaluation cost, up to the common field-operation
// constant that cancels when dense and sparse leading terms are compared.
inline std::int64_t evaluation_work(int n) {
    if (n <= 1) return 1;
    const unsigned z = std::bit_ceil(static_cast<unsigned>(n));
    return static_cast<std::int64_t>(z) * std::countr_zero(z);
}

inline int transform_size(int n, int m) {
    if (n <= 0 || m <= 0) return 0;
    return static_cast<int>(std::bit_ceil(static_cast<unsigned>(n + m - 1)));
}

inline std::int64_t
convolution_dense_work(int n, int m, int precision, bool same, bool ntt_friendly) {
    n = std::min(n, precision);
    m = std::min(m, precision);
    const int z = transform_size(n, m);
    if (z == 0) return 0;

    // A different pair needs two forward and one inverse transform. Squaring
    // reuses the forward transform and needs only one forward transform.
    const int transforms = same ? 2 : 3;
    // Arbitrary-modulus convolution uses three NTT-friendly moduli.
    const int moduli = ntt_friendly ? 1 : 3;
    return static_cast<std::int64_t>(transforms) * moduli * evaluation_work(z);
}

inline std::int64_t inverse_dense_work(int precision, bool ntt_friendly) {
    if (precision <= 1) return 0;
    const int z = static_cast<int>(std::bit_ceil(static_cast<unsigned>(precision)));
    // NTT-friendly uses five transforms per Newton level, whose geometric
    // sum has leading term 10 E(z). The arbitrary-modulus implementation
    // performs two fresh convolutions per level, giving 60 E(z).
    return (ntt_friendly ? 10 : 60) * evaluation_work(z);
}

inline std::int64_t log_dense_work(int n, int precision, bool ntt_friendly) {
    return inverse_dense_work(precision, ntt_friendly)
           + convolution_dense_work(std::max(0, n - 1), precision, precision, false, ntt_friendly);
}

inline long double exp_dense_work(int precision, bool ntt_friendly) {
    if (precision <= 1) return 0;
    const int z = static_cast<int>(std::bit_ceil(static_cast<unsigned>(precision)));
    if (ntt_friendly) {
        if (precision <= 2) return 0;
        // Bostan--Schost, Theorem 1: (33/2) E(z) + (97/4) z.  Only
        // the leading E(z) term matters for the sparsity threshold.
        return 16.5L * evaluation_work(z);
    }
    // FPSArb recomputes a logarithm and a product at every Newton level.
    return 192 * evaluation_work(z);
}

inline long double power_dense_work(int n, int precision, bool ntt_friendly) {
    return log_dense_work(n, precision, ntt_friendly) + exp_dense_work(precision, ntt_friendly);
}

inline std::int64_t division_dense_work(int n, int precision, bool ntt_friendly) {
    return inverse_dense_work(precision, ntt_friendly)
           + convolution_dense_work(
               std::min(n, precision), precision, precision, false, ntt_friendly);
}

inline std::int64_t polynomial_division_dense_work(int quotient_size, bool ntt_friendly) {
    return inverse_dense_work(quotient_size, ntt_friendly)
           + convolution_dense_work(
               quotient_size, quotient_size, quotient_size, false, ntt_friendly);
}

inline std::int64_t sqrt_dense_work(int precision, bool ntt_friendly) {
    if (precision <= 1) return 0;
    const int z = static_cast<int>(std::bit_ceil(static_cast<unsigned>(precision)));
    // Newton uses an inverse and one product at every level. The implementation
    // computes the complete next power-of-two block even at the last level.
    return (ntt_friendly ? 32 : 156) * evaluation_work(z);
}

inline long double
sparse_work(FPSOperation op, int target, std::int64_t nonzero_a, std::int64_t nonzero_b) {
    switch (op) {
        case FPSOperation::CONVOLUTION:
            return static_cast<long double>(nonzero_a) * nonzero_b;
        case FPSOperation::DIVISION:
        case FPSOperation::POLYNOMIAL_DIVISION:
            return static_cast<long double>(target) * nonzero_b;
        case FPSOperation::LOG:
        case FPSOperation::POWER:
        case FPSOperation::INVERSE:
        case FPSOperation::EXP:
        case FPSOperation::SQRT:
            return static_cast<long double>(target) * nonzero_a;
    }
    return 0;
}

inline long double sparse_work_constant(FPSOperation op, bool ntt_friendly) {
    // Calibrated against the simplified sparse-work model at degrees 1024
    // and 4096.  Values are rounded upward near the measured crossover so a
    // close decision favors the dense implementation.
    switch (op) {
        case FPSOperation::CONVOLUTION:
            return ntt_friendly ? 0.90L : 0.55L;
        case FPSOperation::DIVISION:
        case FPSOperation::POLYNOMIAL_DIVISION:
            return ntt_friendly ? 0.90L : 0.40L;
        case FPSOperation::LOG:
            return ntt_friendly ? 2.70L : 1.00L;
        case FPSOperation::POWER:
            return ntt_friendly ? 1.35L : 0.70L;
        case FPSOperation::INVERSE:
            return ntt_friendly ? 1.00L : 0.40L;
        case FPSOperation::EXP:
            return ntt_friendly ? 1.05L : 0.45L;
        case FPSOperation::SQRT:
            return ntt_friendly ? 1.40L : 0.65L;
    }
    return 1.00L;
}

} // namespace fps::sparsity_detail

template <class FPS, class mint = typename FPS::value_type>
bool is_sparse_operation(
    FPSOperation op, bool is_ntt_friendly, const FPS &a, const FPS &b = FPS(), int precision = -1) {
    const int n = a.size(), m = b.size();
    if (n + m == 0) return false;

    const bool convolution = op == FPSOperation::CONVOLUTION;
    const bool division = op == FPSOperation::DIVISION;
    const bool polynomial_division = op == FPSOperation::POLYNOMIAL_DIVISION;
    const int requested =
        precision < 0 ? (convolution ? std::max(0, n + m - 1) : n) : std::max(0, precision);
    const int target = convolution ? std::min(requested, std::max(0, n + m - 1)) : requested;
    const int limit_a = convolution                       ? std::min(n, target) :
                        (division || polynomial_division) ? 0 :
                                                            std::min(n, target);
    const int limit_b = convolution         ? std::min(m, target) :
                        division            ? std::min(m, target) :
                        polynomial_division ? m :
                                              0;
    const auto is_nonzero = [](const mint &x) {
        return x != mint(0);
    };
    const std::int64_t nonzero_a = std::ranges::count_if(a | std::views::take(limit_a), is_nonzero);
    const std::int64_t nonzero_b = std::ranges::count_if(b | std::views::take(limit_b), is_nonzero);

    long double dense_work = 0;
    switch (op) {
        case FPSOperation::CONVOLUTION:
            dense_work = fps::sparsity_detail::convolution_dense_work(
                n, m, target, std::addressof(a) == std::addressof(b), is_ntt_friendly);
            break;
        case FPSOperation::LOG:
            dense_work = fps::sparsity_detail::log_dense_work(n, target, is_ntt_friendly);
            break;
        case FPSOperation::POWER:
            dense_work = fps::sparsity_detail::power_dense_work(n, target, is_ntt_friendly);
            break;
        case FPSOperation::DIVISION:
            dense_work = fps::sparsity_detail::division_dense_work(n, target, is_ntt_friendly);
            break;
        case FPSOperation::POLYNOMIAL_DIVISION:
            dense_work =
                fps::sparsity_detail::polynomial_division_dense_work(target, is_ntt_friendly);
            break;
        case FPSOperation::INVERSE:
            dense_work = fps::sparsity_detail::inverse_dense_work(target, is_ntt_friendly);
            break;
        case FPSOperation::EXP:
            dense_work = fps::sparsity_detail::exp_dense_work(target, is_ntt_friendly);
            break;
        case FPSOperation::SQRT:
            dense_work = fps::sparsity_detail::sqrt_dense_work(target, is_ntt_friendly);
            break;
    }

    const long double sparse_work =
        fps::sparsity_detail::sparse_work(op, target, nonzero_a, nonzero_b);
    return dense_work
           > fps::sparsity_detail::sparse_work_constant(op, is_ntt_friendly) * sparse_work;
}

} // namespace kk2

#endif // KK2_FPS_FPS_SPARSITY_DETECTOR_HPP

namespace kk2::fps::operations {

template <UnivariateFormalPowerSeries FPS> FPS division_identity(int precision) {
    using mint = typename FPS::value_type;
    FPS result(precision, mint(0));
    if (precision > 0) result[0] = mint(1);
    return result;
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_dense_div(FPS &dividend, const FPS &divisor, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!divisor.empty() && divisor[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(dividend.size());
    if (std::addressof(dividend) == std::addressof(divisor))
        return dividend = division_identity<FPS>(precision);

    FPS inverse = divisor.dense_inv(precision);
    return dividend.inplace_pre(precision).inplace_dense_mul(inverse).inplace_pre(precision);
}

template <UnivariateFormalPowerSeries FPS>
FPS dense_div(const FPS &dividend, const FPS &divisor, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!divisor.empty() && divisor[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(dividend.size());
    if (std::addressof(dividend) == std::addressof(divisor))
        return division_identity<FPS>(precision);
    FPS result = dividend;
    return inplace_dense_div(result, divisor, precision);
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_sparse_div(FPS &dividend, const FPS &divisor, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!divisor.empty() && divisor[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(dividend.size());
    if (std::addressof(dividend) == std::addressof(divisor))
        return dividend = division_identity<FPS>(precision);

    const mint constant_inv = divisor[0].inv();
    std::vector<std::pair<int, mint>> support;
    for (int i = 1; i < static_cast<int>(divisor.size()); ++i) {
        if (divisor[i] != mint(0)) support.emplace_back(i, divisor[i] * constant_inv);
    }
    dividend *= constant_inv;
    dividend.resize(precision);
    for (int i = 0; i < precision; ++i) {
        for (const auto &[index, coefficient] : support) {
            if (i + index >= precision) break;
            dividend[i + index] -= dividend[i] * coefficient;
        }
    }
    return dividend;
}

template <UnivariateFormalPowerSeries FPS>
FPS sparse_div(const FPS &dividend, const FPS &divisor, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!divisor.empty() && divisor[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(dividend.size());
    if (std::addressof(dividend) == std::addressof(divisor))
        return division_identity<FPS>(precision);
    FPS result = dividend;
    return inplace_sparse_div(result, divisor, precision);
}

template <UnivariateFormalPowerSeries FPS>
FPS div(const FPS &dividend, const FPS &divisor, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!divisor.empty() && divisor[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(dividend.size());
    if (std::addressof(dividend) == std::addressof(divisor))
        return division_identity<FPS>(precision);
    if (is_sparse_operation(FPSOperation::DIVISION,
                            NTTFriendlyFormalPowerSeries<FPS>,
                            dividend,
                            divisor,
                            precision))
        return sparse_div(dividend, divisor, precision);
    return dense_div(dividend, divisor, precision);
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_div(FPS &dividend, const FPS &divisor, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!divisor.empty() && divisor[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(dividend.size());
    if (std::addressof(dividend) == std::addressof(divisor))
        return dividend = division_identity<FPS>(precision);
    const bool use_sparse = is_sparse_operation(
        FPSOperation::DIVISION, NTTFriendlyFormalPowerSeries<FPS>, dividend, divisor, precision);
    if (use_sparse) return inplace_sparse_div(dividend, divisor, precision);
    return inplace_dense_div(dividend, divisor, precision);
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_dense_quo(FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (dividend.size() < divisor.size()) {
        dividend.clear();
        return dividend;
    }
    if (std::addressof(dividend) == std::addressof(divisor)) {
        dividend = FPS{1};
        return dividend;
    }

    const int quotient_size = dividend.size() - divisor.size() + 1;
    FPS reversed_inverse = divisor.rev().dense_inv(quotient_size);
    return dividend.inplace_rev()
        .inplace_pre(quotient_size)
        .inplace_dense_mul(reversed_inverse)
        .inplace_pre(quotient_size)
        .inplace_rev();
}

template <UnivariateFormalPowerSeries FPS> FPS dense_quo(const FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (std::addressof(dividend) == std::addressof(divisor)) return FPS{1};
    FPS result = dividend;
    return inplace_dense_quo(result, divisor);
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_sparse_quo(FPS &dividend, const FPS &divisor) {
    using mint = typename FPS::value_type;
    assert(!divisor.empty());
    if (dividend.size() < divisor.size()) {
        dividend.clear();
        return dividend;
    }
    if (std::addressof(dividend) == std::addressof(divisor)) {
        dividend = FPS{1};
        return dividend;
    }

    const int quotient_size = dividend.size() - divisor.size() + 1;
    const mint leading_inv = divisor.back().inv();
    std::vector<std::pair<int, mint>> support;
    for (int i = static_cast<int>(divisor.size()) - 2; i >= 0; --i) {
        if (divisor[i] != mint(0))
            support.emplace_back(divisor.size() - 1 - i, divisor[i] * leading_inv);
    }
    FPS reversed_quotient(quotient_size);
    for (int k = 0; k < quotient_size; ++k) {
        reversed_quotient[k] = dividend[dividend.size() - 1 - k] * leading_inv;
        for (const auto &[offset, coefficient] : support) {
            if (offset > k) break;
            reversed_quotient[k] -= reversed_quotient[k - offset] * coefficient;
        }
    }
    reversed_quotient.inplace_rev();
    return dividend = std::move(reversed_quotient);
}

template <UnivariateFormalPowerSeries FPS> FPS sparse_quo(const FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (std::addressof(dividend) == std::addressof(divisor)) return FPS{1};
    FPS result = dividend;
    return inplace_sparse_quo(result, divisor);
}

template <UnivariateFormalPowerSeries FPS> FPS quo(const FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (dividend.size() < divisor.size()) return {};
    if (std::addressof(dividend) == std::addressof(divisor)) return FPS{1};
    const int quotient_size = dividend.size() - divisor.size() + 1;
    if (is_sparse_operation(FPSOperation::POLYNOMIAL_DIVISION,
                            NTTFriendlyFormalPowerSeries<FPS>,
                            dividend,
                            divisor,
                            quotient_size))
        return sparse_quo(dividend, divisor);
    return dense_quo(dividend, divisor);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_quo(FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (dividend.size() < divisor.size()) {
        dividend.clear();
        return dividend;
    }
    if (std::addressof(dividend) == std::addressof(divisor)) {
        dividend = FPS{1};
        return dividend;
    }
    const int quotient_size = dividend.size() - divisor.size() + 1;
    const bool use_sparse = is_sparse_operation(FPSOperation::POLYNOMIAL_DIVISION,
                                                NTTFriendlyFormalPowerSeries<FPS>,
                                                dividend,
                                                divisor,
                                                quotient_size);
    if (use_sparse) return inplace_sparse_quo(dividend, divisor);
    return inplace_dense_quo(dividend, divisor);
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_dense_mod(FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (std::addressof(dividend) == std::addressof(divisor)) {
        dividend.clear();
        return dividend;
    }
    FPS quotient = dense_quo(dividend, divisor);
    return (dividend -= quotient.inplace_dense_mul(divisor)).shrink();
}

template <UnivariateFormalPowerSeries FPS> FPS dense_mod(const FPS &dividend, const FPS &divisor) {
    FPS result = dividend;
    return inplace_dense_mod(result, divisor);
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_sparse_mod(FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (std::addressof(dividend) == std::addressof(divisor)) {
        dividend.clear();
        return dividend;
    }
    FPS quotient = sparse_quo(dividend, divisor);
    return (dividend -= quotient.inplace_sparse_mul(divisor)).shrink();
}

template <UnivariateFormalPowerSeries FPS> FPS sparse_mod(const FPS &dividend, const FPS &divisor) {
    FPS result = dividend;
    return inplace_sparse_mod(result, divisor);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_mod(FPS &dividend, const FPS &divisor) {
    assert(!divisor.empty());
    if (std::addressof(dividend) == std::addressof(divisor)) {
        dividend.clear();
        return dividend;
    }
    if (dividend.size() < divisor.size()) return dividend.shrink();
    const int quotient_size = dividend.size() - divisor.size() + 1;
    const bool use_sparse = is_sparse_operation(FPSOperation::POLYNOMIAL_DIVISION,
                                                NTTFriendlyFormalPowerSeries<FPS>,
                                                dividend,
                                                divisor,
                                                quotient_size);
    if (use_sparse) return inplace_sparse_mod(dividend, divisor);
    return inplace_dense_mod(dividend, divisor);
}

template <UnivariateFormalPowerSeries FPS> FPS mod(const FPS &dividend, const FPS &divisor) {
    FPS result = dividend;
    return inplace_mod(result, divisor);
}

} // namespace kk2::fps::operations

#endif // KK2_FPS_OPERATIONS_DIVISION_HPP
#ifndef KK2_FPS_OPERATIONS_EXPONENTIAL_HPP
#define KK2_FPS_OPERATIONS_EXPONENTIAL_HPP 1



namespace kk2::fps::operations {

template <UnivariateNTTFriendlyFormalPowerSeries FPS>
FPS dense_exp(const FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(f.empty() || f[0] == mint(0));
    if (precision == -1) precision = static_cast<int>(f.size());

    FPS result{1, 1 < static_cast<int>(f.size()) ? f[1] : mint(0)};
    FPS inverse{1}, transformed_inverse, previous_transformed_inverse{1, 1};
    for (int m = 2; m < precision; m <<= 1) {
        FPS transformed_result = result;
        transformed_result.resize(m << 1);
        transformed_result.but();
        transformed_inverse = previous_transformed_inverse;
        FPS correction(m);
        correction = transformed_result.dot(transformed_inverse);
        correction.ibut();
        std::fill_n(correction.begin(), m >> 1, mint(0));
        correction.but();
        correction.inplace_dot(-transformed_inverse);
        correction.ibut();
        inverse.insert(inverse.end(), correction.begin() + (m >> 1), correction.end());
        previous_transformed_inverse = inverse;
        previous_transformed_inverse.resize(m << 1);
        previous_transformed_inverse.but();

        FPS delta(f.begin(), f.begin() + std::min(static_cast<int>(f.size()), m));
        delta.resize(m);
        delta.inplace_diff();
        delta.push_back(mint(0));
        delta.but();
        delta.inplace_dot(transformed_result);
        delta.ibut();
        delta -= result.diff();
        delta.resize(m << 1);
        std::copy_n(delta.begin(), m - 1, delta.begin() + m);
        std::fill_n(delta.begin(), m - 1, mint(0));
        delta.but();
        delta.inplace_dot(previous_transformed_inverse);
        delta.ibut();
        delta.pop_back();
        delta.inplace_int();
        for (int i = m; i < std::min(static_cast<int>(f.size()), m << 1); ++i) delta[i] += f[i];
        std::fill_n(delta.begin(), m, mint(0));
        delta.but();
        delta.inplace_dot(transformed_result);
        delta.ibut();
        result.insert(result.end(), delta.begin() + m, delta.end());
    }
    return FPS(result.begin(), result.begin() + precision);
}

template <UnivariateArbitraryModulusFormalPowerSeries FPS>
FPS dense_exp(const FPS &f, int precision = -1);

template <UnivariateFormalPowerSeries FPS> FPS &inplace_dense_exp(FPS &f, int precision = -1) {
    if (precision == -1) precision = static_cast<int>(f.size());
    return f = dense_exp(std::as_const(f), precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_sparse_exp(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(f.empty() || f[0] == mint(0));
    if (precision == -1) precision = static_cast<int>(f.size());

    std::vector<std::pair<int, mint>> support;
    for (int i = 1; i < static_cast<int>(f.size()); ++i) {
        if (f[i] != mint(0)) support.emplace_back(i, f[i] * i);
    }

    const int mod = mint::getmod();
    static std::vector<mint> inverse{1, 1};
    const int old_size = inverse.size();
    inverse.resize(std::max(old_size, precision + 1));
    for (int i = old_size; i <= precision; ++i) inverse[i] = -inverse[mod % i] * (mod / i);

    f.assign(precision, mint(0));
    if (precision > 0) f[0] = mint(1);
    for (int k = 0; k < precision - 1; ++k) {
        for (const auto &[index, derivative_coefficient] : support) {
            const int derivative_index = index - 1;
            if (k < derivative_index) break;
            f[k + 1] += f[k - derivative_index] * derivative_coefficient;
        }
        f[k + 1] *= inverse[k + 1];
    }
    return f;
}

template <UnivariateFormalPowerSeries FPS> FPS sparse_exp(const FPS &f, int precision = -1) {
    FPS result = f;
    return inplace_sparse_exp(result, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS exp(const FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(f.empty() || f[0] == mint(0));
    if (is_sparse_operation(
            FPSOperation::EXP, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision))
        return sparse_exp(f, precision);
    return dense_exp(f, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_exp(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(f.empty() || f[0] == mint(0));
    const bool use_sparse = is_sparse_operation(
        FPSOperation::EXP, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision);
    if (use_sparse) return inplace_sparse_exp(f, precision);
    return inplace_dense_exp(f, precision);
}

} // namespace kk2::fps::operations

#endif // KK2_FPS_OPERATIONS_EXPONENTIAL_HPP
#ifndef KK2_FPS_OPERATIONS_INVERSE_HPP
#define KK2_FPS_OPERATIONS_INVERSE_HPP 1



namespace kk2::fps::operations {

template <UnivariateNTTFriendlyFormalPowerSeries FPS>
FPS dense_inv(const FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!f.empty() && f[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(f.size());

    FPS result(precision);
    if (precision == 0) return result;
    result[0] = mint(1) / f[0];
    for (int d = 1; d < precision; d <<= 1) {
        FPS lhs(2 * d), rhs(2 * d);
        std::copy_n(f.begin(), std::min(static_cast<int>(f.size()), 2 * d), lhs.begin());
        std::copy_n(result.begin(), d, rhs.begin());
        lhs.but();
        rhs.but();
        lhs.inplace_dot(rhs);
        lhs.ibut();
        std::fill_n(lhs.begin(), d, mint(0));
        lhs.but();
        lhs.inplace_dot(rhs);
        lhs.ibut();
        const int next_precision = std::min(2 * d, precision);
        std::transform(lhs.begin() + d,
                       lhs.begin() + next_precision,
                       result.begin() + d,
                       [](const mint &coefficient) { return -coefficient; });
    }
    return result;
}

template <UnivariateArbitraryModulusFormalPowerSeries FPS>
FPS dense_inv(const FPS &f, int precision = -1);

template <UnivariateFormalPowerSeries FPS> FPS &inplace_dense_inv(FPS &f, int precision = -1) {
    if (precision == -1) precision = static_cast<int>(f.size());
    return f = dense_inv(std::as_const(f), precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_sparse_inv(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!f.empty() && f[0] != mint(0));
    if (precision == -1) precision = static_cast<int>(f.size());

    std::vector<std::pair<int, mint>> support;
    for (int i = 1; i < static_cast<int>(f.size()); ++i) {
        if (f[i] != mint(0)) support.emplace_back(i, f[i]);
    }
    const mint constant_inv = f[0].inv();
    f.resize(precision);
    if (precision > 0) f[0] = constant_inv;
    for (int k = 1; k < precision; ++k) {
        f[k] = mint(0);
        for (const auto &[index, coefficient] : support) {
            if (k < index) break;
            f[k] += f[k - index] * coefficient;
        }
        f[k] *= -constant_inv;
    }
    return f;
}

template <UnivariateFormalPowerSeries FPS> FPS sparse_inv(const FPS &f, int precision = -1) {
    FPS result = f;
    return inplace_sparse_inv(result, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS inv(const FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!f.empty() && f[0] != mint(0));
    if (is_sparse_operation(
            FPSOperation::INVERSE, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision))
        return sparse_inv(f, precision);
    return dense_inv(f, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_inv(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!f.empty() && f[0] != mint(0));
    const bool use_sparse = is_sparse_operation(
        FPSOperation::INVERSE, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision);
    if (use_sparse) return inplace_sparse_inv(f, precision);
    return inplace_dense_inv(f, precision);
}

} // namespace kk2::fps::operations

#endif // KK2_FPS_OPERATIONS_INVERSE_HPP
#ifndef KK2_FPS_OPERATIONS_LOGARITHM_HPP
#define KK2_FPS_OPERATIONS_LOGARITHM_HPP 1


#ifndef KK2_MATH_MOD_INV_TABLE_HPP
#define KK2_MATH_MOD_INV_TABLE_HPP 1


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

template <modint::Modular mint> struct Comb;

/**
 * @brief `[1, n]`のmod逆元を列挙するテーブル
 *
 * @tparam mint
 */
template <class mint> struct InvTable {
    static inline std::vector<mint> _invs{0, 1};

  private:
    static inline mint _factorial = 1;

    static void set_factorial(const mint &factorial) {
        _factorial = factorial;
    }

    template <modint::Modular> friend struct Comb;

  public:
    InvTable() = delete;

    static void set_upper(int m) {
        if ((int)_invs.size() > m) return;
        int start = _invs.size();
        _invs.resize(m + 1);
        const mint factorial_start = _factorial;
        for (int i = start; i <= m; ++i) {
            _factorial *= i;
            _invs[i] = _factorial;
        }
        mint inverse_factorial = _factorial.inv();
        for (int i = m; i > start; --i) {
            _invs[i] = inverse_factorial * _invs[i - 1];
            inverse_factorial *= i;
        }
        _invs[start] = inverse_factorial * factorial_start;
    }

    static inline mint inv(int n) {
        return _invs[n];
    }
};

} // namespace kk2

#endif // KK2_MATH_MOD_INV_TABLE_HPP

namespace kk2::fps::operations {

template <UnivariateFormalPowerSeries FPS> FPS &inplace_dense_log(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!f.empty() && f[0] == mint(1));
    if (precision == -1) precision = static_cast<int>(f.size());
    if (precision == 0) {
        f.clear();
        return f;
    }

    FPS inverse = f.dense_inv(precision);
    return f.inplace_diff().inplace_dense_mul(inverse).inplace_pre(precision - 1).inplace_int();
}

template <UnivariateFormalPowerSeries FPS> FPS dense_log(const FPS &f, int precision = -1) {
    FPS result = f;
    return inplace_dense_log(result, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_sparse_log(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    using ivta = InvTable<mint>;
    assert(!f.empty() && f[0] == mint(1));
    if (precision == -1) precision = static_cast<int>(f.size());

    std::vector<std::pair<int, mint>> support;
    for (int i = 1; i < static_cast<int>(f.size()); ++i) {
        if (f[i] != mint(0)) support.emplace_back(i, f[i]);
    }
    ivta::set_upper(precision);

    f.assign(precision, mint(0));
    std::size_t next_support = 0;
    for (int k = 0; k < precision - 1; ++k) {
        for (const auto &[index, coefficient] : support) {
            if (k < index) break;
            const int i = k - index;
            f[k + 1] -= f[i + 1] * coefficient * (i + 1);
        }
        f[k + 1] *= ivta::inv(k + 1);
        while (next_support < support.size() && support[next_support].first < k + 1) ++next_support;
        if (next_support < support.size() && support[next_support].first == k + 1)
            f[k + 1] += support[next_support].second;
    }
    return f;
}

template <UnivariateFormalPowerSeries FPS> FPS sparse_log(const FPS &f, int precision = -1) {
    FPS result = f;
    return inplace_sparse_log(result, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS log(const FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!f.empty() && f[0] == mint(1));
    if (is_sparse_operation(
            FPSOperation::LOG, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision))
        return sparse_log(f, precision);
    return dense_log(f, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_log(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    assert(!f.empty() && f[0] == mint(1));
    const bool use_sparse = is_sparse_operation(
        FPSOperation::LOG, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision);
    if (use_sparse) return inplace_sparse_log(f, precision);
    return inplace_dense_log(f, precision);
}

} // namespace kk2::fps::operations

#endif // KK2_FPS_OPERATIONS_LOGARITHM_HPP
#ifndef KK2_FPS_OPERATIONS_MULTIPLICATION_HPP
#define KK2_FPS_OPERATIONS_MULTIPLICATION_HPP 1

#ifndef KK2_CONVOLUTION_CONVOLUTION_HPP
#define KK2_CONVOLUTION_CONVOLUTION_HPP 1


#ifndef KK2_FPS_FPS_SPARSITY_DETECTOR_HPP
#define KK2_FPS_FPS_SPARSITY_DETECTOR_HPP 1


namespace kk2 {

enum class FPSOperation {
    CONVOLUTION,
    LOG,
    POWER,
    DIVISION,
    POLYNOMIAL_DIVISION,
    INVERSE,
    EXP,
    SQRT
};

namespace fps::sparsity_detail {

// E(n): the leading FFT evaluation cost, up to the common field-operation
// constant that cancels when dense and sparse leading terms are compared.
inline std::int64_t evaluation_work(int n) {
    if (n <= 1) return 1;
    const unsigned z = std::bit_ceil(static_cast<unsigned>(n));
    return static_cast<std::int64_t>(z) * std::countr_zero(z);
}

inline int transform_size(int n, int m) {
    if (n <= 0 || m <= 0) return 0;
    return static_cast<int>(std::bit_ceil(static_cast<unsigned>(n + m - 1)));
}

inline std::int64_t
convolution_dense_work(int n, int m, int precision, bool same, bool ntt_friendly) {
    n = std::min(n, precision);
    m = std::min(m, precision);
    const int z = transform_size(n, m);
    if (z == 0) return 0;

    // A different pair needs two forward and one inverse transform. Squaring
    // reuses the forward transform and needs only one forward transform.
    const int transforms = same ? 2 : 3;
    // Arbitrary-modulus convolution uses three NTT-friendly moduli.
    const int moduli = ntt_friendly ? 1 : 3;
    return static_cast<std::int64_t>(transforms) * moduli * evaluation_work(z);
}

inline std::int64_t inverse_dense_work(int precision, bool ntt_friendly) {
    if (precision <= 1) return 0;
    const int z = static_cast<int>(std::bit_ceil(static_cast<unsigned>(precision)));
    // NTT-friendly uses five transforms per Newton level, whose geometric
    // sum has leading term 10 E(z). The arbitrary-modulus implementation
    // performs two fresh convolutions per level, giving 60 E(z).
    return (ntt_friendly ? 10 : 60) * evaluation_work(z);
}

inline std::int64_t log_dense_work(int n, int precision, bool ntt_friendly) {
    return inverse_dense_work(precision, ntt_friendly)
           + convolution_dense_work(std::max(0, n - 1), precision, precision, false, ntt_friendly);
}

inline long double exp_dense_work(int precision, bool ntt_friendly) {
    if (precision <= 1) return 0;
    const int z = static_cast<int>(std::bit_ceil(static_cast<unsigned>(precision)));
    if (ntt_friendly) {
        if (precision <= 2) return 0;
        // Bostan--Schost, Theorem 1: (33/2) E(z) + (97/4) z.  Only
        // the leading E(z) term matters for the sparsity threshold.
        return 16.5L * evaluation_work(z);
    }
    // FPSArb recomputes a logarithm and a product at every Newton level.
    return 192 * evaluation_work(z);
}

inline long double power_dense_work(int n, int precision, bool ntt_friendly) {
    return log_dense_work(n, precision, ntt_friendly) + exp_dense_work(precision, ntt_friendly);
}

inline std::int64_t division_dense_work(int n, int precision, bool ntt_friendly) {
    return inverse_dense_work(precision, ntt_friendly)
           + convolution_dense_work(
               std::min(n, precision), precision, precision, false, ntt_friendly);
}

inline std::int64_t polynomial_division_dense_work(int quotient_size, bool ntt_friendly) {
    return inverse_dense_work(quotient_size, ntt_friendly)
           + convolution_dense_work(
               quotient_size, quotient_size, quotient_size, false, ntt_friendly);
}

inline std::int64_t sqrt_dense_work(int precision, bool ntt_friendly) {
    if (precision <= 1) return 0;
    const int z = static_cast<int>(std::bit_ceil(static_cast<unsigned>(precision)));
    // Newton uses an inverse and one product at every level. The implementation
    // computes the complete next power-of-two block even at the last level.
    return (ntt_friendly ? 32 : 156) * evaluation_work(z);
}

inline long double
sparse_work(FPSOperation op, int target, std::int64_t nonzero_a, std::int64_t nonzero_b) {
    switch (op) {
        case FPSOperation::CONVOLUTION:
            return static_cast<long double>(nonzero_a) * nonzero_b;
        case FPSOperation::DIVISION:
        case FPSOperation::POLYNOMIAL_DIVISION:
            return static_cast<long double>(target) * nonzero_b;
        case FPSOperation::LOG:
        case FPSOperation::POWER:
        case FPSOperation::INVERSE:
        case FPSOperation::EXP:
        case FPSOperation::SQRT:
            return static_cast<long double>(target) * nonzero_a;
    }
    return 0;
}

inline long double sparse_work_constant(FPSOperation op, bool ntt_friendly) {
    // Calibrated against the simplified sparse-work model at degrees 1024
    // and 4096.  Values are rounded upward near the measured crossover so a
    // close decision favors the dense implementation.
    switch (op) {
        case FPSOperation::CONVOLUTION:
            return ntt_friendly ? 0.90L : 0.55L;
        case FPSOperation::DIVISION:
        case FPSOperation::POLYNOMIAL_DIVISION:
            return ntt_friendly ? 0.90L : 0.40L;
        case FPSOperation::LOG:
            return ntt_friendly ? 2.70L : 1.00L;
        case FPSOperation::POWER:
            return ntt_friendly ? 1.35L : 0.70L;
        case FPSOperation::INVERSE:
            return ntt_friendly ? 1.00L : 0.40L;
        case FPSOperation::EXP:
            return ntt_friendly ? 1.05L : 0.45L;
        case FPSOperation::SQRT:
            return ntt_friendly ? 1.40L : 0.65L;
    }
    return 1.00L;
}

} // namespace fps::sparsity_detail

template <class FPS, class mint = typename FPS::value_type>
bool is_sparse_operation(
    FPSOperation op, bool is_ntt_friendly, const FPS &a, const FPS &b = FPS(), int precision = -1) {
    const int n = a.size(), m = b.size();
    if (n + m == 0) return false;

    const bool convolution = op == FPSOperation::CONVOLUTION;
    const bool division = op == FPSOperation::DIVISION;
    const bool polynomial_division = op == FPSOperation::POLYNOMIAL_DIVISION;
    const int requested =
        precision < 0 ? (convolution ? std::max(0, n + m - 1) : n) : std::max(0, precision);
    const int target = convolution ? std::min(requested, std::max(0, n + m - 1)) : requested;
    const int limit_a = convolution                       ? std::min(n, target) :
                        (division || polynomial_division) ? 0 :
                                                            std::min(n, target);
    const int limit_b = convolution         ? std::min(m, target) :
                        division            ? std::min(m, target) :
                        polynomial_division ? m :
                                              0;
    const auto is_nonzero = [](const mint &x) {
        return x != mint(0);
    };
    const std::int64_t nonzero_a = std::ranges::count_if(a | std::views::take(limit_a), is_nonzero);
    const std::int64_t nonzero_b = std::ranges::count_if(b | std::views::take(limit_b), is_nonzero);

    long double dense_work = 0;
    switch (op) {
        case FPSOperation::CONVOLUTION:
            dense_work = fps::sparsity_detail::convolution_dense_work(
                n, m, target, std::addressof(a) == std::addressof(b), is_ntt_friendly);
            break;
        case FPSOperation::LOG:
            dense_work = fps::sparsity_detail::log_dense_work(n, target, is_ntt_friendly);
            break;
        case FPSOperation::POWER:
            dense_work = fps::sparsity_detail::power_dense_work(n, target, is_ntt_friendly);
            break;
        case FPSOperation::DIVISION:
            dense_work = fps::sparsity_detail::division_dense_work(n, target, is_ntt_friendly);
            break;
        case FPSOperation::POLYNOMIAL_DIVISION:
            dense_work =
                fps::sparsity_detail::polynomial_division_dense_work(target, is_ntt_friendly);
            break;
        case FPSOperation::INVERSE:
            dense_work = fps::sparsity_detail::inverse_dense_work(target, is_ntt_friendly);
            break;
        case FPSOperation::EXP:
            dense_work = fps::sparsity_detail::exp_dense_work(target, is_ntt_friendly);
            break;
        case FPSOperation::SQRT:
            dense_work = fps::sparsity_detail::sqrt_dense_work(target, is_ntt_friendly);
            break;
    }

    const long double sparse_work =
        fps::sparsity_detail::sparse_work(op, target, nonzero_a, nonzero_b);
    return dense_work
           > fps::sparsity_detail::sparse_work_constant(op, is_ntt_friendly) * sparse_work;
}

} // namespace kk2

#endif // KK2_FPS_FPS_SPARSITY_DETECTOR_HPP
#ifndef KK2_MATH_MOD_BUTTERFLY_HPP
#define KK2_MATH_MOD_BUTTERFLY_HPP 1


#ifndef KK2_MATH_MOD_PRIMITIVE_ROOT_HPP
#define KK2_MATH_MOD_PRIMITIVE_ROOT_HPP 1

#ifndef KK2_MATH_MOD_POW_MOD_HPP
#define KK2_MATH_MOD_POW_MOD_HPP 1


namespace kk2 {

template <class S, class T, class U> constexpr S pow_mod(T x, U n, T m) {
    assert(n >= 0);
    if (m == 1) return S(0);
    S _m = m, r = 1;
    S y = x % _m;
    if (y < 0) y += _m;
    while (n) {
        if (n & 1) r = (r * y) % _m;
        if (n >>= 1) y = (y * y) % _m;
    }
    return r;
}

} // namespace kk2

#endif // KK2_MATH_MOD_POW_MOD_HPP

namespace kk2 {

constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    if (m == 1107296257) return 10;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) { x /= i; }
        }
    }
    if (x > 1) { divs[cnt++] = x; }
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod<long long>(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

template <int m> static constexpr int primitive_root = primitive_root_constexpr(m);

} // namespace kk2

#endif // KK2_MATH_MOD_PRIMITIVE_ROOT_HPP

namespace kk2 {

namespace detail {

template <class mint> int butterfly_log_size(std::size_t n) {
    assert((n > 0 && (n & (n - 1)) == 0) && "butterfly size must be a power of two");
    assert(n < (std::size_t{1} << 30) && "butterfly size is too large");
    assert((static_cast<std::uintmax_t>(mint::getmod()) - 1) % n == 0
           && "butterfly size is not supported by the modulus");
    return __builtin_ctz(static_cast<unsigned int>(n));
}

} // namespace detail

template <class FPS, class mint = typename FPS::value_type> void butterfly(FPS &a) {
    int h = detail::butterfly_log_size<mint>(a.size());
    static int g = primitive_root<mint::getmod()>;
    static bool first = true;
    static mint sum_e2[30]; // sum_e[i] = ies[0] * ... * ies[i - 1] * es[i]
    static mint sum_e3[30];
    static mint es[30], ies[30]; // es[i]^(2^(2+i)) == 1
    if (first) {
        first = false;
        int cnt2 = __builtin_ctz(mint::getmod() - 1);
        mint e = mint(g).pow((mint::getmod() - 1) >> cnt2), ie = e.inv();
        for (int i = cnt2; i >= 2; i--) {
            // e^(2^i) == 1
            es[i - 2] = e;
            ies[i - 2] = ie;
            e *= e;
            ie *= ie;
        }
        mint now = 1;
        for (int i = 0; i <= cnt2 - 2; i++) {
            sum_e2[i] = es[i] * now;
            now *= ies[i];
        }
        now = 1;
        for (int i = 0; i <= cnt2 - 3; i++) {
            sum_e3[i] = es[i + 1] * now;
            now *= ies[i + 1];
        }
    }

    int len = 0;
    while (len < h) {
        if (h - len == 1) {
            int p = 1 << (h - len - 1);
            mint rot = 1;
            for (int s = 0; s < (1 << len); s++) {
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p] * rot;
                    a[i + offset] = l + r;
                    a[i + offset + p] = l - r;
                }
                if (s + 1 != (1 << len)) rot *= sum_e2[__builtin_ctz(~(unsigned int)(s))];
            }
            len++;
        } else {
            int p = 1 << (h - len - 2);
            mint rot = 1, imag = es[0];
            for (int s = 0; s < (1 << len); s++) {
                mint rot2 = rot * rot;
                mint rot3 = rot2 * rot;
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto a0 = a[i + offset];
                    auto a1 = a[i + offset + p] * rot;
                    auto a2 = a[i + offset + p * 2] * rot2;
                    auto a3 = a[i + offset + p * 3] * rot3;
                    auto a1na3imag = (a1 - a3) * imag;
                    a[i + offset] = a0 + a2 + a1 + a3;
                    a[i + offset + p] = a0 + a2 - a1 - a3;
                    a[i + offset + p * 2] = a0 - a2 + a1na3imag;
                    a[i + offset + p * 3] = a0 - a2 - a1na3imag;
                }
                if (s + 1 != (1 << len)) rot *= sum_e3[__builtin_ctz(~(unsigned int)(s))];
            }
            len += 2;
        }
    }
}

template <class FPS, class mint = typename FPS::value_type> void butterfly_inv(FPS &a) {
    int n = int(a.size());
    int h = detail::butterfly_log_size<mint>(a.size());
    static constexpr int g = primitive_root<mint::getmod()>;
    static bool first = true;
    static mint sum_ie2[30]; // sum_ie[i] = es[0] * ... * es[i - 1] * ies[i]
    static mint sum_ie3[30];
    static mint es[30], ies[30]; // es[i]^(2^(2+i)) == 1
    static mint invn[30];
    if (first) {
        first = false;
        int cnt2 = __builtin_ctz(mint::getmod() - 1);
        mint e = mint(g).pow((mint::getmod() - 1) >> cnt2), ie = e.inv();
        for (int i = cnt2; i >= 2; i--) {
            // e^(2^i) == 1
            es[i - 2] = e;
            ies[i - 2] = ie;
            e *= e;
            ie *= ie;
        }
        mint now = 1;
        for (int i = 0; i <= cnt2 - 2; i++) {
            sum_ie2[i] = ies[i] * now;
            now *= es[i];
        }
        now = 1;
        for (int i = 0; i <= cnt2 - 3; i++) {
            sum_ie3[i] = ies[i + 1] * now;
            now *= es[i + 1];
        }

        invn[0] = 1;
        invn[1] = mint::getmod() / 2 + 1;
        for (int i = 2; i < 30; i++) invn[i] = invn[i - 1] * invn[1];
    }
    int len = h;
    while (len) {
        if (len == 1) {
            int p = 1 << (h - len);
            mint irot = 1;
            for (int s = 0; s < (1 << (len - 1)); s++) {
                int offset = s << (h - len + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p];
                    a[i + offset] = l + r;
                    a[i + offset + p] = (l - r) * irot;
                }
                if (s + 1 != (1 << (len - 1))) irot *= sum_ie2[__builtin_ctz(~(unsigned int)(s))];
            }
            len--;
        } else {
            int p = 1 << (h - len);
            mint irot = 1, iimag = ies[0];
            for (int s = 0; s < (1 << ((len - 2))); s++) {
                mint irot2 = irot * irot;
                mint irot3 = irot2 * irot;
                int offset = s << (h - len + 2);
                for (int i = 0; i < p; i++) {
                    auto a0 = a[i + offset];
                    auto a1 = a[i + offset + p];
                    auto a2 = a[i + offset + p * 2];
                    auto a3 = a[i + offset + p * 3];
                    auto a2na3iimag = (a2 - a3) * iimag;

                    a[i + offset] = a0 + a1 + a2 + a3;
                    a[i + offset + p] = (a0 - a1 + a2na3iimag) * irot;
                    a[i + offset + p * 2] = (a0 + a1 - a2 - a3) * irot2;
                    a[i + offset + p * 3] = (a0 - a1 - a2na3iimag) * irot3;
                }
                if (s + 1 != (1 << (len - 2))) irot *= sum_ie3[__builtin_ctz(~(unsigned int)(s))];
            }
            len -= 2;
        }
    }

    for (int i = 0; i < n; i++) a[i] *= invn[h];
}

template <class FPS, class mint = typename FPS::value_type> void doubling(FPS &a) {
    int n = a.size();
    detail::butterfly_log_size<mint>(a.size() * 2);
    auto b = a;
    int z = 1;
    butterfly_inv(b);
    mint r = 1, zeta = mint(primitive_root<mint::getmod()>).pow((mint::getmod() - 1) / (n << 1));
    for (int i = 0; i < n; i++) {
        b[i] *= r;
        r *= zeta;
    }
    butterfly(b);
    std::copy(b.begin(), b.end(), std::back_inserter(a));
}

} // namespace kk2

#endif // KK2_MATH_MOD_BUTTERFLY_HPP

namespace kk2 {

template <class FPS, class mint = typename FPS::value_type>
FPS &inplace_sparse_convolution(FPS &a, const FPS &b, int deg = -1) {
    const int original_a_size = a.size(), original_b_size = b.size();
    if (!original_a_size || !original_b_size) {
        a.clear();
        return a;
    }
    if (deg == -1) deg = original_a_size + original_b_size - 1;
    const int target = std::min(std::max(0, deg), original_a_size + original_b_size - 1);
    if (target == 0) {
        a.clear();
        return a;
    }

    std::vector<std::pair<int, mint>> support_b;
    for (int i = 0; i < std::min(original_b_size, target); ++i) {
        if (b[i] != mint(0)) support_b.emplace_back(i, b[i]);
    }
    a.resize(target);
    for (int i = std::min(original_a_size, target) - 1; i >= 0; --i) {
        const mint coefficient = a[i];
        a[i] = mint(0);
        if (coefficient == mint(0)) continue;
        for (const auto &[j, b_j] : support_b) {
            if (i + j >= target) break;
            a[i + j] += coefficient * b_j;
        }
    }
    return a;
}

template <class FPS> FPS sparse_convolution(const FPS &a, const FPS &b, int deg = -1) {
    FPS result = a;
    inplace_sparse_convolution(result, b, deg);
    return result;
}

template <class FPS> FPS &inplace_dense_convolution(FPS &a, const FPS &b, int deg = -1) {
    const int original_a_size = a.size(), original_b_size = b.size();
    if (!original_a_size || !original_b_size) {
        a.clear();
        return a;
    }
    if (deg == -1) deg = original_a_size + original_b_size - 1;
    const int target = std::min(std::max(0, deg), original_a_size + original_b_size - 1);
    if (target == 0) {
        a.clear();
        return a;
    }

    const int n = std::min(original_a_size, target);
    const int m = std::min(original_b_size, target);

    int z = 1;
    while (z < n + m - 1) z <<= 1;
    if (std::addressof(a) == std::addressof(b)) {
        a.resize(n);
        a.resize(z);
        butterfly(a);
        for (int i = 0; i < z; i++) a[i] *= a[i];
    } else {
        a.resize(n);
        a.resize(z);
        butterfly(a);
        FPS t(b.begin(), b.begin() + m);
        t.resize(z);
        butterfly(t);
        for (int i = 0; i < z; i++) a[i] *= t[i];
    }
    butterfly_inv(a);
    a.resize(target);
    return a;
}

template <class FPS> FPS dense_convolution(const FPS &a, const FPS &b, int deg = -1) {
    FPS result = a;
    inplace_dense_convolution(result, b, deg);
    return result;
}

template <class FPS> FPS &inplace_convolution(FPS &a, const FPS &b, int deg = -1) {
    const bool use_sparse = is_sparse_operation(FPSOperation::CONVOLUTION, true, a, b, deg);
    if (use_sparse) return inplace_sparse_convolution(a, b, deg);
    return inplace_dense_convolution(a, b, deg);
}

template <class FPS> FPS convolution(const FPS &a, const FPS &b, int deg = -1) {
    if (is_sparse_operation(FPSOperation::CONVOLUTION, true, a, b, deg))
        return sparse_convolution(a, b, deg);
    return dense_convolution(a, b, deg);
}

} // namespace kk2

#endif // KK2_CONVOLUTION_CONVOLUTION_HPP

namespace kk2::fps::operations {

template <UnivariateNTTFriendlyFormalPowerSeries FPS>
FPS &inplace_dense_mul(FPS &lhs, const FPS &rhs, int precision = -1) {
    return inplace_dense_convolution(lhs, rhs, precision);
}

template <UnivariateArbitraryModulusFormalPowerSeries FPS>
FPS &inplace_dense_mul(FPS &lhs, const FPS &rhs, int precision = -1);

template <UnivariateFormalPowerSeries FPS>
FPS dense_mul(const FPS &lhs, const FPS &rhs, int precision = -1) {
    FPS result = lhs;
    inplace_dense_mul(result, rhs, precision);
    return result;
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_sparse_mul(FPS &lhs, const FPS &rhs, int precision = -1) {
    return inplace_sparse_convolution(lhs, rhs, precision);
}

template <UnivariateFormalPowerSeries FPS>
FPS sparse_mul(const FPS &lhs, const FPS &rhs, int precision = -1) {
    FPS result = lhs;
    inplace_sparse_mul(result, rhs, precision);
    return result;
}

template <UnivariateFormalPowerSeries FPS>
FPS mul(const FPS &lhs, const FPS &rhs, int precision = -1) {
    if (is_sparse_operation(
            FPSOperation::CONVOLUTION, NTTFriendlyFormalPowerSeries<FPS>, lhs, rhs, precision))
        return sparse_mul(lhs, rhs, precision);
    return dense_mul(lhs, rhs, precision);
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_mul(FPS &lhs, const FPS &rhs, int precision = -1) {
    const bool use_sparse = is_sparse_operation(
        FPSOperation::CONVOLUTION, NTTFriendlyFormalPowerSeries<FPS>, lhs, rhs, precision);
    if (use_sparse) return inplace_sparse_mul(lhs, rhs, precision);
    return inplace_dense_mul(lhs, rhs, precision);
}

} // namespace kk2::fps::operations

#endif // KK2_FPS_OPERATIONS_MULTIPLICATION_HPP
#ifndef KK2_FPS_OPERATIONS_POWER_HPP
#define KK2_FPS_OPERATIONS_POWER_HPP 1



namespace kk2::fps::operations {

struct PowerPreprocessResult {
    int precision;
    int normalized_precision;
    int shift;
    bool finished;
};

template <UnivariateFormalPowerSeries FPS, Integral T>
PowerPreprocessResult inplace_power_preprocess(FPS &f, T exponent, int precision) {
    using mint = typename FPS::value_type;
    if (precision == -1) precision = static_cast<int>(f.size());
    if (exponent == 0) {
        f.assign(precision, mint(0));
        if (precision > 0) f[0] = mint(1);
        return {precision, 0, 0, true};
    }

    int leading_zeros = 0;
    while (leading_zeros != static_cast<int>(f.size()) && f[leading_zeros] == mint(0))
        ++leading_zeros;
    if (leading_zeros == static_cast<int>(f.size())
        || __int128_t(leading_zeros) * exponent >= precision) {
        f.assign(precision, mint(0));
        return {precision, 0, 0, true};
    }

    const int shift = static_cast<int>(__int128_t(leading_zeros) * exponent);
    if (leading_zeros > 0) f.erase(f.begin(), f.begin() + leading_zeros);
    return {precision, precision - shift, shift, false};
}

template <UnivariateFormalPowerSeries FPS>
FPS &inplace_power_postprocess(FPS &f, const PowerPreprocessResult &preprocessed) {
    using mint = typename FPS::value_type;
    if (preprocessed.shift > 0) f.insert(f.begin(), preprocessed.shift, mint(0));
    f.resize(preprocessed.precision);
    return f;
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS &inplace_dense_pow_normalized(FPS &f, T exponent, int precision) {
    const auto leading_coefficient = f[0];
    f *= leading_coefficient.inv();
    f.inplace_dense_log(precision);
    f *= exponent;
    f.inplace_dense_exp(precision);
    f *= leading_coefficient.pow(exponent);
    return f;
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS &inplace_sparse_pow_normalized(FPS &f, T exponent, int precision) {
    using mint = typename FPS::value_type;
    const int mod = mint::getmod();
    static std::vector<mint> inverse{1, 1};
    while (static_cast<int>(inverse.size()) <= precision) {
        const int i = inverse.size();
        inverse.push_back(-inverse[mod % i] * (mod / i));
    }

    const mint constant_term = f[0].pow(exponent);
    exponent %= mod;
    std::vector<std::tuple<int, mint, mint>> support;
    for (int i = 1; i < static_cast<int>(f.size()); ++i) {
        if (f[i] != mint(0)) support.emplace_back(i, f[i], f[i] * mint(i) * (exponent + 1));
    }

    const mint constant_inv = f[0].inv();
    f.assign(precision, mint(0));
    f[0] = constant_term;
    for (int degree = 1; degree < precision; ++degree) {
        for (const auto &[index, coefficient, weighted_coefficient] : support) {
            if (degree < index) break;
            f[degree] += f[degree - index] * (weighted_coefficient - coefficient * degree);
        }
        f[degree] *= constant_inv * inverse[degree];
    }
    return f;
}

template <UnivariateFormalPowerSeries FPS>
bool power_uses_sparse(const FPS &normalized, int normalized_precision) {
    return is_sparse_operation(FPSOperation::POWER,
                               NTTFriendlyFormalPowerSeries<FPS>,
                               normalized,
                               FPS(),
                               normalized_precision);
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS &inplace_dense_pow(FPS &f, T exponent, int precision = -1) {
    const PowerPreprocessResult preprocessed = inplace_power_preprocess(f, exponent, precision);
    if (preprocessed.finished) return f;
    inplace_dense_pow_normalized(f, exponent, preprocessed.normalized_precision);
    return inplace_power_postprocess(f, preprocessed);
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS dense_pow(const FPS &f, T exponent, int precision = -1) {
    FPS result = f;
    inplace_dense_pow(result, exponent, precision);
    return result;
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS &inplace_sparse_pow(FPS &f, T exponent, int precision = -1) {
    const PowerPreprocessResult preprocessed = inplace_power_preprocess(f, exponent, precision);
    if (preprocessed.finished) return f;
    inplace_sparse_pow_normalized(f, exponent, preprocessed.normalized_precision);
    return inplace_power_postprocess(f, preprocessed);
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS sparse_pow(const FPS &f, T exponent, int precision = -1) {
    FPS result = f;
    inplace_sparse_pow(result, exponent, precision);
    return result;
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS &inplace_pow(FPS &f, T exponent, int precision = -1) {
    const PowerPreprocessResult preprocessed = inplace_power_preprocess(f, exponent, precision);
    if (preprocessed.finished) return f;
    if (power_uses_sparse(f, preprocessed.normalized_precision))
        inplace_sparse_pow_normalized(f, exponent, preprocessed.normalized_precision);
    else inplace_dense_pow_normalized(f, exponent, preprocessed.normalized_precision);
    return inplace_power_postprocess(f, preprocessed);
}

template <UnivariateFormalPowerSeries FPS, Integral T>
FPS pow(const FPS &f, T exponent, int precision = -1) {
    FPS result = f;
    inplace_pow(result, exponent, precision);
    return result;
}

} // namespace kk2::fps::operations

#endif // KK2_FPS_OPERATIONS_POWER_HPP
#ifndef KK2_FPS_OPERATIONS_SQRT_HPP
#define KK2_FPS_OPERATIONS_SQRT_HPP 1


#ifndef KK2_MATH_MOD_DETAIL_MOD_SQRT_HPP
#define KK2_MATH_MOD_DETAIL_MOD_SQRT_HPP 1

#ifndef KK2_MATH_MOD_PRIMITIVE_ROOT_HPP
#define KK2_MATH_MOD_PRIMITIVE_ROOT_HPP 1

#ifndef KK2_MATH_MOD_POW_MOD_HPP
#define KK2_MATH_MOD_POW_MOD_HPP 1


namespace kk2 {

template <class S, class T, class U> constexpr S pow_mod(T x, U n, T m) {
    assert(n >= 0);
    if (m == 1) return S(0);
    S _m = m, r = 1;
    S y = x % _m;
    if (y < 0) y += _m;
    while (n) {
        if (n & 1) r = (r * y) % _m;
        if (n >>= 1) y = (y * y) % _m;
    }
    return r;
}

} // namespace kk2

#endif // KK2_MATH_MOD_POW_MOD_HPP

namespace kk2 {

constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    if (m == 1107296257) return 10;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) { x /= i; }
        }
    }
    if (x > 1) { divs[cnt++] = x; }
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod<long long>(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

template <int m> static constexpr int primitive_root = primitive_root_constexpr(m);

} // namespace kk2

#endif // KK2_MATH_MOD_PRIMITIVE_ROOT_HPP

namespace kk2::mod_sqrt_detail {

template <class Mint> long long tonelli_shanks(const Mint &a, Mint z, long long m, long long e) {
    Mint x = a.pow((m - 1) / 2);
    Mint y = a * x * x;
    x *= a;
    while (y != Mint(1)) {
        long long j = 0;
        Mint t = y;
        while (t != Mint(1)) {
            j++;
            t *= t;
        }
        z = z.pow(1LL << (e - j - 1));
        x *= z;
        z *= z;
        y *= z;
        e = j;
    }
    return x.val();
}

template <bool ntt_friendly = false, class mint> long long mod_sqrt(const mint &a) {
    const auto p = mint::getmod();
    if (a.val() < 2) return a.val();

    // Euler's criterion
    if (a.pow((p - 1) / 2) != mint(1)) return -1;

    mint b;
    if constexpr (ntt_friendly) {
        b = primitive_root<mint::getmod()>;
    } else {
        // Find a quadratic non-residue.
        b = 1;
        while (b.pow((p - 1) / 2) == mint(1)) b += 1;
    }

    long long m = p - 1, e = 0;
    while (m % 2 == 0) m >>= 1, e++;

    mint z = b.pow(m);
    return tonelli_shanks(a, z, m, e);
}

} // namespace kk2::mod_sqrt_detail

#endif // KK2_MATH_MOD_DETAIL_MOD_SQRT_HPP

namespace kk2::fps::operations {

template <UnivariateFormalPowerSeries FPS> FPS dense_sqrt(const FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    if (precision == -1) precision = static_cast<int>(f.size());
    if (f.empty()) return FPS(precision, mint(0));
    if (f[0] == mint(0)) {
        for (int i = 1; i < static_cast<int>(f.size()); ++i) {
            if (f[i] == mint(0)) continue;
            if (i & 1) return {};
            if (precision - i / 2 <= 0) break;
            FPS result = dense_sqrt(f >> i, precision - i / 2);
            if (result.empty()) return {};
            result <<= i / 2;
            if (static_cast<int>(result.size()) < precision) result.resize(precision, mint(0));
            return result;
        }
        return FPS(precision, mint(0));
    }

    const long long root = mod_sqrt_detail::mod_sqrt<NTTFriendlyFormalPowerSeries<FPS>>(f[0]);
    if (root == -1) return {};
    assert(root * root % mint::getmod() == f[0].val());
    FPS result{mint(root)};
    const mint inverse_two = mint(2).inv();
    for (int d = 1; d < precision; d <<= 1) {
        result = (result + f.pre(d << 1).dense_mul(result.dense_inv(d << 1))) * inverse_two;
    }
    return result.pre(precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_dense_sqrt(FPS &f, int precision = -1) {
    if (precision == -1) precision = static_cast<int>(f.size());
    return f = dense_sqrt(std::as_const(f), precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_sparse_sqrt(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    if (precision == -1) precision = static_cast<int>(f.size());
    if (f.empty()) {
        f.assign(precision, mint(0));
        return f;
    }
    if (f[0] == mint(0)) {
        for (int i = 1; i < static_cast<int>(f.size()); ++i) {
            if (f[i] == mint(0)) continue;
            if (i & 1) {
                f.clear();
                return f;
            }
            if (precision - i / 2 <= 0) break;
            f >>= i;
            inplace_sparse_sqrt(f, precision - i / 2);
            if (f.empty()) return f;
            f <<= i / 2;
            if (static_cast<int>(f.size()) < precision) f.resize(precision, mint(0));
            return f;
        }
        f.assign(precision, mint(0));
        return f;
    }

    const long long root = mod_sqrt_detail::mod_sqrt<NTTFriendlyFormalPowerSeries<FPS>>(f[0]);
    if (root == -1) {
        f.clear();
        return f;
    }
    return inplace_sparse_pow(f, (mint::getmod() + 1) >> 1, precision) *= mint(root).inv();
}

template <UnivariateFormalPowerSeries FPS> FPS sparse_sqrt(const FPS &f, int precision = -1) {
    FPS result = f;
    return inplace_sparse_sqrt(result, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS sqrt(const FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    if (!f.empty() && f[0] != mint(0)
        && is_sparse_operation(
            FPSOperation::SQRT, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision))
        return sparse_sqrt(f, precision);
    return dense_sqrt(f, precision);
}

template <UnivariateFormalPowerSeries FPS> FPS &inplace_sqrt(FPS &f, int precision = -1) {
    using mint = typename FPS::value_type;
    const bool use_sparse =
        !f.empty() && f[0] != mint(0)
        && is_sparse_operation(
            FPSOperation::SQRT, NTTFriendlyFormalPowerSeries<FPS>, f, FPS(), precision);
    if (use_sparse) return inplace_sparse_sqrt(f, precision);
    return inplace_dense_sqrt(f, precision);
}

} // namespace kk2::fps::operations

#endif // KK2_FPS_OPERATIONS_SQRT_HPP

namespace kk2 {

template <class Derived, modint::Modular mint> struct FormalPowerSeriesBase : std::vector<mint> {
    using std::vector<mint>::vector;
    using FPS = Derived;
    using ivta = InvTable<mint>;

    // CRTPを使って派生クラスの参照を取得
    Derived &derived() { return static_cast<Derived &>(*this); }
    const Derived &derived() const { return static_cast<const Derived &>(*this); }

    template <OutputStream OStream> void debug_output(OStream &os) const {
        os << "[";
        for (size_t i = 0; i < this->size(); i++) {
            os << (*this)[i] << (i + 1 == this->size() ? "" : ", ");
        }
        os << "]";
    }

    template <OutputStream OStream> void output(OStream &os) const {
        for (size_t i = 0; i < this->size(); i++) {
            os << (*this)[i] << (i + 1 == this->size() ? "\n" : " ");
        }
    }
    template <OutputStream OStream> friend OStream &operator<<(OStream &os, const FPS &fps_) {
        for (size_t i = 0; i < fps_.size(); i++) {
            os << fps_[i] << (i + 1 == fps_.size() ? "" : " ");
        }
        return os;
    }

    template <InputStream IStream> FPS &input(IStream &is) {
        for (size_t i = 0; i < this->size(); i++) is >> (*this)[i];
        return derived();
    }

    template <InputStream IStream> friend IStream &operator>>(IStream &is, FPS &fps_) {
        for (auto &x : fps_) is >> x;
        return is;
    }
    FPS &operator+=(const FPS &r) {
        if (this->size() < r.size()) this->resize(r.size());
        for (size_t i = 0; i < r.size(); i++) (*this)[i] += r[i];
        return derived();
    }

    FPS &operator+=(const mint &r) {
        if (this->empty()) this->resize(1);
        (*this)[0] += r;
        return derived();
    }

    FPS &operator-=(const FPS &r) {
        if (this->size() < r.size()) this->resize(r.size());
        for (size_t i = 0; i < r.size(); i++) (*this)[i] -= r[i];
        return derived();
    }

    FPS &operator-=(const mint &r) {
        if (this->empty()) this->resize(1);
        (*this)[0] -= r;
        return derived();
    }

    FPS &operator*=(const mint &r) {
        for (size_t i = 0; i < this->size(); i++) { (*this)[i] *= r; }
        return derived();
    }
    FPS &operator*=(const FPS &r) { return inplace_mul(r); }
    FPS &operator/=(const FPS &r) { return inplace_quo(r); }

    FPS dense_quo(const FPS &r) const { return fps::operations::dense_quo(derived(), r); }
    FPS &inplace_dense_quo(const FPS &r) {
        return fps::operations::inplace_dense_quo(derived(), r);
    }
    FPS sparse_quo(const FPS &r) const { return fps::operations::sparse_quo(derived(), r); }
    FPS &inplace_sparse_quo(const FPS &r) {
        return fps::operations::inplace_sparse_quo(derived(), r);
    }
    FPS quo(const FPS &r) const { return fps::operations::quo(derived(), r); }
    FPS &inplace_quo(const FPS &r) { return fps::operations::inplace_quo(derived(), r); }
    FPS dense_mod(const FPS &r) const { return fps::operations::dense_mod(derived(), r); }
    FPS &inplace_dense_mod(const FPS &r) {
        return fps::operations::inplace_dense_mod(derived(), r);
    }
    FPS sparse_mod(const FPS &r) const { return fps::operations::sparse_mod(derived(), r); }
    FPS &inplace_sparse_mod(const FPS &r) {
        return fps::operations::inplace_sparse_mod(derived(), r);
    }
    FPS mod(const FPS &r) const { return fps::operations::mod(derived(), r); }
    FPS &inplace_mod(const FPS &r) { return fps::operations::inplace_mod(derived(), r); }

    FPS &operator%=(const FPS &r) { return inplace_mod(r); }

    FPS &operator>>=(int n) {
        if (n >= (int)this->size()) {
            this->clear();
        } else {
            this->erase(this->begin(), this->begin() + n);
        }
        return derived();
    }

    FPS &operator<<=(int n) {
        this->insert(this->begin(), n, mint(0));
        return derived();
    }

    // CRTPを使って派生クラスのメソッドを利用した演算子の自動実装
    FPS operator+(const FPS &r) const { return FPS(derived()) += r; }
    FPS operator+(const mint &r) const { return FPS(derived()) += r; }
    FPS operator-(const FPS &r) const { return FPS(derived()) -= r; }
    FPS operator-(const mint &r) const { return FPS(derived()) -= r; }
    FPS operator*(const FPS &r) const { return FPS(derived()) *= r; }
    FPS operator*(const mint &r) const { return FPS(derived()) *= r; }
    FPS operator/(const FPS &r) const { return FPS(derived()) /= r; }
    FPS operator%(const FPS &r) const { return FPS(derived()) %= r; }
    FPS operator>>(int n) const { return FPS(derived()) >>= n; }
    FPS operator<<(int n) const { return FPS(derived()) <<= n; }

    FPS operator-() const {
        FPS ret(this->size());
        std::ranges::transform(
            *this, ret.begin(), [](const mint &coefficient) { return -coefficient; });
        return ret;
    }
    FPS &shrink() {
        while (this->size() && this->back() == mint(0)) this->pop_back();
        return derived();
    }

    FPS &inplace_rev() {
        std::reverse(this->begin(), this->end());
        return derived();
    }

    FPS &inplace_dot(const FPS &r) {
        this->resize(std::min(this->size(), r.size()));
        for (size_t i = 0; i < this->size(); i++) (*this)[i] *= r[i];
        return derived();
    }

    FPS &inplace_pre(int n) {
        this->resize(n);
        return derived();
    }

    FPS &inplace_diff() {
        if (this->empty()) return derived();
        this->erase(this->begin());
        for (size_t i = 1; i <= this->size(); i++) (*this)[i - 1] *= mint(i);
        return derived();
    }

    FPS &inplace_int() {
        ivta::set_upper(this->size());
        this->insert(this->begin(), mint(0));
        for (size_t i = 1; i < this->size(); i++) (*this)[i] *= ivta::inv(i);
        return derived();
    }

    // CRTPを使った便利関数の自動実装
    FPS rev() const { return FPS(derived()).inplace_rev(); }
    FPS dot(const FPS &r) const { return FPS(derived()).inplace_dot(r); }
    FPS pre(int n) const { return FPS(derived()).inplace_pre(n); }
    FPS diff() const { return FPS(derived()).inplace_diff(); }
    FPS integral() const { return FPS(derived()).inplace_int(); }

    mint eval(mint x) const {
        mint r = 0, w = 1;
        for (auto &v : *this) {
            r += w * v;
            w *= x;
        }
        return r;
    }

    FPS dense_log(int precision = -1) const {
        return fps::operations::dense_log(derived(), precision);
    }
    FPS &inplace_dense_log(int precision = -1) {
        return fps::operations::inplace_dense_log(derived(), precision);
    }
    FPS sparse_log(int precision = -1) const {
        return fps::operations::sparse_log(derived(), precision);
    }
    FPS &inplace_sparse_log(int precision = -1) {
        return fps::operations::inplace_sparse_log(derived(), precision);
    }
    FPS log(int precision = -1) const { return fps::operations::log(derived(), precision); }
    FPS &inplace_log(int precision = -1) {
        return fps::operations::inplace_log(derived(), precision);
    }

    template <Integral T> FPS dense_pow(T exponent, int precision = -1) const {
        return fps::operations::dense_pow(derived(), exponent, precision);
    }
    template <Integral T> FPS &inplace_dense_pow(T exponent, int precision = -1) {
        return fps::operations::inplace_dense_pow(derived(), exponent, precision);
    }
    template <Integral T> FPS sparse_pow(T exponent, int precision = -1) const {
        return fps::operations::sparse_pow(derived(), exponent, precision);
    }
    template <Integral T> FPS &inplace_sparse_pow(T exponent, int precision = -1) {
        return fps::operations::inplace_sparse_pow(derived(), exponent, precision);
    }
    template <Integral T> FPS pow(T exponent, int precision = -1) const {
        return fps::operations::pow(derived(), exponent, precision);
    }
    template <Integral T> FPS &inplace_pow(T exponent, int precision = -1) {
        return fps::operations::inplace_pow(derived(), exponent, precision);
    }

    FPS dense_div(const FPS &r, int precision = -1) const {
        return fps::operations::dense_div(derived(), r, precision);
    }
    FPS &inplace_dense_div(const FPS &r, int precision = -1) {
        return fps::operations::inplace_dense_div(derived(), r, precision);
    }
    FPS sparse_div(const FPS &r, int precision = -1) const {
        return fps::operations::sparse_div(derived(), r, precision);
    }
    FPS &inplace_sparse_div(const FPS &r, int precision = -1) {
        return fps::operations::inplace_sparse_div(derived(), r, precision);
    }
    FPS div(const FPS &r, int precision = -1) const {
        return fps::operations::div(derived(), r, precision);
    }
    FPS &inplace_div(const FPS &r, int precision = -1) {
        return fps::operations::inplace_div(derived(), r, precision);
    }

    FPS dense_inv(int precision = -1) const {
        return fps::operations::dense_inv(derived(), precision);
    }
    FPS &inplace_dense_inv(int precision = -1) {
        return fps::operations::inplace_dense_inv(derived(), precision);
    }
    FPS sparse_inv(int precision = -1) const {
        return fps::operations::sparse_inv(derived(), precision);
    }
    FPS &inplace_sparse_inv(int precision = -1) {
        return fps::operations::inplace_sparse_inv(derived(), precision);
    }
    FPS inv(int precision = -1) const { return fps::operations::inv(derived(), precision); }
    FPS &inplace_inv(int precision = -1) {
        return fps::operations::inplace_inv(derived(), precision);
    }

    FPS dense_exp(int precision = -1) const {
        return fps::operations::dense_exp(derived(), precision);
    }
    FPS &inplace_dense_exp(int precision = -1) {
        return fps::operations::inplace_dense_exp(derived(), precision);
    }
    FPS sparse_exp(int precision = -1) const {
        return fps::operations::sparse_exp(derived(), precision);
    }
    FPS &inplace_sparse_exp(int precision = -1) {
        return fps::operations::inplace_sparse_exp(derived(), precision);
    }
    FPS exp(int precision = -1) const { return fps::operations::exp(derived(), precision); }
    FPS &inplace_exp(int precision = -1) {
        return fps::operations::inplace_exp(derived(), precision);
    }

    FPS dense_sqrt(int precision = -1) const {
        return fps::operations::dense_sqrt(derived(), precision);
    }
    FPS &inplace_dense_sqrt(int precision = -1) {
        return fps::operations::inplace_dense_sqrt(derived(), precision);
    }
    FPS sparse_sqrt(int precision = -1) const {
        return fps::operations::sparse_sqrt(derived(), precision);
    }
    FPS &inplace_sparse_sqrt(int precision = -1) {
        return fps::operations::inplace_sparse_sqrt(derived(), precision);
    }
    FPS sqrt(int precision = -1) const { return fps::operations::sqrt(derived(), precision); }
    FPS &inplace_sqrt(int precision = -1) {
        return fps::operations::inplace_sqrt(derived(), precision);
    }

    FPS dense_mul(const FPS &r, int precision = -1) const {
        return fps::operations::dense_mul(derived(), r, precision);
    }
    FPS &inplace_dense_mul(const FPS &r, int precision = -1) {
        return fps::operations::inplace_dense_mul(derived(), r, precision);
    }
    FPS sparse_mul(const FPS &r, int precision = -1) const {
        return fps::operations::sparse_mul(derived(), r, precision);
    }
    FPS &inplace_sparse_mul(const FPS &r, int precision = -1) {
        return fps::operations::inplace_sparse_mul(derived(), r, precision);
    }
    FPS mul(const FPS &r, int precision = -1) const {
        return fps::operations::mul(derived(), r, precision);
    }
    FPS &inplace_mul(const FPS &r, int precision = -1) {
        return fps::operations::inplace_mul(derived(), r, precision);
    }

    FPS &inplace_imos(int n) {
        inplace_pre(n);
        for (int i = 0; i < n - 1; i++) (*this)[i + 1] += (*this)[i];
        return derived();
    }

    FPS &inplace_iimos(int n) {
        inplace_pre(n);
        for (int i = 0; i < n - 1; i++) (*this)[i + 1] -= (*this)[i];
        return derived();
    }
    FPS imos(int n) const { return FPS(derived()).inplace_imos(n); }
    FPS iimos(int n) const { return FPS(derived()).inplace_iimos(n); }
};

} // namespace kk2

#endif // KK2_FPS_FPS_BASE_HPP

namespace kk2 {

template <modint::Modular mint>
struct FormalPowerSeriesNTTFriendly
    : FormalPowerSeriesBase<FormalPowerSeriesNTTFriendly<mint>, mint> {
    using base = FormalPowerSeriesBase<FormalPowerSeriesNTTFriendly<mint>, mint>;
    using FPS = FormalPowerSeriesNTTFriendly<mint>;
    using base::FormalPowerSeriesBase;
    using base::operator*=; // 基底クラスのoperator*=を継承
    using modulus_category = kk2::fps::category::ntt_friendly_modulus;
    using series_category = kk2::fps::category::ordinary;
    using variable_category = kk2::fps::category::univariate;
    static constexpr bool is_ntt_friendly = true;

    void but() { butterfly(*this); }
    void ibut() { butterfly_inv(*this); }
    void db() { doubling(*this); }
    static int but_pr() { return primitive_root<mint::getmod()>; }
};

template <modint::Modular mint> using FPSNTT = FormalPowerSeriesNTTFriendly<mint>;

} // namespace kk2

#endif // KK2_FPS_FPS_NTT_FRIENDLY_HPP
#ifndef KK2_FPS_POWER_SUM_HPP
#define KK2_FPS_POWER_SUM_HPP 1


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
        const bool extend_invs = _invs.size() <= m;
        if (extend_invs) _invs.resize(m + 1);
        for (usize i = n; i <= m; i++) _fact.emplace_back(_fact.back() * i);
        _ifact[m] = _fact[m].inv();
        _invs[m] = _ifact[m] * _fact[m - 1];
        for (usize i = m; i > n; i--) {
            _ifact[i - 1] = _ifact[i] * i;
            _invs[i - 1] = _ifact[i - 1] * _fact[i - 2];
        }
        if (extend_invs) InvTable<mint>::set_factorial(_fact[m]);
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
#ifndef KK2_MATH_MOD_POWER_SUM_HPP
#define KK2_MATH_MOD_POWER_SUM_HPP 1


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

template <modint::Modular mint> struct Comb;

/**
 * @brief `[1, n]`のmod逆元を列挙するテーブル
 *
 * @tparam mint
 */
template <class mint> struct InvTable {
    static inline std::vector<mint> _invs{0, 1};

  private:
    static inline mint _factorial = 1;

    static void set_factorial(const mint &factorial) {
        _factorial = factorial;
    }

    template <modint::Modular> friend struct Comb;

  public:
    InvTable() = delete;

    static void set_upper(int m) {
        if ((int)_invs.size() > m) return;
        int start = _invs.size();
        _invs.resize(m + 1);
        const mint factorial_start = _factorial;
        for (int i = start; i <= m; ++i) {
            _factorial *= i;
            _invs[i] = _factorial;
        }
        mint inverse_factorial = _factorial.inv();
        for (int i = m; i > start; --i) {
            _invs[i] = inverse_factorial * _invs[i - 1];
            inverse_factorial *= i;
        }
        _invs[start] = inverse_factorial * factorial_start;
    }

    static inline mint inv(int n) {
        bool neg = n < 0;
        if (neg) n = -n;
        if (n >= (int)_invs.size()) set_upper(n);
        return neg ? -_invs[n] : _invs[n];
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
        const bool extend_invs = _invs.size() <= m;
        if (extend_invs) _invs.resize(m + 1);
        for (usize i = n; i <= m; i++) _fact.emplace_back(_fact.back() * i);
        _ifact[m] = _fact[m].inv();
        _invs[m] = _ifact[m] * _fact[m - 1];
        for (usize i = m; i > n; i--) {
            _ifact[i - 1] = _ifact[i] * i;
            _invs[i - 1] = _ifact[i - 1] * _fact[i - 2];
        }
        if (extend_invs) InvTable<mint>::set_factorial(_fact[m]);
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

} // namespace kk2

#endif // KK2_MATH_MOD_POWER_SUM_HPP

namespace kk2 {

/**
 * @brief Enumerate `sum_{i=0}^{n-1} i^l` for `l = 0, ..., k`.
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#sum-of-monomial-enumerate
 */
template <fps::ModularUnivariateFormalPowerSeries FPS, UnsignedIntegral T1, UnsignedIntegral T2>
FPS sum_of_monomial_enumerate(T1 n, T2 k) {
    using mint = FPS::value_type;
    using comb = Comb<mint>;
    comb::set_upper(k + 1);
    FPS f(k + 1), g(k + 1);
    mint tmp = n;
    for (usize i = 1; i < k + 2; ++i, tmp *= n) {
        f[i - 1] = comb::ifact(i) * tmp;
        g[i - 1] = comb::ifact(i);
    }
    f.inplace_div(g);
    for (usize i = 1; i <= k; ++i) f[i] *= comb::fact(i);
    return f;
}

/**
 * @brief Return `sum_{i=0}^{n-1} f(i)` from the coefficients of `f`.
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#sum-of-polynomial
 */
template <fps::ModularUnivariateFormalPowerSeries FPS,
          modint::Modular mint = typename FPS::value_type,
          UnsignedIntegral T>
mint sum_of_polynomial(const FPS &f, T n) {
    auto tmp = sum_of_monomial_enumerate<FPS>(n, f.size());
    mint res = 0;
    for (usize i = 0; i < f.size(); ++i) res += f[i] * tmp[i];
    return res;
}

/**
 * @brief Return `sum_{i=0}^{n-1} f(i)` from the samples `f(0), ..., f(k)`.
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#sum-of-polynomial-samples
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
 * @brief Construct the polynomial `g` satisfying `g(n) = sum_{i=0}^n f(i)`.
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#prefix-sum-of-polynomial
 */
template <fps::ModularUnivariateFormalPowerSeries FPS> FPS prefix_sum_of_polynomial(const FPS &f) {
    using mint = FPS::value_type;
    using comb = Comb<mint>;
    const usize n = f.size();
    comb::set_upper(n);
    FPS g(n);
    for (usize i = 0; i < n; ++i) g[i] = comb::ifact(i + 1);
    g.inplace_inv(n);
    FPS f_egf(f);
    for (usize i = 0; i < n; ++i) f_egf[i] *= comb::fact(i);
    f_egf.inplace_rev().inplace_mul(g, n).inplace_rev() <<= 1;
    FPS result = std::move(f_egf);
    for (usize i = 1; i <= n; ++i) result[i] *= comb::ifact(i);
    return result;
}

/**
 * @brief Return the rational-function value of `sum_{i=0}^infty r^i f(i)` from its samples.
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#sum-of-geometric-polynomial-samples-infinite
 */
template <fps::ModularUnivariateFormalPowerSeries FPS,
          modint::Modular mint = typename FPS::value_type>
mint sum_of_geometric_polynomial_samples(mint r, const std::vector<mint> &samples) {
    assert(!samples.empty());
    assert(samples.size() < static_cast<usize>(mint::getmod()));
    assert(r != mint(1));
    if (r == mint(0)) return samples[0];

    const usize k = samples.size() - 1;
    const usize m = k + 1;
    InvTable<mint>::set_upper(static_cast<int>(m));

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
                mint(j) * InvTable<mint>::inv(static_cast<int>(m - j + 1)) * inverse_minus_r;
        }
    }
    return numerator / (mint(1) - r).pow(m);
}

/**
 * @brief Return `sum_{i=0}^{n-1} r^i f(i)` from the samples `f(0), ..., f(k)`.
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#sum-of-geometric-polynomial-samples-finite
 */
template <fps::ModularUnivariateFormalPowerSeries FPS,
          modint::Modular mint = typename FPS::value_type,
          UnsignedIntegral T>
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
    const mint infinite_sum = sum_of_geometric_polynomial_samples<FPS>(r, samples);
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
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#sum-of-geometric-monomial-finite
 */
template <fps::ModularUnivariateFormalPowerSeries FPS,
          modint::Modular mint = typename FPS::value_type,
          UnsignedIntegral T1,
          UnsignedIntegral T2>
mint sum_of_geometric_monomial(mint r, T1 n, T2 k) {
    auto samples = pow_table<mint>(k, k);
    return sum_of_geometric_polynomial_samples<FPS>(r, n, samples);
}

/**
 * @brief Return the rational-function value of `sum_{i=0}^infty r^i i^k`.
 * @see https://kk2a.github.io/library/fps/power_sum.hpp.html#sum-of-geometric-monomial-infinite
 */
template <fps::ModularUnivariateFormalPowerSeries FPS,
          modint::Modular mint = typename FPS::value_type,
          UnsignedIntegral T>
mint sum_of_geometric_monomial(mint r, T k) {
    return sum_of_geometric_polynomial_samples<FPS>(r, pow_table<mint>(k, k));
}

} // namespace kk2

#endif // KK2_FPS_POWER_SUM_HPP
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
    using FPS = kk2::FPSNTT<mint>;

    mint r;
    u64 d, n;
    kin >> r >> d >> n;
    kout << kk2::sum_of_geometric_monomial<FPS>(r, n, d) << kendl;
    return 0;
}
// Author: kk2
// converted by https://github.com/kk2a/cpp-bundle
// 2026-10-03 01:15:18
