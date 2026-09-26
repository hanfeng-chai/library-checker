#line 1 "P11175.cpp"
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
constexpr ld pi = numbers::pi_v<ld>;
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
#line 2 "YRS/random/rng.hpp"

#include <random>

#ifdef MeIoN
std::mt19937 rg(0);
std::mt19937_64 rd_64(0);
#else
std::mt19937 rg(std::chrono::steady_clock::now().time_since_epoch().count());
std::mt19937_64 rd_64(std::chrono::steady_clock::now().time_since_epoch().count());
#endif

uint rng() { return rg(); }
uint rng(uint lim) { return rg() % lim; }
int rng(int l, int r) { return l + rg() % (r - l); }
ull rng_64() { return rd_64(); }
ull rng_64(ull lim) { return rd_64() % lim; }
ll rng_64(ll l, ll r) { return l + rd_64() % (r - l); }

template <typename T>
void shuffle(vector<T> &v) {
  const int N = len(v);
  FOR(i, 1, N) {
    int k = rng(0, i + 1);
    if (i != k) swap(v[i], v[k]);
  }
}
#line 7 "P11175.cpp"
// #include "YRS/ds/basic/retsu.hpp"
// #include "YRS/mod/mint.hpp"
// #include "YRS/aa/def.hpp"
#line 2 "YRS/mod/log_table.hpp"

#line 2 "YRS/mod/primitive_root.hpp"

#line 2 "YRS/pr/factors.hpp"

#line 2 "YRS/pr/ptest.hpp"

struct MM {
  using uu = unsigned __int128;
  inline static ull m, r, nn;
  static void set_mod(ull m) {
    MM::m = m;
    nn = -uu(m) % m;
    r = m;
    FOR(5) r *= 2 - m * r;
    r = -r;
  }
  static ull reduce(uu x) { return (x + uu(ull(x) * r) * m) >> 64; }

  ull x;
  MM() : x(0) {}
  MM(ull x) : x(reduce(uu(x) * nn)) {}
  ull val() const {
    ull y = reduce(x);
    return y >= m ? y - m : y;
  }
  MM &operator+=(MM y) {
    x += y.x - (m << 1);
    x = (ll(x) < 0 ? x + (m << 1) : x);
    return *this;
  }
  MM &operator-=(MM y) {
    x -= y.x;
    x = (ll(x) < 0 ? x + (m << 1) : x);
    return *this;
  }
  MM &operator*=(MM y) {
    x = reduce(uu(x) * y.x);
    return *this;
  }
  MM operator+(MM y) const { return MM(*this) += y; }
  MM operator-(MM y) const { return MM(*this) -= y; }
  MM operator*(MM y) const { return MM(*this) *= y; }
  bool operator==(MM y) const {
    return (x >= m ? x - m : x) == (y.x >= m ? y.x - m : y.x);
  }
  bool operator!=(MM y) const { return not operator==(y); }
  MM pow(ull k) const {
    MM r = 1, a = *this;
    for (; k; k >>= 1, a *= a) if (k & 1) r *= a;
    return r;
  }
};

bool ptest(const ull x) {
  if (x == 2 or x == 3 or x == 5 or x == 7) return 1;
  if (x % 2 == 0 or x % 3 == 0 or x % 5 == 0 or x % 7 == 0) return 0;
  if (x < 121) return x > 1;
  const ull d = (x - 1) >> __builtin_ctzll(x - 1);
  MM::set_mod(x);
  const MM o(1), mo(x - 1);
  Z f = [&](ull a) -> bool {
    MM y = MM(a).pow(d);
    ull t = d;
    while (y != o and y != mo and t != x - 1) y *= y, t <<= 1;
    if (y != mo and t % 2 == 0) return 1;
    return 0;
  };
  if (x < (1ull << 32)) {
    for (ull a : {2, 7, 61}) if (f(a)) return 0;
  } else {
    for (ull a : {2, 325, 9'375, 281'78, 450'775, 978'050'4, 179'526'502'2}) {
      if (x <= a) return 1;
      if (f(a)) return 0;
    }
  }
  return 1;
}
ll rho(ll n, ll c) {
  MM::set_mod(n);
  const MM cc(c);
  Z f = [&](MM x) { return x * x + cc; };
  MM x = 1, y = 2, z = 1, q = 1;
  ll g = 1;
  const ll m = 1ll << (__lg(n) / 5);
  for (ll r = 1; g == 1; r <<= 1) {
    x = y;
    FOR(r) y = f(y);
    for (ll k = 0; k < r and g == 1; k += m) {
      z = y;
      FOR(i, min(m, r - k)) y = f(y), q *= x - y;
      g = gcd(q.val(), n);
    }
  }
  if (g == n) do {
    z = f(z);
    g = gcd((x - z).val(), n);
  } while (g == 1);
  return g;
}
#line 5 "YRS/pr/factors.hpp"

