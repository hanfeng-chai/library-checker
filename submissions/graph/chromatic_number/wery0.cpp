#include <bits/stdc++.h>
using namespace std;


int main() {
    int n, m;
    cin >> n >> m;
    vector g(n, vector<int>());
    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        g[x].emplace_back(y);
        g[y].emplace_back(x);
    }

    const int G = 64;
    assert(n <= G);
    int best = n + 1;
    vector<int> best_col;
    function<void(int, int)> go = [&](int mx, int eso) {
        static vector<int> col(n);
        if (mx >= best) return;
        if (eso == 0) {
            best = mx, best_col = col;
            return;
        }

        int mxb = -1, v;
        bitset<G> maskv;
        for (int i = 0; i < n; ++i) {
            if (col[i]) continue;
            bitset<G> mask;
            for (int h : g[i]) mask[col[h]] = 1;
            int r = mask.count() - mask[0];
            if (r > mxb) mxb = r, v = i, maskv = mask;
        }

        for (int c = 1; c < mx + 2; ++c) {
            if (maskv[c]) continue;
            col[v] = c;
            go(max(mx, c), eso - 1);
        }
        col[v] = 0;
    };
    go(0, n);
    cout << best << '\n';
    for (int i = 0; i < n; ++i) {
        for (auto h : g[i]) assert(best_col[i] != best_col[h]);
    }
}
