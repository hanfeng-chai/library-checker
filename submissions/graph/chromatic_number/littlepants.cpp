#include <bits/stdc++.h>
using namespace std;

int N, M;
int adj[20];       // 鄰接遮罩
int col[20];       // 已指派顏色 (0-indexed)，-1 = 未著色
int sat[20];       // 鄰居已用顏色的 bitmask
int best;          // 目前最佳解 (chromatic number upper bound)

// ── 初始上界：DSATUR 貪心（無回溯） ──────────────────────────
int greedyUpperBound() {
    int c[20]; fill(c, c + N, -1);
    int s[20] = {};
    int maxC = 0;
    for (int iter = 0; iter < N; iter++) {
        int v = -1, bSat = -1, bDeg = -1;
        for (int i = 0; i < N; i++) {
            if (c[i] != -1) continue;
            int si = __builtin_popcount(s[i]);
            int di = __builtin_popcount(adj[i]);
            if (si > bSat || (si == bSat && di > bDeg))
                v = i, bSat = si, bDeg = di;
        }
        int color = __builtin_ctz(~s[v]);   // 最小可用顏色
        c[v] = color;
        maxC = max(maxC, color + 1);
        for (int tmp = adj[v]; tmp; tmp &= tmp - 1)
            if (int u = __builtin_ctz(tmp); c[u] == -1)
                s[u] |= 1 << color;
    }
    return maxC;
}

// ── 下界：貪心最大 Clique ─────────────────────────────────────
int greedyClique() {
    int lb = 1;
    for (int start = 0; start < N; start++) {
        int cand = adj[start], clique = 1 << start;
        while (cand) {
            // 在候選中選鄰居最多的點（在 cand 內）
            int best_v = -1, best_cnt = -1;
            for (int tmp = cand; tmp; tmp &= tmp - 1) {
                int u = __builtin_ctz(tmp);
                int cnt = __builtin_popcount(adj[u] & cand);
                if (cnt > best_cnt) best_cnt = cnt, best_v = u;
            }
            clique |= 1 << best_v;
            cand &= adj[best_v];
        }
        lb = max(lb, __builtin_popcount(clique));
    }
    return lb;
}

// ── DSATUR 回溯搜索 ───────────────────────────────────────────
// colored : 已著色頂點數
// maxColor: 目前用到的顏色數 (= max assigned color + 1)
void solve(int colored, int maxColor) {
    if (colored == N) { best = maxColor; return; }
    if (maxColor >= best) return;   // 剪枝：已無法改善

    // 選飽和度最高的未著色頂點，tie-break by degree
    int v = -1, bSat = -1, bDeg = -1;
    for (int i = 0; i < N; i++) {
        if (col[i] != -1) continue;
        int s = __builtin_popcount(sat[i]);
        int d = __builtin_popcount(adj[i]);
        if (s > bSat || (s == bSat && d > bDeg))
            v = i, bSat = s, bDeg = d;
    }

    // 可嘗試的顏色：
    //   既有顏色 0..maxColor-1 中未被鄰居用的
    //   + 若 maxColor < best-1，可開一個新顏色 maxColor
    int tryMask = ((1 << maxColor) - 1) & ~sat[v];
    if (maxColor < best - 1) tryMask |= (1 << maxColor);

    while (tryMask) {
        int bit   = tryMask & -tryMask;   // 取最低 set bit
        tryMask  ^= bit;
        int c     = __builtin_ctz(bit);

        col[v] = c;
        int newMax = max(maxColor, c + 1);

        // 儲存並更新鄰居的 sat
        int oldSat[20];
        for (int tmp = adj[v]; tmp; tmp &= tmp - 1) {
            int u = __builtin_ctz(tmp);
            if (col[u] == -1) { oldSat[u] = sat[u]; sat[u] |= bit; }
        }

        solve(colored + 1, newMax);

        // 回溯
        col[v] = -1;
        for (int tmp = adj[v]; tmp; tmp &= tmp - 1) {
            int u = __builtin_ctz(tmp);
            if (col[u] == -1) sat[u] = oldSat[u];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    for (int i = 0; i < M; i++) {
        int u, v; cin >> u >> v;
        adj[u] |= 1 << v;
        adj[v] |= 1 << u;
    }

    int lb = greedyClique();
    best   = greedyUpperBound();

    fill(col, col + N, -1);
    fill(sat, sat + N, 0);
    solve(0, 0);

    cout << best << "\n";
}