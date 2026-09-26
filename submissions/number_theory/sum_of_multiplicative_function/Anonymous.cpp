// time O(n^{2/3} / log^{4/3} n)
// author snowy_night
#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <algorithm>
using namespace std;
using u32 = unsigned int;
using u64 = unsigned long long;
using i64 = long long;
constexpr u32 mod = 469762049;
inline constexpr u32 norm(const u32 x) { return x < mod ? x : x - mod; }
struct m32 {
	u32 x;
	m32() { }
	constexpr m32(const u32 _x) : x(_x) { }
};
inline constexpr m32 operator + (const m32 x1, const m32 x2) { return norm(x1.x + x2.x); }
inline constexpr m32 operator - (const m32 x1, const m32 x2) { return norm(x1.x + mod - x2.x); }
inline constexpr m32 operator - (const m32 x) { return x.x ? mod - x.x : 0; }
inline constexpr m32 operator * (const m32 x1, const m32 x2) { return static_cast<u64>(x1.x) * x2.x % mod; }
inline m32& operator += (m32& x1, const m32 x2) { return x1 = x1 + x2; }
inline m32& operator -= (m32& x1, const m32 x2) { return x1 = x1 - x2; }
inline m32& operator *= (m32& x1, const m32 x2) { return x1 = x1 * x2; }
inline bool operator == (const m32 x1, const m32 x2) { return x1.x == x2.x; }
inline bool operator != (const m32 x1, const m32 x2) { return x1.x != x2.x; }

struct block {
	static int v;
	vector<m32> sv;
	vector<m32> lv;
	block() : sv(v + 1, 0), lv(v + 1, 0) {}
};
int block::v;

inline void add(m32& x, const u32 a, const u32 b) { x = (x.x + 1ULL * a * b) % mod; }
inline void add(m32& x, const m32 a, const m32 b) { add(x, a.x, b.x); }
inline void sub(m32& x, const m32 a, const m32 b) { add(x, a.x, mod - b.x); }

