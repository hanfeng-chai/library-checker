// Problem: P2075 区间 LIS
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P2075
// Memory Limit: 128 MB
// Time Limit: 2000 ms
// 
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>
#define int unsigned
#define pb emplace_back
#define fst first
#define scd second
#define mkp make_pair
#define mems(a, x) memset((a), (x), sizeof(a))

using namespace std;
using ll = long long;
using ull = unsigned long long;
using db = double;
using ldb = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

namespace IO {
	const int maxn = 1 << 20;
	
	char ibuf[maxn], *iS, *iT, obuf[maxn], *oS = obuf;

	inline char gc() {
		return (iS == iT ? iT = (iS = ibuf) + fread(ibuf, 1, maxn, stdin), (iS == iT ? EOF : *iS++) : *iS++);
	}

	template<typename T = int>
	inline T read() {
		char c = gc();
		T x = 0;
		bool f = 0;
		while (c < '0' || c > '9') {
			f |= (c == '-');
			c = gc();
		}
		while (c >= '0' && c <= '9') {
			x = (x << 1) + (x << 3) + (c ^ 48);
			c = gc();
		}
		return f ? ~(x - 1) : x;
	}
	
	inline int reads(char *s) {
		char c = gc();
		int len = 0;
		while (isspace(c)) {
			c = gc();
		}
		while (!isspace(c) && c != -1) {
			s[len++] = c;
			c = gc();
		}
		s[len] = '\0';
		return len;
	}
	
	inline string reads() {
		char c = gc();
		string s;
		while (isspace(c)) {
			c = gc();
		}
		while (!isspace(c) && c != -1) {
			s += c;
			c = gc();
		}
		return s;
	}

	inline void flush() {
		fwrite(obuf, 1, oS - obuf, stdout);
		oS = obuf;
	}
	
	struct Flusher {
		~Flusher() {
			flush();
		}
	} AutoFlush;

	inline void pc(char ch) {
		if (oS == obuf + maxn) {
			flush();
		}
		*oS++ = ch;
	}
	
	inline void write(char *s) {
		for (int i = 0; s[i]; ++i) {
			pc(s[i]);
		}
	}
	
	inline void write(const char *s) {
		for (int i = 0; s[i]; ++i) {
			pc(s[i]);
		}
	}

	template<typename T>
	inline void write(T x) {
		static char stk[64], *tp = stk;
		if (x < 0) {
			x = ~(x - 1);
			pc('-');
		}
		do {
			*tp++ = x % 10;
			x /= 10;
		} while (x);
		while (tp != stk) {
			pc((*--tp) | 48);
		}
	}
	
	template<typename T>
	inline void writesp(T x) {
		write(x);
		pc(' ');
	}
	
	template<typename T>
	inline void writeln(T x) {
		write(x);
		pc('\n');
	}
}

using IO::read;
using IO::reads;
using IO::write;
using IO::pc;
using IO::writesp;
using IO::writeln;

const int maxn = 100100;

int n, m, a[maxn], b[maxn], ans[maxn], blo, buc[maxn];
bool vis[maxn];

struct que {
	int l, r, i;
} c[maxn], d[maxn];

namespace BIT {
	int c[maxn];
	
	inline void update(int x, int d) {
		for (int i = x; i; i -= (i & (-i))) {
			c[i] += d;
		}
	}
	
	inline int query(int x) {
		int res = 0;
		for (int i = x; i <= n; i += (i & (-i))) {
			res += c[i];
		}
		return res;
	}
}

struct LargeHeap {
	int n, a[320];
	
	inline void push(int x) {
		a[++n] = x;
		int u = n;
		while (u > 1) {
			if (a[u >> 1] < a[u]) {
				swap(a[u], a[u >> 1]);
				u >>= 1;
			} else {
				break;
			}
		}
	}
	
	inline void pop() {
		swap(a[n--], a[1]);
		int u = 1;
		while ((u << 1) <= n) {
			int v = u << 1 | ((u << 1 | 1) <= n && a[u << 1 | 1] > a[u << 1]);
			if (a[v] > a[u]) {
				swap(a[u], a[v]);
				u = v;
			} else {
				break;
			}
		}
	}
	
