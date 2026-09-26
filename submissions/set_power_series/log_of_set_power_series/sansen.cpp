#pragma GCC optimize ("Ofast")
#pragma GCC optimize ("unroll-loops")

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>
#include <bit>

namespace mylib {

constexpr uint32_t calc_rem_expr(uint32_t m) {
    uint32_t t = 1;
    uint32_t s = ~m + 1;
    uint32_t n = ~(uint32_t)0 >> 2;
    while (n) {
        if (n & 1) t *= s;
        s *= s;
        n >>= 1;
    }
    return t;
}

template <uint32_t M>
class modint {
public:
    constexpr static uint32_t mod() { return M; }
    modint() : v_(0) {}
    template <class T>
    requires std::unsigned_integral<T>
    constexpr modint(T v) {
        v_ = raw(v % mod());
    }
    template <class T>
    requires std::signed_integral<T>
    constexpr modint(T v) {
        int64_t u = v % mod();
        if (u < 0) u += mod();
        v_ = raw(static_cast<uint32_t>(u));
    }
    constexpr uint32_t val() const {
        const uint32_t res = reduce(v_);
        return res >= mod() ? res - mod() : res;
    }
    constexpr modint &operator+=(const modint &rhs) {
        v_ += rhs.v_;
        v_ -= v_ >= 2 * mod() ? 2 * mod() : 0;
        return *this;
    }
    constexpr modint &operator-=(const modint &rhs) {
        v_ = v_ - rhs.v_ + (v_ < rhs.v_ ? 2 * mod() : 0);
        return *this;
    }
    constexpr modint &operator*=(const modint &rhs) {
        v_ = reduce((uint64_t)v_ * rhs.v_);
        return *this;
    }
    constexpr modint &operator/=(const modint &rhs) { return *this *= rhs.inv(); }
    template <class T>
    requires std::integral<T>
    constexpr modint pow(T n) const {
        assert(n >= 0);
        modint t = 1;
        modint r = *this;
        while (n) {
            if (n & 1) t *= r;
            r *= r;
            n >>= 1;
        }
        return t;
    }
    constexpr modint inv() const {
        assert(v_ > 0);
        return pow(mod() - 2);
    }
    friend constexpr modint operator+(const modint &lhs, const modint &rhs) {
        return modint(lhs) += rhs;
    }
    friend constexpr modint operator-(const modint &lhs, const modint &rhs) {
        return modint(lhs) -= rhs;
    }
    friend constexpr modint operator*(const modint &lhs, const modint &rhs) {
        return modint(lhs) *= rhs;
    }
    friend constexpr modint operator/(const modint &lhs, const modint &rhs) {
        return modint(lhs) /= rhs;
    }
    constexpr modint operator+() const {
        return *this;
    }
    constexpr modint operator-() const {
        modint res;
        res.v_ = v_ == 0 ? 0 : 2 * mod() - v_;
        return res;
    }
    constexpr bool operator==(const modint &rhs) const {
        return (v_ >= mod() ? v_ - mod() : v_) == (rhs.v_ >= mod() ? rhs.v_ - mod() : rhs.v_);
    }
private:
    uint32_t v_;
    constexpr static uint32_t rem = calc_rem_expr(M);
    constexpr static uint64_t ini = ((((uint64_t)1 << 32) % mod()) << 32) % mod();
    constexpr static uint32_t reduce(const uint64_t x) {
        return (x + (uint32_t)x * rem * (uint64_t)mod()) >> 32;
    }
    constexpr static uint32_t raw(uint32_t v) { return reduce(v * ini); }
    static_assert(mod() < (1 << 30));
    static_assert((mod() & 1) == 1);
};
}    // namespace mylib

namespace sps {

template <class T>
std::vector<T> conv(const std::vector<T> &a, const std::vector<T> &b) {
    const std::size_t len = a.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1ul << n));
    assert(len == b.size());
    std::vector<T> x((n + 1) << n), y((n + 1) << n);
    for (std::size_t i = 0; i < (1ul << n); ++i) {
        const int cnt = std::popcount(i);
        x[i * (n + 1) + cnt] = a[i];
        y[i * (n + 1) + cnt] = b[i];
    }
    for (std::size_t i = 0; i < (1ul << n); ++i) {
        const std::size_t s = (n + 1) * i;
        const std::size_t t = (n + 1) * (i + 1);
        const int z = std::countr_zero(i | (1 << n));
        for (int j = z - 1; j >= 0; --j) {
            const int w = (n + 1) << j;
            for (int k = 0; k < w; ++k) {
                x[s + w + k] += x[s + k];
                y[s + w + k] += y[s + k];
            }
        }
        const int cnt = std::popcount(i);
        for (int j = std::min(n, 2 * cnt); j >= cnt; --j) {
            T v = 0;
            for (int k = j - cnt; k <= cnt; ++k) {
                v += x[s + k] * y[s + j - k];
            }
            x[s + j] = v;
        }
        const int o = std::countr_one(i);
        for (int j = 0; j < o; ++j) {
            const int w = (n + 1) << j;
            const int s = t - 2 * w;
            for (int k = 0; k < w; ++k) {
                x[s + w + k] -= x[s + k];
            }
        }
    }
    std::vector<T> c(1 << n);
    for (std::size_t i = 0; i < (1ul << n); ++i) {
        const int cnt = std::popcount(i);
        c[i] = x[i * (n + 1) + cnt];
    }
    return c;
}

