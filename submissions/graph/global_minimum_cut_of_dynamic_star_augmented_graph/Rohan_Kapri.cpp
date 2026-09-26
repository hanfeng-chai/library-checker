#include <bits/stdc++.h>
#define sz(v) ((int)(v).size())
#define all(v) (v).begin(), (v).end()
#define cr(v, n) (v).clear(), (v).resize(n);
using namespace std;
using lint = long long;
using pi = array<lint, 2>;

// A nonempty subset X \subseteq V is an extreme set of f (cut fn. here)
// if f(Y) > f(X) holds for all proper nonempty subset of X.
// They form a laminar family, which follows from the submodularity of f.
//
// Given a simple weighted undirected graph as a list of edges
// returns a (laminar representation) tree of 2n-1 vertices, rooted at 2n-2
// where for each rooted subtree, its set of leafs form a extreme set
// specifically, the largest nontrivial subtree is a global min-cut.
//
// O(nm log n)
// Credit: Ayanaa6 (https://judge.yosupo.jp/submission/216101)

vector<vector<int>> EVS(int n, vector<array<lint, 3>> edges) {
    vector<vector<pi>> g(2 * n - 1);
    for (auto &[u, v, w] : edges) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    vector<vector<int>> tr = vector<vector<int>>(2 * n - 1);
    vector<lint> used(2 * n - 1, 1);
    for (int i = 0; i < n - 1; i++) {
        g.push_back(vector<pi>());
        vector<int> ord(n - i), vis(n + i);
        vector<lint> cost(n + i);
        priority_queue<pi, vector<pi>, greater<>> que;
        for (int j = 0; j < n + i; j++)
            if (used[j]) {
                for (auto p : g[j])
                    cost[j] += p[1];
                que.push({cost[j], j});
            }
        for (int j = 0; j < n - i; j++)
            while (1) {
                auto pr = que.top();
                que.pop();
                int v = pr[1];
                if (vis[v])
                    continue;
                vis[v] = 1;
                ord[j] = v;
                for (auto p : g[v]) {
                    int u = p[0];
                    if (!vis[u]) {
                        cost[u] -= p[1];
                        que.push({cost[u], u});
                    }
                }
                break;
            }
        int v0 = ord[n - i - 1], v1 = ord[n - i - 2];
        g[v0].clear(), g[v1].clear();
        used[v0] = used[v1] = 0;
        tr[n + i].push_back(v0);
        tr[n + i].push_back(v1);
        for (int j = 0; j < n + i; j++)
            if (used[j])
                for (auto &p : g[j]) {
                    int u = p[0];
                    if (u == v0 || u == v1) {
                        p[0] = n + i;
                        g[n + i].push_back({j, p[1]});
                    }
                }
    }
    return tr;
}

vector<vector<int>> gph;
vector<int> din, dout, chn, sub, dep;
int par[14][8050];

void dfs(int x) {
    sub[x] = 1;
    for (auto &y : gph[x]) {
        par[0][y] = x;
        dep[y] = dep[x] + 1;
        dfs(y);
        sub[x] += sub[y];
    }
    sort(all(gph[x]), [&](int a, int b) { return sub[a] > sub[b]; });
}

int piv;

int lca(int u, int v) {
    if (dep[u] > dep[v])
        swap(u, v);
    int dx = dep[v] - dep[u];
    for (int i = 0; dx; i++) {
        if (dx & 1) {
            v = par[i][v];
        }
        dx >>= 1;
    }
    if (u == v)
        return u;
    for (int i = 13; i >= 0; i--) {
        if (par[i][u] != par[i][v]) {
            u = par[i][u];
            v = par[i][v];
        }
    }
    return par[0][u];
}

void hld(int x) {
    din[x] = piv++;
    if (sz(gph[x])) {
        chn[gph[x][0]] = chn[x];
        hld(gph[x][0]);
    }
    for (int i = 1; i < sz(gph[x]); i++) {
        chn[gph[x][i]] = gph[x][i];
        hld(gph[x][i]);
    }
    dout[x] = piv;
}

const int MAXT = 18000;

struct seg {
    lint tree[MAXT], lazy[MAXT];
    void add(int s, int e, int ps, int pe, int p, lint v) {
        if (e < ps || pe < s)
            return;
        if (s <= ps && pe <= e) {
            tree[p] += v;
            lazy[p] += v;
            return;
        }
        for (int i = 2 * p; i < 2 * p + 2; i++) {
            tree[i] += lazy[p];
            lazy[i] += lazy[p];
        }
        lazy[p] = 0;
        int pm = (ps + pe) / 2;
        add(s, e, ps, pm, 2 * p, v);
        add(s, e, pm + 1, pe, 2 * p + 1, v);
        tree[p] = min(tree[2 * p], tree[2 * p + 1]);
    }
} seg;
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, q;
    cin >> n >> m >> q;
    vector<int> a(n);
    for (auto &x : a)
        cin >> x;
    vector<array<lint, 3>> edges(m);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v >> w;
    }
    gph = EVS(n, edges);
    cr(din, 2 * n - 1);
    cr(dout, 2 * n - 1);
    cr(chn, 2 * n - 1);
    cr(sub, 2 * n - 1);
    cr(dep, 2 * n - 1);
    dfs(2 * n - 2);
    chn[2 * n - 2] = 2 * n - 2;
    hld(2 * n - 2);
    par[0][2 * n - 2] = 2 * n - 2;
    vector<lint> val(2 * n - 1);
    for (int i = 1; i < 14; i++) {
        for (int j = 0; j < 2 * n - 1; j++) {
            par[i][j] = par[i - 1][par[i - 1][j]];
        }
    }
    for (auto &[u, v, w] : edges) {
        int l = lca(u, v);
        val[u] += w;
        val[v] += w;
        val[l] -= 2 * w;
    }
    for (int i = 0; i < n; i++) {
        val[i] += a[i];
    }
    for (int i = 1; i < 2 * n - 1; i++) {
        for (auto &j : gph[i])
            val[i] += val[j];
    }
    par[0][2 * n - 2] = 2 * n - 1;
    for (int i = 0; i < 2 * n - 1; i++) {
        seg.add(din[i], din[i], 0, 2 * n - 2, 1, val[i]);
    }
    vector<lint> to_init(n);
    while (q--) {
        int x, y;
        cin >> x >> y;
        for (int j = x; j < 2 * n - 1; j = par[0][chn[j]]) {
            int l = din[chn[j]];
            int r = din[j];
            seg.add(l, r, 0, 2 * n - 2, 1, y - a[x]);
        }
        a[x] = y;
        cout << seg.tree[1] << "\n";
    }
}