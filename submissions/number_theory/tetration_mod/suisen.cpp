#define PROBLEM "https://judge.yosupo.jp/problem/tetration_mod"

#include <iostream>

#include <numeric>
#include <optional>

#include <cmath>
#include <iostream>
#include <random>
#include <utility>

#include <cassert>
#include <cstdint>
#include <iterator>

#include <limits>
#include <type_traits>

namespace suisen {
// ! utility
template <typename ...Types>
using constraints_t = std::enable_if_t<std::conjunction_v<Types...>, std::nullptr_t>;
template <bool cond_v, typename Then, typename OrElse>
constexpr decltype(auto) constexpr_if(Then&& then, OrElse&& or_else) {
    if constexpr (cond_v) {
        return std::forward<Then>(then);
    } else {
        return std::forward<OrElse>(or_else);
    }
}

// ! function
template <typename ReturnType, typename Callable, typename ...Args>
using is_same_as_invoke_result = std::is_same<std::invoke_result_t<Callable, Args...>, ReturnType>;
template <typename F, typename T>
using is_uni_op = is_same_as_invoke_result<T, F, T>;
template <typename F, typename T>
using is_bin_op = is_same_as_invoke_result<T, F, T, T>;

template <typename Comparator, typename T>
using is_comparator = std::is_same<std::invoke_result_t<Comparator, T, T>, bool>;

// ! integral
template <typename T, typename = constraints_t<std::is_integral<T>>>
constexpr int bit_num = std::numeric_limits<std::make_unsigned_t<T>>::digits;
template <typename T, unsigned int n>
struct is_nbit { static constexpr bool value = bit_num<T> == n; };
template <typename T, unsigned int n>
static constexpr bool is_nbit_v = is_nbit<T, n>::value;

// ?
template <typename T>
struct safely_multipliable {};
template <>
struct safely_multipliable<int> { using type = long long; };
template <>
struct safely_multipliable<long long> { using type = __int128_t; };
template <>
struct safely_multipliable<unsigned int> { using type = unsigned long long; };
template <>
struct safely_multipliable<unsigned long int> { using type = __uint128_t; };
template <>
struct safely_multipliable<unsigned long long> { using type = __uint128_t; };
template <>
struct safely_multipliable<float> { using type = float; };
template <>
struct safely_multipliable<double> { using type = double; };
template <>
struct safely_multipliable<long double> { using type = long double; };
template <typename T>
using safely_multipliable_t = typename safely_multipliable<T>::type;

template <typename T, typename = void>
struct rec_value_type {
    using type = T;
};
template <typename T>
struct rec_value_type<T, std::void_t<typename T::value_type>> {
    using type = typename rec_value_type<typename T::value_type>::type;
};
template <typename T>
using rec_value_type_t = typename rec_value_type<T>::type;

} // namespace suisen

namespace suisen::miller_rabin {
    namespace internal {
        constexpr uint32_t THRESHOLD_1 = 341531U;
        constexpr uint64_t BASE_1[] { 9345883071009581737ULL };

        constexpr uint32_t THRESHOLD_2 = 1050535501U;
        constexpr uint64_t BASE_2[] { 336781006125ULL, 9639812373923155ULL };

        constexpr uint64_t THRESHOLD_3 = 350269456337ULL;
        constexpr uint64_t BASE_3[] { 4230279247111683200ULL, 14694767155120705706ULL, 16641139526367750375ULL };

        constexpr uint64_t THRESHOLD_4 = 55245642489451ULL;
        constexpr uint64_t BASE_4[] { 2ULL, 141889084524735ULL, 1199124725622454117ULL, 11096072698276303650ULL };

        constexpr uint64_t THRESHOLD_5 = 7999252175582851ULL;
        constexpr uint64_t BASE_5[] { 2ULL, 4130806001517ULL, 149795463772692060ULL, 186635894390467037ULL, 3967304179347715805ULL };

        constexpr uint64_t THRESHOLD_6 = 585226005592931977ULL;
        constexpr uint64_t BASE_6[] { 2ULL, 123635709730000ULL, 9233062284813009ULL, 43835965440333360ULL, 761179012939631437ULL, 1263739024124850375ULL };

        constexpr uint32_t BASE_7[] { 2U, 325U, 9375U, 28178U, 450775U, 9780504U, 1795265022U };

