//      - Art -
#include <bits/stdc++.h>

#define el              cout << '\n'

#define FOR(i, a, b)    for (int i = (a), _b = (b); i <= _b; ++i)
#define REV(i, b, a)    for (int i = (b), _a = (a); i >= _a; --i)
#define REP(i, c)       for (int i = 0, _c = (c); i < _c; ++i)

const int N = 2e3 + 7;

template <class T1, class T2>
    bool maximize(T1 &a, T2 b) {
        if (a < b) {a = b; return true;}
        return false;
    }

template <class T1, class T2>
    bool minimize(T1 &a, T2 b) {
        if (a > b) {a = b; return true;}
        return false;
    }

using namespace std;

struct Edge {
    int u, v, w;
    Edge(int _u = 0, int _v = 0, int _w = 0) {
        u = _u; v = _v; w = _w;
    }
    int other(int x) const {
        return u ^ v ^ x;
    }
} edge[N];

int n;
vector<int> adj[N];
long long d[N];
int trace[N];

void dijkstra(int source) {
    memset(d, 0x3f, sizeof d);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> Q;
    Q.emplace(d[source] = 0, source);
    trace[source] = -1;
    while (!Q.empty()) {
        auto [du, u] = Q.top();
        Q.pop();
        for (int &id : adj[u]) {
            int v = edge[id].other(u);
            int w = edge[id].w;
            if (minimize(d[v], d[u] + w)) {
                Q.emplace(d[v], v);
                trace[v] = id;
            }
        }
    }
}

long long best = 1e18;
vector<int> ansEdge;
bool del[N];

vector<int> tree[N];
long long diameter;
long long f[N][2];

void dfs(int u, int p = -1) {
    for (int &id : tree[u]) {
        int v = edge[id].other(u);
        int w = edge[id].w;
//        cout << u << ' ' << v << ' ' << w, el;
        if (v != p) {
            dfs(v, u);
            if (f[v][1] + w > f[u][1]) {
                f[u][2] = f[u][1];
                f[u][1] = f[v][1] + w;
            }
            else {
                maximize(f[u][2], f[v][1] + w);
            }
        }
    }
    maximize(diameter, f[u][1] + f[u][2]);
}

void Try(int s) {
    dijkstra(s);
    memset(del, 0, sizeof del);
    vector<int> listEdge;

    FOR (i, 1, n) {
        int u = i;
        while (u != s) {
            int id = trace[u];
            if (del[id]) {
                break;
            }
            del[id] = 1;

            listEdge.emplace_back(id);
            int v = edge[id].other(u);
            tree[u].emplace_back(id);
            tree[v].emplace_back(id);
            u = v;
        }
    }
//    FOR (i, 1, n) {
//        for (int &id : tree[i]) {
//            cout << id << ' ';
//        }
//    }

    diameter = 0;
    memset(f, 0, sizeof f);
    dfs(s);
//    cout << diameter, el;

    if (minimize(best, diameter)) {
        ansEdge = move(listEdge);
    }
    FOR (i, 1, n) {
        tree[i].clear();
    }
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int m;
    cin >> n >> m;
    FOR (i, 1, m) {
        int u, v, w;
        cin >> u >> v >> w;
        ++u; ++v;
        edge[i] = Edge(u, v, w);
        adj[u].emplace_back(i);
        adj[v].emplace_back(i);
    }

    FOR (i, 1, n) {
        Try(i);
    }

    cout << best, el;
    for (int &id : ansEdge) {
        cout << id - 1 << ' ';
    }

    return 0;
}