#line 1 "test/formal_power_series/consecutive_terms_of_linear_recurrent_sequence.0.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/consecutive_terms_of_linear_recurrent_sequence"

#line 2 "c_recursive.hpp"

#line 2 "fft.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <vector>

template <typename Tp>
class FftInfo {
    static Tp least_quadratic_nonresidue() {
        for (int i = 2;; ++i)
            if (Tp(i).pow((Tp::mod() - 1) / 2) == -1) return Tp(i);
    }

    const int ordlog2_;
    const Tp zeta_;
    const Tp invzeta_;
    const Tp imag_;
    const Tp invimag_;

    mutable std::vector<Tp> root_;
    mutable std::vector<Tp> invroot_;

    FftInfo()
        : ordlog2_(__builtin_ctzll(Tp::mod() - 1)),
          zeta_(least_quadratic_nonresidue().pow((Tp::mod() - 1) >> ordlog2_)),
          invzeta_(zeta_.inv()), imag_(zeta_.pow(1LL << (ordlog2_ - 2))), invimag_(-imag_),
          root_{Tp(1), imag_}, invroot_{Tp(1), invimag_} {}

public:
    static const FftInfo &get() {
        static FftInfo info;
        return info;
    }

    Tp imag() const { return imag_; }
    Tp inv_imag() const { return invimag_; }
    Tp zeta() const { return zeta_; }
    Tp inv_zeta() const { return invzeta_; }
    const std::vector<Tp> &root(int n) const {
        // [0, n)
        assert((n & (n - 1)) == 0);
        if (const int s = root_.size(); s < n) {
            root_.resize(n);
            for (int i = __builtin_ctz(s); (1 << i) < n; ++i) {
                const int j = 1 << i;
                root_[j]    = zeta_.pow(1LL << (ordlog2_ - i - 2));
                for (int k = j + 1; k < j * 2; ++k) root_[k] = root_[k - j] * root_[j];
            }
        }
        return root_;
    }
    const std::vector<Tp> &inv_root(int n) const {
        // [0, n)
        assert((n & (n - 1)) == 0);
        if (const int s = invroot_.size(); s < n) {
            invroot_.resize(n);
            for (int i = __builtin_ctz(s); (1 << i) < n; ++i) {
                const int j = 1 << i;
                invroot_[j] = invzeta_.pow(1LL << (ordlog2_ - i - 2));
                for (int k = j + 1; k < j * 2; ++k) invroot_[k] = invroot_[k - j] * invroot_[j];
            }
        }
        return invroot_;
    }
};

inline int fft_len(int n) {
    --n;
    n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8;
    return (n | n >> 16) + 1;
}

namespace detail {

template <typename Iterator>
inline void
butterfly_n(Iterator a, int n,
            const std::vector<typename std::iterator_traits<Iterator>::value_type> &root) {
    assert(n > 0);
    assert((n & (n - 1)) == 0);
    const int bn = __builtin_ctz(n);
    if (bn & 1) {
        for (int i = 0; i < n / 2; ++i) {
            const auto a0 = a[i], a1 = a[i + n / 2];
            a[i] = a0 + a1, a[i + n / 2] = a0 - a1;
        }
    }
    for (int i = n >> (bn & 1); i >= 4; i /= 4) {
        const int i4 = i / 4;
        for (int k = 0; k < i4; ++k) {
            const auto a0 = a[k + i4 * 0], a1 = a[k + i4 * 1];
            const auto a2 = a[k + i4 * 2], a3 = a[k + i4 * 3];
            const auto a02p = a0 + a2, a02m = a0 - a2;
            const auto a13p = a1 + a3, a13m = (a1 - a3) * root[1];
            a[k + i4 * 0] = a02p + a13p, a[k + i4 * 1] = a02p - a13p;
            a[k + i4 * 2] = a02m + a13m, a[k + i4 * 3] = a02m - a13m;
        }
        for (int j = i, m = 2; j < n; j += i, m += 2) {
            const auto r = root[m], r2 = r * r, r3 = r2 * r;
            for (int k = j; k < j + i4; ++k) {
                const auto a0 = a[k + i4 * 0], a1 = a[k + i4 * 1] * r;
                const auto a2 = a[k + i4 * 2] * r2, a3 = a[k + i4 * 3] * r3;
                const auto a02p = a0 + a2, a02m = a0 - a2;
                const auto a13p = a1 + a3, a13m = (a1 - a3) * root[1];
                a[k + i4 * 0] = a02p + a13p, a[k + i4 * 1] = a02p - a13p;
                a[k + i4 * 2] = a02m + a13m, a[k + i4 * 3] = a02m - a13m;
            }
        }
    }
}

template <typename Iterator>
inline void
inv_butterfly_n(Iterator a, int n,
                const std::vector<typename std::iterator_traits<Iterator>::value_type> &root) {
    assert(n > 0);
    assert((n & (n - 1)) == 0);
    const int bn = __builtin_ctz(n);
    for (int i = 4; i <= (n >> (bn & 1)); i *= 4) {
        const int i4 = i / 4;
        for (int k = 0; k < i4; ++k) {
            const auto a0 = a[k + i4 * 0], a1 = a[k + i4 * 1];
            const auto a2 = a[k + i4 * 2], a3 = a[k + i4 * 3];
            const auto a01p = a0 + a1, a01m = a0 - a1;
            const auto a23p = a2 + a3, a23m = (a2 - a3) * root[1];
            a[k + i4 * 0] = a01p + a23p, a[k + i4 * 1] = a01m + a23m;
            a[k + i4 * 2] = a01p - a23p, a[k + i4 * 3] = a01m - a23m;
        }
        for (int j = i, m = 2; j < n; j += i, m += 2) {
            const auto r = root[m], r2 = r * r, r3 = r2 * r;
            for (int k = j; k < j + i4; ++k) {
                const auto a0 = a[k + i4 * 0], a1 = a[k + i4 * 1];
                const auto a2 = a[k + i4 * 2], a3 = a[k + i4 * 3];
                const auto a01p = a0 + a1, a01m = a0 - a1;
                const auto a23p = a2 + a3, a23m = (a2 - a3) * root[1];
                a[k + i4 * 0] = a01p + a23p, a[k + i4 * 1] = (a01m + a23m) * r;
                a[k + i4 * 2] = (a01p - a23p) * r2, a[k + i4 * 3] = (a01m - a23m) * r3;
            }
        }
    }
    if (bn & 1) {
        for (int i = 0; i < n / 2; ++i) {
            const auto a0 = a[i], a1 = a[i + n / 2];
            a[i] = a0 + a1, a[i + n / 2] = a0 - a1;
        }
    }
}

} // namespace detail

