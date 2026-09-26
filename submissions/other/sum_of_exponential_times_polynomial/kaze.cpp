#pragma region template
// clang-format off

#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#pragma GCC optimize("fast-math")
#pragma GCC optimize("unroll-loops")

#define _GNU_SOURCE
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef   int8_t      i8;
typedef   int16_t     i16;
typedef   int32_t     i32;
typedef   int64_t     i64;
typedef __int128_t    i128;
typedef   uint8_t     u8;
typedef   uint16_t    u16;
typedef   uint32_t    u32;
typedef   uint64_t    u64;
typedef __uint128_t   u128;
typedef   float       f32;
typedef   double      f64;

// https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html
// https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
#define clz32(a) ((a) ? __builtin_clz((a)) : (32))
#define clz64(a) ((a) ? __builtin_clzll((a)) : (64))
#define ctz32(a) ((a) ? __builtin_ctz((a)) : (32))
#define ctz64(a) ((a) ? __builtin_ctzll((a)) : (64))
#define pct32(a) __builtin_popcount((a))
#define pct64(a) __builtin_popcountll((a))
#define msb32(a) ((a) ? ((31) - __builtin_clz((a))) : (32))
#define msb64(a) ((a) ? ((63) - __builtin_clzll((a))) : (64))
#define bit_width32(a) ((a) ? ((32) - __builtin_clz((a))) : (0))
#define bit_width64(a) ((a) ? ((64) - __builtin_clzll((a))) : (0))
#define bit_ceil32(a) ((!(a)) ? (1) : ((pct32(a)) == (1) ? ((1u) << ((31) - clz32((a)))) : ((1u) << ((32) - clz32(a)))))
#define bit_ceil64(a) ((!(a)) ? (1) : ((pct64(a)) == (1) ? ((1ull) << ((63) - clz64((a)))) : ((1ull) << ((64) - clz64(a)))))
#define bit_floor32(a) ((!(a)) ? (0) : ((1u) << ((31) - clz32((a)))))
#define bit_floor64(a) ((!(a)) ? (0) : ((1ull) << ((63) - clz64((a)))))
#define flip_Nbit(a, n) ((a) ^ ((1) << (n)))
#define only_lsb(a) ((a) & (-(a)))
u32 bit_reverse32(u32 v) { v = (v & 0x55555555) << 1 | (v >> 1 & 0x55555555); v = (v & 0x33333333) << 2 | (v >> 2 & 0x33333333); v = (v & 0x0f0f0f0f) << 4 | (v >> 4 & 0x0f0f0f0f); v = (v & 0x00ff00ff) << 8 | (v >> 8 & 0x00ff00ff); v = (v & 0x0000ffff) << 16 | (v >> 16 & 0x0000ffff); return v; }
u64 bit_reverse64(u64 v) { v = (v & 0x5555555555555555) <<  1 | (v >>  1 & 0x5555555555555555); v = (v & 0x3333333333333333) <<  2 | (v >>  2 & 0x3333333333333333); v = (v & 0x0f0f0f0f0f0f0f0f) <<  4 | (v >>  4 & 0x0f0f0f0f0f0f0f0f); v = (v & 0x00ff00ff00ff00ff) <<  8 | (v >>  8 & 0x00ff00ff00ff00ff); v = (v & 0x0000ffff0000ffff) << 16 | (v >> 16 & 0x0000ffff0000ffff); v = (v & 0x00000000ffffffff) << 32 | (v >> 32 & 0x00000000ffffffff); return v; }
u32 rotr32(u32 x, i64 r) { if (r < 0) { u32 l = (u32)(((u64)(-r)) % 32); return x << l | x >> (-l & 31); } r %= 32; return x >> r | x << (-r & 31); }
u64 rotr64(u64 x, i64 r) { if (r < 0) { u64 l = ((u64)(-r)) % 64; return x << l | x >> (-l & 63); } r %= 64; return x >> r | x << (-r & 63); }
u32 rotl32(u32 x, i64 r) { return rotr32(x, -r); }
u64 rotl64(u64 x, i64 r) { return rotr64(x, -r); }

