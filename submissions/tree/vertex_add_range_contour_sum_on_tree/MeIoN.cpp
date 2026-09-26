#line 1 "Vertex_Add_Range_Contour_Sum_on_Tree.cpp"
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

#define ov(a, b, c, d, e, ...) e
#define FO1(a) for (int _ = 0; _ < (a); ++_)
#define FO2(i, a) for (int i = 0; i < (a); ++i)
#define FO3(i, a, b) for (int i = (a); i < (b); ++i)
#define FO4(i, a, b, c) for (int i = (a); i < (b); i += (c))
#define FOR(...) ov(__VA_ARGS__, FO4, FO3, FO2, FO1)(__VA_ARGS__)
#define FF1(a) for (int _ = (a) - 1; _ >= 0; --_)
#define FF2(i, a) for (int i = (a) - 1; i >= 0; --i)
#define FF3(i, a, b) for (int i = (b) - 1; i >= (a); --i)
#define FF4(i, a, b, c) for (int i = (b) - 1; i >= (a); i -= (c))
#define FOR_R(...) ov(__VA_ARGS__, FF4, FF3, FF2, FF1)(__VA_ARGS__)
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
#line 2 "YRS/tr/near_kinbo.hpp"

#line 2 "YRS/tr/stt.hpp"

#line 2 "YRS/tr/hld.hpp"

#line 2 "YRS/g/Basic.hpp"

#line 2 "YRS/ds/basic/hashmap.hpp"

#line 2 "YRS/random/hash.hpp"

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
#line 4 "YRS/g/Basic.hpp"

// https://www.luogu.com.cn/problem/P5318

TE struct edge {
  int f, to;
  T w;
  int id;
};
template <typename T = int, bool dir = 0>
struct graph {
  static constexpr bool is_dir = dir;
  int N, M;
  using cost_type = T;
  using ee = edge<T>;
  vc<ee> es;
  vc<int> in;
  vc<ee> c;
  
  bool ok;

  bool isok() { return ok; }

  struct px {
    const graph *g;
    int l, r;
    px(const graph *g, int l, int r) : g(g), l(l), r(r) {}
    const ee *begin() const {
      if (l == r) return 0;
      return &g->c[l];
    }
    const ee *end() const {
      if (l == r) return 0;
      return &g->c[r];
    }
  };

  px operator[](int i) const {
    assert(ok);
    return {this, in[i], in[i + 1]};
  }

  graph() : N(0), M(0), ok(0) {}
  graph(int N) : N(N), M(0), ok(0) {}

  void add(int f, int t, T w = 1, int i = -1) {
    assert(not ok);
    assert(-1 < f and -1 < t and t < N and f < N);
    if (i == -1) i = M;
    es.ep(ee{f, t, w, i});
    ++M;
  }

  void build() {
    assert(not ok);
    ok = 1;
    in.assign(N + 1, 0);
    for (Z &&e : es) {
      in[e.f + 1]++;
      if (not dir) in[e.to + 1]++;
    }
    FOR(i, N) in[i + 1] += in[i];
    Z cc = in;
    c.resize(in.back() + 1);
    for (Z &&e : es) {
      c[cc[e.f]++] = e;
      if (not dir) c[cc[e.to]++] = {e.to, e.f, e.w, e.id};
    }
  }

  template <bool wt = 0, int of = 1>
  void sc() { sc<wt, of>(N - 1); }
  template <bool wt = 0, int of = 1>
  void sc(int M) {
    es.reserve(M * (dir ? 1 : 2));
    FOR(M) {
      INT(x, y);
      x -= of, y -= of;
      if (not wt) {
        add(x, y);
      } else {
        T w;
        IN(w);
        add(x, y, w);
      }
    }
    build();
  }

  vc<int> deg() {
    vc<int> in(N);
    for (Z &&e : es) ++in[e.f], ++in[e.to];
    return in;
  }

  pair<vc<int>, vc<int>> deg_inout() {
    vc<int> in(N), ou(N);
    for (Z &&e : es) ++in[e.to], ++ou[e.f];
    return {in, ou};
  }

