
#ifdef LOCAL
#include<bits/stdc++.h>
#include"debug.h"
#else
#pragma GCC optimize("Ofast", "unroll-loops")
#include<bits/stdc++.h>
#pragma GCC target("avx2")
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
#include<immintrin.h>
namespace POLY_AVX2
{
	using Z = unsigned; using X = unsigned long long; using I = __m256i;
	constexpr Z N = 1 << 17;

	constexpr inline Z len(const Z &x) { return x < 3 ? x : 2 << __lg(x - 1); }
	constexpr inline Z Zinv(const Z &x) { Z y = x; for ( Z i = 4 ; i-- ; ) y *= 2 - x * y; return y; }
	
	constexpr Z P = 998244353, PP = P << 1, PPP = P * 3, R = -Zinv(P), RR = -(X)P % P;
	constexpr inline Z reduce(const X &x) { return ( x + (Z)x * R * (X)P ) >> 32; }
	constexpr inline Z fix(const Z &x) { return x < P ? x : x - P; }
	constexpr inline Z mul(const Z &x, const Z &y) { return reduce((X)x * y); }
	constexpr inline Z in(const Z &x) { return mul(x, RR); }
	constexpr inline Z out(const Z &x) { return fix(reduce(x)); }
	constexpr inline Z add(Z x, const Z &y) { return x += y, x < PP ? x : x - PP; }
	constexpr inline Z sub(Z x, const Z &y) { return x -= y, x < PP ? x : x + PP; }
	constexpr inline Z neg(const Z &x) { return PP - x; }
	constexpr inline Z div2(const Z &x) { return ( x & 1 ? x + P : x ) >> 1; }
	constexpr inline Z negdiv2(const Z &x) { return ( x & 1 ? PPP - x : PP - x ) >> 1; }
	constexpr inline Z mul_cube(const Z &x, const Z &y) { return mul(mul(x, y), mul(y, y)); }
	constexpr Z one = in(1), two = in(2), G = in(3), negone = neg(one);
	constexpr inline Z qpow(Z x, Z y, Z z = one) { while ( y ) { if ( y & 1 ) z = mul(z, x); if ( y >>= 1 ) x = mul(x, x); } return z; }
	
	mt19937 rnd(chrono::system_clock::now().time_since_epoch().count()); uniform_int_distribution < Z > uid(1, P - 1);
	inline pair < Z, Z > pmul(const pair < Z, Z > &x, const pair < Z, Z > &y, const Z &z) { return make_pair(fix(reduce((X)x.first * y.first + (X)mul(x.second, y.second) * z)), reduce((X)x.first * y.second + (X)x.second * y.first)); }
	inline Z cipolla(const Z &x) { if ( !x ) return 0; if ( fix(qpow(x, P >> 1)) != one ) return ~0u; Z y, z; do y = uid(rnd), z = sub(mul(y, y), x); while ( fix(qpow(z, P >> 1)) == one ); z = fix(z); pair < Z, Z > p(y, one), q(one, 0); Z o = ( P + 1 ) >> 1; while ( o ) { if ( o & 1 ) q = pmul(q, p, z); if ( o >>= 1 ) p = pmul(p, p, z); } return y = out(q.first), in(min(y, P - y)); }

	inline void inv_n(const Z *x, const Z &n, Z *y) { *y = *x; for ( Z i = 1 ; i != n ; i++ ) y[i] = mul(y[i - 1], x[i]); y[n - 1] = qpow(y[n - 1], P - 2); for ( Z i = n - 1, z ; i ; i-- ) z = y[i], y[i] = mul(z, y[i - 1]), y[i - 1] = mul(z, x[i]); }

	inline I& cast(Z *p) { return *(I *)p; }
	inline const I& cast(const Z *p) { return *(I *)p; }
	constexpr inline I make_4(const X &x) { return __builtin_bit_cast(I, (__attribute__((vector_size(32))) X){x, x, x, x}); }
	constexpr inline I make_8(const Z &x) { return __builtin_bit_cast(I, (__attribute__((vector_size(32))) Z){x, x, x, x, x, x, x, x}); }
	constexpr inline I make_8(const Z &x0, const Z &x1, const Z &x2, const Z &x3, const Z &x4, const Z &x5, const Z &x6, const Z &x7) { return __builtin_bit_cast(I, (__attribute__((vector_size(32))) Z){x0, x1, x2, x3, x4, x5, x6, x7}); }
	constexpr I zero8 = make_8(0), one8 = make_8(1), MSK_LO = make_4(~0u), P4 = make_4(P), P8 = make_8(P), PP_PPP = make_8(PP, PPP, 0, 0, 0, 0, 0, 0), PP8 = make_8(PP), R4 = make_4(R), RR8 = make_8(RR);
	inline I reduce_unshift(const I &x) { return _mm256_add_epi64(x, _mm256_mul_epu32(_mm256_mul_epu32(x, R4), P4)); }
	inline I reduce_shift(const I &x) { return _mm256_srli_epi64(reduce_unshift(x), 32); }
	inline I reduce(const I &x) { return _mm256_blend_epi32(reduce_shift(_mm256_and_si256(x, MSK_LO)), reduce_unshift(_mm256_srli_epi64(x, 32)), 170); }
	inline I fix(const I &x) { return _mm256_min_epu32(x, _mm256_sub_epi32(x, P8)); }
	inline I mul(const I &x, const I &y) { return _mm256_blend_epi32(reduce_shift(_mm256_mul_epu32(x, y)), reduce_unshift(_mm256_mul_epu32(_mm256_srli_epi64(x, 32), _mm256_srli_epi64(y, 32))), 170); }
	inline I mulx(const I &x, const I &y) { return _mm256_blend_epi32(reduce_shift(_mm256_mul_epu32(x, y)), reduce_unshift(_mm256_mul_epu32(_mm256_srli_epi64(x, 32), y)), 170); }
	inline I in(const I &x) { return mulx(x, RR8); }
	inline I out(const I &x) { return fix(reduce(x)); }
	inline I add(I x, const I &y) { return x = _mm256_add_epi32(x, y), _mm256_min_epu32(x, _mm256_sub_epi32(x, PP8)); }
	inline I sub(I x, const I &y) { return x = _mm256_sub_epi32(x, y), _mm256_min_epu32(x, _mm256_add_epi32(x, PP8)); }
	inline I neg(const I &x) { return _mm256_sub_epi32(PP8, x); }
	inline I negdiv2(const I &x) { return _mm256_srli_epi32(_mm256_sub_epi32(_mm256_permutevar8x32_epi32(PP_PPP, _mm256_and_si256(x, one8)), x), 1); }
	inline I mul_cube(const I &x, const I &y) { return mul(mul(x, y), mul(y, y)); }

