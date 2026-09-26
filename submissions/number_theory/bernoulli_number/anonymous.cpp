#pragma region opt
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma endregion opt

#pragma region header
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#pragma endregion header

#pragma region type
/* signed integer */
typedef   int8_t      i8;
typedef   int16_t     i16;
typedef   int32_t     i32;
typedef   int64_t     i64;
typedef __int128_t    i128;
/* unsigned integer */
typedef   uint8_t     u8;
typedef   uint16_t    u16;
typedef   uint32_t    u32;
typedef   uint64_t    u64;
typedef __uint128_t   u128;
/* floating point number */
typedef   float       f32;
typedef   double      f64;
typedef   long double f80;
#pragma endregion type

#pragma region macro
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define SWAP(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))
#define POPCNT32(a) __builtin_popcount((a))
#define POPCNT64(a) __builtin_popcountll((a))
#define CTZ32(a) __builtin_ctz((a))
#define CLZ32(a) __builtin_clz((a))
#define CTZ64(a) __builtin_ctzll((a))
#define CLZ64(a) __builtin_clzll((a))
#define HAS_SINGLE_BIT32(a) (__builtin_popcount((a)) == (1))
#define HAS_SINGLE_BIT64(a) (__builtin_popcountll((a)) == (1))
#define MSB32(a) ((31) - __builtin_clz((a)))
#define MSB64(a) ((63) - __builtin_clzll((a)))
#define BIT_WIDTH32(a) ((a) ? ((32) - __builtin_clz((a))) : (0))
#define BIT_WIDTH64(a) ((a) ? ((64) - __builtin_clzll((a))) : (0))
#define LSBit(a) ((a) & (-(a)))
#define CLSBit(a) ((a) & ((a) - (1)))
#define BIT_CEIL32(a) ((!(a)) ? (1) : ((POPCNT32(a)) == (1) ? ((1u) << ((31) - CLZ32((a)))) : ((1u) << ((32) - CLZ32(a)))))
#define BIT_CEIL64(a) ((!(a)) ? (1) : ((POPCNT64(a)) == (1) ? ((1ull) << ((63) - CLZ64((a)))) : ((1ull) << ((64) - CLZ64(a)))))
#define BIT_FLOOR32(a) ((!(a)) ? (0) : ((1u) << ((31) - CLZ32((a)))))
#define BIT_FLOOR64(a) ((!(a)) ? (0) : ((1ull) << ((63) - CLZ64((a)))))
#define _ROTL32(x, s) (((x) << ((s) % (32))) | (((x) >> ((32) - ((s) % (32))))))
#define _ROTR32(x, s) (((x) >> ((s) % (32))) | (((x) << ((32) - ((s) % (32))))))
#define ROTL32(x, s) (((s) == (0)) ? (x) : ((((i64)(s)) < (0)) ? (_ROTR32((x), -(s))) : (_ROTL32((x), (s)))))
#define ROTR32(x, s) (((s) == (0)) ? (x) : ((((i64)(s)) < (0)) ? (_ROTL32((x), -(s))) : (_ROTR32((x), (s)))))
#define _ROTL64(x, s) (((x) << ((s) % (64))) | (((x) >> ((64) - ((s) % (64))))))
#define _ROTR64(x, s) (((x) >> ((s) % (64))) | (((x) << ((64) - ((s) % (64))))))
#define ROTL64(x, s) (((s) == (0)) ? (x) : ((((i128)(s)) < (0)) ? (_ROTR64((x), -(s))) : (_ROTL64((x), (s)))))
#define ROTR64(x, s) (((s) == (0)) ? (x) : ((((i128)(s)) < (0)) ? (_ROTL64((x), -(s))) : (_ROTR64((x), (s)))))
#pragma endregion macro

