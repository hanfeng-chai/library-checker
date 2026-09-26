#include <iostream>
#include <algorithm>
#include <array>
#include <bitset>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <list>
#include <unordered_set>
#include <bit>
#include <chrono>
#include <functional>
#include <iomanip>
#include <utility>
#include <random>
#include <type_traits>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstring>
#include <ctime>
#include <limits>
#include <ranges>
#include <concepts>

#define TP template
#define TN typename
#define TE TP <TN T>
#define Z auto
#define var const Z &
#define ep emplace_back
#define eb emplace
#define fi first
#define se second
#define bg begin
#define ed end
#define all(x) bg(x), ed(x)

#define OV(a, b, c, d, e, ...) e
#define FO4(i, a, b, c) for (int i = (a); i < (b); i += (c))
#define FO3(i, a, b) FO4(i, a, b, 1)
#define FO2(i, a) FO3(i, 0, a)
#define FO1(a) FO2(_, a)
#define FOR(...) OV(__VA_ARGS__, FO4, FO3, FO2, FO1)(__VA_ARGS__)
#define FF4(i, a, b, c) for (int i = (b) - 1; i >= (a); i -= (c))
#define FF3(i, a, b) FF4(i, a, b, 1)
#define FF2(i, a) FF3(i, 0, a)
#define FF1(a) FF2(_, a)
#define FOR_R(...) OV(__VA_ARGS__, FF4, FF3, FF2, FF1)(__VA_ARGS__)
#define FOR_subset(t, s) for (int t = (s); t > -1; t = (t == 0 ? -1 : (t - 1) & s))

using namespace std;

TE using vc = vector<T>;
TE using vvc = vc<vc<T>>;
TE using T1 = tuple<T>;
TE using T2 = tuple<T, T>;
TE using T3 = tuple<T, T, T>;
TE using T4 = tuple<T, T, T, T>;
TE using max_heap = priority_queue<T>;
TE using min_heap = priority_queue<T, vc<T>, greater<T>>;
using u8 = unsigned char;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using i128 = __int128;
using u128 = __uint128_t;
using f128 = __float128;
using u16 = uint16_t;
using PII = pair<int, int>;
using PLL = pair<ll, ll>;
using vi = vc<int>;
using vl = vc<ll>;

TP<TN T, TN... S> concept nof = (not same_as<T, S> and ...);

TE constexpr bool can_in = 0;
TE constexpr bool can_out = 0;
TP<> constexpr bool can_in<istream> = 1;
TP<> constexpr bool can_out<ostream> = 1;

TE concept in_t = can_in<T>;
TE concept out_t = can_out<T>;

istream &operator>>(istream &I, i128 &x) {
  static string s;
  I >> s;
  int f = s[0] == '-';
  x = 0;
  const int N = (int)s.size();
  FOR(i, f, N) x = x * 10 + s[i] - '0';
  if (f) x = -x;
  return I;
}
ostream &operator<<(ostream &O, i128 x) {
  static string s;
  s.clear();
  bool f = x < 0;
  if (f) x = -x;
  while (x) s += '0' + x % 10, x /= 10;
  if (s.empty()) s += '0';
  if (f) s += '-';
  reverse(all(s));
  return O << s;
}

istream &operator>>(istream &I, f128 &x) {
  static string s;
  I >> s, x = stold(s);
  return I;
}
ostream &operator<<(ostream &O, const f128 x) { return O << ld(x); }

TP<in_t I, TN... S> I &operator>>(I &in, tuple<S...> &a);

TP<in_t I, TN A, TN B> I &operator>>(I &in, pair<A, B> &x);

TP<out_t U, TN A, TN B> U &operator<<(U &out, const pair<A, B> &x);

TP<in_t I, TN T> I &operator>>(I &in, vc<T> &a);

TP<out_t U, TN T> U &operator<<(U &out, const vc<T> &a);

TP<in_t I, TN T, std::size_t N> I &operator>>(I &in, array<T, N> &a);

TP<out_t U, TN T, std::size_t N> U &operator<<(U &out, const array<T, N> &a);

TP <in_t I, TN... S>
I &operator>>(I &in, tuple<S...> &a) {
  apply([&in](Z &...s) { ((in >> s), ...); }, a);
  return in;
}

TP <in_t I, TN A, TN B>
I &operator>>(I &in, pair<A, B> &x) {
  return in >> x.fi >> x.se;
}

TP <out_t U, TN A, TN B>
U &operator<<(U &out, const pair<A, B> &x) {
  return out << x.fi << ' ' << x.se;
}

TP <in_t I, TN T>
I &operator>>(I &in, vc<T> &a) {
  for (Z &x : a) in >> x;
  return in;
}

