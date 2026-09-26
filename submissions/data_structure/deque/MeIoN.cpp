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

template <typename... S>
istream &operator>>(istream &I, tuple<S...> &t) {
  return apply([&I](Z &...s) { ((I >> s), ...); }, t), I;
}

template <typename T, typename U>
istream &operator>>(istream &I, pair<T, U> &x) {
  return I >> x.fi >> x.se;
}
template <typename T, typename U>
ostream &operator<<(ostream &O, const pair<T, U> &x) {
  return O << x.fi << ' ' << x.se;
}

TE requires requires(T &c) { begin(c); end(c); } and
                          (not is_same_v<decay_t<T>, string>)
istream &operator>>(istream &I, T &c) {
  for (Z &e : c) I >> e;
  return I;
}

TE ostream &operator<<(ostream &cout, const vc<T> &a) {
  if (a.empty()) return cout;
  Z i = bg(a);
  cout << *i++;
  for (; i != ed(a); ++i) cout << ' ' << *i;
  return cout;
}

template <typename T, int N> requires(N > 0)
ostream &operator<<(ostream &cout, const array<T, N> &a) {
  cout << a[0];
  FOR(i, 1, N) cout << ' ' << a[i];
  return cout;
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
  if (x == 0) return - 1;
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

#define FIO

static constexpr uint SZ = 1 << 17;
char ibuf[SZ];
char obuf[SZ];
char out[100];

uint pil = 0, pir = 0, por = 0;

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
  memcpy(ibuf, ibuf + pil, pir - pil);
  pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
  pil = 0;
  if (pir < SZ) ibuf[pir++] = '\n';
}

inline void flush() {
  fwrite(obuf, 1, por, stdout);
  por = 0;
}
inline void rd(char &c) {
  do {
    if (pil + 1 > pir) load();
    c = ibuf[pil++];
  } while (isspace(c));
}

inline void rd(string &x) {
  x.clear();
  char c;
  do {
    if (pil + 1 > pir) load();
    c = ibuf[pil++];
  } while (isspace(c));
  do {
    x += c;
    if (pil == pir) load();
    c = ibuf[pil++];
  } while (!isspace(c));
}

TE inline void rd_real(T &x) {
  string s;
  rd(s);
  x = stod(s);
}

TE inline void rd_integer(T &x) {
  if (pil + 100 > pir) load();
  char c;
  do c = ibuf[pil++];
  while (c < '-');
  bool minus = 0;
  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
    if (c == '-') {
      minus = 1, c = ibuf[pil++];
    }
  }
  x = 0;
  while ('0' <= c) {
    x = x * 10 + (c & 15), c = ibuf[pil++];
  }
  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
    if (minus) x = -x;
  }
}

inline void rd(int16_t &x) { rd_integer(x); }
inline void rd(uint16_t &x) { rd_integer(x); }
inline void rd(int &x) { rd_integer(x); }
inline void rd(long &x) { rd_integer(x); }
inline void rd(ll &x) { rd_integer(x); }
inline void rd(i128 &x) { rd_integer(x); }
inline void rd(uint &x) { rd_integer(x); }
inline void rd(ull &x) { rd_integer(x); }
inline void rd(u128 &x) { rd_integer(x); }
inline void rd(double &x) { rd_real(x); }
inline void rd(long double &x) { rd_real(x); }
inline void rd(f128 &x) { rd_real(x); }

template <typename T, typename U>
inline void rd(pair<T, U> &p) {
  return rd(p.fi), rd(p.se);
}
template <size_t N = 0, typename T>
inline void rd_tuple(T &t) {
  if constexpr (N < tuple_size<T>::value) {
    Z &x = get<N>(t);
    rd(x);
    rd_tuple<N + 1>(t);
  }
}
template <typename... T>
inline void rd(tuple<T...> &tpl) {
  rd_tuple(tpl);
}

template <size_t N = 0, typename T>
inline void rd(array<T, N> &x) {
  for (Z &e : x) rd(e);
}
TE inline void rd(vc<T> &x) {
  for (Z &e : x) rd(e);
}

inline void read() {}
template <typename H, typename... T>
inline void read(H &h, T &...t) {
  rd(h), read(t...);
}