// FFT_n: A(x) |-> bit-reversed order of [A(1), A(zeta_n), ..., A(zeta_n^(n-1))]
template <typename Iterator>
inline void fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    detail::butterfly_n(a, n, FftInfo<Tp>::get().root(n / 2));
}

template <typename Tp>
inline void fft(std::vector<Tp> &a) {
    fft_n(a.begin(), a.size());
}

// IFFT_n: bit-reversed order of [A(1), A(zeta_n), ..., A(zeta_n^(n-1))] |-> A(x)
template <typename Iterator>
inline void inv_fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    detail::inv_butterfly_n(a, n, FftInfo<Tp>::get().inv_root(n / 2));
    const Tp iv = Tp::mod() - (Tp::mod() - 1) / n;
    for (int i = 0; i < n; ++i) a[i] *= iv;
}

template <typename Tp>
inline void inv_fft(std::vector<Tp> &a) {
    inv_fft_n(a.begin(), a.size());
}

// IFFT_n^T: A(x) |-> 1/n FFT_n((x^n A(x^(-1))) mod (x^n - 1))
template <typename Iterator>
inline void transposed_inv_fft_n(Iterator a, int n) {
    using Tp    = typename std::iterator_traits<Iterator>::value_type;
    const Tp iv = Tp::mod() - (Tp::mod() - 1) / n;
    for (int i = 0; i < n; ++i) a[i] *= iv;
    detail::butterfly_n(a, n, FftInfo<Tp>::get().inv_root(n / 2));
}

template <typename Tp>
inline void transposed_inv_fft(std::vector<Tp> &a) {
    transposed_inv_fft_n(a.begin(), a.size());
}

// FFT_n^T : FFT_n((x^n A(x^(-1))) mod (x^n - 1)) |-> n A(x)
template <typename Iterator>
inline void transposed_fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    detail::inv_butterfly_n(a, n, FftInfo<Tp>::get().root(n / 2));
}

template <typename Tp>
inline void transposed_fft(std::vector<Tp> &a) {
    transposed_fft_n(a.begin(), a.size());
}

template <typename Tp>
inline std::vector<Tp> convolution_fft(std::vector<Tp> a, std::vector<Tp> b) {
    if (a.empty() || b.empty()) return {};
    const int n   = a.size();
    const int m   = b.size();
    const int len = fft_len(n + m - 1);
    a.resize(len);
    b.resize(len);
    fft(a);
    fft(b);
    for (int i = 0; i < len; ++i) a[i] *= b[i];
    inv_fft(a);
    a.resize(n + m - 1);
    return a;
}

template <typename Tp>
inline std::vector<Tp> square_fft(std::vector<Tp> a) {
    if (a.empty()) return {};
    const int n   = a.size();
    const int len = fft_len(n * 2 - 1);
    a.resize(len);
    fft(a);
    for (int i = 0; i < len; ++i) a[i] *= a[i];
    inv_fft(a);
    a.resize(n * 2 - 1);
    return a;
}

