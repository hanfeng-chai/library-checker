#include <bits/stdc++.h>
#if defined(__x86_64__) || defined(_M_X64)
#include <wmmintrin.h>
#pragma GCC target("pclmul")
#endif
using namespace std;

namespace gf64 {
using u64 = uint64_t;
using u128 = __uint128_t;

#if defined(__x86_64__) || defined(_M_X64)
static inline u128 clmul(u64 a, u64 b) {
    __m128i z = _mm_clmulepi64_si128(
        _mm_cvtsi64_si128((long long)a),
        _mm_cvtsi64_si128((long long)b),
        0x00
    );
    u128 r;
    memcpy(&r, &z, 16);
    return r;
}
#else
static inline u128 clmul(u64 a, u64 b) {
    u128 r = 0;
    for (int i = 0; i < 64; ++i) {
        if ((a >> i) & 1) r ^= u128(b) << i;
    }
    return r;
}
#endif

constexpr u128 clmul_constexpr(u64 a, u64 b) {
    u128 r = 0;
    for (int i = 0; i < 64; ++i) {
        if ((a >> i) & 1) r ^= u128(b) << i;
    }
    return r;
}

namespace aux {
static constexpr u128 MOD = (u128(1) << 64) | u128(27);
// x^64 + x^4 + x^3 + x + 1, low part is 27.

// Inverse of low(MOD) modulo x^64, for GF(2) Montgomery reduction.
static constexpr u64 INV = [] {
    u64 a = 1;
    for (int i = 0; i < 6; ++i) {
        a = (u64)clmul_constexpr(a, (u64)clmul_constexpr(a, (u64)MOD));
    }
    return a;
}();

static constexpr auto pow_x = [](int e) {
    u128 r = 1;
    for (int i = 0; i < e; ++i) {
        r <<= 1;
        if ((r >> 64) & 1) r ^= MOD;
    }
    return (u64)r;
};

static constexpr u64 R2 = pow_x(128);
static_assert((u64)clmul_constexpr((u64)MOD, INV) == 1);
} // namespace aux

struct F {
    // Montgomery representation modulo P(x). Addition is still XOR.
    u64 val;

    F() : val(0) {}
    F(u64 v, int) : val(v) {}
    explicit F(u64 v) : val(reduce(clmul(v, aux::R2))) {}

    static F bit(long long x) {
        return F((u64)(x & 1));
    }

    static u64 reduce(u128 x) {
        u64 f = (u64)clmul((u64)x, aux::INV);
        return (u64)(x >> 64) ^ (u64)(clmul(f, (u64)aux::MOD) >> 64) ^ f;
    }

    u64 get() const {
        return reduce(val);
    }

    F& operator+=(const F& o) {
        val ^= o.val;
        return *this;
    }

    F& operator-=(const F& o) {
        val ^= o.val;
        return *this;
    }

    F& operator*=(const F& o) {
        val = reduce(clmul(val, o.val));
        return *this;
    }

    friend F operator+(F a, const F& b) {
        return a += b;
    }

    friend F operator-(F a, const F& b) {
        return a -= b;
    }

    friend F operator*(F a, const F& b) {
        return a *= b;
    }

    bool operator==(const F& o) const {
        return val == o.val;
    }

    bool operator!=(const F& o) const {
        return val != o.val;
    }

    F pow(u64 e) const {
        F r = bit(1), b = *this;
        while (e) {
            if (e & 1) r *= b;
            b *= b;
            e >>= 1;
        }
        return r;
    }

    F inv() const {
        return pow(~u64(0) - 1);
    }
};
} // namespace gf64

template <class T>
struct Span {
    T* p;
    size_t n;

    Span() : p(nullptr), n(0) {}
    Span(T* p_, size_t n_) : p(p_), n(n_) {}
    Span(vector<T>& v) : p(v.data()), n(v.size()) {}

    T& operator[](size_t i) const {
        return p[i];
    }

    T* begin() const {
        return p;
    }

    T* end() const {
        return p + n;
    }

    size_t size() const {
        return n;
    }

    Span subspan(size_t l, size_t cnt = numeric_limits<size_t>::max()) const {
        cnt = min(cnt, n - l);
        return Span(p + l, cnt);
    }
};

namespace convolution {
using gf64::F;

template <class R>
vector<R> naive(const vector<R>& a, const vector<R>& b) {
    vector<R> c(a.size() + b.size() - 1);
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b.size(); ++j) {
            c[i + j] += a[i] * b[j];
        }
    }
    return c;
}

static vector<F> enumerate_subspace_points(const vector<F>& basis) {
    int lg = (int)basis.size();
    vector<F> g(1u << lg);
    for (int k = 0; k < lg; ++k) {
        for (int i = 0; i < (1 << k); ++i) {
            g[(1 << k) + i] = g[i] + basis[k];
        }
    }
    return g;
}

struct AdditiveFFTData {
    struct Level {
        vector<F> basis;
        vector<F> normalized;
        vector<F> next_basis;
        vector<F> points;
        mutable vector<F> buf;

