#define PROBLEM "https://judge.yosupo.jp/problem/composition_of_formal_power_series_large"
// ============
// ============
#include <cassert>
#include <vector>

template <typename M>
M inv(int n) {
    static std::vector<M> data{M::raw(0), M::raw(1)};
    static constexpr unsigned MOD = M::get_mod();
    assert(0 < n);
    while ((int)data.size() <= n) {
        unsigned k = (unsigned)data.size();
        unsigned r = MOD / k + 1;
        data.push_back(M::raw(r) * data[k * r - MOD]);
    }
    return data[n];
}

template <typename M>
M fact(int n) {
    static std::vector<M> data{M::raw(1), M::raw(1)};
    assert(0 <= n);
    while ((int)data.size() <= n) {
        unsigned k = (unsigned)data.size();
        data.push_back(M::raw(k) * data.back());
    }
    return data[n];
}

template <typename M>
M inv_fact(int n) {
    static std::vector<M> data{M::raw(1), M::raw(1)};
    assert(0 <= n);
    while ((int)data.size() <= n) {
        unsigned k = (unsigned)data.size();
        data.push_back(inv<M>(k) * data.back());
    }
    return data[n];
}

template <typename M>
M binom(int n, int k) {
    assert(0 <= n);
    if (k < 0 || n < k) {
        return M::raw(0);
    }
    return fact<M>(n) * inv_fact<M>(k) * inv_fact<M>(n - k);
}

template <typename M>
M n_terms_sum_k(int n, int k) {
    assert(0 <= n && 0 <= k);
    if (n == 0) {
        return (k == 0 ? M::raw(1) : M::raw(0));
    }
    return binom<M>(n + k - 1, n - 1);
}
// ============
// ============
#include <array>
#include <vector>
// ============

#include <cassert>
#include <iostream>
#include <type_traits>
// ============

#include <utility>

