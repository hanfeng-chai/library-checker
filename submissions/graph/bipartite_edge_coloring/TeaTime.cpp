#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define rep(i, n) for (int i = 0; i < (n); i += 1)
#define all(a) begin(a), end(a)
#define rall(a) rbegin(a), rend(a)
#define len(a) (int)((a).size())

mt19937 rd(228);

// Finds matching in bipartite regular graph in O(n log n)
struct bipartite_regular_matching {
    int n;
    vector<int> mtl, mtr, ord;
    vector<pair<int, int>> path;
    function<int(int)> sample_out;
    bipartite_regular_matching(int n, function<int(int)> sample_out) : n(n), mtl(n, -1), mtr(n, -1), ord(n), sample_out(sample_out) {
        path.reserve(3 * n + 20); // bound for default choice of b
        iota(all(ord), 0);
    }

    //bool truncated_walk(int v, int b) {
    //     path.clear();
    //     assert(mtl[v] == -1);
    //     ++b;
    //     while (b--) {
    //         int u = sample_out(v);
    //         if (u == mtl[v]) continue;
    //         path.push_back({v, u});
    //         if (mtr[u] == -1) {
    //             for (auto c : path) {
    //                 mtl[c.first] = c.second;
    //                 mtr[c.second] = c.first;
    //             }
    //             return true;
    //         }
    //         v = mtr[u];
    //     }
    //     return false;
    // }

    void walk(int s) {
        for (int u = s; u != -1;) {
            int v = sample_out(u);
            swap(mtr[v], u);
        }
    }

    void find_matching() {
        rep(i, n) {
            swap(ord[i], ord[i + rd() % (n - i)]);
            walk(ord[i]);
        }
        for (int i = 0; i < n; ++i) mtl[mtr[i]] = i;
    }
};

