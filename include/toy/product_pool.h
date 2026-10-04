#pragma once
#include <toy/polynomial.h>
namespace toy {
// Output is disjoint from inputs. A single pair of FFT buffers serves the tree.
template <u32 P = 998244353>
struct ProductWorkspace {
    using M = Mod<P>;
    Buffer<u32> left, right;
    void multiply(std::span<const u32> a, std::span<const u32> b, u32 *out) {
        if (a.size() < b.size()) std::swap(a, b);
        usize count = a.size() + b.size() - 1;
        if (b.size() <= 16) {
            for (usize k = 0; k < count; ++k) {
                u64 sum = 0;
                for (usize j = k < a.size() ? 0 : k - a.size() + 1; j < std::min(b.size(), k + 1);
                     ++j)
                    sum += u64(a[k - j]) * b[j];
                out[k] = sum % P;
            }
            return;
        }
        bool cyclic = count > 64 && std::has_single_bit(count - 1);
        usize size = cyclic ? count - 1 : std::bit_ceil(count);
        if (left.capacity < size) left = Buffer<u32>(size), right = Buffer<u32>(size);
        memcpy(left.p, a.data(), a.size() * 4);
        std::fill(left.p + a.size(), left.p + size, 0u);
        memcpy(right.p, b.data(), b.size() * 4);
        std::fill(right.p + b.size(), right.p + size, 0u);
        const auto &ntt = convolution_detail::info<P>;
        auto *x = (convolution_detail::Vec *)left.p;
        auto *y = (convolution_detail::Vec *)right.p;
        ntt.forward(x, size / 8);
        ntt.forward(y, size / 8);
        ntt.products(x, y, size / 8);
        ntt.inverse(x, size / 8);
        memcpy(out, left.p, std::min(count, size) * 4);
        if (cyclic) {
            u32 top = M::mul(a.back(), b.back());
            out[0] = M::sub(out[0], top);
            out[count - 1] = top;
        }
    }
};
template <u32 P = 998244353>
Buffer<u32> polynomial_product_pool(std::span<const std::span<const u32>> values) {
    if (values.empty()) {
        Buffer<u32> one(1);
        one[0] = 1;
        return one;
    }
    Buffer<usize> prefix(values.size() + 1);
    prefix[0] = 0;
    for (usize i = 0; i < values.size(); ++i) prefix[i + 1] = prefix[i] + values[i].size() - 1;
    auto middle = [&](usize l, usize r) {
        usize target = prefix[l] + (prefix[r] - prefix[l]) / 2;
        usize m = std::min(
            r - 1, usize(std::lower_bound(prefix.p + l + 1, prefix.p + r, target) - prefix.p));
        auto distance = [&](usize i) {
            usize a = prefix[i] - prefix[l], b = prefix[r] - prefix[i];
            return std::max(a, b) - std::min(a, b);
        };
        if (m > l + 1 && distance(m - 1) < distance(m)) --m;
        return m;
    };
    auto needed = [&](auto &&self, usize l, usize r) -> usize {
        if (r - l == 1) return 0;
        usize m = middle(l, r), n = prefix[r] - prefix[l] + 1;
        return n + std::max(self(self, l, m),
                            (m - l > 1 ? prefix[m] - prefix[l] + 1 : 0) + self(self, m, r));
    };
    Buffer<u32> pool(needed(needed, 0, values.size()));
    usize used = 0;
    ProductWorkspace<P> workspace;
    auto solve = [&](auto &&self, usize l, usize r) -> std::span<const u32> {
        if (r - l == 1) return values[l];
        usize m = middle(l, r), n = prefix[r] - prefix[l] + 1, mark = used;
        u32 *out = pool.p + used;
        used += n;
        auto a = self(self, l, m), b = self(self, m, r);
        workspace.multiply(a, b, out);
        used = mark + n;
        return {out, n};
    };
    auto value = solve(solve, 0, values.size());
    Buffer<u32> result(value.size());
    memcpy(result.p, value.data(), value.size() * 4);
    return result;
}
} // namespace toy
