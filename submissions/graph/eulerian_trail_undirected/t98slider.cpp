// 無向オイラー路: 次数条件(奇数次数が0か2)+辺の連結 で判定し Hierholzer で復元。複数テストケース。
#include <bits/stdc++.h>
using namespace std;

struct EulerTrailUndirected {
    int n; vector<int> head, nxt, to, used; int m = 0;
    vector<int> deg;
    EulerTrailUndirected(int n) : n(n), head(n, -1), deg(n, 0) {}
    void add_edge(int u, int v) {
        to.push_back(v); nxt.push_back(head[u]); head[u] = (int)to.size() - 1;  // 2m
        to.push_back(u); nxt.push_back(head[v]); head[v] = (int)to.size() - 1;
        deg[u]++; deg[v]++; m++;
    }
    // 成功: (頂点列 M+1, 辺列 M)。失敗: empty
    bool solve(vector<int>& vs, vector<int>& es) {
        if (m == 0) { vs = {0}; es = {}; return true; }
        int start = -1, odd = 0;
        for (int i = 0; i < n; i++) if (deg[i] & 1) { odd++; start = i; }
        if (odd != 0 && odd != 2) return false;
        if (start < 0) for (int i = 0; i < n; i++) if (deg[i]) { start = i; break; }
        used.assign(m, 0); vector<int> cur = head, sv, se; sv.push_back(start);
        while (!sv.empty()) { int u = sv.back();
            int e = cur[u];
            while (e != -1 && used[e >> 1]) e = nxt[e];
            cur[u] = e;
            if (e != -1) { used[e >> 1] = 1; sv.push_back(to[e]); se.push_back(e >> 1); }
            else { vs.push_back(u); sv.pop_back(); if (!se.empty()) { es.push_back(se.back()); se.pop_back(); } }
        }
        if ((int)es.size() != m) return false;            // 非連結
        reverse(vs.begin(), vs.end()); reverse(es.begin(), es.end());
        return true;
    }
};

namespace fio {
const int BUF = 1 << 22; char ib[BUF]; int ip = 0, il = 0;
inline int gc() { if (ip == il) { il = (int)fread(ib, 1, BUF, stdin); ip = 0; if (!il) return -1; } return ib[ip++]; }
inline int rd() { int c = gc(); while (c != -1 && (c < '0' || c > '9')) c = gc(); int x = 0; while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); } return x; }
}
int main() {
    int T = fio::rd(); string out; char tmp[16];
    while (T--) {
        int N = fio::rd(), M = fio::rd();
        EulerTrailUndirected g(N);
        for (int i = 0; i < M; i++) { int u = fio::rd(), v = fio::rd(); g.add_edge(u, v); }
        vector<int> vs, es;
        if (!g.solve(vs, es)) { out += "No\n"; continue; }
        out += "Yes\n";
        for (size_t i = 0; i < vs.size(); i++) { if (i) out.push_back(' '); int p = sprintf(tmp, "%d", vs[i]); out.append(tmp, p); } out.push_back('\n');
        for (size_t i = 0; i < es.size(); i++) { if (i) out.push_back(' '); int p = sprintf(tmp, "%d", es[i]); out.append(tmp, p); } out.push_back('\n');
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
