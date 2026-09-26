#include <bits/stdc++.h>

using u32 = unsigned;
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128;
using u128 = unsigned __int128;

template<class U0, class U1, class S0, U0 P>
struct static_modint {
private:
	static_assert((P >> (sizeof(U0) * 8 - 1)) == 0, "'Mod' must less than max(U0)/2");
	static constexpr U0 Mod = P;
	U0 x;
	template<class T>
	static constexpr U0 safeMod(T x) {
		if constexpr (std::is_unsigned<T>::value) {
			return static_cast<U0>(x % Mod);
		} else {
			((x %= static_cast<S0>(Mod)) < 0) && (x += Mod);
			return static_cast<U0>(x);
		}
	}
public:
	constexpr static_modint(): x(static_cast<U0>(0)) {}
	template<class T>
	constexpr static_modint(T _x): x(safeMod(_x)) {}
	static constexpr static_modint from_raw(U0 _x) noexcept {
		static_modint x;
		return x.x = _x, x;
	}
	static constexpr U0 getMod() {
		return Mod;
	}
	template<class T>
	explicit constexpr operator T() const {
		return static_cast<T>(x);
	}
	constexpr static_modint &operator += (const static_modint &rhs) {
		x += rhs.x, (x - Mod) >> (sizeof(U0) * 8 - 1) || (x -= Mod);
		return *this;
	}
	constexpr static_modint &operator -= (const static_modint &rhs) {
		x -= rhs.x, x >> (sizeof(U0) * 8 - 1) && (x += Mod);
		return *this;
	}
	constexpr static_modint &operator *= (const static_modint &rhs) {
		x = static_cast<U0>(static_cast<U1>(x) * static_cast<U1>(rhs.x) % static_cast<U1>(Mod));
		return *this;
	}
	constexpr static_modint &operator /= (const static_modint &rhs) {
		return (*this *= rhs.inv());
	}
	friend constexpr static_modint fma(const static_modint &a, const static_modint &b, const static_modint &c) {
		return from_raw((static_cast<U1>(a.x) * b.x + c.x) % Mod);
	}
	friend constexpr static_modint fam(const static_modint &a, const static_modint &b, const static_modint &c) {
		return from_raw((a.x + static_cast<U1>(b.x) * c.x) % Mod);
	}
	friend constexpr static_modint fms(const static_modint &a, const static_modint &b, const static_modint &c) {
		return from_raw((static_cast<U1>(a.x) * b.x + Mod - c.x) % Mod);
	}
	friend constexpr static_modint fsm(const static_modint &a, const static_modint &b, const static_modint &c) {
		return from_raw((a.x + static_cast<U1>(Mod - b.x) * c.x) % Mod);
	}
	constexpr static_modint inv() const {
		U0 a = Mod, b = x; S0 y = 0, z = 1;
		while (b) {
			const U0 q = a / b;
			const U0 c = a - q * b;
			a = b, b = c;
			const S0 w = y - static_cast<S0>(q) * z;
			y = z, z = w;
		}
		return from_raw(y < 0 ? y + Mod : y);
	}
	friend constexpr static_modint inv(const static_modint &x) {
		return x.inv();
	}
	friend constexpr static_modint operator + (const static_modint &x) {
		return x;
	}
	friend constexpr static_modint operator - (static_modint x) {
		x.x = x.x ? (Mod - x.x) : 0U;
		return x;
	}
	constexpr static_modint &operator ++ () {
		return *this += 1;
	}
	constexpr static_modint &operator -- () {
		return *this -= 1;
	}
	constexpr static_modint operator ++ (int) {
		static_modint v = *this;
		return *this += 1, v;
	}
	constexpr static_modint operator -- (int) {
		static_modint v = *this;
		return *this -= 1, v;
	}
	friend constexpr static_modint operator + (static_modint x, const static_modint &y) {
		return x += y;
	}
	friend constexpr static_modint operator - (static_modint x, const static_modint &y) {
		return x -= y;
	}
	friend constexpr static_modint operator * (static_modint x, const static_modint &y) {
		return x *= y;
	}
	friend constexpr static_modint operator / (static_modint x, const static_modint &y) {
		return x /= y;
	}
	template<class T>
	constexpr static_modint pow(T y) const {
		if (y < 0) return inv().pow(- y);
		static_modint x = *this, ans = from_raw(1U);
		for (; y; y >>= 1, x *= x) {
			if (y & 1) {
				ans *= x;
			}
		}
		return ans;
	}
	template<class T>
	friend constexpr static_modint pow(const static_modint &x, T y) {
		return x.pow(y);
	}
	std::optional<static_modint> sqrt() const {
		if (x == 0U) {
			return from_raw(0);
		}
		if (Mod == 2U) {
			return from_raw(1);
		}
		if (pow((Mod - 1) / 2) != from_raw(1)) {
			return std::nullopt;
		}
		static std::mt19937_64 rnd(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<U0> uid(1U, Mod - 1);
		static_modint x, y;
		do {
			x = from_raw(uid(rnd));
			y = x * x - *this;
		} while (y.pow((Mod - 1) / 2) == from_raw(1));
		auto mul = [](std::pair<static_modint, static_modint> &f,
			const std::pair<static_modint, static_modint> &g, const static_modint &h) {
			f = {f.first * g.first + f.second * g.second * h,
					 f.first * g.second + f.second * g.first};
		};
		std::pair<static_modint, static_modint> f{x, 1}, g{1, 0};
		auto exp = (Mod + 1) / 2;
		for (; exp; exp >>= 1, mul(f, f, y)) {
			if (exp & 1) {
				mul(g, f, y);
			}
		}
		return from_raw(std::min(g.first.x, Mod - g.first.x));
	}
	friend std::pair<bool, static_modint> sqrt(const static_modint &x) {
		return x.sqrt();
	}
	friend constexpr std::istream& operator >> (std::istream& is, static_modint &x) {
		S0 y;
		is >> y, x = y;
		return is;
	}
	friend constexpr std::ostream& operator << (std::ostream& os, const static_modint &x) {
		return os << x.x;
	}
	friend constexpr bool operator == (const static_modint &x, const static_modint &y) {
		return x.x == y.x;
	}
	friend constexpr bool operator != (const static_modint &x, const static_modint &y) {
		return x.x != y.x;
	}
	friend constexpr bool operator <= (const static_modint &x, const static_modint &y) {
		return x.x <= y.x;
	}
	friend constexpr bool operator >= (const static_modint &x, const static_modint &y) {
		return x.x >= y.x;
	}
	friend constexpr bool operator < (const static_modint &x, const static_modint &y) {
		return x.x < y.x;
	}
	friend constexpr bool operator > (const static_modint &x, const static_modint &y) {
		return x.x > y.x;
	}
};
template<u32 P>
using sm32 = static_modint<u32, u64, int, P>;
template<u64 P>
using sm64 = static_modint<u64, u128, i64, P>;
using Z = sm32<998244353U>;

template<class Z>
struct set_power_series: public std::vector<Z> {
private:
	template<class Oper> static void rec(Z* f, int n, Oper oper) {
		if (n < 1 << 7) {
			for (int i = 1; i < n; i <<= 1) {
				for (int j = 0; j < n; j += i << 1) {
					for (int k = 0; k < i; k ++) {
						oper(f[j + k], f[i + j + k]);
					}
				}
			}
		} else {
			n >>= 1;
			rec(f, n, oper);
			rec(f + n, n, oper);
			for (int i = 0; i < n; i ++) {
				oper(f[i], f[i + n]);
			}
		}
	}
	template<bool t> static void rec_ranked(Z *f, int m, int n, int pc = 0) {
 		if (m < 1 << 4) {
			for (int i = 1; i < m; i <<= 1) {
				for (int j = 0; j < m; j += i << 1) {
					for (int k = 0; k < i; k ++) {
						if constexpr (!t) {
							for (int p = 0; p <= pc + __builtin_popcount(i + j + k); p ++) {
								f[(i + j + k) * (n + 1) + p] += f[(j + k) * (n + 1) + p];
							}
						} else {
							for (int p = pc + __builtin_popcount(i + j + k); p <= n; p ++) {
								f[(i + j + k) * (n + 1) + p] -= f[(j + k) * (n + 1) + p];
							}
						}
					}
				}
			}
		} else {
			m >>= 1;
			rec_ranked<t>(f, m, n, pc);
			rec_ranked<t>(f + m * (n + 1), m, n, pc + 1);
			for (int i = 0; i < m; i ++) {
				if constexpr (!t) {
					for (int p = 0; p <= pc + __builtin_popcount(i); p ++) {
						f[(i + m) * (n + 1) + p] += f[i * (n + 1) + p];
					}
				} else {
					for (int p = pc + __builtin_popcount(i); p <= n; p ++) {
						f[(i + m) * (n + 1) + p] -= f[i * (n + 1) + p];
					}
				}
			}
		}
	}
	static void ranked_zeta(Z* f, Z *F, int n) {
		for (int s = 0; s < 1 << n; s ++) {
			F[s * (n + 1) + __builtin_popcount(s)] = f[s];
		}
		rec_ranked<0>(F, 1 << n, n);
	}
	static void ranked_mobius(Z *F, Z* f, int n) {
		rec_ranked<1>(F, 1 << n, n);
		for (int s = 0; s < 1 << n; s ++) {
			f[s] = F[s * (n + 1) + __builtin_popcount(s)];
		}
	}
	static void subset_conv_naive(Z* f, Z* g, int n, Z* h) {
		for (int s = 1 << n; s --; ) {
			h[s] = f[s] * g[0];
			for (int t = s; t; (-- t) &= s) {
				h[s] += f[s ^ t] * g[t];
			}
		}
	}
	static void subset_conv_fwt(Z* f, Z* g, int n, Z* h) {
		std::vector<Z> F((1 << n) * (n + 1));
		std::vector<Z> G((1 << n) * (n + 1));
		ranked_zeta(f, F.data(), n);
		ranked_zeta(g, G.data(), n);
		for (int s = 0; s < 1 << n; s ++) {
			const int pc = __builtin_popcount(s);
			for (int i = std::min(n, 2 * pc); i >= pc; i --) {
				Z sum = 0;
				for (int j = i - pc; j <= pc; j ++) {
					sum += F[s * (n + 1) + i - j] * G[s * (n + 1) + j];
				}
				F[s * (n + 1) + i] = sum;
			}
		}
		ranked_mobius(F.data(), h, n);
	}
	static void subset_conv_inner(Z* f, Z* g, int n, Z* h) {
		n < 11 ? subset_conv_naive(f, g, n, h) : subset_conv_fwt(f, g, n, h);
	}
	static void subset_div_naive(Z* f, Z* g, int n, Z* h) {
		for (int s = 0; s < 1 << n; s ++) {
			h[s] = f[s];
			for (int t = s; t; (-- t) &= s) {
				h[s] -= g[t] * h[s ^ t];
			}
		}
	}
	static void subset_div_fwt(Z* f, Z* g, int n, Z* h) {
		std::vector<Z> F((1 << n) * (n + 1));
		std::vector<Z> G((1 << n) * (n + 1));
		ranked_zeta(f, F.data(), n);
		ranked_zeta(g, G.data(), n);
		for (int s = 0; s < 1 << n; s ++) {
			for (int i = 0; i <= n; i ++) {
				for (int j = std::max(0, i - __builtin_popcount(s)); j < i; j ++) {
					F[s * (n + 1) + i] -= F[s * (n + 1) + j] * G[s * (n + 1) + i - j];
				}
			}
		}
		ranked_mobius(F.data(), h, n);
	}
	static void subset_div_inner(Z* f, Z* g, int n, Z* h) {
		n < 11 ? subset_div_naive(f, g, n, h) : subset_div_fwt(f, g, n, h);
	}
	static void comp_inner(Z* egf, Z* f, int n, Z* g) {
		std::vector<Z> F((n - 1) * (1 << n) + 1);
		for (int i = 0; i < n; i ++) {
			ranked_zeta(f + (1 << i), F.data() + (i - 1) * (1 << i) + 1, i);
		}
		for (int i = n; i >= 0; i --) {
			for (int j = n - i - 1; j >= 0; j --) {
				std::vector<Z> G((1 << j) * (j + 1));
				ranked_zeta(g, G.data(), j);
				for (int s = 0; s < 1 << j; s ++) {
					const int pc = __builtin_popcount(s);
					for (int p = std::min(j, 2 * pc); p >= pc; p --) {
						Z sum = 0;
						for (int q = p - pc; q <= pc; q ++) {
							sum += G[s * (j + 1) + p - q] * F[(j - 1) * (1 << j) + 1 + s * (j + 1) + q];
						}
						G[s * (j + 1) + p] = sum;
					}
				}
				ranked_mobius(G.data(), g + (1 << j), j);
			}
			g[0] = egf[i];
		}
	}
	static void pow_proj_inner(Z* f, Z* g, int n, Z* h) {
		std::vector<Z> G((n - 1) * (1 << n) + 1);
		for (int i = 0; i < n; i ++) {
			ranked_zeta(g + (1 << i), G.data() + (i - 1) * (1 << i) + 1, i);
		}
		std::vector<Z> sav(1 << std::max(0, n - 1));
		for (int i = 0; i <= n; i ++) {
			h[i] = f[(1 << n) - 1];
			f[(1 << n) - 1] = 0;
			for (int j = 0; j < n - i; j ++) {
				std::vector<Z> F((1 << j) * (j + 1));
				ranked_zeta(f + (1 << n) - (1 << (j + 1)), F.data(), j);
				for (int s = 0; s < 1 << j; s ++) {
					const int pc = __builtin_popcount(s);
					for (int p = std::min(j, 2 * pc); p >= pc; p --) {
						Z sum = 0;
						for (int q = p - pc; q <= pc; q ++) {
							sum += F[s * (j + 1) + p - q] * G[(j - 1) * (1 << j) + 1 + s * (j + 1) + q];
						}
						F[s * (j + 1) + p] = sum;
					}
				}
				ranked_mobius(F.data(), sav.data(), j);
				for (int k = 0; k < 1 << j; k ++) {
					f[(1 << n) - (1 << j) + k] += sav[k];
				}
				for (int k = 0; k < 1 << j; k ++) {
					f[(1 << n) - (1 << (j + 1)) + k] = 0;
				}
			}
		}
	}
public:
	set_power_series(): std::vector<Z>() {}
	explicit set_power_series(int n): std::vector<Z>(n) {}
	explicit set_power_series(const std::vector<Z> &a): std::vector<Z>(a) {}
	set_power_series(const std::initializer_list<Z> &a): std::vector<Z>(a) {}
	template<class _InputIterator, class = std::_RequireInputIter<_InputIterator>>
	explicit set_power_series(_InputIterator __first, _InputIterator __last): std::vector<Z>(__first, __last) {}
	template<class F = Z(*)(int)> explicit set_power_series(int n, F f): std::vector<Z>(n) {
		for (int i = 0; i < n; i ++) {
			(*this)[i] = f(i);
		}
	}