constexpr bool is_prime(unsigned n) {
    if (n == 0 || n == 1) {
        return false;
    }
    for (unsigned i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

constexpr unsigned mod_pow(unsigned x, unsigned y, unsigned mod) {
    unsigned ret = 1, self = x;
    while (y != 0) {
        if (y & 1) {
            ret = (unsigned)((unsigned long long)ret * self % mod);
        }
        self = (unsigned)((unsigned long long)self * self % mod);
        y /= 2;
    }
    return ret;
}

template <unsigned mod>
constexpr unsigned primitive_root() {
    static_assert(is_prime(mod), "`mod` must be a prime number.");
    if (mod == 2) {
        return 1;
    }

    unsigned primes[32] = {};
    int it = 0;
    {
        unsigned m = mod - 1;
        for (unsigned i = 2; i * i <= m; ++i) {
            if (m % i == 0) {
                primes[it++] = i;
                while (m % i == 0) {
                    m /= i;
                }
            }
        }
        if (m != 1) {
            primes[it++] = m;
        }
    }
    for (unsigned i = 2; i < mod; ++i) {
        bool ok = true;
        for (int j = 0; j < it; ++j) {
            if (mod_pow(i, (mod - 1) / primes[j], mod) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return i;
    }
    return 0;
}

// y >= 1
template <typename T>
constexpr T safe_mod(T x, T y) {
    x %= y;
    if (x < 0) {
        x += y;
    }
    return x;
}

// y != 0
template <typename T>
constexpr T floor_div(T x, T y) {
    if (y < 0) {
        x *= -1;
        y *= -1;
    }
    if (x >= 0) {
        return x / y;
    } else {
        return -((-x + y - 1) / y);
    }
}

// y != 0
template <typename T>
constexpr T ceil_div(T x, T y) {
    if (y < 0) {
        x *= -1;
        y *= -1;
    }
    if (x >= 0) {
        return (x + y - 1) / y;
    } else {
        return -(-x / y);
    }
}

// b >= 1
// returns (g, x) s.t. g = gcd(a, b), a * x = g (mod b), 0 <= x < b / g
// from ACL
template <typename T>
std::pair<T, T> extgcd(T a, T b) {
    a = safe_mod(a, b);
    T s = b, t = a, m0 = 0, m1 = 1;
    while (t) {
        T u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        std::swap(s, t);
        std::swap(m0, m1);
    }
    if (m0 < 0) {
        m0 += b / s;
    }
    return std::pair<T, T>(s, m0);
}

// b >= 1
// returns (g, x, y) s.t. g = gcd(a, b), a * x + b * y = g, 0 <= x < b / g, |y| < max(2, |a| / g)
template <typename T>
std::tuple<T, T, T> extgcd2(T a, T b) {
    T _a = safe_mod(a, b);
    T quot = (a - _a) / b;
    T x00 = 0, x01 = 1, y0 = b;
    T x10 = 1, x11 = -quot, y1 = _a;
    while (y1) {
        T u = y0 / y1;
        x00 -= u * x10;
        x01 -= u * x11;
        y0 -= u * y1;
        std::swap(x00, x10);
        std::swap(x01, x11);
        std::swap(y0, y1);
    }
    if (x00 < 0) {
        x00 += b / y0;
        x01 -= a / y0;
    }
    return std::tuple<T, T, T>(y0, x00, x01);
}

// gcd(x, m) == 1
template <typename T>
T inv_mod(T x, T m) {
    return extgcd(x, m).second;
}
// ============

template <unsigned mod>
struct ModInt {
    static_assert(mod != 0, "`mod` must not be equal to 0.");
    static_assert(mod < (1u << 31),
                  "`mod` must be less than (1u << 31) = 2147483648.");

    unsigned val;

    static constexpr unsigned get_mod() { return mod; }

    constexpr ModInt() : val(0) {}
    template <typename T, std::enable_if_t<std::is_signed_v<T>> * = nullptr>
    constexpr ModInt(T x)
        : val((unsigned)((long long)x % (long long)mod + (x < 0 ? mod : 0))) {}
    template <typename T, std::enable_if_t<std::is_unsigned_v<T>> * = nullptr>
    constexpr ModInt(T x) : val((unsigned)(x % mod)) {}

    static constexpr ModInt raw(unsigned x) {
        ModInt<mod> ret;
        ret.val = x;
        return ret;
    }

    constexpr unsigned get_val() const { return val; }

    constexpr ModInt operator+() const { return *this; }
    constexpr ModInt operator-() const { return ModInt<mod>(0u) - *this; }

    constexpr ModInt &operator+=(const ModInt &rhs) {
        val += rhs.val;
        if (val >= mod) val -= mod;
        return *this;
    }
    constexpr ModInt &operator-=(const ModInt &rhs) {
        val -= rhs.val;
        if (val >= mod) val += mod;
        return *this;
    }
    constexpr ModInt &operator*=(const ModInt &rhs) {
        val = (unsigned long long)val * rhs.val % mod;
        return *this;
    }
    constexpr ModInt &operator/=(const ModInt &rhs) {
        val = (unsigned long long)val * rhs.inv().val % mod;
        return *this;
    }

    friend constexpr ModInt operator+(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) += rhs;
    }
    friend constexpr ModInt operator-(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) -= rhs;
    }
    friend constexpr ModInt operator*(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) *= rhs;
    }
    friend constexpr ModInt operator/(const ModInt &lhs, const ModInt &rhs) {
        return ModInt<mod>(lhs) /= rhs;
    }

    constexpr ModInt pow(unsigned long long x) const {
        ModInt<mod> ret = ModInt<mod>::raw(1);
        ModInt<mod> self = *this;
        while (x != 0) {
            if (x & 1) ret *= self;
            self *= self;
            x >>= 1;
        }
        return ret;
    }
    constexpr ModInt inv() const {
        static_assert(is_prime(mod), "`mod` must be a prime number.");
        assert(val != 0);
        return this->pow(mod - 2);
    }

    friend std::istream &operator>>(std::istream &is, ModInt<mod> &x) {
        long long val;
        is >> val;
        x.val = val % mod + (val < 0 ? mod : 0);
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const ModInt<mod> &x) {
        os << x.val;
        return os;
    }

    friend bool operator==(const ModInt &lhs, const ModInt &rhs) {
        return lhs.val == rhs.val;
    }

    friend bool operator!=(const ModInt &lhs, const ModInt &rhs) {
        return lhs.val != rhs.val;
    }
};

template <unsigned mod>
void debug(ModInt<mod> x) {
    std::cerr << x.val;
}
// ============

constexpr int ctz_constexpr(unsigned n) {
    int x = 0;
    while (!(n & (1u << x))) {
        ++x;
    }
    return x;
}

template <unsigned MOD>
struct FFTRoot {
    static constexpr unsigned R = ctz_constexpr(MOD - 1);
    std::array<ModInt<MOD>, R + 1> root, iroot;
    std::array<ModInt<MOD>, R> rate2, irate2;
    std::array<ModInt<MOD>, R - 1> rate3, irate3;
    std::array<ModInt<MOD>, R + 1> inv2;

    constexpr FFTRoot() : root{}, iroot{}, rate2{}, irate2{}, rate3{}, irate3{}, inv2{} {
        unsigned pr = primitive_root<MOD>();
        root[R] = ModInt<MOD>(pr).pow(MOD >> R);
        iroot[R] = root[R].inv();
        for (int i = R - 1; i >= 0; --i) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }
        ModInt<MOD> prod(1), iprod(1);
        for (int i = 0; i < (int)R - 1; ++i) {
            rate2[i] = prod * root[i + 2];
            irate2[i] = iprod * iroot[i + 2];
            prod *= iroot[i + 2];
            iprod *= root[i + 2];
        }
        prod = ModInt<MOD>(1);
        iprod = ModInt<MOD>(1);
        for (int i = 0; i < (int)R - 2; ++i) {
            rate3[i] = prod * root[i + 3];
            irate3[i] = iprod * iroot[i + 3];
            prod *= iroot[i + 3];
            iprod *= root[i + 3];
        }
        ModInt<MOD> i2 = ModInt<MOD>(2).inv();
        inv2[0] = ModInt<MOD>(1);
        for (int i = 0; i < (int)R; ++i) {
            inv2[i + 1] = inv2[i] * i2;
        }
    }
};