#pragma region io
int read_int(void) {
  // -2147483648 ～ 2147483647 (> 10 ^ 9)
  int c, x = 0, f = 1;
  while (c = getchar_unlocked(), c < 48 || c > 57) if (c == 45) f = -f;
  while (47 < c && c < 58) {
    x = x * 10 + c - 48;
    c = getchar_unlocked();
  }
  return f * x;
}
i32 in_i32(void) {
  // -2147483648 ～ 2147483647 (> 10 ^ 9)
  i32 c, x = 0, f = 1;
  while (c = getchar_unlocked(), c < 48 || c > 57) if (c == 45) f = -f;
  while (47 < c && c < 58) {
    x = x * 10 + c - 48;
    c = getchar_unlocked();
  }
  return f * x;
}
u32 in_u32(void) {
  // 0 ～ 4294967295 (> 10 ^ 9)
  u32 c, x = 0;
  while (c = getchar_unlocked(), c < 48 || c > 57);
  while (47 < c && c < 58) {
    x = x * 10 + c - 48;
    c = getchar_unlocked();
  }
  return x;
}
i64 in_i64(void) {
  // -9223372036854775808 ～ 9223372036854775807 (> 10 ^ 18)
  i64 c, x = 0, f = 1;
  while (c = getchar_unlocked(), c < 48 || c > 57) if (c == 45) f = -f;
  while (47 < c && c < 58) {
    x = x * 10 + c - 48;
    c = getchar_unlocked();
  }
  return f * x;
}
u64 in_u64(void) {
  // 0 ～ 18446744073709551615 (> 10 ^ 19)
  u64 c, x = 0;
  while (c = getchar_unlocked(), c < 48 || c > 57);
  while (47 < c && c < 58) {
    x = x * 10 + c - 48;
    c = getchar_unlocked();
  }
  return x;
}
static inline void write_int_inner(int x) {
  if (x >= 10) write_int_inner(x / 10);
  putchar_unlocked(x - x / 10 * 10 + 48);
}
void write_int(int x) {
  if (x < 0) {
    putchar_unlocked('-');
    x = -x;
  }
  write_int_inner(x);
}
static inline void out_i32_inner(i32 x) {
  if (x >= 10) out_i32_inner(x / 10);
  putchar_unlocked(x - x / 10 * 10 + 48);
}
void out_i32(i32 x) {
  if (x < 0) {
    putchar_unlocked('-');
    x = -x;
  }
  out_i32_inner(x);
}
void out_u32(u32 x) {
  if (x >= 10) out_u32(x / 10);
  putchar_unlocked(x - x / 10 * 10 + 48);
}
static inline void out_i64_inner(i64 x) {
  if (x >= 10) out_i64_inner(x / 10);
  putchar_unlocked(x - x / 10 * 10 + 48);
}
void out_i64(i64 x) {
  if (x < 0) {
    putchar_unlocked('-');
    x = -x;
  }
  out_i64_inner(x);
}
void out_u64(u64 x) {
  if (x >= 10) out_u64(x / 10);
  putchar_unlocked(x - x / 10 * 10 + 48);
}
void NL(void) { putchar_unlocked('\n'); }
void SP(void) { putchar_unlocked(' '); }
void write_int_array(int *a, int a_len) {
  for (int i = 0; i < a_len; i++) {
    if (i) SP();
    write_int(a[i]);
  }
  NL();
}
void out_i32_array(i32 *a, int a_len) {
  for (int i = 0; i < a_len; i++) {
    if (i) SP();
    out_i32(a[i]);
  }
  NL();
}
void out_u32_array(u32 *a, int a_len) {
  for (int i = 0; i < a_len; i++) {
    if (i) SP();
    out_u32(a[i]);
  }
  NL();
}
void out_i64_array(i64 *a, int a_len) {
  for (int i = 0; i < a_len; i++) {
    if (i) SP();
    out_i64(a[i]);
  }
  NL();
}
void out_u64_array(u64 *a, int a_len) {
  for (int i = 0; i < a_len; i++) {
    if (i) SP();
    out_u64(a[i]);
  }
  NL();
}
#pragma endregion io

#pragma region XorShift
const f64 _R_ = 1.0 / 0xffffffffffffffff;
static u64 _xorshift_state_ = 88172645463325252ULL;
u64 next_rand_xorshift(void) {
  _xorshift_state_ = _xorshift_state_ ^ (_xorshift_state_ << 7);
  return _xorshift_state_ = _xorshift_state_ ^ (_xorshift_state_ >> 9);
}
void rand_init_xorshift(u64 seed) {
  _xorshift_state_ += seed;
  (void)next_rand_xorshift();
}
u64 random_range_xorshift(u64 l, u64 r) { /* [l, r] */ return next_rand_xorshift() % (r - l + 1) + l; }
f64 probability_xorshift(void) { return _R_ * next_rand_xorshift(); }
#pragma endregion XorShift

