#line 1 "Log_of_Set_Power_Series.cpp"
#define YRSD
// #include "YRS/aa/fast.hpp"
#line 2 "YRS/all.hpp"

#line 2 "YRS/aa/head.hpp"

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
#define ep emplace_back
#define eb emplace
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()

#define OV4(a, b, c, d, e, ...) e
#define FOR1(a) for (int _ = 0; _ < (a); ++_)
#define FOR2(i, a) for (int i = 0; i < (a); ++i)
#define FOR3(i, a, b) for (int i = (a); i < (b); ++i)
#define FOR4(i, a, b, c) for (int i = (a); i < (b); i += (c))
#define FOR(...) OV4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR1_R(a) for (int _ = (a) - 1; _ >= 0; --_)
#define FOR2_R(i, a) for (int i = (a) - 1; i >= 0; --i)
#define FOR3_R(i, a, b) for (int i = (b) - 1; i >= (a); --i)
#define FOR4_R(i, a, b, c) for (int i = (b) - 1; i >= (a); i -= (c))
#define FOR_R(...) OV4(__VA_ARGS__, FOR4_R, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
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
#line 2 "YRS/IO/IO.hpp"

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
TE requires requires(const T &c) { begin(c); end(c); } and 
  (not is_same_v<decay_t<T>, const char*>) and 
  (not is_same_v<decay_t<T>, string>) and 
  (not is_array_v<remove_reference_t<T>> or 
   not is_same_v<remove_extent_t<remove_reference_t<T>>, char>)