  vc<int> ni;
  vc<u8> vis;
  // 使G中的顶点V[i]在新图表中为i
  // {G, es}
  // sum（deg(v)）的计算量
  // 注意它可能大于新图表的n+M
  graph<T, dir> rearrange(const vc<int> &v, bool keep_eid = 0) {
    if (len(ni) != N) ni.assign(N, -1);
    int N = len(v);
    FOR(i, N) ni[v[i]] = i;
    graph<T, dir> g(N);
    vc<int> s;
    FOR(i, N) {
      for (Z &&e : (*this)[v[i]]) {
        if (len(vis) <= e.id) vis.resize(e.id + 1);
        if (vis[e.id]) continue;
        int f = e.f, to = e.to;
        if (ni[f] != -1 and ni[to] != -1) {
          s.ep(e.id);
          vis[e.id] = 1;
          int id = (keep_eid ? e.id : -1);
          g.add(ni[f], ni[to], e.w, id);
        }
      }
    }
    FOR(i, N) ni[v[i]] = -1;
    for (int i : s) vis[i] = 0;
    return g.build(), g;
  }

  ull has(ull x, ull y) {
    if (not dir and x > y) swap(x, y);
    return x * N + y;
  }

  hashmap<int> mp;
  int get_eid(ull x, ull y) {
    if (mp.size() == 0) {
      mp.build(N - 1);
      for (Z &&e : es) {
        ull x = e.f, y = e.to;
        ull k = has(x, y);
        mp[k] = e.id;
      }
    }
    return mp.get(has(x, y), -1);
  }

  graph rev() const requires(dir) {
    graph ng(N);
    for (Z &&[f, t, w, id] : es) ng.add(t, f, w, id);
    return ng;
  }
};
#line 4 "YRS/tr/hld.hpp"

// lca  https://judge.yosupo.jp/problem/lca
// jump https://judge.yosupo.jp/problem/jump_on_tree

