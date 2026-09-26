#line 1 "test/conversion_from_monomial_basis_to_newton_basis.0.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/conversion_from_monomial_basis_to_newton_basis"

#line 2 "modint.hpp"

#include <iostream>
#include <type_traits>

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
#line 2 "subproduct_tree.hpp"

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

template <typename Iterator>
inline void fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    assert((n & (n - 1)) == 0);
    for (int j = 0; j < n / 2; ++j) {
        auto u = a[j], v = a[j + n / 2];
        a[j] = u + v, a[j + n / 2] = u - v;
    }
    auto &&root = FftInfo<Tp>::get().root(n / 2);
    for (int i = n / 2; i >= 2; i /= 2) {
        for (int j = 0; j < i / 2; ++j) {
            auto u = a[j], v = a[j + i / 2];
            a[j] = u + v, a[j + i / 2] = u - v;
        }
        for (int j = i, m = 1; j < n; j += i, ++m)
            for (int k = j; k < j + i / 2; ++k) {
                auto u = a[k], v = a[k + i / 2] * root[m];
                a[k] = u + v, a[k + i / 2] = u - v;
            }
    }
}

template <typename Tp>
inline void fft(std::vector<Tp> &a) {
    fft_n(a.begin(), a.size());
}

template <typename Iterator>
inline void inv_fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    assert((n & (n - 1)) == 0);
    auto &&root = FftInfo<Tp>::get().inv_root(n / 2);
    for (int i = 2; i < n; i *= 2) {
        for (int j = 0; j < i / 2; ++j) {
            auto u = a[j], v = a[j + i / 2];
            a[j] = u + v, a[j + i / 2] = u - v;
        }
        for (int j = i, m = 1; j < n; j += i, ++m)
            for (int k = j; k < j + i / 2; ++k) {
                auto u = a[k], v = a[k + i / 2];
                a[k] = u + v, a[k + i / 2] = (u - v) * root[m];
            }
    }
    const Tp iv = Tp::mod() - Tp::mod() / n;
    for (int j = 0; j < n / 2; ++j) {
        auto u = a[j] * iv, v = a[j + n / 2] * iv;
        a[j] = u + v, a[j + n / 2] = u - v;
    }
}

template <typename Tp>
inline void inv_fft(std::vector<Tp> &a) {
    inv_fft_n(a.begin(), a.size());
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

#line 6 "semi_relaxed_conv.hpp"
#include <utility>
#line 8 "semi_relaxed_conv.hpp"

// returns coefficients generated by closure
// closure: gen(index, current_product)
template <typename Tp, typename Closure>
inline std::enable_if_t<std::is_invocable_r_v<Tp, Closure, int, const std::vector<Tp> &>,
                        std::vector<Tp>>
semi_relaxed_convolution(const std::vector<Tp> &A, Closure gen, int n) {
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
                                      A.begin() + std::min((j + 1) * blocksize, (int)A.size()))
                        .resize(blocksize * 2);
                    fft(dftA[lv][j - 1]);
                } else {
                    dftA[lv].emplace_back(blocksize * 2);
                }
            }

            dftB[lv][j - 1].resize(blocksize * 2);
            std::copy_n(B.begin() + (i - blocksize), blocksize, dftB[lv][j - 1].begin());
            std::fill_n(dftB[lv][j - 1].begin() + blocksize, blocksize, Tp());
            fft(dftB[lv][j - 1]);

            // middle product
            std::vector<Tp> mp(blocksize * 2);
            for (int k = 0; k < j; ++k)
                for (int l = 0; l < blocksize * 2; ++l)
                    mp[l] += dftA[lv][j - 1 - k][l] * dftB[lv][k][l];
            inv_fft(mp);

            for (int k = 0; k < blocksize && i + k < n; ++k) AB[i + k] += mp[k + blocksize];
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
inline std::vector<Tp> inv(const std::vector<Tp> &a, int n) {
    assert(!a.empty());
    if (n <= 0) return {};
    return semi_relaxed_convolution(
        a, [v = a[0].inv()](int n, auto &&c) { return n == 0 ? v : -c[n] * v; }, n);
}

