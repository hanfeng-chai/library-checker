#line 1 "test/formal_power_series/multivariate_power_series.0.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#line 2 "modint.hpp"

#include <iostream>
#include <type_traits>

// clang-format off
template<unsigned Mod> class ModInt {
    static_assert((Mod >> 31) == 0, "`Mod` must less than 2^(31)");
    template<typename Int>
    static std::enable_if_t<std::is_integral_v<Int>, unsigned> safe_mod(Int v) { using D = std::common_type_t<Int, unsigned>; return (v %= (int)Mod) < 0 ? (D)(v + (int)Mod) : (D)v; }
    struct PrivateConstructor {} static inline private_constructor{};
    ModInt(PrivateConstructor, unsigned v) : v_(v) {}
    unsigned v_;

public:
    static unsigned mod() { return Mod; }
    static ModInt from_raw(unsigned v) { return ModInt(private_constructor, v); }
    static ModInt zero() { return from_raw(0); }
    static ModInt one() { return from_raw(1); }
    bool is_zero() const { return v_ == 0; }
    bool is_one() const { return v_ == 1; }
    ModInt() : v_() {}
    template<typename Int, typename std::enable_if_t<std::is_signed_v<Int>, int> = 0> ModInt(Int v) : v_(safe_mod(v)) {}
    template<typename Int, typename std::enable_if_t<std::is_unsigned_v<Int>, int> = 0> ModInt(Int v) : v_(v % Mod) {}
    unsigned val() const { return v_; }
    ModInt operator-() const { return from_raw(v_ == 0 ? v_ : Mod - v_); }
    ModInt pow(long long e) const { if (e < 0) return inv().pow(-e); for (ModInt x(*this), res(from_raw(1));; x *= x) { if (e & 1) res *= x; if ((e >>= 1) == 0) return res; }}
    ModInt inv() const { int x1 = 1, x3 = 0, a = val(), b = Mod; while (b) { const int q = a / b, x1_old = x1, a_old = a; x1 = x3, x3 = x1_old - x3 * q, a = b, b = a_old - b * q; } return from_raw(x1 < 0 ? x1 + (int)Mod : x1); }
    template<bool Odd = (Mod & 1)> std::enable_if_t<Odd, ModInt> div_by_2() const { if (v_ & 1) return from_raw((v_ + Mod) >> 1); return from_raw(v_ >> 1); }
    ModInt &operator+=(const ModInt &a) { if ((v_ += a.v_) >= Mod) v_ -= Mod; return *this; }
    ModInt &operator-=(const ModInt &a) { if ((v_ += Mod - a.v_) >= Mod) v_ -= Mod; return *this; }
    ModInt &operator*=(const ModInt &a) { v_ = (unsigned long long)v_ * a.v_ % Mod; return *this; }
    ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }
    ModInt &operator++() { return *this += one(); }
    ModInt operator++(int) { ModInt o(*this); *this += one(); return o; }
    ModInt &operator--() { return *this -= one(); }
    ModInt operator--(int) { ModInt o(*this); *this -= one(); return o; }
    friend ModInt operator+(const ModInt &a, const ModInt &b) { return ModInt(a) += b; }
    friend ModInt operator-(const ModInt &a, const ModInt &b) { return ModInt(a) -= b; }
    friend ModInt operator*(const ModInt &a, const ModInt &b) { return ModInt(a) *= b; }
    friend ModInt operator/(const ModInt &a, const ModInt &b) { return ModInt(a) /= b; }
    friend bool operator==(const ModInt &a, const ModInt &b) { return a.v_ == b.v_; }
    friend bool operator!=(const ModInt &a, const ModInt &b) { return a.v_ != b.v_; }
    friend std::istream &operator>>(std::istream &a, ModInt &b) { int v; a >> v; b.v_ = safe_mod(v); return a; }
    friend std::ostream &operator<<(std::ostream &a, const ModInt &b) { return a << b.val(); }
};
// clang-format on
#line 2 "mps_basic.hpp"

