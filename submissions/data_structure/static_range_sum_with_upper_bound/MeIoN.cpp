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
#include <bit>
#include <chrono>
#include <functional>
#include <iomanip>
#include <utility>
#include <type_traits>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstring>
#include <ctime>
#include <limits>
#include <ranges>
#include <concepts>

#define TE template <typename T>
#define TES template <typename T, typename ...S>
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
#define FO1(a) for (int _ = 0; _ < (a); ++_)
#define FO2(i, a) for (int i = 0; i < (a); ++i)
#define FO3(i, a, b) for (int i = (a); i < (b); ++i)
#define FO4(i, a, b, c) for (int i = (a); i < (b); i += (c))
#define FOR(...) OV(__VA_ARGS__, FO4, FO3, FO2, FO1)(__VA_ARGS__)
#define FF1(a) for (int _ = (a) - 1; _ >= 0; --_)
#define FF2(i, a) for (int i = (a) - 1; i >= 0; --i)
#define FF3(i, a, b) for (int i = (b) - 1; i >= (a); --i)
#define FF4(i, a, b, c) for (int i = (b) - 1; i >= (a); i -= (c))
#define FOR_R(...) OV(__VA_ARGS__, FF4, FF3, FF2, FF1)(__VA_ARGS__)
#define FOR_subset(t, s) for (int t = (s); t > -1; t = (t == 0 ? -1 : (t - 1) & s))

#define sort ranges::sort

using namespace std;

TE using vc = vector<T>;
TE using vvc = vc<vc<T>>;
TE using T1 = tuple<T>;
TE using T2 = tuple<T, T>;
TE using T3 = tuple<T, T, T>;
TE using T4 = tuple<T, T, T, T>;
TE using max_heap = priority_queue<T>;
TE using min_heap = priority_queue<T, vc<T>, greater<T>>;
using u8 = unsigned char; using uint = unsigned int; using ll = long long;      using ull = unsigned long long;
using ld = long double;   using i128 = __int128;     using u128 = __uint128_t;  using f128 = __float128;
using u16 = uint16_t;
using PII = pair<int, int>;   using PLL = pair<ll, ll>;

#ifdef YRSD
constexpr bool dbg = 1;
#else
constexpr bool dbg = 0;
#endif

TES concept nof = (not same_as<T, S> and ...);

TE constexpr bool can_in = 0;
TE constexpr bool can_out = 0;
template <>
constexpr bool can_in<istream> = 1;
template <>
constexpr bool can_out<ostream> = 1;

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

template <typename I, typename... S>
  requires(can_in<I>)
I &operator>>(I &in, tuple<S...> &a) {
  apply([&in](Z &...s) { ((in >> s), ...); }, a);
  return in;
}

template <typename I, typename A, typename B>
  requires(can_in<I>)
I &operator>>(I &in, pair<A, B> &x) {
  return in >> x.fi >> x.se;
}

template <typename U, typename A, typename B>
  requires(can_out<U>)
U &operator<<(U &out, const pair<A, B> &x) {
  return out << x.fi << ' ' << x.se;
}

template <typename I, typename T>
  requires(can_in<I>)
I &operator>>(I &in, vc<T> &a) {
  for (Z &x : a) in >> x;
  return in;
}

template <typename U, typename T>
  requires(can_out<U>)
U &operator<<(U &out, const vc<T> &a) {
  if (a.empty()) return out;
  Z i = bg(a);
  out << *i++;
  for (; i != ed(a); ++i) out << ' ' << *i;
  return out;
}

template <typename I, typename T, int N>
  requires(can_in<I>)
I &operator>>(I &in, array<T, N> &a) {
  FOR(i, N) in >> a[i];
  return in;
}

template <typename U, typename T, int N>
  requires(can_out<U>)
U &operator<<(U &out, const array<T, N> &a) {
  out << a[0];
  FOR(i, 1, N) out << ' ' << a[i];
  return out;
}

