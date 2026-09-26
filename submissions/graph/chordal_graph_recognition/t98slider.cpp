// Chordal Graph Recognition: 単純無向グラフが chordal(長さ4以上の induced cycle を持たない)か判定。
//   chordal なら perfect elimination ordering(PEO)を、そうでなければ長さ4以上の induced cycle を出力。
//   使い方: ChordalRecognition cr(n); cr.add_edge(a,b); cr.solve();
//           cr.chordal / cr.peo / cr.cycle を参照。
//   手法: 最大基数探索(MCS)で順序を作ると、逆順が PEO 候補(Tarjan-Yannakakis)。各頂点 v の親
//   parent(v)=後方近傍で位置最小の頂点 が v の他の後方近傍すべてに隣接するかを検査(これで PEO 全体の
//   検査に十分)。失敗時は v-p,v-w が辺で p-w が非辺の三つ組が得られるので、N(v) を避けて p→w を
//   最短路 BFS し v,p,…,w を induced cycle として出力(最短路ゆえ弦なし、内部は v に非隣接)。
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct ChordalRecognition {
    int n;
    vector<vector<int>> adj;
    bool chordal = false;
    vector<int> peo;     // chordal 時の perfect elimination ordering
    vector<int> cycle;   // 非 chordal 時の induced cycle (頂点を周回順に)

    explicit ChordalRecognition(int n_) : n(n_), adj(n_) {}
    void add_edge(int u, int v) { adj[u].push_back(v); adj[v].push_back(u); }

    // 最大基数探索: 選択順 ord[0],ord[1],... を返す(ord[0] が最初に選ばれる)
    vector<int> mcs() {
        vector<int> wt(n, 0), nxt(n, -1), prv(n, -1), head(n + 1, -1), ord(n);
        vector<char> sel(n, 0);
        auto push = [&](int w, int v) { prv[v] = -1; nxt[v] = head[w]; if (head[w] != -1) prv[head[w]] = v; head[w] = v; };
        auto rem  = [&](int w, int v) { if (prv[v] != -1) nxt[prv[v]] = nxt[v]; else head[w] = nxt[v]; if (nxt[v] != -1) prv[nxt[v]] = prv[v]; };
        for (int v = 0; v < n; v++) push(0, v);
        int maxw = 0;
        for (int i = 0; i < n; i++) {
            while (maxw > 0 && head[maxw] == -1) maxw--;
            int v = head[maxw];
            rem(maxw, v); sel[v] = 1; ord[i] = v;
            for (int u : adj[v]) if (!sel[u]) { rem(wt[u], u); wt[u]++; push(wt[u], u); if (wt[u] > maxw) maxw = wt[u]; }
        }
        return ord;
    }

    void solve() {
        if (n == 0) { chordal = true; return; }
        vector<int> ord = mcs();
        // PEO 位置: pos が大きいほど後方(MCS では先に選ばれた頂点)
        vector<int> pos(n);
        for (int i = 0; i < n; i++) pos[ord[i]] = n - 1 - i;
        peo.resize(n);
        for (int i = 0; i < n; i++) peo[i] = ord[n - 1 - i];

        // 各 v の親(後方近傍で pos 最小)を求め、「他の後方近傍 w は親に隣接すべき」要件を親ごとに蓄積
        vector<vector<pair<int,int>>> req(n);    // req[p] = {(w, v)}: w は p に隣接していなければならない
        for (int v = 0; v < n; v++) {
            int p = -1, pp = INT_MAX;
            for (int u : adj[v]) if (pos[u] > pos[v] && pos[u] < pp) { pp = pos[u]; p = u; }
            if (p == -1) continue;
            for (int u : adj[v]) if (pos[u] > pos[v] && u != p) req[p].push_back({u, v});
        }
        // 検証: 親 p の隣接集合に各要件 w が含まれるか
        vector<int> mark(n, -1);
        int bv = -1, bp = -1, bw = -1;
        for (int p = 0; p < n && bv == -1; p++) {
            if (req[p].empty()) continue;
            for (int u : adj[p]) mark[u] = p;
            for (auto &pr : req[p]) if (mark[pr.first] != p) { bw = pr.first; bv = pr.second; bp = p; break; }
        }
        if (bv == -1) { chordal = true; return; }   // 全要件成立 → chordal
        chordal = false;
        findHole(bv, bp, bw);
    }

    // v-p,v-w が辺で p-w が非辺。N(v)\{p,w} と v を除いた部分グラフで p→w 最短路を取り、
    // v,p,…,w を induced cycle(長さ≥4)として cycle に格納する。
    void findHole(int v, int p, int w) {
        vector<char> forbid(n, 0);
        forbid[v] = 1;
        for (int u : adj[v]) if (u != p && u != w) forbid[u] = 1;
        vector<int> par(n, -1);
        vector<char> vis(n, 0);
        queue<int> q; q.push(p); vis[p] = 1;
        while (!q.empty()) {
            int x = q.front(); q.pop();
            if (x == w) break;
            for (int y : adj[x]) if (!forbid[y] && !vis[y]) { vis[y] = 1; par[y] = x; q.push(y); }
        }
        vector<int> path;                         // w から親リンクを辿って p まで
        for (int x = w; x != -1; x = par[x]) path.push_back(x);
        reverse(path.begin(), path.end());        // p, …, w
        cycle.clear();
        cycle.push_back(v);
        for (int x : path) cycle.push_back(x);    // v, p, …, w
    }
};

namespace fio { const int BUF=1<<23; char ib[BUF]; int ip=0,il=0; inline int gc(){if(ip==il){il=(int)fread(ib,1,BUF,stdin);ip=0;if(!il)return -1;}return ib[ip++];} inline ll rd(){int c=gc();while(c!=-1&&c!='-'&&(c<'0'||c>'9'))c=gc();int s=1;if(c=='-'){s=-1;c=gc();}ll x=0;while(c>='0'&&c<='9'){x=x*10+(c-'0');c=gc();}return s*x;} }
int main() {
    int N = (int)fio::rd(), M = (int)fio::rd();
    ChordalRecognition cr(N);
    for (int i = 0; i < M; i++) { int a=(int)fio::rd(), b=(int)fio::rd(); cr.add_edge(a, b); }
    cr.solve();
    string out;
    char tmp[16];
    if (cr.chordal) {
        out = "YES\n";
        for (int i = 0; i < N; i++) { int q = sprintf(tmp, "%d", cr.peo[i]); out.append(tmp, q); out.push_back(i + 1 < N ? ' ' : '\n'); }
    } else {
        out = "NO\n";
        int q = sprintf(tmp, "%d", (int)cr.cycle.size()); out.append(tmp, q); out.push_back('\n');
        for (size_t i = 0; i < cr.cycle.size(); i++) { q = sprintf(tmp, "%d", cr.cycle[i]); out.append(tmp, q); out.push_back(i + 1 < cr.cycle.size() ? ' ' : '\n'); }
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
