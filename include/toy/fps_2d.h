#pragma once
#include <toy/fps.h>

namespace toy {
// Row-major coefficients of x^i*y^j, inverse modulo (x^rows,y^columns).
template<u32 P = 998244353>
Buffer<u32> fps_inv_2d(std::span<const u32> input, usize rows, usize columns) {
    using M = Mod<P>;
    if (rows == 1 || columns == 1) return fps_inv<P>(input, rows * columns);
    bool transpose = rows > columns;
    Buffer<u32> transposed;
    if (transpose) {
        transposed = Buffer<u32>(input.size());
        for (usize i = 0; i < rows; ++i) for (usize j = 0; j < columns; ++j) transposed[j * rows + i] = input[i * columns + j];
        input = std::span<const u32>(transposed); std::swap(rows, columns);
    }
    usize stride = std::bit_ceil(2 * columns - 1), seed = std::min(rows, std::max<usize>(1, 32 / stride));
    Buffer<u32> g(rows * columns);
    auto first = fps_inv<P>(input.first(columns), columns);
    memcpy(g.p, first.p, columns * 4);
    u32 inv0 = first[0];
    for (usize i = 1; i < seed; ++i) for (usize j = 0; j < columns; ++j) {
        u32 sum = 0;
        for (usize a = 0; a <= i; ++a) for (usize b = 0; b <= j; ++b) if (a || b)
            sum = M::add(sum, M::mul(input[a * columns + b], g[(i - a) * columns + j - b]));
        g[i * columns + j] = M::mul(M::sub(0, sum), inv0);
    }
    usize capacity = std::bit_ceil(rows) * stride;
    Buffer<u32> work(capacity), spectrum(capacity);
    const auto& ntt = convolution_detail::info<P>;
    auto* x = (convolution_detail::Vec*)work.p;
    auto* y = (convolution_detail::Vec*)spectrum.p;
    for (usize half = seed; half < rows; half *= 2) {
        usize end = std::min(rows, 2 * half), size = 2 * half * stride;
        std::fill(work.p, work.p + size, 0u); std::fill(spectrum.p, spectrum.p + size, 0u);
        for (usize i = 0; i < end; ++i) memcpy(work.p + i * stride, input.data() + i * columns, columns * 4);
        for (usize i = 0; i < half; ++i) memcpy(spectrum.p + i * stride, g.p + i * columns, columns * 4);
        ntt.forward(x, size / 8); ntt.forward(y, size / 8);
        ntt.products(x, y, size / 8); ntt.inverse(x, size / 8);
        // Double-width rows prevent y carries. Discard y^columns and the
        // contaminated low x half before the second Newton product.
        std::fill(work.p, work.p + half * stride, 0u);
        for (usize i = half; i < 2 * half; ++i) std::fill(work.p + i * stride + columns, work.p + (i + 1) * stride, 0u);
        ntt.forward(x, size / 8); ntt.products(x, y, size / 8); ntt.inverse(x, size / 8);
        for (usize i = half; i < end; ++i) for (usize j = 0; j < columns; ++j) g[i * columns + j] = M::sub(0, work[i * stride + j]);
    }
    if (!transpose) return g;
    Buffer<u32> result(rows * columns);
    for (usize i = 0; i < rows; ++i) for (usize j = 0; j < columns; ++j) result[j * rows + i] = g[i * columns + j];
    return result;
}
}
