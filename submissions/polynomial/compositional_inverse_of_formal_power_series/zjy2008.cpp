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
	inline void _chk_i() {}
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
#include<sys/stat.h>
	inline static char *_read_ptr = (char *)mmap(nullptr, [] { struct stat s; return fstat(0, &s), s.st_size; } (), 1, 2, 0, 0);
	inline void _chk_i() {}
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
	template < size_t N, class ...T > inline void _read_tuple(tuple < T... > &x) { read(get < N >(x)); if constexpr ( N + 1 != sizeof...(T) ) _read_tuple < N + 1, T... >(x); }
	template < size_t N, class ...T > inline void _write_tuple(const tuple < T... > &x) { write(get < N >(x)); if constexpr ( N + 1 != sizeof...(T) ) _pc(32), _write_tuple < N + 1, T... >(x); }
	template < class ...T > inline void read(tuple < T... > &x) { _read_tuple < 0, T... >(x); }
	template < class ...T > inline void write(const tuple < T... > &x) { _write_tuple < 0, T... >(x); }
	template < class T1, class T2 > inline void read(pair < T1, T2 > &x) { read(x.first), read(x.second); }
	template < class T1, class T2 > inline void write(const pair < T1, T2 > &x) { write(x.first), _pc(32), write(x.second); }
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
namespace BasicMath{
	typedef vector<int> poly;
	typedef vector<int> Vec;
	typedef vector<Vec> Mat;
	typedef tuple<poly,poly,poly,poly> Mat2;
	mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
	const int Mod=998244353,Mod_G=3;
	const LL Mod2=(LL)Mod*Mod;
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
	inline poly invLinear(poly P){
		const int n=P.size();
		poly Q(n+1,1);
		for(int i=0;i<n;++i)Q[i+1]=(LL)Q[i]*P[i]%Mod;
		int t=qpow(Q[n],Mod-2);Q.pop_back();
		for(int i=n;i--;)Q[i]=(LL)Q[i]*t%Mod,t=(LL)t*P[i]%Mod;
		return Q;
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
	inline int neg(const int&x){
		return x?Mod-x:0;
	}
	inline int div2(const int&x){
		return x&1?(x+Mod)>>1:x>>1;
	}
}
namespace Polynomial{
	using namespace BasicMath;
	vector<uLL>Grt,iGrt;
	inline bool Empty(poly&P){
		for(;!P.empty()&&!P.back();P.pop_back());
		return P.empty();
	}
	inline poly Slice(poly&P,int l,int r){
		if(r<=0||l>=(int)P.size())return poly(r-l);
		if(0<=l&&r<=(int)P.size())return poly(P.begin()+l,P.begin()+r);
		poly Q;
		if(l<0)Q.insert(Q.end(),-l,0),l=0;
		if(r<=(int)P.size())Q.insert(Q.end(),P.begin()+l,P.begin()+r);
		else Q.insert(Q.end(),P.begin()+l,P.end()),Q.insert(Q.end(),r-P.size(),0);
		return Q;
	}
	inline void Reduce(poly&P,int n){
		for(int i=P.size()-1;i>=n;--i)P[i-n]=add(P[i-n],P[i]);
		P.resize(n);
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
		int z=0;
		for(int i=P.size();i--;)z=((LL)z*x+P[i])%Mod;
		return z;
	}
	inline void extend(const int&n){
		if(Grt.empty())Grt.emplace_back(trans(1)),iGrt.emplace_back(trans(1));
		if((int)Grt.size()<n){
			int L=Grt.size();
			for(Grt.resize(n),iGrt.resize(n);L<n;L*=2){
				const int w=qpow(Mod_G,Mod/(L*4)),iw=qpow(w,Mod-2);
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
		const auto F=[&](int x,int y,int z){const int a=P[z],b=mul(P[z+x],Grt[y]);P[z]=add(a,b),P[z+x]=sub(a,b);};
		for(int i=n>>1;i>16;i>>=1)for(int j=0;2*i*j<n;++j)for(int k=0;k<i;k+=32)Butterrep<32,32>(i,j,k+2*i*j,F);
		Butter<16>(n,F),Butter<8>(n,F),Butter<4>(n,F),Butter<2>(n,F),Butter<1>(n,F);
	}
	template<class T>inline void IDFT(T P,int n){
		const uLL ni=trans(Mod-(Mod-1)/n);
		for(int i=0;i<n;++i)P[i]=mul(P[i],ni);
		extend(n);
		const auto F=[&](int x,int y,int z){const int a=P[z],b=P[z+x];P[z]=add(a,b),P[z+x]=mul(a-b+Mod,iGrt[y]);};
		Butter<1>(n,F),Butter<2>(n,F),Butter<4>(n,F),Butter<8>(n,F),Butter<16>(n,F);
		for(int i=32;i<n;i<<=1)for(int j=0;2*i*j<n;++j)for(int k=0;k<i;k+=32)Butterrep<32,32>(i,j,k+2*i*j,F);
	}
	template<class T>inline void rDFT(T P,int n){
		extend(n);
		const auto F=[&](int x,int y,int z){const int a=P[z],b=mul(P[z+x],Grt[y]);P[z]=add(a,b),P[z+x]=sub(a,b);};
		for(int i=n>>1,t=1;i;i>>=1,t<<=1)for(int j=0;2*i*j<n;++j)for(int k=0;k<i;++k)F(i,j+t,k+2*i*j);
	}
	template<class T>inline void rIDFT(T P,int n){
		const uLL ni=trans(Mod-(Mod-1)/n);
		for(int i=0;i<n;++i)P[i]=mul(P[i],ni);
		extend(n);
		const auto F=[&](int x,int y,int z){const int a=P[z],b=P[z+x];P[z]=add(a,b),P[z+x]=mul(a-b+Mod,iGrt[y]);};
		for(int i=1,t=n>>1;i<n;i<<=1,t>>=1)for(int j=0;2*i*j<n;++j)for(int k=0;k<i;++k)F(i,j+t,k+2*i*j);
	}
	inline void DFT(poly&P){DFT(P.begin(),P.size());}
	inline void IDFT(poly&P){IDFT(P.begin(),P.size());}
	inline void rDFT(poly&P){rDFT(P.begin(),P.size());}
	inline void rIDFT(poly&P){rIDFT(P.begin(),P.size());}
	poly Mul(poly P,poly Q){
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
	poly MulT(poly P,poly Q){
		if(P.empty()||Q.empty())return poly();
		reverse(Q.begin(),Q.end());
		const int pn=P.size(),qn=Q.size(),m=2<<__lg(max(1,pn-1));
		P.resize(m),Q.resize(m);
		DFT(P),DFT(Q);
		for(int i=m;i--;)P[i]=(LL)P[i]*Q[i]%Mod;
		IDFT(P);
		return Slice(P,qn-1,pn);
	}
	poly Inv(poly P){
		const int pn=P.size();
		const int m=2<<__lg(max(1,pn-1));
		poly Q({qpow(P[0],Mod-2)}),F,dQ;
		Q.reserve(m);
		for(int n=1;n<m;n*=2){
			F=Slice(P,0,n*2),dQ=Q;
			dQ.resize(n*2),DFT(dQ),DFT(F);
			for(int i=0;i<n*2;++i)F[i]=(LL)(Mod-F[i])*dQ[i]%Mod;
			IDFT(F),fill_n(F.begin(),n,0),DFT(F);
			for(int i=0;i<n*2;++i)dQ[i]=(LL)F[i]*dQ[i]%Mod;
			IDFT(dQ),Q.insert(Q.end(),dQ.begin()+n,dQ.end());
		}
		return Q.resize(pn),Q;
	}
	poly Quo(poly F,poly P){
		const int pn=P.size();
		if(pn<=64){
			const uLL r=trans(qpow(P[0],Mod-2));
			for(int i=0;i<pn;++i){
				LLL v=F[i];
				for(int j=0;j<i;++j)v-=(LLL)F[j]*P[i-j];
				if((F[i]=v%Mod)<0)F[i]+=Mod;
				F[i]=mul(F[i],r);
			}
			return F;
		}
		const int BL=max(1,__lg(pn)),m=2<<__lg(max(1,(pn-1)/BL)),L=(pn-1)/m+1;
		poly H(Slice(F,0,m)),Q=Inv(Slice(P,0,m));
		vector<poly>A(L),B(L-1);
		Q.resize(m*2),H.resize(m*2),DFT(Q),DFT(H);
		for(int i=0;i<m*2;++i)H[i]=(LL)H[i]*Q[i]%Mod;
		IDFT(H),H.resize(m);
		A[0]=Slice(P,0,m),A[0].resize(m*2),DFT(A[0]);
		for(int k=1;k<L;++k){
			A[k]=Slice(P,k*m,(k+1)*m),A[k].resize(m*2),DFT(A[k]);
			B[k-1]=Slice(H,(k-1)*m,k*m),B[k-1].resize(m*2),DFT(B[k-1]);
			poly C(m*2);
			for(int j=0;j<k;++j){
				for(int i=0;i<m;++i)C[i]=(C[i]+(LL)(A[k-j][i]+A[k-1-j][i])*(Mod-B[j][i]))%Mod;
				for(int i=m;i<m*2;++i)C[i]=(C[i]+(LL)(A[k-j][i]+Mod-A[k-1-j][i])*(Mod-B[j][i]))%Mod;
			}
			IDFT(C),fill_n(C.begin()+m,m,0),C=Add(C,Slice(F,k*m,(k+1)*m)),DFT(C);
			for(int i=0;i<m*2;++i)C[i]=(LL)C[i]*Q[i]%Mod;
			IDFT(C),H.insert(H.end(),C.begin(),C.begin()+m);
		}
		return H.resize(pn),H;
	}
	poly Div(poly,poly);
	pair<poly,poly>DivMod(poly,poly);
	poly ModPow(poly,LL,poly);
	Mat2 Hgcd(poly,poly,int);
	poly Gcd(poly,poly);
	poly Qpow(poly,int);
}
namespace Polynomial{
	poly Deriv(poly P){
		const int n=P.size();
		for(int i=1;i<n;++i)P[i-1]=(LL)P[i]*i%Mod;
		return P.pop_back(),P;
	}
	poly Integ(poly P){
		P.emplace_back(0);
		const int n=P.size();Init(n);
		for(int i=n;--i;)P[i]=(LL)P[i-1]*inv[i]%Mod;
		return P[0]=0,P;
	}
	poly Ln(poly P){
		const int n=P.size();
		poly Q=Deriv(P);Q.resize(n),Q=Quo(Q,P);
		return Q.resize(n-1),Integ(Q);
	}
	template<bool op>pair<poly,poly>Expi(poly P){
		const int pn=P.size();
		const int m=2<<__lg(max(1,pn-1));
		P.resize(m);
		poly Q({1}),H({1}),dQ({1}),nF,nQ,dH,dnF;
		Q.reserve(m),H.reserve(m),dQ.reserve(m);
		for(int n=1;n<m;n*=2){
			nF=Deriv(Slice(P,0,n)),dnF=nF,dnF.resize(n),DFT(dnF),nQ.resize(n);
			for(int i=0;i<n;++i)nQ[i]=(LL)dnF[i]*dQ[i]%Mod;
			IDFT(nQ),nQ=Sub(Deriv(Q),nQ),nQ.insert(nQ.begin(),n,0),swap(nQ[n-1],nQ[n*2-1]);
			DFT(nQ),dH=H,dH.resize(n*2),DFT(dH);
			for(int i=0;i<n*2;++i)nQ[i]=(LL)nQ[i]*dH[i]%Mod;
			IDFT(nQ),copy(nF.begin(),nF.end(),nQ.begin());
			nQ=Sub(Integ(nQ),Slice(P,0,n*2)),nQ.resize(n*2),DFT(nQ);
			dQ.insert(dQ.end(),Q.begin(),Q.end()),rDFT(dQ.begin()+n,n);
			for(int i=0;i<n*2;++i)nQ[i]=(LL)(Mod-nQ[i])*dQ[i]%Mod;
			IDFT(nQ),Q.insert(Q.end(),nQ.begin()+n,nQ.end());
			if(!op&&n*2==m)break;
			dQ=Q,DFT(dQ);
			for(int i=0;i<n*2;++i)nQ[i]=(LL)(Mod-dQ[i])*dH[i]%Mod;
			IDFT(nQ),fill_n(nQ.begin(),n,0),DFT(nQ);
			for(int i=0;i<n*2;++i)nQ[i]=(LL)nQ[i]*dH[i]%Mod;
			IDFT(nQ),H.insert(H.end(),nQ.begin()+n,nQ.end());
		}
		return Q.resize(pn),H.resize(pn),make_pair(Q,H);
	}
	poly Exp(poly P){
		const int pn=P.size();
		if(pn<=64)return Expi<0>(P).first;
		const int BL=max(1,__lg(pn)),m=2<<__lg(max(1,(pn-1)/BL)),L=(pn-1)/m+1;
		auto [Q,H]=Expi<1>(Slice(P,0,m));
		H.resize(m*2),DFT(H);
		vector<poly>A(L),B(L-1);
		P.resize(m*L),Init(m*(L+1));
		for(int i=0;i<pn;++i)P[i]=(LL)P[i]*i%Mod;
		A[0]=poly(Slice(P,0,m)),A[0].resize(m*2),DFT(A[0]);
		for(int k=1;k<L;++k){
			A[k]=Slice(P,k*m,(k+1)*m),A[k].resize(m*2),DFT(A[k]);
			B[k-1]=Slice(Q,(k-1)*m,k*m),B[k-1].resize(m*2),DFT(B[k-1]);
			poly C(m*2);
			for(int j=0;j<k;++j){
				for(int i=0;i<m;++i)C[i]=(C[i]+(LL)(A[k-j][i]+A[k-1-j][i])*B[j][i])%Mod;
				for(int i=m;i<m*2;++i)C[i]=(C[i]+(LL)(A[k-j][i]+Mod-A[k-1-j][i])*B[j][i])%Mod;
			}
			IDFT(C),fill_n(C.begin()+m,m,0),DFT(C);
			for(int i=0;i<m*2;++i)C[i]=(LL)C[i]*H[i]%Mod;
			IDFT(C),fill_n(C.begin()+m,m,0);
			for(int i=0;i<m*2;++i)C[i]=(LL)C[i]*inv[m*k+i]%Mod;
			DFT(C);
			for(int i=0;i<m*2;++i)C[i]=(LL)C[i]*B[0][i]%Mod;
			IDFT(C),Q.insert(Q.end(),C.begin(),C.begin()+m);
		}
		return Q.resize(pn),Q;
	}
	poly Qpow(poly P,int k){
		return Exp(Mulx(Ln(P),k));
	}
	poly SafePow(poly P,int k_mod_p,int k_mod_phi,int k_chk_mn){
		const int n=P.size();
		int i=0;while(i<n&&!P[i])++i;
		if(1ll*i*k_chk_mn>=n)return poly(n);
		P=Slice(P,i,i+(n-i*k_chk_mn));
		const int r=P[0];P=Mulx(P,qpow(r,Mod-2));
		P=Qpow(P,k_mod_p),P=Mulx(P,qpow(r,k_mod_phi));
		return P.insert(P.begin(),i*k_chk_mn,0),P;
	}
	template<class T>poly SafePow(poly P,T k){
		return SafePow(P,k%Mod,k%(Mod-1),k<P.size()?k:P.size());
	}
	poly ModPow(poly X,LL k,poly P){
		const int m=P.size();
		if(m==1)return poly();
		poly nP=P;reverse(nP.begin(),nP.end()),nP=Inv(nP);
		const int L=2<<__lg(m*2-1);
		P.resize(L),nP.resize(L),DFT(P),DFT(nP);
		int Xn=X.size();
		X.resize(L),DFT(X);
		const auto MoD=[&](poly F){
			poly nF=F;
			IDFT(nF);
			nF.resize(m*2-3),reverse(nF.begin(),nF.end()),nF.resize(m-2);
			nF.resize(L),DFT(nF);
			for(int i=0;i<L;++i)nF[i]=(LL)nF[i]*nP[i]%Mod;
			IDFT(nF),nF.resize(m-2),reverse(nF.begin(),nF.end());
			nF.resize(L),DFT(nF);
			for(int i=0;i<L;++i)nF[i]=(LL)nF[i]*P[i]%Mod;
			return Sub(F,nF);
		};
		poly Q({1});
		int Qn=Q.size();
		Q.resize(L),DFT(Q);
		while(k){
			if(k&1){
				for(int i=0;i<L;++i)Q[i]=(LL)Q[i]*X[i]%Mod;
				Qn+=Xn-1;
				if(Qn>=m)Q=MoD(Q),Qn=m-1;
			}
			if(k>>=1){
				for(int i=0;i<L;++i)X[i]=(LL)X[i]*X[i]%Mod;
				Xn+=Xn-1;
				if(Xn>=m)X=MoD(X),Xn=m-1;
			}
		}
		return IDFT(Q),Q.resize(m-1),Q;
	}
}
namespace Polynomial{
	poly Shift(poly P,int k){
		if(!k)return P;
		const int n=P.size();
		Init(n);
		for(int i=0;i<n;++i)P[i]=(LL)frc[i]*P[i]%Mod;
		poly Q(n,1);
		for(int i=1;i<n;++i)Q[i]=(LL)Q[i-1]*k%Mod*inv[i]%Mod;
		P.resize(n+n-1),Q=MulT(P,Q);
		for(int i=0;i<n;++i)Q[i]=(LL)Q[i]*ivf[i]%Mod;
		return Q;
	}
	poly Multi(poly P,int k){
		const uLL v=trans(k);
		const int n=P.size();
		for(int i=0,w=1;i<n;++i,w=mul(w,v))P[i]=(LL)P[i]*w%Mod;
		return P;
	}
	poly Comp_solve(poly&P,poly&Q,int d,int n,int v){
		if(n==1){
			poly H(d+1);
			for(int i=0,w=1;i<=d;++i)
				H[i]=(LL)Binom(d+i-1,d-1)*w%Mod,w=(LL)w*v%Mod;
			H=MulT(P,H);
			return H;
		}
		poly F(d*n*4);
		for(int i=0;i<d;++i)copy_n(Q.begin()+i*n,n,F.begin()+i*n*2);
		F[d*n*2]=1,DFT(F);
		poly H(d*n*2);
		for(int i=0;i<d*n*4;i+=2)H[i/2]=(LL)F[i]*F[i+1]%Mod;
		IDFT(H),--H[0];
		for(int i=1;i<d*2;++i)copy_n(H.begin()+i*n,n/2,H.begin()+i*(n/2));
		H.resize(d*n);
		H=Comp_solve(P,H,d*2,n/2,v);
		poly nH(d*n*2);
		for(int i=0;i<d*2;++i)copy_n(H.begin()+i*(n/2),n/2,nH.begin()+i*n);
		DFT(nH);
		for(int i=0;i<d*n*4;i+=2)swap(F[i],F[i+1]),F[i]=(LL)nH[i/2]*F[i]%Mod,F[i+1]=(LL)nH[i/2]*F[i+1]%Mod;
		IDFT(F);
		for(int i=0;i<d;++i)copy_n(F.begin()+(i+d)*n*2,n,F.begin()+i*n);
		return F.resize(d*n),F;
	}
	poly Comp(poly P,poly Q){
		if(P.empty()||Q.empty())return poly();
		const int pn=P.size(),m=2<<__lg(max(1,pn-1)),v=Q[0];
		P.resize(m*2),Q=Neg(Q),Q.resize(m);
		Q=Comp_solve(P,Q,1,m,v);
		return Q.resize(pn),Q;
	}
	poly Compinv(poly P){
		const int n=P.size(),t=qpow(P[1],Mod-2);
		for(int i=1,w=t;i<n;++i)P[i]=(LL)P[i]*w%Mod,w=(LL)w*t%Mod;
		const int m=2<<__lg(n-1);
		poly Q(m);Q[0]=1,P=Neg(P),P.resize(m);
		P.swap(Q);
		int d=1;
		for(int k=n-1;k;d*=2,k/=2){
			const int L=2<<__lg(d*(k+1)*4-1);
			poly nP(L),nQ(L);
			for(int i=0;i<d;++i)
				copy_n(P.begin()+i*(k+1),k+1,nP.begin()+i*(k+1)*2),
				copy_n(Q.begin()+i*(k+1),k+1,nQ.begin()+i*(k+1)*2);
			nQ[d*(k+1)*2]=1,DFT(nP),DFT(nQ);
			P.resize(L/2),Q.resize(L/2);
			if(k&1)for(int i=0;i<L;i+=2)
				P[i/2]=div2(mul(((LL)nP[i]*nQ[i+1]+(LL)(Mod-nP[i+1])*nQ[i])%Mod,iGrt[i/2])),Q[i/2]=(LL)nQ[i]*nQ[i+1]%Mod;
			else for(int i=0;i<L;i+=2)
				P[i/2]=div2(((LL)nP[i]*nQ[i+1]+(LL)nP[i+1]*nQ[i])%Mod),Q[i/2]=(LL)nQ[i]*nQ[i+1]%Mod;
			IDFT(P),IDFT(Q);
			if(d*(k+1)*4>=L)--Q[d*(k+1)*4%L];
			for(int i=1;i<d*2;++i)
				copy_n(P.begin()+i*(k+1),k/2+1,P.begin()+i*(k/2+1)),
				copy_n(Q.begin()+i*(k+1),k/2+1,Q.begin()+i*(k/2+1));
			P.resize(d*2*(k/2+1)),Q.resize(d*2*(k/2+1));
		}
		Init(n);
		Q=P,reverse(Q.begin(),Q.end()),Q.resize(n);
		for(int i=1;i<n;++i)Q[i]=Q[i]*(n-1ll)%Mod*inv[i]%Mod;
		reverse(Q.begin(),Q.end()),Q=Mulx(Qpow(Q,Mod-inv[n-1]),t),Q.insert(Q.begin(),0);
		return Q.resize(n),Q;
	}
}
using namespace BasicMath;
using namespace Polynomial;
signed main(){
	cin.tie(0)->sync_with_stdio(0);
	int n;read(n);poly P(n);
	for(int&i:P)read(i);
	for(int i:Compinv(P))write(i,' ');
	return 0;
}
/*
*/