#pragma region m32
typedef uint32_t m32;
m32 _one_m32(u32 mod) { return (u32)-1u % mod + 1; }
m32 _r2_m32(u32 mod) { return (u64)(i64)-1 % mod + 1; }
m32 _inv_m32(u32 mod) {
  u32 inv = mod;
  for (int i = 0; i < 4; ++i) inv *= 2 - inv * mod;
  return inv;
/**
  u32 u = 1, v = 0, x = 1u << 31;
  for (int i = 0; i < 32; i++) {
    if (u & 1) u = (u + mod) >> 1, v = (v >> 1) + x;
    else u >>= 1, v >>= 1;
  }
  return -v;
*/
}
m32 _reduce_m32(u64 a, m32 inv, u32 mod) {
  u32 y = (u32)(a >> 32) - (u32)(((u64)((u32)a * inv) * mod) >> 32);
  return (i32)y < 0 ? y + mod : y;
}
m32 to_m32(u32 a, m32 r2, m32 inv, u32 mod) { return _reduce_m32((u64)a * r2, inv, mod); }
u32 from_m32(m32 A, m32 inv, u32 mod) { return _reduce_m32(A, inv, mod); }
m32 add_m32(m32 A, m32 B, u32 mod) {
  A += B - mod;
  if ((i32)A < 0) A += mod;
  return A;
}
m32 sub_m32(m32 A, m32 B, u32 mod) {
  if ((i32)(A -= B) < 0) A += 2 * mod;
  return A;
}
m32 min_m32(m32 A, u32 mod) { return sub_m32(0u, A, mod); }
m32 mul_m32(m32 A, m32 B, m32 inv, u32 mod) { return _reduce_m32((u64)A * B, inv, mod); }
m32 pow_m32(m32 A, i32 n, m32 inv, u32 mod) {
  m32 ret = _one_m32(mod);
  while (n > 0) {
    if (n & 1) ret = mul_m32(ret, A, inv, mod);
    A = mul_m32(A, A, inv, mod);
    n >>= 1;
  }
  return ret;
}
m32 inv_m32(m32 A, m32 inv, u32 mod) { return pow_m32(A, (i32)mod - 2, inv, mod); }
m32 div_m32(m32 A, m32 B, m32 inv, u32 mod) {
  /* assert(is_prime(mod)); */
  return mul_m32(A, inv_m32(B, inv, mod), inv, mod);
}
m32 in_m32(m32 r2, m32 inv, u32 mod) {
  u32 c, a = 0;
  while (c = getchar_unlocked(), c < 48 || c > 57);
  while (47 < c && c < 58) {
    a = a * 10 + c - 48;
    c = getchar_unlocked();
  }
  return to_m32(a, r2, inv, mod);
}
void out_m32(m32 A, m32 inv, u32 mod) {
  u32 a = from_m32(A, inv, mod);
  out_u32(a);
}
#pragma endregion m32

#pragma region ntt998244353
const u32 mod  = 998244353u;
const m32 r2   = 932051910u;
const m32 inv  = 3296722945u;
const m32 one  = 301989884u;
const m32 rev  = 696254469u;
const m32 gs[]  = { 691295370, 307583142, 566821959, 878217029, 375146819, 138254384, 500602490, 79119218,  790898700, 978335284, 651424567, 308706579, 723000027, 474797508, 683394121, 44141573, 536892010, 945865189, 175417726, 536169764, 831722880, 721458245 };
const m32 igs[] = { 306948983, 888603487, 138723248, 65668869,  842568658, 953245971, 195169681, 118717521, 792052763, 828450244, 908724728, 218560432, 628507989, 248210924, 566568154, 6285593,  82571768,  49985074,  225413092, 349167278, 61514562,  763211248 };
void ntt(m32 *A, int A_len) {
  int h = 0;
  while (A_len > (1 << h)) h++;
  for (int ph = 1; ph <= h; ph++) {
    int w = 1 << (ph - 1);
    int p = 1 << (h - ph);
    m32 now = one;
    for (int s = 0; s < w; s++) {
      int offset = s << (h - ph + 1);
      for (int i = 0; i < p; i++) {
        m32 l = A[i + offset];
        m32 r = mul_m32(A[i + offset + p], now, inv, mod);
        A[i + offset] = add_m32(l, r, mod);
        A[i + offset + p] = sub_m32(l, r, mod);
      }
      now = mul_m32(now, gs[CTZ32(~s)], inv, mod);
    }
  }
}
void intt(m32 *A, int A_len) {
  int h = 0;
  while (A_len > (1 << h)) h++;
  for (int ph = h; ph >= 1; ph--) {
    int w = 1 << (ph - 1);
    int p = 1 << (h - ph);
    m32 inow = one;
    for (int s = 0; s < w; s++) {
      int offset = s << (h - ph + 1);
      for (int i = 0; i < p; i++) {
        m32 l = A[i + offset];
        m32 r = A[i + offset + p];
        A[i + offset] = add_m32(l, r, mod);
        A[i + offset + p] = mul_m32(sub_m32(l, r, mod), inow, inv, mod);
      }
      inow = mul_m32(inow, igs[CTZ32(~s)], inv, mod);
    }
  }
  m32 inv2t = inv_m32(to_m32(A_len, r2, inv, mod), inv, mod);
  for (int i = 0; i < A_len; i++) A[i] = mul_m32(A[i], inv2t, inv, mod);
}
#pragma endregion ntt998244353

