#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <numeric>
#include <span>
#include <string>
#include <vector>

using namespace std;
using u32 = uint32_t;
using u64 = uint64_t;

namespace fastio
{
static constexpr u32 buf_size = 1 << 17;
char ibuf[buf_size];
char obuf[buf_size];
u32 pil = 0, pir = 0, por = 0;
struct Pre
{
    char t[40000];
    constexpr Pre()
    {
        for (int i = 0; i < 10000; i++)
        {
            int n = i;
            for (int j = 3; j >= 0; j--)
            {
                t[i * 4 + j] = '0' + n % 10;
                n /= 10;
            }
        }
    }
} constexpr pre;
inline void load()
{
    if (pir - pil <= pil)
        memcpy(ibuf, ibuf + pil, pir - pil);
    else
        memmove(ibuf, ibuf + pil, pir - pil);
    pir = pir - pil + fread(ibuf + pir - pil, 1, buf_size - pir + pil, stdin);
    pil = 0;
}
inline void flush()
{
    fwrite(obuf, 1, por, stdout);
    por = 0;
}
inline void rd(char &c)
{
    c = ibuf[pil++];
}
inline void rd(std::string &s)
{
    char c;
    s.clear();
    if (pil == pir)
        load();
    rd(c);
    if (pil == pir)
        load();
    while (!std::isspace(c))
    {
        s += c;
        rd(c);
        if (pil == pir)
            load();
    }
}
inline void rd(__int128_t &x)
{
    if (pil + 50 > pir)
        load();
    char c;
    do
        rd(c);
    while (c < '-');
    bool neg = false;
    if (c == '-')
    {
        neg = true;
        rd(c);
    }
    x = 0;
    while (c >= '0')
    {
        x = x * 10 + (c & 15);
        rd(c);
    }
    if (neg)
        x = -x;
}
template <typename T> inline void rd(T &x)
{
    if (pil + 32 > pir)
        load();
    char c;
    do
        rd(c);
    while (c < '-');
    bool neg = false;
    if constexpr (std::is_signed_v<T>)
    {
        if (c == '-')
        {
            neg = true;
            rd(c);
        }
    }
    x = 0;
    while (c >= '0')
    {
        x = x * 10 + (c & 15);
        rd(c);
    }
    if constexpr (std::is_signed_v<T>)
    {
        if (neg)
            x = -x;
    }
}
inline void wt(char x)
{
    obuf[por++] = x;
}
template <typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr> inline void wt(T x)
{
    if (por + 32 > buf_size)
        flush();
    if (x == 0)
    {
        wt('0');
        return;
    }
    if constexpr (std::is_signed_v<T>)
    {
        if (x < 0)
        {
            wt('-');
            x = -x;
        }
    }
    if (x >= 10000000000000000)
    {
        u32 r1 = x % 100000000;
        u64 q1 = x / 100000000;
        u32 r2 = q1 % 100000000;
        u32 q2 = q1 / 100000000;
        u32 n1 = r1 % 10000, n2 = r1 / 10000, n3 = r2 % 10000, n4 = r2 / 10000;
        if (x >= 1000000000000000000)
        {
            if constexpr (std::is_unsigned_v<T>)
            {
                if (x >= 10000000000000000000ull)
                {
                    obuf[por++] = '1';
                }
            }
            memcpy(obuf + por, pre.t + (q2 << 2) + 1, 3);
            memcpy(obuf + por + 3, pre.t + (n4 << 2), 4);
            memcpy(obuf + por + 7, pre.t + (n3 << 2), 4);
            memcpy(obuf + por + 11, pre.t + (n2 << 2), 4);
            memcpy(obuf + por + 15, pre.t + (n1 << 2), 4);
            por += 19;
        }
        else if (x >= 100000000000000000)
        {
            u32 q3 = (q2 * 205) >> 11;
            u32 r3 = q2 - q3 * 10;
            obuf[por] = '0' + q3;
            obuf[por + 1] = '0' + r3;
            memcpy(obuf + por + 2, pre.t + (n4 << 2), 4);
            memcpy(obuf + por + 6, pre.t + (n3 << 2), 4);
            memcpy(obuf + por + 10, pre.t + (n2 << 2), 4);
            memcpy(obuf + por + 14, pre.t + (n1 << 2), 4);
            por += 18;
        }
        else
        {
            obuf[por] = '0' + q2;
            memcpy(obuf + por + 1, pre.t + (n4 << 2), 4);
            memcpy(obuf + por + 5, pre.t + (n3 << 2), 4);
            memcpy(obuf + por + 9, pre.t + (n2 << 2), 4);
            memcpy(obuf + por + 13, pre.t + (n1 << 2), 4);
            por += 17;
        }
    }
    else
    {
        static char buf[12];
        int i = 8;
        while (x >= 10000)
        {
            memcpy(buf + i, pre.t + ((x % 10000) << 2), 4);
            x /= 10000;
            i -= 4;
        }
        if (x < 100)
        {
            if (x < 10)
                obuf[por++] = '0' + x;
            else
            {
                obuf[por] = '0' + x / 10;
                obuf[por + 1] = '0' + x % 10;
                por += 2;
            }
        }
        else
        {
            if (x < 1000)
            {
                memcpy(obuf + por, pre.t + (x << 2) + 1, 3);
                por += 3;
            }
            else
            {
                memcpy(obuf + por, pre.t + (x << 2), 4);
                por += 4;
            }
        }
        memcpy(obuf + por, buf + i + 4, 8 - i);
        por += 8 - i;
    }
}
inline void wt(const std::string &s)
{
    int sz = s.size();
    if (por + sz > buf_size)
        flush();
    memcpy(obuf + por, s.c_str(), sz);
    por += sz;
}
inline void wt(__int128_t x)
{
    if (por + 50 > buf_size)
        flush();
    if (x == 0)
    {
        wt('0');
        return;
    }
    if (x < 0)
    {
        wt('-');
        x = -x;
    }
    static constexpr __int128_t b = 10000000000000000000ull;
    if (x >= b)
    {
        wt<u64>(x / b);
        u64 y = x % b;
        memcpy(obuf + por, pre.t + ((y / 10000000000000000) << 2) + 1, 3);
        memcpy(obuf + por + 3, pre.t + ((y / 1000000000000 % 10000) << 2), 4);
        memcpy(obuf + por + 7, pre.t + ((y / 100000000 % 10000) << 2), 4);
        memcpy(obuf + por + 11, pre.t + ((y / 10000 % 10000) << 2), 4);
        memcpy(obuf + por + 15, pre.t + ((y % 10000) << 2), 4);
        por += 19;
    }
    else
        wt<u64>(x);
}
struct Dummy
{
    Dummy() { std::atexit(flush); }
} dummy;
} // namespace fastio
using fastio::rd;
using fastio::wt;