	inline void copy_n(const Z *a, const Z &n, Z *b) { memcpy(b, a, n << 2); }
	inline void zero_n(Z *a, const Z &n) { memset(a, 0, n << 2); }
	inline void copy_zero_n(const Z *a, const Z &n, Z *b, const Z &m) { copy_n(a, n, b), zero_n(b + n, m - n); }

#define op_n_1(op) \
	inline void op##_n(Z *a, const Z &n) { Z i = 0; for ( ; i + 7 < n ; i += 8 ) cast(a + i) = op(cast(a + i)); for ( ; i != n ; i++ ) a[i] = op(a[i]); } \
	inline void op##_n(const Z *a, const Z &n, Z *c) { Z i = 0; for ( ; i + 7 < n ; i += 8 ) cast(c + i) = op(cast(a + i)); for ( ; i != n ; i++ ) c[i] = op(a[i]); }
#define op_n_2(op) \
	inline void op##_n(Z *a, const Z *b, const Z &n) { Z i = 0; for ( ; i + 7 < n ; i += 8 ) cast(a + i) = op(cast(a + i), cast(b + i)); for ( ; i != n ; i++ ) a[i] = op(a[i], b[i]); } \
	inline void op##_n(const Z *a, const Z *b, const Z &n, Z *c) { Z i = 0; for ( ; i + 7 < n ; i += 8 ) cast(c + i) = op(cast(a + i), cast(b + i)); for ( ; i != n ; i++ ) c[i] = op(a[i], b[i]); }
#define opx_n_2(op) \
	inline void op##_n(Z *a, const Z &b, const Z &n) { I x = make_8(b); Z i = 0; for ( ; i + 7 < n ; i += 8 ) cast(a + i) = op##x(cast(a + i), x); for ( ; i != n ; i++ ) a[i] = op(a[i], b); } \
	inline void op##_n(const Z *a, const Z &b, const Z &n, Z *c) { I x = make_8(b); Z i = 0; for ( ; i + 7 < n ; i += 8 ) cast(c + i) = op##x(cast(a + i), x); for ( ; i != n ; i++ ) c[i] = op(a[i], b); }
	op_n_1(in) op_n_1(out) op_n_1(fix) op_n_1(neg) op_n_1(negdiv2)
	op_n_2(add) op_n_2(sub) op_n_2(mul) op_n_2(mul_cube)
	opx_n_2(mul)
#undef op_n_1
#undef op_n_2
#undef opx_n_2

	Z nnum; alignas(32) Z num[N << 1], inum[N << 1], fac[N << 1], ifac[N << 1];
	inline void extend_num(const Z &n) { if ( nnum >= n ) [[likely]] return; if ( !nnum ) [[unlikely]] nnum = 2, num[1] = inum[1] = *fac = fac[1] = *ifac = ifac[1] = one; for ( ; nnum < n ; nnum++ ) num[nnum] = in(nnum), inum[nnum] = mul(in(P - P / nnum), inum[P % nnum]), fac[nnum] = mul(fac[nnum - 1], num[nnum]), ifac[nnum] = mul(ifac[nnum - 1], inum[nnum]); }
	
	inline void deriv(const Z *a, const Z &n, Z *b) { extend_num(n); Z i = 1; if ( n > 7 ) [[likely]] { for ( ; i != 8 ; i++ ) b[i - 1] = mul(a[i], num[i]); for ( ; i + 7 < n ; i += 8 ) _mm256_storeu_si256((__m256i_u *)( b + i - 1 ), mul(cast(a + i), cast(num + i))); } for ( ; i != n ; i++ ) b[i - 1] = mul(a[i], num[i]); }
	inline void integ(const Z *a, const Z &n, Z *b) { extend_num(n); Z i = n; if ( n > 7 ) [[likely]] { while ( i & 7 ) i--, b[i] = mul(a[i - 1], inum[i]); while ( i != 8 ) i -= 8, cast(b + i) = mul(_mm256_loadu_si256((const __m256i_u *)( a + i - 1 )), cast(inum + i)); } while ( --i ) b[i] = mul(a[i - 1], inum[i]); *b = 0; }

