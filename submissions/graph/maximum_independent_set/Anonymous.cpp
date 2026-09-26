#include <bits/stdc++.h>
using namespace std;


struct MaximumIndependentSet {
    int n;
    vector<vector<int>> v;

    MaximumIndependentSet(int n_ = 0) : n(n_), v(n_) {}
    MaximumIndependentSet(int n_, vector<vector<int>> v_) : n(n_), v(v_) {}

    void add_edge(int x, int y) {
        v[x].push_back(y);
        v[y].push_back(x);
    }

    void dfs(int x, vector<char>& seen, vector<char>& comp, const vector<char>& can) {
        seen[x] = true;
        comp[x] = true;
        for (auto to : v[x]) {
            if (!seen[to] && can[to]) dfs(to, seen, comp, can);
        }
    }

    // パス order[l], order[l+1], ..., order[r-1] 上の最大安定集合を返す
    vector<int> solve_path(const vector<int>& order, int l, int r) {
        int m = r - l;
        vector<int> dp(m + 1, 0);

        for (int i = 1; i <= m; ++i) {
            dp[i] = dp[i - 1];
            if (i >= 2) dp[i] = max(dp[i], dp[i - 2] + 1);
            else dp[i] = max(dp[i], 1);
        }

        vector<int> res;
        int i = m;
        while (i > 0) {
            int take = 1;
            if (i >= 2) take += dp[i - 2];

            if (dp[i] == dp[i - 1]) {
                --i;
            } else {
                res.push_back(order[l + i - 1]);
                i -= 2;
            }
        }

        reverse(res.begin(), res.end());
        return res;
    }

    // 最大次数2以下の連結成分，すなわちパスまたはサイクルを解く
    vector<int> DegreeLeqTwoCase(const vector<char>& can) {
        vector<int> verts;
        for (int i = 0; i < n; ++i) {
            if (can[i]) verts.push_back(i);
        }

        int num = (int)verts.size();
        if (num == 0) return {};
        if (num == 1) return {verts[0]};

        int start = -1;

        // 次数1以下の頂点があればパス
        for (int x : verts) {
            int deg = 0;
            for (int to : v[x]) {
                if (can[to]) ++deg;
            }
            if (deg <= 1) {
                start = x;
                break;
            }
        }

        bool is_path = (start != -1);

        // 次数1以下の頂点がなければサイクル
        if (!is_path) start = verts[0];

        vector<int> order;
        int pre = -1;
        int cur = start;

        for (int step = 0; step < num; ++step) {
            order.push_back(cur);

            if (step + 1 == num) break;

            int nxt = -1;
            for (int to : v[cur]) {
                if (!can[to]) continue;
                if (to == pre) continue;
                nxt = to;
                break;
            }

            pre = cur;
            cur = nxt;
        }

        if (is_path) {
            return solve_path(order, 0, num);
        } else {
            // サイクルの場合
            // 1. order[0] を使わない場合: order[1..num-1] のパス
            vector<int> res1 = solve_path(order, 1, num);

            // 2. order[0] を使う場合:
            //    order[1] と order[num-1] は使えないので，
            //    order[2..num-2] のパスを解く
            vector<int> res2 = solve_path(order, 2, num - 1);
            res2.push_back(order[0]);

            if (res1.size() >= res2.size()) return res1;
            else return res2;
        }
    }

    vector<int> ConnectedCase(const vector<char>& can) {
        int pMax = -1, pMin = -1;
        int Max = -1, Min = n + 1, num = 0;

        for (int i = 0; i < n; ++i) {
            if (!can[i]) continue;

            ++num;
            int deg = 0;
            for (auto to : v[i]) {
                if (can[to]) ++deg;
            }

            if (Max < deg) {
                Max = deg;
                pMax = i;
            }
            if (Min > deg) {
                Min = deg;
                pMin = i;
            }
        }

        if (num == 0) return {};
        if (num == 1) return {pMin};

        // 連結かつ最大次数2以下なら，パスまたはサイクル
        if (Max <= 2) {
            return DegreeLeqTwoCase(can);
        }

        // 次数1の頂点 pMin がある場合，pMin はある最大安定集合に含めてよい
        if (Min < 2) {
            vector<char> ncan = can;
            ncan[pMin] = false;
            for (auto to : v[pMin]) ncan[to] = false;

            vector<int> res = GeneralCase(ncan);
            res.push_back(pMin);
            return res;
        }

        // pMax を使う場合
        vector<char> ncan = can;
        ncan[pMax] = false;
        for (auto to : v[pMax]) ncan[to] = false;

        vector<int> use = GeneralCase(ncan);
        use.push_back(pMax);

        // pMax を使わない場合
        ncan = can;
        ncan[pMax] = false;

        vector<int> not_use = GeneralCase(ncan);

        if (use.size() >= not_use.size()) return use;
        else return not_use;
    }

    vector<int> GeneralCase(const vector<char>& can) {
        vector<char> seen(n, false);
        vector<int> res;

        for (int i = 0; i < n; ++i) {
            if (!seen[i] && can[i]) {
                vector<char> gcan(n, false);
                dfs(i, seen, gcan, can);

                vector<int> sub = ConnectedCase(gcan);
                for (int x : sub) res.push_back(x);
            }
        }

        return res;
    }

    // 最大安定集合そのものを返す
    vector<int> StableSetVertices() {
        vector<char> can(n, true);
        return GeneralCase(can);
    }

    // 最大安定集合のサイズだけを返す
    int StableSet() {
        return (int)StableSetVertices().size();
    }
};

int main(){
    int n,m;
    cin>>n>>m;
    MaximumIndependentSet mis(n);
    for(int i=0;i<m;i++){
        int a,b;
        cin>>a>>b;
        mis.add_edge(a,b);
    }
    vector<int> ans = mis.StableSetVertices();
    cout<<ans.size()<<endl;
    for(int i=0;i<ans.size();i++){
        cout<<ans[i]<<(i==ans.size()-1? "\n":" ");
    }
}