TE struct hld {
  using G = graph<T, 0>;
  G &g;
  int N, t = 0;
  vc<int> L, R, hd, V, fa, to, d;

  hld(G &g, int r = 0)
      : g(g), N(g.N), L(N, -1), R(L), hd(N, r), V(L), fa(L), to(L), d(N) {
    if (r == -1) return;
    assert(g.isok());
    dfs(r, -1);
    hl(r, r);
  }

  void dfs(int n, int f) {
    fa[n] = f;
    R[n] = 1;
    int l = g.in[n], r = g.in[n + 1], m = 0;
    Z &c = g.c;
    if (r - l > 1 and c[l].to == f) swap(c[l], c[l + 1]);
    FOR(i, l, r) if (c[i].to != f) {
      Z e = c[i];
      to[e.to] = e.id;
      d[e.to] = d[n] + 1;
      dfs(e.to, n);
      R[n] += R[e.to];
      if (chmax(m, R[e.to]) and l < i) swap(c[l], c[i]);
    }
  }

  void hl(int n, int p) {
    R[n] += L[n] = t;
    V[t++] = n;
    bool f = 1;
    for (Z &&e : g[n]) if (e.to != p) {
      hd[e.to] = f ? hd[n] : e.to;
      f = 0;
      hl(e.to, n);
    }
  }

  // return 从节点 n 出发的重链路径
  vc<int> hp(int n) {
    vc<int> s{n};
    while (1) {
      int x = hc(s.back());
      if (x == -1 or hd[x] != n) return s;
      s.ep(x);
    }
  }
  // heavy ch or -1
  inline int hc(int x) {
    int i = L[x] + 1;
    if (i == N) return -1;
    int a = V[i];
    return fa[a] == x ? a : -1;
  }

  // eid -> 边下面的节点
  int ev(int i) {
    Z &e = g.es[i];
    return (fa[e.f] == e.to ? e.f : e.to);
  }
  // 节点 x 对应的边的 ID
  int ve(int x) { return to[x]; }

  // eid of x-y
  int gei(int x, int y) {
    if (fa[x] != y) swap(x, y);
    assert(fa[x] == y);
    return to[x];
  }

  int el(int i) { return 2 * L[i] - d[i]; }
  int er(int i) { return 2 * R[i] - d[i] - 1; }

  // 向上跳k个节点
  int la(int n, int k) {
    assert(k <= d[n]);
    while (1) {
      int x = hd[n];
      if (L[n] - k >= L[x]) return V[L[n] - k];
      k -= L[n] - L[x] + 1;
      n = fa[x];
    }
  }

  int lca(int x, int y) {
    for (;; y = fa[hd[y]]) {
      if (L[x] > L[y]) swap(x, y);
      if (hd[x] == hd[y]) return x;
    }
  }

  int dist(int a, int b) { return d[a] + d[b] - 2 * d[lca(a, b)]; }

  int meet(int a, int b, int c) { return lca(a, b) ^ lca(a, c) ^ lca(b, c); }

  // L is in R
  bool ins(int x, int y) { return L[y] <= L[x] and L[x] < R[y]; }

  // x -> y : k step
  int jump(int x, int y, int k) {
    if (k == 1) {
      if (x == y) return -1;
      return ins(y, x) ? la(y, d[y] - d[x] - 1) : fa[x];
    }
    int c = lca(x, y);
    int a = d[x] - d[c];
    int b = d[y] - d[c];
    if (k > a + b) return -1;
    if (k <= a) return la(x, k);
    return la(y, a + b - k);
  }

  int size(int x, int r = -1) {
    if (r == -1) return R[x] - L[x];
    if (x == r) return N;
    int y = jump(x, r, 1);
    if (ins(x, y)) return R[x] - L[x];
    return N - R[y] + L[y];
  }

  vc<int> size_arr(int r = -1) {
    vc<int> sz(N);
    FOR(i, N) sz[i] = size(i, r);
    return sz;
  }

  vc<int> cc(int n) { // get ch
    vc<int> s;
    for (Z &&e : g[n]) if (e.to != fa[n]) s.ep(e.to);
    return s;
  }

  vc<int> cl(int n) { // get light
    vc<int> s;
    bool f = 1;
    for (Z &&e : g[n]) {
      if (e.to != fa[n]) {
        if (not f) s.ep(e.to);
        f = 0;
      }
    }
    return s;
  }

  // [始点, 終点] "閉"区間列。
  vc<PII> dec(int x, int y, bool e) {
    vc<PII> a, b;
    while (1) {
      if (hd[x] == hd[y]) break;
      if (L[x] < L[y]) {
        b.ep(L[hd[y]], L[y]);
        y = fa[hd[y]];
      } else {
        a.ep(L[x], L[hd[x]]);
        x = fa[hd[x]];
      }
    }
    if (L[x] < L[y]) b.ep(L[x] + e, L[y]);
    else if (L[y] + e <= L[x]) a.ep(L[x], L[y] + e);
    reverse(b);
    a.insert(a.end(), all(b));
    return a;
  }

  // path of x-y
  vc<int> rest_path(int x, int y) {
    vc<int> s;
    for (Z [a, b] : dec(x, y, 0)) {
      if (a <= b) FOR(i, a, b + 1) s.ep(V[i]);
      else FOR_R(i, b, a + 1) s.ep(V[i]);
    }
    return s;
  }

  // 计算两条路径的交点。如果两条路径没有交点，返回 {-1, -1}
  // https://codeforces.com/contest/500/problem/G
  PII cross(int a, int b, int c, int d) {
    int ab = lca(a, b), ac = lca(a, c), ad = lca(a, d);
    int bc = lca(b, c), bd = lca(b, d), cd = lca(c, d);
    int x = ab ^ ac ^ bc, y = ab ^ ad ^ bd;

    if (x != y) return {x, y};
    int z = ac ^ ad ^ cd;
    if (x != z) x = -1;
    return {x, x};
  }

  // 满足 f(x) 的最后一个x, 无返回-1
  int max_path(Z f, int x, int y) {
    if (not f(x)) return -1;
    Z pd = dec(x, y, 0);
    for (Z [a, b] : pd) {
      if (not f(V[a])) return x;
      if (f(V[b])) {
        x = V[b];
        continue;
      }
      int c = bina<0>([&](int c) -> bool { return f(V[c]); }, a, b);
      return V[c];
    }
    return x;
  }
};
#line 4 "YRS/tr/stt.hpp"