	Z nrt; alignas(32) Z rt[N], irt[N]; I rt4[N >> 2], irt4[N >> 2], rt2[N >> 2], irt2[N >> 2], rt1[N >> 2], irt1[N >> 2];
	inline void extend_rt(const Z &n) { if ( nrt >= n ) [[likely]] return; if ( !nrt ) [[unlikely]] nrt = 1, *rt = *irt = one; Z m = nrt >> 2; for ( ; nrt < n ; nrt <<= 1 ) { Z w = qpow(G, P / ( nrt << 2 )); mul_n(rt, w, nrt, rt + nrt), mul_n(irt, qpow(w, P - 2), nrt, irt + nrt), fix_n(irt + nrt, nrt); } for ( ; m != ( n >> 2 ) ; m++ ) rt4[m] = make_4(rt[m]), irt4[m] = make_4(irt[m]), rt2[m] = make_8(rt[m << 1], 0, rt[m << 1], 0, rt[m << 1 | 1], 0, rt[m << 1 | 1], 0), irt2[m] = make_8(irt[m << 1], 0, irt[m << 1], 0, irt[m << 1 | 1], 0, irt[m << 1 | 1], 0), rt1[m] = make_8(rt[m << 2], 0, rt[m << 2 | 1], 0, rt[m << 2 | 2], 0, rt[m << 2 | 3], 0), irt1[m] = make_8(irt[m << 2], 0, irt[m << 2 | 1], 0, irt[m << 2 | 2], 0, irt[m << 2 | 3], 0); }

