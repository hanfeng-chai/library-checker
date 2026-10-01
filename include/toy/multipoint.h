#pragma once
#include <toy/array.h>
#include <toy/polynomial.h>

namespace toy {

template<u32 P = 998244353>
Buffer<u32> inverse_values(std::span<const u32> values) {
    using M = Mod<P>;
    usize n = (values.size() + 7) & -usize(8);
    Buffer<u32> result(values.size(), n), prefix(n);
    if (!n) return result;
    constexpr u32 one = (u64(1) << 32) % P;
    auto cur = _mm256_set1_epi32(one), r2 = _mm256_set1_epi32(M::r2);
    alignas(32) u32 tail[8]; std::fill(tail, tail + 8, 1u);
    if (values.size() & 7) memcpy(tail, values.data() + (values.size() & -usize(8)), (values.size() & 7) * 4);
    auto load = [&](usize i) {
        auto x = _mm256_loadu_si256((const __m256i*)(i + 8 <= values.size() ? values.data() + i : tail));
        return M::mont(x, r2);
    };
    for (usize i = 0; i < n; i += 8) {
        cur = M::mont(cur, load(i)); _mm256_store_si256((__m256i*)(prefix.p + i), cur);
    }
    auto inv = _mm256_set1_epi32(one);
    for (u32 e = P - 2; e; e >>= 1, cur = M::mont(cur, cur)) if (e & 1) inv = M::mont(inv, cur);
    for (usize i = n; i;) {
        i -= 8;
        auto before = i ? _mm256_load_si256((const __m256i*)(prefix.p + i - 8)) : _mm256_set1_epi32(one);
        auto value = M::mont(M::mont(before, inv), _mm256_set1_epi32(1));
        _mm256_store_si256((__m256i*)(result.p + i), value); inv = M::mont(inv, load(i));
    }
    return result;
}

// Reversed product tree Q(z)=prod(1-x_i*z). Zero/repeated evaluation points
// are valid; interpolation additionally requires distinct points.
template<u32 P = 998244353>
struct Multipoint {
    using M = Mod<P>;
    static constexpr usize leaf = 16;
    struct Node { Buffer<u32> polynomial, left, right; };
    std::span<const u32> points;
    usize groups, length;
    Array<Node> tree;

    explicit Multipoint(std::span<const u32> x) : points(x), groups(std::bit_ceil(std::max<usize>(1, (x.size() + leaf - 1) / leaf))),
        length(groups * leaf), tree(2 * groups) {
        for (usize k = 0; k < groups; ++k) {
            auto& q = tree[groups + k].polynomial; q = Buffer<u32>(leaf + 1);
            std::fill(q.p, q.p + q.n, 0u); q[0] = 1;
            for (usize j = 0; j < leaf && k * leaf + j < points.size(); ++j) {
                u32 x = M::mont(points[k * leaf + j], M::r2);
                auto w = _mm256_set1_epi32(x); usize i = j + 1;
                for (; i >= 8; i -= 8) {
                    auto a = _mm256_loadu_si256((const __m256i*)(q.p + i - 7));
                    auto b = _mm256_loadu_si256((const __m256i*)(q.p + i - 8));
                    _mm256_storeu_si256((__m256i*)(q.p + i - 7), M::sub(a, M::mont(b, w)));
                }
                for (; i; --i) q[i] = M::sub(q[i], M::mont(x, q[i - 1]));
            }
        }
        for (usize k = groups; --k;) {
            auto& node = tree[k]; auto& a = tree[2 * k].polynomial; auto& b = tree[2 * k + 1].polynomial;
            usize size = a.n + b.n - 2;
            node.polynomial = Buffer<u32>(size + 1);
            if (size < 64) {
                for (usize k = 0; k <= size; ++k) {
                    usize first = k < a.n ? 0 : k - a.n + 1;
                    u64 sum = M::mul(a[k - first], b[first]);
                    for (usize j = first + 1; j < std::min(b.n, k + 1); ++j) sum += u64(a[k - j]) * b[j];
                    node.polynomial[k] = sum % P;
                }
                continue;
            }
            node.left = Buffer<u32>(size); node.right = Buffer<u32>(size);
            memcpy(node.left.p, a.p, a.n * 4); std::fill(node.left.p + a.n, node.left.p + size, 0u);
            memcpy(node.right.p, b.p, b.n * 4); std::fill(node.right.p + b.n, node.right.p + size, 0u);
            const auto& ntt = convolution_detail::info<P>;
            auto* l = (convolution_detail::Vec*)node.left.p;
            auto* r = (convolution_detail::Vec*)node.right.p;
            auto* out = (convolution_detail::Vec*)node.polynomial.p;
            ntt.forward(l, size / 8); ntt.forward(r, size / 8);
            memcpy(out, l, size * 4); ntt.products(out, r, size / 8); ntt.inverse(out, size / 8);
            // Degree is exactly the cyclic length; restore the wrapped top term.
            node.polynomial[0] = 1;
            node.polynomial[size] = M::mul(a[a.n - 1], b[b.n - 1]);
        }
    }