TP <out_t U, TN T>
U &operator<<(U &out, const vc<T> &a) {
  if (a.empty()) return out;
  Z i = bg(a);
  out << *i++;
  for (; i != ed(a); ++i) out << ' ' << *i;
  return out;
}

TP <in_t I, TN T, std::size_t N>
I &operator>>(I &in, array<T, N> &a) {
  FOR(i, N) in >> a[i];
  return in;
}

TP <out_t U, TN T, std::size_t N>
U &operator<<(U &out, const array<T, N> &a) {
  out << a[0];
  FOR(i, 1, N) out << ' ' << a[i];
  return out;
}

void IN() {}
TE void IN(T &x, Z &...s) { cin >> x, IN(s...); }

void print() { cout << '\n'; }
TP<TN T, TN... S>
void print(T &&x, S &&...y) {
  cout << x;
  if constexpr (sizeof...(S)) cout << ' ';
  print(std::forward<S>(y)...);
}

void put() {}
TP<TN T, TN... S>
void put(T &&x, S &&...y) {
  cout << x, put(std::forward<S>(y)...);
}

#define INT(...)  int    __VA_ARGS__; IN(__VA_ARGS__)
#define UINT(...) uint   __VA_ARGS__; IN(__VA_ARGS__)
#define LL(...)   ll     __VA_ARGS__; IN(__VA_ARGS__)
#define ULL(...)  ull    __VA_ARGS__; IN(__VA_ARGS__)
#define I128(...) i128   __VA_ARGS__; IN(__VA_ARGS__)
#define STR(...)  string __VA_ARGS__; IN(__VA_ARGS__)
#define CH(...)   char   __VA_ARGS__; IN(__VA_ARGS__)
#define REAL(...) re     __VA_ARGS__; IN(__VA_ARGS__)
#define VEC(T, a, n) vc<T> a(n); IN(a)

#if (__cplusplus >= 202002L)
#include <numbers>
constexpr ld pi = numbers::pi_v<ld>;
#else
constexpr ld pi = 3.141592653589793116;
#endif

TE constexpr T inf = numeric_limits<T>::max();
TP <> constexpr i128 inf<i128> = i128(inf<ll>) * 2'000'000'000'000'000'000;
TP <TN T, TN U> constexpr pair<T, U> inf<pair<T, U>> = {inf<T>, inf<U>};

TE constexpr static inline int pc(T x) { return popcount(make_unsigned_t<T>(x)); }
constexpr static inline ll si(var a) { return a.size(); }

TE int ceil_pw(T n) { return n <= 1 ? 0 : bit_width<make_unsigned_t<T>>(n - 1); }

void reverse(Z &a) { reverse(all(a)); }

TE void soin(T l, T r) { sort(l, r); }
TE void soin(T l, T r, var f) { sort(l, r, f); }
void soin(Z &a) { soin(all(a)); }
void soin(Z &a, var f) { soin(all(a), f); }
#define sort soin

void unique(Z &a) {
  sort(all(a));
  a.erase(unique(all(a)), ed(a));
}
TE vc<int> inverse(const vc<T> &a) {
  int N = si(a);
  vc<int> b(N, -1);
  FOR(i, N) if (a[i] != -1) b[a[i]] = i;
  return b;
}

Z QMAX(var a) { return *max_element(all(a)); }
Z QMIN(var a) { return *min_element(all(a)); }
TE Z QMAX(T l, T r) { return *max_element(l, r); }
TE Z QMIN(T l, T r) { return *min_element(l, r); }
constexpr bool chmax(Z &a, var b) { return (b > a ? a = b, 1 : 0); }
constexpr bool chmin(Z &a, var b) { return (b < a ? a = b, 1 : 0); }

vc<int> argsort(var a) {
  vc<int> I(si(a));
  iota(all(I), 0);
  sort(I, [&](int i, int k) { return a[i] < a[k] or (a[i] == a[k] and i < k); });
  return I;
}
TE vc<T> rearrange(const vc<T> &a, const vc<int> &I) {
  int N = si(I);
  vc<T> b(N);
  FOR(i, N) b[i] = a[I[i]];
  return b;
}

TP <int of = 1, TN T>
vc<T> pre_sum(const vc<T> &a) {
  int N = si(a);
  vc<T> c(N + 1);
  FOR(i, N) c[i + 1] = c[i] + a[i];
  if (of == 0) c.erase(bg(c));
  return c;
}

TE constexpr static int topbit(T x) {
  if (x == 0) return -1;
  if constexpr (sizeof(T) <= 4) return 31 - __builtin_clz(x);
  else return 63 - __builtin_clzll(x);
}
TE constexpr static int lowbit(T x) {
  if (x == 0) return -1;
  if constexpr (sizeof(T) <= 4) return __builtin_ctz(x);
  else return __builtin_ctzll(x);
}

