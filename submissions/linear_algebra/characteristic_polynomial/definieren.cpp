#include <bits/stdc++.h>

#define fir first
#define sec second
#define mkp make_pair
#define mkt make_tuple
#ifdef LOCAL
#define dbg(x) cerr << "In Line " << __LINE__ << " the " << #x << " = " << x << '\n'
#define dpi(x, y) cerr << "In Line " << __LINE__ << " the " << #x << " = " << x << " ; " << "the " << #y << " = " << y << '\n'
#define dbgf(fmt, args...) fprintf(stderr, fmt, ##args)
#else
#define dbg(x) void()
#define dpi(x, y) void()
#define dbgf(fmt, args...) void()
#endif

using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned int;
using ldb = long double;
using i128 = __int128_t;
using u128 = __uint128_t;
using pii = pair<int, int>;
using pil = pair<int, i64>;
using pli = pair<i64, int>;
using vi = vector<int>;
using vpii = vector<pii>;

namespace {
bool Mbe;
constexpr int MOD = 998244353;
template<typename T> T Norm(T a, T p = MOD) { return (a % p + p) % p; }
template<typename T> bool cmax(T &a, T b) { return a < b ? a = b, true : false; }
template<typename T> bool cmin(T &a, T b) { return a > b ? a = b, true : false; }
template<typename T> T DivFloor(T a, T b) { return a >= 0 ? a / b : (a - b + 1) / b; }
template<typename T> T DivCeil(T a, T b) { return a >= 0 ? (a + b - 1) / b : a / b; }

namespace FastIO {
	constexpr int LEN = 1 << 20;
	char in[LEN + 1], out[LEN + 1];
	char *pin = in, *pout = out, *ein = in, *eout = out + LEN;

	char gc() { return pin == ein && (ein = (pin = in) + fread(in, 1, LEN, stdin), ein == in) ? EOF : *pin ++; }
	void pc(char c) { pout == eout && (fwrite(out, 1, LEN, stdout), pout = out); (*pout ++) = c; return; }
	struct Flush { ~Flush() { fwrite(out, 1, pout - out, stdout); pout = out; return; } } _flush;