template <typename Tp>
inline std::vector<Tp> convolution_naive(const std::vector<Tp> &a, const std::vector<Tp> &b) {
    if (a.empty() || b.empty()) return {};
    const int n = a.size();
    const int m = b.size();
    std::vector<Tp> res(n + m - 1);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) res[i + j] += a[i] * b[j];
    return res;
}

template <typename Tp>
inline std::vector<Tp> convolution(const std::vector<Tp> &a, const std::vector<Tp> &b) {
    if (std::min(a.size(), b.size()) < 60) return convolution_naive(a, b);
    if (std::addressof(a) == std::addressof(b)) return square_fft(a);
    return convolution_fft(a, b);
}
#line 2 "fft_doubling.hpp"

#line 8 "fft_doubling.hpp"

template <typename Iterator>
inline void fft_doubling_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    assert((n & (n - 1)) == 0);
    std::copy_n(a, n, a + n);
    inv_fft_n(a + n, n);
    Tp k         = 1;
    const auto t = FftInfo<Tp>::get().root(n).at(n / 2);
    for (int i = 0; i < n; ++i) a[i + n] *= k, k *= t;
    fft_n(a + n, n);
}

template <typename Tp>
inline void fft_doubling(std::vector<Tp> &a) {
    const int n = a.size();
    a.resize(n * 2);
    fft_doubling_n(a.begin(), n);
}
#line 2 "fps_basic.hpp"

#line 2 "binomial.hpp"

#line 5 "binomial.hpp"

template <typename Tp>
class Binomial {
    std::vector<Tp> factorial_, invfactorial_;

    Binomial() : factorial_{Tp(1)}, invfactorial_{Tp(1)} {}

    void preprocess(int n) {
        if (const int nn = factorial_.size(); nn < n) {
            int k = nn;
            while (k < n) k *= 2;
            k = std::min<long long>(k, Tp::mod());
            factorial_.resize(k);
            invfactorial_.resize(k);
            for (int i = nn; i < k; ++i) factorial_[i] = factorial_[i - 1] * i;
            invfactorial_.back() = factorial_.back().inv();
            for (int i = k - 2; i >= nn; --i) invfactorial_[i] = invfactorial_[i + 1] * (i + 1);
        }
    }

public:
    static const Binomial &get(int n) {
        static Binomial bin;
        bin.preprocess(n);
        return bin;
    }

    Tp binom(int n, int m) const {
        return n < m ? Tp() : factorial_[n] * invfactorial_[m] * invfactorial_[n - m];
    }
    Tp inv(int n) const { return factorial_[n - 1] * invfactorial_[n]; }
    Tp factorial(int n) const { return factorial_[n]; }
    Tp inv_factorial(int n) const { return invfactorial_[n]; }
};
#line 2 "semi_relaxed_conv.hpp"

#line 5 "semi_relaxed_conv.hpp"
#include <type_traits>
#include <utility>
#line 8 "semi_relaxed_conv.hpp"

template <typename Tp, typename Closure>
inline std::enable_if_t<std::is_invocable_r_v<Tp, Closure, int, const std::vector<Tp> &>,
                        std::vector<Tp>>
semi_relaxed_convolution_naive(const std::vector<Tp> &A, Closure gen, int n) {
    std::vector<Tp> B(n), AB(n);
    for (int i = 0; i < n; ++i) {
        for (int j = std::max(0, i - (int)A.size() + 1); j < i; ++j) AB[i] += A[i - j] * B[j];
        B[i] = gen(i, AB);
        if (!A.empty()) AB[i] += A[0] * B[i];
    }
    return B;
}

// returns coefficients generated by closure
// closure: gen(index, current_product)
template <typename Tp, typename Closure>
inline std::enable_if_t<std::is_invocable_r_v<Tp, Closure, int, const std::vector<Tp> &>,
                        std::vector<Tp>>