#line 2 "binomial.hpp"

#include <algorithm>
#include <vector>

template<typename Tp> class Binomial {
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
#line 2 "fft.hpp"

#line 4 "fft.hpp"
#include <cassert>
#include <iterator>
#include <memory>
#line 8 "fft.hpp"

template<typename Tp> class FftInfo {
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

template<typename Iterator> inline void
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

template<typename Iterator> inline void
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
template<typename Iterator> inline void fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    detail::butterfly_n(a, n, FftInfo<Tp>::get().root(n / 2));
}

template<typename Tp> inline void fft(std::vector<Tp> &a) { fft_n(a.begin(), a.size()); }

// IFFT_n: bit-reversed order of [A(1), A(zeta_n), ..., A(zeta_n^(n-1))] |-> A(x)
template<typename Iterator> inline void inv_fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    detail::inv_butterfly_n(a, n, FftInfo<Tp>::get().inv_root(n / 2));
    const Tp iv = Tp::mod() - (Tp::mod() - 1) / n;
    for (int i = 0; i < n; ++i) a[i] *= iv;
}

template<typename Tp> inline void inv_fft(std::vector<Tp> &a) { inv_fft_n(a.begin(), a.size()); }

// IFFT_n^T: A(x) |-> 1/n FFT_n((x^n A(x^(-1))) mod (x^n - 1))
template<typename Iterator> inline void transposed_inv_fft_n(Iterator a, int n) {
    using Tp    = typename std::iterator_traits<Iterator>::value_type;
    const Tp iv = Tp::mod() - (Tp::mod() - 1) / n;
    for (int i = 0; i < n; ++i) a[i] *= iv;
    detail::butterfly_n(a, n, FftInfo<Tp>::get().inv_root(n / 2));
}

template<typename Tp> inline void transposed_inv_fft(std::vector<Tp> &a) {
    transposed_inv_fft_n(a.begin(), a.size());
}

// FFT_n^T : FFT_n((x^n A(x^(-1))) mod (x^n - 1)) |-> n A(x)
template<typename Iterator> inline void transposed_fft_n(Iterator a, int n) {
    using Tp = typename std::iterator_traits<Iterator>::value_type;
    detail::inv_butterfly_n(a, n, FftInfo<Tp>::get().root(n / 2));
}

template<typename Tp> inline void transposed_fft(std::vector<Tp> &a) {
    transposed_fft_n(a.begin(), a.size());
}

template<typename Tp> inline std::vector<Tp> convolution_fft(std::vector<Tp> a, std::vector<Tp> b) {
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

template<typename Tp> inline std::vector<Tp> square_fft(std::vector<Tp> a) {
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

template<typename Tp>
inline std::vector<Tp> convolution_naive(const std::vector<Tp> &a, const std::vector<Tp> &b) {
    if (a.empty() || b.empty()) return {};
    const int n = a.size();
    const int m = b.size();
    std::vector<Tp> res(n + m - 1);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) res[i + j] += a[i] * b[j];
    return res;
}

template<typename Tp>
inline std::vector<Tp> convolution(const std::vector<Tp> &a, const std::vector<Tp> &b) {
    if (std::min(a.size(), b.size()) < 60) return convolution_naive(a, b);
    if (std::addressof(a) == std::addressof(b)) return square_fft(a);
    return convolution_fft(a, b);
}
#line 2 "md_conv.hpp"

#line 5 "md_conv.hpp"
#include <functional>
#include <numeric>
#line 8 "md_conv.hpp"

class MDConvInfo {
    int len_;
    std::vector<int> degree_bound_;

public:
    explicit MDConvInfo(const std::vector<int> &d)
        : len_(std::accumulate(d.begin(), d.end(), 1, std::multiplies<>())), degree_bound_(d) {}

    int len() const { return len_; }
    int dim() const { return degree_bound_.size(); }
    std::vector<int> degree_bound() const { return degree_bound_; }

    // see:
    // [1]: Elegia. Hello, multivariate multiplication.
    //      https://www.luogu.com/article/wje8kchr
    std::vector<int> chi() const {
        auto pp = degree_bound_;
        for (int i = 1; i < (int)pp.size(); ++i) pp[i] *= pp[i - 1];
        std::vector<int> diff(pp.size());
        // O(max(dim^2, len))
        for (int i = 1; i < (int)diff.size(); ++i) {
            for (int j = 0; j < i; ++j) diff[i] += pp[i - 1] / pp[j];
            diff[i] %= dim();
        }
        std::vector<int> c(len());
        for (int i = 1; i < (int)pp.size(); ++i)
            for (int j = pp[i - 1]; j < pp[i]; ++j)
                if ((c[j] = c[j - pp[i - 1]] + diff[i]) >= dim()) c[j] -= dim();
        return c;
    }
};

namespace detail {

template<typename Tp>
inline std::vector<std::vector<Tp>> multidimensional_hadamard(const std::vector<std::vector<Tp>> &a,
                                                              const std::vector<std::vector<Tp>> &b,
                                                              int dim, int len) {
    std::vector c(dim, std::vector<Tp>(len));
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j) {
            const int k = (i + j) % dim;
            for (int l = 0; l < len; ++l) c[k][l] += a[i][l] * b[j][l];
        }
    return c;
}

} // namespace detail

