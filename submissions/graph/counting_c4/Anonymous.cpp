#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

vector<i64> count_c4_simple(int n, vector<int> u, vector<int> v, vector<i64> w) {
  int m = (int)w.size();

  vector<int> deg(n);
  for (int i = 0; i < m; i++) {
    deg[u[i]]++;
    deg[v[i]]++;
  }

  vector<int> ord(n), pos(n);
  iota(ord.begin(), ord.end(), 0);
  stable_sort(ord.begin(), ord.end(), [&](int a, int b) {
    return deg[a] < deg[b];
  });

  for (int i = 0; i < n; i++) {
    pos[ord[i]] = i;
  }
  for (int i = 0; i < m; i++) {
    u[i] = pos[u[i]];
    v[i] = pos[v[i]];
    if (u[i] < v[i]) swap(u[i], v[i]);
  }

  vector<int> start(n);
  for (int i = 0; i + 1 < n; i++) {
    start[i + 1] = start[i] + deg[ord[i]];
  }

  vector<int> end = start;
  vector<int> eid(2 * m), to(2 * m);

  for (int i = 0; i < m; i++) {
    int a = u[i], b = v[i];
    eid[end[a]] = i;
    to[end[a]] = b;
    end[a]++;
  }

  vector<int> down_end = end;
  for (int x = 0; x < n; x++) {
    for (int p = start[x]; p < down_end[x]; p++) {
      int id = eid[p];
      int y = to[p];
      eid[end[y]] = id;
      to[end[y]] = x;
      end[y]++;
    }
  }

  vector<i64> c(n), ans(m);

  for (int x = n - 1; x >= 0; x--) {
    for (int p = start[x]; p < end[x]; p++) {
      int e1 = eid[p];
      int y = to[p];

      end[y]--;

      for (int q = start[y]; q < end[y]; q++) {
        int e2 = eid[q];
        int z = to[q];
        c[z] += w[e1] * w[e2];
      }
    }

    for (int p = start[x]; p < end[x]; p++) {
      int e1 = eid[p];
      int y = to[p];

      for (int q = start[y]; q < end[y]; q++) {
        int e2 = eid[q];
        int z = to[q];

        i64 cur = w[e1] * w[e2];
        i64 val = c[z] - cur;
        ans[e1] += val * w[e2];
        ans[e2] += val * w[e1];
      }
    }

    for (int p = start[x]; p < end[x]; p++) {
      int y = to[p];
      for (int q = start[y]; q < end[y]; q++) {
        c[to[q]] = 0;
      }
    }
  }

  return ans;
}

void solve() {
  int n, m;
  cin >> n >> m;

  vector<int> u(m), v(m);
  for (int i = 0; i < m; i++) {
    cin >> u[i] >> v[i];
    if (u[i] > v[i]) swap(u[i], v[i]);
  }

  vector<int> id(m);
  iota(id.begin(), id.end(), 0);
  stable_sort(id.begin(), id.end(), [&](int a, int b) {
    if (u[a] != u[b]) return u[a] < u[b];
    return v[a] < v[b];
  });

  vector<int> cu, cv, belong(m);
  vector<i64> cw;

  for (int i = 0; i < m; i++) {
    int e = id[i];
    if (cu.empty() || cu.back() != u[e] || cv.back() != v[e]) {
      cu.push_back(u[e]);
      cv.push_back(v[e]);
      cw.push_back(0);
    }
    cw.back()++;
    belong[e] = (int)cu.size() - 1;
  }

  vector<i64> res = count_c4_simple(n, cu, cv, cw);

  for (int i = 0; i < m; i++) {
    if (i) cout << ' ';
    cout << res[belong[i]];
  }
  cout << '\n';
}

int main() {
  cin.tie(0)->sync_with_stdio(0);
  solve();
  return 0;
}