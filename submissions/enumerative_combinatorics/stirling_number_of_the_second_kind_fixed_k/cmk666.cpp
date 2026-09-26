/*  _              _     _                                             _       __      __      __   
   / \     _   _  | |_  | |__     ___    _ __   _    ___   _ __ ___   | | __  / /_    / /_    / /_  
  / _ \   | | | | | __| | '_ \   / _ \  | '__| (_)  / __| | '_ ` _ \  | |/ / | '_ \  | '_ \  | '_ \ 
 / ___ \  | |_| | | |_  | | | | | (_) | | |     _  | (__  | | | | | | |   <  | (_) | | (_) | | (_) |
/_/   \_\  \__,_|  \__| |_| |_|  \___/  |_|    (_)  \___| |_| |_| |_| |_|\_\  \___/   \___/   \___/ 
[Created Time:       2024-10-07 14:25:04]
[Last Modified Time: 2024-10-18 10:38:49] */
#pragma GCC optimize("Ofast", "unroll-loops")
#include<bits/stdc++.h>
#ifdef LOCAL
#include"debug.h"
#else
#define D(...) ((void)0)
#endif
using namespace std; using ll = long long;
#define For(i, j, k) for ( int i = (j) ; i <= (k) ; i++ )
#define Fol(i, j, k) for ( int i = (j) ; i >= (k) ; i-- )
namespace FastIO
{
#define USE_FastIO
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
template < auto P_ > class MontgomeryModInt
{
	using S = decltype(P_); static_assert(is_same_v < S, int > || is_same_v < S, long long >);
	static_assert(P_ & 1 && 0 < P_ && P_ < ( (S)1 << ( sizeof(S) * 8 - 2 ) ));
	using U = conditional_t < is_same_v < S, int >, unsigned, unsigned long long >; using D = conditional_t < is_same_v < S, int >, unsigned long long, __uint128_t >;
	inline constexpr static U uinv(U x) { U y = x; for ( int i = is_same_v < S, int > ? 4 : 5 ; i-- ; ) y *= 2 - x * y; return y; }
	constexpr static U P = P_, P2 = P << 1, R = -uinv(P), R2 = -(D)P % P; static_assert(P * R == -1);
	inline constexpr static U reduce(D x) { return ( x + (U)x * R * (D)P ) >> ( sizeof(U) * 8 ); }
	inline constexpr MontgomeryModInt(U x, int) : v(x) {} U v;
public:
	inline constexpr static S mod() { return P; }
	inline constexpr MontgomeryModInt() : v(0) {}
	inline constexpr MontgomeryModInt(const MontgomeryModInt &x) : v(x.v) {}
	template < class T, enable_if_t < numeric_limits < T >::is_integer, int > = 0 > inline constexpr MontgomeryModInt(T x) : v(reduce((D)R2 * ( numeric_limits < T >::is_signed && x < 0 ? ( ( x + P < 0 ) && ( x %= P ), x + P ) : ( ( sizeof(T) > sizeof(U) && x >= (T)1 << sizeof(U) ) && ( x %= P ), x ) ))) {}
	inline constexpr S val()const { U x = reduce(v); return ( x - P ) >> ( sizeof(U) * 8 - 1 ) ? x : x - P; }
	template < class T, enable_if_t < numeric_limits < T >::is_integer, int > = 0 > explicit inline constexpr operator T()const { return val(); }
	inline constexpr friend bool operator==(const MontgomeryModInt &x, const MontgomeryModInt &y) { return x.val() == y.val(); }
	inline constexpr friend bool operator!=(const MontgomeryModInt &x, const MontgomeryModInt &y) { return x.val() != y.val(); }
	inline constexpr MontgomeryModInt &operator=(const MontgomeryModInt &x) & { v = x.v; return *this; }
	inline constexpr MontgomeryModInt &operator++() & { return *this += 1; }
	inline constexpr MontgomeryModInt operator++(int) & { MontgomeryModInt x = *this; *this += 1; return x; }
	inline constexpr MontgomeryModInt &operator--() & { return *this -= 1; }
	inline constexpr MontgomeryModInt operator--(int) & { MontgomeryModInt x = *this; *this -= 1; return x; }
	inline constexpr MontgomeryModInt operator-()const { return MontgomeryModInt(v ? P2 - v : 0, 0); }
	inline constexpr MontgomeryModInt &operator+=(const MontgomeryModInt &x) & { v += x.v, ( v - P2 ) >> ( sizeof(U) * 8 - 1 ) || ( v -= P2 ); return *this; }
	inline constexpr MontgomeryModInt &operator-=(const MontgomeryModInt &x) & { v -= x.v, v >> ( sizeof(U) * 8 - 1 ) && ( v += P2 ); return *this; }
	inline constexpr MontgomeryModInt &operator*=(const MontgomeryModInt &x) & { v = reduce((D)v * x.v); return *this; }
	inline constexpr MontgomeryModInt &operator/=(const MontgomeryModInt &x) & { return *this *= x.inv(); }
	inline constexpr friend MontgomeryModInt operator+(MontgomeryModInt x, const MontgomeryModInt &y) { return x += y; }
	inline constexpr friend MontgomeryModInt operator-(MontgomeryModInt x, const MontgomeryModInt &y) { return x -= y; }
	inline constexpr friend MontgomeryModInt operator*(MontgomeryModInt x, const MontgomeryModInt &y) { return x *= y; }
	inline constexpr friend MontgomeryModInt operator/(MontgomeryModInt x, const MontgomeryModInt &y) { return x /= y; }
	template < class T, enable_if_t < numeric_limits < T >::is_integer, int > = 0 > inline constexpr MontgomeryModInt qpow(T y)const { MontgomeryModInt x = *this, z = 1; while ( y ) { if ( y & 1 ) z *= x; if ( y >>= 1 ) x *= x; } return z; }
	template < class T, enable_if_t < numeric_limits < T >::is_integer, int > = 0 > inline constexpr friend MontgomeryModInt qpow(const MontgomeryModInt &x, T y) { return x.qpow(y); }
	inline constexpr MontgomeryModInt inv()const { return qpow(P - 2); }
	inline constexpr friend MontgomeryModInt inv(const MontgomeryModInt &x) { return x.inv(); }
	inline friend istream &operator>>(istream &is, MontgomeryModInt &x) { S y; is >> y, x = y; return is; }
	inline friend ostream &operator<<(ostream &os, const MontgomeryModInt &x) { return os << x.val(); }
#ifdef USE_FastIO
	inline void read() & { S x; ::read(x), *this = x; }
	inline void write()const { ::write(val()); }
#endif
};	using MI = MontgomeryModInt < 998244353 >; // 1000000007 1145141919810000037
template < class MI > struct VALUES
{
	static vector < MI > inv, fac, ifac;
	inline static void extend(int n)
	{
		if ( __builtin_expect((int)inv.size() > n, true) ) return;
		if ( __builtin_expect(inv.empty(), false) ) inv = { 0, 1 }, fac = ifac = { 1, 1 };
		int m = (int)inv.size(); inv.resize(n + 1), fac.resize(n + 1), ifac.resize(n + 1);
		for ( int i = m ; i != n + 1 ; i++ ) inv[i] = MI::mod() / i * -inv[MI::mod() % i],
								  fac[i] = fac[i - 1] * i, ifac[i] = ifac[i - 1] * inv[i];
	}
};	template < class MI > vector < MI > VALUES < MI >::inv;
	template < class MI > vector < MI > VALUES < MI >::fac;
	template < class MI > vector < MI > VALUES < MI >::ifac;
// int VALUES_INIT = [] { return VALUES < MI >::extend(1000000), 0; } ();
inline MI  fac(int x) { return x >= 0 ? VALUES < MI >::extend(max(1, x)), VALUES < MI >:: fac[x] : 0; }
inline MI ifac(int x) { return x >= 0 ? VALUES < MI >::extend(max(1, x)), VALUES < MI >::ifac[x] : 0; }
inline MI c(int x, int y) { return x >= y && y >= 0 ? fac(x) * ifac(y) * ifac(x - y) : 0; }
template < class IT1, class IT2 > inline void linear_inv(int n, IT1 p, IT2 q)
{
	*q = *p; for ( int i = 1 ; i != n ; i++ ) q[i] = q[i - 1] * p[i]; q[n - 1] = q[n - 1].inv();
	for ( int i = n - 1 ; i ; i-- ) { MI x = q[i]; q[i] *= q[i - 1], q[i - 1] = x * p[i]; }
}
template < class MI > inline pair < bool, MI > cipolla(const MI &s)
{
	if ( !s ) return make_pair(true, 0);
	if ( MI::mod() == 2 ) return make_pair(true, 1);
	if ( s.qpow(( MI::mod() - 1 ) >> 1) != 1 ) return make_pair(false, 0);
	static mt19937 rnd(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution < decltype(MI::mod()) > uid(1, MI::mod() - 1);
	MI x, y; do x = uid(rnd), y = x * x - s; while ( y.qpow(( MI::mod() - 1 ) >> 1) == 1 );
	constexpr auto mul = [](pair < MI, MI > &f, const pair < MI, MI > &g, const MI &h) {
		f = make_pair(f.first * g.first  + f.second * g.second * h,
					  f.first * g.second + f.second * g.first);
	}; pair < MI, MI > f(x, 1), g(1, 0); auto z = ( MI::mod() + 1 ) >> 1;
	while ( z ) { if ( z & 1 ) mul(g, f, y); if ( z >>= 1 ) mul(f, f, y); }
	return make_pair(true, min(g.first.val(), MI::mod() - g.first.val()));
}
template < class MI > class Polynomial : public vector < MI >
{
	constexpr static MI G = 3; constexpr static int L = [] { int x = MI::mod() - 1, y = 0; while ( ~x & 1 ) x >>= 1, y++; return y; } ();
	constexpr static array < MI, L + 1 > ivn = [] { array < MI, L + 1 > a{}; for ( int i = 0 ; i != L + 1 ; i++ ) a[i] = MI(1 << i).inv(); return a; } ();
	inline static pair < const Polynomial *, const Polynomial * > get_rt(int n) { static Polynomial rt = { 1 }, irt = { 1 }; if ( rt.size() < n ) { int i = rt.size(); for ( rt.resize(n), irt.resize(n) ; i != n ; i <<= 1 ) { MI w = G.qpow(MI::mod() / ( i << 2 )), iw = w.inv(); for ( int j = 0 ; j != i ; j++ ) rt[j + i] = rt[j] * w, irt[j + i] = irt[j] * iw; } } return make_pair(&rt, &irt); }
	inline constexpr static int get_len(int x) { return x < 3 ? x : 2 << __lg(x - 1); }
	inline void dif(int n) { const Polynomial &rt = *get_rt(n).first; for ( int i = n ; i >>= 1 ; ) for ( int j = 0, k = 0 ; j != n ; j += i << 1, k++ ) for ( int p = j, q = j + i ; p != j + i ; p++, q++ ) { MI x = (*this)[p], y = (*this)[q] * rt[k]; (*this)[p] = x + y, (*this)[q] = x - y; } }
	inline void dit(int n) { const Polynomial &irt = *get_rt(n).second; for ( int i = 1 ; i != n ; i <<= 1 ) for ( int j = 0, k = 0 ; j != n ; j += i << 1, k++ ) for ( int p = j, q = j + i ; p != j + i ; p++, q++ ) { MI x = (*this)[p], y = (*this)[q]; (*this)[p] = x + y, (*this)[q] = ( x - y ) * irt[k]; } MI x = ivn[__lg(n)]; for ( int i = 0 ; i != n ; i++ ) (*this)[i] *= x; }
	inline void dif_rhalf(int n) { const Polynomial &rt = *get_rt(n).first; for ( int i = n, m = 1 ; i >>= 1 ; m <<= 1 ) for ( int j = 0, k = 0 ; j != n ; j += i << 1, k++ ) for ( int p = j, q = j + i ; p != j + i ; p++, q++ ) { MI x = (*this)[p], y = (*this)[q] * rt[m + k]; (*this)[p] = x + y, (*this)[q] = x - y; } }
	inline void dit_rhalf(int n) { const Polynomial &irt = *get_rt(n).second; for ( int i = 1, m = n ; m >>= 1 ; i <<= 1 ) for ( int j = 0, k = 0 ; j != n ; j += i << 1, k++ ) for ( int p = j, q = j + i ; p != j + i ; p++, q++ ) { MI x = (*this)[p], y = (*this)[q]; (*this)[p] = x + y, (*this)[q] = ( x - y ) * irt[m + k]; } MI x = ivn[__lg(n)]; for ( int i = 0 ; i != n ; i++ ) (*this)[i] *= x; }
	inline void dot(const Polynomial &x, int n) { for ( int i = 0 ; i != n ; i++ ) (*this)[i] *= x[i]; }
	inline void deriv(int n, const typename vector < MI >::iterator &x)const { for ( int i = 1 ; i != n ; i++ ) x[i - 1] = (*this)[i] * i; x[n - 1] = 0; }
	inline void integ(int n, const typename vector < MI >::iterator &x)const { VALUES < MI >::extend(n); for ( int i = n - 1 ; i ; i-- ) x[i] = (*this)[i - 1] * VALUES < MI >::inv[i]; x[0] = 0; }
	inline static Polynomial mul_big(Polynomial x, Polynomial y) { int n = x.size() + y.size() - 1, m = get_len(n); x.resize(m), x.dif(m), y.resize(m), y.dif(m), x.dot(y, m), x.dit(m), x.resize(n); return x; }
	inline Polynomial pow_one(int n, int k)const { if ( !k ) { Polynomial x(n); x[0] = 1; return x; } return k == 1 ? modxk(n) : ( modxk(n).ln() * k ).exp(); }
	inline Polynomial pow_non_zero(int n, int k_mod_p, int k_mod_phi_p)const { return (*this)[0] == 1 ? pow_one(n, k_mod_p) : ( *this * (*this)[0].inv() ).pow_one(n, k_mod_p) * (*this)[0].qpow(k_mod_phi_p); }
	inline Polynomial sqrt_non_zero(const MI &t)const { constexpr MI c = -MI(2).inv(); int n = get_len(size()); Polynomial x(size()), y(size()), z(n), u(n), v(n); x[0] = v[0] = t, y[0] = t.inv(); for ( int i = 1, j = 2, k ; i != size() ; j <<= 1 ) { k = j == n ? size() : j, v.dot(v, i), v.dit(i); for ( int l = 0 ; l != i ; l++ ) v[i + l] = v[l] - (*this)[l]; for ( int l = i ; l != k ; l++ ) v[l] -= (*this)[l]; fill(v.begin(), v.begin() + i, 0), v.dif(j), copy(y.begin(), y.begin() + i, u.begin()), u.dif(j), v.dot(u, j), v.dit(j); for ( int l = i ; l != k ; l++ ) x[l] = v[l] * c; if ( j == n ) i = size(); else for ( copy(x.begin(), x.begin() + j, z.begin()), z.dif(j), copy(z.begin(), z.begin() + j, v.begin()), z.dot(u, j), z.dit(j), fill(z.begin(), z.begin() + i, 0), z.dif(j), z.dot(u, j), z.dit(j) ; i != j ; i++ ) y[i] = -z[i]; } return x; }
	struct eval_tree
	{
		int n, m, s; vector < Polynomial > t;
		inline eval_tree(int o, const Polynomial &x) : n(max(2, get_len(o))), m(__lg(n)), s(x.size()), t(m + 1, Polynomial(n << 1)) { Polynomial z(n); for ( int i = 0 ; i != n ; i++ ) { MI w = i < s ? x[i] : 0; t[0][i << 1] = 1 - w, t[0][i << 1 | 1] = -1 - w; } for ( int d = 0, i = 2 ; d != m ; d++, i <<= 1 ) for ( int j = 0 ; j != ( n << 1 ) ; j += i << 1 ) { for ( int k = 0 ; k != i ; k++ ) z[k] = t[d][j + k] * t[d][j + i + k]; if ( d != m - 1 ) copy(z.begin(), z.begin() + i, t[d + 1].begin() + j), z.dit(i), z[0] -= 2, z.dif_rhalf(i), copy(z.begin(), z.begin() + i, t[d + 1].begin() + j + i); else z.dit(i), copy(z.begin(), z.begin() + i, t[d + 1].begin() + j), --t[d + 1][j], ++t[d + 1][i]; } }
		inline Polynomial eval(Polynomial x)const { if ( x.size() > n ) x = x.divmod(t[m]).second; Polynomial y(s), z(n), u = t[m]; x.resize(n << 1), rotate(x.begin(), x.end() - 1, x.end()), x.dif(n << 1), reverse(u.begin(), u.begin() + 1 + n), u = u.modxk(n).inv(); reverse(u.begin(), u.end()), u.resize(n << 1), u.dif(n << 1), u.dot(x, n << 1); for ( int d = m, i = n ; d-- ; i >>= 1 ) for ( int j = 0 ; j != ( n << 1 ) ; j += i << 1 ) { copy(u.begin() + i + j, u.begin() + ( i << 1 ) + j, z.begin()), z.dit_rhalf(i), z.dif(i), copy(z.begin(), z.begin() + i, u.begin() + i + j); for ( int k = 0 ; k != i ; k++ ) { MI w = u[j + k] - u[i + j + k]; u[j + k] = w * t[d][i + j + k], u[i + j + k] = w * t[d][j + k]; } } MI w = ivn[m + 1]; for ( int i = 0 ; i != s ; i++ ) y[i] = ( u[i << 1] - u[i << 1 | 1] ) * w; return y; }
		inline Polynomial inter(const Polynomial &y)const { Polynomial x = t[m], z(n), u(n << 1), v(n << 1); x.erase(x.begin(), x.begin() + n - s), x = eval(x.deriv()), linear_inv(s, x.begin(), v.begin()); for ( int i = 0 ; i != n ; i++ ) u[i << 1] = u[i << 1 | 1] = i < s ? y[i] * v[i] : 0; for ( int d = 0, i = 2 ; d != m ; d++, i <<= 1, swap(u, v) ) for ( int j = 0 ; j != ( n << 1 ) ; j += i << 1 ) { for ( int k = 0 ; k != i ; k++ ) z[k] = t[d][j + k] * u[j + i + k] + u[j + k] * t[d][j + i + k]; if ( d != m - 1 ) copy(z.begin(), z.begin() + i, v.begin() + j), z.dit(i), z.dif_rhalf(i), copy(z.begin(), z.begin() + i, v.begin() + j + i); else z.dit(i), copy(z.begin(), z.begin() + i, v.begin()); } return Polynomial(u.begin() + n - s, u.begin() + n); }
	};
	inline static void shift_poi_non_zero(const Polynomial &y, const MI &k, int m, typename vector < MI >::iterator &z) { int n = y.size(), o = get_len(n + m); Polynomial u(o), v(o), w(n + m), x(n + m); MI p = k - n; VALUES < MI >::extend(max(1, n - 1)); for ( int i = 0 ; i != n ; i++ ) u[i] = ( ( n - i ) & 1 ? y[i] : -y[i] ) * VALUES < MI >::ifac[i] * VALUES < MI >::ifac[n - 1 - i]; u.dif(o); w[0] = 1; for ( int i = 1 ; i != n + m ; i++ ) w[i] = w[i - 1] * ++p; linear_inv(n + m, w.begin(), x.begin()); for ( int i = 0 ; i != n + m - 1 ; i++ ) v[i] = w[i] * x[i + 1]; v.dif(o), u.dot(v, o), u.dit(o); for ( int i = 0 ; i != m ; i++ ) *z++ = u[i + n - 1] * w[i + n] * x[i]; }
	inline void comp_work(Polynomial &x, int o, int m, int n, const MI &w)const { if ( n == 1 ) { x.resize(m + 1), VALUES < MI >::extend(m + m - 1); MI p = VALUES < MI >::ifac[m - 1]; for ( int i = 0 ; i != m + 1 ; i++ ) x[m - i] = p * VALUES < MI >::fac[m - 1 + i] * VALUES < MI >::ifac[i], p *= w; x *= *this, x.erase(x.begin(), x.begin() + m), x.resize(o); return; } Polynomial y(o << 2), z(o << 1), u(o << 1); for ( int i = 0 ; i != m ; i++ ) copy(x.begin() + i * n, x.begin() + i * n + n, y.begin() + ( ( i * n ) << 1 )); y[o << 1] = 1, y.dif(o << 2); for ( int i = 0 ; i != ( o << 2 ) ; i += 2 ) z[i >> 1] = y[i] * y[i + 1]; z.dit(o << 1), --z[0]; for ( int i = 1 ; i != ( m << 1 ) ; i++ ) copy(z.begin() + i * n, z.begin() + i * n + ( n >> 1 ), z.begin() + i * ( n >> 1 )); z.resize(o), comp_work(z, o, m << 1, n >> 1, w); for ( int i = 0 ; i != ( m << 1 ) ; i++ ) copy(z.begin() + i * ( n >> 1 ), z.begin() + i * ( n >> 1 ) + ( n >> 1 ), u.begin() + i * n); u.dif(o << 1), x.resize(o << 2); for ( int i = 0 ; i != ( o << 2 ) ; i += 2 ) x[i] = y[i + 1] * u[i >> 1], x[i + 1] = y[i] * u[i >> 1]; x.dit(o << 2); for ( int i = 0 ; i != m ; i++ ) copy(x.begin() + ( ( ( i + m ) * n ) << 1 ), x.begin() + ( ( ( i + m ) * n ) << 1 ) + n, x.begin() + i * n); x.resize(o); }
public:
	using vector < MI >::vector, vector < MI >::begin, vector < MI >::end;
	inline int size()const { return (int)vector < MI >::size(); }
	inline Polynomial mulxk(int k)const { Polynomial x = *this; x.insert(x.begin(), k, 0); return x; }
	inline Polynomial divxk(int k)const { return Polynomial(begin() + min(k, size()), end()); }
	inline Polynomial modxk(int k)const { return Polynomial(begin(), begin() + min(k, size())); }
	inline void remove0() & { while ( size() && !vector < MI >::back() ) vector < MI >::pop_back(); }
	inline Polynomial mul_xi(const MI &x)const { Polynomial z = *this; if ( size() ) { MI y = 1; for ( int i = 1 ; i != size() ; i++ ) z[i] *= y *= x; } return z; }
	inline MI operator()(const MI &x)const { MI y = vector < MI >::back(); for ( int i = size() - 2 ; ~i ; i-- ) y = y * x + (*this)[i]; return y; }
	inline Polynomial operator-()const { Polynomial x = *this; for ( MI &i : x ) i = -i; return x; }
	inline Polynomial &operator+=(const MI &x) & { if ( !size() ) return Polynomial(1,  x); (*this)[0] += x; return *this; }
	inline Polynomial &operator-=(const MI &x) & { if ( !size() ) return Polynomial(1, -x); (*this)[0] -= x; return *this; }
	inline Polynomial &operator*=(const MI &x) & { for ( MI &i : *this ) i *= x; return *this; }
	inline Polynomial &operator/=(const MI &x) & { return *this *= x.inv(); }
	inline Polynomial &operator+=(const Polynomial &x) & { if ( size() < x.size() ) vector < MI >::resize(x.size()); for ( int i = 0 ; i != x.size() ; i++ ) (*this)[i] += x[i]; return *this; }
	inline Polynomial &operator-=(const Polynomial &x) & { if ( size() < x.size() ) vector < MI >::resize(x.size()); for ( int i = 0 ; i != x.size() ; i++ ) (*this)[i] -= x[i]; return *this; }
	inline Polynomial &operator*=(const Polynomial &x) & { return *this = *this * x; }
	inline friend Polynomial operator+(Polynomial x, const MI &y) { return x += y; }
	inline friend Polynomial operator-(Polynomial x, const MI &y) { return x -= y; }
	inline friend Polynomial operator*(Polynomial x, const MI &y) { return x *= y; }
	inline friend Polynomial operator/(Polynomial x, const MI &y) { return x /= y; }
	inline friend Polynomial operator+(Polynomial x, const Polynomial &y) { return x += y; }
	inline friend Polynomial operator-(Polynomial x, const Polynomial &y) { return x -= y; }
	inline friend Polynomial operator*(const Polynomial &x, const Polynomial &y) { if ( min(x.size(), y.size()) > 8 && max(x.size(), y.size()) > 128 ) return mul_big(x, y); Polynomial z(max(0, x.size() + y.size() - 1)); for ( int i = 0 ; i != x.size() ; i++ ) for ( int j = 0 ; j != y.size() ; j++ ) z[i + j] += x[i] * y[j]; return z; }
	inline Polynomial deriv()const { Polynomial x(size()); deriv(size(), x.begin()), x.pop_back(); return x; }
	inline Polynomial integ()const { Polynomial x(size() + 1); integ(size() + 1, x.begin()); return x; }
	inline Polynomial inv()const { int n = get_len(size()); Polynomial x(size()), y(n), z(n); x[0] = (*this)[0].inv(); for ( int i = 1, j = 2, k ; i != size() ; j <<= 1 ) for ( k = j == n ? size() : j, copy(begin(), begin() + k, y.begin()), y.dif(j), copy(x.begin(), x.begin() + i, z.begin()), z.dif(j), y.dot(z, j), y.dit(j), fill(y.begin(), y.begin() + i, 0), y.dif(j), y.dot(z, j), y.dit(j) ; i != k ; i++ ) x[i] = -y[i]; return x; }
	inline Polynomial exp()const { if ( size() == 1 ) return Polynomial(1, 1); int n = get_len(size()); Polynomial x(size()), y(size()), z(n), u(n), v(n); x[0] = y[0] = u[0] = u[1] = 1; for ( int i = 1, j = 2, k ; i != size() ; j <<= 1 ) { k = j == n ? size() : j, deriv(i, z.begin()), z.dif(i), z.dot(u, i), z.dit(i), z[i - 1] = -z[i - 1], x.deriv(i, z.begin() + i); for ( int l = 0 ; l != i - 1 ; l++ ) z[i + l] -= z[l]; fill(z.begin(), z.begin() + i - 1, 0), z.dif(j), copy(y.begin(), y.begin() + i, v.begin()), v.dif(j), z.dot(v, j); z.dit(j), z.integ(j, z.begin()), fill(z.begin(), z.begin() + i, 0); for ( int l = i ; l != k ; l++ ) z[l] -= (*this)[l]; z.dif(j), z.dot(u, j), z.dit(j); for ( int l = i ; l != k ; l++ ) x[l] = -z[l]; if ( j == n ) i = size(); else { copy(x.begin(), x.begin() + j, u.begin()), u.dif(j + j); for ( copy(u.begin(), u.begin() + j, z.begin()), z.dot(v, j), z.dit(j), fill(z.begin(), z.begin() + i, 0), z.dif(j), z.dot(v, j), z.dit(j) ; i != j ; i++ ) y[i] = -z[i]; } } return x; }
	inline Polynomial ln()const { Polynomial x(size()); deriv(size(), x.begin()), x *= inv(), x.resize(size()), x.integ(size(), x.begin()); return x; }
	inline Polynomial pow(int k_chkmn_sz, int k_mod_p, int k_mod_phi_p)const { if ( !k_chkmn_sz ) { Polynomial x(size()); x[0] = 1; return x; } int i = 0; while ( i != size() && !(*this)[i] ) i++; return (long long)i * k_chkmn_sz >= size() ? Polynomial(size()) : i ? divxk(i).pow_non_zero(size() - i * k_chkmn_sz, k_mod_p, k_mod_phi_p).mulxk(i * k_chkmn_sz) : pow_non_zero(size(), k_mod_p, k_mod_phi_p); }
	template < class T > inline Polynomial pow(T x)const { return pow(x < size() ? x : size(), x % MI::mod(), x % ( MI::mod() - 1 )); }
	inline pair < bool, Polynomial > sqrt()const { int i = 0; while ( i != size() && !(*this)[i] ) i++; if ( i == size() ) return make_pair(true, Polynomial(size())); if ( i & 1 ) return make_pair(false, Polynomial()); auto [o, t] = cipolla((*this)[i]); if ( !o ) return make_pair(false, Polynomial()); if ( i ) { Polynomial x(begin() + i, end()); x.insert(x.end(), i >> 1, 0); return make_pair(true, x.sqrt_non_zero(t).mulxk(i >> 1)); } return make_pair(true, sqrt_non_zero(t)); }
	inline Polynomial div(const Polynomial &x)const { if ( x.size() > size() ) return Polynomial(); int n = size() - x.size() + 1; Polynomial y(n), z(n); reverse_copy(begin() + x.size() - 1, end(), y.begin()), reverse_copy(x.begin() + max(0, x.size() - n), x.end(), z.begin()), y = ( y * z.inv() ).modxk(n), reverse(y.begin(), y.end()); return y; }
	inline pair < Polynomial, Polynomial > divmod(const Polynomial &x)const { if ( x.size() > size() ) return make_pair(Polynomial(), *this); Polynomial y = div(x); return make_pair(y, modxk(x.size() - 1) - ( x.modxk(x.size() - 1) * y.modxk(x.size() - 1) ).modxk(x.size() - 1)); }
	inline Polynomial eval(const Polynomial &x)const { return eval_tree(max(size(), x.size()), x).eval(*this); }
	inline static Polynomial inter(const Polynomial &x, const Polynomial &y) { return eval_tree(x.size(), x).inter(y); }
	inline Polynomial chirpz_eval(int m, const MI &x)const { Polynomial y(m); if ( !x ) { y[0] = (*this)(1); if ( m > 1 ) fill(y.begin() + 1, y.end(), (*this)[0]); return y; } int n = size() + m - 1, o = get_len(n); Polynomial u(o), v(n), z = *this; MI w = x.inv(), p = u[0] = 1, q = v[0] = 1; for ( int i = 1 ; i != n ; i++ ) u[i] = u[i - 1] * p, v[i] = v[i - 1] * q, p *= x, q *= w; z.dot(v, size()), reverse(z.begin(), z.end()), z.resize(o), z.dif(o), u.dif(o), z.dot(u, o), z.dit(o), z = Polynomial(z.begin() + size() - 1, z.begin() + size() - 1 + m), z.dot(v, m); return z; }
	inline static Polynomial chirpz_inter(const MI &x, const Polynomial &y) { int n = y.size(), m = get_len(n << 1); if ( n < 2 ) return y; Polynomial u(( n << 1 ) - 1), v(n), z(m), r(n), s(m); MI w = x.inv(), p = u[0] = z[0] = 1, q = v[0] = 1; for ( int i = 1 ; i != n ; i++ ) u[i] = u[i - 1] * p, v[i] = v[i - 1] * q, p *= x, q *= w, z[i] = z[i - 1] * ( 1 - p ); q = z[n - 1] * ( 1 - p * x ); for ( int i = n ; i != ( n << 1 ) - 1 ; i++ ) u[i] = u[i - 1] * p, p *= x; linear_inv(n, z.begin(), r.begin()); for ( int i = 0 ; i != n ; i++ ) z[i] = ( i & 1 ? -y[i] : y[i] ) * r[i] * r[n - 1 - i] * u[n - 1 - i] * v[i] * v[n - 1]; reverse(z.begin(), z.begin() + n), z.dif(m), copy(u.begin(), u.end(), s.begin()), s.dif(m), z.dot(s, m), z.dit(m), z.erase(z.begin(), z.begin() + n - 1), z.dot(v, n), z[n + 1] = 0, z.resize(m), z.dif(m), s[0] = 1; for ( int i = 1 ; i != n ; i++ ) s[i] = ( i & 1 ? -q : q ) * r[i] * r[n - i] * u[i]; fill(s.begin() + n, s.end(), 0), s.dif(m), z.dot(s, m), z.dit(m), z.resize(n), reverse(z.begin(), z.end()); return z; }
	inline Polynomial shift(const MI &k) & { if ( !k ) return *this; int n = size(), m = get_len(n << 1); Polynomial y(m), z(m); VALUES < MI >::extend(max(1, n - 1)); for ( int i = 0 ; i != n ; i++ ) y[i] = (*this)[i] * VALUES < MI >::fac[i]; y.dif(m), z[n - 1] = 1; for ( int i = n - 1 ; i ; i-- ) z[i - 1] = z[i] * k * VALUES < MI >::inv[n - i]; z.dif(m), y.dot(z, m), y.dit(m), y = Polynomial(y.begin() + n - 1, y.begin() + ( n << 1 ) - 1); for ( int i = 0 ; i != n ; i++ ) y[i] *= VALUES < MI >::ifac[i]; return y; }
	inline static Polynomial shift_poi(const Polynomial &y, MI k, int m) { Polynomial x(m); auto z = x.begin(); int p = k.val(), o = MI::mod() - p; if ( o <= m ) shift_poi_non_zero(y, k, o, z), m -= o, k = p = 0; if ( m && p < y.size() ) o = min(y.size(), p + m), copy(y.begin() + p, y.begin() + o, z), z += o - p, m -= o - p, k = y.size(); if ( m ) shift_poi_non_zero(y, k, m, z); return x; }
	inline Polynomial comp(const Polynomial &x)const { int n = get_len(size()); Polynomial y(n); for ( int i = 0 ; i != x.size() ; i++ ) y[i] = -x[i]; comp_work(y, n, 1, n, x[0]), y.resize(size()); return y; }
	inline Polynomial comp_inv()const { constexpr MI c = MI(2).inv(); int n = get_len(size() << 1), m, o, j = 1; const Polynomial &irt = *get_rt(n).second; Polynomial x(n), y(n), u(n << 1), v(n << 1); MI w = (*this)[1].inv(), p = -1; for ( int i = 1 ; i != size() ; i++ ) p *= w, x[i] = (*this)[i] * p; y[0] = 1; for ( int i = size() ; i != 1 ; i = o, j <<= 1 ) { o = ( i + 1 ) >> 1, m = get_len(( i * j ) << 2), fill(u.begin(), u.begin() + m, 0), fill(v.begin(), v.begin() + m, 0); for ( int k = 0 ; k != j ; k++ ) copy(x.begin() + i * k, x.begin() + i * k + i, u.begin() + ( ( i * k ) << 1 )), copy(y.begin() + i * k, y.begin() + i * k + i, v.begin() + ( ( i * k ) << 1 )); u[( i * j ) << 1] = 1, u.dif(m), v.dif(m), x.resize(m >> 1), y.resize(m >> 1); for ( int k = 0 ; k != m ; k += 2 ) x[k >> 1] = u[k] * u[k + 1]; if ( i & 1 ) for ( int k = 0 ; k != m ; k += 2 ) y[k >> 1] = ( u[k + 1] * v[k] + u[k] * v[k + 1] ) * c; else for ( int k = 0 ; k != m ; k += 2 ) y[k >> 1] = ( u[k + 1] * v[k] - u[k] * v[k + 1] ) * irt[k >> 1] * c; x.dit(m >> 1), y.dit(m >> 1); if ( ( ( i * j ) << 2 ) == m ) --x[0]; for ( int k = 1 ; k != ( j << 1 ) ; k++ ) copy(x.begin() + i * k, x.begin() + i * k + o, x.begin() + o * k), copy(y.begin() + i * k, y.begin() + i * k + o, y.begin() + o * k); } y.resize(j * o), reverse(y.begin(), y.end()), m = size(), y.resize(m), VALUES < MI >::extend(m - 1); for ( int i = 1 ; i != m ; i++ ) y[i] *= ( m - 1 ) * VALUES < MI >::inv[i]; reverse(y.begin(), y.end()), y = y.pow_one(m - 1, ( -VALUES < MI >::inv[m - 1] ).val()) * w, y.insert(y.begin(), 0); return y; }
#ifdef USE_FastIO
	inline void read() & { for ( MI &i : *this ) ::read(i); }
	inline void write()const { for ( int i = 0 ; i != size() ; i++ ) { if ( i ) ::write(' '); ::write((*this)[i]); } }
#endif
};	using Poly = Polynomial < MI >;
inline Poly second_stirling_col(int n, int k)
{
	if ( !k ) { Poly f(n + 1); return f[0] = 1, f; } Poly f = { -1, 1 };
	for ( int i = __lg(k) ; i-- ; )
		f *= f.shift(-( k >> ( i + 1 ) )), ( k >> i ) & 1 && ( f *= Poly({ -( k >> i ), 1 }), 0 );
	return reverse(f.begin(), f.end()), f.resize(n - k + 1), f.inv();
}
int main() { int n, k; read(n, k); println(second_stirling_col(n, k)); return 0; }
// 想上GM捏 想上GM捏 想上GM捏 想上GM捏 想上GM捏
// 伊娜可爱捏 伊娜贴贴捏