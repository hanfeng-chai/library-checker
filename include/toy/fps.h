#pragma once
#include <toy/convolution.h>
#include <toy/mod.h>
#include <toy/mod_sqrt.h>
#include <toy/radix_ntt.h>

namespace toy {

template<u32 P = 998244353>
void fps_scale(std::span<u32> f, u32 coefficient) {
    using M = Mod<P>;
    if (coefficient == 1) return;
    auto c = _mm256_set1_epi32(M::mont(coefficient, M::r2));
    usize i = 0;
    for (; i + 8 <= f.size(); i += 8) _mm256_storeu_si256((__m256i*)(f.data() + i),
        M::mont(_mm256_loadu_si256((const __m256i*)(f.data() + i)), c));
    for (; i < f.size(); ++i) f[i] = M::mul(f[i], coefficient);
}

// Inverses of 1..n, n<P, with entry zero set to zero. Eight factorial chains run
// independently, replacing one scalar integer division per coefficient.
template<u32 P = 998244353, bool Montgomery = false>
Buffer<u32> inverse_numbers(usize n) {
    static_assert((P - 1) % 8 == 0);
    using M = Mod<P>;
    usize size = (n + 7) & -usize(8);
    Buffer<u32> result(n + 1, size + 1), prefix(size);
    result[0] = 0;
    if (!n) return result;
    constexpr u32 one = (u64(1) << 32) % P;
    auto cur = _mm256_set1_epi32(one);
    auto index = M::mont(_mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8), _mm256_set1_epi32(M::r2));
    auto step = _mm256_set1_epi32(M::mul(8, one));
    for (usize i = 0; i < size; i += 8) {
        cur = M::mont(cur, index);
        _mm256_storeu_si256((__m256i*)(prefix.p + i), cur);
        index = M::add(index, step);
    }
    auto inv = _mm256_set1_epi32(one);
    for (u32 e = P - 2; e; e >>= 1, cur = M::mont(cur, cur)) if (e & 1) inv = M::mont(inv, cur);
    for (usize i = size; i;) {
        i -= 8; index = M::sub(index, step);
        auto before = i ? _mm256_loadu_si256((const __m256i*)(prefix.p + i - 8)) : _mm256_set1_epi32(one);
        auto value = M::mont(before, inv);
        if constexpr (!Montgomery) value = M::mont(value, _mm256_set1_epi32(1));
        _mm256_storeu_si256((__m256i*)(result.p + i + 1), value);
        inv = M::mont(inv, index);
    }
    return result;
}

// First n coefficients of 1/f. P is prime and f[0] != 0.
template<u32 P = 998244353>
Buffer<u32> fps_inv(std::span<const u32> f, usize n) {
    static_assert((P - 1) % 8 == 0);
    using M = Mod<P>;
    if (!n) return {};
    f = f.first(std::min(f.size(), n));
    while (f.size() > 1 && !f.back()) f = f.first(f.size() - 1);
    Buffer<u32> b(n, std::bit_ceil(n));
    b[0] = M::pow(f[0], P - 2);
    if (f.size() == 1) { std::fill(b.p + 1, b.p + n, 0u); return b; }
    usize seed = f.size() <= 4 ? n : std::min<usize>(32, n);
    for (usize k = 1; k < seed; ++k) {
        u32 sum = 0;
        for (usize j = 1; j <= std::min(k, f.size() - 1); ++j) sum = M::add(sum, M::mul(f[j], b[k - j]));
        b[k] = M::mul(M::sub(0, sum), b[0]);
    }
    if (seed == n) return b;
    Buffer<u32> work(std::bit_ceil(n)), spectrum(std::bit_ceil(n));
    auto* x = (convolution_detail::Vec*)work.p;
    auto* y = (convolution_detail::Vec*)spectrum.p;
    const auto& ntt = convolution_detail::info<P>;
    for (usize half = seed; half < n; half *= 2) {
        usize size = 2 * half, count = std::min(size, f.size());
        memcpy(work.p, f.data(), count * sizeof(u32));
        std::fill(work.p + count, work.p + size, 0u);
        memcpy(spectrum.p, b.p, half * sizeof(u32));
        std::fill(spectrum.p + half, spectrum.p + size, 0u);
        ntt.forward(x, size / 8); ntt.forward(y, size / 8);
        ntt.products(x, y, size / 8); ntt.inverse(x, size / 8);
        // Cyclic wrap only contaminates the low half. The high half of f*b-1
        // is exact. Multiply it by the cached b spectrum and negate while storing.
        std::fill(work.p, work.p + half, 0u);
        ntt.forward(x, size / 8); ntt.products(x, y, size / 8); ntt.inverse(x, size / 8);
        for (usize i = half; i < std::min(size, n); ++i) b[i] = M::sub(0, work[i]);
    }
    return b;
}

