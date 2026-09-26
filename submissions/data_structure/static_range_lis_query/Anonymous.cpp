#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5, B = 320;
int n, m, a[N], blen, btot, blk[N], L[B], R[B], t[N], w[N], ql[N], ans[N];
basic_string<int> q[N];
priority_queue<int, vector<int>, greater<int> > P[B];
priority_queue<int> Q[B];
void upd(int x) { while (x) t[x]++, x -= x & -x; }
int qry(int x) { int y = 0; while (x <= n) y += t[x], x += x & -x; return y; }
void ins(int x) {
	int v = a[x], b = blk[v];
	if (P[b].size()) {
		for (int i = L[b]; i <= R[b]; i++) if (w[i]) P[b].push(w[i]), w[i] = P[b].top(), P[b].pop();
		P[b] = priority_queue<int, vector<int>, greater<int> >();
	}
	w[v] = x, Q[b].push(x);
	int mx = 0, tg = 0;
	for (int i = v + 1; i <= R[b]; i++) if (mx < w[i]) swap(mx, w[i]), tg = 1;
	if (tg) {
		Q[b] = priority_queue<int>();
		for (int i = L[b]; i <= R[b]; i++) if (w[i]) Q[b].push(w[i]);
	}
	for (int i = b + 1; i <= btot; i++) {
		if (Q[i].size() && mx < Q[i].top()) {
			P[i].push(mx); if (mx) Q[i].push(mx);
			mx = Q[i].top(), Q[i].pop();
		}
	}
	upd(mx);
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> n >> m, blen = sqrt(n);
	for (int i = 1; i <= n; i++) cin >> a[i],a[i]++,blk[i] = (i - 1) / blen + 1; btot = blk[n];
	for (int i = 1; i <= btot; i++) L[i] = R[i - 1] + 1, R[i] = i * blen; R[btot] = n;
	for (int i = 1, r; i <= m; i++) cin >> ql[i] >> r, ql[i]++,q[r].push_back(i), ans[i] = r - ql[i] + 1;
	for (int i = 1; i <= n; i++) { ins(i); for (int j : q[i]) ans[j] -= qry(ql[j]); }
	for (int i = 1; i <= m; i++) cout << ans[i] << '\n';
}