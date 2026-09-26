#line 1 "Convex_Layers.cpp"
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
#line 5 "Convex_Layers.cpp"
// #include "YRS/IO/fast_io.hpp"
// #include "YRS/random/rng.hpp"
// #include "YRS/ds/basic/retsu.hpp"
// #include "YRS/mod/mint.hpp"
// #include "YRS/aa/def.hpp"
#line 2 "YRS/ge/ds/convex_layer.hpp"

#line 2 "YRS/ge/ds/dynamic_hull_del.hpp"

#line 2 "YRS/ge/basic/point.hpp"

TE struct point {
  using P = point;
  T x, y;
  point(T x = 0, T y = 0) : x(x), y(y) {}

  P &operator+=(P p) { return x += p.x, y += p.y, *this; }
  P &operator-=(P p) { return x -= p.x, y -= p.y, *this; }
  P &operator*=(T w) { return x *= w, y *= w, *this; }
  P &operator/=(T w) { return x /= w, y /= w, *this; }

  P operator+(P p) const { return P(*this) += p; }
  P operator-(P p) const { return P(*this) -= p; }
  P operator*(T p) const { return P(*this) *= p; }
  P operator/(T p) const { return P(*this) /= p; }

  bool operator<(P p) const { return x != p.x ? x < p.x : y < p.y; }
  bool operator>(P p) const { return x != p.x ? x > p.x : y > p.y; }
  bool operator==(P p) const { return x == p.x and y == p.y; }
  bool operator!=(P p) const { return x != p.x or y != p.y; }

  P operator-() const { return P{-x, -y}; }

  T dot(P p) const { return x * p.x + y * p.y; }
  T det(P p) const { return x * p.y - y * p.x; }

  T square() const { return x * x + y * y; }

  ld length() const { return sqrtl(square()); }

  // [0, 2pi) 与 x轴 的夹角
  ld angle() const {
    ld ang = atan2(y, x);
    return ang < 0 ? ang + pi * 2 : ang;
  }

  static ld angle(P A, P B, P C) { // ∠ABC 只求劣角
    ld ang = atan2((A - B).det(C - B), (A - B).dot(C - B));
    return abs(ang);
  }

  // 弧度
  P rotate(double ang) const { // float 逆时针
    ld C = cos(ang), S = sin(ang);
    return P{C * x - S * y, S * x + C * y};
  }
  P rol90(bool ccw = true) const { return ccw ? P{-y, x} : P{y, -x}; }

  P nor() const { return (*this) / length(); }

  P div(P p) const {
    T d = p.square();
    return P{(x * p.x + y * p.y) / d, (y * p.x - x * p.y) / d};
  }
};

TE istream &operator>>(istream &I, point<T> &p) {
  return I >> p.x >> p.y;
}
TE ostream &operator<<(ostream &I, const point<T> &p) {
  return I << p.x << ' ' << p.y;
}

// A -> B -> C 逆时针为 +1，顺时针为 -1。
TE int ccw(point<T> a, point<T> b, point<T> c) {
  T x = (b - a).det(c - a);
  return x > 0 ? 1 : x < 0 ? -1 : 0;
}

#ifdef FIO
TE void rd(point<T> &p) {
  T x, y;
  IN(x, y);
  p = {x, y};
}
TE void wt(point<T> x) {
  wt(x.x);
  wt(' ');
  wt(x.y);
}
#endif
#line 4 "YRS/ge/ds/dynamic_hull_del.hpp"