	constexpr I dif_4_idx = make_8(4, 0, 5, 1, 6, 2, 7, 3), dif_2_idx = make_8(4, 0, 6, 2, 5, 1, 7, 3), dit_4_idx = make_8(0, 2, 4, 6, 1, 3, 5, 7), dit_2_idx = make_8(0, 4, 2, 6, 1, 5, 3, 7);
	inline void dif_421(I &x, const Z &k) { x = _mm256_permutevar8x32_epi32(x, dif_4_idx); I y = reduce_unshift(_mm256_mul_epu32(x, rt4[k])); x = _mm256_permutevar8x32_epi32(add(_mm256_blend_epi32(_mm256_srli_epi64(x, 32), x, 170), _mm256_blend_epi32(_mm256_srli_epi64(y, 32), neg(y), 170)), dif_2_idx), y = reduce_unshift(_mm256_mul_epu32(x, rt2[k])), x = _mm256_shuffle_epi32(add(_mm256_blend_epi32(_mm256_srli_epi64(x, 32), x, 170), _mm256_blend_epi32(_mm256_srli_epi64(y, 32), neg(y), 170)), 114), y = reduce_unshift(_mm256_mul_epu32(x, rt1[k])), x = add(_mm256_blend_epi32(_mm256_srli_epi64(x, 32), x, 170), _mm256_blend_epi32(_mm256_srli_epi64(y, 32), neg(y), 170)); }
	inline void dit_124(I &x, const Z &k) { I y = _mm256_srli_epi64(x, 32); x = _mm256_shuffle_epi32(_mm256_blend_epi32(add(x, y), reduce_unshift(_mm256_mul_epu32(_mm256_add_epi32(x, _mm256_sub_epi32(PP8, y)), irt1[k])), 170), 216), y = _mm256_srli_epi64(x, 32), x = _mm256_permutevar8x32_epi32(_mm256_blend_epi32(add(x, y), reduce_unshift(_mm256_mul_epu32(_mm256_add_epi32(x, _mm256_sub_epi32(PP8, y)), irt2[k])), 170), dit_2_idx), y = _mm256_srli_epi64(x, 32), x = _mm256_permutevar8x32_epi32(_mm256_blend_epi32(add(x, y), reduce_unshift(_mm256_mul_epu32(_mm256_add_epi32(x, _mm256_sub_epi32(PP8, y)), irt4[k])), 170), dit_4_idx); }
	inline void dif(Z *a, const Z &n) { if ( n == 1 ) return; extend_rt(n >> 1); if ( n == 2 ) { Z x = *a, y = a[1]; *a = add(x, y), a[1] = sub(x, y); return; } if ( n == 4 ) { Z x = *a, y = a[1], u = a[2], v = a[3]; *a = add(x, u), a[1] = add(y, v), a[2] = sub(x, u), a[3] = sub(y, v), x = *a, y = a[1], u = a[2], v = mul(a[3], rt[1]); *a = add(x, y), a[1] = sub(x, y), a[2] = add(u, v), a[3] = sub(u, v); return; } for ( Z i = n >> 1 ; i != 4 ; i >>= 1 ) { for ( Z p = 0, q = i ; p != i ; p += 8, q += 8 ) { I x = cast(a + p), y = cast(a + q); cast(a + p) = add(x, y), cast(a + q) = sub(x, y); } for ( Z j = i << 1, k = 1 ; j != n ; j += i << 1, k++ ) for ( Z p = j, q = j + i ; p != j + i ; p += 8, q += 8 ) { I x = cast(a + p), y = mulx(cast(a + q), rt4[k]); cast(a + p) = add(x, y), cast(a + q) = sub(x, y); } } for ( Z j = 0, k = 0 ; j != n ; j += 8, k++ ) dif_421(cast(a + j), k); }
	inline void dit(Z *a, const Z &n) { if ( n == 1 ) return; extend_rt(n >> 1); if ( n == 2 ) { Z x = *a, y = a[1]; *a = div2(add(x, y)), a[1] = div2(sub(x, y)); return; } if ( n == 4 ) { Z x = *a, y = a[1], u = a[2], v = a[3]; *a = add(x, y), a[1] = sub(x, y), a[2] = add(u, v), a[3] = mul(u + PP - v, irt[1]), x = *a, y = a[1], u = a[2], v = a[3], *a = div2(div2(add(x, u))), a[1] = div2(div2(add(y, v))), a[2] = div2(div2(sub(x, u))), a[3] = div2(div2(sub(y, v))); return; } for ( Z j = 0, k = 0 ; j != n ; j += 8, k++ ) dit_124(cast(a + j), k); for ( Z i = 8 ; i != n ; i <<= 1 ) { for ( Z p = 0, q = i ; p != i ; p += 8, q += 8 ) { I x = cast(a + p), y = cast(a + q); cast(a + p) = add(x, y), cast(a + q) = sub(x, y); } for ( Z j = i << 1, k = 1 ; j != n ; j += i << 1, k++ ) for ( Z p = j, q = j + i ; p != j + i ; p += 8, q += 8 ) { I x = cast(a + p), y = cast(a + q); cast(a + p) = add(x, y), cast(a + q) = mulx(_mm256_add_epi32(x, _mm256_sub_epi32(PP8, y)), irt4[k]); } } mul_n(a, in(P - ( P - 1 ) / n), n); }
	inline void dif_right_half(Z *a, const Z &n) { if ( n == 1 ) return; extend_rt(n); if ( n == 2 ) { Z x = *a, y = mul(a[1], rt[1]); *a = add(x, y), a[1] = sub(x, y); return; } if ( n == 4 ) { Z x = *a, y = a[1], u = mul(a[2], rt[1]), v = mul(a[3], rt[1]); *a = add(x, u), a[1] = add(y, v), a[2] = sub(x, u), a[3] = sub(y, v), x = *a, y = mul(a[1], rt[2]), u = a[2], v = mul(a[3], rt[3]); *a = add(x, y), a[1] = sub(x, y), a[2] = add(u, v), a[3] = sub(u, v); return; } Z m = 1; for ( Z i = n >> 1 ; i != 4 ; i >>= 1, m <<= 1 ) for ( Z j = 0, k = 0 ; j != n ; j += i << 1, k++ ) for ( Z p = j, q = j + i ; p != j + i ; p += 8, q += 8 ) { I x = cast(a + p), y = mulx(cast(a + q), rt4[m + k]); cast(a + p) = add(x, y), cast(a + q) = sub(x, y); } for ( Z j = 0, k = 0 ; j != n ; j += 8, k++ ) dif_421(cast(a + j), m + k); }
	inline void dit_right_half(Z *a, const Z &n) { if ( n == 1 ) return; extend_rt(n); if ( n == 2 ) { Z x = *a, y = a[1]; *a = div2(add(x, y)), a[1] = div2(mul(x + PP - y, irt[1])); return; } if ( n == 4 ) { Z x = *a, y = a[1], u = a[2], v = a[3]; *a = add(x, y), a[1] = mul(x + PP - y, irt[2]), a[2] = add(u, v), a[3] = mul(u + PP - v, irt[3]), x = *a, y = a[1], u = a[2], v = a[3], *a = div2(div2(add(x, u))), a[1] = div2(div2(add(y, v))), a[2] = div2(div2(mul(x + PP - u, irt[1]))), a[3] = div2(div2(mul(y + PP - v, irt[1]))); return; } Z m = n >> 3; for ( Z j = 0, k = 0 ; j != n ; j += 8, k++ ) dit_124(cast(a + j), m + k); m >>= 1; for ( Z i = 8 ; i != n ; i <<= 1, m >>= 1 ) for ( Z j = 0, k = 0 ; j != n ; j += i << 1, k++ ) for ( Z p = j, q = j + i ; p != j + i ; p += 8, q += 8 ) { I x = cast(a + p), y = cast(a + q); cast(a + p) = add(x, y), cast(a + q) = mulx(_mm256_add_epi32(x, _mm256_sub_epi32(PP8, y)), irt4[m + k]); } mul_n(a, in(P - ( P - 1 ) / n), n); }
	
	inline void inv(const Z *a, const Z &n, Z *b) { const Z m = len(n); alignas(32) Z c[m], d[m]; *b = qpow(*a, P - 2); for ( Z i = 1, j = 2, k ; i != n ; i = k, j <<= 1 ) k = j == m ? n : j, copy_zero_n(a, k, c, j), dif(c, j), copy_zero_n(b, i, d, j), dif(d, j), mul_n(c, d, j), dit(c, j), zero_n(c, i), dif(c, j), mul_n(c, d, j), dit(c, j), neg_n(c + i, k - i, b + i); }
	
