#include<bits/stdc++.h>
#define LL long long
#define LLL __int128
#define uint unsigned
#define ldb long double
#define uLL unsigned long long
using namespace std;
#ifndef FASTIO
#define FASTIO
namespace FastIO
{
// ------------------------------
// #define DISABLE_MMAP
// ------------------------------
#if ( defined(LOCAL) || defined(_WIN32) ) && !defined(DISABLE_MMAP)
#define DISABLE_MMAP
#endif
#ifdef LOCAL
	inline constexpr void _chk_i() {}
	inline char _gc_nochk() { return getchar(); }
	inline char _gc() { return getchar(); }
	inline void _chk_o() {}
	inline void _pc_nochk(char c) { putchar(c); }
	inline void _pc(char c) { putchar(c); }
	template < int n > inline void _pnc_nochk(const char *c) { for ( int i = 0 ; i < n ; i++ ) putchar(c[i]); }
#else
#ifdef DISABLE_MMAP
	inline constexpr int _READ_SIZE = 1 << 18; inline static char _read_buffer[_READ_SIZE + 40], *_read_ptr = nullptr, *_read_ptr_end = nullptr; static inline bool _eof = false;
	inline void _chk_i() { if ( __builtin_expect(!_eof, true) && __builtin_expect(_read_ptr_end - _read_ptr < 40, false) ) { int sz = _read_ptr_end - _read_ptr; if ( sz ) memcpy(_read_buffer, _read_ptr, sz); char *beg = _read_buffer + sz; _read_ptr = _read_buffer, _read_ptr_end = beg + fread(beg, 1, _READ_SIZE, stdin); if ( __builtin_expect(_read_ptr_end != beg + _READ_SIZE, false) ) _eof = true, *_read_ptr_end = EOF; } }
	inline char _gc_nochk() { return __builtin_expect(_eof && _read_ptr == _read_ptr_end, false) ? EOF : *_read_ptr++; }
	inline char _gc() { _chk_i(); return _gc_nochk(); }
#else
#include<sys/mman.h>
	inline static char *_read_ptr = (char *)mmap(nullptr, 0x7fffffff, 1, 2, 0, 0);
	inline constexpr void _chk_i() {}
	inline char _gc_nochk() { return *_read_ptr++; }
	inline char _gc() { return *_read_ptr++; }
#endif
	inline constexpr int _WRITE_SIZE = 1 << 18; inline static char _write_buffer[_WRITE_SIZE + 40], *_write_ptr = _write_buffer;
	inline void _chk_o() { if ( __builtin_expect(_write_ptr - _write_buffer > _WRITE_SIZE, false) ) fwrite(_write_buffer, 1, _write_ptr - _write_buffer, stdout), _write_ptr = _write_buffer; }
	inline void _pc_nochk(char c) { *_write_ptr++ = c; }
	inline void _pc(char c) { *_write_ptr++ = c, _chk_o(); }
	template < int n > inline void _pnc_nochk(const char *c) { memcpy(_write_ptr, c, n), _write_ptr += n; }
	inline struct _auto_flush { inline ~_auto_flush() { fwrite(_write_buffer, 1, _write_ptr - _write_buffer, stdout); } } _auto_flush;
#endif
#define println println_ // don't use C++23 std::println
	template < class T > inline constexpr bool _is_signed = numeric_limits < T >::is_signed;
	template < class T > inline constexpr bool _is_unsigned = numeric_limits < T >::is_integer && !_is_signed < T >;
#if __SIZEOF_LONG__ == 64
	template <> inline constexpr bool _is_signed < __int128 > = true;
	template <> inline constexpr bool _is_unsigned < __uint128_t > = true;
#endif
	inline bool _isgraph(char c) { return c >= 33; }
	inline bool _isdigit(char c) { return 48 <= c && c <= 57; } // or faster, remove c <= 57
	constexpr struct _table {
#ifndef LOCAL
	int i[65536];
#endif
	char o[40000]; constexpr _table() :
#ifndef LOCAL
	i{},
#endif
	o{} {
#ifndef LOCAL
	for ( int x = 0 ; x < 65536 ; x++ ) i[x] = -1; for ( int x = 0 ; x <= 9 ; x++ ) for ( int y = 0 ; y <= 9 ; y++ ) i[x + y * 256 + 12336] = x * 10 + y;
#endif
	for ( int x = 0 ; x < 10000 ; x++ ) for ( int y = 3, z = x ; ~y ; y-- ) o[x * 4 + y] = z % 10 + 48, z /= 10; } } _table;
	template < class T, int digit > inline constexpr T _pw10 = 10 * _pw10 < T, digit - 1 >;
	template < class T > inline constexpr T _pw10 < T, 0 > = 1;
	inline void read(char &c) { do c = _gc(); while ( !_isgraph(c) ); }
	inline void read_cstr(char *s) { char c = _gc(); while ( !_isgraph(c) ) c = _gc(); while ( _isgraph(c) ) *s++ = c, c = _gc(); *s = 0; }
	inline void read(string &s) { char c = _gc(); s.clear(); while ( !_isgraph(c) ) c = _gc(); while ( _isgraph(c) ) s.push_back(c), c = _gc(); }
	template < class T, bool neg >
#ifndef LOCAL
	__attribute__((no_sanitize("undefined")))
#endif
	inline void _read_int_suf(T &x) { _chk_i(); char c; while
#ifndef LOCAL
	( ~_table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)] ) if constexpr ( neg ) x = x * 100 - _table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)++]; else x = x * 100 + _table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)++]; if
