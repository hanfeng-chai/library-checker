#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
struct BoundedFlowNetwork {
    struct Arc {
        int u, v;
        T cap;
        U cost;
        Arc(int u, int v, T cap, U cost) : u(u), v(v), cap(cap), cost(cost) {}
    };

    int n, m;
    vector<Arc> arcs;
    vector<T> balance;
    vector<U> potential;
    vector<pair<int, int>> parent;
    vector<int> depth, next, prev;
    U lb_offset;

    BoundedFlowNetwork(int n) : n(n), lb_offset(0), balance(n + 1, 0), potential(n + 1, 0), parent(n + 1, {-1, -1}),
                                depth(n + 1, 1), next(2 * (n + 1), 0), prev(2 * (n + 1), 0) {}

    void add_supply(int v, T b) {
        balance[v] += b;
    }

    void add_demand(int v, T b) {
        balance[v] -= b;
    }

    int add_arc(int u, int v, T lb, T ub, U cost = 0) {
        arcs.emplace_back(u, v, ub - lb, cost);
        arcs.emplace_back(v, u, 0, -cost);
        lb_offset += lb * cost;
        add_supply(v, lb);
        add_demand(u, lb);
        return arcs.size() - 2;
    }

    void connect(int u, int v) {
        next[u] = v;
        prev[v] = u;
    }

    int build_spanning_tree(int s = -1, int t = -1) {
        m = arcs.size();
        U penalty = 1;
        for (int e = 0; e < m; e += 2) {
            U c = arcs[e].cost;
            if (c < 0) c = -c;
            penalty += c;
        }
        connect(2 * n, 2 * n + 1);
        connect(2 * n + 1, 2 * n);
        for (int i = 0; i < n; i++) {
            int u = n, v = i;
            T b = balance[i];
            if (b < 0) {
                b = -b;
                swap(u, v);
            }
            int e = add_arc(u, v, 0, b, -penalty);
            e ^= arcs[e].u != i;
            parent[i] = {n, e};
            potential[i] = potential[n] - arcs[e].cost;
            connect(2 * i, 2 * i + 1);
            connect(2 * i + 1, next[2 * n]);
            connect(2 * n, 2 * i);
        }

        if (~s && ~t) return add_arc(t, s, 0, numeric_limits<T>::max() >> 2, -penalty) ^ 1;
        return -1;
    }