// a/b mod x^n, b[0]!=0. Reuse small transforms of denominator and answer blocks.
template<u32 P = 998244353>
Buffer<u32> fps_div(std::span<const u32> a, std::span<const u32> b, usize n) {
    using M = Mod<P>;
    Buffer<u32> result(n);
    if (!n) return result;
    b = b.first(std::min(b.size(), n));
    while (b.size() > 1 && !b.back()) b = b.first(b.size() - 1);
    if (n <= 64 || b.size() <= 4) {
        u32 inverse = M::pow(b[0], P - 2);
        for (usize i = 0; i < n; ++i) {
            u32 value = i < a.size() ? a[i] : 0;
            for (usize j = 1; j <= std::min(i, b.size() - 1); ++j) value = M::sub(value, M::mul(b[j], result[i - j]));
            result[i] = M::mul(value, inverse);
        }
        return result;
    }
    usize block = std::max<usize>(64, std::bit_ceil(n) / 16), size = 2 * block;
    usize inputs = (b.size() + block - 1) / block, outputs = (n + block - 1) / block;
    Buffer<u32> denominator(inputs * size), quotient(outputs * size), work(size);
    NTT<P> ntt(size);
    constexpr u32 one = (u64(1) << 32) % P;
    auto inverse = fps_inv<P>(b.first(std::min(b.size(), block)), block);
    inverse.resize(size); fps_scale<P>(inverse, one); ntt.forward(inverse);
    std::fill(denominator.p, denominator.p + denominator.n, 0u);
    for (usize k = 0; k < inputs; ++k) {
        u32* row = denominator.p + k * size;
        usize count = std::min(block, b.size() - k * block);
        memcpy(row, b.data() + k * block, count * 4);
        fps_scale<P>(std::span(row, count), one); ntt.forward(std::span(row, size));
    }
    for (usize k = 0; k < outputs; ++k) {
        if (k) {
            for (usize i = 0; i < size; i += 8) {
                auto sum = _mm256_setzero_si256();
                for (usize first = 1; first <= std::min(k, inputs); first += 8) {
                    auto even = _mm256_setzero_si256(), odd = even;
                    for (usize t = first; t < std::min(first + 8, std::min(k, inputs) + 1); ++t) {
                        auto x = _mm256_loadu_si256((const __m256i*)(quotient.p + (k - t) * size + i));
                        auto y = t < inputs ? _mm256_loadu_si256((const __m256i*)(denominator.p + t * size + i)) : _mm256_setzero_si256();
                        auto z = _mm256_loadu_si256((const __m256i*)(denominator.p + (t - 1) * size + i));
                        // x^block is +1 on the first frequency half, -1 on the
                        // second: move the previous product's high part down.
                        y = i < block ? M::add(y, z) : M::sub(y, z);
                        even = _mm256_add_epi64(even, _mm256_mul_epu32(x, y));
                        odd = _mm256_add_epi64(odd, _mm256_mul_epu32(_mm256_srli_epi64(x, 32), _mm256_srli_epi64(y, 32)));
                    }
                    sum = M::add(sum, M::mont_sum8(even, odd));
                }
                _mm256_storeu_si256((__m256i*)(work.p + i), sum);
            }
            ntt.inverse(work);
        } else std::fill(work.p, work.p + block, 0u);
        usize count = std::min(block, n - k * block);
        for (usize i = 0; i < block; ++i) {
            usize j = k * block + i;
            work[i] = M::sub(j < a.size() ? a[j] : 0, work[i]);
        }
        std::fill(work.p + block, work.p + size, 0u);
        ntt.forward(work);
        for (usize i = 0; i < size; i += 8) _mm256_storeu_si256((__m256i*)(work.p + i), M::mont(
            _mm256_loadu_si256((const __m256i*)(work.p + i)), _mm256_loadu_si256((const __m256i*)(inverse.p + i))));
        ntt.inverse(work);
        memcpy(result.p + k * block, work.p, count * 4);
        if (k + 1 < outputs) {
            u32* row = quotient.p + k * size;
            memcpy(row, work.p, block * 4); std::fill(row + block, row + size, 0u);
            ntt.forward(std::span(row, size));
        }
    }
    return result;
}

