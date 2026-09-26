#include <assert.h>
#include <string.h>
#include <algorithm>
#include <vector>

using std::max;
using std::min;
using std::vector;

// !!!Watch out for stack overflow!!!
template <class T, int N> struct SetMul {
  // Except for mul, these are enough for n <= N + 1.
  T zas[1 << N][N + 1], zbs[1 << N][N + 1];

  // as * bs
  // O(2^n n^2) time
  vector<T> operator()(int n, const vector<T> &as, const vector<T> &bs) {
    return mul(n, as, bs);
  }
  vector<T> mul(int n, const vector<T> &as, const vector<T> &bs) {
    assert(0 <= n); assert(n <= N);
    assert(static_cast<int>(as.size()) == 1 << n);
    assert(static_cast<int>(bs.size()) == 1 << n);
    vector<T> cs(1 << n);
    mul(n, as.data(), bs.data(), cs.data());
    return cs;
  }
  void mul(int n, const T *as, const T *bs, T *cs) {
    for (int h = 0; h < 1 << n; ++h) {
      for (int k = 0; k <= n; ++k) zas[h][k] = 0;
      zas[h][__builtin_popcount(h)] = as[h];
    }
    for (int h = 0; h < 1 << n; ++h) {
      for (int k = 0; k <= n; ++k) zbs[h][k] = 0;
      zbs[h][__builtin_popcount(h)] = bs[h];
    }
    rec(n, n, 0);
    for (int h = 0; h < 1 << n; ++h) cs[h] = zas[h][__builtin_popcount(h)];
  }
  void rec(int n, int m, int h0) {
    const int pop0 = __builtin_popcount(h0);
    if (m) {
      --m;
      for (int h = h0; h < h0 + (1 << m); ++h) {
        const int pop = __builtin_popcount(h);
        for (int k = pop - pop0; k <= pop; ++k) zas[h | 1 << m][k] += zas[h][k];
        for (int k = pop - pop0; k <= pop; ++k) zbs[h | 1 << m][k] += zbs[h][k];
      }
      rec(n, m, h0);
      rec(n, m, h0 | 1 << m);
      for (int h = h0; h < h0 + (1 << m); ++h) {
        const int pop = __builtin_popcount(h);
        for (int k = pop; k <= min(2 * pop, pop + (n - m - pop0)); ++k) zas[h | 1 << m][k] -= zas[h][k];
      }
    } else {
      for (int k = min(2 * pop0, n); k >= pop0; --k) {
        T t = 0;
        for (int l = max(0, k - pop0); l <= min(pop0, k); ++l) t += zas[h0][l] * zbs[h0][k - l];
        zas[h0][k] = t;
      }
    }
  }

  // \sum[0<=i<=n] fs[i] as^i/i!
  //   Assumes as[0] = 0.
  //   f(a0 + a1 x) = f(a0) + f'(a0) a1 x
  // O(2^n n^2) time
  vector<T> com(int n, const vector<T> &fs, const vector<T> &as) {
    assert(0 <= n); assert(n <= N + 1);
    assert(static_cast<int>(as.size()) == 1 << n);
    vector<T> bs(1 << n);
    com(fs.size(), n, fs.data(), as.data(), bs.data());
    return bs;
  }
  void com(int fsLen, int n, const T *fs, const T *as, T *bs) {
    assert(!as[0]);
    for (int i = n; i >= 0; --i) {
      for (int m = n - i; --m >= 0; ) mul(m, bs, as + (1 << m), bs + (1 << m));
      bs[0] = (i < fsLen) ? fs[i] : 0;
    }
  }

  // [x^[n]] as bs^i/i!  for 0 <= i <= n
  //   Assumes bs[0] = 0
  // O(2^n n^2) time
  vector<T> powProj(int n, vector<T> as, const vector<T> &bs) {
    assert(0 <= n); assert(n <= N + 1);
    assert(static_cast<int>(as.size()) == 1 << n);
    assert(static_cast<int>(bs.size()) == 1 << n);
    assert(!bs[0]);
    vector<T> cs(1 << max(n - 1, 0));
    vector<T> fs(n + 1);
    for (int i = 0; i <= n; ++i) {
      fs[i] = as[(1 << n) - 1];
      as[(1 << n) - 1] = 0;
      for (int m = 0; m < n - i; ++m) {
        mul(m, as.data() + ((1 << n) - (1 << (m + 1))), bs.data() + (1 << m), cs.data());
        for (int h = 0; h < 1 << m; ++h) as[(1 << n) - (1 << m) + h] += cs[h];
        for (int h = 0; h < 1 << m; ++h) as[(1 << n) - (1 << (m + 1)) + h] = 0;
      }
    }
    return fs;
  }

  // [x^[n]] as bs^i  for 0 <= i < len
  //   not necessarily as[0] = 0
  // O(len n + 2^n n^2) time
  vector<T> powProjPoly(int n, const vector<T> &as, vector<T> bs, int len) {
    assert(0 <= n); assert(n <= N + 1);
    assert(static_cast<int>(as.size()) == 1 << n);
    assert(static_cast<int>(bs.size()) == 1 << n);
    assert(0 <= len);
    const T b = bs[0];
    bs[0] = 0;
    const auto gs = powProj(n, as, bs);
    vector<T> fs(len, 0), hs(n + 1, 0);
    hs[0] = 1;
    for (int i = 0; i < len; ++i) {
      for (int j = 0; j <= n; ++j) fs[i] += hs[j] * gs[j];
      for (int j = n; --j >= 0; ) hs[j + 1] = hs[j] * (i + 1);
      hs[0] *= b;
    }
    return fs;
  }
};

