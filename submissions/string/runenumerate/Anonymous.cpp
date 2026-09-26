#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;

template <class Seq>
struct LCE64 {
	static inline uint64_t B = random_device{}() | 1;

	int n;
	vector<uint64_t> pw, h;

	LCE64(const Seq& a): n(a.size()), pw(n + 1), h(n + 1) {
		pw[0] = 1;
		for (int i = 0; i < n; ++i) pw[i + 1] = pw[i] * B;
		for (int i = 0; i < n; ++i) h[i + 1] = h[i] * B + a[i];
	}

	inline uint64_t range(int l, int r) const { // [l, r)
		return h[r] - h[l] * pw[r - l];
	}

	int lcp(int i, int j, int limit = INT_MAX) const {
		if (i == j) return min(limit, n - i);
		int lo = 0, hi = min({limit, n - i, n - j});
		if (hi >= 16) {
			int mid = 5;
			if (h[i + mid] - h[j + mid] == (h[i] - h[j]) * pw[mid]) lo = mid;
			else hi = mid - 1;
		}
		while (lo < hi) {
			int mid = lo + hi + 1 >> 1;
			if (h[i + mid] - h[j + mid] == (h[i] - h[j]) * pw[mid]) lo = mid;
			else hi = mid - 1;
		}
		return lo;
	}

	int lcs(int i, int j, int limit) const {
		if (i == j) return min(limit, i + 1);
		int lo = 0, hi = min({limit, i + 1, j + 1});
		if (hi >= 16) {
			int mid = 5;
			if (h[i + 1] - h[j + 1] == (h[i + 1 - mid] - h[j + 1 - mid]) * pw[mid]) lo = mid;
			else hi = mid - 1;
		}
		while (lo < hi) {
			int mid = lo + hi + 1 >> 1;
			if (h[i + 1] - h[j + 1] == (h[i + 1 - mid] - h[j + 1 - mid]) * pw[mid]) lo = mid;
			else hi = mid - 1;
		}
		return lo;
	}
};

template <class Seq>
vector<array<int, 3>> calculate_runs(const Seq& a) {
	const int n = (int)a.size();
	vector<array<int, 3>> runs;
	if (n <= 1) return runs;

	LCE64<Seq> lce(a);
	vector<int> st(n + 1);

	for (int inv = 0; inv < 2; ++inv) {
		int top = 0;
		st[top] = n;

		for (int i = n - 1; i >= 0; --i) {
			int l = 0;
			while (top >= 1) {
				int j = st[top];
				int k = st[top - 1];
				int bound = min(j - i, k - j);
				l = lce.lcp(i, j, bound);
				if (l == bound ? j - i >= k - j : (a[i + l] >= a[j + l]) ^ inv) break;
				--top;
			}

			int j = st[top];
			int p = j - i;
			st[++top] = i;

			int x = i > 0 && j > 0 ? lce.lcs(i - 1, j - 1, p) : 0;
			if (x < p) {
				int y = l + lce.lcp(i + l, j + l);
				if (x + y >= p) runs.push_back({p, i - x, j + y - 1});
			}
		}
	}

	sort(runs.begin(), runs.end());
	runs.erase(unique(runs.begin(), runs.end()), runs.end());
	return runs;
}

int main() {
	ios::sync_with_stdio(0);cin.tie(0);

	string s;
	cin >> s;
	auto runs = calculate_runs(s);
	cout << runs.size() << '\n';
	for (auto &t : runs) {
		cout << t[0] << ' ' << t[1] << ' ' << t[2] + 1 << '\n';
	}
}