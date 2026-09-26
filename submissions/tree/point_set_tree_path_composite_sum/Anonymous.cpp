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
#define TES TP <TN T, TN ...S>
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
TP<> constexpr bool can_in<istream> = 1;
TP<> constexpr bool can_out<ostream> = 1;

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

#define c constexpr
template <int mod>
struct mint_t {
  using T = mint_t;
  static c uint m = mod;
  uint x;

  c inline uint val() const { return x; }

  c mint_t() : x(0) {}
  TE requires(is_unsigned_v<T>) c mint_t(T x) : x(x % m) {}
  c mint_t(u128 x) : x(x % m) {}
  TE requires(is_signed_v<T>) c mint_t(T x) : x((x %= mod) < 0 ? x + mod : x) {}
  c mint_t(i128 x) : x((x %= mod) < 0 ? x + mod : x) {}

  c T &operator+=(T p) {
    if ((x += p.x) >= m) x -= m;
    return *this;
  }
  c T &operator-=(T p) {
    if ((x += m - p.x) >= m) x -= m;
    return *this;
  }
  c T operator+(T p) const { return T(*this) += p; }
  c T operator-(T p) const { return T(*this) -= p; }

  c T &operator*=(T p) {
    x = ull(x) * p.x % m;
    return *this;
  }
  c T operator*(T p) const { return T(*this) *= p; }

  c T &operator/=(T p) { return *this *= p.inv(); }
  c T operator/(T p) const { return T(*this) /= p; }

  c T operator-() const { return T::gen(x ? mod - x : 0); }

  c T inv() const {
    int a = x, b = mod, x = 1, y = 0;
    while (b > 0) {
      int t = a / b;
      swap(a -= t * b, b);
      swap(x -= t * y, y);
    }
    return T(x);
  }

  c T pow(ll k) const {
    if (k < 0) return inv().pow(-k);
    T s(1), a(x);
    for (; k; k >>= 1, a *= a) {
      if (k & 1) s *= a;
    }
    return s;
  }

  c bool operator<(T p) const { return x < p.x; }
  c bool operator==(T p) const { return x == p.x; }
  c bool operator!=(T p) const { return x != p.x; }

  static c T gen(uint x) {
    T s;
    s.x = x;
    return s;
  }

  template <typename I> requires(can_in<I>)
  friend I &operator>>(I &in, T &p) {
    ll t;
    in >> t;
    p = t;
    return in;
  }

  template <typename U> requires(can_out<U>)
  friend U &operator<<(U &cout, const T &p) { return cout << p.x; }

  static c int get_mod() { return mod; }