        template <auto BASE, std::size_t SIZE, typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
        constexpr bool miller_rabin(T _n) {
            using U = std::make_unsigned_t<T>;
            using M = safely_multipliable_t<U>;

            U n = _n, d = (n - 1) >> __builtin_ctzll(n - 1);

            if (n == 2 or n == 3 or n == 5 or n == 7) return true;
            if (n % 2 == 0 or n % 3 == 0 or n % 5 == 0 or n % 7 == 0) return false;

            for (std::size_t i = 0; i < SIZE; ++i) {
                M y = 1, p = BASE[i] % n;
                if (p == 0) continue;
                for (U d2 = d; d2; d2 >>= 1) {
                    if (d2 & 1) y = y * p % n;
                    p = p * p % n;
                }
                if (y == 1) continue;
                for (U t = d; y != n - 1; t <<= 1) {
                    y = y * y % n;
                    if (y == 1 or t == n - 1) return false;
                }
            }
            return true;
        }
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
    constexpr bool is_prime(T n) {
        if (n <= 1) return false;
        using U = std::make_unsigned_t<T>;
        U n2 = n;
        using namespace internal;
        if (n2 < THRESHOLD_1) {
            return miller_rabin<BASE_1, 1>(n2);
        } else if (n2 < THRESHOLD_2) {
            return miller_rabin<BASE_2, 2>(n2);
        } else if (n2 < THRESHOLD_3) {
            return miller_rabin<BASE_3, 3>(n2);
        } else if (n2 < THRESHOLD_4) {
            return miller_rabin<BASE_4, 4>(n2);
        } else if (n2 < THRESHOLD_5) {
            return miller_rabin<BASE_5, 5>(n2);
        } else if (n2 < THRESHOLD_6) {
            return miller_rabin<BASE_6, 6>(n2);
        } else {
            return miller_rabin<BASE_7, 7>(n2);
        }
    }
} // namespace suisen::miller_rabin

#include <vector>

namespace suisen::internal::sieve {

constexpr std::uint8_t K = 8;
constexpr std::uint8_t PROD = 2 * 3 * 5;
constexpr std::uint8_t RM[K] = { 1,  7, 11, 13, 17, 19, 23, 29 };
constexpr std::uint8_t DR[K] = { 6,  4,  2,  4,  2,  4,  6,  2 };
constexpr std::uint8_t DF[K][K] = {
    { 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 0, 1, 1, 1, 1 },
    { 2, 2, 0, 2, 0, 2, 2, 1 }, { 3, 1, 1, 2, 1, 1, 3, 1 },
    { 3, 3, 1, 2, 1, 3, 3, 1 }, { 4, 2, 2, 2, 2, 2, 4, 1 },
    { 5, 3, 1, 4, 1, 3, 5, 1 }, { 6, 4, 2, 4, 2, 4, 6, 1 },
};
constexpr std::uint8_t DRP[K] = { 48, 32, 16, 32, 16, 32, 48, 16 };
constexpr std::uint8_t DFP[K][K] = {
    {  0,  0,  0,  0,  0,  0,  0,  8 }, {  8,  8,  8,  0,  8,  8,  8,  8 },
    { 16, 16,  0, 16,  0, 16, 16,  8 }, { 24,  8,  8, 16,  8,  8, 24,  8 },
    { 24, 24,  8, 16,  8, 24, 24,  8 }, { 32, 16, 16, 16, 16, 16, 32,  8 },
    { 40, 24,  8, 32,  8, 24, 40,  8 }, { 48, 32, 16, 32, 16, 32, 48,  8 },
};

constexpr std::uint8_t MASK[K][K] = {
    { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 }, { 0x02, 0x20, 0x10, 0x01, 0x80, 0x08, 0x04, 0x40 },
    { 0x04, 0x10, 0x01, 0x40, 0x02, 0x80, 0x08, 0x20 }, { 0x08, 0x01, 0x40, 0x20, 0x04, 0x02, 0x80, 0x10 },
    { 0x10, 0x80, 0x02, 0x04, 0x20, 0x40, 0x01, 0x08 }, { 0x20, 0x08, 0x80, 0x02, 0x40, 0x01, 0x10, 0x04 },
    { 0x40, 0x04, 0x08, 0x80, 0x01, 0x10, 0x20, 0x02 }, { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 },
};
constexpr std::uint8_t OFFSET[K][K] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, },
    { 1, 5, 4, 0, 7, 3, 2, 6, },
    { 2, 4, 0, 6, 1, 7, 3, 5, },
    { 3, 0, 6, 5, 2, 1, 7, 4, },
    { 4, 7, 1, 2, 5, 6, 0, 3, },
    { 5, 3, 7, 1, 6, 0, 4, 2, },
    { 6, 2, 3, 7, 0, 4, 5, 1, },
    { 7, 6, 5, 4, 3, 2, 1, 0, },
};

