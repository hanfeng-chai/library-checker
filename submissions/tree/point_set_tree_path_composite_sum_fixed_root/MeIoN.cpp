#line 1 "Point_Set_Tree_Path_Composite_Sum_Fixed_Root.cpp"
#define YRSD
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
template <typename ...S> istream &operator>>(istream &I, tuple<S...> &t) {
  return apply([&I](Z &...args) { ((I >> args), ...); }, t), I;
}
template <typename T, typename U>
istream &operator>>(istream &I, pair<T, U> &x) {
  return I >> x.fi >> x.se;
}
template <typename T, typename U>
ostream &operator<<(ostream &O, const pair<T, U> &x) {
  return O << x.fi << ' ' << x.se;
}
template <typename T>
requires requires(T &c) { begin(c); end(c); } and 
                          (not is_same_v<decay_t<T>, string>)
istream &operator>>(istream &I, T &c) {
  for (Z &e : c) I >> e;
  return I;
}
template <typename T>requires requires(const T &c) { begin(c); end(c); } and 
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
#define LL(...)   ll     __VA_ARGS__; IN(__VA_ARGS__)
#define ULL(...)  ull    __VA_ARGS__; IN(__VA_ARGS__)
#define I128(...) i128   __VA_ARGS__; IN(__VA_ARGS__)
#define STR(...)  string __VA_ARGS__; IN(__VA_ARGS__)
#define CH(...)   char   __VA_ARGS__; IN(__VA_ARGS__)
#define REAL(...) re     __VA_ARGS__; IN(__VA_ARGS__)
#define VEC(T, a, n) vc<T> a(n); IN(a)
#define VVEC(T, a, n, m) vvc<T> a(n, vc<T>(m)); IN(a)

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
template <> constexpr i128 inf<i128> = i128(numeric_limits<ll>::max()) * 2'000'000'000'000'000'000;
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
constexpr bool chmax(Z &a, const Z &b) { return (a < b ? a = b, true : false); }
constexpr bool chmin(Z &a, const Z &b) { return (a > b ? a = b, true : false); }