#pragma region cipolla
m32 sqrt_mod998244353(m32 Y) {
  if (Y == 0 || Y == one) return Y;
  if (pow_m32(Y, mod >> 1u, inv, mod) != one) return 0;
  m32 c, cc;
  while (1) {
    c = to_m32(random_range_xorshift(1, mod - 1), r2, inv, mod);
    cc = sub_m32(mul_m32(c, c, inv, mod), Y, mod);
    if (pow_m32(cc, (mod - 1) >> 1u, inv, mod) == rev) break;
  }
  m32 ta, tb;
  m32 aa = c, bb = one, a = one, b = 0;
  m32 theta2 = cc;
  int i = (mod >> 1u) + 1;
  while (i) {
    if (i & 1) {
      ta = add_m32(mul_m32(a, aa, inv, mod), mul_m32(mul_m32(b, bb, inv, mod), theta2, inv, mod), mod);
      tb = add_m32(mul_m32(a, bb, inv, mod), mul_m32(b, aa, inv, mod), mod);
      a = ta;
      b = tb;
    }
    ta = add_m32(mul_m32(aa, aa, inv, mod), mul_m32(mul_m32(bb, bb, inv, mod), theta2, inv, mod), mod);
    tb = mul_m32(aa, mul_m32(bb, to_m32(2, r2, inv, mod), inv, mod), inv, mod);
    aa = ta;
    bb = tb;
    i >>= 1;
  }
  return a;
}
#pragma endregion cipolla

