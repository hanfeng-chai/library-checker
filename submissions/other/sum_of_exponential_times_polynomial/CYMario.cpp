#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <algorithm>
using namespace std;
typedef int i32;
typedef unsigned u32;
typedef long long i64;
typedef unsigned long long u64;
// wheel-sieve
namespace prime_sieve
{
    const int max_primes = 160000;
    const int sieve_span = 1 << 22;
    const int sieve_words = sieve_span >> 7;
    const int wheel_size = 3 * 5 * 7 * 11 * 13;

    u64 mask[64]; // mask[i] = 1ull << i;
    bool built_mask;
    void build_mask()
    {
        if (built_mask)
            return;
        built_mask = true;
        mask[0] = 1ull;
        for (int i = 1; i < 64; ++i)
            mask[i] = mask[i - 1] << 1ull;
    }

    int primes[max_primes], mcnt;
    u64 sieve[sieve_words];
    u64 pattern[wheel_size];
    void mark(u64 *s, int o) { s[o >> 6] |= mask[o & 63]; }
    void unmark(u64 *s, int o) { s[o >> 6] &= ~mask[o & 63]; }
    int test(u64 *s, int o) { return (s[o >> 6] & mask[o & 63]) == 0; }

    int all[6000000 + sieve_span], pcnt;

    int get_prime(int k) { return all[k < pcnt ? k : pcnt - 1]; }

    bool pre_sieved;

    void pre_sieve()
    {
        if (pre_sieved)
            return;
        pre_sieved = true;
        for (int i = 0; i < (1048576 >> 7); ++i)
            sieve[i] = 0;
        for (int i = 3; i < 1024; i += 2)
            if (test(sieve, i >> 1))
                for (int j = (i * i) >> 1; j < 1048576; j += i)
                    mark(sieve, j);
        mcnt = 0;
        for (int i = 8; i < 1048576; ++i)
            if (test(sieve, i))
                primes[mcnt++] = (i << 1) + 1;
        // printf("m is %d\n", m);
        for (int i = 0; i < wheel_size; ++i)
            pattern[i] = 0;
        for (int i = 1; i < wheel_size * 64; i += 3)
            mark(pattern, i);
        for (int i = 2; i < wheel_size * 64; i += 5)
            mark(pattern, i);
        for (int i = 3; i < wheel_size * 64; i += 7)
            mark(pattern, i);
        for (int i = 5; i < wheel_size * 64; i += 11)
            mark(pattern, i);
        for (int i = 6; i < wheel_size * 64; i += 13)
            mark(pattern, i);
    }

    void update_sieve(int base)
    {
        int o = base % wheel_size;
        o = (o + ((o * 105) & 127) * wheel_size) >> 7;
        for (int i = 0, k; i < sieve_words; i += k, o = 0)
        {
            k = min(wheel_size - o, sieve_words - i);
            memcpy(sieve + i, pattern + o, sizeof(*pattern) * k);
        }
        if (base == 0)
        { // mark 1 as not prime, and mark 3, 5, 7, 11, and 13 as prime
            sieve[0] |= mask[0];
            sieve[0] &= ~(mask[1] | mask[2] | mask[3] | mask[5] | mask[6]);
        }
        for (int i = 0; i < mcnt; ++i)
        {
            i64 j = primes[i] * primes[i];
            if (j > base + sieve_span - 1)
                break;
            if (j > base)
                j = (j - base) >> 1;
            else
            {
                j = primes[i] - base % primes[i];
                if ((j & 1) == 0)
                    j += primes[i];
                j >>= 1;
            }
            while (j < sieve_span >> 1)
            {
                mark(sieve, j);
                j += primes[i];
            }
        }
    }

    // sieve [base, min(base+span, lim)]
    void segment_sieve_with_lower(int base, int lower, int upper)
    {
        update_sieve(base);
        int u = min(base + sieve_span, upper);
        for (int i = 0; i < sieve_words; ++i)
        {
            u64 o = ~sieve[i];
            while (o)
            {
                int p = __builtin_ctzll(o);
                i64 u = base + (i << 7) + (p << 1) + 1;
                if (u >= upper)
                    break;
                if (u >= lower)
                    all[pcnt++] = u;
                o -= o & ((~o) + 1);
            }
        }
    }

    void segment_sieve(int base, int upper)
    {
        update_sieve(base);
        int u = min(base + sieve_span, upper);
        for (int i = 0; i < sieve_words; ++i)
        {
            u64 o = ~sieve[i];
            while (o)
            {
                int p = __builtin_ctzll(o);
                i64 u = base + (i << 7) + (p << 1) + 1;
                if (u >= upper)
                    break;
                all[pcnt++] = u;
                o -= o & ((~o) + 1);
            }
        }
    }