template <typename M>
void fft(M *a, int n) {
    using ull = unsigned long long;
    static_assert(M::get_mod() < (1u << 30));
    static constexpr FFTRoot<M::get_mod()> fftroot;
    static constexpr ull CEIL = 2ULL * M::get_mod() * M::get_mod();
    int l = __builtin_ctz(n);
    int ph = 0;
    while (ph < l) {
        if (ph + 1 == l) {
            int b = 1 << ph;
            M z = M::raw(1);
            for (int i = 0; i < b; ++i) {
                int offset = i << 1;
                M x = a[offset];
                M y = a[offset + 1] * z;
                a[offset] = x + y;
                a[offset + 1] = x - y;
                z *= fftroot.rate2[__builtin_ctz(~i)];
            }
            ++ph;
        } else {
            int bl = 1 << ph;
            int wd = 1 << (l - 2 - ph);
            M zeta = M::raw(1);
            for (int i = 0; i < bl; ++i) {
                int offset = i << (l - ph);
                M zeta2 = zeta * zeta;
                M zeta3 = zeta2 * zeta;
                for (int j = 0; j < wd; ++j) {
                    ull w = a[offset + j].val;
                    ull x = (ull)a[offset + j + wd].val * zeta.val;
                    ull y = (ull)a[offset + j + 2 * wd].val * zeta2.val;
                    ull z = (ull)a[offset + j + 3 * wd].val * zeta3.val;
                    ull ix_m_iz = (CEIL + x - z) % M::get_mod() * fftroot.root[2].val;
                    a[offset + j] = M(w + x + y + z);
                    a[offset + j + wd] = M(CEIL + w - x + y - z);
                    a[offset + j + 2 * wd] = M(CEIL + w - y + ix_m_iz);
                    a[offset + j + 3 * wd] = M(CEIL + w - y - ix_m_iz);
                }
                zeta *= fftroot.rate3[__builtin_ctz(~i)];
            }
            ph += 2;
        }
    }
}

