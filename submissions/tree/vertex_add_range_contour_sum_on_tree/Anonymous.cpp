#pragma GCC optimize("Ofast,unroll-loops")
// 如果确认评测机支持 AVX2/BMI，可以取消下一行注释，可能还能再快一点。
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

static const int MAXN = 100000 + 5;
static const int MAXE = 2 * MAXN;
static const int MAXLOG = 20;
static const int MAXID = 2 * MAXN + 5;
static const int MAXBIT = 6000000;

static const int SHIFT = 20;
static const unsigned MASK = (1u << SHIFT) - 1;

struct FastInput {
    static const int SZ = 1 << 20;

    int idx = 0, size = 0;
    char buf[SZ];

    inline char gc() {
        if (idx >= size) {
            size = (int)fread(buf, 1, SZ, stdin);
            idx = 0;
            if (!size) return 0;
        }
        return buf[idx++];
    }

    inline int nextInt() {
        char c = gc();

        while (c != '-' && (c < '0' || c > '9')) {
            c = gc();
        }

        int sign = 1;
        if (c == '-') {
            sign = -1;
            c = gc();
        }

        int x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = gc();
        }

        return x * sign;
    }

    inline ll nextLong() {
        char c = gc();

        while (c != '-' && (c < '0' || c > '9')) {
            c = gc();
        }

        ll sign = 1;
        if (c == '-') {
            sign = -1;
            c = gc();
        }

        ll x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = gc();
        }

        return x * sign;
    }
} in;

struct FastOutput {
    static const int SZ = 1 << 20;

    int idx = 0;
    char buf[SZ];

    inline void flush() {
        if (idx) {
            fwrite(buf, 1, idx, stdout);
            idx = 0;
        }
    }

    inline void pc(char c) {
        if (idx == SZ) flush();
        buf[idx++] = c;
    }

    inline void writeLong(ll x) {
        if (x == 0) {
            pc('0');
            pc('\n');
            return;
        }

        if (x < 0) {
            pc('-');
            x = -x;
        }

        char s[24];
        int n = 0;

        while (x) {
            s[n++] = char('0' + x % 10);
            x /= 10;
        }

        while (n--) pc(s[n]);
        pc('\n');
    }
} out;

int N, Q;

int head[MAXN], to_[MAXE], nxt_[MAXE], ecnt;
ll a[MAXN];

bool dead_[MAXN];
int par_[MAXN], sub_[MAXN];

int st_[MAXN], ord_[MAXN], ordCnt;
int cu_[MAXN], cd_[MAXN], compCnt;
int su_[MAXN], sp_[MAXN], sd_[MAXN];

int plen[MAXN];

// pathInfo[u][i]:
// low  20 bits: distance
// mid  20 bits: all Fenwick id
// high bits:    subtract Fenwick id
uint64_t pathInfo[MAXN][MAXLOG];

int centroidAllId[MAXN];

ll bit[MAXBIT], bitTotal[MAXID];
int bitOff[MAXID], bitLen[MAXID];
int bitPtr = 0, bitCnt = 0;

inline uint64_t packInfo(int d, int all, int sub) {
    return (uint64_t)d |
           ((uint64_t)all << SHIFT) |
           ((uint64_t)sub << (2 * SHIFT));
}

inline void addEdge(int u, int v) {
    to_[ecnt] = v;
    nxt_[ecnt] = head[u];
    head[u] = ecnt++;
}

inline int newBit(int n) {
    int id = ++bitCnt;
    bitOff[id] = bitPtr;
    bitLen[id] = n;
    bitPtr += n + 1;
    return id;
}

inline void addPath(int u, int c, int d, int sid) {
    pathInfo[u][plen[u]++] = packInfo(d, c, sid);
}

int getCentroid(int root) {
    int top = 0;
    ordCnt = 0;

    par_[root] = -1;
    st_[top++] = root;

    while (top) {
        int u = st_[--top];
        ord_[ordCnt++] = u;

        for (int e = head[u]; e != -1; e = nxt_[e]) {
            int v = to_[e];

            if (v == par_[u] || dead_[v]) continue;

            par_[v] = u;
            st_[top++] = v;
        }
    }

    int total = ordCnt;

    for (int i = total - 1; i >= 0; --i) {
        int u = ord_[i];
        int s = 1;

        for (int e = head[u]; e != -1; e = nxt_[e]) {
            int v = to_[e];

            if (!dead_[v] && par_[v] == u) {
                s += sub_[v];
            }
        }

        sub_[u] = s;
    }

    int best = ord_[0];
    int bestMx = total + 1;

    for (int i = 0; i < total; ++i) {
        int u = ord_[i];
        int mx = total - sub_[u];

        for (int e = head[u]; e != -1; e = nxt_[e]) {
            int v = to_[e];

            if (!dead_[v] && par_[v] == u && sub_[v] > mx) {
                mx = sub_[v];
            }
        }

        if (mx < bestMx) {
            bestMx = mx;
            best = u;
        }
    }

    return best;
}

int collectComponent(int start, int cent) {
    int top = 0;
    compCnt = 0;

    su_[top] = start;
    sp_[top] = cent;
    sd_[top++] = 1;

    int maxD = 1;

    while (top) {
        --top;

        int u = su_[top];
        int p = sp_[top];
        int d = sd_[top];

        cu_[compCnt] = u;
        cd_[compCnt++] = d;

        if (d > maxD) maxD = d;

        for (int e = head[u]; e != -1; e = nxt_[e]) {
            int v = to_[e];

            if (v == p || dead_[v]) continue;

            su_[top] = v;
            sp_[top] = u;
            sd_[top++] = d + 1;
        }
    }

    return maxD;
}