inline u64 isqrt(u64 x)
{
    u64 res = sqrt(x);
    // This constant is the first input where floor(sqrt(n)) returns the wrong value.
    if (x < 4'503'599'761'588'224)
        return res;
    while (res * res > x)
        --res;
    return res;
}

inline u64 icbrt(u64 x)
{
    u64 res = std::pow((double)x, (1.0 + numeric_limits<double>::epsilon()) / 3);
    while (res * res * res > x)
        --res;
    return res;
}

/// Class for integers modulo a modulus. The modulus must be a compile-time constant.
template <u32 M>
    requires(M > 0 && M <= std::numeric_limits<u32>::max() / 2)
class ZMod
{
  public:
    using value_type = u32;

  private:
    value_type value_;

  public:
    static constexpr value_type modulus = M;

    ZMod() = default;

    /// Generic constructor to avoid unintentional narrowing conversion bugs!
    template <std::integral T> constexpr ZMod(T value) : value_((value % M + M) % M) {}

    // Shortcut the modulus operation if we know value is between 0 and M-1.
    constexpr ZMod(bool /*unused*/, const u32 &value) : value_(value) {}

    constexpr explicit operator value_type() const { return value_; }

    std::strong_ordering operator<=>(const ZMod &other) const = default;

    constexpr ZMod &operator+=(const ZMod &other)
    {
        value_ += other.value_;
        if (value_ >= M)
            value_ -= M;
        return *this;
    }

    [[nodiscard]] constexpr friend ZMod operator+(ZMod left, const ZMod &right)
    {
        left += right;
        return left;
    }

    constexpr ZMod &operator-=(const ZMod &other)
    {
        value_ = value_ < other.value_ ? value_ + (M - other.value_) : value_ - other.value_;
        return *this;
    }

    [[nodiscard]] constexpr friend ZMod operator-(ZMod left, const ZMod &right)
    {
        left -= right;
        return left;
    }

    constexpr ZMod &operator*=(const ZMod &other)
    {
        u64 res = (u64)value_ * other.value_;
        if (res >= M)
            res %= M;
        value_ = res;
        return *this;
    }

    [[nodiscard]] constexpr friend ZMod operator*(ZMod left, const ZMod &right)
    {
        left *= right;
        return left;
    }

    constexpr ZMod operator-() const { return {true, value_ == 0 ? 0 : M - value_}; }

    /// Returns the modular exponentiation of this number.
    template <std::integral E> [[nodiscard]] constexpr ZMod pow(E exponent) const
    {
        if (exponent == 0 || value_ == 1)
            return 1;
        if (value_ == M - 1)
            return exponent % 2 == 0 ? 1 : -1;
        ZMod x = 1;
        ZMod y = *this;
        while (true)
        {
            if (exponent & 1)
            {
                x *= y;
                if (exponent == 1)
                    break;
            }
            exponent >>= 1;
            y *= y;
        }
        return x;
    }

    [[nodiscard]] constexpr ZMod inverse() const
    {
        if (value_ == 0)
            return 0;
        if (value_ == 1)
            return 1;
        if (value_ == M - 1)
            return M - 1;
        if constexpr (M % 2 != 0)
        {
            if (value_ == 2)
                return {true, (M + 1) / 2};
            if (value_ == 4)
                return {true, M % 4 == 1 ? M - (M - 1) / 4 : (M + 1) / 4};
        }
        if constexpr (M % 3 != 0)
            if (value_ == 3)
                return {true, M % 3 == 1 ? M - (M - 1) / 3 : (M + 1) / 3};
        if constexpr (M % 2 != 0 && M % 3 != 0)
            if (value_ == 6)
                return {true, M % 6 == 1 ? M - (M - 1) / 6 : (M + 1) / 6};
        // 998244353 is prime.
        return pow(M - 2);
    }

    constexpr ZMod operator~() const { return inverse(); }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &o, const ZMod &x)
    {
        return o << x.value_;
    }

    [[nodiscard]] constexpr value_type value() const { return value_; }
};

