#include <cstdint>
#include <iostream>
#include <vector>

namespace {

constexpr int64_t MOD = 998244353LL;

struct Mask {
    uint64_t lo = 0;
    uint64_t hi = 0;
};

inline Mask and_mask(const Mask &a, const Mask &b) {
    return Mask{a.lo & b.lo, a.hi & b.hi};
}

inline bool any_mask(const Mask &m) {
    return (m.lo | m.hi) != 0ULL;
}

inline void set_bit(Mask &m, int v) {
    if (v < 64) {
        m.lo |= (1ULL << v);
    } else {
        m.hi |= (1ULL << (v - 64));
    }
}

inline int pop_lsb(Mask &m) {
    if (m.lo != 0ULL) {
        const int v = __builtin_ctzll(m.lo);
        m.lo &= (m.lo - 1ULL);
        return v;
    }
    const int v = __builtin_ctzll(m.hi);
    m.hi &= (m.hi - 1ULL);
    return v + 64;
}

inline Mask all_mask_n(int n) {
    Mask m{};
    if (n <= 0) return m;

    if (n >= 64) {
        m.lo = ~0ULL;
    } else {
        m.lo = (1ULL << n) - 1ULL;
    }

    const int hi_bits = n - 64;
    if (hi_bits <= 0) {
        m.hi = 0ULL;
    } else if (hi_bits >= 64) {
        m.hi = ~0ULL;
    } else {
        m.hi = (1ULL << hi_bits) - 1ULL;
    }
    return m;
}

struct Solver {
    int n = 0;
    std::vector<int64_t> x;
    std::vector<Mask> adj;
    int64_t ans = 0;

    void dfs(Mask candidates, int64_t prod) {
        while (any_mask(candidates)) {
            const int v = pop_lsb(candidates);
            const int64_t next_prod = (prod * x[v]) % MOD;
            ans += next_prod;
            if (ans >= MOD) ans -= MOD;

            const Mask next_candidates = and_mask(candidates, adj[v]);
            dfs(next_candidates, next_prod);
        }
    }
};

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N, M;
    std::cin >> N >> M;

    Solver solver;
    solver.n = N;
    solver.x.assign(N, 0);
    solver.adj.assign(N, Mask{});

    for (int i = 0; i < N; ++i) {
        int64_t v;
        std::cin >> v;
        solver.x[i] = v % MOD;
    }

    for (int i = 0; i < M; ++i) {
        int u, v;
        std::cin >> u >> v;
        set_bit(solver.adj[u], v);
        set_bit(solver.adj[v], u);
    }

    const Mask all = all_mask_n(N);
    solver.dfs(all, 1);

    std::cout << solver.ans << '\n';
    return 0;
}
