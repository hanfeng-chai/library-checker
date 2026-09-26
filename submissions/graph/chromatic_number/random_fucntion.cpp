#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int N = 20;
bool adj[N][N];
int dp[1 << N];
bool indep[1 << N];

signed main()
{
    cin.tie(0)->sync_with_stdio(0);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> G(n);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        cin >> u >> v;
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    int ans = n + 1;
    vector<int> col;
    const auto dfs = [&](auto &dfs, int c, int cnt) -> void
    {
        static vector<int> cur(n);
        if (c >= ans)
            return;
        if (!cnt)
            return ans = c, col = cur, void();
        int u = -1, d = -1;
        ll q = 0;
        for (int i = 0; i < n; ++i)
            if (!cur[i])
            {
                ll s = 0;
                for (int j : G[i])
                    if (cur[j])
                        s |= 1ll << cur[j];
                int t = __builtin_popcountll(s);
                if (t > d)
                    d = t, u = i, q = s;
            }
        for (int i = 1; i <= c + 1; ++i)
            if (!(q >> i & 1))
                cur[u] = i, dfs(dfs, max(c, i), cnt - 1);
        cur[u] = 0;
    };
    dfs(dfs, 0, n);
    cout << ans;
    return 0;
}