semi_relaxed_convolution(const std::vector<Tp> &A, Closure gen, int n) {
    if (A.size() < 60) return semi_relaxed_convolution_naive(A, gen, n);
    enum { BaseCaseSize = 32 };
    static_assert((BaseCaseSize & (BaseCaseSize - 1)) == 0);

    static const int Block[]     = {16, 16, 16, 16, 16};
    static const int BlockSize[] = {
        BaseCaseSize,
        BaseCaseSize * Block[0],
        BaseCaseSize * Block[0] * Block[1],
        BaseCaseSize * Block[0] * Block[1] * Block[2],
        BaseCaseSize * Block[0] * Block[1] * Block[2] * Block[3],
        BaseCaseSize * Block[0] * Block[1] * Block[2] * Block[3] * Block[4],
    };

    // returns (which_block, level)
    auto blockinfo = [](int ind) {
        int i = ind / BaseCaseSize, lv = 0;
        while ((i & (Block[lv] - 1)) == 0) i /= Block[lv++];
        return std::make_pair(i & (Block[lv] - 1), lv);
    };

    std::vector<Tp> B(n), AB(n);
    std::vector<std::vector<std::vector<Tp>>> dftA, dftB;

    for (int i = 0; i < n; ++i) {
        const int s = i & (BaseCaseSize - 1);

        // blocked contribution
        if (i >= BaseCaseSize && s == 0) {
            const auto [j, lv]  = blockinfo(i);
            const int blocksize = BlockSize[lv];

            if (blocksize * j == i) {
                if ((int)dftA.size() == lv) {
                    dftA.emplace_back();
                    dftB.emplace_back(Block[lv] - 1);
                }
                if ((j - 1) * blocksize < (int)A.size()) {
                    dftA[lv]
                        .emplace_back(A.begin() + (j - 1) * blocksize,
                                      A.begin() + std::min<int>((j + 1) * blocksize, A.size()))
                        .resize(blocksize * 2);
                    fft(dftA[lv][j - 1]);
                }
            }

            if (!dftA[lv].empty()) {
                dftB[lv][j - 1].resize(blocksize * 2);
                std::copy_n(B.begin() + (i - blocksize), blocksize, dftB[lv][j - 1].begin());
                std::fill_n(dftB[lv][j - 1].begin() + blocksize, blocksize, Tp(0));
                fft(dftB[lv][j - 1]);

                // middle product
                std::vector<Tp> mp(blocksize * 2);
                for (int k = 0; k < std::min<int>(j, dftA[lv].size()); ++k)
                    for (int l = 0; l < blocksize * 2; ++l)
                        mp[l] += dftA[lv][k][l] * dftB[lv][j - 1 - k][l];
                inv_fft(mp);

                for (int k = 0; k < blocksize && i + k < n; ++k) AB[i + k] += mp[k + blocksize];
            }
        }

        // basecase contribution
        for (int j = std::max(i - s, i - (int)A.size() + 1); j < i; ++j) AB[i] += A[i - j] * B[j];
        B[i] = gen(i, AB);
        if (!A.empty()) AB[i] += A[0] * B[i];
    }

    return B;
}
#line 7 "fps_basic.hpp"

template <typename Tp>
inline int order(const std::vector<Tp> &a) {
    for (int i = 0; i < (int)a.size(); ++i)
        if (a[i] != 0) return i;
    return -1;
}

template <typename Tp>
inline std::vector<Tp> fps_inv(const std::vector<Tp> &a, int n) {
    assert(!a.empty());
    if (n <= 0) return {};
    return semi_relaxed_convolution(
        a, [v = a[0].inv()](int n, auto &&c) { return n == 0 ? v : -c[n] * v; }, n);
}

template <typename Tp>
inline std::vector<Tp> fps_div(const std::vector<Tp> &a, const std::vector<Tp> &b, int n) {
    assert(!b.empty());
    if (n <= 0) return {};
    return semi_relaxed_convolution(
        b,
        [&, v = b[0].inv()](int n, auto &&c) {
            if (n < (int)a.size()) return (a[n] - c[n]) * v;
            return -c[n] * v;
        },
        n);
}

template <typename Tp>
inline std::vector<Tp> deriv(const std::vector<Tp> &a) {
    const int n = (int)a.size() - 1;
    if (n <= 0) return {};
    std::vector<Tp> res(n);
    for (int i = 1; i <= n; ++i) res[i - 1] = a[i] * i;
    return res;
}

template <typename Tp>
inline std::vector<Tp> integr(const std::vector<Tp> &a, Tp c = {}) {
    const int n = a.size() + 1;
    auto &&bin  = Binomial<Tp>::get(n);
    std::vector<Tp> res(n);
    res[0] = c;
    for (int i = 1; i < n; ++i) res[i] = a[i - 1] * bin.inv(i);
    return res;
}

template <typename Tp>
inline std::vector<Tp> fps_log(const std::vector<Tp> &a, int n) {
    return integr(fps_div(deriv(a), a, n - 1));
}

template <typename Tp>
inline std::vector<Tp> fps_exp(const std::vector<Tp> &a, int n) {
    if (n <= 0) return {};
    assert(!a.empty() && a[0] == 0);
    return semi_relaxed_convolution(
        deriv(a),
        [bin = Binomial<Tp>::get(n)](int n, auto &&c) {
            return n == 0 ? Tp(1) : c[n - 1] * bin.inv(n);
        },
        n);
}

template <typename Tp>
inline std::vector<Tp> fps_pow(std::vector<Tp> a, long long e, int n) {
    if (n <= 0) return {};
    if (e == 0) {
        std::vector<Tp> res(n);
        res[0] = 1;
        return res;
    }

    const int o = order(a);
    if (o < 0 || o > n / e || (o == n / e && n % e == 0)) return std::vector<Tp>(n);
    if (o != 0) a.erase(a.begin(), a.begin() + o);

    const Tp ia0 = a[0].inv();
    const Tp a0e = a[0].pow(e);
    const Tp me  = e;

    for (int i = 0; i < (int)a.size(); ++i) a[i] *= ia0;
    a = fps_log(a, n - o * e);
    for (int i = 0; i < (int)a.size(); ++i) a[i] *= me;
    a = fps_exp(a, n - o * e);
    for (int i = 0; i < (int)a.size(); ++i) a[i] *= a0e;

    a.insert(a.begin(), o * e, Tp(0));
    return a;
}
#line 2 "poly_basic.hpp"