TE struct stt {
  hld<T> &t;
  int N;
  vc<int> L, R, fa, A, B;  // A, B (top-down)
  vc<u8> vis;

  stt(hld<T> &t) : t(t), N(t.N), L(N, -1), R(L), fa(L), A(L), B(L), vis(N) {
    FOR(i, N) A[i] = t.fa[i], B[i] = i;
    dfs(t.V[0]);
  }

  int f(int l, int r, int a, int b, bool c) { // newnode
    int x = len(fa);
    fa.ep(-1), L.ep(l), R.ep(r);
    A.ep(a), B.ep(b), vis.ep(c);
    return fa[l] = fa[r] = x;
  }

  // height, node idx
  PII dfs(int n) {
    Z pa = t.hp(n);
    vc<PII> s;
    s.ep(0, pa[0]);

    Z mer = [&]() -> void {
      Z [a, b] = pop(s);
      Z [c, d] = pop(s);
      s.ep(max(c, a) + 1, f(d, b, A[d], B[b], 1));
    };

    FOR(i, 1, len(pa)) {
      min_heap<PII> q;
      int k = pa[i];
      q.eb(0, k);
      for (int c : t.cl(pa[i - 1])) q.eb(dfs(c));
      while (len(q) > 1) {
        Z [a, b] = pop(q);
        Z [c, d] = pop(q);
        if (d == k) swap(b, d);
        int e = f(b, d, A[b], B[b], 0);
        if (k == b) k = e;
        q.eb(max(a, c) + 1, e);
      }
      s.ep(pop(q));

      while (1) {
        int n = len(s);
        if (n >= 3 and (s[n - 3].fi == s[n - 2].fi or
                        s[n - 3].fi <= s[n - 1].fi)) {
          Z [a, b] = pop(s);
          mer();
          s.ep(a, b);
        } else if (n > 1 and s[n - 2].fi <= s[n - 1].fi) {
          mer();
        } else {
          break;
        }
      }
    }
    while (len(s) > 1) mer();
    return s[0];
  }
};
#line 4 "YRS/tr/near_kinbo.hpp"

// 不要拷贝(
struct near_kinbo {
  struct re {
    int N, M, *a;
    void f(int n, int m) {
      N = n, M = m;
      a = new int[N * M];
      fill(a, a + N * M, -1);
    }
    ~re() { delete[] a; }
    int* operator[](int i) { return a + i * M; }
  };

  int N, tt;
  vc<PII> t;
  re dis;
  vc<int> fa, lr;
  u8 *dp;

  TE near_kinbo(graph<T, 0> &g) : N(g.N), t(N + N - 1), lr(N) {
    hld v(g);
    stt s(v);
    dp = new u8[N + N];
    fill(dp, dp + N + N, 0);
    FOR_R(i, N, N + N - 1) dp[s.L[i]] = dp[s.R[i]] = dp[i] + 1;

    int D = QMAX(dp, dp + N + N - 1);
    dis.f(N, D);

    vc<int> q(N + D + 5);
    int l = 0, r = 0;
    vc<u8> vis(N);
    tt = N;

    Z f = [&](Z &f, int n) -> vc<int> {
      if (n < N) {
        vis[n] = 1;
        return {n};
      }
      int d = dp[n], ls = s.L[n], rs = s.R[n], ps = l;
      bool ok = 0;

      vc<int> lc = f(f, ls);
      l = ps;

      if (s.A[ls] == s.A[rs]) {
        for (int x : lc) dis[x][d] = 1, q[r++] = x;
        ok = 1;
      } else {
        dis[s.B[ls]][d] = 0, q[r++] = s.B[ls];
      }

      while (l < r) {
        int x = q[l++];
        if (x < 0) {
          x = ~x;
          for (int y : lc)
            if (x != y) {
              dis[y][d] = dis[x][d] + 2, q[r++] = y;
            }
          continue;
        }
        vis[x] = 0;
        for (Z &&e : g[x]) {
          if (vis[e.to] and dis[e.to][d] == -1) {
            dis[e.to][d] = dis[x][d] + 1, q[r++] = e.to;
          } else if (not ok and e.to == s.A[ls]) {
            ok = 1, q[r++] = ~x;
          }
        }
      }

      int le = q[r - 1] < 0 ? dis[q[r - 2]][d] : dis[q[r - 1]][d];
      t[ls] = {tt, le + 1};
      tt += le + 1;

      int pr = l;
      vc<int> rc = f(f, rs);

      for (int x : rc) dis[x][d] = 1, q[r++] = x;
      while (l < r) {
        int x = q[l++];
        for (Z &&e : g[x]) {
          if (vis[e.to] and dis[e.to][d] == -1)
            dis[e.to][d] = dis[x][d] + 1, q[r++] = e.to;
        }
      }

      t[rs] = {tt, dis[q[r - 1]][d] + 1};
      tt += dis[q[r - 1]][d] + 1;

      while (pr > ps) {
        int nq = q[--pr];
        if (nq > -1) vis[nq] = 1;
      }

      if (s.A[ls] == s.A[rs]) {
        if (len(lc) < len(rc)) swap(lc, rc);
        lc.insert(lc.end(), all(rc));
      }
      l = r = ps;
      return lc;
    };

    f(f, N + N - 2);
    fa = move(s.fa);
    FOR(i, N - 1) lr[i] = s.L[i + N] ^ s.R[i + N];
  }
  ~near_kinbo() { delete[] dp; }