template<typename Tp>
inline std::vector<Tp> multidimensional_convolution(const MDConvInfo &info,
                                                    const std::vector<Tp> &a,
                                                    const std::vector<Tp> &b) {
    assert((int)a.size() == info.len());
    assert((int)b.size() == info.len());
    if (info.dim() == 0) return {a[0] * b[0]};
    const int len = fft_len(info.len() * 2 - 1);
    std::vector aa(info.dim(), std::vector<Tp>(len));
    std::vector bb(info.dim(), std::vector<Tp>(len));
    const auto chi = info.chi();
    for (int i = 0; i < info.len(); ++i) aa[chi[i]][i] = a[i], bb[chi[i]][i] = b[i];
    for (int i = 0; i < info.dim(); ++i) fft(aa[i]), fft(bb[i]);
    auto cc = detail::multidimensional_hadamard(aa, bb, info.dim(), len);
    for (int i = 0; i < info.dim(); ++i) inv_fft(cc[i]);
    std::vector<Tp> c(info.len());
    for (int i = 0; i < info.len(); ++i) c[i] = cc[chi[i]][i];
    return c;
}
#line 8 "mps_basic.hpp"
#include <sstream>
#include <string>
#line 11 "mps_basic.hpp"

// Multivariate Power Series [inv, exp, log, pow]
// Store MPS A(x0, x1, ..., x(d-1)) with A(x, x1^N0, ...) in a 1-dim array
// using Kronecker substitution
// TODO: opt

template<typename Tp>
inline std::string to_string(const MDConvInfo &info, const std::vector<Tp> &a) {
    assert((int)a.size() == info.len());
    std::stringstream ss;
    ss << '[';
    const auto degree_bound = info.degree_bound();
    std::vector<int> deg(info.dim());
    for (int i = 0; i < (int)a.size(); ++i) {
        if (i) ss << " + ";
        ss << a[i];
        for (int j = 0; j < (int)deg.size(); ++j) ss << "*x" << j << "^(" << deg[j] << ')';
        for (int j = 0; j < (int)deg.size(); ++j) {
            if (++deg[j] < degree_bound[j]) break;
            deg[j] = 0;
        }
    }
    ss << ']';
    return ss.str();
}

