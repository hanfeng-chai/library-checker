// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

// #include <atcoder/all>
// using mint = atcoder::modint998244353;

using ll = long long;
#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)

// const ll dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
// const ll dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
const ll dy[] = {-1, 0, 0, 1};
const ll dx[] = {0, -1, 1, 0};

template <class T, class T1, class T2> bool isrange(T target, T1 low, T2 high) { return low <= target && target < high; }
template <class T, class U> T min(const T &t, const U &u) { return t < u ? t : u; }
template <class T, class U> T max(const T &t, const U &u) { return t < u ? u : t; }
template <class T, class U> bool chmin(T &t, const U &u) { if (t > u) { t = u; return true; } return false; }
template <class T, class U> bool chmax(T &t, const U &u) { if (t < u) { t = u; return true; } return false; }
template<class K, class V> using hash_map = gp_hash_table<K, V>;
template<class K> using hash_set = gp_hash_table<K, null_type>;

// #include "titan_cpplib/others/io.cpp"
// #include "titan_cpplib/others/print.cpp"
// #include "titan_cpplib/ds/tree_contour_add.cpp"
/// https://github.com/titan-23/Library_cpp/blob/main/titan_cpplib/ds/tree_contour_add.cpp

#include <algorithm>
#include <cassert>
#include <vector>

// #include "titan_cpplib/graph/contour_index.cpp"
/// https://github.com/titan-23/Library_cpp/blob/main/titan_cpplib/graph/contour_index.cpp

#include <algorithm>
#include <bit>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;

namespace titan23 {

/// Shared index for distance range queries on a static tree
/// Construction is O(N log N)
class ContourIndex {
public:
    int n = 0;
    vector<int> path_off = {0}, path_dist, cpar, all_off = {0}, sub_off = {0};
    vector<uint8_t> path_dist8;
    vector<uint16_t> path_dist16;