#line 10 "poly_basic.hpp"

template <typename Tp>
inline int degree(const std::vector<Tp> &a) {
    int n = (int)a.size() - 1;
    while (n >= 0 && a[n] == 0) --n;
    return n;
}

template <typename Tp>
inline void shrink(std::vector<Tp> &a) {
    a.resize(degree(a) + 1);
}

template <typename Tp>
inline std::vector<Tp> taylor_shift(std::vector<Tp> a, Tp c) {
    const int n = a.size();
    auto &&bin  = Binomial<Tp>::get(n);
    for (int i = 0; i < n; ++i) a[i] *= bin.factorial(i);
    Tp cc = 1;
    std::vector<Tp> b(n);
    for (int i = 0; i < n; ++i) {
        b[i] = cc * bin.inv_factorial(i);
        cc *= c;
    }
    std::reverse(a.begin(), a.end());
    auto ab = convolution(a, b);
    ab.resize(n);
    std::reverse(ab.begin(), ab.end());
    for (int i = 0; i < n; ++i) ab[i] *= bin.inv_factorial(i);
    return ab;
}

// returns (quotient, remainder)
// O(deg(Q)deg(B))
template <typename Tp>
inline std::pair<std::vector<Tp>, std::vector<Tp>> euclidean_div_naive(const std::vector<Tp> &A,
                                                                       const std::vector<Tp> &B) {
    const int degA = degree(A);
    const int degB = degree(B);
    assert(degB >= 0);
    const int degQ = degA - degB;
    if (degQ < 0) return {std::vector<Tp>{Tp(0)}, A};
    std::vector<Tp> Q(degQ + 1), R = A;
    const auto inv = B[degB].inv();
    for (int i = degQ, n = degA; i >= 0; --i)
        if ((Q[i] = R[n--] * inv) != 0)
            for (int j = 0; j <= degB; ++j) R[i + j] -= Q[i] * B[j];
    R.resize(degB);
    return {Q, R};
}

// O(min(deg(Q)^2,deg(Q)deg(B)))
template <typename Tp>
inline std::vector<Tp> euclidean_div_quotient_naive(const std::vector<Tp> &A,
                                                    const std::vector<Tp> &B) {
    const int degA = degree(A);
    const int degB = degree(B);
    assert(degB >= 0);
    const int degQ = degA - degB;
    if (degQ < 0) return {Tp(0)};
    const auto inv = B[degB].inv();
    std::vector<Tp> Q(degQ + 1);
    for (int i = 0; i <= degQ; ++i) {
        for (int j = 1; j <= std::min(i, degB); ++j) Q[degQ - i] += B[degB - j] * Q[degQ - i + j];
        Q[degQ - i] = (A[degA - i] - Q[degQ - i]) * inv;
    }
    return Q;
}

// returns (quotient, remainder)
template <typename Tp>
inline std::pair<std::vector<Tp>, std::vector<Tp>> euclidean_div(const std::vector<Tp> &A,
                                                                 const std::vector<Tp> &B) {
    const int degA = degree(A);
    const int degB = degree(B);
    assert(degB >= 0);
    // A = Q*B + R => A/B = Q + R/B in R((x^(-1)))
    const int degQ = degA - degB;
    if (degQ < 0) return {std::vector<Tp>{Tp(0)}, A};
    if (degQ < 60 || degB < 60) return euclidean_div_naive(A, B);

    auto Q = fps_div(std::vector(A.rend() - (degA + 1), A.rend()),
                     std::vector(B.rend() - (degB + 1), B.rend()), degQ + 1);
    std::reverse(Q.begin(), Q.end());

    // returns a mod (x^n-1)
    auto make_cyclic = [](const std::vector<Tp> &a, int n) {
        assert((n & (n - 1)) == 0);
        std::vector<Tp> b(n);
        for (int i = 0; i < (int)a.size(); ++i) b[i & (n - 1)] += a[i];
        return b;
    };

    const int len      = fft_len(std::max(degB, 1));
    const auto cyclicA = make_cyclic(A, len);
    auto cyclicB       = make_cyclic(B, len);
    auto cyclicQ       = make_cyclic(Q, len);

    fft(cyclicQ);
    fft(cyclicB);
    for (int i = 0; i < len; ++i) cyclicQ[i] *= cyclicB[i];
    inv_fft(cyclicQ);

    // R = A - QB mod (x^n-1) (n >= degB)
    std::vector<Tp> R(degB);
    for (int i = 0; i < degB; ++i) R[i] = cyclicA[i] - cyclicQ[i];
    return {Q, R};
}

