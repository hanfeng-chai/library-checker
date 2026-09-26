#include <bits/stdc++.h>
#define pii pair<int, int>
#define pll pair<long long, long long>
#define piii pair<int, pii>
#define plll pair<long long, pll>
#define tiii array<int, 3>
#define tiiii array<int, 4>
#define ff first
#define ss second
#define ee ss.ff
#define rr ss.ss
#define int long long
typedef long long ll;
typedef long double ld;
#define all(x) x.begin(), x.end()

#include <bits/stdc++.h>
using namespace std;
// https://judge.yosupo.jp/submission/201334

namespace blossom {
#define d(x) (lab[x.u] + lab[x.v] - 2 * e[x.u][x.v].w)
    const int N = 403 * 2;
    const int inf = 1e18;
    struct Q{ int u, v, w; } e[N][N];
    vector<int> p[N];
    int n, m = 0, id, h, t, lk[N], sl[N], st[N], f[N], b[N][N], s[N], ed[N], q[N], lab[N];
    void upd(int u, int v) { if (!sl[v] || d(e[u][v]) < d(e[sl[v]][v])) sl[v] = u; }
    void ss(int v) {
        sl[v] = 0;
        for (int u = 1; u <= n; ++u) if (e[u][v].w > 0 && st[u] != v && !s[st[u]]) upd(u, v);
    }
    void ins(int u){ if (u <= n) q[++t] = u; else for (int v : p[u]) ins(v); }
    void ch(int u, int w) { st[u] = w; if (u > n) for (int v : p[u]) ch(v, w); }
    int gr(int u, int v) {
        if ((v = find(all(p[u]), v) - p[u].begin()) & 1) {
            reverse(1 + all(p[u]));
            return (int)p[u].size() - v;
        }
        return v;
    }
    void stm(int u, int v) {
        lk[u] = e[u][v].v;
        if (u <= n) return; Q w = e[u][v];
        int x = b[u][w.u], y = gr(u,x);
        for (int i = 0; i < y; ++i) stm(p[u][i], p[u][i^1]);
        stm(x, v); rotate(p[u].begin(), y+all(p[u]));
    }
    void aug(int u, int v) {
        int w = st[lk[u]];stm(u, v);if (!w) return;
        stm(w, st[f[w]]);
        aug(st[f[w]], w);
    }
    int lca(int u, int v) {
        for (id++; u|v; swap(u, v)) {
            if (!u) continue;if(ed[u] == id) return u;
            ed[u] = id; if (u = st[lk[u]]) u = st[f[u]]; // =, not ==
        }
        return 0;
    }
    void add(int u, int a, int v) {
        int x = n + 1; while (x <= m && st[x]) ++x;
        if (x > m) ++m;
        lab[x] = s[x] = st[x] = 0;
        lk[x] = lk[a];
        p[x].clear();
        p[x].push_back(a);
#define op(q) for (int i = q, j = 0; i != a; i=st[f[j]]) p[x].push_back(i), p[x].push_back(j=st[lk[i]]), ins(j) // also not ==
        op(u); reverse(1+all(p[x]));op(v);
        ch(x, x); for (int i = 1; i <= m; ++i) e[x][i].w = e[i][x].w = 0;
        fill(b[x], b[x]+n+1, 0);
        for (int u : p[x]) {
            for (int v = 1; v <= m; ++v) if (!e[x][v].w || d(e[u][v]) < d(e[x][v])) e[x][v] = e[u][v], e[v][x] = e[v][u];
            for (int v = 1; v <= n; ++v) if (b[u][v]) b[x][v] = u;
        }
        ss(x);
    }
    void ex(int u) {
        for (int x : p[u]) ch(x, x);
        int a = b[u][e[u][f[u]].u], r = gr(u, a);
        for (int i = 0; i < r; i += 2) {
            int x = p[u][i], y = p[u][i + 1];
            f[x] = e[y][x].u; s[x] = 1; s[y] = 0; sl[x] = 0; ss(y); ins(y);
        }
        s[a] = 1; f[a] = f[u];
        for (int i = r + 1; i < p[u].size(); ++i) s[p[u][i]] = -1, ss(p[u][i]);
        st[u] = 0;
    }
    bool on(const Q &e) {
        int u = st[e.u], v = st[e.v], a;
        if (s[v] == -1) {
            f[v] = e.u, s[v] = 1, a = st[lk[v]], sl[v] = sl[a] = s[a] = 0, ins(a);
        } else if (!s[v]) {
            a = lca(u, v); if (!a) return aug(u, v), aug(v, u), 1; else add(u, a, v);
        }
        return 0;
    }
    bool bfs() {
        fill(s, s+m+1, -1);fill(sl, sl+m+1, 0); // s is filled with -1
        h = 1, t = 0; for (int i = 1; i <= m; ++i) if (st[i] == i && !lk[i]) f[i] = s[i] = 0, ins(i);
        if (h > t) return 0;
        while (1) {
            while (h <= t) {
                int u = q[h++];
                if (s[st[u]] != 1) {
                    for (int v = 1; v <= n; ++v) if (e[u][v].w > 0 && st[u] != st[v]) {
                        if (d(e[u][v])) upd(u, st[v]); else if (on(e[u][v])) return 1;
                    }
                }
            }
            int x = inf;
            for (int i = n+1; i <= m; ++i) if (st[i] == i && s[i] == 1) x = min(x, lab[i]/2);
            for (int i = 1; i <= m; ++i) if (st[i] == i && sl[i] && s[i] != 1) x = min(x, d(e[sl[i]][i])>>s[i]+1);
            for (int i = 1; i <= n; ++i) if (~s[st[i]]) if ((lab[i] += (s[st[i]] * 2 - 1) * x) <=0) return 0;
            for (int i = n + 1; i <= m; ++i) if (st[i] == i && ~s[st[i]]) lab[i] += (2 - 4 * s[st[i]]) * x;
            h = 1, t = 0;
            for (int i = 1; i <= m; ++i) if (st[i] == i && sl[i] && st[sl[i]] != i && !d(e[sl[i]][i]) && on(e[sl[i]][i])) return 1;
            for (int i = n+1; i <= m; ++i) if (st[i] == i && s[i] == 1 && !lab[i]) ex(i);
        }
    }

    pair<int, vector<array<int, 2>>> run(int N, vector<array<int, 3>> edges) {
        for (auto &[u, v, w] : edges) ++u, ++v;
        fill(ed, ed+m+1, 0);
        fill(lk, lk+m+1, 0);
        n = m = N;
        id = 0;
        iota(st + 1, st + n + 1, 1);
        int wm = 0, weight = 0;
        for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j) e[i][j] = {i,j,0};
        for (auto [u, v, w] : edges) wm = max(wm, e[v][u].w = e[u][v].w = max(e[u][v].w, w));
        for (int i = 1; i <= n; ++i) p[i].clear();
        for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j) b[i][j] = i==j?i:0;
        fill_n(lab+1, n, wm); while (bfs());
        vector<array<int, 2>> matching;
        for (int i = 1; i <= n; ++i) if (i < lk[i]) weight += e[i][lk[i]].w, matching.push_back({i - 1, lk[i] - 1});
        return {weight, matching};
    }
}
int32_t main()
{
    ios_base::sync_with_stdio(false);
    int n, m;
    cin>> n >> m;
    vector<array<int, 3>> e(m);
    for (auto &[u, v, w] : e) cin >> u >> v >> w;
    auto [ans, ma] = blossom::run(n + 1, e);
    cout << ma.size() << ' ' << ans << '\n';
    for (auto [u, v] : ma) cout << u << ' ' << v << '\n';
    return 0;
}