u32 in_u32(void) { u32 c, x = 0; while (c = getchar_unlocked(), c < '0' || c > '9') {} while ('/' < c && c < ':') { x = x * 10 + c - '0'; c = getchar_unlocked(); } return x; }
u64 in_u64(void) { u64 c, x = 0; while (c = getchar_unlocked(), c < '0' || c > '9') {} while ('/' < c && c < ':') { x = x * 10 + c - '0'; c = getchar_unlocked(); } return x; }
u128 in_u128(void) { u128 c, x = 0; while (c = getchar_unlocked(), c < '0' || c > '9') {} while ('/' < c && c < ':') { x = x * 10 + c - '0'; c = getchar_unlocked(); } return x; }
i32 in_i32(void) { i32 c, x = 0, f = 1; while (c = getchar_unlocked(), c < '0' || c > '9') { if (c == '-') { f = -f; } } while ('/' < c && c < ':') { x = x * 10 + c - '0'; c = getchar_unlocked(); } return f * x; }
i64 in_i64(void) { i64 c, x = 0, f = 1; while (c = getchar_unlocked(), c < '0' || c > '9') { if (c == '-') { f = -f; } } while ('/' < c && c < ':') { x = x * 10 + c - '0'; c = getchar_unlocked(); } return f * x; }
i128 in_i128(void) { i128 c, x = 0, f = 1; while (c = getchar_unlocked(), c < '0' || c > '9') { if (c == '-') { f = -f; } } while ('/' < c && c < ':') { x = x * 10 + c - '0'; c = getchar_unlocked(); } return f * x; }
void out_u32(u32 x) { if (x >= 10) { out_u32(x / 10); } putchar_unlocked(x - x / 10 * 10 + '0'); }
void out_u64(u64 x) { if (x >= 10) { out_u64(x / 10); } putchar_unlocked(x - x / 10 * 10 + '0'); }
void out_u128(u128 x) { if (x >= 10) { out_u128(x / 10); } putchar_unlocked(x - x / 10 * 10 + '0'); }
void out_i32(i32 x) { if (x < 0) { putchar_unlocked('-'); x = -x; } out_u32((u32)x); }
void out_i64(i64 x) { if (x < 0) { putchar_unlocked('-'); x = -x; } out_u64((u64)x); }
void out_i128(i128 x) { if (x < 0) { putchar_unlocked('-'); x = -x; } out_u128((u128)x); }
void newline(void) { putchar_unlocked('\n'); }
void space(void) { putchar_unlocked(' '); }
void printb_32bit(u32 v) { u32 mask = (u32)1 << (sizeof(v) * CHAR_BIT - 1); do { putchar_unlocked(mask & v ? '1' : '0'); } while (mask >>= 1); }
void printb_64bit(u64 v) { u64 mask = (u64)1 << (sizeof(v) * CHAR_BIT - 1); do { putchar_unlocked(mask & v ? '1' : '0'); } while (mask >>= 1); }

// https://en.wikipedia.org/wiki/Linear_congruential_generator
u32 lcg_rand(void) { static u64 lcg_state = 14534622846793005ull; lcg_state = 6364136223846793005ull * lcg_state + 1442695040888963407ull; return (u32)lcg_state; }
u32 lcg_range(u32 l, u32 r) { return l + lcg_rand() % (r - l + 1); }
f32 lcg_randf(void) { u32 a = 0x3F800000u | (lcg_rand() >> 9); return (*((f32 *)(&a))) - 1; }
// https://en.wikipedia.org/wiki/Permuted_congruential_generator
// https://www.pcg-random.org/index.html
u32 pcg_rand(void) { static u64 pcg_state = 0x853c49e6748fea9bull; u64 t = pcg_state; pcg_state = t * 0x5851f42d4c957f2dull + 0xda3e39cb94b95bdbull; u32 sh = ((t >> 18u) ^ t) >> 27u; u32 ro = t >> 59u; return (sh >> ro) | (sh << ((-ro) & 31)); }
u32 pcg_range(u32 l, u32 r) { return l + pcg_rand() % (r - l + 1); }
f32 pcg_randf(void) { u32 a = 0x3F800000u | (pcg_rand() >> 9); return (*((f32 *)(&a))) - 1; }
// https://en.wikipedia.org/wiki/Middle-square_method
// https://doi.org/10.48550/arXiv.1704.00358
u64 msws_rand(void) { static u64 msws_state1 = 0; static u64 msws_state2 = 0; static u64 msws_state3 = 0xb5ad4eceda1ce2a9ul; static u64 msws_state4 = 0; static u64 msws_state5 = 0; static u64 msws_state6 = 0x278c5a4d8419fe6bul; u64 ret; msws_state1 *= msws_state1; ret = msws_state1 += (msws_state2 += msws_state3); msws_state1 = (msws_state1 >> 32) | (msws_state1 << 32); msws_state4 *= msws_state4; msws_state4 += (msws_state5 += msws_state6); msws_state4 = (msws_state4 >> 32) | (msws_state4 << 32); return ret ^ msws_state4; }
u64 msws_range(u64 l, u64 r) { return l + msws_rand() % (r - l + 1); }
f64 msws_randf(void) { u64 a = 0x3FF0000000000000ull | (msws_rand() >> 12); return (*((f64 *)(&a))) - 1; }
// https://ja.wikipedia.org/wiki/Xorshift
// https://prng.di.unimi.it/xoroshiro128plus.c
u64 xrsr128p_rand(void) { static u64 xrsr128p_state1 = 0x1ull; static u64 xrsr128p_state2 = 0x2ull; const u64 s0 = xrsr128p_state1; u64 s1 = xrsr128p_state2; const u64 ret = s0 + s1; s1 ^= s0; xrsr128p_state1 = rotl64(s0, 24) ^ s1 ^ (s1 << 16); xrsr128p_state2 = rotl64(s1, 37); return ret; }
u64 xrsr128p_range(u64 l, u64 r) { return l + xrsr128p_rand() % (r - l + 1); }
f64 xrsr128p_randf(void) { u64 a = 0x3FF0000000000000ull | (xrsr128p_rand() >> 12); return (*((f64 *)(&a))) - 1; }
// https://prng.di.unimi.it/xoroshiro128starstar.c
u64 xrsr128ss_rand(void) { static u64 xrsr128ss_state1 = 0x1ull; static u64 xrsr128ss_state2 = 0x2ull; const u64 s0 = xrsr128ss_state1; u64 s1 = xrsr128ss_state2; const u64 ret = rotl64(s0 * 5, 7) * 9; s1 ^= s0; xrsr128ss_state1 = rotl64(s0, 24) ^ s1 ^ (s1 << 16); xrsr128ss_state2 = rotl64(s1, 37); return ret; }
u64 xrsr128ss_range(u64 l, u64 r) { return l + xrsr128ss_rand() % (r - l + 1); }
f64 xrsr128ss_randf(void) { u64 a = 0x3FF0000000000000ull | (xrsr128ss_rand() >> 12); return (*((f64 *)(&a))) - 1; }