ostream &operator<<(ostream &O, const T &a) {
  if (a.empty()) return O;
  Z i = a.begin();
  O << *i++;
  for (; i != a.end(); ++i) O << ' ' << *i;
  return O;
}
void IN() {}
TE void IN(T &x, Z &...s) { cin >> x, IN(s...); }
void print() { cout << '\n'; }
TES void print(T &&x, S &&...y) {
  cout << x;
  if constexpr (sizeof...(S)) cout << ' ';
  print(forward<S>(y)...);
}
void put() { cout << ' '; }
TES void put(T &&x, S &&...y) {
  cout << x;
  if constexpr (sizeof...(S)) cout << ' ';
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
#line 5 "YRS/all.hpp"

#if (__cplusplus >= 202002L)
#include <numbers>
constexpr ld pi = numbers::pi;
#endif
TE constexpr T inf = numeric_limits<T>::max();
template <> constexpr i128 inf<i128> = i128(inf<ll>) * 2'000'000'000'000'000'000;
template <typename T, typename U>
constexpr pair<T, U> inf<pair<T, U>> = {inf<T>, inf<U>};

TE constexpr static inline int pc(T x) { return popcount(make_unsigned_t<T>(x)); }
constexpr static inline ll len(const Z &a) { return a.size(); }

void reverse(Z &a) { reverse(all(a)); }

void unique(Z &a) {
  sort(a);
  a.erase(unique(all(a)), a.end());
}
TE vc<int> inverse(const vc<T> &a) {
  int N = len(a);
  vc<int> b(N, -1);
  FOR(i, N) if (a[i] != -1) b[a[i]] = i;
  return b;
}

Z QMAX(const Z &a) { return *max_element(all(a)); }
Z QMIN(const Z &a) { return *min_element(all(a)); }
TE Z QMAX(T l, T r) { return *max_element(l, r); }
TE Z QMIN(T l, T r) { return *min_element(l, r); }
constexpr bool chmax(Z &a, const Z &b) { return (a < b ? a = b, 1 : 0); }
constexpr bool chmin(Z &a, const Z &b) { return (a > b ? a = b, 1 : 0); }

vc<int> argsort(const Z &a) {
  vc<int> I(len(a));
  iota(all(I), 0);
  sort(I, [&](int i, int k) { return a[i] < a[k] or (a[i] == a[k] and i < k); });
  return I;
}
TE vc<T> rearrange(const vc<T> &a, const vc<int> &I) {
  int N = len(I);
  vc<T> b(N);
  FOR(i, N) b[i] = a[I[i]];
  return b;
}
template <int of = 1, typename T> 
vc<T> pre_sum(const vc<T> &a) {
  int N = len(a);
  vc<T> c(N + 1);
  FOR(i, N) c[i + 1] = c[i] + a[i];
  if (of == 0) c.erase(c.begin());
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

TE constexpr T floor(T x, T y) { return x / y - (x % y and (x ^ y) < 0); }
TE constexpr T ceil(T x, T y) { return floor(x + y - 1, y); }
TE constexpr T bmod(T x, T y) { return x - floor(x, y) * y; }
TE constexpr pair<T, T> divmod(T x, T y) {
  T q = floor(x, y);
  return pair{q, x - q * y};
}
template <typename T = ll>
T SUM(const Z &v) {
  return accumulate(all(v), T(0));
}
int lb(const Z &a, Z x) { return lower_bound(all(a), x) - a.begin(); }
TE int lb(T l, T r, Z x) { return lower_bound(l, r, x) - l; }
int ub(const Z &a, Z x) { return upper_bound(all(a), x) - a.begin(); }
TE int ub(T l, T r, Z x) { return upper_bound(l, r, x) - l; }

template <bool ck = 1>
ll bina(Z f, ll l, ll r) {
  if constexpr (ck) assert(f(l));
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

Z pop(Z &s) {
  if constexpr (requires { s.pop_back(); }) {
    Z x = s.back();
    return s.pop_back(), x;
  } else if constexpr (requires { s.top(); }) {
    Z x = s.top();
    return s.pop(), x;
  } else {
    Z x = s.front();
    return s.pop(), x;
  }
}
void setp(int x) { cout << fixed << setprecision(x); }

TE inline void sh(vc<T> &a, int N, T b = {}) {
  a.resize(N, b);
}
#line 1 "YRS/debug.hpp"
#ifdef YRSD
void DBG() { cerr << "]" << endl; }
TES void DBG(T &&x, S &&...y) {
  cerr << x;
  if constexpr (sizeof...(S)) cerr << ", ";
  DBG(forward<S>(y)...);
}
#define debug(...) cerr << "[" << __LINE__ << "]: [" #__VA_ARGS__ "] = [", DBG(__VA_ARGS__)
void ERR() { cerr << endl; }
TES void ERR(T &&x, S &&...y) {
  cerr << x;
  if constexpr (sizeof...(S)) cerr << ", ";
  ERR(forward<S>(y)...);
}
#define err(...) cerr << "[" << __LINE__ << "]: ", ERR(__VA_ARGS__)
#define asser assert
#else
#define debug(...) void(0721)
#define err(...)   void(0721)
#define asser(...) void(0721)
#endif
#line 2 "YRS/IO/fast_io.hpp"

#define FIO

static constexpr uint SZ = 1 << 17;
char ibuf[SZ];
char obuf[SZ];
char out[100];
// pointer of ibuf, obuf
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
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) wt(s[i]);
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

TE inline void wt_real(T x) {
  ostringstream oss;
  oss << fixed << setprecision(10) << double(x);
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
  int N = len(a);
  FOR(i, N) {
    if (i) wt(' ');
    wt(a[i]);
  }
}
TE inline void wt(const vc<vc<T>> &v) {
  int N = len(v);
  FOR(i, N) {
    wt(v[i]);
    if (i + 1 != N) wt('\n');
  }
}
template <typename T, const size_t s>
inline void wt(const vc<array<T, s>> &v) {
  int N = len(v);
  FOR(i, N) {
    wt(v[i]);
    if (i + 1 != N) wt('\n');
  }
}

// gcc expansion. called automaticall after main.
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
#line 6 "Log_of_Set_Power_Series.cpp"
// #include "YRS/random/rng.hpp"
// #include "YRS/ds/basic/retsu.hpp"
#line 2 "YRS/mod/mint.hpp"

#line 2 "YRS/mod/modint_common.hpp"

TE concept is_mint = requires(T x) {
  { T::get_mod() };
  { T::gen(0ull) } -> same_as<T>;
  x.val;
};
TE concept has_const_mod =
    requires { integral_constant<int, (int)T::get_mod()> {}; };

TE static vc<T> &invs() {
  static vc<T> a{0, 1};
  return a;
}
TE static vc<T> &fac() {
  static vc<T> a{1, 1};
  return a;
}
TE static vc<T> &ifac() {
  static vc<T> a{1, 1};
  return a;
}

TE static int Set_inv(int N) {
  static vc<T> &inv = invs<T>();
  if (len(inv) >= N) return N;
  inv.resize(N + 1);
  inv[0] = 1, inv[1] = 1;
  FOR(i, 1, N) inv[i + 1] = inv[i] * i;
  T t = pop(inv).inv();
  FOR_R(i, N) inv[i] *= t, t *= i;
  return N;
}
TE static int Set_comb(int N) {
  static vc<T> &fa = fac<T>(), &ifa = ifac<T>();
  if (len(fa) >= N) return N;
  fa.resize(N);
  ifa.resize(N);
  FOR(i, 1, N) fa[i] = fa[i - 1] * i;
  ifa[N - 1] = fa[N - 1].inv();
  FOR_R(i, N - 1) ifa[i] = ifa[i + 1] * (i + 1);
  return N;
}

template <typename mint>
mint inv(int n) {
  static const int mod = mint::get_mod();
  static vc<mint> &a = invs<mint>();
  assert(0 <= n);
  while (len(a) <= n) {
    int k = len(a);
    int q = (mod + k - 1) / k;
    int r = k * q - mod;
    a.ep(a[r] * mint(q));
  }
  return a[n];
}
template <typename mint>
mint fact(int n) {
  static const int mod = mint::get_mod();
  static vc<mint> &a = fac<mint>();
  assert(0 <= n);
  if (n >= mod) return 0;
  while (len(a) <= n) {
    int k = len(a);
    a.ep(a[k - 1] * mint(k));
  }
  return a[n];
}

template <typename mint>
mint fact_inv(int n) {
  static vc<mint> &a = ifac<mint>();
  if (n < 0) return mint(0);
  while (len(a) <= n)
    a.ep(a[len(a) - 1] * inv<mint>(len(a)));
  return a[n];
}

template <typename mint, typename... Ts>
mint fact_invs(Ts... xs) {
  return (mint(1) * ... * fact_inv<mint>(xs));
}

template <typename mint, typename X, typename... S>
mint multinomial(X&& a, S&&... b) {
  return fact<mint>(a) * fact_invs<mint>(forward<S>(b)...);
}

template <typename mint>
mint C_dense(int n, int k) {
  assert(n >= 0);
  if (k < 0 or n < k) return 0;
  static vc<vc<mint>> C;
  static int H = 0, W = 0;
  Z calc = [&](int i, int j) -> mint {
    if (i == 0) return(j == 0 ? mint(1) : mint(0));
    return C[i - 1][j] + (j ? C[i - 1][j - 1] : 0);
  };
  if (W <= k) {
    for (int i = 0; i < H; ++i) {
      C[i].resize(k + 1);
      for (int j = W; j < k + 1; ++j) {
        C[i][j] = calc(i, j);
      }
    }
    W = k + 1;
  }
  if (H <= n) {
    C.resize(n + 1);
    for (int i = H; i < n + 1; ++i) {
      C[i].resize(W);
      for (int j = 0; j < W; ++j) {
        C[i][j] = calc(i, j);
      }
    }
    H = n + 1;
  }
  return C[n][k];
}

template <typename mint>
mint C(int N, int K) {
  assert(N >= 0);
  if (K < 0 or N < K) return 0;
  return fact<mint>(N) * fact_inv<mint>(K) * fact_inv<mint>(N - K);
}

template <typename mint>
mint lucas(ll N, ll K) {
  static constexpr int P = mint::get_mod();
  if (K > N) return 0;
  if (K == 0) return 1;
  return C<mint>(N % P, K % P) * lucas<mint>(N / P, K / P);
}

template <typename mint, bool large = false, bool dense = false>
mint binom(ll n, ll k) {
  assert(n >= 0);
  if (k < 0 or n < k) return 0;
  if constexpr (dense) return C_dense<mint>(n, k);
  if constexpr (not large) return multinomial<mint>(n, k, n - k);
  k = min(k, n - k);
  mint x(1);
  FOR(i, k) x *= mint(n - i);
  return x * fact_inv<mint>(k);
}

template <typename mint, bool large = false>
mint C_inv(ll n, ll k) {
  assert(n >= 0);
  assert(0 <= k and k <= n);
  if (not large) return fact_inv<mint>(n) * fact<mint>(k) * fact<mint>(n - k);
  return mint(1) / binom<mint, 1>(n, k);
}

// [x^d](1-x)^{-n}
template <typename mint, bool large = false, bool dense = false>
mint C_negative(ll n, ll d) {
  assert(n >= 0);
  if (d < 0) return mint(0);
  if (n == 0) return (d == 0 ? mint(1) : mint(0));
  return binom<mint, large, dense>(n + d - 1, d);
}

#define fac fact<T>
#define ifac fact_inv<T>
#define CC C<mint>
#define set_comb Set_comb<mint>
#define set_inv Set_inv<mint>
#line 4 "YRS/mod/mint.hpp"

#define C constexpr
template <int mod>
struct mint_t {
  using mint = mint_t;
  static C uint m = mod;
  uint x;

  C inline uint val() const { return x; }

  C mint_t() : x(0) {}
  C mint_t(uint x) : x(x % m) {}
  C mint_t(ull x) : x(x % m) {}
  C mint_t(u128 x) : x(x % m) {}
  C mint_t(int x) : x((x %= mod) < 0 ? x + mod : x) {}
  C mint_t(ll x) : x((x %= mod) < 0 ? x + mod : x) {}
  C mint_t(i128 x) : x((x %= mod) < 0 ? x + mod : x) {}

  C mint &operator+=(mint p) {
    if ((x += p.x) >= m) x -= m;
    return *this;
  }
  C mint &operator-=(mint p) {
    if ((x += m - p.x) >= m) x -= m;
    return *this;
  }
  C mint operator+(mint p) const { return mint(*this) += p; }
  C mint operator-(mint p) const { return mint(*this) -= p; }

  C mint &operator*=(mint p) {
    x = ull(x) * p.x % m;
    return *this;
  }
  C mint operator*(mint p) const { return mint(*this) *= p; }

  C mint &operator/=(mint p) { return *this *= p.inv(); }
  C mint operator/(mint p) const { return mint(*this) /= p; }

  C mint operator-() const { return mint::gen(x ? mod - x : 0); }

  C mint inv() const {
    int a = x, b = mod, x = 1, y = 0;
    while (b > 0) {
      int t = a / b;
      swap(a -= t * b, b);
      swap(x -= t * y, y);
    }
    return mint(x);
  }

  C mint pow(ll k) const {
    if (k < 0) return inv().pow(-k);
    mint s(1), a(x);
    for (; k; k >>= 1, a *= a)
      if (k & 1) s *= a;
    return s;
  }

  C bool operator<(mint p) const { return x < p.x; }
  C bool operator==(mint p) const { return x == p.x; }
  C bool operator!=(mint p) const { return x != p.x; }

  static C mint gen(uint x) {
    mint s;
    s.x = x;
    return s;
  }

  friend istream &operator>>(istream &cin, mint &p) {
    ll t;
    cin >> t;
    p = t;
    return cin;
  }
  friend ostream &operator<<(ostream &cout, mint p) { return cout << p.x; }

  static C int get_mod() { return mod; }

  static C PII ntt_info() {
    if (mod == 167772161) return {25, 17};
    if (mod == 469762049) return {26, 30};
    if (mod == 754974721) return {24, 362};
    if (mod == 998244353) return {23, 31};
    if (mod == 120586241) return {20, 74066978};
    if (mod == 880803841) return {23, 211};
    if (mod == 943718401) return {22, 663003469};
    if (mod == 1004535809) return {21, 582313106};
    if (mod == 1012924417) return {21, 368093570};
    return {-1, -1};
  }
  
  static C bool can_ntt() { return ntt_info().fi != -1; }
};
#undef C

using M99 = mint_t<998244353>;
using M17 = mint_t<1000000007>;

#ifdef FIO
template <int mod>
void rd(mint_t<mod> &x) {
  LL(y);
  x = y;
}
template <int mod>
void wt(mint_t<mod> x) {
  wt(x.x);
}
#endif
#line 2 "YRS/sps/all.hpp"

#line 2 "YRS/sps/conv.hpp"

TE vc<T> sps_conv(const vc<T> &a, const vc<T> &b) {
  int N = len(a), n = lowbit(N);
  assert((1 << n) == N);
  assert(N == len(b));
  vc<T> x((n + 1) << n), y(x);
  FOR(i, N) x[i * (n + 1) + pc(i)] = a[i];
  FOR(i, N) y[i * (n + 1) + pc(i)] = b[i];

  FOR(i, N) {
    int s = (n + 1) * i, t = (n + 1) * (i + 1), z = lowbit(N | i);
    FOR_R(j, z) {
      int w = (n + 1) << j;
      FOR(k, w) x[s + w + k] += x[s + k];
      FOR(k, w) y[s + w + k] += y[s + k];
    }
    int c = pc(i);
    FOR_R(j, c, min(n, c << 1) + 1) {
      T v = 0;
      FOR(k, j - c, c + 1) v += x[s + k] * y[s + j - k];
      x[s + j] = v;
    }
    c = lowbit(~i);
    FOR(j, c) {
      int w = (n + 1) << j, s = t - 2 * w;
      FOR(k, w) x[s + w + k] -= x[s + k];
    }
  }
  vc<T> c(N);
  FOR(i, N) c[i] = x[i * (n + 1) + pc(i)];
  return c;
}
#line 2 "YRS/sps/conv_inv.hpp"

// a * b = c, return b
TE vc<T> sps_conv_inv(const vc<T> &a, const vc<T> &c) {
  int N = len(a), n = lowbit(N);
  assert((1 << n) == N);
  assert(N == len(c));
  assert(a[0] == T(1));
  vc<T> x((n + 1) << n), y(x);
  FOR(i, N) x[i * (n + 1) + pc(i)] = a[i];
  FOR(i, N) y[i * (n + 1) + pc(i)] = c[i];
  FOR(i, N) {
    int s = (n + 1) * i, t = (n + 1) * (i + 1), z = lowbit(1 << n | i);
    FOR_R(j, z) {
      int w = (n + 1) << j;
      FOR(k, w) x[s + w + k] += x[s + k];
      FOR(k, w) y[s + w + k] += y[s + k];
    }
    int c = pc(i);
    FOR(j, n + 1) {
      T v = y[s + j];
      FOR(k, 1, min(n - j, c) + 1) y[s + j + k] -= v * x[s + k];
    }
    c = lowbit(~i);
    FOR(j, c) {
      int w = (n + 1) << j, s = t - 2 * w;
      FOR(k, w) y[s + w + k] -= y[s + k];
    }
  }
  vc<T> b(N);
  FOR(i, N) b[i] = y[i * (n + 1) + pc(i)];
  return b;
}
#line 2 "YRS/sps/exp.hpp"

#line 4 "YRS/sps/exp.hpp"

TE vc<T> sps_exp(const vc<T> &a) {
  int N = len(a), n = lowbit(N);
  assert((1 << n) == N);
  assert(a[0] == T(0));
  vc<T> s(N);
  s[0] = 1;
  FOR(i, n) {
    vc<T> x(begin(s), begin(s) + (1 << i));
    vc<T> y(begin(a) + (1 << i), begin(a) + (2 << i));
    vc<T> c = sps_conv(x, y);
    copy(all(c), begin(s) + (1 << i));
  }
  return s;
}
#line 2 "YRS/sps/log.hpp"

#line 4 "YRS/sps/log.hpp"

TE vc<T> sps_log(const vc<T>& a){
  int N = len(a), n = lowbit(N);
  assert((1 << n) == N);
  assert(a[0] == T(1));
  vc<T> s(N);
  FOR(i, n) {
    vc<T> x(begin(a), begin(a) + (1 << i));
    vc<T> y(begin(a) + (1 << i), begin(a) + (2 << i));
    vc<T> c = sps_conv_inv(x, y);
    copy(all(c), begin(s) + (1 << i));
  }
  return s;
}
#line 2 "YRS/sps/inv.hpp"

#line 4 "YRS/sps/inv.hpp"

TE vc<T> sps_inv(vc<T> a) {
  assert(a[0] != T(0));
  int N = len(a);
  T in = a[0].inv();
  FOR(i, N) a[i] *= in;
  vc<T> c(N); c[0] = 1;
  vc<T> b = sps_conv_inv(a, c);
  FOR(i, N) b[i] *= in;
  return b;
}
#line 2 "YRS/sps/comp.hpp"

#line 4 "YRS/sps/comp.hpp"

// sps comp poly
// f poly, g sps, return f(g(x))
TE vc<T> sps_comp(vc<T> f, const vc<T> &g) {
  int N = len(g), n = lowbit(N);
  assert((1 << n) == N);
  vc<T> dp[24];
  FOR(i, n + 1) {
    vc<T> &d = dp[i];
    T r = g[0], s = 0;
    int sz = len(f);
    FOR_R(i, sz) s = s * r + f[i];
    d = {s};
    FOR(i, 1, sz) f[i - 1] = f[i] * i;
    if (sz > 0) sh(f, sz - 1);
  }
  FOR(i, n) {
    vc<T> a(begin(g) + (1 << i), begin(g) + (2 << i));
    int sz = n - i + 1;
    FOR(j, 1, sz) {
      vc<T> h = sps_conv(a, dp[j]);
      copy(all(h), back_inserter(dp[j - 1]));
    }
  }
  return dp[0];
}
#line 1 "YRS/aa/def.hpp"
#ifdef fac
#undef fac
#undef ifac
#endif
#define fac fact<mint>
#define ifac fact_inv<mint>
#line 11 "Log_of_Set_Power_Series.cpp"

using mint = M99;
void Yorisou() {
  INT(N);
  // VEC(mint, f, 1 << N);
  N = 1 << N;
  vc<mint> f(N);
  FOR(i, N) {
    INT(x);
    f[i] = mint::gen(x);
  }
  print(sps_log(f));
}
constexpr int tests = 0, fl = 0, DB = 10;
#line 1 "YRS/aa/main.hpp"
int main() {
  cin.tie(nullptr)->sync_with_stdio(0);
  int T = 1;
  if (fl) cerr.tie(0);
  if (tests and not fl) IN(T);
  for (int i = 0; i < T or fl; ++i) {
    Yorisou();
    if (fl and i % DB == 0) cerr << "Case: " << i << '\n';
  }
  return 0;
}
#line 26 "Log_of_Set_Power_Series.cpp"