	inline void quo(const Z *a, const Z *b, const Z &n, Z *c) { if ( n <= 64 ) { const Z m = len(n << 1); alignas(32) Z d[m], e[m]; copy_zero_n(a, n, d, m), dif(d, m), inv(b, n, e), zero_n(e + n, m - n), dif(e, m), mul_n(d, e, m), dit(d, m), copy_n(d, n, c); return; } const Z m = len(n) >> 4, o = ( n + m - 1 ) / m; alignas(32) Z d[m << 1], e[m << 1], f[o - 1][m << 1], g[o][m << 1]; copy_zero_n(a, m, d, m << 1), dif(d, m << 1), inv(b, m, e), zero_n(e + m, m), dif(e, m << 1), mul_n(d, e, m << 1), dit(d, m << 1), copy_n(d, m, c), copy_zero_n(b, m, *g, m << 1), dif(*g, m << 1); for ( Z i = 1, j = m, k ; j < n ; i++, j += m ) { k = min(m, n - j), copy_zero_n(b + j, k, g[i], m << 1), dif(g[i], m << 1), copy_zero_n(c + j - m, m, f[i - 1], m << 1), dif(f[i - 1], m << 1), fix_n(f[i - 1], m << 1), zero_n(d, m << 1); for ( Z p = 0, q ; p != i ; p++ ) { for ( q = 0 ; q != m ; q += 8 ) cast(d + q) = sub(cast(d + q), mul(cast(f[p] + q), _mm256_add_epi32(cast(g[i - p] + q), cast(g[i - 1 - p] + q)))); for ( ; q != ( m << 1 ) ; q += 8 ) cast(d + q) = sub(cast(d + q), mul(cast(f[p] + q), _mm256_add_epi32(cast(g[i - p] + q), _mm256_sub_epi32(PP8, cast(g[i - 1 - p] + q))))); } dit(d, m << 1), add_n(d, a + j, k), zero_n(d + m, m), dif(d, m << 1), mul_n(d, e, m << 1), dit(d, m << 1), copy_n(d, k, c + j); } }

	inline void ln(const Z *a, const Z &n, Z *b) { alignas(32) Z c[n]; deriv(a, n, b), b[n - 1] = 0, quo(b, a, n, c), integ(c, n, b); }

	inline void integ_righthalf(Z *a, const Z &m, const Z &n) { extend_num(n); if ( n - m > 7 ) [[likely]] for ( Z i = n ; i != m ; ) i -= 8, cast(a + i) = mul(_mm256_loadu_si256((const __m256i_u *)( a + i - 1 )), cast(inum + i)); else for ( Z i = n ; i != m ; ) i--, a[i] = mul(a[i - 1], inum[i]); }
	template < bool calc_inv > inline void exp_impl(const Z *a, const Z &n, Z *b, Z *c) { *b = *c = one; if ( n == 1 ) return; const Z m = len(n); alignas(32) Z d[m], e[m], f[m]; *e = e[1] = one; for ( Z i = 1, j = 2, k ; i != n ; i = k, j <<= 1 ) { k = j == m ? n : j; deriv(a, i, d), d[i - 1] = 0, dif(d, i), mul_n(d, e, i), dit(d, i), d[i - 1] = neg(d[i - 1]), deriv(b, i, d + i), d[i + i - 1] = 0, sub_n(d + i, d, i), zero_n(d, i - 1), dif(d, j), copy_zero_n(c, i, f, j), dif(f, j), mul_n(d, f, j), dit(d, j), integ_righthalf(d, i, j), zero_n(d, i), sub_n(d + i, a + i, k - i), dif(d, j), mul_n(d, e, j), dit(d, j), neg_n(d + i, k - i, b + i); if ( !calc_inv && j == m ) break; copy_zero_n(b, k, e, j << 1), dif(e, j << 1), mul_n(e, f, j, d), dit(d, j), zero_n(d, i), dif(d, j), mul_n(d, f, j), dit(d, j), neg_n(d + i, i, c + i); } }
	inline void exp(const Z *a, const Z &n, Z *b) { if ( n <= 64 ) { alignas(32) Z c[n]; exp_impl<false>(a, n, b, c); return; } const Z m = len(n) >> 4, o = ( n + m - 1 ) / m; alignas(32) Z d[m << 1], e[m << 1], f[o - 1][m << 1], g[o][m << 1]; extend_num(n), exp_impl<true>(a, m, b, e), zero_n(e + m, m), dif(e, m << 1), mul_n(a, num, m, *g), zero_n(*g + m, m), dif(*g, m << 1); for ( Z i = 1, j = m, k ; j < n ; i++, j += m ) { k = min(m, n - j), mul_n(a + j, num + j, k, g[i]), zero_n(g[i] + k, ( m << 1 ) - k), dif(g[i], m << 1), copy_zero_n(b + j - m, m, f[i - 1], m << 1), dif(f[i - 1], m << 1), fix_n(f[i - 1], m << 1), zero_n(d, m << 1); for ( Z p = 0, q ; p != i ; p++ ) { for ( q = 0 ; q != m ; q += 8 ) cast(d + q) = sub(cast(d + q), mul(cast(f[p] + q), _mm256_add_epi32(cast(g[i - p] + q), cast(g[i - 1 - p] + q)))); for ( ; q != ( m << 1 ) ; q += 8 ) cast(d + q) = sub(cast(d + q), mul(cast(f[p] + q), _mm256_add_epi32(cast(g[i - p] + q), _mm256_sub_epi32(PP8, cast(g[i - 1 - p] + q))))); } dit(d, m << 1), zero_n(d + m, m), dif(d, m << 1), mul_n(d, e, m << 1), dit(d, m << 1), neg_n(d, m << 1), mul_n(d, inum + j, k), zero_n(d + k, ( m << 1 ) - k), dif(d, m << 1), mul_n(d, *f, m << 1), dit(d, m << 1), copy_n(d, k, b + j); } }