// a * b = c なるbを返す
template <class T>
std::vector<T> conv_inv(const std::vector<T> &a, const std::vector<T> &c) {
    const std::size_t len = a.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1ul << n));
    assert(len == c.size());
    assert(a[0] == 1);
    std::vector<T> x((n + 1) << n), y((n + 1) << n);
    for (std::size_t i = 0; i < (1ul << n); ++i) {
        const int cnt = std::popcount(i);
        x[i * (n + 1) + cnt] = a[i];
        y[i * (n + 1) + cnt] = c[i];
    }
    for (std::size_t i = 0; i < (1ul << n); ++i) {
        const std::size_t s = (n + 1) * i;
        const std::size_t t = (n + 1) * (i + 1);
        const int z = std::countr_zero(i | (1 << n));
        for (int j = z - 1; j >= 0; --j) {
            const int w = (n + 1) << j;
            for (int k = 0; k < w; ++k) {
                x[s + w + k] += x[s + k];
                y[s + w + k] += y[s + k];
            }
        }
        const int cnt = std::popcount(i);
        for (int j = 0; j <= n; ++j) {
            const auto v = y[s + j];
            for (int k = 1; k <= std::min(n - j, cnt); ++k) {
                y[s + j + k] -= v * x[s + k];
            }
        }
        const int o = std::countr_one(i);
        for (int j = 0; j < o; ++j) {
            const int w = (n + 1) << j;
            const int s = t - 2 * w;
            for (int k = 0; k < w; ++k) {
                y[s + w + k] -= y[s + k];
            }
        }
    }
    std::vector<T> b(1 << n);
    for (std::size_t i = 0; i < (1ul << n); ++i) {
        const int cnt = std::popcount(i);
        b[i] = y[i * (n + 1) + cnt];
    }
    return b;
}

template <class T>
std::vector<T> exp(const std::vector<T> &a) {
    const std::size_t len = a.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1ul << n));
    assert(a[0] == 0);
    std::vector<T> res(len);
    res[0] = 1;
    for (int i = 0; i < n; ++i) {
        const std::vector<T> x(res.begin(), res.begin() + (1 << i));
        const std::vector<T> y(a.begin() + (1 << i), a.begin() + (2 << i));
        const auto c = conv(x, y);
        std::copy(c.begin(), c.end(), res.begin() + (1 << i));
    }
    return res;
}

template <class T>
std::vector<T> log(const std::vector<T> &a) {
    const std::size_t len = a.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1ul << n));
    assert(a[0] == 1);
    std::vector<T> res(len);
    res[0] = 0;
    for (int i = 0; i < n; ++i) {
        const std::vector<T> x(a.begin(), a.begin() + (1 << i));
        const std::vector<T> y(a.begin() + (1 << i), a.begin() + (2 << i));
        const auto c = conv_inv(x, y);
        std::copy(c.begin(), c.end(), res.begin() + (1 << i));
    }
    return res;
}

// f: 多項式
// g: sps
template<class T>
std::vector<T> composite(std::vector<T> f, const std::vector<T> &g) {
    const std::size_t len = g.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1ul << n));
    std::vector<std::vector<T>> dp(n + 1);
    for (auto &d : dp) {
        const auto r = g[0];
        T val = 0;
        for (std::size_t i = f.size(); i > 0; --i) {
            val = val * r + f[i - 1];
        }
        d = {val};
        for (int i = 1; i < (int)f.size(); ++i) {
            f[i - 1] = i * f[i];
        }
        const auto s = f.size();
        if (s > 0) {
            f.resize(s - 1);
        }
    }
    for (int i = 0; i < n; ++i) {
        const std::vector<T> a = {g.begin() + (1 << i), g.begin() + (2 << i)};
        for (std::size_t j = 1; j < dp.size(); ++j) {
            const auto h = conv(a, dp[j]);
            std::copy(h.begin(), h.end(), std::back_inserter(dp[j - 1]));
        }
        dp.resize(n - i);
    }
    return dp[0];
}

}    // namespace sps

template <class T, class F>
void bitwise_transform(std::vector<T> &a, F f) {
    const std::size_t len = a.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1ul << n));
    for (int i = 0; i < n; ++i) {
        const int w = 1 << i;
        for (int j = 0; j < (1 << n); j += 2 * w) {
            for (int k = 0; k < w; ++k) {
                f(a[j + k], a[j + k + w]);
            }
        }
    }
}

using mint = mylib::modint<998244353>;

int main(void)
{
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    std::vector<mint> a(1 << n), b(1 << n);
    for (auto &x : a) {
        int v;
        std::cin >> v;
        x = v;
    }
    auto ans = sps::log(a);
    for (int i = 0; i < (1 << n); ++i) {
        std::cout << ans[i].val() << " \n"[i == (1 << n) - 1];
    }
    return 0;
}
