#pragma once
#include <toy/fps.h>

namespace toy {

// Multiply shortest polynomials first. Entries are moved from as they merge.
template<u32 P = 998244353>
Buffer<u32> polynomial_product(std::span<Buffer<u32>> values) {
    using M = Mod<P>;
    Buffer<usize> heap(values.size());
    usize count = 0;
    u32 constant = 1;
    for (usize i = 0; i < values.size(); ++i) {
        if (!values[i].n) return {};
        if (values[i].n == 1) constant = M::mul(constant, values[i][0]);
        else heap[count++] = i;
    }
    if (!count) { Buffer<u32> result(1); result[0] = constant; return result; }
    auto compare = [&](usize a, usize b) { return values[a].n > values[b].n; };
    std::make_heap(heap.p, heap.p + count, compare);
    auto pop = [&] { std::pop_heap(heap.p, heap.p + count, compare); return heap[--count]; };
    while (count > 1) {
        usize a = pop(), b = pop();
        values[a] = convolution<P>(std::move(values[a]), std::move(values[b]));
        heap[count++] = a; std::push_heap(heap.p, heap.p + count, compare);
    }
    auto result = std::move(values[heap[0]]);
    fps_scale<P>(result, constant);
    return result;
}

template<u32 P = 998244353>
std::pair<Buffer<u32>, Buffer<u32>> factorials(usize n) {
    using M = Mod<P>;
    Buffer<u32> f(n + 1), inverse(n + 1);
    f[0] = 1;
    for (usize i = 1; i <= n; ++i) f[i] = M::mul(f[i - 1], i);
    inverse[n] = M::pow(f[n], P - 2);
    for (usize i = n; i; --i) inverse[i - 1] = M::mul(inverse[i], i);
    return {std::move(f), std::move(inverse)};
}

template<u32 P = 998244353>
u32 polynomial_eval(std::span<const u32> f, u32 x) {
    u32 value = 0;
    for (usize i = f.size(); i--;) value = Mod<P>::add(Mod<P>::mul(value, x), f[i]);
    return value;
}

// f(x+c), with f.size()<P.
template<u32 P = 998244353>
Buffer<u32> polynomial_taylor_shift(Buffer<u32> f, u32 c) {
    using M = Mod<P>;
    usize n = f.n;
    if (!n || !c) return f;
    auto [factorial, inverse] = factorials<P>(n - 1);
    Buffer<u32> b(n);
    u32 power = 1;
    for (usize i = 0; i < n; ++i) {
        f[i] = M::mul(f[i], factorial[i]);
        b[i] = M::mul(power, inverse[i]); power = M::mul(power, c);
    }
    std::reverse(f.p, f.p + n);
    f = convolution<P>(std::move(f), std::move(b));
    f.n = n; std::reverse(f.p, f.p + n);
    for (usize i = 0; i < n; ++i) f[i] = M::mul(f[i], inverse[i]);
    return f;
}

// f(a*r^i), 0<=i<count. Repeated points and zero a/r are supported.
template<u32 P = 998244353>
Buffer<u32> polynomial_eval_geometric(std::span<const u32> f, usize count, u32 a, u32 r) {
    using M = Mod<P>;
    Buffer<u32> result(count);
    if (!count) return result;
    if (f.empty() || !a || !r || r == 1) {
        u32 constant = f.empty() ? 0 : f[0];
        std::fill(result.p, result.p + count, r == 1 ? polynomial_eval<P>(f, a) : constant);
        if (!r) result[0] = polynomial_eval<P>(f, a);
        return result;
    }
    usize size = std::bit_ceil(f.size() + count - 1);
    Buffer<u32> left(size), right(size);
    std::fill(left.p, left.p + size, 0u); std::fill(right.p, right.p + size, 0u);
    u32 ir = M::pow(r, P - 2), value = 1, step = a;
    for (usize j = 0; j < f.size(); ++j) {
        left[(size - j) & (size - 1)] = M::mul(f[j], value);
        value = M::mul(value, step); step = M::mul(step, ir);
    }
    // ij=C(i+j,2)-C(i,2)-C(j,2), so no square root of r is needed.
    value = step = 1;
    for (usize j = 0; j < f.size() + count - 1; ++j) {
        right[j] = value; value = M::mul(value, step); step = M::mul(step, r);
    }
    left = convolution_cyclic<P>(std::move(left), std::move(right));
    value = step = 1;
    for (usize i = 0; i < count; ++i) {
        result[i] = M::mul(left[i], value);
        value = M::mul(value, step); step = M::mul(step, ir);
    }
    return result;
}

// Interpolate at distinct a*r^i, with values.size()<P.
template<u32 P = 998244353>
Buffer<u32> polynomial_interpolate_geometric(Buffer<u32> values, u32 a, u32 r) {
    using M = Mod<P>;
    usize n = values.n;
    if (n <= 1) return values;
    if (!r) {
        u32 slope = M::mul(M::sub(values[0], values[1]), M::pow(a, P - 2));
        values[0] = values[1]; values[1] = slope;
        return values;
    }
    Buffer<u32> power(n + 1), prefix(n), inverse(n), reversed(n);
    power[0] = prefix[0] = 1;
    for (usize i = 1; i <= n; ++i) power[i] = M::mul(power[i - 1], r);
    for (usize i = 1; i < n; ++i) prefix[i] = M::mul(prefix[i - 1], M::sub(1, power[i]));
    inverse[n - 1] = M::pow(prefix[n - 1], P - 2);
    for (usize i = n - 1; i; --i) inverse[i - 1] = M::mul(inverse[i], M::sub(1, power[i]));
    u32 factor = M::pow(a, P - n), step = M::sub(0, M::pow(M::pow(r, P - 2), n - 2));
    // G'(a*r^i)=a^(n-1)*(-1)^i*r^(i*(n-1)-i*(i+1)/2)*S[i]*S[n-1-i].
    for (usize i = 0; i < n; ++i) {
        values[i] = M::mul(values[i], M::mul(factor, M::mul(inverse[i], inverse[n - 1 - i])));
        factor = M::mul(factor, step); step = M::mul(step, r);
    }
    auto sums = polynomial_eval_geometric<P>(values, n, 1, r);
    factor = 1;
    for (usize i = 0; i < n; ++i) sums[i] = M::mul(sums[i], factor), factor = M::mul(factor, a);
    // The nonconstant part of G in reverse order, from Gaussian binomials.
    u32 total = M::mul(prefix[n - 1], M::sub(1, power[n]));
    if (!total) { std::reverse(sums.p, sums.p + n); return sums; }
    reversed[0] = 1; factor = 1; step = M::sub(0, a);
    for (usize k = 1; k < n; ++k) {
        factor = M::mul(factor, step); step = M::mul(step, r);
        reversed[k] = M::mul(M::mul(total, factor), M::mul(inverse[k], inverse[n - k]));
    }
    sums = convolution<P>(std::move(reversed), std::move(sums));
    sums.n = n; std::reverse(sums.p, sums.p + n);
    return sums;
}

// Values at c+i from f(0)..f(n-1), with n,count<P. Wraparound is allowed.
template<u32 P = 998244353>
Buffer<u32> polynomial_shift_samples(std::span<const u32> samples, usize count, u32 c) {
    using M = Mod<P>;
    usize n = samples.size();
    Buffer<u32> result(count);
    if (!count) return result;
    if (!n || n == 1) { std::fill(result.p, result.p + count, n ? samples[0] : 0); return result; }
    if (c < n && count <= n - c) { memcpy(result.p, samples.data() + c, count * 4); return result; }
    auto [factorial, inv_factorial] = factorials<P>(n);
    usize used = n + count - 1, size = std::bit_ceil(used);
    Buffer<u32> a(size), b(size);
    std::fill(a.p + n, a.p + size, 0u); std::fill(b.p + used, b.p + size, 0u);
    for (usize i = 0; i < n; ++i) {
        a[i] = M::mul(samples[i], M::mul(inv_factorial[i], inv_factorial[n - 1 - i]));
        if ((n - 1 - i) & 1) a[i] = M::sub(0, a[i]);
    }
    u32 start = M::sub(c, n - 1), value = start, product = 1;
    for (usize i = 0; i < used; ++i) {
        b[i] = product;
        if (value) product = M::mul(product, value);
        value = M::add(value, 1);
    }
    product = M::pow(product, P - 2);
    for (usize i = used; i--;) {
        value = M::sub(value, 1);
        u32 inverse = M::mul(b[i], product);
        if (value) product = M::mul(product, value);
        b[i] = value ? inverse : 0;
    }
    // The wanted convolution slice starts at n-1; cyclic wrap lands below it.
    Buffer<u32> denominator(count);
    memcpy(denominator.p, b.p, count * 4);
    a = convolution_cyclic<P>(std::move(a), std::move(b));
    product = 1;
    for (usize j = 0; j < n; ++j) product = M::mul(product, M::sub(c, j));
    value = c;
    for (usize i = 0; i < count; ++i) {
        result[i] = value < n ? samples[value] : M::mul(product, a[n - 1 + i]);
        value = M::add(value, 1);
        product = value == n ? factorial[n] : M::mul(M::mul(product, value), denominator[i]);
    }
    return result;
}

// g(0)=0 and g(x+1)-g(x)=f(x), with f.size()<P-1.
template<u32 P = 998244353>
Buffer<u32> polynomial_prefix_sum(Buffer<u32> f) {
    using M = Mod<P>;
    usize n = f.n;
    auto [factorial, inverse] = factorials<P>(n);
    Buffer<u32> denominator(n);
    for (usize i = 0; i < n; ++i) denominator[i] = inverse[i + 1];
    // t/(exp(t)-1) generates the Bernoulli numbers divided by factorials.
    auto bernoulli = fps_inv<P>(denominator, n);
    for (usize i = 0; i < n; ++i) f[i] = M::mul(f[i], factorial[i]);
    std::reverse(f.p, f.p + n);
    f = convolution<P>(std::move(f), std::move(bernoulli));
    Buffer<u32> result(n + 1); result[0] = 0;
    for (usize j = 1; j <= n; ++j) result[j] = M::mul(f[n - j], inverse[j]);
    return result;
}

// Nonzero leading coefficients, divisor nonempty. Consume the dividend.
template<u32 P = 998244353>
std::pair<Buffer<u32>, Buffer<u32>> polynomial_divmod(Buffer<u32> f, std::span<const u32> g) {
    using M = Mod<P>;
    if (f.n < g.size()) return {Buffer<u32>{}, std::move(f)};
    usize n = f.n, m = g.size(), count = n - m + 1;
    if (m == 1) { fps_scale<P>(f, M::pow(g[0], P - 2)); return {std::move(f), Buffer<u32>{}}; }
    Buffer<u32> q(count);
    if (std::min(m, count) <= 16) {
        u32 inverse = M::pow(g.back(), P - 2);
        for (usize k = count; k--;) {
            q[k] = M::mul(f[k + m - 1], inverse);
            u32 factor = M::mont(q[k], M::r2);
            auto w = _mm256_set1_epi32(factor); usize j = 0;
            for (; j + 8 < m; j += 8) {
                auto x = _mm256_loadu_si256((const __m256i*)(f.p + k + j));
                auto y = _mm256_loadu_si256((const __m256i*)(g.data() + j));
                _mm256_storeu_si256((__m256i*)(f.p + k + j), M::sub(x, M::mont(y, w)));
            }
            for (; j < m - 1; ++j) f[k + j] = M::sub(f[k + j], M::mont(factor, g[j]));
        }
    } else {
        Buffer<u32> reversed(std::min(m, count)), numerator(count);
        for (usize i = 0; i < reversed.n; ++i) reversed[i] = g[m - 1 - i];
        for (usize i = 0; i < count; ++i) numerator[i] = f[n - 1 - i];
        q = fps_div<P>(std::span<const u32>(numerator), std::span<const u32>(reversed), count);
        std::reverse(q.p, q.p + count);
        // Only the low m-1 coefficients contribute to the remainder.
        Buffer<u32> lowq(std::min(count, m - 1)), lowg(m - 1);
        memcpy(lowq.p, q.p, lowq.n * 4); memcpy(lowg.p, g.data(), lowg.n * 4);
        auto product = convolution<P>(std::move(lowq), std::move(lowg));
        for (usize i = 0; i < m - 1; ++i) f[i] = M::sub(f[i], product[i]);
    }
    f.n = m - 1;
    while (f.n && !f[f.n - 1]) --f.n;
    return {std::move(q), std::move(f)};
}
}
