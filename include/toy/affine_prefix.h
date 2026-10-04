#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>
namespace toy {
// Montgomery-encoded coefficients, nonzero slopes modulo a prime P. Store exclusive inverse
// prefixes; updates conjugate one delta.
template <u32 P = 998244353>
struct AffinePrefixTree {
    using R = Montgomery<P>;
    struct alignas(64) Node {
        u32 a[16], b[16];
    };
    struct Fraction {
        u32 numerator, denominator;
    };
    Buffer<Node> tree;
    std::array<u32, 9> offset{}, groups{};
    u32 height = 0;
    std::array<Node *, 9> layer{};
    explicit AffinePrefixTree(std::span<const Affine<P>> values) {
        u32 size = 0, n = values.size() + 1;
        do {
            offset[height] = size;
            groups[height] = (n + 15) / 16;
            size += groups[height++] + 1;
            n = (n + 15) / 16;
        } while (n > 1);
        usize bytes = (size * sizeof(Node) + (1 << 21) - 1) & -usize(1 << 21);
        tree.p = (Node *)aligned_alloc(1 << 21, bytes);
        tree.n = tree.capacity = size;
        madvise(tree.p, bytes, MADV_HUGEPAGE);
        Buffer<Affine<P>> totals(size);
        Buffer<u32> prefix(size);
        u32 product = R::one;
        for (u32 k = 0; k < height; ++k) {
            u32 children = k ? groups[k - 1] : values.size();
            for (u32 i = 0; i < groups[k]; ++i) {
                auto &node = tree[offset[k] + i];
                u32 a = R::one, b = 0;
                for (u32 j = 0; j < 16; ++j) {
                    u32 child = 16 * i + j;
                    Affine<P> f{R::one, 0};
                    if (child < children) f = k ? totals[offset[k - 1] + child] : values[child];
                    node.b[j] = b;
                    a = R::multiply(f.a, a);
                    b = R::add(R::multiply(f.a, b), f.b);
                }
                u32 at = offset[k] + i;
                totals[at] = {a, b};
                prefix[at] = product;
                product = R::multiply(product, a);
            }
            auto &sentinel = tree[offset[k] + groups[k]];
            std::fill(sentinel.a, sentinel.a + 16, R::one);
            std::fill(sentinel.b, sentinel.b + 16, 0u);
        }
        u32 inverse = R::power(product, P - 2);
        for (u32 k = height; k--;) {
            u32 children = k ? groups[k - 1] : values.size();
            for (u32 i = groups[k]; i--;) {
                u32 at = offset[k] + i;
                auto &node = tree[at];
                u32 current = R::multiply(inverse, prefix[at]);
                inverse = R::multiply(inverse, totals[at].a);
                for (u32 j = 16; j--;) {
                    u32 child = 16 * i + j;
                    u32 a = child < children
                                ? (k ? totals[offset[k - 1] + child].a : values[child].a)
                                : R::one;
                    current = R::multiply(current, a);
                    node.a[j] = current;
                    node.b[j] = R::subtract(0, R::multiply(node.b[j], current));
                }
            }
        }
        for (u32 k = 0; k < height; ++k) layer[k] = tree.p + offset[k];
    }
    void prefetch(u32 l, u32 r) const {
        for (u32 k = 0; k < std::min(height, 2u); ++k, l >>= 4, r >>= 4) {
            const auto *a = layer[k] + l / 16;
            const auto *b = layer[k] + r / 16;
            __builtin_prefetch(a->a, 0, 3);
            __builtin_prefetch(a->b, 0, 3);
            __builtin_prefetch(b->a, 0, 3);
            __builtin_prefetch(b->b, 0, 3);
        }
    }
    static u32 multiply_sum(u32 a, u32 b, u32 c, u32 d) {
        u64 z = u64(a) * b + u64(c) * d;
        u32 x = (z + u64(u32(z) * Mod<P>::inverse) * P) >> 32;
        return std::min(x, x - 2 * P);
    }
    // scale and translation encode replacement^-1 o old.
    void change(u32 position, u32 scale, u32 translation, u32 other) {
        u32 scalar = R::decode(scale), quotient = (u64(scalar) << 32) / P;
        auto m = _mm256_set1_epi32(scalar), q = _mm256_set1_epi32(quotient),
             p = _mm256_set1_epi32(P), lanes = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        auto multiply = [&](__m256i x) {
            auto even = _mm256_mul_epu32(x, q), odd = _mm256_mul_epu32(_mm256_srli_epi64(x, 32), q);
            auto high = _mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa);
            return _mm256_sub_epi32(_mm256_mullo_epi32(x, m), _mm256_mullo_epi32(high, p));
        };
        for (u32 k = 0; k < height; ++k, position >>= 4) {
            auto &node = layer[k][position / 16];
            u32 digit = position & 15;
            translation = multiply_sum(node.a[digit], translation, other, node.b[digit]);
            auto shift = _mm256_set1_epi32(translation);
            for (u32 j = (digit + 1) & -8u; j < 16; j += 8) {
                auto selected = _mm256_cmpgt_epi32(lanes, _mm256_set1_epi32(int(digit) - int(j)));
                auto *ap = (__m256i *)(node.a + j);
                auto *bp = (__m256i *)(node.b + j);
                auto a = _mm256_load_si256(ap), b = _mm256_load_si256(bp);
                _mm256_store_si256(ap, _mm256_blendv_epi8(a, multiply(a), selected));
                _mm256_store_si256(bp, _mm256_blendv_epi8(b, R::add(multiply(b), shift), selected));
            }
        }
    }
    void change(u32 position, u32 scale, u32 translation) {
        change(position, scale, translation, R::subtract(R::one, scale));
    }
    template <u32 Top>
    Fraction query_fixed(u32 l, u32 r, u32 x) const {
        u32 a = R::encode(x), b = 0, denominator = R::one;
        [&]<usize... K>(std::index_sequence<K...>) {
            (([&] {
                 constexpr u32 k = K;
                 u32 le = l >> (4 * k), re = r >> (4 * k);
                 const auto &first = layer[k][le / 16];
                 const auto &last = layer[k][re / 16];
                 a = R::add(R::multiply(first.a[le & 15], a), first.b[le & 15]);
                 if constexpr (k == 0) {
                     b = last.b[re & 15];
                     denominator = last.a[re & 15];
                 } else {
                     b = R::add(R::multiply(last.a[re & 15], b), last.b[re & 15]);
                     denominator = R::multiply(denominator, last.a[re & 15]);
                 }
             }()),
             ...);
        }(std::make_index_sequence<Top + 1>{});
        return {R::subtract(a, b), denominator};
    }
    Fraction query(u32 l, u32 r, u32 x) const {
        if (l == r) return {R::encode(x), R::one};
        switch ((std::bit_width(l ^ r) - 1) / 4) {
        case 0:
            return query_fixed<0>(l, r, x);
        case 1:
            return query_fixed<1>(l, r, x);
        case 2:
            return query_fixed<2>(l, r, x);
        case 3:
            return query_fixed<3>(l, r, x);
        case 4:
            return query_fixed<4>(l, r, x);
        case 5:
            return query_fixed<5>(l, r, x);
        case 6:
            return query_fixed<6>(l, r, x);
        default:
            return query_fixed<7>(l, r, x);
        }
    }
};
} // namespace toy