constexpr std::uint8_t mask_to_index(const std::uint8_t bits) {
    switch (bits) {
        case 1 << 0: return 0;
        case 1 << 1: return 1;
        case 1 << 2: return 2;
        case 1 << 3: return 3;
        case 1 << 4: return 4;
        case 1 << 5: return 5;
        case 1 << 6: return 6;
        case 1 << 7: return 7;
        default: assert(false);
    }
}
} // namespace suisen::internal::sieve

namespace suisen {

template <unsigned int N>
class SimpleSieve {
    private:
        static constexpr unsigned int siz = N / internal::sieve::PROD + 1;
        static std::uint8_t flag[siz];
    public:
        SimpleSieve() {
            using namespace internal::sieve;
            flag[0] |= 1;
            unsigned int k_max = (unsigned int) std::sqrt(N + 2) / PROD;
            for (unsigned int kp = 0; kp <= k_max; ++kp) {
                for (std::uint8_t bits = ~flag[kp]; bits; bits &= bits - 1) {
                    const std::uint8_t mp = mask_to_index(bits & -bits), m = RM[mp];
                    unsigned int kr = kp * (PROD * kp + 2 * m) + m * m / PROD;
                    for (std::uint8_t mq = mp; kr < siz; kr += kp * DR[mq] + DF[mp][mq], ++mq &= 7) {
                        flag[kr] |= MASK[mp][mq];
                    }
                }
            }
        }
        std::vector<int> prime_list(unsigned int max_val = N) const {
            using namespace internal::sieve;
            std::vector<int> res { 2, 3, 5 };
            res.reserve(max_val / 25);
            for (unsigned int i = 0, offset = 0; i < siz and offset < max_val; ++i, offset += PROD) {
                for (uint8_t f = ~flag[i]; f;) {
                    uint8_t g = f & -f;
                    res.push_back(offset + RM[mask_to_index(g)]);
                    f ^= g;
                }
            }
            while (res.size() and (unsigned int) res.back() > max_val) res.pop_back();
            return res;
        }
        bool is_prime(const unsigned int p) const {
            using namespace internal::sieve;
            switch (p) {
                case 2: case 3: case 5: return true;
                default:
                    switch (p % PROD) {
                        case RM[0]: return ((flag[p / PROD] >> 0) & 1) == 0;
                        case RM[1]: return ((flag[p / PROD] >> 1) & 1) == 0;
                        case RM[2]: return ((flag[p / PROD] >> 2) & 1) == 0;
                        case RM[3]: return ((flag[p / PROD] >> 3) & 1) == 0;
                        case RM[4]: return ((flag[p / PROD] >> 4) & 1) == 0;
                        case RM[5]: return ((flag[p / PROD] >> 5) & 1) == 0;
                        case RM[6]: return ((flag[p / PROD] >> 6) & 1) == 0;
                        case RM[7]: return ((flag[p / PROD] >> 7) & 1) == 0;
                        default: return false;
                    }
            }
        }
};
template <unsigned int N>
std::uint8_t SimpleSieve<N>::flag[SimpleSieve<N>::siz];

template <unsigned int N>
class Sieve {
    private:
        static constexpr unsigned int base_max = (N + 1) * internal::sieve::K / internal::sieve::PROD;
        static unsigned int pf[base_max + internal::sieve::K];