    Buffer<u32> evaluate(std::span<const u32> f) {
        Buffer<u32> answer(points.size());
        if (points.empty()) return answer;
        if (f.size() <= 16 || points.size() <= 16) {
            for (usize i = 0; i < points.size(); ++i) answer[i] = polynomial_eval<P>(f, points[i]);
            return answer;
        }
        Buffer<u32> reversed(f.size());
        std::reverse_copy(f.begin(), f.end(), reversed.p);
        Buffer<u32> weights;
        if (f.size() > length) weights = fps_div<P>(reversed, tree[1].polynomial, f.size());
        else {
            auto inv = fps_inv<P>(tree[1].polynomial, f.size());
            weights = convolution<P>(std::move(reversed), std::move(inv)); weights.n = f.size();
        }
        std::reverse(weights.p, weights.p + weights.n); weights.resize(length);
        auto visit = [&](auto&& self, usize node, usize start, Buffer<u32> u) -> void {
            usize size = u.n;
            if (start >= points.size()) return;
            if (size == leaf) {
                u32 remainder[leaf]; const auto& q = tree[node].polynomial;
                for (usize i = 0; i < leaf; ++i) {
                    u64 sum = 0;
                    for (usize j = 0; j < leaf - i; ++j) sum += u64(q[j]) * u[i + j];
                    remainder[i] = sum % P;
                }
                alignas(32) u32 point[leaf] = {}, value[leaf];
                usize used = std::min(leaf, points.size() - start);
                memcpy(point, points.data() + start, used * 4);
                // Eight Horner chains in parallel, one point per lane.
                for (usize i = 0; i < leaf; i += 8) {
                    auto x = M::mont(_mm256_load_si256((const __m256i*)(point + i)), _mm256_set1_epi32(M::r2));
                    auto v = _mm256_setzero_si256();
                    for (usize j = leaf; j--;) v = M::add(M::mont(v, x), _mm256_set1_epi32(remainder[j]));
                    _mm256_store_si256((__m256i*)(value + i), v);
                }
                memcpy(answer.p + start, value, used * 4);
                return;
            }
            usize half = size / 2;
            Buffer<u32> left(half), right(half);
            if (size < 64) {
                const auto& l = tree[2 * node].polynomial; const auto& r = tree[2 * node + 1].polynomial;
                for (usize i = 0; i < half; ++i) {
                    u64 a = u[i], b = u[i];
                    for (usize j = 1; j <= half; ++j) {
                        a += u64(r[j]) * u[i + j]; b += u64(l[j]) * u[i + j];
                    }
                    left[i] = a % P; right[i] = b % P;
                }
            } else {
                std::reverse(u.p, u.p + size); Buffer<u32> temp(size);
                const auto& ntt = convolution_detail::info<P>;
                auto* work = (convolution_detail::Vec*)temp.p;
                ntt.forward((convolution_detail::Vec*)u.p, size / 8);
                auto correlate = [&](Buffer<u32>& sibling, Buffer<u32>& child) {
                    memcpy(temp.p, u.p, size * 4);
                    ntt.products(work, (convolution_detail::Vec*)sibling.p, size / 8); ntt.inverse(work, size / 8);
                    for (usize i = 0; i < half; ++i) child[i] = temp[size - 1 - i];
                };
                correlate(tree[node].right, left); correlate(tree[node].left, right);
            }
            u = Buffer<u32>{};
            self(self, 2 * node, start, std::move(left));
            self(self, 2 * node + 1, start + half, std::move(right));
        };
        visit(visit, 1, 0, std::move(weights));
        return answer;
    }

