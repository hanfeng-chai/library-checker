#pragma once
#include <toy/convolution.h>
#include <toy/mod.h>
namespace toy {
// Lowercase letters and '*'. Every mismatching pair contributes (a-b)^2;
// 625*m < 998244353 makes one NTT modulus exact throughout the supported range.
template <bool Filter = true>
Buffer<char> wildcard_match(std::string_view s, std::string_view t) {
    constexpr u32 P = 998244353;
    using M = Mod<P>;
    u32 n = s.size(), m = t.size(), count = n - m + 1;
    Buffer<char> answer(count);
    std::fill(answer.p, answer.p + count, '1');
    if constexpr (Filter) {
        std::array<u32, 26> cs{}, ct{};
        u32 nonzero_t = 0;
        for (char c : s)
            if (c != '*') ++cs[c - 'a'];
        for (char c : t)
            if (c != '*') {
                ++ct[c - 'a'];
                ++nonzero_t;
            }
        u64 work = 0;
        for (u32 c = 0; c < 26; ++c) work += u64(cs[c]) * (nonzero_t - ct[c]);
        if (!work) return answer;
        if (work <= 2ull * (n + m)) {
            std::array<Buffer<u32>, 26> left, right;
            for (u32 c = 0; c < 26; ++c) {
                left[c] = Buffer<u32>(0, cs[c]);
                right[c] = Buffer<u32>(0, ct[c]);
            }
            for (u32 i = 0; i < n; ++i)
                if (s[i] != '*') {
                    auto &a = left[s[i] - 'a'];
                    a.p[a.n++] = i;
                }
            for (u32 i = 0; i < m; ++i)
                if (t[i] != '*') {
                    auto &a = right[t[i] - 'a'];
                    a.p[a.n++] = i;
                }
            for (u32 a = 0; a < 26; ++a)
                for (u32 b = 0; b < 26; ++b)
                    if (a != b)
                        for (u32 x : std::span(left[a].p, left[a].n))
                            for (u32 y : std::span(right[b].p, right[b].n)) {
                                u32 offset = x - y;
                                if (offset < count) answer[offset] = '0';
                            }
            return answer;
        }
        // One bit per text position; each pattern letter intersects a shifted
        // compatibility bitmap. Empty alignment blocks are removed immediately.
        u32 width = (n + 63) / 64 + 1, blocks = (count + 63) / 64;
        Buffer<u64> masks(26 * width), alive(blocks);
        Buffer<u32> active(blocks);
        std::fill(alive.p, alive.p + blocks, ~0ull);
        std::iota(active.p, active.p + blocks, 0u);
        if (count % 64) alive[blocks - 1] = (1ull << (count % 64)) - 1;
        auto fill = [&](u32 w, const char *p) {
            auto a = _mm256_loadu_si256((const __m256i *)p),
                 b = _mm256_loadu_si256((const __m256i *)(p + 32)), star = _mm256_set1_epi8('*');
            u64 wildcard = u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, star))) |
                           (u64(u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(b, star)))) << 32);
            for (u32 c = 0; c < 26; ++c)
                if (ct[c]) {
                    auto letter = _mm256_set1_epi8('a' + c);
                    masks[c * width + w] =
                        wildcard | u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, letter))) |
                        (u64(u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(b, letter)))) << 32);
                }
        };
        for (u32 w = 0; w < n / 64; ++w) fill(w, s.data() + 64 * w);
        if (n % 64) {
            char tail[64]{};
            memcpy(tail, s.data() + n / 64 * 64, n % 64);
            fill(n / 64, tail);
        }
        for (u32 c = 0; c < 26; ++c)
            if (ct[c]) masks[c * width + width - 1] = 0;
        // A coprime stride visits every pattern position, spreading early
        // constraints across the pattern instead of repeating one long region.
        u32 step = std::max(1u, u32((u64(m) * 0x9e3779b9u) >> 32));
        while (std::gcd(step, m) != 1) ++step;
        u64 budget = 8ull * (n + m);
        bool complete = true;
        for (u32 seen = 0, j = 0; seen < m && active.n;
             ++seen, j = j + step < m ? j + step : j + step - m)
            if (t[j] != '*') {
                if (active.n > budget) {
                    complete = false;
                    break;
                }
                budget -= active.n;
                u32 keep = 0, shift = j % 64;
                const u64 *row = masks.p + (t[j] - 'a') * width + j / 64;
                for (u32 w : std::span(active.p, active.n)) {
                    u64 valid = row[w] >> shift;
                    if (shift) valid |= row[w + 1] << (64 - shift);
                    if (alive[w] &= valid) active[keep++] = w;
                }
                active.n = keep;
            }
        if (complete) {
            std::fill(answer.p, answer.p + count, '0');
            for (u32 w : std::span(active.p, active.n))
                for (u64 bits = alive[w]; bits; bits &= bits - 1)
                    answer[64 * w + std::countr_zero(bits)] = '1';
            return answer;
        }
    }
    u32 size = std::max(64u, std::bit_ceil(n + m - 1));
    Buffer<u32> sum(size), a(size), b(size);
    std::fill(sum.p, sum.p + size, 0u);
    const auto &ntt = convolution_detail::info<P>;
    for (u32 term = 0; term < 3; ++term) {
        std::fill(a.p, a.p + size, 0u);
        std::fill(b.p, b.p + size, 0u);
        for (u32 i = 0; i < n; ++i) {
            u32 x = s[i] == '*' ? 0 : s[i] - 'a' + 1;
            a[i] = term == 0 ? x * x : term == 1 ? bool(x) : x;
        }
        for (u32 i = 0; i < m; ++i) {
            u32 x = t[i] == '*' ? 0 : t[i] - 'a' + 1;
            b[m - 1 - i] = term == 0 ? bool(x) : term == 1 ? x * x : x;
        }
        ntt.forward((convolution_detail::Vec *)a.p, size / 8);
        ntt.forward((convolution_detail::Vec *)b.p, size / 8);
        ntt.products((convolution_detail::Vec *)a.p, (convolution_detail::Vec *)b.p, size / 8);
        for (u32 i = 0; i < size; ++i) {
            u32 x = std::min(a[i], a[i] - P);
            sum[i] = term < 2 ? M::add(sum[i], x) : M::sub(sum[i], M::add(x, x));
        }
    }
    ntt.inverse((convolution_detail::Vec *)sum.p, size / 8);
    for (u32 i = 0; i < count; ++i) answer[i] = sum[i + m - 1] ? '0' : '1';
    return answer;
}
} // namespace toy