template <typename M>
void ifft(M *a, int n) {
    using ull = unsigned long long;
    static_assert(M::get_mod() < (1u << 30));
    static constexpr FFTRoot<M::get_mod()> fftroot;
    int l = __builtin_ctz(n);
    int ph = l;
    while (ph > 0) {
        if (ph == 1) {
            --ph;
            int wd = 1 << (l - 1);
            for (int i = 0; i < wd; ++i) {
                M x = a[i];
                M y = a[i + wd];
                a[i] = x + y;
                a[i + wd] = x - y;
            }
        } else {
            ph -= 2;
            int bl = 1 << ph;
            int wd = 1 << (l - 2 - ph);
            M zeta = M::raw(1);
            for (int i = 0; i < bl; ++i) {
                int offset = i << (l - ph);
                M zeta2 = zeta * zeta;
                M zeta3 = zeta2 * zeta;
                for (int j = 0; j < wd; ++j) {
                    unsigned w = a[offset + j].val;
                    unsigned x = a[offset + j + wd].val;
                    unsigned y = a[offset + j + 2 * wd].val;
                    unsigned z = a[offset + j + 3 * wd].val;
                    unsigned iy_m_iz = (ull)(M::get_mod() + y - z) * fftroot.root[2].val % M::get_mod();
                    a[offset + j] = M(w + x + y + z);
                    a[offset + j + wd] = M((ull)zeta.val * (2 * M::get_mod() + w - x - iy_m_iz));
                    a[offset + j + 2 * wd] = M((ull)zeta2.val * (2 * M::get_mod() + w + x - y - z));
                    a[offset + j + 3 * wd] = M((ull)zeta3.val * (M::get_mod() + w - x + iy_m_iz));
                }
                zeta *= fftroot.irate3[__builtin_ctz(~i)];
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        a[i] *= fftroot.inv2[l];
    }
}

template <typename M>
void fft(std::vector<M> &a) {
    fft(a.data(), (int)a.size());
}
template <typename M>
void ifft(std::vector<M> &a) {
    ifft(a.data(), (int)a.size());
}

template <typename M>
std::vector<M> convolve_naive(const std::vector<M> &a,
                              const std::vector<M> &b) {
    int n = (int)a.size();
    int m = (int)b.size();
    std::vector<M> c(n + m - 1);
    if (n < m) {
        for (int j = 0; j < m; ++j) {
            for (int i = 0; i < n; ++i) {
                c[i + j] += a[i] * b[j];
            }
        }
    } else {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                c[i + j] += a[i] * b[j];
            }
        }
    }
    return c;
}

template <typename M>
std::vector<M> convolve_fft(std::vector<M> a, std::vector<M> b) {
    int n = (int)a.size() + (int)b.size() - 1;
    int m = 1;
    while (m < n) {
        m <<= 1;
    }
    bool shr = false;
    M last;
    if (n >= 3 && n == m / 2 + 1) {
        shr = true;
        last = a.back() * b.back();
        m /= 2;
        while ((int)a.size() > m) {
            a[(int)a.size() - 1 - m] += a.back();
            a.pop_back();
        }
        while ((int)b.size() > m) {
            b[(int)b.size() - 1 - m] += b.back();
            b.pop_back();
        }
    }
    a.resize(m);
    b.resize(m);
    fft(a);
    fft(b);
    for (int i = 0; i < m; ++i) {
        a[i] *= b[i];
    }
    ifft(a);
    a.resize(n);
    if (shr) {
        a[0] -= last;
        a[n - 1] = last;
    }
    return a;
}

template <typename M>
std::vector<M> convolve(const std::vector<M> &a, const std::vector<M> &b) {
    if (a.empty() || b.empty()) {
        return std::vector<M>(0);
    }
    if (std::min(a.size(), b.size()) <= 60) {
        return convolve_naive(a, b);
    } else {
        return convolve_fft(a, b);
    }
}

template <typename M>
std::vector<M> convolve_square_fft(std::vector<M> a) {
    int n = (int)2 * a.size() - 1;
    int m = 1;
    while (m < n) {
        m <<= 1;
    }
    bool shr = false;
    M last;
    if (n >= 3 && n == m / 2 + 1) {
        shr = true;
        last = a.back() * a.back();
        m /= 2;
        while ((int)a.size() > m) {
            a[(int)a.size() - 1 - m] += a.back();
            a.pop_back();
        }
    }
    a.resize(m);
    fft(a);
    for (int i = 0; i < m; ++i) {
        a[i] *= a[i];
    }
    ifft(a);
    a.resize(n);
    if (shr) {
        a[0] -= last;
        a[n - 1] = last;
    }
    return a;
}

template <typename M>
std::vector<M> convolve_square(const std::vector<M> &a) {
    if (a.empty()) {
        return std::vector<M>(0);
    }
    if ((int)a.size() <= 60) {
        return convolve_naive(a, a);
    } else {
        return convolve_square_fft(a);
    }
}