// https://yukicoder.me/problems/no/36 => factor();

ll find_pr_e(ll x) {
  assert(x > 1);
  if (ptest(x)) return x;
  FOR(100) {
    ll e = rho(x, rng_64(x));
    if (ptest(e)) return e;
    x = e;
  }
  err("failed");
  assert(0);
}

vc<pair<ll, int>> factor(ll x) {
  assert(x >= 1);
  vc<pair<ll, int>> r;
  for (int e = 2; e < 100; ++e) {
    if (e * e > x) break;
    if (x % e == 0) {
      int c = 0;
      do {
        x /= e, c += 1;
      } while (x % e == 0);
      r.ep(e, c);
    }
  }
  while (x > 1) {
    ll e = find_pr_e(x);
    int c = 0;
    do {
      x /= e, c += 1;
    } while (x % e == 0);
    r.ep(e, c);
  }
  return sort(r), r;
}
vc<pair<ll, int>> factor_by_lpf(ll n, vc<int> &lpf) {
  vc<pair<ll, int>> s;
  while (n > 1) {
    int p = lpf[n], e = 0;
    while (n % p == 0) n /= p, ++e;
    s.ep(p, e);
  }
  return s;
}
#line 2 "YRS/mod/mod_pow.hpp"

#line 2 "YRS/mod/barrett.hpp"

struct barrett {
  uint m;
  ull im;

  explicit barrett(uint m = 1) : m(m), im(ull(-1) / m + 1) {}

  uint mo(ull z) const {
    if (m == 1) return 0;
    ull x = ull((u128(z) * im) >> 64);
    ull y = x * m;
    return (z - y + (z < y ? m : 0));
  }

  inline uint mul(uint a, uint b) const { return mo(ull(a) * b); }

  uint pow(uint a, uint b) const {
    uint s = 1;
    for (; b; b >>= 1, a = mul(a, a)) {
      if (b & 1) s = mul(s, a);
    }
    return s - (s >= m ? m : 0);
  }

  ull floor(ull z) const {
    if (m == 1) return z;
    ull x = (ull)(((u128)z * im) >> 64);
    ull y = x * m;
    return (z < y ? x - 1 : x);
  }

  pair<ull, uint> divmod(ull z) const {
    if (m == 1) return {z, 0};
    ull x = ull((u128(z) * im) >> 64), y = x * m;
    if (z < y) return {x - 1, z - y + m};
    return {x, z - y};
  }

  uint umod() const { return m; }
};

struct barrett_64 {
  u128 m, mh, ml;

  explicit barrett_64(ull mod = 1) : m(mod) {
    u128 m = u128(-1) / mod;
    if (m * mod + mod == u128(0)) ++m;
    mh = m >> 64;
    ml = m & ull(-1);
  }

  ull mo(u128 x) const {
    u128 z = (x & ull(-1)) * ml;
    z = (x & ull(-1)) * mh + (x >> 64) * ml + (z >> 64);
    z = (x >> 64) * mh + (z >> 64);
    x -= z * m;
    return x < m ? x : x - m;
  }

  inline ull mul(ull a, ull b) const { return mo(u128(a) * b); }

  ull pow(ull a, ull b) const {
    ull s = 1;
    for (; b; b >>= 1, a = mul(a, a)) {
      if (b & 1) s = mul(s, a);
    }
    return s - (s >= m ? m : 0);
  }

  ull umod() const { return m; }
};
#line 4 "YRS/mod/mod_pow.hpp"