    void build(const vector<vector<int>> &G) {
        n = (int)G.size();
        if (n == 0) return;

        vector<int> all_len, sub_len;
        all_len.reserve(n + 1);
        sub_len.reserve(n + 1);
        all_len.resize(n);
        sub_len.resize(n);
        cpar.assign(n, -1);

        {
            struct Record { int v, dist; };
            vector<int> path_len(n);
            const int levels = bit_width((unsigned)n) - 1;
            vector<Record> records;
            records.reserve(n * levels);
            int max_dist = 0;

            {
                path_off.resize(n + 1);
                vector<int> &adj_off = path_off;
                vector<int> adj;
                adj.reserve(2 * (n - 1));
                for (int v = 0; v < n; ++v) {
                    adj.insert(adj.end(), G[v].begin(), G[v].end());
                    adj_off[v + 1] = (int)adj.size();
                }
                vector<uint8_t> dead(n);
                vector<int> par(n), sz(n);

                struct State { int v, p, dist; };
                vector<State> states;
                states.reserve(n);

                auto add_entry = [&](int v, int dist) {
                    records.push_back({v, dist});
                    ++path_len[v];
                };

                auto select_centroid = [&](int v, int total) {
                    while (true) {
                        int next = -1;
                        for (int i = adj_off[v]; i < adj_off[v + 1]; ++i) {
                            const int x = adj[i];
                            if (dead[x]) continue;
                            const int part = par[x] == v ? sz[x] : total - sz[v];
                            if (part * 2 > total) {
                                next = x;
                                break;
                            }
                        }
                        if (next == -1) return v;
                        v = next;
                    }
                };

                auto find_centroid = [&](int start) {
                    records.clear();
                    states.clear();
                    states.push_back({start, -1, 0});
                    while (!states.empty()) {
                        const State cur = states.back();
                        states.pop_back();
                        records.push_back({cur.v, 0});
                        par[cur.v] = cur.p;
                        sz[cur.v] = 1;
                        for (int i = adj_off[cur.v]; i < adj_off[cur.v + 1]; ++i) {
                            const int x = adj[i];
                            if (x == cur.p) continue;
                            states.push_back({x, cur.v, 0});
                        }
                    }
                    for (int i = (int)records.size() - 1; i > 0; --i) {
                        const int v = records[i].v;
                        sz[par[v]] += sz[v];
                    }
                    return select_centroid(records[records.size() / 2].v, (int)records.size());
                };

                auto decompose = [&](auto &&self, int c, int cp, int clen) -> void {
                    cpar[c] = cp;
                    sub_len[c] = clen;
                    dead[c] = 1;
                    int all_max = 0;
                    for (int j = adj_off[c]; j < adj_off[c + 1]; ++j) {
                        const int root = adj[j];
                        if (dead[root]) continue;
                        int sub_max = 0;
                        const int first = (int)records.size();
                        states.clear();
                        states.push_back({root, c, 1});
                        while (!states.empty()) {
                            const State cur = states.back();
                            states.pop_back();
                            par[cur.v] = cur.p;
                            sz[cur.v] = 1;
                            add_entry(cur.v, cur.dist);
                            sub_max = max(sub_max, cur.dist);
                            for (int i = adj_off[cur.v]; i < adj_off[cur.v + 1]; ++i) {
                                const int x = adj[i];
                                if (x == cur.p || dead[x]) continue;
                                states.push_back({x, cur.v, cur.dist + 1});
                            }
                        }
                        const int last = (int)records.size();
                        for (int i = last - 1; i > first; --i) {
                            const int v = records[i].v;
                            sz[par[v]] += sz[v];
                        }
                        all_max = max(all_max, sub_max);
                        const int mid = first + (last - first) / 2;
                        self(self, select_centroid(records[mid].v, last - first), c, sub_max);
                    }
                    all_len[c] = all_max + 1;
                    max_dist = max(max_dist, all_max);
                };
                const int root = find_centroid(0);
                records.clear();
                decompose(decompose, root, -1, 0);
            }

            path_off.resize(n + 1);
            for (int v = 0; v < n; ++v) path_off[v + 1] = path_off[v] + path_len[v];
            for (int v = 0; v < n; ++v) path_len[v] = path_off[v + 1];
            auto scatter = [&](auto &out) {
                using D = typename decay_t<decltype(out)>::value_type;
                for (const Record &rec : records) {
                    out[--path_len[rec.v]] = static_cast<D>(rec.dist);
                }
            };
            if (max_dist <= UINT8_MAX) {
                path_dist8.resize(path_off.back());
                scatter(path_dist8);
            } else if (max_dist <= UINT16_MAX) {
                path_dist16.resize(path_off.back());
                scatter(path_dist16);
            } else {
                path_dist.resize(path_off.back());
                scatter(path_dist);
            }
        }

        all_len.push_back(0);
        int pref = 0;
        for (int i = 0; i < n; ++i) {
            const int len = all_len[i];
            all_len[i] = pref;
            pref += len;
        }
        all_len[n] = pref;
        all_off = move(all_len);

        sub_len.push_back(0);
        pref = all_off.back();
        for (int i = 0; i < n; ++i) {
            const int len = sub_len[i];
            sub_len[i] = pref;
            pref += len;
        }
        sub_len[n] = pref;
        sub_off = move(sub_len);
    }

    ContourIndex() = default;

    explicit ContourIndex(const vector<vector<int>> &G) {
        build(G);
    }

};

}  // namespace titan23
using namespace std;

namespace titan23 {

/// Distance range add and point get on a static tree
/// Operations are O(log^2 N)
template<typename T>
class TreeContourAdd {
private:
    ContourIndex idx;
    vector<T> base, bit;

    static void add_bit(T *bit, int l, int n, int k, const T &x) {
        for (; k < n; k |= k + 1) bit[l + k] += x;
    }

    static T get_bit(const T *bit, const int *off, int id, int k) {
        const int l = off[id];
        T res{};
        for (++k; k > 0; k &= k - 1) res += bit[l + k - 1];
        return res;
    }