template <typename M>
void transposed_fft(M *a, int n) {
    ifft(a, n);
    std::reverse(a + 1, a + n);
    M c(n);
    for (int i = 0; i < n; ++i) {
        a[i] *= c;
    }
}
template <typename M>
void transposed_fft(std::vector<M> &a) {
    transposed_fft(a.data(), (int)a.size());
}

template <typename M>
void transposed_ifft(M *a, int n) {
    static constexpr FFTRoot<M::get_mod()> roots;
    std::reverse(a + 1, a + n);
    fft(a, n);
    M c = roots.inv2[__builtin_ctz(n)];
    for (int i = 0; i < n; ++i) {
        a[i] *= c;
    }
}
template <typename M>
void transposed_ifft(std::vector<M> &a) {
    transposed_ifft(a.data(), (int)a.size());
}
// ============
// ============
// ============
// ============
// ============
// ============
#include <algorithm>
// f(x) -> f(x+c)
template <typename M>
std::vector<M> taylor_shift(std::vector<M> f, M c) {
    for (int i = 0; i < (int)f.size(); ++i) {
        f[i] *= fact<M>(i);
    }
    std::reverse(f.begin(), f.end());
    M cp(1);
    std::vector<M> g(f.size());
    for (int i = 0; i < (int)f.size(); ++i) {
        g[i] = cp * inv_fact<M>(i);
        cp *= c;
    }
    std::vector<M> h = convolve(f, g);
    h.resize(f.size());
    std::reverse(h.begin(), h.end());
    for (int i = 0; i < (int)f.size(); ++i) {
        h[i] *= inv_fact<M>(i);
    }
    return h;
}
// ============