////////////////////////////////////////////////////////////////////////////////



#ifndef LIBRA_ALGEBRA_MODINT_H_
#define LIBRA_ALGEBRA_MODINT_H_

#include <assert.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
template <unsigned M_> struct ModInt {
  static constexpr unsigned M = M_;
  unsigned x;
  constexpr ModInt() : x(0U) {}
  constexpr ModInt(unsigned x_) : x(x_ % M) {}
  constexpr ModInt(unsigned long long x_) : x(x_ % M) {}
  constexpr ModInt(int x_) : x(((x_ %= static_cast<int>(M)) < 0) ? (x_ + static_cast<int>(M)) : x_) {}
  constexpr ModInt(long long x_) : x(((x_ %= static_cast<long long>(M)) < 0) ? (x_ + static_cast<long long>(M)) : x_) {}
  ModInt &operator+=(const ModInt &a) { x = ((x += a.x) >= M) ? (x - M) : x; return *this; }
  ModInt &operator-=(const ModInt &a) { x = ((x -= a.x) >= M) ? (x + M) : x; return *this; }
  ModInt &operator*=(const ModInt &a) { x = (static_cast<unsigned long long>(x) * a.x) % M; return *this; }
  ModInt &operator/=(const ModInt &a) { return (*this *= a.inv()); }
  ModInt pow(long long e) const {
    if (e < 0) return inv().pow(-e);
    ModInt a = *this, b = 1U; for (; e; e >>= 1) { if (e & 1) b *= a; a *= a; } return b;
  }
  ModInt inv() const {
    unsigned a = M, b = x; int y = 0, z = 1;
    for (; b; ) { const unsigned q = a / b; const unsigned c = a - q * b; a = b; b = c; const int w = y - static_cast<int>(q) * z; y = z; z = w; }
    assert(a == 1U); return ModInt(y);
  }
  ModInt operator+() const { return *this; }
  ModInt operator-() const { ModInt a; a.x = x ? (M - x) : 0U; return a; }
  ModInt operator+(const ModInt &a) const { return (ModInt(*this) += a); }
  ModInt operator-(const ModInt &a) const { return (ModInt(*this) -= a); }
  ModInt operator*(const ModInt &a) const { return (ModInt(*this) *= a); }
  ModInt operator/(const ModInt &a) const { return (ModInt(*this) /= a); }
  template <class T> friend ModInt operator+(T a, const ModInt &b) { return (ModInt(a) += b); }
  template <class T> friend ModInt operator-(T a, const ModInt &b) { return (ModInt(a) -= b); }
  template <class T> friend ModInt operator*(T a, const ModInt &b) { return (ModInt(a) *= b); }
  template <class T> friend ModInt operator/(T a, const ModInt &b) { return (ModInt(a) /= b); }
  explicit operator bool() const { return x; }
  bool operator==(const ModInt &a) const { return (x == a.x); }
  bool operator!=(const ModInt &a) const { return (x != a.x); }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) { return os << a.x; }
};
////////////////////////////////////////////////////////////////////////////////

#endif  // LIBRA_ALGEBRA_MODINT_H_


// https://judge.yosupo.jp/problem/exp_of_set_power_series
#include <stdio.h>

constexpr unsigned MO = 998244353;
using Mint = ModInt<MO>;

SetMul<Mint, 19> setMul;

int main() {
  int N, M;
  for (; ~scanf("%d%d", &N, &M); ) {
    vector<Mint> A(1 << N), B(1 << N);
    for (int h = 0; h < 1 << N; ++h) scanf("%u", &B[h].x);
    for (int h = 1 << N; --h >= 0; ) scanf("%u", &A[h].x);
    const vector<Mint> ans = setMul.powProjPoly(N, A, B, M);
    for (int i = 0; i < M; ++i) {
      if (i) printf(" ");
      printf("%u", ans[i].x);
    }
    puts("");
  }
  return 0;
}