inline void wt(const char c) {
  if (por == SZ) flush();
  obuf[por++] = c;
}
inline void wt(const string s) {
  for (char c : s) wt(c);
}
inline void wt(const char *s) {
  size_t si = strlen(s);
  for (size_t i = 0; i < si; i++) wt(s[i]);
}

TE inline void wt_integer(T x) {
  if (por > SZ - 100) flush();
  if (x < 0) {
    obuf[por++] = '-', x = -x;
  }
  int outi;
  for (outi = 96; x >= 10000; outi -= 4) {
    memcpy(out + outi, pre.num[x % 10000], 4);
    x /= 10000;
  }
  if (x >= 1000) {
    memcpy(obuf + por, pre.num[x], 4);
    por += 4;
  } else if (x >= 100) {
    memcpy(obuf + por, pre.num[x] + 1, 3);
    por += 3;
  } else if (x >= 10) {
    int q = (x * 103) >> 10;
    obuf[por] = q | '0';
    obuf[por + 1] = (x - q * 10) | '0';
    por += 2;
  } else
    obuf[por++] = x | '0';
  memcpy(obuf + por, out + outi + 4, 96 - outi);
  por += 96 - outi;
}

#ifndef SETP
#define SETP 10
#endif

TE inline void wt_real(T x) {
  ostringstream oss;
  oss << fixed << setprecision(SETP) << double(x);
  string s = oss.str();
  wt(s);
}

inline void wt(int x) { wt_integer(x); }
inline void wt(long x) { wt_integer(x); }
inline void wt(ll x) { wt_integer(x); }
inline void wt(i128 x) { wt_integer(x); }
inline void wt(uint x) { wt_integer(x); }
inline void wt(ull x) { wt_integer(x); }
inline void wt(u128 x) { wt_integer(x); }
inline void wt(double x) { wt_real(x); }
inline void wt(long double x) { wt_real(x); }
inline void wt(f128 x) { wt_real(x); }

template <typename T, typename U>
inline void wt(const pair<T, U> &val) {
  wt(val.fi);
  wt(' ');
  wt(val.se);
}
template <size_t N = 0, typename T>
inline void wt_tuple(const T &t) {
  if constexpr (N < tuple_size<T>::value) {
    if constexpr (N > 0) {
      wt(' ');
    }
    const Z x = get<N>(t);
    wt(x);
    wt_tuple<N + 1>(t);
  }
}
template <typename... T>
inline void wt(tuple<T...> &tpl) {
  wt_tuple(tpl);
}
template <typename T, size_t S>
inline void wt(const array<T, S> &val) {
  Z n = val.size();
  for (size_t i = 0; i < n; i++) {
    if (i) wt(' ');
    wt(val[i]);
  }
}
TE inline void wt(const vc<T> &a) {
  int N = si(a);
  FOR(i, N) {
    if (i) wt(' ');
    wt(a[i]);
  }
}
TE inline void wt(const vc<vc<T>> &v) {
  int N = si(v);
  FOR(i, N) {
    wt(v[i]);
    if (i + 1 != N) wt('\n');
  }
}
template <typename T, const size_t s>
inline void wt(const vc<array<T, s>> &v) {
  int N = si(v);
  FOR(i, N) {
    wt(v[i]);
    if (i + 1 != N) wt('\n');
  }
}

inline void __attribute__((destructor)) _d() { flush(); }

inline void println() { wt('\n'); }
template <typename Head, typename... Tail>
inline void println(Head &&head, Tail &&...tail) {
  wt(head);
  if (sizeof...(Tail)) wt(' ');
  println(forward<Tail>(tail)...);
}

#define IN(...) read(__VA_ARGS__)
#define print(...) println(__VA_ARGS__)
#define FLUSH() flush()

void Yorisou() {
  INT(Q);
  deque<int> q;
  FOR(Q) {
    INT(op);
    if (op == 0) {
      INT(x);
      q.emplace_front(x);
    } else if (op == 1) {
      INT(x);
      q.emplace_back(x);
    } else if (op == 2) {
      q.pop_front();
    } else if (op == 3) {
      q.pop_back();
    } else {
      INT(x);
      print(q[x]);
    }
  }
}

int main() {
  Yorisou();
  return 0;
}