struct Barrett64
{
    u64 m;
    u64 k;

    Barrett64(u64 mod) : m(mod), k(UINT64_MAX / mod) {}

    [[nodiscard]] u64 reduce(u64 x) const
    {
        u64 const q = ((__uint128_t)x * k) >> 64;
        u64 const r = x - q * m;
        return r >= m ? r - m : r;
    }

    friend u64 operator/(u64 lhs, const Barrett64 &rhs)
    {
        u64 const q = ((__uint128_t)lhs * rhs.k) >> 64;
        u64 const r = lhs - q * rhs.m;
        return r >= rhs.m ? q + 1 : q;
    }
};

using Z = ZMod<998'244'353U>;

template <typename T> class DV
{
  public:
    using value_type = T;

  private:
    u64 n_ = 0, u_ = 0, d_ = 0;
    std::vector<value_type> data_;

  public:
    DV() = default;
    constexpr DV(u64 N) : n_(N), u_(isqrt(N)), d_(N / (u_ + 1)), data_(u_ + d_ + 1) {}

    /// The value of `N`.
    [[nodiscard]] constexpr u64 n() const { return n_; }
    /// The number of up elements.
    [[nodiscard]] constexpr u64 u() const { return u_; }
    /// The number of down elements.
    [[nodiscard]] constexpr u64 d() const { return d_; }
    /// Size of the data. Equals `u + d + 1`.
    [[nodiscard]] constexpr size_t size() const noexcept { return data_.size(); }
    /// The index of the element with key `k`.
    [[nodiscard]] constexpr size_t index(u64 k) const noexcept { return k <= u() ? k : size() - n_ / k; }

    [[nodiscard]] constexpr std::span<value_type> up() noexcept { return {data_.begin(), u() + 1}; }
    [[nodiscard]] constexpr std::span<const value_type> up() const noexcept { return {data_.begin(), u() + 1}; }
    [[nodiscard]] constexpr value_type &up(u64 k) noexcept { return data_[k]; }
    [[nodiscard]] constexpr const value_type &up(u64 k) const noexcept { return data_[k]; }

    [[nodiscard]] constexpr std::span<value_type> down() noexcept { return {data_.begin() + u() + 1, d()}; }
    [[nodiscard]] constexpr std::span<const value_type> down() const noexcept { return {data_.begin() + u() + 1, d()}; }
    [[nodiscard]] constexpr value_type &down(u64 i) noexcept { return data_[data_.size() - i]; }
    [[nodiscard]] constexpr const value_type &down(u64 i) const noexcept { return data_[data_.size() - i]; }

    /// Returns the data as one contiguous span.
    [[nodiscard]] constexpr std::span<value_type> data() noexcept { return data_; }
    /// Returns the data as one contiguous span.
    [[nodiscard]] constexpr std::span<const value_type> data() const noexcept { return data_; }
    [[nodiscard]] constexpr auto begin() noexcept { return data_.begin() + 1; }
    [[nodiscard]] constexpr auto begin() const noexcept { return data_.begin() + 1; }
    [[nodiscard]] constexpr auto end() noexcept { return data_.end(); }
    [[nodiscard]] constexpr auto end() const noexcept { return data_.end(); }

    [[nodiscard]] constexpr value_type &operator[](u64 k) { return data_[index(k)]; }
    [[nodiscard]] constexpr const value_type &operator[](u64 k) const { return data_[index(k)]; }

    /// Returns a single value. Requires `k ≤ √N`.
    [[nodiscard]] constexpr T value(u64 k) const noexcept { return up(k) - up(k - 1); }

    constexpr void accumulate(u64 limit = std::numeric_limits<u64>::max())
    {
        limit = std::min(n(), limit);
        auto const end_it = (limit == n()) ? data_.end() : (data_.begin() + index(limit + 1));
        std::partial_sum(data_.begin() + 1, end_it, data_.begin() + 1);
    }

    constexpr void diff(u64 limit = std::numeric_limits<u64>::max())
    {
        limit = std::min(n(), limit);
        auto const end_it = (limit == n()) ? data_.end() : (data_.begin() + index(limit + 1));
        std::adjacent_difference(data_.begin() + 1, end_it, data_.begin() + 1);
    }
};