template<typename Tp>
inline std::vector<Tp> mps_inv(const MDConvInfo &info, const std::vector<Tp> &a) {
    assert((int)a.size() == info.len());
    assert(a[0] != 0);
    const auto bound = info.degree_bound();
    std::vector<Tp> res(info.len());
    res[0] = a[0].inv();
    std::vector<int> d(info.dim());
    for (int i = 0, pp = 1; i < (int)bound.size(); pp *= bound[i++]) {
        for (d[i] = 1; d[i] < bound[i]; d[i] = std::min(d[i] * 2, bound[i])) {
            auto nextd     = std::vector(d.begin(), d.begin() + (i + 1));
            nextd[i]       = std::min(d[i] * 2, bound[i]);
            const int len  = fft_len(pp * nextd[i]);
            const auto chi = MDConvInfo(nextd).chi();
            std::vector shopA(i + 1, std::vector<Tp>(len));
            std::vector shopB(i + 1, std::vector<Tp>(len));
            for (int j = 0; j < pp * nextd[i]; ++j) shopA[chi[j]][j] = a[j];
            for (int j = 0; j < pp * d[i]; ++j) shopB[chi[j]][j] = res[j];
            for (int j = 0; j <= i; ++j) fft(shopA[j]), fft(shopB[j]);
            shopA = detail::multidimensional_hadamard(shopA, shopB, i + 1, len);
            for (int j = 0; j <= i; ++j) inv_fft(shopA[j]);
            {
                std::vector<Tp> shopC(pp * (nextd[i] - d[i]));
                for (int j = pp * d[i]; j < pp * nextd[i]; ++j)
                    shopC[j - pp * d[i]] = shopA[chi[j]][j];
                for (int j = 0; j <= i; ++j) std::fill(shopA[j].begin(), shopA[j].end(), Tp(0));
                for (int j = pp * d[i]; j < pp * nextd[i]; ++j)
                    shopA[chi[j]][j] = shopC[j - pp * d[i]];
            }
            for (int j = 0; j <= i; ++j) fft(shopA[j]);
            shopA = detail::multidimensional_hadamard(shopA, shopB, i + 1, len);
            for (int j = 0; j <= i; ++j) inv_fft(shopA[j]);
            for (int j = pp * d[i]; j < pp * nextd[i]; ++j) res[j] = -shopA[chi[j]][j];
        }
    }
    return res;
}

// see:
// [1]: Elegia. Hello, multivariate multiplication.
//      https://www.luogu.com/article/wje8kchr
template<typename Tp> inline std::vector<Tp> mps_deriv(std::vector<Tp> a) {
    for (int i = 0; i < (int)a.size(); ++i) a[i] *= i;
    return a;
}

template<typename Tp> inline std::vector<Tp> mps_integr(std::vector<Tp> a, Tp c = {}) {
    auto &&bin = Binomial<Tp>::get(a.size());
    a[0]       = c;
    for (int i = 1; i < (int)a.size(); ++i) a[i] *= bin.inv(i);
    return a;
}

template<typename Tp>
inline std::vector<Tp> mps_log(const MDConvInfo &info, const std::vector<Tp> &a) {
    assert((int)a.size() == info.len());
    assert(a[0] == 1);
    return mps_integr(multidimensional_convolution(info, mps_deriv(a), mps_inv(info, a)));
}

template<typename Tp>
inline std::vector<Tp> mps_exp(const MDConvInfo &info, const std::vector<Tp> &a) {
    assert((int)a.size() == info.len());
    assert(a[0] == 0);
    const auto bound = info.degree_bound();
    std::vector<Tp> res(info.len());
    res[0] = 1;
    std::vector<int> d(info.dim());
    for (int i = 0, pp = 1; i < (int)bound.size(); pp *= bound[i++]) {
        for (d[i] = 1; d[i] < bound[i]; d[i] = std::min(d[i] * 2, bound[i])) {
            auto nextd = std::vector(d.begin(), d.begin() + (i + 1));
            nextd[i]   = std::min(d[i] * 2, bound[i]);
            const MDConvInfo ainfo(nextd);
            auto shopA = mps_log(ainfo, std::vector(res.begin(), res.begin() + pp * nextd[i]));
            std::fill_n(shopA.begin(), pp * d[i], Tp(0));
            for (int j = pp * d[i]; j < pp * nextd[i]; ++j) shopA[j] -= a[j];
            shopA = multidimensional_convolution(
                ainfo, std::vector(res.begin(), res.begin() + pp * nextd[i]), shopA);
            for (int j = pp * d[i]; j < pp * nextd[i]; ++j) res[j] = -shopA[j];
        }
    }
    return res;
}

