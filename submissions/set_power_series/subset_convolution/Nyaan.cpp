#define NDEBUG

#line 1 "verify\verify-yosupo-math\yosupo-subset-convolution-fast.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"
//
#include <immintrin.h>
//
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

#line 2 "misc\fastio.hpp"

#line 4 "misc\fastio.hpp"
#include <type_traits>
#line 6 "misc\fastio.hpp"

using namespace std;

namespace fastio {
static constexpr int SZ = 1 << 17;
char inbuf[SZ], outbuf[SZ];
int in_left = 0, in_right = 0, out_right = 0;

struct Pre {
  char num[40000];
  constexpr Pre() : num() {
    for (int i = 0; i < 10000; i++) {
      int n = i;
      for (int j = 3; j >= 0; j--) {
        num[i * 4 + j] = n % 10 + '0';
        n /= 10;
      }
    }
  }
} constexpr pre;

inline void load() {
  int len = in_right - in_left;
  memmove(inbuf, inbuf + in_left, len);
  in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
  in_left = 0;
}

inline void flush() {
  fwrite(outbuf, 1, out_right, stdout);
  out_right = 0;
}

inline void skip_space() {
  if (in_left + 32 > in_right) load();
  while (inbuf[in_left] <= ' ') in_left++;
}

inline void rd(char& c) {
  if (in_left + 32 > in_right) load();
  c = inbuf[in_left++];
}
template <typename T>
inline void rd(T& x) {
  if (in_left + 32 > in_right) load();
  char c;
  do c = inbuf[in_left++];
  while (c < '-');
  [[maybe_unused]] bool minus = false;
  if constexpr (is_signed<T>::value == true) {
    if (c == '-') minus = true, c = inbuf[in_left++];
  }
  x = 0;
  while (c >= '0') {
    x = x * 10 + (c & 15);
    c = inbuf[in_left++];
  }
  if constexpr (is_signed<T>::value == true) {
    if (minus) x = -x;
  }
}
inline void rd() {}
template <typename Head, typename... Tail>
inline void rd(Head& head, Tail&... tail) {
  rd(head);
  rd(tail...);
}

inline void wt(char c) {
  if (out_right > SZ - 32) flush();
  outbuf[out_right++] = c;
}
inline void wt(bool b) {
  if (out_right > SZ - 32) flush();
  outbuf[out_right++] = b ? '1' : '0';
}
template <typename T>
inline void wt(T x) {
  if (out_right > SZ - 32) flush();
  if (!x) {
    outbuf[out_right++] = '0';
    return;
  }
  if constexpr (is_signed<T>::value == true) {
    if (x < 0) outbuf[out_right++] = '-', x = -x;
  }
  int i = 12;
  char buf[16];
  while (x >= 10000) {
    memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
    x /= 10000;
    i -= 4;
  }
  if (x < 100) {
    if (x < 10) {
      outbuf[out_right] = '0' + x;
      ++out_right;
    } else {
      uint32_t q = (uint32_t(x) * 205) >> 11;
      uint32_t r = uint32_t(x) - q * 10;
      outbuf[out_right] = '0' + q;
      outbuf[out_right + 1] = '0' + r;
      out_right += 2;
    }
  } else {
    if (x < 1000) {
      memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
      out_right += 3;
    } else {
      memcpy(outbuf + out_right, pre.num + (x << 2), 4);
      out_right += 4;
    }
  }
  memcpy(outbuf + out_right, buf + i + 4, 12 - i);
  out_right += 12 - i;
}
inline void wt() {}
template <typename Head, typename... Tail>
inline void wt(Head&& head, Tail&&... tail) {
  wt(head);
  wt(forward<Tail>(tail)...);
}
template <typename... Args>
inline void wtn(Args&&... x) {
  wt(forward<Args>(x)...);
  wt('\n');
}

struct Dummy {
  Dummy() { atexit(flush); }
} dummy;

}  // namespace fastio
using fastio::rd;
using fastio::skip_space;
using fastio::wt;
using fastio::wtn;
#line 20 "verify\verify-yosupo-math\yosupo-subset-convolution-fast.test.cpp"
//
#line 2 "modint\montgomery-modint.hpp"



template <uint32_t mod>
struct LazyMontgomeryModInt {
  using mint = LazyMontgomeryModInt;
  using i32 = int32_t;
  using u32 = uint32_t;
  using u64 = uint64_t;

  static constexpr u32 get_r() {
    u32 ret = mod;
    for (i32 i = 0; i < 4; ++i) ret *= 2 - mod * ret;
    return ret;
  }

  static constexpr u32 r = get_r();
  static constexpr u32 n2 = -u64(mod) % mod;
  static_assert(r * mod == 1, "invalid, r * mod != 1");
  static_assert(mod < (1 << 30), "invalid, mod >= 2 ^ 30");
  static_assert((mod & 1) == 1, "invalid, mod % 2 == 0");

