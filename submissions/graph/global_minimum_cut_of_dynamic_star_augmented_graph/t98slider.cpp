// apex全域最小カット動的更新: SWの最大隣接順序で最小カット対を併合し2N-1ノードの併合二分木を作る。
//   各元辺wを端点パスに加算しb[node]=基底カット重み。apex a_iを葉→根へ区間加算しmin=全域最小カット。
//   HLD+遅延セグ木で O((N+Q)logN)。a_x更新は根パスに差分加算。
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1LL << 60;

struct LazySeg {
    int n;
    vector<ll> mn, lz;
    void init(int n_) {
        n = 1;
        while (n < n_) n *= 2;
        mn.assign(2 * n - 1, INF);
        lz.assign(2 * n - 1, 0);
    }
    void set_l(int k, ll x) { mn[k] += x; lz[k] += x; }
    void push(int k) {
        set_l(2 * k + 1, lz[k]);
        set_l(2 * k + 2, lz[k]);
        lz[k] = 0;
    }
    void rec(int a, int b, int k, int l, int r, ll x) {
        if (r <= a || b <= l) return;
        if (a <= l && r <= b) { set_l(k, x); return; }
        push(k);
        int m = (l + r) / 2;
        rec(a, b, 2 * k + 1, l, m, x);
        rec(a, b, 2 * k + 2, m, r, x);
        mn[k] = min(mn[2 * k + 1], mn[2 * k + 2]);
    }
    ll open(int k) {
        k += n - 1;
        ll r = mn[k];
        while (k > 0) { k = (k - 1) / 2; r += lz[k]; }
        return r;
    }
    void add(int a, int b, ll x) { rec(a, b, 0, 0, n, x); }
    void upd(int k, ll x) { add(k, k + 1, x - open(k)); }
    ll mn0() { return mn[0]; }
};

struct ApexMinCut {
    int n;
    vector<vector<pair<int,int>>> g;
    vector<vector<int>> tr;
    vector<ll> b;
    vector<int> vid, head, hvy, par, dep;
    LazySeg seg;
    ApexMinCut(int n) : n(n), g(n) {}
    void add_edge(int u, int v, int w) {
        if (w) { g[u].push_back({v, w}); g[v].push_back({u, w}); }
    }
    // Stoer-Wagner 併合木: 2n-1 ノード (g は破壊するので複製で処理)
    void evs() {
        tr.assign(2 * n - 1, {});
        vector<int> used(2 * n - 1, 1);
        auto gg = g; gg.resize(2 * n - 1);
        for (int i = 0; i < n - 1; i++) {
            vector<int> ord(n - i), vis(n + i, 0);
            vector<ll> cost(n + i, 0);
            priority_queue<pair<ll,ll>> que;
            for (int j = 0; j < n + i; j++) if (used[j]) {
                for (auto p : gg[j]) cost[j] += p.second;
                que.push({-cost[j], j});
            }
            for (int j = 0; j < n - i; j++) while (1) {
                auto pr = que.top(); que.pop();
                int v = (int)pr.second;
                if (vis[v]) continue;
                vis[v] = 1; ord[j] = v;
                for (auto p : gg[v]) { int u = p.first; if (!vis[u]) { cost[u] -= p.second; que.push({-cost[u], u}); } }
                break;
            }
            int v0 = ord[n - i - 1], v1 = ord[n - i - 2];
            gg[v0].clear(); gg[v1].clear(); used[v0] = used[v1] = 0;
            for (int j = 0; j < n + i; j++) if (used[j])
                for (auto& p : gg[j]) if (p.first == v0 || p.first == v1) { p.first = n + i; gg[n + i].push_back({j, p.second}); }
            tr[n + i].push_back(v0); tr[n + i].push_back(v1);
        }
    }
    int dfs(int v) {
        int mx = 0, t = 1;
        for (int u : tr[v]) { par[u] = v; dep[u] = dep[v] + 1; int s = dfs(u); t += s; if (s > mx) { mx = s; hvy[v] = u; } }
        return t;
    }
    void bfs(int rt) {
        int id = 0;
        queue<int> q; q.push(rt);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int u = v; u != -1; u = hvy[u]) { vid[u] = id++; head[u] = v; for (int w : tr[u]) if (w != hvy[u]) q.push(w); }
        }
    }
    void hld() {
        int K = tr.size();
        head = dep = vector<int>(K, 0);
        vid = hvy = par = vector<int>(K, -1);
        seg.init(K);
        dfs(K - 1); bfs(K - 1);
    }
    ll query(int v, ll x) {
        while (v >= 0) { seg.add(vid[head[v]], vid[v] + 1, x); v = par[head[v]]; }
        return seg.mn0();
    }
    void build(const vector<int>& a) {
        evs(); hld();
        b.assign(2 * n - 1, 0);
        for (int i = 0; i < n; i++) for (auto p : g[i]) {
            int u = i, v = p.first;
            if (u > v) continue;
            while (u != v) { if (dep[u] < dep[v]) swap(u, v); b[u] += p.second; u = par[u]; }
        }
        for (int i = 0; i < 2 * n - 1; i++) seg.upd(vid[i], b[i]);
        for (int i = 0; i < n; i++) query(i, a[i]);
    }
};
namespace fio {
    const int BUF = 1 << 22; char ib[BUF]; int ip = 0, il = 0;
    inline int gc() { if (ip == il) { il = (int)fread(ib, 1, BUF, stdin); ip = 0; if (!il) return -1; } return ib[ip++]; }
    inline ll rd() { int c = gc(); while (c != -1 && (c < '0' || c > '9')) c = gc(); ll x = 0; while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); } return x; }
}
int main() {
    int N = (int)fio::rd(), M = (int)fio::rd(), Q = (int)fio::rd();
    vector<int> a(N);
    for (int i = 0; i < N; i++) a[i] = (int)fio::rd();
    ApexMinCut mc(N);
    for (int i = 0; i < M; i++) { int u = (int)fio::rd(), v = (int)fio::rd(), w = (int)fio::rd(); mc.add_edge(u, v, w); }
    mc.build(a);
    string out; char tmp[24];
    for (int i = 0; i < Q; i++) {
        int v = (int)fio::rd(); ll w = fio::rd();
        ll r = mc.query(v, w - a[v]); a[v] = (int)w;
        int q = sprintf(tmp, "%lld", r); out.append(tmp, q); out.push_back('\n');
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
