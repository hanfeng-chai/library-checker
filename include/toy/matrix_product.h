#pragma once
#include <toy/mod.h>
#include <toy/page_buffer.h>
namespace toy {
namespace matrix_detail {
template <u32 P>
struct Product {
    using M = Mod<P>;
    // Eight independent accumulators form a 4x8 microkernel. After eight
    // products, subtract 2P from each high word; the residue modulo P is intact.
    template <u32 Size = 0>
    [[gnu::noinline]] static void leaf(const u32 *a, const u32 *b, u32 *c, u32 n, u32 m, u32 k) {
        if constexpr (Size) n = m = k = Size;
        auto limit = _mm256_set1_epi64x(u64(2 * P) << 32);
        for (u32 i = 0; i < n; i += 4)
            for (u32 j = 0; j < k; j += 8) {
                __m256i acc[4][2];
                for (auto &r : acc) r[0] = r[1] = _mm256_setzero_si256();
                for (u32 first = 0; first < m; first += 8) {
                    for (u32 z = first; z < first + 8; ++z) {
                        auto x = _mm256_loadu_si256((const __m256i *)(b + usize(z) * k + j)),
                             y = _mm256_srli_epi64(x, 32);
#pragma GCC unroll 4
                        for (u32 r = 0; r < 4; ++r) {
                            auto scale = _mm256_set1_epi32(a[usize(i + r) * m + z]);
                            acc[r][0] = _mm256_add_epi64(acc[r][0], _mm256_mul_epu32(x, scale));
                            acc[r][1] = _mm256_add_epi64(acc[r][1], _mm256_mul_epu32(y, scale));
                        }
                    }
                    for (auto &r : acc)
                        for (auto &x : r) x = _mm256_min_epu32(x, _mm256_sub_epi32(x, limit));
                }
                for (u32 r = 0; r < 4; ++r)
                    _mm256_storeu_si256((__m256i *)(c + usize(i + r) * k + j),
                                        M::mont_sum8(acc[r][0], acc[r][1]));
            }
    }
    template <bool Subtract = false>
    static void combine(const u32 *a, const u32 *b, u32 *c, u32 count) {
        for (u32 i = 0; i < count; i += 8) {
            auto x = _mm256_loadu_si256((const __m256i *)(a + i)),
                 y = _mm256_loadu_si256((const __m256i *)(b + i));
            _mm256_storeu_si256((__m256i *)(c + i), Subtract ? M::sub(x, y) : M::add(x, y));
        }
    }
    [[gnu::noinline]] static void multiply(const u32 *a, const u32 *b, u32 *c, u32 n, u32 m, u32 k,
                                           u32 *work, u32 depth) {
        if (!depth) {
            if (n == 64 && m == 64 && k == 64)
                leaf<64>(a, b, c, n, m, k);
            else
                leaf(a, b, c, n, m, k);
            return;
        }
        n /= 2;
        m /= 2;
        k /= 2;
        --depth;
        auto a00 = a, a01 = a + n * m, a10 = a + 2 * n * m, a11 = a + 3 * n * m;
        auto b00 = b, b01 = b + m * k, b10 = b + 2 * m * k, b11 = b + 3 * m * k;
        auto c00 = c, c01 = c + n * k, c10 = c + 2 * n * k, c11 = c + 3 * n * k;
        u32 *s = work, *t = s + n * m, *p = t + m * k;
        work = p + n * k;
        // Winograd's seven products and fifteen additions; children reuse work.
        multiply(a00, b00, c11, n, m, k, work, depth);
        multiply(a01, b10, c00, n, m, k, work, depth);
        combine(c00, c11, c00, n * k);
        combine(a10, a11, s, n * m);
        combine<true>(b01, b00, t, m * k);
        multiply(s, t, c01, n, m, k, work, depth);
        combine<true>(s, a00, s, n * m);
        combine<true>(b11, t, t, m * k);
        multiply(s, t, c10, n, m, k, work, depth);
        combine(c11, c10, c10, n * k);
        combine<true>(a01, s, s, n * m);
        multiply(s, b11, p, n, m, k, work, depth);
        combine(c10, c01, c11, n * k);
        combine(c11, p, c01, n * k);
        combine<true>(t, b10, t, m * k);
        multiply(a11, t, p, n, m, k, work, depth);
        combine<true>(c10, p, c10, n * k);
        combine<true>(a00, a10, s, n * m);
        combine<true>(b11, b01, t, m * k);
        multiply(s, t, p, n, m, k, work, depth);
        combine(c10, p, c10, n * k);
        combine(c11, p, c11, n * k);
    }
    template <class F>
    static void visit(u32 *p, u32 n, u32 m, u32 depth, u32 row, u32 col, F &emit) {
        if (!depth) {
            emit(p, n, m, row, col);
            return;
        }
        n /= 2;
        m /= 2;
        --depth;
        visit(p, n, m, depth, row, col, emit);
        visit(p + n * m, n, m, depth, row, col + m, emit);
        visit(p + 2 * n * m, n, m, depth, row + n, col, emit);
        visit(p + 3 * n * m, n, m, depth, row + n, col + m, emit);
    }
};
} // namespace matrix_detail
// Row-major canonical residues; only the left operand is Montgomery-encoded.
template <u32 P = 998244353, bool Strassen = true>
Buffer<u32> matrix_product(u32 n, u32 m, u32 k, std::span<const u32> a, std::span<const u32> b) {
    using K = matrix_detail::Product<P>;
    using M = Mod<P>;
    Buffer<u32> answer(usize(n) * k);
    if (!n || !m || !k) {
        std::fill(answer.p, answer.p + answer.n, 0u);
        return answer;
    }
    u32 pn = (n + 3) & ~3u, pm = (m + 7) & ~7u, pk = (k + 7) & ~7u, depth = 0;
    if constexpr (Strassen)
        if (std::min({n, m, k}) > 64) {
            pn = (n + 31) & ~31u;
            pm = (m + 31) & ~31u;
            pk = (k + 31) & ~31u;
            for (u32 x = pn, y = pm, z = pk;
                 std::min({x, y, z}) > 64 && !(x % 16 || y % 16 || z % 16); x /= 2, y /= 2, z /= 2)
                ++depth;
        }
    usize entries = usize(pn) * pm + usize(pm) * pk + usize(pn) * pk;
    auto buffer = page_buffer<u32>(entries + entries / 3 + 64);
    u32 *ap = buffer.p, *bp = ap + usize(pn) * pm, *cp = bp + usize(pm) * pk,
        *work = cp + usize(pn) * pk;
    std::fill(ap, cp, 0u);
    auto pack = [&](std::span<const u32> source, u32 rows, u32 cols, u32 *target, u32 nr, u32 nc) {
        auto emit = [&](u32 *p, u32 h, u32 w, u32 r, u32 c) {
            if (r >= rows || c >= cols) return;
            u32 width = std::min(w, cols - c);
            for (u32 i = 0; i < h && r + i < rows; ++i)
                memcpy(p + usize(i) * w, source.data() + usize(r + i) * cols + c, width * 4);
        };
        K::visit(target, nr, nc, depth, 0, 0, emit);
    };
    pack(a, n, m, ap, pn, pm);
    pack(b, m, k, bp, pm, pk);
    auto scale = _mm256_set1_epi32(M::r2);
    for (usize i = 0; i < usize(pn) * pm; i += 8)
        _mm256_storeu_si256((__m256i *)(ap + i),
                            M::mont(_mm256_loadu_si256((const __m256i *)(ap + i)), scale));
    K::multiply(ap, bp, cp, pn, pm, pk, work, depth);
    auto unpack = [&](u32 *p, u32 h, u32 w, u32 r, u32 c) {
        if (r >= n || c >= k) return;
        u32 width = std::min(w, k - c);
        for (u32 i = 0; i < h && r + i < n; ++i)
            memcpy(answer.p + usize(r + i) * k + c, p + usize(i) * w, width * 4);
    };
    K::visit(cp, pn, pk, depth, 0, 0, unpack);
    return answer;
}
} // namespace toy