	template < bool calc_inv > inline void sqrt_impl(const Z *a, const Z &n, Z *b, Z *c, const Z &w) { const Z m = len(n); alignas(32) Z d[m], e[m], f[m]; *b = *f = w, *c = qpow(w, P - 2); for ( Z i = 1, j = 2, k ; i != n ; i = k, j <<= 1 ) { k = j == m ? n : j, mul_n(f, f, i), dit(f, i), sub_n(f, a, i, f + i), zero_n(f, i), sub_n(f + i, a + i, k - i), zero_n(f + k, j - k), dif(f, j), copy_zero_n(c, i, e, j), dif(e, j), mul_n(f, e, j), dit(f, j), negdiv2_n(f + i, k - i, b + i); if ( !calc_inv && j == m ) break; copy_n(b, j, d), dif(d, j), copy_n(d, j, f), mul_n(d, e, j), dit(d, j), zero_n(d, i), dif(d, j), mul_n(d, e, j), dit(d, j), neg_n(d + i, i, c + i); } }
	inline void sqrt_non_zero(const Z *a, const Z &n, Z *b, const Z &w) { if ( n <= 64 ) { alignas(32) Z c[n]; sqrt_impl<false>(a, n, b, c, w); return; } const Z m = len(n) >> 4, o = ( n + m - 1 ) / m; alignas(32) Z d[m << 1], e[m << 1], f[o - 1][m << 1]; sqrt_impl<true>(a, m, b, e, w), zero_n(e + m, m), dif(e, m << 1); for ( Z i = 1, j = m, k ; j < n ; i++, j += m ) { k = min(m, n - j), copy_zero_n(b + j - m, m, f[i - 1], m << 1), dif(f[i - 1], m << 1), fix_n(f[i - 1], m << 1), mul_n(*f, f[i - 1], m << 1, d), neg_n(d, m); for ( Z p = 1, q ; p != i ; p++ ) { for ( q = 0 ; q != m ; q += 8 ) cast(d + q) = sub(cast(d + q), mul(cast(f[p] + q), _mm256_add_epi32(cast(f[i - p] + q), cast(f[i - 1 - p] + q)))); for ( ; q != ( m << 1 ) ; q += 8 ) cast(d + q) = sub(cast(d + q), mul(cast(f[p] + q), _mm256_add_epi32(cast(f[i - p] + q), _mm256_sub_epi32(PP8, cast(f[i - 1 - p] + q))))); } dit(d, m << 1), neg_n(d, m << 1), sub_n(d, a + j, k), zero_n(d + m, m), dif(d, m << 1), mul_n(d, e, m << 1), dit(d, m << 1), negdiv2_n(d, k, b + j); } }
	inline bool sqrt(const Z *a, const Z &n, Z *b) { Z i = 0; while ( i != n && !a[i] ) i++; if ( i == n ) return zero_n(b, n), true; if ( i & 1 ) return false; Z w = cipolla(a[i]); if ( w == ~0u ) return false; if ( i ) { const Z m = n - ( i >> 1 ); alignas(32) Z c[m], d[m]; copy_zero_n(a + i, n - i, c, m), sqrt_non_zero(c, m, d, w), zero_n(b, i >> 1), copy_n(d, m, b + ( i >> 1 )); } else sqrt_non_zero(a, n, b, w); return true; }
	inline void inv_sqrt(const Z *a, const Z &n, Z *b) { const Z m = len(n); alignas(32) Z c[m << 1], d[m << 1]; *b = qpow(cipolla(*a), P - 2); for ( Z i = 1, j = 2, k ; i != n ; i = k, j <<= 1 ) { k = j == m ? n : j, copy_zero_n(a, k, c, j << 1), dif(c, j << 1), copy_zero_n(b, i, d, j << 1), dif(d, j << 1), mul_cube_n(c, d, j << 1), dit(c, j << 1), negdiv2_n(c + i, k - i, b + i); } }

