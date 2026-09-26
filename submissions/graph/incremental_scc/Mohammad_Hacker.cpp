#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define log(x) (63^__builtin_clzll(x))

const int N = 1e6 + 5, mod = 998244353;

pair<int, int> edges[N];
basic_string<int> adj[N];
int low[N], scc[N], scc_cnt, tick;
int stk[N], stk_top;
 
void dfs(int v) {
  low[v] = ++tick;
  int cur = low[v];
  stk[++stk_top] = v;
  for(int u : adj[v]){
    if(!low[u]) dfs(u);
    low[v] = min(low[v], low[u]);
  }
  if(low[v] == cur){
    scc_cnt++;
    while(1){
      int u = stk[stk_top--];
      scc[u] = scc_cnt;
      low[u] = N;
      if(u == v) break;
    }
  }
}

void _clear(int sz) {
  for(int v = 1 ; v <= sz ; v++){
    scc[v] = low[v] = 0;
    basic_string<int>().swap(adj[v]);
  }
  scc_cnt = tick = 0;
}
 
int id[N], id_tick;
int min_time[N];

void dac(int L, int R, vector<array<int, 3>> &E) {
  if(L > R || E.empty()) return;
  int mid = (L + R) / 2;
  for(auto [x, y, i] : E) {
    if(!id[x]) id[x] = ++id_tick;
    if(!id[y]) id[y] = ++id_tick;
    if(i <= mid) adj[id[x]].push_back(id[y]);
  }
  for(int v = 1 ; v <= id_tick ; v++){
    if(!scc[v]) dfs(v);
  }
  vector<array<int, 3> > Left, Right;
  for(auto [x, y, i] : E){
    x = id[x]; y = id[y];
    if(i > mid){
      Right.push_back({scc[x], scc[y], i});
    } else {
      if(scc[x] == scc[y]){
        min_time[i] = (min_time[i] ? min(min_time[i], mid) : mid);
        if(i < mid) Left.push_back({x, y, i});
      } else {
        Right.push_back({scc[x], scc[y], i});
      }
    }
  }
  _clear(id_tick);
  for(auto [x, y, i] : E){
    id[x] = id[y] = 0;
  }
  id_tick = 0;
  vector<array<int, 3>>().swap(E);
  dac(L, mid - 1, Left);
  dac(mid + 1, R, Right);
}

int par[N], sz[N];
ll tot[N], tot2[N];
ll cost;

int find(int x) {
  if(x == par[x]) return x;
  return par[x] = find(par[x]);
}
 
void merge(int x, int y) {
  x = find(x);
  y = find(y);
  if(x == y) return;
  if(sz[x] < sz[y]) swap(x, y);
  par[y] = x;
  cost -= tot[x] * tot[x] % mod - tot2[x];
  cost -= tot[y] * tot[y] % mod - tot2[y];
  sz[x] += sz[y];
  tot2[x] += tot2[y];
  tot2[x] %= mod;
  tot[x] += tot[y];
  tot[x] %= mod;
  cost += tot[x] * tot[x] % mod - tot2[x];
  cost %= mod;
  cost += mod;
  cost %= mod;
}

// ==================================
int x[N];

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0);
    ll inv2 = (mod + 1) / 2;
    int n, m; cin >> n >> m;
    for(int i=1;i<=n;i++) cin >> x[i];
    vector<array<int, 3>> E;
    for(int i=0;i<m;i++) {
        int u, v; cin >> u >> v;
        u++, v++;
        edges[i] = {u, v};
        E.push_back({u, v, i});
    }
    dac(0, m-1, E);
    for(int i=1;i<=n;i++) {
        tot[i] = x[i];
        tot2[i] = x[i] * x[i] % mod;
        sz[i] = 1;
        par[i] = i;
    }
    vector<int> g[m];
    for(int i=0;i<m;i++) {
        if(!min_time[i]) continue;
        g[min_time[i]].push_back(i);
    }
    for(int i=0;i<m;i++) {
        for(int e: g[i]) {
            merge(edges[e].first, edges[e].second);
        }
        cout << cost * inv2 % mod << '\n';
    }
}