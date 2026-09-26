#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 高速入力
namespace fio {
const int BUF = 1 << 22;
char ibuf[BUF];
int ip = 0, il = 0;
inline int gc() { if (ip == il) { il = (int)fread(ibuf, 1, BUF, stdin); ip = 0; if (!il) return -1; } return ibuf[ip++]; }
inline int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    bool neg = false; if (c == '-') { neg = true; c = gc(); }
    int x = 0; while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return neg ? -x : x;
}
} // namespace fio

// 各辺について、それを含む C4(4辺サイクル) の個数を数える。
//   多重辺は端点ペアでまとめ mult を持つ（平行辺は同じ答え）。
//   次数降順に頂点を処理し、処理後に削除（Chiba-Nishizeki）。
//   各 4-サイクルは「最初に処理した頂点 u を対角の一端」として 1 回だけ数える。
//   対角 {u,w} の中間頂点 m（u,w の共通隣接）について
//     g_m = mult(u,m)*mult(m,w),  S = Σ g_m
//   各中間 m で 4 本のスポーク辺に寄与:
//     A[(u,m)] += mult(m,w)*(S - g_m),  A[(m,w)] += mult(u,m)*(S - g_m)
struct E { int to, mult, seid; };

int main() {
    int N = fio::readInt(), M = fio::readInt();
    vector<int> eu(M), ev(M);
    for (int i = 0; i < M; i++) { eu[i] = fio::readInt(); ev[i] = fio::readInt(); }

    // 端点ペアでまとめて super-edge を作る
    vector<array<int, 3>> pe(M); // (a, b, 元の添字)
    for (int i = 0; i < M; i++) { int a = eu[i], b = ev[i]; if (a > b) swap(a, b); pe[i] = {a, b, i}; }
    sort(pe.begin(), pe.end());
    vector<int> edgeSeid(M);
    vector<int> seU, seV, seMult;
    seU.reserve(M); seV.reserve(M); seMult.reserve(M);
    for (int i = 0; i < M;) {
        int j = i;
        while (j < M && pe[j][0] == pe[i][0] && pe[j][1] == pe[i][1]) j++;
        int id = (int)seU.size();
        seU.push_back(pe[i][0]); seV.push_back(pe[i][1]); seMult.push_back(j - i);
        for (int k = i; k < j; k++) edgeSeid[pe[k][2]] = id;
        i = j;
    }
    int seCnt = (int)seU.size();

    // 隣接リスト
    vector<int> deg(N, 0);
    for (int s = 0; s < seCnt; s++) { deg[seU[s]]++; deg[seV[s]]++; }
    vector<vector<E>> adj(N);
    for (int v = 0; v < N; v++) adj[v].reserve(deg[v]);
    for (int s = 0; s < seCnt; s++) {
        adj[seU[s]].push_back({seV[s], seMult[s], s});
        adj[seV[s]].push_back({seU[s], seMult[s], s});
    }

    // 次数降順
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) { return deg[a] > deg[b]; });

    vector<char> deleted(N, 0), touched(N, 0);
    vector<ll> S(N, 0), Asuper(seCnt, 0);
    vector<int> tlist;

    for (int u : order) {
        // pass1: 共通隣接の重み S[w] を集計
        for (const E& em : adj[u]) {
            int m = em.to;
            if (deleted[m]) continue;
            ll mu = em.mult;
            for (const E& ew : adj[m]) {
                int w = ew.to;
                if (deleted[w] || w == u) continue;
                S[w] += mu * (ll)ew.mult;
                if (!touched[w]) { touched[w] = 1; tlist.push_back(w); }
            }
        }
        // pass2: 4 本のスポークに寄与
        for (const E& em : adj[u]) {
            int m = em.to;
            if (deleted[m]) continue;
            ll mu = em.mult;
            int seUM = em.seid;
            for (const E& ew : adj[m]) {
                int w = ew.to;
                if (deleted[w] || w == u) continue;
                ll mw = ew.mult;
                ll g = mu * mw;
                ll rest = S[w] - g;
                Asuper[seUM] += mw * rest;
                Asuper[ew.seid] += mu * rest;
            }
        }
        for (int w : tlist) { S[w] = 0; touched[w] = 0; }
        tlist.clear();
        deleted[u] = 1;
    }

    string out;
    out.reserve((size_t)M * 8);
    for (int i = 0; i < M; i++) {
        if (i) out += ' ';
        out += to_string(Asuper[edgeSeid[i]]);
    }
    out += '\n';
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