TE constexpr inline T floor(T x, T y) { return x / y - (x % y and (x ^ y) < 0); }
TE constexpr inline T ceil(T x, T y) { return floor(x + y - 1, y); }
TE constexpr inline T bmod(T x, T y) { return x - floor(x, y) * y; }
TE constexpr inline pair<T, T> divmod(T x, T y) {
  T q = floor(x, y);
  return pair{q, x - q * y};
}

TE T SUM(var v) { return accumulate(all(v), T()); }
TE T SUM(Z l, Z r) { return accumulate(l, r, T()); }
int lb(var a, Z x) { return lower_bound(all(a), x) - a.begin(); }
TE int lb(T l, T r, Z x) { return lower_bound(l, r, x) - l; }
int ub(var a, Z x) { return upper_bound(all(a), x) - a.begin(); }
TE int ub(T l, T r, Z x) { return upper_bound(l, r, x) - l; }

TP <bool ck = 1>
ll bina(Z f, ll l, ll r) {
  if (ck) assert(f(l));
  while (abs(l - r) > 1) {
    ll x = (r + l) >> 1;
    (f(x) ? l : r) = x;
  }
  return l;
}
TE T bina_real(Z f, T l, T r, int c = 100) {
  while (c--) {
    T x = (l + r) / 2;
    (f(x) ? l : r) = x;
  }
  return (l + r) / 2;
}

TE T pop(vc<T> &a) {
  T x = a.back();
  a.pop_back();
  return x;
}
TE T pop(max_heap<T> &q) {
  T x = q.top();
  q.pop();
  return x;
}
TE T pop(min_heap<T> &q) {
  T x = q.top();
  q.pop();
  return x;
}
char pop(string &s) {
  char x = s.back();
  s.pop_back();
  return x;
}

void setp(int x) { cout << fixed << setprecision(x); }

TE inline void sh(vc<T> &a, int N, T b = {}) { a.resize(N, b); }

TE bool fd(const vc<T> &a, var x) { return binary_search(all(a), x); }

namespace fio {
  static constexpr uint sz = 1 << 17;
  char a[sz];
  char b[sz];
  char t[100];

  uint l = 0, r = 0, pr = 0;

  struct Pre {
    char num[10000][4];
    constexpr Pre() : num() {
      for (int i = 0; i < 10000; i++) {
        int n = i;
        for (int j = 3; j >= 0; j--) {
          num[i][j] = n % 10 | '0';
          n /= 10;
        }
      }
    }
  } constexpr pre;

  inline void load() {
    memcpy(a, a + l, r - l);
    r = r - l + fread(a + r - l, 1, sz - r + l, stdin);
    l = 0;
    if (r < sz) a[r++] = '\n';
  }

  inline void flush() { fwrite(b, 1, pr, stdout), pr = 0; }

  inline void rd(char &c) {
    do {
      if (l + 1 > r) load();
      c = a[l++];
    } while (isspace(c));
  }

  inline void rd(string &s) {
    s.clear();
    char c;
    do {
      if (l + 1 > r) load();
      c = a[l++];
    } while (isspace(c));
    do {
      s += c;
      if (l == r) load();
      c = a[l++];
    } while (!isspace(c));
  }

  TE inline void rd_re(T &x) {
    static string s;
    rd(s);
    x = stod(s);
  }

  TE inline void rd_inte(T &x) {
    if (l + 100 > r) load();
    char c;
    do c = a[l++];
    while (c < '-');
    bool op = 0;
    if constexpr (is_signed_v<T> or is_same_v<T, i128>) {
      if (c == '-') op = 1, c = a[l++];
    }
    x = 0;
    while ('0' <= c) x = x * 10 + (c & 15), c = a[l++];
    if constexpr (is_signed_v<T> or is_same_v<T, i128>) {
      if (op) x = -x;
    }
  }

  struct Fin {
    inline Fin &operator>>(char &c) {
      rd(c);
      return *this;
    }

    inline Fin &operator>>(string &s) {
      rd(s);
      return *this;
    }

    TE requires(is_integral_v<T> or is_same_v<T, i128> or is_same_v<T, u128>)
    inline Fin &operator>>(T &x) {
      rd_inte(x);
      return *this;
    }

    TE requires(is_floating_point_v<T> or is_same_v<T, f128>)
    inline Fin &operator>>(T &x){
      rd_re(x);
      return *this;
    }
  } fin;

  inline void wt(char c) {
    if (pr == sz) flush();
    b[pr++] = c;
  }

  inline void wt(const string &s) {
    for (char c : s) wt(c);
  }

  inline void wt(const char *s) {
    size_t si = strlen(s);
    for (size_t i = 0; i < si; i++) wt(s[i]);
  }

