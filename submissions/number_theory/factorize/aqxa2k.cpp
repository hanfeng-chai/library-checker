#include <bits/stdc++.h>
#include <cstdint>
using namespace std; 

namespace {
    constexpr int LEN = 1 << 20;
    char buffer_in[LEN + 1], buffer_out[LEN + 1];
    char *pin = buffer_in, *pout = buffer_out, *ein = buffer_in, *eout = buffer_out + LEN;

    char gc() { return pin == ein && (ein = (pin = buffer_in) + fread(buffer_in, 1, LEN, stdin), ein == buffer_in) ? EOF : *pin++; }
    void pc(char c) { pout == eout && (fwrite(buffer_out, 1, LEN, stdout), pout = buffer_out); (*pout++) = c; return; }
    struct Flush { ~Flush() { fwrite(buffer_out, 1, pout - buffer_out, stdout); pout = buffer_out; return; } } _flush;

    template<typename T> T rd() {
        T x = 0; int f = 1; char ch = gc();
        while (ch < '0' || ch > '9') f = (ch == '-' ? (~f + 1) : f), ch = gc();
        while (ch >= '0' && ch <= '9') x = (x << 1) + (x << 3) + (ch ^ 48), ch = gc();
        return x * f;
    }
    void rd(char *s) {
        char ch = gc();
        while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') ch = gc();
        while ((ch != EOF) && !(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')) *s = ch, s++, ch = gc();
        *s = '\0'; return;
    }
    template<typename T> void rd(T &x) { x = rd<T>(); return; }
    template<typename T, typename ...Args>
    void rd(T &x, Args &...args) { rd(x), rd(args...); return; }

    template<typename T> void wr(T x) {
        static char stk[40]; int tp = 0;
        if (x < 0) pc('-'), x = ~x + 1;
        do stk[tp++] = x % 10 + 48, x /= 10; while (x);
        while (tp--) pc(stk[tp]);
        return;
    }
    void wr(char ch) { pc(ch); return; }
    void wr(const char *s) {
        while (*s != '\0') pc(*s), s++;
        return;
    }
    void ps(const char *s) {
        wr(s), pc('\n'); return;
    }
    void ps() { pc('\n'); }

    template<typename T, typename ...Args>
    void wr(T x, Args ...args) { wr(x), wr(args...); return; }
}



template<class U0, class U1>
struct Montgomery {
	constexpr static unsigned B0 = sizeof(U0) * 8U;
	U0 n, nr, rs, np;
	
	constexpr Montgomery(const U0 &Mod) {
		SetMod(Mod);
	}
	
	constexpr U0 GetMod() const noexcept {
		return n;
	}
	constexpr void SetMod(const U0& Mod) {
		assert(Mod >= 2), assert(Mod % 2 == 1);
		assert((Mod >> (B0 - 2)) == 0);
		n = nr = Mod, rs = -static_cast<U1>(n) % n;
		for (uint32_t i = 0; i < __lg(B0); i ++) {
			nr *= 2 - n * nr;
		}
		np = Reduce(static_cast<U0>(1), rs);
	}
	constexpr U0 Reduce(const U0& x) const noexcept {
		const U0 q = x * nr;
		const U0 m = (static_cast<U1>(q) * n) >> B0;
		return n - m;
	}
	constexpr U0 Reduce(const U0& x, const U0& y) const noexcept {
		const U1 t = static_cast<U1>(x) * y;
		const U0 c = t, d = t >> B0;
		const U0 q = c * nr;
		const U0 m = (static_cast<U1>(q) * n) >> B0;
		return d + n - m;
	}
	constexpr U0 Reduce(const U0& x, const U0& y, const U0& z) const noexcept {
		const U1 t = static_cast<U1>(x) * y;
		const U0 c = t, d = t >> B0;
		const U0 q = c * nr;
		const U0 m = (static_cast<U1>(q) * n) >> B0;
		return z + d + n - m;
	}
	constexpr U0 val(const U0& x) const noexcept {
		const uint64_t t = Reduce(x);
		return (t == n) ? static_cast<U0>(0) : t;
	}
	constexpr U0 zero() const noexcept {
		return static_cast<U0>(0);
	}
	constexpr U0 one() const noexcept {
		return np;
	}
	constexpr U0 raw(const U0& x) const noexcept {
		return Reduce(x, rs);
	}
	template<class U> requires std::unsigned_integral<U>
	constexpr U0 trans(const U& x) const noexcept {
		if (__builtin_expect(x < n, 1)) {
			return raw(x);
		}
		return Reduce(x % n, rs);
	}
	template<class S> requires std::signed_integral<S>
	constexpr U0 trans(S x) const noexcept {
		if (__builtin_expect(0 <= x && x < static_cast<S>(n), 1)) {
			return Raw(x);
		}
		if ((x %= static_cast<S>(n)) < 0) {
			(x += static_cast<S>(n)) %= static_cast<S>(n);
		}
		return Reduce(x, rs);
	}
	constexpr U0 neg(const U0& x) const noexcept {
		return (x != 0) ? (2 * n - x) : x;
	}
	constexpr U0 inc(const U0& x) const noexcept {
		return add(x, np);
	}
	constexpr U0 dec(const U0& x) const noexcept {
		return sub(x, np);
	}
	constexpr U0 add(const U0& x, const U0& y) const noexcept {
		return (x + y >= 2 * n) ? (x + y - 2 * n) : (x + y);
	}
	constexpr U0 sub(const U0& x, const U0& y) const noexcept {
		return (x < y) ? (x - y + 2 * n) : (x - y);
	}
	constexpr U0 mul(const U0& x, const U0& y) const noexcept {
		return Reduce(x, y);
	}
	constexpr U0 mul_add(const U0& x, const U0& y, const U0& z) const noexcept {
		return Reduce(x, y, z);
	}
	constexpr bool same(const U0& x, const U0& y) const noexcept {
		const U0 dif = x - y;
		return (dif == 0) || (dif == n) || (dif == -n);
	}
};

constexpr bool Is_Prime(uint64_t x) noexcept {
	if (x <= 1) {
		return false;
	}
	if (x % 2 == 0) {
		return x == 2;
	}
	
	constexpr array<uint64_t, 11> Base{2, 3, 5, 7, 2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	const uint32_t s = __builtin_ctzll(x - 1);
	const uint64_t d = (x - 1) >> s;
	const int q = 63 ^ __builtin_clzll(d);
	const Montgomery<uint64_t, unsigned __int128> Mod(x);
	const int l = (x >> 32) ? 4 : 0;
	const int r = (x >> 32) ? 11 : 4;
	for (int _ = l; _ < r; _ ++) {
		uint64_t base = Base[_];
		if (base % x == 0) {
			continue;
		}
		base = Mod.trans(base);
		uint64_t a = base;
		for (int i = q - 1; ~i; i --) {
			a = Mod.mul(a, a);
			if ((d >> i) & 1) {
				a = Mod.mul(a, base);
			}
		}
		if (Mod.same(a, Mod.one())) {
			continue;
		}
		for (uint32_t t = 1; t < s && !Mod.same(a, x - Mod.one()); ++ t) {
			a = Mod.mul(a, a);
		}
		if (!Mod.same(a, x - Mod.one())) {
			return false;
		}
	}
	return true;
}

template<bool sorted>
vector<pair<uint64_t, uint32_t>> Factorize(uint64_t n) {
	vector<pair<uint64_t, uint32_t>> ans;
	if (n % 2 == 0) {
		uint32_t z = __builtin_ctzll(n);
		ans.push_back({2ULL, z}), n >>= z;
	}
	auto upd = [&](const uint64_t& x) {
		for (auto &[p, c] : ans) {
			if (x == p) {
				++ c;
				return;
			}
		}
		ans.push_back({x, 1});
	};
	auto Pollard_Rho = [&](const uint64_t& n) -> uint64_t{
		if (n % 2 == 0) {
			return 2ULL;
		}
		const Montgomery<uint64_t, unsigned __int128> Mod(n);
		const uint64_t C1 = 1, C2 = 2, M = 600;
		uint64_t Z1 = 1, Z2 = 2, ans = 0;
		auto find = [&]() {
			uint64_t z1 = Z1, z2 = Z2;
			for (uint64_t k = M; ; k *= 2) {
				const uint64_t x1 = z1 + n, x2 = z2 + n;
				for (uint64_t j = 0; j < k; j += M) {
					const uint64_t y1 = z1, y2 = z2;
					uint64_t q1 = 1, q2 = 2;
					z1 = Mod.mul_add(z1, z1, C1), z2 = Mod.mul_add(z2, z2, C2);
					for (uint64_t i = 0; i < M; ++i) {
						uint64_t t1 = x1 - z1, t2 = x2 - z2;
						z1 = Mod.mul_add(z1, z1, C1), z2 = Mod.mul_add(z2, z2, C2);
						q1 = Mod.mul(q1, t1), q2 = Mod.mul(q2, t2);
					}
					q1 = Mod.mul(q1, x1 - z1), q2 = Mod.mul(q2, x2 - z2);
					const uint64_t q3 = Mod.mul(q1, q2), g3 = std::gcd(n, q3);
					if (g3 == 1) {
						continue;
					}
					if (g3 != n) {
						ans = g3;
						return;
					}
					const uint64_t g1 = std::gcd(n, q1);
					const uint64_t g2 = std::gcd(n, q2);
					const uint64_t C = g1 != 1 ? C1 : C2;
					const uint64_t x = g1 != 1 ? x1 : x2;
					uint64_t z = g1 != 1 ? y1 : y2;
					uint64_t g = g1 != 1 ? g1 : g2;
					if (g == n) {
						do {
							z = Mod.mul_add(z, z, C);
							g = std::gcd(n, x - z);
						} while (g == 1);
					}
					if (g != n) {
						ans = g;
						return;
					}
					Z1 += 2, Z2 += 2;
					return;
				}
			}
    };
    do {
    	find();
    } while (!ans);
        return ans;
	};
	auto DFS = [&](auto &&self, const uint64_t &n) -> void {
		if (Is_Prime(n)) {
			return upd(n);
		}
		uint64_t d = Pollard_Rho(n);
		self(self, d), self(self, n / d);
	};
	if (n > 1) {
		DFS(DFS, n); 
	}
	if constexpr (sorted) {
		sort(ans.begin(), ans.end());
	}
	return ans;
}

int main() {

    int t; 
    rd(t); 
    while (t--) {
        uint64_t x; 
        rd(x); 

        auto f = Factorize<true>(x); 
        uint32_t s = 0; 
        for (auto [p, c] : f) {
            s += c; 
        }
        wr(s, ' '); 
		for (auto [p, c] : f) {
			while (c--) {
				wr(p, ' ');
			}
		}
        ps(); 
    }


	return 0; 
}