uint mod_pow(int a, ll k, uint mod) {
  a %= mod;
  uint s = 1;
  barrett X(mod);
  for (; k; k >>= 1, a = X.mul(a, a)) 
    if (k & 1) s = X.mul(s, a);
  return s;
}
template <uint mod>
uint mod_pow(ull a, ull k) {
  a %= mod;
  ull s = 1;
  for (; k; k >>= 1, a = a * a % mod)
    if (k & 1) s = s * a % mod;
  return s;
}
// a ^ (b ^ c) % mod
template <uint mod>
uint mod_pow_tri(ull a, ll b, ull c) {
  return a % mod == 0 ? 0 : mod_pow<mod>(a, mod_pow<mod - 1>(b, c));
}
ull mod_pow_64(ll a, ll k, ull mod) {
  a %= mod;
  ll s = 1;
  for (; k; k >>= 1, a = u128(a) * a % mod) 
    if (k & 1) s = u128(s) * a % mod;
  return s;
}
#line 6 "YRS/mod/primitive_root.hpp"

int primitive_root(int p) {
  Z pf = factor(p - 1);
  Z is_ok = [&](int g) -> bool {
    for (Z [q, e] : pf)
      if (mod_pow(g, (p - 1) / q, p) == 1) return 0;
    return 1;
  };
  while (1) {
    int x = rng(1, p);
    if (is_ok(x)) return x;
  }
  return -1;
}

ll primitive_root_64(ll p) {
  Z pf = factor(p - 1);
  Z is_ok = [&](ll g) -> bool {
    for (Z [q, e]: pf)
      if (mod_pow_64(g, (p - 1) / q, p) == 1) return 0;
    return 1;
  };
  while (1) {
    ll x = rng_64(1, p);
    if (is_ok(x)) return x;
  }
  return -1;
}
#line 4 "YRS/mod/log_table.hpp"

// O(mod)
vc<int> log_table(int mod, int r = -1) {
  vc<int> t(mod);
  int p = mod - 1, g = r == -1 ? primitive_root(mod) : r, c = 1;
  t[1] = 0;
  FOR(e, 1, p) t[c = 1ll * c * g % mod] = e;
  return t;
}
#line 2 "YRS/mod/modfast_d.hpp"

#line 2 "YRS/pr/lpf_table.hpp"

#line 2 "YRS/pr/ptable.hpp"

// [0, lm]
inline vc<int> ptable(int lm) {
  ++lm;
  static constexpr int sz = 32768;
  static int N = 2;
  static vc<int> s{2}, vis(sz + 1);

  if (N < lm) {
    N = lm;
    s = {2}, vis.assign(sz + 1, 0);
    int R = lm / 2;
    s.reserve(int(lm / log(lm) * 1.1));
    vc<PII> cp;
    FOR(i, 3, sz + 1, 2) {
      if (not vis[i]) {
        cp.ep(i, 1ll * i * i / 2);
        FOR(j, 1ll * i * i, sz + 1, i << 1) vis[j] = 1;
      }
    }
    FOR(L, 1, R + 1, sz) {
      array<bool, sz> f{};
      for (Z &[p, id] : cp)
        for (int i = id; i < sz + L; id = (i += p)) f[i - L] = 1;
      FOR(i, min(sz, R - L)) if (not f[i]) s.ep((L + i) << 1 | 1);
    }
  }
  int k = lb(s, lm + 1);
  return {s.begin(), s.begin() + k};
}
#line 4 "YRS/pr/lpf_table.hpp"

// 最大质因子 [0, N] minp or -1
vc<int> lpf_table(int N) {
  vc<int> a = ptable(N), b(N + 1, -1);
  int sz = len(a);
  FOR_R(i, sz) {
    int p = a[i];
    FOR(k, 1, N / p + 1) b[p * k] = p;
  }
  return b;
}
#line 2 "YRS/ds/basic/hashmap.hpp"

#line 2 "YRS/random/hash.hpp"

#line 4 "YRS/random/hash.hpp"

TE ull hsh(const pair<T, T> &X) {
  static ull B = rng_64();
  if (not B) B = rng_64();
  return B * X.fi + X.se;
}
#line 4 "YRS/ds/basic/hashmap.hpp"

