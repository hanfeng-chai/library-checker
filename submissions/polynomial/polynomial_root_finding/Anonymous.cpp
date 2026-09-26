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
namespace NimProduct{
    unsigned short E[65536*2+7],L[65536];
    inline void __attribute__((constructor))init(){
        unsigned short c[]={10279,15417,35722,52687,44124,62628,15661,5686,3862,1323,334,647,61560,20636,4267,8445};
        unsigned short d[65536];
        for(int i=0;i<16;++i)for(int j=1<<i;j--;)d[1<<i|j]=d[j]^c[i];
        E[0]=1;
        for(int i=0;i<65534;++i)E[i+1]=d[E[i]];
        memcpy(E+65535,E,131070),memcpy(E+131070,E,14);
        for(int i=0;i<65535;++i)L[E[i]]=i;
    }
    inline uint mul16(const uint&x,const uint&y){return x&&y?E[uint(L[x])+L[y]]:0;}
    inline uint mul16_15(const uint&x,const uint&y){return x&&y?E[uint(L[x])+L[y]+3]:0;}
    inline uint mul15_15(const uint&x,const uint&y){return x&&y?E[uint(L[x])+L[y]+6]:0;}
    inline uint mul15(const uint&x){return x?E[L[x]+3]:0;}
    inline uint mul(const uint&x,const uint&y){
        const uint xh=x>>16,xl=x&65535,yh=y>>16,yl=y&65535,c=mul16(xl,yl);
        return (uLL)(mul16(xl^xh,yl^yh)^c)<<16|(mul16_15(xh,yh)^c);
    }
    inline uint mul32_31(const uint&x,const uint&y){
        const uint xh=x>>16,xl=x&65535,yh=y>>16,yl=y&65535,c=mul16_15(xl^xh,yl^yh);
        return (uLL)(c^mul15_15(xh,yh))<<16|mul15(mul16_15(xl,yl)^c);
    }
    inline uLL mul(const uLL&x,const uLL&y){
        const uint xh=x>>32,xl=x&UINT_MAX,yh=y>>32,yl=y&UINT_MAX,c=mul(xl,yl);
        return (uLL)(mul(xl^xh,yl^yh)^c)<<32|(mul32_31(xh,yh)^c);
    }
}
namespace Polynomial{
#include<bits/stdc++.h>
#define LL long long
#define LLL __int128
#define uint unsigned
#define ldb long double
#define uLL unsigned long long
using namespace std;
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
inline int neg(const int&x){
	return x?Mod-x:0;
}
inline int div2(const int&x){
    return x&1?(x+Mod)>>1:x>>1;
}
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
poly PointShift(poly,int,int);
poly Div(poly,poly);
pair<poly,poly>DivMod(poly,poly);
poly ModPow(poly,LL,poly);
Mat2 Hgcd(poly,poly,int);
poly Gcd(poly,poly);
poly Qpow(poly,int);
}
namespace Polynomial{
poly Div(poly P,poly Q){
    const int n=P.size(),m=Q.size();
    if(n<m)return poly();
	if(n-m+1>m*2&&m<=16){
		poly H(n-m+1);
		vector<uLL>nQ(m-1);
		for(int i=0;i<m-1;++i)nQ[i]=trans(Q[i]);
		const uLL v=trans(qpow(Q.back(),Mod-2));
		for(int i=n-m+1;i--;){
			H[i]=mul(P[i+m-1],v);
			for(int j=max(0,m-1-i);j<m-1;++j)P[i+j]=sub(P[i+j],mul(H[i],nQ[j]));
		}
		return H;
	}
	reverse(P.begin(),P.end());
	reverse(Q.begin(),Q.end());
    P.resize(n-m+1),Q.resize(n-m+1);
	P=Quo(P,Q),reverse(P.begin(),P.end());
	return P;
}
pair<poly,poly>DivMod(poly P,poly Q){
    const int n=P.size(),m=Q.size();
    if(n<m)return make_pair(poly(),P);
	if(min(m,n-m+1)<=64){
		poly H(n-m+1);
		vector<uLL>nQ(m-1);
		for(int i=0;i<m-1;++i)nQ[i]=trans(Q[i]);
		const uLL v=trans(qpow(Q.back(),Mod-2));
		for(int i=n-m+1;i--;){
			H[i]=mul(P[i+m-1],v);
			for(int j=0;j<m-1;++j)P[i+j]=sub(P[i+j],mul(H[i],nQ[j]));
		}
		return P.resize(m-1),Empty(P),make_pair(H,P);
	}
    poly H=Div(P,Q),R=H;
	const int q=2<<__lg(max(1,m-2));
	Reduce(P,q),Reduce(Q,q),Reduce(R,q),DFT(Q),DFT(R);
	for(int i=0;i<q;++i)Q[i]=(LL)Q[i]*R[i]%Mod;
	return IDFT(Q),make_pair(H,Sub_Empty(P,Q));
}
}
namespace Polynomial{
poly Direv(poly P){
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
	poly Q=Direv(P);Q.resize(n),Q=Quo(Q,P);
	return Q.resize(n-1),Integ(Q);
}
template<bool op>pair<poly,poly>Expi(poly P){
	const int pn=P.size();
	const int m=2<<__lg(max(1,pn-1));
	P.resize(m);
	poly Q({1}),H({1}),dQ({1}),nF,nQ,dH,dnF;
	Q.reserve(m),H.reserve(m),dQ.reserve(m);
	for(int n=1;n<m;n*=2){
		nF=Direv(Slice(P,0,n)),dnF=nF,dnF.resize(n),DFT(dnF),nQ.resize(n);
		for(int i=0;i<n;++i)nQ[i]=(LL)dnF[i]*dQ[i]%Mod;
		IDFT(nQ),nQ=Sub(Direv(Q),nQ),nQ.insert(nQ.begin(),n,0),swap(nQ[n-1],nQ[n*2-1]);
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
inline Mat2 operator*(const Mat2&x,const Mat2&y){
	return Mat2(
		Add_Empty(Mul(get<0>(x),get<0>(y)),Mul(get<1>(x),get<2>(y))),
		Add_Empty(Mul(get<0>(x),get<1>(y)),Mul(get<1>(x),get<3>(y))),
		Add_Empty(Mul(get<2>(x),get<0>(y)),Mul(get<3>(x),get<2>(y))),
		Add_Empty(Mul(get<2>(x),get<1>(y)),Mul(get<3>(x),get<3>(y)))
	);
}
pair<Mat2,Mat2>Hgcdi(poly P,poly Q,int d,int L){
	const int n=P.size()-1,m=Q.size()-1;
	const auto calc=[&](Mat2 A){
		Mat2 B=A;
		Reduce(get<0>(B),L),DFT(get<0>(B));
		Reduce(get<1>(B),L),DFT(get<1>(B));
		Reduce(get<2>(B),L),DFT(get<2>(B));
		Reduce(get<3>(B),L),DFT(get<3>(B));
		return make_pair(A,B);
	};
	if(m<n-d)return calc(Mat2(poly({1}),poly(),poly(),poly({1})));
	if(d==1)return calc(Mat2(poly(),poly({1}),poly({1}),Neg(Div(poly(P.begin()+n-2,P.end()),poly(Q.begin()+n-2,Q.end())))));
	const int h=L/2;
	if(d<=h){
		auto [A,B]=Hgcdi(P,Q,d,h);poly C;
		C=get<0>(A);for(int i=h;i<C.size();++i)C[i&(h-1)]=sub(C[i&(h-1)],C[i]);C.resize(h),rDFT(C),get<0>(B).insert(get<0>(B).end(),C.begin(),C.end());
		C=get<1>(A);for(int i=h;i<C.size();++i)C[i&(h-1)]=sub(C[i&(h-1)],C[i]);C.resize(h),rDFT(C),get<1>(B).insert(get<1>(B).end(),C.begin(),C.end());
		C=get<2>(A);for(int i=h;i<C.size();++i)C[i&(h-1)]=sub(C[i&(h-1)],C[i]);C.resize(h),rDFT(C),get<2>(B).insert(get<2>(B).end(),C.begin(),C.end());
		C=get<3>(A);for(int i=h;i<C.size();++i)C[i&(h-1)]=sub(C[i&(h-1)],C[i]);C.resize(h),rDFT(C),get<3>(B).insert(get<3>(B).end(),C.begin(),C.end());
		return make_pair(A,B);
	}
	const int sx=max(0,n-h-h);
	auto [A,B]=Hgcdi(poly(P.begin()+sx,P.end()),poly(Q.begin()+sx,Q.end()),h,L);
	const int t=h-get<3>(A).size()+1;
	poly Px=Slice(P,n-h+t-L,n-h+t),Py=Slice(P,n-h-h-L,n-h-h);
	poly Qx=Slice(Q,n-h+t-L,n-h+t),Qy=Slice(Q,n-h-h-L,n-h-h);
	DFT(Px),DFT(Qx),DFT(Py),DFT(Qy);
	for(int i=0,x,y;i<L;++i){
		x=((LL)Px[i]*get<0>(B)[i]+(LL)Qx[i]*get<1>(B)[i])%Mod;
		y=((LL)Px[i]*get<2>(B)[i]+(LL)Qx[i]*get<3>(B)[i])%Mod;
		Px[i]=x,Qx[i]=y;
		x=((LL)Py[i]*get<0>(B)[i]+(LL)Qy[i]*get<1>(B)[i])%Mod;
		y=((LL)Py[i]*get<2>(B)[i]+(LL)Qy[i]*get<3>(B)[i])%Mod;
		Py[i]=x,Qy[i]=y;
	}
	IDFT(Px),IDFT(Qx),IDFT(Py),IDFT(Qy);
	poly Pz(Py.end()-h-t,Py.end()),Qz(Qy.end()-h-t,Qy.end());
	Pz.insert(Pz.end(),Px.end()-h-t,Px.end());
	Qz.insert(Qz.end(),Qx.end()-h-t,Qx.end()),Empty(Qz);
	int v=0;
	for(int i=0,j=n-h+t;i<=j;++i)
		v=(v+(LL)(i<P.size()?P[i]:0)*(j-i<get<0>(A).size()?get<0>(A)[j-i]:0)+(LL)(i<Q.size()?Q[i]:0)*(j-i<get<1>(A).size()?get<1>(A)[j-i]:0))%Mod;
	Pz.emplace_back(v);
	if(Qz.size()<=h*3+t-d)return make_pair(A,B);
	int hh=d-get<3>(A).size()+1,w=get<3>(A).back(),k=get<3>(A).size()-1;
	if(t>0){
		const int r=max(0,h*3+t-n);
		auto [C,D]=DivMod(poly(Pz.begin()+r,Pz.end()),poly(Qz.begin()+r,Qz.end()));
		w=(LL)w*(Mod-C.back())%Mod,k+=C.size()-1,hh-=C.size()-1;
		Reduce(C,L),DFT(C);
		for(int i=0;i<L;++i){
			get<0>(B)[i]=(get<0>(B)[i]+(LL)(Mod-C[i])*get<2>(B)[i])%Mod;
			get<1>(B)[i]=(get<1>(B)[i]+(LL)(Mod-C[i])*get<3>(B)[i])%Mod;
		}
		swap(get<0>(B),get<2>(B)),swap(get<1>(B),get<3>(B));
		Pz.swap(Qz),Qz.swap(D),Qz.insert(Qz.begin(),r,0);
	}
	const int sy=max(0,h*3+t-d-hh);
	auto [C,D]=Hgcdi(poly(Pz.begin()+sy,Pz.end()),(sy<=Qz.size()?poly(Qz.begin()+sy,Qz.end()):poly()),hh,L);
	for(int i=0;i<L;++i){
		int a0=((LL)get<0>(D)[i]*get<0>(B)[i]+(LL)get<1>(D)[i]*get<2>(B)[i])%Mod;
		int a1=((LL)get<0>(D)[i]*get<1>(B)[i]+(LL)get<1>(D)[i]*get<3>(B)[i])%Mod;
		int a2=((LL)get<2>(D)[i]*get<0>(B)[i]+(LL)get<3>(D)[i]*get<2>(B)[i])%Mod;
		int a3=((LL)get<2>(D)[i]*get<1>(B)[i]+(LL)get<3>(D)[i]*get<3>(B)[i])%Mod;
		get<0>(B)[i]=a0,get<1>(B)[i]=a1,get<2>(B)[i]=a2,get<3>(B)[i]=a3;
	}
	D=B;
	IDFT(get<0>(D)),get<0>(D).resize(d),Empty(get<0>(D));
	IDFT(get<1>(D)),get<1>(D).resize(d),Empty(get<1>(D));
	IDFT(get<2>(D)),get<2>(D).resize(d),Empty(get<2>(D));
	IDFT(get<3>(D));
	k+=get<3>(C).size()-1;
	if(k==L){
		get<3>(D).resize(d+1);
		const int x=(LL)w*get<3>(C).back()%Mod;
		get<3>(D)[d]=x,get<3>(D)[0]=sub(get<3>(D)[0],x);
	}
	Empty(get<3>(D));
	return make_pair(D,B);
}
Mat2 Hgcd(poly P,poly Q,int d){
    if(P.size()==Q.size()){
        const auto [X,Y]=DivMod(P,Q);
		const auto [A,B]=Hgcdi(Q,Y,d,2<<__lg(max(1,d-1)));
        return make_tuple(get<2>(A),get<3>(A),Sub_Empty(get<0>(A),Mul(get<2>(A),X)),Sub_Empty(get<1>(A),Mul(get<3>(A),X)));
    }
	else if(P.size()<Q.size()){
		const auto [A,B]=Hgcdi(Q,P,d,2<<__lg(max(1,d-1)));
		return make_tuple(get<1>(A),get<0>(A),get<3>(A),get<2>(A));
	}
	else return Hgcdi(P,Q,d,2<<__lg(max(1,d-1))).first;
}
tuple<poly,poly,poly>Exgcd(poly P,poly Q){
	Empty(P),Empty(Q);
    const Mat2 M=Hgcd(P,Q,max(P.size(),Q.size())-1);
    const poly H=Add_Empty(Mul(get<0>(M),P),Mul(get<1>(M),Q));
    const int v=qpow(H.back(),Mod-2);
    return make_tuple(Mulx(get<0>(M),v),Mulx(get<1>(M),v),Mulx(H,v));
}
poly Gcd(poly P,poly Q){
	Empty(P),Empty(Q);
    const Mat2 M=Hgcd(P,Q,max(P.size(),Q.size())-1);
    const poly H=Add_Empty(Mul(get<0>(M),P),Mul(get<1>(M),Q));
	return Mulx(H,qpow(H.back(),Mod-2));
}
poly Modinv(poly P,poly Q){
	Empty(P),Empty(Q);
	if(P.size()>=Q.size())P=DivMod(P,Q).second;
    const Mat2 M=Hgcd(Q,P,Q.size()-1);
    const poly H=Add_Empty(Mul(get<0>(M),Q),Mul(get<1>(M),P));
	if(H.size()>1)return poly({-1});
	return Mulx(get<1>(M),qpow(H.back(),Mod-2));
}
}
namespace Polynomial{
poly BerlekampMassey(poly P){
    const int n=P.size();
    reverse(P.begin(),P.end()),Empty(P);
    poly Q(n+1);Q[n]=1;
    Mat2 M=Hgcd(Q,P,n/2);
    const poly C=Add_Empty(Mul(get<0>(M),Q),Mul(get<1>(M),P));
    const poly D=Add_Empty(Mul(get<2>(M),Q),Mul(get<3>(M),P));
    if((int)(D.size()+C.size())>n+1)M=Mat2({poly(),poly({1}),poly({1}),Neg(Div(C,D))})*M;
    Q=get<3>(M),Q=Mulx(Q,qpow(Mod-Q.back(),Mod-2));
    Q.pop_back(),reverse(Q.begin(),Q.end());
    return Q;
}
void FindRoot_solve(poly P,poly&W){
	if(P.size()==1)return;
	if(P.size()==2){
		W.emplace_back(qpow(P[1],Mod-2,Mod-P[0]));
		return;
	}
	poly Q({uniform_int_distribution<int>(0,Mod-1)(rng),1});
	Q=ModPow(Q,Mod>>1,P);
	if(Empty(Q))return FindRoot_solve(P,W);
	Q[0]=sub(Q[0],1);
	Q=Gcd(P,Q),P=Div(P,Q);
	FindRoot_solve(P,W),FindRoot_solve(Q,W);
}
poly FindRoot(poly P){
	Empty(P);
	if(P.size()<2)return poly();
	poly Q=ModPow(poly({0,1}),Mod,P);
	if(Q.size()<2)Q.resize(2);
    Q[1]=sub(Q[1],1);
	P=Gcd(P,Q);
	poly W;
	FindRoot_solve(P,W);
	sort(W.begin(),W.end());
	return W;
}
}
using namespace Polynomial;
signed main(){
	cin.tie(0)->sync_with_stdio(0);
	int n;read(n),++n;poly P(n);
	for(int&i:P)read(i);
	poly Q=FindRoot(P);
	println(Q.size());
	for(int i:Q)write(i,' ');
	return 0;
}
/*
*/