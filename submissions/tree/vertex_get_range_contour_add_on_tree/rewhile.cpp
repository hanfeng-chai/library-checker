#include <bits/stdc++.h>
using ll = long long;
using namespace std;

struct Contour {
  vector<int> C, B, head, off, len;
  vector<int> info;
  vector<ll> bit, value;

  Contour(vector<int> deg, vector<int> P) {
    int n = P.size(), lg = 1;
    while ((1 << lg) <= n) ++lg;
    C.assign(n, -1), B.assign(n, -1), head.assign(n + 1, 0);
    off.resize(2 * n), len.resize(2 * n), value.resize(n);
    bit.reserve(2 * n * lg);
    vector<int> ord(n), tmp(n), sz(n, 1), d(n), cur(n);
    int k = n;
    deg[0] += 2;
    for (int i = 0; i < n; ++i) {
      int u = i;
      while (deg[u] == 1) {
        int v = P[u];
        deg[u] = 0, --deg[v], P[v] ^= u;
        ord[--k] = u;
        u = v;
      }
    }
    ord[0] = 0, sz[0] = n;
    {
      vector<int> first = ord;
      for (int active = n, level = 0; active; ++level) {
        int next = 0;
        for (int l = 0; l < active;) {
          int root = ord[l], m = sz[root], r = l + m, c = root;
          for (int i = l; i < r; ++i) sz[ord[i]] = 1;
          bool found = false;
          for (int i = r - 1; i > l; --i) {
            int u = ord[i];
            if (!found && 2 * sz[u] > m) c = u, found = true;
            sz[P[u]] += sz[u];
          }
          head[c + 1] = level;
          int rest = m - sz[c];
          for (int i = l; i < r; ++i) {
            int u = ord[i];
            if (u == c) continue;
            if (u == root || P[u] == c) {
              deg[u] = u, cur[u] = next;
              if (u == root) sz[u] = rest;
              next += sz[u];
            } else deg[u] = deg[P[u]];
          }
          for (int i = l; i < r; ++i) {
            int u = ord[i];
            if (u != c) tmp[cur[deg[u]]++] = u;
          }
          l = r;
        }
        ord.swap(tmp), active = next;
      }
      ord.swap(first);
    }
    for (int i = 0; i < n; ++i) head[i + 1] += head[i];
    info.resize(head[n]), sz[0] = n;
    int groups = n;
    for (int active = n, level = 0; active; ++level) {
      int next = 0;
      for (int l = 0; l < active;) {
        int root = ord[l], m = sz[root], r = l + m, c = root;
        for (int i = l; i < r; ++i) sz[ord[i]] = 1, d[ord[i]] = -1;
        bool found = false;
        for (int i = r - 1; i > l; --i) {
          int u = ord[i];
          if (!found && 2 * sz[u] > m) c = u, found = true;
          sz[P[u]] += sz[u];
        }
        C[c] = C[root], B[c] = B[root];
        int rest = m - sz[c];
        d[c] = 0;
        for (int u = c; u != root; u = P[u]) d[P[u]] = d[u] + 1;
        int height = 0, branches = 0;
        for (int i = l; i < r; ++i) {
          int u = ord[i];
          if (d[u] < 0) d[u] = d[P[u]] + 1;
          height = max(height, d[u]);
          if (u == c) continue;
          if (u == root || P[u] == c) deg[u] = u, cur[u] = 0, ++branches;
          else deg[u] = deg[P[u]];
          cur[deg[u]] = max(cur[deg[u]], d[u]);
        }
        if (branches > 2) {
          off[c] = bit.size(), len[c] = height + 1;
          bit.resize(bit.size() + len[c]);
        } else off[c] = groups, len[c] = -branches;
        for (int i = l; i < r; ++i) {
          int u = ord[i];
          if (u == c || deg[u] != u) continue;
          int g = B[u] = groups++;
          off[g] = bit.size(), len[g] = cur[u];
          bit.resize(bit.size() + len[g]);
          cur[u] = next;
          if (u == root) sz[u] = rest;
          next += sz[u], C[u] = c;
        }
        for (int i = l; i < r; ++i) {
          int u = ord[i], g = u == c ? -1 : B[deg[u]];
          if (u != c) info[head[u + 1] - 1 - level] = d[u];
          if (g >= 0) {
            tmp[cur[deg[u]]++] = u;
          }
        }
        l = r;
      }
      ord.swap(tmp), active = next;
    }
  }

  void add(int g, int i, ll x) {
    for (; i < len[g]; i |= i + 1) bit[off[g] + i] += x;
  }

  ll get(int g, int i) const {
    ll ans = 0;
    for (++i; i > 0; i &= i - 1) ans += bit[off[g] + i - 1];
    return ans;
  }

  void range_add(int g, int l, int r, ll x) {
    l = max(l, 0), r = min(r, len[g]);
    if (l < r) add(g, l, x), add(g, r, -x);
  }

  void apply(int u, int l, int r, ll x) {
    if (len[u] > 0) range_add(u, l, r, x);
    else {
      if (l == 0) value[u] += x;
      for (int g = off[u]; g < off[u] - len[u]; ++g) {
        range_add(g, l - 1, r - 1, x);
      }
    }
    for (int c = C[u], p = u, j = head[u]; c >= 0; p = c, c = C[c], ++j) {
      int d = info[j], sub = B[p];
      if (len[c] > 0) {
        range_add(c, l - d, r - d, x);
        range_add(sub, l - d - 1, r - d - 1, x);
      } else {
        if (l <= d && d < r) value[c] += x;
        if (len[c] == -2) range_add(2 * off[c] + 1 - sub, l - d - 1, r - d - 1, x);
      }
    }
  }

  ll get(int u) const {
    ll ans = value[u];
    if (len[u] > 0) ans += get(u, 0);
    for (int c = C[u], p = u, j = head[u]; c >= 0; p = c, c = C[c], ++j) {
      int d = info[j], sub = B[p];
      if (len[c] > 0) ans += get(c, d) - get(sub, d - 1);
      else ans += get(sub, d - 1);
    }
    return ans;
  }
};

int32_t main() {
  cin.tie(0)->sync_with_stdio(0);
  int n, q;
  cin >> n >> q;
  vector<ll> a(n);
  for (ll& x : a) cin >> x;
  vector<int> deg(n), P(n);
  for (int i = 1, u, v; i < n; ++i) {
    cin >> u >> v;
    ++deg[u], ++deg[v], P[u] ^= v, P[v] ^= u;
  }
  Contour tree(move(deg), move(P));
  for (int i = 0, t, p; i < q; ++i) {
    cin >> t >> p;
    if (t == 0) {
      int l, r;
      ll x;
      cin >> l >> r >> x;
      tree.apply(p, l, r, x);
    } else {
      cout << a[p] + tree.get(p) << '\n';
    }
  }
}
