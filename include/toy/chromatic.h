#pragma once
#include <toy/clique.h>
#include <toy/set_series.h>
namespace toy {
// Exact DSATUR branch and bound, n <= 64. A maximum clique supplies the lower
// bound; saturation masks are restored after each color choice.
inline u32 chromatic_number(std::span<const u64> g) {
    u32 n = g.size();
    if (!n) return 0;
    u64 all = n == 64 ? ~0ull : (1ull << n) - 1;
    u32 lower = std::popcount(maximum_clique(g));
    u64 forbidden[64]{};
    auto choose = [&](u64 todo) {
        u64 remaining = todo;
        u32 best = 0, score = 0;
        while (todo) {
            u32 v = std::countr_zero(todo);
            todo &= todo - 1;
            u32 s = 1 + 128 * std::popcount(forbidden[v]) + std::popcount(g[v] & remaining);
            if (s > score) {
                score = s;
                best = v;
            }
        }
        return best;
    };
    u32 best = 0;
    for (u64 todo = all; todo;) {
        u32 v = choose(todo), c = std::countr_one(forbidden[v]);
        best = std::max(best, c + 1);
        todo ^= 1ull << v;
        u64 next = g[v] & todo;
        while (next) {
            u32 u = std::countr_zero(next);
            next &= next - 1;
            forbidden[u] |= 1ull << c;
        }
    }
    std::fill(forbidden, forbidden + n, 0ull);
    auto dfs = [&](auto &&self, u64 todo, u32 used) -> void {
        if (best == lower || used >= best) return;
        if (!todo) {
            best = used;
            return;
        }
        u32 v = choose(todo);
        todo ^= 1ull << v;
        u64 choices = (used >= 63 ? ~0ull : (1ull << (used + 1)) - 1) & ~forbidden[v];
        while (choices) {
            u32 c = std::countr_zero(choices);
            choices &= choices - 1;
            u64 neighbors = g[v] & todo, old[64];
            for (u64 bits = neighbors; bits; bits &= bits - 1) {
                u32 u = std::countr_zero(bits);
                old[u] = forbidden[u];
                forbidden[u] |= 1ull << c;
            }
            self(self, todo, std::max(used, c + 1));
            for (u64 bits = neighbors; bits; bits &= bits - 1) {
                u32 u = std::countr_zero(bits);
                forbidden[u] = old[u];
            }
        }
    };
    dfs(dfs, all, 0);
    return best;
}
template <u32 P = 998244353, bool Split = true>
Buffer<u32> chromatic_polynomial(std::span<const u32> g) {
    using M = Mod<P>;
    u32 n = g.size();
    Buffer<u32> answer(n + 1);
    std::fill(answer.p, answer.p + answer.n, 0u);
    if (!n) {
        answer[0] = 1;
        return answer;
    }
    for (u32 v = 0; v < n; ++v)
        if (g[v] >> v & 1) return answer;
    if constexpr (Split) {
        // Multiplicativity over components also removes isolated vertices.
        u32 unseen = (1u << n) - 1;
        Buffer<u32> product(1);
        product[0] = 1;
        while (unseen) {
            u32 group = unseen & -unseen, old = 0;
            while (old != group) {
                old = group;
                for (u32 v = 0; v < n; ++v)
                    if (group >> v & 1) group |= g[v];
            }
            unseen &= ~group;
            if (group == ((1u << n) - 1)) break;
            u32 k = std::popcount(group), map[32], id = 0;
            for (u32 v = 0; v < n; ++v)
                if (group >> v & 1) map[v] = id++;
            Buffer<u32> sub(k);
            std::fill(sub.p, sub.p + k, 0u);
            for (u32 v = 0; v < n; ++v)
                if (group >> v & 1)
                    for (u32 w = 0; w < n; ++w)
                        if (g[v] >> w & 1) sub[map[v]] |= 1u << map[w];
            auto part = chromatic_polynomial<P, Split>(sub);
            Buffer<u32> next(product.n + k);
            std::fill(next.p, next.p + next.n, 0u);
            for (u32 i = 0; i < product.n; ++i)
                for (u32 j = 0; j < part.n; ++j)
                    next[i + j] = M::add(next[i + j], M::mul(product[i], part[j]));
            product = std::move(next);
        }
        if (product.n == n + 1) return product;
        // A simplicial elimination gives a product of linear factors.
        u32 alive = (1u << n) - 1;
        Buffer<u32> factors(0, n);
        while (alive) {
            u32 v = 0;
            for (; v < n; ++v)
                if (alive >> v & 1) {
                    u32 neighbors = g[v] & alive;
                    bool clique = true;
                    for (u32 bits = neighbors; bits; bits &= bits - 1) {
                        u32 u = std::countr_zero(bits);
                        if ((g[u] & neighbors) != (neighbors ^ (1u << u))) {
                            clique = false;
                            break;
                        }
                    }
                    if (clique) break;
                }
            if (v == n) break;
            factors.p[factors.n++] = std::popcount(g[v] & alive);
            alive ^= 1u << v;
        }
        if (!alive) {
            answer[0] = 1;
            u32 degree = 0;
            for (u32 x : std::span(factors.p, factors.n)) {
                for (u32 j = ++degree; j; --j)
                    answer[j] = M::sub(answer[j - 1], M::mul(x, answer[j]));
                answer[0] = M::sub(0, M::mul(x, answer[0]));
            }
            return answer;
        }
    }
    u32 size = 1u << n, half = size / 2;
    Buffer<u32> independent(size);
    independent[0] = 1;
    for (u32 mask = 1; mask < size; ++mask) {
        u32 v = std::countr_zero(mask), rest = mask & (mask - 1);
        independent[mask] = independent[rest] && !(g[v] & rest);
    }
    Buffer<u32> weight(half);
    for (u32 mask = 0; mask < half; ++mask) weight[mask] = independent[size - 1 - mask];
    independent[0] = 0;
    auto powers = set_power_projection<P>(std::span<const u32>(independent.p, half),
                                          std::span<const u32>(weight), n);
    auto [fact, inverse] = factorials<P>(n);
    Buffer<u32> falling(n + 1);
    std::fill(falling.p, falling.p + falling.n, 0u);
    falling[0] = 1;
    for (u32 k = 1; k <= n; ++k) {
        for (u32 j = k; j; --j) falling[j] = M::sub(falling[j - 1], M::mul(k - 1, falling[j]));
        falling[0] = 0;
        u32 partitions = M::mul(powers[k - 1], inverse[k - 1]);
        for (u32 j = 0; j <= k; ++j) answer[j] = M::add(answer[j], M::mul(partitions, falling[j]));
    }
    return answer;
}
} // namespace toy