        void init() {
            int lg = (int)basis.size();
            buf.resize(1u << lg);

            F last_inv = basis.back().inv();

            normalized.assign(lg - 1, F());
            for (int i = 0; i < lg - 1; ++i) {
                normalized[i] = basis[i] * last_inv;
            }

            next_basis.assign(lg - 1, F());
            for (int i = 0; i < lg - 1; ++i) {
                next_basis[i] = normalized[i] * normalized[i] + normalized[i];
            }

            points = enumerate_subspace_points(normalized);
        }
    };

    vector<Level> level;

    void prepare(int lg) {
        if ((int)level.size() > lg) return;

        // Artin-Schreier chain: x, x^2+x, x^4+x, ...
        mt19937_64 rng;
        vector<F> chain;

        while ((int)chain.size() < lg) {
            chain.clear();
            for (F x = F(rng()); x != F(); x = x * x + x) {
                chain.push_back(x);
            }
        }

        chain.erase(chain.begin(), chain.begin() + ((int)chain.size() - lg));

        level.resize(lg + 1);
        level[lg].basis = chain;

        for (int k = lg; k > 0; --k) {
            level[k].init();
            level[k - 1].basis = level[k].next_basis;
        }
    }
};

static AdditiveFFTData fft_data;

template <bool inverse>
static void taylor_shift(Span<F> f) {
    for (size_t len = inverse ? 1 : f.size() / 4;
         inverse ? len * 4 <= f.size() : len >= 1;
         inverse ? len *= 2 : len /= 2) {
        for (size_t base = 0; base < f.size(); base += 4 * len) {
            for (size_t i = 0; i < len; ++i) {
                F b = f[base + len + i];
                F c = f[base + 2 * len + i];
                F d = f[base + 3 * len + i];

                f[base + len + i] = inverse ? b + c : b + c + d;
                f[base + 2 * len + i] = c + d;
            }
        }

        if (!inverse && len == 1) break;
    }
}

template <bool inverse>
static void fft(Span<F> f) {
    size_t n = f.size();
    if (n == 1) return;

    int lg = 31 - __builtin_clz((unsigned)n);
    const auto& d = fft_data.level[lg];

    if (n == 2) {
        f[1] = f[0] + f[1];
        return;
    }

    Span<F> even(d.buf.data(), n / 2);
    Span<F> odd(d.buf.data() + n / 2, n / 2);

    if constexpr (!inverse) {
        taylor_shift<false>(f);

        for (size_t i = 0; i < n / 2; ++i) {
            even[i] = f[2 * i];
            odd[i] = f[2 * i + 1];
        }

        fft<false>(even);
        fft<false>(odd);

        for (size_t i = 0; i < n / 2; ++i) {
            F a = even[i] + d.points[i] * odd[i];
            f[i] = a;
            f[i + n / 2] = a + odd[i];
        }
    } else {
        for (size_t i = 0; i < n / 2; ++i) {
            F a = f[i];
            F b = f[i + n / 2];

            odd[i] = a + b;
            even[i] = a + d.points[i] * odd[i];
        }

        fft<true>(even);
        fft<true>(odd);

        for (size_t i = 0; i < n / 2; ++i) {
            f[2 * i] = even[i];
            f[2 * i + 1] = odd[i];
        }

        taylor_shift<true>(f);
    }
}

static vector<F> convolve(vector<F> a, vector<F> b) {
    if (a.empty() || b.empty()) return {};

    size_t n = a.size();
    size_t m = b.size();
    size_t need = n + m - 1;

    int lg = 0;
    while ((1ULL << lg) < need) ++lg;

    // Small cases are faster naively.
    if ((__uint128_t)n * m <= (__uint128_t(1) << lg) * (lg + 1) * (lg + 1)) {
        return naive(a, b);
    }

    // Avoid a transform twice as large for the exact half+1 boundary.
    if (lg > 3 && need == (1ULL << (lg - 1)) + 1) {
        vector<F> add(m);

        for (size_t i = 0; i < m; ++i) {
            add[i] = a.back() * b[i];
        }

        a.pop_back();

        vector<F> res = convolve(move(a), move(b));
        res.push_back(F());

        for (size_t i = 0; i < m; ++i) {
            res[(n - 1) + i] += add[i];
        }

        return res;
    }

    size_t len = 1ULL << lg;

    fft_data.prepare(lg);

    a.resize(len);
    b.resize(len);

    fft<false>(Span<F>(a));
    fft<false>(Span<F>(b));

    for (size_t i = 0; i < len; ++i) {
        a[i] *= b[i];
    }

    fft<true>(Span<F>(a));

    a.resize(need);
    return a;
}
} // namespace convolution

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n, m;
    cin >> n >> m;

    vector<gf64::F> a(n), b(m);

    for (auto& x : a) {
        uint64_t v;
        cin >> v;
        x = gf64::F(v);
    }

    for (auto& x : b) {
        uint64_t v;
        cin >> v;
        x = gf64::F(v);
    }

    vector<gf64::F> c = convolution::convolve(move(a), move(b));

    for (size_t i = 0; i < c.size(); ++i) {
        cout << c[i].get() << (i + 1 == c.size() ? '\n' : ' ');
    }

    return 0;
}