// power with upperbound
u64 saturate_pow_u64(u64 x, u64 y) { if (y == 0) { return 1ull; } u64 res = 1ull; while (y) { if (y & 1) { res = __builtin_mul_overflow_p(res, x, (u64)0) ? UINT64_MAX : res * x; } x = __builtin_mul_overflow_p(x, x, (u64)0) ? UINT64_MAX : x * x; y >>= 1; } return res; }
// floor(a^(1/k))
u64 floor_kth_root_integer(u64 a, u64 k) { if (a <= 1 || k == 1) { return a; } if (k >= 64) { return 1; } if (k == 2) { return sqrtl(a); } if (a == UINT64_MAX) { a--; } u64 res = (k == 3 ? cbrt(a) - 1 : pow(a, nextafter(1 / (double)k, 0))); while (saturate_pow_u64(res + 1, k) <= a) { res++; } return res; }

// https://en.wikipedia.org/wiki/Jacobi_symbol
int jacobi_symbol(long long a, long long n) { int j = 1; long long t; while (a) { if (a < 0) { a = -a; if ((n & 3) == 3) { j = -j; } } int s = ctz64(a); a >>= s; if ((((n & 7) == 3) || ((n & 7) == 5)) && (s & 1)) { j = -j; } if ((a & n & 3) == 3) { j = -j; } t = a, a = n, n = t; a %= n; if ((a << 1) > n) { a -= n; } } return n == 1 ? j : 0; }

// https://en.wikipedia.org/wiki/Binary_GCD_algorithm#Algorithm
u32 gcd32(u32 a, u32 b) { if (!a || !b) { return a | b; } u32 t, s = ctz32(a | b); a >>= ctz32(a); do { b >>= ctz32(b); if (a > b) { t = a, a = b, b = t; } b -= a; } while (b); return a << s; }
u64 gcd64(u64 a, u64 b) { if (!a || !b) { return a | b; } u64 t, s = ctz64(a | b); a >>= ctz64(a); do { b >>= ctz64(b); if (a > b) { t = a, a = b, b = t; } b -= a; } while (b); return a << s; }