/// Dirichlet division of flat arrays.
template <std::ranges::random_access_range F, typename G> void dirichletDivideArray(F &&f, G &&g)
{
    using T = std::ranges::range_value_t<F>;
    if (f.size() <= 1)
        return;
    auto const get_g = [&](size_t i) {
        if constexpr (std::invocable<G, size_t>)
            return g(i);
        else
            return g[i];
    };
    size_t const n = f.size() - 1;
    size_t const B = std::max(1UZ, (1UZ << 17) / sizeof(T));
    auto const g1 = get_g(1);
    auto const inv_g1 = ~g1;
    if (g1 != 1)
        f[1] *= inv_g1;
    for (size_t start = 2; start <= n; start <<= 1)
    {
        size_t const end = std::min(n, (start << 1) - 1);
        size_t const s = isqrt(end);
        for (size_t i = isqrt(start - 1) + 1; i <= s; ++i)
            f[i * i] -= f[i] * get_g(i);
        for (size_t min_i = start; min_i <= end; min_i += B)
        {
            size_t const max_i = std::min(end, min_i + B - 1);
            for (size_t i = min_i; i <= max_i; ++i)
                f[i] -= f[1] * get_g(i);
            for (size_t i = 2; i <= s; ++i)
            {
                T const a = T(f[i]), b = T(get_g(i));
                for (size_t j = std::max(i + 1, (min_i + i - 1) / i), k = i * j; k <= max_i; ++j, k += i)
                    f[k] -= a * get_g(j) + b * f[j];
            }
            if (g1 != 1)
                for (size_t i = min_i; i <= max_i; ++i)
                    f[i] *= inv_g1;
        }
    }
}