int solve(const i64 N, const m32 A, const m32 B) {
	const int v = sqrt(N + 0.5);
	const int n_4 = sqrt(v + 0.5);
	const int n_8 = sqrt(n_4 + 0.5);
	block::v = v;

	vector<int> primes;
	vector<int> pi(v + 1);
	vector<bool> is_prime(v + 1);
	primes.push_back(1);
	is_prime[2] = true;
	for (int i = 3; i <= v; i += 2) is_prime[i] = true;
	for (int i = 3; i * i <= v; i += 2)
		for (int j = i * i; is_prime[i] && j <= v; j += (i << 1))
			is_prime[j] = false;
	for (int i = 2; i <= v; ++i) {
		pi[i] = pi[i - 1] + is_prime[i];
		if (is_prime[i]) primes.push_back(i);
	}

	vector<m32> sup;
	sup.resize(primes.size());
	u32 rec[4];
	rec[1] = 1;
	for (int i = 2; i <= 3; ++i)
		rec[i] = (i64)(mod - mod / i) * rec[mod % i] % mod;
	m32 inv3 = m32(rec[3]);

	const auto divide = [](i64 n, i64 d) -> i64 {return double(n) / d; };

	auto calc_medium = [&](const function<m32(u64)>& fp) {
		sup.clear();
		sup[0] = m32(0);
		for (int i = 1; i <= pi[v]; ++i) sup[i] = fp(primes[i]);

		vector<m32> lq(v + 1, 0);
		for (int i = 1; i <= pi[v]; ++i) lq[primes[i]] += sup[i];
		for (int i = 1; i <= v; ++i) lq[i] += lq[i - 1];

		block f;
		i64 r = pi[v];
		for (int i = pi[n_4] + 1; i <= pi[v]; ++i) {
			const i64 m = divide(N, primes[i]);
			if (i * primes[i] > m) break;
			while (r * primes[r] > m) r--;
			for (int j = i; j <= r; ++j) f.lv[divide(m, primes[j])] += sup[i] * sup[j];
		}
		for (int i = v - 1; i; --i) f.lv[i] += f.lv[i + 1];
		r = pi[v];
		for (int i = pi[n_4] + 1; i <= pi[v]; ++i) {
			const i64 m = divide(N, primes[i]);
			while (r * primes[r] > m) r--;
			const int j = max(primes[r], primes[i] - 1), t1 = divide(m, j), t0 = divide(v, primes[i]);
			for (int k = 1; k <= t0; ++k)
				add(f.lv[k], sup[i], lq[v] - lq[j]);
			for (int k = t0 + 1; k <= t1; ++k)
				add(f.lv[k], sup[i], lq[divide(m, k)] - lq[j]);
		}
		for (int k = 1; k <= n_4; ++k) {
			int t = v / k;
			i64 m = N / k;
			m32 ans = m32(0);
			for (int i = pi[n_4] + 1; i <= pi[t]; ++i) ans += sup[i] * f.lv[primes[i] * k];
			for (int i = pi[n_4] + 1; i <= pi[t]; ++i) {
				i64 q = (i64)primes[i] * primes[i];
				if (q * n_4 > m) break;
				ans += sup[i] * sup[i] * (lq[divide(m, q)] - lq[n_4]);
			}
			t = cbrt(m + 0.5);
			for (int i = pi[n_4] + 1; i <= pi[t]; ++i) ans += sup[i] * sup[i] * sup[i];
			f.lv[k] += ans * inv3;
		}
		for (int i = 1; i <= v; ++i) f.lv[i] += lq[v] - lq[n_4] + 1;
		for (int i = 1; i <= n_4; ++i) f.sv[i] = 1;
		for (int i = n_4 + 1; i <= v; ++i) f.sv[i] = lq[i] - lq[n_4] + 1;
		for (int i = 0; i <= v; ++i) lq[i] = 0;


		for (int i = v; i; --i) f.sv[i] -= f.sv[i - (i & -i)];
		int mm = v * max(sqrt(log(N)) * 0.5, 1.);
		int K = min(N, (i64)(mm * pow(N, 0.125))), B = N / K;
		m32 sum_s = 0;
		const auto add_s = [&](int x, m32 cnt) -> void {
			sum_s += cnt;
			while (x <= v) f.sv[x] += cnt, x += x & -x;
			};
		const auto add_l = [&](int x, m32 cnt) -> void {
			while (x) lq[x] += cnt, x ^= x & -x;
			};
		function <void(int, int, m32)> dfs = [&](int n, int id, m32 fn) {
			if (n <= v) add_s(n, fn);
			else add_l(divide(N, n), fn);
			for (int i = id; i <= pi[v]; ++i) {
				i64 q = (i64)n * primes[i];
				if (q > K) break;
				dfs(q, i, fn * sup[i]);
			}
			};
		auto query_s = [&](int x) -> m32 {
			m32 ans = 0;
			while (x) ans += f.sv[x], x ^= x & -x;
			return ans;
			};
		auto query_l = [&](int x) -> m32 {
			m32 ans = sum_s;
			while (x <= v) ans += lq[x], x += x & -x;
			return ans;
			};
		int K2, B2;
		for (int id = pi[n_4]; id > pi[n_8]; --id) {
			const int p = primes[id];
			const u64 m = N / p;
			dfs(p, id, sup[id]);
			const int t0 = B / p, t1 = min(B, v / p);
			for (int i = B; i > t1; --i) add(f.lv[i], sup[id], query_s(divide(m, i)));
			for (int i = t1; i > t0; --i) add(f.lv[i], sup[id], f.lv[i * p] + query_l(i * p));
			for (int i = t0; i; --i) add(f.lv[i], sup[id], f.lv[i * p]);
			K2 = mm * sqrt(p);
			B2 = N / K2;
			for (int i = B2; i > B; --i) f.lv[i] += query_l(i);
			K = K2, B = B2;
		}
		for (int i = v; i > B; --i)
			if(i + (i & -i) <= v) lq[i] += lq[i + (i & -i)];
		for (int i = B + 1; i <= v; ++i) f.lv[i] += sum_s + lq[i];
		for (int i = 1; i <= v; ++i)
			if (i & (i - 1)) f.sv[i] += f.sv[i & (i - 1)];

		return f;
		};

	auto attach_small = [&](block&& f, const function<m32(u64)>& fp) {
		for (int id = pi[n_8]; id; --id) {
			const int p = primes[id], t = v / p;
			const i64 m = N / p;
			for (int j = 1, i = p; j <= t; ++j) {
				const m32 c1 = sup[id] * f.sv[j];
				for (int e = min(v + 1, i + p); i < e; ++i) f.sv[i] += c1;
			}
			for (int i = v; i > t; --i) add(f.lv[i], sup[id], f.sv[divide(m, i)]);
			for (int i = t; i >= 1; --i) add(f.lv[i], sup[id], f.lv[i * p]);
		}
		return move(f);
		};

	auto calc_large = [&](const function<m32(u64)>& fp, const function<m32(u64)>& sum_fp) {
		block f = attach_small(calc_medium(fp), fp);
		block res;
		for (int i = v; i >= 1; --i) {
			m32 ans = sum_fp(N / i) - f.lv[i];
			for (int j = 2; i * j <= v; ++j) sub(ans, fp(j), res.lv[i * j]);
			res.lv[i] = ans;
		}
		return res;
		};

	auto mult_large = [&](block&& f, const block& l) {
		for (int i = 1; i <= v; ++i) {
			for (int j = 1; i * j <= v; ++j)
				if (f.sv[j] != f.sv[j - 1])
					add(f.lv[i], f.sv[j] - f.sv[j - 1], l.lv[i * j]);
		}
		return move(f);
		};

	auto mult_powerful = [&](block&& f, const function<m32(u32, u32)>& fpp) {
		block h;
		function< void(u64, int, m32)> dfs = [&](u64 n, int beg, m32 coeff) -> void {
			if (n <= v) h.sv[n] += coeff;
			else h.lv[divide(N, n)] += coeff;
			u64 t = divide(N, n);
			for (int i = beg; i <= pi[v]; ++i) {
				const int p = primes[i];
				u64 q = 1ULL * p * p;
				if (q > t) break;
				for (int e = 2; q <= t; q *= p, ++e)
					dfs(n * q, i + 1, coeff * (fpp(p, e) - fpp(p, 1) * fpp(p, e - 1)));
			}
			};
		dfs(1, 1, 1);

		block res;
		for (int i = 1; i <= v; ++i)
			if (h.sv[i].x) {
				const i64 m = divide(N, i);
				const int t0 = sqrt(m + 0.5);
				for (int k = 1; k * i <= v; ++k)
					add(res.lv[k], h.sv[i], f.sv[v] - f.sv[t0]);
				for (int k = v / i + 1; k <= t0; ++k)
					add(res.lv[k], h.sv[i], f.sv[divide(m, k)] - f.sv[t0]);
			}
		for (int i = 1; i < v; ++i) res.lv[i] -= res.lv[i + 1];

		m32 sum_s = f.sv[v];
		f.sv[v] -= f.sv[v - 1];
		for (int i = v - 1; i; --i) h.lv[i] += h.lv[i + 1], f.sv[i] -= f.sv[i - 1];
		for (int j = 1; j <= v; ++j) {
			const int t0 = v / j;
			for (int t = 1; t < t0; ++t)
				add(res.lv[t], f.sv[j], h.lv[j * t] - h.lv[j * (t + 1)]);
			add(res.lv[t0], f.sv[j], h.lv[j * t0]);
		}
		for (int i = 1; i <= v; ++i)
			if (h.sv[i].x) {
				const i64 m = divide(N, i);
				const int t = sqrt(m + 0.5), t0 = v / i;

				for (int t = 1; t < t0; ++t) add(res.lv[t], h.sv[i], f.lv[t * i] - f.lv[i * (t + 1)]);
				add(res.lv[t0], h.sv[i], f.lv[t0 * i] - sum_s);
				for (int j = t0 + 1; j <= t; ++j) add(res.lv[divide(m, j)], h.sv[i], f.sv[j]);

				for (int j = 1; j <= t0; ++j) add(res.sv[i * j], h.sv[i], f.sv[j]);
			}

		for (int i = 1; i <= v; ++i) res.sv[i] += res.sv[i - 1];
		res.lv[v] += res.sv[v];
		for (int i = v - 1; i; --i) res.lv[i] += res.lv[i + 1];
		return res;
		};
	block l0 = calc_large([&](u64 n) { return 1; }, [&](u64 n) { return m32(n % mod); });
	block l1 = calc_large([&](u64 n) { return m32(n % mod); }, [&](u64 n) { return n %= mod, m32(n * (n + 1) / 2 % mod); });
	for (int i = 1; i <= v; ++i) l1.lv[i] = A * l0.lv[i] + B * l1.lv[i], l1.sv[i] = 0;

	auto fp = [&](u32 p) { return A + B * p; };
	auto fpp = [&](u32 p, u32 e) { return A * e + B * p; };
	block f = attach_small(mult_large(calc_medium(fp), l1), fp);
	function< m32(u64, int, m32)> dfs = [&](u64 n, int beg, m32 coeff) -> m32 {
		m32 ret = coeff * (n > v ? f.lv[divide(N, n)] : f.sv[n]);
		for (int i = beg; i <= pi[v]; ++i) {
			const int p = primes[i];
			i64 q = i64(p) * p;
			if (q > n) break;
			i64 nn = divide(n, q);
			for (int e = 2; nn; nn = divide(nn, p), ++e)
				ret += dfs(nn, i + 1, coeff * (fpp(p, e) - fpp(p, 1) * fpp(p, e - 1)));
		}
		return ret;
		};
	return dfs(N, 1, 1).x;
}
signed main() {
	i64 T, n;
	m32 A, B;
	cin >> T;
	while (T--) {
		cin >> n >> A.x >> B.x;
		cout << solve(n, A, B) << '\n';
	}
	return 0;
}