// https://ja.wikipedia.org/wiki/%E3%82%B3%E3%83%A0%E3%82%BD%E3%83%BC%E3%83%88#%E6%94%B9%E8%89%AF%E7%89%88%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0
void combsort11_32(int a_len, u32 *a) { int g = a_len; u32 t; while (true) { int flag = 1; g = (((g * 10) / 13) > 1) ? ((g * 10) / 13) : 1; if (g == 9 || g == 10) { g = 11; } for (int i = 0; i + g < a_len; i++) { if (a[i] > a[i + g]) { t = a[i], a[i] = a[i + g], a[i + g] = t; flag = 0; } } if (g == 1 && flag) { break; } } }
void combsort11_64(int a_len, u64 *a) { int g = a_len; u64 t; while (true) { int flag = 1; g = (((g * 10) / 13) > 1) ? ((g * 10) / 13) : 1; if (g == 9 || g == 10) { g = 11; } for (int i = 0; i + g < a_len; i++) { if (a[i] > a[i + g]) { t = a[i], a[i] = a[i + g], a[i + g] = t; flag = 0; } } if (g == 1 && flag) { break; } } }

// a * x + b * y = gcd(a, b)
typedef struct { i32 a, b; u32 d; } Bezout32;
Bezout32 bezout32(u32 x, u32 y) { u32 t; bool swap = x < y; if (swap) { t = x; x = y; y = t; } if (y == 0) { if (x == 0) { return (Bezout32){0, 0, 0}; } else if (swap) { return (Bezout32){0, 1, x}; } else { return (Bezout32){1, 0, x}; } } i32 s0 = 1, s1 = 0, t0 = 0, t1 = 1; while (true) { u32 q = x / y, r = x % y; if (r == 0) { if (swap) { return (Bezout32){t1, s1, y}; } else { return (Bezout32){s1, t1, y}; } } i32 s2 = s0 - (i32)(q) * s1, t2 = t0 - (i32)(q) * t1; x = y, y = r; s0 = s1, s1 = s2, t0 = t1, t1 = t2; } }
u32 modinv32(u32 x, u32 mod) { Bezout32 abd = bezout32(x, mod); return abd.a < 0 ? mod + abd.a : (u32)abd.a; }
typedef struct { i64 a, b; u64 d; } Bezout64;
Bezout64 bezout64(u64 x, u64 y) { u64 t; bool swap = x < y; if (swap) { t = x; x = y; y = t; } if (y == 0) { if (x == 0) { return (Bezout64){0, 0, 0}; } else if (swap) { return (Bezout64){0, 1, x}; } else { return (Bezout64){1, 0, x}; } } i64 s0 = 1, s1 = 0, t0 = 0, t1 = 1; while (true) { u64 q = x / y, r = x % y; if (r == 0) { if (swap) { return (Bezout64){t1, s1, y}; } else { return (Bezout64){s1, t1, y}; } } i64 s2 = s0 - (i64)(q) * s1, t2 = t0 - (i64)(q) * t1; x = y, y = r; s0 = s1, s1 = s2, t0 = t1, t1 = t2; } }
u64 modinv64(u64 x, u64 mod) { Bezout64 abd = bezout64(x, mod); return abd.a < 0 ? mod + abd.a : (u64)abd.a; }

// https://en.wikipedia.org/wiki/Montgomery_modular_multiplication#The_REDC_algorithm
/* mod < 1073741824 */
static u32 n_m32, n2_m32, ni_m32, r1_m32, r2_m32, r3_m32;
void set_m32(u32 mod) { if (mod == n_m32) { return; } n_m32 = mod; n2_m32 = mod << 1; ni_m32 = mod; ni_m32 *= 2 - ni_m32 * mod; ni_m32 *= 2 - ni_m32 * mod; ni_m32 *= 2 - ni_m32 * mod; ni_m32 *= 2 - ni_m32 * mod; r1_m32 = (u32)(i32)-1 % mod + 1; r2_m32 = (u64)(i64)-1 % mod + 1; r3_m32 = (u32)(((u64)r1_m32 * (u64)r2_m32) % mod); }
u32 mr32(u64 a) { u32 y = (u32)(a >> 32) - (u32)(((u64)((u32)a * ni_m32) * n_m32) >> 32); return (i32)y < 0 ? y + n_m32 : y; }
u32 to_m32(u32 a) { return mr32((u64)a * r2_m32); }
u32 from_m32(u32 a) { return mr32((u64)a); }
u32 add_m32(u32 a, u32 b) { a += b; a -= (a >= n_m32 ? n_m32 : 0); return a; }
u32 sub_m32(u32 a, u32 b) { a += (a < b ? n_m32 : 0); a -= b; return a; }
u32 min_m32(u32 a) { return sub_m32(0, a); }
u32 mul_m32(u32 a, u32 b) { return mr32((u64)a * b); }
u32 squ_m32(u32 a) { return mr32((u64)a * a); }
u32 shl_m32(u32 a) { return (a <<= 1) >= n_m32 ? a - n_m32 : a; }
u32 shr_m32(u32 a) { return (a & 1) ? ((a >> 1) + (n_m32 >> 1) + 1) : (a >> 1); }
u32 pow_m32(u32 a, u64 k) { u32 ret = r1_m32; u64 deg = k; while (deg > 0) { if (deg & 1) { ret = mul_m32(ret, a); } a = squ_m32(a); deg >>= 1; } return ret; }
u32 inv_m32(u32 a) { return mr32((u64)r3_m32 * modinv32(a, n_m32)); }
u32 div_m32(u32 a, u32 b) { return mul_m32(a, inv_m32(b)); }

