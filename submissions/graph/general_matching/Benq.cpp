// modifying nor's sub

#include <cstdio>
#include <utility>

using namespace std;

const int BufL = 1100000;

char buf[BufL], *ins = buf, *outs = buf;

inline int getint() {
    while (*ins < '0' || *ins > '9') ++ins;
    int res = 0;
    while (*ins >= '0' && *ins <= '9') res = res * 10 + *ins++ - '0';
    return res;
}
inline void putint(int x, char c = ' ') {
    if (!x)
        *outs++ = '0';
    else {
        char s_pool[4], *s = s_pool;
        for (; x; x /= 10) *s++ = x % 10 + '0';
        while (s-- != s_pool) *outs++ = *s;
    }
    *outs++ = c;
}

const int MaxN = 501;
const int MaxM = MaxN * (MaxN - 1);

int n, m;
int nE, adj[MaxN];
int next[MaxM], go[MaxM];

inline void addEdge(const int &u, const int &v) {
    next[++nE] = adj[u], go[adj[u] = nE] = v;
    next[++nE] = adj[v], go[adj[v] = nE] = u;
}

int n_matches;
int mate[MaxN];

int q_n;
int q[MaxN];
int book_mark;
int book[MaxN];

int type[MaxN];
int fa[MaxN];
int bel[MaxN];

inline void augment(int u) {
    while (u) {
        int nu = mate[fa[u]];
        mate[mate[u] = fa[u]] = u;
        u = nu;
    }
}

inline int get_lca(int u, int v) {
    ++book_mark;
    while (true) {
        if (u) {
            if (book[u] == book_mark) return u;
            book[u] = book_mark;
            u = bel[fa[mate[u]]];
        }
        swap(u, v);
    }
}

inline void go_up(int u, int v, const int &mv) {
    while (bel[u] != mv) {
        fa[u] = v;
        v = mate[u];
        if (type[v] == 1) type[q[++q_n] = v] = 0;
        // if (bel[u] == u) bel[u] = mv;
        // if (bel[v] == v) bel[v] = mv;
        bel[u] = bel[v] = mv;
        u = fa[v];
    }
}
inline void after_go_up() {
    for (int u = 1; u <= n; ++u) bel[u] = bel[bel[u]];
}

inline bool match(const int &sv) {
    for (int u = 1; u <= n; ++u) bel[u] = u, type[u] = -1;
    type[q[q_n = 1] = sv] = 0;
    for (int i = 1; i <= q_n; ++i) {
        int u = q[i];
        for (int e = adj[u]; e; e = next[e]) {
            int v = go[e];
            if (!~type[v]) {
                fa[v] = u, type[v] = 1;
                int nu = mate[v];
                if (!nu) {
                    augment(v);
                    return true;
                }
                type[q[++q_n] = nu] = 0;
            } else if (!type[v] && bel[u] != bel[v]) {
                int lca = get_lca(u, v);
                go_up(u, v, lca);
                go_up(v, u, lca);
                after_go_up();
            }
        }
    }
    return false;
}

inline void calc_max_match() {
    n_matches = 0;
    for (int u = 1; u <= n; ++u)
        if (!mate[u] && match(u)) ++n_matches;
}

int main() {
    fread(buf, 1, BufL, stdin);
    n = getint(), m = getint();
    while (m--) addEdge(getint() + 1, getint() + 1);
    calc_max_match();
    putint(n_matches, '\n');
    for (int u = 1; u <= n; ++u)
        if (mate[u] > u) putint(mate[u] - 1, ' '), putint(u - 1, '\n');
    fwrite(buf, 1, outs - buf, stdout);
    return 0;
}