  vc<int> vs(int n) {
    static vc<int> s;
    s.clear();
    s.ep(n);
    int d = dp[n] - 1, x = n;
    while (d > -1) s.ep(t[n].fi + dis[x][d]), n = fa[n], --d;
    return s;
  }

  vc<PII> range(int n, int l, int r) {
    static vc<PII> s;
    s.clear();
    if (l >= r) return s;
    if (l == 0) s.ep(n, n + 1);
    int x = n, f = fa[n];
    while (f != -1) {
      int a = lr[f - N] ^ n, d = dp[f], of = dis[x][d];
      int nl = clamp(l - of, 0, t[a].se);
      int nr = clamp(r - of, 0, t[a].se);
      if (nl != nr) s.ep(t[a].fi + nl, t[a].fi + nr);
      n = f, f = fa[f];
    }
    return s;
  }

  int size() { return tt; }
};
#line 7 "Vertex_Add_Range_Contour_Sum_on_Tree.cpp"

template <uint B = 1 << 6>
struct pars {
  int N;
  ll *a;

  pars(const vc<ll> &s) : N(len(s)) {
    a = new ll[N << 1];
    copy(all(s), a + N);
    fill(a, a + N, 0);
    for (uint i = (N << 1) - 1; i >= 1; --i) a[i / B] += a[i];
  }

  ~pars() { delete[] a; }

  void add(uint i, ll x) {
    a[i += N] += x;
    while (i /= B) a[i] += x;
  }

  ll prod(uint l, uint r) {
    l += N, r += N;
    ll s = 0;
    while (l / B != r / B) {
      while (l & (B - 1)) s += a[l++];
      while (r & (B - 1)) s += a[--r];
      l /= B, r /= B;
    }
    for (uint i = l; i < r; ++i) s += a[i];
    return s;
  }
};
void Yorisou() {
  INT(N, Q);
  VEC(int, a, N);
  graph g(N);
  g.sc<0, 0>();
  near_kinbo ds(g);
  vc<ll> dat(ds.tt);
  FOR(i, N) for (int x : ds.vs(i)) dat[x] += a[i];
  pars bit(dat);
  FOR(Q) {
    INT(op);
    if (op == 0) {
      INT(x, y);
      for (int i : ds.vs(x)) bit.add(i, y);
    } else {
      INT(x, l, r);
      ll s = 0;
      for (Z [l, r] : ds.range(x, l, r)) s += bit.prod(l, r);
      print(s);
    }
  }
}
constexpr int tests = 0, fl = 0, DB = 10;
#line 1 "YRS/aa/main.hpp"
int main() {
  cin.tie(0)->sync_with_stdio(0);
  int T = 1;
  if (fl) cerr.tie(0);
  if (tests and not fl) IN(T);
  for (int i = 0; i < T or fl; ++i) {
    Yorisou();
    if (fl and i % DB == 0) cerr << "Case: " << i << '\n';
  }
  return 0;
}
#line 63 "Vertex_Add_Range_Contour_Sum_on_Tree.cpp"