/* mod < 4611686018427387904 */
static u64 n_m64, n2_m64, ni_m64, r1_m64, r2_m64, r3_m64;
void set_m64(u64 mod) { if (mod == n_m64) { return; } n_m64 = mod; n2_m64 = mod << 1; ni_m64 = mod; ni_m64 *= 2 - ni_m64 * mod; ni_m64 *= 2 - ni_m64 * mod; ni_m64 *= 2 - ni_m64 * mod; ni_m64 *= 2 - ni_m64 * mod; ni_m64 *= 2 - ni_m64 * mod; r1_m64 = (u64)(i64)-1 % mod + 1; r2_m64 = (u128)(i128)-1 % mod + 1; r3_m64 = (u64)(((u128)r1_m64 * (u128)r2_m64) % mod); }
u64 mr64(u128 a) { u64 y = (u64)(a >> 64) - (u64)(((u128)((u64)a * ni_m64) * n_m64) >> 64); return (i64)y < 0 ? y + n_m64 : y; }
u64 to_m64(u64 a) { return mr64((u128)a * r2_m64); }
u64 from_m64(u64 a) { return mr64((u128)a); }
u64 add_m64(u64 a, u64 b) { a += b; a -= (a >= n_m64 ? n_m64 : 0); return a; }
u64 sub_m64(u64 a, u64 b) { a += (a < b ? n_m64 : 0); a -= b; return a; }
u64 min_m64(u64 a) { return sub_m64(0, a); }
u64 mul_m64(u64 a, u64 b) { return mr64((u128)a * b); }
u64 squ_m64(u64 a) { return mr64((u128)a * a); }
u64 shl_m64(u64 a) { return (a <<= 1) >= n_m64 ? a - n_m64 : a; }
u64 shr_m64(u64 a) { return (a & 1) ? ((a >> 1) + (n_m64 >> 1) + 1) : (a >> 1); }
u64 pow_m64(u64 a, u64 k) { u64 ret = r1_m64, deg = k; while (deg > 0) { if (deg & 1) { ret = mul_m64(ret, a); } a = squ_m64(a); deg >>= 1; } return ret; }
u64 inv_m64(u64 a) { return mr64((u128)r3_m64 * modinv64(a, n_m64)); }
u64 div_m64(u64 a, u64 b) { return mul_m64(a, inv_m64(b)); }

// https://en.wikipedia.org/wiki/Barrett_reduction
static u64 m_b32, im_b32, div_b32, rem_b32;
void set_b32(u64 mod) { m_b32 = mod; im_b32 = ((((u128)(1ull) << 64)) + mod - 1) / mod; div_b32 = 0; rem_b32 = 0; }
void br32(u64 a) { u64 x = (u64)(((u128)(a) * im_b32) >> 64); u64 y = x * m_b32; unsigned long long z; u32 w = __builtin_usubll_overflow(a, y, &z) ? m_b32 : 0; div_b32 = x; rem_b32 = z + w; }
u32 add_b32(u32 a, u32 b) { a += b; a -= (a >= (u32)m_b32 ? (u32)m_b32 : 0); return a; }
u32 sub_b32(u32 a, u32 b) { a += (a < b ? (u32)m_b32 : 0); a -= b; return a; }
u32 min_b32(u32 a) { return sub_b32(0, a); }
u32 mul_b32(u32 a, u32 b) { br32((u64)a * b); return (u32)rem_b32; }
u32 squ_b32(u32 a) { br32((u64)a * a); return (u32)rem_b32; }
u32 shl_b32(u32 a) { return (a <<= 1) >= m_b32 ? a - m_b32 : a; }
u32 shr_b32(u32 a) { return (a & 1) ? ((a >> 1) + (m_b32 >> 1) + 1) : (a >> 1); }
u32 pow_b32(u32 a, u64 k) { u32 ret = 1u; u64 deg = k; while (deg > 0) { if (deg & 1) { ret = mul_b32(ret, a); } a = squ_b32(a); deg >>= 1; } return ret; }