template <typename Tp>
inline std::vector<Tp> div(const std::vector<Tp> &a, const std::vector<Tp> &b, int n) {
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
inline std::vector<Tp> log(const std::vector<Tp> &a, int n) {
    return integr(div(deriv(a), a, n - 1));
}

template <typename Tp>
inline std::vector<Tp> exp(const std::vector<Tp> &a, int n) {
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
inline std::vector<Tp> pow(std::vector<Tp> a, long long e, int n) {
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
    a = log(a, n - o * e);
    for (int i = 0; i < (int)a.size(); ++i) a[i] *= me;
    a = exp(a, n - o * e);
    for (int i = 0; i < (int)a.size(); ++i) a[i] *= a0e;

    a.insert(a.begin(), o * e, Tp());
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
    int n      = a.size();
    auto &&bin = Binomial<Tp>::get(n);
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
template <typename Tp>
inline std::pair<std::vector<Tp>, std::vector<Tp>> euclidean_div(const std::vector<Tp> &A,
                                                                 const std::vector<Tp> &B) {
    // returns a mod (x^n-1)
    auto make_cyclic = [](const std::vector<Tp> &a, int n) {
        assert((n & (n - 1)) == 0);
        std::vector<Tp> b(n);
        for (int i = 0; i < (int)a.size(); ++i) b[i & (n - 1)] += a[i];
        return b;
    };

    const int degA = degree(A);
    const int degB = degree(B);
    assert(degB >= 0);
    // A = Q*B + R => A/B = Q + R/B in R((x^(-1)))
    const int degQ = degA - degB;
    if (degQ < 0) return {std::vector<Tp>{Tp(0)}, A};

    auto Q = div(std::vector(A.rend() - (degA + 1), A.rend()),
                 std::vector(B.rend() - (degB + 1), B.rend()), degQ + 1);
    std::reverse(Q.begin(), Q.end());

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

    auto Q = div(std::vector(A.rend() - (degA + 1), A.rend()),
                 std::vector(B.rend() - (degB + 1), B.rend()), degQ + 1);
    std::reverse(Q.begin(), Q.end());
    return Q;
}
#line 9 "subproduct_tree.hpp"

template <typename Tp>
class SubproductTree {
public:
    // LV=0   => T[0..S]  = DFT((x-X_0)..(x-X_(N-1))     mod (x^S     - 1))
    //        => T[S..2S] = (x-X_0)..(x-X_(N-1))         mod (x^S     + 1)  (* SPECIAL CASE)
    // LV=1   => T[..]    = DFT((x-X_0)..(x-X_(S/2-1))   mod (x^(S/2) - 1))
    //        => T[..]    = DFT((x-X_(S/2))..(x-X_(N-1)) mod (x^(S/2) - 1)) (* GENERAL CASE)
    // LV=2.. => ..                                                         (* GENERAL CASE)
    std::vector<Tp> T;
    int N;
    int S;

    SubproductTree(const std::vector<Tp> &X)
        : N(X.size()), S(N == 0 ? 2 : std::max(fft_len(N), 2)) {
        int LogS = 1;
        while ((1 << LogS) < S) ++LogS;
        T.assign((LogS + 1) * S * 2, 1);
        for (int i = 0; i < N; ++i) {
            T[LogS * S * 2 + i * 2]     = 1 - X[i];
            T[LogS * S * 2 + i * 2 + 1] = -1 - X[i];
        }
        for (int lv = LogS - 1, len = 2; lv >= 0; --lv, len *= 2) {
            for (int i = 0; i < (1 << lv); ++i) {
                auto C = T.begin() + (lv * S * 2 + i * len * 2);       // current
                auto L = T.begin() + ((lv + 1) * S * 2 + i * len * 2); // left child
                for (int j = 0; j < len; ++j) C[j] = C[len + j] = L[j] * L[len + j];
                inv_fft_n(C + len, len);
                if ((i + 1) * len <= N) C[len] -= 2;
                if (lv) {
                    Tp k         = 1;
                    const auto t = FftInfo<Tp>::get().root(len).at(len / 2);
                    for (int j = 0; j < len; ++j) C[len + j] *= k, k *= t;
                    fft_n(C + len, len);
                }
            }
        }
    }

    std::vector<Tp> product() const {
        std::vector res(T.begin() + S, T.begin() + S * 2);
        if (N == S) {
            res[0] += 1;
            res.emplace_back(1);
        }
        res.resize(N + 1);
        return res;
    }

    // see:
    // [1]: A. Bostan, Grégoire Lecerf, É. Schost. Tellegen's principle into practice.
    // [2]: D. Bernstein. SCALED REMAINDER TREES.
    std::vector<Tp> evaluation(const std::vector<Tp> &F) const {
        const int degF = degree(F);
        const auto P   = product();
        // find x^(-1),...,x^(-N) of F/P in R((x^(-1)))
        auto res = div(std::vector(F.rend() - (degF + 1), F.rend()),
                       std::vector(P.rbegin(), P.rend()), degF + 1);
        if (degF >= N) res.erase(res.begin(), res.begin() + (degF - N + 1));
        std::reverse(res.begin(), res.end());
        res.resize(S);
        for (int lv = 0, len = S; (1 << lv) < S; ++lv, len /= 2) {
            std::vector<Tp> LL(len), RR(len);
            for (int i = 0; i < (1 << lv); ++i) {
                auto C = res.begin() + i * len;                        // current
                auto L = T.begin() + ((lv + 1) * S * 2 + i * len * 2); // left child
                fft_n(C, len);
                for (int j = 0; j < len; ++j) {
                    LL[j] = C[j] * L[len + j];
                    RR[j] = C[j] * L[j];
                }
                inv_fft(LL);
                inv_fft(RR);
                const int degL = std::max(std::min((i * len) + len / 2, N) - i * len, 0);
                const int degR = std::max(std::min((i + 1) * len, N) - ((i * len) + len / 2), 0);
                std::copy_n(LL.begin() + degR, len / 2, C);
                std::copy_n(RR.begin() + degL, len / 2, C + len / 2);
            }
        }
        res.resize(N);
        return res;
    }

    std::vector<Tp> interpolation(const std::vector<Tp> &Y) const {
        assert((int)Y.size() == N);
        const auto D = evaluation(deriv(product())); // denominator => P'(x_i)
        std::vector<Tp> res(S * 2);
        for (int i = 0; i < N; ++i) res[i * 2] = res[i * 2 + 1] = Y[i] / D[i];
        int LogS = 1;
        while ((1 << LogS) < S) ++LogS;
        for (int lv = LogS - 1, len = 2; lv >= 0; --lv, len *= 2) {
            for (int i = 0; i < (1 << lv); ++i) {
                auto C = res.begin() + i * len * 2;                    // current
                auto L = T.begin() + ((lv + 1) * S * 2 + i * len * 2); // left child
                for (int j = 0; j < len; ++j)
                    C[j] = C[len + j] = C[j] * L[len + j] + C[len + j] * L[j];
                inv_fft_n(C + len, len);
                if (lv) {
                    Tp k         = 1;
                    const auto t = FftInfo<Tp>::get().root(len).at(len / 2);
                    for (int j = 0; j < len; ++j) C[len + j] *= k, k *= t;
                    fft_n(C + len, len);
                }
            }
        }
        return std::vector(res.begin() + S, res.begin() + S + N);
    }

    // see:
    // [1]: A. Bostan, É. Schost. Polynomial evaluation and interpolation on special sets of points.
    // [2]: noshi91. 転置原理なしで Monomial 基底から Newton 基底への変換.
    //      https://noshi91.hatenablog.com/entry/2023/05/01/022946
    std::vector<Tp> monomial_to_newton(const std::vector<Tp> &F) const {
        const int degF = degree(F);
        assert(degF < N);
        const auto P = product();
        // find x^(-1),...,x^(-N) of F/P in R((x^(-1)))
        auto res = div(std::vector(F.rend() - (degF + 1), F.rend()),
                       std::vector(P.rbegin(), P.rend()), degF + 1);
        std::reverse(res.begin(), res.end());
        res.resize(S);
        for (int lv = 0, len = S; (1 << lv) < S; ++lv, len /= 2) {
            std::vector<Tp> RR(len / 2);
            for (int i = 0; i < (1 << lv); ++i) {
                auto C = res.begin() + i * len;                              // current
                auto R = T.begin() + ((lv + 1) * S * 2 + (i * 2 + 1) * len); // right child
                std::copy_n(C + len / 2, len / 2, RR.begin());
                fft_n(C, len);
                for (int j = 0; j < len; ++j) C[j] *= R[j];
                inv_fft_n(C, len);
                const int degR = std::max(std::min((i + 1) * len, N) - ((i * len) + len / 2), 0);
                std::rotate(C, C + degR, C + (degR + len / 2));
                std::copy_n(RR.begin(), len / 2, C + len / 2);
            }
        }
        res.resize(N);
        return res;
    }

    std::vector<Tp> newton_to_monomial(const std::vector<Tp> &F) const {
        const int degF = degree(F);
        assert(degF < N);
        std::vector<Tp> res(S * 2);
        for (int i = 0; i <= degF; ++i) res[i * 2] = res[i * 2 + 1] = F[i];
        int LogS = 1;
        while ((1 << LogS) < S) ++LogS;
        for (int lv = LogS - 1, len = 2; lv >= 0; --lv, len *= 2) {
            for (int i = 0; i < (1 << lv); ++i) {
                auto C = res.begin() + i * len * 2;                    // current
                auto L = T.begin() + ((lv + 1) * S * 2 + i * len * 2); // left child
                for (int j = 0; j < len; ++j) C[j] = C[len + j] = C[j] + C[len + j] * L[j];
                inv_fft_n(C + len, len);
                if (lv) {
                    Tp k         = 1;
                    const auto t = FftInfo<Tp>::get().root(len).at(len / 2);
                    for (int j = 0; j < len; ++j) C[len + j] *= k, k *= t;
                    fft_n(C + len, len);
                }
            }
        }
        return std::vector(res.begin() + S, res.begin() + S + N);
    }
};
#line 7 "test/conversion_from_monomial_basis_to_newton_basis.0.test.cpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    using mint = ModInt<998244353>;
    int n;
    std::cin >> n;
    std::vector<mint> F(n), X(n);
    for (int i = 0; i < n; ++i) std::cin >> F[i];
    for (int i = 0; i < n; ++i) std::cin >> X[i];
    SubproductTree<mint> T(X);
    const auto res = T.monomial_to_newton(F);
    for (int i = 0; i < n; ++i) std::cout << res[i] << ' ';
    return 0;
}