void buildCentroid() {
    static int roots[MAXN];

    int rtop = 0;
    roots[rtop++] = 0;

    while (rtop) {
        int root = roots[--rtop];

        if (dead_[root]) continue;

        int c = getCentroid(root);
        dead_[c] = true;

        addPath(c, c, 0, 0);

        int maxAllD = 0;

        for (int e = head[c]; e != -1; e = nxt_[e]) {
            int v = to_[e];

            if (dead_[v]) continue;

            int maxD = collectComponent(v, c);
            int sid = newBit(maxD + 1);

            for (int i = 0; i < compCnt; ++i) {
                addPath(cu_[i], c, cd_[i], sid);
            }

            if (maxD > maxAllD) maxAllD = maxD;

            roots[rtop++] = v;
        }

        centroidAllId[c] = newBit(maxAllD + 1);
    }

    for (int u = 0; u < N; ++u) {
        for (int i = 0; i < plen[u]; ++i) {
            uint64_t x = pathInfo[u][i];

            int d = x & MASK;
            int c = (x >> SHIFT) & MASK;
            int sid = x >> (2 * SHIFT);

            pathInfo[u][i] = packInfo(d, centroidAllId[c], sid);
        }
    }
}

inline void rawAddBit(int id, int pos, ll x) {
    bitTotal[id] += x;
    bit[bitOff[id] + pos + 1] += x;
}

inline void buildAllFenwick() {
    for (int id = 1; id <= bitCnt; ++id) {
        ll *b = bit + bitOff[id];
        int n = bitLen[id];

        for (int i = 1; i <= n; ++i) {
            int j = i + (i & -i);
            if (j <= n) b[j] += b[i];
        }
    }
}

inline void addBit(int id, int pos, ll x) {
    bitTotal[id] += x;

    ll *b = bit + bitOff[id];
    int n = bitLen[id];

    for (int i = pos + 1; i <= n; i += i & -i) {
        b[i] += x;
    }
}

inline ll prefixNoCheck(int id, int cnt) {
    ll res = 0;
    ll *b = bit + bitOff[id];

    for (int i = cnt; i; i &= i - 1) {
        res += b[i];
    }

    return res;
}

// sum of positions [l, r)
inline ll rangeBit(int id, int l, int r) {
    if (r <= 0) return 0;

    int n = bitLen[id];

    if (l <= 0) {
        if (r >= n) return bitTotal[id];
        return prefixNoCheck(id, r);
    }

    if (l >= n) return 0;

    if (r >= n) {
        return bitTotal[id] - prefixNoCheck(id, l);
    }

    return prefixNoCheck(id, r) - prefixNoCheck(id, l);
}

inline void pointAdd(int u, ll x) {
    int L = plen[u];

    for (int i = 0; i < L; ++i) {
        uint64_t e = pathInfo[u][i];

        int d = e & MASK;
        int aid = (e >> SHIFT) & MASK;
        int sid = e >> (2 * SHIFT);

        addBit(aid, d, x);

        if (sid) {
            addBit(sid, d, x);
        }
    }
}

inline ll queryRange(int u, int l, int r) {
    ll ans = 0;
    int L = plen[u];

    for (int i = 0; i < L; ++i) {
        uint64_t e = pathInfo[u][i];

        int d = e & MASK;
        int lo = l - d;
        int hi = r - d;

        if (hi <= 0) continue;

        int aid = (e >> SHIFT) & MASK;
        int sid = e >> (2 * SHIFT);

        ans += rangeBit(aid, lo, hi);

        if (sid) {
            ans -= rangeBit(sid, lo, hi);
        }
    }

    return ans;
}

int main() {
    N = in.nextInt();
    Q = in.nextInt();

    for (int i = 0; i < N; ++i) {
        head[i] = -1;
    }

    for (int i = 0; i < N; ++i) {
        a[i] = in.nextLong();
    }

    for (int i = 0; i < N - 1; ++i) {
        int u = in.nextInt();
        int v = in.nextInt();

        addEdge(u, v);
        addEdge(v, u);
    }

    buildCentroid();

    // 初始值：先按原数组写入，再线性 build Fenwick。
    for (int u = 0; u < N; ++u) {
        ll x = a[u];
        int L = plen[u];

        for (int i = 0; i < L; ++i) {
            uint64_t e = pathInfo[u][i];

            int d = e & MASK;
            int aid = (e >> SHIFT) & MASK;
            int sid = e >> (2 * SHIFT);

            rawAddBit(aid, d, x);

            if (sid) {
                rawAddBit(sid, d, x);
            }
        }
    }

    buildAllFenwick();

    for (int qi = 0; qi < Q; ++qi) {
        int type = in.nextInt();

        if (type == 0) {
            int p = in.nextInt();
            ll x = in.nextLong();

            pointAdd(p, x);
        } else {
            int p = in.nextInt();
            int l = in.nextInt();
            int r = in.nextInt();

            out.writeLong(queryRange(p, l, r));
        }
    }

    out.flush();
    return 0;
}