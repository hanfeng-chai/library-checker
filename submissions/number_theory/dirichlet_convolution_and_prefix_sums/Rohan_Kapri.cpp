#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
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

u64 icbrt(u64 x)
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

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &o, const ZMod &x)
    {
        return o << x.value_;
    }

    [[nodiscard]] constexpr value_type value() const { return value_; }
};

using Z = ZMod<998'244'353U>;

template <typename T> struct DV
{
    using value_type = T;

    u64 N, s_N, d_N;
    vector<value_type> up, down;

    DV(u64 N) : N(N), s_N(isqrt(N)), d_N(N / (s_N + 1)), up(s_N + 1), down(d_N + 1) {}

    constexpr T &operator[](size_t k) { return k <= s_N ? up[k] : down[N / k]; }
    constexpr const T &operator[](size_t k) const { return k <= s_N ? up[k] : down[N / k]; }

    /// Takes a partial sum in place.
    constexpr DV &accumulate()
    {
        for (size_t i = 2; i <= s_N; ++i)
            up[i] += up[i - 1];
        down.back() += up.back();
        for (size_t i = d_N; i-- > 1;)
            down[i] += down[i + 1];
        return *this;
    }
};

// Rectangle method. O(N^(2/3)) time without any log factors.
inline DV<Z> convolve(const DV<Z> &F, const DV<Z> &G)
{
    auto const f = [&](u64 k) { return F.up[k] - F.up[k - 1]; };
    auto const g = [&](u64 k) { return G.up[k] - G.up[k - 1]; };

    u64 const N = F.N, s_N = isqrt(N), c_N = icbrt(N), d_N = N / (s_N + 1);
    u64 const B = (1 << 16) / sizeof(Z); // Guess this is L1 cache
    u64 const num_segs_s = (s_N + B - 1) / B;
    u64 const num_segs_d = (d_N + B - 1) / B;
    DV<Z> res(N);
    // Phase 1.0: Diagonal (negligible runtime).
    for (u64 x = 1; x <= c_N; ++x)
    {
        Z const val_diag = f(x) * g(x);
        res[x * x] += val_diag;
        res.down[x - 1] -= val_diag;
    }
    // Phase 1.1: x*y <= s_N. O(√N log N). Tile by x*y.
    for (u64 s = 0; s < num_segs_s; ++s)
    {
        u64 const min_xy = s * B + 1, max_xy = min(min_xy + B - 1, s_N);
        u64 const max_x = min(c_N, isqrt(max_xy));
        for (u64 x = 1; x <= max_x; ++x)
        {
            Z const f_x = f(x), g_x = g(x);
            for (u64 y = max(x + 1, (min_xy + x - 1) / x); x * y <= max_xy; ++y)
            {
                Z const val = f_x * g(y) + g_x * f(y);
                res.up[x * y] += val; // Random write to h!
                res.down[y - 1] -= val;
            }
        }
    }
    // Phase 1.2: x*y > s_N => N/(x*y) <= d_N. Tile by z = N/(x*y). y ascending -> z descending.
    // O(N^(2/3) + N^(3/4)/B). As L1 cache >> N^(1/12), the second term never sees the light of day.
    for (u64 s = 0; s < num_segs_d; ++s)
    {
        u64 const start_z = s * B + 1, end_z = min(start_z + B, d_N + 1);
        u64 const max_x = min(c_N, isqrt(N / start_z));
        for (u64 x = 1; x <= max_x; ++x)
        {
            u64 const N_x = N / x;
            u64 const min_y = max(x, N_x / end_z) + 1;
            u64 const max_y = min(isqrt(N / x), N_x / start_z);
            if (min_y > max_y)
                continue;
            Z const f_x = f(x), g_x = g(x);
            for (u64 y = min_y; y <= max_y; ++y)
            {
                Z const val = f_x * g(y) + g_x * f(y);
                res.down[N_x / y] += val; // Random write to h!
                res.down[y - 1] -= val;
            }
        }
    }
    res.accumulate();
    // Phase 2.0: Diagonal (negligible runtime).
    for (u64 x = 1; x <= c_N; ++x)
    {
        Z const f_xx = x * x <= d_N ? F.down[x * x] : F.up[N / (x * x)];
        Z const g_xx = x * x <= d_N ? G.down[x * x] : G.up[N / (x * x)];
        res.down[x] += f(x) * (g_xx - G.up[x]) + g(x) * (f_xx - F.up[x]);
    }

    // Phase 2.1: x*y <= d_N => N/(x*y) > s_N (so we read down(x*y)). O(√N log N). Tile by x*y.
    for (u64 s = 0; s < num_segs_d; ++s)
    {
        u64 const min_xy = s * B + 1, max_xy = min(min_xy + B - 1, d_N);
        u64 const max_x = min(c_N, isqrt(max_xy));
        for (u64 x = 1; x <= max_x; ++x)
        {
            auto const f_x = f(x), g_x = g(x);
            Z hx_acc = res.down[x];
            for (u64 y = max(x + 1, (min_xy + x - 1) / x); x * y <= max_xy; ++y)
            {
                u64 const xy = x * y;
                Z const g_xy = G.down[xy], f_xy = F.down[xy];
                hx_acc += f(y) * (g_xy - G.up[y - 1]) + g(y) * (f_xy - F.up[y]); // Random reads of f and g!
                res.down[y] += f_x * (g_xy - G.up[y]) + g_x * (f_xy - F.up[y]);  // Random reads of f and g!
            }
            res.down[x] = hx_acc;
        }
    }
    // Phase 2.2: x*y > d_N (so we read up(N/(x*y))). Tile by z = N/(x*y). z ascending -> y descending.
    // O(N^(2/3) + N^(3/4)/B). As L1 cache >> N^(1/12), the second term never sees the light of day.
    // This is hard to optimize because the innermost loop reads from up to 7 different places in memory.
    for (u64 s = 0; s < num_segs_s; ++s)
    {
        u64 const start_z = s * B + 1, end_z = min(start_z + B, s_N + 1);
        u64 const max_x = min(c_N, isqrt(N / start_z));
        for (u64 x = 1; x <= max_x; ++x)
        {
            u64 const N_x = N / x;
            u64 const min_y = max(x, N_x / end_z) + 1;
            u64 const max_y = min(isqrt(N / x), N_x / start_z);
            if (min_y > max_y)
                continue;
            Z const f_x = f(x), g_x = g(x);
            Z hx_acc = 0;
            for (u64 y = max_y; y >= min_y; --y)
            {
                u64 const N_xy = N_x / y;
                Z const f_xy = F.up[N_xy], g_xy = G.up[N_xy];
                hx_acc += f(y) * (g_xy - G.up[y - 1]) + g(y) * (f_xy - F.up[y]);
                res.down[y] += f_x * (g_xy - G.up[y]) + g_x * (f_xy - F.up[y]);
            }
            res.down[x] += hx_acc;
        }
    }
    return res;
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
        DV<Z> f(N), g(N);
        for (u64 k = 1; k <= s_N; k++)
        {
            u32 x;
            rd(x);
            f.up[k] = x;
        }
        for (u64 i = d_N; i >= 1; --i)
        {
            u32 x;
            rd(x);
            f.down[i] = x;
        }
        for (u64 k = 1; k <= s_N; k++)
        {
            u32 x;
            rd(x);
            g.up[k] = x;
        }
        for (u64 i = d_N; i >= 1; --i)
        {
            u32 x;
            rd(x);
            g.down[i] = x;
        }
        auto const H = convolve(f, g);
        for (u64 k = 1; k <= s_N; ++k, wt(' '))
            wt(H.up[k].value());
        for (u64 i = d_N; i >= 1; --i, wt(' '))
            wt(H.down[i].value());
        wt('\n');
    }
}