template<typename Tp>
inline std::vector<Tp> mps_pow(const MDConvInfo &info, std::vector<Tp> a, long long e) {
    assert((int)a.size() == info.len());
    if (e == 0) {
        std::vector<Tp> res(info.len());
        res[0] = 1;
        return res;
    }

    if (a[0] != 0) {
        const Tp ia0 = a[0].inv();
        const Tp a0e = a[0].pow(e);
        const Tp me  = e;

        for (int i = 0; i < (int)a.size(); ++i) a[i] *= ia0;
        a = mps_log(info, a);
        for (int i = 0; i < (int)a.size(); ++i) a[i] *= me;
        a = mps_exp(info, a);
        for (int i = 0; i < (int)a.size(); ++i) a[i] *= a0e;

        return a;
    }

    assert(e > 0);
    std::vector<Tp> res(info.len());
    res[0] = 1;
    for (;;) {
        if (e & 1) res = multidimensional_convolution(info, res, a);
        if ((e >>= 1) == 0) return res;
        a = multidimensional_convolution(info, a, a);
    }
}
#line 2 "random.hpp"

#line 2 "rng.hpp"

#include <cstdint>
#include <limits>

// see: https://prng.di.unimi.it/xoshiro256starstar.c
// original license CC0 1.0
class xoshiro256starstar {
    using u64 = std::uint64_t;

    static inline u64 rotl(const u64 x, int k) { return (x << k) | (x >> (64 - k)); }

    u64 s_[4];

    u64 next() {
        const u64 res = rotl(s_[1] * 5, 7) * 9;
        const u64 t   = s_[1] << 17;
        s_[2] ^= s_[0];
        s_[3] ^= s_[1];
        s_[1] ^= s_[2];
        s_[0] ^= s_[3];
        s_[2] ^= t;
        s_[3] = rotl(s_[3], 45);
        return res;
    }

public:
    // see: https://prng.di.unimi.it/splitmix64.c
    // original license CC0 1.0
    explicit xoshiro256starstar(u64 seed) {
        for (int i = 0; i < 4; ++i) {
            u64 z = (seed += 0x9e3779b97f4a7c15);
            z     = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
            z     = (z ^ (z >> 27)) * 0x94d049bb133111eb;
            s_[i] = z ^ (z >> 31);
        }
    }
    // see: https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator
    using result_type = u64;
    static constexpr u64 min() { return std::numeric_limits<u64>::min(); }
    static constexpr u64 max() { return std::numeric_limits<u64>::max(); }
    u64 operator()() { return next(); }
};
#line 4 "random.hpp"
#include <random>
#line 6 "random.hpp"

template<typename Tp> inline std::vector<Tp> random_vector(int n) {
    std::vector<Tp> res(n);
    xoshiro256starstar rng(std::random_device{}());
    std::uniform_int_distribution<decltype(Tp::mod())> dis(0, Tp::mod() - 1);
    for (int i = 0; i < n; ++i) res[i] = dis(rng);
    return res;
}

template<typename Tp> inline std::vector<Tp> random_vector_without_zero(int n) {
    std::vector<Tp> res(n);
    xoshiro256starstar rng(std::random_device{}());
    std::uniform_int_distribution<decltype(Tp::mod())> dis(1, Tp::mod() - 1);
    for (int i = 0; i < n; ++i) res[i] = dis(rng);
    return res;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using mint = ModInt<998244353>;

    int n, m;
    std::cin >> n >> m;

    std::vector<mint> a(n * m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) { std::cin >> a[i * m + j]; }
    }

    MDConvInfo info({m, n});
    const auto invA = mps_inv(info, a);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (j) std::cout << " ";
            std::cout << invA[i * m + j];
        }
        std::cout << "\n";
    }

    return 0;
}