	template<typename T> T Read() {
		T x = 0; int f = 1; char ch = gc();
		while (ch < '0' || ch > '9') f = (ch == '-' ? (~f + 1) : f), ch = gc();
		while (ch >= '0' && ch <= '9') x = (x << 1) + (x << 3) + (ch ^ 48), ch = gc();
		return x * f;
	}
	void Read(char *s) {
		char ch = gc();
		while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') ch = gc();
		while ((ch != EOF) && !(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')) *s = ch, s ++, ch = gc();
		*s = '\0'; return;
	}
	template<typename T> void Read(T &x) { x = Read<T>(); return; }
	template<typename T, typename ...Args>
	void Read(T &x, Args &...args) { Read(x), Read(args...); return; }
	template<typename T> void Write(T x) {
		static char stk[40]; int tp = 0;
		if (x < 0) pc('-'), x = ~x + 1;
		do stk[tp++] = x % 10 + 48, x /= 10; while (x);
		while (tp --) pc(stk[tp]);
		return;
	}
	void Write(char ch) { pc(ch); return; }
	void Write(const char *s) {
		while (*s != '\0') pc(*s), s ++;
		return;
	}
	void Puts(const char *s) {
		Write(s), pc('\n'); return;
	}
	template<typename T, typename ...Args>
	void Write(T x, Args ...args) { Write(x), Write(args...); return; }
} using namespace FastIO;

template<class U0, class U1, class S0, U0 P>
struct Static_Modint {
private:
	static_assert((P >> (sizeof(U0) * 8 - 1)) == 0, "'Mod' must less than max(U0)/2");
	static constexpr U0 Mod = P;
	U0 x;
	template<class T>
	static constexpr unsigned SafeMod(T x) {
		if constexpr (is_unsigned<T>::value) {
			x %= static_cast<T>(Mod);
			return static_cast<U0>(x);
		} else {
			if ((x %= static_cast<T>(Mod)) < 0)
				x += static_cast<T>(Mod);
			return static_cast<U0>(x);
		}
	}
public:
	constexpr Static_Modint(): x(static_cast<U0>(0)) {}
	template<class T>
	constexpr Static_Modint(T _x): x(SafeMod(_x)) {}
	static constexpr Static_Modint raw(U0 _x) {
		Static_Modint x;
		return x.x = _x, x;
	}
	static constexpr U0 GetMod() {
		return Mod;
	}
	template<class T>
	explicit constexpr operator T() const {
		return static_cast<T>(x);
	}
	constexpr Static_Modint &operator += (const Static_Modint &rhs) {
		x = ((x += rhs.x) >= Mod) ? (x - Mod) : x;
		return *this;
	}
	constexpr Static_Modint &operator -= (const Static_Modint &rhs) {
		x = ((x -= rhs.x) >= Mod) ? (x + Mod) : x;
		return *this;
	}
	constexpr Static_Modint &operator *= (const Static_Modint &rhs) {
		x = (static_cast<U1>(x) * rhs.x) % Mod;
		return *this;
	}
	constexpr Static_Modint &operator /= (const Static_Modint &rhs) {
		return (*this *= rhs.inv());
	}
	friend constexpr Static_Modint fma(const Static_Modint &a, const Static_Modint &b, const Static_Modint &c) {
		return raw((static_cast<U1>(a.x) * b.x + c.x) % Mod);
	}
	friend constexpr Static_Modint fam(const Static_Modint &a, const Static_Modint &b, const Static_Modint &c) {
		return raw((a.x + static_cast<U1>(b.x) * c.x) % Mod);
	}
	friend constexpr Static_Modint fms(const Static_Modint &a, const Static_Modint &b, const Static_Modint &c) {
		return raw((static_cast<U1>(a.x) * b.x + Mod - c.x) % Mod);
	}
	friend constexpr Static_Modint fsm(const Static_Modint &a, const Static_Modint &b, const Static_Modint &c) {
		return raw((a.x + static_cast<U1>(Mod - b.x) * c.x) % Mod);
	}
	constexpr Static_Modint div_2() const {
		return raw(((x & 1) ? (x + Mod) : x) >> 1);
	}
	constexpr Static_Modint inv() const {
		U0 a = Mod, b = x; S0 y = 0, z = 1;
		while (b) {
			const U0 q = a / b;
			const U0 c = a - q * b;
			a = b, b = c;
			const S0 w = y - static_cast<S0>(q) * z;
			y = z, z = w;
		}
		return raw(y < 0 ? y + Mod : y);
	}
	friend constexpr Static_Modint operator + (const Static_Modint &x) {
		return x;
	}
	friend constexpr Static_Modint operator - (Static_Modint x) {
		x.x = x.x ? (Mod - x.x) : 0U;
		return x;
	}
	constexpr Static_Modint &operator ++ () {
		x = (x + 1 == Mod) ? 0U : (x + 1);
		return *this;
	}
	constexpr Static_Modint &operator -- () {
		x = (x == 0U) ? (Mod - 1) : (x - 1);
		return *this;
	}
	constexpr Static_Modint operator ++ (int) {
		Static_Modint tmp = (*this);
		return ++ (*this), tmp;
	}
	constexpr Static_Modint operator -- (int) {
		Static_Modint tmp = (*this);
		return -- (*this), tmp;
	}
	friend constexpr Static_Modint operator + (Static_Modint x, const Static_Modint &y) {
		return x += y;
	}
	friend constexpr Static_Modint operator - (Static_Modint x, const Static_Modint &y) {
		return x -= y;
	}
	friend constexpr Static_Modint operator * (Static_Modint x, const Static_Modint &y) {
		return x *= y;
	}
	friend constexpr Static_Modint operator / (Static_Modint x, const Static_Modint &y) {
		return x /= y;
	}
	constexpr Static_Modint Pow(long long y) const {
		if (y < 0) return inv().Pow(- y);
		Static_Modint x = *this, ans;
		ans.x = static_cast<U0>(1);
		for (; y; y >>= 1, x *= x)
			if (y & 1) ans *= x;
		return ans;
	}
	friend constexpr ostream& operator << (ostream& os, const Static_Modint &x) {
		return os << x.x;
	}
	friend constexpr bool operator == (const Static_Modint &x, const Static_Modint &y) {
		return x.x == y.x;
	}
	friend constexpr bool operator != (const Static_Modint &x, const Static_Modint &y) {
		return x.x != y.x;
	}
	friend constexpr bool operator <= (const Static_Modint &x, const Static_Modint &y) {
		return x.x <= y.x;
	}
	friend constexpr bool operator >= (const Static_Modint &x, const Static_Modint &y) {
		return x.x >= y.x;
	}
	friend constexpr bool operator < (const Static_Modint &x, const Static_Modint &y) {
		return x.x < y.x;
	}
	friend constexpr bool operator > (const Static_Modint &x, const Static_Modint &y) {
		return x.x > y.x;
	}
};
template<u32 P>
using sm32 = Static_Modint<u32, u64, int, P>;
template<u64 P>
using sm64 = Static_Modint<u64, u128, i64, P>;
using Z = sm32<MOD>;

std::vector<Z> charPoly(std::vector<std::vector<Z>> A) {
	const size_t n = A.size();
	for (size_t i = 0; i < n; i ++) {
		for (size_t j = 0; j < n; j ++) {
			A[i][j] = -A[i][j];
		}
	}
	for (size_t i = 0; i + 2 < n; i ++) {
		size_t pivot = i + 1;
		for (; pivot < n && !A[pivot][i]; ++ pivot);
		if (pivot == n) {
			continue;
		}
		if (pivot > i + 1) {
			for (size_t j = i; j < n; j ++) {
				std::swap(A[i + 1][j], A[pivot][j]);
			}
			for (size_t j = 0; j < n; j ++) {
				std::swap(A[j][i + 1], A[j][pivot]);
			}
		}
		const Z inv = A[i + 1][i].inv();
		for (size_t j = i + 2; j < n; j ++) {
			if (A[j][i]) {
				const Z t = A[j][i] * inv;
				for (size_t k = i; k < n; k ++) {
					A[j][k] = fsm(A[j][k], t, A[i + 1][k]);
				}
				for (size_t k = 0; k < n; k ++) {
					A[k][i + 1] = fam(A[k][i + 1], t, A[k][j]);
				}
			}
		}
	}
	std::vector<std::vector<Z>> dp(n + 1);
	dp[0] = {Z(1)};
	for (size_t i = 0; i < n; i ++) {
		dp[i + 1].assign(i + 2, Z(0));
		for (size_t k = 0; k <= i; k ++) {
			dp[i + 1][k + 1] = dp[i][k];
		}
		for (size_t k = 0; k <= i; k ++) {
			dp[i + 1][k] = fam(dp[i + 1][k], A[i][i], dp[i][k]);
		}
		Z prod = 1;
		for (size_t j = i; j; j --) {
			prod *= -A[j][j - 1];
			const Z t = prod * A[j - 1][i];
			for (size_t k = 0; k < j; k ++) {
				dp[i + 1][k] = fam(dp[i + 1][k], t, dp[j - 1][k]);
			}
		}
	}
	return dp[n];
}

void slv() {
	
	int N;
	Read(N);
	
	vector<vector<Z>> A(N, vector<Z>(N));
	for (int i = 0; i < N; i ++) {
		for (int j = 0; j < N; j ++) {
			A[i][j] = Read<u32>();
		}
	}
	
	auto p = charPoly(A);
	for (int i = 0; i <= N; i ++) {
		Write((u32)p[i], ' ');
	}
	
	return;
}
void clr() {

	return;
}
bool Med;
}

int main() {
#ifdef LOCAL
	freopen("!in.in", "r", stdin);
	freopen("!out.out", "w", stdout);
	fprintf(stderr, "%.3lf Mb\n", fabs((&Mbe - &Med) / 1048576.0));
#endif
	int T = 1;
//	int T = Read<int>();
	while (T --) slv(), clr();
#ifdef LOCAL
	fprintf(stderr, "%d ms\n", (int)clock());
#endif
	return 0;
}