    void network_simplex() {
        auto reduced_cost = [&](int e) {
            auto [u, v, cap, cost] = arcs[e];
            return cost + potential[u] - potential[v];
        };

        depth[n] = 0;
        auto pivot = [&](int in) {
            auto [u, v, cap, cost] = arcs[in];
            U phi = cost + potential[u] - potential[v];

            T flow = arcs[in].cap;
            int out = in, dir = -1, b = -1;
            auto climb = [&](int x, int y) {
                auto walk = [&](int &a, int d, int diff = 1) {
                    for (; diff; diff--, a = parent[a].first) {
                        int e = parent[a].second;
                        T f = arcs[e ^ !d].cap;
                        if (make_pair(flow, out) > make_pair(f, e)) {
                            tie(flow, out) = tie(f, e);
                            dir = d;
                            b = a;
                        }
                    }
                };
                if (depth[x] >= depth[y]) walk(x, 0, depth[x] - depth[y]);
                else walk(y, 1, depth[y] - depth[x]);

                while (x != y) {
                    walk(x, 0);
                    walk(y, 1);
                }
                return x;
            };
            int lca = climb(u, v);
            arcs[in].cap -= flow;
            arcs[in ^ 1].cap += flow;
            auto augment = [&](int a, int d) {
                for (; a != lca; a = parent[a].first) {
                    int e = parent[a].second;
                    arcs[e ^ !d].cap -= flow;
                    arcs[e ^ d].cap += flow;
                }
            };
            augment(u, 0);
            augment(v, 1);

            if (in == out || dir == -1 || b == -1) return;

            auto basis_exchange = [&](int a, int par, int par_e) {
                auto update = [&](int a, int par) {
                    for (int t = a, d = depth[par >> 1]; t != a + 1; t = next[t])
                        if (!(t & 1)) {
                            potential[t >> 1] += phi;
                            depth[t >> 1] = ++d;
                        } else d--;

                    connect(prev[a], next[a + 1]);
                    connect(a + 1, next[par]);
                    connect(par, a);
                };

                do {
                    update(a << 1, par << 1);
                    if (a == b) break;
                    auto [t, e] = parent[a];
                    parent[a] = {par, par_e};
                    par = exchange(a, t);
                    par_e = e ^ 1;
                } while (true);
                parent[b] = {par, par_e};
            };

            if (!dir) {
                phi = -phi;
                basis_exchange(u, v, in);
            } else basis_exchange(v, u, in ^ 1);
        };

        auto basis_edge = [&](int e) {
            auto [u, v, cap, cost] = arcs[e];
            int a = parent[u].second;
            if (~a && (a >> 1) == (e >> 1)) return true;
            int b = parent[v].second;
            if (~b && (b >> 1) == (e >> 1)) return true;
            return false;
        };

        list<int> candidates;
        for (int arc = 0, aug = arcs.size(), size = max(64, (int) sqrt(aug / 2)), len = size / 4, k = len / 10, count = 0;;) {
            for (int _ = 0; _ < k && !candidates.empty(); _++) {
                U cost = 0;
                int in = -1;
                for (auto it = candidates.begin(); it != candidates.end();) {
                    int e = *it;
                    U c = 0;
                    if (arcs[e].cap <= 0 || basis_edge(e) || (c = reduced_cost(e)) >= 0) {
                        it = candidates.erase(it);
                        continue;
                    }
                    if (make_pair(cost, in) > make_pair(c, e)) tie(cost, in) = tie(c, e);
                    it++;
                }
                if (!~in) break;
                pivot(in);
                count = 0;
            }
            candidates.clear();

            U cost = 0;
            int in = -1;
            for (int block = count + size; count < block; count++, ++arc %= aug)
                if (!basis_edge(arc) && arcs[arc].cap > 0) {
                    U c = reduced_cost(arc);
                    if (c < 0) {
                        if (candidates.size() < len) candidates.emplace_back(arc);
                        if (make_pair(cost, in) > make_pair(c, arc)) tie(cost, in) = tie(c, arc);
                    }
                }

            if (candidates.empty()) {
                if (count >= aug) break;
                continue;
            }
            pivot(in);
            count = 0;
        }
    }

    bool feasible() {
        for (int i = 0; i < n; i++)
            if (arcs[(i << 1) + m].cap) return false;
        return true;
    }

    U min_cost() {
        U cost = lb_offset;
        for (int e = 0; e < m; e += 2) cost += arcs[e].cost * arcs[e ^ 1].cap;
        return cost;
    }

    tuple<T, U, bool> min_cost_max_flow(int s, int t) {
        int e = build_spanning_tree(s, t);
        network_simplex();
        if (!feasible()) return {(U) 0, (T) 0, false};
        return {arcs[e].cap, min_cost(), true};
    }

    pair<U, bool> min_cost_b_flow() {
        build_spanning_tree();
        network_simplex();
        if (!feasible()) return {(U) 0, false};
        return {min_cost(), true};
    }
};

namespace std {
    ostream& operator<<(ostream& os, __int128 x) {
        const long long M = 1e18;
        if (x < 0) {
            os << '-' << (-x);
        } else if (x < M) {
            os << (long long) x;
        } else {
            os << x / M << setfill('0') << setw(18) << (long long) (x % M) << setw(0);
        }
        return os;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    if (n == 0) {
        cout << 0;
        exit(0);
    }

    BoundedFlowNetwork<__int128, __int128> bfn(n);
    for (int i = 0; i < n; i++) {
        int b;
        cin >> b;

        bfn.add_supply(i, b);
    }

    vector<int> lo(m);
    for (int i = 0; i < m; i++) {
        int a, b, l, u, k;
        cin >> a >> b >> l >> u >> k;

        bfn.add_arc(a, b, l, u, k);
        lo[i] = l;
    }

    auto [cost, valid] = bfn.min_cost_b_flow();
    if (!valid) cout << "infeasible";
    else {
        cout << cost << "\n";
        for (int i = 0; i < n; i++) cout << bfn.potential[i] - bfn.potential[0] << "\n";
        for (int i = 0; i < m; i++) cout << bfn.arcs[(i << 1) ^ 1].cap + lo[i] << "\n";
    }
}