// First n coefficients of log(f), with f[0]=1 and n<P.
template<u32 P = 998244353>
Buffer<u32> fps_log(std::span<const u32> f, usize n) {
    using M = Mod<P>;
    Buffer<u32> result(n);
    if (!n) return result;
    result[0] = 0;
    f = f.first(std::min(f.size(), n));
    while (f.size() > 1 && !f.back()) f = f.first(f.size() - 1);
    if (f.size() <= 1) { std::fill(result.p + 1, result.p + n, 0u); return result; }
    auto g = fps_inv<P>(f, n - 1);
    Buffer<u32> derivative(f.size() - 1);
    for (usize i = 0; i < derivative.n; ++i) derivative[i] = M::mul(i + 1, f[i + 1]);
    auto h = convolution<P>(std::move(derivative), std::move(g));
    auto inverse = inverse_numbers<P, true>(n - 1);
    usize i = 1;
    for (; i + 8 <= n; i += 8) _mm256_storeu_si256((__m256i*)(result.p + i), M::mont(
        _mm256_loadu_si256((const __m256i*)(h.p + i - 1)), _mm256_loadu_si256((const __m256i*)(inverse.p + i))));
    for (; i < n; ++i) result[i] = M::mont(h[i - 1], inverse[i]);
    return result;
}

// exp(f) mod x^n, f[0]=0 and n<P. Maintain g and its reciprocal together.
template<u32 P = 998244353>
Buffer<u32> fps_exp(std::span<const u32> f, usize n) {
    using M = Mod<P>;
    if (!n) return {};
    f = f.first(std::min(f.size(), n));
    while (!f.empty() && !f.back()) f = f.first(f.size() - 1);
    usize capacity = std::bit_ceil(n);
    Buffer<u32> g(n, capacity);
    g[0] = 1;
    if (f.empty()) { std::fill(g.p + 1, g.p + n, 0u); return g; }
    auto inverse = inverse_numbers<P>(n - 1);
    Buffer<u32> derivative(f.size() - 1);
    for (usize i = 0; i < derivative.n; ++i) derivative[i] = M::mul(i + 1, f[i + 1]);
    usize seed = f.size() <= 4 ? n : std::min<usize>(32, n);
    for (usize i = 1; i < seed; ++i) {
        u32 sum = 0;
        for (usize j = 1; j <= std::min(i, derivative.n); ++j) sum = M::add(sum, M::mul(derivative[j - 1], g[i - j]));
        g[i] = M::mul(sum, inverse[i]);
    }
    if (seed == n) return g;
    Buffer<u32> h(capacity), gs(capacity), hs(capacity), work(capacity), other(capacity);
    h[0] = 1;
    for (usize i = 1; i < seed; ++i) {
        u32 sum = 0;
        for (usize j = 1; j <= i; ++j) sum = M::add(sum, M::mul(g[j], h[i - j]));
        h[i] = M::sub(0, sum);
    }
    const auto& ntt = convolution_detail::info<P>;
    auto* x = (convolution_detail::Vec*)work.p;
    auto* y = (convolution_detail::Vec*)other.p;
    auto* G = (convolution_detail::Vec*)gs.p;
    auto* H = (convolution_detail::Vec*)hs.p;
    for (usize half = seed; half < n; half *= 2) {
        usize size = 2 * half, count = std::min(size - 1, derivative.n), end = std::min(size, n);
        memcpy(gs.p, g.p, half * 4); std::fill(gs.p + half, gs.p + size, 0u);
        memcpy(hs.p, h.p, half * 4); std::fill(hs.p + half, hs.p + size, 0u);
        memcpy(work.p, derivative.p, count * 4); std::fill(work.p + count, work.p + size, 0u);
        ntt.forward(G, size / 8); ntt.forward(H, size / 8); ntt.forward(x, size / 8);
        ntt.products(x, G, size / 8); ntt.inverse(x, size / 8);
        // f'-g'/g=(f'g-g')/g. The residual starts at degree half-1;
        // cyclic wrap is below that, and the old reciprocal suffices here.
        std::fill(work.p, work.p + half - 1, 0u); work[size - 1] = 0;
        ntt.forward(x, size / 8); ntt.products(x, H, size / 8); ntt.inverse(x, size / 8);
        for (usize i = end; i-- > half;) work[i] = M::mul(work[i - 1], inverse[i]);
        std::fill(work.p, work.p + half, 0u); std::fill(work.p + end, work.p + size, 0u);
        if (size < n) {
            // h_new = h - h*((g*h-1)+delta), where delta=f-log(g).
            memcpy(other.p, gs.p, size * 4);
            ntt.products(y, H, size / 8); ntt.inverse(y, size / 8);
            std::fill(other.p, other.p + half, 0u);
            for (usize i = half; i < size; ++i) other[i] = M::add(other[i], work[i]);
            ntt.forward(y, size / 8); ntt.products(y, H, size / 8); ntt.inverse(y, size / 8);
            for (usize i = half; i < size; ++i) h[i] = M::sub(0, other[i]);
        }
        ntt.forward(x, size / 8); ntt.products(x, G, size / 8); ntt.inverse(x, size / 8);
        memcpy(g.p + half, work.p + half, (end - half) * 4);
    }
    return g;
}

