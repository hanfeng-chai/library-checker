// Problem: P1797 【模板】Stern-Brocot 树
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P1797
// Memory Limit: 512 MB
// Time Limit: 1000 ms
// 
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>
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
	
	inline void write(const char *s) {
		for (int i = 0; s[i]; ++i) {
			pc(s[i]);
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

const ll inf = 0x3f3f3f3f3f3f3f3fLL;

char op[99];

void solve() {
	reads(op);
	if (op[0] == 'D') {
		int k = read();
		ll lx = 0, ly = 1, rx = 1, ry = 0;
		while (k--) {
			char o[9];
			reads(o);
			int x = read();
			if (o[0] == 'L') {
				rx += lx * x;
				ry += ly * x;
			} else {
				lx += rx * x;
				ly += ry * x;
			}
		}
		writesp(lx + rx);
		writeln(ly + ry);
	} else if (op[0] == 'E') {
		ll x = read();
		ll y = read();
		ll lx = 0, ly = 1, rx = 1, ry = 0;
		vector< pair<char, ll> > ans;
		while (1) {
			ll mx = lx + rx, my = ly + ry;
			if (mx == x && my == y) {
				break;
			}
			if (lx * y < ly * x && x * my < y * mx) {
				ll k = (y * rx - x * ry - 1) / (x * ly - y * lx);
				ans.pb('L', k);
				rx += lx * k;
				ry += ly * k;
			} else {
				ll k = (x * ly - y * lx - 1) / (y * rx - x * ry);
				ans.pb('R', k);
				lx += rx * k;
				ly += ry * k;
			}
		}
		write((int)ans.size());
		for (auto p : ans) {
			pc(' ');
			pc(p.fst);
			pc(' ');
			write(p.scd);
		}
		pc('\n');
	} else if (op[0] == 'L') {
		ll ax = read();
		ll ay = read();
		ll bx = read();
		ll by = read();
		if (ax * by > bx * ay) {
			swap(ax, bx);
			swap(ay, by);
		}
		ll lx = 0, rx = 1, ly = 1, ry = 0;
		while (1) {
			ll mx = lx + rx, my = ly + ry;
			if (ax * my <= ay * mx && mx * by <= my * bx) {
				writesp(mx);
				writeln(my);
				break;
			}
			if (bx * my < by * mx) {
				ll k = (by * rx - bx * ry - 1) / (bx * ly - by * lx);
				rx += lx * k;
				ry += ly * k;
			} else {
				ll k = (ax * ly - ay * lx - 1) / (ay * rx - ax * ry);
				lx += rx * k;
				ly += ry * k;
			}
		}
	} else if (op[0] == 'A') {
		ll z = read();
		ll x = read();
		ll y = read();
		ll lx = 0, ly = 1, rx = 1, ry = 0;
		vector< pair<char, ll> > ans;
		while (1) {
			ll mx = lx + rx, my = ly + ry;
			if (mx == x && my == y) {
				break;
			}
			if (lx * y < ly * x && x * my < y * mx) {
				ll k = (y * rx - x * ry - 1) / (x * ly - y * lx);
				ans.pb('L', k);
				rx += lx * k;
				ry += ly * k;
			} else {
				ll k = (x * ly - y * lx - 1) / (y * rx - x * ry);
				ans.pb('R', k);
				lx += rx * k;
				ly += ry * k;
			}
		}
		lx = 0;
		ly = 1;
		rx = 1;
		ry = 0;
		for (auto p : ans) {
			if (p.fst == 'L') {
				if (z <= p.scd) {
					rx += lx * z;
					ry += ly * z;
					z = 0;
					break;
				}
				z -= p.scd;
				rx += lx * p.scd;
				ry += ly * p.scd;
			} else {
				if (z <= p.scd) {
					lx += rx * z;
					ly += ry * z;
					z = 0;
					break;
				}
				z -= p.scd;
				lx += rx * p.scd;
				ly += ry * p.scd;
			}
		}
		if (z) {
			writeln(-1);
			return;
		}
		writesp(lx + rx);
		writeln(ly + ry);
	} else {
		ll x = read();
		ll y = read();
		ll lx = 0, rx = 1, ly = 1, ry = 0;
		while (1) {
			ll mx = lx + rx, my = ly + ry;
			if (mx == x && my == y) {
				writesp(lx);
				writesp(ly);
				writesp(rx);
				writeln(ry);
				return;
			}
			if (lx * y < ly * x && x * my < y * mx) {
				ll k = (y * rx - x * ry - 1) / (x * ly - y * lx);
				rx += lx * k;
				ry += ly * k;
			} else {
				ll k = (x * ly - y * lx - 1) / (y * rx - x * ry);
				lx += rx * k;
				ly += ry * k;
			}
		}
	}
}

int main() {
	int T = 1;
	scanf("%d", &T);
	while (T--) {
		solve();
	}
	return 0;
}
