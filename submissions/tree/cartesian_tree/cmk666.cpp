/*  _              _     _                                             _       __      __      __   
   / \     _   _  | |_  | |__     ___    _ __   _    ___   _ __ ___   | | __  / /_    / /_    / /_  
  / _ \   | | | | | __| | '_ \   / _ \  | '__| (_)  / __| | '_ ` _ \  | |/ / | '_ \  | '_ \  | '_ \ 
 / ___ \  | |_| | | |_  | | | | | (_) | | |     _  | (__  | | | | | | |   <  | (_) | | (_) | | (_) |
/_/   \_\  \__,_|  \__| |_| |_|  \___/  |_|    (_)  \___| |_| |_| |_| |_|\_\  \___/   \___/   \___/ 
[Created Time:       2024-10-07 14:25:04]
[Last Modified Time: 2024-10-22 15:03:15] */
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
template < class IT, class F > inline int cartesian_tree(int n, IT a, int *lc, int *rc, F &&f)
{
	static int stk[1000009]; int tp = 0, k;
	For(i, 1, n)
	{
		for ( k = tp ; k && f(a[i], a[stk[k]]) ; k-- );
		if ( k ) rc[stk[k]] = i;
		if ( k < tp ) lc[i] = stk[k + 1];
		stk[tp = ++k] = i;
	}
	return stk[1];
}
int n, a[1000009], rt, lc[1000009], rc[1000009], fa[1000009];
int main()
{
	read(n);
	For(i, 1, n) read(a[i]);
	rt = cartesian_tree(n, a, lc, rc, less()), fa[rt] = rt;
	For(i, 1, n) fa[lc[i]] = fa[rc[i]] = i;
	For(i, 1, n) write(fa[i] - 1, i == n ? '\n' : ' ');
	return 0;
}
// 想上GM捏 想上GM捏 想上GM捏 想上GM捏 想上GM捏
// 伊娜可爱捏 伊娜贴贴捏