	inline void replace(int x) {
		a[1] = x;
		int u = 1;
		while ((u << 1) <= n) {
			int v = u << 1 | ((u << 1 | 1) <= n && a[u << 1 | 1] > a[u << 1]);
			if (a[v] > a[u]) {
				swap(a[u], a[v]);
				u = v;
			} else {
				break;
			}
		}
	}
	
	inline void clear() {
		for (int i = 1; i <= n; ++i) {
			a[i] = 0;
		}
		n = 0;
	}
} Q[320];

struct SmallHeap {
	int n, a[320];
	
	inline void push(int x) {
		a[++n] = x;
		int u = n;
		while (u > 1) {
			if (a[u >> 1] > a[u]) {
				swap(a[u], a[u >> 1]);
				u >>= 1;
			} else {
				break;
			}
		}
	}
	
	inline void pop() {
		swap(a[n--], a[1]);
		int u = 1;
		while ((u << 1) <= n) {
			int v = u << 1 | ((u << 1 | 1) <= n && a[u << 1 | 1] < a[u << 1]);
			if (a[v] < a[u]) {
				swap(a[u], a[v]);
				u = v;
			} else {
				break;
			}
		}
	}
	
	inline void replace(int x) {
		a[1] = x;
		int u = 1;
		while ((u << 1) <= n) {
			int v = u << 1 | ((u << 1 | 1) <= n && a[u << 1 | 1] < a[u << 1]);
			if (a[v] < a[u]) {
				swap(a[u], a[v]);
				u = v;
			} else {
				break;
			}
		}
	}
	
	inline void clear() {
		for (int i = 1; i <= n; ++i) {
			a[i] = 0;
		}
		n = 0;
	}
} pq;

inline void build(int k) {
	if (!vis[k]) {
		return;
	}
	vis[k] = 0;
	int l = (k - 1) * blo + 1, r = min(k * blo, n);
	pq.clear();
	for (int i = 1; i <= Q[k].n; ++i) {
		int u = Q[k].a[i];
		if (u & 1) {
			if (pq.n < r - l + 1) {
				pq.push(u >> 1);
			} else {
				pq.replace(u >> 1);
			}
		}
	}
	Q[k].clear();
	for (int i = l; i <= r; ++i) {
		if (pq.n && b[i] > pq.a[1]) {
			int t = b[i];
			b[i] = pq.a[1];
			pq.replace(t);
		}
	}
}

inline void push(int k) {
	int l = (k - 1) * blo + 1, r = min(k * blo, n);
	Q[k].clear();
	for (int i = l; i <= r; ++i) {
		if (b[i]) {
			Q[k].a[++Q[k].n] = (b[i] << 1);
		}
	}
	make_heap(Q[k].a + 1, Q[k].a + Q[k].n + 1);
}

void solve() {
	n = read();
	m = read();
	for (int i = 1; i <= n; ++i) {
		a[i] = read() + 1;
	}
	for (int i = 1; i <= m; ++i) {
		c[i].l = read() + 1;
		c[i].r = read();
		c[i].i = i;
		++buc[c[i].r];
	}
	for (int i = 1; i <= n; ++i) {
		buc[i] += buc[i - 1];
	}
	for (int i = m; i; --i) {
		d[buc[c[i].r]--] = c[i];
	}
	blo = sqrt(n);
	int tot = (n + blo - 1) / blo;
	for (int i = 1, j = 1; i <= n; ++i) {
		int x = a[i], y = 0;
		int k = (x + blo - 1) / blo;
		build(k);
		b[x] = i;
		BIT::update(i, 1);
		for (int j = x + 1; j <= min(k * blo, n); ++j) {
			if (b[j] > y) {
				swap(b[j], y);
			}
		}
		for (int j = k + 1; j <= tot; ++j) {
			if (Q[j].n && y < (Q[j].a[1] >> 1)) {
				vis[j] = 1;
				int t = y;
				y = Q[j].a[1] >> 1;
				Q[j].replace(t << 1 | 1);
			}
		}
		BIT::update(y, -1);
		push(k);
		while (j <= m && d[j].r == i) {
			ans[d[j].i] = BIT::query(d[j].l);
			++j;
		}
	}
	for (int i = 1; i <= m; ++i) {
		writeln(ans[i]);
	}
}

signed main() {
	int T = 1;
	// scanf("%d", &T);
	while (T--) {
		solve();
	}
	return 0;
}
