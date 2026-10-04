#pragma once
#include <toy/array.h>
#include <toy/polynomial.h>
#include <toy/subset_convolution.h>

namespace toy {
// Set-series arrays have power-of-two length. Multiplication is modulo x_i^2.
template <u32 P = 998244353>
Buffer<u32> set_exp(std::span<const u32> f) {
    Buffer<u32> result(f.size());
    result[0] = 1; // f[0]=0.
    for (usize width = 1; width < f.size(); width *= 2) {
        // exp(A+xB)=exp(A)+x*B*exp(A), since x^2=0.
        auto high =
            subset_convolution<P>(f.subspan(width, width), std::span<const u32>(result.p, width));
        memcpy(result.p + width, high.p, width * 4);
    }
    return result;
}

template <u32 P = 998244353>
Buffer<u32> set_log(std::span<const u32> f) {
    Buffer<u32> result(f.size());
    result[0] = 0; // f[0]=1.
    for (usize width = 1; width < f.size(); width *= 2) {
        auto high = subset_division<P>(f.subspan(width, width), f.first(width));
        memcpy(result.p + width, high.p, width * 4);
    }
    return result;
}

template <u32 P = 998244353>
Buffer<u32> set_compose(std::span<const u32> polynomial, std::span<const u32> f) {
    using M = Mod<P>;
    usize bits = std::countr_zero(f.size());
    if (polynomial.size() <= 1) {
        Buffer<u32> result(f.size());
        std::fill(result.p, result.p + result.n, 0u);
        result[0] = polynomial.empty() ? 0 : polynomial[0];
        return result;
    }
    usize degree = std::min(bits, polynomial.size() - 1);
    Buffer<u32> derivatives(degree + 1);
    std::fill(derivatives.p, derivatives.p + derivatives.n, 0u);
    if (!f[0])
        memcpy(derivatives.p, polynomial.data(), derivatives.n * 4);
    else
        for (usize i = polynomial.size(); i--;) {
            for (usize j = degree; j; --j)
                derivatives[j] = M::add(M::mul(derivatives[j], f[0]), derivatives[j - 1]);
            derivatives[0] = M::add(M::mul(derivatives[0], f[0]), polynomial[i]);
        }
    Array<Buffer<u32>> value(degree + 1);
    u32 factorial = 1;
    for (usize i = 0; i <= degree; ++i) {
        if (i) factorial = M::mul(factorial, i);
        value[i] = Buffer<u32>(1, usize(1) << (bits - i));
        value[i][0] = M::mul(derivatives[i], factorial);
    }
    for (usize width = 1, k = 1; width < f.size(); width *= 2, ++k) {
        // C_r=f^(r)(A): adding one variable gives C_r+x*B*C_(r+1).
        // Ascending r keeps the next derivative at its previous precision.
        for (usize r = 0; r <= std::min(degree, bits - k); ++r) {
            value[r].resize(2 * width);
            if (r == degree) continue;
            if (r + 1 == degree) {
                for (usize i = 0; i < width; ++i)
                    value[r][width + i] = M::mul(f[width + i], value[r + 1][0]);
            } else {
                auto high = subset_convolution<P>(f.subspan(width, width),
                                                  std::span<const u32>(value[r + 1]));
                memcpy(value[r].p + width, high.p, width * 4);
            }
        }
    }
    return std::move(value[0]);
}

template <u32 P = 998244353>
Buffer<u32> set_power_projection(std::span<const u32> f, std::span<const u32> weights,
                                 usize count) {
    using M = Mod<P>;
    Buffer<u32> result(count);
    if (!count) return result;
    if (count == 1) {
        result[0] = weights[0];
        return result;
    }
    usize bits = std::countr_zero(f.size());
    Array<Buffer<u32>> value(bits + 1);
    value[0] = Buffer<u32>(f.size());
    memcpy(value[0].p, weights.data(), weights.size() * 4);
    for (usize width = f.size() / 2, active = 0; width; width /= 2, ++active) {
        for (usize r = active + 1; r--;) {
            // Transpose of subset multiplication: complement both the incoming
            // upper weights and the product. Process r downward before adding.
            Buffer<u32> reversed(width), b(width);
            std::reverse_copy(value[r].p + width, value[r].p + 2 * width, reversed.p);
            memcpy(b.p, f.data() + width, width * 4);
            auto high = subset_convolution<P>(std::move(reversed), std::move(b));
            value[r].n = width;
            value[r + 1].resize(width);
            for (usize i = 0; i < width; ++i)
                value[r + 1][i] = M::add(value[r + 1][i], high[width - 1 - i]);
        }
    }
    if (!f[0]) {
        std::fill(result.p, result.p + count, 0u);
        u32 factorial = 1;
        for (usize i = 0; i < std::min(count, bits + 1); ++i) {
            if (i) factorial = M::mul(factorial, i);
            result[i] = M::mul(value[i][0], factorial);
        }
        return result;
    }
    auto [factorial, inverse] = factorials<P>(count - 1);
    Buffer<u32> a(std::min(count, bits + 1)), b(count);
    u32 power = 1;
    for (usize i = 0; i < a.n; ++i) a[i] = value[i][0];
    for (usize i = 0; i < count; ++i) b[i] = M::mul(power, inverse[i]), power = M::mul(power, f[0]);
    auto product = convolution<P>(std::move(a), std::move(b));
    for (usize i = 0; i < count; ++i) result[i] = M::mul(product[i], factorial[i]);
    return result;
}
} // namespace toy