// https://judge.yosupo.jp/problem/associative_array

// reserve N 时最快
TE struct hashmap {
  uint ls, msk;
  vc<ull> ke;
  vc<T> val;
  vc<u8> vis;

  ull hash(ull x) const {
    static const ull bs =
        chrono::steady_clock::now().time_since_epoch().count();
    x += bs;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return (x ^ (x >> 31)) & msk;
  }

  void extend() {
    vc<pair<ull, T>> dat;
    const int N = len(vis);
    dat.reserve(N / 2 - ls);
    FOR(i, N) if (vis[i]) dat.ep(ke[i], val[i]);
    build(dat.size() << 1);
    for (Z &[a, b] : dat) (*this)[a] = b;
  }

  hashmap(uint N = 0) { build(N); }

  void build(uint N) {
    uint k = 8;
    while (k < (N << 1)) k <<= 1;
    ls = k >> 1, msk = k - 1;
    ke.resize(k);
    val.resize(k);
    vis.assign(k, 0);
  }

  void clear() {
    fill(all(vis), 0);
    ls = (msk + 1) >> 1;
  }

  ll size() const { return vis.size() / 2 - ls; }

  int id(ull k) const {
    int i = hash(k);
    while (vis[i] and ke[i] != k) i = (i + 1) & msk;
    return i;
  }

  T &operator[](ull k) {
    if (ls == 0) extend();
    int i = id(k);
    if (not vis[i]) {
      vis[i] = 1;
      ke[i] = k;
      val[i] = T {};
      --ls;
    }
    return val[i];
  }
  
  T &operator[](PII p) {
    ll k = hsh(p);
    if (ls == 0) extend();
    int i = id(k);
    if (not vis[i]) {
      vis[i] = 1;
      ke[i] = k;
      val[i] = T {};
      --ls;
    }
    return val[i];
  }

  T get(ull k, T fail) const {
    int i = id(k);
    return (vis[i] ? val[i] : fail);
  }

  bool contains(ull k) const {
    int i = id(k);
    return vis[i] and ke[i] == k;
  }

  vc<pair<ull, T>> get_all() const {
    int N = len(vis);
    vc<pair<ull, T>> s;
    FOR(i, N) if (vis[i]) s.ep(ke[i], val[i]);
    return s;
  }

  // f(ke, val);
  void enumerate_all(Z f) const {
    const int N = len(vis);
    FOR(i, N) if (vis[i]) f(ke[i], val[i]);
  }
};
#line 7 "YRS/mod/modfast_d.hpp"

struct modfast_d {
  const int p;
  const uint g;
  const barrett X, Y;

  static constexpr int K = 1 << 21;
  inline static array<uint, 65537> pw[2];
  inline static array<pair<u16, u16>, 1 << 20 | 1> fra;
  inline static array<uint, K << 1 | 1> lg, in;
  // inline static array<uint, 1> pw[2];
  // inline static array<pair<u16, u16>, 1> fra;
  // static constexpr int K = 1;

  modfast_d(int p, int g) : p(p), g(g), X(p), Y(p - 1) { build(); }

  modfast_d(int p) : p(p), g(primitive_root(p)), X(p), Y(p - 1) { build(); }

  void build() {
    build_pow();
    // build_inv();
    build_log();
    build_frac();
  }

  uint pow(uint a, ll k) {
    assert(0 <= a and a < (uint)p and 0 <= k and k < (1 << 30));
    if (a == 0) return k == 0 ? 1 : 0;
    return pow_r_32(log_r(a) * k % (p - 1));
  }

  uint pow_r_32(uint k) {
    assert(0 <= k and k < (uint)p);
    return X.mul(pw[0][k & 32767], pw[1][k >> 15]);
  }

  uint pow_r(ll k) {
    assert(k >= 0);
    k = Y.mo(k);
    return X.mul(pw[0][k & 32767], pw[1][k >> 15]);
  }

  uint log_r(uint x) {
    assert(0 < x and x < (uint)p);
    Z [a, b] = fra[x >> 10];
    uint t = x * b - uint(a) * p;
    return Y.mo(lg[K + t] - lg[K + b] + p - 1);
  }