template <typename M>
std::vector<M> transposed_power_projection(int n, const std::vector<M> &f, const std::vector<M> &g) {
    static constexpr FFTRoot<M::get_mod()> roots;
    static constexpr M INV2 = M(2).inv();
    int lg = __builtin_ctz(n);
    std::vector<int> btr(n, 0);
    for (int i = 1; i < n; ++i) {
        btr[i] = (btr[i >> 1] >> 1) | ((i & 1) << (lg - 1));
    }
    M omega = roots.iroot[lg + 1];
    M pw(1);
    std::vector<M> invs(n);
    for (int idx : btr) {
        invs[idx] = pw;
        pw *= omega;
    }
    std::vector<M> p = g;
    std::reverse(p.begin(), p.end());
    transposed_ifft(p);
    p.resize(2 * n);
    std::vector<M> q(2 * n);
    for (int i = 0; i < n; ++i) {
        q[2 * i] = -f[i];
    }
    q[0] += M(1);
    auto rec = [&](auto rec, int h, int w) -> void {
        if (h == 1) {
            return;
        }
        M omega = roots.root[__builtin_ctz(w) + 1];
        for (int i = 0; i < 2 * n; i += 2 * w) {
            std::copy(q.begin() + i, q.begin() + i + w, q.begin() + i + w);
            ifft(q.data() + i + w, w);
            if (i == 0) {
                q[i + w] -= M(2);
            }
            M pw(1);
            for (int j = i + w; j < i + 2 * w; ++j) {
                q[j] *= pw;
                pw *= omega;
            }
            fft(q.data() + i + w, w);
        }
        std::vector<std::vector<M>> rq(2 * w, std::vector<M>(2 * h));
        std::vector<M> wq(h);
        for (int j = 0; j < 2 * w; ++j) {
            for (int i = 0; i < h; ++i) {
                rq[j][i] = q[2 * w * i + j];
            }
            fft(rq[j]);
            for (int i = 0; i < h; ++i) {
                wq[i] = rq[j][2 * i] * rq[j][2 * i + 1];
            }
            ifft(wq);
            for (int i = 0; i < h / 2; ++i) {
                q[4 * w * i + j] = wq[i];
                q[4 * w * i + j + 2 * w] = M();
            }
        }
        rec(rec, h / 2, w * 2);
        std::vector<M> rp(2 * h);
        for (int j = 0; j < 2 * w; ++j) {
            for (int i = 0; i < h / 2; ++i) {
                wq[i] = p[4 * w * i + j];
            }
            std::fill(wq.begin() + h / 2, wq.end(), M());
            transposed_ifft(wq);
            for (int i = 0; i < h; ++i) {
                rp[2 * i] = wq[i] * rq[j][2 * i + 1] * INV2 * invs[i];
                rp[2 * i + 1] = -wq[i] * rq[j][2 * i] * INV2 * invs[i];
            }
            transposed_fft(rp);
            for (int i = 0; i < h; ++i) {
                p[2 * w * i + j] = rp[i];
            }
        }
        for (int i = 0; i < 2 * n; i += 2 * w) {
            transposed_fft(p.data() + i + w, w);
            M pw(1);
            for (int j = i + w; j < i + 2 * w; ++j) {
                p[j] *= pw;
                pw *= omega;
            }
            transposed_ifft(p.data() + i + w, w);
            for (int j = i; j < i + w; ++j) {
                p[j] += p[j + w];
                p[j + w] = M();
            }
        }
    };
    rec(rec, n, 1);
    std::vector<M> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = p[2 * i];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

// f(g(x)), length: max(f.size(), g.size())
template <typename M>
std::vector<M> compose(std::vector<M> f, std::vector<M> g) {
    if (f.empty() || g.empty()) {
        return std::vector<M>();
    }
    int m = (int)std::max(f.size(), g.size());
    int n = 1;
    while (n < m) {
        n *= 2;
    }
    f.resize(n);
    g.resize(n);
    if (g[0] != M()) {
        f = taylor_shift(f, g[0]);
        g[0] = M();
    }
    std::vector<M> h = transposed_power_projection(n, g, f);
    h.resize(m);
    return h;
}
// ============
// ============
#include <bits/stdc++.h>
#define OVERRIDE(a, b, c, d, ...) d
#define REP2(i, n) for (i32 i = 0; i < (i32)(n); ++i)
#define REP3(i, m, n) for (i32 i = (i32)(m); i < (i32)(n); ++i)
#define REP(...) OVERRIDE(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define PER2(i, n) for (i32 i = (i32)(n)-1; i >= 0; --i)
#define PER3(i, m, n) for (i32 i = (i32)(n)-1; i >= (i32)(m); --i)
#define PER(...) OVERRIDE(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define ALL(x) begin(x), end(x)
#define LEN(x) (i32)(x.size())
using namespace std;
using u32 = unsigned int;
using u64 = unsigned long long;
using i32 = signed int;
using i64 = signed long long;
using f64 = double;
using f80 = long double;
using pi = pair<i32, i32>;
using pl = pair<i64, i64>;
template <typename T>
using V = vector<T>;
template <typename T>
using VV = V<V<T>>;
template <typename T>
using VVV = V<V<V<T>>>;
template <typename T>
using VVVV = V<V<V<V<T>>>>;
template <typename T>
using PQR = priority_queue<T, V<T>, greater<T>>;
template <typename T>
bool chmin(T &x, const T &y) {
    if (x > y) {
        x = y;
        return true;
    }
    return false;
}
template <typename T>
bool chmax(T &x, const T &y) {
    if (x < y) {
        x = y;
        return true;
    }
    return false;
}
template <typename T>
i32 lob(const V<T> &arr, const T &v) {
    return (i32)(lower_bound(ALL(arr), v) - arr.begin());
}
template <typename T>
i32 upb(const V<T> &arr, const T &v) {
    return (i32)(upper_bound(ALL(arr), v) - arr.begin());
}
template <typename T>
V<i32> argsort(const V<T> &arr) {
    V<i32> ret(arr.size());
    iota(ALL(ret), 0);
    sort(ALL(ret), [&](i32 i, i32 j) -> bool {
        if (arr[i] == arr[j]) {
            return i < j;
        } else {
            return arr[i] < arr[j];
        }
    });
    return ret;
}
#ifdef INT128
using u128 = __uint128_t;
using i128 = __int128_t;
#endif
[[maybe_unused]] constexpr i32 INF = 1000000100;
[[maybe_unused]] constexpr i64 INF64 = 3000000000000000100;
struct SetUpIO {
    SetUpIO() {
#ifdef FAST_IO
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
#endif
        cout << fixed << setprecision(15);
    }
} set_up_io;
void scan(char &x) { cin >> x; }
void scan(u32 &x) { cin >> x; }
void scan(u64 &x) { cin >> x; }
void scan(i32 &x) { cin >> x; }
void scan(i64 &x) { cin >> x; }
void scan(f64 &x) { cin >> x; }
void scan(string &x) { cin >> x; }
template <typename T>
void scan(V<T> &x) {
    for (T &ele : x) {
        scan(ele);
    }
}
void read() {}
template <typename Head, typename... Tail>
void read(Head &head, Tail &...tail) {
    scan(head);
    read(tail...);
}
#define CHAR(...)     \
    char __VA_ARGS__; \
    read(__VA_ARGS__);
#define U32(...)     \
    u32 __VA_ARGS__; \
    read(__VA_ARGS__);
#define U64(...)     \
    u64 __VA_ARGS__; \
    read(__VA_ARGS__);
#define I32(...)     \
    i32 __VA_ARGS__; \
    read(__VA_ARGS__);
#define I64(...)     \
    i64 __VA_ARGS__; \
    read(__VA_ARGS__);
#define F64(...)     \
    f64 __VA_ARGS__; \
    read(__VA_ARGS__);
#define STR(...)        \
    string __VA_ARGS__; \
    read(__VA_ARGS__);
#define VEC(type, name, size) \
    V<type> name(size);       \
    read(name);
#define VVEC(type, name, size1, size2)    \
    VV<type> name(size1, V<type>(size2)); \
    read(name);
// ============
// ============
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>

// unable to read INT_MIN (int), LLONG_MIN (long long)
class Reader {
    FILE *fp;
    static constexpr int BUF = 1 << 18;
    char buf[BUF];
    char *pl, *pr;

    void reread() {
        int wd = pr - pl;
        std::memcpy(buf, pl, wd);
        pl = buf;
        pr = buf + wd;
        pr += std::fread(pr, 1, BUF - wd, fp);
    }

    char skip() {
        char ch = *pl++;
        while (ch <= ' ') {
            ch = *pl++;
        }
        return ch;
    }

    template <typename T>
    void read_unsigned(T &x) {
        if (pr - pl < 64) {
            reread();
        }
        x = 0;
        char ch = skip();
        while ('0' <= ch) {
            x = 10 * x + (0xf & ch);
            ch = *pl++;
        }
    }
    template <typename T>
    void read_signed(T &x) {
        if (pr - pl < 64) {
            reread();
        }
        x = 0;
        bool neg = false;
        char ch = skip();
        if (ch == '-') {
            ch = *pl++;
            neg = true;
        }
        while ('0' <= ch) {
            x = 10 * x + (0xf & ch);
            ch = *pl++;
        }
        if (neg) {
            x = -x;
        }
    }

    void read_single(int &x) { read_signed(x); }
    void read_single(unsigned &x) { read_unsigned(x); }
    void read_single(long &x) { read_signed(x); }
    void read_single(unsigned long &x) { read_signed(x); }
    void read_single(long long &x) { read_signed(x); }
    void read_single(unsigned long long &x) { read_unsigned(x); }

public:
    Reader(FILE *fp) : fp(fp), pl(buf), pr(buf) { reread(); }

    void read() {}
    template <typename Head, typename... Tail>
    void read(Head &head, Tail &...tail) {
        read_single(head);
        read(tail...);
    }
};

struct NumberToString {
    char buf[10000][4];
    constexpr NumberToString() : buf() {
        for (int i = 0; i < 10000; ++i) {
            int n = i;
            for (int j = 3; j >= 0; --j) {
                buf[i][j] = '0' + n % 10;
                n /= 10;
            }
        }
    }
} constexpr number_to_string_precalc;

class Writer {
    FILE *fp;
    static constexpr int BUF = 1 << 18;
    char buf[BUF];
    char *ptr;

    void write_u32(unsigned x) {
        if ((buf + BUF - ptr) < 32) {
            flush();
        }
        static char sml[12];
        int t = 8;
        while (x >= 10000) {
            unsigned n = x % 10000;
            x /= 10000;
            std::memcpy(sml + t, number_to_string_precalc.buf[n], 4);
            t -= 4;
        }
        if (x >= 1000) {
            std::memcpy(ptr, number_to_string_precalc.buf[x], 4);
            ptr += 4;
        } else if (x >= 100) {
            std::memcpy(ptr, number_to_string_precalc.buf[x] + 1, 3);
            ptr += 3;
        } else if (x >= 10) {
            unsigned q = (x * 103) >> 10;
            *ptr++ = q | '0';
            *ptr++ = (x - 10 * q) | '0';
        } else {
            *ptr++ = '0' | x;
        }
        std::memcpy(ptr, sml + (t + 4), 8 - t);
        ptr += 8 - t;
    }

    void write_u64(unsigned long long x) {
        if ((buf + BUF - ptr) < 32) {
            flush();
        }
        if (x >= 10000000000000000) {
            unsigned long long z = x % 100000000;
            x /= 100000000;
            unsigned long long y = x % 100000000;
            x /= 100000000;
            if (x >= 1000) {
                std::memcpy(ptr, number_to_string_precalc.buf[x], 4);
                ptr += 4;
            } else if (x >= 100) {
                std::memcpy(ptr, number_to_string_precalc.buf[x] + 1, 3);
                ptr += 3;
            } else if (x >= 10) {
                unsigned q = (x * 103) >> 10;
                *ptr++ = q | '0';
                *ptr++ = (x - 10 * q) | '0';
            } else {
                *ptr++ = '0' | x;
            }
            std::memcpy(ptr, number_to_string_precalc.buf[y / 10000], 4);
            std::memcpy(ptr + 4, number_to_string_precalc.buf[y % 10000], 4);
            std::memcpy(ptr + 8, number_to_string_precalc.buf[z / 10000], 4);
            std::memcpy(ptr + 12, number_to_string_precalc.buf[z % 10000], 4);
            ptr += 16;
        } else {
            static char sml[12];
            int t = 8;
            while (x >= 10000) {
                unsigned long long n = x % 10000;
                x /= 10000;
                std::memcpy(sml + t, number_to_string_precalc.buf[n], 4);
                t -= 4;
            }
            if (x >= 1000) {
                std::memcpy(ptr, number_to_string_precalc.buf[x], 4);
                ptr += 4;
            } else if (x >= 100) {
                std::memcpy(ptr, number_to_string_precalc.buf[x] + 1, 3);
                ptr += 3;
            } else if (x >= 10) {
                unsigned q = (x * 103) >> 10;
                *ptr++ = q | '0';
                *ptr++ = (x - 10 * q) | '0';
            } else {
                *ptr++ = '0' | x;
            }
            std::memcpy(ptr, sml + (t + 4), 8 - t);
            ptr += 8 - t;
        }
    }

    void write_char(char c) {
        if (ptr == buf + BUF) {
            flush();
        }
        *ptr++ = c;
    }

    template <typename T>
    void write_unsigned(T x) {
        if constexpr (std::is_same_v<T, unsigned long long> ||
                      std::is_same_v<T, unsigned long>) {
            write_u64(x);
        } else {
            write_u32(x);
        }
    }

    template <typename T>
    void write_signed(T x) {
        std::make_unsigned_t<T> y = x;
        if (x < 0) {
            write_char('-');
            y = -y;
        }
        write_unsigned(y);
    }
    
    void write_string(const std::string &s) {
        for (char c : s) {
            write_char(c);
        }
    }

    void write_single(int x) { write_signed(x); }
    void write_single(unsigned x) { write_unsigned(x); }
    void write_single(long x) { write_signed(x); }
    void write_single(unsigned long x) { write_unsigned(x); }
    void write_single(long long x) { write_signed(x); }
    void write_single(unsigned long long x) { write_unsigned(x); }
    void write_single(char c) { write_char(c); }
    void write_single(const std::string &s) { write_string(s); }

public:
    Writer(FILE *fp) : fp(fp), ptr(buf) {}
    ~Writer() { flush(); }

    void flush() {
        std::fwrite(buf, 1, ptr - buf, fp);
        ptr = buf;
    }

    void write() {}
    template <typename Head, typename... Tail>
    void write(Head &&head, Tail &&...tail) {
        write_single(head);
        if (sizeof...(Tail)) {
            write_char(' ');
        }
        write(std::forward<Tail>(tail)...);
    }

    template <typename... T>
    void writeln(T &&...t) {
        write(std::forward<T>(t)...);
        write_char('\n');
    }
};

Reader rd(stdin);
Writer wr(stdout);
// ============

int main() {
    using M = ModInt<998244353>;
    i32 n;
    rd.read(n);
    std::vector<M> f(n);
    REP(i, n) {
        rd.read(f[i].val);   
    }
    std::vector<M> g(n);
    REP(i, n) {
        rd.read(g[i].val);
    }
    std::vector<M> h = compose(f, g);
    assert(LEN(h) == n);
    REP(i, n) {
        wr.writeln(h[i].val);
    }
}