/// In-place convolution. O(N^(2/3)) time without any log factors. Does not support aliasing between F and G.
/// In the below comments, x, y, and z always obey the inequality x ≤ y ≤ z.
inline void divInPlace(DV<Z> &F, auto &&G)
{
    u64 const N = F.n(), u = F.u(), d = F.d(), c_N = icbrt(N);
    auto const f = [&](u64 k) { return F.value(k); };
    auto const g = [&](u64 k) { return G.value(k); };

    Z const g1(g(1));
    Z const inv_g1 = ~g1;

    if (N == 1)
    {
        F.up(1) *= inv_g1;
        return;
    }

    u64 const M = N / c_N;
    F.diff(M);
    // Basic value-space convolution. O(√N log N).
    dirichletDivideArray(F.up(), g);
    // Prefix sum only what we need, as we are still in value space.
    F.accumulate(u);
    // Writes to F(N/z), reads from up.
    for (u64 x = 2; x <= c_N; ++x)
    {
        u64 const max_y = isqrt(N / x);
        F.down(max_y) += f(x) * G.up(max_y) + g(x) * F.up(max_y);
        if (x * x > u)
            F.down(N / (x * x)) -= f(x) * g(x);
        for (u64 y = max(x, u / x) + 1; y <= max_y; ++y)
            F.down(N / x / y) -= f(x) * g(y) + g(x) * f(y);
    }
    F.down(c_N) -= F.up(c_N) * G.up(c_N);
    // Boundary correction since F had data in up when we diff'd.
    F.down(d) += f(1) * G.up(u) + (g1 - 1) * F.up(u);
    // Prefix sum the rest.
    partial_sum(F.data().begin() + u, F.data().begin() + F.index(M + 1), F.data().begin() + u);

    // Writes to F(N/x) and F(N/y), reads from up.
    for (u64 x = 2; x <= c_N; ++x)
    {
        u64 const min_y = max(x, d / x) + 1;
        u64 const max_y = isqrt(N / x);
        for (u64 y = min_y; y <= max_y; ++y)
        {
            Z const F_z = F.up(N / x / y), G_z = G.up(N / x / y);
            F.down(x) -= f(y) * G_z + g(y) * F_z;
            F.down(y) -= f(x) * G_z + g(x) * F_z;
        }
        F.down(x) += F.up(max_y) * G.up(max_y);
    }

    // Writes to F(N/x) and F(N/y), reads from down (except squares). Wavefront required. O(√N log N).
    for (u64 stop_i = d; stop_i >= 2; stop_i >>= 1)
    {
        u64 const start_i = (stop_i >> 1) + 1;
        for (u64 x = 2; x <= c_N; ++x)
        {
            u64 const max_y = min(stop_i, d / x);
            for (u64 y = max(x + 1, start_i); y <= max_y; ++y)
                F.down(y) -= f(x) * G.down(x * y) + g(x) * F.down(x * y);
        }
        for (u64 x = start_i; x <= stop_i; ++x)
        {
            if (x <= c_N)
            {
                F.down(x) -= f(x) * G[N / (x * x)] + g(x) * F[N / (x * x)];
                for (u64 y = x + 1; x * y <= d; ++y)
                    F.down(x) -= f(y) * G.down(x * y) + g(y) * F.down(x * y);
            }
            F.down(x) -= f(1) * G.down(x);
            if (g1 != 1)
                F.down(x) *= inv_g1;
        }
    }

    // Set F(1).
    F.down(1) -= f(1) * G.down(1) - F.up(u) * G.up(u);
    for (u64 y = 2; y <= u; ++y)
        F.down(1) -= f(y) * G.down(y) + g(y) * F.down(y);
    if (g1 != 1)
        F.down(1) *= inv_g1;
}

int main()
{
    int t;
    rd(t);
    while (t--)
    {
        u64 N;
        rd(N);
        u64 const s_N = isqrt(N), d_N = N / (s_N + 1);
        DV<Z> F(N), G(N);
        for (u64 k = 1; k <= s_N; ++k)
            F.up(k) = 1;
        for (u64 i = d_N; i >= 1; --i)
            F.down(i) = 1;
        for (u64 k = 1; k <= s_N; ++k)
        {
            u32 x;
            rd(x);
            G.up(k) = Z(true, x);
        }
        for (u64 i = d_N; i >= 1; --i)
        {
            u32 x;
            rd(x);
            G.down(i) = Z(true, x);
        }
        divInPlace(F, G);
        for (u64 k = 1; k <= s_N; ++k, wt(' '))
            wt(F.up(k).value());
        for (u64 i = d_N; i >= 1; --i, wt(' '))
            wt(F.down(i).value());
        wt('\n');
    }
}
