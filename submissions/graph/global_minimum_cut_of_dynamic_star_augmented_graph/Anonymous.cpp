#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

struct SegTree {
  int n = 1;
  vector<i64> mn, lazy;

  void init(int sz) {
    n = 1;
    while (n < sz) n <<= 1;
    mn.assign(n * 2, (i64)4e18);
    lazy.assign(n * 2, 0);
  }

  void apply(int p, i64 x) {
    mn[p] += x;
    lazy[p] += x;
  }

  void push(int p) {
    if (lazy[p] == 0) return;
    apply(p << 1, lazy[p]);
    apply(p << 1 | 1, lazy[p]);
    lazy[p] = 0;
  }

  void range_add(int p, int l, int r, int ql, int qr, i64 x) {
    if (qr <= l || r <= ql) return;
    if (ql <= l && r <= qr) {
      apply(p, x);
      return;
    }
    push(p);
    int mid = (l + r) >> 1;
    range_add(p << 1, l, mid, ql, qr, x);
    range_add(p << 1 | 1, mid, r, ql, qr, x);
    mn[p] = min(mn[p << 1], mn[p << 1 | 1]);
  }

  void add(int l, int r, i64 x) {
    range_add(1, 0, n, l, r, x);
  }

  i64 point_get(int p, int l, int r, int pos) {
    if (r - l == 1) return mn[p];
    push(p);
    int mid = (l + r) >> 1;
    if (pos < mid) return point_get(p << 1, l, mid, pos);
    return point_get(p << 1 | 1, mid, r, pos);
  }

  i64 get(int pos) {
    return point_get(1, 0, n, pos);
  }

  void set_val(int pos, i64 x) {
    i64 cur = get(pos);
    add(pos, pos + 1, x - cur);
  }

  i64 all_min() const {
    return mn[1];
  }
};

void solve() {
  int n, m, q;
  cin >> n >> m >> q;

  vector<i64> a(n);
  for (int i = 0; i < n; i++) cin >> a[i];

  vector<vector<pair<int, i64>>> g(n);
  vector<tuple<int, int, i64>> edges;
  for (int i = 0; i < m; i++) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    edges.push_back({u, v, w});
    if (w > 0) {
      g[u].push_back({v, w});
      g[v].push_back({u, w});
    }
  }

  vector<vector<int>> tr(2 * n - 1);

  auto build_tree = [&]() {
    vector<vector<pair<int, i64>>> cur = g;
    vector<int> used(2 * n - 1, 1);
    cur.resize(2 * n - 1);

    for (int step = 0; step < n - 1; step++) {
      int tot = n + step;
      vector<int> ord(n - step), vis(tot, 0);
      vector<i64> cost(tot, 0);
      priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;

      for (int v = 0; v < tot; v++) {
        if (!used[v]) continue;
        for (auto [u, w] : cur[v]) cost[v] += w;
        pq.push({cost[v], v});
      }

      for (int j = 0; j < n - step; j++) {
        while (true) {
          auto [c, v] = pq.top();
          pq.pop();
          if (vis[v]) continue;
          vis[v] = 1;
          ord[j] = v;
          for (auto [u, w] : cur[v]) {
            if (!vis[u]) {
              cost[u] -= w;
              pq.push({cost[u], u});
            }
          }
          break;
        }
      }

      int x = ord[n - step - 1];
      int y = ord[n - step - 2];
      used[x] = used[y] = 0;
      cur[x].clear();
      cur[y].clear();

      for (int v = 0; v < tot; v++) {
        if (!used[v]) continue;
        for (auto &[u, w] : cur[v]) {
          if (u == x || u == y) {
            u = tot;
            cur[tot].push_back({v, w});
          }
        }
      }

      tr[tot].push_back(x);
      tr[tot].push_back(y);
    }
  };

  build_tree();

  int sz = 2 * n - 1;
  int root = sz - 1;
  vector<int> parent(sz, -1), depth(sz, 0), heavy(sz, -1), head(sz), id(sz);

  auto dfs = [&](auto self, int v) -> int {
    int size = 1, best = 0;
    for (int u : tr[v]) {
      parent[u] = v;
      depth[u] = depth[v] + 1;
      int sub = self(self, u);
      size += sub;
      if (sub > best) {
        best = sub;
        heavy[v] = u;
      }
    }
    return size;
  };

  dfs(dfs, root);

  int timer = 0;
  queue<int> que;
  que.push(root);
  while (!que.empty()) {
    int start = que.front();
    que.pop();
    for (int v = start; v != -1; v = heavy[v]) {
      head[v] = start;
      id[v] = timer++;
      for (int u : tr[v]) {
        if (u != heavy[v]) que.push(u);
      }
    }
  }

  vector<i64> cut(sz, 0);
  for (auto [u0, v0, w] : edges) {
    int u = u0, v = v0;
    while (u != v) {
      if (depth[u] < depth[v]) swap(u, v);
      cut[u] += w;
      u = parent[u];
    }
  }

  SegTree seg;
  seg.init(sz);
  for (int v = 0; v < sz; v++) {
    seg.set_val(id[v], cut[v]);
  }

  auto path_add = [&](int v, i64 delta) {
    while (v != -1) {
      seg.add(id[head[v]], id[v] + 1, delta);
      v = parent[head[v]];
    }
  };

  for (int i = 0; i < n; i++) {
    path_add(i, a[i]);
  }

  while (q--) {
    int x;
    i64 y;
    cin >> x >> y;
    path_add(x, y - a[x]);
    a[x] = y;
    cout << seg.all_min() << '\n';
  }
}

int main() {
  cin.tie(0)->sync_with_stdio(0);

  solve();
  return 0;
}