// Unit-series power, solving f*Dg=k*(Df)*g block by block, D=x*d/dx.
template<u32 P>
Buffer<u32> fps_power_blocks(std::span<const u32> f, usize n, u32 k) {
    using M = Mod<P>;
    if (n <= 64 || (P - 1) % std::bit_ceil(n)) {
        auto h = fps_log<P>(f, n); fps_scale<P>(h, k); return fps_exp<P>(h, n);
    }
    usize target = std::bit_width(n) - 1;
    usize block = std::max<usize>(32, std::bit_ceil((n + target - 1) / target)), size = 2 * block;
    usize count = (n + block - 1) / block;
    auto g = fps_power_blocks<P>(f.first(std::min(f.size(), block)), block, k); g.resize(count * block);
    Buffer<u32> nf(count * size), df(count * size), ng(count * size), psi(size), phi(size);
    RadixNTT<P> ntt(size);
    constexpr u32 one = (u64(1) << 32) % P;
    auto transform = [&](const u32* a, usize m, u32* out) {
        if (m) memcpy(out, a, m * 4); std::fill(out + m, out + size, 0u);
        ntt.forward(std::span(out, size));
    };
    auto fixed = [&](Buffer<u32>& a, const u32* b) {
        std::fill(a.p + block, a.p + size, 0u); ntt.forward(a);
        for (usize i = 0; i < size; i += 8) _mm256_storeu_si256((__m256i*)(a.p + i), M::mont(
            _mm256_loadu_si256((const __m256i*)(a.p + i)), _mm256_loadu_si256((const __m256i*)(b + i))));
        ntt.inverse(a);
    };
    Buffer<u32> base(block), input(block);
    memcpy(base.p, g.p, block * 4); std::fill(input.p, input.p + block, 0u);
    memcpy(input.p, f.data(), std::min(f.size(), block) * 4);
    auto product = convolution<P>(std::move(base), std::move(input));
    auto h = fps_inv<P>(std::span<const u32>(product.p, block), block); h.resize(size);
    fps_scale<P>(h, one); ntt.forward(h);
    transform(g.p, block, ng.p);
    Buffer<u32> gs(size); memcpy(gs.p, ng.p, size * 4); fps_scale<P>(gs, one);
    for (usize b = 0; b < count; ++b) {
        usize offset = b * block;
        u32* a = nf.p + b * size; u32* d = df.p + b * size;
        std::fill(a, a + size, 0u); std::fill(d, d + size, 0u);
        usize used = offset < f.size() ? std::min(block, f.size() - offset) : 0, i = 0;
        auto r2 = _mm256_set1_epi32(M::r2), step = _mm256_set1_epi32(M::mul(8, one));
        auto index = M::mont(_mm256_setr_epi32(offset, offset + 1, offset + 2, offset + 3,
            offset + 4, offset + 5, offset + 6, offset + 7), r2);
        for (; i + 8 <= used; i += 8) {
            auto value = M::mont(_mm256_loadu_si256((const __m256i*)(f.data() + offset + i)), r2);
            _mm256_store_si256((__m256i*)(a + i), value);
            _mm256_store_si256((__m256i*)(d + i), M::mont(value, index)); index = M::add(index, step);
        }
        for (; i < used; ++i) {
            a[i] = M::mont(f[offset + i], M::r2); d[i] = M::mul(a[i], offset + i);
        }
        ntt.forward(std::span(a, size)); ntt.forward(std::span(d, size));
    }
    // x^block equals +1 / -1 on the two frequency halves. Folding the
    // previous block supplies its high product without another transform.
    for (usize b = count; --b;) for (usize i = 0; i < size; i += 8) {
        auto fold = [&](u32* a) {
            auto x = _mm256_loadu_si256((const __m256i*)(a + b * size + i));
            auto y = _mm256_loadu_si256((const __m256i*)(a + (b - 1) * size + i));
            _mm256_storeu_si256((__m256i*)(a + b * size + i), i < block ? M::add(x, y) : M::sub(x, y));
        };
        fold(nf.p); fold(df.p);
    }
    auto inverses = inverse_numbers<P, true>(n - 1);
    u32 kp = M::add(k, 1);
    for (usize b = 1; b < count; ++b) {
        if (b > 1) transform(g.p + (b - 1) * block, block, ng.p + (b - 1) * size);
        for (usize i = 0; i < size; i += 8) {
            auto sum = _mm256_setzero_si256(), dsum = sum;
            for (usize first = 0; first < b; first += 8) {
                auto even = _mm256_setzero_si256(), odd = even, deven = even, dodd = even;
                for (usize j = first; j < std::min(b, first + 8); ++j) {
                    auto x = _mm256_loadu_si256((const __m256i*)(ng.p + j * size + i));
                    auto y = _mm256_loadu_si256((const __m256i*)(nf.p + (b - j) * size + i));
                    auto d = _mm256_loadu_si256((const __m256i*)(df.p + (b - j) * size + i));
                    auto hi = _mm256_srli_epi64(x, 32);
                    even = _mm256_add_epi64(even, _mm256_mul_epu32(x, y));
                    odd = _mm256_add_epi64(odd, _mm256_mul_epu32(hi, _mm256_srli_epi64(y, 32)));
                    deven = _mm256_add_epi64(deven, _mm256_mul_epu32(x, d));
                    dodd = _mm256_add_epi64(dodd, _mm256_mul_epu32(hi, _mm256_srli_epi64(d, 32)));
                }
                sum = M::add(sum, M::mont_sum8(even, odd));
                dsum = M::add(dsum, M::mont_sum8(deven, dodd));
            }
            _mm256_storeu_si256((__m256i*)(psi.p + i), sum);
            _mm256_storeu_si256((__m256i*)(phi.p + i), dsum);
        }
        ntt.inverse(psi); ntt.inverse(phi);
        usize offset = b * block, used = std::min(block, n - offset);
        auto coefficient = _mm256_set1_epi32(M::mont(kp, M::r2));
        auto index = M::mont(_mm256_setr_epi32(offset, offset + 1, offset + 2, offset + 3,
            offset + 4, offset + 5, offset + 6, offset + 7), _mm256_set1_epi32(M::r2));
        auto step = _mm256_set1_epi32(M::mul(8, one));
        for (usize i = 0; i < block; i += 8) {
            auto x = _mm256_load_si256((const __m256i*)(psi.p + i));
            auto y = _mm256_load_si256((const __m256i*)(phi.p + i));
            _mm256_store_si256((__m256i*)(psi.p + i), M::sub(M::mont(y, coefficient), M::mont(x, index)));
            index = M::add(index, step);
        }
        fixed(psi, h.p);
        usize i = 0;
        for (; i + 8 <= used; i += 8) _mm256_store_si256((__m256i*)(psi.p + i), M::mont(
            _mm256_load_si256((const __m256i*)(psi.p + i)), _mm256_loadu_si256((const __m256i*)(inverses.p + offset + i))));
        for (; i < used; ++i) psi[i] = M::mont(psi[i], inverses[offset + i]);
        std::fill(psi.p + used, psi.p + block, 0u);
        fixed(psi, gs.p); memcpy(g.p + offset, psi.p, used * 4);
    }
    g.n = n; return g;
}