#pragma region fps998244353
m32 _fact[1<<20];
m32 _inv_fact[1<<20];
m32 _inv_table[1<<20];
void pre_fact(int n) {
  _fact[0] = one;
  for (int i = 0; i <= n + 1; i++) _fact[i + 1] = mul_m32(_fact[i], to_m32(i + 1, r2, inv, mod), inv, mod);
  _inv_fact[n + 2] = inv_m32(_fact[n + 2], inv, mod);
  for (int i = n + 2; i > 0; i--) _inv_fact[i - 1] = mul_m32(_inv_fact[i], to_m32(i, r2, inv, mod), inv, mod);
  for (int i = 1; i <= n + 1; i++) _inv_table[i] = mul_m32(_inv_fact[i], _fact[i - 1], inv, mod);
}
m32 *fps_rev(m32 *A, int A_len) {
  m32 *ret = (m32 *)calloc(A_len, sizeof(m32));
  #ifdef LOCAL
  if (ret == NULL) exit(EXIT_FAILURE);
  #endif
  for (int i = 0, j = A_len - 1; i < A_len; i++, j--) ret[i] = A[j];
  return ret;
}
m32 *fps_pre(m32 *A, int A_len, int deg) {
  m32 *ret = (m32 *)calloc(MIN(A_len, deg), sizeof(m32));
  #ifdef LOCAL
  if (ret == NULL) exit(EXIT_FAILURE);
  #endif
  for (int i = 0; i < MIN(A_len, deg); i++) ret[i] = A[i];
  return ret;
}
m32 *fps_mul(m32 *A, int A_len, m32 *B, int B_len, int deg) {
  int C_len = BIT_CEIL32(A_len + B_len - 1);
  m32 *C = (m32 *)calloc(C_len, sizeof(m32));
  m32 *D = (m32 *)calloc(C_len, sizeof(m32));
  m32 *ret = (m32 *)calloc(deg, sizeof(m32));
  #ifdef LOCAL
  if (C == NULL || D == NULL || ret == NULL) exit(EXIT_FAILURE);
  #endif
  for (int i = 0; i < A_len; i++) C[i] = A[i];
  for (int i = 0; i < B_len; i++) D[i] = B[i];
  ntt(C, C_len);
  ntt(D, C_len);
  for (int i = 0; i < C_len; i++) C[i] = mul_m32(C[i], D[i], inv, mod);
  intt(C, C_len);
  for (int i = 0; i < deg; i++) ret[i] = C[i];
  free(C);
  free(D);
  return ret;
}
m32 *fps_inv(m32 *A, int A_len, int deg) {
  m32 *ret = (m32 *)calloc(deg, sizeof(m32));
  m32 *work0 = (m32 *)calloc(3 << 19, sizeof(m32));
  m32 *work1 = (m32 *)calloc(3 << 19, sizeof(m32));
  #ifdef LOCAL
  if (ret == NULL || work0 == NULL || work1 == NULL) exit(EXIT_FAILURE);
  #endif
  ret[0] = inv_m32(A[0], inv, mod);
  for (int m = 1; m < deg; m <<= 1) {
    int mx2 = m << 1;
    for (int i = 0; i < MIN(mx2, A_len); i++) work0[i] = A[i];
    for (int i = MIN(mx2, A_len); i < mx2; i++) work0[i] = 0;
    for (int i = 0; i < MIN(mx2, deg); i++) work1[i] = ret[i];
    for (int i = MIN(mx2, deg); i < mx2; i++) work1[i] = 0;
    ntt(work0, mx2);
    ntt(work1, mx2);
    for (int i = 0; i < mx2; i++) work0[i] = mul_m32(work0[i], work1[i], inv, mod);
    intt(work0, mx2);
    for (int i = 0; i < m; i++) work0[i] = 0;
    ntt(work0, mx2);
    for (int i = 0; i < mx2; i++) work0[i] = mul_m32(work0[i], work1[i], inv, mod);
    intt(work0, mx2);
    for (int i = m, i0 = MIN(mx2, deg); i < i0; i++) ret[i] = min_m32(work0[i], mod);
  }
  free(work0);
  free(work1);
  return ret;
}
m32 *fps_exp(m32 *A, int A_len, int deg) {
  if (deg == 1) {
    m32 *ret = (m32 *)calloc(1, sizeof(m32));
    #ifdef LOCAL
    if (ret == NULL) exit(EXIT_FAILURE);
    #endif
    ret[0] = one;
    return ret;
  }
  if (deg == 2) {
    m32 *ret = (m32 *)calloc(2, sizeof(m32));
    #ifdef LOCAL
    if (ret == NULL) exit(EXIT_FAILURE);
    #endif
    ret[0] = one;
    ret[1] = 1 < A_len ? A[1]: 0;
    return ret;
  }
  m32 *ret = (m32 *)calloc(deg, sizeof(m32));
  m32 *work0 = (m32 *)calloc(1u << 20, sizeof(m32));
  m32 *work1 = (m32 *)calloc(1u << 20, sizeof(m32));
  m32 *work2 = (m32 *)calloc(1u << 20, sizeof(m32));
  m32 *work3 = (m32 *)calloc(1u << 20, sizeof(m32));
  #ifdef LOCAL
  if (work0 == NULL || work1 == NULL || work2 == NULL || work3 == NULL || ret == NULL) exit(EXIT_FAILURE);
  #endif
  ret[0] = work1[0] = work1[1] = work2[0] = one;
  int m;
  for (m = 1; (m << 1) < deg; m <<= 1) {
    int mx2 = m << 1;
    for (int i = 0, i0 = MIN(m, A_len); i < i0; i++) work0[i] = mul_m32(to_m32(i, r2, inv, mod), A[i], inv, mod);
    for (int i = MIN(m, A_len); i < m; i++) work0[i] = 0;
    ntt(work0, m);
    for (int i = 0; i < m; i++) work0[i] = mul_m32(work0[i], work1[i], inv, mod);
    intt(work0, m);
    for (int i = 0; i < m; i++) work0[i] = sub_m32(work0[i], mul_m32(to_m32(i, r2, inv, mod), ret[i], inv, mod), mod);
    for (int i = m; i < mx2; i++) work0[i] = 0;
    ntt(work0, mx2);
    for (int i = 0; i < m; i++) work3[i] = work2[i];
    for (int i = m; i < mx2; i++) work3[i] = 0;
    ntt(work3, mx2);
    for (int i = 0; i < mx2; i++) work0[i] = mul_m32(work0[i], work3[i], inv, mod);
    intt(work0, mx2);
    for (int i = 0; i < m; i++) work0[i] = mul_m32(work0[i], _inv_table[m + i], inv, mod);
    for (int i = 0, i0 = MIN(m, A_len - m); i < i0; i++) work0[i] = add_m32(work0[i], A[m + i], mod);
    for (int i = m; i < mx2; i++) work0[i] = 0;
    ntt(work0, mx2);
    for (int i = 0; i < mx2; i++) work0[i] = mul_m32(work0[i], work1[i], inv, mod);
    intt(work0, mx2);
    for (int i = m; i < mx2; i++) ret[i] = work0[i - m];
    for (int i = 0; i < mx2; i++) work1[i] = ret[i];
    for (int i = mx2; i < (m << 2); i++) work1[i] = 0;
    ntt(work1, m << 2);
    for (int i = 0; i < mx2; i++) work0[i] = mul_m32(work1[i], work3[i], inv, mod);
    intt(work0, mx2);
    for (int i = 0; i < m; i++) work0[i] = 0;
    ntt(work0, mx2);
    for (int i = 0; i < mx2; i++) work0[i] = mul_m32(work0[i], work3[i], inv, mod);
    intt(work0, mx2);
    for (int i = m; i < mx2; i++) work2[i] = min_m32(work0[i], mod);
  }
  int mx2 = m << 1, mhalf = m >> 1;
  for (int i = 0, i0 = MIN(m, A_len); i < i0; i++) work0[i] = mul_m32(to_m32(i, r2, inv, mod), A[i], inv, mod);
  for (int i = MIN(m, A_len); i < m; i++) work0[i] = 0;
  ntt(work0, m);
  for (int i = 0; i < m; i++) work0[i] = mul_m32(work0[i], work1[i], inv, mod);
  intt(work0, m);
  for (int i = 0; i < m; i++) work0[i] = sub_m32(work0[i], mul_m32(to_m32(i, r2, inv, mod), ret[i], inv, mod), mod);
  for (int i = m; i < m + mhalf; i++) work0[i] = work0[i - mhalf];
  for (int i = mhalf; i < m; i++) work0[i] = 0;
  for (int i = m + mhalf; i < mx2; i++) work0[i] = 0;
  ntt(work0, m);
  ntt(work0 + m, m);
  for (int i = m; i < m + mhalf; i++) work3[i] = work2[i - mhalf];
  for (int i = m + mhalf; i < mx2; i++) work3[i] = 0;
  ntt(work3 + m, m);
  for (int i = 0; i < m; i++) work0[m + i] = add_m32(mul_m32(work0[i], work3[m + i], inv, mod), mul_m32(work0[m + i], work3[i], inv, mod), mod);
  for (int i = 0; i < m; i++) work0[i] = mul_m32(work0[i], work3[i], inv, mod);
  intt(work0, m);
  intt(work0 + m, m);
  for (int i = 0; i < mhalf; i++) work0[mhalf + i] = add_m32(work0[mhalf + i], work0[m + i], mod);
  for (int i = 0; i < m; i++) work0[i] = mul_m32(work0[i], _inv_table[m + i], inv, mod);
  for (int i = 0, i0 = MIN(m, A_len - m); i < i0; i++) work0[i] = add_m32(work0[i], A[m + i], mod);
  for (int i = m; i < mx2; i++) work0[i] = 0;
  ntt(work0, mx2);
  for (int i = 0; i < mx2; i++) work0[i] = mul_m32(work0[i], work1[i], inv, mod);
  intt(work0, mx2);
  for (int i = m; i < deg; i++) ret[i] = work0[i - m];
  free(work0);
  free(work1);
  free(work2);
  free(work3);
  return ret;
}
m32 *fps_div(m32 *A, int A_len, m32 *B, int B_len, int deg) {
  if (deg == 1) {
    m32 *ret = (m32 *)calloc(1, sizeof(m32));
    #ifdef LOCAL
    if (ret == NULL) exit(EXIT_FAILURE);
    #endif
    ret[0] = div_m32(A[0], B[0], inv, mod);
    return ret;
  }
  const int m = 1 << (31 - __builtin_clz(deg - 1));
  int mm = m << 1;
  m32 *ret   = (m32 *)calloc(deg, sizeof(m32));
  m32 *work0 = (m32 *)calloc(1 << 20, sizeof(m32));
  m32 *work1 = (m32 *)calloc(1 << 20, sizeof(m32));
  m32 *work2 = (m32 *)calloc(mm, sizeof(m32));
  #ifdef LOCAL
  if (work0 == NULL || work1 == NULL || work2 == NULL || ret == NULL) exit(EXIT_FAILURE);
  #endif
  m32 *work3 = fps_inv(B, B_len, m);
  for (int i = 0; i < m; i++) work2[i] = work3[i];
  free(work3);
  for (int i = m; i < mm; i++) work2[i] = 0;
  ntt(work2, mm);
  for (int i = 0; i < MIN(m, A_len); i++) work0[i] = A[i];
  for (int i = MIN(m, A_len); i < mm; i++) work0[i] = 0;
  ntt(work0, mm);
  for (int i = 0; i < mm; i++) work0[i] = mul_m32(work0[i], work2[i], inv, mod);
  intt(work0, mm);
  for (int i = 0; i < m; i++) ret[i] = work0[i];
  for (int i = m; i < mm; i++) work0[i] = 0;
  ntt(work0, mm);
  for (int i = 0; i < MIN(mm, B_len); i++) work1[i] = B[i];
  for (int i = MIN(mm, B_len); i < mm; i++) work1[i] = 0;
  ntt(work1, mm);
  for (int i = 0; i < mm; i++) work0[i] = mul_m32(work0[i], work1[i], inv, mod);
  intt(work0, mm);
  for (int i = 0; i < m; i++) work0[i] = 0;
  for (int i = m, i0 = MIN(mm, A_len); i < i0; i++) work0[i] = sub_m32(work0[i], A[i], mod);
  ntt(work0, mm);
  for (int i = 0; i < mm; i++) work0[i] = mul_m32(work0[i], work2[i], inv, mod);
  intt(work0, mm);
  for (int i = m; i < deg; i++) ret[i] = min_m32(work0[i], mod);
  free(work0);
  free(work1);
  free(work2);
  return ret;
}
m32 *fps_log(m32 *A, int A_len, int deg) {
  m32 *work = (m32 *)calloc(MIN(A_len, deg), sizeof(m32));
  #ifdef LOCAL
  if (work == NULL) exit(EXIT_FAILURE);
  #endif
  for (int i = 0; i < MIN(A_len, deg); i++) work[i] = mul_m32(A[i], to_m32(i, r2, inv, mod), inv, mod);
  m32 *ret = fps_div(work, MIN(A_len, deg), A, A_len, deg);
  for (int i = 1; i < deg; i++) ret[i] = mul_m32(ret[i], _inv_table[i], inv, mod);
  free(work);
  return ret;
}
m32 *fps_pow(m32 *A, int A_len, i64 indx, int deg) {
  if (A == NULL || A_len == 0 || A[0] != one) {
    if (indx == 0) {
      m32 *ret = (m32 *)calloc(deg, sizeof(m32));
      #ifdef LOCAL
      if (ret == NULL) exit(EXIT_FAILURE);
      #endif
      ret[0] = one;
      return ret;
    }
    int o;
    for (o = 0; o < A_len; o++) {
      if (A[o]) break;
      if (o == A_len - 1) {
        o = -1;
        break;
      }
    }
    if (o == -1 || o > (deg - 1) / indx) {
      m32 *ret = (m32 *)calloc(deg, sizeof(m32));
      return ret;
    }
    const m32 b = inv_m32(A[o], inv, mod);
    const m32 c = pow_m32(A[o], indx, inv, mod);
    const int d = MIN(deg - indx * o, A_len - o);
    m32 *work0 = (m32 *)calloc(d, sizeof(m32));
    #ifdef LOCAL
    if (work0 == NULL) exit(EXIT_FAILURE);
    #endif
    for (int i = 0; i < d; i++) work0[i] = mul_m32(b, A[o + i], inv, mod);
    m32 *work1 = fps_pow(work0, d, indx, deg - indx * o);
    m32 *ret = (m32 *)calloc(deg, sizeof(m32));
    #ifdef LOCAL
    if (ret == NULL) exit(EXIT_FAILURE);
    #endif
    for (int i = 0; i < deg - indx * o; i++) ret[indx * o + i] = mul_m32(c, work1[i], inv, mod);
    free(work0);
    free(work1);
    return ret;
  }
  m32 *work2 = fps_log(A, A_len, deg);
  for (int i = 0; i < deg; i++) work2[i] = mul_m32(work2[i], to_m32(indx, r2, inv, mod), inv, mod);
  return fps_exp(work2, deg, deg);
}
m32 *fps_taylor_shift(m32 *A, int A_len, m32 c) {
  m32 *ret = (m32 *)calloc(A_len, sizeof(m32));
  m32 *work0 = (m32 *)calloc(A_len, sizeof(m32));
  #ifdef LOCAL
  if (ret == NULL || work0 == NULL) exit(EXIT_FAILURE);
  #endif
  for (int i = 0; i < A_len; i++) ret[A_len - 1 - i] = mul_m32(A[i], _fact[i], inv, mod);
  work0[0] = one;
  for (int i = 1; i < A_len; i++) work0[i] = mul_m32(mul_m32(mul_m32(work0[i - 1], c, inv, mod), _inv_fact[i], inv, mod), _fact[i - 1], inv, mod);
  m32 *work1 = fps_mul(ret, A_len, work0, A_len, (A_len << 1) - 1);
  for (int i = 0; i < A_len; i++) ret[i] = mul_m32(work1[A_len - 1 - i], _inv_fact[i], inv, mod);
  free(work0);
  free(work1);
  return ret;
}
m32 *fps_quo(m32 *A, int A_len, m32 *B, int B_len) {
  if (A_len < B_len) return NULL;
  int n = A_len - B_len + 1;
  /**
  if (B_len <= 64) {
    int b_len = B_len;
    while (b_len > 0 && B[b_len - 1] == 0) --b_len;
    m32 *ret = (m32 *)calloc(n, sizeof(m32));
    m32 *work0 = (m32 *)calloc(A_len, sizeof(m32));
    m32 *work1 = (m32 *)calloc(b_len, sizeof(m32));
    m32 *work2 = (m32 *)calloc(A_len - b_len + 1, sizeof(m32));
    #ifdef LOCAL
    if (ret == NULL || work0 == NULL || work1 == NULL || work2 == NULL) exit(EXIT_FAILURE);
    #endif
    m32 coef = inv_m32(B[b_len - 1], inv, mod);
    for (int i = 0; i < A_len; i++) work0[i] = A[i];
    for (int i = 0; i < b_len; i++) work1[i] = mul_m32(B[i], coef, inv, mod);
    for (int i = A_len - b_len; i >= 0; i--) {
      work2[i] = work0[i + b_len - 1];
      for (int j = 0; j < b_len; j++) work0[i + j] = sub_m32(work0[i + j], mul_m32(work2[i], work1[j], inv, mod), mod);
    }
    for (int i = 0; i < A_len - b_len + 1; i++) ret[i] = mul_m32(work2[i], coef, inv, mod);
    if (A_len - b_len + 1 < n) for (int i = A_len - b_len + 1; i < n; i++) ret[i] = 0;
    return ret;
  }
  */
  return fps_rev(fps_mul(fps_pre(fps_rev(A, A_len), A_len, n), n, fps_inv(fps_rev(B, B_len), B_len, n), n, n), n);
}
m32 *fps_rem(m32 *A, int A_len, m32 *B, int B_len) {
  m32 *work0 = fps_quo(A, A_len, B, B_len);
  if (work0 == NULL) {
    m32 *ret = (m32 *)calloc(A_len + 1, sizeof(m32));
    #ifdef LOCAL
    if (ret == NULL) exit(EXIT_FAILURE);
    #endif
    ret[0] = A_len;
    for (int i = 1; i <= A_len; i++) ret[i] = A[i - 1];
    free(work0);
    return ret;
  }
  m32 *work1 = fps_mul(work0, A_len - B_len + 1, B, B_len, A_len);
  m32 *work2 = (m32 *)calloc(A_len, sizeof(m32));
  #ifdef LOCAL
  if (work2 == NULL) exit(EXIT_FAILURE);
  #endif
  for (int i = 0; i < A_len; i++) work2[i] = sub_m32(A[i], work1[i], mod);
  int ret_len = A_len;
  while (ret_len > 0 && work2[ret_len - 1] == 0) ret_len--;
  m32 *ret = (m32 *)calloc(ret_len + 1, sizeof(m32));
  #ifdef LOCAL
  if (ret == NULL) exit(EXIT_FAILURE);
  #endif
  ret[0] = ret_len;
  for (int i = 1; i <= ret_len; i++) ret[i] = work2[i - 1];
  free(work0);
  free(work1);
  free(work2);
  return ret;
}
#pragma endregion fps998244353

void Main(void) {
  int n = read_int();
  pre_fact(500002);
  m32 A[500001];
  for (int i = 0; i <= n; i++) A[i] = _inv_fact[i + 1];
  m32 *ret = fps_inv(A, n + 1, n + 1);
  for (int i = 0; i <= n; i++) {
    if (i) SP();
    out_m32(mul_m32(ret[i], _fact[i], inv, mod), inv, mod);
  }
  NL();
}

int main(void) {
  Main();
  return 0;
}