    public:
        Sieve() {
            using namespace internal::sieve;
            pf[0] = 1;
            unsigned int k_max = ((unsigned int) std::sqrt(N + 1) - 1) / PROD;
            for (unsigned int kp = 0; kp <= k_max; ++kp) {
                const int base_i = kp * K, base_act_i = kp * PROD;
                for (int mp = 0; mp < K; ++mp) {
                    const int m = RM[mp], i = base_i + mp;
                    if (pf[i] == 0) {
                        unsigned int act_i = base_act_i + m;
                        unsigned int base_k = (kp * (PROD * kp + 2 * m) + m * m / PROD) * K;
                        for (std::uint8_t mq = mp; base_k <= base_max; base_k += kp * DRP[mq] + DFP[mp][mq], ++mq &= 7) {
                            pf[base_k + OFFSET[mp][mq]] = act_i;
                        }
                    }
                }
            }
        }
        bool is_prime(const unsigned int p) const {
            using namespace internal::sieve;
            switch (p) {
                case 2: case 3: case 5: return true;
                default:
                    switch (p % PROD) {
                        case RM[0]: return pf[p / PROD * K + 0] == 0;
                        case RM[1]: return pf[p / PROD * K + 1] == 0;
                        case RM[2]: return pf[p / PROD * K + 2] == 0;
                        case RM[3]: return pf[p / PROD * K + 3] == 0;
                        case RM[4]: return pf[p / PROD * K + 4] == 0;
                        case RM[5]: return pf[p / PROD * K + 5] == 0;
                        case RM[6]: return pf[p / PROD * K + 6] == 0;
                        case RM[7]: return pf[p / PROD * K + 7] == 0;
                        default: return false;
                    }
            }
        }
        int prime_factor(const unsigned int p) const {
            using namespace internal::sieve;
            switch (p % PROD) {
                case  0: case  2: case  4: case  6: case  8:
                case 10: case 12: case 14: case 16: case 18:
                case 20: case 22: case 24: case 26: case 28: return 2;
                case  3: case  9: case 15: case 21: case 27: return 3;
                case  5: case 25: return 5;
                case RM[0]: return pf[p / PROD * K + 0] ? pf[p / PROD * K + 0] : p;
                case RM[1]: return pf[p / PROD * K + 1] ? pf[p / PROD * K + 1] : p;
                case RM[2]: return pf[p / PROD * K + 2] ? pf[p / PROD * K + 2] : p;
                case RM[3]: return pf[p / PROD * K + 3] ? pf[p / PROD * K + 3] : p;
                case RM[4]: return pf[p / PROD * K + 4] ? pf[p / PROD * K + 4] : p;
                case RM[5]: return pf[p / PROD * K + 5] ? pf[p / PROD * K + 5] : p;
                case RM[6]: return pf[p / PROD * K + 6] ? pf[p / PROD * K + 6] : p;
                case RM[7]: return pf[p / PROD * K + 7] ? pf[p / PROD * K + 7] : p;
                default: assert(false);
            }
        }
        /**
         * Returns a vector of `{ prime, index }`.
         */
        std::vector<std::pair<int, int>> factorize(unsigned int n) const {
            assert(0 < n and n <= N);
            std::vector<std::pair<int, int>> prime_powers;
            while (n > 1) {
                int p = prime_factor(n), c = 0;
                do { n /= p, ++c; } while (n % p == 0);
                prime_powers.emplace_back(p, c);
            }
            return prime_powers;
        }
        /**
         * Returns the divisors of `n`.
         * It is NOT guaranteed that the returned vector is sorted.
         */
        std::vector<int> divisors(unsigned int n) const {
            assert(0 < n and n <= N);
            std::vector<int> divs { 1 };
            for (auto [prime, index] : factorize(n)) {
                int sz = divs.size();
                for (int i = 0; i < sz; ++i) {
                    int d = divs[i];
                    for (int j = 0; j < index; ++j) {
                        divs.push_back(d *= prime);
                    }
                }
            }
            return divs;
        }
};
template <unsigned int N>
unsigned int Sieve<N>::pf[Sieve<N>::base_max + internal::sieve::K];
} // namespace suisen