  u32 a;

  constexpr LazyMontgomeryModInt() : a(0) {}
  constexpr LazyMontgomeryModInt(const int64_t &b)
      : a(reduce(u64(b % mod + mod) * n2)){};

  static constexpr u32 reduce(const u64 &b) {
    return (b + u64(u32(b) * u32(-r)) * mod) >> 32;
  }

  constexpr mint &operator+=(const mint &b) {
    if (i32(a += b.a - 2 * mod) < 0) a += 2 * mod;
    return *this;
  }

  constexpr mint &operator-=(const mint &b) {
    if (i32(a -= b.a) < 0) a += 2 * mod;
    return *this;
  }

  constexpr mint &operator*=(const mint &b) {
    a = reduce(u64(a) * b.a);
    return *this;
  }

  constexpr mint &operator/=(const mint &b) {
    *this *= b.inverse();
    return *this;
  }

  constexpr mint operator+(const mint &b) const { return mint(*this) += b; }
  constexpr mint operator-(const mint &b) const { return mint(*this) -= b; }
  constexpr mint operator*(const mint &b) const { return mint(*this) *= b; }
  constexpr mint operator/(const mint &b) const { return mint(*this) /= b; }
  constexpr bool operator==(const mint &b) const {
    return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
  }
  constexpr bool operator!=(const mint &b) const {
    return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
  }
  constexpr mint operator-() const { return mint() - mint(*this); }