template<u32 P = 998244353>
Buffer<u32> fps_pow(std::span<const u32> f, usize n, u64 exponent) {
    using M = Mod<P>;
    if (!n) return {};
    auto zero = [&] { Buffer<u32> result(n); std::fill(result.p, result.p + n, 0u); return result; };
    if (!exponent) { auto result = zero(); result[0] = 1; return result; }
    usize first = 0;
    while (first < std::min(n, f.size()) && !f[first]) ++first;
    if (first == std::min(n, f.size()) || u128(first) * exponent >= n) return zero();
    if (exponent == 1) { auto result = zero(); memcpy(result.p, f.data(), std::min(n, f.size()) * 4); return result; }
    usize shift = first * exponent, count = n - shift;
    f = f.subspan(first, std::min(f.size() - first, count));
    while (f.size() > 1 && !f.back()) f = f.first(f.size() - 1);
    u32 constant = M::pow(f[0], exponent);
    if (f.size() == 1) { auto result = zero(); result[shift] = constant; return result; }
    Buffer<u32> normalized(f.size());
    memcpy(normalized.p, f.data(), f.size() * 4);
    fps_scale<P>(normalized, M::pow(f[0], P - 2));
    auto g = fps_power_blocks<P>(normalized, count, exponent % P);
    fps_scale<P>(g, constant);
    if (!shift) return g;
    auto result = zero();
    memcpy(result.p + shift, g.p, count * 4);
    return result;
}