struct faster_bipartite_edge_coloring {
    vector<int> get_circuit(int n, const vector<pair<int, int>>& edges) {
        vector<int> pt(2 * n);
        vector<vector<int>> gr(2 * n);
        rep(i, len(edges)) {
            gr[edges[i].first].push_back(i);
            gr[edges[i].second + n].push_back(i);
        }
        vector<int> del(len(edges));
        vector<int> circuit;
        circuit.reserve(len(edges));
        auto eul = [&](int s) {
            if (pt[s] >= len(gr[s])) return;
            vector<int> st = {s}, indexes_st;
            while (!st.empty()) {
                int v = st.back();
                bool flag = false;
                while (pt[v] < len(gr[v])) {
                    int e_ind = gr[v][pt[v]];
                    if (del[e_ind]) {
                        ++pt[v];
                    } else {
                        del[e_ind] = 1;
                        st.push_back(v ^ edges[e_ind].first ^ (edges[e_ind].second + n));
                        indexes_st.push_back(e_ind);
                        ++pt[v];
                        flag = true;
                        break;
                    }
                }
                if (flag) continue;
                st.pop_back();
                if (!indexes_st.empty()) {
                    circuit.push_back(indexes_st.back());
                    indexes_st.pop_back();
                }
            }
        };
        rep(i, 2 * n) {
            eul(i);
        }
        return circuit;
    }
    vector<int> solve_pow2(int n, vector<pair<int, int>>& edges) {
        assert(!edges.empty());
        vector<int> degsl(n), degsr(n);
        for (auto c : edges) ++degsl[c.first], ++degsr[c.second];
        int pw = 1;
        while (pw < degsl[0]) pw *= 2;
        for (int i = 0; i < len(degsl); ++i) assert(degsl[i] == pw && degsr[i] == pw);
        if (pw == 1) return vector<int>(len(edges), 0);
        vector<int> circ = get_circuit(n, edges);
        vector<pair<int, int>> a(len(edges) / 2), b(len(edges) / 2);
        for (int i = 0; i < len(edges); i += 2) a[i / 2] = edges[circ[i]];
        for (int i = 1; i < len(edges); i += 2) b[(i - 1) / 2] = edges[circ[i]];
        vector<int> col1 = solve_pow2(n, a), col2 = solve_pow2(n, b);
        vector<int> ans(len(edges));
        for (int i = 0; i < len(col1); ++i) ans[circ[2 * i]] = col1[i];
        for (int i = 0; i < len(col2); ++i) ans[circ[2 * i + 1]] = col2[i] + pw / 2;
        return ans;
    }
    vector<int> solve_regular(int n, const vector<pair<int, int>>& edges) {
        if (edges.empty()) return {};
        vector<int> degl(n, 0), degr(n, 0);
        for (auto c : edges) degl[c.first]++, degr[c.second]++;
        int d = degl[0];
        rep(i, n) {
            assert(degl[i] == d && degr[i] == d);
        }
        if (d % 2 == 1) {
            vector<vector<int>> gr(n);
            for (auto c : edges) gr[c.first].push_back(c.second);
            auto sample_out = [&g = gr](int v) {
                return g[v][rd() % len(g[v])];
            };
            bipartite_regular_matching bip(n, sample_out);
            vector<int> taken(len(edges), 0);
            vector<pair<int, int>> rec;
            vector<int> ind;
            vector<int> ans(len(edges));
            bip.find_matching();
            rep(i, len(edges)) {
                if (bip.mtl[edges[i].first] == edges[i].second) {
                    bip.mtl[edges[i].first] = -1;
                    taken[i] = 1;
                    ans[i] = d - 1;
                } else {
                    rec.push_back(edges[i]);
                    ind.push_back(i);
                }
            }
            vector<int> colors = solve_regular(n, rec);
            rep(i, len(colors)) {
                ans[ind[i]] = colors[i];
            }
            return ans;
        } else {
            vector<int> circuit = get_circuit(n, edges);
            vector<pair<int, int>> rec, left;
            rec.reserve(len(edges) / 2);
            for (int i = 0; i < len(edges); i += 2) {
                rec.push_back(edges[circuit[i]]);
            }
            vector<int> colors = solve_regular(n, rec);
            int pw = 1;
            while (pw * 2 < d) pw *= 2;
            vector<int> degsl(n), degsr(n), inds(len(edges) / 2);
            left.reserve(len(edges) / 2);
            int curd = 0;
            for (int i = 1; i < len(edges); i += 2) {
                left.push_back(edges[circuit[i]]);
                inds[(i - 1) / 2] = circuit[i];
                degsl[edges[circuit[i]].first]++;
                degsr[edges[circuit[i]].second]++;
            }
            vector<int> ans(len(edges));
            curd = max(*max_element(all(degsl)), *max_element(all(degsr)));
            int addit_colors = pw - curd;
            for (int i = 0; i < len(rec); ++i) {
                if (colors[i] < addit_colors) {
                    left.push_back(rec[i]);
                    inds.push_back(circuit[i * 2]);
                } else {
                    ans[circuit[i * 2]] = colors[i] + pw - addit_colors;
                }
            }

            colors = solve_pow2(n, left);
            for (int i = 0; i < len(left); ++i) {
                ans[inds[i]] = colors[i];
            }
            
            return ans;
        }
    }
    vector<int> solve(int a, int b, const vector<pair<int, int>>& edges) {
        vector<int> degl(max(a, b), 0), degr(max(a, b), 0), mrgl(max(a, b), 0), mrgr(max(a, b), 0);
        iota(all(mrgl), 0);
        iota(all(mrgr), 0);
        for (auto c : edges) degl[c.first]++, degr[c.second]++;
        int d = max(*max_element(all(degl)), *max_element(all(degr)));

        int pr = 0, pt = 0;
        vector<int> tol(max(a, b)), tor(max(a, b));
        iota(all(tol), 0);
        iota(all(tor), 0);
        int newa = 1, newb = 1;
        for (int i = 1; i < a; ++i) {
            if (degl[pr] + degl[i] <= d) {
                degl[pr] += degl[i];
                mrgl[i] = pr;
                tol[i] = tol[pr];
            } else {
                ++newa;
                ++pt;
                tol[i] = pt;
                mrgl[i] = i;
                pr = i;
            }
        }
        pr = 0, pt = 0;
        for (int i = 1; i < b; ++i) {
            if (degr[pr] + degr[i] <= d) {
                degr[pr] += degr[i];
                mrgr[i] = pr;
                tor[i] = tor[pr];
            } else {
                ++newb;
                ++pt;
                tor[i] = pt;
                mrgr[i] = i;
                pr = i;
            }
        }
        vector<pair<int, int>> mrg_edges(len(edges));
        for (int i = 0; i < len(edges); ++i) {
            mrg_edges[i] = {tol[mrgl[edges[i].first]], tor[mrgr[edges[i].second]]};
        }
        pt = 0;
        int nodes = max(newa, newb);
        degl.assign(nodes, 0);
        degr.assign(nodes, 0);
        for (auto c : mrg_edges) ++degl[c.first], ++degr[c.second];
        for (int i = 0; i < nodes; ++i) {
            while (degl[i] < d) {
                ++degl[i];
                while (degr[pt] == d) ++pt;
                ++degr[pt];
                mrg_edges.push_back({i, pt});
            }
        }

        vector<int> colors = solve_regular(nodes, mrg_edges);
        colors.resize(len(edges));

        return colors;
    }

    vector<int> colors;
    faster_bipartite_edge_coloring(int a, int b, const vector<pair<int, int>>& edges) {
        colors = solve(a, b, edges);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, r, m;
    cin >> l >> r >> m;

    vector<pair<int, int>> edges(m);
    for (auto &c : edges) {
        cin >> c.first >> c.second;
    }

    faster_bipartite_edge_coloring coloring(l, r, edges);
    cout << (*max_element(all(coloring.colors))) + 1 << "\n";
    for (auto c : coloring.colors) cout << c << "\n";

    return 0;
} 