    Buffer<u32> interpolate(std::span<const u32> values) {
        usize n = points.size();
        if (!n) return {};
        Buffer<u32> derivative(n);
        for (usize i = 0; i < n; ++i) derivative[i] = M::mul(i + 1, tree[1].polynomial[n - i - 1]);
        auto denominators = evaluate(derivative);
        auto weight = inverse_values<P>(denominators);
        for (usize i = 0; i < n; ++i) weight[i] = M::mul(weight[i], values[i]);
        auto merge = [&](auto&& self, usize node, usize start, usize size) -> Buffer<u32> {
            Buffer<u32> result(size < 64 ? size : 0);
            if (size == leaf) {
                alignas(32) u32 point[leaf] = {}, w[leaf] = {};
                usize used = start < n ? std::min(leaf, n - start) : 0;
                if (used) { memcpy(point, points.data() + start, used * 4); memcpy(w, weight.p + start, used * 4); }
                const auto& q = tree[node].polynomial;
                auto r2 = _mm256_set1_epi32(M::r2);
                auto x = M::mont(_mm256_load_si256((const __m256i*)point), r2);
                auto y = M::mont(_mm256_load_si256((const __m256i*)(point + 8)), r2);
                auto a = M::mont(_mm256_load_si256((const __m256i*)w), r2);
                auto b = M::mont(_mm256_load_si256((const __m256i*)(w + 8)), r2);
                auto u = _mm256_set1_epi32(1), v = u;
                for (usize j = 0; j < leaf; ++j) {
                    auto sum = M::add(M::mont(u, a), M::mont(v, b));
                    sum = M::add(sum, _mm256_permute2x128_si256(sum, sum, 1));
                    sum = M::add(sum, _mm256_shuffle_epi32(sum, _MM_SHUFFLE(1, 0, 3, 2)));
                    sum = M::add(sum, _mm256_shuffle_epi32(sum, _MM_SHUFFLE(2, 3, 0, 1)));
                    result[j] = _mm256_extract_epi32(sum, 0);
                    auto c = _mm256_set1_epi32(q[j + 1]);
                    u = M::add(c, M::mont(u, x)); v = M::add(c, M::mont(v, y));
                }
                return result;
            }
            usize half = size / 2;
            auto a = self(self, 2 * node, start, half), b = self(self, 2 * node + 1, start + half, half);
            if (size < 64) {
                const auto& l = tree[2 * node].polynomial; const auto& r = tree[2 * node + 1].polynomial;
                for (usize k = 0; k < size; ++k) {
                    u64 x = 0, y = 0;
                    for (usize j = k < half ? 0 : k - half + 1; j <= std::min(half, k); ++j) {
                        x += u64(a[k - j]) * r[j]; y += u64(b[k - j]) * l[j];
                    }
                    result[k] = M::add(x % P, y % P);
                }
                return result;
            }
            a.resize(size); b.resize(size);
            const auto& ntt = convolution_detail::info<P>;
            auto* x = (convolution_detail::Vec*)a.p; auto* y = (convolution_detail::Vec*)b.p;
            ntt.forward(x, size / 8); ntt.forward(y, size / 8);
            ntt.products(x, (convolution_detail::Vec*)tree[node].right.p, size / 8);
            ntt.products(y, (convolution_detail::Vec*)tree[node].left.p, size / 8);
            for (usize i = 0; i < size; ++i) a[i] = M::add(std::min(a[i], a[i] - P), std::min(b[i], b[i] - P));
            ntt.inverse(x, size / 8);
            return a;
        };
        auto result = merge(merge, 1, 0, length);
        result.n = n; std::reverse(result.p, result.p + n);
        return result;
    }

    // Coefficients in the Newton basis of points; f.size()<=points.size().
    Buffer<u32> to_newton(Buffer<u32> f) {
        Buffer<u32> answer(points.size());
        if (points.empty()) return answer;
        f.resize(length);
        auto visit = [&](auto&& self, usize node, usize start, Buffer<u32> value) -> void {
            if (start >= points.size()) return;
            usize size = value.n;
            if (size == leaf) {
                u32* a = value.p;
                for (usize i = 0; i < leaf && start + i < points.size(); ++i, ++a) {
                    for (usize j = leaf - i - 1; j; --j) a[j - 1] = M::add(a[j - 1], M::mul(points[start + i], a[j]));
                    answer[start + i] = a[0];
                }
                return;
            }
            const auto& q = tree[2 * node].polynomial;
            Buffer<u32> divisor(q.n); std::reverse_copy(q.p, q.p + q.n, divisor.p);
            auto [high, low] = polynomial_divmod<P>(std::move(value), divisor);
            high.resize(size / 2); low.resize(size / 2);
            self(self, 2 * node, start, std::move(low));
            self(self, 2 * node + 1, start + size / 2, std::move(high));
        };
        visit(visit, 1, 0, std::move(f));
        return answer;
    }
};
}