	inline void pow_one(const Z *a, const Z &n, Z *b, const Z &k) { if ( !k ) *b = one, zero_n(b + 1, n - 1); else if ( k == 1 ) copy_n(a, n, b); else { alignas(32) Z c[n]; ln(a, n, c), mul_n(c, in(k), n), exp(c, n, b); } }
	inline void pow_non_zero(const Z *a, const Z &n, Z *b, const Z &k_mod_p, const Z &k_mod_phi_p) { if ( fix(*a) == one ) pow_one(a, n, b, k_mod_p); else { alignas(32) Z c[n]; mul_n(a, qpow(*a, P - 2), n, c), pow_one(c, n, b, k_mod_p), mul_n(b, qpow(*a, k_mod_phi_p), n); } }
	inline void pow(const Z *a, const Z &n, Z *b, const Z &k_chkmn_n, const Z &k_mod_p, const Z &k_mod_phi_p) { if ( !k_chkmn_n ) { *b = one, zero_n(b + 1, n - 1); return; } Z i = 0; while ( i != n && !a[i] ) i++; if ( (X)i * k_chkmn_n >= n ) zero_n(b, n); else if ( i ) { const Z m = n - i * k_chkmn_n; alignas(32) Z c[m], d[m]; copy_n(a + i, m, c), pow_non_zero(c, m, d, k_mod_p, k_mod_phi_p), zero_n(b, i * k_chkmn_n), copy_n(d, m, b + i * k_chkmn_n); } else pow_non_zero(a, n, b, k_mod_p, k_mod_phi_p); }
	inline void pow(const Z *a, const Z &n, Z *b, const Z &k) { return pow(a, n, b, min(n, k), k, k % ( P - 1 )); }

	inline void div(const Z *a, const Z &n, const Z *b, const Z &m, Z *c) { const Z o = n - m + 1, p = min(o, m); alignas(32) Z d[o], e[o]; reverse_copy(a + m - 1, a + n, d), reverse_copy(b + m - p, b + m, e), zero_n(e + p, o - p), quo(d, e, o, c), reverse(c, c + o); }
	inline void divmod(const Z *a, const Z &n, const Z *b, const Z &m, Z *c, Z *d) { div(a, n, b, m, c); const Z o = len(min(n, ( m << 2 ) - 3)), p = m - 1, q = min(p, n - p); alignas(32) Z e[o], f[o]; copy_zero_n(b, p, e, o), dif(e, o), copy_zero_n(c, q, f, o), dif(f, o), mul_n(e, f, o), dit(e, o), sub_n(a, e, p, d); }
	inline void mod(const Z *a, const Z &n, const Z *b, const Z &m, Z *c) { alignas(32) Z d[n - m + 1]; divmod(a, n, b, m, d, c); }

	inline void tree_build(const Z *x, const Z &n, const Z &m, Z *tt) { auto t = (Z (*)[m << 1])tt; const Z o = __lg(m); alignas(32) Z a[m]; for ( Z i = 0, w ; i != m ; i++ ) w = i < n ? x[i] : 0, t[0][i << 1] = sub(one, w), t[0][i << 1 | 1] = sub(negone, w); for ( Z i = 0, j = 2 ; i != o ; i++, j <<= 1 ) { fix_n(t[i], m << 1); for ( Z k = 0 ; k != ( m << 1 ) ; k += j << 1 ) { mul_n(t[i] + k, t[i] + j + k, j, a); if ( i != o - 1 ) copy_n(a, j, t[i + 1] + k), dit(a, j), *a = sub(*a, two), dif_right_half(a, j), copy_n(a, j, t[i + 1] + j + k); else dit(a, j), copy_n(a, j, t[i + 1]), t[i + 1][0] = sub(t[i + 1][0], one), t[i + 1][j] = one; } } }
	inline void tree_eval(const Z *a, const Z &n, const Z &m, const Z *tt, const Z &p, Z *y) { auto t = (Z (*)[m << 1])tt; const Z o = __lg(m); alignas(32) Z b[m << 1], c[m], d[m << 1]; if ( n > m ) mod(a, n, t[o], m << 1, b), b[( m << 1 ) - 1] = 0; else copy_zero_n(a, n, b, m << 1); rotate(b, b + ( m << 1 ) - 1, b + ( m << 1 )), dif(b, m << 1), reverse_copy(t[o] + 1, t[o] + m + 1, c), inv(c, m, d), reverse(d, d + m), zero_n(d + m, m), dif(d, m << 1), mul_n(d, b, m << 1); for ( Z i = o - 1, j = m ; ~i ; i--, j >>= 1 ) for ( Z k = 0, p ; k != ( m << 1 ) ; k += j << 1 ) { dit_right_half(d + j + k, j), dif(d + j + k, j), p = 0; for ( I w ; p + 7 < j ; p += 8 ) w = _mm256_add_epi32(cast(d + k + p), _mm256_sub_epi32(PP8, cast(d + j + k + p))), cast(d + k + p) = mul(w, cast(t[i] + j + k + p)), cast(d + j + k + p) = mul(w, cast(t[i] + k + p)); for ( Z w ; p != j ; p++ ) w = d[k + p] + PP - d[j + k + p], d[k + p] = mul(w, t[i][j + k + p]), d[j + k + p] = mul(w, t[i][k + p]); } for ( Z w = fix(in(P - ( ( P - 1 ) >> ( o + 1 ) ))), i = 0 ; i != p ; i++ ) y[i] = mul(d[i << 1] + PP - d[i << 1 | 1], w); }
	inline void tree_inter(const Z *y, const Z &n, const Z &m, const Z *tt, Z *a) { auto t = (Z (*)[m << 1])tt; const Z o = __lg(m); alignas(32) Z b[m], c[2][m << 1]; Z *d = c[0], *e = c[1]; copy_n(t[o] + m - n, n + 1, d), deriv(d, n + 1, b), tree_eval(b, n, m, tt, n, b), inv_n(b, n, e); for ( Z i = 0 ; i != n ; i++ ) d[i << 1] = d[i << 1 | 1] = mul(y[i], e[i]); zero_n(d + ( n << 1 ), ( m - n ) << 1); for ( Z i = 0, j = 2 ; i != o ; i++, j <<= 1, swap(d, e) ) for ( Z k = 0, p ; k != ( m << 1 ) ; k += j << 1 ) { for ( p = 0 ; p + 7 < j ; p += 8 ) cast(b + p) = _mm256_blend_epi32(reduce_shift(_mm256_add_epi64(_mm256_mul_epi32(cast(t[i] + k + p), cast(d + j + k + p)), _mm256_mul_epi32(cast(t[i] + j + k + p), cast(d + k + p)))), reduce_unshift(_mm256_add_epi64(_mm256_mul_epi32(_mm256_srli_epi64(cast(t[i] + k + p), 32), _mm256_srli_epi64(cast(d + j + k + p), 32)), _mm256_mul_epi32(_mm256_srli_epi64(cast(t[i] + j + k + p), 32), _mm256_srli_epi64(cast(d + k + p), 32)))), 170); for ( ; p != j ; p++ ) b[p] = reduce((X)t[i][k + p] * d[j + k + p] + (X)t[i][j + k + p] * d[k + p]); if ( i != o - 1 ) copy_n(b, j, e + k), dit(b, j), dif_right_half(b, j), copy_n(b, j, e + j + k); else dit(b, j), copy_n(b, j, e); } copy_n(d + m - n, n, a); }
	inline void eval(const Z *a, const Z &n, const Z *x, const Z &m, Z *y) { const Z o = len(max(4u, max(n, m))); alignas(32) Z t[( __lg(o) + 1 ) * ( o << 1 )]; tree_build(x, m, o, t), tree_eval(a, n, o, t, m, y); }
	inline void inter(const Z *x, const Z *y, const Z &n, Z *a) { const Z o = len(max(4u, n)); alignas(32) Z t[( __lg(o) + 1 ) * ( o << 1 )]; tree_build(x, n, o, t), tree_inter(y, n, o, t, a); }

