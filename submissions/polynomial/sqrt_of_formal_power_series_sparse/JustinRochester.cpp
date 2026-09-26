#include<bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define pw(x) (1ll<<(x))
#define lc(x) ((x)<<1)
#define rc(x) ((x)<<1|1)
#define rep(i, a, b) for(int i=(a), i##i=(b); i<=i##i; ++i)
#define per(i, a, b) for(int i=(b), i##i=(a); i>=i##i; --i)

#define de(a) cerr << #a << " = " << a << endl
#define dd(a) cerr << #a << " = " << a << " "
#define rsz(a, x) (a.resize(x))

#define mp make_pair
#define pb push_back
#define eb emplace_back
#define sz(a) (a.size())
#define all(a) a.begin(), a.end()
#define DEFOP(t, op, stm) \
inline t& operator op##= (const t &x) { do{stm;}while(0); return *this; } \
inline t operator op (const t &x) const { return t(*this) op##= x; }

typedef unsigned uint;
typedef unsigned long long ull;
typedef long long ll;
typedef pair<int, int> pii;
typedef double db;

struct FIO {
	char s[1<<20|1], *p1, *p2;
	char buf[1<<20|1], *p;
	inline FIO() { p1=p2=s; p=buf; }
	inline void flush() { p-=fwrite(buf, 1, p-buf, stdout); }
	inline ~FIO() { flush(); }
	inline char gc() {
		return (p1==p2)&&(p2=(p1=s)+fread(s, 1, 1<<20, stdin), p1==p2)?EOF:*(p1++);
	}
	inline FIO& operator >> (uint &x) {
		x=0;
		char c=0;
		while(!isdigit(c)) c=gc();
		while(isdigit(c)) x=x*10+c-'0', c=gc();
		return *this;
	}
	inline FIO& operator >> (ull &x) {
		x=0;
		char c=0;
		while(!isdigit(c)) c=gc();
		while(isdigit(c)) x=x*10+c-'0', c=gc();
		return *this;
	}
	inline FIO& operator << (char c) {
		if(p-buf>>20) flush();
		*(p++)=c;
		return *this;
	}
	inline FIO& operator << (char *s) {
		for(char *t=s; *t; ++t) *this<<*t;
		return *this;
	}
	inline FIO& operator << (uint x) {
		static char tmp[20]={0};
		char *t=tmp+19;
		if(x==0)
			return *this<<'0';
		for(;x;x/=10)
			*(--t)=x%10+'0';
		return *this<<t;
	}
}fio;

inline uint kpow(uint a, ull x, uint p) { uint ans=1; for(; x; x>>=1, a=(ull)a*a%p) if(x&1) ans=(ull)ans*a%p; return ans; }

template<class T>
inline T norm(T v, T p) { return v>=p?v-p:v; }

template<class T>
T exgcd(T a, T b, T &x, T &y) {
	static T g;
	return b?(exgcd(b, a%b, y, x), y-=a/b*x, g):(x=1, y=0, g=a);
}

template<class T>
inline T inv(T a, T p) {
	static T x, y;
	return exgcd(a, p, x, y)==1?norm(x+p, p):0;
}

namespace Cipolla {
	mt19937 rnd(time(0));
	const int P=998244353;
	int img2;
	inline int lengendre(int a, int p) { return kpow(a, p>>1, p); }
	struct Z {
		int r, i;
		Z(int r_=0, int i_=0):r(r_), i(i_) {}
		inline Z& operator *= (const Z &x) {
			tie(r, i)=make_pair (
				((ll)r*x.r+(ll)i*x.i%P*img2)%P,
				((ll)r*x.i+(ll)i*x.r)%P
				);
			return *this;
		}
	};
	inline Z kpow(Z a, ll x) { Z ans(1, 0); for(;x;x>>=1, a*=a) if(x&1) ans*=a; return ans; }
	inline int work(int n, int p) {
		int x=lengendre(n, P);
		if(x==0) return 0;
		else if(x==P-1) return -1;
		for(x=rnd()%P; lengendre( img2=((ll)x*x%P+P-n)%P, P )!=P-1; x=rnd()%P);
		return kpow(Z(x, 1), (P+1>>1)).r;
	}
}
auto sqrt_mod=Cipolla::work;

inline uint mul(uint a, uint b, uint m) { return (ull)a*b%m; }
//inline ull mul(ull a, ull b, ull m) { return (__uint128_t)a*b%m; }
//inline ull mul(ull a, ull b, ull m) { return (a*b-(ull)((long double)a/m*b)*m+m)%m; }
struct Z{
	typedef unsigned u;
	//typedef unsigned long long u;
	static const u m;
	inline static void setm(u m_) {}
	
	u v;
	inline Z(u v_=0):v(v_) {}
	DEFOP(Z, +, v=norm(v+x.v, m))
	DEFOP(Z, -, v=norm(v+m-x.v, m))
	DEFOP(Z, *, v=mul(v, x.v, m))
	
	inline Z operator ! () const { return Z(inv(get(), m)); }
	inline Z operator - () const { return Z() - *this; }
	inline Z pow(ull x) const { Z ans(1), a(*this); for(; x; x>>=1, a*=a) if(x&1) ans*=a; return ans; }
	inline u get() const { return v; }
	inline operator u() const { return get(); }
};
const Z::u Z::m = 998244353;

typedef vector<Z> poly;
template<class T>
inline T& operator << (T &out, const poly &p) {
	if(!p.empty()) out<<p[0];
	rep(i, 1, sz(p)-1) out<<' '<<p[i];
	return out;
}
typedef vector<pair<uint, Z> > sparse_poly;

const uint P=998244353;
namespace Poly {
	poly sparse_inv(const sparse_poly &f, int n) {
		poly g(n);
		g[0]=!f[0].se;
		rep(i, 1, n-1) {
			Z &v=g[i];
			for(const auto &e : f) {
				int idx=e.fi;
				Z val=e.se;
				if(idx==0 || idx>i) continue;
				v-=val*g[i-idx];
			}
			v*=g[0];
		}
		return g;
	}
	poly sparse_exp(const sparse_poly &f, uint n) {
		poly g(n);
		g[0]=g[1]=1; rep(i, 2, n-1) g[i]=-Z(P/i)*g[P%i];
		rep(i, 1, n-1) {
			Z v=0;
			for(const auto &e : f) {
				int idx=e.fi;
				Z val=e.se;
				if(idx==0 || idx>i) continue;
				v+=Z(idx)*val*g[i-idx];
			}
			g[i]*=v;
		}
		return g;
	}
	poly sparse_log(const sparse_poly &f, uint n) {
		poly g(n);
		g[1]=1; rep(i, 2, n-1) g[i]=-Z(P/i)*g[P%i];
		int cur=1;
		rep(i, 1, n-1) {
			Z v=0;
			for(const auto &e : f) {
				int idx=e.fi;
				Z val=e.se;
				if(idx==0 || idx>i) continue;
				v-=Z(i-idx)*g[i-idx]*val;
			}
			g[i]*=v;
			if(cur<sz(f) && f[cur].fi == i)
				g[i]+=f[cur++].se;
		}
		return g;
	}
	poly sparse_pow(const sparse_poly &f, uint n, ull m) {
		poly g(n); m%=P;
		g[0]=g[1]=1; rep(i, 2, n-1) g[i]=-Z(P/i)*g[P%i];
		rep(i, 1, n-1) {
			Z v=0;
			for(const auto &e : f) {
				int idx=e.fi;
				Z val=e.se;
				if(idx==0 || idx>i) continue;
				v+=val*g[i-idx]*Z(((m+1)*idx-i)%P+P);
			}
			g[i]*=v;
		}
		return g;
	}
	poly sparse_pow_any(const sparse_poly &f, uint n, ull m) {
		if(m==0) {
			poly g(n); g[0]=1;
			return g;
		}
		if(f.empty()) return poly(n);
		uint offset=f[0].fi; Z val=!f[0].se;
		if(offset>=(n-1)/m+1) return poly(n);
		sparse_poly g=f;
		rep(i, 0, sz(g)-1) g[i].fi-=offset, g[i].se*=val;
		poly h = sparse_pow(g, n-offset*m, m); val=f[0].se.pow(m);
		rep(i, 0, n-offset*m-1) h[i]*=val;
		h.insert(h.begin(), offset*m, Z());
		return h;
	}
	poly sparse_sqrt(const sparse_poly &f, uint n) {
		return sparse_pow(f, n, !Z(2));
	}
	poly sparse_sqrt_any(const sparse_poly &f, uint n) {
		if(f.empty()) return poly(n);
		if(f[0].fi&1) return poly();
		sparse_poly g = f;
		uint offset=g[0].fi; Z val=!g[0].se;
		int v=sqrt_mod(g[0].se.get(), P);
		if(v==-1) return poly();
		rep(i, 0, sz(g)-1) g[i].fi-=offset, g[i].se*=val;
		poly h = sparse_sqrt(g, n-offset/2); val=Z(v);
		rep(i, 0, n-offset/2-1) h[i]*=val;
		h.insert(h.begin(), offset/2, Z());
		return h;
	}
}

uint n, k;
sparse_poly f;

int main() {
	fio>>n>>k;
	rsz(f, k);
	for(uint i=0, v; i<k; ++i) fio>>f[i].fi>>v, f[i].se=v; sort(all(f));
	poly g = Poly::sparse_sqrt_any(f, n);
	if(g.empty()) fio<<"-1";
	else fio<<g;
	return 0;
}