static u128 m_b64, im_b64, div_b64, rem_b64;
void set_b64(u128 mod) { if (mod == m_b64) { return; } m_b64 = mod; im_b64 = (~((u128)0ull)) / mod; div_b64 = 0; rem_b64 = 0; }
void br64(u128 x) { if (m_b64 == 1) { div_b64 = x; rem_b64 = 0; return; } u8 f; u128 a = x >> 64; u128 b = x & 0xffffffffffffffffull; u128 c = im_b64 >> 64; u128 d = im_b64 & 0xffffffffffffffffull; u128 ac = a * c; u128 bd = (b * d) >> 64; u128 ad = a * d; u128 bc = b * c; f = (ad > ((u128)((i128)(-1L)) - bd)); bd += ad; ac += f; f = (bc > ((u128)((i128)(-1L)) - bd)); bd += bc; ac += f; u128 q = ac + (bd >> 64); u128 r = x - q * m_b64; if (m_b64 <= r) { r -= m_b64; q += 1; } div_b64 = q; rem_b64 = r; }
u64 add_b64(u64 a, u64 b) { a += b; a -= (a >= (u64)m_b64 ? (u64)m_b64 : 0); return a; }
u64 sub_b64(u64 a, u64 b) { a += (a < b ? (u64)m_b64 : 0); a -= b; return a; }
u64 min_b64(u64 a) { return sub_b64(0, a); }
u64 mul_b64(u64 a, u64 b) { br64((u128)a * b); return (u64)rem_b64; }
u64 squ_b64(u64 a) { br64((u128)a * a); return (u64)rem_b64; }
u64 shl_b64(u64 a) { return (a <<= 1) >= m_b64 ? a - m_b64 : a; }
u64 shr_b64(u64 a) { return (a & 1) ? ((a >> 1) + (m_b64 >> 1) + 1) : (a >> 1); }
u64 pow_b64(u64 a, u64 k) { u64 ret = 1ull, deg = k; while (deg > 0) { if (deg & 1) { ret = mul_b64(ret, a); } a = squ_b64(a); deg >>= 1; } return ret; }

// clang-format on
#pragma endregion template

#pragma region enumerate primes
// clang-format off

#define WHEEL_CYCLE 0xd2
#define WHEEL_REM_COUNT 0x30
#define BLOCK_SIZE 0x21d470
#define BS_SHIFT 0x6
#define BS_MASK 0x3f
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

const int wheel_primes[4] = {2, 3, 5, 7};
const int pre_primes[4] = {11, 13, 17, 19};
const int wheel_rem[WHEEL_REM_COUNT] = {1, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 121, 127, 131, 137, 139, 143, 149, 151, 157, 163, 167, 169, 173, 179, 181, 187, 191, 193, 197, 199, 209};
const int wheel_pos[WHEEL_CYCLE] = {-1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, 2, -1, -1, -1, 3, -1, 4, -1, -1, -1, 5, -1, -1, -1, -1, -1, 6, -1, 7, -1, -1, -1, -1, -1, 8, -1, -1, -1, 9, -1, 10, -1, -1, -1, 11, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, 13, -1, 14, -1, -1, -1, -1, -1, 15, -1, -1, -1, 16, -1, 17, -1, -1, -1, -1, -1, 18, -1, -1, -1, 19, -1, -1, -1, -1, -1, 20, -1, -1, -1, -1, -1, -1, -1, 21, -1, -1, -1, 22, -1, 23, -1, -1, -1, 24, -1, 25, -1, -1, -1, 26, -1, -1, -1, -1, -1, -1, -1, 27, -1, -1, -1, -1, -1, 28, -1, -1, -1, 29, -1, -1, -1, -1, -1, 30, -1, 31, -1, -1, -1, 32, -1, -1, -1, -1, -1, 33, -1, 34, -1, -1, -1, -1, -1, 35, -1, -1, -1, -1, -1, 36, -1, -1, -1, 37, -1, 38, -1, -1, -1, 39, -1, -1, -1, -1, -1, 40, -1, 41, -1, -1, -1, -1, -1, 42, -1, -1, -1, 43, -1, 44, -1, -1, -1, 45, -1, 46, -1, -1, -1, -1, -1, -1, -1, -1, -1, 47};

u64 pre_block[34642];
u64 block[34642];
int prime_next[4800];
u8 prime_cyc[4800];
int prime_jumps[4800][48];