template <typename T, typename U>
constexpr static pair<T, U> operator-(const pair<T, U> &p) {
  return pair<T, U>(-p.fi, -p.se);
}

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
template <int off = 1, typename T> 
vc<T> pre_sum(const vc<T> &a) {
  int N = len(a);
  vc<T> c(N + 1);
  FOR(i, N) c[i + 1] = c[i] + a[i];
  if constexpr (off == 0) c.erase(c.begin());
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
TE constexpr T bmod(T x, T y) { return x - floor(x, y) * x; }
TE constexpr pair<T, T> divmod(T x, T y) {
  T q = floor(x, y);
  return pair{q, x - q * y};
}
template <typename T = ll>
T SUM(const Z &v) {
  return accumulate(all(v), T(0));
}
int lb(const Z &a, Z x) { return lower_bound(all(a), x) - a.begin(); }
int ub(const Z &a, Z x) { return upper_bound(all(a), x) - a.begin(); }

template <bool ck = true>
ll bina(Z F, ll l, ll r) {
  if constexpr (ck) assert(F(l));
  while (abs(l - r) > 1) {
    ll x = (r + l) >> 1;
    (F(x) ? l : r) = x;
  }
  return l;
}
TE T bina_real(const Z &F, T l, T r, int c = 100) {
  while (c--) {
    T m = (l + r) / 2;
    (F(m) ? l : r) = m;
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

TE inline void sh(vc<T> &a, int N) {
  a.resize(N, T(0));
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
#line 5 "Point_Set_Tree_Path_Composite_Sum_Fixed_Root.cpp"
// #include "YRS/random/rng.hpp"
#line 2 "YRS/tr/ddp.hpp"

#line 2 "YRS/tr/stt.hpp"

#line 2 "YRS/tr/hld.hpp"

#line 2 "YRS/g/Basic.hpp"

#line 2 "YRS/ds/basic/hashmap.hpp"

template <typename T>
struct hash_map {

  hash_map(uint N = 0) { build(N); }

  void build(uint N) {
    uint k = 8;
    while (k < (N << 1)) k <<= 1;
    ls = k >> 1, msk = k - 1;
    ke.resize(k), val.resize(k), vis.assign(k, 0);
  }

  void clear() {
    fill(all(vis), 0);
    ls = (msk + 1) >> 1;
  }

  int size() const { return vis.size() / 2 - ls; }

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

  T get(ull k, T fail) const {
    int i = id(k);
    return (vis[i] ? val[i] : fail);
  }

  bool contains(ull k) const {
    int i = id(k);
    return vis[i] and ke[i] == k;
  }

  // f(ke, val);
  template <typename F>
  void enumerate_all(F f) const {
    const int N = len(vis);
    FOR(i, N) if (vis[i]) f(ke[i], val[i]);
  }

 private:
  uint ls, msk;
  vc<ull> ke;
  vc<T> val;
  vc<u8> vis;

  ull hash(ull x) const {
    static const ull FIXED_RANDOM =
        std::chrono::steady_clock::now().time_since_epoch().count();
    x += FIXED_RANDOM;
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
    Z e = ee({f, t, w, i});
    es.ep(e);
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
      if (not dir) c[cc[e.to]++] = ee({e.to, e.f, e.w, e.id});
    }
  }

  template <bool wt = 0, int of = 1>
  void read_tree() { sc<wt, of>(N - 1); }
  template <bool wt = 0, int of = 1>
  void read_graph(int M) { sc<wt, of>(M); }
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

  hash_map<int> mp;
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

  graph reverse_graph() const {
    static_assert(dir);
    graph ng(N);
    for (Z &&[f, t, w, id] : es) ng.add(t, f, w, id);
    return ng;
  }
};
#line 4 "YRS/tr/hld.hpp"

// https://www.luogu.com.cn/problem/P3379 lca
// https://www.luogu.com.cn/problem/P6374 lca

TE struct hld {
  using G = graph<T, 0>;
  G &g;
  int N, t = 0;
  vc<int> L, R, hd, V, fa, to, d;

  hld(G &g, int r = 0)
      : g(g), N(g.N), L(N, -1), R(L), hd(N, r), V(L), fa(L), to(L), d(L) {
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
      if (x == y) return - 1;
      return ins(y, x) ? la(y, d[y] - d[x] - 1) : fa[x];
    }
    int c = lca(x, y);
    int a = d[x] - d[c];
    int b = d[y] - d[c];
    if (k > a + b) return - 1;
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
  const vc<PII> &dec(int x, int y, bool e) {
    static vc<PII> a, b;
    a.clear(), b.clear();
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
#line 4 "YRS/tr/ddp.hpp"

// 注意rake的时候如果存在某些和bot相关的信息，可能需要从R的top穿过整个L来更新信息
template <typename T, typename DP>
struct ddp_t {
  using X = DP::X;
  stt<T> g;
  int N;
  vc<X> dp;

  inline void upd(int i) {
    X &a = dp[g.L[i]], &b = dp[g.R[i]];
    dp[i] = g.vis[i] ? DP::com(a, b) : DP::rak(a, b);
  }

  ddp_t(hld<T> &g, Z f) : g(g), N(g.N), dp(2 * N - 1) {
    FOR(i, N) dp[i] = f(i);
    FOR(i, N, 2 * N - 1) upd(i);
  }

  void set(int i, X x) {
    dp[i] = x;
    for (i = g.fa[i]; i != -1; i = g.fa[i]) upd(i);
  }

  X prod() { return dp.back(); }
};
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

#define CC C<mint>
#define fac fact<mint>
#define ifac fact_inv<mint>
#define set_comb Set_comb<mint>
#define set_inv Set_inv<mint>
#line 4 "YRS/mod/mint.hpp"

#define C constexpr
template <int mod>
struct mint_t {
  using mint = mint_t;
  static C uint m = mod;
  uint x;

  C uint val() const { return x; }

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
#line 8 "Point_Set_Tree_Path_Composite_Sum_Fixed_Root.cpp"

#define tests 0
#define fl 0
#define DB 10
using mint = M99;
struct DP {
  using X = struct {
    mint a, b, cnt, ans;
  };
  static X rak(X &L, X &R) { return {L.a, L.b, L.cnt + R.cnt, L.ans + R.ans}; }
  static X com(X &L, X &R) {
    mint a = L.a, b = L.b;
    mint c = R.a, d = R.b;
    mint aa = a * c, bb = a * d + b;
    mint cnt = L.cnt + R.cnt;
    mint ans = L.ans + a * R.ans + b * R.cnt;
    return {aa, bb, cnt, ans};
  }
};
void Yorisou() {
  INT(N, Q);
  VEC(mint, a, N);
  vc<pair<mint, mint>> dat(N - 1);
  graph g(N);
  g.es.reserve(N - 1);
  FOR(i, N - 1) {
    INT(x, y);
    g.add(x, y);
    IN(dat[i]);
  }
  g.build();
  hld v(g);
  Z sing = [&](int i) -> DP::X {
    if (i == 0) { return {1, 0, 1, a[i]}; }
    int e = v.ve(i);
    return {dat[e].fi, dat[e].se, 1, dat[e].fi * a[i] + dat[e].se};
  };
  ddp_t<int, DP> dp(v, sing);
  FOR(Q) {
    INT(t);
    if (t == 0) {
      INT(i, x);
      a[i] = x;
      dp.set(i, sing(i));
    } else {
      INT(i, b, c);
      dat[i].fi = b, dat[i].se = c;
      int x = v.ev(i);
      dp.set(x, sing(x));
    }
    print(dp.prod().ans);
  }
}
#line 1 "YRS/aa/main.hpp"
int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int T = 1;
  if (fl) cerr.tie(0);
  if (tests and not fl) IN(T);
  for (int i = 0; i < T or fl; ++i) {
    Yorisou();
    if (fl and i % DB == 0) cerr << "Case: " << i << '\n';
  }
  return 0;
}
#line 62 "Point_Set_Tree_Path_Composite_Sum_Fixed_Root.cpp"
