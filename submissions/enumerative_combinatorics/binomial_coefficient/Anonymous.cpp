#include <bits/stdc++.h>

using namespace std;
using LL = long long;

LL exgcd(LL a, LL b, LL &x, LL &y) {
  if (!b) return x = 1, y = 0, a;
  LL g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
LL inv(LL a, LL p) {
  LL x, y;
  exgcd(a, p, x, y);
  return (x % p + p) % p;
}

using namespace std;

struct A {
  int p, q, m; // p^q <= 2e7
  vector<int> fac, ifac;
  int delta;

  using i64 = LL;
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  u64 im, ip;

  A(int _p, int _q) : p(_p), q(_q), m(1) {
    assert(_q > 0);
    while (_q--) m *= p;
    im = u64(-1) / m + 1;
    ip = u64(-1) / p + 1;
    
    fac.resize(m);
    ifac.resize(m);
    fac[0] = fac[1] = 1;
    for (int i = 2; i < m; i++) {
      fac[i] = i % p ? mod(1LL * fac[i - 1] * i) : fac[i - 1];
    }
    ifac[m - 1] = fpow(fac[m - 1], m / p * (p - 1) - 1);
    for (int i = m - 1; i; i--) {
      ifac[i - 1] = i % p ? mod(1LL * ifac[i] * i) : ifac[i];
    }
    delta = p == 2 && q >= 3 ? 1 : m - 1;
  }

  i64 mod(u64 n) {
    i64 r = n - u64((u128(n) * im) >> 64) * m;
    return r < 0 ? r + m : r;
  }

  int fpow(int a, LL k) {
    int r = 1;
    for (; k; k >>= 1, a = mod(1LL * a * a)) {
      if (k & 1) r = mod(1LL * r * a);
    }
    return r;
  }

  i64 div_p(u64 n) {
    u64 x = (u128(n) * ip) >> 64;
    i64 r = n - x * p;
    return r < 0 ? x - 1: x;
  }

  pair<i64, int> quo_p(u64 n) {
    u64 x = (u128(n) * ip) >> 64;
    i64 r = n - x * p;
    if (r < 0) r += m, x--;
    return {i64(x), r};
  }

  LL lucas(LL n, LL m) {
    int res = 1;
    while (n) {
      int n0, m0;
      tie(n, n0) = quo_p(n);
      tie(m, m0) = quo_p(m);
      if (n0 < m0) return 0;
      res = mod(1LL * res * fac[n0]);
      res = mod(1LL * res * ifac[m0]);
      res = mod(1LL * res * ifac[n0 - m0]);
    }
    return res;
  }

  LL bin(LL n, LL m) {
    if (n < m || n < 0 || m < 0) return 0;
    if (q == 1) return lucas(n, m);
    LL r = n - m;
    int e0 = 0, eq = 0, i = 0;
    int res = 1;
    while (n) {
      res = mod(1LL * res * fac[mod(n)]);
      res = mod(1LL * res * ifac[mod(m)]);
      res = mod(1LL * res * ifac[mod(r)]);
      n = div_p(n), m = div_p(m), r = div_p(r);
      int eps = n - m - r;
      e0 += eps;
      if (e0 >= q) return 0;
      if (++i >= q) eq += eps;
    }
    res = mod(1LL * res * fpow(delta, eq));
    res = mod(1LL * res * fpow(p, e0));
    return res;
  }
};

// constraints:
// (M <= 1e7 and max(N) <= 1e18) or (M < 2^30 and max(N) <= 2e7)

struct B {
  int mod;
  vector<int> m;
  vector<LL> ims;
  vector<A> cs;

  B(LL x) : mod(x) {
    assert(1 <= x);
    for (int i = 2; i * i <= x; i++) {
      if (x % i == 0) {
        int j = 0, k = 1;
        while (x % i == 0) x /= i, j++, k *= i;
        m.push_back(k);
        cs.emplace_back(i, j);
        assert(m.back() == cs.back().m);
      }
    }
    if (x != 1) {
      m.push_back(x);
      cs.emplace_back(x, 1);
    }
    assert(m.size() == cs.size());

    vector<LL> ms;
    for (auto& c : cs) ms.push_back(c.m);
    
    LL m0 = 1;
    int n = m.size();
    for (int i = 0; i < n; i++) {
      LL m1 = m[i];
      if (m0 < m1) swap(m0, m1);
      ims.push_back(inv(m0, m1));
      m0 *= m1;
    }
  }

  LL bin(LL n, LL k) {
    if (mod == 1) return 0;

    LL r0 = 0, m0 = 1;
    for (int i = 0; i < (int)cs.size(); i++) {
      LL r1 = cs[i].bin(n, k), m1 = m[i], im = ims[i];
      if (m0 < m1) swap(r0, r1), swap(m0, m1);
      LL x = (r1 - r0) * im % m1;
      r0 += x * m0;
      m0 *= m1;
      if (r0 < 0) r0 += m0;
    }
    
    return r0;
  }
};


namespace fastio {
static constexpr int SZ = 1 << 17;
char ibuf[SZ], obuf[SZ];
int pil = 0, pir = 0, por = 0;

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
  memcpy(ibuf, ibuf + pil, pir - pil);
  pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
  pil = 0;
}
inline void flush() {
  fwrite(obuf, 1, por, stdout);
  por = 0;
}

inline void skip_space() {
  if (pil + 32 > pir) load();
  while (ibuf[pil] <= ' ') pil++;
}

inline void rd(char& c) {
  if (pil + 32 > pir) load();
  c = ibuf[pil++];
}
template <typename T>
inline void rd(T& x) {
  if (pil + 32 > pir) load();
  char c;
  do c = ibuf[pil++];
  while (c < '-');
  [[maybe_unused]] bool minus = false;
  if constexpr (is_signed<T>::value == true) {
    if (c == '-') minus = true, c = ibuf[pil++];
  }
  x = 0;
  while (c >= '0') {
    x = x * 10 + (c & 15);
    c = ibuf[pil++];
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
  if (por > SZ - 32) flush();
  obuf[por++] = c;
}
inline void wt(bool b) { 
  if (por > SZ - 32) flush();
  obuf[por++] = b ? '1' : '0'; 
}
template <typename T>
inline void wt(T x) {
  if (por > SZ - 32) flush();
  if (!x) {
    obuf[por++] = '0';
    return;
  }
  if constexpr (is_signed<T>::value == true) {
    if (x < 0) obuf[por++] = '-', x = -x;
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
      obuf[por] = '0' + x;
      ++por;
    } else {
      uint32_t q = (uint32_t(x) * 205) >> 11;
      uint32_t r = uint32_t(x) - q * 10;
      obuf[por] = '0' + q;
      obuf[por + 1] = '0' + r;
      por += 2;
    }
  } else {
    if (x < 1000) {
      memcpy(obuf + por, pre.num + (x << 2) + 1, 3);
      por += 3;
    } else {
      memcpy(obuf + por, pre.num + (x << 2), 4);
      por += 4;
    }
  }
  memcpy(obuf + por, buf + i + 4, 12 - i);
  por += 12 - i;
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


int main() {
  // freopen("t.in", "r", stdin);
  // freopen(".out", "w", stdout);
  unsigned int T, M;
  rd(T, M);
  B C(M);
  while (T--) {
    unsigned long long n, k;
    rd(n, k);
    wtn(C.bin(n, k));
  }
}
