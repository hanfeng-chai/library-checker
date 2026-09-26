#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (ll)2e18;

// 高速入力
namespace fio {
const int BUF = 1 << 16;
char ibuf[BUF];
int ip = 0, il = 0;
inline int gc() {
    if (ip == il) { il = (int)fread(ibuf, 1, BUF, stdin); ip = 0; if (il == 0) return -1; }
    return ibuf[ip++];
}
inline int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return neg ? -x : x;
}
} // namespace fio

// Kinetic Segment Tree（区間に b*i + c を加算、区間 min）
//   各要素 i は傾き sl=i の直線。ノードに最小値 mn、その傾き sl、
//   「最小が入れ替わるまでの猶予」melt(前方=傾き加算が正) / meltb(後方=負) を持つ。
//   add(l, r, b, c) : [l, r) に b*i + c を加算
//   range_min(l, r) : [l, r) の最小値
struct KineticSegTree {
    int n;
    vector<ll> mn, sl, melt, meltb, lzB, lzC;

    KineticSegTree(const vector<int>& a) {
        n = (int)a.size();
        int sz = 4 * n;
        mn.resize(sz);
        sl.resize(sz);
        melt.resize(sz);
        meltb.resize(sz);
        lzB.assign(sz, 0);
        lzC.assign(sz, 0);
        build(1, 0, n - 1, a);
    }

    void pull(int nd) {
        int L = 2 * nd, R = 2 * nd + 1;
        int a, b; // a=最小側, b=他方
        if (mn[L] <= mn[R]) { mn[nd] = mn[L]; sl[nd] = sl[L]; a = L; b = R; }
        else { mn[nd] = mn[R]; sl[nd] = sl[R]; a = R; b = L; }
        melt[nd] = min(melt[L], melt[R]);
        meltb[nd] = min(meltb[L], meltb[R]);
        ll Amn = mn[a], Asl = sl[a], Bmn = mn[b], Bsl = sl[b];
        if (Asl > Bsl) { // 前方で b が a を追い越す
            ll cross = (Bmn - Amn) / (Asl - Bsl);
            melt[nd] = min(melt[nd], cross);
        }
        if (Bsl > Asl) { // 後方で b が a を追い越す
            ll crossb = (Bmn - Amn) / (Bsl - Asl);
            meltb[nd] = min(meltb[nd], crossb);
        }
    }

    void applyShift(int nd, ll B, ll C) { // 安全な一様シフト
        mn[nd] += B * sl[nd] + C;
        melt[nd] = min(INF, melt[nd] - B);
        meltb[nd] = min(INF, meltb[nd] + B);
        lzB[nd] += B;
        lzC[nd] += C;
    }

    void addNode(int nd, int l, int r, ll B, ll C) { // [l,r] 全体に B*i + C
        if (l == r) { mn[nd] += B * sl[nd] + C; return; } // 葉（sl=l 固定, melt INF）
        bool safe = (B >= 0) ? (B <= melt[nd]) : (-B <= meltb[nd]);
        if (safe) { applyShift(nd, B, C); return; }
        pushdown(nd, l, r);
        int mid = (l + r) / 2;
        addNode(2 * nd, l, mid, B, C);
        addNode(2 * nd + 1, mid + 1, r, B, C);
        pull(nd);
    }

    void pushdown(int nd, int l, int r) {
        if (lzB[nd] != 0 || lzC[nd] != 0) {
            int mid = (l + r) / 2;
            addNode(2 * nd, l, mid, lzB[nd], lzC[nd]);
            addNode(2 * nd + 1, mid + 1, r, lzB[nd], lzC[nd]);
            lzB[nd] = 0;
            lzC[nd] = 0;
        }
    }

    void build(int nd, int l, int r, const vector<int>& a) {
        lzB[nd] = lzC[nd] = 0;
        if (l == r) { mn[nd] = a[l]; sl[nd] = l; melt[nd] = INF; meltb[nd] = INF; return; }
        int mid = (l + r) / 2;
        build(2 * nd, l, mid, a);
        build(2 * nd + 1, mid + 1, r, a);
        pull(nd);
    }

    void update(int nd, int l, int r, int ql, int qr, ll B, ll C) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) { addNode(nd, l, r, B, C); return; }
        pushdown(nd, l, r);
        int mid = (l + r) / 2;
        update(2 * nd, l, mid, ql, qr, B, C);
        update(2 * nd + 1, mid + 1, r, ql, qr, B, C);
        pull(nd);
    }

    ll query(int nd, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return INF;
        if (ql <= l && r <= qr) return mn[nd];
        pushdown(nd, l, r);
        int mid = (l + r) / 2;
        return min(query(2 * nd, l, mid, ql, qr), query(2 * nd + 1, mid + 1, r, ql, qr));
    }

    void add(int l, int r, ll b, ll c) { update(1, 0, n - 1, l, r - 1, b, c); } // [l, r)
    ll range_min(int l, int r) { return query(1, 0, n - 1, l, r - 1); }          // [l, r)
};

int main() {
    int N = fio::readInt(), Q = fio::readInt();
    vector<int> a(N);
    for (int i = 0; i < N; i++) a[i] = fio::readInt();

    KineticSegTree st(a);

    string out;
    while (Q--) {
        int t = fio::readInt();
        if (t == 0) {
            int l = fio::readInt(), r = fio::readInt();
            ll b = fio::readInt(), c = fio::readInt();
            st.add(l, r, b, c);
        } else {
            int l = fio::readInt(), r = fio::readInt();
            out += to_string(st.range_min(l, r));
            out += '\n';
        }
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