template<u32 P = 998244353>
std::optional<Buffer<u32>> fps_sqrt(std::span<const u32> f, usize n) {
    using M = Mod<P>;
    usize first = 0;
    while (first < std::min(n, f.size()) && !f[first]) ++first;
    if (first == std::min(n, f.size())) { Buffer<u32> zero(n); if (n) std::fill(zero.p, zero.p + n, 0u); return zero; }
    if (first & 1) return std::nullopt;
    auto root = mod_sqrt<P>(f[first]);
    if (!root) return std::nullopt;
    usize count = n - first, capacity = std::bit_ceil(count), seed = std::min<usize>(32, count);
    f = f.subspan(first, std::min(f.size() - first, count));
    while (f.size() > 1 && !f.back()) f = f.first(f.size() - 1);
    if (f.size() == 1) {
        Buffer<u32> result(n); std::fill(result.p, result.p + n, 0u);
        result[first / 2] = *root; return result;
    }
    Buffer<u32> g(capacity), h(capacity), gs(capacity), hs(capacity), work(capacity);
    g[0] = *root; h[0] = M::pow(*root, P - 2);
    u32 inv2 = (P + 1) / 2, factor = M::mul(h[0], inv2);
    for (usize i = 1; i < seed; ++i) {
        u32 sum = 0;
        for (usize j = 1; j < i; ++j) sum = M::add(sum, M::mul(g[j], g[i - j]));
        g[i] = M::mul(M::sub(i < f.size() ? f[i] : 0, sum), factor);
        sum = 0;
        for (usize j = 1; j <= i; ++j) sum = M::add(sum, M::mul(g[j], h[i - j]));
        h[i] = M::mul(M::sub(0, sum), h[0]);
    }
    const auto& ntt = convolution_detail::info<P>;
    auto* x = (convolution_detail::Vec*)work.p;
    auto* G = (convolution_detail::Vec*)gs.p;
    auto* H = (convolution_detail::Vec*)hs.p;
    for (usize half = seed; half < count; half *= 2) {
        usize size = 2 * half, end = std::min(size, count);
        memcpy(gs.p, g.p, half * 4); std::fill(gs.p + half, gs.p + size, 0u);
        memcpy(hs.p, h.p, half * 4); std::fill(hs.p + half, hs.p + size, 0u);
        ntt.forward(G, size / 8); ntt.forward(H, size / 8);
        memcpy(work.p, gs.p, size * 4);
        ntt.products(x, G, size / 8); ntt.inverse(x, size / 8);
        std::fill(work.p, work.p + half, 0u);
        for (usize i = half; i < size; ++i) work[i] = M::sub(i < f.size() ? f[i] : 0, work[i]);
        ntt.forward(x, size / 8); ntt.products(x, H, size / 8); ntt.inverse(x, size / 8);
        for (usize i = half; i < end; ++i) g[i] = (work[i] + (work[i] & 1) * P) >> 1;
        if (size < count) {
            // Extend the reciprocal for the next precision using the new g.
            memcpy(work.p, g.p, size * 4);
            ntt.forward(x, size / 8); ntt.products(x, H, size / 8); ntt.inverse(x, size / 8);
            std::fill(work.p, work.p + half, 0u);
            ntt.forward(x, size / 8); ntt.products(x, H, size / 8); ntt.inverse(x, size / 8);
            for (usize i = half; i < size; ++i) h[i] = M::sub(0, work[i]);
        }
    }
    if (!first) { g.n = n; return g; }
    Buffer<u32> result(n); std::fill(result.p, result.p + n, 0u);
    memcpy(result.p + first / 2, g.p, count * 4);
    return result;
}
}