  constexpr mint pow(u64 n) const {
    mint ret(1), mul(*this);
    while (n > 0) {
      if (n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }
  
  constexpr mint inverse() const { return pow(mod - 2); }

  friend ostream &operator<<(ostream &os, const mint &b) {
    return os << b.get();
  }

  friend istream &operator>>(istream &is, mint &b) {
    int64_t t;
    is >> t;
    b = LazyMontgomeryModInt<mod>(t);
    return (is);
  }
  
  constexpr u32 get() const {
    u32 ret = reduce(a);
    return ret >= mod ? ret - mod : ret;
  }

  static constexpr u32 get_mod() { return mod; }
};
#line 22 "verify\verify-yosupo-math\yosupo-subset-convolution-fast.test.cpp"
//
#line 2 "math-fast\subset-convolution.hpp"

#line 2 "modint\vectorize-modint.hpp"

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")

using m256 = __m256i;
struct alignas(32) mmint {
  m256 x;
  static mmint R, mR, M0, M1, M2, N2;

  mmint() : x() {}
  inline mmint(const m256& _x) : x(_x) {}
  inline mmint(int a) : x(_mm256_set1_epi32(a)) {}
  inline mmint(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
      : x(_mm256_set_epi32(a7, a6, a5, a4, a3, a2, a1, a0)) {}
  inline operator m256&() { return x; }
  inline operator const m256&() const { return x; }
  inline int& operator[](int i) { return *(reinterpret_cast<int*>(&x) + i); }
  inline const int& operator[](int i) const {
    return *(reinterpret_cast<const int*>(&x) + i);
  }

  friend ostream& operator<<(ostream& os, const mmint& m) {
    unsigned r = R[0], mod = M1[0];
    auto reduce1 = [&](const uint64_t& b) {
      unsigned res = (b + uint64_t(unsigned(b) * unsigned(-r)) * mod) >> 32;
      return res >= mod ? res - mod : res;
    };
    for (int i = 0; i < 8; i++) {
      os << reduce1(m[i]) << (i == 7 ? "" : " ");
    }
    return os;
  }

  template <typename mint>
  static void set_mod() {
    R = _mm256_set1_epi32(mint::r);
    M0 = _mm256_setzero_si256();
    M1 = _mm256_set1_epi32(mint::get_mod());
    M2 = _mm256_set1_epi32(mint::get_mod() * 2);
    N2 = _mm256_set1_epi32(mint::n2);
    mR = _mm256_sub_epi32(M0, R);
  }

  static inline mmint reduce(const mmint& prod02, const mmint& prod13) {
    m256 unpalo = _mm256_unpacklo_epi32(prod02, prod13);
    m256 unpahi = _mm256_unpackhi_epi32(prod02, prod13);
    m256 prodlo = _mm256_unpacklo_epi64(unpalo, unpahi);
    m256 prodhi = _mm256_unpackhi_epi64(unpalo, unpahi);
    m256 hiplm1 = _mm256_add_epi32(prodhi, M1);
    m256 lomulr = _mm256_mullo_epi32(prodlo, R);
    m256 a13 = _mm256_shuffle_epi32(lomulr, 0xF5);
    m256 prod02_ = _mm256_mul_epu32(lomulr, M1);
    m256 prod13_ = _mm256_mul_epu32(a13, M1);
    m256 unpalo_ = _mm256_unpacklo_epi32(prod02_, prod13_);
    m256 unpahi_ = _mm256_unpackhi_epi32(prod02_, prod13_);
    m256 prod = _mm256_unpackhi_epi64(unpalo_, unpahi_);
    return _mm256_sub_epi32(hiplm1, prod);
  }

  static inline mmint itom(const mmint& A) { return A * N2; }

  static inline mmint mtoi(const mmint& A) {
    m256 lomulr = _mm256_mullo_epi32(A, mR);
    m256 a13 = _mm256_shuffle_epi32(lomulr, 0xF5);
    m256 prod02_ = _mm256_mul_epu32(lomulr, M1);
    m256 prod13_ = _mm256_mul_epu32(a13, M1);
    m256 unpalo_ = _mm256_unpacklo_epi32(prod02_, prod13_);
    m256 unpahi_ = _mm256_unpackhi_epi32(prod02_, prod13_);
    m256 prod = _mm256_unpackhi_epi64(unpalo_, unpahi_);
    m256 cmpA = _mm256_cmpgt_epi32(A, M0);
    m256 addA = _mm256_and_si256(cmpA, mmint(1));
    return _mm256_add_epi32(prod, addA);
  }

  friend inline mmint operator+(const mmint& A, const mmint& B) {
    m256 apb = _mm256_add_epi32(A, B);
    m256 ret = _mm256_sub_epi32(apb, M2);
    m256 cmp = _mm256_cmpgt_epi32(M0, ret);
    m256 add = _mm256_and_si256(cmp, M2);
    return _mm256_add_epi32(add, ret);
  }

  friend inline mmint operator-(const mmint& A, const mmint& B) {
    m256 ret = _mm256_sub_epi32(A, B);
    m256 cmp = _mm256_cmpgt_epi32(M0, ret);
    m256 add = _mm256_and_si256(cmp, M2);
    return _mm256_add_epi32(add, ret);
  }

  friend inline mmint operator*(const mmint& A, const mmint& B) {
    m256 a13 = _mm256_shuffle_epi32(A, 0xF5);
    m256 b13 = _mm256_shuffle_epi32(B, 0xF5);
    m256 prod02 = _mm256_mul_epu32(A, B);
    m256 prod13 = _mm256_mul_epu32(a13, b13);
    return reduce(prod02, prod13);
  }

  inline mmint& operator+=(const mmint& A) { return (*this) = (*this) + A; }
  inline mmint& operator-=(const mmint& A) { return (*this) = (*this) - A; }
  inline mmint& operator*=(const mmint& A) { return (*this) = (*this) * A; }

  bool operator==(const mmint& A) {
    m256 sub = _mm256_sub_epi32(x, A.x);
    return _mm256_testz_si256(sub, sub) == 1;
  }
  bool operator!=(const mmint& A) { return !((*this) == A); }
};
__attribute__((aligned(32))) mmint mmint::R, mmint::mR;
__attribute__((aligned(32))) mmint mmint::M0, mmint::M1, mmint::M2, mmint::N2;

/**
 * @brief vectorize modint
 */
#line 4 "math-fast\subset-convolution.hpp"

template <typename mint>
vector<mint> fast_multiply(const vector<mint>& a, const vector<mint>& b) {
  int n = a.size();
  int d = __builtin_ctz(n);
  assert(n <= 23);
  mmint* a1 = new mmint[max(n, 8) * 3];
  mmint* a2 = new mmint[max(n, 8) * 3];
  memset((void*)a1, 0, max(n, 8) * 3 * sizeof(mmint));
  memset((void*)a2, 0, max(n, 8) * 3 * sizeof(mmint));
  mmint b1[24], b2[24], b3[24];

  for (int i = 0; i < n; i++) {
    unsigned int pc = __builtin_popcount(i);
    a1[i * 3 + pc / 8][pc % 8] = a[i].a;
    a2[i * 3 + pc / 8][pc % 8] = b[i].a;
  }

  for (int j = 2; j <= n; j += 2) {
    unsigned int pc = __builtin_popcount(j);
    unsigned int ctz = __builtin_ctz(j);
    for (int h = 0; h < d; h++) {
      if (j & (1 << h)) break;
      int li = j - 2 * (1 << h), ri = j - (1 << h);
      if (pc + ctz <= 16) {
        for (int i = 0; i < 3 * (1 << h); i += 3) {
          a1[ri * 3 + i + 0] += a1[li * 3 + i + 0];
          a2[ri * 3 + i + 0] += a2[li * 3 + i + 0];
          a1[ri * 3 + i + 1] += a1[li * 3 + i + 1];
          a2[ri * 3 + i + 1] += a2[li * 3 + i + 1];
        }
      } else {
        for (int i = 0; i < 3 * (1 << h); i++) {
          a1[ri * 3 + i] += a1[li * 3 + i];
          a2[ri * 3 + i] += a2[li * 3 + i];
        }
      }
    }
  }

  mmint th = _mm256_set1_epi64x(4LL * mmint::M1[0] * mmint::M1[0]);

  for (int is = 0; is < n; is += 8) {
    int mpc = d;

    for (int i = is; i < is + 8; i++) {
      int pc = __builtin_popcount(i);
      mpc = min(mpc, pc);
      int mod = mmint::M1[0];
      for (int j = 0; j <= d; j++) {
        b1[j][i - is] = a1[i * 3 + j / 8][j % 8];
        b2[j][i - is] = a2[i * 3 + j / 8][j % 8];
        b3[j][i - is] = 0;
      }
    }

    for (int j = 0; j <= d; j++) {
      m256 cmpB1 = _mm256_cmpgt_epi32(mmint::M1, b1[j]);
      m256 cmpB2 = _mm256_cmpgt_epi32(mmint::M1, b2[j]);
      m256 difB1 = _mm256_andnot_si256(cmpB1, mmint::M1);
      m256 difB2 = _mm256_andnot_si256(cmpB2, mmint::M1);
      b1[j] = _mm256_sub_epi32(b1[j], difB1);
      b2[j] = _mm256_sub_epi32(b2[j], difB2);
    }

    for (int l = mpc; l <= d; l++) {
#define PROD(k)                                             \
  m256 A13##k = _mm256_shuffle_epi32(b1[j + k], 0xF5);      \
  m256 B13##k = _mm256_shuffle_epi32(b2[l - j - k], 0xF5);  \
  m256 p02##k = _mm256_mul_epi32(b1[j + k], b2[l - j - k]); \
  m256 p13##k = _mm256_mul_epi32(A13##k, B13##k);           \
  prod02 = _mm256_add_epi64(prod02, p02##k);                \
  prod13 = _mm256_add_epi64(prod13, p13##k)
#define COMP()                                   \
  do {                                           \
    m256 cmp02 = _mm256_cmpgt_epi64(prod02, th); \
    m256 cmp13 = _mm256_cmpgt_epi64(prod13, th); \
    m256 dif02 = _mm256_and_si256(cmp02, th);    \
    m256 dif13 = _mm256_and_si256(cmp13, th);    \
    prod02 = _mm256_sub_epi64(prod02, dif02);    \
    prod13 = _mm256_sub_epi64(prod13, dif13);    \
  } while (0)

      int j = 0;
      mmint prod02 = mmint::M0, prod13 = mmint::M0;

      for (; j <= l - 3; j += 4) {
        PROD(0);
        PROD(1);
        PROD(2);
        PROD(3);
        COMP();
      }
      for (; j <= l; j++) {
        PROD(0);
      }
      COMP();
      b3[l] = mmint::reduce(prod02, prod13);
    }

#undef PROD
#undef COMP

    for (int i = is; i < is + 8; i++) {
      for (unsigned j = mpc; j <= unsigned(d); j++) {
        a1[i * 3 + j / 8][j % 8] = b3[j][i - is];
      }
    }
  }

  for (int j = 2; j <= n; j += 2) {
    for (int h = 0; h < d; h++) {
      if (j & (1 << h)) break;
      int li = j - 2 * (1 << h), ri = j - (1 << h);
      for (int i = 0; i < 3 * (1 << h); i++) {
        a1[ri * 3 + i] -= a1[li * 3 + i];
      }
    }
  }

  vector<mint> c(n);
  for (int i = 0; i < n; i++) {
    unsigned int pc = __builtin_popcount(i);
    c[i].a = a1[i * 3 + pc / 8][pc % 8];
  }

  delete[](a1);
  delete[](a2);
  return c;
}
#line 24 "verify\verify-yosupo-math\yosupo-subset-convolution-fast.test.cpp"

int main() {
  using mint = LazyMontgomeryModInt<998244353>;
  using vm = vector<mint>;
  mmint::set_mod<mint>();

  int N;
  rd(N);
  using vm = vector<mint>;
  vm a(1 << N), b(1 << N);
  unsigned int n;
  for (int i = 0; i < 1 << N; i++) {
    rd(n);
    a[i].a = mint::reduce(uint64_t(n) * mint::n2);
  }
  for (int i = 0; i < 1 << N; i++) {
    rd(n);
    b[i].a = mint::reduce(uint64_t(n) * mint::n2);
  }
  auto c = fast_multiply(a, b);

  for (int i = 0; i < 1 << N; i++) {
    if (i) wt(' ');
    wt(c[i].get());
  }
  wt('\n');
}
