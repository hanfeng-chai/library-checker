#include <bits/stdc++.h>
using namespace std;
using lint = long long;
using pi = array<lint, 2>;
#define sz(v) ((int)(v).size())
#define all(v) (v).begin(), (v).end()
#define cr(v, n) (v).clear(), (v).resize(n);

// Let a matrix be monotone if Opt(i) <= Opt(i + 1) for all rows i.
// Given a totally monotone matrix (where every 2x2 submatrix is monotone)
// find the list of row maxima positions.
// Source: maroonrk

template <class F, class D = lint> vector<int> smawk(F f, int n, int m) {
	vector<int> ans(n, -1);
	auto rec = [&](auto self, int *const rs, int x, int *const cs, int y) -> void {
		const int t = 8;
		if (x <= t || y <= t) {
			for (int i = 0; i < x; i++) {
				int r = rs[i];
				D mx;
				for (int j = 0; j < y; j++) {
					int c = cs[j];
					D w = f(r, c);
					if (ans[r] == -1 || w > mx) {
						ans[r] = c;
						mx = w;
					}
				}
			}
			return;
		}
		if (x < y) {
			int s = 0;
			for (int i = 0; i < y; i++) {
				int c = cs[i];
				while (s && f(rs[s - 1], cs[s - 1]) < f(rs[s - 1], c))
					s--;
				if (s < x)
					cs[s++] = c;
			}
			y = s;
		}
		auto a = rs + x, b = cs + y;
		int z = 0;
		for (int i = 1; i < x; i += 2)
			a[z++] = rs[i];
		for (int i = 0; i < y; i++)
			b[i] = cs[i];
		self(self, a, z, b, y);
		int k = 0;
		for (int i = 0; i < x; i += 2) {
			int to = i + 1 < x ? ans[rs[i + 1]] : cs[y - 1];
			D mx;
			while (1) {
				D w = f(rs[i], cs[k]);
				if (ans[rs[i]] == -1 || w > mx) {
					ans[rs[i]] = cs[k];
					mx = w;
				}
				if (cs[k] == to)
					break;
				k++;
			}
		}
	};
	int *rs = new int[n * 2];
	for (int i = 0; i < n; i++)
		rs[i] = i;
	int *cs = new int[max(m, n * 2)];
	for (int i = 0; i < m; i++)
		cs[i] = i;
	rec(rec, rs, n, cs, m);
	delete[] rs;
	delete[] cs;
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	int t;
	cin >> t;
	while (t--) {
		int n;
		cin >> n;
		vector<pi> a(n);
		for (auto &[x, y] : a)
			cin >> x >> y;
		auto f = [&](int r, int c) {
			if (r < c && c < r + n) {
				r %= n;
				c %= n;
				lint dx = a[r][0] - a[c][0];
				lint dy = a[r][1] - a[c][1];
				return dx * dx + dy * dy;
			} else
				return -1ll;
		};
		auto g = smawk(f, n, 2 * n);
		for (auto &x : g)
			cout << x % n << " ";
		cout << "\n";
	}
}