namespace suisen::fast_factorize {
    namespace internal {
        template <typename T>
        constexpr int floor_log2(T n) {
            int i = 0;
            while (n) n >>= 1, ++i;
            return i - 1;
        }
        template <typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
        T pollard_rho(T n) {
            using M = safely_multipliable_t<T>;
            const T m = T(1) << (floor_log2(n) / 5);

            static std::mt19937_64 rng{std::random_device{}()};
            std::uniform_int_distribution<T> dist(0, n - 1);

            while (true) {
                T c = dist(rng);
                auto f = [&](T x) -> T { return (M(x) * x + c) % n; };
                T x, y = 2, ys, q = 1, g = 1;
                for (T r = 1; g == 1; r <<= 1) {
                    x = y;
                    for (T i = 0; i < r; ++i) y = f(y);
                    for (T k = 0; k < r and g == 1; k += m) {
                        ys = y;
                        for (T i = 0; i < std::min(m, r - k); ++i) y = f(y), q = M(q) * (x > y ? x - y : y - x) % n;
                        g = std::gcd(q, n);
                    }
                }
                if (g == n) {
                    g = 1;
                    while (g == 1) ys = f(ys), g = std::gcd(x > ys ? x - ys : ys - x, n);
                }
                if (g < n) {
                    if (miller_rabin::is_prime(g)) return g;
                    if (T d = n / g; miller_rabin::is_prime(d)) return d;
                    return pollard_rho(g);
                }
            }
        }
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
    std::vector<std::pair<T, int>> factorize(T n) {
        static constexpr int threshold = 1000000;
        static Sieve<threshold> sieve;

        if (n <= threshold) return sieve.factorize(n);

        std::vector<std::pair<T, int>> res;
        if ((n & 1) == 0) {
            int q = 0;
            do ++q, n >>= 1; while ((n & 1) == 0);
            res.emplace_back(2, q);
        }
        for (T p = 3; p * p <= n; p += 2) {
            if (p >= 101 and n >= 1 << 20) {
                while (n > 1) {
                    if (miller_rabin::is_prime(n)) {
                        res.emplace_back(std::exchange(n, 1), 1);
                    } else {
                        p = internal::pollard_rho(n);
                        int q = 0;
                        do ++q, n /= p; while (n % p == 0);
                        res.emplace_back(p, q);
                    }
                }
                break;
            }
            if (n % p == 0) {
                int q = 0;
                do ++q, n /= p; while (n % p == 0);
                res.emplace_back(p, q);
            }
        }
        if (n > 1) res.emplace_back(n, 1);
        return res;
    }
} // namespace suisen::fast_factorize

/**
 * @brief $a \uparrow \uparrow b \pmod{m}$
*/

namespace suisen {
    namespace internal::tetration_mod {
        int saturation_tetration(int a, int b) {
            constexpr int max_value = std::numeric_limits<int>::max();
            if (b == 0) return 1;
            if (a == 1) return 1;
            auto saturation_pow = [](int a, int b) -> int {
                if (b >= 32) return max_value;
                long long res = 1, pow_a = a;
                for (; b; b >>= 1) {
                    if (b & 1) res = std::min(res * pow_a, (long long) max_value);
                    pow_a = std::min(pow_a * pow_a, (long long) max_value);
                }
                return res;
            };
            int exponent = 1;
            for (int i = 0; i < b; ++i) {
                exponent = saturation_pow(a, exponent);
                if (exponent == max_value) break;
            }
            return exponent;
        }
        int pow_int(int a, int b) {
            int res = 1, pow_a = a;
            for (; b; b >>= 1) {
                if (b & 1) res *= pow_a;
                pow_a *= pow_a;
            }
            return res;
        }
        int pow_mod(int a, int b, int m) {
            long long res = 1, pow_a = a;
            for (; b; b >>= 1) {
                if (b & 1) res = (res * pow_a) % m;
                pow_a = (pow_a * pow_a) % m;
            }
            return res;
        }

        int tetration_mod_naive(int a, int b, int m) {
            int res = 1;
            for (int i = 0; i < b - 1; ++i) res = pow_int(a, res);
            return pow_mod(a, res, m);
        }

        int tetration_mod_impl(int a, int b, int m, int x) {
            if (m == 1) return x;
            if (a == 0) return 1 ^ (b & 1);
            if (b == 0) return 1;
            int nx = 0, m2 = m;
            while (true) {
                int g = std::gcd(m2, a);
                if (g == 1) break;
                m2 /= g, ++nx;
            }
            int phi_m2 = m2;
            for (auto [p, q] : fast_factorize::factorize(m2)) {
                phi_m2 /= p, phi_m2 *= p - 1;
            }
            int exponent = saturation_tetration(a, b - 1);
            int res = exponent >= nx ? pow_mod(a, tetration_mod_impl(a, b - 1, phi_m2, nx), m) : pow_mod(a, exponent, m);
            return res >= x ? res : res + m * ((x - res + m - 1) / m);
        }
    }

    /**
     * @brief Calculates a↑↑b mod m (= a^(a^(a^...(b times)...)) mod m)
     * @param a base
     * @param b number of power operations
     * @param m mod
     * @return a↑↑b mod m
     */
    int tetration_mod(int a, int b, int m) {
        return internal::tetration_mod::tetration_mod_impl(a, b, m, 0);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    for (int i = 0; i < t; ++i) {
        int a, b, m;
        std::cin >> a >> b >> m;
        std::cout << suisen::tetration_mod(a, b, m) << '\n';
    }

    return 0;
}

