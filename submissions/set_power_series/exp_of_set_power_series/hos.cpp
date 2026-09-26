#include <assert.h>
#include <string.h>
#include <algorithm>
#include <vector>

using std::max;
using std::min;
using std::vector;

// !!!Watch out for stack overflow!!!
// Usage: SetMul<T, N>()(n, as, bs)
template <class T, int N> struct SetMul {
  T zas[1 << N][N + 1], zbs[1 << N][N + 1];
  vector<T> operator()(int n, const vector<T> &as, const vector<T> &bs) {
    assert(static_cast<int>(as.size()) == 1 << n);
    assert(static_cast<int>(bs.size()) == 1 << n);
    for (int h = 0; h < 1 << n; ++h) {
      for (int k = 0; k <= n; ++k) zas[h][k] = 0;
      zas[h][__builtin_popcount(h)] = as[h];
    }
    for (int h = 0; h < 1 << n; ++h) {
      for (int k = 0; k <= n; ++k) zbs[h][k] = 0;
      zbs[h][__builtin_popcount(h)] = bs[h];
    }
    rec(n, n, 0);
    vector<T> cs(1 << n);
    for (int h = 0; h < 1 << n; ++h) cs[h] = zas[h][__builtin_popcount(h)];
    return cs;
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
};

// exp(as)
//   Assumes as[0] = 0
//   exp(a0 + a1 X) = exp(a0) + exp(a0) a1 X
// Usage: SetExp<T, N>()(n, as)
template <class T, int N> struct SetExp {
  SetMul<T, max(N - 1, 0)> setMul;
  vector<T> operator()(int n, const vector<T> &as) {
    assert(static_cast<int>(as.size()) == 1 << n);
    assert(!as[0]);
    vector<T> bs{1};
    for (int m = 0; m < n; ++m) {
      const auto cs = setMul(m, bs, vector<T>(as.begin() + (1 << m), as.begin() + (1 << (m + 1))));
      bs.insert(bs.end(), cs.begin(), cs.end());
    }
    return bs;
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

SetExp<Mint, 20> setExp;

int main() {
  int N;
  for (; ~scanf("%d", &N); ) {
    vector<Mint> A(1 << N);
    for (int h = 0; h < 1 << N; ++h) scanf("%u", &A[h].x);
    const vector<Mint> ans = setExp(N, A);
    for (int h = 0; h < 1 << N; ++h) {
      if (h) printf(" ");
      printf("%u", ans[h].x);
    }
    puts("");
  }
  return 0;
}