int from_wheeled(int i) { return i / WHEEL_REM_COUNT * WHEEL_CYCLE + wheel_rem[i % WHEEL_REM_COUNT]; }
int to_wheeled(int i) { return i / WHEEL_CYCLE * WHEEL_REM_COUNT + wheel_pos[i % WHEEL_CYCLE]; }

int cnt;
int primes[664579];
void enumerate_primes(int n) { cnt = 0; for (int i = 0; i < 4; ++i) { int p = wheel_primes[i]; if (p > n) { break; } primes[cnt++] = p; } int small_prime_cnt = 0; int sqrt_n = (int)sqrtl(n); bool *sieve = (bool *)malloc((sqrt_n + 1) * sizeof(bool)); for (int i = 0; i <= sqrt_n; ++i) { sieve[i] = true; } for (int p = 3; p <= sqrt_n; p += 2) { if (sieve[p]) { if (p > 19) { int prev = prime_next[small_prime_cnt] = to_wheeled(p * p); int *jumps = prime_jumps[small_prime_cnt++]; for (int i = 2, done = 0; done < 48; i += 2) { if (wheel_pos[p * (p + i) % WHEEL_CYCLE] != -1) { int wheeled = to_wheeled(p * (p + i)); jumps[done++] = wheeled - prev; prev = wheeled; } } for (int i = p * p; i <= sqrt_n; i += p) { sieve[i] = false; } } else { for (int i = p * p; i <= sqrt_n; i += p) { sieve[i] = false; } } } } free(sieve); for (int i = 0; i < 34642; ++i) { pre_block[i] = (u64)(i64)-1; } for (int i = 0; i < 4; ++i) { int p = pre_primes[i]; for (int i = p, j = 0; j < BLOCK_SIZE; i += 2 * p) { if (wheel_pos[i % WHEEL_CYCLE] != -1 && (j = to_wheeled(i)) < BLOCK_SIZE) { pre_block[j >> BS_SHIFT] &= ~(1ull << (j & BS_MASK)); } } } int n2 = to_wheeled(n - n % WHEEL_CYCLE + 1); while (from_wheeled(n2) <= n) { ++n2; } for (int i = 0; i < small_prime_cnt; ++i) { prime_cyc[i] = 0; } for (int l = 0, r = MIN(n2, BLOCK_SIZE); l < n2; l += BLOCK_SIZE, r = MIN(r + BLOCK_SIZE, n2)) { memcpy(block, pre_block, 34642 * sizeof(u64)); if (l == 0) { block[0] &= ~1ull; for (int i = 0; i < 4; ++i) { int p = pre_primes[i]; block[0] |= 1ull << to_wheeled(p); } } for (int i = 0; i < small_prime_cnt; ++i) { int next = prime_next[i]; u8 cyc = prime_cyc[i]; while (next < r) { block[(next - l) >> BS_SHIFT] &= ~(1ull << ((next - l) & BS_MASK)); next += prime_jumps[i][cyc]; if (++cyc == 48) { cyc = 0; } } prime_next[i] = next; prime_cyc[i] = cyc; } int off = l; int i = 0; for (; off + 64 < r; ++i, off += 64) { if (block[i]) { u64 bs = block[i]; int pc = __builtin_popcountll(bs); for (; bs; bs &= bs - 1) { primes[cnt++] = from_wheeled(off + __builtin_ctzll(bs)); } } } u64 bs = block[i] & ((1ull << (r - off)) - 1); int pc = __builtin_popcountll(bs); for (; bs; bs &= bs - 1) { primes[cnt++] = from_wheeled(off + __builtin_ctzll(bs)); } } }

// clang-format on
#pragma endregion enumerate primes

#pragma region binomial mod

u32 inv_fac_table[100000100];
u32 table_size;
void binom_init(u32 n)
{
    if (table_size >= n + 10)
    {
        return;
    }
    table_size = n + 10;
    n += 9;
    inv_fac_table[0] = r1_m32;
    for (u32 i = 1; i <= n; i++)
    {
        inv_fac_table[i] = mul_m32(inv_fac_table[i - 1], to_m32(i));
    }
    inv_fac_table[n] = inv_m32(inv_fac_table[n]);
    for (u32 i = n - 1; i > 0; i--)
    {
        inv_fac_table[i] = mul_m32(inv_fac_table[i + 1], to_m32(i + 1));
    }
}
u32 get_fac(int i) { return inv_m32(inv_fac_table[i]); }
u32 get_finv(int i) { return inv_fac_table[i]; }
u32 get_inv(int i) { return mul_m32(inv_m32(inv_fac_table[i - 1]), inv_fac_table[i]); }