template <typename Tp>
inline std::vector<Tp> euclidean_div_quotient(const std::vector<Tp> &A, const std::vector<Tp> &B) {
    const int degA = degree(A);
    const int degB = degree(B);
    assert(degB >= 0);
    // A = Q*B + R => A/B = Q + R/B in R((x^(-1)))
    const int degQ = degA - degB;
    if (degQ < 0) return {Tp(0)};
    if (std::min(degQ, degB) < 60) return euclidean_div_quotient_naive(A, B);

    auto Q = fps_div(std::vector(A.rend() - (degA + 1), A.rend()),
                     std::vector(B.rend() - (degB + 1), B.rend()), degQ + 1);
    std::reverse(Q.begin(), Q.end());
    return Q;
}
#line 10 "c_recursive.hpp"

// see:
// [1]: Alin Bostan, Ryuhei Mori.
//      A Simple and Fast Algorithm for Computing the N-th Term of a Linearly Recurrent Sequence.
//      https://arxiv.org/abs/2008.08822

template <typename Tp>
inline std::vector<Tp> fps_inv_newton(const std::vector<Tp> &a, int n) {
    assert(!a.empty());
    if (n <= 0) return {};
    const int len = fft_len(n);
    std::vector<Tp> invA(len), shopA(len), shopB(len);
    invA[0] = a[0].inv();
    for (int i = 2; i <= len; i *= 2) {
        std::fill(std::copy_n(a.begin(), std::min<int>(a.size(), i), shopA.begin()),
                  shopA.begin() + i, Tp(0));
        std::copy_n(invA.begin(), i, shopB.begin());
        fft_n(shopA.begin(), i);
        fft_n(shopB.begin(), i);
        for (int j = 0; j < i; ++j) shopA[j] *= shopB[j];
        inv_fft_n(shopA.begin(), i);
        std::fill_n(shopA.begin(), i / 2, Tp(0));
        fft_n(shopA.begin(), i);
        for (int j = 0; j < i; ++j) shopA[j] *= shopB[j];
        inv_fft_n(shopA.begin(), i);
        for (int j = i / 2; j < i; ++j) invA[j] = -shopA[j];
    }
    invA.resize(n);
    return invA;
}

template <typename Tp>
inline void fft_high(std::vector<Tp> &a) {
    const int n = a.size();
    inv_fft_n(a.begin() + n / 2, n / 2);
    Tp k         = 1;
    const auto t = FftInfo<Tp>::get().inv_root(n / 2).at(n / 4);
    for (int i = 0; i < n / 2; ++i) a[i + n / 2] *= k, k *= t;
    fft_n(a.begin() + n / 2, n / 2);
    for (int i = 0; i < n / 2; ++i) a[i] = (a[i] - a[i + n / 2]).div_by_2();
    a.resize(n / 2);
}

// returns DFT([x^[L,L+len/2)]1/Q)
// 1/Q in R((x))
// requires len/2 > deg(Q), len/2 is even
template <typename Tp>
inline std::vector<Tp> bostan_mori_laurent_series(std::vector<Tp> dftQ, long long L) {
    const int len = dftQ.size() * 2;
    if (L <= 0) {
        inv_fft(dftQ);
        const int ordQ = order(dftQ);
        assert(ordQ >= 0);
        if (L + len / 2 <= -ordQ) return std::vector<Tp>(len / 2);
        auto invQ =
            fps_inv_newton(std::vector(dftQ.begin() + ordQ, dftQ.end()), L + len / 2 + ordQ);
        if (-ordQ < (int)L) {
            // ?x^(-ord(Q)) + ... + ?x^L + ... + ?x^(L+len/2-1)
            invQ.erase(invQ.begin(), invQ.begin() + (L + ordQ));
        } else {
            // ?x^L + ... + ?x^(-ord(Q)) + ... + ?x^(L+len/2-1)
            invQ.insert(invQ.begin(), -ordQ - L, Tp(0));
        }
        fft(invQ);
        return invQ;
    }

    fft_doubling(dftQ);
    std::vector<Tp> dftV(len / 2);
    for (int i = 0; i < len; i += 2) dftV[i / 2] = dftQ[i] * dftQ[i + 1];
    const auto dftT =
        bostan_mori_laurent_series(dftV, (L - len / 2 + (L & 1)) / 2 /* ceil((L-len/2)/2) */);

    std::vector<Tp> dftU(len);
    if (L & 1) {
        auto &&root = FftInfo<Tp>::get().root(len / 2);
        for (int i = 0; i < len; i += 2) {
            dftU[i]     = dftT[i / 2] * dftQ[i + 1] * root[i / 2];
            dftU[i + 1] = dftT[i / 2] * dftQ[i] * -root[i / 2];
        }
    } else {
        for (int i = 0; i < len; i += 2) {
            dftU[i]     = dftT[i / 2] * dftQ[i + 1];
            dftU[i + 1] = dftT[i / 2] * dftQ[i];
        }
    }

    fft_high(dftU);
    return dftU;
}