    // sieve from [0, n)
    void fast_sieve(int lim)
    {
        build_mask();
        pre_sieve();
        pcnt = 0;
        all[pcnt++] = 2;
        int i = 0, now = 1;
        for (int base = 0; base < lim; base += sieve_span)
            segment_sieve(base, lim);
    }
    // sieve from [lo, hi)
    void segmented_sieve(int lo, int hi)
    {
        build_mask();
        pre_sieve();
        pcnt = 0;
        if (lo <= 2)
            all[pcnt++] = 2;
        int base = (lo / sieve_span) * sieve_span;
        segment_sieve_with_lower(base, lo, hi), base += sieve_span;
        while (base < hi)
            segment_sieve(base, hi), base += sieve_span;
    }
}

template <uint32_t mod>
struct LazyMontgomeryModInt
{
    using mint = LazyMontgomeryModInt;

    static constexpr u32 get_r()
    {
        u32 ret = mod;
        for (i32 i = 0; i < 4; ++i)
            ret *= 2 - mod * ret;
        return ret;
    }

    static constexpr u32 r = get_r();
    static constexpr u32 n2 = -u64(mod) % mod;
    static_assert(r * mod == 1, "invalid, r * mod != 1");
    static_assert(mod < (1 << 30), "invalid, mod >= 2 ^ 30");
    static_assert((mod & 1) == 1, "invalid, mod % 2 == 0");

    u32 a;

    constexpr LazyMontgomeryModInt() : a(0) {}
    constexpr LazyMontgomeryModInt(const int64_t &b)
        : a(reduce(u64(b % mod + mod) * n2)){};

    static constexpr u32 reduce(const u64 &b) { return (b + u64(u32(b) * u32(-r)) * mod) >> 32; }

    constexpr mint &operator+=(const mint &b)
    {
        if (i32(a += b.a - 2 * mod) < 0)
            a += 2 * mod;
        return *this;
    }

    constexpr mint &operator-=(const mint &b)
    {
        if (i32(a -= b.a) < 0)
            a += 2 * mod;
        return *this;
    }

    constexpr mint &operator*=(const mint &b)
    {
        a = reduce(u64(a) * b.a);
        return *this;
    }

    constexpr mint &operator/=(const mint &b)
    {
        *this *= b.inverse();
        return *this;
    }

    constexpr mint operator+(const mint &b) const { return mint(*this) += b; }
    constexpr mint operator-(const mint &b) const { return mint(*this) -= b; }
    constexpr mint operator*(const mint &b) const { return mint(*this) *= b; }
    constexpr mint operator/(const mint &b) const { return mint(*this) /= b; }
    constexpr bool operator==(const mint &b) const
    {
        return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
    }
    constexpr bool operator!=(const mint &b) const
    {
        return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
    }
    constexpr mint operator-() const { return mint() - mint(*this); }