  static c PII ntt_info() {
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

  static c bool can_ntt() { return ntt_info().fi != -1; }
};
#undef c

using M99 = mint_t<998244353>;
using M17 = mint_t<1000000007>;
using M11 = M17;

TE struct hashmap {
  uint ls, ms;
  vc<ull> a;
  vc<T> b;
  vc<u8> vis;

  ull hash(ull x) const {
    static const ull bs =
        chrono::steady_clock::now().time_since_epoch().count();
    x += bs;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return (x ^ (x >> 31)) & ms;
  }

  void extend() {
    vc<pair<ull, T>> s;
    int N = si(vis);
    s.reserve(N / 2 - ls);
    FOR(i, N) if (vis[i]) s.ep(a[i], b[i]);
    build(si(s) << 1);
    for (var [l, r] : s) (*this)[l] = r;
  }

  hashmap(uint N = 0) { build(N); }

  void build(uint N) {
    uint k = 8;
    while (k < (N << 1)) k <<= 1;
    ls = k >> 1, ms = k - 1;
    a.resize(k);
    b.resize(k);
    vis.assign(k, 0);
  }

  void clear() {
    fill(all(vis), 0);
    ls = (ms + 1) >> 1;
  }

  ll size() const { return vis.size() / 2 - ls; }

  int id(ull k) const {
    int i = hash(k);
    while (vis[i] and a[i] != k) i = (i + 1) & ms;
    return i;
  }

  T &operator[](ull k) {
    if (ls == 0) extend();
    int i = id(k);
    if (not vis[i]) {
      vis[i] = 1;
      a[i] = k;
      b[i] = T();
      --ls;
    }
    return b[i];
  }

  T get(ull k, T fl) const {
    int i = id(k);
    return (vis[i] ? b[i] : fl);
  }

  bool contains(ull k) const {
    int i = id(k);
    return vis[i] and a[i] == k;
  }

  vc<pair<ull, T>> get_all() const {
    int N = si(vis);
    vc<pair<ull, T>> s;
    FOR(i, N) if (vis[i]) s.ep(a[i], b[i]);
    return s;
  }
};

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
    vc<int> a(N);
    for (Z &&e : es) ++a[e.f], ++a[e.to];
    return a;
  }

  pair<vc<int>, vc<int>> deg_inout() {
    vc<int> a(N), b(N);
    for (Z &&e : es) ++a[e.to], ++b[e.f];
    return {a, b};
  }

  vc<int> ni;
  vc<u8> vis;

  graph<T, dir> rearrange(const vc<int> &v, bool keep_eid = 0) {
    if (si(ni) != N) ni.assign(N, -1);
    int N = si(v);
    FOR(i, N) ni[v[i]] = i;
    graph<T, dir> g(N);
    vc<int> s;
    FOR(i, N) {
      for (Z &&e : (*this)[v[i]]) {
        if (si(vis) <= e.id) vis.resize(e.id + 1);
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

  vc<int> hp(int n) {
    vc<int> s{n};
    while (1) {
      int x = hc(s.back());
      if (x == -1 or hd[x] != n) return s;
      s.ep(x);
    }
  }

  inline int hc(int x) {
    int i = L[x] + 1;
    if (i == N) return -1;
    int a = V[i];
    return fa[a] == x ? a : -1;
  }

  int ev(int i) {
    Z &e = g.es[i];
    return (fa[e.f] == e.to ? e.f : e.to);
  }

  int ve(int x) { return to[x]; }

  int gei(int x, int y) {
    if (fa[x] != y) swap(x, y);
    assert(fa[x] == y);
    return to[x];
  }

  int el(int i) { return 2 * L[i] - d[i]; }
  int er(int i) { return 2 * R[i] - d[i] - 1; }

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

  bool ins(int x, int y) { return L[y] <= L[x] and L[x] < R[y]; }

  int jump(int x, int y, int k) {
    if (k == 1) {
      if (x == y) return -1;
      return ins(y, x) ? la(y, d[y] - d[x] - 1) : fa[x];
    }
    int c = lca(x, y), a = d[x] - d[c], b = d[y] - d[c];
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

  vc<int> cc(int n) {
    static vc<int> s;
    s.clear();
    for (Z &&e : g[n]) if (e.to != fa[n]) s.ep(e.to);
    return s;
  }

  vc<int> cl(int n) {
    static vc<int> s;
    s.clear();
    bool f = 1;
    for (Z &&e : g[n]) {
      if (e.to != fa[n]) {
        if (not f) s.ep(e.to);
        f = 0;
      }
    }
    return s;
  }

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

  vc<int> rest_path(int x, int y) {
    vc<int> s;
    for (Z [a, b] : dec(x, y, 0)) {
      if (a <= b) FOR(i, a, b + 1) s.ep(V[i]);
      else FOR_R(i, b, a + 1) s.ep(V[i]);
    }
    return s;
  }

  PII cross(int a, int b, int c, int d) {
    int ab = lca(a, b), ac = lca(a, c), ad = lca(a, d);
    int bc = lca(b, c), bd = lca(b, d), cd = lca(c, d);
    int x = ab ^ ac ^ bc, y = ab ^ ad ^ bd;

    if (x != y) return {x, y};
    int z = ac ^ ad ^ cd;
    if (x != z) x = -1;
    return {x, x};
  }

  int max_path(Z f, int x, int y) {
    if (not f(x)) return -1;
    for (Z [a, b] : dec(x, y, 0)) {
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

TE struct stt {
  hld<T> &t;
  int N;
  vc<int> L, R, fa, A, B;
  vc<u8> vis;

  stt(hld<T> &t) : t(t), N(t.N), L(N, -1), R(L), fa(L), A(L), B(L), vis(N) {
    FOR(i, N) A[i] = t.fa[i], B[i] = i;
    dfs(t.V[0]);
  }

  int f(int l, int r, int a, int b, bool c) {
    int x = si(fa);
    fa.ep(-1), L.ep(l), R.ep(r);
    A.ep(a), B.ep(b), vis.ep(c);
    return fa[l] = fa[r] = x;
  }

  PII dfs(int n) {
    Z pa = t.hp(n);
    vc<PII> s;
    s.ep(0, pa[0]);

    Z mer = [&]() -> void {
      Z [a, b] = pop(s);
      Z [c, d] = pop(s);
      s.ep(max(c, a) + 1, f(d, b, A[d], B[b], 1));
    };

    FOR(i, 1, si(pa)) {
      min_heap<PII> q;
      int k = pa[i];
      q.eb(0, k);
      for (int c : t.cl(pa[i - 1])) q.eb(dfs(c));
      while (si(q) > 1) {
        Z [a, b] = pop(q);
        Z [c, d] = pop(q);
        if (d == k) swap(b, d);
        int e = f(b, d, A[b], B[b], 0);
        if (k == b) k = e;
        q.eb(max(a, c) + 1, e);
      }
      s.ep(pop(q));

      while (1) {
        int n = si(s);
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
    while (si(s) > 1) mer();
    return s[0];
  }
};

TE struct DP_mode {
  struct X {

    int op, s, t;
    T a, b, c, ans;
  };

  static X rak(var L, var R) {
    X res = {0, L.s, L.t, {}, {}, {}, {}};
    res.a = L.a, res.b = L.b;
    res.c = L.c + R.c, res.ans = L.ans + R.ans;
    return res;
  }

  static X rak1(var L, var R) {
    X res = {1, L.s, L.t, {}, {}, {}, {}};
    res.a = L.a, res.b = L.b;
    res.c = L.c + R.c, res.ans = L.ans + R.ans;
    return res;
  }

  static X com(var L, var R) {
    X res = {0, L.s, R.t, {}, {}, {}, {}};
    res.a = L.a * R.a, res.b = L.a * R.b + L.b;
    res.c = L.c + R.c, res.ans = L.ans + (L.a * R.ans + L.b * R.c);
    return res;
  }

  static X com1(var L, var R) {
    X res = {1, L.s, R.t, {}, {}, {}, {}};
    res.a = L.a * R.a, res.b = L.a * R.b + L.b;
    res.c = L.c + R.c, res.ans = L.ans + (L.a * R.ans + L.b * R.c);
    return res;
  }

  static X com2(var L, var R) {
    X res = {1, L.s, L.t, {}, {}, {}, {}};
    res.a = L.a, res.b = L.b;
    res.c = L.c + R.c, res.ans = L.ans + (L.a * R.ans + L.b * R.c);
    return res;
  }
};

template <typename T, typename DP>
struct dynamic_tree_dp_re {
  using X = DP::X;
  using XX = pair<X, X>;
  stt<T> g;
  int N;
  vc<XX> dp;

  inline void upd(int i) {
    var [l, ll] = dp[g.L[i]];
    var [r, rr] = dp[g.R[i]];
    if (g.vis[i]) dp[i] = {DP::com(l, r), DP::com1(rr, ll)};
    else dp[i] = {DP::rak(l, r), DP::com2(ll, r)};
  }

  dynamic_tree_dp_re(hld<T> &g, Z f) : g(g), N(g.N), dp(2 * N - 1) {
    FOR(i, N) dp[i] = f(i);
    FOR(i, N, 2 * N - 1) upd(i);
  }

  void set(int i, XX x) {
    dp[i] = x;
    for (i = g.fa[i]; i != -1; i = g.fa[i]) upd(i);
  }

  X prod(int i) {
    X a = dp[i].se, b{}, c{};
    b.op = c.op = -1;
    while (1) {
      int f = g.fa[i];
      if (f == -1) break;
      int l = g.L[f], r = g.R[f];
      if (g.vis[f]) {
        if (l == i) b = b.op == -1 ? dp[r].fi : DP::com(b, dp[r].fi);
        else a = a.op == -1 ? dp[l].se : DP::com1(a, dp[l].se);
      } else {
        if (g.L[f] == i) {
          if (a.op == -1) b = b.op == -1 ? dp[r].fi : DP::rak(b, dp[r].fi);
          else a = DP::com2(a, dp[r].fi);
        } else {
          if (a.op == -1) {
            c = DP::com2(c, b);
            a.op = -1;
            b = dp[l].fi;
          } else {
            a = b.op == -1 ? a : DP::rak1(a, b);
            c = c.op == -1 ? a : DP::com1(c, a);
            a.op = -1;
            b = dp[l].fi;
          }
        }
      }
      i = f;
    }
    a = b.op == -1 ? a : DP::rak1(a, b);
    return c.op == -1 ? a : DP::com1(c, a);
  }
};

using mint = M99;
struct DP {
  struct X {
    int op;
    mint a, b, cnt, sum;
  };

  static X none() {
    X x{};
    x.op = -1;
    return x;
  }

  static X rak(const X &L, const X &R) {
    if (L.op == -1) return R;
    if (R.op == -1) return L;
    X x{};
    x.op = 0;
    x.a = L.a;
    x.b = L.b;
    x.cnt = L.cnt + R.cnt;
    x.sum = L.sum + R.sum;
    return x;
  }

  static X rak1(const X &L, const X &R) {
    X x = rak(L, R);
    x.op = 1;
    return x;
  }

  static X com(const X &L, const X &R) {
    if (L.op == -1) return R;
    if (R.op == -1) return L;
    X x{};
    x.op = 0;
    x.a = L.a * R.a;
    x.b = L.a * R.b + L.b;
    x.cnt = L.cnt + R.cnt;
    x.sum = L.sum + (L.a * R.sum + L.b * R.cnt);
    return x;
  }

  static X com1(const X &L, const X &R) {
    X x = com(L, R);
    x.op = 1;
    return x;
  }

  static X com2(const X &L, const X &R) {
    if (L.op == -1) return R;
    if (R.op == -1) return L;
    X x{};
    x.op = 1;
    x.a = L.a;
    x.b = L.b;
    x.cnt = L.cnt + R.cnt;
    x.sum = L.sum + (L.a * R.sum + L.b * R.cnt);
    return x;
  }
};
using X = DP::X;
void Yorisou() {
  INT(N, Q);
  VEC(mint, a, N);
  vc<PII> es(N - 1);
  vc<mint> b(N - 1), c(N - 1);
  graph g(N);
  FOR(i, N - 1) {
    IN(es[i], b[i], c[i]);
    g.add(es[i].fi, es[i].se);
  }
  g.build();
  hld v(g);
  vc<int> ch(N - 1),fe(N, -1);
  FOR(i, N - 1) {
    var [x, y] = es[i];
    if (v.d[x] > v.d[y]) ch[i] = x, fe[x] = i;
    else ch[i] = y, fe[y] = i;
  }

  Z make = [&](int i) -> pair<X, X> {
    X dn{}, up{};
    dn.cnt = up.cnt = 1;
    if (fe[i] == -1) {
      dn.a = up.a = 1;
      dn.b = up.b = 0;
      dn.sum = up.sum = a[i];
    } else {
      int id = fe[i];
      dn.a = up.a = b[id];
      dn.b = up.b = c[id];
      dn.sum = b[id] * a[i] + c[id];
      up.sum = a[i];
    }
    return {dn, up};
  };

  dynamic_tree_dp_re<int, DP> dp(v, make);
  FOR(Q) {
    INT(op);
    if (op == 0) {
      INT(i, x);
      a[i] = x;
      dp.set(i, make(i));
    } else {
      INT(i, d, e);
      b[i] = d, c[i] = e;
      dp.set(ch[i], make(ch[i]));
    }
    INT(i);
    print(dp.prod(i).sum);
  }
}

int main() {
  Yorisou();
  return 0;
}