// returns DFT([x^[-len/2,0)]x^k/Q)
// x^k/Q in R((x^(-1)))
// requires len/2 > degQ
template <typename Tp>
inline std::vector<Tp> bostan_mori_reversed_laurent_series(std::vector<Tp> dftQ, long long k) {
    assert(k >= 0);
    const int len = dftQ.size() * 2;
    if (k < len / 2LL) {
        inv_fft(dftQ);
        const int degQ = degree(dftQ);
        assert(degQ >= 0);
        dftQ.resize(degQ + 1);
        std::reverse(dftQ.begin(), dftQ.end());
        auto invQ = fps_inv_newton(dftQ, len / 2 - degQ + k + 1);
        std::reverse(invQ.begin(), invQ.end());
        invQ.resize(len / 2);
        fft(invQ);
        return invQ;
    }

    fft_doubling(dftQ);
    std::vector<Tp> dftV(len / 2);
    for (int i = 0; i < len; i += 2) dftV[i / 2] = dftQ[i] * dftQ[i + 1];
    const auto dftT = bostan_mori_reversed_laurent_series(dftV, k / 2);

    std::vector<Tp> dftU(len);
    if (k & 1) {
        auto &&root = FftInfo<Tp>::get().root(len / 2);
        for (int i = 0; i < len; i += 2) {
            dftU[i]     = dftT[i / 2] * dftQ[i + 1] * root[i / 2];
            dftU[i + 1] = dftT[i / 2] * dftQ[i] * -root[i / 2];
        }
    } else {
        for (int i = 0; i < len; i += 2) {
            dftU[i]     = dftT[i / 2] * dftQ[i + 1];
            dftU[i + 1] = dftT[i / 2] * dftQ[i];
        }
    }

    fft_high(dftU);
    return dftU;
}

// returns x^k mod Q
template <typename Tp>
inline std::vector<Tp> xk_mod(long long k, const std::vector<Tp> &Q) {
    assert(k >= 0);
    const int degQ = degree(Q);
    assert(degQ >= 0);
    if (degQ == 0) return {};
    if (k < degQ) {
        std::vector<Tp> res(degQ);
        res[k] = 1;
        return res;
    }

    const int len = fft_len(degQ * 2 + 1);
    if (k < len / 2LL) {
        auto invQ = fps_inv_newton(std::vector(Q.rend() - (degQ + 1), Q.rend()), k + 1);
        std::reverse(invQ.begin(), invQ.end());
        invQ.resize(degQ);
        auto res = convolution(invQ, Q);
        res.erase(res.begin(), res.begin() + degQ);
        res.resize(degQ);
        return res;
    }

    auto dftQ = std::vector(Q.rend() - (degQ + 1), Q.rend());
    dftQ.resize(len);
    fft(dftQ);
    std::vector<Tp> dftV(len / 2);
    for (int i = 0; i < len; i += 2) dftV[i / 2] = dftQ[i] * dftQ[i + 1];
    const long long L = k + 1 - degQ;
    const auto dftT   = bostan_mori_laurent_series(dftV, (L - len / 2 + (L & 1)) / 2);
    std::vector<Tp> dftU(len);
    if (L & 1) {
        auto &&root = FftInfo<Tp>::get().root(len / 2);
        for (int i = 0; i < len; i += 2) {
            dftU[i]     = dftT[i / 2] * dftQ[i + 1] * root[i / 2];
            dftU[i + 1] = dftT[i / 2] * dftQ[i] * -root[i / 2];
        }
    } else {
        for (int i = 0; i < len; i += 2) {
            dftU[i]     = dftT[i / 2] * dftQ[i + 1];
            dftU[i + 1] = dftT[i / 2] * dftQ[i];
        }
    }
    inv_fft(dftU);
    dftU.erase(dftU.begin(), dftU.begin() + len / 2);
    dftU.resize(degQ);
    dftU.resize(len);
    fft(dftU);
    for (int i = 0; i < len; ++i) dftU[i] *= dftQ[i];
    inv_fft(dftU);
    dftU.resize(degQ);
    std::reverse(dftU.begin(), dftU.end());
    return dftU;
}

// returns [x^[L,R)]P/Q
// P: polynomial
// Q: non-zero polynomial, ord(Q)=0
template <typename Tp>
inline std::vector<Tp> slice_coeff_rational(const std::vector<Tp> &P, const std::vector<Tp> &Q,
                                            long long L, long long R) {
    assert(L >= 0);
    assert(order(Q) == 0);
    const int degP = degree(P);
    if (degP < 0) return std::vector<Tp>(R - L);
    const int degQ = degree(Q);
    const int N    = std::max(degP + 1, degQ);
    auto P0 = P, Q0 = Q;
    P0.resize(N);
    std::reverse(P0.begin(), P0.end());
    Q0.resize(N + 1);
    std::reverse(Q0.begin(), Q0.end());
    auto [q, r] = euclidean_div(convolution(xk_mod(L, Q0), P0), Q0);
    r.resize(N);
    std::reverse(r.begin(), r.end());
    return fps_div(r, Q, R - L);
}