TE struct dynamic_hull_up_del {
  using P = point<T>;
  struct lk {
    P p;
    lk *l, *r;
    int i;
  };
  using lp = lk*;
  struct node {
    lp c, b, t;
    int l, r;
  };

  int N, sz;
  vc<node> se;
  vc<u8> vis;
  vc<lk> lks;

  int ccw(const P &a, const P &b) {
    T x = a.det(b);
    return (x > 0) - (x < 0);
  }
  int ccw(const P &a, const P &b, const P &c) { return ccw(b - a, c - a); }

  pair<lp, lp> find_bridge(lp l, lp r) {
    while (l->r or r->r) {
      if (not r->r or (l->r and ccw(l->r->p - l->p, r->r->p - r->p) <= 0)) {
        if (ccw(l->p, l->r->p, r->p) <= 0) l = l->r;
        else break;
      } else {
        if (ccw(l->p, r->p, r->r->p) > 0) r = r->r;
        else break;
      }
    }
    return {l, r};
  }

  pair<lp, lp> find_bridge_rev(lp l, lp r) {
    while (r->l or l->l) {
      if (not l->l or (r->l and ccw(r->l->p - r->p, l->l->p - l->p) >= 0)) {
        if (ccw(r->p, r->l->p, l->p) >= 0) r = r->l;
        else break;
      } else {
        if (ccw(r->p, l->p, l->l->p) < 0) l = l->l;
        else break;
      }
    }
    return {r, l};
  }

  template <bool rev>
  void fx(int n, lp l, lp r) {
    if (rev) tie(r, l) = find_bridge_rev(l, r);
    else tie(l, r) = find_bridge(l, r);
    node &ls = se[se[n].l], &rs = se[se[n].r];

    se[n].t = l;
    se[n].c = ls.c;
    se[n].b = rs.b;
    ls.c = l->r;
    rs.b = r->l;
    
    if (l->r) l->r->l = {};
    else ls.b = {};
    if (r->l) r->l->r = {};
    else rs.c = {};

    l->r = r;
    r->l = l;
  }

  void rb(int x, int y) {
    se[x].c = se[y].c;
    se[x].b = se[y].b;
    se[y].c = {};
    se[y].b = {};
  }

  void del(int n, int a, int b, int i) {
    if (i < a or i >= b or n == -1) return;
    int m = (a + b) >> 1;
    int p = i < m ? se[n].l : se[n].r;
    if (not se[n].t) {
      se[p].c = se[n].c;
      se[p].b = se[n].b;
      if (i < m) del(p, a, m, i);
      else del(p, m, b, i);
      return rb(n, p);
    }
    lp l = se[n].t, r = l->r;
    node &ls = se[se[n].l], &rs = se[se[n].r];

    l->r = ls.c;
    if (ls.c) ls.c->l = l;
    else ls.b = l;
    ls.c = se[n].c;

    r->l = rs.b;
    if (rs.b) rs.b->r = r;
    else rs.c = r;
    rs.b = se[n].b;

    if (se[p].c == se[p].b and se[p].c->i == i) {
      se[n].c = se[n].b = {};
      rb(n, i < m ? se[n].r : se[n].l);
      se[n].t = {};
    } else if (i < m) {
      if (l->i == i) l = l->r;
      del(p, a, m, i);
      if (not l) l = ls.b;
      fx<1>(n, l, r);
    } else {
      if (r->i == i) r = r->l;
      del(p, m, b, i);
      if (not r) r = rs.c;
      fx<0>(n, l, r);
    }
  }

  dynamic_hull_up_del(const vc<P> &a) : N(len(a)), sz(N), se(N << 1), vis(N) {
    lks.reserve(N);
    NULL;
    FOR(i, N) lks.ep(a[i], nullptr, nullptr, i);
    FOR(i, N) se[i + N] = {&lks[i], &lks[i], nullptr, -1, -1};
    if (len(a) == 1) se[0] = se[1];
    int t = 0;
    Z g = [&](Z &g, int l, int r) -> int {
      if (r - l == 1) return l + N;
      int m = (l + r) >> 1, s = t++;
      se[s].l = g(g, l, m);
      se[s].r = g(g, m, r);
      fx<0>(s, se[se[s].l].c, se[se[s].r].c);
      return s;
    };
    g(g, 0, N);
  }

  int size() const { return sz; }

  bool empty() const { return sz == 0; }

  bool del(int k) {
    assert(0 <= k and k < N);
    if (vis[k]) return 0;
    vis[k] = 1;
    --sz;
    if (se[0].c == se[0].b) se[0].c = se[0].b = {};
    else del(0, 0, N, k);
    return 1;
  }

  vc<int> get_hull() const {
    vc<int> s;
    for (lp i = se[0].c; i; i = i->r) s.ep(i->i);
    return s;
  }
};
#line 4 "YRS/ge/ds/convex_layer.hpp"

TE vc<int> convex_layers(const vc<point<T>> &a) {
  using P = point<T>;
  int N = len(a);
  vc<int> I = argsort(a);
  vc<P> us = rearrange(a, I), ds(rbegin(us), rend(us));
  for (Z &[x, y] : ds) x = -x, y = -y;
  dynamic_hull_up_del<T> up(us), lo(ds);

  vc<int> ch,  s(N, -1);
  ch.reserve(N);
  for (int i = 0; not up.empty(); ++i) {
    for (int k : up.get_hull()) {
      s[I[k]] = i;
      ch.ep(k);
    }
    for (int k : lo.get_hull()) {
      k = N - k - 1;
      if (s[I[k]] == -1) {
        s[I[k]] = i;
        ch.ep(k);
      }
    }
    for (int k : ch) up.del(k), lo.del(N - k - 1);
    ch.clear();
  }
  return s;
}
#line 11 "Convex_Layers.cpp"

using P = point<ll>;
void Yorisou() {
  INT(N);
  VEC(P, a, N);
  for (int i : convex_layers(a)) print(i + 1);
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
#line 20 "Convex_Layers.cpp"