	friend void subset_zeta(set_power_series& f) {
		rec(f.data(), f.size(), [](Z& f, Z& g) { g += f; });
	}
	friend void supset_zeta(set_power_series& f) {
		rec(f.data(), f.size(), [](Z& f, Z& g) { f += g; });
	}
	friend void subset_mobius(set_power_series& f) {
		rec(f.data(), f.size(), [](Z& f, Z& g) { g -= f; });
	}
	friend void supset_mobius(set_power_series& f) {
		rec(f.data(), f.size(), [](Z& f, Z& g) { f -= g; });
	}
	friend void fwt(set_power_series& f) {
		rec(f.data(), f.size(), [](Z& f, Z& g) { std::tie(f, g) = std::pair{f + g, f - g}; });
	}
	friend void ifwt(set_power_series& f) {
		rec(f.data(), f.size(), [](Z& f, Z& g) { std::tie(f, g) = std::pair{f + g, f - g}; });
		const Z inv = Z(f.size()).inv();
		for (int i = 0; i < (int)f.size(); i ++) {
			f[i] *= inv;
		}
	}
	friend set_power_series or_conv(set_power_series f, set_power_series g) {
		subset_zeta(f);
		subset_zeta(g);
		for (int i = 0; i < (int)f.size(); i ++) {
			f[i] *= g[i];
		}
		subset_mobius(f);
		return f;
	}
	friend set_power_series and_conv(set_power_series f, set_power_series g) {
		supset_zeta(f);
		supset_zeta(g);
		for (int i = 0; i < (int)f.size(); i ++) {
			f[i] *= g[i];
		}
		supset_mobius(f);
		return f;
	}
	friend set_power_series xor_conv(set_power_series f, set_power_series g) {
		fwt(f);
		fwt(g);
		for (int i = 0; i < (int)f.size(); i ++) {
			f[i] *= g[i];
		}
		ifwt(f);
		return f;
	}
	friend set_power_series subset_conv(set_power_series f, set_power_series g) {
		subset_conv_inner(f.data(), g.data(), std::__lg(f.size()), f.data());
		return f;
	}
	friend set_power_series exp(set_power_series f) {
		assert(f[0] == 0);
		const int n = std::__lg(f.size());
		set_power_series g(1 << n);
		g[0] = 1;
		for (int i = 0; i < n; i ++) {
			subset_conv_inner(g.data(), f.data() + (1 << i), i, g.data() + (1 << i));
		}
		return g;
	}
	friend set_power_series subset_div(set_power_series f, set_power_series g) {
		assert(g[0]);
		const int n = f.size();
		Z c = g[0].inv();
		for (int i = 0; i < n; i ++) {
			g[i] *= c;
		}
		subset_div_inner(f.data(), g.data(), std::__lg(n), f.data());
		for (int i = 0; i < n; i ++) {
			f[i] *= c;
		}
		return f;
	}
	friend set_power_series inv(set_power_series f) {
		assert(f[0] != 0);
		const int n = f.size();
		Z c = f[0].inv();
		for (int i = 0; i < n; i ++) {
			f[i] *= c;
		}
		set_power_series g(n);
		g[0] = c;
		subset_div_inner(g.data(), f.data(), std::__lg(n), g.data());
		return g;
	}
	friend set_power_series log(set_power_series f) {
		assert(f[0] == 1);
		const int n = std::__lg(f.size());
		set_power_series g(1 << n);
		for (int i = n - 1; i >= 0; i --) {
			subset_div_inner(f.data() + (1 << i), f.data(), i, g.data() + (1 << i));
		}
		return g;
	}
	friend set_power_series egf_comp(std::vector<Z> egf, set_power_series f) {
		assert(f[0] == 0);
		egf.resize(f.size() + 1);
		set_power_series g(f.size());
		comp_inner(egf.data(), f.data(), std::__lg(f.size()), g.data());
		return g;
	}
	friend set_power_series poly_comp(std::vector<Z> poly, set_power_series f) {
		const int n = std::__lg(f.size());
		const int m = poly.size();
		if (__builtin_expect(!m, 0)) {
			return set_power_series(1 << n);
		}
		std::vector<Z> egf(n + 1);
		for (int i = 0; i <= n; i ++) {
			Z x = 0;
			for (int j = m - 1; j >= 0; j --) {
				(x *= f[0]) += poly[j];
			}
			egf[i] = x;
			for (int j = 1; j < m; j ++) {
				poly[j - 1] = poly[j] * j;
			}
			poly[m - 1] = 0;
		}
		f[0] = 0;
		set_power_series g(1 << n);
		comp_inner(egf.data(), f.data(), n, g.data());
		return g;
	}
	friend std::vector<Z> egf_pow_proj(set_power_series f, set_power_series g) {
		const int n = std::__lg(f.size());
		assert(g[0] == 0);
		std::vector<Z> ans(n + 1);
		pow_proj_inner(f.data(), g.data(), n, ans.data());
		return ans;
	}
	friend std::vector<Z> poly_pow_proj(set_power_series f, set_power_series g, int m) {
		const int n = std::__lg(f.size());
		std::vector<Z> res(n + 1);
		const Z g0 = g[0];
		g[0] = 0;
		pow_proj_inner(f.data(), g.data(), n, res.data());
		std::vector<Z> ans(m), h(n + 1);
		h[0] = 1;
		for (int i = 0; i < m; i ++) {
			for (int j = 0; j <= n; j ++) {
				ans[i] += h[j] * res[j];
			}
			for (int j = n - 1; j >= 0; j --) {
				h[j + 1] = h[j] * (i + 1);
			}
			h[0] *= g0;
		}
		return ans;
	}
};
using sps = set_power_series<Z>;

int main() {
#ifdef LOCAL
	freopen("!in.in", "r", stdin);
	freopen("!out.out", "w", stdout);
#endif
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int n, m;
	std::cin >> n >> m;
	
	sps f(1 << n), g(1 << n);
	for (int s = 0; s < 1 << n; s ++) {
		std::cin >> g[s];
	}
	for (int s = 1 << n; s --; ) {
		std::cin >> f[s];
	}
	
	auto ans = poly_pow_proj(f, g, m);
	for (int i = 0; i < m; i ++) {
		std::cout << ans[i] << ' ';
	}
	std::cout << '\n';

	return 0;
}