// returns [x^k]P/Q
// P: polynomial
// Q: non-zero polynomial, ord(Q)=0
template <typename Tp>
inline Tp div_at(const std::vector<Tp> &P, const std::vector<Tp> &Q, long long k) {
    return slice_coeff_rational(P, Q, k, k + 1).at(0);
}
#line 2 "modint.hpp"

#include <iostream>
#line 5 "modint.hpp"

template <unsigned Mod>
class ModInt {
    static_assert((Mod >> 31) == 0, "`Mod` must less than 2^(31)");
    template <typename Int>
    static std::enable_if_t<std::is_integral_v<Int>, unsigned> safe_mod(Int v) {
        using D = std::common_type_t<Int, unsigned>;
        return (v %= (int)Mod) < 0 ? (D)(v + (int)Mod) : (D)v;
    }

    struct PrivateConstructor {};
    static inline PrivateConstructor private_constructor{};
    ModInt(PrivateConstructor, unsigned v) : v_(v) {}

    unsigned v_;

public:
    static unsigned mod() { return Mod; }
    static ModInt from_raw(unsigned v) { return ModInt(private_constructor, v); }
    ModInt() : v_() {}
    template <typename Int, typename std::enable_if_t<std::is_signed_v<Int>, int> = 0>
    ModInt(Int v) : v_(safe_mod(v)) {}
    template <typename Int, typename std::enable_if_t<std::is_unsigned_v<Int>, int> = 0>
    ModInt(Int v) : v_(v % Mod) {}
    unsigned val() const { return v_; }

    ModInt operator-() const { return from_raw(v_ == 0 ? v_ : Mod - v_); }
    ModInt pow(long long e) const {
        if (e < 0) return inv().pow(-e);
        for (ModInt x(*this), res(from_raw(1));; x *= x) {
            if (e & 1) res *= x;
            if ((e >>= 1) == 0) return res;
        }
    }
    ModInt inv() const {
        int x1 = 1, x3 = 0, a = val(), b = Mod;
        while (b) {
            int q = a / b, x1_old = x1, a_old = a;
            x1 = x3, x3 = x1_old - x3 * q, a = b, b = a_old - b * q;
        }
        return from_raw(x1 < 0 ? x1 + (int)Mod : x1);
    }
    template <bool Odd = (Mod & 1)>
    std::enable_if_t<Odd, ModInt> div_by_2() const {
        if (v_ & 1) return from_raw((v_ + Mod) >> 1);
        return from_raw(v_ >> 1);
    }

    ModInt &operator+=(const ModInt &a) {
        if ((v_ += a.v_) >= Mod) v_ -= Mod;
        return *this;
    }
    ModInt &operator-=(const ModInt &a) {
        if ((v_ += Mod - a.v_) >= Mod) v_ -= Mod;
        return *this;
    }
    ModInt &operator*=(const ModInt &a) {
        v_ = (unsigned long long)v_ * a.v_ % Mod;
        return *this;
    }
    ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

    friend ModInt operator+(const ModInt &a, const ModInt &b) { return ModInt(a) += b; }
    friend ModInt operator-(const ModInt &a, const ModInt &b) { return ModInt(a) -= b; }
    friend ModInt operator*(const ModInt &a, const ModInt &b) { return ModInt(a) *= b; }
    friend ModInt operator/(const ModInt &a, const ModInt &b) { return ModInt(a) /= b; }
    friend bool operator==(const ModInt &a, const ModInt &b) { return a.v_ == b.v_; }
    friend bool operator!=(const ModInt &a, const ModInt &b) { return a.v_ != b.v_; }
    friend std::istream &operator>>(std::istream &a, ModInt &b) {
        int v;
        a >> v;
        b.v_ = safe_mod(v);
        return a;
    }
    friend std::ostream &operator<<(std::ostream &a, const ModInt &b) { return a << b.val(); }
};
#line 8 "test/formal_power_series/consecutive_terms_of_linear_recurrent_sequence.0.test.cpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    using mint = ModInt<998244353>;
    int d;
    long long k;
    int M;
    std::cin >> d >> k >> M;
    std::vector<mint> Q(d + 1), init_v(d);
    for (int i = 0; i < d; ++i) std::cin >> init_v[i];
    Q[0] = 1;
    for (int i = 1; i <= d; ++i) {
        std::cin >> Q[i];
        Q[i] = -Q[i];
    }
    auto P = convolution(Q, init_v);
    P.resize(d);
    const auto res = slice_coeff_rational(P, Q, k, k + M);
    for (int i = 0; i < (int)res.size(); ++i) std::cout << res[i] << ' ';
    return 0;
}