	inline void comp_impl(const Z *a, Z *b, const Z &m, const Z &n, const Z &w) { if ( n == 1 ) { alignas(32) Z c[m << 1]; extend_num(m << 1); for ( Z v = ifac[m - 1], i = 0 ; i != m + 1 ; i++, v = mul(v, w) ) b[m - i] = mul(v, mul(fac[m - 1 + i], ifac[i])); zero_n(b + m + 1, m - 1), dif(b, m << 1), copy_zero_n(a, m, c, m << 1), dif(c, m << 1), mul_n(c, b, m << 1), dit(c, m << 1), copy_n(c + m, m, b); return; } const Z o = m * n; alignas(32) Z c[o << 2], d[o << 2], e[o << 1]; for ( Z i = 0 ; i != m ; i++ ) copy_zero_n(b + i * n, n, c + ( i << 1 ) * n, n << 1); c[o << 1] = one, zero_n(c + ( o << 1 ) + 1, ( o << 1 ) - 1), dif(c, o << 2); for ( Z i = 0 ; i != ( o << 1 ) ; i++ ) d[i] = mul(c[i << 1], c[i << 1 | 1]); dit(d, o << 1), *d = sub(*d, one); for ( Z i = 1 ; i != ( m << 1 ) ; i++ ) copy_n(d + i * n, n >> 1, d + i * ( n >> 1 )); comp_impl(a, d, m << 1, n >> 1, w); for ( Z i = 0 ; i != ( m << 1 ) ; i++ ) copy_zero_n(d + i * ( n >> 1 ), n >> 1, e + i * n, n); dif(e, o << 1); for ( Z i = 0 ; i != ( o << 1 ) ; i++ ) b[i << 1] = mul(c[i << 1 | 1], e[i]), b[i << 1 | 1] = mul(c[i << 1], e[i]); dit(b, o << 2); for ( Z i = 0 ; i != m ; i++ ) copy_n(b + ( ( ( i + m ) << 1 ) * n ), n, b + i * n); }
	inline void comp(const Z *a, const Z &n, const Z *b, const Z &m, Z *c) { const Z o = len(n); alignas(32) Z d[o << 2]; neg_n(b, m, d), zero_n(d + m, o - m), comp_impl(a, d, 1, o, *b), copy_n(d, n, c); }
}
using POLY_AVX2::Z, POLY_AVX2::N;
Z n, m; alignas(32) Z x[N], y[N], a[N];	
int main()
{
	read(n, m);
	For(i, 0, n - 1) read(a[i]);
	For(i, 0, m - 1) read(x[i]);
	POLY_AVX2::in_n(a, n), POLY_AVX2::in_n(x, m);
	POLY_AVX2::eval(a, n, x, m, y);
	POLY_AVX2::out_n(y, m);
	For(i, 0, m - 1) write(y[i], i == m - 1 ? '\n' : ' ');
	return 0;
}
