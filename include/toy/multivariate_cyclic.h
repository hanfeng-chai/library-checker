#pragma once
#include <toy/convolution_crt.h>
#include <toy/primitive_root.h>

namespace toy {

// p is prime <2^30, each dimension >=2 divides p-1. First axis varies fastest.
inline Buffer<u32> multivariate_cyclic(std::span<const u32> dimensions, Buffer<u32> a,
                                       Buffer<u32> b, u32 p) {
    Barrett mod(p);
    if (dimensions.empty()) {
        a[0] = mod.mul(a[0], b[0]);
        return a;
    }
    u32 generator = primitive_root(p);
    usize size = a.n, stride = 1;
    struct Chirp {
        usize n, length;
        Buffer<u32> inverse;
        CyclicCRT product;
        static Buffer<u32> kernel(usize n, u32 w, const Barrett &mod) {
            Buffer<u32> b(std::bit_ceil(2 * n - 1));
            std::fill(b.p, b.p + b.n, 0u);
            u32 value = 1, step = 1;
            // w^(i*(i-1)/2), avoiding a square root of w for even dimensions.
            for (usize i = 0; i < 2 * n - 1; ++i)
                b[i] = value, value = mod.mul(value, step), step = mod.mul(step, w);
            return b;
        }
        Chirp(usize n, u32 w, const Barrett &mod)
            : n(n), length(std::bit_ceil(2 * n - 1)), inverse(n),
              product(kernel(n, w, mod), mod.p) {
            u32 value = 1, step = 1, iw = mod.pow(w, mod.p - 2);
            for (usize i = 0; i < n; ++i)
                inverse[i] = value, value = mod.mul(value, step), step = mod.mul(step, iw);
        }
        void apply(Buffer<u32> &f, const Barrett &mod, usize stride, bool reversed) {
            Buffer<u32> input(length);
            for (usize base = 0; base < f.n; base += stride * n)
                for (usize j = 0; j < stride; ++j) {
                    std::fill(input.p, input.p + length, 0u);
                    for (usize i = 0; i < n; ++i)
                        input[(length - i) & (length - 1)] =
                            mod.mul(f[base + i * stride + j], inverse[i]);
                    auto c = product(input);
                    for (usize i = 0; i < n; ++i)
                        f[base + (reversed && i ? n - i : i) * stride + j] =
                            mod.mul(c[i], inverse[i]);
                }
        }
    };
    // Own plans without C++ allocation operators; all live until the inverse.
    Buffer<u32> roots(dimensions.size());
    Buffer<Chirp *> plan(dimensions.size());
    std::fill(plan.p, plan.p + plan.n, nullptr);
    auto small = [&](Buffer<u32> &f, usize n, usize step, u32 w) {
        if (n == 2) {
            for (usize base = 0; base < size; base += 2 * step)
                for (usize j = 0; j < step; ++j) {
                    u32 x = f[base + j], y = f[base + step + j], s = x + y, d = x - y;
                    f[base + j] = std::min(s, s - p);
                    f[base + step + j] = std::min(d, d + p);
                }
            return;
        }
        u32 matrix[16][16], temp[16], r = 1;
        for (usize i = 0; i < n; ++i, r = mod.mul(r, w)) {
            u32 power = 1;
            for (usize j = 0; j < n; ++j) matrix[i][j] = power, power = mod.mul(power, r);
        }
        for (usize base = 0; base < size; base += step * n)
            for (usize j = 0; j < step; ++j) {
                for (usize i = 0; i < n; ++i) temp[i] = f[base + i * step + j];
                for (usize i = 0; i < n; ++i) {
                    u64 sum = 0;
                    for (usize k = 0; k < n; ++k) sum += u64(temp[k]) * matrix[i][k];
                    f[base + i * step + j] = sum % p;
                }
            }
    };
    for (usize d = 0; d < dimensions.size(); stride *= dimensions[d++]) {
        usize n = dimensions[d];
        u32 w = roots[d] = mod.pow(generator, (p - 1) / n);
        if (n <= 16) {
            small(a, n, stride, w);
            small(b, n, stride, w);
        } else {
            plan[d] = std::construct_at((Chirp *)malloc(sizeof(Chirp)), n, w, mod);
            plan[d]->apply(a, mod, stride, false);
            plan[d]->apply(b, mod, stride, false);
        }
    }
    u32 scale = mod.pow(size, p - 2);
    for (usize i = 0; i < size; ++i) a[i] = mod.mul(mod.mul(a[i], b[i]), scale);
    stride = 1;
    for (usize d = 0; d < dimensions.size(); stride *= dimensions[d++]) {
        usize n = dimensions[d];
        if (n <= 16)
            small(a, n, stride, mod.pow(roots[d], p - 2));
        else {
            plan[d]->apply(a, mod, stride, true);
            std::destroy_at(plan[d]);
            free(plan[d]);
        }
    }
    return a;
}
} // namespace toy