#endif
	( _isdigit(c = _gc_nochk()) ) if constexpr ( neg ) x = x * 10 - ( c & 15 ); else x = x * 10 + ( c & 15 ); }
	template < class T, enable_if_t < _is_signed < T >, int > = 0 > inline void read(T &x) { char c; while ( !_isdigit(c = _gc()) ) if ( c == 45 ) { _read_int_suf < T, true >(x = -( _gc_nochk() & 15 )); return; } _read_int_suf < T, false >(x = c & 15); }
	template < class T, enable_if_t < _is_unsigned < T >, int > = 0 > inline void read(T &x) { char c; while ( !_isdigit(c = _gc()) ); _read_int_suf < T, false >(x = c & 15); }
	inline void write(bool x) { _pc(x | 48); }
	inline void write(char c) { _pc(c); }
	inline void write_cstr(const char *s) { while ( *s ) _pc(*s++); }
	inline void write(const string &s) { for ( char c : s ) _pc(c); }
	template < class T, bool neg, int digit > inline void _write_int_suf(T x) { if constexpr ( digit == 4 ) _pnc_nochk < 4 >(_table.o + ( neg ? -x : x ) * 4); else _write_int_suf < T, neg, digit / 2 >(x / _pw10 < T, digit / 2 >), _write_int_suf < T, neg, digit / 2 >(x % _pw10 < T, digit / 2 >); }
	template < class T, bool neg, int digit > inline void _write_int_pre(T x) { if constexpr ( digit <= 4 ) if ( digit >= 3 && ( neg ? x <= -100 : x >= 100 ) ) if ( digit >= 4 && ( neg ? x <= -1000 : x >= 1000 ) ) _pnc_nochk < 4 >(_table.o + ( neg ? -x : x ) * 4); else _pnc_nochk < 3 >(_table.o + ( neg ? -x : x ) * 4 + 1); else if ( digit >= 2 && ( neg ? x <= -10 : x >= 10 ) ) _pnc_nochk < 2 >(_table.o + ( neg ? -x : x ) * 4 + 2); else _pc_nochk(( neg ? -x : x ) | 48); else { constexpr int cur = 1 << __lg(digit - 1); if ( neg ? x <= -_pw10 < T, cur > : x >= _pw10 < T, cur > ) _write_int_pre < T, neg, digit - cur >(x / _pw10 < T, cur >), _write_int_suf < T, neg, cur >(x % _pw10 < T, cur >); else _write_int_pre < T, neg, cur >(x); } }
	template < class T, enable_if_t < _is_signed < T >, int > = 0 > inline void write(T x) { if ( x >= 0 ) _write_int_pre < T, false, numeric_limits < T >::digits10 + 1 >(x); else _pc_nochk(45), _write_int_pre < T, true, numeric_limits < T >::digits10 + 1 >(x); _chk_o(); }
	template < class T, enable_if_t < _is_unsigned < T >, int > = 0 > inline void write(T x) { _write_int_pre < T, false, numeric_limits < T >::digits10 + 1 >(x), _chk_o(); }
	template < class T > inline auto read(T &x) -> decltype(x.read(), void()) { x.read(); }
	template < class T > inline auto write(const T &x) -> decltype(x.write(), void()) { x.write(); }
	template < class T1, class ...T2 > inline void read(T1 &x, T2 &...y) { read(x), read(y...); }
	template < class ...T > inline void read_cstr(char *x, T *...y) { read_cstr(x), read_cstr(y...); }
	template < class T1, class ...T2 > inline void write(const T1 &x, const T2 &...y) { write(x), write(y...); }
	template < class ...T > inline void write_cstr(const char *x, const T *...y) { write_cstr(x), write_cstr(y...); }
	template < class T > inline void print(const T &x) { write(x); }
	inline void print_cstr(const char *x) { write_cstr(x); }
	template < class T1, class ...T2 > inline void print(const T1 &x, const T2 &...y) { write(x), _pc(32), print(y...); }
	template < class ...T > inline void print_cstr(const char *x, const T *...y) { write_cstr(x), _pc(32), print_cstr(y...); }
	inline void println() { _pc(10); }
	inline void println_cstr() { _pc(10); }
	template < class ...T > inline void println(const T &...x) { print(x...), _pc(10); }
	template < class ...T > inline void println_cstr(const T *...x) { print_cstr(x...), _pc(10); }
}	using FastIO::read, FastIO::read_cstr, FastIO::write, FastIO::write_cstr, FastIO::println, FastIO::println_cstr;
#endif
/*** standard polynomial ***/
typedef vector<int> poly;
typedef vector<int> Vec;
typedef vector<Vec> Mat;
typedef tuple<poly,poly,poly,poly> Mat2;
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
const int Mod=998244353,G=3;
const LL Mod2=(LL)Mod*Mod;
vector<uLL>Grt,iGrt;
poly frc({1,1}),inv({0,1}),ivf({1,1});
inline int qpow(int x,int y,int z=1){
	for(;y;(y>>=1)&&(x=(LL)x*x%Mod))if(y&1)z=(LL)z*x%Mod;return z;
}
inline void Init(const int&n){
	for(int i=frc.size();i<=n;++i)
		frc.emplace_back((LL)frc.back()*i%Mod),
		inv.emplace_back(Mod-Mod/i*(LL)inv[Mod%i]%Mod),
		ivf.emplace_back((LL)ivf.back()*inv.back()%Mod);
}
inline int Binom(const int&n,const int&m){
	if(n<m||m<0)return 0;
	return Init(n),(LL)frc[n]*ivf[m]%Mod*ivf[n-m]%Mod;
}
inline uLL trans(const uLL&x){
    constexpr uLL A=-(uLL)Mod/Mod+1;
    constexpr uLL Q=(((__uint128_t)(-(uLL)Mod%Mod)<<64)+Mod-1)/Mod;
    return x*A+(uLL)((__uint128_t)x*Q>>64)+1;
}
inline uLL mul(const uLL&x,const uLL&y){
    return x*y*(__uint128_t)Mod>>64;
}
inline int add(int x,const int&y){
    return ((x+=y)-Mod)>=0?x-Mod:x;
}
inline int sub(int x,const int&y){
    return (x-=y)<0?x+Mod:x;
}
inline int div2(const int&x){
    return x&1?(x+Mod)>>1:x>>1;
}
inline bool Empty(poly&P){
	for(;!P.empty()&&!P.back();P.pop_back());
	return P.empty();
}
inline poly Add(poly P,poly Q){
    if(P.size()<Q.size())P.swap(Q);
    for(int i=Q.size();i--;)P[i]=add(P[i],Q[i]);
    return P;
}
inline poly Add_Empty(poly P,poly Q){
    if(P.size()<Q.size())P.swap(Q);
    for(int i=Q.size();i--;)P[i]=add(P[i],Q[i]);
    return Empty(P),P;
}
inline poly Sub(poly P,poly Q){
    if(P.size()<Q.size())P.resize(Q.size());
    for(int i=Q.size();i--;)P[i]=sub(P[i],Q[i]);
    return P;
}
inline poly Sub_Empty(poly P,poly Q){
    if(P.size()<Q.size())P.resize(Q.size());
    for(int i=Q.size();i--;)P[i]=sub(P[i],Q[i]);
    return Empty(P),P;
}
inline poly Mulx(poly P,int x){
    const uLL v=trans(x);
	for(int&i:P)i=mul(i,v);
	return P;
}
inline poly Neg(poly P){
	for(int i=P.size();i--;)P[i]&&(P[i]=Mod-P[i]);
	return P;
}
inline int Eval(poly&P,int x){
    const uLL v=trans(x);
	int z=0;
	for(int i=P.size();i--;)z=add(mul(z,x),P[i]);
	return z;
}
inline poly invLinear(poly P){
	const int n=P.size();
	poly Q(n+1,1);
	for(int i=0;i<n;++i)Q[i+1]=(LL)Q[i]*P[i]%Mod;
	int t=qpow(Q[n],Mod-2);Q.pop_back();
	for(int i=n;i--;)Q[i]=(LL)Q[i]*t%Mod,t=(LL)t*P[i]%Mod;
	return Q;
}
inline void extend(const int&n){
    if(Grt.empty())Grt.emplace_back(trans(1)),iGrt.emplace_back(trans(1));
    if((int)Grt.size()<n){
        int L=Grt.size();
        for(Grt.resize(n),iGrt.resize(n);L<n;L*=2){
            const int w=qpow(G,Mod/(L*4)),iw=qpow(w,Mod-2);
            for(int i=0;i<L;++i)Grt[i+L]=trans(mul(Grt[i],w)),iGrt[i+L]=trans(mul(iGrt[i],iw));
        }
    }
}
template<int A,int B,int C=0,class fun>inline void Butterrep(int i,int j,int k,fun F){
    if(A!=C)F(i,j+C/B,k+C*2-C%B),Butterrep<A,B,C+(C<A)>(i,j,k,F);
}
template<int i,class fun>inline void Butter(int n,fun F){
    if(n>32)for(int j=0;2*i*j<n;j+=32/i)Butterrep<32,i>(i,j,2*i*j,F);
    else if(i<n)for(int j=0;2*i*j<n;++j)for(int k=0;k<i;++k)F(i,j,k+2*i*j);
}
template<class T>inline void DFT(T P,int n){
    extend(n);
    const auto F=[&](int x,int y,int z){
        const int a=P[z],b=mul(P[z+x],Grt[y]);
        P[z]=add(a,b),P[z+x]=sub(a,b);
    };
    for(int i=n>>1;i>16;i>>=1)for(int j=0;2*i*j<n;++j)for(int k=0;k<i;k+=32)Butterrep<32,32>(i,j,k+2*i*j,F);
    Butter<16>(n,F),Butter<8>(n,F),Butter<4>(n,F),Butter<2>(n,F),Butter<1>(n,F);
}
template<class T>inline void IDFT(T P,int n){
    const uLL ni=trans(Mod-(Mod-1)/n);
	for(int i=0;i<n;++i)P[i]=mul(P[i],ni);
    extend(n);
    const auto F=[&](int x,int y,int z){
        const int a=P[z],b=P[z+x];
        P[z]=add(a,b),P[z+x]=mul(a-b+Mod,iGrt[y]);
    };
    Butter<1>(n,F),Butter<2>(n,F),Butter<4>(n,F),Butter<8>(n,F),Butter<16>(n,F);
    for(int i=32;i<n;i<<=1)for(int j=0;2*i*j<n;++j)for(int k=0;k<i;k+=32)Butterrep<32,32>(i,j,k+2*i*j,F);
}
inline void DFT(poly&P){DFT(P.begin(),P.size());}
inline void IDFT(poly&P){IDFT(P.begin(),P.size());}
inline poly Mul(poly P,poly Q){
    if(P.empty()||Q.empty())return poly();
	const int pn=P.size(),qn=Q.size(),rn=pn+qn-1;
	if(min(pn,qn)<=32||max(pn,qn)<=64){
		if(pn<=qn){
			vector<uLL>H(rn);
			for(int i=0;i<pn;++i){
				if(i%8==0)for(int j=qn;j--;)(H[i+j]+=1ll*P[i]*Q[j])>=(Mod2<<3)&&(H[i+j]-=Mod2<<3);
				else for(int j=qn;j--;)H[i+j]+=1ll*P[i]*Q[j];
			}
			Q.resize(rn);
			for(int i=rn;i--;)Q[i]=H[i]%Mod;
			return Q;
		}
		else{
			vector<uLL>H(rn);
			for(int i=0;i<qn;++i){
				if(i%8==0)for(int j=pn;j--;)(H[i+j]+=1ll*Q[i]*P[j])>=(Mod2<<3)&&(H[i+j]-=Mod2<<3);
				else for(int j=pn;j--;)H[i+j]+=1ll*Q[i]*P[j];
			}
			P.resize(rn);
			for(int i=rn;i--;)P[i]=H[i]%Mod;
			return P;
		}
	}
	if(rn<=256){
		const int k=max(pn,qn)/2;
		poly A=(k<pn?poly(P.begin()+k,P.end()):poly());
		poly B=(k<pn?poly(P.begin(),P.begin()+k):P);
		poly C=(k<qn?poly(Q.begin()+k,Q.end()):poly());
		poly D=(k<qn?poly(Q.begin(),Q.begin()+k):Q);
		poly AC=Mul(A,C),BD=Mul(B,D),H=Sub(Mul(Add(A,B),Add(C,D)),Add(AC,BD));
		AC.insert(AC.begin(),k*2,0),H.insert(H.begin(),k,0);
		H=Add(AC,Add(H,BD));
		return H.resize(rn),H;
	}
	const int m=2<<__lg(max(1,rn-1));
	P.resize(m),Q.resize(m);
	DFT(P),DFT(Q);
	for(int i=m;i--;)P[i]=(LL)P[i]*Q[i]%Mod;
	IDFT(P);
	return P.resize(rn),P;
}
class MillerRabin{private:
inline bool mr32(const int&n){
    const auto mul=[&](int x,int y){return (LL)x*y%n;};
    const auto qpow=[&](int x,int y){LL z=1;for(;y;(y>>=1)&&(x=mul(x,x)))if(y&1)z=mul(z,x);return z;};
    const int r=__builtin_ctz(n-1),k=n>>r;int v;
    if(2%n&&(v=qpow(2,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(7%n&&(v=qpow(7,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(61%n&&(v=qpow(61,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    return 1;
}
inline bool mr64(const LL&n){
    const auto mul=[&](LL x,LL y){LL r=x*y-n*(LL)(1.l/n*x*y);return r-n*(r>=n)+n*(r<0);};
    const auto qpow=[&](LL x,LL y){LL z=1;for(;y;(y>>=1)&&(x=mul(x,x)))if(y&1)z=mul(z,x);return z;};
    const LL r=__builtin_ctzll(n-1),k=n>>r;LL v;
    if(2%n&&(v=qpow(2,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(325%n&&(v=qpow(325,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(9375%n&&(v=qpow(9375,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(28178%n&&(v=qpow(28178,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(450775%n&&(v=qpow(450775,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(9780504%n&&(v=qpow(9780504,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    if(1795265022%n&&(v=qpow(1795265022,k))!=1)for(int j=r;v!=n-1;v=mul(v,v))if(!--j)return 0;
    return 1;
}
public:inline bool isprime(const LL&n){return (n<3||!(n&1))?n==2:n<(1ll<<31)?mr32(n):mr64(n);}
}MR;
class PollardRho{private:
	inline LL calc(const LL&n){
		if(!(n&1))return 2;
        uniform_int_distribution<LL>rnd(1,n-1);
		LL x=rnd(rng),y=x,p=1,q,g,rr=rnd(rng);
        const auto mul=[&](LL x,LL y){LL r=x*y-n*(LL)(1.l/n*x*y);return r-n*(r>=n)+n*(r<0);};
        const auto F=[&](LL x){LL z=mul(x,x)+rr;return z<n?z:z-n;};
		for(int i=0;(i&255)||(g=__gcd(p,n))==1;++i,x=F(x),y=F(F(y))){
			if(x==y)x=rnd(rng),rr=rnd(rng),y=F(x);if(q=mul(p,x-y+n))p=q;
		}
		return g;
	}
public:inline vector<LL>split(const LL&n){
		if(n==1)return {};
		if(MR.isprime(n))return {n};
		LL d=calc(n);
		while(d==1||d==n)d=calc(n);
		vector<LL>v1=split(d),v2=split(n/d),v3(v1.size()+v2.size());
		merge(v1.begin(),v1.end(),v2.begin(),v2.end(),v3.begin());
		return v3;
	}
}PR;
inline LL PrimitiveRoot(const LL&n){
	auto D=PR.split(n-1);D.erase(unique(D.begin(),D.end()),D.end());
    const auto mul=[&](LL x,LL y){LL r=x*y-n*(LL)(1.l/n*x*y);return r-n*(r>=n)+n*(r<0);};
    const auto qpow=[&](LL x,LL y){LL z=1;for(;y;(y>>=1)&&(x=mul(x,x)))if(y&1)z=mul(z,x);return z;};
	const auto check=[&](const LL&g){
		for(auto i:D)if(qpow(g,(n-1)/i)==1)return 0;
		return 1;
	};
	LL g=1;for(;!check(g);++g);
	return g;
}
signed main(){
    cin.tie(0)->sync_with_stdio(0);
	int p;read(p);
	int g=PrimitiveRoot(p);
	vector<int>ind(p,-1);
	for(int i=1,j=0;j<p-1;i=1ll*i*g%p,++j)ind[i]=j;
	poly P(p-1),Q(p-1);
	int a0=0,b0=0;LL sa=0,sb=0;
	read(a0);for(int i=1;i<p;++i)read(P[ind[i]]),sa+=P[ind[i]];
	read(b0);for(int i=1;i<p;++i)read(Q[ind[i]]),sb+=Q[ind[i]];
	sa%=Mod,sb%=Mod,P=Mul(P,Q),P.emplace_back(0);
	write((1ll*a0*b0+a0*sb+sa*b0)%Mod);
	for(int i=1;i<p;++i)write(' ',add(P[ind[i]],P[ind[i]+p-1]));
    return 0;
}
/*
*/