#pragma endregion binomial mod

u32 f_size;
u32 f[100000100];
u32 dp[100000100];

u32 lagrange_interpolation(u32 y, u64 x)
{
    u32 N = y - 1;
    if (x <= N)
    {
        return f[x];
    }
    u32 ret = 0;
    dp[0] = r1_m32;
    u32 a = to_m32(x % n_m32);
    for (int i = 0; i < N; i++)
    {
        dp[i + 1] = mul_m32(dp[i], a);
        a = sub_m32(a, r1_m32);
    }
    u32 tmp = a;
    for (int i = N; i > 0; i--)
    {
        dp[i - 1] = mul_m32(dp[i - 1], tmp);
        a = add_m32(a, r1_m32);
        tmp = mul_m32(tmp, a);
    }
    for (int i = 0; i <= N; i++)
    {
        u32 tmp = mul_m32(mul_m32(f[i], dp[i]), mul_m32(get_finv(i), get_finv(N - i)));
        ret = ((N - i) & 1) ? sub_m32(ret, tmp) : add_m32(ret, tmp);
    }
    return ret;
}
u32 sum_of_exp(u32 fac_f_size, u32 a, u64 n)
{
    if (n == 0)
    {
        return 0;
    }
    if (a == 0)
    {
        return f[0];
    }
    if (a == 1)
    {
        u32 tmp = f[0];
        u32 tmp2 = 0;
        f[0] = 0;
        for (u32 i = 1; i < f_size + 1; i++)
        {
            tmp2 = f[i];
            f[i] = add_m32(f[i - 1], tmp);
            tmp = tmp2;
        }
        return lagrange_interpolation(f_size + 1, n);
    }
    u32 K = f_size - 1;
    u32 buf = r1_m32;
    a = to_m32(a);
    for (u32 i = 0; i < f_size; i++)
    {
        f[i] = mul_m32(f[i], buf);
        buf = mul_m32(buf, a);
    }
    for (u32 i = 1; i < f_size; i++)
    {
        f[i] = add_m32(f[i], f[i - 1]);
    }
    u32 c = 0;
    u32 buf2 = r1_m32;
    for (int i = 0; i <= K; i++)
    {
        c = add_m32(c, mul_m32(mul_m32(fac_f_size, mul_m32(get_finv(K + 1 - i), get_finv(i))), mul_m32(buf2, f[K - i])));
        buf2 = mul_m32(buf2, min_m32(a));
    }

    c = div_m32(c, pow_m32(add_m32(min_m32(a), r1_m32), K + 1));
    u32 buf3 = r1_m32;
    u32 ia = inv_m32(a);
    for (u32 i = 0; i < f_size; i++)
    {
        f[i] = mul_m32(sub_m32(f[i], c), buf3);
        buf3 = mul_m32(buf3, ia);
    }
    u32 tn = lagrange_interpolation(f_size, n - 1);
    return add_m32(mul_m32(tn, pow_m32(a, n - 1)), c);
}
void exp_enumerate(u32 d)
{
    f_size = d + 1;
    memset(f, 0, f_size * sizeof(u32));
    if (!d)
    {
        f[0] = r1_m32;
        return;
    }
    f[1] = r1_m32;
#define INSIDE(i) ((i) < (664579))
#define GET_PRIME(i) ((i) < (664579) ? (i) : (664578))
    for (u32 i = 0; INSIDE(i) && primes[GET_PRIME(i)] <= d; i++)
    {
        f[primes[GET_PRIME(i)]] = pow_m32(to_m32(primes[GET_PRIME(i)]), d);
    }
    for (u32 i = 2; i <= d; ++i)
    {
        for (u32 j = 0; INSIDE(j) && (primes[GET_PRIME(j)] <= i) && ((u64)i * primes[GET_PRIME(j)] <= d); j++)
        {
            f[i * primes[GET_PRIME(j)]] = mul_m32(f[i], f[primes[GET_PRIME(j)]]);
            if (!(i % primes[GET_PRIME(j)]))
            {
                break;
            }
        }
    }
#undef INSIDE
#undef GET_PRIME
}
u32 sum_of_exponential_times_polynomial(u32 r, u64 n)
{
    return sum_of_exp(get_fac(f_size), r, n);
}

int main(void)
{
    set_m32(998244353u);
    enumerate_primes(10000000);
    binom_init(10000000);
    u32 r = in_u32();
    u32 d = in_u32();
    u64 n = in_u64();
    exp_enumerate(d);
    out_u64(from_m32(sum_of_exponential_times_polynomial(r, n)));
    newline();
    return 0;
}