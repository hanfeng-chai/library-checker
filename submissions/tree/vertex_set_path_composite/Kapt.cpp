#include <bits/stdc++.h>
using namespace std;

/// Sorry, Alice, just have to check
// fast input from https://judge.yosupo.jp/submission/270199

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
#define fun auto
#define def auto
#include <sys/mman.h>
#include <sys/stat.h>

struct rd {
    static constexpr fun all_digit(u32 x) {
        x ^= 0x30303030;
        x &= 0xf0f0f0f0;
        return !x;
    }
    static constexpr fun all_digit(u64 x) {
        x ^= 0x3030303030303030;
        x &= 0xf0f0f0f0f0f0f0f0;
        return !x;
    }

    char* p;
    rd() {
        struct stat st;
        fstat(0, &st);
        p = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    }

    fun u1() -> u32 {
        u32 x = *p++ - '0';
        return ++p, x;
    }

    fun ub() -> u32 {
        u32 x = *p++ - '0';
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun uh() -> u32 {
        u32 x{};
        u32 a;
        memcpy(&a, p, sizeof(a));
        if (all_digit(a)) {
            a ^= 0x30303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff;
            x = a, p += sizeof(a);
        }
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun uw() -> u32 {
        u32 x{};
        u64 a;
        memcpy(&a, p, sizeof(a));
        if (all_digit(a)) {
            a ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            x = a, p += sizeof(a);
        }
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun ud() -> u64 {
        u64 x{};
        union {
            char ch[16];
            u64 d[2];
        };
        memcpy(ch, p, sizeof(ch));
        u64 a = d[0], b = d[1];
        if (all_digit(a) && all_digit(b)) {
            a ^= 0x3030303030303030;
            b ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            b = (b * 10 + (b >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            b = (b * 100 + (b >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            b = (b * 10000 + (b >> 32)) & 0x00000000ffffffff;
            x = a * 100000000 + b, p += sizeof(ch);
        }
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun i1() -> i32 { return *p == '-' ? ++p, -u1() : u1(); }
    fun ib() -> i32 { return *p == '-' ? ++p, -ub() : ub(); }
    fun ih() -> i32 { return *p == '-' ? ++p, -uh() : uh(); }
    fun iw() -> i32 { return *p == '-' ? ++p, -uw() : uw(); }
    fun id() -> i64 { return *p == '-' ? ++p, -ud() : ud(); }
};

struct wt {
    static constexpr def low = [] {
        std::array<std::array<char, 4>, 10000> res;
        for (int i = 0; i < 10000; ++i) {
            res[i][0] = '0' + i / 1000;
            res[i][1] = '0' + i / 100 % 10;
            res[i][2] = '0' + i / 10 % 10;
            res[i][3] = '0' + i % 10;
        }
        return res;
    }();
    static constexpr def pos = [] {
        std::array<std::array<char, 4>, 10000> res;
        for (int i = 0; i < 10000; ++i) {
            res[i][0] = '0' + i / 1000;
            res[i][1] = '0' + i / 100 % 10;
            res[i][2] = '0' + i / 10 % 10;
            res[i][3] = '0' + i % 10;
            if (i < 1000) res[i][0] = ' ';
            if (i < 100) res[i][1] = ' ';
            if (i < 10) res[i][2] = ' ';
        }
        return res;
    }();

    inline static char buf[1 << 20];
    static constexpr char* sentinel = buf + sizeof(buf) - 50;
    char* p = buf;
    ~wt() { flush(); }
    void flush() { write(1, buf, p - buf), p = buf; }
    void print_low(u64 x) { memcpy(p, &low[x], 4), p += 4; }
    void print_pos(u64 x) { memcpy(p, &pos[x], 4), p += 4; }

    void puts(const char* src, int n) {
        if (sentinel < p + n) flush();
        memcpy(p, src, n), p += n;
    }

    void uw(u32 x) {
        if (sentinel < p) flush();
        *p++ = ' ';
        if (x > 9999'9999) {
            print_pos(x / 10000 / 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999) {
            print_pos(x / 10000);
            print_low(x % 10000);
        } else {
            print_pos(x);
        }
    }

    void ud(u64 x) {
        if (sentinel < p) flush();
        *p++ = ' ';
        if (x > 9999'9999'9999'9999) {
            print_pos(x / 10000 / 10000 / 10000 / 10000);
            print_low(x / 10000 / 10000 / 10000 % 10000);
            print_low(x / 10000 / 10000 % 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999'9999'9999) {
            print_pos(x / 10000 / 10000 / 10000);
            print_low(x / 10000 / 10000 % 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999'9999) {
            print_pos(x / 10000 / 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999) {
            print_pos(x / 10000);
            print_low(x % 10000);
        } else {
            print_pos(x);
        }
    }
};

constexpr uint32_t MOD = 998244353;

uint32_t add(uint32_t a, uint32_t b) {
    return a + b >= MOD ? a + b - MOD : a + b;
}

uint32_t sub(uint32_t a, uint32_t b) {
    return a >= b ? a - b : a + MOD - b;
}

uint32_t mul(uint32_t a, uint32_t b) {
    return (uint64_t(a) * b) % MOD;
}

uint32_t op(uint32_t a, uint32_t b, uint32_t c) {
    return (uint64_t(a) * b + c) % MOD;
}

struct Line {
    uint32_t k;
    uint32_t b;

    Line(uint32_t k = 1, uint32_t b = 0): k(k), b(b) {}

    Line operator()(const Line& l) const {
        return Line(mul(k, l.k), op(k, l.b, b));
    }

    uint32_t operator()(uint32_t x) const {
        return op(k, x, b);
    }
};

struct Val {
    uint32_t k;
    uint32_t b;
    uint32_t c;

    Line up() const {
        return {k, b};
    }

    uint32_t up(uint32_t x) const {
        return op(k, x, b);
    }

    Line down() const {
        return {k, c};
    }

    uint32_t down(uint32_t x) const {
        return op(k, x, c);
    }
};

Val operator*(const Val& a, const Val& b) {
    return {mul(a.k, b.k), op(b.k, a.b, b.b), op(a.k, b.c, a.c)};
}

const int MAXN = 2e5 + 100;
vector<int> g[MAXN];
int sz[MAXN], szl[MAXN];
int par[MAXN];

Line val[MAXN];

struct Jump {
    int to;
    /// int from;
    Val val;
    int par = -1;
    int first = -1;
};

int n;

Jump jumps[2 * MAXN];

int heavy[MAXN];

void calc_sz(int v) {
    sz[v] = 1;
    heavy[v] = -1;
    int idh = -1;
    int ind = -1;
    for (int u : g[v]) {
        ++ind;
        if (u == par[v]) continue;
        par[u] = v;
        calc_sz(u);
        sz[v] += sz[u];
        if (heavy[v] == -1 || sz[u] > sz[heavy[v]]) {
            heavy[v] = u;
            idh = ind;
        }
    }
    if (heavy[v] != -1) {
        szl[v] = sz[v] - sz[heavy[v]];
        swap(g[v][0], g[v][idh]);
    } else {
        szl[v] = sz[v];
    }
}

void pull(int jump) {
    jumps[jump].val = jumps[jumps[jump].first].val * jumps[jumps[jump].first - n].val;
}

const int INF = 1e9;

int head[MAXN];
int lst[MAXN];

vector<array<int, 3>> st;

void build(int v, int hd) {
    head[v] = hd;
    int cur_sz = szl[v];
    int last_jump = -1;

    auto add_jump = [&](int jump) {
        jumps[jump].to = st.back()[0];
        /// jumps[jump].from = v;
        if (last_jump == -1) {
            lst[v] = jump;
            jumps[jump].val = {val[v].k, val[v].b, val[v].b};
        } else {
            jumps[jump].first = last_jump;
            jumps[last_jump].par = jump;
            jumps[last_jump - n].par = jump;
            pull(jump);
        }
    };

    while (cur_sz > st.back()[2]) {
        int u = st.back()[0];
        add_jump(u + n);
        cur_sz += st.back()[1];
        st.pop_back();
        last_jump = u + n;
    }
    add_jump(v);
    if (heavy[v] != -1) {
        st.push_back({v, cur_sz, min(st.back()[2] - cur_sz, cur_sz)});
        build(heavy[v], hd);
    }
    for (int u : g[v]) {
        if (u == par[v] || u == heavy[v]) continue;
        st.clear();
        st.push_back({v, 0, INF});
        build(u, u);
    }
}

vector<int> path;

void build2(int v, int hd, int p) {
    head[v] = hd;
    if (v == hd) {
        path.clear();
        for (int u = v; u != -1; u = heavy[u]) {
            path.push_back(u);
        }
        auto rec = [&](auto self, int pre, int l, int r, int s) -> void {
            int need = (sz[path[l]] + s + 1) / 2;
            int lft = l, rgh = r + 1;
            while (rgh - lft > 1) {
                int m = (lft + rgh) / 2;
                if (sz[path[m]] >= need) {
                    lft = m;
                } else {
                    rgh = m;
                }
            }
            int rt = path[lft];
            if (lft != r) {
                self(self, rt, lft + 1, r, s);
            }
            int last_jump = -1;

            auto add_jump = [&](int jump) {
                if (last_jump == -1) {
                    lst[rt] = jump;
                    jumps[jump].val = {val[rt].k, val[rt].b, val[rt].b};
                } else {
                    jumps[jump].first = last_jump;
                    jumps[last_jump].par = jumps[last_jump - n].par = jump;
                    pull(jump);
                }
            };

            if (lft != l) {
                self(self, pre, l, lft - 1, sz[rt]);
                for (int u = path[lft - 1]; u >= path[l]; u = jumps[u].to) {
                    jumps[u + n].to = u;
                    add_jump(u + n);
                    last_jump = u + n;
                }
            }
            jumps[rt].to = pre;
            add_jump(rt);
        };

        rec(rec, p, 0, path.size() - 1, 0);
    }
    for (int u : g[v]) {
        if (u == par[v]) continue;
        if (u == heavy[v]) {
            build2(u, hd, p);
        } else {
            build2(u, u, v);
        }
    }
}

vector<array<int, 2>> st3;

int id3[MAXN];
int i3 = 0;

void calc_id3(int v) {
    id3[v] = i3++;
    for (int u : g[v]) {
        if (u == par[v] || u == heavy[v]) continue;
        calc_id3(u);
    }
    if (heavy[v] != -1) {
        calc_id3(heavy[v]);
    }
}

void build3(int v, int hd) {
    head[v] = hd;
    int last_jump = -1;

    auto add_jump = [&](int jump) {
        jumps[jump].to = st3.back()[0];
        if (last_jump == -1) {
            lst[v] = jump;
            jumps[jump].val = {val[v].k, val[v].b, val[v].b};
        } else {
            jumps[jump].first = last_jump;
            jumps[last_jump].par = jump;
            jumps[last_jump - n].par = jump;
            pull(jump);
        }
    };

    int cur_pr = __lg(id3[v] ^ (id3[v] + szl[v]));
    while (cur_pr > st3.back()[1]) {
        int u = st3.back()[0];
        add_jump(u + n);
        st3.pop_back();
        last_jump = u + n;
    }
    add_jump(v);
    if (heavy[v] != -1) {
        st3.push_back({v, cur_pr});
        build3(heavy[v], hd);
    }
    for (int u : g[v]) {
        if (u == par[v] || u == heavy[v]) continue;
        st3.clear();
        st3.push_back({v, INF});
        build3(u, u);
    }
}

void update(int v, Line l) {
    int jump = lst[v];
    val[v] = l;
    jumps[jump].val = {l.k, l.b, l.b};
    while (jumps[jump].par != -1) {
        jump = jumps[jump].par;
        pull(jump);
    }
}

Line mem_l[100];

uint32_t get(int u, int v, uint32_t x) {
    int ptr = 0;
    while (head[u] != head[v]) {
        if (u > v) {
            x = jumps[u].val.up(x);
            u = jumps[u].to;
        } else {
            mem_l[ptr++] = jumps[v].val.down();
            v = jumps[v].to;
        }
    }
    while (jumps[u].to >= v) {
        x = jumps[u].val.up(x);
        u = jumps[u].to;
    }
    while (jumps[v].to >= u) {
        mem_l[ptr++] = jumps[v].val.down();
        v = jumps[v].to;
    }
    if (u < v) {
        x = val[u](x);
    } else {
        mem_l[ptr++] = val[v];
    }
    while (u > v) {
        mem_l[ptr++] = jumps[v + n].val.up();
        v += n;
        while (v >= n) v = jumps[v].par;
        /// v = jumps[v + n].from;
    }
    while (u < v) {
        x = jumps[u + n].val.down(x);
        u += n;
        while (u >= n) u = jumps[u].par;
        /// u = jumps[u + n].from;
    }
    while (ptr > 0) {
        x = mem_l[--ptr](x);
    }
    return x;
}

int tin[MAXN], tim = 0;

void calc_tin(int v) {
    tin[v] = tim++;
    for (int u : g[v]) {
        if (u == par[v]/** || u == heavy[v]*/) continue;
        calc_tin(u);
    }
    /**if (heavy[v] != -1) {
        calc_tin(heavy[v]);
    }*/
}

void reorder(int n) {
    calc_tin(0);
    for (int v = 0; v < n; ++v) {
        for (int& u : g[v]) {
            u = tin[u];
        }
        if (heavy[v] != -1) {
            heavy[v] = tin[heavy[v]];
        }
        par[v] = tin[par[v]];
    }
    vector<int> mem(tin, tin + n);
    for (int v = 0; v < n; ++v) {
        while (tin[v] != v) {
            swap(g[v], g[tin[v]]);
            swap(sz[v], sz[tin[v]]);
            swap(szl[v], szl[tin[v]]);
            swap(par[v], par[tin[v]]);
            swap(heavy[v], heavy[tin[v]]);
            swap(val[v], val[tin[v]]);
            swap(tin[v], tin[tin[v]]);
        }
    }
    for (int i = 0; i < n; ++i) {
        tin[i] = mem[i];
    }
}

void solve_() {
    rd rd;
    wt wt;
    int q;
    /// cin >> n >> q;
    n = rd.uh();
    q = rd.uh();
    for (int i = 0; i < n; ++i) {
        val[i].k = rd.uw();
        val[i].b = rd.uw();
        /// cin >> val[i].k >> val[i].b;
    }
    for (int i = 1; i < n; ++i) {
        int u, v;
        /// cin >> u >> v;
        u = rd.uh();
        v = rd.uh();
        g[u].push_back(v);
        g[v].push_back(u);
    }
    calc_sz(0);
    sz[n] = n + 1;
    reorder(n);

    /// st.push_back({-1, 0, INF});
    /// build(0, 0);

    /// build2(0, 0, -1);

    calc_id3(0);
    st3.push_back({-1, INF});
    build3(0, 0);

    while (q--) {
        int tp;
        /// cin >> tp;
        tp = rd.u1();
        if (tp == 0) {
            int v;
            Line l;
            /// cin >> v >> l.k >> l.b;
            v = rd.uh();
            l.k = rd.uw();
            l.b = rd.uw();
            v = tin[v];
            update(v, l);
        } else {
            uint32_t u, v, x;
            /// cin >> u >> v >> x;
            u = rd.uh(); v = rd.uh(); x = rd.uw();
            u = tin[u]; v = tin[v];
            /// cout << get(u, v, x) << "\n";
            wt.uw(get(u, v, x));
        }
    }
}


/// #define MULTITEST

signed main() {
#ifdef LOCAL
    freopen("../input.txt", "r", stdin);
    freopen("../output.txt", "w", stdout);
#else
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#endif
    int tst = 1;
#ifdef MULTITEST
    cin >> tst;
#endif // MULTITEST
    while (tst--) {
        solve_();
    }
    return 0;
}