    constexpr mint pow(u64 n) const
    {
        mint ret(1), mul(*this);
        while (n > 0)
        {
            if (n & 1)
                ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    constexpr mint inverse() const { return pow(mod - 2); }

    constexpr u32 get() const
    {
        u32 ret = reduce(a);
        return ret >= mod ? ret - mod : ret;
    }

    static constexpr u32 get_mod() { return mod; }
};

template <typename T>
struct Binomial
{
    vector<T> fac_, invfac_;

    Binomial(int MAX) : fac_(MAX + 10), invfac_(MAX + 10)
    {
        MAX += 9;
        fac_[0] = invfac_[0] = 1;
        for (int i = 1; i <= MAX; i++)
            fac_[i] = fac_[i - 1] * i;
        invfac_[MAX] = fac_[MAX].inverse();
        for (int i = MAX - 1; i > 0; i--)
            invfac_[i] = invfac_[i + 1] * (i + 1);
    }
    inline T fac(int i) const { return fac_[i]; }
    inline T finv(int i) const { return invfac_[i]; }
    inline T inv(int i) const { return fac_[i - 1] * invfac_[i]; }

    T C(int n, int r) const
    {
        if (n < r || r < 0)
            return T(0);
        return fac_[n] * invfac_[n - r] * invfac_[r];
    }
    T P(int n, int r) const
    {
        if (n < r || r < 0)
            return T(0);
        return fac_[n] * invfac_[n - r];
    }
    T H(int n, int r) const
    {
        if (n < 0 || r < 0)
            return (0);
        return r == 0 ? 1 : C(n + r - 1, r);
    }
};

// given y(x=0)...y(k) , return y(x)  deg(y) <= k+1
template <typename mint>
mint lagrange_interpolation(const vector<mint> &y, long long x, const Binomial<mint> &C)
{
    int N = (int)y.size() - 1;
    if (x <= N)
        return y[x];
    mint ret = 0;
    vector<mint> dp(N + 1, 1), pd(N + 1, 1);
    mint a = x, one = 1;
    for (int i = 0; i < N; i++)
        dp[i + 1] = dp[i] * a, a -= one;
    for (int i = N; i > 0; i--)
        pd[i - 1] = pd[i] * a, a += one;
    for (int i = 0; i <= N; i++)
    {
        mint tmp = y[i] * dp[i] * pd[i] * C.finv(i) * C.finv(N - i);
        ret += ((N - i) & 1) ? -tmp : tmp;
    }
    return ret;
}

// given f(0)...f(k) (deg(f) = k)
// return \sum_{i=0...n-1} a^i f(i)
template <typename mint>
mint sum_of_exp(const vector<mint> &f, mint a, long long n, const Binomial<mint> &C)
{
    if (n == 0)
        return mint(0);
    if (a == mint(0))
        return f[0];
    if (a == mint(1))
    {
        vector<mint> g(f.size() + 1, mint(0));
        for (int i = 1; i < (int)g.size(); i++)
            g[i] = g[i - 1] + f[i - 1];
        return lagrange_interpolation(g, n, C);
    }
    int K = f.size() - 1;
    vector<mint> g(f.size());
    mint buf = 1;
    for (int i = 0; i < (int)g.size(); i++)
        g[i] = f[i] * buf, buf *= a;
    for (int i = 1; i < (int)g.size(); i++)
        g[i] += g[i - 1];
    mint c = 0, buf2 = 1;
    for (int i = 0; i <= K; i++)
        c += C.C(K + 1, i) * buf2 * g[K - i], buf2 *= -a;
    c /= (-a + 1).pow(K + 1);
    mint buf3 = 1, ia = a.inverse();
    for (int i = 0; i < (int)g.size(); i++)
        g[i] = (g[i] - c) * buf3, buf3 *= ia;
    mint tn = lagrange_interpolation(g, n - 1, C);
    return tn * a.pow(n - 1) + c;
}

// given f(0)...f(k) (deg(f) = k)
// return \sum_{i=0...infty} a^i f(i)
template <typename mint>
mint sum_of_exp_limit(const vector<mint> &f, mint a, const Binomial<mint> &C)
{
    if (a == mint(0))
        return f[0];
    int K = f.size() - 1;
    vector<mint> g(f.size());
    mint buf = 1;
    for (int i = 0; i < (int)g.size(); i++)
        g[i] = f[i] * buf, buf *= a;
    for (int i = 1; i < (int)g.size(); i++)
        g[i] += g[i - 1];
    mint c = 0, buf2 = 1;
    for (int i = 0; i <= K; i++)
        c += C.C(K + 1, i) * buf2 * g[K - i], buf2 *= -a;
    c /= (-a + 1).pow(K + 1);
    return c;
}

template <typename mint>
vector<mint> exp_enamurate(int p, int n)
{
    vector<mint> f(n + 1, mint(0));
    if (!p)
        return f[0] = 1, f;
    f[1] = 1;
    for (int i = 0; prime_sieve::get_prime(i) <= n; ++i)
        f[prime_sieve::get_prime(i)] = (mint(prime_sieve::get_prime(i))).pow(p);
    for (int i = 2; i <= n; ++i)
        for (int j = 0; (prime_sieve::get_prime(j) <= i) && (1ll * i * prime_sieve::get_prime(j) <= n); ++j)
        {
            f[i * prime_sieve::get_prime(j)] = f[i] * f[prime_sieve::get_prime(j)];
            if (!(i % prime_sieve::get_prime(j)))
                break;
        }
    return f;
}
// \sum_{i=0}^{n-1}(r^i)*(i^d)
template <typename mint>
mint sum_of_exp2(int d, mint r, long long n, const Binomial<mint> &C)
{
    vector<mint> f = exp_enamurate<mint>(d, d);
    return sum_of_exp(f, r, n, C);
}
// \sum_{i=0}^{+\infty}(r^i)*(i^d)  r \in (-1, 1), modulo prime.
template <typename mint>
mint sum_of_exp_limit2(int d, mint r, const Binomial<mint> &C)
{
    vector<mint> f = exp_enamurate<mint>(d, d);
    return sum_of_exp_limit(f, r, C);
}

using mint = LazyMontgomeryModInt<998244353>;
long long r, d, n;
Binomial<mint> C(10010000);
int main()
{
    prime_sieve::segmented_sieve(1, 10010000);
    scanf("%lld%lld%lld", &r, &d, &n);
    printf("%d", sum_of_exp2<mint>(d, mint(r), n, C).get());
}