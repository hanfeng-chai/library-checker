//这回只花了114514min就打完了。
//真好。记得多手造几组。ACM拍什么拍。 
#include "bits/stdc++.h"
using namespace std;
using ui = unsigned; using db = long double; using ull = unsigned long long; using lll = __int128;
template<class T1, class T2> istream &operator>>(istream &cin, pair<T1, T2> &a) { return cin >> a.first >> a.second; }
template <std::size_t Index = 0, typename... Ts> typename std::enable_if<Index == sizeof...(Ts), void>::type tuple_read(std::istream &is, std::tuple<Ts...> &t) { }
template <std::size_t Index = 0, typename... Ts> typename std::enable_if < Index < sizeof...(Ts), void>::type tuple_read(std::istream &is, std::tuple<Ts...> &t) { is >> std::get<Index>(t); tuple_read<Index + 1>(is, t); }
template <typename... Ts>std::istream &operator>>(std::istream &is, std::tuple<Ts...> &t) { tuple_read(is, t); return is; }
template<class T1> istream &operator>>(istream &cin, valarray<T1> &a);
template<class T1> istream &operator>>(istream &cin, vector<T1> &a) { for (auto &x : a) cin >> x; return cin; }
template<class T1> istream &operator>>(istream &cin, valarray<T1> &a) { for (auto &x : a) cin >> x; return cin; }
template<class T1, class T2> bool cmin(T1 &x, const T2 &y) { if (y < x) { x = y; return 1; } return 0; }
template<class T1, class T2> bool cmax(T1 &x, const T2 &y) { if (x < y) { x = y; return 1; } return 0; }
template<class T1> vector<T1> range(T1 l, T1 r, T1 step = 1) { assert(step > 0); int n = (r - l + step - 1) / step, i; vector<T1> res(n); for (i = 0; i < n; i++) res[i] = l + step * i; return res; }
template<class T1> basic_string<T1> operator*(const basic_string<T1> &s, int m) { auto r = s; m *= s.size(); r.resize(m); for (int i = s.size(); i < m; i++) r[i] = r[i - s.size()]; return r; }
istream &operator>>(istream &cin, lll &x) { bool flg = 0; x = 0; static string s; cin >> s; if (s[0] == '-') flg = 1, s = s.substr(1); for (char c : s) x = x * 10 + (c - '0'); if (flg) x = -x; return cin; }
ostream &operator<<(ostream &cout, lll x) { static char s[60]; if (x < 0) cout << '-', x = -x; int tp = 1; s[0] = '0' + (x % 10); while (x /= 10) s[tp++] = '0' + (x % 10); while (tp--) cout << s[tp]; return cout; }
#if !defined(ONLINE_JUDGE)&&defined(LOCAL)
#include "my_header/debug.h"
#else
#define dbg(...) ;
#endif
template<class T1, class T2> ostream &operator<<(ostream &cout, const pair<T1, T2> &a) { return cout << a.first << ' ' << a.second; }
template<class T1, class T2> ostream &operator<<(ostream &cout, const vector<pair<T1, T2>> &a) { for (auto &x : a) cout << x << '\n'; return cout; }
template<class T1> ostream &operator<<(ostream &cout, vector<T1> a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << ' ' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const valarray<T1> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << ' ' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const vector<valarray<T1>> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << '\n' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const vector<vector<T1>> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << '\n' << a[i]; return cout; }
#define all(x) (x).begin(),(x).end()
#define print(...) cout<<format(__VA_ARGS__)
#define println(...) cout<<format(__VA_ARGS__)<<'\n'
#define err(...) cerr<<format(__VA_ARGS__)
#define errln(...) cerr<<format(__VA_ARGS__)<<'\n'
typedef unsigned long long ll;
const ll p = 998244353;
ll ksm(ll x, ll y)
{
	ll r = 1;
	while (y)
	{
		if (y & 1) r = r * x % p;
		x = x * x % p; y >>= 1;
	}
	return r;
}
struct matrix;
matrix E(int n);
struct matrix :vector<vector<ll>>
{
	explicit matrix(int n = 0, int m = 0) :vector(n, vector<ll>(m)) { }
	pair<int, int> sz() const { if (size()) return {size(), back().size()}; return {0, 0}; }
	matrix &operator+=(const matrix &b)
	{
		assert(sz() == b.sz());
		auto [n, m] = sz();
		for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) ((*this)[i][j] += b[i][j]) %= p;
		return *this;
	}
	matrix &operator-=(const matrix &b)
	{
		assert(sz() == b.sz());
		auto [n, m] = sz();
		for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) ((*this)[i][j] += p - b[i][j]) %= p;
		return *this;
	}
	matrix operator*(const matrix &b) const
	{
		auto [n, m] = sz();
		auto [_, q] = b.sz();
		assert(m == _);
		int i, j, k;
		matrix c(n, q);
		for (k = 0; k < m; k++)
		{
			for (i = 0; i < n; i++) for (j = 0; j < q; j++) c[i][j] += (*this)[i][k] * b[k][j];
			if (!((k ^ q - 1) & 15)) for (auto &v : c) for (ll &x : v) x %= p;
		}
		static_assert(-1llu / p / p > 17);
		return c;
	}
	matrix operator+(const matrix &b) const { auto a = *this; return a += b; }
	matrix operator-(const matrix &b) const { auto a = *this; return a -= b; }
	matrix &operator*=(const matrix &b) { return *this = *this * b; }
	matrix &operator*=(ll k) { for (auto &v : *this) for (ll &x : v) x = x * k % p; return *this; }
	matrix operator*(ll k) const { auto a = *this; return a *= k; }
	matrix transpose() const
	{
		auto [n, m] = sz();
		matrix res(m, n);
		for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) res[j][i] = (*this)[i][j];
		return res;
	}
	int rank() const
	{
		auto [n, m] = sz();
		vector<vector<ll>> a = n <= m ? *this : transpose();
		if (n > m) ::swap(n, m);
		int i, j, k, l, r = 0;
		for (i = 0, j = 0; i < n && j < m; j++)
		{
			for (k = i; k < n; k++) if (a[k][j]) break;
			if (k == n) continue;
			::swap(a[i], a[k]);
			ll iv = ksm(a[i][j], p - 2);
			for (k = j; k < m; k++) a[i][k] = a[i][k] * iv % p;
			for (k = i + 1; k < n; k++) for (l = j + 1; l < m; l++) a[k][l] = (a[k][l] + (p - a[k][j]) * a[i][l]) % p;
			++i; ++r;
		}
		return r;
	}
	vector<ll> poly() const
	{
		auto [n, m] = sz();
		vector<vector<ll>> a = *this;
		assert(n == m);
		int i, j, k;
		for (i = 1; i < n; i++)
		{
			for (j = i; j < n && !a[j][i - 1]; j++);
			if (j == n) continue;
			if (j > i)
			{
				::swap(a[i], a[j]);
				for (k = 0; k < n; k++) ::swap(a[k][j], a[k][i]);
			}
			ll r = a[i][i - 1];
			for (j = 0; j < n; j++) a[j][i] = a[j][i] * r % p;
			r = ksm(r, p - 2);
			for (j = i - 1; j < n; j++) a[i][j] = a[i][j] * r % p;
			for (j = i + 1; j < n; j++)
			{
				r = a[j][i - 1];
				for (k = 0; k < n; k++) a[k][i] = (a[k][i] + a[k][j] * r) % p;
				r = p - r;
				for (k = i - 1; k < n; k++) a[j][k] = (a[j][k] + a[i][k] * r) % p;
			}
		}
		vector g(n + 1, vector<ll>(n + 1));
		g[0][0] = 1;
		for (i = 0; i < n; i++)
		{
			ll r = p - 1, rr;
			for (j = i; j >= 0; j--)//第 j 行选第 n 列
			{
				rr = r * a[j][i] % p;
				for (k = 0; k <= j; k++) g[i + 1][k] = (g[i + 1][k] + rr * g[j][k]) % p;
				if (j) r = r * a[j][j - 1] % p;
			}
			for (k = 1; k <= i + 1; k++) (g[i + 1][k] += g[i][k - 1]) %= p;
		}
		auto f = g[n];
		//if (n&1) for (i=0;i<=n;i++) if (f[i]) f[i]=p-f[i];//若注释掉则为 |kE-A|
		return f;
	}
	ll det() const
	{
		auto [n, m] = sz();
		vector<vector<ll>> a = *this;
		assert(n == m);
		int i, j, k;
		ll r = 1;
		for (i = 0; i < n; i++)
		{
			for (j = i; j < n; j++) if (a[j][i]) break;
			if (j == n) return 0;
			if (i != j) r = p - r, ::swap(a[i], a[j]);
			(r *= a[i][i]) %= p;
			ll iv = ksm(a[i][i], p - 2);
			for (j = i; j < n; j++) a[i][j] = a[i][j] * iv % p;
			for (j = i + 1; j < n; j++) for (k = i + 1; k < n; k++) a[j][k] = (a[j][k] + (p - a[i][k]) * a[j][i]) % p;
		}
		return r % p;
	}
	tuple<int, vector<ll>, vector<vector<ll>>> gauss(const vector<ll> &b) const//Ax=b, rank of base, one sol, base
	{
		auto [n, m] = sz();
		if (b.size() != n) return {-1, { }, { }};
		vector<vector<ll>> a = *this;
		int i, j, k, R = m;
		for (i = 0; i < n; i++) a[i].push_back(b[i]);
		vector<int> fix(m, -1);
		for (i = k = 0; i < m; i++)
		{
			for (j = k; j < n; j++) if (a[j][i]) break;
			if (j == n) continue;
			fix[i] = k; --R;
			::swap(a[k], a[j]);
			auto &u = a[k];
			ll x = ksm(u[i], p - 2);
			for (j = i; j <= m; j++) u[j] = u[j] * x % p;
			for (auto &v : a) if (v.data() != u.data())
			{
				x = p - v[i];
				for (j = i; j <= m; j++) v[j] = (v[j] + x * u[j]) % p;
			}
			++k;
		}
		for (i = k; i < n; i++) if (a[i][m]) return {-1, { }, { }};
		vector<ll> r(m);
		vector<vector<ll>> c;
		for (i = 0; i < m; i++) if (fix[i] != -1) r[i] = a[fix[i]][m];
		for (i = 0; i < m; i++) if (fix[i] == -1)
		{
			vector<ll> r(m);
			r[i] = 1;
			for (j = 0; j < m; j++) if (fix[j] != -1) r[j] = (p - a[fix[j]][i]) % p;
			c.push_back(r);
		}
		return {R, r, c};
	}
	optional<matrix> inverse() const
	{
		auto [n, m] = sz();
		assert(n == m);
		vector<int> ih(n, -1), jh(n, -1);
		matrix a = *this;
		int i, j, k;
		for (k = 0; k < n; k++)
		{
			for (i = k; i < n; i++) if (ih[k] == -1) for (j = k; j < n; j++) if (a[i][j])
			{
				ih[k] = i;
				jh[k] = j;
				break;
			}
			if (ih[k] == -1) return { };
			::swap(a[k], a[ih[k]]);
			for (i = 0; i < n; i++) ::swap(a[i][k], a[i][jh[k]]);
			if (!a[k][k]) return { };
			a[k][k] = ksm(a[k][k], p - 2);
			for (i = 0; i < n; i++) if (i != k) (a[k][i] *= a[k][k]) %= p;
			for (i = 0; i < n; i++) if (i != k) for (j = 0; j < n; j++) if (j != k)
				(a[i][j] += (p - a[i][k]) * a[k][j]) %= p;
			for (i = 0; i < n; i++) if (i != k) (a[i][k] *= p - a[k][k]) %= p;
		}
		for (k = n - 1; k >= 0; k--)
		{
			::swap(a[k], a[jh[k]]);
			for (i = 0; i < n; i++) ::swap(a[i][k], a[i][ih[k]]);
		}
		return a;
	}
	matrix adjugate() const
	{
		auto [n, m] = sz();
		assert(n == m);
		int R = rank();
		if (n == 1) return E(1);
		if (R == n) return *inverse() * det();
		if (R == n - 1)
		{
			int i, j, k, l;
			auto [_, x, dx] = gauss(vector<ll>(n));
			auto [__, y, dy] = transpose().gauss(vector<ll>(n));
			if (count(all(x), 0) == n) x = dx[0];
			if (count(all(y), 0) == n) y = dy[0];
			for (k = 0; k < n; k++) if (x[k]) break;
			for (l = 0; l < n; l++) if (y[l]) break;
			assert(k < n && l < n);
			matrix res(n, n), c(n - 1, n - 1);
			for (i = 0; i < n; i++) if (i != l) for (j = 0; j < n; j++) if (j != k) c[i - (i > l)][j - (j > k)] = (*this)[i][j];
			for (i = 0; i < n; i++) for (j = 0; j < n; j++) res[i][j] = x[i] * y[j] % p;
			ll t = c.det() * ksm((k + l & 1) ? p - res[k][l] : res[k][l], p - 2) % p;
			assert(res[k][l]);
			assert(c.det());
			assert(t);
			return res * t;
		}
		return matrix(n, n);
	}
};
istream &operator>>(istream &cin, matrix &r) { for (auto &v : r) for (ll &x : v) cin >> x; return cin; }
ostream &operator<<(ostream &cout, const matrix &r) { auto [n, m] = r.sz(); for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) cout << r[i][j] << " \n"[j + 1 == m]; return cout; }
matrix E(int n) { matrix r(n, n); for (int i = 0; i < n; i++) r[i][i] = 1; return r; }
matrix pow(matrix a, long long k)
{
	assert(k >= 0);
	auto [n, m] = a.sz();
	assert(n == m);
	matrix r = k & 1 ? a : E(n);
	k >>= 1;
	while (k)
	{
		a *= a;
		if (k & 1) r *= a;
		k >>= 1;
	}
	return r;
}
matrix pow2(matrix a, long long k)
{
	vector<ll> f = a.poly();
	int n = f.size() - 1, i, j;
	if (!n) return matrix();
	if (n == 1) return E(1) * ksm(a[0][0], k);
	assert(f[n] == 1);
	vector<ll> r(n), x(n), t(n * 2);
	r[0] = x[1] = 1;
	for (ll &x : f) x = (p - x) % p;
	reverse(all(f));
	fill(all(t), 0);
	if (k & 1)
	{
		for (i = 0; i < n; i++) for (j = 0; j < n; j++) t[i + j] = (t[i + j] + r[i] * x[j]) % p;
		for (i = n * 2 - 2; i >= n; i--) for (j = 1; j <= n; j++) t[i - j] = (t[i - j] + f[j] * t[i]) % p;
		for (i = 0; i < n; i++) r[i] = t[i];
	}
	k >>= 1;
	while (k)
	{
		fill(all(t), 0);
		for (i = 0; i < n; i++) for (j = 0; j < n; j++) t[i + j] = (t[i + j] + x[i] * x[j]) % p;
		for (i = n * 2 - 2; i >= n; i--) for (j = 1; j <= n; j++) t[i - j] = (t[i - j] + f[j] * t[i]) % p;
		for (i = 0; i < n; i++) x[i] = t[i];
		if (k & 1)
		{
			fill(all(t), 0);
			for (i = 0; i < n; i++) for (j = 0; j < n; j++) t[i + j] = (t[i + j] + r[i] * x[j]) % p;
			for (i = n * 2 - 2; i >= n; i--) for (j = 1; j <= n; j++) t[i - j] = (t[i - j] + f[j] * t[i]) % p;
			for (i = 0; i < n; i++) r[i] = t[i];
		}
		k >>= 1;
	}
	matrix res(n, n);
	int b = ceil(sqrt(n));
	vector<matrix> s(b + 1);
	s[0] = E(n); s[1] = a;
	for (i = 2; i <= b; i++) s[i] = s[i - 1] * a;
	for (i = b - 1; i >= 0; i--)
	{
		res *= s[b];
		for (j = min(n, (i + 1) * b) - 1; j >= i * b; j--) res += s[j - i * b] * r[j];
	}
	return res;
}
int main()
{
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(15);
	int n, i;
	cin >> n;
	matrix a(n, n);
	cin >> a;
	cout << a.adjugate();
}