void IN() {}
TE void IN(T &x, Z &...s) { cin >> x, IN(s...); }
void print() { cout << '\n'; }
TES void print(T &&x, S &&...y) {
  cout << x;
  if constexpr (sizeof...(S)) cout << ' ';
  print(forward<S>(y)...);
}
void put() {}
TES void put(T &&x, S &&...y) {
  cout << x;
  put(forward<S>(y)...);
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

void YES(bool o = 1) { print(o ? "YES" : "NO"); }
void Yes(bool o = 1) { print(o ? "Yes" : "No"); }
void yes(bool o = 1) { print(o ? "yes" : "no"); }
void NO(bool o = 1) { YES(not o); }
void No(bool o = 1) { Yes(not o); }
void no(bool o = 1) { yes(not o); }
void ALICE(bool o = 1) { print(o ? "ALICE" : "BOB"); }
void Alice(bool o = 1) { print(o ? "Alice" : "Bob"); }
void alice(bool o = 1) { print(o ? "alice" : "bob"); }
void BOB(bool o = 1) { ALICE(not o); }
void Bob(bool o = 1) { Alice(not o); }
void bob(bool o = 1) { alice(not o); }
void POSSIBLE(bool o = 1) { print(o ? "POSSIBLE" : "IMPOSSIBLE"); }
void Possible(bool o = 1) { print(o ? "Possible" : "Impossible"); }
void possible(bool o = 1) { print(o ? "possible" : "impossible"); }
void IMPOSSIBLE(bool o = 1) { POSSIBLE(not o); }
void Impossible(bool o = 1) { Possible(not o); }
void impossible(bool o = 1) { possible(not o); }
void TAK(bool o = 1) { print(o ? "TAK" : "NIE"); }
void NIE(bool o = 1) { TAK(not o); }

#if (__cplusplus >= 202002L)
#include <numbers>
constexpr ld pi = numbers::pi_v<ld>;
#endif

TE constexpr T inf = numeric_limits<T>::max();
template <> constexpr i128 inf<i128> = i128(inf<ll>) * 2'000'000'000'000'000'000;
template <typename T, typename U>
constexpr pair<T, U> inf<pair<T, U>> = {inf<T>, inf<U>};

TE constexpr static inline int pc(T x) { return popcount(make_unsigned_t<T>(x)); }
constexpr static inline ll si(var a) { return a.size(); }

void reverse(Z &a) { reverse(all(a)); }

void unique(Z &a) {
  sort(a);
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
constexpr bool chmax(Z &a, var b) { return (a < b ? a = b, 1 : 0); }
constexpr bool chmin(Z &a, var b) { return (a > b ? a = b, 1 : 0); }

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
template <int of = 1, typename T>
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

template <bool ck = 1>
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
  TES inline void pt(T &&x, S &&...y) {
    fout << x;
    if constexpr (sizeof...(S)) fout << ' ';
    pt(forward<S>(y)...);
  }

  void inline pu() {}
  TES inline void pu(T &&x, S &&...y) {
    fout << x;
    pu(forward<S>(y)...);
  }
}
template <>
constexpr bool can_in<fio::Fin> = 1;
template <>
constexpr bool can_out<fio::Fout> = 1;

#define setp(x) fio::fout.setp(x)
#define IN fio::sc
#define print fio::pt
#define put fio::pu

vc<pair<int, ll>> range_count_prod_upper(const vc<int> &a, const vc<T3<int>> &q) {
  struct bit {
    int N;
    vc<pair<int, ll>> a;
    bit(int N) : N(N), a(N) {}
    void add(int i, int x) {
      for (; i < N; i |= i + 1) a[i].fi += 1, a[i].se += x;
    }
    pair<int, ll> prod(int l, int r) {
      int c = 0;
      ll s = 0;
      while (l < r) c += a[r - 1].fi, s += a[r - 1].se, r -= r & -r;
      while (r < l) c -= a[l - 1].fi, s -= a[l - 1].se, l -= l & -l;
      return {c, s};
    }
  };
  int N = si(a), Q = si(q);
  vc<PII> I(N + Q);
  FOR(i, N) I[i] = {a[i], ~i};
  FOR(i, Q) I[i + N] = {get<2>(q[i]), i};
  sort(I);
  bit s(N);
  vc<pair<int, ll>> ans(Q);
  for (Z [x, i] : I) {
    if (i < 0) s.add(~i, x);
    else ans[i] = s.prod(get<0>(q[i]), get<1>(q[i]));
  }
  return ans;
}
void Yorisou() {
  INT(N, Q);
  VEC(int, a, N);
  VEC(T3<int>, q, Q);
  for (var [c, x] : range_count_prod_upper(a, q)) print(c, x);
}

int main() {
  Yorisou();
  return 0;
}