  uint inv(uint x) {
    assert(0 < x and x < (uint)p);
    Z [a, b] = fra[x >> 10];
    uint t = x * b - uint(a) * p;
    return X.mul(in[K + t], b);
  }
  
  void build_pow() {
    pw[0][0] = pw[1][0] = 1;
    FOR(i, 1 << 15) pw[0][i + 1] = X.mul(pw[0][i], g);
    FOR(i, 1 << 15) pw[1][i + 1] = X.mul(pw[1][i], pw[0][1 << 15]);
  }

  void build_inv() {
    in[K + 1] = 1;
    for (uint i = 2; i < K + 1; ++i) {
      ull q = (p + i - 1) / i;
      in[K + i] = in[K + i * q - p] * ull(q) % p;
    }
    FOR(i, 1, K + 1) in[K - i] = p - in[K + i];
  }

  void build_log() {
    constexpr int lm = 1 << 21, S = 1 << 17;
    vc<int> lpf = lpf_table(lm);

    hashmap<uint> mp(S);
    uint w = 1;
    FOR(k, S) mp[w] = k, w = ull(g) * w % p;

    uint q = pow_r_32((p - 1 - S % (p - 1)) % (p - 1));

    Z bsgs = [&](uint s) -> uint {
      uint ans = 0;
      while (1) {
        uint x = mp.get(s, -1);
        if (x != uint(-1)) return (ans + x) % (p - 1);
        ans += S, s = ull(s) * q % p;
      }
      return 0;
    };
    lg[K + 1] = 0;
    FOR(i, 2, 1 + (1 << 21)) {
      if (i >= p) {
        lg[K + i] = lg[K + i % p];
        continue;
      }
      if (lpf[i] < i) {
        lg[K + i] = (lg[K + lpf[i]] + lg[K + i / lpf[i]]) % (p - 1);
        continue;
      }
      if (i < 100) {
        lg[K + i] = bsgs(i);
        continue;
      }
      if (1ll * i * i > p) {
        Z [j, k] = divmod(p, i);
        lg[K + i] = (lg[K + k] + (p - 1) / 2 + p - 1 - lg[K + j]) % (p - 1);
        continue;
      }
      while (1) {
        uint k = rng(0, p - 1);
        ull ans = p - 1 - k;
        uint x = ull(i) * pow_r_32(k) % p;
        Z f = [&](uint q) { x /= q, ans += lg[K + q]; };
        static constexpr uint qs[]{2, 3, 5, 7, 11, 13, 17, 19};
        for (uint q : qs) while (x % q == 0) f(q);
        if (x >= lm) continue;
        while ((uint)i < x and x < lm and lpf[x] < i) f(lpf[x]);
        if (1 < x and x < (uint)i) f(x);
        if (x == 1) {
          lg[K + i] = ans % (p - 1);
          break;
        }
      }
    }
    FOR(i, 1, 1 << 21 | 1) lg[K - i] = (lg[K + i] + (p - 1) / 2) % (p - 1);
  }

  void build_frac() {
    vc<T4<u16>> q;
    q.reserve(1'000'00);
    q.ep(0, 1, 1, 1);
    while (not q.empty()) {
      Z [a, b, c, d] = pop(q);
      if (b + d < 2048) {
        q.ep(a + c, b + d, c, d);
        q.ep(a, b, a + c, b + d);
        continue;
      }
      uint s = ull(a) * p / (1024 * b);
      uint t = ull(c) * p / (1024 * d);
      fra[s] = {a, b};
      fra[t] = {c, d};
      a = min(a, c), b = min(b, d);
      for (uint i = s + 1; i < t; ++i) fra[i] = {a, b};
    }
  }
};
#line 12 "P11175.cpp"


void Yorisou() {
  INT(mod, g, Q);
  if (mod <= 1'000'000) {
    vc<int> t = log_table(mod, g);
    FOR(Q) {
      INT(x);
      print(t[x]);
    }
    return;
  }
  modfast_d X(mod, g);
  FOR(Q) {
    INT(x);
    print(X.log_r(x));
  }
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
#line 32 "P11175.cpp"
