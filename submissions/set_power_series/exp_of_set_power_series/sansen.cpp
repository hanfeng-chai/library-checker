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

#include <atcoder/modint>

using mint = atcoder::modint998244353;

template <class T>
std::vector<T> conv(const std::vector<T> &a, const std::vector<T> &b)
{
    const std::size_t len = a.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1 << n));
    assert(len == b.size());
    std::vector<T> x((n + 1) << n), y((n + 1) << n);
    for (std::size_t i = 0; i < (1 << n); ++i)
    {
        const int cnt = std::popcount(i);
        x[i * (n + 1) + cnt] = a[i];
        y[i * (n + 1) + cnt] = b[i];
    }
    for (std::size_t i = 0; i < (1 << n); ++i)
    {
        const std::size_t s = (n + 1) * i;
        const std::size_t t = (n + 1) * (i + 1);
        const int z = std::countr_zero(i | (1 << n));
        for (int j = z - 1; j >= 0; --j)
        {
            const int w = (n + 1) << j;
            for (int k = 0; k < w; ++k)
            {
                x[s + w + k] += x[s + k];
                y[s + w + k] += y[s + k];
            }
        }
        const int cnt = std::popcount(i);
        for (int j = std::min(n, 2 * cnt); j >= cnt; --j)
        {
            T v = 0;
            for (int k = j - cnt; k <= cnt; ++k)
            {
                v += x[s + k] * y[s + j - k];
            }
            x[s + j] = v;
        }
        const int o = std::countr_one(i);
        for (int j = 0; j < o; ++j)
        {
            const int w = (n + 1) << j;
            const int s = t - 2 * w;
            for (int k = 0; k < w; ++k)
            {
                x[s + w + k] -= x[s + k];
            }
        }
    }
    std::vector<T> c(1 << n);
    for (std::size_t i = 0; i < (1 << n); ++i)
    {
        const int cnt = std::popcount(i);
        c[i] = x[i * (n + 1) + cnt];
    }
    return c;
}

template<class T>
std::vector<T> exp(const std::vector<T> &a)
{
    const std::size_t len = a.size();
    const int n = std::countr_zero(len);
    assert(len > 0 && len == (1 << n));
    assert(a[0] == 0);
    std::vector<T> res(len);
    res[0] = 1;
    for (int i = 0; i < n; ++i)
    {
        const std::vector<T> x(res.begin(), res.begin() + (1 << i));
        const std::vector<T> y(a.begin() + (1 << i), a.begin() + (2 << i));
        const auto c = conv(x, y);
        std::copy(c.begin(), c.end(), res.begin() + (1 << i));
    }
    return res;
}

int main(void)
{
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    std::vector<mint> a(1 << n);
    for (auto &x : a) {
        int v;
        std::cin >> v;
        x = v;
    }
    auto ans = exp(a);
    for (int i = 0; i < (1 << n); ++i) {
        std::cout << ans[i].val() << " \n"[i == (1 << n) - 1];
    }
    return 0;
}