  TE inline void wt_inte(T x) {
    if (pr > sz - 100) flush();
    if (x < 0) b[pr++] = '-', x = -x;
    int outi = 96;
    for (; x >= 10000; outi -= 4) {
      memcpy(t + outi, pre.num[x % 10000], 4);
      x /= 10000;
    }
    if (x >= 1000) {
      memcpy(b + pr, pre.num[x], 4);
      pr += 4;
    } else if (x >= 100) {
      memcpy(b + pr, pre.num[x] + 1, 3);
      pr += 3;
    } else if (x >= 10) {
      int q = (x * 103) >> 10;
      b[pr] = q | '0';
      b[pr + 1] = (x - q * 10) | '0';
      pr += 2;
    } else b[pr++] = x | '0';
    memcpy(b + pr, t + outi + 4, 96 - outi);
    pr += 96 - outi;
  }

  int w = 10;
  TE inline void wt_real(T x) {
    ostringstream oss;
    oss << fixed << setprecision(w) << double(x);
    string s = oss.str();
    wt(s);
  }

  struct Fout {
    void setp(int x) { w = x; }

    inline Fout &operator<<(const char &c) {
      wt(c);
      return *this;
    }

    inline Fout &operator<<(const string &s) {
      wt(s);
      return *this;
    }

    TE requires(is_integral_v<T> or is_same_v<T, i128> or is_same_v<T, u128>)
    inline Fout &operator<<(const T &x) {
      wt_inte(x);
      return *this;
    }

    TE requires(is_floating_point_v<T> or is_same_v<T, f128>)
    inline Fout &operator<<(const T &x) {
      wt_real(x);
      return *this;
    }
  } fout;

  inline void __attribute__((destructor)) _d() { flush(); }

  inline void sc() {}
  TE inline void sc(T &x, Z &...s) { fin >> x, sc(s...); }

  void inline pt() { fout << '\n'; }
  TP<TN T, TN... S> inline void pt(T &&x, S &&...y) {
    fout << x;
    if constexpr (sizeof...(S)) fout << ' ';
    pt(forward<S>(y)...);
  }

  void inline pu() {}
  TP<TN T, TN... S> inline void pu(T &&x, S &&...y) {
    fout << x;
    pu(forward<S>(y)...);
  }
}
TP<> constexpr bool can_in<fio::Fin> = 1;
TP<> constexpr bool can_out<fio::Fout> = 1;

#define setp(x) fio::fout.setp(x)
#define IN fio::sc
#define print fio::pt
#define put fio::pu

struct flower {
  int N, tt;
  vc<vc<int>> g;
  vc<int> pr, ma, s, t, fa, q;
  int l, r;
  flower(int N) : N(N), tt{}, g(N), pr(N + 1, N), ma(pr), s(N + 1), t(N), fa(s), q(s) {}
  void add(int x, int y) { g[x].ep(y), g[y].ep(x); }
  int f(int x) { return x == fa[x] ? x : fa[x] = f(fa[x]); }
  int lca(int x, int y) {
    for (tt++, x = f(x), y = f(y);; swap(x, y)) if (x != N) {
      if (t[x] == tt) return x;
      t[x] = tt, x = f(pr[ma[x]]);
    }
  }
  void bl(int x, int y, int c) {
    for (; f(x) != c; x = pr[y]) {
      pr[x] = y, y = ma[x];
      if (s[y] == 1) q[r++] = y, s[y] = 0;
      if (fa[x] == x) fa[x] = c;
      if (fa[y] == y) fa[y] = c;
    }
  }
  bool go(int st) {
    iota(all(fa), 0), fill(all(s), -1);
    for (l = r = 0, s[q[r++] = st] = 0; l < r; ) {
      for (int n = q[l++]; int x : g[n]) {
        if (s[x] == -1) {
          pr[x] = n, s[x] = 1;
          if (ma[x] == N) {
            for (int a = x, b = n, ls; b != N; a = ls, b = pr[a]) {
              ls = ma[b], ma[b] = a, ma[a] = b;
            }
            return 1;
          }
          s[q[r++] = ma[x]] = 0;
        } else if (not s[x] and f(x) != f(n)) {
          int a = lca(x, n);
          bl(n, x, a), bl(x, n, a);
        }
      }
    }
    return 0;
  }
  int slv() {
    int s = 0;
    FOR(i, N) if (ma[i] == N) s += go(i);
    return s;
  }
};

void Yorisou() {
  INT(N, M);
  flower g(N);
  FOR(M) {
    INT(a, b);
    g.add(a, b);
  }
  int rs = g.slv();
  vc<int> ma = std::move(g.ma);
  print(rs);
  FOR(i, N) if (ma[i] != N and i < ma[i]) print(i, ma[i]);
}

int main() {
  Yorisou();
  return 0;
}