    static void add_range(T *bit, const int *off, int id, int ql, int qr, const T &x, const T &nx) {
        const int l = off[id];
        const int n = off[id + 1] - l;
        if (ql < 0) ql = 0;
        if (qr > n) qr = n;
        if (ql >= qr) return;
        add_bit(bit, l, n, ql, x);
        if (qr < n) add_bit(bit, l, n, qr, nx);
    }

    template<typename D>
    void add_impl(int v, int l, int r, T x, const D *pd) {
        const T nx = T{} - x;
        T *b = bit.data();
        const int *po = idx.path_off.data();
        const int *ao = idx.all_off.data();
        const int *so = idx.sub_off.data();
        const int *cp = idx.cpar.data();
        int k = po[v];
        const int end = po[v + 1];
        int c = v;
        add_range(b, ao, c, l, r, x, nx);
        int child = c;
        c = cp[c];
        for (; k < end; ++k) {
            const int dist = pd[k];
            const int ql = l - dist;
            const int qr = r - dist;
            add_range(b, ao, c, ql, qr, x, nx);
            add_range(b, so, child, ql - 1, qr - 1, nx, x);
            child = c;
            c = cp[c];
        }
    }

    template<typename D>
    T get_impl(int v, const D *pd) const {
        T res = base[v];
        const T *b = bit.data();
        const int *po = idx.path_off.data();
        const int *ao = idx.all_off.data();
        const int *so = idx.sub_off.data();
        const int *cp = idx.cpar.data();
        int k = po[v];
        const int end = po[v + 1];
        int c = v;
        res += get_bit(b, ao, c, 0);
        int child = c;
        c = cp[c];
        for (; k < end; ++k) {
            const int dist = pd[k];
            res += get_bit(b, ao, c, dist);
            res += get_bit(b, so, child, dist - 1);
            child = c;
            c = cp[c];
        }
        return res;
    }

    void init(const vector<T> &a) {
        assert((int)a.size() == idx.n);
        base = a;
        bit.assign(idx.sub_off.back(), T{});
    }

public:
    TreeContourAdd() = default;

    explicit TreeContourAdd(const vector<vector<int>> &G) : idx(G) {
        init(vector<T>(G.size()));
    }

    TreeContourAdd(const vector<vector<int>> &G, const vector<T> &a) : idx(G) {
        init(a);
    }

    int len() const {
        return (int)base.size();
    }

    void add(int v, int l, int r, const T &x) {
        assert(0 <= v && v < len());
        l = clamp(l, 0, idx.n);
        r = clamp(r, 0, idx.n);
        if (l >= r) return;
        const T y = x;
        if (!idx.path_dist8.empty()) add_impl(v, l, r, y, idx.path_dist8.data());
        else if (!idx.path_dist16.empty()) add_impl(v, l, r, y, idx.path_dist16.data());
        else add_impl(v, l, r, y, idx.path_dist.data());
    }

    T get(int v) const {
        assert(0 <= v && v < len());
        if (!idx.path_dist8.empty()) return get_impl(v, idx.path_dist8.data());
        if (!idx.path_dist16.empty()) return get_impl(v, idx.path_dist16.data());
        return get_impl(v, idx.path_dist.data());
    }
};

}  // namespace titan23


void solve() {
	int n, q; cin >> n >> q;
	vector<ll> A(n);
	rep(i, n) cin >> A[i];
	vector<vector<int>> G(n);
	rep(i, n-1) {
		int u, v; cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	titan23::TreeContourAdd<ll> tree(G, A);
	rep(qdx, q) {
		int c; cin >> c;
		if (c == 0) {
			int p, l, r, x; cin >> p >> l >> r >> x;
			tree.add(p, l, r, x);
		} else {
			int p; cin >> p;
			cout << tree.get(p) << "\n";
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout << fixed << setprecision(15);
	cerr << fixed << setprecision(15);

	int t = 1;
	// cin >> t;
	for (int i = 0; i < t; ++i) {
		solve();
	}

	return 0;
}
